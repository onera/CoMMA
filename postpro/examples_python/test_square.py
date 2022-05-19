from CoMMA import *
import meshio
from dualGPy.CGNS_Adapter import CGNS_Adapter
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D
import dualGPy.Utils as ut
import pandas as pd
import numpy as np
def isNaN(num):
    for i in num:
        if i!= i:
           return True
    return False
points = []
cells_test = []
n=5
for i in range(n):
 for j in range(n):
  points.append([j,i**2])
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
Mesh1.get_boundary_faces()
Mesh1.ComputeGeometry()
#Mesh1.boundary_detection()
# graph
Graph1 = Graph2D(Mesh1)
Graph1.get_CSR()
Mesh1.boundary_detection_Easy()
nb_fc = len(Graph1.vertex)-1
adjMatrix_row_ptr= np.array(Graph1.vertex , dtype='long')
print("adjMatrix_row_ptr",adjMatrix_row_ptr)
adjMatrix_col_ind= np.array(Graph1.edges ,dtype='long')
print("adjMatrix_col_ind",adjMatrix_col_ind)
adjMatrix_areaValues=np.array(Mesh1.area,dtype='double')
print("adjMatrix_area",adjMatrix_areaValues)
volumes = np.array(Mesh1.volume,dtype='double')
print("volumes",volumes)
isOnBnd = np.array(Mesh1.boundary_cells,dtype='long')
print("isOnBnd",isOnBnd)
array_isOnRidge=np.array(Mesh1.onRidge,dtype='long')
print("array_isOnRidge",array_isOnRidge)
array_isOnValley=np.array(Mesh1.onValley, dtype='long')
print("array_isOnValley",array_isOnValley)
array_isOnCorner=np.array(Mesh1.onCorner, dtype='long')
print("array_isOnCorner",array_isOnCorner)
fc_to_cc = np.full(nb_fc, -1,dtype='long')
indCoarseCell = 0
minCard = 4
goalCard = 4
maxCard = 4
verbose = 0
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc,dtype='long')
agglomerationLines_Idx = np.array([0],dtype='long')
agglomerationLines = np.array([0],dtype='long')
isFirstAgglomeration = 1
isAnisotropic = 1
dimension = 2
is_basic_or_triconnected = 1

print("CoMMA call")
#isotropic
fc_to_cc_res1,agglomerationLines_Idx_res_iso,agglomerationLines_res_iso=agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,arrayOfFineAnisotropicCompliantCells,isOnBnd,array_isOnValley,array_isOnRidge,array_isOnCorner,isFirstAgglomeration,0,fc_to_cc,agglomerationLines_Idx,agglomerationLines,is_basic_or_triconnected,dimension,goalCard,minCard,maxCard)
#anisotropic
fc_to_cc_res2,agglomerationLines_Idx_res,agglomerationLines_res=agglomerate_one_level(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,arrayOfFineAnisotropicCompliantCells,isOnBnd,array_isOnValley,array_isOnRidge,array_isOnCorner,isFirstAgglomeration,1,fc_to_cc,agglomerationLines_Idx,agglomerationLines,is_basic_or_triconnected,dimension,goalCard,minCard,maxCard)
#print("anisotropic",len(agglomerationLines_Idx_res),"not anisotropic",len(agglomerationLines_Idx_res_iso))
#print("anisotropic",len(agglomerationLines_res),"not anisotropic",len(agglomerationLines_res_iso))
#print(agglomerationLines_Idx_res)

#for k, ind in enumerate(fc_to_cc_res2):
#  print("fc",k,"cc",ind)

d = dict(enumerate(fc_to_cc_res2))
# we invert the CoMMA result for verification by passing through a dict
cc_ind={}
for key, value in d.items(): 
 if value in cc_ind.keys():   
  cc_ind[value].append(key)
 else:
  cc_ind.update({value:[key]})
#for k, ind in enumerate(agglomerationLines_Idx_res):
#  print("idx+1",ind)
#for k, ind in enumerate(agglomerationLines_res):
#  print("agglLines+1",ind)



#Mesh1.draw_bnd("valley.png",Mesh1.onValley)
#Mesh1.draw_bnd("ridge.png",Mesh1.onRidge)
agglomeration_lines = {}
for j in range(len(agglomerationLines_Idx_res)-1):
 agglomeration_lines.update({j:[]})
 actual =agglomerationLines_res[agglomerationLines_Idx_res[j]:agglomerationLines_Idx_res[j+1]]
 print(actual)
 for k, ind in enumerate(actual):
     agglomeration_lines[j].extend(cc_ind[ind])
print(agglomeration_lines)
#agglomeration_lines.update({len(agglomerationLines_Idx_res):[]})
#agglomeration_lines[len(agglomerationLines_Idx_res)]=fc_to_cc_res2[agglomerationLines_res[agglomerationLines_Idx_res[-1]+1:-1]]
#Mesh1.draw_aggl_lines("aggl_lines_quad.png",agglomeration_lines,0.0,0.05,-0.05,0.05)
Mesh1.draw_aggl_lines_full("aggl_lines_full_quad.png",agglomeration_lines)
 
print("finalizing")
#fine_cells_iso = ut.address_agglomerated_cells(fc_to_cc_res1,10)
#fine_cells_aniso = ut.address_agglomerated_cells(fc_to_cc_res2,10)
fine_cells_iso = [value for j,value in enumerate(fc_to_cc_res1)]
fine_cells_aniso = [value for j,value in enumerate(fc_to_cc_res2)]
fine_cells = [j for j,value in enumerate(fc_to_cc_res2)]

meshOUT_iso = meshio.Mesh(
    points,
    cells,
    cell_data = {"agglomerate":[fine_cells_iso],"id": [fine_cells]},
)
meshOUT_aniso = meshio.Mesh(
    points,
    cells,
    cell_data = {"agglomerate":[fine_cells_aniso] ,"id": [fine_cells]},
)


print("writing")
meshOUT_iso.write("out_iso_quad_4.vtk")
meshOUT_aniso.write("out_aniso_quad_4.vtk")

