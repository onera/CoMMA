#ifndef STACK_PROJECT_COMMA_H
#define STACK_PROJECT_COMMA_H

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

#include <cassert>
#include <vector>
#include <deque>
#include <iostream>

using namespace std;
/** @brief A template class implementing the Queue data structure **/
template <typename T>
class Queue {
 private:
  /** @brief The vector of elements **/
  deque<T> _elems;

 public:
  /** @brief Constructor **/
  Queue() {};
  /** @brief Destructor **/
  ~Queue() {};
  /** @brief push an element on the top of the Queue.
  *   philosophy **/
  void push(T const& elem) { _elems.push_back(elem); }
  /** @brief pop an element from the bottom of the Queue
   *  @return The element popped**/
  T pop() {
    if (_elems.empty()) {
      return static_cast<T>(-1);
    }
    T elem = _elems.front();  // Copy last element
    _elems.pop_front();       // remove last element
    return elem;
  };
  /** @brief Scope to the element on the bottom of the Queue
   *  @return The bottom element of the queue**/
  T top() const {
    if (_elems.empty()) {
      return static_cast<T>(-1);
    }
    return _elems.front();
  }
  inline bool empty() const {return _elems.empty(); }
};
#endif
