from CoMMA import *
import meshio
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D
import dualGPy.Utils as ut
import numpy as np

Mesh1 = Mesh2D(5,False)
Mesh1.get_boundary_faces()
Mesh1.ComputeGeometry()
Graph1 = Graph2D(Mesh1)
Graph1.get_CSR()
Mesh1.boundary_detection()
nb_fc = len(Graph1.vertex)-1
adjMatrix_row_ptr= np.array(Graph1.vertex , dtype='long')
adjMatrix_col_ind= np.array(Graph1.edges ,dtype='long')
adjMatrix_areaValues=np.array(Mesh1.area,dtype='double')
volumes = np.array(Mesh1.volume,dtype='double')
isOnBnd = np.array(Mesh1.boundary_cells,dtype='long')
fc_to_cc = np.full(nb_fc, -1,dtype='long')
indCoarseCell = 0
minCard = 4
goalCard = 4
maxCard = 4
verbose = 0
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc,dtype='long')
agglomerationLines_Idx = np.array([0],dtype='long')
agglomerationLines = np.array([0],dtype='long')
isFirstAgglomeration = True
isAnisotropic = True
dimension = 2
corrections = True
threshold_anisotropy = 4.
iso_agglo_type = 1

print("CoMMA call")
#isotropic
fc_to_cc_res1,agglomerationLines_Idx_res_iso,agglomerationLines_res_iso=agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,arrayOfFineAnisotropicCompliantCells,isOnBnd,isFirstAgglomeration,False,threshold_anisotropy,iso_agglo_type,fc_to_cc,agglomerationLines_Idx,agglomerationLines,corrections,dimension,goalCard,minCard,maxCard)

print("end CoMMA")
fine_cells_iso = [value for j,value in enumerate(fc_to_cc_res1)]

meshOUT_iso = meshio.Mesh(
    Mesh1.mesh.points,
    Mesh1.mesh.cells,
    cell_data = {"agglomerate":[fine_cells_iso]},
)

print("writing")
meshOUT_iso.write("out_iso_quad_4.vtk")

