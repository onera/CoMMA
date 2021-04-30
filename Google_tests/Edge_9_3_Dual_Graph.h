//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_EDGE_NINE_THREE_DUAL_GRAPH_H
#define COMMA_PROJECT_EDGE_NINE_THREE_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_graph.h"

class Edge_9_3_Dual_Graph : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int Edge_9_3_nb_c;
    int Edge_9_3_col_ind_size;
    vector<long> v_Edge_9_3_row_ptr;
    vector<long> v_Edge_9_3_col_ind;
    vector<double> v_Edge_9_3_values;
    vector<double> v_Edge_9_3_volumes;
    unordered_set<long> s_Edge_9_3_is_on_corner;
    unordered_set<long> s_Edge_9_3_is_on_ridge;
    unordered_set<long> s_Edge_9_3_is_on_valley;

    virtual void SetUp() {

        Edge_9_3_nb_c = 9;
        Edge_9_3_col_ind_size = 33;

        v_Edge_9_3_row_ptr = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
        v_Edge_9_3_col_ind = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1, 3};

        v_Edge_9_3_values = vector<double>(Edge_9_3_col_ind_size, 1.0);
        v_Edge_9_3_volumes = vector<double>(Edge_9_3_col_ind_size, 1.0);

        unordered_map<long, int> Edge_9_3_d_is_on_bnd;


        g = new Dual_Graph(Edge_9_3_nb_c,
                           v_Edge_9_3_row_ptr,
                           v_Edge_9_3_col_ind,
                           v_Edge_9_3_values,
                           v_Edge_9_3_volumes,
                           Edge_9_3_d_is_on_bnd,
                           unordered_set<long>({}),
                           unordered_set<long>({}),
                           unordered_set<long>({}),
                           unordered_set<long>({}),
                           0,
                           2);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_EDGE_NINE_THREE_DUAL_GRAPH_H
