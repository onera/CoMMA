//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_MGRIDGEN_DUAL_GRAPH_H
#define COMMA_PROJECT_MGRIDGEN_DUAL_GRAPH_H

#include"../../../CoMMA_lib/Dual_Graph.h"

class MGridGen_Dual_Graph : public ::testing::Test {
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

        v_MGridGen_row_ptr = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
        v_MGridGen_col_ind = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4, 7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13,
                              11, 14};

        v_MGridGen_values = {6.82842712, 2., 2., 2., 4.23606798,
                             1., 2., 4., 2.23606798, 2.23606798,
                             1., 4., 2.23606798, 6.82842712, 2.,
                             2., 2.23606798, 2., 2., 1.,
                             2.23606798, 2.23606798, 4., 2., 2., 2., 1., 2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2., 3.16227766, 2., 6.,
                             3.16227766, 3.16227766, 3.60555128,
                             11., 3.16227766, 3.16227766, 8., 3.16227766, 7.};


        MGridGen_nb_c = 15;
        MGridGen_col_ind_size = 51;
        v_MGridGen_volumes = {2., 1., 2., 1., 2., 1., 2., 1., 0.5, 2., 1., 3., 3., 3., 1.5};

        unordered_map<long, int> MGridGen_d_is_on_bnd{{0,  1},
                                                      {1,  1},
                                                      {3,  1},
                                                      {4,  1},
                                                      {12, 2},
                                                      {13, 1},
                                                      {14, 2}
        };

        s_MGridGen_is_on_corner = {};
        s_MGridGen_is_on_ridge = {12, 14};
        s_MGridGen_is_on_valley = {0, 1, 3, 4, 13};

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

#endif //COMMA_PROJECT_MGRIDGEN_DUAL_GRAPH_H
