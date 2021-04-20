//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_EXAMPLE_NINE_DUAL_GRAPH_H
#define COMMA_PROJECT_EXAMPLE_NINE_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_graph.h"

class Example_9_Dual_Graph : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int Example_9_nb_c;
    int Example_9_col_ind_size;
    vector<long> v_Example_9_row_ptr;
    vector<long> v_Example_9_col_ind;
    vector<double> v_Example_9_values;
    vector<double> v_Example_9_volumes;
    unordered_set<long> s_Example_9_is_on_corner;
    unordered_set<long> s_Example_9_is_on_ridge;
    unordered_set<long> s_Example_9_is_on_valley;

    virtual void SetUp() {

        Example_9_nb_c = 12;
        Example_9_col_ind_size = 39;

        v_Example_9_row_ptr = {0, 2, 4, 8, 11, 14, 19, 23, 27, 30, 32, 35};
        v_Example_9_col_ind = {0, 7, 1, 5, 2, 3, 5, 10, 2, 3, 8, 4, 6, 10, 1, 2, 5, 7, 8, 4, 6, 7, 9, 0, 5,
                               6, 7, 3, 5, 8, 6, 9, 2, 4, 10};

        v_Example_9_values = vector<double>(Example_9_col_ind_size, 1.0);
        v_Example_9_volumes = vector<double>(Example_9_col_ind_size, 1.0);

        unordered_map<long, int> Example_9_d_is_on_bnd;


        g = new Dual_Graph(Example_9_nb_c,
                           v_Example_9_row_ptr,
                           v_Example_9_col_ind,
                           v_Example_9_values,
                           v_Example_9_volumes,
                           Example_9_d_is_on_bnd);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_EXAMPLE_NINE_DUAL_GRAPH_H
