
#include "gtest/gtest.h"

//#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"

#include "MGridGen_Dual_Graph.h"
#include "MGridGen_ext_Dual_Graph.h"
#include "Nine_squares_3x3_Dual_Graph.h"
#include "Box_5x5x5_Dual_Graph.h"
#include "Box_1_prism_Dual_Graph.h"



TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_tric_v2_seed_0) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph *cc_graph = new Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "triconnected";
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

}

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_tric_v2_seed_1) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph *cc_graph = new Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "triconnected";
    short min_card = -1;
    short goal_card = 4;
    short max_card = 6;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 1;
    unsigned short compactness = 0;  // output
    bool is_order_primary = false;  // useless for triconnected agglomeration
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({0, 1, 2, 3, 4, 5});
    EXPECT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    EXPECT_EQ(2, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    EXPECT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[1] = deque<long>({7});
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

    unordered_map<long, unsigned short> ref_d_neighbours_of_seed = {{2, 2},
                                                                    {5, 3},
                                                                    {6, 2},
                                                                    {7, 3}};
}

TEST_F(MGridGen_ext_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_tric_v2_seed_0) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph *cc_graph = new Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "triconnected";
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
    EXPECT_EQ(2, compactness);

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

TEST_F(MGridGen_ext_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_tric_v2_seed_7) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph *cc_graph = new Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "triconnected";
    short min_card = -1;
    short goal_card = 4;
    short max_card = 6;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 7;
    unsigned short compactness = 0;  // output
    bool is_order_primary = false;  // useless for triconnected agglomeration
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({5, 3, 4, 11, 12, 13, 6, 19, 7, 20, 14});
    EXPECT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    EXPECT_EQ(2, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    EXPECT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[0] = deque<long>({1});
    ref_l_deque_of_seeds[2] = deque<long>({0, 7, 15, 21});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);
}

TEST_F(MGridGen_ext_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_tric_v2_seed_21) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph *cc_graph = new Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "triconnected";
    short min_card = -1;
    short goal_card = 4;
    short max_card = 6;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 21;
    unsigned short compactness = 0;  // output
    bool is_order_primary = false;  // useless for triconnected agglomeration
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({12, 13, 14, 19, 20, 21});
    EXPECT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    EXPECT_EQ(2, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    EXPECT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[1] = deque<long>({18});
    ref_l_deque_of_seeds[2] = deque<long>({0, 7, 15, 21});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);
}

TEST_F(MGridGen_ext_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_tric_v2_seed_15) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph *cc_graph = new Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "triconnected";
    short min_card = -1;
    short goal_card = 4;
    short max_card = 6;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 15;
    unsigned short compactness = 0;  // output
    bool is_order_primary = false;  // useless for triconnected agglomeration
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({16, 17, 8, 9, 15});
    EXPECT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    EXPECT_EQ(2, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    EXPECT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[1] = deque<long>({18});
    ref_l_deque_of_seeds[2] = deque<long>({0, 7, 15, 21});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);
}

TEST_F(MGridGen_ext_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_tric_v2_seed_12) {

    Agglomerator agg((*g), 0, 1, 2, true);

    Coarse_Cell_Graph *cc_graph = new Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "triconnected";
    short min_card = -1;
    short goal_card = 4;
    short max_card = 6;
    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);

    long seed = 12;
    unsigned short compactness = 0;  // output
    bool is_order_primary = false;  // useless for triconnected agglomeration
    bool increase_neighbouring = true;
    unordered_set<long> s_fc_for_current_cc = agg._choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                             compactness,
                                                                                             is_order_primary,
                                                                                             increase_neighbouring);

    unordered_set<long> ref_s_of_fc({12, 13, 14, 19, 20});
    EXPECT_EQ(ref_s_of_fc, s_fc_for_current_cc);
    EXPECT_EQ(2, compactness);

    vector<long> ref_fc_2_cc((*g).number_of_cells, -1);
    EXPECT_EQ(ref_fc_2_cc, (*agg.__cc_graphs)._fc_2_cc);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[0] = deque<long>({11, 6});
    ref_l_deque_of_seeds[1] = deque<long>({18});
    ref_l_deque_of_seeds[2] = deque<long>({0, 7, 15, 21});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);
}

TEST_F(MGridGen_ext_Dual_Graph, agglomerate_one_level_tric) {

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
    string kind_of_agglomerator = "triconnected";
    agg.agglomerate_one_level(false, 0, anisotropic_lines,
                              kind_of_agglomerator);
    ASSERT_EQ(5, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 4, 4, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3};
    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(MGridGen_Dual_Graph, agglomerate_one_level_tric) {

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
    string kind_of_agglomerator = "triconnected";
    agg.agglomerate_one_level(false, 0, anisotropic_lines,
                              kind_of_agglomerator);
    EXPECT_EQ(2, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0};
    EXPECT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
}

TEST_F(Box_5x5x5_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_v2_seed_0_tric) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 3;

    Agglomerator agg((*g), verbose, is_visu_data_stored, dimension, true);

    Coarse_Cell_Graph *cc_graph = new Coarse_Cell_Graph((*g));
    agg.initialize_l_cc_graphs_for_tests_only(cc_graph);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "triconnected";
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

    ref_l_deque_of_seeds[2] = deque<long>({32, 8, 2});
    ref_l_deque_of_seeds[3] = deque<long>({15, 0, 60, 3, 63, 48, 12, 51});
    EXPECT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);
}

TEST_F(Box_5x5x5_Dual_Graph, agglomerate_one_level_tric_aniso) {

    // The mesh does not contain anisotropic cells: thus it is an isotropic agglomeration : same results as agglomerate_one_level_tric_iso)

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

    string kind_of_agglomerator = "triconnected";
    agg.agglomerate_one_level(true, 0, anisotropic_lines, kind_of_agglomerator);

    ASSERT_EQ(8, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4, 5, 5, 7, 7, 5, 5, 7, 7,
                                2, 2, 4, 4, 2, 2, 4, 4};
    cout << endl;
    for (auto i:agg.get_fc_2_cc()) {
        cout << i << ", ";
    }
    cout << endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_5x5x5_Dual_Graph, agglomerate_one_level_tric_iso) {

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

    string kind_of_agglomerator = "triconnected";
    agg.agglomerate_one_level(false, 0, anisotropic_lines, kind_of_agglomerator);

    ASSERT_EQ(8, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {1, 1, 3, 3,
                                1, 1, 3, 3,
                                6, 6, 0, 0,
                                6, 6, 0, 0,
            //
                                1, 1, 3, 3,
                                1, 1, 3, 3,
                                6, 6, 0, 0,
                                6, 6, 0, 0,
            //
                                5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4};
    cout << endl;
    for (auto i:agg.get_fc_2_cc()) {
        cout << i << ", ";
    }
    cout << endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}

TEST_F(Box_1_prism_Dual_Graph, agglomerate_one_level_tric) {

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

    string kind_of_agglomerator = "triconnected";
    agg.agglomerate_one_level(false, 0, anisotropic_lines, kind_of_agglomerator);
    ASSERT_EQ(40, agg.get_nb_cc());
    vector<long> ref_fc_2_cc = {1, 16, 16, 5, 18, 18, 8, 21, 21, 28, 24, 24, 6, 6, 6, 34, 34, 6, 36, 36, 34, 28, 28, 28, 6, 6, 25, 35, 6, 6, 13, 35, 35, 14, 14, 13, 6, 6, 6, 34, 6, 6, 36, 34, 34, 28,
                                28, 28, 6, 6, 6, 35, 35, 6, 36, 36, 35, 31, 14, 14, 6, 20, 20, 6, 6, 6, 34, 34, 34, 28, 12, 12, 3, 2, 2, 3, 3, 3, 27, 7, 7, 10, 10, 27, 3, 3, 23, 29, 3, 3, 13, 27, 27,
                                13, 13, 13, 3, 23, 23, 3, 3, 3, 27, 27, 7, 32, 32, 27, 3, 3, 3, 25, 25, 3, 13, 38, 27, 13, 13, 13, 0, 0, 0, 19, 19, 19, 4, 4, 4, 9, 9, 26, 16, 16, 0, 18, 18, 22, 21,
                                21, 4, 24, 24, 9, 3, 3, 3, 25, 3, 3, 38, 27, 38, 13, 10, 10, 0, 0, 0, 4, 19, 19, 4, 4, 4, 11, 9, 9, 2, 2, 0, 7, 2, 2, 7, 7, 4, 10, 37, 7, 2, 0, 0, 39, 19, 19, 7, 4, 4,
                                37, 37, 7, 17, 17, 17, 1, 1, 15, 5, 5, 33, 8, 8, 11, 2, 0, 0, 4, 4, 2, 7, 4, 4, 10, 11, 11, 16, 16, 16, 18, 18, 18, 21, 21, 21, 24, 24, 24, 16, 16, 16, 18, 18, 18, 21,
                                21, 21, 24, 24, 24, 1, 17, 17, 5, 1, 1, 8, 5, 5, 28, 8, 8, 17, 17, 17, 15, 15, 15, 11, 11, 15, 30, 11, 11, 1, 1, 6, 5, 5, 1, 8, 8, 5, 28, 28, 8, 17, 17, 17, 15, 15, 15,
                                11, 11, 15, 12, 10, 11, 20, 20, 20, 38, 15, 15, 38, 38, 38, 12, 12, 12, 20, 20, 3, 38, 15, 15, 38, 11, 38, 12, 12, 10, 25, 25, 25, 25, 25, 25, 13, 13, 38, 14, 13, 13,
                                2, 2, 2, 7, 7, 2, 7, 7, 7, 10, 10, 7};
    cout << endl;
    for (auto i:agg.get_fc_2_cc()) {
        cout << i << ", ";
    }
    cout << endl;

    ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());

}
