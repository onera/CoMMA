#ifndef BIMAP_PROJECT_COMMA_H
#define BIMAP_PROJECT_COMMA_H

#include <cassert>
#include <type_traits>
#include <map>
#include <iostream>

using namespace std;
/** @brief An easy and straight forward implementation of a Bimap. **/
template<typename A, typename B>
class Bimap {
  public:
   /** @brief Constructor **/
    Bimap(){};
   /** @brief Destructor **/
    ~Bimap(){};
   /** @brief Insertion function in the Bimap
 *     @param[in] a element A to add
 *     @param[in] b element B to add**/
    void insert(const A &a,const B &b) {
      auto aitr = mapA.insert({b, nullptr}).first; // creates first pair
      const B * bp = &(aitr->first);  // get pointer of our stored copy of b
      auto bitr = mapB.insert({a, bp}).first;       //insert second pair {a, pointer_to_b}
      const A * ap = &(bitr->first);  // update pointer in mapA to point to a
      auto aim = mapA.insert_or_assign(b,ap);
      }

   /** @brief Function to print the map */
   void print(){
       for (const auto& [key, value] : mapB) {
        cout << '[' << key << "] = " << *value << "; ";}
      }

    /**@brief Getter of the B value starting from a A value
 *     @param[in] the associated value we want to search*/
      B get_B(const A &a){
      // We pass the associated value we want to search to the member 
      // variable
      auto it = mapB.find(a);
      return(*(it->second));
    }

    /**@brief Getter of the A value starting from a B value
 *     @param[in] the associated value we want to search*/
      A get_A(const B &b){
      // We pass the associated value we want to search to the member 
      // variable
      auto it = mapA.find(b);
      return(*(it->second));
    }



    /**@brief Eraser of the  value starting from a A value
 *     @param[in] the associated value we want to search in order to delete the relative entry*/
    void erase_B(const A &a){
      // We pass the associated value we want to search to the member 
      // variable
      auto itB = mapB.find(a);
      auto b= *(itB->second);
      auto itA = mapA.find(b);
      mapB.erase(itB);
      mapA.erase(itA);
    }
  /** @brief Check if the Bimap is empty*/
    inline bool empty(){ return(mapA.empty());}
   
  protected:
    /** @brief Left map **/
    map<B, const A*> mapA;
    /** @Brief Right map*/
    map<A, const B*> mapB; 
};
#endif
