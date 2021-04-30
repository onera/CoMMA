//
// Created by Nicolas Lantos on 21/04/2021.
//

#ifndef COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
#define COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
#include <unordered_set>

using namespace std;

class First_Order_Neighbourhood {
public :
    First_Order_Neighbourhood(unordered_set<long> s_neighbours_of_seed);
    unordered_set<long> update(long new_fc, unordered_set<long> s_new_neighbour);
public :
    unordered_set<long> __s_fc;
    unordered_set<long> __s_neighbours_of_seed;
    unordered_set<long> __first_order_neighbourhood;
};


#endif //COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
