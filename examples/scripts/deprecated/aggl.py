# CoMMA
#
# Copyright © 2023 ONERA
#
# Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
# Contributors: Karim Anemiche
#
# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

# Created by Alberto Remigi 27/08
# Parser for the debugging output file of CoMMA

import numpy as np
from scipy.sparse import csr_matrix
import networkx as nx
import matplotlib.pyplot as plt


with open('data_497.txt', 'r') as file:
	data = file.read().split(";")

# Store in a dictionary the vectors
a = {}
# Cycle on the data extracted list

for j in range(len(data)-1):
    
    # First thing is to create the  key for the map
    # and it is possible only if we have a series of letters
    # and that string is not containing the e+ and e-
    # REMEMBER: all the members of the list are strings! 
    if (data[j].lower().islower() and "e-" not in data[j] and "e+" not in data[j]):
      key = data[j]
      a[key]=np.array([])
    else:
    # If we are not considering a key we increment the vector associated with 
    # the element of the map.
      a[key]=np.append(a[key],np.float64(data[j]))   

#set the matrix (WARNING: The vector is started from 1 because of a bug or in the writing of the data or in the interface CoMMA/CODA-------------------------> TO BE CHECKED <----------------------------
sp = csr_matrix((a[' \nadjMatrix_areaValues'][1:], a[' \nadjMatrix_col_ind'][1:], a['adjMatrix_row_ptr'][1:]))
# Convert to the Adjacency matrix for Networkx
G = nx.from_scipy_sparse_matrix(
    sp, parallel_edges=True, create_using=nx.MultiGraph)

# to draw subgraph
res = [0,1,2,3,4,5] #selected nodes to draw
k = G.subgraph(res)  

# setting of spring layout
pos = nx.spring_layout(G, k=0.8)

# node attributes (area/volume)
#nx.set_node_attributes(G,a[' \nadjMatrix_areaValues'][1:] , 'val')
#node_color = nx.get_node_attributes(G,'val'),cmap = plt.cm.get_cmap('rainbow'),

#figure
plt.figure()
# Here the choice for a subgraph or for an entire graph
nx.draw_networkx(k, pos=pos)
#nx.draw(G, pos,  with_labels = False, width=0.1, node_size=4)
plt.savefig("test.png")
#

