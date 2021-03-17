#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "gtest/gtest.h"


TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_025) {

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


TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_10265813119) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_136111314) {

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


TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_520810131411) {

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


TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_145208101311) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_457810) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_205) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_All) {
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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_All_shuffled) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_Case_2) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_Case_3_seed_0) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_Case_3_seed_1) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_Case_3_seed_6) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_Case_3_seed_8) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_Case_4_random) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod__Case_5_random) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_Case_6) {

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

TEST(Agglomerator_Isostropic_TestSuite, FindSeedViaFrontalMethod_Case_6_seed_6) {

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
