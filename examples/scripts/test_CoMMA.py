# CoMMA
#
# Copyright © 2024 ONERA
#
# Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
# Contributors: Karim Anemiche
#
# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

"""Simple tests to check the soundness of the python module of CoMMA."""
import CoMMA


def agglomerate_aniso_lines():
    """We take the mesh of example `ex_aniso_lines.py`, here hardcoded, and perform an
    agglomeration."""
    dimension = 2
    anisotropic = True
    build_lines = True
    minCard, goalCard, maxCard = 4, 4, 4
    correction = False
    threshold_anisotropy = 1.5
    odd_line_length = True
    AR = CoMMA.AR.DIAMETER_OVER_RADIUS
    seed_order = CoMMA.SeedsPool.BOUNDARY
    neigh_type = CoMMA.Neighbourhood.EXTENDED
    sing_card = 1
    max_cells_in_line = None  # Or positive number
    fc_iter = 1
    # fmt: off
    adjMatrix_row_ptr = [
        0, 2, 5, 8, 11, 14, 16, 19, 23, 27, 31, 35, 38, 42, 46, 50, 54, 57, 60, 62, 65,
        67, 70, 73, 76,
    ]
    adjMatrix_col_ind = [
        1, 11, 0, 2, 10, 1, 3, 9, 2, 4, 8, 3, 5, 7, 4, 6, 5, 7, 17, 4, 8, 6, 15, 3, 9,
        7, 13, 2, 10, 8, 12, 1, 11, 9, 14, 0, 10, 16, 9, 14, 13, 23, 8, 12, 15, 21, 10,
        16, 12, 22, 7, 13, 17, 19, 11, 14, 20, 6, 15, 18, 17, 19, 15, 21, 18, 16, 22,
        13, 23, 19, 14, 20, 23, 12, 22, 21,
    ]
    adjMatrix_areaValues = [
        1. , 0.2, 1. , 1. , 0.2, 1. , 1. , 0.2, 1. , 1. , 0.2, 1. , 1. , 0.2, 1. , 1. ,
        1. , 1. , 1. , 0.2, 1. , 1. , 0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. , 1. , 0.2, 0.2,
        1. , 1. , 0.2, 0.2, 1. , 0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. ,
        1. , 0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. , 0.2, 1. , 1. , 1. , 1. , 1. , 0.2, 1. ,
        1. , 0.2, 1. , 0.2, 1. , 1. , 0.2, 1. , 1. , 0.2, 1. , 1.,
    ]
    volumes = [
        0.2, 0.2, 0.2, 0.2, 0.2, 1. , 1. , 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2,
        0.2, 1. , 1. , 0.2, 0.2, 0.2, 0.2, 0.2,
    ]
    centers = [
        [0.5, 0.1], [0.5, 0.3], [0.5, 0.5], [0.5, 0.7], [0.5, 0.9], [0.5, 1.5],
        [1.5, 1.5], [1.5, 0.9], [1.5, 0.7], [1.5, 0.5], [1.5, 0.3], [1.5, 0.1],
        [2.5, 0.5], [2.5, 0.7], [2.5, 0.3], [2.5, 0.9], [2.5, 0.1], [2.5, 1.5],
        [3.5, 1.5], [3.5, 0.9], [3.5, 0.1], [3.5, 0.7], [3.5, 0.3], [3.5, 0.5],
    ]
    n_bnd_faces = [
        2, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 2, 1, 2, 1, 1, 1
    ]
    # fmt: on
    n_cells = len(volumes)
    weights = [float(n_cells - i) for i in range(n_cells)]
    anisoCompliantCells = list(range(n_cells))
    return CoMMA.agglomerate_one_level(
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
        [],
        [],
        correction,
        dimension,
        goalCard,
        minCard,
        maxCard,
        AR,
        sing_card,
        max_cells_in_line,
        fc_iter,
        neigh_type,
    )


class Test_Aniso_Agglomeration:
    # Run agglomeration and store results
    f2c, alines_idx, alines = agglomerate_aniso_lines()
    # Reference results
    ref_f2c = [0, 0, 1, 1, 1, 8, 8, 3, 3, 3, 2, 2, 5, 5, 4, 5, 4, 8, 8, 7, 6, 7, 6, 7]
    ref_alines_idx = [0, 2, 4, 6, 8]
    ref_alines = [0, 1, 2, 3, 4, 5, 6, 7]

    def test_lines(self):
        assert self.alines_idx == self.ref_alines_idx
        assert self.alines == self.ref_alines

    def test_coarse_cells(self):
        assert self.f2c == self.ref_f2c
