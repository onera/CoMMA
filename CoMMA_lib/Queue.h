#ifndef STACK_PROJECT_COMMA_H
#define STACK_PROJECT_COMMA_H

#include <cassert>
#include <vector>
#include <deque>
#include <iostream>

using namespace std;
/** @brief A template class implementing the Queue data structure **/
template<typename T>
class Queue {
  private:
    /** @brief The vector of elements **/
    deque<T> elems;
  public:
   /** @brief Constructor **/
    Queue(){};
   /** @brief Destructor **/
    ~Queue(){};
   /** @brief push an element on the top of the Queue.
   *   philosophy **/
    void push(T const& elem){
     elems.push_back(elem);
    }
   /** @brief pop an element from the bottom of the Queue 
    *  @return The element popped**/
    T pop(){
      if (elems.empty()) {
        return static_cast<T>(-1); 
     }
     T elem = elems.front(); // Copy last element
     elems.pop_front();      // remove last element
     return elem;   
    };
    /** @brief Scope to the element on the bottom of the Queue 
     *  @return The bottom element of the queue**/
    T top() const{
     if (elems.empty()) {
        return static_cast<T>(-1); 
     }
     return elems.front();
    }
};
#endif
