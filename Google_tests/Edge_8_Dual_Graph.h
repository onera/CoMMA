//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_EDGE_EIGHT_DUAL_GRAPH_H
#define COMMA_PROJECT_EDGE_EIGHT_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_graph.h"

class Edge_8_Dual_Graph : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int Edge_8_nb_c;
    int Edge_8_col_ind_size;
    vector<long> v_Edge_8_row_ptr;
    vector<long> v_Edge_8_col_ind;
    vector<double> v_Edge_8_values;
    vector<double> v_Edge_8_volumes;
    unordered_set<long> s_Edge_8_is_on_corner;
    unordered_set<long> s_Edge_8_is_on_ridge;
    unordered_set<long> s_Edge_8_is_on_valley;

    virtual void SetUp() {

        Edge_8_nb_c = 9;
        Edge_8_col_ind_size = 31;

        v_Edge_8_row_ptr = {0, 4, 8, 14, 17, 22, 27, 30, 31};
        v_Edge_8_col_ind = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1};

        v_Edge_8_values = vector<double>(Edge_8_col_ind_size, 1.0);
        v_Edge_8_volumes = vector<double>(Edge_8_col_ind_size, 1.0);

        unordered_map<long, int> Edge_8_d_is_on_bnd;


        g = new Dual_Graph(Edge_8_nb_c,
                           v_Edge_8_row_ptr,
                           v_Edge_8_col_ind,
                           v_Edge_8_values,
                           v_Edge_8_volumes,
                           Edge_8_d_is_on_bnd);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_EDGE_EIGHT_DUAL_GRAPH_H
