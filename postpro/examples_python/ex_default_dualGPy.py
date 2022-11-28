"""Goals: show how to use `dualGPy` to create a Cartesian-like mesh and agglomerate it with `CoMMA`

Create a mesh with the default constructor of `dualGPy, agglomerate with `CoMMA` and export the result.
"""
from CoMMA import *
import meshio
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D, Mesh3D
import dualGPy.Utils as ut
import numpy as np

isotropic_agglo_types = [
        'Biconnected',
        'Pure front advancing'
        ]

# USER PARAMETERS
#################
# Input-related parameters
dimension = 2
##
anisotropic = False
## Points per edge of the quad/hex
n = 25
## CoMMA parameters
isFirstAgglomeration = True
if dimension == 3:
    minCard, goalCard, maxCard = 8, 8, 8
else:
    minCard, goalCard, maxCard = 4, 4, 4
correction = False
threshold_anisotropy = 4.
isotropic_agglo = 0 # 0 = Biconnected (standard), 1 = Pure front advancing

# Output-related parameters
# If < 1, the value associated to the coarse cells are the ID. Otherwise, only
# numbers from 1 to the given value are used (it makes it easier to distinguish the
# coarse cells in Paraview
renumber_coarse = 10 # -1 #
#########

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
print(f' * isotropic_agglo={isotropic_agglo_types[isotropic_agglo]}')
print( ' [Output]')
renum = renumber_coarse > 1
print(f' * Coarse cell renumbering={renum}' + (f" (from 0 to {renumber_coarse-1})" if renum else ""))
print()

outname = 'out_' + \
            ('an' if anisotropic else '') + \
            'iso_' + \
            ('quad_2D' if dimension == 2 else 'hexa_3D') + \
            ('_corrected' if correction else '') + \
            '.vtu'

mesh = Mesh2D(n, anisotropic) if dimension == 2 \
       else Mesh3D(n, anisotropic)
mesh.get_boundary_faces()
mesh.ComputeGeometry()
graph = Graph2D(mesh)
graph.get_CSR()
mesh.boundary_detection()
nb_fc = len(graph.vertex)-1
adjMatrix_row_ptr= np.array(graph.vertex , dtype='long')
adjMatrix_col_ind= np.array(graph.edges ,dtype='long')

adjMatrix_areaValues = np.array(mesh.area,dtype='double')
volumes = np.array(mesh.volume,dtype='double')
isOnBnd = np.array(mesh.boundary_cells,dtype='long')
fc_to_cc = np.full(nb_fc, -1,dtype='long')
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc,dtype='long')
agglomerationLines_Idx = np.array([0],dtype='long')
agglomerationLines = np.array([0],dtype='long')

print("CoMMA call")
fc_to_cc_res,agglomerationLines_Idx_res_iso,agglomerationLines_res_iso = \
        agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes, mesh.centers,
                              arrayOfFineAnisotropicCompliantCells,isOnBnd, isFirstAgglomeration,
                              anisotropic, threshold_anisotropy, isotropic_agglo,
                              fc_to_cc,agglomerationLines_Idx,agglomerationLines,
                              correction, dimension,goalCard,minCard,maxCard)

print("Finalizing")
# agglo = [ut.address_agglomerated_cells(fc_to_cc_res, renumber_coarse)] if renum \
                  # else [fc_to_cc_res]
# As long as the data is composed of (integer) IDs, the following is equivalent but much faster
agglo = [(np.asarray(fc_to_cc_res) % renumber_coarse)] if renum \
                  else [fc_to_cc_res]

print(f"Writing in {outname}")
meshio.Mesh(
    mesh.mesh.points,
    mesh.mesh.cells,
    cell_data = {"agglomerate":agglo},
).write(outname)
