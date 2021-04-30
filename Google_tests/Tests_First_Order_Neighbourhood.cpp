//
// Created by Nicolas Lantos on 21/04/2021.
//
#include "../CoMMA_lib/First_Order_Neighbourhood.h"
#include "MGridGen_ext_v2_Dual_Graph.h"
#include "gtest/gtest.h"

TEST_F(MGridGen_ext_v2_Dual_Graph, compute_neighbourhood_of_cc_MGridGen_case_1) {

    long seed = 11;

    vector<bool> a_is_fc_agglomerated((*g).number_of_cells, false);

    unsigned short goal_card = 6;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {seed};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1,  3},
                                                      {2,  2},
                                                      {3,  2},
                                                      {4,  1},
                                                      {5,  1},
                                                      {6,  2},
                                                      {7,  3},
                                                      {8,  3},
                                                      {9,  2},
                                                      {10, 1},
                                                      {12, 1},
                                                      {13, 2},
                                                      {14, 2},
                                                      {17, 3},
                                                      {18, 2},
                                                      {19, 2},
                                                      {20, 3},
                                                      {22, 3},
                                                      {23, 1},
                                                      {24, 2},
                                                      {25, 2}
    };


//    d_neighbours_of_seed, _ = (*g).compute_neighbourhood({seed},
//                                                      nb_of_order_of_neighbourhood=3,
//                                                      max_card=6)


    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    unordered_set<long> s_neighbours_of_seed = {};
    for (const auto &i_k_v : d_neighbours_of_seed) {
        s_neighbours_of_seed.insert(i_k_v.first);
    }
    First_Order_Neighbourhood f_o_neighbourhood = First_Order_Neighbourhood(s_neighbours_of_seed);
    vector<long> v = (*g).get_neighbours(seed);
    unordered_set<long> s_up(v.begin(), v.end());

    unordered_set<long> fon = f_o_neighbourhood.update(seed, s_up);
    unordered_set<long> ref_fon = {4, 5, 23, 10, 12};
    ASSERT_EQ(ref_fon, fon);

    seed = 12;
    s_up.clear();
    for (const long &i_fc:   (*g).get_neighbours(seed)) {
        s_up.insert(i_fc);
    }
    fon = f_o_neighbourhood.update(seed, s_up);
    ref_fon = {4, 5, 10, 13, 14, 19, 23};
    ASSERT_EQ(ref_fon, fon);

    seed = 23;
    s_up.clear();
    for (const long &i_fc:   (*g).get_neighbours(seed)) {
        s_up.insert(i_fc);
    }
    fon = f_o_neighbourhood.update(seed, s_up);
    ref_fon = {4, 5, 9, 10, 13, 14, 18, 19};
    ASSERT_EQ(ref_fon, fon);

    seed = 10;
    s_up.clear();
    for (const long &i_fc:   (*g).get_neighbours(seed)) {
        s_up.insert(i_fc);
    }
    fon = f_o_neighbourhood.update(seed, s_up);
    ref_fon = {2, 4, 5, 9, 13, 14, 18, 19, 24};
    ASSERT_EQ(ref_fon, fon);
}