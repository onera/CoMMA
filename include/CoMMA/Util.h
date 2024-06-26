#ifndef COMMA_PROJECT_UTIL_H
#define COMMA_PROJECT_UTIL_H

/*
 * CoMMA
 *
 * Copyright © 2024 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iterator>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "CoMMA/Args.h"

namespace comma {

/** @def CoMMAUnused(var)
 *  @brief Convenient function to avoid unused warnings
 */
#define CoMMAUnused(var) (void)(var)

/** @brief Threshold used in combination with a dot product to tell whether two
 * vector deviate. It is set to \f$ \cos(20^\circ) \f$.
 */
constexpr double deviate_thresh = 0.9396926207859084;

/** @brief Square
 * @tparam T The type of the quantity
 * @param[in] x The quantity
 * @return the square of \p x
 */
template<typename T>
inline T constexpr _sq(const T x) {
  return x * x;
}

/** @brief Cube
 * @tparam T The type of the quantity
 * @param[in] x The quantity
 * @return the cube of \p x
 */
template<typename T>
inline T constexpr _cb(const T x) {
  return x * x * x;
}

/** @brief Raise a quantity to an integer power
 * @tparam p The power
 * @tparam T The type of the quantity
 * @param[in] x The quantity
 * @return \f$ x^p \f$
 */
template<unsigned int p, typename T>
inline T constexpr int_power(const T x) {
  // See https://stackoverflow.com/a/1505791/12152457
  if constexpr (p == 0) return 1;
  if constexpr (p == 1) return x;
  if constexpr (p == 2) return x * x;
  if constexpr (p == 3) return x * x * x;

  constexpr int tmp = int_power<p / 2>(x);
  if constexpr ((p % 2) == 0) {
    return tmp * tmp;
  } else {
    return x * tmp * tmp;
  }
}

/** @brief Tell whether the dot product given as input comes from two parallel
 * vectors. Compared against \ref deviate_thresh.
 * @tparam T Input type
 * @param[in] dot Dot product
 * @return true if higher than a reference threshold
 */
template<typename T>
inline bool constexpr dot_deviate(const T dot) {
  return fabs(dot) < decltype(fabs(dot)){deviate_thresh};
}

/** @brief Compute the dot product between two vectors. No check on size is
 * performed
 * @tparam T Input type
 * @param[in] a First vector
 * @param[in] b Second vector
 * @return the dot product
 */
template<typename T>
inline T dot_product(const std::vector<T> &a, const std::vector<T> &b) {
#if 0
  const CoMMAWeightType dot = inner_product(
      prev_dir.begin(), prev_dir.end(), cur_dir.begin(),
      CoMMAWeightType{0.});
#endif
  T dot{0.};
  for (auto i = decltype(a.size()){0}; i < a.size(); ++i)
    dot += a[i] * b[i];
  return dot;
}

/** @brief Compute the direction from point \p a to point \p b and store it as
 * unit vector in \p dir.
 * @tparam T Input type
 * @param[in] a Starting point
 * @param[in] b End point
 * @param[out] dir Unit vector of the direction
 * @return the distance from the two points (the norm used for the
 * normalization)
 */
template<typename T>
inline T get_direction(
  const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &dir
) {
  T norm{0.};
  for (auto i = decltype(a.size()){0}; i < a.size(); ++i) {
    const T di = b[i] - a[i];
    dir[i] = di;
    norm += di * di;
  }
  norm = sqrt(norm);
  const T ov_norm = T{1.} / norm;
  for (auto &di : dir)
    di *= ov_norm;
  return norm;
}

/** @brief Compute the squared Euclidean distance between two points seen as
 * vectors. We use vectors because we can have both 2- and 3D points (also we
 * can have 3D points even if the dimension given to CoMMA is 2D, e.g., with
 * CODA pseudo-2D meshes). The dimension used as reference is the one of the
 * first point
 *  @tparam T Type for real numbers
 *  @param[in] a First point
 *  @param[in] b Second point
 *  @return The squared Euclidean distance between the two points
 */
template<typename T>
inline T squared_euclidean_distance(
  const std::vector<T> &a, const std::vector<T> &b
) {
#if 0
  return
      transform_reduce(a.cbegin(), a.cend(), b.cbegin(), T{0.},
                       [](const auto sum, const auto val){return sum + val*val;},
                       minus<T>());
#endif
  T dist{0.};
  for (auto i = decltype(a.size()){0}; i < a.size(); ++i) {
    const auto diff = a[i] - b[i];
    dist += diff * diff;
  }
  return dist;
}

/** @brief Functor for pairs implementing a custom 'less than'. It relies on the
 * 'less than' operator for the second elements and 'greater than' for the first
 * ones.
 *  @tparam PairT Type for pairs
 */
template<class PairT>
struct CustomPairLessFunctor {
  /** @brief Functor for pairs implementing a custom 'less than'. It relies on
   * the 'less than' operator for the second elements and 'greater than' for the
   * first ones.
   *  @param[in] left pair
   *  @param[in] right Second pair
   *  @return true if left.second < right.second or (left.second == right.second
   * and left.first > right.first); false otherwise
   */
  inline bool operator()(const PairT &left, const PairT &right) const {
    if (left.second < right.second) return true;
    if (left.second > right.second) return false;
    /* left.second == right.second */
    return left.first > right.first;
  }
};

/** @brief Functor for pairs implementing a custom 'greater than'. It relies on
 * the 'greater than' operator for the second elements and 'less than' for the
 * first ones.
 *  @tparam PairT Type for pairs
 */
template<typename PairT>
struct CustomPairGreaterFunctor {
  /** @brief Functor for pairs implementing a custom 'greater than'. It relies
   * on the 'greater than' operator for the second elements and 'less than' for
   * the first ones.
   *  @param[in] left First pair
   *  @param[in] right Second pair
   *  @return true if left.second > right.second or (left.second == right.second
   * and left.first < right.first); false otherwise
   */
  inline bool operator()(const PairT &left, const PairT &right) const {
    // I tried this: return !CustomPairLessFunctor<PairT>()(left,right);
    // but it didn't work well for equality
    if (left.second > right.second) return true;
    if (left.second < right.second) return false;
    /* left.second == right.second */
    return left.first < right.first;
  }
};

/** @brief Functor for pairs implementing a less operator based only on the
 * second element of the pair.
 *  @tparam PairT Type for pairs
 */
template<typename PairT>
struct PairSecondBasedLessFunctor {
  /** @brief Functor for pairs implementing a less operator based only on the
   * second element of the pair
   *  @param[in] left First pair
   *  @param[in] right Second pair
   *  @return true if left.second < right.second; false otherwise
   */
  inline bool operator()(const PairT &left, const PairT &right) const {
    return left.second < right.second;
  }
};

/** @brief Given a container of pairs, return a vector with first elements only
 *  @tparam CoMMAContainerPairType Type of the input container
 *  @param[in] cont A container of pairs from which the first elements will be
 * extracted
 *  @return The first elements of each pair
 */
template<typename CoMMAContainerPairType>
inline std::vector<typename CoMMAContainerPairType::value_type::first_type>
vector_of_first_elements(const CoMMAContainerPairType &cont) {
  std::vector<typename CoMMAContainerPairType::value_type::first_type> fe;
  fe.reserve(cont.size());
  for (const auto &pr : cont)
    fe.emplace_back(pr.first);
  return fe;
}

/** @brief Functor implementing an operator telling if a given value if the
 * first one of pair
 *  @tparam PairT Type for pairs
 */
template<typename PairT>
class PairFindFirstBasedFunctor {
public:
  /** @brief Constructor */
  PairFindFirstBasedFunctor() :
    _target(){};
  /** @brief Constructor
   *  @param target Reference value that will be sought
   */
  // NOLINTNEXTLINE
  PairFindFirstBasedFunctor(const typename PairT::first_type &target) :
    _target(target){};
  /** @brief Destructor */
  ~PairFindFirstBasedFunctor() = default;

  /** @brief Operator telling if the first value of the given pair is equal to
   * the reference one
   *  @param[in] pr The pair
   *  @return a bool
   */
  inline bool operator()(const PairT &pr) const { return pr.first == _target; }

private:
  /** @brief Reference value to be sought */
  typename PairT::first_type _target;
};

/** @brief Utility function for creating a set out of the keys of a map
 *  @tparam KeyT Type of the keys of the map
 *  @tparam ValueT Type of the values of the map
 *  @param[in] dict A map
 *  @return a set
 */
template<typename KeyT, typename ValueT>
inline std::unordered_set<KeyT> d_keys_to_set(
  const std::unordered_map<KeyT, ValueT> &dict
) {
  std::unordered_set<KeyT> s_neighbours_of_seed = {};
  for (const auto &i_k_v : dict) {
    s_neighbours_of_seed.insert(i_k_v.first);
  }
  return s_neighbours_of_seed;
}

/** @brief Given the connectivity of the graph, filter cells keeping only those
 * with the desired number of edges / neighbours.
 * @tparam IndexT Type for indices.
 * @tparam IntT Type for integers.
 * @param[in] indices Indices of the connectivity of the graph.
 * @param[in] n_bnd_faces Number of boundary faces per cell.
 * @param[in] allowed Set with the accepted number of edges.
 * @param[out] filtered Filtered cells.
 */
template<typename IndexT, typename IntT>
inline void filter_cells_by_n_edges(
  const std::vector<IndexT> &indices,
  const std::vector<IntT> &n_bnd_faces,
  const std::unordered_set<IntT> &allowed,
  std::vector<IndexT> &filtered
) {
  filtered.clear();
  auto target = decltype(allowed){};
  std::copy_if(
    allowed.begin(),
    allowed.end(),
    std::inserter(target, target.begin()),
    [](const auto n) { return n > 0; }
  );
  if (!target.empty()) {
    const auto n_cells = static_cast<IndexT>(n_bnd_faces.size());
    filtered.reserve(n_cells);
    for (auto c = decltype(n_cells){0}; c < n_cells; ++c) {
      if (allowed.find(
            static_cast<IntT>(indices[c + 1] - indices[c] + n_bnd_faces[c])
          )
          != allowed.end())
        filtered.emplace_back(c);
    }
    filtered.shrink_to_fit();
  }
}

/** @brief Compute a neighbourhood-base wall-distance, that is, the distance of
 * a given cell from a wall is the number of cells though which the minimum path
 * starting from the cell and ending at the wall. For example, in a Cartesian
 * grids this is equivalent to the minimum of the Manhattan distance.\n If the
 * vector defining the wall is empty, return negative values.\n If a cell is
 * unconnected to the domain with the wall, its distance will be negative.\n It
 * takes a compressed version of the connectivity of the mesh. It uses a BFS
 * algorithm to visit all the cells.
 * @tparam IndexT Type for cell indices
 * @tparam DistT Type for distance (should be signed)
 * @param[in] neigh_idxs Indices used to recover the neighbours of each cells
 * provided in \p neighs. The length is \f$ N_{cells} + 1 \f$
 * @param[in] neighs Neighbours of the cells
 * @param[in] wall Cells composing the wall from which the distance is computed
 * @param[out] dist Distance from the wall. This vector is resized inside the
 * function to hold all the cells
 * @warning This function is \b experimental. Moreover, since CoMMA has
 * knowledge of the current domain only, this function might not give the right
 * result if the domain has been partitioned. It is advised to use this function
 * only when considering one domain only.
 */
template<typename IndexT, typename DistT>
void compute_neighbourhood_based_wall_distance(
  const std::vector<IndexT> &neigh_idxs,
  const std::vector<IndexT> &neighs,
  const std::vector<IndexT> &wall,
  std::vector<DistT> &dist
) {
  static_assert(
    std::is_signed<DistT>::value,
    "The distance type should be signed to allow flags (negative values)"
  );
  dist.resize(neigh_idxs.size() - 1);
  std::fill(dist.begin(), dist.end(), DistT{-1});
  std::queue<IndexT> to_visit{};
  for (const auto &cell : wall) {
    dist[cell] = DistT{0};
    to_visit.emplace(cell);
  }
  while (!to_visit.empty()) {
    // If you get wrong results, this could might be to the fact that one should
    // consider not specially the first of the queue, but the one with lowest
    // distance visited so far. So, the definition of `cur` below should be
    // changed, and with that, another container should be used instead of a
    // queue, since one might pop from whenever and not only the front (a deque
    // should work).
    const auto cur = to_visit.front();
    to_visit.pop();
    const auto cur_dist = dist[cur] + DistT{1};
    for (auto neigh = neighs.cbegin() + neigh_idxs[cur];
         neigh < neighs.cbegin() + neigh_idxs[cur + 1];
         ++neigh) {
      if (dist[*neigh] < DistT{0}) {
        dist[*neigh] = cur_dist;
        to_visit.emplace(*neigh);
      } else if (dist[*neigh] > cur_dist) {
        dist[*neigh] = cur_dist;
      }
    }
  }
}

/** @brief Build a coarse graph from the fine graph and the result of a
 * previous agglomeration.
 * @tparam IndexT Type for indices
 * @tparam RealT Type for reals
 * @tparam IntT Type for integers
 * @param[in] f2c Result of the previous agglomeration
 * @param[in] f_adj_idx Adjacency indices of the fine graph
 * @param[in] f_adj Adjacency of the fine graph
 * @param[in] f_weights Adjacency weights of the fine graph
 * @param[in] f_volumes Volumes of the previous graph
 * @param[in] f_centers Centers of the cells of the fine graph
 * @param[in] f_priority Priorities of the cells of the fine graph
 * @param[in] f_n_bnd Number of boundary faces of the cells of the fine graph
 * @param[out] c_adj_idx Adjacency indices of the coarse graph
 * @param[out] c_adj Adjacency of the coarse graph
 * @param[out] c_weights Adjacency weights of the coarse graph
 * @param[out] c_volumes Volumes of the previous graph
 * @param[out] c_centers Centers of the cells of the coarse graph
 * @param[out] c_priority Priorities of the cells of the coarse graph (computed
 * as maximum of the priorities of the fine cells)
 * @param[out] c_n_bnd Number of boundary faces of the cells of the coarse graph
 */
template<typename IndexT, typename RealT, typename IntT>
void build_coarse_graph(
  const std::vector<IndexT> &f2c,
  const std::vector<IndexT> &f_adj_idx,
  const std::vector<IndexT> &f_adj,
  const std::vector<RealT> &f_weights,
  const std::vector<RealT> &f_volumes,
  const std::vector<std::vector<RealT>> &f_centers,
  const std::vector<RealT> &f_priority,
  const std::vector<IntT> &f_n_bnd,
  // output
  std::vector<IndexT> &c_adj_idx,
  std::vector<IndexT> &c_adj,
  std::vector<RealT> &c_weights,
  std::vector<RealT> &c_volumes,
  std::vector<std::vector<RealT>> &c_centers,
  std::vector<RealT> &c_priority,
  std::vector<IntT> &c_n_bnd
) {
  const auto n_cells = *(std::max_element(f2c.begin(), f2c.end())) + 1;
  const auto dim = static_cast<IntT>(f_centers[0].size());
  // Preparing outputs
  c_adj_idx.clear();
  c_adj_idx.emplace_back(0);
  c_adj.clear();
  c_weights.clear();
  // For these, we have the final size
  c_volumes.clear();
  c_volumes.resize(n_cells);
  std::fill(c_volumes.begin(), c_volumes.end(), RealT{0});
  c_n_bnd.clear();
  c_n_bnd.resize(n_cells);
  c_priority.clear();
  c_priority.resize(n_cells);
  std::fill(c_n_bnd.begin(), c_n_bnd.end(), IntT{0});
  c_centers.clear();
  c_centers.reserve(n_cells);
  for (auto cc = decltype(n_cells){0}; cc < n_cells; ++cc) {
    // https://stackoverflow.com/questions/18189362/how-best-to-fill-a-vector-of-vectors-avoiding-wasting-memory-and-unnecessary-al
    auto tmp = std::vector<RealT>(dim, RealT{0});
    c_centers.emplace_back(std::move(tmp));
  }  // for cc
  // Now building
  // For each coarse cell, find its fine cells
  std::vector<std::set<IndexT>> c2f(n_cells);
  for (auto fc = decltype(f2c.size()){0}; fc < f2c.size(); ++fc) {
    const auto cc = f2c[fc];
    // Adding FC to CC
    c2f[cc].emplace(fc);
    // Adding volume
    c_volumes[cc] += f_volumes[fc];
    // Updating center - Weighted average
    for (auto xyz = decltype(dim){0}; xyz < dim; ++xyz)
      c_centers[cc][xyz] += f_volumes[fc] * f_centers[fc][xyz];
  }  // for fc
  for (auto cc = decltype(n_cells){0}; cc < n_cells; ++cc) {
    const auto &fcs = c2f[cc];
    const RealT ov_n_fc(RealT{1.} / fcs.size());
    // Finishing center
    for (auto xyz = decltype(dim){0}; xyz < dim; ++xyz)
      c_centers[cc][xyz] *= ov_n_fc;
    auto n_bnd = std::numeric_limits<IntT>::min();
    auto priority = std::numeric_limits<RealT>::min();
    std::map<IndexT, RealT> cc_neighs;
    for (const auto fc : fcs) {
      if (f_n_bnd[fc] > n_bnd) n_bnd = f_n_bnd[fc];
      if (f_priority[fc] > priority) priority = f_priority[fc];
      for (auto fc_neigh = f_adj.cbegin() + f_adj_idx[fc];
           fc_neigh != f_adj.cbegin() + f_adj_idx[fc + 1];
           ++fc_neigh) {
        // Update if not one of the FC of the CC
        if (fcs.find(*fc_neigh) == fcs.end()) {
          const auto cc_neigh = f2c[*fc_neigh];
          // Add if not yet present
          cc_neighs.try_emplace(cc_neigh, 0.0);
          cc_neighs.at(cc_neigh) += f_weights[*fc_neigh];
        }
      }  // for fc_neigh
    }  // for fc
    // Max of f_bnd
    c_n_bnd[cc] = n_bnd;
    c_priority[cc] = priority;
    c_adj_idx.emplace_back(c_adj_idx.back() + cc_neighs.size());
    for (const auto &[neigh, weight] : cc_neighs) {
      c_adj.emplace_back(neigh);
      c_weights.emplace_back(weight);
    }
  }  // for cc
}

}  // end namespace comma

#endif  // COMMA_PROJECT_UTIL_H
