//
// Created by Nicolas Lantos on 22/03/2021.
//

#ifndef COMMA_PROJECT_SEEDS_POOL_H
#define COMMA_PROJECT_SEEDS_POOL_H

#define NULL nullptr

#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <deque>
#include <vector>
#include <list>


using namespace std;

class Seeds_Pool {

public :
    Seeds_Pool(int number_of_cells,
               unordered_map<long, int> &d_is_on_bnd,
               unordered_set<long> is_on_corner = unordered_set<long>({}),
               unordered_set<long> is_on_ridge = unordered_set<long>({}),
               unordered_set<long> is_on_valley = unordered_set<long>({}),
               int init_bnd_level = 3);

    int number_of_cells;
    unordered_map<long, int> d_is_on_bnd;
    unordered_set<long> is_on_corner;
    unordered_set<long> is_on_ridge;
    unordered_set<long> is_on_valley;
    int init_bnd_level;
    vector<deque<long>> l_deque_of_seeds;

    long choose_new_seed(const bool *a_is_fc_agglomerated);

    int boundary_value(const long i_fc);

    void update(list<long> l_new_seeds, bool is_extremal_vertex = false);
};

#endif //COMMA_PROJECT_SEEDS_POOL_H
