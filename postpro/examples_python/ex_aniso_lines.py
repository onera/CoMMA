"""Goals: show how to draw with `dualGPy` anisotropic lines obtained by `CoMMA`.

This script creates a small handmade 2D mesh with a block of anisotropic cells
at the bottom, then agglomerates it and draws the mesh including the computed
anisotropic lines using the related features of `dualGPy`.
"""
from CoMMA import *
import meshio
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D, Mesh3D
import dualGPy.Utils as ut
import numpy as np
import meshio as mio

neigh_type_types = [
        'Extended',
        'Pure front advancing'
        ]

seed_ordering_types = {
        0:  'Boundary priority',
        1:  'Neighbourhood priority',
        10: 'Boundary priority with point initialization',
        11: 'Neighbourhood priority with point initialization'
        }

# USER PARAMETERS
#################
# Input-related parameters
dimension = 2
anisotropic = True
## CoMMA parameters
isFirstAgglomeration = True
minCard, goalCard, maxCard = 4, 4, 4
correction = False
threshold_anisotropy = 1.5
odd_line_length = True
neigh_type = 0 # 0 = Extended (standard), 1 = Pure front advancing
seed_order = 0 # 0  = Boundary priority, 1 = Neighbourhood priority,
               # 10 = Boundary priority with point initialization
               # 11 = Neighbourhood priority with point initialization
# Number of iterations for iterative fine-cell research algorithm
fc_iter = 1

# Output-related parameters
# If < 1, the value associated to the coarse cells are the ID. Otherwise, only
# numbers from 1 to the given value are used (it makes it easier to distinguish the
# coarse cells in Paraview
renumber_coarse = 10 # -1 #
#########

basename = "ani_3cell_test"
outname = basename + '.vtu'

print('Requested arguments:')
print( ' [Input]')
print(f' * {anisotropic=}')
print(f' * {isFirstAgglomeration=}')
print(f' * {minCard=}')
print(f' * {goalCard=}')
print(f' * {maxCard=}')
print(f' * {correction=}')
print(f' * {threshold_anisotropy=}')
print(f' * {odd_line_length=}')
print(f' * neigh_type={neigh_type_types[neigh_type]}')
print(f' * seed_ordering={seed_ordering_types[seed_order]}')
print(f' * Fine-cell research iterations={fc_iter}')
print( ' [Output]')
renum = renumber_coarse > 1
print(f' * Coarse cell renumbering={renum}' + (f" (from 0 to {renumber_coarse-1})" if renum else ""))
print()

# CoMMATypes-like
CoMMAIndex  = np.uint # Unsigned long
CoMMAInt    = int
CoMMAWeight = np.double

print("Creating mesh and dual graph...", flush = True, end = '')
ref_pts = np.zeros((7,2), dtype=float)
ref_pts[:-1,1] = range(6)
ref_pts[:-1,1] *= 0.2
ref_pts[-1,1]  = 2

points = np.empty((35,2), dtype=float)
to_add = np.array([[1.,0.]], dtype=float)
for i in range(5):
    points[7*i:7*(i+1),:] = ref_pts + i*to_add

# Cells are not ordered on purpose to try the scheme
# Order of the points in the cell from bottom left node in anti-clockwork order
base_cell = [0,7,8,1]
cells = np.array([base_cell for _ in range(24)], dtype = int)
# Bottom left nodes
btlxnd = [0,1,2,3,4,5,12,11,10,9,8,7,16,17,15,18,14,19,26,25,21,24,22,23]
# bottom left corner + base structure
assert(len(btlxnd) == cells.shape[0])
for i, nd in enumerate(btlxnd):
    cells[i,:] += nd
# print(cells)
mio_m = mio.Mesh(points = points, cells = {'quad': cells})
#mio_m.write('aniso_3cell.msh', file_format = "ansys")
m = Mesh2D(mio_m)
m.get_boundary_faces()
m.ComputeGeometry()
m.boundary_detection()

g = Graph2D(m)
g.get_CSR()
print('OK')

adjMatrix_row_ptr= np.array(g.vertex, dtype = CoMMAIndex)
adjMatrix_col_ind= np.array(g.edges, dtype = CoMMAIndex)
adjMatrix_areaValues = np.array(m.area,dtype = CoMMAWeight)
isOnBnd = np.array(m.boundary_cells,dtype = CoMMAInt)
volumes = np.array(m.volume, dtype = CoMMAWeight)
nb_fc = len(g.vertex)-1
weights = np.arange(start = nb_fc-1, stop = 0, step = -1, dtype = CoMMAWeight)
fc_to_cc = np.empty(nb_fc, dtype = CoMMAIndex)
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc, dtype = CoMMAIndex)
agglomerationLines_Idx = np.array([0], dtype = CoMMAIndex)
agglomerationLines = np.array([0], dtype = CoMMAIndex)

print("CoMMA call...", flush = True, end = '')
fc_to_cc_res,alines_Idx,alines = \
        agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
                              m.centers.astype(CoMMAWeight, copy = False), weights,
                              arrayOfFineAnisotropicCompliantCells,isOnBnd, isFirstAgglomeration,
                              anisotropic, odd_line_length, threshold_anisotropy, seed_order,
                              fc_to_cc,agglomerationLines_Idx,agglomerationLines,
                              correction, dimension,goalCard,minCard,maxCard, fc_iter, neigh_type)
print('OK')

print("Finalizing...", flush = True, end = '')
# fine_cells_renum = ut.address_agglomerated_cells(fc_to_cc_res, renumber_coarse) if renum \
        # else fc_to_res
# As long as the data is composed of (integer) IDs, the following is equivalent but much faster
fine_cells_renum = (np.asarray(fc_to_cc_res) % renumber_coarse) if renum \
        else fc_to_cc_res

agglo = []
if len(m.mesh.cells) > 1:
    # More than one element type -> Adapt cell_data to meshio cellblock format
    start = 0
    for cells_by_type in m.mesh.cells:
        n_cell = cells_by_type.data.shape[0]
        agglo.append( fine_cells_renum[start:start+n_cell] )
        start += n_cell
else:
    agglo = [fine_cells_renum]

f2c = np.asarray(fc_to_cc_res, dtype=int)
# Building a dictionary 'aniso line ID' : [fine cells in line]
# One-liner:
dic = { i: np.flatnonzero(
                np.isin(f2c, alines[alines_Idx[i]:alines_Idx[i+1]])) \
        for i in range(len(alines_Idx)-1)
}
# Second option, better explained
# dic = {}
# # For every line...
# for i in range(len(alines_Idx)-1):
    # mask = np.full(len(fc_to_cc_res), False)
    # # For every coarse cell in the line...
    # for cc in alines[alines_Idx[i]:alines_Idx[i+1]]:
        # # Take all the fine cells that are in the coarse cell
        # mask |= f2c == cc
    # dic[i] = np.flatnonzero(mask)

print('OK')
line_draw = "test_lines.png"
print(f"Drawing graph and lines in {line_draw}")
m.draw_aggl_lines(line_draw, dic, -0.02, 4.02, -0.02, 2.02, show_axes = False)

print(f"Writing in {outname}")
meshio.Mesh(
    m.mesh.points,
    m.mesh.cells,
    cell_data = {"agglomerate":agglo}
).write(outname)
