//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_HEXAHEDRA_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_HEXAHEDRA_H

#include "gtest/gtest.h"
#include "../../../CoMMA_lib/Dual_Graph.h"

class Triconnected_graph_Biconnected_Hexahedron : public ::testing::Test {

protected:

    Triconnected_graph *g;
    int nb_nodes;
    int nb_edges;
    vector<long> v_row_ptr;
    vector<long> v_col_ind;
    vector<double> v_values;

    virtual void SetUp() {

        v_row_ptr = {0, 3, 7, 10, 14, 19, 21, 24, 26, 30, 35, 37, 42, 45, 47, 50, 52, 54};
        v_col_ind = {1, 3, 8, 0, 2, 4, 9, 1, 5, 10, 0, 4, 6, 11, 1, 3, 5,
                     7, 12, 2, 4, 3, 7, 13, 4, 6, 0, 9, 11, 14, 1, 8, 10, 12,
                     15, 2, 9, 3, 8, 12, 13, 16, 4, 9, 11, 6, 11, 8, 15, 16, 9,
                     14, 11, 14};

        v_values = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                    1., 1., 1.};

        nb_nodes = 17;
        nb_edges = 27;

        g = new Triconnected_graph(v_row_ptr,
                                   v_col_ind,
                                   v_values);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_BICONNECTED_HEXAHEDRA_H
