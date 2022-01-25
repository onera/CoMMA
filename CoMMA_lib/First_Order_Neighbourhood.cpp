//
// Created by Nicolas Lantos on 21/04/2021.
//

#include "First_Order_Neighbourhood.h"

First_Order_Neighbourhood::First_Order_Neighbourhood(unordered_set<long> s_neighbours_of_seed) {
    __s_fc = {};  // definition of the cc
    __s_neighbours_of_seed = s_neighbours_of_seed;  // defined once and for all
    __first_order_neighbourhood = {};

}

unordered_set<long> First_Order_Neighbourhood::update(long new_fc, unordered_set<long> s_new_neighbour) {
    __s_fc.insert(new_fc);
    if (__first_order_neighbourhood.count(new_fc))
    {
        __first_order_neighbourhood.erase(new_fc);
    }
    for (const long& i_fc : s_new_neighbour){
        if (__s_fc.count(i_fc)==0)
        {
            if (__s_neighbours_of_seed.count(i_fc))
            {
                __first_order_neighbourhood.insert(i_fc);
            }
        }
    }
    return __first_order_neighbourhood;
}
