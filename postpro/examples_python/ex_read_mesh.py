"""Goals: show how to read an existing mesh with `meshio` and agglomerate it with
`CoMMA`; show how to treat meshes with more than one element-type

Read an existing mesh via `meshio`, agglomerate with `CoMMA` and export the result
Features: A special case of VTK output is used if hybrid mesh with multi-elements
(e.g. quads *and* tetras as in `raebis`)
"""
from CoMMA import *
import meshio
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D, Mesh3D
import dualGPy.Utils as ut
import numpy as np
import os.path

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
# Format is one of those accepted by meshio or None
# e.g., input_mesh, input_format = "3D_CRM.vtu", None
input_mesh, input_format = "../meshes/naca2d_structured_ansys.msh", "ansys"
##
dimension = 2
##
anisotropic = False
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

# Output-related parameters
# If < 1, the value associated to the coarse cells are the ID. Otherwise, only
# numbers from 1 to the given value are used (it makes it easier to distinguish the
# coarse cells in Paraview
renumber_coarse = 10 # -1 #
#########

basename = os.path.basename(input_mesh)
basename, ext = os.path.splitext(basename)
outname = basename + '.vtu'

print('Requested arguments:')
print( ' [Input]')
print(f' * {input_mesh=}')
print(f' * {dimension=}')
print(f' * {anisotropic=}')
print(f' * {isFirstAgglomeration=}')
print(f' * {minCard=}')
print(f' * {goalCard=}')
print(f' * {maxCard=}')
print(f' * {correction=}')
print(f' * {threshold_anisotropy=}')
print(f' * {odd_line_length=}')
print(f' * neigh_type={neigh_type_types[neigh_type]}')
print(f' * Priority weights: reversed ID')
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

print("Reading mesh and creating dual graph...", flush = True, end = '')
if not os.path.exists(input_mesh):
    raise OSError(f'Cannot find mesh', filename = input_mesh)
mmio = meshio.read(input_mesh, file_format = input_format)
mio_dim = mmio.points.shape[-1]
if dimension != mio_dim:
    raise ValueError(f'''You requested dimension {dimension}, but mesh is of dimension {mio_dim}. Please, change one of the two
Reminder: VTK mesh are read as 3D by meshio even if they are 2D. See dualGPy ex_dualize.py for more info''')

mesh = Mesh2D(mmio) if dimension == 2 \
        else Mesh3D(mmio)
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
weights = np.arange(start = nb_fc-1, stop = 0, step = -1, dtype = CoMMAWeight)
isOnBnd = np.array(mesh.boundary_cells, dtype = CoMMAInt)
fc_to_cc = np.empty(nb_fc, dtype = CoMMAIndex)
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc, dtype= CoMMAIndex)
agglomerationLines_Idx = np.array([0], dtype = CoMMAIndex)
agglomerationLines = np.array([0], dtype = CoMMAIndex)

print("CoMMA call...", flush = True, end = '')
fc_to_cc_res,agglomerationLines_Idx_res_iso,agglomerationLines_res_iso = \
        agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
                              mesh.centers.astype(CoMMAWeight, copy = False), weights,
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
if len(mesh.mesh.cells) > 1:
    # More than one element type -> Adapt cell_data to meshio cellblock format
    start = 0
    for cells_by_type in mesh.mesh.cells:
        n_cell = cells_by_type.data.shape[0]
        agglo.append( fine_cells_renum[start:start+n_cell] )
        start += n_cell
else:
    agglo = [fine_cells_renum]
print('OK')

print(f"Writing in {outname}")
meshio.Mesh(
    mesh.mesh.points,
    mesh.mesh.cells,
    cell_data = {"agglomerate":agglo}
).write(outname)
