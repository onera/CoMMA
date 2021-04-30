#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator_anisotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "../CoMMA_lib/Dual_Graph.h"
#include "../CoMMA_lib/First_Order_Neighbourhood.h"
#include "MGridGen_Dual_Graph.h"
#include "Nine_squares_3x3_Dual_Graph.h"
#include "gtest/gtest.h"

TEST_F(MGridGen_Dual_Graph, Agglomerator_Constructor) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    ASSERT_TRUE(agg.__is_visu_data_stored);
    ASSERT_EQ(0, agg.__verbose);
    ASSERT_EQ(2, agg.__dimension);
    ASSERT_TRUE(agg.__checks);
    ASSERT_FALSE(agg.__is_anisotropic);
}

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
    unordered_set<long> s_fc_for_current_cc = agg.__choose_optimal_cc_basic((*g),
                                                                            seed,
                                                                            d_neighbours_of_seed,
                                                                            goal_card,
                                                                            max_card,
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

TEST_F(Nine_squares_3x3_Dual_Graph, _choose_optimal_cc_and_update_seed_pool_9_Squares_isOrderPrimary_True) {

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

//    agg.agglomerate_one_level(false);

}

/////////////////
/////////////////
/////////////////
/////////////////
/////////////////


TEST(Agglomerator_TestSuite, agglomerate_Isotropic_First_Step_Box_5x5x5) {

    // without CGNS
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62, 66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132, 138, 144, 150, 155, 161, 167,
                                  172, 177, 183, 189, 194, 200, 206, 212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278, 282, 287, 293, 299, 304, 309, 315,
                                  321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind[344] = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7, 19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10, 22,
                                   3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25, 6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28,
                                   9, 12, 13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32, 1, 16, 17, 18, 21, 33, 2, 17, 18, 19,
                                   22, 34, 3, 18, 19, 23, 35, 4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26, 38, 7, 19, 22, 23,
                                   27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26, 29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24,
                                   28, 29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30, 31, 47, 16, 32, 33, 36, 48, 17, 32, 33,
                                   34, 37, 49, 18, 33, 34, 35, 38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38, 41, 53, 22, 34,
                                   37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40, 41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58,
                                   27, 39, 42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42, 45, 46, 47, 62, 31, 43, 46, 47,
                                   63, 32, 48, 49, 52, 33, 48, 49, 50, 53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49, 52, 53,
                                   54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52, 56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58,
                                   59, 62, 43, 55, 58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63, 47, 59, 62, 63};

    double adjMatrix_areaValues[344] = {30., 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 30., 2.5, 25., 2.5, 27.5, 2.5,
                                        2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 27.5,
                                        2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5,
                                        30., 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5,
                                        5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 5., 25., 25., 5.,
                                        2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5,
                                        2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5,
                                        5., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75.};

    double volumes[64] = {12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125};

    bool isFineCellAgglomerated[64];
    for (int i = 0; i < 64; i++) {
        isFineCellAgglomerated[i] = false;
    }
    long fineCellIndicesToCoarseCellIndices[64];
    for (int i = 0; i < 64; i++) {
        fineCellIndicesToCoarseCellIndices[i] = -1;
    }

    int goalCard = 8;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    unordered_set<long> isOnRidge = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    unordered_set<long> isOnValley = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    unordered_set<long> isOnCorner = {0, 3, 12, 15, 48, 51, 60, 63};

    int isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int maxCard = 10;

    int numberOfInts = 4;

    long indCoarseCell = 0;

    long sizes[4] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells};
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];
//    long isOnValley_size = sizes[4];
//    long isOnRidge_size = sizes[5];
//    long isOnCorner_size = sizes[6];

    agglomerate_Isotropic_First_Step(sizes,
                                     adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
                                     dict_DistributionOfCardinalOfCoarseElements,
                                     dict_Coarse_Cells,
                                     dict_Card_Coarse_Cells,
            //numberOfFineCells,
                                     isOnValley,
                                     isOnRidge,
                                     isOnCorner,
//                                     indCoarseCell,
//                                     numberOfFineAgglomeratedCells,
                                     isFineCellAgglomerated,
                                     isOnFineBnd,
                                     fineCellIndicesToCoarseCellIndices,
                                     delayedCoarseCells,
                                     goalCard,
                                     thresholdCard,
                                     maxCard);
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];
    ASSERT_EQ(64, sizes[0]);
    ASSERT_EQ(344, sizes[1]);
    ASSERT_EQ(8, sizes[2]);//indCoarseCell
    ASSERT_EQ(64, sizes[3]);
//    for (int i_cc = 0; i_cc < 8; i_cc++) {
//        cout << "dict_Coarse_Cells[" << i_cc << "]: [";
//        for (auto i:dict_Coarse_Cells[i_cc]) {
//            cout << i << ", ";
//        }
//        cout << "]" << endl;
//    }

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({42, 43, 46, 47, 58, 59, 62, 63});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({40, 41, 44, 45, 56, 57, 60, 61});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({34, 35, 38, 39, 50, 51, 54, 55});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({32, 33, 36, 37, 48, 49, 52, 53});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({10, 11, 14, 15, 26, 27, 30, 31});
    ref_dict_Coarse_Cells[5] = unordered_set<long>({8, 9, 12, 13, 24, 25, 28, 29});
    ref_dict_Coarse_Cells[6] = unordered_set<long>({2, 3, 6, 7, 18, 19, 22, 23});
    ref_dict_Coarse_Cells[7] = unordered_set<long>({0, 1, 4, 5, 16, 17, 20, 21});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[8] = unordered_set<long>({0, 1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[8] = 8;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

//    assert(dict_Coarse_Cells[0].count(42) == 1);
//    assert(dict_Coarse_Cells[0].count(43) == 1);
//    assert(dict_Coarse_Cells[0].count(46) == 1);
//    assert(dict_Coarse_Cells[0].count(47) == 1);
//    assert(dict_Coarse_Cells[0].count(58) == 1);
//    assert(dict_Coarse_Cells[0].count(59) == 1);
//    assert(dict_Coarse_Cells[0].count(62) == 1);
//    assert(dict_Coarse_Cells[0].count(63) == 1);

//    assert(dict_Coarse_Cells[1].count(40) == 1);
//    assert(dict_Coarse_Cells[1].count(41) == 1);
//    assert(dict_Coarse_Cells[1].count(44) == 1);
//    assert(dict_Coarse_Cells[1].count(45) == 1);
//    assert(dict_Coarse_Cells[1].count(56) == 1);
//    assert(dict_Coarse_Cells[1].count(57) == 1);
//    assert(dict_Coarse_Cells[1].count(60) == 1);
//    assert(dict_Coarse_Cells[1].count(61) == 1);

//    assert(dict_Coarse_Cells[2].count(34) == 1);
//    assert(dict_Coarse_Cells[2].count(35) == 1);
//    assert(dict_Coarse_Cells[2].count(38) == 1);
//    assert(dict_Coarse_Cells[2].count(39) == 1);
//    assert(dict_Coarse_Cells[2].count(50) == 1);
//    assert(dict_Coarse_Cells[2].count(51) == 1);
//    assert(dict_Coarse_Cells[2].count(54) == 1);
//    assert(dict_Coarse_Cells[2].count(55) == 1);


//    assert(dict_Coarse_Cells[5].count(8) == 1);
//    assert(dict_Coarse_Cells[5].count(9) == 1);
//    assert(dict_Coarse_Cells[5].count(12) == 1);
//    assert(dict_Coarse_Cells[5].count(13) == 1);
//    assert(dict_Coarse_Cells[5].count(24) == 1);
//    assert(dict_Coarse_Cells[5].count(25) == 1);
//    assert(dict_Coarse_Cells[5].count(28) == 1);
//    assert(dict_Coarse_Cells[5].count(29) == 1);

//    assert(dict_Coarse_Cells[4].count(10) == 1);
//    assert(dict_Coarse_Cells[4].count(11) == 1);
//    assert(dict_Coarse_Cells[4].count(14) == 1);
//    assert(dict_Coarse_Cells[4].count(15) == 1);
//    assert(dict_Coarse_Cells[4].count(26) == 1);
//    assert(dict_Coarse_Cells[4].count(27) == 1);
//    assert(dict_Coarse_Cells[4].count(30) == 1);
//    assert(dict_Coarse_Cells[4].count(31) == 1);

//    assert(dict_Coarse_Cells[3].count(32) == 1);
//    assert(dict_Coarse_Cells[3].count(33) == 1);
//    assert(dict_Coarse_Cells[3].count(36) == 1);
//    assert(dict_Coarse_Cells[3].count(37) == 1);
//    assert(dict_Coarse_Cells[3].count(48) == 1);
//    assert(dict_Coarse_Cells[3].count(49) == 1);
//    assert(dict_Coarse_Cells[3].count(52) == 1);
//    assert(dict_Coarse_Cells[3].count(53) == 1);

//    assert(dict_Coarse_Cells[6].count(2) == 1);
//    assert(dict_Coarse_Cells[6].count(3) == 1);
//    assert(dict_Coarse_Cells[6].count(6) == 1);
//    assert(dict_Coarse_Cells[6].count(7) == 1);
//    assert(dict_Coarse_Cells[6].count(18) == 1);
//    assert(dict_Coarse_Cells[6].count(19) == 1);
//    assert(dict_Coarse_Cells[6].count(22) == 1);
//    assert(dict_Coarse_Cells[6].count(23) == 1);

//    assert(dict_Coarse_Cells[7].count(0) == 1);
//    assert(dict_Coarse_Cells[7].count(1) == 1);
//    assert(dict_Coarse_Cells[7].count(4) == 1);
//    assert(dict_Coarse_Cells[7].count(5) == 1);
//    assert(dict_Coarse_Cells[7].count(16) == 1);
//    assert(dict_Coarse_Cells[7].count(17) == 1);
//    assert(dict_Coarse_Cells[7].count(20) == 1);
//    assert(dict_Coarse_Cells[7].count(21) == 1);


//    assert(!dict_Card_Coarse_Cells.empty());
//    assert(dict_Card_Coarse_Cells.size() == 1);
//    assert(dict_Card_Coarse_Cells[8].count(0) == 1);
//    assert(dict_Card_Coarse_Cells[8].count(1) == 1);
//    assert(dict_Card_Coarse_Cells[8].count(2) == 1);
//    assert(dict_Card_Coarse_Cells[8].count(3) == 1);
//    assert(dict_Card_Coarse_Cells[8].count(4) == 1);
//    assert(dict_Card_Coarse_Cells[8].count(5) == 1);
//    assert(dict_Card_Coarse_Cells[8].count(6) == 1);
//    assert(dict_Card_Coarse_Cells[8].count(7) == 1);
//    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
//    assert(dict_DistributionOfCardinalOfCoarseElements[8] == 8);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 5, 5, 4, 4, 7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 5, 5, 4, 4, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1,
                                                             0, 0, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0};
    for (int i = 0; i < 64; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(Agglomerator_TestSuite, agglomerate_Isotropic_One_Level_v_2_Box_5x5x5) {

    // without CGNS
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62, 66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132, 138, 144, 150, 155, 161, 167,
                                  172, 177, 183, 189, 194, 200, 206, 212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278, 282, 287, 293, 299, 304, 309, 315,
                                  321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind[344] = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7, 19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10, 22,
                                   3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25, 6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28,
                                   9, 12, 13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32, 1, 16, 17, 18, 21, 33, 2, 17, 18, 19,
                                   22, 34, 3, 18, 19, 23, 35, 4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26, 38, 7, 19, 22, 23,
                                   27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26, 29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24,
                                   28, 29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30, 31, 47, 16, 32, 33, 36, 48, 17, 32, 33,
                                   34, 37, 49, 18, 33, 34, 35, 38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38, 41, 53, 22, 34,
                                   37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40, 41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58,
                                   27, 39, 42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42, 45, 46, 47, 62, 31, 43, 46, 47,
                                   63, 32, 48, 49, 52, 33, 48, 49, 50, 53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49, 52, 53,
                                   54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52, 56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58,
                                   59, 62, 43, 55, 58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63, 47, 59, 62, 63};

    double adjMatrix_areaValues[344] = {30., 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 30., 2.5, 25., 2.5, 27.5, 2.5,
                                        2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 27.5,
                                        2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5,
                                        30., 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5,
                                        5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 5., 25., 25., 5.,
                                        2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5,
                                        2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5,
                                        5., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75.};

    double volumes[64] = {12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125};

    bool isFineCellAgglomerated[64];
    for (int i = 0; i < 64; i++) {
        isFineCellAgglomerated[i] = false;
    }
    long fineCellIndicesToCoarseCellIndices[64];
    for (int i = 0; i < 64; i++) {
        fineCellIndicesToCoarseCellIndices[i] = -1;
    }

    unordered_set<long> isOnRidge = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    unordered_set<long> isOnValley = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    unordered_set<long> isOnCorner = {0, 3, 12, 15, 48, 51, 60, 63};

    int isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int minCard = 6;
    int goalCard = 8;
    int maxCard = 10;

    int numberOfInts = 4;

    long indCoarseCell = 0;

    long sizes[4] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells};

    bool checks = true;
    bool verbose = true;

//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];


    agglomerate_Isotropic_One_Level_v_2(sizes,
                                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                                        fineCellIndicesToCoarseCellIndices,
                                        isFineCellAgglomerated,

                                        isOnValley,
                                        isOnRidge,
                                        isOnCorner,

                                        isOnFineBnd,
                                        minCard,
                                        goalCard,
                                        maxCard,
                                        thresholdCard,
                                        checks,
                                        verbose);
    ASSERT_EQ(64, sizes[0]);
    ASSERT_EQ(344, sizes[1]);
    ASSERT_EQ(8, sizes[2]);//indCoarseCell
    ASSERT_EQ(64, sizes[3]);//numberOfFineAgglomeratedCells

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 5, 5, 4, 4, 7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 5, 5, 4, 4, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1,
                                                             0, 0, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0};

    for (int i = 0; i < 64; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(Agglomerator_TestSuite, agglomerate_Isotropic_One_Level_v_2_Box_5x5x5_goalCard_9) {

    // without CGNS
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62, 66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132, 138, 144, 150, 155, 161, 167,
                                  172, 177, 183, 189, 194, 200, 206, 212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278, 282, 287, 293, 299, 304, 309, 315,
                                  321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind[344] = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7, 19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10, 22,
                                   3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25, 6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28,
                                   9, 12, 13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32, 1, 16, 17, 18, 21, 33, 2, 17, 18, 19,
                                   22, 34, 3, 18, 19, 23, 35, 4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26, 38, 7, 19, 22, 23,
                                   27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26, 29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24,
                                   28, 29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30, 31, 47, 16, 32, 33, 36, 48, 17, 32, 33,
                                   34, 37, 49, 18, 33, 34, 35, 38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38, 41, 53, 22, 34,
                                   37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40, 41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58,
                                   27, 39, 42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42, 45, 46, 47, 62, 31, 43, 46, 47,
                                   63, 32, 48, 49, 52, 33, 48, 49, 50, 53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49, 52, 53,
                                   54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52, 56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58,
                                   59, 62, 43, 55, 58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63, 47, 59, 62, 63};

    double adjMatrix_areaValues[344] = {30., 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 30., 2.5, 25., 2.5, 27.5, 2.5,
                                        2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 27.5,
                                        2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5,
                                        30., 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5,
                                        5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 5., 25., 25., 5.,
                                        2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5,
                                        2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5,
                                        5., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75.};

    double volumes[64] = {12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125};

    bool isFineCellAgglomerated[64];
    for (int i = 0; i < 64; i++) {
        isFineCellAgglomerated[i] = false;
    }
    long fineCellIndicesToCoarseCellIndices[64];
    for (int i = 0; i < 64; i++) {
        fineCellIndicesToCoarseCellIndices[i] = -1;
    }

    unordered_set<long> isOnRidge = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    unordered_set<long> isOnValley = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    unordered_set<long> isOnCorner = {0, 3, 12, 15, 48, 51, 60, 63};

    int isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int minCard = 6;
    int goalCard = 9;
    int maxCard = 10;

    int numberOfInts = 4;

    long indCoarseCell = 0;

    long sizes[4] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells};

    bool checks = false;
    bool verbose = false;

//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];

    agglomerate_Isotropic_One_Level_v_2(sizes,
                                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                                        fineCellIndicesToCoarseCellIndices,
                                        isFineCellAgglomerated,

                                        isOnValley,
                                        isOnRidge,
                                        isOnCorner,

                                        isOnFineBnd,
                                        minCard,
                                        goalCard,
                                        maxCard,
                                        thresholdCard,
                                        checks,
                                        verbose);
    ASSERT_EQ(64, sizes[0]);
    ASSERT_EQ(344, sizes[1]);
    ASSERT_EQ(7, sizes[2]);//indCoarseCell
    ASSERT_EQ(64, sizes[3]);//numberOfFineAgglomeratedCells

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {3, 6, 6, 6, 5, 6, 6, 4, 5, 5, 4, 4, 5, 5, 4, 4, 3, 6, 6, 6, 5, 6, 6, 4, 5, 5, 4, 4, 5, 5, 4, 4, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1,
                                                             0, 0, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0};

    for (int i = 0; i < 64; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(Agglomerator_TestSuite, agglomerate_Anisotropic_One_Level) {

    long numberOfFineCells = 10;
    long numberOfFineAgglomeratedCells = 0;
    long indCoarseCell = 0;

    long fineAgglomerationLines_array_Idx_size = 5;
    long fineAgglomerationLines_array_Idx[5] = {0, 3, 4, 6, 10};
    long fineAgglomerationLines_array[10] = {0, 1, 5, 2, 3,
                                             6, 4, 7, 8, 9};

    long fineCellToCoarseCell[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    bool isFineCellAgglomerated[10] = {false, false, false, false, false, false, false, false, false, false};
    long AnisotropicCompliantCoarseCells_size = 10;
    long sizes[5] = {fineAgglomerationLines_array_Idx_size, numberOfFineCells, numberOfFineAgglomeratedCells, indCoarseCell, AnisotropicCompliantCoarseCells_size};

    long AnisotropicCompliantCoarseCells_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    agglomerate_Anisotropic_One_Level_without_list_lines(sizes,
                                                         fineAgglomerationLines_array_Idx, fineAgglomerationLines_array,
                                                         fineCellToCoarseCell,
                                                         isFineCellAgglomerated,
                                                         AnisotropicCompliantCoarseCells_array);

    fineAgglomerationLines_array_Idx_size = sizes[0];
    numberOfFineCells = sizes[1];
    numberOfFineAgglomeratedCells = sizes[2];
    indCoarseCell = sizes[3];
    AnisotropicCompliantCoarseCells_size = sizes[4];

    ASSERT_EQ(9, numberOfFineAgglomeratedCells);
    ASSERT_EQ(4, indCoarseCell);
    long ref_fineCellToCoarseCell[10] = {0, 0, -1, 1, 2, 0, 1, 2, 3, 3};
    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_fineCellToCoarseCell[i], fineCellToCoarseCell[i]);
    }

    long ref_AnisotropicCompliantCoarseCells_array[4] = {0, 1, 2, 3};
    for (int i = 0; i < AnisotropicCompliantCoarseCells_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        ASSERT_EQ(ref_AnisotropicCompliantCoarseCells_array[i], AnisotropicCompliantCoarseCells_array[i]);
    }
//    cout<<endl;
    bool ref_isFineCellAgglomerated[10] = {true, true, false, true, true, true, true, true, true, true};
    for (int i = 0; i < numberOfFineCells; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        ASSERT_EQ(ref_isFineCellAgglomerated[i], isFineCellAgglomerated[i]);
    }
//    cout<<endl;
    ASSERT_EQ(3, fineAgglomerationLines_array_Idx_size - 1);

    long ref_fineAgglomerationLines_array_Idx[4] = {0, 1, 2, 4};
    for (int i = 0; i < fineAgglomerationLines_array_Idx_size; i++) {
        ASSERT_EQ(ref_fineAgglomerationLines_array_Idx[i], fineAgglomerationLines_array_Idx[i]);
    }
    long ref_fineAgglomerationLines_array[4] = {0, 1, 2, 3};
    for (int i = 0; i < fineAgglomerationLines_array_Idx[fineAgglomerationLines_array_Idx_size - 1]; i++) {
        ASSERT_EQ(ref_fineAgglomerationLines_array[i], fineAgglomerationLines_array[i]);
    }


}

TEST(Agglomerator_TestSuite, agglomerate_Anisotropic_One_Level_2) {


    long numberOfFineCells = 21;
    long numberOfFineAgglomeratedCells = 0;
    long indCoarseCell = 1;

    long fineAgglomerationLines_array_Idx_size = 8;
    long fineAgglomerationLines_array_Idx[8] = {0, 6, 9, 10, 12, 16, 21, 21};
    long fineAgglomerationLines_array[21] = {15, 16, 17, 18, 19,
                                             20, 0, 1, 5, 2,
                                             3, 6, 4, 7, 8,
                                             9, 10, 11, 12, 13,
                                             14};

    long fineCellToCoarseCell[21] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    bool isFineCellAgglomerated[21] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    long AnisotropicCompliantCoarseCells_size = 21;
    long sizes[5] = {fineAgglomerationLines_array_Idx_size, numberOfFineCells, numberOfFineAgglomeratedCells, indCoarseCell, AnisotropicCompliantCoarseCells_size};

    long AnisotropicCompliantCoarseCells_array[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    agglomerate_Anisotropic_One_Level_without_list_lines(sizes,
                                                         fineAgglomerationLines_array_Idx, fineAgglomerationLines_array,
                                                         fineCellToCoarseCell,
                                                         isFineCellAgglomerated,
                                                         AnisotropicCompliantCoarseCells_array);

    fineAgglomerationLines_array_Idx_size = sizes[0];
    numberOfFineCells = sizes[1];
    numberOfFineAgglomeratedCells = sizes[2];
    indCoarseCell = sizes[3];
    AnisotropicCompliantCoarseCells_size = sizes[4];

    assert(numberOfFineAgglomeratedCells == 20);
    assert(indCoarseCell == 10);
    long ref_fineCellToCoarseCell[21] = {4, 4, -1, 5, 6, 4, 5, 6, 7, 7, 8, 8, 9, 9, 9, 1, 1, 2, 2, 3, 3};
    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_fineCellToCoarseCell[i], fineCellToCoarseCell[i]);
    }

    long ref_AnisotropicCompliantCoarseCells_array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < AnisotropicCompliantCoarseCells_size; i++) {
        ASSERT_EQ(ref_AnisotropicCompliantCoarseCells_array[i], AnisotropicCompliantCoarseCells_array[i]);
    }

    bool ref_isFineCellAgglomerated[21] = {true, true, false, true, true, true, true, true, true, true,
                                           true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated[i], isFineCellAgglomerated[i]);
    }
    ASSERT_EQ(fineAgglomerationLines_array_Idx_size - 1, 5);

    long ref_fineAgglomerationLines_array_Idx[6] = {0, 3, 4, 5, 7, 9};
    for (int i = 0; i < fineAgglomerationLines_array_Idx_size; i++) {
        ASSERT_EQ(ref_fineAgglomerationLines_array_Idx[i], fineAgglomerationLines_array_Idx[i]);
    }

    long ref_fineAgglomerationLines_array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < fineAgglomerationLines_array_Idx[fineAgglomerationLines_array_Idx_size - 1]; i++) {

        ASSERT_EQ(ref_fineAgglomerationLines_array[i], fineAgglomerationLines_array[i]);
    }

}

TEST(Agglomerator_TestSuite, agglomerate_Anisotropic_One_Level_Box_5x5x5_Aniso_MG_1_level) {

    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long indCoarseCell = 0;

    long fineAgglomerationLines_array_Idx_size = 17;
    long fineAgglomerationLines_array_Idx[17] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36,
                                                 40, 44, 48, 52, 56, 60, 64};
    long fineAgglomerationLines_array[64] = {5, 21, 37, 53, 6, 22, 38, 54, 9, 25,
                                             41, 57, 10, 26, 42, 58, 49, 33, 17, 1,
                                             50, 34, 18, 2, 52, 36, 20, 4, 55, 39,
                                             23, 7, 56, 40, 24, 8, 59, 43, 27, 11,
                                             61, 45, 29, 13, 62, 46, 30, 14, 48, 32,
                                             16, 0, 51, 35, 19, 3, 60, 44, 28, 12,
                                             63, 47, 31, 15};

    long *fineCellToCoarseCell = new long[64];
    bool *isFineCellAgglomerated = new bool[64];
    long *AnisotropicCompliantCoarseCells_array = new long[64];
    for (int i = 0; i < 64; i++) {
        fineCellToCoarseCell[i] = -1;
        isFineCellAgglomerated[i] = false;
        AnisotropicCompliantCoarseCells_array[i] = i;
    }

    long AnisotropicCompliantCoarseCells_size = 64;
    long sizes[5] = {fineAgglomerationLines_array_Idx_size, numberOfFineCells, numberOfFineAgglomeratedCells, indCoarseCell, AnisotropicCompliantCoarseCells_size};


    agglomerate_Anisotropic_One_Level_without_list_lines(sizes,
                                                         fineAgglomerationLines_array_Idx, fineAgglomerationLines_array,
                                                         fineCellToCoarseCell,
                                                         isFineCellAgglomerated,
                                                         AnisotropicCompliantCoarseCells_array);

    fineAgglomerationLines_array_Idx_size = sizes[0];
    numberOfFineCells = sizes[1];
    numberOfFineAgglomeratedCells = sizes[2];
    indCoarseCell = sizes[3];
    AnisotropicCompliantCoarseCells_size = sizes[4];
//    cout << "numberOfFineAgglomeratedCells " << numberOfFineAgglomeratedCells << endl;
//    cout << "indCoarseCell " << indCoarseCell << endl;
    ASSERT_EQ(64, numberOfFineAgglomeratedCells);
    ASSERT_EQ(32, indCoarseCell);
    long ref_fineCellToCoarseCell[64] = {25, 9, 11, 27,
                                         13, 0, 2, 15,
                                         17, 4, 6, 19,
                                         29, 21, 23, 31,

                                         25, 9, 11, 27,
                                         13, 0, 2, 15,
                                         17, 4, 6, 19,
                                         29, 21, 23, 31,

                                         24, 8, 10, 26,
                                         12, 1, 3, 14,
                                         16, 5, 7, 18,
                                         28, 20, 22, 30,

                                         24, 8, 10, 26,
                                         12, 1, 3, 14,
                                         16, 5, 7, 18,
                                         28, 20, 22, 30};
    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_fineCellToCoarseCell[i], fineCellToCoarseCell[i]);
    }

    long ref_AnisotropicCompliantCoarseCells_array[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                                                          17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    for (int i = 0; i < AnisotropicCompliantCoarseCells_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        ASSERT_EQ(ref_AnisotropicCompliantCoarseCells_array[i], AnisotropicCompliantCoarseCells_array[i]);
    }
//    cout<<endl;
    bool *ref_isFineCellAgglomerated = new bool[64];
    for (int i = 0; i < numberOfFineCells; i++) {
        ref_isFineCellAgglomerated[i] = true;
    }
    for (int i = 0; i < numberOfFineCells; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        ASSERT_EQ(ref_isFineCellAgglomerated[i], isFineCellAgglomerated[i]);
    }
//    cout<<endl;
    ASSERT_EQ(16, fineAgglomerationLines_array_Idx_size - 1);

    long ref_fineAgglomerationLines_array_Idx[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    for (int i = 0; i < fineAgglomerationLines_array_Idx_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        ASSERT_EQ(ref_fineAgglomerationLines_array_Idx[i], fineAgglomerationLines_array_Idx[i]);
    }
    long ref_fineAgglomerationLines_array[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                                 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                                                 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    for (int i = 0; i < fineAgglomerationLines_array_Idx[fineAgglomerationLines_array_Idx_size - 1]; i++) {
        ASSERT_EQ(ref_fineAgglomerationLines_array[i], fineAgglomerationLines_array[i]);
    }

    delete[] fineCellToCoarseCell;
    delete[] isFineCellAgglomerated;
    delete[] AnisotropicCompliantCoarseCells_array;
    delete[] ref_isFineCellAgglomerated;

}
