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
