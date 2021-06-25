//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_MGRIDGEN_EXT_DUAL_GRAPH_H
#define COMMA_PROJECT_MGRIDGEN_EXT_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class MGridGen_ext_Dual_Graph : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int MGridGen_nb_c;
    int MGridGen_col_ind_size;
    vector<long> v_MGridGen_row_ptr;
    vector<long> v_MGridGen_col_ind;
    vector<double> v_MGridGen_values;
    vector<double> v_MGridGen_volumes;
    unordered_set<long> s_MGridGen_is_on_corner;
    unordered_set<long> s_MGridGen_is_on_ridge;
    unordered_set<long> s_MGridGen_is_on_valley;

    virtual void SetUp() {

        v_MGridGen_row_ptr = {0, 2, 6, 10, 14, 18, 21, 25, 27, 30, 34, 38, 42, 46, 50, 53, 55, 59, 63, 66, 70, 74, 76};
        v_MGridGen_col_ind = {0, 1, 0, 1, 2, 3, 1, 2, 4, 10, 1, 3, 4, 5, 2, 3, 4,
                              11, 3, 5, 6, 5, 6, 7, 13, 6, 7, 8, 9, 16, 8, 9, 10, 17,
                              2, 9, 10, 11, 4, 10, 11, 12, 11, 12, 13, 19, 6, 12, 13, 14, 13,
                              14, 20, 15, 16, 8, 15, 16, 17, 9, 16, 17, 18, 17, 18, 19, 12, 18,
                              19, 20, 14, 19, 20, 21, 20, 21};

        v_MGridGen_values = {11., 3.60555128, 3.60555128, 4., 2.23606798,
                             2., 2.23606798, 2., 1., 2.,
                             2., 2., 1.41421356, 3.16227766, 1.,
                             1.41421356, 1., 1., 3.16227766, 8.,
                             3.16227766, 3.16227766, 6., 3.16227766, 2.,
                             3.16227766, 7., 4., 2.23606798, 1.,
                             2.23606798, 2., 2.82842712, 1., 2.,
                             2.82842712, 4., 2., 1., 2.,
                             2., 2.23606798, 2.23606798, 4., 2.23606798,
                             2., 2., 2.23606798, 4., 2.23606798,
                             2.23606798, 4., 1., 5., 2.23606798,
                             1., 2.23606798, 2., 2., 1.,
                             2., 2., 2.23606798, 2.23606798, 9.,
                             2.82842712, 2., 2.82842712, 4., 2.,
                             1., 2., 2., 2.23606798, 2.23606798,
                             5.};


        MGridGen_nb_c = 22;
        MGridGen_col_ind_size = 124;
        v_MGridGen_volumes = {3., 2., 1., 1., 0.5, 3., 3., 1.5, 1., 1., 2., 1., 2.,
                              2., 1., 1., 1., 1., 3., 2., 1., 1.};

        unordered_map<long, int> MGridGen_d_is_on_bnd{{0,  2},
                                                      {5,  1},
                                                      {7,  2},
                                                      {8,  1},
                                                      {14, 1},
                                                      {15, 2},
                                                      {18, 1},
                                                      {21, 2},
        };

        s_MGridGen_is_on_corner = {};
        s_MGridGen_is_on_ridge = {0, 7, 15, 21};
        s_MGridGen_is_on_valley = {5, 8, 14, 18};

        g = new Dual_Graph(MGridGen_nb_c,
                           v_MGridGen_row_ptr,
                           v_MGridGen_col_ind,
                           v_MGridGen_values,
                           v_MGridGen_volumes,
                           MGridGen_d_is_on_bnd,
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

#endif //COMMA_PROJECT_MGRIDGEN_EXT_V2_DUAL_GRAPH_H
