//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_TWELVE_SQUARES_V2_DUAL_GRAPH_H
#define COMMA_PROJECT_TWELVE_SQUARES_V2_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../../../../CoMMA_lib/Dual_Graph.h"

class Twelve_squares_4x3_v2_Dual_Graph : public ::testing::Test {

protected:

    Dual_Graph *g;
    int Twelve_squares_nb_c;
    int Twelve_squares_col_ind_size;
    vector<long> v_Twelve_squares_row_ptr;
    vector<long> v_Twelve_squares_col_ind;
    vector<double> v_Twelve_squares_values;
    vector<double> v_Twelve_squares_volumes;
    unordered_set<long> s_Twelve_squares_is_on_corner;
    unordered_set<long> s_Twelve_squares_is_on_ridge;
    unordered_set<long> s_Twelve_squares_is_on_valley;

    virtual void SetUp() {

        v_Twelve_squares_row_ptr = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
        v_Twelve_squares_col_ind = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4,
                                    5, 7, 2, 4, 5, 8, 3, 6, 7, 9, 4, 6, 7, 8, 10, 5, 7,
                                    8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};

        v_Twelve_squares_values = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1,
                                   1., 1.1, 0.1, 1., 0.1, 1.2, 1., 0.1, 1., 1., 1., 1., 0.1,
                                   1., 1.2, 0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1,
                                   1., 0.1, 1., 0.1, 1., 0.1, 1.};

        Twelve_squares_nb_c = 12;
        Twelve_squares_col_ind_size = 46;
        v_Twelve_squares_volumes = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        unordered_map<long, int> Twelve_squares_d_is_on_bnd{{0,  2},
                                                            {1,  1},
                                                            {2,  2},
                                                            {3,  1},
                                                            {5,  1},
                                                            {6,  1},
                                                            {8,  1},
                                                            {9,  2},
                                                            {10, 1},
                                                            {11, 2}
        };

        s_Twelve_squares_is_on_corner = {};
        s_Twelve_squares_is_on_ridge = {0, 2, 9, 11};
        s_Twelve_squares_is_on_valley = {1, 3, 5, 7, 6, 8, 10};

        g = new Dual_Graph(Twelve_squares_nb_c,
                           v_Twelve_squares_row_ptr,
                           v_Twelve_squares_col_ind,
                           v_Twelve_squares_values,
                           v_Twelve_squares_volumes,
                           Twelve_squares_d_is_on_bnd,
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

#endif //COMMA_PROJECT_TWELVE_SQUARES_V2_DUAL_GRAPH_H
