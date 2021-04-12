#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator_anisotropic.h"
#include "../CoMMA_lib/Coarse_Cell.h"
#include "gtest/gtest.h"
#include "MGridGen_Dual_Graph.h"
#include "MGridGen_ext_v2_Dual_Graph.h"
#include <iomanip>

TEST_F(MGridGen_Dual_Graph, Coarse_Cell_init_Case_1_MGridGen) {

    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    unsigned short int ref_cc_cardinality[5] = {1, 3, 4, 4, 3};
    unsigned short int ref_cc_compactness[5] = {0, 1, 2, 1, 1};
    double ref_cc_volumes[5] = {1.0, 5.0, 4.5, 9.5, 6.0};
    double ref_cc_boundary_area[5] = {4.0, 15.064495099999998, 11.828427120000001, 25.0, 17.0};

    vector<Coarse_Cell> v_cc;

    // CC N°0
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
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc;
    ref_d_compactness_to_s_fc[0] = s_fc_0;
    ASSERT_EQ(ref_d_compactness_to_s_fc, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);

    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    // i_cc 0 has two coarse neighbours i_cc 1 and i_cc 3 and in each case the edge cut is 1.
    // the inner fine cell at the interface is i_fc=3
    unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>> ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{3, {1, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{3, {{1, {{1, 1.0}}},
                                                                                                                                   {3, {{6, 2.2360679800000001}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[3][3][6], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[3][3][6], 1e-10);

    unordered_map<long, unordered_map<long, double>> ref__tmp_fc_fine_cut_edges = {{3, {{1, 1.0},
                                                                                               {6, 2.2360679800000001}}}};
    ASSERT_EQ(ref__tmp_fc_fine_cut_edges, v_cc[i_cc].__tmp_fc_fine_cut_edges);
    ASSERT_NEAR(ref__tmp_fc_fine_cut_edges[3][6], v_cc[i_cc].__tmp_fc_fine_cut_edges[3][6], 1e-10);

    // CC N°1
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

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_1 = {{1, {1, 2}},
                                                                                          {2, {0}}};
    ASSERT_EQ(ref_d_compactness_to_s_fc_1, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

    // i_fc 3 has 2 neighbours outside i_cc 0
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{1, {0}},
                                                                          {2, {2, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(s_fc_1, (*v_cc[i_cc].get_s_fc_v2()));

    // CC N°2
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

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_2 = {{2, {8, 4, 5, 7}}};
    ASSERT_EQ(ref_d_compactness_to_s_fc_2, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{5, {1}},
                                                                          {7, {4}},
                                                                          {8, {4}},
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(s_fc_2, (*v_cc[i_cc].get_s_fc_v2()));

    // CC N°3
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
    ASSERT_EQ(s_fc_3, (*v_cc[i_cc].get_s_fc_v2()));

    // CC N°4
    //==========
    i_cc = 4;
    unordered_set<long> s_fc_4 = {9, 10, 12};

    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_4 = {{1, {10, 12}},
                                                                                          {2, {9}}};

    ASSERT_EQ(ref_d_compactness_to_s_fc_4, v_cc[i_cc].__d_compactness_to_s_fc);
    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{9, {2}},
                                                                          {10, {2, 3}}
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(s_fc_4, (*v_cc[i_cc].get_s_fc_v2()));
}

TEST_F(MGridGen_Dual_Graph, Coarse_Cell_init_Case_1_MGridGen_v2) {
    // Creation of anisotropic cc

    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    unsigned short int ref_cc_cardinality[5] = {1, 3, 4, 4, 3};
    unsigned short int ref_cc_compactness[5] = {0, 1, 2, 1, 1};
    double ref_cc_volumes[5] = {1.0, 5.0, 4.5, 9.5, 6.0};
    double ref_cc_boundary_area[5] = {4.0, 15.064495099999998, 11.828427120000001, 25.0, 17.0};

    vector<Coarse_Cell> v_cc;

    // CC N°0
    //==========
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {3};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0,
                               false));

    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_NEAR(ref_cc_volumes[i_cc], v_cc[i_cc].volume, 1e-10);
    ASSERT_EQ(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area);
    ASSERT_TRUE(v_cc[i_cc].is_connected());
    ASSERT_FALSE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc;
    ref_d_compactness_to_s_fc[0] = s_fc_0;
    ASSERT_EQ(ref_d_compactness_to_s_fc, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);

    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    // i_cc 0 has two coarse neighbours i_cc 1 and i_cc 3 and in each case the edge cut is 1.
    // the inner fine cell at the interface is i_fc=3
    unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>> ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{3, {1, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{3, {{1, {{1, 1.0}}},
                                                                                                                                   {3, {{6, 2.2360679800000001}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[3][3][6], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[3][3][6], 1e-10);

    unordered_map<long, unordered_map<long, double>> ref__tmp_fc_fine_cut_edges = {{3, {{1, 1.0},
                                                                                               {6, 2.2360679800000001}}}};
    ASSERT_EQ(ref__tmp_fc_fine_cut_edges, v_cc[i_cc].__tmp_fc_fine_cut_edges);
    ASSERT_NEAR(ref__tmp_fc_fine_cut_edges[3][6], v_cc[i_cc].__tmp_fc_fine_cut_edges[3][6], 1e-10);
    ASSERT_EQ(s_fc_0, (*v_cc[i_cc].get_s_fc_v2()));

    // CC N°1
    //==========
    i_cc = 1;
    unordered_set<long> s_fc_1 = {0, 1, 2};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_1,
                               false));
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
    ASSERT_FALSE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

    // i_fc 3 has 2 neighbours outside i_cc 0
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{1, {0}},
                                                                          {2, {2, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(s_fc_1, (*v_cc[i_cc].get_s_fc_v2()));

    // CC N°2
    //==========
    i_cc = 2;
    unordered_set<long> s_fc_2 = {4, 5, 7, 8};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2,
                               false));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);
    ASSERT_TRUE(v_cc[i_cc].is_connected());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_2 = {{2, {8, 4, 5, 7}}};
    ASSERT_EQ(ref_d_compactness_to_s_fc_2, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ASSERT_FALSE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{5, {1}},
                                                                          {7, {4}},
                                                                          {8, {4}},
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(s_fc_2, (*v_cc[i_cc].get_s_fc_v2()));

    // CC N°3
    //==========
    i_cc = 3;
    unordered_set<long> s_fc_3 = {6, 11, 13, 14};

    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3,
                               false));
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
    ASSERT_FALSE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);
//    {1: {6: {0, 1},
//         13: {4}}}
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{6, {0, 1}},
                                                                          {13, {4}}
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(s_fc_3, (*v_cc[i_cc].get_s_fc_v2()));

    // CC N°4
    //==========
    i_cc = 4;
    unordered_set<long> s_fc_4 = {9, 10, 12};

    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_4,
                               false));
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
    ASSERT_FALSE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{9, {2}},
                                                                          {10, {2, 3}}
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(s_fc_4, (*v_cc[i_cc].get_s_fc_v2()));

}

TEST_F(MGridGen_Dual_Graph, is_connected_MGridGen) {

    vector<long> ref_fc_2_cc = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3};

    // CC N°0
    //==========
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {0, 1, 2};
    Coarse_Cell cc_0((*g), i_cc, s_fc_0);
    ASSERT_TRUE(cc_0.is_connected());

    // CC N°1
    //==========
    i_cc = 1;
    unordered_set<long> s_fc_1 = {3, 4, 5};

    Coarse_Cell cc_1((*g), i_cc, s_fc_1);
    ASSERT_FALSE(cc_1.is_connected());

    // CC N°2
    //==========
    i_cc = 2;
    unordered_set<long> s_fc_2 = {6, 7, 8};

    Coarse_Cell cc_2((*g), i_cc, s_fc_2);
    ASSERT_FALSE(cc_1.is_connected());

    // CC N°3
    //==========
    i_cc = 3;
    unordered_set<long> s_fc_3 = {9, 10, 11, 12, 13, 14};

    Coarse_Cell cc_3((*g), i_cc, s_fc_3);
    ASSERT_FALSE(cc_1.is_connected());

}

TEST_F(MGridGen_Dual_Graph, fill_cc_neighbouring_Case_1_MGridGen) {


    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};

    vector<Coarse_Cell> v_cc;

    //========================
    // Create coarse cell 0
    //========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {3};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);
    unordered_map<long, unordered_map<long, unordered_map<long, double>>> ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{3, {{1, {{1, 1.0}}},
                                                                                                                                   {3, {{6, 2.2360679800000001}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>> ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{3, {1, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //========================
    // Create coarse cell 1
    //========================
    i_cc = 1;
    unordered_set<long> s_fc_1 = {0, 1, 2};

    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_1));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{1, {{0, {{3, 1.0}}}}},
                                                 {2, {{2, {{5, 2.2360679800000001}}},
                                                             {3, {{6, 2.2360679800000001}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[2][3][6], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[2][3][6], 1e-10);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{1, {0}},
                                                                          {2, {2, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //========================
    // Create coarse cell 2
    //========================
    i_cc = 2;
    unordered_set<long> s_fc_2 = {4, 5, 7, 8};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{5, {{1, {{2,  2.2360679800000001}}}}},
                                                 {7, {{4, {{9,  2.2360679800000001}}}}},
                                                 {8, {{4, {{10, 1.4142135600000001}}}}},
    };

    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[5][1][2], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[5][1][2], 1e-10);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[7][4][9], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[7][4][9], 1e-10);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[8][4][10], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[8][4][10], 1e-10);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{5, {1}},
                                                                          {7, {4}},
                                                                          {8, {4}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //========================
    // Create coarse cell 3
    //========================

    i_cc = 3;
    unordered_set<long> s_fc_3 = {6, 11, 13, 14};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{6,  {{0, {{3,  2.2360679800000001}}},
                                                              {1, {{2, 2.2360679800000001}}}}},
                                                 {13, {{4, {{10, 3.16227766}}}}},

    };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[6][0][3], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[6][0][3], 1e-15);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[6][1][2], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[6][1][2], 1e-15);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[13][4][10], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[13][4][10], 1e-15);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{6, {0, 1}},
                                                                          {13, {4}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);


    //========================
    // Create coarse cell 4
    //========================
    i_cc = 4;
    unordered_set<long> s_fc_4 = {9, 10, 12};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{9,  {{2, {{7, 2.2360679800000001}}}}},
                                                 {10, {{2, {{8, 1.4142135600000001}}},
                                                              {3, {{13, 3.16227766}}}}},

    };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][2][7], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][2][7], 1e-15);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][2][8], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][2][8], 1e-15);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][3][13], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][3][13], 1e-15);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{9, {2}},
                                                                          {10, {2, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
}

TEST_F(MGridGen_Dual_Graph, fill_cc_neighbouring_Case_1_MGridGen_2) {

    vector<long> ref_fc_2_cc = {3, 3, 2, 3, 1, 2, 3, 1, 1, 0, 1, 4, 0, 4, 4};

    vector<Coarse_Cell> v_cc;

    //========================
    // Create coarse cell 0
    //========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {9, 12};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{9, {{1, {{7, 2.2360679800000001},
                                                                                                                                        {10, 2.0}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][1][7], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][1][7], 1e-10);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][1][10], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][1][10], 1e-10);
    unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>> ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{2, {{9, {1}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //========================
    // Create coarse cell 1
    //========================
    i_cc = 1;
    unordered_set<long> s_fc_1 = {4, 7, 8, 10};

    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_1));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{4,  {{2, {{5, 2.0}}}}},
                                                 {7,  {{0, {{9, 2.2360679800000001}}},}},
                                                 {8,  {{2, {{5, 1.0}}}}},
                                                 {10, {{0, {{9, 2.0}}},
                                                              {4, {{13, 3.16227766}}}}}
    };
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[4][2][5], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[4][2][5], 1e-10);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[7][0][9], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[7][0][9], 1e-10);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[8][2][5], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[8][2][5], 1e-10);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][0][9], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][0][9], 1e-10);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][4][13], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][4][13], 1e-10);

    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{4, {2}},
                                                                          {7, {0}},
                                                                          {8, {2}},
                                                                          {10, {0, 4}},
                                                                  }}};

    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //========================
    // Create coarse cell 2
    //========================
    i_cc = 2;
    unordered_set<long> s_fc_2 = {2, 5};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);
//    ASSERT_EQ({2: {
//                         3: {0: 2.0,
//                             6: 2.23606797749979}
//                          },
//                     5: { 1: { 4: 2.0,
//                               8: 1.0}
//                     }},
//                     l_cc[i_cc]._Coarse_Cell_v2__d_i_fc_to_j_cc_neighbourhood_to_j_fc)

    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{2, {{3, {{0, 2.0},
                                                                  {6, 2.2360679800000001}
                                                          }}}},
                                                 {5, {{1, {{4, 2.0},
                                                                  {8, 1.0},
                                                          }}}}
    };

//    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[2][3][0], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[2][3][0], 1e-10);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[2][3][6], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[2][3][6], 1e-10);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[5][1][4], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[5][1][4], 1e-10);
    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[5][1][8], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[5][1][8], 1e-10);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{2, {{2, {3}},
                                                                          {5, {1}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //========================
    // Create coarse cell 3
    //========================

    i_cc = 3;
    unordered_set<long> s_fc_3 = {0, 1, 3, 6};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);

    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {
            {0, {{2, {{2, 2.0}}}}},
            {6, {{2, {{2, 2.2360679800000001}}},
                        {4, {{11, 2.0}}}}},
    };
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[6][2][2], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[6][2][2], 1e-15);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[6][1][2], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[6][1][2], 1e-15);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[13][4][10], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[13][4][10], 1e-15);
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{0, {2}},
                                                                          {6, {2, 4}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //========================
    // Create coarse cell 4
    //========================
    i_cc = 4;
    unordered_set<long> s_fc_4 = {11, 13, 14};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_TRUE(v_cc[i_cc].__is_cc_neighbourhood_build);
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{11, {{3, {{6,  2.0}}}}},
                                                 {13, {{1, {{10, 3.16227766}}}}},

    };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][2][7], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][2][7], 1e-15);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][2][8], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][2][8], 1e-15);
//    ASSERT_NEAR(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][3][13], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[10][3][13], 1e-15);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{11, {3}},
                                                                          {13, {1}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
}

TEST_F(MGridGen_Dual_Graph, get_s_cc_neighbours_Case_1_MGridGen) {
    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    vector<Coarse_Cell> v_cc;

    //========================
    // Create coarse cell 0
    //========================

    long i_cc = 0;
    unordered_set<long> s_fc_0 = {3};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> s_cc = v_cc[i_cc].get_s_cc_neighbours();
    ASSERT_EQ(unordered_set<long>({1, 3}), s_cc);

    //========================
    // Create coarse cell 1
    //========================
    i_cc = 1;
    unordered_set<long> s_fc_1 = {0, 1, 2};

    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_1));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    s_cc = v_cc[i_cc].get_s_cc_neighbours();
    ASSERT_EQ(unordered_set<long>({0, 2, 3}), s_cc);

    //========================
    // Create coarse cell 2
    //========================
    i_cc = 2;
    unordered_set<long> s_fc_2 = {4, 5, 7, 8};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    s_cc = v_cc[i_cc].get_s_cc_neighbours();
    ASSERT_EQ(unordered_set<long>({1, 4}), s_cc);

    //========================
    // Create coarse cell 3
    //========================

    i_cc = 3;
    unordered_set<long> s_fc_3 = {6, 11, 13, 14};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    s_cc = v_cc[i_cc].get_s_cc_neighbours();
    ASSERT_EQ(unordered_set<long>({0, 1, 4}), s_cc);

    //========================
    // Create coarse cell 4
    //========================
    i_cc = 4;
    unordered_set<long> s_fc_4 = {9, 10, 12};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    s_cc = v_cc[i_cc].get_s_cc_neighbours();
    ASSERT_EQ(unordered_set<long>({2, 3}), s_cc);

}

TEST_F(MGridGen_Dual_Graph, compute_s_leaves_Case_1_MGridGen) {

    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    vector<Coarse_Cell> v_cc;


    //========================
    // Create coarse cell 0
    //========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {3};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    ASSERT_EQ(unordered_set<long>({3}), v_cc[i_cc].compute_s_leaves());

    //========================
    // Create coarse cell 1
    //========================
    i_cc = 1;
    unordered_set<long> s_fc_1 = {0, 1, 2};

    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_1));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(unordered_set<long>({1, 2}), v_cc[i_cc].compute_s_leaves());

    //========================
    // Create coarse cell 2
    //========================
    i_cc = 2;
    unordered_set<long> s_fc_2 = {4, 5, 7, 8};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(unordered_set<long>({}), v_cc[i_cc].compute_s_leaves());

    //========================
    // Create coarse cell 3
    //========================

    i_cc = 3;
    unordered_set<long> s_fc_3 = {6, 11, 13, 14};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(unordered_set<long>({6, 13, 14}), v_cc[i_cc].compute_s_leaves());

    //========================
    // Create coarse cell 4
    //========================
    i_cc = 4;
    unordered_set<long> s_fc_4 = {9, 10, 12};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(unordered_set<long>({10, 12}), v_cc[i_cc].compute_s_leaves());
}

TEST_F(MGridGen_Dual_Graph, add_fc_Case_1_MGridGen) {

    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    unsigned short int ref_cc_cardinality[5] = {1, 3, 4, 4, 3};
    unsigned short int ref_cc_compactness[5] = {0, 1, 2, 1, 1};
    double ref_cc_volumes[5] = {1.0, 5.0, 4.5, 9.5, 6.0};
    double ref_cc_boundary_area[5] = {4.0, 15.064495099999998, 11.828427120000001, 25.0, 17.0};


//    vector<long> fc_2_cc((*g).number_of_cells, -1);
    vector<long> fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    vector<Coarse_Cell> v_cc;

    //========================
    // Create coarse cell 0
    //========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {};
    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_0));

    long i_source_cc = -1;
    long i_dest_cc = 0;
    unordered_set<long> s_fc = {3};

    v_cc[i_dest_cc].add_fc(s_fc, fc_2_cc);

    // Checks:
    //=======
    unordered_map<long, unordered_map<long, double>> ref_d_def;
    ASSERT_EQ(ref_d_def, v_cc[i_cc].__d_def);
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_NEAR(ref_cc_volumes[i_cc], v_cc[i_cc].volume, 1e-10);
    ASSERT_EQ(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area);
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);
    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ASSERT_FALSE(v_cc[i_dest_cc].__is_connectivity_up_to_date);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc = {{0, unordered_set<long>({3})}};
    ASSERT_EQ(ref_d_compactness_to_s_fc, v_cc[i_cc].__d_compactness_to_s_fc);

    v_cc[i_dest_cc].fill_cc_neighbouring(fc_2_cc);
    for (const long &i_fc: s_fc) {
        fc_2_cc[i_fc] = i_dest_cc;
    }

    ASSERT_TRUE(v_cc[i_dest_cc].is_connected());
    ASSERT_EQ(unordered_set<long>({3}), v_cc[i_dest_cc].compute_s_leaves());
    ASSERT_EQ(unordered_set<long>({3}), v_cc[i_dest_cc].get_s_fc_w_outer_neighbours());

    unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>> ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{3, {1, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{3, {{1, {{1, 1.0}}},
                                                                                                                                   {3, {{6, 2.2360679800000001}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ASSERT_TRUE(v_cc[i_dest_cc].check_consistency(fc_2_cc));

    unordered_map<long, unordered_map<long, double>> ref__tmp_fc_fine_cut_edges = {{3, {{1, 1.0},
                                                                                               {6, 2.2360679800000001}}}};
    ASSERT_EQ(ref__tmp_fc_fine_cut_edges, v_cc[i_cc].__tmp_fc_fine_cut_edges);

    //========================
    // Create coarse cell 1
    //========================
    i_cc = 1;
    unordered_set<long> s_fc_1 = {0};
    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_1));
    fc_2_cc[0] = i_cc;

    ASSERT_EQ(1, v_cc[i_cc].__card);
    ASSERT_EQ(0, v_cc[i_cc].__compactness);

    ref__tmp_fc_fine_cut_edges = {{0, {{1, 2.0},
                                              {2, 2.0}}}};
    ASSERT_EQ(ref__tmp_fc_fine_cut_edges, v_cc[i_cc].__tmp_fc_fine_cut_edges);
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    // Add N°1
    i_source_cc = -1;
    i_dest_cc = 1;
    s_fc = {1};

    v_cc[i_dest_cc].add_fc(s_fc, fc_2_cc = fc_2_cc);
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    for (const long &i_fc: s_fc) {
        fc_2_cc[i_fc] = i_dest_cc;
    }
    ASSERT_EQ(2, v_cc[i_cc].__card);
    ASSERT_EQ(1, v_cc[i_cc].__compactness);

    // Add N°2
    s_fc = {2};
    v_cc[i_dest_cc].add_fc(s_fc, fc_2_cc = fc_2_cc);
    for (const long &i_fc: s_fc) {
        fc_2_cc[i_fc] = i_dest_cc;
    }

    //========
    // Checks
    //========
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_1 = {{1, {1, 2}},
                                                                                          {2, {0}}};
    ASSERT_EQ(ref_d_compactness_to_s_fc_1, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
    ASSERT_FALSE(v_cc[i_cc].__is_delayed);

    // i_fc 3 has 2 neighbours outside i_cc 0
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{1, {0}},
                                                                          {2, {2, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(unordered_set<long>({0, 1, 2}), (*v_cc[i_cc].get_s_fc_v2()));

    //========================
    // Create coarse cell 2
    //========================
    i_cc = 2;
    unordered_set<long> s_fc_2 = {4};
    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_2));
    fc_2_cc[4] = i_cc;

    ASSERT_EQ(1, v_cc[i_cc].__card);
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    // Add N°1
    i_source_cc = -1;
    i_dest_cc = 2;
    s_fc = {5, 7};

    v_cc[i_dest_cc].add_fc(s_fc, fc_2_cc = fc_2_cc);

    for (const long &i_fc: s_fc) {
        fc_2_cc[i_fc] = i_dest_cc;
    }
    ASSERT_EQ(3, v_cc[i_cc].__card);

    // Add N°2
    s_fc = {8};
    v_cc[i_dest_cc].add_fc(s_fc, fc_2_cc = fc_2_cc);
    for (const long &i_fc: s_fc) {
        fc_2_cc[i_fc] = i_dest_cc;
    }

    //========
    // Checks
    //========
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_to_s_fc_2 = {{2, {8, 4, 5, 7}}};
    ASSERT_EQ(ref_d_compactness_to_s_fc_2, v_cc[i_cc].__d_compactness_to_s_fc);

    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{5, {1}},
                                                                          {7, {4}},
                                                                          {8, {4}},
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
    ASSERT_EQ(unordered_set<long>({4, 5, 7, 8}), (*v_cc[i_cc].get_s_fc_v2()));


    //========================
    // Create coarse cell 3
    //========================
    i_cc = 3;
    unordered_set<long> s_fc_3 = {6, 11};
    v_cc.push_back(Coarse_Cell((*g), i_cc, s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    fc_2_cc[6] = i_cc;
    fc_2_cc[11] = i_cc;
    ASSERT_EQ(2, v_cc[i_cc].__card);

    // Add N°1
    i_source_cc = -1;
    i_dest_cc = 3;
    s_fc = {13, 14};

    v_cc[i_dest_cc].add_fc(s_fc, fc_2_cc = fc_2_cc);

    for (const long &i_fc: s_fc) {
        fc_2_cc[i_fc] = i_dest_cc;
    }

    ASSERT_EQ(ref_cc_cardinality[i_cc], v_cc[i_cc].__card);
    ASSERT_EQ(ref_cc_compactness[i_cc], v_cc[i_cc].__compactness);
    ASSERT_EQ(ref_cc_volumes[i_cc], v_cc[i_cc].volume);
    ASSERT_NEAR(ref_cc_boundary_area[i_cc], v_cc[i_cc].__boundary_area, 1e-10);

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
    ASSERT_EQ(unordered_set<long>({6, 11, 13, 14}), (*v_cc[i_cc].get_s_fc_v2()));
}

TEST_F(MGridGen_Dual_Graph, add_fc_scenario_Case_1_MGridGen) {

//    scenario = [['A', -1, 0, {3}, 1, 0, True, {3}, {3}],
//    ['A', -1, 0, {6}, 2, 1, True, {3, 6}, {3, 6}],
//    ['A', -1, 0, {0, 1, 2}, 5, 2, True, set(), {2, 6}],
//    ['A', -1, 0, {14}, 6, 0, False, {14}, {2, 6, 14}],
//    ['A', -1, 0, {11, 13}, 8, 1, True, {13, 14}, {2, 13}],
//    ['A', -1, 0, {5}, 9, 1, True, {5, 13, 14}, {5, 13}],
//    ['A', -1, 0, {8, 10}, 11, 1, True, {14}, {5, 8, 10, }],
//    ['A', -1, 0, {9, 12}, 13, 1, True, {12, 14}, {5, 8, 9}],
//    ['A', -1, 0, {7, 4}, 15, 1, True, {12, 14}, set()],
//    ]

    vector<long> fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};

    vector<unordered_set<long>> v_s_fc = {unordered_set<long>({3}),
                                          unordered_set<long>({6}),
                                          unordered_set<long>({0, 1, 2}),
                                          unordered_set<long>({14}),
                                          unordered_set<long>({11, 13}),
                                          unordered_set<long>({5}),
                                          unordered_set<long>({8, 10}),
                                          unordered_set<long>({9, 12}),
                                          unordered_set<long>({7, 4})
    };
    vector<unsigned short int> v_cardinality = {1, 2, 5, 6, 8, 9, 11, 13, 15};
    vector<unsigned short int> v_compactness = {0, 1, 2, 0, 1, 1, 1, 1, 1};
    vector<bool> v_connectivity = {true, true, true, false, true, true, true, true, true};

    vector<unordered_set<long>> v_s_leaves = {unordered_set<long>({3}),
                                              unordered_set<long>({3, 6}),
                                              unordered_set<long>({}),
                                              unordered_set<long>({14}),
                                              unordered_set<long>({13, 14}),
                                              unordered_set<long>({5, 13, 14}),
                                              unordered_set<long>({14}),
                                              unordered_set<long>({12, 14}),
                                              unordered_set<long>({12, 14})
    };

    vector<unordered_set<long>> v_s_fc_w_outer_neighbours = {unordered_set<long>({3}),
                                                             unordered_set<long>({3, 6}),
                                                             unordered_set<long>({2, 6}),
                                                             unordered_set<long>({2, 6, 14}),
                                                             unordered_set<long>({2, 13}),
                                                             unordered_set<long>({5, 13}),
                                                             unordered_set<long>({5, 8, 10}),
                                                             unordered_set<long>({5, 8, 9}),
                                                             unordered_set<long>({})
    };

    vector<unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>>> v_d_outer = {{{1, {{3,  {1, 3}}}}},
            // 1
                                                                                                     {{1, {{3,  {1}},
                                                                                                                      {6,  {1, 3}}}}},
            // 2
                                                                                                     {{1, {{2,  {2}},
                                                                                                                      {6,  {3}}}}},
            // 3
                                                                                                     {{1, {{2,  {2}},
                                                                                                                      {6,  {3}},
                                                                                                                                 {14, {3}}
                                                                                                          }}},
            // 4
                                                                                                     {{1, {{2,  {2}},
                                                                                                                      {13, {4}}}}
                                                                                                     },
            //5
                                                                                                     {{1, {{13, {4}}}}, {2, {{5, {2}}}}},
            //6
                                                                                                     {{1, {{5,  {2}}, {10, {4}}, {8,  {2}}}}},
            //7

                                                                                                     {{1, {{5,  {2}}, {8,  {2}}, {9,  {2}}}}},
            //8

                                                                                                     {}

    };
    vector<unordered_map<long, unordered_map<long, unordered_map<long, double>>>> v_d_i_fc_to_j_cc = {{{3,  {{1, {{1,  1.0}}},
                                                                                                                    {3, {{6, 2.2360679800000001}}}}}},
            // 1
                                                                                                      {{3,  {{1, {{1,  1.0}}}}},
                                                                                                                                                {6,  {{3, {{11, 2.0}}},
                                                                                                                                                             {1, {{2, 2.2360679800000001}}}}},
                                                                                                      },
            // 2
                                                                                                      {{2,  {{2, {{5,  2.2360679800000001}}}}},
                                                                                                                                                {6,  {{3, {{11, 2.0}}}}},
                                                                                                      },
            // 3
                                                                                                      {{2,  {{2, {{5,  2.2360679800000001}}}}},
                                                                                                                                                {6,  {{3, {{11, 2.0}}}}},
                                                                                                                                                                        {14, {{3, {{11, 3.16227766}}}}},
                                                                                                      },
            // 4
                                                                                                      {{13, {{4, {{10, 3.16227766}}}}},
                                                                                                                                                {2,  {{2, {{5,  2.2360679800000001}}}}}},
            // 5
                                                                                                      {{13, {{4, {{10, 3.16227766}}}}},
                                                                                                                                                {5,  {{2, {{8,  1.0},
                                                                                                                                                                  {4, 2.0}}}}}},
            // 6
                                                                                                      {{8,  {{2, {{7,  1}}}}},                  {10, {{4, {{9,  2}}}}}, {5,  {{2, {{4,  2}}}}}},
                                                                                                      {{9,  {{2, {{7,  2.2360679800000001}}}}}, {8,  {{2, {{7,  1}}}}}, {5,  {{2, {{4,  2}}}}}},
                                                                                                      {}
    };

    vector<unordered_map<long, unordered_map<long, double>>> v_tmp_fc_fine_cut_edges = {{{3,  {{1,  1.0},
                                                                                                        {6, 2.2360679800000001}}}},
            // 1
                                                                                        {{3,  {{1,  1.0}}},
                                                                                                                           {6,  {{11, 2.0},
                                                                                                                                        {2, 2.2360679800000001}}}
                                                                                        },
            // 2
                                                                                        {{2,  {{5,  2.2360679800000001}}},
                                                                                                                           {6,  {{11, 2.0}}}
                                                                                        },
            // 3
                                                                                        {{2,  {{5,  2.2360679800000001}}},
                                                                                                                           {6,  {{11, 2.0}}},
                                                                                                                                            {14, {{11, 3.16227766}}}
                                                                                        },
            // 4
                                                                                        {{13, {{10, 3.16227766}}},         {2,  {{5,  2.2360679800000001}}}},

            // 5
                                                                                        {{5,  {{8,  1}, {4, 2}}},          {13, {{10, 3.16227766}}}},
            // 6
                                                                                        {{8,  {{7,  1}}},                  {10, {{9,  2}}}, {5,  {{4,  2}}}},
            // 7
                                                                                        {{9,  {{7,  2.2360679800000001}}}, {8,  {{7,  1}}}, {5,  {{4,  2}}}},
            // 8
                                                                                        {}

    };

//========================
// Create coarse cell 0
//========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {};
    Coarse_Cell cc((*g), i_cc, s_fc_0);

    for (int i = 0; i < 9; i++) {
//        cout << "\ni= " << i << endl;
        cc.add_fc(v_s_fc[i], fc_2_cc = fc_2_cc);
        cc.fill_cc_neighbouring(fc_2_cc);
        for (const long &i_fc : v_s_fc[i]) {
            fc_2_cc[i_fc] = 0;
        }
        ASSERT_EQ(v_cardinality[i], cc.__card);
        ASSERT_EQ(v_compactness[i], cc.__compactness);
        ASSERT_EQ(v_connectivity[i], cc.is_connected());
        ASSERT_EQ(v_s_leaves[i], cc.compute_s_leaves());
        ASSERT_EQ(v_s_fc_w_outer_neighbours[i], cc.get_s_fc_w_outer_neighbours());
        ASSERT_EQ(v_d_i_fc_to_j_cc[i], cc.__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
        ASSERT_EQ(v_d_outer[i], cc.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
        ASSERT_EQ(v_tmp_fc_fine_cut_edges[i], cc.__tmp_fc_fine_cut_edges);
    }
}

TEST_F(MGridGen_Dual_Graph, get_s_fc_w_outer_neighbours_Case_1_MGridGen) {

    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};

    vector<Coarse_Cell> v_cc;

    //========================
    // Create coarse cell 0
    //========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {3};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> empty_set = unordered_set<long>({});

    ASSERT_EQ(s_fc_0, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(s_fc_0, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(s_fc_0, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));

    //========================
    // Create coarse cell 1
    //========================
    i_cc = 1;
    unordered_set<long> s_fc_1 = {0, 1, 2};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_1));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> ref_s_fc_1 = unordered_set<long>({1, 2});
    ASSERT_EQ(ref_s_fc_1, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_1, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_1, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));

    //========================
    // Create coarse cell 2
    //========================
    i_cc = 2;
    unordered_set<long> s_fc_2 = {4, 5, 7, 8};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    unordered_set<long> ref_s_fc_2 = unordered_set<long>({8, 5, 7});
    ASSERT_EQ(ref_s_fc_2, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_2, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_2, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));

    //========================
    // Create coarse cell 3
    //========================
    i_cc = 3;
    unordered_set<long> s_fc_3 = {6, 11, 13, 14};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> ref_s_fc_3 = unordered_set<long>({6, 13});
    ASSERT_EQ(ref_s_fc_3, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_3, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_3, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));

    //========================
    // Create coarse cell 4
    //========================
    i_cc = 4;
    unordered_set<long> s_fc_4 = {9, 10, 12};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> ref_s_fc_4 = unordered_set<long>({10, 9});
    ASSERT_EQ(ref_s_fc_4, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_4, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_4, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));
}

TEST_F(MGridGen_Dual_Graph, get_s_fc_w_outer_neighbours_Case_1_MGridGen_2) {

    vector<long> ref_fc_2_cc = {3, 3, 2, 3, 1, 2, 3, 1, 1, 0, 1, 4, 0, 4, 4};

    vector<Coarse_Cell> v_cc;

    //========================
    // Create coarse cell 0
    //========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {9, 12};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> empty_set = unordered_set<long>({});
    unordered_set<long> ref_s_fc_0 = unordered_set<long>({9});
    ASSERT_EQ(ref_s_fc_0, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_0, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_0, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(ref_s_fc_0, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(3));

    //========================
    // Create coarse cell 1
    //========================
    i_cc = 1;
    unordered_set<long> s_fc_1 = {4, 7, 8, 10};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_1));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> ref_s_fc_1 = unordered_set<long>({8, 10, 4, 7});

    ASSERT_EQ(ref_s_fc_1, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_1, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_1, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));

    //========================
    // Create coarse cell 2
    //========================
    i_cc = 2;
    unordered_set<long> s_fc_2 = {2, 5};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> ref_s_fc_2 = unordered_set<long>({2, 5});

    ASSERT_EQ(ref_s_fc_2, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_2, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_2, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(ref_s_fc_2, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(3));

    //========================
    // Create coarse cell 3
    //========================
    i_cc = 3;
    unordered_set<long> s_fc_3 = {0, 1, 3, 6};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> ref_s_fc_3 = unordered_set<long>({0, 6});

    ASSERT_EQ(ref_s_fc_3, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_3, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_3, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));

    //========================
    // Create coarse cell 4
    //========================
    i_cc = 4;
    unordered_set<long> s_fc_4 = {11, 13, 14};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    unordered_set<long> ref_s_fc_4 = unordered_set<long>({11, 13});

    ASSERT_EQ(ref_s_fc_4, v_cc[i_cc].get_s_fc_w_outer_neighbours());
    ASSERT_EQ(ref_s_fc_4, v_cc[i_cc].get_s_fc_w_outer_neighbours(0));
    ASSERT_EQ(ref_s_fc_4, v_cc[i_cc].get_s_fc_w_outer_neighbours(1));
    ASSERT_EQ(empty_set, v_cc[i_cc].get_s_fc_w_outer_neighbours(2));

}


TEST_F(MGridGen_Dual_Graph, remove_fc_Case_1_MGridGen) {

// add or remove, from i_cc, to i_cc, s_fc, card, compactness, is_connected?, s_leaves, s_fc_outer_degree
//scenario = [['A', -1, 0, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, 15, 1, True, {12, 14}, set()],
//['R', 0, 1, {6},           14, 1, True, {3, 12, 14}, {3, 2, 11}],
//['R', 0, 2, {12},          13, 1, True, {3, 14}, {3, 2, 9, 11}],
//['R', 0, 5, {14},          12, 1, True, {3, 11}, {3, 2, 9, 11}],
//['R', 0, 3, {0, 1, 2, 3, }, 8, 1, True, {11}, {11, 5, 9}],
//['R', 0, 4, {11, 13},       6, 2, True, set(), {5, 10, 9}],
//]
    vector<long> v_i_dest_cc = {1, 2, 5, 3, 4};
    vector<unordered_set<long>> v_s_fc = {unordered_set<long>({6}),
                                          unordered_set<long>({12}),
                                          unordered_set<long>({14}),
                                          unordered_set<long>({0, 1, 2, 3}),
                                          unordered_set<long>({11, 13}),
    };
    vector<long> ref_fc_2_cc = {1, 1, 1, 1, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};

    unsigned short int ref_cc_cardinality[6] = {14, 13, 12, 8, 6};
    unsigned short int ref_cc_compactness[6] = {1, 1, 1, 1, 2};
    vector<bool> v_connectivity = {true, true, true, true, true};
    double ref_cc_volumes[5] = {1.0, 5.0, 4.5, 9.5, 6.0};
    double ref_cc_boundary_area[5] = {4.0, 15.064495099999998, 11.828427120000001, 25.0, 17.0};

    vector<unordered_set<long>> v_s_leaves = {unordered_set<long>({3, 14, 12}),
                                              unordered_set<long>({3, 14}),
                                              unordered_set<long>({3, 11}),
                                              unordered_set<long>({11}),
                                              unordered_set<long>({})
    };

    vector<unordered_set<long>> v_s_fc_w_outer_neighbours = {unordered_set<long>({2, 3, 11}),
                                                             unordered_set<long>({2, 3, 9, 11}),
                                                             unordered_set<long>({2, 3, 9, 11}),
                                                             unordered_set<long>({5, 9, 11}),
                                                             unordered_set<long>({5, 9, 10})
    };

    vector<unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>>> v_d_outer = {{{1, {{2,  {1}}, {3,  {1}}, {11, {1}}}}},
            // 1
                                                                                                     {{1, {{2,  {1}}, {3,  {1}}, {11, {1}},    {9, {2}}}}},
            // 2
                                                                                                     {{1, {{2,  {1}}, {3,  {1}}, {11, {1, 5}}, {9, {2}}}}},
            // 3
                                                                                                     {{1, {{9,  {2}},
                                                                                                                      {11, {5, 1}},
                                                                                                                                 {5,  {3}}
                                                                                                          }}},
            // 4
                                                                                                     {{1, {{9,  {2}},
                                                                                                                      {5,  {3}},
                                                                                                                                 {10, {4}}}}
                                                                                                     },
            //5
                                                                                                     {{1, {{13, {4}}}}, {2, {{5, {2}}}}}
    };

    vector<unordered_map<long, unordered_map<long, unordered_map<long, double>>>> v_d_i_fc_to_j_cc = {{{11, {{1, {{6,  2}}}}},
                                                                                                                                                {3,  {{1, {{6, 2.2360679800000001}}}}},
                                                                                                                                                                                        {2, {{1, {{6,  2.2360679800000001}}}}}},
            // 1
                                                                                                      {{9,  {{2, {{12, 3.605551280000000}}}}},
                                                                                                                                                {11, {{1, {{6, 2}}}}},
                                                                                                                                                                                        {3, {{1, {{6,  2.2360679800000001}}}}},
                                                                                                              {2, {{1, {{6, 2.2360679800000001}}}}}},
            // 2
                                                                                                      {{9,  {{2, {{12, 3.605551280000000}}}}},
                                                                                                                                                {11, {{1, {{6, 2}}},
                                                                                                                                                             {5, {{14, 3.16227766}}}}},
                                                                                                                                                                                        {3, {{1, {{6,  2.2360679800000001}}}}},
                                                                                                              {2, {{1, {{6, 2.2360679800000001}}}}}},
            // 3
                                                                                                      {{9,  {{2, {{12, 3.605551280000000}}}}},
                                                                                                                                                {11, {{1, {{6, 2}}},
                                                                                                                                                             {5, {{14, 3.16227766}}}}},
                                                                                                                                                                                        {5, {{3, {{2,  2.2360679800000001}}}}},
                                                                                                      },
            // 4
                                                                                                      {{10, {{4, {{13, 3.16227766}}}}},         {5,  {{3, {{2, 2.2360679800000001}}}}}, {9, {{2, {{12, 3.605551280000000}}}}}},
            // 5
                                                                                                      {{13, {{4, {{10, 3.16227766}}}}},
                                                                                                                                                {5,  {{2, {{8, 1.0},
                                                                                                                                                                  {4, 2.0}}}}}},
            // 6
                                                                                                      {{8,  {{2, {{7,  1}}}}},                  {10, {{4, {{9, 2}}}}},                  {5, {{2, {{4,  2}}}}}},
                                                                                                      {{9,  {{2, {{7,  2.2360679800000001}}}}}, {8,  {{2, {{7, 1}}}}},                  {5, {{2, {{4,  2}}}}}},
                                                                                                      {}
    };

    vector<unordered_map<long, unordered_map<long, double>>> v_tmp_fc_fine_cut_edges = {{{11, {{6,  2}}},                  {3,  {{6,  2.2360679800000001}}},           {2, {{6,  2.2360679800000001}}}},
            // 1
                                                                                        {{9,  {{12, 3.605551280000000}}},  {11, {{6,  2}}},                            {3, {{6,  2.2360679800000001}}}, {2, {{6, 2.2360679800000001}}}},
            // 2
                                                                                        {{9,  {{12, 3.605551280000000}}},  {11, {{6,  2},
                                                                                                                                                   {14, 3.16227766}}}, {3, {{6,  2.2360679800000001}}}, {2, {{6, 2.2360679800000001}}}},
            // 3
                                                                                        {{5,  {{2,  2.2360679800000001}}}, {11, {{14, 3.16227766}, {6,  2}}},          {9, {{12, 3.605551280000000}}}},
            // 4
                                                                                        {{10, {{13, 3.16227766}}},
                                                                                                                           {5,  {{2,  2.2360679800000001}}},
                                                                                                                                                                       {9, {{12, 3.605551280000000}}}
                                                                                        }
    };

    vector<Coarse_Cell> v_cc;

    //========================
    // Create coarse cell 0
    //========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);
    for (const long &i_fc: s_fc_0) {
        ref_fc_2_cc[i_fc] = 0;
    }
    long i_dest_cc = 1;
    //========================
    // Remove some cells: step 1
    //========================
    for (int i = 0; i < 5; i++) {
//        cout << "\ni= " << i << endl;

        i_dest_cc = v_i_dest_cc[i];
        for (const long &i_fc: v_s_fc[i]) {
            ref_fc_2_cc[i_fc] = i_dest_cc;
        }
        v_cc[i_cc].remove_fc(v_s_fc[i], ref_fc_2_cc);
//        if( i==1){
////            cout << setprecision(15) << fixed;
////            cout<<v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][2][12]<<endl;
//            ASSERT_NEAR(v_d_i_fc_to_j_cc[i][9][2][12],v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc[9][2][12],  1e-15);
//        }
        ASSERT_EQ(ref_cc_cardinality[i], v_cc[i_cc].__card);
        ASSERT_EQ(ref_cc_compactness[i], v_cc[i_cc].__compactness);
        ASSERT_EQ(v_connectivity[i], v_cc[i_cc].is_connected());
        ASSERT_EQ(v_s_leaves[i], v_cc[i_cc].compute_s_leaves());
        ASSERT_EQ(v_s_fc_w_outer_neighbours[i], v_cc[i_cc].get_s_fc_w_outer_neighbours());
        ASSERT_EQ(v_d_i_fc_to_j_cc[i], v_cc[i_cc].__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
        ASSERT_EQ(v_d_outer[i], v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);
        ASSERT_EQ(v_tmp_fc_fine_cut_edges[i], v_cc[i_cc].__tmp_fc_fine_cut_edges);
//    ASSERT_NEAR(ref_cc_volumes[i_dest_cc], v_cc[i_cc].volume, 1e-10);
//    ASSERT_EQ(ref_cc_boundary_area[i_dest_cc], v_cc[i_cc].__boundary_area);
//    ASSERT_TRUE(v_cc[i_cc].__is_isotropic);
//    ASSERT_FALSE(v_cc[i_cc].__is_delayed);
//    ASSERT_FALSE(v_cc[i_cc]._is_finalized);
//    ASSERT_FALSE(v_cc[i_dest_cc].__is_connectivity_up_to_date);
    }

//
//for i, l in enumerate(scenario):
//
//op = l[0]
//
//i_source_cc = l[1]
//i_dest_cc = l[2]
//s_fc = l[3]
//
//if op == "A":
//cc = l_cc[i_dest_cc]
//for i_fc in s_fc:
//fc_2_cc[i_fc] = i_dest_cc
//
//        cc.add_fc(s_fc, fc_2_cc=fc_2_cc)
//# cc.fill_cc_neighbouring(fc_2_cc)
//
//elif l[0] == "R":
//cc = l_cc[i_source_cc]
//for i_fc in s_fc:
//fc_2_cc[i_fc] = i_dest_cc
//
//        cc.remove_fc(s_fc, fc_2_cc=fc_2_cc)
//# cc.fill_cc_neighbouring(fc_2_cc)
//
//self.assertEqual(l[4], cc.card)
//self.assertEqual(l[5], cc.compactness)
//self.assertEqual(l[6], cc.is_connected())
//self.assertEqual(l[7], cc.compute_s_leaves())
//self.assertEqual(l[8], cc.get_s_fc_w_outer_neighbours())
}

TEST_F(MGridGen_ext_v2_Dual_Graph, update_cc_neighbour_MGridGen_ext_v2) {


    vector<long> ref_fc_2_cc = {0, 0, 0, 0, 0,
                                0, 0, 1, 3, 3,
                                2, 2, 2, 2, 5,
                                3, 3, 3, 4, 4,
                                4, 4, 5, 4, 2,
                                0};
    vector<Coarse_Cell> v_cc;

    //========================
    // Create coarse cell 0
    //========================
    long i_cc = 0;
    unordered_set<long> s_fc_0 = {0, 1, 2, 3, 4, 5, 6, 25};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_0));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

    //========================
    // Create coarse cell 1
    //========================
    i_cc = 1;
    unordered_set<long> s_fc_1 = {7};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_1));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

//    ccg.cc_create_a_cc({7})
    //========================
    // Create coarse cell 2
    //========================
    i_cc = 2;
    unordered_set<long> s_fc_2 = {10, 11, 12, 13, 24};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_2));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);


//    ccg.cc_create_a_cc({10, 11, 12, 13, 24})
    //========================
    // Create coarse cell 3
    //========================
    i_cc = 3;
    unordered_set<long> s_fc_3 = {8, 9, 15, 16, 17};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_3));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

//    ccg.cc_create_a_cc({8, 9, 15, 16, 17})
    //========================
    // Create coarse cell 4
    //========================
    i_cc = 4;
    unordered_set<long> s_fc_4 = {18, 19, 20, 21, 23};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_4));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

//    ccg.cc_create_a_cc({18, 19, 20, 21, 23})
    //========================
    // Create coarse cell 5
    //========================
    i_cc = 5;
    unordered_set<long> s_fc_5 = {14, 22};
    v_cc.push_back(Coarse_Cell((*g),
                               i_cc,
                               s_fc_5));
    v_cc[i_cc].fill_cc_neighbouring(ref_fc_2_cc);

//    ccg.cc_create_a_cc({14, 22})


// cc = ccg._d_isotropic_cc[0]
// fc 9: 3->2
    long i_origin_cc = 3;
    long i_destination_cc = 2;

    unordered_set<long> s_fc = {9};

//    origin_cc = ccg._d_isotropic_cc[i_origin_cc]
//    destination_cc = ccg._d_isotropic_cc[i_destination_cc]

    v_cc[i_destination_cc].add_fc(s_fc, ref_fc_2_cc);

    unordered_set<long> union_s_fc = v_cc[i_destination_cc].get_s_fc();
    for (const long &i_fc :v_cc[i_origin_cc].get_s_fc()) {
        union_s_fc.insert(i_fc);
    }

    unordered_set<long> s_fc_n;

    long i_fc = 9;
    vector<long> v_neighbours = (*g).get_neighbours(i_fc);
    for (const long &i_fc_n :  v_neighbours) {
        s_fc_n.insert(i_fc_n);
    }

    for (const long &i_fc_n :  union_s_fc) {
        if (s_fc_n.count(i_fc_n) > 0) {
            s_fc_n.erase(i_fc_n);
        }
    }
    unordered_set<long> ref_s_fc_n = {18, 23};
    ASSERT_EQ(ref_s_fc_n, s_fc_n);
    // s_fc_n = set(ccg._fc_graph.get_neighbours(i_fc)) - union_s_fc

    for (const long &i_fc_n: s_fc_n) {
        const long &i_cc_n = ref_fc_2_cc[i_fc_n];
        v_cc[i_cc_n].update_cc_neighbour(i_fc_n, i_fc, i_origin_cc, i_destination_cc);
    }


    ref_fc_2_cc[i_fc] = i_destination_cc;
    v_cc[i_origin_cc].remove_fc(s_fc, ref_fc_2_cc);

    ASSERT_TRUE(v_cc[i_destination_cc].check_consistency(ref_fc_2_cc));
    ASSERT_TRUE(v_cc[i_origin_cc].check_consistency(ref_fc_2_cc));
}