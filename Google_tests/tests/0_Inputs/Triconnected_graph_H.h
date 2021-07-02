//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_G_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_G_H

#include "gtest/gtest.h"
#include "../../../../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_H : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 4, 8, 12, 16, 20, 24, 28, 32};
        v_col_ind = {0, 1, 3, 4, 0, 1, 2, 5, 1, 2, 3, 6, 0, 2, 3, 7, 0, 4, 5, 7, 1, 4, 5, 6, 2, 5, 6, 7, 3, 4, 6, 7};
        v_values = {3., 1., 1., 1., 1., 3., 1., 1., 1., 3., 1., 1., 1., 1., 3., 1., 1., 3., 1., 1., 1., 1., 3., 1., 1., 1., 3., 1., 1., 3., 1., 3.};

        nb_nodes = 8;
        nb_edges = 12;

        g = new Triconnected_graph(v_row_ptr, v_col_ind, v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_G_H
