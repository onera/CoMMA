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
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

#define CoMMAUnused(var) (void)(var)

/** @brief Functor for pairs implementing a custom 'less than'. It relies on the
 * 'less than' operator for the second elements and 'greater than' for the first ones.
 *  @tparam CoMMAPairType type of pair
 *  @param[in] p First pair
 *  @param[in] q Second pair
 *  @return true if p.second < q.second or (p.second == q.second and p.first > q.first);
 * false otherwise
 */
template<class CoMMAPairType>
struct CustomPairLessFunctor {
  inline bool operator() (CoMMAPairType p, CoMMAPairType q) const {
    if      (p.second < q.second)   return true;
    else if (p.second > q.second)   return false;
    else /* p.second == q.second */ return p.first > q.first;
  }
};

/** @brief Functor for pairs implementing a custom 'greater than'. It relies on
 * CustomPairLessFunctor, indeed it just returns its negation
 *  @tparam CoMMAPairType type of pair
 *  @param[in] p First pair
 *  @param[in] q Second pair
 *  @return Negation of CustomPairLessFunctor(p,q)
 */
template<typename CoMMAPairType>
struct CustomPairGreaterFunctor {
  inline bool operator() (CoMMAPairType p, CoMMAPairType q) const {
    return !CustomPairLessFunctor<CoMMAPairType>()(p,q);
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
 *  @tparam CoMMAPairType Type of pair
 */
template<typename CoMMAPairType>
class PairFindFirstBasedFunctor {
  public:
  /** @brief Constructor */
  PairFindFirstBasedFunctor() : _target() {};
  /** @brief Constructor
   *  @param target Reference value that will be sought
   */
  PairFindFirstBasedFunctor(const typename CoMMAPairType::first_type &target) :
    _target(target) {};
  /** @brief Destructor */
  ~PairFindFirstBasedFunctor() {};
  /** @brief Operator telling if the first value of the given pair is equal to
   * the reference one
   *  @param[in] p The pair
   *  @return a bool
   */
  inline bool operator() (const CoMMAPairType &p) const {return p.first == _target;}
  private:
  /** @brief Reference value to be sought */
  typename CoMMAPairType::first_type _target;
};

/** @brief Utility function for creating a set out of the keys of a map
 *  @tparam KeyType Type of the keys of the map
 *  @tparam ValueType Type of the values of the map
 *  @param[in] dict A map
 *  @return a set
 */
template <typename KeyType, typename ValueType>
inline unordered_set<KeyType> d_keys_to_set(
    const unordered_map<KeyType, ValueType> &dict) {
  unordered_set<KeyType> s_neighbours_of_seed = {};
  for (const auto &i_k_v : dict) {
    s_neighbours_of_seed.insert(i_k_v.first);
  }
  return s_neighbours_of_seed;
}

#endif  // COMMA_PROJECT_UTIL_H
