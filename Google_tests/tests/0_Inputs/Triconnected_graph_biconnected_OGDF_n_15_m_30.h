//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_OGDF_N_FIFTEEN_M_THIRTY_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_OGDF_N_FIFTEEN_M_THIRTY_H

#include "gtest/gtest.h"
#include "../../../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_Biconnected_OGDF_n_15_m_30 : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 8, 14, 20, 24, 29, 33, 35, 37, 41, 43, 47, 51, 55, 58, 60};
        v_col_ind = {1, 3, 6, 8, 9, 12, 13, 14, 0, 2, 3, 5, 11, 13, 1, 4, 5,
                     7, 10, 11, 0, 1, 5, 13, 2, 7, 8, 10, 12, 1, 2, 3, 10, 0,
                     8, 2, 4, 0, 4, 6, 12, 0, 11, 2, 4, 5, 12, 1, 2, 9, 14,
                     0, 4, 8, 10, 0, 1, 3, 0, 11};

        v_values = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                    1., 1., 1., 1., 1., 1., 1., 1., 1.};

        nb_nodes = 15;
        nb_edges = 30;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_OGDF_N_FIFTEEN_M_THIRTY_H
