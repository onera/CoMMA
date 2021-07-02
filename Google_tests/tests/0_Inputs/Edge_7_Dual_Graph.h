//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_EDGE_SEVEN_DUAL_GRAPH_H
#define COMMA_PROJECT_EDGE_SEVEN_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../../../CoMMA_lib/Dual_Graph.h"
class Edge_7_Dual_Graph : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int Edge_7_nb_c;
    int Edge_7_col_ind_size;
    vector<long> v_Edge_7_row_ptr;
    vector<long> v_Edge_7_col_ind;
    vector<double> v_Edge_7_values;
    vector<double> v_Edge_7_volumes;
    unordered_set<long> s_Edge_7_is_on_corner;
    unordered_set<long> s_Edge_7_is_on_ridge;
    unordered_set<long> s_Edge_7_is_on_valley;

    virtual void SetUp() {

        Edge_7_nb_c = 7;
        Edge_7_col_ind_size = 29;

        v_Edge_7_row_ptr = {0, 4, 7, 13, 16, 21, 26, 29};
        v_Edge_7_col_ind = {0, 1, 2, 5, 0, 1, 2, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6};

        v_Edge_7_values = vector<double>(Edge_7_col_ind_size, 1.0);
        v_Edge_7_volumes= vector<double>(Edge_7_col_ind_size, 1.0);

        unordered_map<long, int> Edge_7_d_is_on_bnd;


        g = new Dual_Graph(Edge_7_nb_c,
                           v_Edge_7_row_ptr,
                           v_Edge_7_col_ind,
                           v_Edge_7_values,
                           v_Edge_7_volumes,
                           Edge_7_d_is_on_bnd,
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

#endif //COMMA_PROJECT_EDGE_SEVEN_DUAL_GRAPH_H
