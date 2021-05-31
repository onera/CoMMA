#include "../CoMMA_lib/Edge.h"
#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Util.h"

#include "gtest/gtest.h"

TEST(Utils_TestSuite, PartsList_tmp) {

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq);
//    for (auto l:listOfSet) {
//        for (auto i: l) {
//            cout << i << ", ";
//        }
//        cout << endl;
//    }

    EXPECT_EQ(7, listOfSet.size());
    //ASSERT_EQ(
}

TEST(Edge_TestSuite, Constructor) {

    Edge e_1 = Edge(0, 1);
    Edge e_2 = Edge(1, 0);

    ASSERT_TRUE(e_1 == e_2);
    ASSERT_TRUE(e_1 == e_2);
    ASSERT_FALSE(e_1 != e_2);

    e_2.reverse();
    ASSERT_TRUE(e_1 == e_2);

//    ASSERT_EQ(0, agg.__verbose);
//    ASSERT_EQ(2, agg.__dimension);
//    ASSERT_TRUE(agg.__checks);
//    ASSERT_FALSE(agg.__is_anisotropic);
}

