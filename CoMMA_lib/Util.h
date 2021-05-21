//
// Created by Nicolas Lantos on 06/04/2021.
//

#ifndef COMMA_PROJECT_UTIL_H
#define COMMA_PROJECT_UTIL_H

#include <assert.h>
#include <deque>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <forward_list>
#include <cmath>
#include <iostream>


using namespace std;

void convert_agglomeration_lines_arrays_to_agglomeration_lines(
        const unsigned long fineAgglomerationLines_array_Idx_size,
        const unsigned long fineAgglomerationLines_array_size,
        long *fineAgglomerationLines_array_Idx,
        long *fineAgglomerationLines_array,
        long &nb_agglomeration_lines,
        forward_list<deque<long> *> *agglomeration_lines);

forward_list<deque < long> *> convertFineAgglomerationLines_Arrays_to_agglomeration_lines(
        unsigned long fineAgglomerationLines_array_Idx_size,
        unsigned long fineAgglomerationLines_array_size,
        long *fineAgglomerationLines_array_Idx,
        long *fineAgglomerationLines_array);

forward_list<deque < long> > convertFineAgglomerationLines_arraysToFineAgglomerationLines_without_pointers(
        unsigned long fineAgglomerationLines_array_Idx_size,
        unsigned long fineAgglomerationLines_array_size,
        long *fineAgglomerationLines_array_Idx,
        long *fineAgglomerationLines_array);
forward_list<int> test();

void convert_agglo_lines_to_agglomeration_lines_arrays(const unsigned long nb_lines,
                                                       forward_list<deque<long> *> *agglo_lines,
                                                       long *sizes,
                                                       long *agglo_lines_array_idx,
                                                       long *agglo_lines_array);

list<unordered_set<long>> parts_list(vector<long> seq, int length);

inline unordered_set<long> d_keys_to_set(const unordered_map<long, unsigned short>& dict){
    unordered_set<long> s_neighbours_of_seed = {};
    for (const auto &i_k_v : dict) {
        s_neighbours_of_seed.insert(i_k_v.first);
    }
return s_neighbours_of_seed;
}

void print_queue(queue<long> q);

#endif //COMMA_PROJECT_UTIL_H
