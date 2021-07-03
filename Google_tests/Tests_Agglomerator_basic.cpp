#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"

#include "MGridGen_Dual_Graph.h"
#include "MGridGen_ext_Dual_Graph.h"
#include "MGridGen_ext_v2_Dual_Graph.h"
#include "Nine_squares_3x3_Dual_Graph.h"
#include "Box_1_prism_Dual_Graph.h"
#include "Box_5x5x5_Dual_Graph.h"
#include "Box_5x5x5_Aniso_Dual_Graph.h"
#include "Box_5x5x5_Aniso_lvl_2_Dual_Graph.h"
#include "Box_5x5x5_iso_and_Aniso_Dual_Graph.h"

#include "gtest/gtest.h"

TEST_F(Nine_squares_3x3_Dual_Graph, __compute_best_fc_to_add_9_Squares_isOrderPrimary_False) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short goal_card = 4;
    unsigned short max_card = 6;
    unsigned short compactness = 0;
    bool is_order_primary = false;

    // Computation of the initial aspect ratio: we need cc_surf: i.e. the external area (perimeter in 2D and sum of external faces in 3D) and volume
    double cc_surf = 0.0;
    vector<double> a_neighbours_weights = (*g).get_weights(seed);
    unsigned short nb_neighbours = (*g).get_nb_of_neighbours(seed);
    for (const double &w:a_neighbours_weights) {
        cc_surf += w;
    }

    // volume of cc is at first the volume of the seed.
    double vol_cc = (*g)._volumes[seed];

    unordered_set<long> s_of_fc_for_current_cc = {seed};

    unordered_map<long, unsigned short> d_neighbours_of_seed;
    d_neighbours_of_seed[1] = 1;
    d_neighbours_of_seed[2] = 2;
    d_neighbours_of_seed[3] = 1;
    d_neighbours_of_seed[4] = 2;
    d_neighbours_of_seed[5] = 3;
    d_neighbours_of_seed[6] = 2;
    d_neighbours_of_seed[7] = 3;

    unordered_set<long> s_neighbours_of_seed = d_keys_to_set(d_neighbours_of_seed);
    First_Order_Neighbourhood f_o_neighbourhood = First_Order_Neighbourhood(s_neighbours_of_seed);

    vector<long> v = (*g).get_neighbours(seed);
    unordered_set<long> s_up(v.begin(), v.end());

    unordered_set<long> fon = f_o_neighbourhood.update(seed, s_up);

    long argmin_ar = -1;
    double min_ar_surf = numeric_limits<double>::max();
    double min_ar_vol = numeric_limits<double>::max();
    unsigned short max_faces_in_common = 0;

    agg.__compute_best_fc_to_add((*g),
                                 fon,
                                 d_neighbours_of_seed,
                                 is_order_primary,
                                 cc_surf,
                                 vol_cc,
                                 s_of_fc_for_current_cc,
                                 argmin_ar,
                                 max_faces_in_common,
                                 min_ar_surf,
                                 min_ar_vol);
    ASSERT_EQ(3, argmin_ar);
    ASSERT_EQ(1, max_faces_in_common);
    ASSERT_EQ(5.0, min_ar_surf);
    ASSERT_EQ(2.0, min_ar_vol);
}

TEST_F(Nine_squares_3x3_Dual_Graph, __choose_optimal_cc_basic_9_Squares_isOrderPrimary_False) {

    Agglomerator agg((*g), 0, 1, 2, true);
    bool is_anisotropic = false;
    unsigned short min_card = 0;
    unsigned short goal_card = 4;
    unsigned short max_card = 6;
    string kind_of_agglomerator = "basic";

    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short compactness = 0;
    unordered_map<long, unsigned short> d_neighbours_of_seed;
    d_neighbours_of_seed[1] = 1;
    d_neighbours_of_seed[2] = 2;
    d_neighbours_of_seed[3] = 1;
    d_neighbours_of_seed[4] = 2;
    d_neighbours_of_seed[5] = 3;
    d_neighbours_of_seed[6] = 2;
    d_neighbours_of_seed[7] = 3;
    unordered_set<long> s_fc_for_current_cc = agg.__choose_optimal_cc_basic_v2_sub(seed,
                                                                                   d_neighbours_of_seed,
                                                                                   compactness,
                                                                                   false);

    unordered_set<long> ref_s_of_fc({0, 1, 3, 4});
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{2, 2},
                                                                    {5, 3},
                                                                    {6, 2},
                                                                    {7, 3}};
    ASSERT_EQ(ref_d_neighbours_of_seed, d_neighbours_of_seed);
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_9_Squares_isOrderPrimary_False) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short goal_card = 4;
    unsigned short max_card = 6;
    unsigned short compactness = 0;
    unordered_map<long, unsigned short> d_neighbours_of_seed;
    d_neighbours_of_seed[1] = 1;
    d_neighbours_of_seed[2] = 2;
    d_neighbours_of_seed[3] = 1;
    d_neighbours_of_seed[4] = 2;
    d_neighbours_of_seed[5] = 3;
    d_neighbours_of_seed[6] = 2;
    d_neighbours_of_seed[7] = 3;

    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool(cc_graph,
                                                                                          seed,
                                                                                          d_neighbours_of_seed,
                                                                                          goal_card,
                                                                                          max_card,
                                                                                          "basic",
                                                                                          compactness,
                                                                                          false);

    unordered_set<long> ref_s_of_fc({0, 1, 3, 4});
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8, 2, 6});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{2, 2},
                                                                    {5, 3},
                                                                    {6, 2},
                                                                    {7, 3}};
    ASSERT_EQ(ref_d_neighbours_of_seed, d_neighbours_of_seed);
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_v2_9_Squares_isOrderPrimary_False) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short min_card = -1;
    short goal_card = 4;
    short max_card = 6;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 0;
    unsigned short compactness = 0;
    bool is_order_primary = false;
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({0, 1, 3, 4});
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8, 6, 2});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{2, 2},
                                                                    {5, 3},
                                                                    {6, 2},
                                                                    {7, 3}};
}

TEST_F(MGridGen_ext_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_v2_seed_0) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short min_card = -1;
    short goal_card = 4;
    short max_card = 6;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 0;
    unsigned short compactness = 0;  // output
    bool is_order_primary = false;  // useless for triconnected agglomeration
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({0, 1, 2, 3, 4});
    EXPECT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    EXPECT_EQ(1, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    EXPECT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[0] = deque<long>({10});
    ref_l_deque_of_seeds[1] = deque<long>({5});
    ref_l_deque_of_seeds[2] = deque<long>({0, 7, 15, 21});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);


}

TEST_F(MGridGen_ext_Dual_Graph, __choose_optimal_cc_basic_v2_sub) {

    Agglomerator agg((*g), 0, 1, 2, true);
    bool is_anisotropic = false;
    unsigned short min_card = 0;
    unsigned short goal_card = 4;
    unsigned short max_card = 6;
    string kind_of_agglomerator = "basic";

    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short compactness = 0;
    unordered_map<long, unsigned short> d_neighbours_of_seed = {{1,  1},
                                                                {2,  2},
                                                                {3,  2},
                                                                {5,  3},
                                                                {4,  3},
                                                                {10, 3}};
    unordered_set<long> s_fc_for_current_cc = agg.__choose_optimal_cc_basic_v2_sub(seed,
                                                                                   d_neighbours_of_seed,
                                                                                   compactness,
                                                                                   false);

    unordered_set<long> ref_s_of_fc({0, 1, 2, 3, 4});
    EXPECT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    EXPECT_EQ(1, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    EXPECT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 7, 15, 21});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{5,  3},
                                                                    {10, 3}};
    EXPECT_EQ(ref_d_neighbours_of_seed, d_neighbours_of_seed);
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_9_Squares_isOrderPrimary_True) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short goal_card = 4;
    unsigned short max_card = 6;
    unsigned short compactness = 0;
    unordered_map<long, unsigned short> d_neighbours_of_seed = {{1, 1},
                                                                {2, 2},
                                                                {3, 1},
                                                                {4, 2},
                                                                {5, 3},
                                                                {6, 2},
                                                                {7, 3},
    };

    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool(cc_graph,
                                                                                          seed,
                                                                                          d_neighbours_of_seed,
                                                                                          goal_card,
                                                                                          max_card,
                                                                                          "basic",
                                                                                          compactness,
                                                                                          true);

    unordered_set<long> ref_s_of_fc({0, 1, 2, 3});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(1, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[0] = deque<long>({4});
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8, 6});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{4, 2},
                                                                    {5, 3},
                                                                    {6, 2},
                                                                    {7, 3}};
    ASSERT_EQ(ref_d_neighbours_of_seed, d_neighbours_of_seed);
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_v2_9_Squares_isOrderPrimary_True) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short min_card = -1;
    short goal_card = 4;
    short max_card = 6;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 0;
    unsigned short compactness = 0;
    bool is_order_primary = true;
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({0, 1, 2, 3});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(1, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[0] = deque<long>({4});
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8, 6});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{4, 2},
                                                                    {5, 3},
                                                                    {6, 2},
                                                                    {7, 3}};
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_9_Squares_isOrderPrimary_False_GoalCard_8) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short goal_card = 8;
    unsigned short max_card = 8;
    unsigned short compactness = 0;
    unordered_map<long, unsigned short> d_neighbours_of_seed;
    d_neighbours_of_seed[1] = 1;
    d_neighbours_of_seed[2] = 2;
    d_neighbours_of_seed[3] = 1;
    d_neighbours_of_seed[4] = 2;
    d_neighbours_of_seed[5] = 3;
    d_neighbours_of_seed[6] = 2;
    d_neighbours_of_seed[7] = 3;

    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool(cc_graph,
                                                                                          seed,
                                                                                          d_neighbours_of_seed,
                                                                                          goal_card,
                                                                                          max_card,
                                                                                          "basic",
                                                                                          compactness,
                                                                                          false);

    unordered_set<long> ref_s_of_fc({7, 6, 5, 3, 0, 1, 4, 2});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {};
    ASSERT_EQ(ref_d_neighbours_of_seed, d_neighbours_of_seed);
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_v2_9_Squares_isOrderPrimary_False_GoalCard_8) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short min_card = -1;
    short goal_card = 8;
    short max_card = 8;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 0;
    unsigned short compactness = 0;
    bool is_order_primary = false;
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({7, 6, 5, 3, 0, 1, 4, 2});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {};

}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_9_Squares_isOrderPrimary_True_GoalCard_8) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short goal_card = 8;
    unsigned short max_card = 8;
    unsigned short compactness = 0;
    unordered_map<long, unsigned short> d_neighbours_of_seed;
    d_neighbours_of_seed[1] = 1;
    d_neighbours_of_seed[2] = 2;
    d_neighbours_of_seed[3] = 1;
    d_neighbours_of_seed[4] = 2;
    d_neighbours_of_seed[5] = 3;
    d_neighbours_of_seed[6] = 2;
    d_neighbours_of_seed[7] = 3;

    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool(cc_graph,
                                                                                          seed,
                                                                                          d_neighbours_of_seed,
                                                                                          goal_card,
                                                                                          max_card,
                                                                                          "basic",
                                                                                          compactness,
                                                                                          true);

    unordered_set<long> ref_s_of_fc({7, 6, 5, 3, 0, 1, 4, 2});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {};
    ASSERT_EQ(ref_d_neighbours_of_seed, d_neighbours_of_seed);
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_v2_pool_9_Squares_isOrderPrimary_True_GoalCard_8) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short min_card = -1;
    short goal_card = 8;
    short max_card = 8;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 0;
    unsigned short compactness = 0;
    bool is_order_primary = true;
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

//    long seed = 0;
//    unsigned short goal_card = 8;
//    unsigned short max_card = 8;
//    unsigned short compactness = 0;
//    unordered_map<long, unsigned short> d_neighbours_of_seed;
//    d_neighbours_of_seed[1] = 1;
//    d_neighbours_of_seed[2] = 2;
//    d_neighbours_of_seed[3] = 1;
//    d_neighbours_of_seed[4] = 2;
//    d_neighbours_of_seed[5] = 3;
//    d_neighbours_of_seed[6] = 2;
//    d_neighbours_of_seed[7] = 3;
//
//    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool(cc_graph,
//                                                                                          seed,
//                                                                                          d_neighbours_of_seed,
//                                                                                          goal_card,
//                                                                                          max_card,
//                                                                                          "basic",
//                                                                                          compactness,
//                                                                                          true);

    unordered_set<long> ref_s_of_fc({7, 6, 5, 3, 0, 1, 4, 2});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {};
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_9_Squares_isOrderPrimary_False_GoalCard_8_neighbours_4) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short goal_card = 8;
    unsigned short max_card = 8;
    unsigned short compactness = 0;
    unordered_map<long, unsigned short> d_neighbours_of_seed;
    d_neighbours_of_seed[1] = 1;
    d_neighbours_of_seed[2] = 2;
    d_neighbours_of_seed[3] = 1;
    d_neighbours_of_seed[4] = 2;
    d_neighbours_of_seed[5] = 3;
    d_neighbours_of_seed[6] = 2;
    d_neighbours_of_seed[7] = 3;
    d_neighbours_of_seed[8] = 4;

    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool(cc_graph,
                                                                                          seed,
                                                                                          d_neighbours_of_seed,
                                                                                          goal_card,
                                                                                          max_card,
                                                                                          "basic",
                                                                                          compactness,
                                                                                          false);

    unordered_set<long> ref_s_of_fc({7, 6, 5, 3, 0, 1, 4, 2});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{8, 4}};
    ASSERT_EQ(ref_d_neighbours_of_seed, d_neighbours_of_seed);
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_v2_9_Squares_isOrderPrimary_False_GoalCard_8_neighbours_4) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    agg.__min_neighbourhood = 4;

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short min_card = -1;
    short goal_card = 8;
    short max_card = 8;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 0;
    unsigned short compactness = 0;
    bool is_order_primary = false;
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({7, 6, 5, 3, 0, 1, 4, 2});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{8, 4}};
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_9_Squares_isOrderPrimary_True_GoalCard_8_neighbours_4) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    long seed = 0;
    unsigned short goal_card = 8;
    unsigned short max_card = 8;
    unsigned short compactness = 0;
    unordered_map<long, unsigned short> d_neighbours_of_seed;
    d_neighbours_of_seed[1] = 1;
    d_neighbours_of_seed[2] = 2;
    d_neighbours_of_seed[3] = 1;
    d_neighbours_of_seed[4] = 2;
    d_neighbours_of_seed[5] = 3;
    d_neighbours_of_seed[6] = 2;
    d_neighbours_of_seed[7] = 3;
    d_neighbours_of_seed[8] = 4;

    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool(cc_graph,
                                                                                          seed,
                                                                                          d_neighbours_of_seed,
                                                                                          goal_card,
                                                                                          max_card,
                                                                                          "basic",
                                                                                          compactness,
                                                                                          true);

    unordered_set<long> ref_s_of_fc({7, 6, 5, 3, 0, 1, 4, 2});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{8, 4}};
    ASSERT_EQ(ref_d_neighbours_of_seed, d_neighbours_of_seed);
}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_v2_9_Squares_isOrderPrimary_True_GoalCard_8_neighbours_4) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    agg.__min_neighbourhood = 4;

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short min_card = -1;
    short goal_card = 8;
    short max_card = 8;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 0;
    unsigned short compactness = 0;
    bool is_order_primary = true;
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

//    long seed = 0;
//    unsigned short goal_card = 8;
//    unsigned short max_card = 8;
//    unsigned short compactness = 0;
//    unordered_map<long, unsigned short> d_neighbours_of_seed;
//    d_neighbours_of_seed[1] = 1;
//    d_neighbours_of_seed[2] = 2;
//    d_neighbours_of_seed[3] = 1;
//    d_neighbours_of_seed[4] = 2;
//    d_neighbours_of_seed[5] = 3;
//    d_neighbours_of_seed[6] = 2;
//    d_neighbours_of_seed[7] = 3;
//    d_neighbours_of_seed[8] = 4;
//
//    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool(cc_graph,
//                                                                                          seed,
//                                                                                          d_neighbours_of_seed,
//                                                                                          goal_card,
//                                                                                          max_card,
//                                                                                          "basic",
//                                                                                          compactness,
//                                                                                          true);

    unordered_set<long> ref_s_of_fc({7, 6, 5, 3, 0, 1, 4, 2});  //0,1, 3, 6 is also correct!
    ASSERT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    ASSERT_EQ(2, compactness);
    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    ASSERT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{8, 4}};

}

TEST_F(MGridGen_Dual_Graph, agglomerate_one_level) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> anisotropic_lines = {};
    agg.agglomerate_one_level(false, 0, anisotropic_lines);
    ASSERT_EQ(4, agg.get_nb_cc());

    vector<long> ref_fc_2_cc = {1, 1, 1, 1, 2, 2, 1, 3, 2, 3, 2, 0, 3, 0, 0};
    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(MGridGen_Dual_Graph, agglomerate_one_level_without_correction) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 0;
    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(4, agg.get_nb_cc());

    vector<long> ref_fc_2_cc = {2, 2, 2, 2, 3, 3, 0, 1, 1, 1, 1, 0, 1, 0, 0};
    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(MGridGen_Dual_Graph, agglomerate_one_level_with_correction_step_1) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 1;
    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    EXPECT_EQ(3, agg.get_nb_cc());

    vector<long> ref_fc_2_cc = {2, 2, 2, 2, 1, 2, 0, 1, 1, 1, 1, 0, 1, 0, 0};
    EXPECT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(MGridGen_Dual_Graph, agglomerate_one_level_with_correction_step_2) {

    //no change in adding step 2

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 2;
    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(3, agg.get_nb_cc());

    vector<long> ref_fc_2_cc = {2, 2, 2, 2, 1, 2, 0, 1, 1, 1, 1, 0, 1, 0, 0}; //no change in adding step 2
    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

//    agg.get_agglo_lines
}

TEST_F(MGridGen_Dual_Graph, agglomerate_one_level_with_correction_step_3) {

    //no change in adding step 3

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 3;
    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    EXPECT_EQ(3, agg.get_nb_cc());

    vector<long> ref_fc_2_cc = {2, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 0, 1, 0, 0};  //no change in adding step 3
    EXPECT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(MGridGen_Dual_Graph, agglomerate_one_level_with_correction_step_4) {

    //no change in adding step 4

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 4;
    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(3, agg.get_nb_cc());

    vector<long> ref_fc_2_cc = {2, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 0, 1, 0, 0};  //no change in adding step 4
    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(MGridGen_Dual_Graph, agglomerate_one_level_with_correction_step_5) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 5;
    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(4, agg.get_nb_cc());

    vector<long> ref_fc_2_cc = {1, 1, 1, 1, 2, 2, 1, 3, 2, 3, 2, 0, 3, 0, 0};  //no change in adding step 4
    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(MGridGen_ext_Dual_Graph, agglomerate_one_level) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> anisotropic_lines = {};
    agg.agglomerate_one_level(false, 0, anisotropic_lines);
    ASSERT_EQ(5, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 0, 0, 0, 2, 4, 3, 3, 3, 4, 4, 4, 4};
    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_1_prism_Dual_Graph, agglomerate_one_level) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> anisotropic_lines = {};
    agg.agglomerate_one_level(false, 0, anisotropic_lines);
    ASSERT_EQ(41, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {6, 0, 0, 9, 6, 6, 13, 9, 9, 13, 13, 13, 20, 17, 17, 39, 18, 18, 22, 39, 39, 23, 23, 22, 20, 20, 24, 19, 19, 20, 25, 19, 19, 22, 22, 25, 17, 17, 17, 18, 18, 18, 15, 15,
                                15, 23, 23, 15, 20, 20, 20, 39, 19, 19, 22, 39, 39, 23, 22, 22, 14, 14, 14, 18, 18, 18, 29, 38, 38, 23, 29, 29, 8, 8, 8, 33, 33, 8, 28, 33, 33, 27, 27, 28, 11, 11, 11,
                                26, 26, 11, 26, 26, 26, 28, 28, 28, 11, 11, 8, 26, 11, 11, 26, 26, 26, 28, 28, 28, 24, 12, 12, 21, 24, 24, 21, 21, 21, 25, 25, 25, 2, 2, 2, 40, 40, 2, 31, 40, 40, 31,
                                31, 31, 1, 1, 1, 3, 3, 1, 7, 7, 3, 34, 7, 7, 12, 12, 12, 24, 24, 37, 38, 38, 38, 27, 27, 27, 1, 1, 1, 3, 40, 1, 7, 7, 3, 34, 34, 7, 4, 4, 4, 33, 5, 5, 30, 33, 33, 30,
                                30, 30, 2, 2, 2, 5, 5, 2, 31, 31, 5, 31, 31, 31, 16, 16, 16, 36, 16, 16, 32, 32, 36, 32, 32, 32, 4, 4, 4, 35, 35, 40, 36, 36, 35, 34, 34, 34, 0, 0, 0, 6, 3, 3, 10, 7,
                                6, 10, 10, 10, 0, 0, 0, 6, 6, 3, 10, 10, 6, 10, 10, 10, 16, 16, 16, 9, 9, 16, 13, 32, 9, 13, 13, 13, 14, 14, 14, 36, 35, 35, 32, 36, 36, 29, 29, 32, 17, 17, 17, 18, 9,
                                9, 15, 15, 15, 23, 23, 15, 12, 12, 4, 35, 35, 35, 36, 36, 36, 34, 34, 34, 14, 14, 14, 37, 37, 37, 38, 38, 37, 29, 29, 29, 12, 12, 12, 37, 37, 37, 38, 37, 37, 27, 27,
                                27, 20, 24, 24, 21, 21, 20, 25, 21, 21, 25, 25, 25, 8, 8, 4, 33, 8, 8, 30, 33, 33, 30, 30, 30};
//    cout<<endl;
//    for (auto i:agg.get_fc_2_cc())
//    {
//        cout<<i<<", ";
//    }
//    cout<<endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_1_prism_Dual_Graph, agglomerate_one_level_without_correction) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 0;

    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    EXPECT_EQ(45, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {6, 0, 0, 9, 6, 6, 13, 9, 9, 13, 13, 13, 20, 17, 17, 18, 18, 18, 22, 22, 19, 23, 23, 22, 20, 20, 24, 19, 19, 20, 19, 19, 21, 22, 22, 25, 17, 17, 17, 18, 18, 18, 15, 15,
                                15, 23, 23, 15, 20, 20, 20, 19, 19, 19, 22, 19, 19, 23, 22, 22, 43, 14, 14, 18, 18, 18, 29, 44, 44, 23, 29, 29, 8, 8, 8, 33, 33, 41, 28, 33, 33, 27, 27, 28, 11, 11, 11,
                                26, 24, 11, 26, 26, 26, 28, 28, 28, 11, 11, 8, 26, 11, 11, 26, 26, 26, 28, 28, 28, 24, 12, 12, 21, 24, 24, 21, 21, 21, 25, 25, 25, 2, 2, 2, 5, 5, 2, 31, 5, 5, 31, 31,
                                31, 1, 1, 1, 3, 3, 1, 7, 7, 3, 34, 7, 7, 12, 12, 12, 24, 24, 42, 44, 44, 44, 27, 27, 27, 1, 1, 1, 3, 5, 1, 7, 38, 3, 34, 31, 7, 4, 4, 4, 33, 40, 5, 30, 33, 33, 30, 30,
                                30, 2, 2, 2, 5, 5, 2, 31, 31, 5, 31, 31, 31, 16, 16, 16, 37, 16, 16, 32, 32, 37, 32, 32, 32, 4, 4, 4, 35, 35, 5, 34, 39, 35, 34, 34, 34, 0, 0, 0, 6, 3, 3, 7, 7, 6, 10,
                                10, 10, 0, 0, 0, 6, 6, 3, 10, 10, 6, 10, 10, 10, 16, 16, 16, 9, 9, 36, 13, 13, 9, 13, 13, 13, 14, 14, 14, 37, 35, 35, 32, 37, 37, 29, 29, 32, 17, 17, 17, 18, 9, 9, 15,
                                15, 15, 23, 23, 15, 12, 12, 4, 35, 35, 35, 37, 37, 37, 34, 34, 32, 14, 14, 14, 42, 42, 42, 44, 44, 42, 29, 29, 29, 12, 12, 12, 42, 42, 42, 44, 42, 42, 27, 27, 27, 20,
                                24, 24, 21, 21, 20, 25, 21, 21, 25, 25, 25, 8, 8, 4, 33, 8, 8, 30, 33, 33, 30, 30, 30
    };
//    cout << endl;
//    for (auto i:agg.get_fc_2_cc()) {
//        cout << i << ", ";
//    }
//    cout << endl;

    EXPECT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(Box_1_prism_Dual_Graph, agglomerate_one_level_with_correction_step_1) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 1;

    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(39, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {6, 0, 0, 9, 6, 6, 13, 9, 9, 13, 13, 13, 20, 17, 17, 18, 18, 18, 22, 22, 19, 23, 23, 22, 20, 20, 24, 19, 19, 20, 19, 19, 21, 22, 22, 25, 17, 17, 17, 18, 18, 18, 15, 15,
                                15, 23, 23, 15, 20, 20, 20, 19, 19, 19, 22, 19, 19, 23, 22, 22, 14, 14, 14, 18, 18, 18, 29, 38, 38, 23, 29, 29, 8, 8, 8, 33, 33, 8, 28, 33, 33, 27, 27, 28, 11, 11, 11,
                                26, 24, 11, 26, 26, 26, 28, 28, 28, 11, 11, 8, 26, 11, 11, 26, 26, 26, 28, 28, 28, 24, 12, 12, 21, 24, 24, 21, 21, 21, 25, 25, 25, 2, 2, 2, 5, 5, 2, 31, 5, 5, 31, 31,
                                31, 1, 1, 1, 3, 3, 1, 7, 7, 3, 34, 7, 7, 12, 12, 12, 24, 24, 37, 38, 38, 38, 27, 27, 27, 1, 1, 1, 3, 5, 1, 7, 35, 3, 34, 31, 7, 4, 4, 4, 33, 5, 5, 30, 33, 33, 30, 30,
                                30, 2, 2, 2, 5, 5, 2, 31, 31, 5, 31, 31, 31, 16, 16, 16, 36, 16, 16, 32, 32, 36, 32, 32, 32, 4, 4, 4, 35, 35, 5, 34, 36, 35, 34, 34, 34, 0, 0, 0, 6, 3, 3, 7, 7, 6, 10,
                                10, 10, 0, 0, 0, 6, 6, 3, 10, 10, 6, 10, 10, 10, 16, 16, 16, 9, 9, 16, 13, 13, 9, 13, 13, 13, 14, 14, 14, 36, 35, 35, 32, 36, 36, 29, 29, 32, 17, 17, 17, 18, 9, 9, 15,
                                15, 15, 23, 23, 15, 12, 12, 4, 35, 35, 35, 36, 36, 36, 34, 34, 32, 14, 14, 14, 37, 37, 37, 38, 38, 37, 29, 29, 29, 12, 12, 12, 37, 37, 37, 38, 37, 37, 27, 27, 27, 20,
                                24, 24, 21, 21, 20, 25, 21, 21, 25, 25, 25, 8, 8, 4, 33, 8, 8, 30, 33, 33, 30, 30, 30};

//    cout<<endl;
//    for (auto i:agg.get_fc_2_cc())
//    {
//        cout<<i<<", ";
//    }
//    cout<<endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_1_prism_Dual_Graph, agglomerate_one_level_with_correction_step_2) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 2;

    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(39, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {6, 0, 0, 9, 6, 6, 13, 9, 9, 13, 13, 13, 20, 17, 17, 18, 18, 18, 22, 22, 19, 23, 23, 22, 20, 20, 24, 19, 19, 20, 19, 19, 21, 22, 22, 25, 17, 17, 17, 18, 18, 18, 15, 15,
                                15, 23, 23, 15, 20, 20, 20, 19, 19, 19, 22, 19, 19, 23, 22, 22, 14, 14, 14, 18, 18, 18, 29, 38, 38, 23, 29, 29, 8, 8, 8, 33, 33, 8, 28, 33, 33, 27, 27, 28, 11, 11, 11,
                                26, 24, 11, 26, 26, 26, 28, 28, 28, 11, 11, 8, 26, 11, 11, 26, 26, 26, 28, 28, 28, 24, 12, 12, 21, 24, 24, 21, 21, 21, 25, 25, 25, 2, 2, 2, 5, 5, 2, 31, 5, 5, 31, 31,
                                31, 1, 1, 1, 3, 3, 1, 7, 7, 3, 34, 7, 7, 12, 12, 12, 24, 24, 37, 38, 38, 38, 27, 27, 27, 1, 1, 1, 3, 5, 1, 7, 35, 3, 34, 31, 7, 4, 4, 4, 33, 5, 5, 30, 33, 33, 30, 30,
                                30, 2, 2, 2, 5, 5, 2, 31, 31, 5, 31, 31, 31, 16, 16, 16, 36, 16, 16, 32, 32, 36, 32, 32, 32, 4, 4, 4, 35, 35, 5, 34, 36, 35, 34, 34, 34, 0, 0, 0, 6, 3, 3, 7, 7, 6, 10,
                                10, 10, 0, 0, 0, 6, 6, 3, 10, 10, 6, 10, 10, 10, 16, 16, 16, 9, 9, 16, 13, 13, 9, 13, 13, 13, 14, 14, 14, 36, 35, 35, 32, 36, 36, 29, 29, 32, 17, 17, 17, 18, 9, 9, 15,
                                15, 15, 23, 23, 15, 12, 12, 4, 35, 35, 35, 36, 36, 36, 34, 34, 32, 14, 14, 14, 37, 37, 37, 38, 38, 37, 29, 29, 29, 12, 12, 12, 37, 37, 37, 38, 37, 37, 27, 27, 27, 20,
                                24, 24, 21, 21, 20, 25, 21, 21, 25, 25, 25, 8, 8, 4, 33, 8, 8, 30, 33, 33, 30, 30, 30};

//    cout << endl;
//    for (auto i:agg.get_fc_2_cc()) {
//        cout << i << ", ";
//    }
//    cout << endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_1_prism_Dual_Graph, agglomerate_one_level_with_correction_step_3) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 3;

    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(39, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {6, 0, 0, 9, 6, 6, 13, 9, 9, 13, 13, 13, 20, 17, 17, 19, 18, 18, 22, 22, 19, 23, 23, 22, 20, 20, 24, 19, 19, 20, 19, 19, 19, 22, 22, 25, 17, 17, 17, 18, 18, 18, 15, 15,
                                15, 23, 23, 15, 20, 20, 20, 19, 19, 19, 22, 19, 19, 23, 22, 22, 14, 14, 14, 18, 18, 18, 29, 38, 38, 23, 29, 29, 8, 8, 8, 33, 33, 8, 28, 33, 33, 27, 27, 28, 11, 11, 11,
                                26, 26, 11, 26, 26, 26, 28, 28, 28, 11, 11, 8, 26, 11, 11, 26, 26, 26, 28, 28, 28, 24, 12, 12, 21, 24, 24, 21, 21, 21, 25, 25, 25, 2, 2, 2, 5, 5, 2, 31, 5, 5, 31, 31,
                                31, 1, 1, 1, 3, 3, 1, 7, 7, 3, 34, 7, 7, 12, 12, 12, 24, 24, 37, 38, 38, 38, 27, 27, 27, 1, 1, 1, 3, 5, 1, 7, 35, 3, 34, 34, 7, 4, 4, 4, 33, 5, 5, 30, 33, 33, 30, 30,
                                30, 2, 2, 2, 5, 5, 2, 31, 31, 5, 31, 31, 31, 16, 16, 16, 36, 16, 16, 32, 32, 36, 32, 32, 32, 4, 4, 4, 35, 35, 5, 36, 36, 35, 34, 34, 34, 0, 0, 0, 6, 3, 3, 10, 7, 6, 10,
                                10, 10, 0, 0, 0, 6, 6, 3, 10, 10, 6, 10, 10, 10, 16, 16, 16, 9, 9, 16, 13, 13, 9, 13, 13, 13, 14, 14, 14, 36, 35, 35, 32, 36, 36, 29, 29, 32, 17, 17, 17, 18, 9, 9, 15,
                                15, 15, 23, 23, 15, 12, 12, 4, 35, 35, 35, 36, 36, 36, 34, 34, 34, 14, 14, 14, 37, 37, 37, 38, 38, 37, 29, 29, 29, 12, 12, 12, 37, 37, 37, 38, 37, 37, 27, 27, 27, 20,
                                24, 24, 21, 21, 20, 25, 21, 21, 25, 25, 25, 8, 8, 4, 33, 8, 8, 30, 33, 33, 30, 30, 30};

//    cout << endl;
//    for (auto i:agg.get_fc_2_cc()) {
//        cout << i << ", ";
//    }
//    cout << endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_1_prism_Dual_Graph, agglomerate_one_level_with_correction_step_4) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 4;

    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(39, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {6, 0, 0, 9, 6, 6, 13, 9, 9, 13, 13, 13, 20, 17, 17, 19, 18, 18, 22, 22, 19, 23, 23, 22, 20, 20, 24, 19, 19, 20, 19, 19, 19, 22, 22, 25, 17, 17, 17, 18, 18, 18, 15, 15,
                                15, 23, 23, 15, 20, 20, 20, 19, 19, 19, 22, 19, 19, 23, 22, 22, 14, 14, 14, 18, 18, 18, 29, 38, 38, 23, 29, 29, 8, 8, 8, 33, 33, 8, 28, 33, 33, 27, 27, 28, 11, 11, 11,
                                26, 26, 11, 26, 26, 26, 28, 28, 28, 11, 11, 8, 26, 11, 11, 26, 26, 26, 28, 28, 28, 24, 12, 12, 21, 24, 24, 21, 21, 21, 25, 25, 25, 2, 2, 2, 5, 5, 2, 31, 5, 5, 31, 31,
                                31, 1, 1, 1, 3, 3, 1, 7, 7, 3, 34, 7, 7, 12, 12, 12, 24, 24, 37, 38, 38, 38, 27, 27, 27, 1, 1, 1, 3, 5, 1, 7, 7, 3, 34, 34, 7, 4, 4, 4, 33, 5, 5, 30, 33, 33, 30, 30,
                                30, 2, 2, 2, 5, 5, 2, 31, 31, 5, 31, 31, 31, 16, 16, 16, 36, 16, 16, 32, 32, 36, 32, 32, 32, 4, 4, 4, 35, 35, 5, 36, 36, 35, 34, 34, 34, 0, 0, 0, 6, 3, 3, 10, 7, 6, 10,
                                10, 10, 0, 0, 0, 6, 6, 3, 10, 10, 6, 10, 10, 10, 16, 16, 16, 9, 9, 16, 13, 32, 9, 13, 13, 13, 14, 14, 14, 36, 35, 35, 32, 36, 36, 29, 29, 32, 17, 17, 17, 18, 9, 9, 15,
                                15, 15, 23, 23, 15, 12, 12, 4, 35, 35, 35, 36, 36, 36, 34, 34, 34, 14, 14, 14, 37, 37, 37, 38, 38, 37, 29, 29, 29, 12, 12, 12, 37, 37, 37, 38, 37, 37, 27, 27, 27, 20,
                                24, 24, 21, 21, 20, 25, 21, 21, 25, 25, 25, 8, 8, 4, 33, 8, 8, 30, 33, 33, 30, 30, 30};

//    cout<<endl;
//    for (auto i:agg.get_fc_2_cc())
//    {
//        cout<<i<<", ";
//    }
//    cout<<endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_1_prism_Dual_Graph, agglomerate_one_level_with_correction_step_5) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> anisotropic_lines = {};
    vector<long> debug_only_fc_to_cc = {};

    const short debug_only_steps = 5;

    agg.agglomerate_one_level(false,
                              nb_aniso_agglo_lines, anisotropic_lines,
                              "basic",
                              -1, -1, -1,
                              debug_only_fc_to_cc,
                              debug_only_steps);
    ASSERT_EQ(41, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {6, 0, 0, 9, 6, 6, 13, 9, 9, 13, 13, 13, 20, 17, 17, 39, 18, 18, 22, 39, 39, 23, 23, 22, 20, 20, 24, 19, 19, 20, 25, 19, 19, 22, 22, 25, 17, 17, 17, 18, 18, 18, 15, 15,
                                15, 23, 23, 15, 20, 20, 20, 39, 19, 19, 22, 39, 39, 23, 22, 22, 14, 14, 14, 18, 18, 18, 29, 38, 38, 23, 29, 29, 8, 8, 8, 33, 33, 8, 28, 33, 33, 27, 27, 28, 11, 11, 11,
                                26, 26, 11, 26, 26, 26, 28, 28, 28, 11, 11, 8, 26, 11, 11, 26, 26, 26, 28, 28, 28, 24, 12, 12, 21, 24, 24, 21, 21, 21, 25, 25, 25, 2, 2, 2, 40, 40, 2, 31, 40, 40, 31,
                                31, 31, 1, 1, 1, 3, 3, 1, 7, 7, 3, 34, 7, 7, 12, 12, 12, 24, 24, 37, 38, 38, 38, 27, 27, 27, 1, 1, 1, 3, 40, 1, 7, 7, 3, 34, 34, 7, 4, 4, 4, 33, 5, 5, 30, 33, 33, 30,
                                30, 30, 2, 2, 2, 5, 5, 2, 31, 31, 5, 31, 31, 31, 16, 16, 16, 36, 16, 16, 32, 32, 36, 32, 32, 32, 4, 4, 4, 35, 35, 40, 36, 36, 35, 34, 34, 34, 0, 0, 0, 6, 3, 3, 10, 7,
                                6, 10, 10, 10, 0, 0, 0, 6, 6, 3, 10, 10, 6, 10, 10, 10, 16, 16, 16, 9, 9, 16, 13, 32, 9, 13, 13, 13, 14, 14, 14, 36, 35, 35, 32, 36, 36, 29, 29, 32, 17, 17, 17, 18, 9,
                                9, 15, 15, 15, 23, 23, 15, 12, 12, 4, 35, 35, 35, 36, 36, 36, 34, 34, 34, 14, 14, 14, 37, 37, 37, 38, 38, 37, 29, 29, 29, 12, 12, 12, 37, 37, 37, 38, 37, 37, 27, 27,
                                27, 20, 24, 24, 21, 21, 20, 25, 21, 21, 25, 25, 25, 8, 8, 4, 33, 8, 8, 30, 33, 33, 30, 30, 30};

//    cout<<endl;
//    for (auto i:agg.get_fc_2_cc())
//    {
//        cout<<i<<", ";
//    }
//    cout<<endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_5x5x5_Aniso_Dual_Graph, agglomerate_one_level_aniso) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> anisotropic_lines = {};
    agg.agglomerate_one_level(true, 0, anisotropic_lines);

    EXPECT_EQ(32, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {1, 11, 9, 5, 15, 29, 31, 21, 13, 25, 27, 23, 3, 17, 19, 7,
                                1, 11, 9, 5, 15, 29, 31, 21, 13, 25, 27, 23, 3, 17, 19, 7,
            //
                                0, 10, 8, 4, 14, 28, 30, 20, 12, 24, 26, 22, 2, 16, 18, 6,
                                0, 10, 8, 4, 14, 28, 30, 20, 12, 24, 26, 22, 2, 16, 18, 6};
//    cout<<endl;
//    for (auto i:agg.get_fc_2_cc())
//    {
//        cout<<i<<", ";
//    }
//    cout<<endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

//    print_agglomeration_lines(agg.__v_lines[0]);
//    print_agglomeration_lines(agg.__v_lines[1]);

    // Check agglomeration lines for level 0
    //======================================
    int i_level = 0;
    long sizes[2] = {0, 0};
    const long nb_fc = (*g).number_of_cells; //TODO change number_of_cells -> nb_cells ou nb_fc?
    long agglo_lines_array_idx[nb_fc];
    long agglo_lines_array[nb_fc];

    agg.get_agglo_lines(i_level,
                        sizes,
                        agglo_lines_array_idx,
                        agglo_lines_array);
//    cout<<"sizes: "<<sizes[0]<<", "<<sizes[1]<<endl;
    EXPECT_EQ(17, sizes[0]);
    EXPECT_EQ(64, sizes[1]);

    long ref_agglo_lines_array_idx[17] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64};
    long ref_agglo_lines_array[64] = {48, 32, 16, 0,
                                      60, 44, 28, 12,
                                      51, 35, 19, 3,
                                      63, 47, 31, 15,
                                      50, 34, 18, 2,
                                      49, 33, 17, 1,
                                      56, 40, 24, 8,
                                      52, 36, 20, 4,
                                      61, 45, 29, 13,
                                      62, 46, 30, 14,
                                      55, 39, 23, 7,
                                      59, 43, 27, 11,
                                      57, 41, 25, 9,
                                      58, 42, 26, 10,
                                      53, 37, 21, 5,
                                      54, 38, 22, 6};

//    cout<<endl;
//    for (long i_fc=0; i_fc<sizes[0]; i_fc++)
//    {
//        cout<<agglo_lines_array_idx[i_fc]<<", ";
//    }
//    cout<<endl;
//    for (long i_fc=0; i_fc<sizes[1]; i_fc++)
//    {
//        cout<<agglo_lines_array[i_fc]<<", ";
//    }
//    cout<<endl;
    for (int i = 0; i < sizes[0]; i++) {
        EXPECT_EQ(ref_agglo_lines_array_idx[i], agglo_lines_array_idx[i]);
    }

    for (int i = 0; i < sizes[1]; i++) {
        EXPECT_EQ(ref_agglo_lines_array[i], agglo_lines_array[i]);
    }

    // Check agglomeration lines for level 1
    //======================================
    i_level = 1;
    sizes[0] = -1;
    sizes[1] = -1;
    for (int i = 0; i < nb_fc; i++) {
        agglo_lines_array_idx[i] = 0;
        agglo_lines_array[i] = 0;
    }

    agg.get_agglo_lines(i_level,
                        sizes,
                        agglo_lines_array_idx,
                        agglo_lines_array);
//    cout<<"sizes: "<<sizes[0]<<", "<<sizes[1]<<endl;
    EXPECT_EQ(17, sizes[0]);
    EXPECT_EQ(32, sizes[1]);

    long ref_agglo_lines_array_idx_1[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    long ref_agglo_lines_array_1[64] = {0, 1,
                                        2, 3,
                                        4, 5,
                                        6, 7,
                                        8, 9,
                                        10, 11,
                                        12, 13,
                                        14, 15,
                                        16, 17,
                                        18, 19,
                                        20, 21,
                                        22, 23,
                                        24, 25,
                                        26, 27,
                                        28, 29,
                                        30, 31};

//    cout<<endl;
//    for (long i_fc=0; i_fc<sizes[0]; i_fc++)
//    {
//        cout<<agglo_lines_array_idx[i_fc]<<", ";
//    }
//    cout<<endl;
//    for (long i_fc=0; i_fc<sizes[1]; i_fc++)
//    {
//        cout<<agglo_lines_array[i_fc]<<", ";
//    }
//    cout<<endl;
    for (int i = 0; i < sizes[0]; i++) {
        EXPECT_EQ(ref_agglo_lines_array_idx_1[i], agglo_lines_array_idx[i]);
    }

    for (int i = 0; i < sizes[1]; i++) {
        EXPECT_EQ(ref_agglo_lines_array_1[i], agglo_lines_array[i]);
    }
}

TEST_F(Box_5x5x5_Aniso_lvl_2_Dual_Graph, agglomerate_one_level_aniso_without_agglomeration_lines) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> agglo_lines;

    string kind_of_agglomerator = "basic";
    const short goal_card = -1;
    const short min_card = -1;
    const short max_card = -1;


    agg.agglomerate_one_level(true,
                              nb_aniso_agglo_lines,
                              agglo_lines,
                              kind_of_agglomerator,
                              goal_card,
                              min_card,
                              max_card);

    ASSERT_EQ(16, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {12, 12, 13, 13, 14, 14, 15, 15, 4, 4, 8, 8, 5, 5, 6, 6, 9, 9, 11, 11, 7, 7, 10, 10, 3, 3, 2, 2, 1, 1, 0, 0};
//    cout << endl;
//    for (auto i:agg.get_fc_2_cc()) {
//        cout << i << ", ";
//    }
//    cout << endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(Box_5x5x5_Aniso_lvl_2_Dual_Graph, agglomerate_one_level_aniso) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> agglo_lines;

    deque<long> *a = new deque<long>({30, 31});
    agglo_lines.push_front(a);

    a = new deque<long>({28, 29});
    agglo_lines.push_front(a);

    a = new deque<long>({26, 27});
    agglo_lines.push_front(a);

    a = new deque<long>({24, 25});
    agglo_lines.push_front(a);

    a = new deque<long>({22, 23});
    agglo_lines.push_front(a);

    a = new deque<long>({20, 21});
    agglo_lines.push_front(a);

    a = new deque<long>({18, 19});
    agglo_lines.push_front(a);

    a = new deque<long>({16, 17});
    agglo_lines.push_front(a);

    a = new deque<long>({14, 15});
    agglo_lines.push_front(a);

    a = new deque<long>({12, 13});
    agglo_lines.push_front(a);

    a = new deque<long>({10, 11});
    agglo_lines.push_front(a);

    a = new deque<long>({8, 9});
    agglo_lines.push_front(a);

    a = new deque<long>({6, 7});
    agglo_lines.push_front(a);

    a = new deque<long>({4, 5});
    agglo_lines.push_front(a);

    // 1
    a = new deque<long>({2, 3});
    agglo_lines.push_front(a);

    // 0
    a = new deque<long>({0, 1});
    agglo_lines.push_front(a);

    string kind_of_agglomerator = "basic";
    const short goal_card = -1;
    const short min_card = -1;
    const short max_card = -1;
    unsigned long nb_aniso_agglo_lines = 16;


//    agg.agglomerate_one_level(true, 0, agglo_lines);

    agg.agglomerate_one_level(true,
                              nb_aniso_agglo_lines,
                              agglo_lines,
                              kind_of_agglomerator,
                              goal_card,
                              min_card,
                              max_card);

    ASSERT_EQ(16, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15};
//    cout << endl;
//    for (auto i:agg.get_fc_2_cc()) {
//        cout << i << ", ";
//    }
//    cout << endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(Box_5x5x5_Aniso_lvl_2_Dual_Graph, agglomerate_one_level_aniso_case_2) {
    /**
     * Modification of agglomeration lines.
     */
    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> agglo_lines;

    deque<long> *a = new deque<long>({30, 31});
    agglo_lines.push_front(a);

    a = new deque<long>({28, 29});
    agglo_lines.push_front(a);

    a = new deque<long>({26, 27});
    agglo_lines.push_front(a);

    a = new deque<long>({24, 25});
    agglo_lines.push_front(a);

    a = new deque<long>({22, 23});
    agglo_lines.push_front(a);

    a = new deque<long>({20, 21});
    agglo_lines.push_front(a);

    a = new deque<long>({18, 19});
    agglo_lines.push_front(a);

    a = new deque<long>({16, 17});
    agglo_lines.push_front(a);

    a = new deque<long>({14, 15});
    agglo_lines.push_front(a);

    a = new deque<long>({12, 13});
    agglo_lines.push_front(a);

    a = new deque<long>({10});
    agglo_lines.push_front(a);

    a = new deque<long>({8, 9});
    agglo_lines.push_front(a);

    a = new deque<long>({6, 7});
    agglo_lines.push_front(a);

    a = new deque<long>({4, 5});
    agglo_lines.push_front(a);

    // 1
    a = new deque<long>({2, 3});
    agglo_lines.push_front(a);

    // 0
    a = new deque<long>({0});
    agglo_lines.push_front(a);

    string kind_of_agglomerator = "basic";
    const short goal_card = -1;
    const short min_card = -1;
    const short max_card = -1;
    unsigned long nb_aniso_agglo_lines = 16;

    agg.agglomerate_one_level(true,
                              nb_aniso_agglo_lines,
                              agglo_lines,
                              kind_of_agglomerator,
                              goal_card,
                              min_card,
                              max_card);

    ASSERT_EQ(16, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {15, 15, 0, 0, 1, 1, 2, 2, 3, 3, 14, 14, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
//    cout << endl;
//    for (auto i:agg.get_fc_2_cc()) {
//        cout << i << ", ";
//    }
//    cout << endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(box_5x5x5_iso_and_aniso, agglomerate_one_level_basic_aniso) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> anisotropic_lines = {};
    agg.agglomerate_one_level(true, 0, anisotropic_lines);
//    EXPECT_EQ(41, agg.get_nb_cc());
    ASSERT_EQ(18, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {0, 5, 4, 1, 6, 13, 12, 10, 7, 15, 14, 11, 2, 9, 8, 3,
                                0, 5, 4, 1, 6, 13, 12, 10, 7, 15, 14, 11, 2, 9, 8, 3,
                                0, 5, 4, 1, 6, 13, 12, 10, 7, 15, 14, 11, 2, 9, 8, 3,
                                16, 16, 16, 17, 16, 16, 16, 17, 16, 16, 17, 17, 17, 17, 17, 17,};
//    cout<<endl;
//    for (auto i:agg.get_fc_2_cc())
//    {
//        cout<<i<<", ";
//    }
//    cout<<endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(Box_5x5x5_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_v2_seed_0_basic) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;

    Agglomerator agg((*g), verbose, is_visu_data_stored, dimension, true);

    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(&cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short min_card = -1;
    short goal_card = -1;
    short max_card = -1;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 0;
    unsigned short compactness = 0;  // output
    bool is_order_primary = false;  // useless for triconnected agglomeration
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);
    unordered_set<long> ref_s_of_fc({0, 1, 4, 5, 16, 17, 20, 21});
    EXPECT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    EXPECT_EQ(3, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    EXPECT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }

    ref_l_deque_of_seeds[2] = deque<long>({8, 2, 32});
    ref_l_deque_of_seeds[3] = deque<long>({15, 0, 60, 3, 63, 48, 12, 51});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);
}

TEST_F(Box_5x5x5_Dual_Graph, agglomerate_one_level) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> anisotropic_lines = {};
    agg.agglomerate_one_level(true, 0, anisotropic_lines);

    ASSERT_EQ(8, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4, 5, 5, 7, 7, 5, 5, 7, 7,
                                2, 2, 4, 4, 2, 2, 4, 4};


//    cout<<endl;
//    for (auto i:agg.get_fc_2_cc())
//    {
//        cout<<i<<", ";
//    }
//    cout<<endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}