"""A collection of useful functions"""
import numpy as np
import typing
import numpy.typing as npt

def compute_neighbourhood_wall_distance(
        neigh_idxs : npt.ArrayLike,
        neighs     : npt.ArrayLike,
        wall       : npt.ArrayLike) \
    -> npt.NDArray[int]:
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
neighs: array-like
    Neighbours of each cell.
wall : array-like
    IDs of the cells composing the wall from which we compute the distance.

Returns
-------
:obj:`np.ndarray` of `int`
    Wall distance

Notes
-----
It might be quite time consuming especially with large sets.

Sometimes np raises errors when indexing with np.uint64 :shrug:, hence ensuring
simple int only.
"""
    # Sanity checks
    neigh_idxs = np.asarray(neigh_idxs)
    if len(neigh_idxs.shape) > 1:
        raise ValueError("`neigh_idxs` - Expecting 1D array")
    neighs = np.asarray(neighs)
    if len(neighs.shape) > 1:
        raise ValueError("`neighs` - Expecting 1D array")
    # Sometimes np raises errors when indexing with np.uint64 :shrug:, hence ensuring
    # int only
    wall = np.asarray(wall, dtype = int)
    if len(wall.shape) > 1:
        raise ValueError("`wall` - Expecting 1D array")

    n_cells = neigh_idxs.shape[0] - 1

    dist = np.full(n_cells, -1, dtype = int)
    dist[wall] = 0
    to_visit = wall.tolist()
    while (to_visit):
        # Always start from the min, otherwise counting might be bad if we come to a cell
        # by two different paths
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
        for neigh in np.nditer(neighs[neigh_idxs[cur]:neigh_idxs[cur+1]],
                               op_flags=['readonly']):
            if dist[neigh] < 0:
                # Not visited: set dist and add it to queue
                dist[neigh] = next_tag
                to_visit.append(int(neigh))
            elif dist[neigh] > next_tag:
                dist[neigh] = next_tag

    if dist.min() < 0:
        from warnings import warn
        warn("Some cells have not been visited, possibly the graph is not connected."
             " The distance for these cells are negative")

    return dist

def dump_graph(
        filename       : str,
        CSR_row        : npt.ArrayLike,
        CSR_col        : npt.ArrayLike,
        CSR_val        : npt.ArrayLike,
        volumes        : npt.ArrayLike,
        n_bnd_faces    : npt.ArrayLike,
        centers        : npt.ArrayLike,
        precision      : int = 15,
        integer_format : [str, None] = None,
        float_format   : [str, None] = None) \
    -> None:
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
    Format for float numbers
"""
    with open(filename, 'w') as outfile:
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
