# CoMMA
#
# Copyright © 2024 ONERA
#
# Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
# Contributors: Karim Anemiche
#
# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

"""/!\ WARNING /!\
I was not able to reconstruct the cell centers, so this test could be used in the
newest CoMMA version"""

import numpy as np
from CoMMA import agglomerate_one_level

# This is a problem...
centers = None

# CoMMATypes-like
CoMMAIndex = np.uint  # Unsigned long
CoMMAInt = int
CoMMAWeight = np.double
nb_fc = 15
adjMatrix_col_ind_size = 51
adjMatrix_row_ptr = np.array(
    [0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51], dtype=CoMMAIndex
)
adjMatrix_col_ind = np.array(
    [
        0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4, 7,
        8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13,
        11, 14,  # fmt: skip
    ],
    dtype=CoMMAIndex,
)
adjMatrix_areaValues = np.array(
    [
        6.82842712, 2.0, 2.0, 2.0, 4.23606798, 1.0, 2.0, 4.0, 2.23606798, 2.23606798,
        1.0, 4.0, 2.23606798, 6.82842712, 2.0, 2.0, 2.23606798, 2.0, 2.0, 1.0,
        2.23606798, 2.23606798, 4.0, 2.0, 2.0, 2.0, 1.0, 2.23606798, 1.0, 1.0, 1.0,
        1.41421356, 2.23606798, 4.0, 2.0, 3.60555128, 1.41421356, 2.0, 2.0, 3.16227766,
        2.0, 6.0, 3.16227766, 3.16227766, 3.60555128, 11.0, 3.16227766, 3.16227766, 8.0,
        3.16227766, 7.0,  # fmt: skip
    ],
    dtype=CoMMAWeight,
)
volumes = np.array(
    [2.0, 1.0, 2.0, 1.0, 2.0, 1.0, 2.0, 1.0, 0.5, 2.0, 1.0, 3.0, 3.0, 3.0, 1.5],
    dtype=CoMMAWeight,
)
weights = np.arange(start=nb_fc - 1, stop=0, step=-1, dtype=CoMMAWeight)
n_bnd_faces = np.array([1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2], dtype=CoMMAInt)
array_isOnRidge = np.array([0, 1, 3, 4, 13])
array_isOnValley = np.array([12, 14])
array_isOnCorner = np.array([], dtype=CoMMAIndex)
fc_to_cc = np.empty(nb_fc, dtype=CoMMAIndex)
indCoarseCell = 0
minCard = 4
goalCard = 4
maxCard = 4
checks = 1
arrayOfFineAnisotropicCompliantCells = np.arange(nb_fc, dtype=CoMMAIndex)
agglomerationLines_Idx = np.zeros(nb_fc, dtype=CoMMAIndex)
agglomerationLines = np.zeros(nb_fc, dtype=CoMMAIndex)
build_lines = True
isAnisotropic = False
dimension = 2
corrections = True
threshold_anisotropy = 4.0
odd_line_length = True
neigh_type = 0
seed_order = 0  # 0 = Boundary priority, 1 = Neighbourhood priority,
#                 10 = Boundary priority with point initialization
#                 11 = Neighbourhood priority with point initialization
# Threshold cardinality for a coarse cell to be considered singular
sing_card = 1
fc_iter = 1


(
    fc_to_cc_res,
    agglomerationLines_Idx_res,
    agglomerationLines_res,
) = agglomerate_one_level(
    adjMatrix_row_ptr,
    adjMatrix_col_ind,
    adjMatrix_areaValues,
    volumes,
    centers,
    weights,
    arrayOfFineAnisotropicCompliantCells,
    n_bnd_faces,
    array_isOnValley,
    array_isOnRidge,
    array_isOnCorner,
    build_lines,
    isAnisotropic,
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

print(fc_to_cc_res)
