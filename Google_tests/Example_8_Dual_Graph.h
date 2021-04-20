//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_EXAMPLE_EIGHT_DUAL_GRAPH_H
#define COMMA_PROJECT_EXAMPLE_EIGHT_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_graph.h"

class Example_8_Dual_Graph : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int Example_8_nb_c;
    int Example_8_col_ind_size;
    vector<long> v_Example_8_row_ptr;
    vector<long> v_Example_8_col_ind;
    vector<double> v_Example_8_values;
    vector<double> v_Example_8_volumes;
    unordered_set<long> s_Example_8_is_on_corner;
    unordered_set<long> s_Example_8_is_on_ridge;
    unordered_set<long> s_Example_8_is_on_valley;

    virtual void SetUp() {

        Example_8_nb_c = 12;
        Example_8_col_ind_size = 39;

        v_Example_8_row_ptr = {0, 5, 8, 11, 15, 20, 23, 26, 28, 32, 36, 39};
        v_Example_8_col_ind = {0, 1, 2, 3, 4, 0, 1, 9, 0, 2, 6, 0, 3, 6, 8, 0, 4, 5, 8, 9, 4, 5, 7, 2, 3, 6, 5, 7, 3, 4, 8, 10, 1, 4, 9, 10, 8, 9, 10};

        v_Example_8_values = vector<double>(Example_8_col_ind_size, 1.0);
        v_Example_8_volumes = vector<double>(Example_8_col_ind_size, 1.0);

        unordered_map<long, int> Example_8_d_is_on_bnd;


        g = new Dual_Graph(Example_8_nb_c,
                           v_Example_8_row_ptr,
                           v_Example_8_col_ind,
                           v_Example_8_values,
                           v_Example_8_volumes,
                           Example_8_d_is_on_bnd);

    }

    virtual void TearDown() {
        delete g;
    }
};

#endif //COMMA_PROJECT_EXAMPLE_EIGHT_DUAL_GRAPH_H
