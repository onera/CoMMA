#ifndef COMMA_PROJECT_UTIL_H
#define COMMA_PROJECT_UTIL_H

/*
 *  * CoMMA
    * Copyright © 2022 ONERA
    *
    * Authors and Architects : Alberto Remigi and Nicolas Lantos
    *
    * Contributors : Riccardo Milani
    *
    * This program is free software: you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation, either version 3 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
    * You should have received a copy of the GNU General Public License
    * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

#define CoMMAUnused(var) (void)(var)

template <typename CoMMAIndexType, typename CoMMAIntType>
inline unordered_set<CoMMAIndexType> d_keys_to_set(
    const unordered_map<CoMMAIndexType, CoMMAIntType> &dict) {
  unordered_set<CoMMAIndexType> s_neighbours_of_seed = {};
  for (const auto &i_k_v : dict) {
    s_neighbours_of_seed.insert(i_k_v.first);
  }
  return s_neighbours_of_seed;
}

/** @brief Compute the Euclidean distance between two points seen as vectors. We use
 * vectors because we can have both 2- and 3D points (also we can have 3D points even
 * if the dimension given to CoMMA is 2D, e.g., with CODA pseudo-2D meshes). The
 * dimension used as reference is the one of the first point
 *  @tparam CoMMAWeightType Type for real numbers
 *  @param[in] a First point
 *  @param[in] b Second point
 *  @return The Euclidean distance between the two points
 */
template <typename CoMMAWeightType>
inline CoMMAWeightType euclidean_distance(const vector<CoMMAWeightType> a,
    const vector<CoMMAWeightType> b) {
  vector<CoMMAWeightType> diff;
  transform(a.begin(),a.end(), b.begin(), back_inserter(diff), minus<CoMMAWeightType>());
  return sqrt(inner_product(diff.begin(), diff.end(), diff.begin(), CoMMAWeightType{0.}));
}

#endif  // COMMA_PROJECT_UTIL_H
