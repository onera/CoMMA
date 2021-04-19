//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_GRAPH_BIC_DUAL_GRAPH_H
#define COMMA_PROJECT_GRAPH_BIC_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"
class Graphs_biconnected : public ::testing::Test {

protected:

    Dual_Graph *g;
    int graph_biconnected_nb_c;
    int graph_biconnected_col_ind_size;
    vector<long> v_graph_biconnected_row_ptr;
    vector<long> v_graph_biconnected_col_ind;
    vector<double> v_graph_biconnected_values;
    vector<double> v_graph_biconnected_volumes;
    unordered_set<long> s_graph_biconnected_is_on_corner;
    unordered_set<long> s_graph_biconnected_is_on_ridge;
    unordered_set<long> s_graph_biconnected_is_on_valley;

    virtual void SetUp() {


        v_graph_biconnected_row_ptr = {0, 3, 7, 11, 14, 17, 21, 25, 28};
        v_graph_biconnected_col_ind = {1, 4, 5, 0, 2, 4, 5, 1, 3, 6, 7, 2, 6, 7, 0, 1, 5, 0, 1, 4, 6, 2, 3, 5, 7, 2, 3, 6};

        v_graph_biconnected_values = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                      1., 1., 1., 1., 1., 1., 1.,
                                      1., 1.};


        graph_biconnected_nb_c = 8;
        graph_biconnected_col_ind_size = 27;
        v_graph_biconnected_volumes = {1., 1., 1., 1.,1., 1.,1., 1.};

        unordered_map<long, int> graph_biconnected_d_is_on_bnd {{0, 2},
                                                                {4, 2},
                                                                {3, 2},
                                                                {7, 2}
                                                                };
//        graph_biconnected_d_is_on_bnd[0] = 1;
//        graph_biconnected_d_is_on_bnd[1] = 1;
//        graph_biconnected_d_is_on_bnd[3] = 1;
//        graph_biconnected_d_is_on_bnd[4] = 1;
//        graph_biconnected_d_is_on_bnd[12] = 2;
//        graph_biconnected_d_is_on_bnd[13] = 1;
//        graph_biconnected_d_is_on_bnd[14] = 2;

//        s_graph_biconnected_is_on_corner = {};
//        s_graph_biconnected_is_on_ridge = {12, 14};
//        s_graph_biconnected_is_on_valley = {0, 1, 3, 4, 13};

        g = new Dual_Graph(graph_biconnected_nb_c,
                           v_graph_biconnected_row_ptr,
                           v_graph_biconnected_col_ind,
                           v_graph_biconnected_values,
                           v_graph_biconnected_volumes,
                           graph_biconnected_d_is_on_bnd);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_GRAPH_BIC_DUAL_GRAPH_H
