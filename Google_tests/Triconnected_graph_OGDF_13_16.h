//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_OGDF_THIRTEEN_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_OGDF_THIRTEEN_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_OGDF_13_16 : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 4, 6, 11, 12, 14, 17, 21, 25, 26, 28, 30, 31, 32};
        v_col_ind = {1, 2, 5, 7, 0, 6, 0, 3, 4, 5, 7, 2, 2, 8, 0, 2, 7,
                     1, 10, 11, 12, 0, 2, 5, 9, 4, 7, 10, 6, 9, 6, 6};

        v_values = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};

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

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_OGDF_THIRTEEN_H
