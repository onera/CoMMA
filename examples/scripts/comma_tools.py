# CoMMA
#
# Copyright © 2024 ONERA
#
# Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
# Contributors: Karim Anemiche
#
# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

"""A collection of useful functions"""
import typing

import numpy as np
import numpy.typing as npt
from CoMMA import AR, CellCoupling, Neighbourhood, SeedsPool

AR_DESCRIPTIONS = {
    AR.DIAMETER_OVER_RADIUS: "Diameter over radius",
    AR.DIAMETER_OVER_MIN_EDGE: "Diameter over minimum edge",
    AR.DIAMETER: "Diameter",
    AR.ONE_OVER_MEASURE: "One over the measure (e.g., volume) of the cell",
    AR.ONE_OVER_INTERNAL_WEIGHTS: "One over the internal weights",
    AR.PERIMETER_OVER_RADIUS: "Perimeter over radius",
    AR.EXTERNAL_WEIGHTS: "External weights, that is, perimeter",
    AR.MAX_BARY_DIST_OVER_RADIUS: "Maximum FC-center distance from barycenter over radius",  # noqa: E501
    AR.MAX_OVER_MIN_BARY_DIST: "Maximum over minimum FC-center distance from barycenter",  # noqa: E501
    AR.ALGEBRAIC_PERIMETER_OVER_MEASURE: "Algebraic-like perimeter over measure, that is, external weights over cell weight",  # noqa: E501
}
NEIGHBOURHOOD_DESCRIPTIONS = {
    Neighbourhood.EXTENDED: "Extended",
    Neighbourhood.PURE_FRONT: "Pure front advancing",
}
SEED_ORDERING_DESCRIPTIONS = {
    SeedsPool.BOUNDARY: "Boundary priority",
    SeedsPool.NEIGHBOURHOOD: "Neighbourhood priority",
    SeedsPool.BOUNDARY_POINT_INIT: "Boundary priority with point initialization",  # noqa: E501
    SeedsPool.NEIGHBOURHOOD_POINT_INIT: "Neighbourhood priority with point initialization",  # noqa: E501
}
CELL_COUPLING_DESCRIPTIONS = {
    CellCoupling.MAX_WEIGHT: "Maximum edge weight",
    CellCoupling.MIN_DISTANCE: "Minimum distance between cell centers",
}


def compute_neighbourhood_wall_distance(
    neigh_idxs: npt.ArrayLike, neighs: npt.ArrayLike, wall: npt.ArrayLike
) -> npt.NDArray[int]:
    """For each cell, computes its distance from a boundary as neighbourhood order.
    That is, a cell at a boundary has distance equal to 0, its neighbours 1, the
    neighbours of its neighbours 2, and so on. In a Cartesian grid this is the Manhattan
    norm with respect to the closest boundary cell.

    It uses a BFS A* algorithm to visit all the cells starting from the borders and
    keeping track of the distance walked so far.

    Parameters
    ----------
    neigh_idxs : array-like
        Indices used to recover the neighbours of each cells provided in `neighs`. The
        length is `N_cells + 1`
    neighs : array-like
        Neighbours of each cell.
    wall : array-like
        IDs of the cells composing the wall from which we compute the distance.

    Returns
    -------
    :obj: `np.ndarray` of `int`
        Wall distance

    Notes
    -----
    It might be quite time consuming especially with large sets.

    Sometimes np raises errors when indexing with np.uint64 :shrug:, hence ensuring
    simple int only."""
    # Sanity checks
    neigh_idxs = np.asarray(neigh_idxs)
    if len(neigh_idxs.shape) > 1:
        raise ValueError("`neigh_idxs` - Expecting 1D array")
    neighs = np.asarray(neighs)
    if len(neighs.shape) > 1:
        raise ValueError("`neighs` - Expecting 1D array")
    # Sometimes np raises errors when indexing with np.uint64 :shrug:, hence ensuring
    # int only
    wall = np.asarray(wall, dtype=int)
    if len(wall.shape) > 1:
        raise ValueError("`wall` - Expecting 1D array")

    n_cells = neigh_idxs.shape[0] - 1

    dist = np.full(n_cells, -1, dtype=int)
    dist[wall] = 0
    to_visit = wall.tolist()
    while to_visit:
        # Always start from the min, otherwise counting might be bad if we come to a
        # cell by two different paths
        # Two ways:
        # 1. One-liner:
        # idx = min(range(len(to_visit)), key = lambda i: dist[to_visit[i]])
        # ... or even better using numpy
        # idx = np.argmin(dist[to_visit])
        # 2. Loop but better readable
        # idx = 0
        # for i, tt in enumerate(to_visit):
        # if dist[tt] < dist[idx]:
        # idx = i
        # Actually, in the end its always the first...
        cur = to_visit.pop(0)
        next_tag = dist[cur] + 1
        for neigh in np.nditer(
            neighs[neigh_idxs[cur] : neigh_idxs[cur + 1]], op_flags=["readonly"]
        ):
            if dist[neigh] < 0:
                # Not visited: set dist and add it to queue
                dist[neigh] = next_tag
                to_visit.append(int(neigh))
            elif dist[neigh] > next_tag:
                dist[neigh] = next_tag

    if dist.min() < 0:
        from warnings import warn

        warn(
            "Some cells have not been visited, possibly the graph is not connected."
            " The distance for these cells are negative"
        )

    return dist


def dump_graph(
    filename: str,
    CSR_row: npt.ArrayLike,
    CSR_col: npt.ArrayLike,
    CSR_val: npt.ArrayLike,
    volumes: npt.ArrayLike,
    n_bnd_faces: npt.ArrayLike,
    centers: npt.ArrayLike,
    precision: int = 15,
    integer_format: typing.Optional[str] = None,
    float_format: typing.Optional[str] = None,
) -> None:
    """Dump a graph in a format readable by CoMMA `test_util.h`

    Parameters
    ----------
    filename : str
        File where to dump the data
    CSR_row : array-like
        The row pointer of the CRS representation
    CSR_col : array-like
        The column index of the CRS representation
    CSR_val : array-like
        The values of the CRS representation
    volumes : array-like
        The volumes of the cells
    n_bnd_faces : array-like
        Vector telling how many boundary faces each cell has
    centers : array-like
        Cell centers
    float_precision : int, default: 15
        Precision for float numbers
    integer_format : str or None, default None
        Format for integer numbers
    float_format : str or None, default None
        Format for float numbers"""
    with open(filename, "w") as outfile:
        args = {"sep": ","}
        int_args, float_args = args.copy(), args.copy()
        if integer_format is not None:
            int_args["format"] = integer_format
        if float_format is not None:
            float_args["format"] = float_format
        cnt_ar = np.asarray(centers)
        outfile.write(f"{cnt_ar.shape[1]}\n")
        np.asarray(CSR_row).tofile(outfile, **int_args)
        outfile.write("\n")
        np.asarray(CSR_col).tofile(outfile, **int_args)
        outfile.write("\n")
        np.asarray(CSR_val).round(precision).tofile(outfile, **float_args)
        outfile.write("\n")
        np.asarray(volumes).round(precision).tofile(outfile, **float_args)
        outfile.write("\n")
        np.asarray(n_bnd_faces).round(precision).tofile(outfile, **int_args)
        outfile.write("\n")
        cnt_ar.round(precision).tofile(outfile, **float_args)
        outfile.write("\n")


def build_coarse_graph(
    fc2cc: npt.ArrayLike,
    CSR_row: npt.ArrayLike,
    CSR_col: npt.ArrayLike,
    CSR_val: npt.ArrayLike,
    volumes: npt.ArrayLike,
    n_bnd: npt.ArrayLike,
    centers: npt.ArrayLike,
) -> typing.Tuple[
    npt.NDArray[int],
    npt.NDArray[int],
    npt.NDArray[float],
    npt.NDArray[float],
    npt.NDArray[int],
    npt.NDArray[float],
]:
    """Starting from the description of a fine mesh / graph and the result of an
    agglomeration (`fc2cc`), build the description of the coarse graph

    Parameters
    ----------
    fc2cc : array-like
        Result of an agglomeration telling giving the relation FC to CC
    CSR_row : array-like
        The row pointer of the CSR representation of the fine graph
    CSR_col : array-like
        The column index of the CSR representation of the fine graph
    CSR_val : array-like
        The values of the CSR representation of the fine graph
    volumes : array-like
        The volumes of the cells of the fine graph
    n_bnd_faces : array-like
        Vector telling how many boundary faces each cell of the fine graph has
    centers : array-like
        Cell centers of the fine graph

    Returns
    -------
    :obj:`np.ndarray` of `int`
        The row pointer of the CSR representation of the coarse graph
    :obj:`np.ndarray` of `int`
        The column index of the CSR representation of the coarse graph
    :obj:`np.ndarray` of `float`
        The values of the CSR representation of the coarse graph
    :obj:`np.ndarray` of `float`
        The volumes of the cells of the coarse graph
    :obj:`np.ndarray` of `int`
        Vector telling how many boundary faces each cell of the coarse graph has
    :obj:`np.ndarray` of `float`
        Cell centers of the coarse graph (weighted average of the fine one)"""
    fc2cc = np.asarray(fc2cc, dtype=int)
    CSR_row = np.asarray(CSR_row, dtype=int)
    CSR_col = np.asarray(CSR_col, dtype=int)
    CSR_val = np.asarray(CSR_val, dtype=float)
    volumes = np.asarray(volumes, dtype=float)
    n_bnd = np.asarray(n_bnd, dtype=int)
    centers = np.asarray(centers, dtype=float)

    # This is possibly not the most efficient way to do the following, but it works and
    # it is enough for now
    nb_cc = fc2cc.max() + 1
    coarse_n_bnd = np.empty(nb_cc, dtype=int)
    coarse_volumes = np.empty(nb_cc, dtype=float)
    coarse_centers = np.empty((nb_cc, centers.shape[-1]), dtype=float)
    coarse_CSR_row = [0]
    coarse_CSR_col = []
    coarse_CSR_val = []
    for cc in range(nb_cc):
        mask_fc = fc2cc == cc
        assert mask_fc.any()
        # (Previous) fine cells in current coarse cells
        fcs = np.flatnonzero(mask_fc)
        coarse_n_bnd[cc] = np.max(n_bnd[mask_fc])
        coarse_volumes[cc] = np.sum(volumes[mask_fc])
        # This is not actually very good, but still...
        coarse_centers[cc, :] = np.average(
            centers[mask_fc, :], axis=0, weights=volumes[mask_fc]
        )
        neighs_cc = {}
        # for every (previous) fine cell composing the current coarse cell...
        for fc in fcs:
            # ...loop over its neighbours
            for neigh_fc in CSR_col[CSR_row[fc] : CSR_row[fc + 1]]:
                # ...and if they are not in the same coarse cell...
                if neigh_fc not in fcs:
                    # ...update the interface with the coarse cell to which the
                    # neighbours belongs
                    neigh_cc = fc2cc[neigh_fc]
                    neighs_cc[neigh_cc] = (
                        neighs_cc.get(neigh_cc, 0.0) + CSR_val[neigh_fc]
                    )
        coarse_CSR_col.extend(neighs_cc.keys())
        coarse_CSR_val.extend(neighs_cc.values())
        coarse_CSR_row.append(coarse_CSR_row[-1] + len(neighs_cc))
    return (
        np.array(coarse_CSR_row, dtype=int),
        np.array(coarse_CSR_col, dtype=int),
        np.array(coarse_CSR_val, dtype=float),
        coarse_volumes,
        coarse_n_bnd,
        coarse_centers,
    )


def prepare_meshio_celldata(data, cellblocks):
    """Prepare data so that it can be forwarded to `Meshio` as cell data.

    data: array like
        Data.
    cellblocks: list of `Meshio` `CellBlock`s
        Cell-blocks of which the `Meshio` mesh is composed.
    """
    if len(cellblocks) < 2:
        return [data]
    ret = []
    # More than one element type -> Adapt cell_data to meshio cellblock format
    start = 0
    for cells_by_type in cellblocks:
        n_cells = cells_by_type.data.shape[0]
        ret.append(data[start : start + n_cells])
        start += n_cells
    return ret


def prepare_meshio_agglomeration_data(
    data, cellblocks, modulo_renumbering=None, shuffle=False
):
    """Prepare data coming from the algorithm so that it can be forwarded to `Meshio` as
    cell data. If requested, the data is shuffled and/or modified by taking a modulo
    operation: both these lasts two features may result in better visualization.

    data: array like
        Data.
    cellblocks: list of `Meshio` `CellBlock`s
        Cell-blocks of which the `Meshio` mesh is composed.
    modulo_renumbering: optional, None or int
        Value of the modulo operation applied to the data.
    shuffle: bool, default: False
        Whether to shuffle the data before exporting.
    """
    final_data = np.asarray(data)
    if shuffle:
        n_cells = final_data.max() + 1
        new_data = np.arange(n_cells)
        np.random.default_rng().shuffle(new_data)
        for i in range(final_data.shape[0]):
            final_data[i] = new_data[final_data[i]]
    if modulo_renumbering is not None and modulo_renumbering > 1:
        # fine_cells_renum = (
        #     dGut.address_agglomerated_cells(fc_to_cc_res, renumber_coarse)
        #     if renum
        #     else fc_to_res
        # )
        # As long as the data is composed of (integer) IDs, the following is equivalent
        # but much faster
        final_data %= modulo_renumbering
    return prepare_meshio_celldata(final_data, cellblocks)


def assign_anisotropic_line_data_to_cells(
    idxs, cells, fc2cc, default_value=-1, modulo_renumbering=None, shuffle=False
):
    """Given the CSR definition of the anisotropic lines returned by `CoMMA`, give to
    each (fine) cell the ID of anistotropic line to which it belongs, otherwise a
    default values. Since the line definition returned by CoMMA is is terms of coarse
    cells, knowledge to relate the fine cells to the coarse ones is needed, hence
    `fc2cc`. It returns a (`numpy`) vector of the same length as `fc2cc` where the value
    at position `i` is the ID of the anisotropic line to which cell `i` belongs. If cell
    `i` does not belong to any line, the given default value is used. The line IDs:
    * are positive and 0-based if the default value is negative
    * are positive and 1-based if the default value is 0
    * are negative if the default value is positive

    idxs: array like
        Indices for lines.
    cells: array like
        Flat array containing the all the cells of belonging to the anisotropic lines.
    fc2cc: array like
        Result of the agglomeration, ID of the coarse cell to which a fine cell belongs.
    default_value: int, default = -1
        Value to assign to cells which do not belong to any line
    modulo_renumbering: optional, None or int
        Value of the modulo operation applied to the data.
    shuffle: bool, default: False
        Whether to shuffle the ID
    """
    n_lines = len(idxs) - 1
    lines = np.arange(n_lines)
    f2c = np.asarray(fc2cc)
    ret = np.full_like(f2c, default_value, dtype=int)
    if shuffle:
        np.random.default_rng().shuffle(lines)
    if modulo_renumbering is not None and modulo_renumbering > 1:
        lines %= modulo_renumbering
    if default_value == 0:
        lines += 1
    elif default_value > 0:
        lines = -lines
    for ln in range(n_lines):
        # With the following commented definitions, the actual code would me more
        # readable:
        # coarse_cells_in_line = cells[idxs[ln] : idxs[ln + 1]]
        # mask_fine_cells_in_line = np.isin(f2c, coarse_cells_in_line)
        ret[np.isin(f2c, cells[idxs[ln] : idxs[ln + 1]])] = lines[ln]
    return ret
