//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_TWO_GRAPHS_BICONNECTED_N_EIGHT_M_FOURTEEN_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_TWO_GRAPHS_BICONNECTED_N_EIGHT_M_FOURTEEN_H

#include "gtest/gtest.h"
#include "../../../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_2_Graphs_Biconnected_n_8_m_14 : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 3, 7, 11, 14, 17, 21, 25, 28};
        v_col_ind = {1, 4, 5, 0, 2, 4, 5, 1, 3, 6, 7, 2, 6, 7, 0, 1, 5, 0, 1, 4, 6, 2, 3, 5, 7, 2, 3, 6};

        v_values = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};

        nb_nodes = 8;
        nb_edges = 14;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_TWO_GRAPHS_BICONNECTED_N_EIGHT_M_FOURTEEN_H
