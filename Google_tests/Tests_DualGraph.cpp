#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "gtest/gtest.h"

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_205) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    double values[51] = {6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
                         6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
                         2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
                         3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
                         3.16227766, 7.};
    long sizes[3] = {16, 51, 0};
    vector<long> v_fc = {2, 0, 5};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, v_fc,
                                                                                        row_ptr, col_ind);
    long new_seed = 0;
    unordered_map<long, queue<long> *> dict_SpanningTree;
    dict_SpanningTree[0] = new queue<long>({2});
    dict_SpanningTree[2] = new queue<long>({5});

    unordered_set<long> s_fc({2, 0, 5});
    removeSeparatingVertex(new_seed, dict_SpanningTree, s_fc, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : s_fc) {
//        cout<<i<<", ";
//    }
//    cout << endl;
//    cout<<"Size= "<<s_fc.size()<<endl;
    unordered_set<long> ref_s_fc({2, 0, 5});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_136111314) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
//    double values[51] = {6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
//                         6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
//                         2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
//                         3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
//                         3.16227766, 7.};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {1, 3, 6, 11, 13, 14};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    long new_seed = 1;

    unordered_set<long> s_fc({1, 3, 6, 11, 13, 14});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({1, 3, 6, 13, 14});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_10265813119) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
//    double values[51] = {6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
//                         6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
//                         2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
//                         3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
//                         3.16227766, 7.};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {10, 2, 6, 5, 8, 13, 11, 9};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = 1;

    unordered_set<long> s_fc({10, 2, 6, 5, 8, 13, 11, 9});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({2, 5, 6, 8, 9, 10, 11, 13});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_All) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};

    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    //    dict_ConnectivityTree = {0: [1], 2: [0, 6], 3: [1], 4: [5], 5: [2], 6: [2, 3], 7: [4, 8], 8: [5], 9: [7, 10], 10: [8, 13], 11: [6, 14], 12: [9], 13: [11]}



    long new_seed = 12;

    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13, 14});
    ASSERT_EQ(ref_s_fc, s_fc);
}


TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_2) {

    long row_ptr[8] = {0, 4, 7, 13, 16, 21, 26, 29};
    long col_ind[29] = {0, 1, 2, 5, 0, 1, 2, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6};

    long sizes[3] = {8, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    long new_seed = 1;
    unordered_map<long, queue<long> *> dict_SpanningTree;
    dict_SpanningTree[0] = new queue<long>({5});
    dict_SpanningTree[1] = new queue<long>({0, 2});
    dict_SpanningTree[2] = new queue<long>({3, 4});
    dict_SpanningTree[5] = new queue<long>({6});

    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6});
    removeSeparatingVertex(new_seed, dict_SpanningTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({0, 1, 2, 3, 5, 6});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_3_seed_0) {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};


    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = 7;
    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);
    unordered_set<long> ref_s_fc({0, 1, 2, 5, 6, 7, 8});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_4_random) {

    long row_ptr[9] = {0, 4, 8, 14, 17, 22, 27, 30, 31};
    long col_ind[31] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1};

    long sizes[3] = {9, 31, 0};
    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = 7;
//    long new_seed = sizes[2];
//    cout<<"new_seed "<<new_seed<<endl;
//    unordered_map<long, queue<long> *> dict_SpanningTree;
//    dict_SpanningTree[0] = new queue<long>({5});
//    dict_SpanningTree[1] = new queue<long>({0,2});
//    dict_SpanningTree[2] = new queue<long>({3,4});
//    dict_SpanningTree[5] = new queue<long>({6});
    //= {0:[2], 2:[5]}
    unordered_set<long> s_fc({6, 7, 1, 3, 5, 4, 0, 2});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({0, 1, 2, 3, 5, 6, 7});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_5_random) {

    long row_ptr[10] = {0, 4, 8, 15, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 8, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        2};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = 7;

    unordered_set<long> s_fc({6, 7, 1, 3, 5, 4, 0, 2, 8});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);
    unordered_set<long> ref_s_fc({0, 1, 3, 5, 6, 7, 8});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_6) {

    long row_ptr[10] = {0, 4, 7, 11, 15, 19, 22, 26, 32, 37};
    long col_ind[37] = {0, 1, 5, 8, 0, 1, 2, 1, 2, 6, 7, 3, 4, 6, 7, 3, 4, 7, 8, 0, 5, 8, 2, 3, 6, 7, 2, 3, 4, 6, 7, 8,
                        0, 4, 5, 7, 8};

    long sizes[3] = {10, 37, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = sizes[2];
    ASSERT_EQ(4, new_seed);

    unordered_set<long> s_fc({6, 7, 1, 3, 5, 4, 0, 2, 8});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({1, 3, 4, 5, 6, 7, 8});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_7_seed_0) {

    long row_ptr[12] = {0, 3, 8, 12, 15, 19, 22, 27, 31, 34, 37, 40};
    long col_ind[40] = {0, 2, 7, 1, 3, 4, 8, 9, 0, 2, 5, 6, 1, 3, 9, 1, 4, 6, 9, 2, 5, 10, 2, 4, 6,
                        7, 10, 0, 6, 7, 8, 1, 7, 8, 3, 4, 9, 5, 6, 10};

    long sizes[3] = {12, 40, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = sizes[2];
    ASSERT_EQ(5, new_seed);

    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({0, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_7_seed_1) {
    long row_ptr[12] = {0, 3, 8, 12, 15, 19, 22, 27, 31, 34, 37, 40};
    long col_ind[40] = {0, 2, 7, 1, 3, 4, 8, 9, 0, 2, 5, 6, 1, 3, 9, 1, 4, 6, 9, 2, 5, 10, 2, 4, 6,
                        7, 10, 0, 6, 7, 8, 1, 7, 8, 3, 4, 9, 5, 6, 10};

    long sizes[3] = {12, 40, 0};
    vector<long> listOfFineCells = {1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = sizes[2];
    ASSERT_EQ(3, new_seed);

    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({0, 1, 3, 4, 5, 6, 7, 8, 9, 10});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_7_seed_9) {

    long row_ptr[12] = {0, 3, 8, 12, 15, 19, 22, 27, 31, 34, 37, 40};
    long col_ind[40] = {0, 2, 7, 1, 3, 4, 8, 9, 0, 2, 5, 6, 1, 3, 9, 1, 4, 6, 9, 2, 5, 10, 2, 4, 6,
                        7, 10, 0, 6, 7, 8, 1, 7, 8, 3, 4, 9, 5, 6, 10};

    long sizes[3] = {12, 40, 0};
    vector<long> listOfFineCells = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    long new_seed = sizes[2];
    ASSERT_EQ(5, new_seed);

    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({0, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    ASSERT_EQ(ref_s_fc, s_fc);

}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_8_random) {
    long row_ptr[12] = {0, 5, 8, 11, 15, 20, 23, 26, 28, 32, 36, 39};
    long col_ind[39] = {0, 1, 2, 3, 4, 0, 1, 9, 0, 2, 6, 0, 3, 6, 8, 0, 4, 5, 8, 9, 4, 5, 7, 2, 3,
                        6, 5, 7, 3, 4, 8, 10, 1, 4, 9, 10, 8, 9, 10};

    long sizes[3] = {12, 39, 0};
    vector<long> listOfFineCells = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = sizes[2];
    ASSERT_EQ(7, new_seed);

    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({1, 2, 3, 4, 5, 6, 7, 10});
    ASSERT_EQ(ref_s_fc, s_fc);


}

TEST(DualGraph_TestSuite, RemoveSeparatingVertex_Case_9_random) {

    long row_ptr[12] = {0, 2, 4, 8, 11, 14, 19, 23, 27, 30, 32, 35};
    long col_ind[35] = {0, 7, 1, 5, 2, 3, 5, 10, 2, 3, 8, 4, 6, 10, 1, 2, 5, 7, 8, 4, 6, 7, 9, 0, 5,
                        6, 7, 3, 5, 8, 6, 9, 2, 4, 10};

    long sizes[3] = {12, 35, 0};
    vector<long> listOfFineCells = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = sizes[2];
    ASSERT_EQ(9, new_seed);
    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, s_fc, row_ptr, col_ind);

    unordered_set<long> ref_s_fc({0, 1,2,  3, 4, 6,  8, 9, 10});
    ASSERT_EQ(ref_s_fc, s_fc);


}
