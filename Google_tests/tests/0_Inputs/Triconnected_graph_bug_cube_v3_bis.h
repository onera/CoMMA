//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_BUG_VTHREE_BIS_CUBE_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_BUG_VTHREE_BIS_CUBE_H

#include "gtest/gtest.h"
#include "../../../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_bug_cube_v3_bis : public ::testing::Test {

protected:
    Dual_Graph *dg;
    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 2, 4, 8, 11, 14, 17, 20, 22, 24, 26, 30, 32};
        v_col_ind = {10, 4,
                     11, 6,
                     10, 4, 5, 7,
                     10, 11, 5,
                     0, 2, 6,  // 4
                     2, 3, 6,  // 5
                     1, 4, 5,
                     8, 2,
                     9, 7,
                     8, 10,  // 9
                     0, 9, 2, 3,
                     1, 3};

        v_values = {0.00236286, 0.00254951,
                    0.00183239, 0.00205744,
                    0.00256546, 0.00254951, 0.00211225, 0.00290172,
                    0.00227574, 0.00203716, 0.00171482,
                    0.00254951, 0.00254951, 0.00254951,
                    0.00211225, 0.00171482, 0.00205452,
                    0.00205744, 0.00254951, 0.00205452, 0.00248467, 0.00290172, 0.00305823,
                    0.00248467, 0.00305823, 0.00306435, 0.00236286, 0.00306435, 0.00256546,
                    0.00227574, 0.00183239, 0.00203716};

        nb_nodes = 12;
        nb_edges = 18;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

        vector<double> v_volumes(nb_nodes, 1.0);
        unordered_map<long, int> d_is_on_bnd = {};
        dg = new Dual_Graph(nb_nodes,
                            v_row_ptr,
                            v_col_ind,
                            v_values,
                            v_volumes,
                            d_is_on_bnd
        );
    }

    virtual void TearDown() {
        delete g;
        delete dg;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_BUG_VTHREE_BIS_CUBE_H
