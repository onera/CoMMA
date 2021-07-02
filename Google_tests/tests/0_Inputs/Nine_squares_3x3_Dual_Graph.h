//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_NINE_SQUARES_DUAL_GRAPH_H
#define COMMA_PROJECT_NINE_SQUARES_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../../../CoMMA_lib/Dual_Graph.h"

class Nine_squares_3x3_Dual_Graph : public ::testing::Test {

protected:

    Dual_Graph *g;
    int Nine_squares_nb_c;
    int Nine_squares_col_ind_size;
    vector<long> v_Nine_squares_row_ptr;
    vector<long> v_Nine_squares_col_ind;
    vector<double> v_Nine_squares_values;
    vector<double> v_Nine_squares_volumes;
    unordered_set<long> s_Nine_squares_is_on_corner;
    unordered_set<long> s_Nine_squares_is_on_ridge;
    unordered_set<long> s_Nine_squares_is_on_valley;

    virtual void SetUp() {

        v_Nine_squares_row_ptr = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
        v_Nine_squares_col_ind = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5,
                                  8, 3, 6, 7, 4, 6, 7, 8, 5, 7, 8};

        v_Nine_squares_values = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
                                 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};

        Nine_squares_nb_c = 9;
        Nine_squares_col_ind_size = 33;
        v_Nine_squares_volumes = {1, 1, 1, 1, 1, 1, 1, 1, 1};

        unordered_map<long, int> Nine_squares_d_is_on_bnd{{0, 2},
                                                          {1, 1},
                                                          {2, 2},
                                                          {3, 1},
                                                          {5, 1},
                                                          {6, 2},
                                                          {7, 1},
                                                          {8, 2}
        };

        s_Nine_squares_is_on_corner = {};
        s_Nine_squares_is_on_ridge = {0, 2, 6, 8};
        s_Nine_squares_is_on_valley = {1, 3, 5, 7};

        g = new Dual_Graph(Nine_squares_nb_c,
                           v_Nine_squares_row_ptr,
                           v_Nine_squares_col_ind,
                           v_Nine_squares_values,
                           v_Nine_squares_volumes,
                           Nine_squares_d_is_on_bnd,
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

#endif //COMMA_PROJECT_NINE_SQUARES_DUAL_GRAPH_H
