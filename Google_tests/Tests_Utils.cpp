#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "gtest/gtest.h"


TEST(Utils_TestSuite, PartsList){

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq);
//    for (auto l:listOfSet) {
//        for (auto i: l) {
//            cout << i << ", ";
//        }
//        cout << endl;
//    }

    EXPECT_EQ(7,listOfSet.size());
    //ASSERT_EQ(
}

TEST(Utils_TestSuite, PartsList_length_1){

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq, 1);

//    for (auto l:listOfSet) {
//        for (auto i: l) {
//            cout << i << ", ";
//        }
//        cout << endl;
//    }
    EXPECT_EQ(3,listOfSet.size());
    //ASSERT_EQ(
}

TEST(Utils_TestSuite, PartsList_length_2){

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq, 2);
    EXPECT_EQ(3,listOfSet.size());
    //ASSERT_EQ(
}

TEST(Utils_TestSuite, PartsList_length_3){

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq, 3);
    EXPECT_EQ(1,listOfSet.size());

}

//queue<unordered_set<long>> partsList(vector<long> seq, int length)

TEST(Utils_TestSuite, CheckConnectivity_MGridGenDoc_example){
    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};
    unordered_set<long> datas({0, 1, 3});
    ASSERT_TRUE(checkConnectivity_w_set(datas, row_ptr, col_ind));

    datas = {3, 1, 0};
    ASSERT_TRUE(checkConnectivity_w_set(datas, row_ptr, col_ind));

    datas = {1, 3, 0};
    ASSERT_TRUE(checkConnectivity_w_set(datas, row_ptr, col_ind));

    unordered_set<long> datas2({0, 2});
    ASSERT_TRUE(checkConnectivity_w_set(datas2, row_ptr, col_ind));

    datas2 = {2, 0};
    ASSERT_TRUE(checkConnectivity_w_set(datas2, row_ptr, col_ind));

    datas2 = {0, 6};
    ASSERT_FALSE(checkConnectivity_w_set(datas2, row_ptr, col_ind));

    datas2 = {6, 0};
    ASSERT_FALSE(checkConnectivity_w_set(datas2, row_ptr, col_ind));

    unordered_set<long> datas3({0, 1, 2, 3, 4, 5, 6, 7, 14});
    ASSERT_FALSE(checkConnectivity_w_set(datas3, row_ptr, col_ind));

    datas3 = {0, 1, 2, 3, 4, 5, 6, 7, 11};
    ASSERT_TRUE(checkConnectivity_w_set(datas3, row_ptr, col_ind));

    unordered_set<long> datas4({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    ASSERT_TRUE(checkConnectivity_w_set(datas4, row_ptr, col_ind));

    unordered_set<long> datas5({14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
    ASSERT_TRUE(checkConnectivity_w_set(datas5, row_ptr, col_ind));
}


TEST(Utils_TestSuite, ComputeNumberOfCommonFaces){

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};

    long listFineCells_1[12] = {9, 4, 7, 14, 2, 0, 12, 13, 5, 11, 8, 10};
    long listFineCells_0[3] = {1, 3, 6};

    long *fine_Cell_indices_To_C_C_Indices = new long[15];
    for (int i = 0; i < 12; i++) {
        fine_Cell_indices_To_C_C_Indices[listFineCells_1[i]] = 1;
    }
    for (int i = 0; i < 3; i++) {
        fine_Cell_indices_To_C_C_Indices[listFineCells_0[i]] = 0;
    }

    long iCC = 1;

    ASSERT_EQ(1,computeNumberOfCommonFaces(0, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(1,computeNumberOfCommonFaces(1, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(2,computeNumberOfCommonFaces(2, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(0,computeNumberOfCommonFaces(3, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(2,computeNumberOfCommonFaces(4, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(3,computeNumberOfCommonFaces(5, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(2,computeNumberOfCommonFaces(6, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(2,computeNumberOfCommonFaces(11, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));

}