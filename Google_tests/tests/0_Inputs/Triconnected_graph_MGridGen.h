//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_MGRIDGEN_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_MGRIDGEN_H

#include "gtest/gtest.h"
#include "../../../../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_MGridGen : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 2, 4, 7, 9, 11, 14, 17, 20, 23, 26, 29, 32, 33, 35, 36};
        v_col_ind = {1, 2, 0, 3, 0, 5, 6, 1, 6, 5, 7, 8, 2, 4, 11, 2, 3,
                     8, 9, 4, 10, 5, 7, 10, 12, 7, 8, 9, 13, 14, 6, 13, 9, 10,
                     11, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        v_values = {2., 2., 2., 1., 2.,
                    2.23606798, 2.23606798, 1., 2.23606798, 2.,
                    2., 1., 2.23606798, 2., 2.,
                    2.23606798, 2.23606798, 1., 2.23606798, 2.,
                    1.41421356, 1., 1., 2., 3.60555128,
                    2.23606798, 1.41421356, 2., 3.16227766, 3.16227766,
                    2., 3.16227766, 3.60555128, 3.16227766, 3.16227766,
                    3.16227766, 0., 0., 0., 0.,
                    0., 0., 0., 0., 0.,
                    0., 0., 0., 0., 0.,
                    0.};

        nb_nodes = 15;
        nb_edges = 46;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_MGRIDGEN_H
