#ifndef COMMA_PROJECT_UTIL_H
#define COMMA_PROJECT_UTIL_H

/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

/** @def CoMMAUnused(var)
 *  @brief Convenient function to avoid unused warnings
 */
#define CoMMAUnused(var) (void)(var)

/** @brief Threshold used in combination with a dot product to tell whether two
 * vector deviate. It is set to \f$ \cos(20^\circ) \f$.
 */
constexpr double deviate_thresh = 0.9396926207859084;

/** @brief Tell whether the dot product given as input comes from two parallel
 * vectors. Compared against \ref deviate_thresh.
 * @tparam T Input type
 * @param[in] dot Dot product
 * @return true if higher than a reference threshold
 */
template<typename T>
inline bool dot_deviate(const T dot) {
  return fabs(dot) < decltype(fabs(dot)){deviate_thresh};
}

/** @brief Compute the dot product between two vectors. No check on size is performed
 * @tparam T Input type
 * @param[in] a First vector
 * @param[in] b Second vector
 * @return the dot product
 */
template<typename T>
inline T dot_product(const vector<T> &a, const vector<T> &b) {
#if 0
  const CoMMAWeightType dot = inner_product(
      prev_dir.begin(), prev_dir.end(), cur_dir.begin(),
      CoMMAWeightType{0.});
#endif
  T dot{0.};
  for (auto i = decltype(a.size()){0}; i < a.size(); ++i)
    dot += a[i]*b[i];
  return dot;
}

/** @brief Compute the direction from point \p a to point \p b and store it as unit
 * vector in \p dir.
 * @tparam T Input type
 * @param[in] a Starting point
 * @param[in] b End point
 * @param[out] dir Unit vector of the direction
 * @return the distance from the two points (the norm used for the normalization)
 */
template<typename T>
inline T get_direction(const vector<T>& a, const vector<T>& b, vector<T>& dir) {
  T norm{0.};
  for (auto i = decltype(a.size()){0}; i < a.size(); ++i) {
    const T di = b[i] - a[i];
    dir[i] = di;
    norm += di*di;
  }
  norm = sqrt(norm);
  const T ov_norm = T{1.} / norm;
  for (auto& di : dir)
    di *= ov_norm;
  return norm;
}

/** @brief Compute the Euclidean distance between two points seen as vectors. We use
 * vectors because we can have both 2- and 3D points (also we can have 3D points even
 * if the dimension given to CoMMA is 2D, e.g., with CODA pseudo-2D meshes). The
 * dimension used as reference is the one of the first point
 *  @tparam T Type for real numbers
 *  @param[in] a First point
 *  @param[in] b Second point
 *  @return The Euclidean distance between the two points
 */
template <typename T>
inline T euclidean_distance(const vector<T> a, const vector<T> b) {
#if 0
  return sqrt(
      transform_reduce(a.cbegin(), a.cend(), b.cbegin(), T{0.},
                       [](const auto sum, const auto val){return sum + val*val;},
                       minus<T>())
      );
#endif
  T ret{0.};
  for (auto i = decltype(a.size()){0}; i < a.size(); ++i) {
    const auto d = a[i] - b[i];
    ret += d*d;
  }
  return sqrt(ret);
}

/** @brief Functor for pairs implementing a custom 'less than'. It relies on the
 * 'less than' operator for the second elements and 'greater than' for the first ones.
 *  @tparam PairT Type for pairs
 */
template<class PairT>
struct CustomPairLessFunctor {
  /** @brief Functor for pairs implementing a custom 'less than'. It relies on the
   * 'less than' operator for the second elements and 'greater than' for the first ones.
   *  @param[in] p First pair
   *  @param[in] q Second pair
   *  @return true if p.second < q.second or (p.second == q.second and p.first > q.first);
   * false otherwise
   */
  inline bool operator() (PairT p, PairT q) const {
    if      (p.second < q.second)   return true;
    else if (p.second > q.second)   return false;
    else /* p.second == q.second */ return p.first > q.first;
  }
};

/** @brief Functor for pairs implementing a custom 'greater than'. It relies on the
 * 'greater than' operator for the second elements and 'less than' for the first ones.
 *  @tparam PairT Type for pairs
 */
template<typename PairT>
struct CustomPairGreaterFunctor {
  /** @brief Functor for pairs implementing a custom 'greater than'. It relies on the
   * 'greater than' operator for the second elements and 'less than' for the first ones.
   *  @param[in] p First pair
   *  @param[in] q Second pair
   *  @return true if p.second > q.second or (p.second == q.second and p.first < q.first);
   * false otherwise
   */
  inline bool operator() (PairT p, PairT q) const {
    // I tried this: return !CustomPairLessFunctor<PairT>()(p,q);
    // but it didn't work well for equality
    if      (p.second > q.second)   return true;
    else if (p.second < q.second)   return false;
    else /* p.second == q.second */ return p.first < q.first;
  }
};

/** @brief Given a container of pairs, return a vector with first elements only
 *  @tparam CoMMAContainerPairType Type of the input container
 *  @param[in] c A container of pairs from which the first elements will be extracted
 *  @return The first elements of each pair
 */
template<typename CoMMAContainerPairType>
inline vector<typename CoMMAContainerPairType::value_type::first_type>
vector_of_first_elements(const CoMMAContainerPairType &c) {
  vector<typename CoMMAContainerPairType::value_type::first_type> fe;
  fe.reserve(c.size());
  for (const auto &p : c)
    fe.emplace_back(p.first);
  return fe;
}

/** @brief Functor implementing an operator telling if a given value if the first
 * one of pair
 *  @tparam PairT Type for pairs
 */
template<typename PairT>
class PairFindFirstBasedFunctor {
  public:
  /** @brief Constructor */
  PairFindFirstBasedFunctor() : _target() {};
  /** @brief Constructor
   *  @param target Reference value that will be sought
   */
  PairFindFirstBasedFunctor(const typename PairT::first_type &target) :
    _target(target) {};
  /** @brief Destructor */
  ~PairFindFirstBasedFunctor() {};
  /** @brief Operator telling if the first value of the given pair is equal to
   * the reference one
   *  @param[in] p The pair
   *  @return a bool
   */
  inline bool operator() (const PairT &p) const {return p.first == _target;}
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
template <typename KeyT, typename ValueT>
inline unordered_set<KeyT> d_keys_to_set(
    const unordered_map<KeyT, ValueT> &dict) {
  unordered_set<KeyT> s_neighbours_of_seed = {};
  for (const auto &i_k_v : dict) {
    s_neighbours_of_seed.insert(i_k_v.first);
  }
  return s_neighbours_of_seed;
}

/** @brief Compute a neighbourhood-base wall-distance, that is, the distance of a
 * given cell from a wall is the number of cells though which the minimum path
 * starting from the cell and ending at the wall. For example, in a Cartesian grids
 * this is equivalent to the minimum of the Manhattan distance.\n
 * If the vector defining the wall is empty, return negative values.\n
 * If a cell is unconnected to the domain with the wall, its distance will be
 * negative.\n
 * It takes a compressed version of the connectivity of the mesh. It uses a BFS
 * algorithm to visit all the cells.
 * @tparam IndexT Type for cell indices
 * @tparam DistT Type for distance (should be signed)
 * @param[in] neigh_idxs Indices used to recover the neighbours of each cells
 * provided in \p neighs. The length is \f$ N_{cells} + 1 \f$
 * @param[in] neighs Neighbours of the cells
 * @param[in] wall Cells composing the wall from which the distance is computed
 * @param[out] dist Distance from the wall. This vector is resized inside the
 * function to hold all the cells
 * @warning This function is \b experimental. Moreover, since CoMMA has knoledge of
 * the current domain only, this function might not give the right result if the
 * domain hab been partitioned. It is advised to use this function only when
 * considering one domain only.
 */
template <typename IndexT, typename DistT>
void compute_neighbourhood_based_wall_distance(
    const vector<IndexT>& neigh_idxs,
    const vector<IndexT>& neighs,
    const vector<IndexT>& wall,
    vector<DistT>& dist) {
  static_assert(is_signed<DistT>::value,
                "The distance type should be signed to allow flags (negative values)");
  dist.resize(neigh_idxs.size() - 1);
  fill(dist.begin(), dist.end(), DistT{-1});
  queue<IndexT> to_visit{};
  for (const auto & cell : wall) {
    dist[cell] = DistT{0};
    to_visit.emplace(cell);
  }
  while (!to_visit.empty()) {
    // If you get wrong results, this could might be to the fact that one should
    // consider not specially the first of the queue, but the one with lowest
    // distance visited so far. So, the definition of `cur` below should be changed,
    // and with that, another container should be used instead of a queue, since one
    // might pop from whenever and not only the front (a deque should work).
    const auto cur = to_visit.front();
    to_visit.pop();
    const auto d = dist[cur] + DistT{1};
    for (auto neigh = neighs.cbegin() + neigh_idxs[cur];
         neigh < neighs.cbegin() + neigh_idxs[cur + 1];
         ++neigh) {
      if (dist[*neigh] < DistT{0}) {
        dist[*neigh] = d;
        to_visit.emplace(*neigh);
      }
      else if (dist[*neigh] > d) {
        dist[*neigh] = d;
      }
    }
  }
}

#endif  // COMMA_PROJECT_UTIL_H
