from CoMMA import *
import meshio
from dualGPy.Mesh import Mesh2D
from dualGPy.Coarse_mesh import Coarse_mesh
from dualGPy.Graph import Graph2D
import dualGPy.Utils as ut
import numpy as np
import time


points = []
cells_test = []
n=5
for i in range(n):
 for j in range(n):
  points.append([j,i])
for k,element in enumerate(points):
  if (k+1)%n!=0 and (k < (len(points)-n)) :
   cells_test.append([k,k+1,n+k+1,n+k])
print(len(cells_test))
cells =[("quad",cells_test)]
mesh = meshio.Mesh(
    points,
    cells,
)
Mesh1 = Mesh2D(mesh)
print(Mesh1.cells)
Mesh1.get_boundary_faces()
Mesh1.ComputeGeometry()
Mesh1.boundary_detection()
Graph1 = Graph2D(Mesh1)
Graph1.get_CSR()
Graph1.draw_graph(Mesh1,"graph.png")
nb_fc = len(Graph1.vertex)-1
adjMatrix_row_ptr= np.array(Graph1.vertex , dtype='long')
adjMatrix_col_ind= np.array(Graph1.edges ,dtype='long')
print("row_ptr",adjMatrix_row_ptr)
print("col_ind",adjMatrix_col_ind) 
adjMatrix_areaValues=Mesh1.area
volumes = np.array(Mesh1.volume,dtype='double')
isOnBnd = np.array(Mesh1.boundary_cells,dtype='long')
array_isOnRidge=np.array(Mesh1.onRidge,dtype='long')
array_isOnValley=np.array(Mesh1.onValley, dtype='long')
array_isOnCorner=np.array(Mesh1.onCorner, dtype='long')
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
isAnisotropic = False
dimension = 2
corrections = False
threshold_anisotropy = 4.

start = time.time()
fc_to_cc_res,agglomerationLines_Idx_res,agglomerationLines_res=agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,arrayOfFineAnisotropicCompliantCells,isOnBnd,array_isOnValley,array_isOnRidge,array_isOnCorner,isFirstAgglomeration,isAnisotropic,threshold_anisotropy,fc_to_cc,agglomerationLines_Idx,agglomerationLines,corrections,dimension,goalCard,minCard,maxCard)
end = time.time()
fine_cells = []
for j in range(len(fc_to_cc_res)):
        fine_cells.append(np.float64(fc_to_cc_res[j]))


meshOUT = meshio.Mesh(
    points,
    cells,
    # Optionally provide extra data on points, cells, etc.
    # Each item in cell data must match the cells array
    cell_data = {"agglomerate":[fine_cells] },
)
meshOUT.write("stack.vtu")

print("time CoMMA",end - start)
