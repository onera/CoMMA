import abc
import mypy
import numpy as np
import matplotlib.pyplot as plt
import networkx as nx
from dualGPy import Utils as ut
class Graph(abc.ABC): 
    """Base class for the Graph2D and Graph3D representation.
     WARNING! 2D and 3D are considered from a geometric point of
     view:

     mesh : Mesh representation from meshio library
    """
    def __init__(self, mesh):
     self.mesh = mesh
     self.graph={}
     self.nx_graph = nx.Graph(self.mesh.connectivity)
     self.edges = []
     self.vertex = [0]
     self.weight = []
    
    @abc.abstractmethod
    def get_CSR(self):
     raise NotImplementedError

    def adj_to_csr(self):
     """Parsing the adjacency matrix in numpy in a CSR (Compressed Sparse Row) representation
     Parameters:

     adj : Adjacency matrix

     """
     # the CSR representation is built starting from the definition, hence
     # we cycle over the adjacency matrix, we identify the non zero entry
     # and finally we fill the vertex vector with the position where we find the 
     # non zero elements in the edges vector. We hence fill the edges vector at the same
     # way
     # numpy adjacency matrix
     adj = nx.to_numpy_array(self.nx_graph, nodelist=range(len(self.mesh.cells)))
     non_zero = np.count_nonzero(self.adj,axis=1)
     for i,e in enumerate(adj[:,1]):
       print(i) 
       self.vertex.append(np.sum(non_zero[:i]))
       for j,f in enumerate(adj[i,:]):
           if adj[i,j]!=0:
            self.edges.append(j)
     self.vertex.append(np.sum(np.count_nonzero(self.adj)))


class Graph2D(Graph):
    def __init__(self, mesh):
        super().__init__(mesh)

    def get_CSR(self):
     edges = []
     vertex = []
     somma = 0
     # Initialize the keys of the graph dictionary
     # cycle on the points
     for key in self.mesh.connectivity:
        self.edges.extend(self.mesh.connectivity[key])
        somma += len(self.mesh.connectivity[key])
        self.vertex.append(somma)
              
                 

    def draw_graph(self,Mesh1,string):
     """Draw the mesh and the graph"""
     plt.figure()
     ### cycle on the elements
     for elemento in Mesh1.cells:
      ### cycle on the point of the elements
         for i in range(len(elemento)):
       ## # plot the grid
          x_value = [Mesh1.mesh.points[elemento[i-1],0],Mesh1.mesh.points[elemento[i],0]]
          y_value = [Mesh1.mesh.points[elemento[i-1],1],Mesh1.mesh.points[elemento[i],1]]
          plt.plot(x_value,y_value,c='r')  
     ##draw the adjacency graph
     nx.draw(self.nx_graph,pos=Mesh1.centers,with_labels = True)
     plt.savefig(string)

