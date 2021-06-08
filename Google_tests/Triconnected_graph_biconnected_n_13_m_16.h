//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_N_THIRTEEN_M_SIXTEEN_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_N_THIRTEEN_M_SIXTEEN_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_Biconnected_n_13_m_16 : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 4, 7, 10, 12, 14, 17, 19, 22, 24, 26, 28, 30, 32};
        v_col_ind = {3, 6, 7, 10, 2, 5, 7, 1, 9, 10, 0, 11, 6, 8, 1, 11, 12,
                     0, 4, 0, 1, 12, 4, 9, 2, 8, 0, 2, 3, 5, 5, 7};

        v_values = {1., 1., 1., 1., 2., 1., 1., 2., 4., 1., 1., 1., 1., 1., 1., 1., 1.,
                    1., 1., 1., 1., 2., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};

        nb_nodes = 13;
        nb_edges = 16;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_N_THIRTEEN_M_SIXTEEN_H
