#ifndef COMMA_PROJECT_PRIORITY_PAIR_H
#define COMMA_PROJECT_PRIORITY_PAIR_H

/*
 * CoMMA
 * Copyright © 2023 ONERA
 *
 * Authors and Architects : Alberto Remigi, Nicolas Lantos, and Riccardo
 * Milani
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
 **/

#include <utility>

using namespace std;

/** @brief Wrapper around the STL pair with custom 'less than' operator: as in the
 * standard case, first we compare the first elements, then the second ones; however
 * it relies on the 'greater than' on the first elements, e.g., (4,X) < (3,Y),
 * whereas standard rules apply to second elements, e.g., (4,3) < (4,4).
 * @tparam A Type of the first element of the pair
 * @tparam B Type of the second element of the pair
 **/
template< typename A, typename B>
class Priority_Pair {
private:
  /** @brief STL pair around which current class is built */
  pair<A,B> _p;

public:
  /** @brief Constructor */
  Priority_Pair() {};

  /** @brief Constructor
   *  @param a First element
   *  @param b Second element
   */
  Priority_Pair(const A &a, const B &b) : _p(a,b) {};

  /** @brief Destructor */
  ~Priority_Pair() {};

  /** @brief Accessor to the first element
   *  @return The first element
   */
  // We accept to have a copy passed
  A first()  const {return _p.first;}

  /** @brief Accessor to the first element
   *  @return The second element
   */
  B second() const {return _p.second;}

  /** @brief Operator 'less than'. It actually relies on the 'greater than' operator
   * for the first elements and 'less than' for the second ones
   *  @param a First pair
   *  @param b Second pair
   *  @return true if a.first > b.first or (a.first == b.first and a.second < b.second);
   * false otherwise
   */
  inline friend bool operator< (const Priority_Pair &a, const Priority_Pair &b) {
      if      (a._p.first > b._p.first)   return true;
      else if (a._p.first < b._p.first)   return false;
      else /* a._p.first == b._p.first */ return a._p.second < b._p.second;
  }

  /** @brief Operator 'equal'
   *  @param a First pair
   *  @param b Second pair
   *  @return true if a and b are equal
   */
  inline friend bool operator== (const Priority_Pair &a, const Priority_Pair &b) {
      return (a._p.first == b._p.first) && (a._p.second == b._p.second);
  }
};
#endif // COMMA_PROJECT_PRIORITY_PAIR_H
