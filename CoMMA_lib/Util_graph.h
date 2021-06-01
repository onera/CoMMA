//
// Created by Nicolas Lantos on 01/06/2021.
//

#ifndef COMMA_PROJECT_UTIL_GRAPH_H
#define COMMA_PROJECT_UTIL_GRAPH_H

#include "Triconnected_graph.h"
#include <vector>
#include <utility>      // std::pair, std::make_pair

bool BCC_NR(const Triconnected_graph &graph, list<unordered_set<long>> &component);

unordered_map<long, short> compute_Degree(const unordered_set<long> &set_Of_FC, const vector<long> &matrixAdj_CRS_row_ptr, const vector<long> &matrixAdj_CRS_col_ind);

void removeLeaf(unordered_set<long> &set_Of_FC, const Triconnected_graph &graph);

#endif //COMMA_PROJECT_UTIL_GRAPH_H
