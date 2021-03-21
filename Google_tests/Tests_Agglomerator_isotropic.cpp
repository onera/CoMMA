#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "gtest/gtest.h"


TEST(Agglomerator_Isostropic_TestSuite, computation_Of_Neighbourhood) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};

    //unordered_set<long> *setOfFineCells =new unordered_set<long> ({});
    int size_isFineCellAgglomerated_tmp = 15;

    bool *isFineCellAgglomerated_tmp = new bool[size_isFineCellAgglomerated_tmp];
    for (int i = 0; i < size_isFineCellAgglomerated_tmp; i++) {
        isFineCellAgglomerated_tmp[i] = false;
    }

    int maxCard = 8;
    int numberOfOrderOfNeighbourhood = 3;
    long seed = 0;

    unordered_map<long, int> dict_Neighbours_Of_Seed =
            computation_Of_Neighbourhood(seed,
                                         numberOfOrderOfNeighbourhood,
                                         row_ptr, col_ind,
                                         maxCard,
                                         isFineCellAgglomerated_tmp);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 1;
    ref[3] = 2;
    ref[4] = 3;
    ref[5] = 2;
    ref[6] = 2;
    ref[8] = 3;
    ref[11] = 3;


    for (auto i:dict_Neighbours_Of_Seed) {
//        cout << i.first << " : " << i.second << endl;
        EXPECT_EQ(ref[i.first], i.second);
    }
//    self.assertEqual(4, maxOrder);

    delete[] isFineCellAgglomerated_tmp;
}

TEST(Agglomerator_Isostropic_TestSuite, computation_Of_Neighbourhood_FineCellsConstraints) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};

    unordered_set<long> *setOfFineCells = new unordered_set<long>({9, 4, 7, 14, 2, 0, 12, 13, 5, 6, 1, 11, 3, 8, 10});
    int size_isFineCellAgglomerated_tmp = 15;

    bool *isFineCellAgglomerated_tmp = new bool[size_isFineCellAgglomerated_tmp];
    for (int i = 0; i < size_isFineCellAgglomerated_tmp; i++) {
        isFineCellAgglomerated_tmp[i] = false;
    }

    int maxCard = 8;
    int numberOfOrderOfNeighbourhood = 3;
    long seed = 0;

    unordered_map<long, int> dict_Neighbours_Of_Seed =
            computation_Of_Neighbourhood(seed,
                                         numberOfOrderOfNeighbourhood,
                                         row_ptr, col_ind,
                                         maxCard,
                                         isFineCellAgglomerated_tmp,
                                         setOfFineCells);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 1;
    ref[3] = 2;
    ref[4] = 3;
    ref[5] = 2;
    ref[6] = 2;
    ref[8] = 3;
    ref[11] = 3;


    for (auto i:dict_Neighbours_Of_Seed) {
//        cout << i.first << " : " << i.second << endl;
//        assert(ref[i.first] == i.second);
        EXPECT_EQ(ref[i.first], i.second);
    }
//    self.assertEqual(4, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
}

TEST(Agglomerator_Isostropic_TestSuite, computation_Of_Neighbourhood_FineCellsConstraints_11) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};

    unordered_set<long> *setOfFineCells = new unordered_set<long>({9, 4, 7, 14, 2, 0, 12, 13, 5, 6, 1, 3, 8, 10});
    int size_isFineCellAgglomerated_tmp = 15;

    bool *isFineCellAgglomerated_tmp = new bool[size_isFineCellAgglomerated_tmp];
    for (int i = 0; i < size_isFineCellAgglomerated_tmp; i++) {
        isFineCellAgglomerated_tmp[i] = false;
    }

    int maxCard = 8;
    int numberOfOrderOfNeighbourhood = 3;
    long seed = 0;

    unordered_map<long, int> dict_Neighbours_Of_Seed =
            computation_Of_Neighbourhood(seed,
                                         numberOfOrderOfNeighbourhood,
                                         row_ptr, col_ind,
                                         maxCard,
                                         isFineCellAgglomerated_tmp,
                                         setOfFineCells);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 1;
    ref[3] = 2;
    ref[4] = 3;
    ref[5] = 2;
    ref[6] = 2;
    ref[8] = 3;


    for (auto i:dict_Neighbours_Of_Seed) {
//        cout << i.first << " : " << i.second << endl;
//        assert(ref[i.first] == i.second);
        EXPECT_EQ(ref[i.first], i.second);
    }
//    self.assertEqual(4, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
}

TEST(Agglomerator_Isostropic_TestSuite, computation_Of_Neighbourhood_FineCellsConstraints_118) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};

    unordered_set<long> *setOfFineCells = new unordered_set<long>({9, 4, 7, 2, 0, 12, 13, 5, 6, 1, 3, 10});
    int size_isFineCellAgglomerated_tmp = 15;

    bool *isFineCellAgglomerated_tmp = new bool[size_isFineCellAgglomerated_tmp];
    for (int i = 0; i < size_isFineCellAgglomerated_tmp; i++) {
        isFineCellAgglomerated_tmp[i] = false;
    }

    int maxCard = 8;
    int numberOfOrderOfNeighbourhood = 3;
    long seed = 0;

    unordered_map<long, int> dict_Neighbours_Of_Seed =
            computation_Of_Neighbourhood(seed,
                                         numberOfOrderOfNeighbourhood,
                                         row_ptr, col_ind,
                                         maxCard,
                                         isFineCellAgglomerated_tmp,
                                         setOfFineCells);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 1;
    ref[3] = 2;
    ref[4] = 3;
    ref[5] = 2;
    ref[6] = 2;
    ref[7] = 4;


    for (auto i:dict_Neighbours_Of_Seed) {
//        cout << i.first << " : " << i.second << endl;
//        assert(ref[i.first] == i.second);
        EXPECT_EQ(ref[i.first], i.second);
    }
//    self.assertEqual(5, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
}

TEST(Agglomerator_Isostropic_TestSuite, computation_Of_Neighbourhood_FineCellsConstraints_118_maxCard) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};

    unordered_set<long> *setOfFineCells = new unordered_set<long>({9, 4, 7, 2, 0, 12, 13, 5, 6, 1, 3, 10});
    int size_isFineCellAgglomerated_tmp = 15;

    bool *isFineCellAgglomerated_tmp = new bool[size_isFineCellAgglomerated_tmp];
    for (int i = 0; i < size_isFineCellAgglomerated_tmp; i++) {
        isFineCellAgglomerated_tmp[i] = false;
    }

    int maxCard = 10;
    int numberOfOrderOfNeighbourhood = 3;
    long seed = 0;

    unordered_map<long, int> dict_Neighbours_Of_Seed =
            computation_Of_Neighbourhood(seed,
                                         numberOfOrderOfNeighbourhood,
                                         row_ptr, col_ind,
                                         maxCard,
                                         isFineCellAgglomerated_tmp,
                                         setOfFineCells);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 1;
    ref[3] = 2;
    ref[4] = 3;
    ref[5] = 2;
    ref[6] = 2;
    ref[7] = 4;
    ref[9] = 5;
    ref[10] = 6;
    ref[12] = 6;


    for (auto i:dict_Neighbours_Of_Seed) {
//        cout << i.first << " : " << i.second << endl;
//        assert(ref[i.first] == i.second);
        EXPECT_EQ(ref[i.first], i.second);
    }
//    self.assertEqual(5, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
}


TEST(Agglomerator_Isostropic_TestSuite, computation_Of_Neighbourhood_FineCellsConstraints_1184) {
    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};

    unordered_set<long> *setOfFineCells = new unordered_set<long>({9, 7, 2, 0, 12, 13, 5, 6, 1, 3, 10});
    int size_isFineCellAgglomerated_tmp = 15;

    bool *isFineCellAgglomerated_tmp = new bool[size_isFineCellAgglomerated_tmp];
    for (int i = 0; i < size_isFineCellAgglomerated_tmp; i++) {
        isFineCellAgglomerated_tmp[i] = false;
    }

    int maxCard = 8;
    int numberOfOrderOfNeighbourhood = 3;
    long seed = 0;

    unordered_map<long, int> dict_Neighbours_Of_Seed =
            computation_Of_Neighbourhood(seed,
                                         numberOfOrderOfNeighbourhood,
                                         row_ptr, col_ind,
                                         maxCard,
                                         isFineCellAgglomerated_tmp,
                                         setOfFineCells);
    unordered_map<long, int> ref;
    ref[1] = 1;
    ref[2] = 1;
    ref[3] = 2;
    ref[4] = 3;
    ref[5] = 2;
    ref[6] = 2;


    for (auto i:dict_Neighbours_Of_Seed) {
//        cout << i.first << " : " << i.second << endl;
//        assert(ref[i.first] == i.second);
        EXPECT_EQ(ref[i.first], i.second);
    }
//    self.assertEqual(5, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
}

TEST(Agglomerator_Isostropic_TestSuite, computation_Of_Neighbourhood_FineCellsConstraints_13) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};

    unordered_set<long> *setOfFineCells = new unordered_set<long>({9, 4, 7, 14, 2, 0, 12, 13, 5, 6, 11, 8, 10});
    int size_isFineCellAgglomerated_tmp = 15;

    bool *isFineCellAgglomerated_tmp = new bool[size_isFineCellAgglomerated_tmp];
    for (int i = 0; i < size_isFineCellAgglomerated_tmp; i++) {
        isFineCellAgglomerated_tmp[i] = false;
    }

    int maxCard = 8;
    int numberOfOrderOfNeighbourhood = 3;
    long seed = 0;

    unordered_map<long, int> dict_Neighbours_Of_Seed =
            computation_Of_Neighbourhood(seed,
                                         numberOfOrderOfNeighbourhood,
                                         row_ptr, col_ind,
                                         maxCard,
                                         isFineCellAgglomerated_tmp,
                                         setOfFineCells);
    unordered_map<long, int> ref;
    ref[2] = 1;
    ref[4] = 3;
    ref[5] = 2;
    ref[6] = 2;
    ref[7] = 4;
    ref[8] = 3;
    ref[10] = 4;
    ref[11] = 3;
    ref[13] = 4;
    ref[14] = 4;


    for (auto i:dict_Neighbours_Of_Seed) {
//        cout << i.first << " : " << i.second << endl;
//        assert(ref[i.first] == i.second);
        EXPECT_EQ(ref[i.first], i.second);

    }
//    self.assertEqual(5, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
}
