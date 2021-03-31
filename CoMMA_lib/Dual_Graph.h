//
// Created by Nicolas Lantos on 31/03/2021.
//

#ifndef COMMA_PROJECT_DUAL_GRAPH_H
#define COMMA_PROJECT_DUAL_GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <assert.h>
#include <deque>
#include <vector>
#include <list>
#include <forward_list>
#include "Seeds_Pool.h"

using namespace std;

class Dual_Graph {

public :
    Dual_Graph(long nb_c,
               const vector<long>& m_crs_row_ptr,
               const vector<long>& m_crs_col_ind,
               const vector<double>& m_crs_values,
               double* volumes,
               unordered_map<long, int> &d_is_on_bnd,
               unordered_set<long> is_on_corner = unordered_set<long>({}),
               unordered_set<long> is_on_ridge = unordered_set<long>({}),
               unordered_set<long> is_on_valley = unordered_set<long>({}),
               unordered_set<long> s_anisotropic_compliant_fc = unordered_set<long>({}),
               int verbose = 0,
               int dim = 3
    );

    // Attributes
    int number_of_cells;
    int _dimension;
    int _verbose;
    const vector<long> _m_CRS_Row_Ptr;
    const vector<long> _m_CRS_Col_Ind;
    const vector<double> _m_CRS_Values;
    double* _volumes;

    // TODO manage this. A pointer is not what I want.
    Seeds_Pool* seeds_pool;
    unordered_set<long> s_anisotropic_compliant_cells;
    int _lines_size = 0;
    forward_list<deque<long> *> _lines;

    // Methods
    vector<long> get_neighbours(long i_c);
    vector<double> get_weights(long i_c);
};


#endif //COMMA_PROJECT_DUAL_GRAPH_H
