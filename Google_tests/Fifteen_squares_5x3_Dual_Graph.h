//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_FIFTEEN_SQUARES_DUAL_GRAPH_H
#define COMMA_PROJECT_FIFTEEN_SQUARES_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_Graph.h"

class Fifteen_squares_5x3_Dual_Graph : public ::testing::Test {

protected:

    Dual_Graph *g;
    int Fifteen_squares_nb_c;
    int Fifteen_squares_col_ind_size;
    vector<long> v_Fifteen_squares_row_ptr;
    vector<long> v_Fifteen_squares_col_ind;
    vector<double> v_Fifteen_squares_values;
    vector<double> v_Fifteen_squares_volumes;
    unordered_set<long> s_Fifteen_squares_is_on_corner;
    unordered_set<long> s_Fifteen_squares_is_on_ridge;
    unordered_set<long> s_Fifteen_squares_is_on_valley;

    virtual void SetUp() {

        v_Fifteen_squares_row_ptr = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 40, 44, 47, 50, 54, 57};
        v_Fifteen_squares_col_ind = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4,
                                     5, 7, 2, 4, 5, 8, 3, 6, 7, 9, 4, 6, 7, 8, 10, 5, 7,
                                     8, 11, 6, 9, 10, 12, 7, 9, 10, 11, 8, 10, 11, 9, 12, 13, 10,
                                     12, 13, 14, 11, 13, 14};

        v_Fifteen_squares_values = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1,
                                    1., 1.1, 0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1,
                                    1., 1., 0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1,
                                    1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 0.1,
                                    1., 0.1, 1., 0.1, 1.};

        Fifteen_squares_nb_c = 15;
        Fifteen_squares_col_ind_size = 57;
        v_Fifteen_squares_volumes = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        unordered_map<long, int> Fifteen_squares_d_is_on_bnd{{0,  2},
                                                             {1,  1},
                                                             {2,  2},
                                                             {3,  1},
                                                             {5,  1},
                                                             {6,  1},
                                                             {8,  1},
                                                             {9,  1},
                                                             {10, 1},
                                                             {11, 1},
                                                             {12, 2},
                                                             {13, 1},
                                                             {14, 2}
        };

        s_Fifteen_squares_is_on_corner = {};
        s_Fifteen_squares_is_on_ridge = {0, 2, 12, 14};
        s_Fifteen_squares_is_on_valley = {1, 3, 5, 7, 6, 8, 9, 10, 11, 13};

        g = new Dual_Graph(Fifteen_squares_nb_c,
                           v_Fifteen_squares_row_ptr,
                           v_Fifteen_squares_col_ind,
                           v_Fifteen_squares_values,
                           v_Fifteen_squares_volumes,
                           Fifteen_squares_d_is_on_bnd,
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

#endif //COMMA_PROJECT_FIFTEEN_SQUARES_DUAL_GRAPH_H
