"""Read an existing mesh via meshio, agglomerate with CoMMA and export the result
Features: A special case of VTK output is used if hybrid mesh with multi-elements
(e.g. quads *and* tetras as in raebis)
"""
from CoMMA import *
import meshio
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D, Mesh3D
import dualGPy.Utils as ut
import numpy as np
import os.path

isotropic_agglo_types = [
        'Biconnected',
        'Pure front advancing'
        ]

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
threshold_anisotropy = .25
isotropic_agglo = 0 # 0 = Biconnected (standard), 1 = Pure front advancing

# Output-related parameters
# If < 1, the value associated to the coarse cells are the ID. Otherwise, only
# numbers from 1 to the given value are used (it makes it easier to distinguish the
# coarse cells in Paraview
renumber_coarse = 10 # -1 #
#########

if not os.path.exists(input_mesh):
    raise OSError(f'Cannot find mesh', filename = input_mesh)
mmio = meshio.read(input_mesh, file_format = input_format)
mio_dim = mmio.points.shape[-1]
if dimension != mio_dim:
    raise ValueError(f'''You requested dimension {dimension}, but mesh is of dimension {mio_dim}. Please, change one of the two
Reminder: VTK mesh are read as 3D by meshio even if they are 2D. See dualGPy ex_dualize.py for more info''')

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
print(f' * isotropic_agglo={isotropic_agglo_types[isotropic_agglo]}')
print( ' [Output]')
renum = renumber_coarse > 1
print(f' * Coarse cell renumbering={renum}' + (f" (from 0 to {renumber_coarse-1})" if renum else ""))
print()

mesh = Mesh2D(mmio) if dimension == 2 \
        else Mesh3D(mmio)
mesh.get_boundary_faces()
mesh.ComputeGeometry()
graph = Graph2D(mesh)
graph.get_CSR()
mesh.boundary_detection()
nb_fc = len(graph.vertex)-1
adjMatrix_row_ptr= np.array(graph.vertex, dtype = 'long')
adjMatrix_col_ind= np.array(graph.edges, dtype = 'long')

adjMatrix_areaValues = np.array(mesh.area, dtype = 'double')
volumes = np.array(mesh.volume, dtype = 'double')
isOnBnd = np.array(mesh.boundary_cells, dtype = 'long')
fc_to_cc = np.full(nb_fc, -1, dtype = 'long')
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc, dtype= 'long')
agglomerationLines_Idx = np.array([0], dtype = 'long')
agglomerationLines = np.array([0], dtype = 'long')

print("CoMMA call")
fc_to_cc_res,agglomerationLines_Idx_res_iso,agglomerationLines_res_iso = \
        agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes, mesh.centers,
                              arrayOfFineAnisotropicCompliantCells,isOnBnd, isFirstAgglomeration,
                              anisotropic, threshold_anisotropy, isotropic_agglo,
                              fc_to_cc,agglomerationLines_Idx,agglomerationLines,
                              correction, dimension,goalCard,minCard,maxCard)

print("Finalizing")
agglo = []
if renum:
    fine_cells_renum = ut.address_agglomerated_cells(fc_to_cc_res, renumber_coarse)
    # Adapt cell_data to meshio cellblock format
    agglo = []
    start = 0
    for cells_by_type in mesh.mesh.cells:
        n_cell = cells_by_type.data.shape[0]
        agglo.append( fine_cells_renum[start:start+n_cell] )
        start += n_cell
else:
    agglo = [fc_to_cc_res]

print(f"Writing in {outname}")
meshio.Mesh(
    mesh.mesh.points,
    mesh.mesh.cells,
    cell_data = {"agglomerate":agglo}
).write(outname)
