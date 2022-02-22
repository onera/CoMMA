import CGNS.MAP as CGM
import CGNS.PAT.cgnsclass as CGC
import CGNS.PAT.cgnskeywords as CGK
import CGNS.PAT.cgnsutils as CGU
import CGNS.PAT.cgnslib as CGL
import numpy as np

class CGNS_Adapter:
 """ Class that adapts the CGNS file to 
 be processed with DualGPy
 """ 
 def __init__(self, name):
  self.name = name
 
 @staticmethod
 def get_NGon_Node(n_zone):
  """ Method that retrives the NGon method """
  # Search the element types in the childs of the zone
  # in order to retrive the node relative to the ngon elements
  for n_elts in n_zone.nextChild(sidstype=CGK.Elements_ts):
      # NGON NODE
   if n_elts.data[0] == CGK.NGON_n:
    return n_elts


 def get_ngon_a_from_zone_node(self,n_zone):
  """ Retrive the element range and the connectivity ARRAY of the current
        zone. remember that it is composed as follows:
        elementConnectivityArray = [num_el_face0, coord[0].... coord[num_el], num_el_face1, coord[0].....coord[num_el]]"""
  nGonNode = self.get_NGon_Node(n_zone).node
  if not nGonNode:
        raise ValueError("No NGon Node in the tree")
  elementConnectivityArray = CGU.hasChildName(nGonNode, CGK.ElementConnectivity_s)
  elemRange = CGU.hasChildName(nGonNode, CGK.ElementRange_s)
  ParentElem = CGU.hasChildName(nGonNode, CGK.ParentElements_s)
  return elemRange[1], elementConnectivityArray[1],ParentElem[1]


 @staticmethod
 def initialize_coordinates(n_zone):
  """ Retrives the coordinates of the points composing the mesh """
  tpath_coord = [CGK.Zone_ts, CGK.GridCoordinates_ts]
  zpl_coord = CGU.getAllNodesByTypeList(n_zone, tpath_coord)
  # For tree t_initial, we get the nodes of types tpath
  for zp_coord in zpl_coord:
            parent_node = CGU.getNodeByPath(n_zone, zp_coord)
            x = CGU.hasChildName(parent_node, "CoordinateX")[1]
            y = CGU.hasChildName(parent_node, "CoordinateY")[1]
            z = CGU.hasChildName(parent_node, "CoordinateZ")[1]
  return(x,y,z) 

 def prepare_cgns_for_meshio(self):
  """ extraxt the faces and the points for the meshio elaboration. The flag 3D defines
         if we treat with a 3D or 2D mesh""" 
  (t_initial, t_ini_lk, t_ini_path) = CGM.load(self.name)
  n_tree = CGC.CGNSPython(t_initial)
  for n_base in n_tree.nextChild(sidstype=CGK.CGNSBase_ts):
        for n_zone in n_base.nextChild(sidstype=CGK.Zone_ts):
               x,y,z = self.initialize_coordinates(n_zone.node)
               ngon_node = self.get_NGon_Node(n_zone).node
               elem_range, element_connectivity_array, parent_elem = self.get_ngon_a_from_zone_node(n_zone)
               # we need to define the faces. The CGNS format ngon define as a first index
               # the number of vertices and subsequently the vertices hence we define
               # a variable index and a position variable to retrive the number of vertices
               # for each faces
               index = 0
               pos=0
               # cells will be the final list of elemento that will be the collaction of the vertices
               # of the cell.
               cells = []
               elemento = []
               while pos!=len(element_connectivity_array): 
                 # we retrive the number of vertices in the cell
                 index = element_connectivity_array[pos]
                 # we increment the pos to start collecting the indices
                 pos+=1
                 # we collect the indices with a loop adapted to the number of vertices
                 for i in range(index):
                     elemento.append(element_connectivity_array[i+pos])
                 cells.append(elemento)
                 # we increment pos
                 pos+=index 
                 # we empty elemento
                 elemento = []      
               #modified will lead to the modification of indices. CGNS start counting from
               # 1 not 0, hence we must decrement the verices relationship         
               modified = []
               for actual_l in cells:
              #  print(len(actual_l))
                minus_one = [number - 1 for number in actual_l]
                modified.append(minus_one)
              # Creation of the dictionary of the volumes based
              # on the CGNS info
               number_faces =  len(modified)
               print("num faces", number_faces)
               assert(number_faces == len(parent_elem))
               number_cells = int(number_faces/4) 
               print("num cells", number_cells)
               
               print(np.max(parent_elem))
               print(parent_elem[100])
              #init dictionary
               elems = {}
               # dictionary key global index cell, value faces composing the cell
               for i,face in enumerate(modified):
               # cycle on the two neigh 
                 for j in range(2):
               # we check if the key exist in elems, if not we create it
               # and we add the neighbouring cells.
                   if parent_elem[i,j] in elems: 
                      elems[parent_elem[i][j]].append(i)
                   else:
                      elems.update({parent_elem[i,j] :[]})
                      elems[parent_elem[i][j]].append(i)

  # we collect the coordinates
  temp = []
  coord = []
  for num in range(len(x)):
        temp.append(x[num])
        temp.append(y[num])
        temp.append(z[num])
        coord.append(temp)
        temp=[]
  points = coord
  return(points,modified,elems) 
