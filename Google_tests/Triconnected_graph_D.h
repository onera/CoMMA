//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_D_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_D_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_D : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 1, 4, 6, 8, 10};
        v_col_ind = {1, 0, 2, 4, 1, 3, 2, 4, 1, 3};

        v_values = {1., 1., 4., 4., 6., 7., 11., 10., 13., 12.};

        nb_nodes = 5;
        nb_edges = 5;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_D_H
