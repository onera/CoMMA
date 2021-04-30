//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_EXAMPLE_SIX_DUAL_GRAPH_H
#define COMMA_PROJECT_EXAMPLE_SIX_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Example_6_Dual_Graph : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int Example_6_nb_c;
    int Example_6_col_ind_size;
    vector<long> v_Example_6_row_ptr;
    vector<long> v_Example_6_col_ind;
    vector<double> v_Example_6_values;
    vector<double> v_Example_6_volumes;
    unordered_set<long> s_Example_6_is_on_corner;
    unordered_set<long> s_Example_6_is_on_ridge;
    unordered_set<long> s_Example_6_is_on_valley;

    virtual void SetUp() {

        Example_6_nb_c = 9;
        Example_6_col_ind_size = 37;

        v_Example_6_row_ptr = {0, 4, 7, 11, 15, 19, 22, 26, 32, 37};
        v_Example_6_col_ind = {0, 1, 5, 8, 0, 1, 2, 1, 2, 6, 7, 3, 4, 6, 7, 3, 4, 7, 8, 0, 5, 8, 2, 3, 6, 7, 2, 3, 4, 6, 7, 8, 0, 4, 5, 7, 8};

        v_Example_6_values = vector<double>(Example_6_col_ind_size, 1.0);
        v_Example_6_volumes = vector<double>(Example_6_col_ind_size, 1.0);

        unordered_map<long, int> Example_6_d_is_on_bnd;


        g = new Dual_Graph(Example_6_nb_c,
                           v_Example_6_row_ptr,
                           v_Example_6_col_ind,
                           v_Example_6_values,
                           v_Example_6_volumes,
                           Example_6_d_is_on_bnd,
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

#endif //COMMA_PROJECT_EXAMPLE_SIX_DUAL_GRAPH_H
