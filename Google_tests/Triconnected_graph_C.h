//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_C_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_C_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_C : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 1, 3, 6, 8, 11, 13, 14, 17, 19, 22, 24, 25, 27, 28};
        v_col_ind = {1, 0, 2, 1, 3, 7, 2, 4, 9, 3, 5, 4, 6, 5, 8, 2, 10,
                     9, 7, 8, 4, 12, 11, 7, 10, 9, 13, 12};

        v_values = {1., 1., 4., 6., 7., 8., 11., 10., 13., 12., 12., 14., 16.,
                    17., 23., 21., 24., 27., 25., 30., 29., 31., 33., 32., 35., 38.,
                    39., 41.};

        nb_nodes = 14;
        nb_edges = 14;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_C_H
