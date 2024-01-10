#ifndef STACK_PROJECT_COMMA_H
#define STACK_PROJECT_COMMA_H

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
#include <deque>
#include <iostream>
#include <vector>

namespace comma {

/** @brief A template class implementing a custom queue data structure
 *  @tparam T Type of the elements
 *  @deprecated Not used anymore
 */
template<typename T>
class Queue {
private:
  /** @brief The vector of elements */
  std::deque<T> _elems;

public:
  /** @brief Constructor */
  Queue() {}
  /** @brief Destructor */
  ~Queue() {}
  /** @brief Push an element on the top of the queue.
   *  @param elem The element to add
   */
  void push(T const &elem) { _elems.push_back(elem); }
  /** @brief pop an element from the bottom of the queue
   *  @return The element popped
   */
  T pop() {
    if (_elems.empty()) { return static_cast<T>(-1); }
    T elem = _elems.front();  // Copy last element
    _elems.pop_front();  // remove last element
    return elem;
  }
  /** @brief Scope to the element on the bottom of the queue
   *  @return The bottom element of the queue
   */
  T top() const {
    if (_elems.empty()) { return static_cast<T>(-1); }
    return _elems.front();
  }
  /** @brief Whether the queue is empty
   *  @return a bool
   */
  inline bool empty() const { return _elems.empty(); }
};

}  // end namespace comma

#endif
