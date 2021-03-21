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



TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_025) {

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
    vector<long> listOfFineCells = {0, 2, 5};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    ASSERT_EQ(5, sizes[2]);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({2});
    ref_max_dict[2] = new queue<long>({0, 5});
    ref_max_dict[5] = new queue<long>({2});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout<<i.first<<": [";
//        while(! (*i.second).empty())
//        {
//            cout <<(*i.second).front()<< ", ";
//            (*i.second).pop();
//        }
//        cout<<"]"<<endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
//        cout<<i.first<<": [";
//        while(! (*i.second).empty())
//        {
//            cout <<(*i.second).front()<< ", ";
//            (*i.second).pop();
//        }
//        cout<<"]"<<endl;
    }
}


TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_10265813119) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {10, 2, 6, 5, 8, 13, 11, 9};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    EXPECT_EQ(9, sizes[2]);
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[10] = new queue<long>({8,9,13});
    ref_max_dict[8] = new queue<long>({5, 10});
    ref_max_dict[9] = new queue<long>({10});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[5] = new queue<long>({2, 8});
    ref_max_dict[11] = new queue<long>({6,13});
    ref_max_dict[2] = new queue<long>({5,6});
    ref_max_dict[6] = new queue<long>({2, 11});


    for (auto i : dict_ConnectivityTree) {

        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }

}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_136111314) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {1, 3, 6, 11, 13, 14};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(13, sizes[2]);//14 est aussi correct
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[1] = new queue<long>({3});
    ref_max_dict[14] = new queue<long>({11});
    ref_max_dict[3] = new queue<long>({1, 6});
    ref_max_dict[6] = new queue<long>({3, 11});
    ref_max_dict[11] = new queue<long>({6, 13, 14});
    ref_max_dict[13] = new queue<long>({11});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//
//        }
//        cout << "]" << endl;
    }
//    cout << sizes[2] << endl;


}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_520810131411) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {5, 2, 0, 8, 10, 13, 14, 11};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[5] = new queue<long>({2, 8});
    ref_max_dict[2] = new queue<long>({0, 5});
    ref_max_dict[8] = new queue<long>({5, 10});
    ref_max_dict[0] = new queue<long>({2});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[10] = new queue<long>({8, 13});
    ref_max_dict[11] = new queue<long>({13, 14});
    ref_max_dict[14] = new queue<long>({11});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//
//        }
//        cout << "]" << endl;
    }
//    cout << sizes[2] << endl;
    EXPECT_EQ(0, sizes[2]);
}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_145208101311) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {14, 5, 2, 0, 8, 10, 13, 11};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[14] = new queue<long>({11});
    ref_max_dict[11] = new queue<long>({13, 14});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[0] = new queue<long>({2});
    ref_max_dict[10] = new queue<long>({8, 13});
    ref_max_dict[8] = new queue<long>({5, 10});
    ref_max_dict[5] = new queue<long>({2, 8});
    ref_max_dict[2] = new queue<long>({0, 5});

    for (auto i : dict_ConnectivityTree) {
        EXPECT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//
//        }
//        cout << "]" << endl;
    }
    EXPECT_EQ(0, sizes[2]);

}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_457810) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {8, 7, 5, 4, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[8] = new queue<long>({5, 7, 10});
    ref_max_dict[5] = new queue<long>({4, 8});
    ref_max_dict[7] = new queue<long>({4, 8});
    ref_max_dict[10] = new queue<long>({8});
    ref_max_dict[4] = new queue<long>({5, 7});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            //assert()
//            (*i.second).pop();
//
//        }
//        cout << "]" << endl;
    }
    EXPECT_EQ(10, sizes[2]);
}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_205) {

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
    vector<long> listOfFineCells = {2, 0, 5};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    EXPECT_EQ(5, sizes[2]); // 0 est aussi correct
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[2] = new queue<long>({0, 5});
    ref_max_dict[0] = new queue<long>({2,});
    ref_max_dict[5] = new queue<long>({2,});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }

}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_All) {
    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};

    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(12, sizes[2]);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[10] = new queue<long>({8, 9, 13});
    ref_max_dict[7] = new queue<long>({4, 8, 9});
    ref_max_dict[12] = new queue<long>({9,});
    ref_max_dict[9] = new queue<long>({7, 10, 12});
    ref_max_dict[0] = new queue<long>({1, 2,});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[1] = new queue<long>({0, 3});
    ref_max_dict[14] = new queue<long>({11});
    ref_max_dict[2] = new queue<long>({0, 5, 6});
    ref_max_dict[3] = new queue<long>({1, 6});
    ref_max_dict[5] = new queue<long>({2, 4, 8});
    ref_max_dict[6] = new queue<long>({2, 3, 11});
    ref_max_dict[4] = new queue<long>({5, 7});
    ref_max_dict[8] = new queue<long>({5, 7, 10});
    ref_max_dict[11] = new queue<long>({6, 13, 14});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_All_shuffled) {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};

    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {9, 4, 7, 14, 2, 0, 12, 13, 5, 6, 1, 11, 3, 8, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    EXPECT_EQ(12, sizes[2]);  // 0 est aussi correct

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[14] = new queue<long>({11});
    ref_max_dict[6] = new queue<long>({2, 3, 11});
    ref_max_dict[1] = new queue<long>({0, 3});
    ref_max_dict[3] = new queue<long>({1, 6});
    ref_max_dict[9] = new queue<long>({7, 10, 12});
    ref_max_dict[7] = new queue<long>({4, 8, 9});
    ref_max_dict[10] = new queue<long>({8, 9, 13});
    ref_max_dict[12] = new queue<long>({9,});
    ref_max_dict[4] = new queue<long>({5, 7});
    ref_max_dict[8] = new queue<long>({5, 7, 10});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[0] = new queue<long>({1, 2,});
    ref_max_dict[5] = new queue<long>({2, 4, 8});
    ref_max_dict[11] = new queue<long>({6, 13, 14});
    ref_max_dict[2] = new queue<long>({0, 5, 6});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_Case_2) {

    long row_ptr[8] = {0, 4, 7, 13, 16, 21, 26, 29};
    long col_ind[29] = {0, 1, 2, 5, 0, 1, 2, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6};

    long sizes[3] = {8, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    EXPECT_EQ(6, sizes[2]);   // correct answer 1, 3, 6

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[3] = new queue<long>({2, 4});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[6] = new queue<long>({4, 5});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_Case_3_seed_0) {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(7, sizes[2]);  // correct answer 7, 8
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[3] = new queue<long>({2, 4, 8});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[8] = new queue<long>({3});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }

}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_Case_3_seed_1) {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(8, sizes[2]);  // correct answer 7, 8

    unordered_map<long, queue<long> *> ref_max_dict;

    ref_max_dict[1] = new queue<long>({2, 7});
    ref_max_dict[2] = new queue<long>({1, 3, 4, 5});
    ref_max_dict[3] = new queue<long>({2, 4, 8});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[8] = new queue<long>({3});
//
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_Case_3_seed_6) {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {6, 7, 8, 1, 3, 5, 4, 0, 2};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(8, sizes[2]);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[3] = new queue<long>({2, 4, 8});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[8] = new queue<long>({3});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
//        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
    }
//    {0: [1, 2, 5], 1: [0, 2, 7], 2: [0, 1, 3, 4, 5], 3: [2, 4, 8], 4: [2, 3, 5, 6], 5: [0, 2, 4, 6], 6: [4, 5],
//        7: [1], 8: [3]}
}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_Case_3_seed_8) {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {8, 1, 2, 3, 4, 5, 0, 6, 7};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(7, sizes[2]);
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[3] = new queue<long>({2, 4, 8});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[8] = new queue<long>({3});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }

}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_Case_4_random) {

    long row_ptr[9] = {0, 4, 8, 14, 17, 22, 27, 30, 31};
    long col_ind[31] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1};

    long sizes[3] = {9, 31, 0};
    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(7, sizes[2]);
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[3] = new queue<long>({2, 4});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[6] = new queue<long>({4,5});
    ref_max_dict[7] = new queue<long>({1});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        EXPECT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
//    {0: [1, 2, 5], 1: [0, 2, 7], 2: [0, 1, 3, 4, 5], 3: [2, 4, 8], 4: [2, 3, 5, 6], 5: [0, 2, 4, 6], 6: [4, 5],
//        7: [1], 8: [3]},


}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod__Case_5_random) {

    long row_ptr[10] = {0, 4, 8, 15, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 8, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        2};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(7, sizes[2]);
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0,1,3,4,5,8});
    ref_max_dict[3] = new queue<long>({2,4});
    ref_max_dict[4] = new queue<long>({2,3,5,6});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[8] = new queue<long>({2});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }

}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_Case_6) {

    long row_ptr[10] = {0, 4, 7, 11, 15, 19, 22, 26, 32, 37};
    long col_ind[37] = {0, 1, 5, 8, 0, 1, 2, 1, 2, 6, 7, 3, 4, 6, 7, 3, 4, 7, 8, 0, 5, 8, 2, 3, 6, 7, 2, 3, 4, 6, 7, 8,
                        0, 4, 5, 7, 8};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

    EXPECT_EQ(4, sizes[2]);
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 5, 8});
    ref_max_dict[1] = new queue<long>({0,2});
    ref_max_dict[2] = new queue<long>({1, 6, 7});
    ref_max_dict[3] = new queue<long>({4, 6, 7});
    ref_max_dict[4] = new queue<long>({3, 7, 8});
    ref_max_dict[5] = new queue<long>({0, 8});
    ref_max_dict[6] = new queue<long>({2, 3, 7});
    ref_max_dict[7] = new queue<long>({2, 3, 4, 6, 8});
    ref_max_dict[8] = new queue<long>({0, 4, 5, 7});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }

}

TEST(DualGraph_TestSuite, FindSeedViaFrontalMethod_Case_6_seed_6) {

    long row_ptr[10] = {0, 4, 7, 11, 15, 19, 22, 26, 32, 37};
    long col_ind[37] = {0, 1, 5, 8, 0, 1, 2, 1, 2, 6, 7, 3, 4, 6, 7, 3, 4, 7, 8, 0, 5, 8, 2, 3, 6, 7, 2, 3, 4, 6, 7, 8,
                        0, 4, 5, 7, 8};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {6, 0, 1, 2, 3, 4, 5, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    EXPECT_EQ(4, sizes[2]);
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 5, 8});
    ref_max_dict[1] = new queue<long>({0,2});
    ref_max_dict[2] = new queue<long>({1, 6, 7});
    ref_max_dict[3] = new queue<long>({4, 6, 7});
    ref_max_dict[4] = new queue<long>({3, 7, 8});
    ref_max_dict[5] = new queue<long>({0, 8});
    ref_max_dict[6] = new queue<long>({2, 3, 7});
    ref_max_dict[7] = new queue<long>({2, 3, 4, 6, 8});
    ref_max_dict[8] = new queue<long>({0, 4, 5, 7});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }

}

TEST(DualGraph_TestSuite, computeAnisotropicLine_1_AnisotropicCell) {

    long numberOfFineCells = 4;
    long adjMatrix_row_ptr[5] = {0, 3, 6, 9, 12};

    long adjMatrix_col_ind_size = 12;
    long adjMatrix_col_ind[12] = {0, 1, 2,
                                  0, 1, 3,
                                  0, 2, 3,
                                  1, 2, 3};

    double adjMatrix_areaValues[12] = {32.5, 2.5, 25,
                                       2.5, 32.5, 25,
                                       25, 32.5, 25,
                                       25, 25, 32.5};

    long arrayOfFineAnisotropicCompliantCells_size = 2;
    long arrayOfFineAnisotropicCompliantCells[2] = {0, 1};

    bool verbose = true;
    long *agglomerationLines_Idx = new long[numberOfFineCells];
    long *agglomerationLines = new long[numberOfFineCells];

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long agglomerationLines_Idx_size = numberOfFineCells;
    long agglomerationLines_size = numberOfFineCells;

    long sizes[10] = {numberOfFineCells, adjMatrix_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};

    bool isAgglomerationLines = computeAnisotropicLine(sizes,
                                                       adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues,
                                                       arrayOfFineAnisotropicCompliantCells,
                                                       agglomerationLines_Idx,
                                                       agglomerationLines, verbose);

    ASSERT_FALSE(isAgglomerationLines);

    long numberOfAnisotropicLinesPOne_size = sizes[8];
    agglomerationLines_size = sizes[9];

    ASSERT_EQ(2, numberOfAnisotropicLinesPOne_size);
    ASSERT_EQ(0, agglomerationLines_size);
    long ref_agglomerationLines_Idx[2] = {0, 0};
    for (int i = 0; i < 2; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }
    delete[]agglomerationLines_Idx;
    delete[]agglomerationLines;
}

TEST(DualGraph_TestSuite, computeAnisotropicLine_Box_5x5x5_iso_and_Aniso_MG_1_level) {
    long numberOfFineCells = 64;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62, 66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132, 138, 144, 150, 155, 161, 167,
                                  172, 177, 183, 189, 194, 200, 206, 212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278, 282, 287, 293, 299, 304, 309, 315,
                                  321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind_size = 344;
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
                                        2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 30.,
                                        2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25., 25.,
                                        2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
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
    long numberOfFineAnisotropicCompliantCells = 64;
    long arrayOfFineAnisotropicCompliantCells[64] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                     19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
                                                     36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
                                                     53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};
    long numberOfAnisotropicLinesPOne_size = numberOfFineAnisotropicCompliantCells;
    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = numberOfFineCells;
    long agglomerationLines_Idx_size = numberOfFineCells;
    long agglomerationLines_size = numberOfFineAnisotropicCompliantCells;
    long numberOfInts = 5;

    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    long verbose = 0;

    long sizes[10] = {numberOfFineCells, adjMatrix_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};

    bool isAggloLines = computeAnisotropicLine(sizes,
                                               adjMatrix_row_ptr,
                                               adjMatrix_col_ind,
                                               adjMatrix_areaValues,
                                               arrayOfFineAnisotropicCompliantCells,
                                               agglomerationLines_Idx,
                                               agglomerationLines,
                                               verbose);
    ASSERT_TRUE(isAggloLines);
    ASSERT_EQ(64, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(17, sizes[8]);
    ASSERT_EQ(48,  sizes[9]);
    long ref_agglomerationLines_Idx[17] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48};
    long ref_agglomerationLines[48] = {0, 16, 32,
                                       15, 31, 47,
                                       3, 19, 35,
                                       12, 28, 44, 2, 18, 34, 1, 17, 33, 11, 27, 43, 8, 24, 40, 7, 23, 39, 4, 20, 36, 14, 30, 46, 13, 29, 45, 6, 22, 38, 5, 21, 37, 10, 26, 42, 9, 25, 41};
    for (int i = 0; i < 17; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }

    for (int i = 0; i < 48; i++) {
//        cout << agglomerationLines[i] << ", ";
//        if ((i + 1) % 3 == 0) {
//            cout << endl;
//        }
        ASSERT_EQ(ref_agglomerationLines[i], agglomerationLines[i]);
    }
//    cout << endl;
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
}

TEST(DualGraph_TestSuite, computeAnisotropicLine_MostAnisotropicCellInTheMiddle) {

    long numberOfFineCells = 24;
    long adjMatrix_row_ptr[25] = {0, 3, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82, 86, 89, 92};

    long adjMatrix_col_ind_size = 92;
    long adjMatrix_col_ind[92] = {0, 1, 2,  // 0
                                  0, 1, 3,  // 1
                                  0, 2, 3, 4,  // 2
                                  1, 2, 3, 5,
                                  2, 4, 5, 6,  // 4
                                  3, 4, 5, 7,
                                  4, 6, 7, 8,  // 6
                                  5, 6, 7, 9,
                                  6, 8, 9, 10,  // 8
                                  7, 8, 9, 11,
                                  8, 10, 11, 12,  // 10
                                  9, 10, 11, 13,
                                  10, 12, 13, 14,  // 12
                                  11, 12, 13, 15,
                                  12, 14, 15, 16,  // 14
                                  13, 14, 15, 17,
                                  14, 16, 17, 18,  // 16
                                  15, 16, 17, 19,
                                  16, 18, 19, 20,  // 18
                                  17, 18, 19, 21,
                                  18, 20, 21, 22,
                                  19, 20, 21, 23,
                                  20, 22, 23,
                                  21, 22, 23};
    double adjMatrix_areaValues[92] = {32.5, 2.5, 25,
                                       2.5, 32.5, 25,
                                       25, 6.75, 2.25, 25,  // 2
                                       25, 2.25, 6.75, 25,  // 3
                                       25, 6.075, 2.025, 25,
                                       25, 2.025, 6.075, 25,
                                       25, 5.4675, 1.8225, 25, // 6
                                       25, 1.8225, 5.4675, 25,
                                       25, 4.92075, 1.64025, 25,
                                       25, 1.64025, 4.92075, 25,
                                       25, 4.428675, 1.476225, 25,  // 10
                                       25, 1.476225, 4.428675, 25,
                                       25, 4.8715425, 1.6238475, 25,  // 12
                                       25, 1.6238475, 4.8715425, 25,
                                       25, 5.35869675, 1.78623225, 25,// 14
                                       25, 1.78623225, 5.35869675, 25,
                                       25, 5.894566425, 1.964855475, 25,// 16
                                       25, 1.964855475, 5.894566425, 25,
                                       25, 6.4840230675, 2.1613410225, 25, // 18
                                       25, 2.1613410225, 6.4840230675, 25,
                                       25, 7.1324253743, 2.3774751248, 25, // 20
                                       25, 2.3774751248, 7.1324253743, 25,
                                       25, 32.5, 2.5,  // 22
                                       25, 2.5, 32.5,};
    long numberOfFineAnisotropicCompliantCells = 24;
    long arrayOfFineAnisotropicCompliantCells[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                     19, 20, 21, 22, 23};
    long numberOfAnisotropicLinesPOne_size = numberOfFineAnisotropicCompliantCells;
    long agglomerationLines_size = numberOfFineAnisotropicCompliantCells;

    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    bool verbose = false;

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = numberOfFineCells;
    long agglomerationLines_Idx_size = numberOfFineCells;

    long sizes[10] = {numberOfFineCells, adjMatrix_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      numberOfFineAnisotropicCompliantCells, agglomerationLines_Idx_size, agglomerationLines_size};

    computeAnisotropicLine(sizes,
                           adjMatrix_row_ptr,
                           adjMatrix_col_ind,
                           adjMatrix_areaValues,
                           arrayOfFineAnisotropicCompliantCells,
                           agglomerationLines_Idx,
                           agglomerationLines,
                           verbose);

    ASSERT_EQ(24, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(3, sizes[8]);  // number of anisotropic lines
    ASSERT_EQ(24, sizes[9]);  // number of cells in these lines
    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 24;
    long ref_agglomerationLines_Idx[3] = {0, 12, 24};
    long ref_agglomerationLines[24] = {22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0, 23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }

    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
}

TEST(DualGraph_TestSuite, computeAnisotropicLine_MostAnisotropicCellInTheMiddle_2) {

    long numberOfFineCells = 24;
    long adjMatrix_row_ptr[25] = {0, 3, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82, 86, 89, 92};

    long adjMatrix_col_ind_size = 92;
    long adjMatrix_col_ind[92] = {0, 1, 2,  // 0
                                  0, 1, 3,  // 1
                                  0, 2, 3, 4,  // 2
                                  1, 2, 3, 5,
                                  2, 4, 5, 6,  // 4
                                  3, 4, 5, 7,
                                  4, 6, 7, 8,  // 6
                                  5, 6, 7, 9,
                                  6, 8, 9, 10,  // 8
                                  7, 8, 9, 11,
                                  8, 10, 11, 12,  // 10
                                  9, 10, 11, 13,
                                  10, 12, 13, 14,  // 12
                                  11, 12, 13, 15,
                                  12, 14, 15, 16,  // 14
                                  13, 14, 15, 17,
                                  14, 16, 17, 18,  // 16
                                  15, 16, 17, 19,
                                  16, 18, 19, 20,  // 18
                                  17, 18, 19, 21,
                                  18, 20, 21, 22,
                                  19, 20, 21, 23,
                                  20, 22, 23,
                                  21, 22, 23};
    double adjMatrix_areaValues[92] = {32.5, 2.5, 25,
                                       2.5, 32.5, 25,
                                       25, 6.75, 2.25, 25,
                                       25, 2.25, 6.75, 25,
                                       25, 6.075, 2.025, 25,
                                       25, 2.025, 6.075, 25,
                                       25, 5.4675, 1.8225, 25,
                                       25, 1.8225, 5.4675, 25,
                                       25, 6.01425, 2.00475, 25,
                                       25, 2.00475, 6.01425, 25,
                                       25, 6.615675, 2.205225, 25,
                                       25, 2.205225, 6.615675, 25,
                                       25, 5.9541075, 1.9847025, 25,
                                       25, 1.9847025, 5.9541075, 25,
                                       25, 5.35869675, 1.78623225, 25,
                                       25, 1.78623225, 5.35869675, 25,
                                       25, 5.894566425, 1.964855475, 25,
                                       25, 1.964855475, 5.894566425, 25,
                                       25, 6.4840230675, 2.1613410225, 25,
                                       25, 2.1613410225, 6.4840230675, 25,
                                       25, 7.1324253743, 2.3774751248, 25,
                                       25, 2.3774751248, 7.1324253743, 25,
                                       25, 32.5, 2.5,
                                       25, 2.5, 32.5};
    long numberOfFineAnisotropicCompliantCells = 24;
    long arrayOfFineAnisotropicCompliantCells[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                     19, 20, 21, 22, 23};
    long numberOfAnisotropicLinesPOne_size = numberOfFineAnisotropicCompliantCells;
    long agglomerationLines_size = numberOfFineAnisotropicCompliantCells;

    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    bool verbose = false;

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = numberOfFineCells;
    long agglomerationLines_Idx_size = numberOfFineCells;

    long sizes[10] = {numberOfFineCells, adjMatrix_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      numberOfFineAnisotropicCompliantCells, agglomerationLines_Idx_size, agglomerationLines_size};

    computeAnisotropicLine(sizes,
                           adjMatrix_row_ptr,
                           adjMatrix_col_ind,
                           adjMatrix_areaValues,
                           arrayOfFineAnisotropicCompliantCells,
                           agglomerationLines_Idx,
                           agglomerationLines,
                           verbose);
    ASSERT_EQ(24, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(3,sizes[8]);  // number of anisotropic lines
    ASSERT_EQ(24,sizes[9]);  // number of cells in these lines

    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 24;
    long ref_agglomerationLines_Idx[3] = {0, 12, 24};
    long ref_agglomerationLines[24] = {23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i],  agglomerationLines_Idx[i]);
    }
    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
}

TEST(DualGraph_TestSuite, computeAnisotropicLine_MostAnisotropicCellInTheMiddle_Iso_at_both_ends) {

    long numberOfFineCells = 24;
    long adjMatrix_row_ptr[25] = {0, 3, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82, 86, 89, 92};

    long adjMatrix_col_ind_size = 92;
    long adjMatrix_col_ind[92] = {0, 1, 2,  // 0
                                  0, 1, 3,  // 1
                                  0, 2, 3, 4,  // 2
                                  1, 2, 3, 5,
                                  2, 4, 5, 6,  // 4
                                  3, 4, 5, 7,
                                  4, 6, 7, 8,  // 6
                                  5, 6, 7, 9,
                                  6, 8, 9, 10,  // 8
                                  7, 8, 9, 11,
                                  8, 10, 11, 12,  // 10
                                  9, 10, 11, 13,
                                  10, 12, 13, 14,  // 12
                                  11, 12, 13, 15,
                                  12, 14, 15, 16,  // 14
                                  13, 14, 15, 17,
                                  14, 16, 17, 18,  // 16
                                  15, 16, 17, 19,
                                  16, 18, 19, 20,  // 18
                                  17, 18, 19, 21,
                                  18, 20, 21, 22,
                                  19, 20, 21, 23,
                                  20, 22, 23,
                                  21, 22, 23};
    double adjMatrix_areaValues[92] = {100, 25, 25,
                                       25, 100, 25,
                                       25, 6.75, 2.25, 25,
                                       25, 2.25, 6.75, 25,
                                       25, 6.075, 2.025, 25,
                                       25, 2.025, 6.075, 25,
                                       25, 5.4675, 1.8225, 25,
                                       25, 1.8225, 5.4675, 25,
                                       25, 6.01425, 2.00475, 25,
                                       25, 2.00475, 6.01425, 25,
                                       25, 6.615675, 2.205225, 25,
                                       25, 2.205225, 6.615675, 25,
                                       25, 5.9541075, 1.9847025, 25,
                                       25, 1.9847025, 5.9541075, 25,
                                       25, 5.35869675, 1.78623225, 25,
                                       25, 1.78623225, 5.35869675, 25,
                                       25, 5.894566425, 1.964855475, 25,
                                       25, 1.964855475, 5.894566425, 25,
                                       25, 6.4840230675, 2.1613410225, 25,
                                       25, 2.1613410225, 6.4840230675, 25,
                                       25, 75, 25, 25,
                                       25, 25, 75, 25,
                                       25, 100, 25,
                                       25, 25, 100,};
    long numberOfFineAnisotropicCompliantCells = 18;
    long arrayOfFineAnisotropicCompliantCells[18] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                     19};
    long numberOfAnisotropicLinesPOne_size = numberOfFineAnisotropicCompliantCells;
    long agglomerationLines_size = numberOfFineAnisotropicCompliantCells;


    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    bool verbose = 0;

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = numberOfFineCells;
    long agglomerationLines_Idx_size = numberOfFineCells;

    long sizes[10] = {numberOfFineCells, adjMatrix_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      numberOfFineAnisotropicCompliantCells, agglomerationLines_Idx_size, agglomerationLines_size};


    computeAnisotropicLine(sizes,
                           adjMatrix_row_ptr,
                           adjMatrix_col_ind,
                           adjMatrix_areaValues,
                           arrayOfFineAnisotropicCompliantCells,
                           agglomerationLines_Idx,
                           agglomerationLines,
                           verbose);
    ASSERT_EQ(18, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(3,  sizes[8]);  // number of anisotropic lines
    ASSERT_EQ(18, sizes[9]);  // number of cells in these lines
    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 18;
    long ref_agglomerationLines_Idx[3] = {0, 9, 18};
    long ref_agglomerationLines[18] = {3, 5, 7, 9, 11, 13, 15, 17, 19, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }
    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i],  agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
}

TEST(DualGraph_TestSuite, computeConnectedComponent_9_Squares) {
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
    unordered_set<long> listInitialCoarseCell({0, 1, 2, 3, 4, 5, 6, 7, 8});

    list<unordered_set<long>> listOfConnectedSet = computeConnectedComponent(listInitialCoarseCell, row_ptr, col_ind);

    list<unordered_set<long>> ref_listOfConnectedSet;
    ref_listOfConnectedSet.push_front(unordered_set<long>({0,1, 2, 3, 4, 5, 6, 7, 8}));
    ASSERT_EQ(ref_listOfConnectedSet, listOfConnectedSet);

    unordered_set<long> listInitialCoarseCell_2({0, 1, 2, 6, 7, 8});
    list<unordered_set<long>> listOfConnectedSet_2 = computeConnectedComponent(listInitialCoarseCell_2, row_ptr,
                                                                               col_ind);
    list<unordered_set<long>> ref_listOfConnectedSet_2;
    ref_listOfConnectedSet_2.push_front(unordered_set<long>({2, 0, 1}));
    ref_listOfConnectedSet_2.push_front(unordered_set<long>({8, 6, 7}));
    ASSERT_EQ(ref_listOfConnectedSet_2, listOfConnectedSet_2);

    unordered_set<long> listInitialCoarseCell_3({0, 2, 4, 6, 8});
    list<unordered_set<long>> listOfConnectedSet_3 = computeConnectedComponent(listInitialCoarseCell_3, row_ptr,
                                                                               col_ind);
//    cout << "end computeConnectedComponent" << endl;
//    cout << "listOfConnectedSet_3.size() " << listOfConnectedSet_3.size() << endl;

//    assert(listOfConnectedSet_3.size() == 5);
    list<unordered_set<long>> ref_listOfConnectedSet_3;
    ref_listOfConnectedSet_3.push_front(unordered_set<long>({6}));
    ref_listOfConnectedSet_3.push_front(unordered_set<long>({4}));
    ref_listOfConnectedSet_3.push_front(unordered_set<long>({2}));
    ref_listOfConnectedSet_3.push_front(unordered_set<long>({0}));
    ref_listOfConnectedSet_3.push_front(unordered_set<long>({8}));
    ASSERT_EQ(ref_listOfConnectedSet_3, listOfConnectedSet_3);

    unordered_set<long> listInitialCoarseCell_4({0, 2, 4, 1, 8});
    list<unordered_set<long>> listOfConnectedSet_4 = computeConnectedComponent(listInitialCoarseCell_4, row_ptr,
                                                                               col_ind);
//    cout << "end computeConnectedComponent" << endl;
//    cout << "listOfConnectedSet_4.size() " << listOfConnectedSet_4.size() << endl;
//    cout << "listOfConnectedSet_4.front().size() " << listOfConnectedSet_4.front().size() << endl;
//    assert(listOfConnectedSet_4.size() == 2);
//    assert(listOfConnectedSet_4.back().size() == 4);
//    assert(listOfConnectedSet_4.back().count(0) == 1);
//    assert(listOfConnectedSet_4.back().count(1) == 1);
//    assert(listOfConnectedSet_4.back().count(4) == 1);
//    assert(listOfConnectedSet_4.back().count(2) == 1);
//
//    assert(listOfConnectedSet_4.front().size() == 1);
//    assert(listOfConnectedSet_4.front().count(8) == 1);

    list<unordered_set<long>> ref_listOfConnectedSet_4;
    ref_listOfConnectedSet_4.push_front(unordered_set<long>({8}));
    ref_listOfConnectedSet_4.push_back(unordered_set<long>({0,1, 4, 2}));
    ASSERT_EQ(ref_listOfConnectedSet_4, listOfConnectedSet_4);
}