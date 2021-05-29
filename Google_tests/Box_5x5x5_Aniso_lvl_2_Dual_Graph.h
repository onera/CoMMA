//
// Created by Nicolas Lantos on 03/04/2021.
//

#ifndef COMMA_PROJECT_BOX_FIVEXFIVEXFIVE_ANISO_LVL_2_DUAL_GRAPH_H
#define COMMA_PROJECT_BOX_FIVEXFIVEXFIVE_ANISO_LVL_2_DUAL_GRAPH_H

#include "gtest/gtest.h"
#include "../CoMMA_lib/Dual_graph.h"

class Box_5x5x5_Aniso_lvl_2_Dual_Graph : public ::testing::Test {
    //box_2_rects_5_d_F.hdf
protected:

    Dual_Graph *g;
    int box_5x5x5_number_of_cells;
    int box_5x5x5_number_of_anisotropic_compliant_fc;
    int box_5x5x5_col_ind_size;
    vector<long> v_box_5x5x5_row_ptr;
    vector<long> v_box_5x5x5_col_ind;
    vector<double> v_box_5x5x5_values;
    vector<double> v_box_5x5x5_volumes;
    unordered_set<long> box_5x5x5_s_anisotropic_compliant_cells;

    virtual void SetUp() {


        v_box_5x5x5_row_ptr = {0, 6, 12, 18, 24, 30, 36, 42, 48, 53, 58, 63, 68,
                               73, 78, 83, 88, 93, 98, 103, 108, 113, 118, 123, 128, 132,
                               136, 140, 144, 148, 152, 156, 160};

        v_box_5x5x5_col_ind = {0, 1, 2, 4, 9, 13, 0, 1, 3, 5, 8, 12, 0, 2, 3, 6, 11,
                               15, 1, 2, 3, 7, 10, 14, 0, 4, 5, 6, 17, 21, 1, 4, 5, 7,
                               16, 20, 2, 4, 6, 7, 19, 23, 3, 5, 6, 7, 18, 22, 1, 8, 9,
                               10, 24, 0, 8, 9, 11, 25, 3, 8, 10, 11, 26, 2, 9, 10, 11, 27,
                               1, 12, 13, 16, 24, 0, 12, 13, 17, 25, 3, 14, 15, 18, 26, 2, 14,
                               15, 19, 27, 5, 12, 16, 17, 28, 4, 13, 16, 17, 29, 7, 14, 18, 19,
                               30, 6, 15, 18, 19, 31, 5, 20, 21, 22, 28, 4, 20, 21, 23, 29, 7,
                               20, 22, 23, 30, 6, 21, 22, 23, 31, 8, 12, 24, 25, 9, 13, 24, 25,
                               10, 14, 26, 27, 11, 15, 26, 27, 16, 20, 28, 29, 17, 21, 28, 29, 18,
                               22, 30, 31, 19, 23, 30, 31};

        v_box_5x5x5_values = {25., 25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 5.,
                              25., 25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 25.,
                              25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 5., 25.,
                              25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 30., 25., 5.,
                              5., 5., 25., 30., 5., 5., 5., 5., 30., 25., 5., 5., 5.,
                              25., 30., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5., 5.,
                              5., 30., 25., 5., 5., 5., 25., 30., 5., 5., 5., 5., 30.,
                              25., 5., 5., 5., 25., 30., 5., 5., 5., 30., 25., 5., 5.,
                              5., 25., 30., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5.,
                              5., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5., 5., 5.,
                              35., 25., 5., 5., 25., 35., 5., 5., 35., 25., 5., 5., 25.,
                              35., 5., 5., 35., 25., 5., 5., 25., 35., 5., 5., 35., 25.,
                              5., 5., 25., 35.};

        box_5x5x5_number_of_cells = 32;
        box_5x5x5_col_ind_size = 160;

        v_box_5x5x5_volumes = {25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                               25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                               25., 25., 25., 25., 25., 25.};

        int box_5x5x5_is_on_bnd[64] = {1, 1, 1, 1,
                                       1, 1, 1, 1,
                                       2, 2, 2, 2,
                                       2, 2, 2, 2,
                                       2, 2, 2, 2,
                                       2, 2, 2, 2,
                                       3, 3, 3, 3,
                                       3, 3, 3, 3};

        box_5x5x5_number_of_anisotropic_compliant_fc = 32;
        box_5x5x5_s_anisotropic_compliant_cells = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                                   10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                                                   20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                                                   30, 31};

        unordered_map<long, int> box_5x5x5_d_is_on_bnd;
        for (int i = 0; i < box_5x5x5_number_of_cells; i++) {
            box_5x5x5_d_is_on_bnd[i] = box_5x5x5_is_on_bnd[i];
        }
        unordered_set<long> empty_set = unordered_set<long>({});
        g = new Dual_Graph(box_5x5x5_number_of_cells,
                           v_box_5x5x5_row_ptr,
                           v_box_5x5x5_col_ind,
                           v_box_5x5x5_values,
                           v_box_5x5x5_volumes,
                           box_5x5x5_d_is_on_bnd,
                           empty_set,
                           empty_set,
                           empty_set,
                           box_5x5x5_s_anisotropic_compliant_cells);

    }

    virtual void TearDown() {
        delete g;
//        delete v_box_5x5x5_row_ptr;
//        delete v_box_5x5x5_col_ind;
//        delete v_box_5x5x5_values;
    }
};

#endif //COMMA_PROJECT_BOX_FIVEXFIVEXFIVE_ANISO_LVL_2_DUAL_GRAPH_H
