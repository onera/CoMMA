#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator_anisotropic.h"
#include "../CoMMA_lib/Coarse_Cell.h"
#include "gtest/gtest.h"
#include "MGridGen_Dual_Graph.h"

TEST_F(MGridGen_Dual_Graph, Coarse_Cell_init_Case_1_MGridGen) {

    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
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
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc;
    ref_d_compactness_to_s_fc[0] = s_fc_0;
    ASSERT_EQ(ref_d_compactness_to_s_fc, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);

    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    // i_cc 0 has two coarse neighbours i_cc 1 and i_cc 3 and in each case the edge cut is 1.
    // the inner fine cell at the interface is i_fc=3
    unordered_map<long, unordered_map<long, unordered_set<long>>> ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{3, {1, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{3, {{1, {{1, 1.0}}},
                                                                                                                                   {3, {{6, 2.2360679800000001}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[3][3][6], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[3][3][6], 1e-10);

    unordered_map<long, unordered_map<long, double>> ref__tmp_fc_fine_cut_edges = {{3, {{1, 1.0},
                                                                                               {6, 2.2360679800000001}}}};
    ASSERT_EQ(ref__tmp_fc_fine_cut_edges, v_cc[i_cc].__tmp_fc_fine_cut_edges);
    ASSERT_NEAR(ref__tmp_fc_fine_cut_edges[3][6], v_cc[i_cc].__tmp_fc_fine_cut_edges[3][6], 1e-10);

    // CC #1
    //==========
    i_cc = 1;
    unordered_set<long> s_fc_1 = {0, 1, 2};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_1));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_1 = {{1, {1, 2}},
                                                                                          {2, {0}}};
    ASSERT_EQ(ref_d_compactness_to_s_fc_1, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

//    self.assertEqual({1: {1: {0}, 2: {2, 3}}},
//                     l_cc[
//                             i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour)  # i_fc 3 has 2 neighbours outside i_cc 0
    // i_fc 3 has 2 neighbours outside i_cc 0
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{1, {0}},
                                                                          {2, {2, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    // CC #2
    //==========
    i_cc = 2;
    unordered_set<long> s_fc_2 = {4, 5, 7, 8};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_2 = {{2, {8, 4, 5, 7}}};
    ASSERT_EQ(ref_d_compactness_to_s_fc_2, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{5, {1}},
                                                                          {7, {4}},
                                                                          {8, {4}},
                                                                          }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    // CC #3
    //==========
    i_cc = 3;
    unordered_set<long> s_fc_3 = {6, 11, 13, 14};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_3 = {{1, {6, 13, 14}},
                                                                                          {3, {11}}};

    ASSERT_EQ(ref_d_compactness_to_s_fc_3, v_cc[i_cc].__d_compactness_to_s_fc);
    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);
//    {1: {6: {0, 1},
//         13: {4}}}
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{6, {0, 1}},
                                                                          {13, {4}}
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);


    // CC #4
    //==========
    i_cc = 4;
    unordered_set<long> s_fc_4 = {9, 10, 12};

    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_4 = {{1, {10, 12}},
                                                                                          {2, {9}}};

    ASSERT_EQ(ref_d_compactness_to_s_fc_4, v_cc[i_cc].__d_compactness_to_s_fc);
    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

//    {1: {9: {2},
//         10: {2, 3}}}
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{9, {2}},
                                                                          {10, {2,3}}
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

}



