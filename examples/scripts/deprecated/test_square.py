# CoMMA
#
# Copyright © 2024 ONERA
#
# Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
# Contributors: Karim Anemiche
#
# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

import meshio
import numpy as np
from CoMMA import agglomerate_one_level
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D

# CoMMATypes-like
CoMMAIndex = np.uint  # Unsigned long
CoMMAInt = int
CoMMAWeight = np.double

Mesh1 = Mesh2D(5, False)
Mesh1.get_boundary_faces()
Mesh1.ComputeGeometry()
Graph1 = Graph2D(Mesh1)
Graph1.get_CSR()
Mesh1.boundary_detection()
nb_fc = len(Graph1.vertex) - 1
adjMatrix_row_ptr = np.array(Graph1.vertex, dtype=CoMMAIndex)
adjMatrix_col_ind = np.array(Graph1.edges, dtype=CoMMAIndex)
adjMatrix_areaValues = np.array(Mesh1.area, dtype=CoMMAWeight)
volumes = np.array(Mesh1.volume, dtype=CoMMAWeight)
weights = np.arange(start=nb_fc - 1, stop=0, step=-1, dtype=CoMMAWeight)
n_bnd_faces = np.array(Mesh1.boundary_cells, dtype=CoMMAInt)
fc_to_cc = np.empty(nb_fc, dtype=CoMMAIndex)
indCoarseCell = 0
minCard = 4
goalCard = 4
maxCard = 4
verbose = 0
anisoCompliantCells = np.arange(nb_fc, dtype=CoMMAIndex)
agglomerationLines_Idx = np.array([0], dtype=CoMMAIndex)
agglomerationLines = np.array([0], dtype=CoMMAIndex)
build_lines = True
isAnisotropic = True
dimension = 2
corrections = True
threshold_anisotropy = 4.0
odd_line_length = True
neigh_type = 0
seed_order = 0  # 0 = Boundary priority, 1 = Neighbourhood priority,
# 10 = Boundary priority with point initialization
# 11 = Neighbourhood priority with point initialization
# Threshold cardinality for a coarse cell to be considered singular
sing_card = 1
fc_iter = 1

print("CoMMA call")
# isotropic
(
    fc_to_cc_res1,
    agglomerationLines_Idx_res_iso,
    agglomerationLines_res_iso,
) = agglomerate_one_level(
    adjMatrix_row_ptr,
    adjMatrix_col_ind,
    adjMatrix_areaValues,
    volumes,
    Mesh1.centers.astype(CoMMAWeight, copy=False),
    weights,
    anisoCompliantCells,
    n_bnd_faces,
    build_lines,
    False,
    odd_line_length,
    threshold_anisotropy,
    seed_order,
    fc_to_cc,
    agglomerationLines_Idx,
    agglomerationLines,
    corrections,
    dimension,
    goalCard,
    minCard,
    maxCard,
    sing_card,
    fc_iter,
    neigh_type,
)

print("end CoMMA")
fine_cells_iso = [value for j, value in enumerate(fc_to_cc_res1)]

meshOUT_iso = meshio.Mesh(
    Mesh1.mesh.points,
    Mesh1.mesh.cells,
    cell_data={"agglomerate": [fine_cells_iso]},
)

print("writing")
meshOUT_iso.write("out_iso_quad_4.vtk")
