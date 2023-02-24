"""Goals: show how to agglomerate several consecutive times the same mesh.

Create a mesh with the default constructor of `dualGPy`, apply several levels of agglomeration with `CoMMA` and export the result.
"""
from CoMMA import *
import meshio
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D, Mesh3D
# import dualGPy.Utils as ut
import numpy as np
from comma_tools import build_coarse_graph

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
##
anisotropic = False
## Points per edge of the quad/hex
n = 21
## CoMMA parameters
isFirstAgglomeration = True
if dimension == 3:
    minCard, goalCard, maxCard = 8, 8, 8
else:
    minCard, goalCard, maxCard = 4, 4, 4
correction = False
threshold_anisotropy = 4.
odd_line_length = True
neigh_type = 0 # 0 = Extended (standard), 1 = Pure front advancing
seed_order = 0 # 0 = Boundary priority, 1 = Neighbourhood priority,
               # 10 = Boundary priority with point initialization
               # 11 = Neighbourhood priority with point initialization
# Number of iterations for iterative fine-cell research algorithm
fc_iter = 1
agglomeration_levels = 3

# Output-related parameters
# If < 1, the value associated to the coarse cells are the ID. Otherwise, only
# numbers from 1 to the given value are used (it makes it easier to distinguish the
# coarse cells in Paraview
renumber_coarse = 10 # -1 #
#########

assert(agglomeration_levels > 0)

print('Requested arguments:')
print( ' [Input]')
print(f' * {dimension=}')
print(f' * {anisotropic=}')
print(f' * {n=}')
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
print(f' * {agglomeration_levels=}')
print( ' [Output]')
renum = renumber_coarse > 1
print(f' * Coarse cell renumbering={renum}' + (f" (from 0 to {renumber_coarse-1})" if renum else ""))
print()

# CoMMATypes-like
CoMMAIndex  = np.uint # Unsigned long
CoMMAInt    = int
CoMMAWeight = np.double

print("Creating mesh and dual graph...", flush = True, end = '')
outname = 'out_' + \
            ('an' if anisotropic else '') + \
            'iso_' + \
            ('quad_2D' if dimension == 2 else 'hexa_3D') + \
            ('_corrected' if correction else '') + \
            f'_{agglomeration_levels}agglolvl'+ \
            '.vtu'

mesh = Mesh2D(n, anisotropic) if dimension == 2 \
       else Mesh3D(n, anisotropic)
mesh.get_boundary_faces()
mesh.ComputeGeometry()
graph = Graph2D(mesh)
graph.get_CSR()
mesh.boundary_detection()
print('OK')
nb_fc = len(graph.vertex)-1
adjMatrix_row_ptr= np.array(graph.vertex, dtype = CoMMAIndex)
adjMatrix_col_ind= np.array(graph.edges, dtype = CoMMAIndex)

adjMatrix_areaValues = np.array(mesh.area, dtype = CoMMAWeight)
volumes = np.array(mesh.volume, dtype = CoMMAWeight)
centers = mesh.centers.copy()
weights = np.arange(start = nb_fc-1, stop = 0, step = -1, dtype = CoMMAWeight)
isOnBnd = np.array(mesh.boundary_cells, dtype = CoMMAIndex)
fc_to_cc = np.empty(nb_fc, dtype = CoMMAIndex)
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc, dtype = CoMMAIndex)
agglomerationLines_Idx = np.array([0], dtype = CoMMAIndex)
agglomerationLines = np.array([0], dtype = CoMMAIndex)
agglo = []
fc_to_cur_lvl = list(range(nb_fc))
for level in range(agglomeration_levels):
    print(f"* Level {level}:")
    if level > 0:
        print(' - Preparing data...', flush = True, end = '')
        # This is possibly not the most efficient way to do the following, but it works and it is enough for now
        f2c = np.asarray(fc_to_cc)
        (
            adjMatrix_row_ptr,
            adjMatrix_col_ind,
            adjMatrix_areaValues,
            volumes,
            isOnBnd,
            centers
        ) = build_coarse_graph(
            fc_to_cc,
            adjMatrix_row_ptr,
            adjMatrix_col_ind,
            adjMatrix_areaValues,
            volumes,
            isOnBnd,
            centers)
        nb_cc = isOnBnd.shape[0]
        weights = np.arange(start = nb_cc-1, stop = 0, step = -1, dtype = CoMMAWeight)
        fc_to_cc = np.empty(nb_cc, dtype = CoMMAIndex)
        arrayOfFineAnisotropicCompliantCells = np.arange(nb_cc, dtype = CoMMAIndex)
        print('OK')

    print(" - CoMMA call...", flush = True, end = '')
    fc_to_cc,agglomerationLines_Idx,agglomerationLines = \
            agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
                                  centers, weights,
                                  arrayOfFineAnisotropicCompliantCells,isOnBnd, level == 0,
                                  anisotropic, odd_line_length, threshold_anisotropy, seed_order,
                                  fc_to_cc,agglomerationLines_Idx,agglomerationLines,
                                  correction, dimension,goalCard,minCard,maxCard, fc_iter, neigh_type)
    print('OK')

    # Update direction from original fine cell to current level coarse cell
    for i in range(len(fc_to_cur_lvl)):
        fc_to_cur_lvl[i] = fc_to_cc[fc_to_cur_lvl[i]]

    print(" - Finalizing...", flush = True, end = '')
    # agglo.append([ut.address_agglomerated_cells(fc_to_cur_lvl, renumber_coarse)] if renum \
                          # else [fc_to_cur_lvl])
    # As long as the data is composed of (integer) IDs, the following is equivalent but much faster
    agglo.append([(np.asarray(fc_to_cur_lvl) % renumber_coarse)] if renum \
                      else [fc_to_cur_lvl])
    print('OK')
    print()
    if max(fc_to_cc) == 0 and level < agglomeration_levels-1:
        # Only one coarse cell that spans the whole mesh, we should quit now
        print("ATTENTION: Only one coarse cell. Quitting here")
        print()
        break

out_data = { f'agglomerate_{i}': data for i, data in enumerate(agglo)}
print(f"Writing in {outname}")
meshio.Mesh(
    mesh.mesh.points,
    mesh.mesh.cells,
    cell_data = out_data,
).write(outname)
