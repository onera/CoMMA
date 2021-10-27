from CoMMA import *
import meshio
from dualGPy.Mesh import Mesh2D
from dualGPy.Graph import Graph2D
import matplotlib.pyplot as plt
import numpy as np
import networkx as nx

points = [
    [0.0, 0.0], 
    [1.0, 0.0],
    [0.0, 1.0],
    [1.0, 1.0],
    [2.0, 0.0],
    [2.0, 1.0],
    [3.0, 0.0],
    [3.0, 1.0],
    [3.0, 2.0],
    [2.0, 2.0],
    [1.0, 2.0],
    [0.0, 2.0],
    [0.0, -1.0],
    [1.0, -1.0],
    [2.0, -1.0],
    [3.0, -1.0],
    [4.0, -1.0],
    [4.0,0.0],
    [4.0,1.0],
    [4.0,2.0],
    [4.0,3.0],
    [3.0,3.0],
    [2.0,3.0],
    [1.0,3.0],
    [0.0,3.0],
    [0.0,-2.0],
    [1.0,-2.0],
    [2.0,-2.0],
    [3.0,-2.0],
    [4.0,-2.0]
]
cells = [
    ("triangle", [[0, 1, 2], [1, 3, 2]]),
    ("quad", [[1, 4, 5, 3],[4,6,7,5],[5,7,8,9],[3,5,9,10],[3,10,11,2],[14,15,6,4],[13,14,4,1],[12,13,1,0],[15,16,17,6],[6,17,18,7],[7,18,19,8],[8,19,20,21],[9,8,21,22],[10,9,22,23],[11,10,23,24],[25,26,13,12],[26,27,14,13],[27,28,15,14],[28,29,16,15]]),
]

mesh = meshio.Mesh(
    points,
    cells,
    # Optionally provide extra data on points, cells, etc.
    # Each item in cell data must match the cells array
)

Mesh1 = Mesh2D(mesh)
Mesh1.get_boundary_faces()
Mesh1.ComputeVolume()
Mesh1.ComputeArea()
Mesh1.boundary_detection()
# graph
Graph1 = Graph2D(Mesh1)
Graph1.get_adj_matrix()
Graph1.adj_to_csr()
print(Graph1.adj)

nb_fc = len(Graph1.vertex)-1
adjMatrix_row_ptr= np.array(Graph1.vertex , dtype='long')
adjMatrix_col_ind= np.array(Graph1.edges ,dtype='long')
print("row_ptr/vertex:",adjMatrix_row_ptr)
print("column index/edges:", adjMatrix_col_ind)
adjMatrix_areaValues=np.array(Mesh1.area,dtype='double')
volumes = np.array(Mesh1.volume,dtype='double')
print("row ptr len:", len(adjMatrix_row_ptr))
print("col ind len:", len(adjMatrix_col_ind))
print("vol len:", len(volumes))
isOnBnd = np.array(Mesh1.boundary_cells,dtype='long')
print("bound_cells:", len(Mesh1.boundary_cells))
array_isOnRidge=np.array(Mesh1.onRidge,dtype='long')
array_isOnValley=np.array(Mesh1.onValley, dtype='long')
array_isOnCorner=np.array(Mesh1.onCorner, dtype='long')
fc_to_cc = np.full(nb_fc, -1,dtype='long')
print("onridge",len(array_isOnRidge))
print("onvalley",len(array_isOnValley))
print("oncorner",len(array_isOnCorner))
indCoarseCell = 0
minCard = -1
goalCard = -1
maxCard = -1
checks = 1
verbose = 0
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc,dtype='long')
agglomerationLines_Idx = np.zeros(nb_fc,dtype='long')
agglomerationLines = np.zeros(nb_fc,dtype='long')
isFirstAgglomeration = 1
isAnisotropic = 1
dimension = 2
is_basic_or_triconnected = 1


fc_to_cc_res,agglomerationLines_Idx_res,agglomerationLines_res=agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,arrayOfFineAnisotropicCompliantCells,isOnBnd,array_isOnValley,array_isOnRidge,array_isOnCorner,isFirstAgglomeration,isAnisotropic,fc_to_cc,agglomerationLines_Idx,agglomerationLines,is_basic_or_triconnected,dimension,goalCard,minCard,maxCard,checks,verbose)


fine_cells_triangle = []
fine_cells = []

for j in range(len(fc_to_cc_res)):
    if len(Mesh1.cells[j])==3:
        fine_cells_triangle.append(np.float64(fc_to_cc_res[j]))
    else:
        fine_cells.append(np.float64(fc_to_cc_res[j]))
print(len(fine_cells_triangle))
meshOUT = meshio.Mesh(
    points,
    cells,
    # Optionally provide extra data on points, cells, etc.
    # Each item in cell data must match the cells array
    cell_data = {"agglomerate":[fine_cells_triangle,fine_cells] },
)
meshOUT.write("out.vtk")

