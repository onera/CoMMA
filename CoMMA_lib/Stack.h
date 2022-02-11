#ifndef STACK_PROJECT_COMMA_H
#define STACK_PROJECT_COMMA_H

#include <cassert>
#include <vector>
#include <iostream>

using namespace std;
/** @brief A template class implementing the Stack data structure **/
template<typename T>
class Stack {
  private:
    /** @brief The vector of elements **/
    vector<T> elems;
  public:
   /** @brief Constructor **/
    Stack(){};
   /** @brief Destructor **/
    ~Stack(){};
   /** @brief push an element on the top of the stack following a LIFO (Last In First Out) 
   *   philosophy **/
    void push(T const& elem){
     elems.push_back(elem);
    }
   /** @brief pop an element from the top of the stack 
    *  @return The element popped**/
    T pop(){
      if (elems.empty()) {
        return static_cast<T>(-1); 
     }
     T elem = elems.back(); // Copy last element
     elems.pop_back();      // remove last element
     return elem;   
    };
    /** @brief Scope to the last element in, hence to the top of the Stack 
     *  @return The top element of the stack**/
    T top() const{
     if (elems.empty()) {
        return static_cast<T>(-1); 
     }
     return elems.back();
    }
};
#endif
