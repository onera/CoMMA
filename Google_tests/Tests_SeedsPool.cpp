#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "gtest/gtest.h"

#include "../CoMMA_lib/Seeds_Pool.h"

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Ridge_1) {

    long numberOfFineCells = 27;
    bool *isFineCellAgglomerated = new bool[numberOfFineCells];
    for (int i = 0; i < numberOfFineCells; i++) {
        isFineCellAgglomerated[i] = false;
    }
    isFineCellAgglomerated[0] = true;
    isFineCellAgglomerated[2] = true;
    isFineCellAgglomerated[6] = true;
    isFineCellAgglomerated[8] = true;
    isFineCellAgglomerated[18] = true;
    isFineCellAgglomerated[20] = true;
    isFineCellAgglomerated[24] = true;
    isFineCellAgglomerated[26] = true;

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>();
    listOfSeeds[3] = queue<long>({0, 2, 6, 8, 18, 20, 24, 26});

    unordered_set<long> isOnRidge({1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25});
    unordered_set<long> isOnValley({4, 10, 12, 14, 16, 22});

    long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated,
                                                     isOnRidge,
                                                     isOnValley);
    ASSERT_EQ(25, seed);
    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>();
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);

    delete[] isFineCellAgglomerated;
}

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Ridge_1_V2) {

    long numberOfFineCells = 27;
    bool *isFineCellAgglomerated = new bool[numberOfFineCells];
    for (int i = 0; i < numberOfFineCells; i++) {
        isFineCellAgglomerated[i] = false;
    }
    isFineCellAgglomerated[0] = true;
    isFineCellAgglomerated[2] = true;
    isFineCellAgglomerated[6] = true;
    isFineCellAgglomerated[8] = true;
    isFineCellAgglomerated[18] = true;
    isFineCellAgglomerated[20] = true;
    isFineCellAgglomerated[24] = true;
    isFineCellAgglomerated[26] = true;

    isFineCellAgglomerated[3] = true;

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>({3, 1});
    listOfSeeds[3] = queue<long>({0, 2, 6, 8, 18, 20, 24, 26});

    unordered_set<long> isOnRidge({1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25});
    unordered_set<long> isOnValley({4, 10, 12, 14, 16, 22});

    long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated, isOnRidge,
                                                     isOnValley);

    ASSERT_EQ(1, seed);
    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>();
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);

//    assert(seed == 1);
//    assert(listOfSeeds[0].empty());
//    assert(listOfSeeds[1].empty());
//    assert(listOfSeeds[2].empty());
//    assert(listOfSeeds[2].empty());
    delete[] isFineCellAgglomerated;
}

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Valley_4) {

    long numberOfFineCells = 27;
    bool *isFineCellAgglomerated = new bool[numberOfFineCells];
    for (int i = 0; i < numberOfFineCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    isFineCellAgglomerated[4] = false;
    isFineCellAgglomerated[10] = false;
    isFineCellAgglomerated[12] = false;
    isFineCellAgglomerated[14] = false;
    isFineCellAgglomerated[16] = false;
    isFineCellAgglomerated[22] = false;

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>({3, 1});
    listOfSeeds[3] = queue<long>({0, 2, 6, 8, 18, 20, 24, 26});

    unordered_set<long> isOnRidge({1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25});
    unordered_set<long> isOnValley({4, 10, 12, 14, 16, 22});

    long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated, isOnRidge,
                                                     isOnValley);
    ASSERT_EQ(4, seed);
    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>();
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);
    delete[] isFineCellAgglomerated;

}

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Valley_10) {

    long numberOfFineCells = 27;
    bool *isFineCellAgglomerated = new bool[numberOfFineCells];
    for (int i = 0; i < numberOfFineCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    isFineCellAgglomerated[4] = false;
    isFineCellAgglomerated[10] = false;
    isFineCellAgglomerated[12] = false;
    isFineCellAgglomerated[14] = false;
    isFineCellAgglomerated[16] = false;
    isFineCellAgglomerated[22] = false;

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>({10});
    listOfSeeds[2] = queue<long>({3, 1});
    listOfSeeds[3] = queue<long>({0, 2, 6, 8, 18, 20, 24, 26});

    unordered_set<long> isOnRidge({1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25});
    unordered_set<long> isOnValley({4, 10, 12, 14, 16, 22});

    long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated, isOnRidge,
                                                     isOnValley);
    ASSERT_EQ(10, seed);
    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>();
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);
    delete[] isFineCellAgglomerated;

}

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Valley_13) {

    long numberOfFineCells = 27;
    bool *isFineCellAgglomerated = new bool[numberOfFineCells];
    for (int i = 0; i < numberOfFineCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    isFineCellAgglomerated[13] = false;

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>({10});
    listOfSeeds[2] = queue<long>({3, 1});
    listOfSeeds[3] = queue<long>({0, 2, 6, 8, 18, 20, 24, 26});

    unordered_set<long> isOnRidge({1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25});
    unordered_set<long> isOnValley({4, 10, 12, 14, 16, 22});

    long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated, isOnRidge,
                                                     isOnValley);

    ASSERT_EQ(13, seed);
    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>();
    ref_l_of_seeds[3] = queue<long>();
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);
    delete[] isFineCellAgglomerated;

}

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_0) {

    long numberOfFineCells = 27;
    bool *isFineCellAgglomerated = new bool[numberOfFineCells];
    for (int i = 0; i < numberOfFineCells; i++) {
        isFineCellAgglomerated[i] = false;
    }
    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>();
    listOfSeeds[3] = queue<long>({0, 2, 6, 8, 18, 20, 24, 26});

    unordered_set<long> isOnRidge({1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25});
    unordered_set<long> isOnValley({4, 10, 12, 14, 16, 22});

    long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated, isOnRidge,
                                                     isOnValley);

    ASSERT_EQ(0, seed);
    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>();
    ref_l_of_seeds[3] = queue<long>({2, 6, 8, 18, 20, 24, 26});
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);

//    assert(seed == 0);
//    assert(listOfSeeds[0].empty());
//    assert(listOfSeeds[1].empty());
//    assert(listOfSeeds[2].empty());
//    assert(!listOfSeeds[3].empty());
//    assert(listOfSeeds[3].size() == 7);
//    assert(listOfSeeds[3].front() == 2);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 6);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 8);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 18);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 20);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 24);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 26);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].empty());

    delete[] isFineCellAgglomerated;
}

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_2) {

    long numberOfFineCells = 27;
    bool *isFineCellAgglomerated = new bool[numberOfFineCells];
    for (int i = 0; i < numberOfFineCells; i++) {
        isFineCellAgglomerated[i] = false;
    }
    isFineCellAgglomerated[0] = true;
    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>();
    listOfSeeds[3] = queue<long>({0, 2, 6, 8, 18, 20, 24, 26});

    unordered_set<long> isOnRidge({1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25});
    unordered_set<long> isOnValley({4, 10, 12, 14, 16, 22});

    long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated, isOnRidge,
                                                     isOnValley);

    ASSERT_EQ(2, seed);
    vector<queue<long>> ref_l_of_seeds(4);
    ref_l_of_seeds[0] = queue<long>();
    ref_l_of_seeds[1] = queue<long>();
    ref_l_of_seeds[2] = queue<long>();
    ref_l_of_seeds[3] = queue<long>({6, 8, 18, 20, 24, 26});
    ASSERT_EQ(ref_l_of_seeds, listOfSeeds);

//    assert(seed == 2);
//    assert(listOfSeeds[0].empty());
//    assert(listOfSeeds[1].empty());
//    assert(listOfSeeds[2].empty());
//    assert(!listOfSeeds[3].empty());
//    assert(listOfSeeds[3].size() == 6);
//    assert(listOfSeeds[3].front() == 6);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 8);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 18);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 20);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 24);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].front() == 26);
//    listOfSeeds[3].pop();
//    assert(listOfSeeds[3].empty());

    delete[] isFineCellAgglomerated;
}

TEST(SeedsPool_TestSuite, init_box_5x5x5) {
    long Box_5x5x5_is_on_bnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1,
                                    1, 2, 3, 2, 2, 3, 2, 1, 1, 2,
                                    1, 0, 0, 1, 1, 0, 0, 1, 2, 1,
                                    1, 2, 2, 1, 1, 2, 1, 0, 0, 1,
                                    1, 0, 0, 1, 2, 1, 1, 2, 3, 2,
                                    2, 3, 2, 1, 1, 2, 2, 1, 1, 2,
                                    3, 2, 2, 3};

    unordered_map<long, int> Box_5x5x5_is_on_bnd_d;
    Box_5x5x5_is_on_bnd_d[0] = 3;
    Box_5x5x5_is_on_bnd_d[1] = 2;
    Box_5x5x5_is_on_bnd_d[2] = 2;
    Box_5x5x5_is_on_bnd_d[3] = 3;
    Box_5x5x5_is_on_bnd_d[4] = 2;
    Box_5x5x5_is_on_bnd_d[5] = 1;
    Box_5x5x5_is_on_bnd_d[6] = 1;
    Box_5x5x5_is_on_bnd_d[7] = 2;
    Box_5x5x5_is_on_bnd_d[8] = 2;
    Box_5x5x5_is_on_bnd_d[9] = 1;
    Box_5x5x5_is_on_bnd_d[10] = 1;
    Box_5x5x5_is_on_bnd_d[11] = 2;
    Box_5x5x5_is_on_bnd_d[12] = 3;
    Box_5x5x5_is_on_bnd_d[13] = 2;
    Box_5x5x5_is_on_bnd_d[14] = 2;
    Box_5x5x5_is_on_bnd_d[15] = 3;
    Box_5x5x5_is_on_bnd_d[16] = 2;
    Box_5x5x5_is_on_bnd_d[17] = 1;
    Box_5x5x5_is_on_bnd_d[18] = 1;
    Box_5x5x5_is_on_bnd_d[19] = 2;
    Box_5x5x5_is_on_bnd_d[20] = 1;
    Box_5x5x5_is_on_bnd_d[23] = 1;
    Box_5x5x5_is_on_bnd_d[24] = 1;
    Box_5x5x5_is_on_bnd_d[27] = 1;
    Box_5x5x5_is_on_bnd_d[28] = 2;
    Box_5x5x5_is_on_bnd_d[29] = 1;
    Box_5x5x5_is_on_bnd_d[30] = 1;
    Box_5x5x5_is_on_bnd_d[31] = 2;
    Box_5x5x5_is_on_bnd_d[32] = 2;
    Box_5x5x5_is_on_bnd_d[33] = 1;
    Box_5x5x5_is_on_bnd_d[34] = 1;
    Box_5x5x5_is_on_bnd_d[35] = 2;
    Box_5x5x5_is_on_bnd_d[36] = 1;
    Box_5x5x5_is_on_bnd_d[39] = 1;
    Box_5x5x5_is_on_bnd_d[40] = 1;
    Box_5x5x5_is_on_bnd_d[43] = 1;
    Box_5x5x5_is_on_bnd_d[44] = 2;
    Box_5x5x5_is_on_bnd_d[45] = 1;
    Box_5x5x5_is_on_bnd_d[46] = 1;
    Box_5x5x5_is_on_bnd_d[47] = 2;
    Box_5x5x5_is_on_bnd_d[48] = 3;
    Box_5x5x5_is_on_bnd_d[49] = 2;
    Box_5x5x5_is_on_bnd_d[50] = 2;
    Box_5x5x5_is_on_bnd_d[51] = 3;
    Box_5x5x5_is_on_bnd_d[52] = 2;
    Box_5x5x5_is_on_bnd_d[53] = 1;
    Box_5x5x5_is_on_bnd_d[54] = 1;
    Box_5x5x5_is_on_bnd_d[55] = 2;
    Box_5x5x5_is_on_bnd_d[56] = 2;
    Box_5x5x5_is_on_bnd_d[57] = 1;
    Box_5x5x5_is_on_bnd_d[58] = 1;
    Box_5x5x5_is_on_bnd_d[59] = 2;
    Box_5x5x5_is_on_bnd_d[60] = 3;
    Box_5x5x5_is_on_bnd_d[61] = 2;
    Box_5x5x5_is_on_bnd_d[62] = 2;
    Box_5x5x5_is_on_bnd_d[63] = 3;

    int Box_5x5x5_number_of_cells = 64;
    long Box_5x5x5_is_on_corner[8] = {0, 3, 12, 15, 48, 51, 60, 63};
    unordered_set<long> s_Box_5x5x5_is_on_corner = unordered_set<long>({0, 3, 12, 15, 48, 51, 60, 63});

    long Box_5x5x5_is_on_ridge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19,
                                      28, 31, 32, 35, 44, 47, 49, 50, 52, 55,
                                      56, 59, 61, 62};

    unordered_set<long> s_Box_5x5x5_is_on_ridge = unordered_set<long>({1, 2, 4, 7, 8, 11, 13, 14, 16, 19,
                                                                       28, 31, 32, 35, 44, 47, 49, 50, 52, 55,
                                                                       56, 59, 61, 62});

    long Box_5x5x5_is_on_valley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27,
                                       29, 30, 33, 34, 36, 39, 40, 43, 45, 46,
                                       53, 54, 57, 58};
    unordered_set<long> s_Box_5x5x5_is_on_valley = unordered_set<long>({5, 6, 9, 10, 17, 18, 20, 23, 24, 27,
                                                                        29, 30, 33, 34, 36, 39, 40, 43, 45, 46,
                                                                        53, 54, 57, 58});

    Seeds_Pool sp = Seeds_Pool(Box_5x5x5_number_of_cells, Box_5x5x5_is_on_bnd_d);

    unordered_set<long> empty_set = unordered_set<long>({});
    ASSERT_EQ(empty_set, sp.is_on_corner);
    ASSERT_EQ(s_Box_5x5x5_is_on_ridge, sp.is_on_ridge);
    ASSERT_EQ(s_Box_5x5x5_is_on_valley, sp.is_on_valley);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[3] = deque<long>({15, 0, 60, 3, 63, 48, 12, 51});
    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);

    Seeds_Pool sp_2 = Seeds_Pool(Box_5x5x5_number_of_cells,
                                Box_5x5x5_is_on_bnd_d,
                                s_Box_5x5x5_is_on_corner,
                                s_Box_5x5x5_is_on_ridge,
                                s_Box_5x5x5_is_on_valley);


    ref_l_deque_of_seeds[3] = deque<long>({0, 3, 15, 48, 12, 51, 60, 63});


    ASSERT_EQ(empty_set, sp_2.is_on_corner);
    ASSERT_EQ(s_Box_5x5x5_is_on_ridge, sp_2.is_on_ridge);
    ASSERT_EQ(s_Box_5x5x5_is_on_valley, sp_2.is_on_valley);
    //[deque([]), deque([]), deque([]), deque([0, 3, 12, 15, 48, 51, 60, 63])],
    ASSERT_EQ(ref_l_deque_of_seeds, sp_2.l_deque_of_seeds);

}

TEST(SeedsPool_TestSuite, init_MGridGen) {

    int MGridGen_nb_cells = 15;
//    MGridGen_is_on_bnd = np.array([1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2], dtype=int)
    unordered_map<long, int> MGridGen_is_on_bnd_d;
    MGridGen_is_on_bnd_d[0] = 1;
    MGridGen_is_on_bnd_d[1 ] = 1;
    MGridGen_is_on_bnd_d[3 ] = 1;
    MGridGen_is_on_bnd_d[4 ] = 1;
    MGridGen_is_on_bnd_d[12] = 2;
    MGridGen_is_on_bnd_d[13] = 1;
    MGridGen_is_on_bnd_d[14] = 2;
    unordered_set<long> s_MGridGen_is_on_corner = unordered_set<long>({});
    unordered_set<long> s_MGridGen_is_on_ridge = unordered_set<long>({12, 14});
    unordered_set<long> s_MGridGen_is_on_valley = unordered_set<long>({0, 1, 3, 4, 13});

    unordered_set<long> empty_set = unordered_set<long>({});

    Seeds_Pool sp = Seeds_Pool(MGridGen_nb_cells, MGridGen_is_on_bnd_d);

    ASSERT_EQ(s_MGridGen_is_on_corner, sp.is_on_corner);
    ASSERT_EQ(s_MGridGen_is_on_ridge, sp.is_on_ridge);
    ASSERT_EQ(s_MGridGen_is_on_valley, sp.is_on_valley);

    ASSERT_EQ(MGridGen_is_on_bnd_d, sp.d_is_on_bnd);
    ASSERT_EQ(empty_set, sp.is_on_corner);
    ASSERT_EQ(s_MGridGen_is_on_ridge, sp.is_on_ridge);
    ASSERT_EQ(s_MGridGen_is_on_valley, sp.is_on_valley);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
//    ref_l_deque_of_seeds[3] = deque<long>({15, 0, 60, 3, 63, 48, 12, 51});
    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);

    bool a_is_fc_agglomerated[MGridGen_nb_cells];
    for (int i = 0; i<MGridGen_nb_cells; i++)
    {
        a_is_fc_agglomerated[i]=false;
    }

    ASSERT_EQ(14, sp.choose_new_seed(a_is_fc_agglomerated));
    ASSERT_EQ(12, sp.choose_new_seed(a_is_fc_agglomerated));
    ASSERT_EQ(0, sp.choose_new_seed(a_is_fc_agglomerated)) ;

    // New Seeds_Pool
    sp = Seeds_Pool(MGridGen_nb_cells, MGridGen_is_on_bnd_d,
                    empty_set, empty_set, empty_set, 2);
    ASSERT_EQ(empty_set, sp.is_on_corner);
    ASSERT_EQ(empty_set, sp.is_on_ridge);
    ASSERT_EQ(s_MGridGen_is_on_valley, sp.is_on_valley);

    ASSERT_EQ(MGridGen_is_on_bnd_d, sp.d_is_on_bnd);
    vector<deque<long>> ref_l_deque_of_seeds_2(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds_2[i] = deque<long>();
    }
    ref_l_deque_of_seeds_2[2] = deque<long>({14, 12});
    ASSERT_EQ(ref_l_deque_of_seeds_2, sp.l_deque_of_seeds);


    ASSERT_EQ(14, sp.choose_new_seed(a_is_fc_agglomerated));
    ASSERT_EQ(12, sp.choose_new_seed(a_is_fc_agglomerated));
    ASSERT_EQ(0, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(13, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(1, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(3, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(4, sp.choose_new_seed(a_is_fc_agglomerated));

    // New Seeds_Pool
    sp = Seeds_Pool(MGridGen_nb_cells, MGridGen_is_on_bnd_d,
                    empty_set, empty_set, empty_set, 1);
    ASSERT_EQ(empty_set, sp.is_on_corner);
    ASSERT_EQ(empty_set, sp.is_on_ridge);
    ASSERT_EQ(empty_set, sp.is_on_valley);

    ASSERT_EQ(MGridGen_is_on_bnd_d, sp.d_is_on_bnd);
    vector<deque<long>> ref_l_deque_of_seeds_3(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds_3[i] = deque<long>();
    }
    ref_l_deque_of_seeds_3[1] = deque<long>({ 0, 13, 1, 3, 4 });
    ref_l_deque_of_seeds_3[2] = deque<long>({14, 12});
    ASSERT_EQ(ref_l_deque_of_seeds_3, sp.l_deque_of_seeds);

    ASSERT_EQ(14, sp.choose_new_seed(a_is_fc_agglomerated));
    ASSERT_EQ(12, sp.choose_new_seed(a_is_fc_agglomerated));
    ASSERT_EQ(0, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(13, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(1, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(3, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(4, sp.choose_new_seed(a_is_fc_agglomerated));
    // Strange behaviour but as we don't mark fc as read, 0 is OK.
    ASSERT_EQ(0, sp.choose_new_seed(a_is_fc_agglomerated));

}

TEST(SeedsPool_TestSuite, choose_new_seed) {

    int Box_5x5x5_number_of_cells = 64;

    unordered_map<long, int> Box_5x5x5_is_on_bnd_d;
    Box_5x5x5_is_on_bnd_d[0] = 3;
    Box_5x5x5_is_on_bnd_d[1] = 2;
    Box_5x5x5_is_on_bnd_d[2] = 2;
    Box_5x5x5_is_on_bnd_d[3] = 3;
    Box_5x5x5_is_on_bnd_d[4] = 2;
    Box_5x5x5_is_on_bnd_d[5] = 1;
    Box_5x5x5_is_on_bnd_d[6] = 1;
    Box_5x5x5_is_on_bnd_d[7] = 2;
    Box_5x5x5_is_on_bnd_d[8] = 2;
    Box_5x5x5_is_on_bnd_d[9] = 1;
    Box_5x5x5_is_on_bnd_d[10] = 1;
    Box_5x5x5_is_on_bnd_d[11] = 2;
    Box_5x5x5_is_on_bnd_d[12] = 3;
    Box_5x5x5_is_on_bnd_d[13] = 2;
    Box_5x5x5_is_on_bnd_d[14] = 2;
    Box_5x5x5_is_on_bnd_d[15] = 3;
    Box_5x5x5_is_on_bnd_d[16] = 2;
    Box_5x5x5_is_on_bnd_d[17] = 1;
    Box_5x5x5_is_on_bnd_d[18] = 1;
    Box_5x5x5_is_on_bnd_d[19] = 2;
    Box_5x5x5_is_on_bnd_d[20] = 1;
    Box_5x5x5_is_on_bnd_d[23] = 1;
    Box_5x5x5_is_on_bnd_d[24] = 1;
    Box_5x5x5_is_on_bnd_d[27] = 1;
    Box_5x5x5_is_on_bnd_d[28] = 2;
    Box_5x5x5_is_on_bnd_d[29] = 1;
    Box_5x5x5_is_on_bnd_d[30] = 1;
    Box_5x5x5_is_on_bnd_d[31] = 2;
    Box_5x5x5_is_on_bnd_d[32] = 2;
    Box_5x5x5_is_on_bnd_d[33] = 1;
    Box_5x5x5_is_on_bnd_d[34] = 1;
    Box_5x5x5_is_on_bnd_d[35] = 2;
    Box_5x5x5_is_on_bnd_d[36] = 1;
    Box_5x5x5_is_on_bnd_d[39] = 1;
    Box_5x5x5_is_on_bnd_d[40] = 1;
    Box_5x5x5_is_on_bnd_d[43] = 1;
    Box_5x5x5_is_on_bnd_d[44] = 2;
    Box_5x5x5_is_on_bnd_d[45] = 1;
    Box_5x5x5_is_on_bnd_d[46] = 1;
    Box_5x5x5_is_on_bnd_d[47] = 2;
    Box_5x5x5_is_on_bnd_d[48] = 3;
    Box_5x5x5_is_on_bnd_d[49] = 2;
    Box_5x5x5_is_on_bnd_d[50] = 2;
    Box_5x5x5_is_on_bnd_d[51] = 3;
    Box_5x5x5_is_on_bnd_d[52] = 2;
    Box_5x5x5_is_on_bnd_d[53] = 1;
    Box_5x5x5_is_on_bnd_d[54] = 1;
    Box_5x5x5_is_on_bnd_d[55] = 2;
    Box_5x5x5_is_on_bnd_d[56] = 2;
    Box_5x5x5_is_on_bnd_d[57] = 1;
    Box_5x5x5_is_on_bnd_d[58] = 1;
    Box_5x5x5_is_on_bnd_d[59] = 2;
    Box_5x5x5_is_on_bnd_d[60] = 3;
    Box_5x5x5_is_on_bnd_d[61] = 2;
    Box_5x5x5_is_on_bnd_d[62] = 2;
    Box_5x5x5_is_on_bnd_d[63] = 3;

    bool a_is_fc_agglomerated[Box_5x5x5_number_of_cells];
    for (int i = 0; i<Box_5x5x5_number_of_cells; i++)
    {
        a_is_fc_agglomerated[i]=false;
    }

    Seeds_Pool sp = Seeds_Pool(Box_5x5x5_number_of_cells, Box_5x5x5_is_on_bnd_d);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[3] = deque<long>({ 15, 0, 60, 3, 63, 48, 12, 51 });
    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);

    ASSERT_EQ(15, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(0, sp.choose_new_seed(a_is_fc_agglomerated))  ;
    ASSERT_EQ(60, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(3, sp.choose_new_seed(a_is_fc_agglomerated))  ;
    ASSERT_EQ(63, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(48, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(12, sp.choose_new_seed(a_is_fc_agglomerated)) ;
    ASSERT_EQ(51, sp.choose_new_seed(a_is_fc_agglomerated)) ;

    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }

    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);

    sp = Seeds_Pool(Box_5x5x5_number_of_cells, Box_5x5x5_is_on_bnd_d);

    a_is_fc_agglomerated[0]  = true;
    a_is_fc_agglomerated[3]  = true;
    a_is_fc_agglomerated[12] = true;
    a_is_fc_agglomerated[15] = true;
    a_is_fc_agglomerated[48] = true;
    a_is_fc_agglomerated[51] = true;
    ASSERT_EQ(60, sp.choose_new_seed(a_is_fc_agglomerated));
    a_is_fc_agglomerated[63] = true;
    ASSERT_EQ(28, sp.choose_new_seed(a_is_fc_agglomerated));

    sp = Seeds_Pool(Box_5x5x5_number_of_cells, Box_5x5x5_is_on_bnd_d);
//    sp = SPdq.Seeds_Pool_deque(self.Box_5x5x5_number_of_cells, self.Box_5x5x5_is_on_bnd_d)
    for(auto i_kv:Box_5x5x5_is_on_bnd_d){
        if (i_kv.second > 1)
        {
            a_is_fc_agglomerated[i_kv.first] = true;
        }
    }
    a_is_fc_agglomerated[5] = true;
    ASSERT_EQ(6, sp.choose_new_seed(a_is_fc_agglomerated));

    sp = Seeds_Pool(Box_5x5x5_number_of_cells, Box_5x5x5_is_on_bnd_d);
    for(auto i_kv:Box_5x5x5_is_on_bnd_d){
        a_is_fc_agglomerated[i_kv.first] = true;
    }
    ASSERT_EQ(21, sp.choose_new_seed(a_is_fc_agglomerated));
}


TEST(SeedsPool_TestSuite, boundary_value) {
    int MGridGen_nb_cells = 15;
//    MGridGen_is_on_bnd = np.array([1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2], dtype=int)
    unordered_map<long, int> MGridGen_is_on_bnd_d;
    MGridGen_is_on_bnd_d[0] = 1;
    MGridGen_is_on_bnd_d[1] = 1;
    MGridGen_is_on_bnd_d[3] = 1;
    MGridGen_is_on_bnd_d[4] = 1;
    MGridGen_is_on_bnd_d[12] = 2;
    MGridGen_is_on_bnd_d[13] = 1;
    MGridGen_is_on_bnd_d[14] = 2;

    Seeds_Pool sp = Seeds_Pool(MGridGen_nb_cells, MGridGen_is_on_bnd_d);

    ASSERT_EQ(1, sp.boundary_value(0));
    ASSERT_EQ(1, sp.boundary_value(1));
    ASSERT_EQ(0, sp.boundary_value(2));
    ASSERT_EQ(1, sp.boundary_value(3));
    ASSERT_EQ(1, sp.boundary_value(4));
    ASSERT_EQ(0, sp.boundary_value(5));
    ASSERT_EQ(0, sp.boundary_value(6));
    ASSERT_EQ(0, sp.boundary_value(7));
    ASSERT_EQ(0, sp.boundary_value(8));
    ASSERT_EQ(0, sp.boundary_value(9));
    ASSERT_EQ(0, sp.boundary_value(10));
    ASSERT_EQ(0, sp.boundary_value(11));
    ASSERT_EQ(2, sp.boundary_value(12));
    ASSERT_EQ(1, sp.boundary_value(13));
    ASSERT_EQ(2, sp.boundary_value(14));

    unordered_map<long, int> MGridGen_is_on_bnd_d_crooked;
    MGridGen_is_on_bnd_d_crooked[0] = 1;
    MGridGen_is_on_bnd_d_crooked[1] = 1;
    MGridGen_is_on_bnd_d_crooked[2] = 4;  // wrong value, for test only
    MGridGen_is_on_bnd_d_crooked[3] = 1;
    MGridGen_is_on_bnd_d_crooked[4] = 1;
    MGridGen_is_on_bnd_d_crooked[12] = 2;
    MGridGen_is_on_bnd_d_crooked[13] = 1;
    MGridGen_is_on_bnd_d_crooked[14] = 2;


    sp = Seeds_Pool(MGridGen_nb_cells, MGridGen_is_on_bnd_d_crooked);

    ASSERT_EQ(1, sp.boundary_value(0));
    ASSERT_EQ(1, sp.boundary_value(1));
    ASSERT_EQ(3, sp.boundary_value(2));
    ASSERT_EQ(1, sp.boundary_value(3));
    ASSERT_EQ(1, sp.boundary_value(4));
    ASSERT_EQ(0, sp.boundary_value(5));
    ASSERT_EQ(0, sp.boundary_value(6));
    ASSERT_EQ(0, sp.boundary_value(7));
    ASSERT_EQ(0, sp.boundary_value(8));
    ASSERT_EQ(0, sp.boundary_value(9));
    ASSERT_EQ(0, sp.boundary_value(10));
    ASSERT_EQ(0, sp.boundary_value(11));
    ASSERT_EQ(2, sp.boundary_value(12));
    ASSERT_EQ(1, sp.boundary_value(13));
    ASSERT_EQ(2, sp.boundary_value(14));

}

TEST(SeedsPool_TestSuite, update) {
    int MGridGen_nb_cells = 15;
//    MGridGen_is_on_bnd = np.array([1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2], dtype=int)
    unordered_map<long, int> MGridGen_is_on_bnd_d;
    MGridGen_is_on_bnd_d[0] = 1;
    MGridGen_is_on_bnd_d[1] = 1;
    MGridGen_is_on_bnd_d[3] = 1;
    MGridGen_is_on_bnd_d[4] = 1;
    MGridGen_is_on_bnd_d[12] = 2;
    MGridGen_is_on_bnd_d[13] = 1;
    MGridGen_is_on_bnd_d[14] = 2;

    Seeds_Pool sp = Seeds_Pool(MGridGen_nb_cells, MGridGen_is_on_bnd_d);

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
//    ref_l_deque_of_seeds[3] = deque<long>({ 15, 0, 60, 3, 63, 48, 12, 51 });
    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);
    std::list<long> l_first;
    l_first.push_back(12);
    l_first.push_back(0);
    sp.update(l_first);

    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[1] = deque<long>({0 });
    ref_l_deque_of_seeds[2] = deque<long>({12 });
    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);

    std::list<long> l_2;
    l_2.push_back(14);
    l_2.push_back(2);
    sp.update(l_2);

    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[0] = deque<long>({2 });
    ref_l_deque_of_seeds[1] = deque<long>({0 });
    ref_l_deque_of_seeds[2] = deque<long>({12, 14 });
    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);

    unordered_map<long, int> MGridGen_is_on_bnd_d_crooked;
    MGridGen_is_on_bnd_d_crooked[0] = 1;
    MGridGen_is_on_bnd_d_crooked[1] = 1;
    MGridGen_is_on_bnd_d_crooked[2] = 4;  // wrong value, for test only
    MGridGen_is_on_bnd_d_crooked[3] = 1;
    MGridGen_is_on_bnd_d_crooked[4] = 1;
    MGridGen_is_on_bnd_d_crooked[12] = 2;
    MGridGen_is_on_bnd_d_crooked[13] = 1;
    MGridGen_is_on_bnd_d_crooked[14] = 2;

    sp = Seeds_Pool(MGridGen_nb_cells, MGridGen_is_on_bnd_d_crooked);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[3] = deque<long>({2 });
    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);
    sp.d_is_on_bnd[2] = 4;

    std::list<long> l_3;
    l_3.push_back(14);
    l_3.push_back(2);
    sp.update(l_3);

    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({14 });
    ref_l_deque_of_seeds[3] = deque<long>({2, 2 });
    ASSERT_EQ(ref_l_deque_of_seeds, sp.l_deque_of_seeds);
}