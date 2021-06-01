//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_F_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_F_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_F : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 1, 6, 8, 10, 12, 14};
        v_col_ind = {1, 0, 2, 3, 4, 5, 1, 3, 2, 1, 1, 5, 1, 4};

        v_values = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};

        nb_nodes = 6;
        nb_edges = 7;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_F_H
