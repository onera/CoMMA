import abc 
import meshio
import mypy
import matplotlib.pyplot as plt
import numpy as np
import itertools
import time
from dualGPy import Utils as ut 
from dualGPy.Geometry import Face2D,Tetra 

class Mesh(abc.ABC):
    """ Interface class to compute all the geometrical characteristics of the mesh """
    def __init__(self, mesh):
        self.mesh = mesh
        self.cells = []  
        # non directional faces (in graph sense)
        self.faces = {}
        # Directional faces in graph sense
        self.Dfaces ={}
        self.centers = []
        self.volume = []
        self.area = []
        self.onValley  = []
        self.onRidge  = []
        self.onCorner  = []
        self.boundary_cells = []
        self.connectivity = {}

    @abc.abstractmethod
    def ComputeGeometry(self,points):        
        """ compute the volume of the cells with respect to the dimensionality (2D area, 3D volume) """
        raise NotImplementedError
    @abc.abstractmethod
    def get_boundary_faces(self):
        """ compute the Area of the faces of the cell with respect to the dimensionality """
        raise NotImplementedError

    @abc.abstractmethod
    def boundary_detection(self):
        """ compute the Area of the faces of the cell with respect to the dimensionality """
        raise NotImplementedError

    @abc.abstractmethod
    def setup_mesh(self):
        """ Method that setup the elements to elaborate the graph and hence casts all the cells in a vector starting from a meshio.Mesh object. It fills the cells list of array and the boundary list. 
        The method fills also the vector of the center points of the mesh.
        Results:
            - class.boundary faces
            - class.cells
            - class.centers """
        raise NotImplementedError

class Mesh2D(Mesh) :
    
    def __init__(self, mesh):
        super().__init__(mesh)
        self.setup_mesh()

    def setup_mesh(self):
        for i,e in enumerate(self.mesh.cells):
          for j,f in enumerate(self.mesh.cells[i][1]):
           self.cells.append(self.mesh.cells[i][1][j])
        x_centerpoint=0
        y_centerpoint=0
        # initialization of the vector of all the centerpoints of the cells
        for elemento in self.cells:
        # cycle on the point of the elements
           for i in range(len(elemento)):
        # compute the centerpoint (accumulating)
             x_centerpoint += self.mesh.points[elemento[i],0]
             y_centerpoint += self.mesh.points[elemento[i],1]
        # define the center point
           x_centerpoint/=len(elemento)
           y_centerpoint/=len(elemento)
        #  append to the centerpoints vector the element computed
           self.centers.append([x_centerpoint,y_centerpoint])
        #  re-initialize the accumulation vectors
           x_centerpoint=0
           y_centerpoint=0




    def draw_graph(self,string):
     """Draw the mesh and the graph"""
     plt.figure()
    ### cycle on the elements
     for i,elemento in enumerate(self.cells):
      ### cycle on the point of the elements
         print(i)
     #to print numero cell
     #    if i<100:
     #     print("true")
     #     plt.text(self.centers[i][0],self.centers[i][1], i, fontsize = 10)
         for i in range(len(elemento)):
       ## # plot the grid
           x_value = [self.mesh.points[elemento[i-1],0],self.mesh.points[elemento[i],0]]
           y_value = [self.mesh.points[elemento[i-1],1],self.mesh.points[elemento[i],1]]
           plt.plot(x_value,y_value,c='k')
         #else:
         # break  
     x1,x2,y1,y2 = plt.axis()  
     plt.axis((0.0,0.25,-0.1,0.1))
     ##draw the adjacency graph
     plt.savefig(string)

    def draw_aggl_lines_full(self,string,lines_dict):
     """Draw the mesh and the graph"""
     plt.figure()
     plt.axis('equal')
    ### cycle on the elements
     for i,elemento in enumerate(self.cells):
      ### cycle on the point of the elements
         print(i)
     #to print numero cell
     #    if i<100:
     #     print("true")
     #     plt.text(self.centers[i][0],self.centers[i][1], i, fontsize = 10)
         for i in range(len(elemento)):
       ## # plot the grid
           x_value = [self.mesh.points[elemento[i-1],0],self.mesh.points[elemento[i],0]]
           y_value = [self.mesh.points[elemento[i-1],1],self.mesh.points[elemento[i],1]]
           plt.plot(x_value,y_value,c='b')
         #else:
         # break  
     x_line = []
     y_line = []
     for key,value in lines_dict.items():
         for cell in value:
             x_line.append(self.centers[cell][0])
             y_line.append(self.centers[cell][1])
         plt.plot(x_line,y_line,linewidth=2.0,c='r')
         x_line = []
         y_line = []
     ##draw the adjacency graph
     plt.savefig(string)


    def draw_aggl_lines(self,string,lines_dict,xlim_min,xlim_max,ylim_min,ylim_max):
     """Draw the mesh and the graph"""
     plt.figure()
    ### cycle on the elements
     for i,elemento in enumerate(self.cells):
      ### cycle on the point of the elements
         print(i)
     #to print numero cell
     #    if i<100:
     #     print("true")
     #     plt.text(self.centers[i][0],self.centers[i][1], i, fontsize = 10)
         for i in range(len(elemento)):
       ## # plot the grid
           x_value = [self.mesh.points[elemento[i-1],0],self.mesh.points[elemento[i],0]]
           y_value = [self.mesh.points[elemento[i-1],1],self.mesh.points[elemento[i],1]]
           plt.plot(x_value,y_value,c='b')
         #else:
         # break  
     x_line = []
     y_line = []
     for key,value in lines_dict.items():
         for cell in value:
             x_line.append(self.centers[cell][0])
             y_line.append(self.centers[cell][1])
         plt.plot(x_line,y_line,linewidth=2.0,c='r')
         x_line = []
         y_line = []
     x1,x2,y1,y2 = plt.axis()  
     plt.axis((xlim_min,xlim_max,ylim_min,ylim_max))
     ##draw the adjacency graph
     plt.savefig(string)


    def ComputeGeometry(self):
        """ In the case of the 2D class it will be an Area """
        # points of the specific cell
        cell_points=[]
        # cycle on the cells
        for i,cell in enumerate(self.cells):
        # cycle on the indexes
            for index in cell:
        # accumulating the cell points
                cell_points.extend(self.mesh.points[index])
        # applying shoelace formul
        # 1. reshape the cell points vector to operate directly with vectors, avoiding unnecessary loops
        # 2. apply the shoelace
            cella = Face2D(self.faces[i],cell_points)
            cella.ComputeArea()
            cella.ComputeLength(self.mesh.points)
            self.volume.append(cella.area)
            self.area.extend(cella.leng_segments)
            cell_points =[]
 
    def get_boundary_faces(self):
     """ Returns the dual mesh held in a dictionary Graph with dual["points"] giving the coordinates and
     dual["cells"] giving the indicies of all the cells of the dual mesh.
     """
     # Get the first set of points of the dual mesh
     compliant_cells = ut.get_dual_points(self.cells, 0)    
     for i in range(len(self.cells)):
        self.faces.update({i :[]}) 
        self.Dfaces.update({i :[]}) 
        self.connectivity.update({i :[]}) 
     # cycle on the points 
     for idx in range(1, len(self.mesh.points)):
        # Get the dual mesh points for a given mesh vertex and the compliant cells to be analysed
        compliant_cells = ut.get_dual_points(self.cells, idx) 
        # in this part we build the graph: for each point of the mesh we have the compliant cells
        # and we cycle over the compliant cells (two nested loop, with an if that avoids to inspect the same cell)
        # me create the list inter that check the common point between two vectors (that can have also different 
        # dimension, considering that they can represent cells of completely different shape. 
        # checked that we have more than two vertex in common (WE ARE IN 2D HERE), and that the node is not already
        # connected with the analysed cell, we add it to the respective dictionary key.
        for i in compliant_cells:
          for j in compliant_cells:
             if i!=j:
               inter = list(set(self.cells[i]).intersection(self.cells[j]))
        # in the faces part we have to associate with each cell all the faces
        # like in a bi-directed graph
               if ((len(inter)>=2) and (inter not in self.faces[i]) and (inter not in self.faces[j])):
                 self.Dfaces[i].append(inter) 
               if ((len(inter)>=2) and (inter not in self.faces[i])):
                 self.faces[i].append(inter)
                 self.connectivity[i].append(j)
        print(idx)


    def boundary_detection(self):
     """ automatically determine the boundary condition
         Right now we generate all the combination of possible faces and we see if they
         are in the neightborhood. If they are not we print them out."""
     # define the dictionary of boundaries to determine later exactly the boundaty based on number of diagonals
     boundary_dict = {}
     for i in range(len(self.cells)):
        # initialize boundary dict
        boundary_dict.update({i :[]})
        # solution from https://stackoverflow.com/questions/69618239/missing-couple-of-elements-in-a-vector
        combination = itertools.combinations(self.cells[i],2)
        inter_boundary = set(combination).difference(map(tuple,self.faces[i]))
        list_inter_boundary = list(map(list,inter_boundary))
        loop_boundary = list_inter_boundary.copy()
        # We create a copy of the list with copy method because we cannot remove elements from a list we are looping
        # https://stackoverflow.com/questions/14126726/python-throws-valueerror-list-removex-x-not-in-list   
        # We check the presence of the iverted faces and we free the list of the boundaries.
        for element in loop_boundary:
           for face_cell in self.faces[i]:
              if sorted(element) == sorted(face_cell):
                 list_inter_boundary.remove(element)
        boundary_dict[i].extend(list_inter_boundary)
        # if is more than number of diagonal i should add it to the boundary cells, because
        # one face is the boundary (I am not currently interested in which are the boundary faces)
        # and build the on boundary vector
        num_diag = len(self.cells[i])*(len(self.cells[i])-3)/2
        num_boundaries = len(boundary_dict[i]) - num_diag
        if (len(boundary_dict[i]) > num_diag) : 
            self.boundary_cells.append(np.int(num_boundaries))
            if (num_boundaries == 1):
                self.onValley.append(i)
            elif (num_boundaries == 2):
                self.onRidge.append(i)
            elif (num_boundaries>=3):
                self.onCorner.append(i)
        else:
            self.boundary_cells.append(np.int(0))


class Mesh3D(Mesh):  
    """ Implements the 3D mesh: ATTENTION! Right now only tetra supported, but flexible to implement also
        hexa and pyramids""" 
    def __init__(self, mesh):
        super().__init__(mesh)
        self.setup_mesh()

    def setup_mesh(self):
        for i,e in enumerate(self.mesh.cells):
          for j,f in enumerate(self.mesh.cells[i][1]):
           self.cells.append(self.mesh.cells[i][1][j])
        x_centerpoint=0
        y_centerpoint=0
        z_centerpoint=0
        # initialization of the vector of all the centerpoints of the cells
        for elemento in self.cells:
        # cycle on the point of the elements
           for i in range(len(elemento)):
        # compute the centerpoint (accumulating)
             x_centerpoint += self.mesh.points[elemento[i],0]
             y_centerpoint += self.mesh.points[elemento[i],1]
             z_centerpoint += self.mesh.points[elemento[i],2]
        # define the center point
           x_centerpoint/=len(elemento)
           y_centerpoint/=len(elemento)
           z_centerpoint/=len(elemento)
        #  append to the centerpoints vector the element computed
           self.centers.append([x_centerpoint,y_centerpoint,z_centerpoint])
        #  re-initialize the accumulation vectors
           x_centerpoint=0
           y_centerpoint=0
           z_centerpoint=0


    def get_boundary_faces(self):
     """ Returns the dual mesh held in a dictionary Graph with dual["points"] giving the coordinates and
     dual["cells"] giving the indicies of all the cells of the dual mesh.
     """
     # Get the first set of points of the dual mesh
     compliant_cells = ut.get_dual_points(self.cells, 0)    
     for i in range(len(self.cells)):
        self.faces.update({i :[]}) 
        self.Dfaces.update({i :[]}) 
        self.connectivity.update({i :[]}) 
     # cycle on the points 
     for idx in range(1, len(self.mesh.points)):
        # Get the dual mesh points for a given mesh vertex and the compliant cells to be analysed
        compliant_cells = ut.get_dual_points(self.cells, idx) 
        # in this part we build the graph: for each point of the mesh we have the compliant cells
        # and we cycle over the compliant cells (two nested loop, with an if that avoids to inspect the same cell)
        # me create the list inter that check the common point between two vectors (that can have also different 
        # dimension, considering that they can represent cells of completely different shape. 
        # checked that we have more than two vertex in common (WE ARE IN 2D HERE), and that the node is not already
        # connected with the analysed cell, we add it to the respective dictionary key.
        for i in compliant_cells:
          for j in compliant_cells:
             if i!=j:
               inter = list(set(self.cells[i]).intersection(self.cells[j]))
        # in the faces part we have to associate with each cell all the faces
        # like in a bi-directed graph
               if ((len(inter)>=3) and (inter not in self.faces[i]) and (inter not in self.faces[j])):
                 self.Dfaces[i].append(inter) 
               if ((len(inter)>=3) and (inter not in self.faces[i])):
                 self.faces[i].append(inter)
                 self.connectivity[i].append(j)
        print(idx)


    def ComputeGeometry(self):
        """ In the case of the 2D class it will be an Area """
        # points of the specific cell
        cell_points=[]
        # cycle on the cells
        for i,cell in enumerate(self.cells):
        # cycle on the indexes
            for index in cell:
        # accumulating the cell points
                cell_points.extend(self.mesh.points[index])
        # applying shoelace formul
        # 1. reshape the cell points vector to operate directly with vectors, avoiding unnecessary loops
        # 2. apply the shoelace
            cella = Tetra(cell_points,self.faces[i])
            cella.ComputeArea(self.mesh.points)
            cella.ComputeVolume()
            self.volume.append(cella.volume)
            self.area.extend(cella.AreaFaces)
            cell_points =[]
 
    def boundary_detection(self):
     """ automatically determine the boundary condition
         Right now we generate all the combination of possible faces and we see if they
         are in the neightborhood. If they are not we print them out."""
     # define the dictionary of boundaries to determine later exactly the boundaty based on number of diagonals
     boundary_dict = {}
     for i in range(len(self.cells)):
        # initialize boundary dict
        boundary_dict.update({i :[]})
        # solution from https://stackoverflow.com/questions/69618239/missing-couple-of-elements-in-a-vector
        combination = itertools.combinations(self.cells[i],3)
        inter_boundary = set(combination).difference(map(tuple,self.faces[i]))
        list_inter_boundary = list(map(list,inter_boundary))
        loop_boundary = list_inter_boundary.copy()
        # We create a copy of the list with copy method because we cannot remove elements from a list we are looping
        # https://stackoverflow.com/questions/14126726/python-throws-valueerror-list-removex-x-not-in-list   
        # We check the presence of the inverted faces and we free the list of the boundaries.
        for element in loop_boundary:
           for face_cell in self.faces[i]:
              if sorted(element) == sorted(face_cell):
                 list_inter_boundary.remove(element)
        boundary_dict[i].extend(list_inter_boundary)
        # In case of tetra we do not have diagonals, hence given 3 points
        # we define exactly the boundary
        num_boundaries = len(boundary_dict[i]) 
        self.boundary_cells.append(np.int(num_boundaries))
        if (num_boundaries == 1):
            self.onValley.append(i)
        elif (num_boundaries == 2):
            self.onRidge.append(i)
        elif (num_boundaries>=3):
            self.onCorner.append(i)
        else:
            self.boundary_cells.append(np.int(0))


