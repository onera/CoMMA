//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_A_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_A_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_A : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 2, 5, 8, 11, 13, 16, 20, 24, 28, 31, 33, 36, 39, 42, 44};
        v_col_ind = {1, 5, 0, 2, 6, 1, 3, 7, 8, 2, 4, 9, 3, 0, 10, 6, 1,
                     11, 5, 7, 8, 2, 12, 6, 9, 3, 13, 7, 8, 4, 14, 11, 5, 10,
                     12, 6, 11, 13, 7, 8, 12, 14, 9, 13};

        v_values = {1., 2., 3., 4., 5., 6., 7., 8., 11., 9., 10., 13., 12.,
                    14., 16., 15., 17., 20., 18., 19., 23., 21., 24., 22., 27., 25.,
                    28., 26., 30., 29., 31., 33., 32., 35., 36., 34., 38., 39., 37.,
                    40., 41., 42., 43., 44.};

        nb_nodes = 15;
        nb_edges = 22;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_A_H
