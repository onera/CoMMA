import numpy as np
from numba import njit, prange


# http://www.katrin-affolter.ch/Python/performance_of_all_and_any
@njit
def any_non_zero_values(elements,index):

    for el in elements:

        if el == index:

            return True

    return False

def convert_faces_to_cells(dictio,faces):
   """Convert the dictionary created in the CGNS reading
      in a 3D Volume mesh starting from the faces given
      in the dictionary as input """
   # boundary values
   # the first key of the dictionary contains all the faces that has
   # a boundary value
   del dictio[0]
   # final result to pass to the meshio input
   risultato = []
   # final element to be added to the tuple of the elements for meshio
   elements_final = []
   # actual analysed cell colletting the indices of all the faces composing it
   cella = []
   # final element freed of double vertices
   cella_finale = []
   # cycle on the index
   for key,item in dictio.items():
      for index in item:    
   # I collect all the faces composing the cell in cella
          cella.extend(faces[index])
   #free the double values
      cella_finale=list(set(cella))
      elements_final.append(cella_finale)
   #re-initialize cella
      cella = [] 
   # version for homogeneous meshes, for multi element meshes
   # develop version with a for on elements_final and an append on the apposite
   # tetra or other element as has been done in the CGNS adapter
   if len(cella_finale) == 4: 
      risultato.append(("tetra",elements_final))
   #todo: to be impemented new elements
   return(risultato)

def dualize_mesh(points,cells,direc):
    """ Passing from a 3D to a 2D mesh in the desired direction (direc) """
    # coordinate to suppress 
    z_del=points[0][direc]
    # correlation dictionary of the old index with the new one
    corr = {}
    # tmp to keep track of cell orderint
    tmp = []
    # final vector of new points
    new_point = []
    # final list of cells
    new_cell = []
    # we build up the correlation
    for i,point in enumerate(points):
      corr.update({i:[]})
    for i,point in enumerate(points):
      print(i)
      # we create the key
      # if the required direction of the 
      # current point is different from the direction we want to delate
      # so we can append the new point to the vector of new points
      if point[direc] != z_del:
        deleted = np.delete(point,direc)
        new_point.append(deleted)
    # we append to the correlation vector the new index of the point
    # that is the value of the length - 1
        corr[i].append(len(new_point)-1)
    # we proceed with the cells
    marker = 0
    for i,cell in enumerate(cells):
      print(i)
      for point in cell:
    # if it exist the correspective it means that
    # the point has been renumbered and it exist
        if corr[point]:
          tmp.append(corr[point][0])
        else:
          marker = 1
    # if marker is 0 it means that there are no problematic
    # points so tmp has been created with ht new index
      if marker == 0:
        new_cell.append(tmp)
    # reinitialize temp and marker
      tmp=[]
      marker = 0
    polyg = []
    tri = []
    quad =[]
    final = []
    for i,cell in enumerate(new_cell):
      if len(cell)==3:
         tri.append(cell)
      elif len(cell)==4:
         quad.append(cell)
      else:
         polyg.append(cell)
    if tri:
      final.append(("triangle",tri))
    if quad:
      final.append(("quad",quad))
    if polyg:
      final.append(("polygon",polyg))
    return(new_point,final) 

    
    
def get_dual_points(compliant_cells, index ):
      """ Function that returns the points of the dual mesh nearest to the point in the mesh given by the index.
          Parameters:
          mesh:       meshio.Mesh object
              Input mesh.
          index:      int
              Index of the point in the input mesh for which to calculate the nearest points of the dual mesh.
          Return:
              compliant 
        # Find the cells where the given index appears, REMEMBER the where statement gives you immediately back the indexof the compliant cell
        # building the compliant cells list
      """       
      compliant = [i for i,e in enumerate(compliant_cells) if any_non_zero_values(e,index)]
      return compliant 


def address_agglomerated_cells(fc_to_cc_res,num_interval):
    """ Helper function to analyse the agglomeration in a Paraview environment, with
        a num_interval scale."""
    #initialize vector of fine cells
    fine_cells = [np.float64(-1) for i in range(len(fc_to_cc_res))]
    #initialize touched flag
    touched= [0 for i in range(len(fc_to_cc_res))]
    #create the marker
    index = [i for i in range(num_interval)]
    #iterator to advance index
    i=0
    for j,cell_1 in enumerate(fc_to_cc_res):
         #assign riempi
         riempi = index[i]
         # if already touched advance
         if (touched[j]==1):
            continue
         for k,cell_2 in enumerate(fc_to_cc_res):
            if (cell_2 == cell_1):
              #it means they are agglomerated together 
              # so fill
              fine_cells[k]=np.float64(riempi)
              # assign touched
              touched[k]=1
         #cycle the iterator
         if riempi == index[-1]:
            i=0
         else:
            i+=1
    return(fine_cells)

