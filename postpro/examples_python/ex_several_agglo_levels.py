"""Create a mesh with the default constructor of dualGPy, apply several levels of agglomeration with CoMMA and export the result.

Goal: show how to agglomerate several consecutive times the same mesh.
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
n = 21
## CoMMA parameters
isFirstAgglomeration = True
if dimension == 3:
    minCard, goalCard, maxCard = 8, 8, 8
else:
    minCard, goalCard, maxCard = 4, 4, 4
correction = False
threshold_anisotropy = .25
isotropic_agglo = 0 # 0 = Biconnected (standard), 1 = Pure front advancing
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
print(f' * isotropic_agglo={isotropic_agglo_types[isotropic_agglo]}')
print(f' * {agglomeration_levels=}')
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
centers = mesh.centers.copy()
isOnBnd = np.array(mesh.boundary_cells,dtype='long')
fc_to_cc = np.full(nb_fc, -1,dtype='long')
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc,dtype='long')
agglomerationLines_Idx = np.array([0],dtype='long')
agglomerationLines = np.array([0],dtype='long')
agglo = []
fc_to_cur_lvl = list(range(nb_fc))
for level in range(agglomeration_levels):
    print(f"* Level {level}:")
    if level > 0:
        print(' - Preparing data')
        # This is possibly not the most efficient way to do the following, but it works and it is enough for now
        f2c = np.asarray(fc_to_cc)
        nb_cc = f2c.max() + 1
        new_isOnBnd = np.empty(nb_cc, dtype='int')
        new_volumes = np.empty(nb_cc, dtype='double')
        new_centers = np.empty((nb_cc, centers.shape[-1]), dtype = 'double')
        new_adjMatrix_row_ptr = [0]
        new_adjMatrix_col_ind = []
        new_adjMatrix_areaValues = []
        # For every coarse cell...
        for cc in range(nb_cc):
            mask_fc = f2c == cc
            assert(mask_fc.any())
            # (Previous) fine cells in current coarse cells
            fcs = np.flatnonzero(mask_fc)
            new_isOnBnd[cc] = np.max(isOnBnd[mask_fc])
            new_volumes[cc] = np.sum(volumes[mask_fc])
            # This is not actually very good, but still...
            new_centers[cc,:] = centers[mask_fc,:].mean(axis = 0)
            neighs_cc = {}
            # for every (previous) fine cell composing the current coarse cell...
            for fc in fcs:
                # ...loop over its neighbours
                for neigh_fc in adjMatrix_col_ind[adjMatrix_row_ptr[fc]:adjMatrix_row_ptr[fc+1]]:
                    # ...and if they are not in the same coarse cell...
                    if neigh_fc not in fcs:
                        # ...update the interface with the coarse cell to which the neighbours belongs
                        neigh_cc = fc_to_cc[neigh_fc]
                        cur_area = neighs_cc.get(neigh_cc, 0.) # Use default value in order to avoid errors
                        neighs_cc[neigh_cc] = cur_area + adjMatrix_areaValues[neigh_fc]
            new_adjMatrix_col_ind.extend(neighs_cc.keys())
            new_adjMatrix_areaValues.extend(neighs_cc.values())
            new_adjMatrix_row_ptr.append(new_adjMatrix_row_ptr[-1] + len(neighs_cc))
        isOnBnd = new_isOnBnd
        volumes = new_volumes
        centers = new_centers
        adjMatrix_col_ind = new_adjMatrix_col_ind
        adjMatrix_row_ptr = new_adjMatrix_row_ptr
        adjMatrix_areaValues = new_adjMatrix_areaValues
        fc_to_cc = np.full(nb_cc, -1,dtype='long')
        arrayOfFineAnisotropicCompliantCells = np.arange(nb_cc,dtype='long')

    print(" - CoMMA call")
    fc_to_cc,agglomerationLines_Idx,agglomerationLines = \
            agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes, centers,
                                  arrayOfFineAnisotropicCompliantCells,isOnBnd, level == 0,
                                  anisotropic, threshold_anisotropy, isotropic_agglo,
                                  fc_to_cc,agglomerationLines_Idx,agglomerationLines,
                                  correction, dimension,goalCard,minCard,maxCard)

    # Update direction from original fine cell to current level coarse cell
    for i in range(len(fc_to_cur_lvl)):
        fc_to_cur_lvl[i] = fc_to_cc[fc_to_cur_lvl[i]]

    print(" - Finalizing")
    # agglo.append([ut.address_agglomerated_cells(fc_to_cur_lvl, renumber_coarse)] if renum \
                          # else [fc_to_cur_lvl])
    # As long as the data is composed of (integer) IDs, the following is equivalent but much faster
    agglo.append([(np.asarray(fc_to_cur_lvl) % renumber_coarse)] if renum \
                      else [fc_to_cur_lvl])
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
