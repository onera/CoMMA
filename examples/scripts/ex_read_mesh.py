# CoMMA
#
# Copyright © 2024 ONERA
#
# Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
# Contributors: Karim Anemiche
#
# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

"""Goals: show how to read an existing mesh with `meshio` and agglomerate it with
`CoMMA`; show how to treat meshes with more than one element-type

Read an existing mesh via `meshio`, agglomerate with `CoMMA` and export the result
Features: A special case of VTK output is used if hybrid mesh with multi-elements
(e.g. quads *and* tetras as in `raebis`)
"""
import os

# import dualGPy.Utils as dGut
import CoMMA
import meshio
import numpy as np
from dualGPy.Graph import Graph2D
from dualGPy.Mesh import Mesh2D, Mesh3D

from comma_tools import (
    AR_DESCRIPTIONS,
    CELL_COUPLING_DESCRIPTIONS,
    NEIGHBOURHOOD_DESCRIPTIONS,
    SEED_ORDERING_DESCRIPTIONS,
    prepare_meshio_agglomeration_data,
)

# USER PARAMETERS
#################
# Input-related parameters
# Format is one of those accepted by meshio or None
# e.g., input_mesh, input_format = "3D_CRM.vtu", None
input_mesh, input_format = "../meshes/naca2d_structured_ansys.msh", "ansys"
##
dimension = 2
##
anisotropic = False
## CoMMA parameters
build_lines = True
if dimension == 3:
    minCard, goalCard, maxCard = 8, 8, 8
else:
    minCard, goalCard, maxCard = 4, 4, 4
correction = False
threshold_anisotropy = 4.0
odd_line_length = True
# Which type of aspect ratio to use. We give the available list below, but for more
# details refer to Documentation/AR_note.pdf, and
# include/CoMMA/{CoMMADefs.h,AR_computer.h}
# - AR.DIAMETER_OVER_RADIUS
# - AR.DIAMETER_OVER_MIN_EDGE
# - AR.DIAMETER
# - AR.ONE_OVER_MEASURE
# - AR.ONE_OVER_INTERNAL_WEIGHTS
# - AR.PERIMETER_OVER_RADIUS
# - AR.EXTERNAL_WEIGHTS
# - AR.MAX_BARY_DIST_OVER_RADIUS
# - AR.MAX_OVER_MIN_BARY_DIST
# - AR.ALGEBRAIC_PERIMETER_OVER_MEASURE
AR = CoMMA.AR.DIAMETER_OVER_RADIUS
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
# Anisotropic cells coupling. Choices:
# - CellCoupling.MAX_WEIGHT = max edge weight (i.e., area)
# - CellCoupling.MIN_DISTANCE = minimum distance between cell centers
aniso_cell_coupling = CoMMA.CellCoupling.MAX_WEIGHT
# Whether to force aniso lines to have a straight direction
force_line_direction = True
# Number of iterations for iterative fine-cell research algorithm
fc_iter = 1

# Output-related parameters, they should help with visualization. One can try only one
# or both at the same time.
# If < 1, the value associated to the coarse cells are the ID. Otherwise, only
# numbers from 1 to the given value are used (it makes it easier to distinguish the
# coarse cells in Paraview
renumber_coarse = -1  # 12  # -1 #
# Whether to shuffle the ID of the coarse cells (so that adjacent cell should not have
# closer ID)
shuffle_coarse = False
#########

basename = os.path.basename(input_mesh)
basename, ext = os.path.splitext(basename)
outname = basename + ".vtu"

print("Requested arguments:")
print(" [Input]")
print(f" * {input_mesh=}")
print(f" * {dimension=}")
print(f" * {anisotropic=}")
print(f" * {build_lines=}")
print(f" * {minCard=}")
print(f" * {goalCard=}")
print(f" * {maxCard=}")
print(f" * aspect ratio={AR_DESCRIPTIONS[AR]}")
print(f" * {correction=}")
print(f" * {threshold_anisotropy=}")
print(f" * {odd_line_length=}")
print(" * Priority weights: reversed ID")
print(f" * neigh_type={NEIGHBOURHOOD_DESCRIPTIONS[neigh_type]}")
print(f" * seed_ordering={SEED_ORDERING_DESCRIPTIONS[seed_order]}")
print(f" * Threshold cardinality for singular cells={sing_card}")
print(f" * Max cells in anisotropic line={max_cells_in_line}")
print(
    f" * Cell coupling for aniso cells={CELL_COUPLING_DESCRIPTIONS[aniso_cell_coupling]}"
)
print(f" * Force aniso lines direction={force_line_direction}")
print(f" * Fine-cell research iterations={fc_iter}")
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

print("Reading mesh and creating dual graph...", flush=True, end="")
if not os.path.exists(input_mesh):
    raise OSError(f"Cannot find mesh {input_mesh}")
mmio = meshio.read(input_mesh, file_format=input_format)
mio_dim = mmio.points.shape[-1]
if dimension != mio_dim:
    raise ValueError(
        f"""You requested dimension {dimension}, but mesh is of dimension {mio_dim}. Please, change one of the two
Reminder: VTK mesh are read as 3D by meshio even if they are 2D. See dualGPy ex_dualize.py for more info"""  # noqa: E501
    )

mesh = Mesh2D(mmio) if dimension == 2 else Mesh3D(mmio)
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
weights = np.arange(start=nb_fc - 1, stop=0, step=-1, dtype=CoMMAWeight)
n_bnd_faces = np.array(mesh.boundary_cells, dtype=CoMMAInt)
anisoCompliantCells = np.arange(nb_fc, dtype=CoMMAIndex)
aniso_lines_idx = np.array([0], dtype=CoMMAIndex)
aniso_lines = np.array([0], dtype=CoMMAIndex)

print("CoMMA call...", flush=True, end="")
(
    fc_to_cc,
    aniso_lines_idx_res_iso,
    aniso_lines_res_iso,
) = CoMMA.agglomerate_one_level(
    adjMatrix_row_ptr,
    adjMatrix_col_ind,
    adjMatrix_areaValues,
    volumes,
    mesh.centers.astype(CoMMAWeight, copy=False),
    weights,
    anisoCompliantCells,
    n_bnd_faces,
    build_lines,
    anisotropic,
    odd_line_length,
    threshold_anisotropy,
    seed_order,
    aniso_lines_idx,
    aniso_lines,
    correction,
    dimension,
    goalCard,
    minCard,
    maxCard,
    AR,
    sing_card,
    max_cells_in_line,
    aniso_cell_coupling,
    force_line_direction,
    fc_iter,
    neigh_type,
)
print("OK")

print("Finalizing...", flush=True, end="")
agglo = prepare_meshio_agglomeration_data(
    fc_to_cc,
    mesh.mesh.cells,
    modulo_renumbering=renumber_coarse,
    shuffle=shuffle_coarse,
)
print("OK")

print(f"Writing in {outname}")
meshio.Mesh(mesh.mesh.points, mesh.mesh.cells, cell_data={"agglomerate": agglo}).write(
    outname
)
