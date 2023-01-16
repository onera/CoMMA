#ifndef BIMAP_PROJECT_COMMA_H
#define BIMAP_PROJECT_COMMA_H

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

#include <cassert>
#include <iostream>
#include <map>
#include <type_traits>

using namespace std;
/** @brief An easy and straight forward implementation of a Bimap.
 *  @todo implement and understand the use of iterators**/
template <typename A, typename B>
class Bimap {
 public:
  /** @brief Constructor **/
  Bimap() {};
  /** @brief Destructor **/
  ~Bimap() {};

  //  using container= map<A, const B*>;
  //  using iterator=typename container::iterator;
  //  inline iterator begin() noexcept { return _mapB.begin(); }
  //  inline iterator end() noexcept { return _mapB.end(); }

  /** @brief Insertion function in the Bimap
*     @param[in] a element A to add
*     @param[in] b element B to add**/

  void insert(const A &a, const B &b) {
    auto aitr = _mapA.insert({b, nullptr}).first;  // creates first pair
    const B *bp = &(aitr->first);  // get pointer of our stored copy of b
    auto bitr =
        _mapB.insert({a, bp}).first;  // insert second pair {a, pointer_to_b}
    const A *ap = &(bitr->first);    // update pointer in _mapA to point to a
    _mapA.insert_or_assign(b, ap);
  }

  /** @brief Function to print the map */
  void print() {
    for (const auto & [ key, value ] : _mapB) {
      cout << '[' << key << "] = " << *value << "; ";
    }
  }

  /** @brief Update of the key of the map B and hence the value of the node A
* @param[in] a_old the old value of the node A (in case you do not know you can
* find it through the find.
* @param[in] a_new the new value we want to attribute to the node A. */
  void update_nodeB(const A &a_old, const A &a_new) {
    auto node = _mapB.extract(a_old);
    if (!node.empty()) {
      node.key() = a_new;
      _mapB.insert(move(node));
    }
  }
  /** @brief Update of the key of the map A and hence the value of the node B
* @param[in] b_old the old value of the node B (in case you do not know you can
* find it through the find.
* @param[in] b_new the new value we want to attribute to the node B. */
  void update_nodeA(const B &b_old, const B &b_new) {
    auto node = _mapA.extract(b_old);
    if (!node.empty()) {
      node.key() = b_new;
      _mapA.insert(move(node));
    }
  }
  /**@brief Getter of the B value starting from a A value
*     @param[in] a the associated value we want to search*/
  B get_B(const A &a) {
    // We pass the associated value we want to search to the member
    // variable
    auto it = _mapB.find(a);
    return (*(it->second));
  }

  /**@brief Getter of the A value starting from a B value
*     @param[in] b the associated value we want to search*/
  A get_A(const B &b) {
    // We pass the associated value we want to search to the member
    // variable
    auto it = _mapA.find(b);
    return (*(it->second));
  }

  /**@brief Eraser of the  value starting from a A value
*     @param[in] a the associated value we want to search in order to delete the
* relative entry*/
  void erase_B(const A &a) {
    // We pass the associated value we want to search to the member
    // variable
    auto itB = _mapB.find(a);
    auto b = *(itB->second);
    auto itA = _mapA.find(b);
    _mapB.erase(itB);
    _mapA.erase(itA);
  }
  /** @brief Check if the Bimap is empty*/
  inline bool empty() { return (_mapA.empty()); }
  /** @brief Returns the size of the container*/
  inline size_t lung() { return (_mapB.size()); }

 protected:
  /** @brief Left map **/
  map<B, const A *> _mapA;
  /** @brief Right map*/
  map<A, const B *> _mapB;
};
#endif
