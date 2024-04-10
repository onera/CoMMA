# CoMMA
#
# Copyright © 2024 ONERA
#
# Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
# Contributors: Karim Anemiche
#
# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

"""Goals: show how to agglomerate several consecutive times the same mesh.

Create a mesh with the default constructor of `dualGPy`, apply several levels of
agglomeration with `CoMMA` and export the result.
"""
# import dualGPy.Utils as dGut
import CoMMA
import meshio
import numpy as np
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D, Mesh3D

from comma_tools import build_coarse_graph, prepare_meshio_agglomeration_data

neigh_type_types = {
    CoMMA.Neighbourhood.EXTENDED: "Extended",
    CoMMA.Neighbourhood.PURE_FRONT: "Pure front advancing",
}

seed_ordering_types = {
    CoMMA.SeedsPool.BOUNDARY: "Boundary priority",
    CoMMA.SeedsPool.NEIGHBOURHOOD: "Neighbourhood priority",
    CoMMA.SeedsPool.BOUNDARY_POINT_INIT: "Boundary priority with point initialization",  # noqa: E501
    CoMMA.SeedsPool.NEIGHBOURHOOD_POINT_INIT: "Neighbourhood priority with point initialization",  # noqa: E501
}

# USER PARAMETERS
#################
# Input-related parameters
dimension = 2
##
anisotropic = False
## Points per edge of the quad/hex
n = 21
## CoMMA parameters
build_lines = True
if dimension == 3:
    minCard, goalCard, maxCard = 8, 8, 8
else:
    minCard, goalCard, maxCard = 4, 4, 4
correction = False
threshold_anisotropy = 4.0
odd_line_length = True
# Seeds pool ordering choices:
# - SeedsPool.BOUNDARY:  Boundary priority, 0
# - SeedsPool.NEIGHBOURHOOD: Neighbourhood priority, 1
# - SeedsPool.BOUNDARY_POINT_INIT: Boundary priority with point initialization,
#       10
# - SeedsPool.NEIGHBOURHOOD_POINT_INIT: Neighbourhood priority with point
#       initialization, 11
seed_order = CoMMA.SeedsPool.BOUNDARY
# Neighbourhood type choices:
# - Neighbourhood.EXTENDED: Extended, 0, standard
# - Neighbourhood.PURE_FRONT: Pure front advancing, 1, experimental
neigh_type = CoMMA.Neighbourhood.EXTENDED
# Threshold cardinality for a coarse cell to be considered singular
sing_card = 1
# Max cells in an anisotropic line
max_cells_in_line = None  # Or positive number
# Number of iterations for iterative fine-cell research algorithm
fc_iter = 1
agglomeration_levels = 3

# Output-related parameters, they should help with visualization. One can try only one
# or both at the same time.
# If < 1, the value associated to the coarse cells are the ID. Otherwise, only
# numbers from 1 to the given value are used (it makes it easier to distinguish the
# coarse cells in Paraview
renumber_coarse = 12  # -1 #
# Whether to shuffle the ID of the coarse cells (so that adjacent cell should not have
# closer ID)
shuffle_coarse = False
#########

assert agglomeration_levels > 0

print("Requested arguments:")
print(" [Input]")
print(f" * {dimension=}")
print(f" * {anisotropic=}")
print(f" * {n=}")
print(f" * {build_lines=}")
print(f" * {minCard=}")
print(f" * {goalCard=}")
print(f" * {maxCard=}")
print(f" * {correction=}")
print(f" * {threshold_anisotropy=}")
print(f" * {odd_line_length=}")
print(f" * neigh_type={neigh_type_types[neigh_type]}")
print(f" * seed_ordering={seed_ordering_types[seed_order]}")
print(f" * Threshold cardinality for singular cells={sing_card}")
print(f" * Max cells in anisotropic line={max_cells_in_line}")
print(f" * Fine-cell research iterations={fc_iter}")
print(f" * {agglomeration_levels=}")
print(" [Output]")
renum = renumber_coarse > 1
print(
    f" * Coarse cell renumbering={renum}"
    + (f" (from 0 to {renumber_coarse-1})" if renum else "")
)
print(f" * Shuffle coarse-cell IDs: {shuffle_coarse}")
print()

# CoMMATypes-like
CoMMAIndex = np.uint  # We could have used CoMMA.IndexType = Unsigned long
CoMMAInt = CoMMA.IntType  # = int
CoMMAWeight = CoMMA.WeightType  # = double

print("Creating mesh and dual graph...", flush=True, end="")
outname = (
    "out_"
    + ("an" if anisotropic else "")
    + "iso_"
    + ("quad_2D" if dimension == 2 else "hexa_3D")
    + ("_corrected" if correction else "")
    + f"_{agglomeration_levels}agglolvl"
    + ".vtu"
)

mesh = Mesh2D(n, anisotropic) if dimension == 2 else Mesh3D(n, anisotropic)
mesh.get_boundary_faces()
mesh.ComputeGeometry()
graph = Graph2D(mesh)
graph.get_CSR()
mesh.boundary_detection()
print("OK")
nb_fc = len(graph.vertex) - 1
adjMatrix_row_ptr = np.array(graph.vertex, dtype=CoMMAIndex)
adjMatrix_col_ind = np.array(graph.edges, dtype=CoMMAIndex)

adjMatrix_areaValues = np.array(mesh.area, dtype=CoMMAWeight)
volumes = np.array(mesh.volume, dtype=CoMMAWeight)
centers = mesh.centers.copy()
weights = np.arange(start=nb_fc - 1, stop=0, step=-1, dtype=CoMMAWeight)
n_bnd_faces = np.array(mesh.boundary_cells, dtype=CoMMAInt)
fc_to_cc = np.empty(nb_fc, dtype=CoMMAIndex)
anisoCompliantCells = np.arange(nb_fc, dtype=CoMMAIndex)
aniso_lines_idx = np.array([0], dtype=CoMMAIndex)
aniso_lines = np.array([0], dtype=CoMMAIndex)

# Storage for the all the agglomeration levels
out_data = {}
# Since the results of the agglomeration step are given in terms of the previous level,
# we store here the results wrt the initial fine mesh
fc_to_cur_lvl = list(range(nb_fc))

for level in range(agglomeration_levels):
    print(f"* Level {level}:")
    if level > 0:
        print(" - Preparing data...", flush=True, end="")
        build_lines = False
        # This is possibly not the most efficient way to do the following, but it works
        # and it is enough for now
        (
            adjMatrix_row_ptr,
            adjMatrix_col_ind,
            adjMatrix_areaValues,
            volumes,
            n_bnd_faces,
            centers,
        ) = build_coarse_graph(
            fc_to_cc,
            adjMatrix_row_ptr,
            adjMatrix_col_ind,
            adjMatrix_areaValues,
            volumes,
            n_bnd_faces,
            centers,
        )
        nb_cc = n_bnd_faces.shape[0]
        weights = np.arange(start=nb_cc - 1, stop=0, step=-1, dtype=CoMMAWeight)
        fc_to_cc = np.empty(nb_cc, dtype=CoMMAIndex)
        anisoCompliantCells = np.arange(nb_cc, dtype=CoMMAIndex)
        print("OK")

    print(" - CoMMA call...", flush=True, end="")
    fc_to_cc, aniso_lines_idx, aniso_lines = CoMMA.agglomerate_one_level(
        adjMatrix_row_ptr,
        adjMatrix_col_ind,
        adjMatrix_areaValues,
        volumes,
        centers,
        weights,
        anisoCompliantCells,
        n_bnd_faces,
        build_lines,
        anisotropic,
        odd_line_length,
        threshold_anisotropy,
        seed_order,
        fc_to_cc,
        aniso_lines_idx,
        aniso_lines,
        correction,
        dimension,
        goalCard,
        minCard,
        maxCard,
        sing_card,
        max_cells_in_line,
        fc_iter,
        neigh_type,
    )
    print("OK")

    # Update direction from original fine cell to current level coarse cell
    for i in range(len(fc_to_cur_lvl)):
        fc_to_cur_lvl[i] = fc_to_cc[fc_to_cur_lvl[i]]

    print(" - Finalizing...", flush=True, end="")
    out_data[f"agglomerate_{level}"] = prepare_meshio_agglomeration_data(
        fc_to_cur_lvl,
        mesh.mesh.cells,
        modulo_renumbering=renumber_coarse,
        shuffle=shuffle_coarse,
    )
    print("OK")
    print()
    if max(fc_to_cc) == 0 and level < agglomeration_levels - 1:
        # Only one coarse cell that spans the whole mesh, we should quit now
        print("ATTENTION: Only one coarse cell. Quitting here")
        print()
        break

print(f"Writing in {outname}")
meshio.Mesh(mesh.mesh.points, mesh.mesh.cells, cell_data=out_data).write(outname)
