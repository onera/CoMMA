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
import os
from math import floor, log  # noqa: F401

import meshio

# import dualGPy.Utils as dGut
import numpy as np
from CoMMA import agglomerate_one_level
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D

from comma_tools import (
    assign_anisotropic_line_data_to_cells,
    build_coarse_graph,
    compute_neighbourhood_wall_distance,
    prepare_meshio_agglomeration_data,
    prepare_meshio_celldata,
)


def limit_line_length(idxs, cells, max_cells_in_line):
    """Cut the line so that it only has a given number of cells."""
    cur_idx = 0
    ret_idxs, ret_cells = [cur_idx], []
    # For every line
    for ln in range(len(idxs) - 1):
        new_n_cells_in_line = min(max_cells_in_line, idxs[ln + 1] - idxs[ln])
        start = idxs[ln]
        ret_cells.extend(cells[start : start + new_n_cells_in_line])
        cur_idx += new_n_cells_in_line
        ret_idxs.append(cur_idx)
    return ret_idxs, ret_cells


neigh_type_types = ["Extended", "Pure front advancing"]

seed_ordering_types = {
    0: "Boundary priority",
    1: "Neighbourhood priority",
    10: "Boundary priority with point initialization",
    11: "Neighbourhood priority with point initialization",
}

# USER PARAMETERS
#################
input_mesh, input_format = "../meshes/raebis_ansys.msh", "ansys"
# Number of cells in the aniso lines in the last agglomeration step
# n_cells_in_aniso_line = 2
##
anisotropic = True
## CoMMA parameters
build_lines = True
minCard, goalCard, maxCard = 2, 4, 6
correction = True
threshold_anisotropy = -4.0
odd_line_length = True
neigh_type = 0  # 0 = Extended (standard), 1 = Pure front advancing
seed_order = 0  # 0 = Boundary priority, 1 = Neighbourhood priority,
#                 10 = Boundary priority with point initialization
#                 11 = Neighbourhood priority with point initialization
# Threshold cardinality for a coarse cell to be considered singular
sing_card = 1
# Max cells in an anisotropic line
max_cells_in_line = None  # Or positive number
# Number of iterations for iterative fine-cell research algorithm
fc_iter = 1
# Number of iteration steps to perform
agglomeration_levels = 3

# Output-related parameters, they should help with visualization. One can try only one
# or both at the same time.
# If < 1, the value associated to the coarse cells are the ID. Otherwise, only
# numbers from 1 to the given value are used (it makes it easier to distinguish the
# coarse cells in Paraview
renumber_coarse = -1  # 12  # -1 #
# Whether to shuffle the ID of the coarse cells (so that adjacent cell should not have
# closer ID)
shuffle_coarse = True
#########

basename = os.path.basename(input_mesh)
basename, ext = os.path.splitext(basename)
outname = basename + ".vtu"

assert agglomeration_levels > 0

print("Requested arguments:")
print(" [Input]")
print(f" * {input_mesh=}")
print(f" * {anisotropic=}")
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
CoMMAIndex = np.uint  # Unsigned long
CoMMAInt = int
CoMMAWeight = np.double

outname = (
    "raebis_"
    + ("an" if anisotropic else "")
    + "iso"
    + ("_corrected" if correction else "")
    + f"_{agglomeration_levels}agglolvl"
    + ".vtu"
)

dimension = 2

print("Reading mesh and creating dual graph...", flush=True, end="")
if not os.path.exists(input_mesh):
    raise OSError(f"Cannot find mesh {input_mesh}")
mmio = meshio.read(input_mesh, file_format=input_format)
mesh = Mesh2D(mmio)
mesh.get_boundary_faces()
mesh.ComputeGeometry()
graph = Graph2D(mesh)
graph.get_CSR()
mesh.boundary_detection()

nb_fc = len(graph.vertex) - 1
adjMatrix_row_ptr = np.array(graph.vertex, dtype=CoMMAIndex)
adjMatrix_col_ind = np.array(graph.edges, dtype=CoMMAIndex)
adjMatrix_areaValues = np.array(mesh.area, dtype=CoMMAWeight)
volumes = np.array(mesh.volume, dtype=CoMMAWeight)
centers = mesh.centers.copy()
n_bnd_faces = np.array(mesh.boundary_cells, dtype=CoMMAInt)

# The mesh is hybrid with squares in the BL and triangles else where
squares = [i for i, cell in enumerate(mesh.cells) if len(cell) == 4]
foil = [i for i in squares if mesh.boundary_cells[i] > 0]
foil_distance = compute_neighbourhood_wall_distance(
    adjMatrix_row_ptr, adjMatrix_col_ind, foil
)

# Choosing the whole BL as compliant
arrayOfFineAnisotropicCompliantCells = squares
# Weights inversely proportional to the distance so that cells closer to the
# boundary have higher priority (+1 to avoid division by 0)
weights = np.reciprocal(1.0 + foil_distance.astype(float))

fc_to_cc = np.empty(nb_fc, dtype=CoMMAIndex)

agglomerationLines_Idx = np.array([0], dtype=CoMMAIndex)
agglomerationLines = np.array([0], dtype=CoMMAIndex)
print("OK")

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
        # Here, it's hard to redo what we did for the distance to the foil, but
        # it is OK
        weights = np.arange(start=nb_cc - 1, stop=0, step=-1, dtype=CoMMAWeight)
        fc_to_cc = np.empty(nb_cc, dtype=CoMMAIndex)
        arrayOfFineAnisotropicCompliantCells = np.arange(nb_cc, dtype=CoMMAIndex)
        # Here we try some ways to limit the anisotropic zone after the first
        # agglomeration step
        # if level >= 1:
        if False:
            # Plainly, switch it off
            # anisotropic = False

            # Limit the length of the line
            # Some empirical laws
            # max_cells = 2**max(1, 4 - level**2)
            # max_cells = (
            # n_cells_in_aniso_line * 2**(agglomeration_levels - 1 - level)
            # )
            line_length = agglomerationLines_Idx[1] - agglomerationLines_Idx[0]
            # Reduce by 2 so that the excluded cells will be agglomerated with
            # their neighbours (also excluded) and form a nice coarse with
            # cardinality 4
            max_cells = max(2, line_length - 2)
            # Closest smallest power of 2, since the agglomeration is always 2-by-2
            # max_cells = 2**int(floor(log(line_length, 2)))
            agglomerationLines_Idx, agglomerationLines = limit_line_length(
                agglomerationLines_Idx, agglomerationLines, max_cells
            )
        print("OK")

    print(" - CoMMA call...", flush=True, end="")
    fc_to_cc, agglomerationLines_Idx, agglomerationLines = agglomerate_one_level(
        adjMatrix_row_ptr,
        adjMatrix_col_ind,
        adjMatrix_areaValues,
        volumes,
        centers,
        weights,
        arrayOfFineAnisotropicCompliantCells,
        n_bnd_faces,
        build_lines,
        anisotropic,
        odd_line_length,
        threshold_anisotropy,
        seed_order,
        fc_to_cc,
        agglomerationLines_Idx,
        agglomerationLines,
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
    if level == 0:
        out_data["anisotropic_lines"] = prepare_meshio_celldata(
            assign_anisotropic_line_data_to_cells(
                agglomerationLines_Idx,
                agglomerationLines,
                fc_to_cc,
                modulo_renumbering=renumber_coarse,
                shuffle=shuffle_coarse,
            ),
            mesh.mesh.cells,
        )
    print("OK")
    print()
    if max(fc_to_cc) == 0 and level < agglomeration_levels - 1:
        # Only one coarse cell that spans the whole mesh, we should quit now
        print("ATTENTION: Only one coarse cell. Quitting here")
        print()
        break

out_data["foil_distance"] = prepare_meshio_celldata(foil_distance, mesh.mesh.cells)

print(f"Writing in {outname}")
meshio.Mesh(mesh.mesh.points, mesh.mesh.cells, cell_data=out_data).write(outname)
