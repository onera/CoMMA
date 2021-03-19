#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "gtest/gtest.h"

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Ridge_1){

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

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Ridge_1_V2){

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

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Valley_4){

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

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Valley_10){

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

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_Valley_13){

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
TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_0){

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

TEST(SeedsPool_TestSuite, Choice_Of_Seed_Case_Seed_2){

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
    ref_l_of_seeds[3] = queue<long>({ 6, 8, 18, 20, 24, 26});
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

