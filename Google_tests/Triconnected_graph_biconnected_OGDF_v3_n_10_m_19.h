//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_OGDF_V3_N_TEN_M_NINETEEN_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_OGDF_V3_N_TEN_M_NINETEEN_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_Biconnected_OGDF_v3_n_10_m_19 : public ::testing::Test {
    // Same as Triconnected_graph_Biconnected_OGDF_n_10_m_19 but a different order in the data structure (col_ind: i.e. the neighbours of a vertex are visited in a different order.
protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 5, 9, 14, 19, 24, 28, 30, 32, 36, 38};
        v_col_ind = {3, 2, 6, 9, 8, 2, 3, 5, 9, 0, 1, 4, 5, 7, 0, 1, 4, 5, 8, 2, 3, 5,
                     7, 8, 1, 2, 3, 4, 0, 8, 2, 4, 0, 3, 4, 6, 0, 1};

        v_values = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};

        nb_nodes = 10;
        nb_edges = 19;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_OGDF_V3_N_TEN_M_NINETEEN_H
