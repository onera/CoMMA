//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_EXAMPLE_ONE_DUAL_GRAPH_H
#define COMMA_PROJECT_EXAMPLE_ONE_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include"../CoMMA_lib/Dual_Graph.h"

class Example_One_Dual_Graph : public ::testing::Test {

protected:

    Dual_Graph *g;
    int Example_One_nb_c;
    int Example_One_col_ind_size;
    vector<long> v_Example_One_row_ptr;
    vector<long> v_Example_One_col_ind;
    vector<double> v_Example_One_values;
    vector<double> v_Example_One_volumes;
    unordered_set<long> s_Example_One_is_on_corner;
    unordered_set<long> s_Example_One_is_on_ridge;
    unordered_set<long> s_Example_One_is_on_valley;

    virtual void SetUp() {
        Example_One_nb_c = 9;
        Example_One_col_ind_size = 31;

        v_Example_One_row_ptr = {0, 3, 7, 10, 15, 18, 21, 25, 28, 31};
        v_Example_One_col_ind = {0, 1, 2, 0, 1, 2, 3, 0, 1, 2, 1, 3, 6, 7, 8, 4, 5, 6, 4, 5, 6, 3,
                                 4, 5, 6, 3, 7, 8, 3, 7, 8};

        v_Example_One_values = {6.82842712, 2., 2., 2., 4.23606798,
                                2., 1., 2., 2., 4.,
                                1., 4., 1., 1., 1.,
                                6.82842712, 1., 1., 1., 6.82842712,
                                1., 1., 1., 1., 5.,
                                1., 4., 1., 1., 1.,
                                4.};


        v_Example_One_volumes = {1., 1., 1., 1., 1., 1., 1., 1., 1.};

        unordered_map<long, int> Example_One_d_is_on_bnd{{0,  2},
                                                         {2,  2},
                                                         {4,  2},
                                                         {5,  2},
                                                         {7, 2},
                                                         {8, 2}
        };

        s_Example_One_is_on_corner = {};
        s_Example_One_is_on_ridge = {0, 2, 4, 5, 7, 8};
        s_Example_One_is_on_valley = {};

        g = new Dual_Graph(Example_One_nb_c,
                           v_Example_One_row_ptr,
                           v_Example_One_col_ind,
                           v_Example_One_values,
                           v_Example_One_volumes,
                           Example_One_d_is_on_bnd,
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

#endif //COMMA_PROJECT_EXAMPLE_ONE_DUAL_GRAPH_H
