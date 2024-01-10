#ifndef BIMAP_PROJECT_COMMA_H
#define BIMAP_PROJECT_COMMA_H

/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <cassert>
#include <iostream>
#include <map>
#include <type_traits>

namespace comma {

/** @brief An easy and straight forward implementation of a Bimap.
 *  @deprecated Not used anymore
 */
template<typename A, typename B>
class Bimap {
public:
  /** @brief Constructor */
  Bimap(){};
  /** @brief Destructor */
  ~Bimap(){};

  //  using container= std::map<A, const B*>;
  //  using iterator=typename container::iterator;
  //  inline iterator begin() noexcept { return _mapB.begin(); }
  //  inline iterator end() noexcept { return _mapB.end(); }

  /** @brief Insertion function in the Bimap
   *  @param[in] a element A to add
   *  @param[in] b element B to add
   */
  void insert(const A &a, const B &b) {
    auto aitr = _mapA.insert({b, nullptr}).first;  // creates first pair
    const B *bp = &(aitr->first);  // get pointer of our stored copy of b
    auto bitr =
      _mapB.insert({a, bp}).first;  // insert second pair {a, pointer_to_b}
    const A *ap = &(bitr->first);  // update pointer in _mapA to point to a
    _mapA.insert_or_assign(b, ap);
  }

  /** @brief Function to print the map */
  void print() {
    for (const auto &[key, value] : _mapB) {
      std::cout << '[' << key << "] = " << *value << "; ";
    }
  }

  /** @brief Update of the key of the map B and hence the value of the node A
   * @param[in] a_old the old value of the node A (in case you do not know you
   * can find it through the find.
   * @param[in] a_new the new value we want to attribute to the node A.
   */
  void update_nodeB(const A &a_old, const A &a_new) {
    auto node = _mapB.extract(a_old);
    if (!node.empty()) {
      node.key() = a_new;
      _mapB.insert(std::move(node));
    }
  }
  /** @brief Update of the key of the map A and hence the value of the node B
   * @param[in] b_old the old value of the node B (in case you do not know you
   * can find it through the find.
   * @param[in] b_new the new value we want to attribute to the node B.
   */
  void update_nodeA(const B &b_old, const B &b_new) {
    auto node = _mapA.extract(b_old);
    if (!node.empty()) {
      node.key() = b_new;
      _mapA.insert(std::move(node));
    }
  }
  /**@brief Getter of the B value starting from a A value
   * @param[in] a the associated value we want to search
   * @return the value associated to \p a
   */
  B get_B(const A &a) {
    // We pass the associated value we want to search to the member
    // variable
    auto it = _mapB.find(a);
    return (*(it->second));
  }

  /**@brief Getter of the A value starting from a B value
   * @param[in] b the associated value we want to search
   * @return the value associated to \p b
   */
  A get_A(const B &b) {
    // We pass the associated value we want to search to the member
    // variable
    auto it = _mapA.find(b);
    return (*(it->second));
  }

  /**@brief Eraser of the  value starting from a A value
   * @param[in] a the associated value we want to search in order to delete the
   * relative entry
   */
  void erase_B(const A &a) {
    // We pass the associated value we want to search to the member
    // variable
    auto itB = _mapB.find(a);
    auto b = *(itB->second);
    auto itA = _mapA.find(b);
    _mapB.erase(itB);
    _mapA.erase(itA);
  }
  /** @brief Check if the Bimap is empty
   *  @return a boolean
   */
  inline bool empty() { return (_mapA.empty()); }
  /** @brief Returns the size of the container
   *  @return the size
   */
  inline size_t lung() { return (_mapB.size()); }

protected:
  /** @brief Left map  */
  std::map<B, const A *> _mapA;
  /** @brief Right map */
  std::map<A, const B *> _mapB;
};

}  // end namespace comma

#endif
