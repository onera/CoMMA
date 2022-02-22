import numpy as np
from collections import defaultdict,Counter

# right now this tool is intended for square meshes.
class Coarse_mesh:
    def __init__(self, Mesh1, fc_to_cc):
        self.fc_to_cc = fc_to_cc
        self.mesh = Mesh1

    def Create_mesh(self):
        d = {}
        dic_cells = {}
        # We create a dictionary where we associate to the coarse cell (key) with the relative fine cells
        for i, x in enumerate(self.fc_to_cc):
            if x not in d:
               d.update({x:[]})
            d[x].append(i)
        # quad and polyg6
        quad = []
        polyg6 =[]
        # vector of the points for the agglomerated cells. We need to define the agglomerated points reordered as far we are going to get rid of a particular set of points.
        agglomerated_points = []
        #new cell index based on the order of the points
        cell_counter = 0
        for key,value in d.items():
        # single coarse cell indexes split
            coarse_cell = []
            for j in value:
               coarse_cell.extend(self.mesh.cells[j])
            num_elem =Counter(coarse_cell)
            coarse_cell_clean = []
            cell_points = []
           # take only the needed points
            for key, value in num_elem.items():
               if (value < 4):
                  cell_points.append(self.mesh.mesh.points[key])
            # reordering of the points
            cent=(sum([p[0] for p in cell_points])/len(cell_points),sum([p[1] for p in cell_points])/len(cell_points))
            cell_points.sort(key=lambda p: np.arctan2(p[1]-cent[1],p[0]-cent[0]))
           # renumbering of the coarse cell based on the new points vector 
            for i,value in enumerate(cell_points):
               coarse_cell_clean.append(cell_counter)
               cell_counter+=1   
            agglomerated_points.extend(cell_points)
            if len(coarse_cell_clean) not in dic_cells:
               dic_cells.update({len(coarse_cell_clean):[]})
            dic_cells[len(coarse_cell_clean)].append(coarse_cell_clean)

        cc = []
        for key,value in dic_cells.items():
            cc.append(("polygon",value))        
        return(d,agglomerated_points,cc)
