//
// Created by Nicolas Lantos on 06/04/2021.
//

#ifndef COMMA_PROJECT_UTIL_H
#define COMMA_PROJECT_UTIL_H

#include <assert.h>
#include <unordered_set>
#include <unordered_map>

using namespace std;


inline unordered_set<long> d_keys_to_set(const unordered_map<long,short> &dict) {
    unordered_set<long> s_neighbours_of_seed = {};
    for (const auto &i_k_v : dict) {
        s_neighbours_of_seed.insert(i_k_v.first);
    }
    return s_neighbours_of_seed;
}

#endif //COMMA_PROJECT_UTIL_H
