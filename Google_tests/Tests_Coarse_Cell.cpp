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

    // CC //0
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

    // CC //1
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
//                             i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour)  // i_fc 3 has 2 neighbours outside i_cc 0
    // i_fc 3 has 2 neighbours outside i_cc 0
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{1, {0}},
                                                                          {2, {2, 3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    // CC //2
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

    // CC //3
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


    // CC //4
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
                                                                          {10, {2, 3}}
                                                                  }}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, v_cc[i_cc].d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

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
//    self.assertEqual({2: {
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

TEST_F(MGridGen_Dual_Graph, add_Case_1_MGridGen) {

    vector<long> fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    vector<Coarse_Cell> v_cc;

//    Coarse_Cell cc& =  v_cc[i_dest_cc];

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
    v_cc[i_dest_cc].fill_cc_neighbouring(fc_2_cc);
    for (const long &i_fc: s_fc) {
        fc_2_cc[i_fc] = i_dest_cc;
    }

    v_cc[i_dest_cc].check_consistency()
    ASSERT_EQ(1, v_cc[i_dest_cc].__card);
    ASSERT_EQ(0, v_cc[i_dest_cc].__compactness);
    ASSERT_TRUE(v_cc[i_dest_cc].is_connected());
    ASSERT_EQ(unordered_set<long>({3}), v_cc[i_dest_cc].compute_s_leaves());
    ASSERT_EQ(unordered_set<long>({3}), v_cc[i_dest_cc].get_s_fc_w_outer_neighbours());
    unordered_map<unsigned short int, unordered_set<long>> ref_d = {{0, unordered_set<long>({3})}};
    ASSERT_EQ(ref_d, v_cc[i_dest_cc].__d_compactness_to_s_fc);

    i_source_cc = -1;
    i_dest_cc = 0;
    s_fc = {6};

    v_cc[i_dest_cc].add_fc(s_fc, fc_2_cc = fc_2_cc);
    for (const long &i_fc: s_fc) {
        fc_2_cc[i_fc] = i_dest_cc;
    }

    v_cc[i_dest_cc].check_consistency();
    ASSERT_EQ(2, v_cc[i_dest_cc].__card);
    ASSERT_EQ(1, v_cc[i_dest_cc].__compactness);
    ASSERT_TRUE(v_cc[i_dest_cc].is_connected());
    ASSERT_EQ(unordered_set<long>({3, 6}), v_cc[i_dest_cc].compute_s_leaves());
    ASSERT_EQ(unordered_set<long>({3, 6}), v_cc[i_dest_cc].get_s_fc_w_outer_neighbours());
    ref_d = {{1, unordered_set<long>({3, 6})}};
    ASSERT_EQ(ref_d, v_cc[i_dest_cc].__d_compactness_to_s_fc);
}