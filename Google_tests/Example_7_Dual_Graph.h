//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_EXAMPLE_SEVEN_DUAL_GRAPH_H
#define COMMA_PROJECT_EXAMPLE_SEVEN_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_graph.h"

class Example_7_Dual_Graph : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int Example_7_nb_c;
    int Example_7_col_ind_size;
    vector<long> v_Example_7_row_ptr;
    vector<long> v_Example_7_col_ind;
    vector<double> v_Example_7_values;
    vector<double> v_Example_7_volumes;
    unordered_set<long> s_Example_7_is_on_corner;
    unordered_set<long> s_Example_7_is_on_ridge;
    unordered_set<long> s_Example_7_is_on_valley;

    virtual void SetUp() {

        Example_7_nb_c = 12;
        Example_7_col_ind_size = 40;

        v_Example_7_row_ptr = {0, 3, 8, 12, 15, 19, 22, 27, 31, 34, 37, 40};
        v_Example_7_col_ind = {0, 2, 7, 1, 3, 4, 8, 9, 0, 2, 5, 6, 1, 3, 9, 1, 4, 6, 9, 2, 5, 10, 2, 4, 6,
                               7, 10, 0, 6, 7, 8, 1, 7, 8, 3, 4, 9, 5, 6, 10};

        v_Example_7_values = vector<double>(Example_7_col_ind_size, 1.0);
        v_Example_7_volumes = vector<double>(Example_7_col_ind_size, 1.0);

        unordered_map<long, int> Example_7_d_is_on_bnd;


        g = new Dual_Graph(Example_7_nb_c,
                           v_Example_7_row_ptr,
                           v_Example_7_col_ind,
                           v_Example_7_values,
                           v_Example_7_volumes,
                           Example_7_d_is_on_bnd,
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

#endif //COMMA_PROJECT_EXAMPLE_SEVEN_DUAL_GRAPH_H
