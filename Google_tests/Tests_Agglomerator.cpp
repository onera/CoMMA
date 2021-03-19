#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "gtest/gtest.h"

TEST(Agglomerator_TestSuite, choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_False) {
    //"""
    //9 squares (3*3)
    //:return:
    //"""
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[33] = {1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0};

    long seed = 0;
    vector<queue<long>> listOfSeeds(4);
    for (int i = 0; i < 4; i++) {
        listOfSeeds[i] = queue<long>();
    }

    int goalCard = 4;
    int maxCard = 6;
    bool isFineCellAgglomerated_tmp[9] = {false, false, false, false, false, false, false, false, false};
    unordered_map<long, int> dict_Neighbours_Of_Seed = computation_Of_Neighbourhood(seed, 3, row_ptr,
                                                                                    col_ind, maxCard,  // __goalCard
                                                                                    isFineCellAgglomerated_tmp);
    unordered_map<long, int> ref_d_neighbours_of_seed;
    ref_d_neighbours_of_seed[1] = 1;
    ref_d_neighbours_of_seed[2] = 2;
    ref_d_neighbours_of_seed[3] = 1;
    ref_d_neighbours_of_seed[4] = 2;
    ref_d_neighbours_of_seed[5] = 3;
    ref_d_neighbours_of_seed[6] = 2;
    ref_d_neighbours_of_seed[7] = 3;

    ASSERT_EQ(ref_d_neighbours_of_seed, dict_Neighbours_Of_Seed);

    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
    unordered_set<long> set_of_fine_cells_for_Current_Coarse_Cell =
            choice_Of_Agglomerated_Cells(
                    seed, listOfSeeds, dict_Neighbours_Of_Seed,
                    row_ptr,
                    col_ind, values, volumes,
                    goalCard,
                    maxCard,
                    isFineCellAgglomerated_tmp,
                    isOnBnd,
                    numberOfFineAgglomeratedCells_tmp,
                    false);

    unordered_set<long> ref_s_of_fc({0, 1, 3, 4});
    ASSERT_EQ(ref_s_of_fc, set_of_fine_cells_for_Current_Coarse_Cell);

    ASSERT_EQ(4, numberOfFineAgglomeratedCells_tmp);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, false, true, true, false, false, false, false};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
    }

    unordered_map<long, int> ref_d_neighbours_of_seed_2;
    ref_d_neighbours_of_seed_2[2] = 2;
    ref_d_neighbours_of_seed_2[5] = 3;
    ref_d_neighbours_of_seed_2[6] = 2;
    ref_d_neighbours_of_seed_2[7] = 3;
    ASSERT_EQ(ref_d_neighbours_of_seed_2, dict_Neighbours_Of_Seed);

    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>({2, 6});
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);
}

TEST(Agglomerator_TestSuite, choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True) {
    //"""
    //9 squares (3*3)
    //:return:
    //"""
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1, 3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6, 7, 4, 6, 7, 8,
                        5, 7, 8};
    double values[33] = {1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0};

    int iLevel = 1;
    long seed = 0;
    vector<queue<long>> listOfSeeds(4);
    for (int i = 0; i < 4; i++) {
        listOfSeeds[i] = queue<long>();
    }

    int goalCard = 4;
    int maxCard = 6;
    bool isFineCellAgglomerated_tmp[9] = {false, false, false, false, false, false, false, false, false};
    unordered_map<long, int> dict_Neighbours_Of_Seed = computation_Of_Neighbourhood(seed, 3, row_ptr,
                                                                                    col_ind, maxCard,  // __goalCard
                                                                                    isFineCellAgglomerated_tmp);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 2;
    ref[3] = 1;
    ref[4] = 2;
    ref[5] = 3;
    ref[6] = 2;
    ref[7] = 3;

    ASSERT_EQ(ref, dict_Neighbours_Of_Seed);

    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
    unordered_set<long> set_of_fine_cells_for_Current_Coarse_Cell =
            choice_Of_Agglomerated_Cells(
                    seed, listOfSeeds, dict_Neighbours_Of_Seed,
                    row_ptr,
                    col_ind, values, volumes,
                    goalCard,
                    maxCard,
                    isFineCellAgglomerated_tmp,
                    isOnBnd,
                    numberOfFineAgglomeratedCells_tmp,
                    true);

    unordered_set<long> ref_s_of_fc({0, 1, 3, 6});  //0,1, 2, 3 is also correct!
    ASSERT_EQ(ref_s_of_fc, set_of_fine_cells_for_Current_Coarse_Cell);

    ASSERT_EQ(4, numberOfFineAgglomeratedCells_tmp);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, false, true, false, false, true, false, false};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
    }
    unordered_map<long, int> ref_d_neighbours_of_seed_2;
    ref_d_neighbours_of_seed_2[4] = 2;
    ref_d_neighbours_of_seed_2[5] = 3;
    ref_d_neighbours_of_seed_2[2] = 2;
    ref_d_neighbours_of_seed_2[7] = 3;
    ASSERT_EQ(ref_d_neighbours_of_seed_2, dict_Neighbours_Of_Seed);

    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>({4});
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>({2});
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);

}

TEST(Agglomerator_TestSuite, choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_False_GoalCard_8_neighbours_4) {
    //"""
    //9 squares (3*3)
    //:return:
    //"""

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[33] = {1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0};

    long seed = 0;
    vector<queue<long>> listOfSeeds(4);
    for (int i = 0; i < 4; i++) {
        listOfSeeds[i] = queue<long>();
    }

    int goalCard = 8;
    int maxCard = 8;
    bool isFineCellAgglomerated_tmp[9] = {false, false, false, false, false, false, false, false, false};
    unordered_map<long, int> dict_Neighbours_Of_Seed = computation_Of_Neighbourhood(seed, 4, row_ptr,
                                                                                    col_ind, maxCard,  // __goalCard
                                                                                    isFineCellAgglomerated_tmp);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 2;
    ref[3] = 1;
    ref[4] = 2;
    ref[5] = 3;
    ref[6] = 2;
    ref[7] = 3;
    ref[8] = 4;
    ASSERT_EQ(ref, dict_Neighbours_Of_Seed);

    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
    unordered_set<long> set_of_fine_cells_for_Current_Coarse_Cell =
            choice_Of_Agglomerated_Cells(
                    seed, listOfSeeds, dict_Neighbours_Of_Seed,
                    row_ptr,
                    col_ind, values, volumes,
                    goalCard,
                    maxCard,
                    isFineCellAgglomerated_tmp,
                    isOnBnd,
                    numberOfFineAgglomeratedCells_tmp,
                    false);

    unordered_set<long> ref_s_of_fc({0, 1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(ref_s_of_fc, set_of_fine_cells_for_Current_Coarse_Cell);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, false};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
    }

    unordered_map<long, int> ref_d_neighbours_of_seed_2;
    ref_d_neighbours_of_seed_2[8] = 4;
    ASSERT_EQ(ref_d_neighbours_of_seed_2, dict_Neighbours_Of_Seed);

    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>({8});
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);
}

TEST(Agglomerator_TestSuite, choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True_GoalCard_8_neighbours_4) {

    //"""
    //9 squares (3*3)
    //:return:
    //"""
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[33] = {1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0};

    long seed = 0;
    vector<queue<long>> listOfSeeds(4);
    for (int i = 0; i < 4; i++) {
        listOfSeeds[i] = queue<long>();
    }

    int goalCard = 8;
    int maxCard = 8;
    bool isFineCellAgglomerated_tmp[9] = {false, false, false, false, false, false, false, false, false};
    unordered_map<long, int> dict_Neighbours_Of_Seed = computation_Of_Neighbourhood(seed, 4, row_ptr,
                                                                                    col_ind, maxCard,  // __goalCard
                                                                                    isFineCellAgglomerated_tmp);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 2;
    ref[3] = 1;
    ref[4] = 2;
    ref[5] = 3;
    ref[6] = 2;
    ref[7] = 3;
    ref[8] = 4;
    ASSERT_EQ(ref, dict_Neighbours_Of_Seed);

    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
    unordered_set<long> set_of_fine_cells_for_Current_Coarse_Cell =
            choice_Of_Agglomerated_Cells(
                    seed, listOfSeeds, dict_Neighbours_Of_Seed,
                    row_ptr,
                    col_ind, values, volumes,
                    goalCard,
                    maxCard,
                    isFineCellAgglomerated_tmp,
                    isOnBnd,
                    numberOfFineAgglomeratedCells_tmp,
                    true);

    unordered_set<long> ref_s_of_fc({0, 1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(ref_s_of_fc, set_of_fine_cells_for_Current_Coarse_Cell);

    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, false};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
    }

    unordered_map<long, int> ref_d_neighbours_of_seed_2;
    ref_d_neighbours_of_seed_2[8] = 4;
    ASSERT_EQ(ref_d_neighbours_of_seed_2, dict_Neighbours_Of_Seed);

    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>({8});
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);
}

TEST(Agglomerator_TestSuite, choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True_GoalCard_8) {

    //"""
    //9 squares (3*3)
    //:return:
    //"""
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[33] = {1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0, 1.0, 1.0,
                         1.0, 1.0, 1.0};

    long seed = 0;
    vector<queue<long>> listOfSeeds(4);
    for (int i = 0; i < 4; i++) {
        listOfSeeds[i] = queue<long>();
    }

    int goalCard = 8;
    int maxCard = 8;
    bool isFineCellAgglomerated_tmp[9] = {false, false, false, false, false, false, false, false, false};
    unordered_map<long, int> dict_Neighbours_Of_Seed = computation_Of_Neighbourhood(seed, 3, row_ptr,
                                                                                    col_ind, maxCard,  // __goalCard
                                                                                    isFineCellAgglomerated_tmp);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 2;
    ref[3] = 1;
    ref[4] = 2;
    ref[5] = 3;
    ref[6] = 2;
    ref[7] = 3;
    ASSERT_EQ(ref, dict_Neighbours_Of_Seed);

    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
    unordered_set<long> set_of_fine_cells_for_Current_Coarse_Cell =
            choice_Of_Agglomerated_Cells(
                    seed, listOfSeeds, dict_Neighbours_Of_Seed,
                    row_ptr,
                    col_ind, values, volumes,
                    goalCard,
                    maxCard,
                    isFineCellAgglomerated_tmp,
                    isOnBnd,
                    numberOfFineAgglomeratedCells_tmp,
                    true);

    unordered_set<long> ref_s_of_fc({0, 1, 2, 3, 4, 5, 6, 7});
    ASSERT_EQ(ref_s_of_fc, set_of_fine_cells_for_Current_Coarse_Cell);

    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, false};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
    }
    unordered_map<long, int> ref_d_neighbours_of_seed_2;
    ASSERT_EQ(ref_d_neighbours_of_seed_2, dict_Neighbours_Of_Seed);

    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>({8});
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);
}

