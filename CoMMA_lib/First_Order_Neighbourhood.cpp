//
// Created by Nicolas Lantos on 21/04/2021.
//

#include "First_Order_Neighbourhood.h"

First_Order_Neighbourhood::First_Order_Neighbourhood(
    unordered_set<long> s_neighbours_of_seed) {
  _s_fc = {};                                    // definition of the cc
  _s_neighbours_of_seed = s_neighbours_of_seed;  // defined once and for all
  _first_order_neighbourhood = {};
}

unordered_set<long> First_Order_Neighbourhood::update(
    long new_fc, unordered_set<long> s_new_neighbour) {
  _s_fc.insert(new_fc);
  if (_first_order_neighbourhood.count(new_fc)) {
    _first_order_neighbourhood.erase(new_fc);
  }
  for (const long& i_fc : s_new_neighbour) {
    if (_s_fc.count(i_fc) == 0) {
      if (_s_neighbours_of_seed.count(i_fc)) {
        _first_order_neighbourhood.insert(i_fc);
      }
    }
  }
  return _first_order_neighbourhood;
}
