//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_C_THREE_BCC_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_C_THREE_BCC_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_C_3_BCC : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 2, 4, 7, 10, 12, 15, 18, 20, 22};
        v_col_ind = {1, 2, 0, 2, 0, 1, 3, 2, 4, 5, 3, 5, 4, 3, 6, 5, 7, 8, 6, 8, 6, 7};

        v_values = {1., 1., 1., 4., 1., 6., 7., 11., 10., 10., 13., 12., 14.,
                    16., 1., 17., 1., 1., 23., 21., 27., 25.};

        nb_nodes = 9;
        nb_edges = 11;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_C_THREE_BCC_H
