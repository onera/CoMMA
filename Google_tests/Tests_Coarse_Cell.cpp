#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator_anisotropic.h"
#include "../CoMMA_lib/Coarse_Cell.h"
#include "gtest/gtest.h"
#include "MGridGen_Dual_Graph.h"

TEST_F(MGridGen_Dual_Graph, Coarse_Cell_init_Case_1_MGridGen) {

    long ref_fc_2_cc[15] = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    unsigned short int ref_cc_cardinality[5] = {1, 3, 4, 4, 3};
    unsigned short int ref_cc_compactness[5] = {0, 1, 2, 1, 1};
    double ref_cc_volumes[5] = {1.0, 5.0, 4.5, 9.5, 6.0};
    double ref_cc_boundary_area[5] = {4.0, 15.064495099999998, 11.828427120000001, 25.0, 17.0};

    vector<Coarse_Cell> v_cc;

    // CC #0
    //==========
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {3};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));

    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_NEAR(ref_cc_volumes[i_cc], v_cc[i_cc].volume, 1e-10);
    ASSERT_EQ(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area);
    ASSERT_TRUE(v_cc[i_cc].is_connected());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc;
    ref_d_compactness_to_s_fc[0] = s_fc_0;
    ASSERT_EQ(ref_d_compactness_to_s_fc, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);

//    l_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc)
//
//    self.assertEqual({1: {3: {1, 3}}},
//                     l_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour)
//    self.assertEqual({3: {1: {1: 1.0}, 3: {6: 2.23606797749979}}},
//                     l_cc[i_cc]._Coarse_Cell_v2__d_i_fc_to_j_cc_neighbourhood_to_j_fc)
//# print(l_cc[i_cc]._Coarse_Cell_v2__tmp_fc_fine_cut_edges.__repr__())
//    self.assertEqual({3: {1: 1.0, 6: 2.23606797749979}},
//                     l_cc[i_cc]._Coarse_Cell_v2__tmp_fc_fine_cut_edges)
//# print(l_cc[i_cc]._Coarse_Cell_v2__d_compactness_to_s_fc.__repr__())
//
//# i_cc 0 has two coarse neighbours i_cc 1 and i_cc 3 and in each case the edge cut is 1.
//# the inner fine cell at the interface is i_fc=3
//    self.assertEqual({1: {3: {1, 3}}},
//                     l_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour)

    // CC #1
    //==========
    i_cc = 1;
    unordered_set<long> s_fc_1 = {0, 1, 2};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_1));
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());
    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_1;
    unordered_set<long> s_1 = {1, 2};
    unordered_set<long> s_2 = {0};
    ref_d_compactness_to_s_fc_1[1] = s_1;
    ref_d_compactness_to_s_fc_1[2] = s_2;

    ASSERT_EQ(ref_d_compactness_to_s_fc_1, v_cc[i_cc].__d_compactness_to_s_fc);
    ASSERT_FALSE(v_cc[i_cc]._is_finalized);

    // CC #2
    //==========
    i_cc = 2;
    unordered_set<long> s_fc_2 = {4, 5, 7, 8};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());
    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_2;
    s_2 = {8, 4, 5, 7};
    ref_d_compactness_to_s_fc_2[2] = s_2;

    ASSERT_EQ(ref_d_compactness_to_s_fc_2, v_cc[i_cc].__d_compactness_to_s_fc);
    ASSERT_FALSE(v_cc[i_cc]._is_finalized);

    // CC #3
    //==========
    i_cc = 3;
    unordered_set<long> s_fc_3 = {6, 11, 13, 14};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_3;
    s_1 = {6, 13, 14};
    unordered_set<long> s_3 =  {11};
    ref_d_compactness_to_s_fc_3[1] = s_1;
    ref_d_compactness_to_s_fc_3[3] = s_3;

    ASSERT_EQ(ref_d_compactness_to_s_fc_3, v_cc[i_cc].__d_compactness_to_s_fc);
    ASSERT_FALSE(v_cc[i_cc]._is_finalized);

    // CC #4
    //==========
    i_cc = 4;
    unordered_set<long> s_fc_4 = {9, 10, 12};

    v_cc.push_back(Coarse_Cell((*g), i_cc,s_fc_4));

    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_4;
    s_1 = {10, 12};
    s_2 = {9};
    ref_d_compactness_to_s_fc_4[1] = s_1;
    ref_d_compactness_to_s_fc_4[2] = s_2;

    ASSERT_EQ(ref_d_compactness_to_s_fc_4, v_cc[i_cc].__d_compactness_to_s_fc);
    ASSERT_FALSE(v_cc[i_cc]._is_finalized);

}



