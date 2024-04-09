#ifndef COMMA_PROJECT_PRIORITY_PAIR_H
#define COMMA_PROJECT_PRIORITY_PAIR_H

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

#include <utility>

namespace comma {

/** @brief Wrapper around the STL pair with custom 'less than' operator: as in
 * the standard case, first we compare the first elements, then the second ones;
 * however it relies on the 'greater than' on the first elements, e.g., (4,X) <
 * (3,Y), whereas standard rules apply to second elements, e.g., (4,3) < (4,4).
 * @tparam A Type of the first element of the pair
 * @tparam B Type of the second element of the pair
 * @deprecated Not used anymore
 */
template<typename A, typename B>
class Priority_Pair {
private:
  /** @brief STL pair around which current class is built */
  std::pair<A, B> _p;

public:
  /** @brief Constructor */
  Priority_Pair() = default;
  ;

  /** @brief Constructor
   *  @param a First element
   *  @param b Second element
   */
  Priority_Pair(const A &a, const B &b) :
    _p(a, b){};

  /** @brief Destructor */
  ~Priority_Pair() = default;
  ;

  /** @brief Accessor to the first element
   *  @return The first element
   */
  // We accept to have a copy passed
  A first() const { return _p.first; }

  /** @brief Accessor to the first element
   *  @return The second element
   */
  B second() const { return _p.second; }

  /** @brief Operator 'less than'. It actually relies on the 'greater than'
   * operator for the first elements and 'less than' for the second ones
   *  @param a First pair
   *  @param b Second pair
   *  @return true if a.first > b.first or (a.first == b.first and a.second <
   * b.second); false otherwise
   */
  inline friend bool operator<(const Priority_Pair &a, const Priority_Pair &b) {
    if (a._p.first > b._p.first) return true;
    if (a._p.first < b._p.first) return false;
    /* a._p.first == b._p.first */
    return a._p.second < b._p.second;
  }

  /** @brief Operator 'equal'
   *  @param a First pair
   *  @param b Second pair
   *  @return true if a and b are equal
   */
  inline friend bool operator==(
    const Priority_Pair &a, const Priority_Pair &b
  ) {
    return (a._p.first == b._p.first) && (a._p.second == b._p.second);
  }
};

}  // end namespace comma

#endif  // COMMA_PROJECT_PRIORITY_PAIR_H
