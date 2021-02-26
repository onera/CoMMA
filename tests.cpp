//
// Created by Nicolas Lantos on 18/10/2017.
//
//#include "gtest/gtest.h"
#include "Agglomerator_Isotropic.h"
#include "Agglomerator.h"
#include<assert.h>
#include<iostream>


using namespace std;

int test_FindSeedViaFrontalMethod_025() {

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
    assert(5 == sizes[2]);
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


//    for (auto i : dict_ConnectivityTree){
//        cout<<i.first<<": [";
//        while(! (*i.second).empty())
//        {
//            cout <<(*i.second).front()<< ", ";
//            (*i.second).pop();
//        }
//        cout<<"]"<<endl;
//    }
    //self.assertEqual({0:[2], 2:[0, 5], 5:[2]}, dict_ConnectivityTree);
    return 1;
}


int test_FindSeedViaFrontalMethod_10265813119() {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {10, 2, 6, 5, 8, 13, 11, 9};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    assert(9 == sizes[2]);
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            //assert()
            (*i.second).pop();

        }
        cout << "]" << endl;
    }
    return 1;
//    self.assertEqual(
//    {2: [5, 6], 5: [2, 8], 6: [2, 11], 8: [5, 10], 9: [10], 10: [8, 9, 13], 11: [6, 13], 13: [10, 11]},
//    dict_ConnectivityTree)
}

int test_FindSeedViaFrontalMethod_136111314() {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {1, 3, 6, 11, 13, 14};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

//    unordered_map<long, list<long>> ref_max_dict;
//    ref_max_dict[0] = list<long>({2});
//    ref_max_dict[2] = list<long>({0,5});
//    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            //assert()
            (*i.second).pop();

        }
        cout << "]" << endl;
    }
    cout << sizes[2] << endl;
    assert(13 == sizes[2]);  //14 est aussi correct
//    self.assertEqual(
//    {1: [3], 3: [1, 6], 6: [3, 11], 11: [6, 13, 14], 13: [11], 14: [11]},
//    dict_ConnectivityTree)
    return 1;
}

int test_FindSeedViaFrontalMethod_520810131411() {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {5, 2, 0, 8, 10, 13, 14, 11};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

//    unordered_map<long, list<long>> ref_max_dict;
//    ref_max_dict[0] = list<long>({2});
//    ref_max_dict[2] = list<long>({0,5});
//    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            //assert()
            (*i.second).pop();

        }
        cout << "]" << endl;
    }
    cout << sizes[2] << endl;
    assert(0 == sizes[2]);
//    self.assertEqual(
//    {0: [2], 2: [0, 5], 5: [2, 8], 8: [5, 10], 10: [8, 13], 11: [13, 14], 13: [10, 11], 14: [11]},
//    dict_ConnectivityTree)
    return 1;
}


int test_FindSeedViaFrontalMethod_145208101311() {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {14, 5, 2, 0, 8, 10, 13, 11};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

//    unordered_map<long, list<long>> ref_max_dict;
//    ref_max_dict[0] = list<long>({2});
//    ref_max_dict[2] = list<long>({0,5});
//    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            //assert()
            (*i.second).pop();

        }
        cout << "]" << endl;
    }
    cout << sizes[2] << endl;
    assert(0 == sizes[2]);
//    self.assertEqual(
//    {0: [2], 2: [0, 5], 5: [2, 8], 8: [5, 10], 10: [8, 13], 11: [13, 14], 13: [10, 11], 14: [11]},
//    dict_ConnectivityTree)
    return 1;
}

int test_FindSeedViaFrontalMethod_457810() {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};
    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {8, 7, 5, 4, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);

//    unordered_map<long, list<long>> ref_max_dict;
//    ref_max_dict[0] = list<long>({2});
//    ref_max_dict[2] = list<long>({0,5});
//    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            //assert()
            (*i.second).pop();

        }
        cout << "]" << endl;
    }
    cout << sizes[2] << endl;
    assert(10 == sizes[2]);
//    self.assertEqual(
//    4: [5, 7], 5: [4, 8], 7: [4, 8], 8: [5, 7, 10], 10: [8]
//    dict_ConnectivityTree)
    return 1;
}

int test_FindSeedViaFrontalMethod_205() {

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
    assert(5 == sizes[2]);  // 0 est aussi correct
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
    //self.assertEqual({0:[2], 2:[0, 5], 5:[2]}, dict_ConnectivityTree);
    return 1;
}

int test_FindSeedViaFrontalMethod_All() {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};

    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    assert(12 == sizes[2]);  // 0 est aussi correct
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
    // {0: [1, 2], 1: [0, 3], 2: [0, 5, 6], 3: [1, 6], 4: [5, 7], 5: [2, 4, 8], 6: [2, 3, 11], 7: [4, 8, 9],
//    8: [5, 7, 10], 9: [7, 10, 12], 10: [8, 9, 13], 11: [6, 13, 14], 12: [9], 13: [10, 11], 14: [11]},
    return 1;
}

int test_FindSeedViaFrontalMethod_All_shuffled() {

    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                        14};

    long sizes[3] = {16, 51, 0};
    vector<long> listOfFineCells = {9, 4, 7, 14, 2, 0, 12, 13, 5, 6, 1, 11, 3, 8, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    assert(12 == sizes[2]);  // 0 est aussi correct
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
    // {0: [1, 2], 1: [0, 3], 2: [0, 5, 6], 3: [1, 6], 4: [5, 7], 5: [2, 4, 8], 6: [2, 3, 11], 7: [4, 8, 9],
//    8: [5, 7, 10], 9: [7, 10, 12], 10: [8, 9, 13], 11: [6, 13, 14], 12: [9], 13: [10, 11], 14: [11]},
    return 1;
}


int test_FindSeedViaFrontalMethod_Case_2() {

    long row_ptr[8] = {0, 4, 7, 13, 16, 21, 26, 29};
    long col_ind[29] = {0, 1, 2, 5, 0, 1, 2, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6};

    long sizes[3] = {8, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    cout << "newSeed= " << sizes[2] << endl;
    assert(6 == sizes[2]);  // correct answer 1, 3, 6
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
//    {0: [1, 2, 5], 1: [0, 2], 2: [0, 1, 3, 4, 5], 3: [2, 4], 4: [2, 3, 5, 6], 5: [0, 2, 4, 6], 6: [4, 5]},
    return 1;
}


int test_FindSeedViaFrontalMethod_Case_3_seed_0() {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    cout << "newSeed= " << sizes[2] << endl;
    assert(7 == sizes[2]);  // correct answer 7, 8
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
//   {0: [1, 2, 5], 1: [0, 2, 7], 2: [0, 1, 3, 4, 5], 3: [2, 4, 8], 4: [2, 3, 5, 6], 5: [0, 2, 4, 6], 6: [4, 5],
//    7: [1], 8: [3]},

    return 1;
}


int test_FindSeedViaFrontalMethod_Case_3_seed_1() {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    cout << "newSeed= " << sizes[2] << endl;
    assert(8 == sizes[2]);  // correct answer 7, 8
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
//   {0: [1, 2, 5], 1: [0, 2, 7], 2: [0, 1, 3, 4, 5], 3: [2, 4, 8], 4: [2, 3, 5, 6], 5: [0, 2, 4, 6], 6: [4, 5],
//    7: [1], 8: [3]},

    return 1;
}

int test_FindSeedViaFrontalMethod_Case_3_seed_6() {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {6, 7, 8, 1, 3, 5, 4, 0, 2};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    cout << "newSeed= " << sizes[2] << endl;
    assert(8 == sizes[2]);  // correct answer 7, 8
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
//    {0: [1, 2, 5], 1: [0, 2, 7], 2: [0, 1, 3, 4, 5], 3: [2, 4, 8], 4: [2, 3, 5, 6], 5: [0, 2, 4, 6], 6: [4, 5],
//        7: [1], 8: [3]}

    return 1;
}


int test_FindSeedViaFrontalMethod_Case_3_seed_8() {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {8, 1, 2, 3, 4, 5, 0, 6, 7};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    cout << "newSeed= " << sizes[2] << endl;
    assert(7 == sizes[2]);  // correct answer 7, 8
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
//   {0: [1, 2, 5], 1: [0, 2, 7], 2: [0, 1, 3, 4, 5], 3: [2, 4, 8], 4: [2, 3, 5, 6], 5: [0, 2, 4, 6], 6: [4, 5],
//    7: [1], 8: [3]},

    return 1;
}


int test_FindSeedViaFrontalMethod_Case_4_random() {

    long row_ptr[9] = {0, 4, 8, 14, 17, 22, 27, 30, 31};
    long col_ind[31] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1};

    long sizes[3] = {9, 31, 0};
    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    cout << "newSeed= " << sizes[2] << endl;
    assert(7 == sizes[2]);  // correct answer 7, 8
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
//    {0: [1, 2, 5], 1: [0, 2, 7], 2: [0, 1, 3, 4, 5], 3: [2, 4, 8], 4: [2, 3, 5, 6], 5: [0, 2, 4, 6], 6: [4, 5],
//        7: [1], 8: [3]},

    return 1;
}

int test_FindSeedViaFrontalMethod_Case_5_random() {

    long row_ptr[10] = {0, 4, 8, 15, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 8, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        2};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    cout << "newSeed= " << sizes[2] << endl;
    assert(7 == sizes[2]);
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }

    return 1;

}

int test_FindSeedViaFrontalMethod_Case_6() {

    long row_ptr[10] = {0, 4, 7, 11, 15, 19, 22, 26, 32, 37};
    long col_ind[37] = {0, 1, 5, 8, 0, 1, 2, 1, 2, 6, 7, 3, 4, 6, 7, 3, 4, 7, 8, 0, 5, 8, 2, 3, 6, 7, 2, 3, 4, 6, 7, 8,
                        0, 4, 5, 7, 8};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    cout << "newSeed= " << sizes[2] << endl;
    assert(4 == sizes[2]);
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
//    {0: [1, 5, 8], 1: [0, 2], 2: [1, 6, 7], 3: [4, 6, 7], 4: [3, 7, 8], 5: [0, 8], 6: [2, 3, 7],
//        7: [2, 3, 4, 6, 8], 8: [0, 4, 5, 7]},

    return 1;
}

int test_FindSeedViaFrontalMethod_Case_6_seed_6() {

    long row_ptr[10] = {0, 4, 7, 11, 15, 19, 22, 26, 32, 37};
    long col_ind[37] = {0, 1, 5, 8, 0, 1, 2, 1, 2, 6, 7, 3, 4, 6, 7, 3, 4, 7, 8, 0, 5, 8, 2, 3, 6, 7, 2, 3, 4, 6, 7, 8,
                        0, 4, 5, 7, 8};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {6, 0, 1, 2, 3, 4, 5, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    cout << "newSeed= " << sizes[2] << endl;
    assert(4 == sizes[2]);
    unordered_map<long, list<long>> ref_max_dict;
    ref_max_dict[0] = list<long>({2});
    ref_max_dict[2] = list<long>({0, 5});
    ref_max_dict[5] = list<long>({2});


    for (auto i : dict_ConnectivityTree) {
        cout << i.first << ": [";
        while (!(*i.second).empty()) {
            cout << (*i.second).front() << ", ";
            (*i.second).pop();
        }
        cout << "]" << endl;
    }
//    {0: [1, 5, 8], 1: [0, 2], 2: [1, 6, 7], 3: [4, 6, 7], 4: [3, 7, 8], 5: [0, 8], 6: [2, 3, 7],
//        7: [2, 3, 4, 6, 8], 8: [0, 4, 5, 7]},

    return 1;

}


int test_Computation_Of_Neighbourhood() {

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
        cout << i.first << " : " << i.second << endl;
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual(4, maxOrder);

    delete[] isFineCellAgglomerated_tmp;
    return 1;
}

int test_Computation_Of_Neighbourhood_FineCellsConstraints() {

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
        cout << i.first << " : " << i.second << endl;
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual(4, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
    return 1;
}

int test_Computation_Of_Neighbourhood_FineCellsConstraints_11() {

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
        cout << i.first << " : " << i.second << endl;
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual(4, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
    return 1;
}

int test_Computation_Of_Neighbourhood_FineCellsConstraints_118() {

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
        cout << i.first << " : " << i.second << endl;
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual(5, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
    return 1;
}


int test_Computation_Of_Neighbourhood_FineCellsConstraints_118_maxCard() {

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
        cout << i.first << " : " << i.second << endl;
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual(5, maxOrder);
    delete[] isFineCellAgglomerated_tmp;
    return 1;
}

int test_Computation_Of_Neighbourhood_FineCellsConstraints_1184() {

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
        cout << i.first << " : " << i.second << endl;
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual(5, maxOrder);
    delete[] isFineCellAgglomerated_tmp;

    return 1;
}

int test_Computation_Of_Neighbourhood_FineCellsConstraints_13() {

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
        cout << i.first << " : " << i.second << endl;
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual(5, maxOrder);
    delete[] isFineCellAgglomerated_tmp;

    return 1;
}


int test_ComputeNumberOfCommonFaces() {

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

    assert(1 == computeNumberOfCommonFaces(0, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    assert(1 == computeNumberOfCommonFaces(1, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    assert(2 == computeNumberOfCommonFaces(2, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    assert(0 == computeNumberOfCommonFaces(3, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    assert(2 == computeNumberOfCommonFaces(4, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    assert(3 == computeNumberOfCommonFaces(5, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    assert(2 == computeNumberOfCommonFaces(6, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    assert(2 == computeNumberOfCommonFaces(11, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));

    return 1;
}

int test_CheckConnectivity_MGridGenDoc_example() {
    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};
//    vector<long> datas({0, 1, 3});
    unordered_set<long> datas({0, 1, 3});
    assert(checkConnectivity_w_set(datas, row_ptr, col_ind));
    datas = {3, 1, 0};
//    datas[0] = 3;
//    datas[1] = 1;
//    datas[2] = 0;

    assert(checkConnectivity_w_set(datas, row_ptr, col_ind));
    datas = {1, 3, 0};
//    datas[0] = 1;
//    datas[1] = 3;
//    datas[2] = 0;
    assert(checkConnectivity_w_set(datas, row_ptr, col_ind));

    unordered_set<long> datas2({0, 2});
    assert(checkConnectivity_w_set(datas2, row_ptr, col_ind));
    datas2 = {2, 0};
//    datas2[0] = 2;
//    datas2[1] = 0;
    assert(checkConnectivity_w_set(datas2, row_ptr, col_ind));
    datas2 = {0, 6};
//    datas2[0] = 0;
//    datas2[1] = 6;
    assert(!checkConnectivity_w_set(datas2, row_ptr, col_ind));
    datas2 = {6, 0};
//    datas2[0] = 6;
//    datas2[1] = 0;

    assert(!checkConnectivity_w_set(datas2, row_ptr, col_ind));

    unordered_set<long> datas3({0, 1, 2, 3, 4, 5, 6, 7, 14});
    assert(!checkConnectivity_w_set(datas3, row_ptr, col_ind));
//    datas3[8] = 11;
    datas3 = {0, 1, 2, 3, 4, 5, 6, 7, 11};
    assert(checkConnectivity_w_set(datas3, row_ptr, col_ind));

    unordered_set<long> datas4({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    assert(checkConnectivity_w_set(datas4, row_ptr, col_ind));

    unordered_set<long> datas5({14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
    assert(checkConnectivity_w_set(datas5, row_ptr, col_ind));


    return 1;
}


int test_PartsList() {


    //queue<unordered_set<long>> partsList(vector<long> seq, int length)
    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfList = partsList(seq);
    // print listOfList
    for (auto l:listOfList) {
        for (auto i: l) {
            cout << i << ", ";
        }
        cout << endl;
    }
    //assert([['A'], [3], ['A', 3], ['C'], ['A', 'C'], [3, 'C'], ['A', 3, 'C']], l);

    return 1;
}

int test_PartsList_length_1() {


    //queue<unordered_set<long>> partsList(vector<long> seq, int length)
    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfList = partsList(seq, 1);
    // print listOfList
    for (auto l:listOfList) {
        for (auto i: l) {
            cout << i << ", ";
        }
        cout << endl;
    }
    //assert([['A'], [3], ['A', 3], ['C'], ['A', 'C'], [3, 'C'], ['A', 3, 'C']], l);

    return 1;
}

int test_PartsList_length_2() {


    //queue<unordered_set<long>> partsList(vector<long> seq, int length)
    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfList = partsList(seq, 2);
    // print listOfList
    for (auto l:listOfList) {
        for (auto i: l) {
            cout << i << ", ";
        }
        cout << endl;
    }
    //assert([['A'], [3], ['A', 3], ['C'], ['A', 'C'], [3, 'C'], ['A', 3, 'C']], l);

    return 1;
}

int test_PartsList_length_3() {


    //queue<unordered_set<long>> partsList(vector<long> seq, int length)
    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfList = partsList(seq, 3);
    // print listOfList
    for (auto l:listOfList) {
        for (auto i: l) {
            cout << i << ", ";
        }
        cout << endl;
    }
    //assert([['A'], [3], ['A', 3], ['C'], ['A', 'C'], [3, 'C'], ['A', 3, 'C']], l);

    return 1;
}


int test_RemoveSeparatingVertex_205() {

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
    long new_seed = 0;
    unordered_map<long, queue<long> *> dict_SpanningTree;
    dict_SpanningTree[0] = new queue<long>({2});
    dict_SpanningTree[2] = new queue<long>({5});
    //= {0:[2], 2:[5]}
    unordered_set<long> setOfFineCells({2, 0, 5});
    removeSeparatingVertex(new_seed, dict_SpanningTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
    cout << endl;
//    cout<<"Size= "<<setOfFineCells.size()<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.size() == 3);
//    self.assertEqual({0, 2, 5}, setOfFineCells)

    return 1;
}

int test_RemoveSeparatingVertex_136111314() {

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
//    new_seed = 1
//    dict_SpanningTree = {1: [3], 3: [6], 6: [11], 11: [13, 14]}
//    setOfFineCells = set([1, 3, 6, 11, 13, 14])
//    Util.removeSeparatingVertex(new_seed, dict_SpanningTree, setOfFineCells, row_ptr, col_ind)
//    self.assertEqual({1, 3, 6, 13, 14}, setOfFineCells)

    long new_seed = 1;
//    long new_seed = sizes[2];
    cout << "new_seed " << new_seed << endl;
//    unordered_map<long, queue<long> *> dict_SpanningTree;
//    dict_SpanningTree[0] = new queue<long>({2});
//    dict_SpanningTree[2] = new queue<long>({5});
    //= {0:[2], 2:[5]}
    unordered_set<long> setOfFineCells({1, 3, 6, 11, 13, 14});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
    cout << "\nsetOfFineCells:" << endl;
    for (auto i : setOfFineCells) {
        cout << i << ", ";
    }
    cout << endl;
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(13) == 1);
    assert(setOfFineCells.count(14) == 1);

    assert(setOfFineCells.size() == 5);


    return 1;
}

int test_RemoveSeparatingVertex_10265813119() {

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
//    new_seed = 1
//    dict_SpanningTree = {1: [3], 3: [6], 6: [11], 11: [13, 14]}
//    setOfFineCells = set([1, 3, 6, 11, 13, 14])
//    Util.removeSeparatingVertex(new_seed, dict_SpanningTree, setOfFineCells, row_ptr, col_ind)
//    self.assertEqual({1, 3, 6, 13, 14}, setOfFineCells)

    long new_seed = 1;
//    long new_seed = sizes[2];
    cout << "new_seed " << new_seed << endl;
//    unordered_map<long, queue<long> *> dict_SpanningTree;
//    dict_SpanningTree[0] = new queue<long>({2});
//    dict_SpanningTree[2] = new queue<long>({5});
    //= {0:[2], 2:[5]}
    unordered_set<long> setOfFineCells({10, 2, 6, 5, 8, 13, 11, 9});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
    cout << "\nsetOfFineCells:" << endl;
    for (auto i : setOfFineCells) {
        cout << i << ", ";
    }
    cout << endl;
    assert(setOfFineCells.count(10) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.count(13) == 1);
    assert(setOfFineCells.count(11) == 1);
    assert(setOfFineCells.count(9) == 1);

    assert(setOfFineCells.size() == 8);


    return 1;
}

int test_RemoveSeparatingVertex_All() {

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
//    long new_seed = sizes[2];
//    cout<<"new_seed "<<new_seed<<endl;
//    unordered_map<long, queue<long> *> dict_SpanningTree;
//    dict_SpanningTree[0] = new queue<long>({2});
//    dict_SpanningTree[2] = new queue<long>({5});
    //= {0:[2], 2:[5]}
    unordered_set<long> setOfFineCells({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(4) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.count(9) == 1);
    assert(setOfFineCells.count(10) == 1);
    assert(setOfFineCells.count(12) == 1);
    assert(setOfFineCells.count(13) == 1);
    assert(setOfFineCells.count(14) == 1);

    assert(setOfFineCells.size() == 13);


    return 1;
}


int test_RemoveSeparatingVertex_Case_2() {

    long row_ptr[8] = {0, 4, 7, 13, 16, 21, 26, 29};
    long col_ind[29] = {0, 1, 2, 5, 0, 1, 2, 0, 1, 2, 3, 4, 5, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6};

    long sizes[3] = {8, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};


//    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
//                                                                                        row_ptr, col_ind);
    //    dict_ConnectivityTree = {0: [1], 2: [0, 6], 3: [1], 4: [5], 5: [2], 6: [2, 3], 7: [4, 8], 8: [5], 9: [7, 10], 10: [8, 13], 11: [6, 14], 12: [9], 13: [11]}



    long new_seed = 1;
//    long new_seed = sizes[2];
//    cout<<"new_seed "<<new_seed<<endl;
    unordered_map<long, queue<long> *> dict_SpanningTree;
    dict_SpanningTree[0] = new queue<long>({5});
    dict_SpanningTree[1] = new queue<long>({0, 2});
    dict_SpanningTree[2] = new queue<long>({3, 4});
    dict_SpanningTree[5] = new queue<long>({6});
    //= {0:[2], 2:[5]}
    unordered_set<long> setOfFineCells({0, 1, 2, 3, 4, 5, 6});
    removeSeparatingVertex(new_seed, dict_SpanningTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.size() == 6);


    return 1;
}

int test_RemoveSeparatingVertex_Case_3_seed_0() {

    long row_ptr[10] = {0, 4, 8, 14, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 2, 3, 4, 8, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        3};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};


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
    unordered_set<long> setOfFineCells({0, 1, 2, 3, 4, 5, 6, 7, 8});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.size() == 7);


    return 1;
}


int test_RemoveSeparatingVertex_Case_4_random() {

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
    unordered_set<long> setOfFineCells({6, 7, 1, 3, 5, 4, 0, 2});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.size() == 7);


    return 1;
}

int test_RemoveSeparatingVertex_Case_5_random() {

    long row_ptr[10] = {0, 4, 8, 15, 18, 23, 28, 31, 32, 33};
    long col_ind[33] = {0, 1, 2, 5, 0, 1, 2, 7, 0, 1, 2, 3, 4, 5, 8, 2, 3, 4, 2, 3, 4, 5, 6, 0, 2, 4, 5, 6, 4, 5, 6, 1,
                        2};

    long sizes[3] = {10, 33, 0};
    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
    long new_seed = 7;
//    long new_seed = sizes[2];
//    cout<<"new_seed "<<new_seed<<endl;

    unordered_set<long> setOfFineCells({6, 7, 1, 3, 5, 4, 0, 2, 8});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.size() == 7);

    return 1;

}


int test_RemoveSeparatingVertex_Case_6() {

    long row_ptr[10] = {0, 4, 7, 11, 15, 19, 22, 26, 32, 37};
    long col_ind[37] = {0, 1, 5, 8, 0, 1, 2, 1, 2, 6, 7, 3, 4, 6, 7, 3, 4, 7, 8, 0, 5, 8, 2, 3, 6, 7, 2, 3, 4, 6, 7, 8,
                        0, 4, 5, 7, 8};

    long sizes[3] = {10, 37, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    long new_seed = 7;
    long new_seed = sizes[2];
    cout << "new_seed " << new_seed << endl;
    assert(new_seed == 4);
    unordered_set<long> setOfFineCells({6, 7, 1, 3, 5, 4, 0, 2, 8});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(4) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.size() == 7);
    return 1;
}


int test_RemoveSeparatingVertex_Case_7_seed_0() {

    long row_ptr[12] = {0, 3, 8, 12, 15, 19, 22, 27, 31, 34, 37, 40};
    long col_ind[40] = {0, 2, 7, 1, 3, 4, 8, 9, 0, 2, 5, 6, 1, 3, 9, 1, 4, 6, 9, 2, 5, 10, 2, 4, 6,
                        7, 10, 0, 6, 7, 8, 1, 7, 8, 3, 4, 9, 5, 6, 10};

    long sizes[3] = {12, 40, 0};
    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    long new_seed = 7;
    long new_seed = sizes[2];
//    cout << "new_seed " << new_seed << endl;
    assert(new_seed == 5);
    unordered_set<long> setOfFineCells({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(4) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.count(9) == 1);
    assert(setOfFineCells.count(10) == 1);
    assert(setOfFineCells.size() == 10);
    return 1;
}


int test_RemoveSeparatingVertex_Case_7_seed_1() {

    long row_ptr[12] = {0, 3, 8, 12, 15, 19, 22, 27, 31, 34, 37, 40};
    long col_ind[40] = {0, 2, 7, 1, 3, 4, 8, 9, 0, 2, 5, 6, 1, 3, 9, 1, 4, 6, 9, 2, 5, 10, 2, 4, 6,
                        7, 10, 0, 6, 7, 8, 1, 7, 8, 3, 4, 9, 5, 6, 10};

    long sizes[3] = {12, 40, 0};
    vector<long> listOfFineCells = {1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    long new_seed = 7;
    long new_seed = sizes[2];
//    cout << "new_seed " << new_seed << endl;
    assert(new_seed == 3);
    unordered_set<long> setOfFineCells({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(4) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.count(9) == 1);
    assert(setOfFineCells.count(10) == 1);
    assert(setOfFineCells.size() == 10);
    return 1;
}


int test_RemoveSeparatingVertex_Case_7_seed_9() {

    long row_ptr[12] = {0, 3, 8, 12, 15, 19, 22, 27, 31, 34, 37, 40};
    long col_ind[40] = {0, 2, 7, 1, 3, 4, 8, 9, 0, 2, 5, 6, 1, 3, 9, 1, 4, 6, 9, 2, 5, 10, 2, 4, 6,
                        7, 10, 0, 6, 7, 8, 1, 7, 8, 3, 4, 9, 5, 6, 10};

    long sizes[3] = {12, 40, 0};
    vector<long> listOfFineCells = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    long new_seed = 7;
    long new_seed = sizes[2];
//    cout << "new_seed " << new_seed << endl;
    assert(new_seed == 5);
    unordered_set<long> setOfFineCells({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(4) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.count(9) == 1);
    assert(setOfFineCells.count(10) == 1);
    assert(setOfFineCells.size() == 10);

    return 1;
}

int test_RemoveSeparatingVertex_Case_8_random() {

    long row_ptr[12] = {0, 5, 8, 11, 15, 20, 23, 26, 28, 32, 36, 39};
    long col_ind[39] = {0, 1, 2, 3, 4, 0, 1, 9, 0, 2, 6, 0, 3, 6, 8, 0, 4, 5, 8, 9, 4, 5, 7, 2, 3,
                        6, 5, 7, 3, 4, 8, 10, 1, 4, 9, 10, 8, 9, 10};

    long sizes[3] = {12, 39, 0};
    vector<long> listOfFineCells = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    long new_seed = 7;
    long new_seed = sizes[2];
//    cout << "new_seed " << new_seed << endl;
    assert(new_seed == 7);
    unordered_set<long> setOfFineCells({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(4) == 1);
    assert(setOfFineCells.count(5) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(7) == 1);
    assert(setOfFineCells.count(10) == 1);
    assert(setOfFineCells.size() == 8);

    return 1;
}


int test_RemoveSeparatingVertex_Case_9_random() {

    long row_ptr[12] = {0, 2, 4, 8, 11, 14, 19, 23, 27, 30, 32, 35};
    long col_ind[35] = {0, 7, 1, 5, 2, 3, 5, 10, 2, 3, 8, 4, 6, 10, 1, 2, 5, 7, 8, 4, 6, 7, 9, 0, 5,
                        6, 7, 3, 5, 8, 6, 9, 2, 4, 10};

    long sizes[3] = {12, 35, 0};
    vector<long> listOfFineCells = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};

    unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, sizes, listOfFineCells,
                                                                                        row_ptr, col_ind);
//    long new_seed = 7;
    long new_seed = sizes[2];
//    cout << "new_seed " << new_seed << endl;
    assert(new_seed == 9);
    unordered_set<long> setOfFineCells({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    removeSeparatingVertex(new_seed, dict_ConnectivityTree, setOfFineCells, row_ptr, col_ind);
//    cout<<"\nsetOfFineCells:"<<endl;
//    for (auto i : setOfFineCells) {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(setOfFineCells.count(0) == 1);
    assert(setOfFineCells.count(1) == 1);
    assert(setOfFineCells.count(2) == 1);
    assert(setOfFineCells.count(3) == 1);
    assert(setOfFineCells.count(4) == 1);
    assert(setOfFineCells.count(6) == 1);
    assert(setOfFineCells.count(8) == 1);
    assert(setOfFineCells.count(9) == 1);
    assert(setOfFineCells.count(10) == 1);
    assert(setOfFineCells.size() == 9);

    return 1;
}


int test_SwapFineCell_Case_1() {

    int iLevel = 0;
    long iFineCell = 0;
    long iOrigineCoarseCell = 0;
    long iDestinationCoarseCell = 1;
//    unordered_map<long, unordered_set<long>> dict_Coarse_Elem,
//    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells,
//    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements,

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({0, 3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 1});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 2;

    long fine_Cell_indices_To_Coarse_Cell_Indices[4] = {0, 1, 1, 0};

    unordered_set<long> set_removedCoarseCells = swapFineCell(iFineCell, iOrigineCoarseCell, iDestinationCoarseCell,
                                                              dict_Coarse_Elem,
                                                              dict_Card_Coarse_Cells,
                                                              dict_DistributionOfCardinalOfCoarseElements,
                                                              fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(set_removedCoarseCells.size() == 0);
    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem.count(0) == 1);
//    cout<<"dict_Coarse_Elem[0].size() "<<dict_Coarse_Elem[0].size()<<endl;
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);
    assert(dict_Coarse_Elem.count(1) == 1);
    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);
    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);


    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);


    long ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};
    for (int i = 0; i < 4; i++) {
//        cout<<i<<" "<<fine_Cell_indices_To_Coarse_Cell_Indices[i]<<endl;
        assert(ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[i] == fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
//self.assertEqual(set(), set_removedCoarseCells)
//self.assertEqual({0: [3], 1: [1, 2, 0]}, dict_Coarse_Elem)
//self.assertEqual({1: {0}, 3: {1}}, dict_Card_Coarse_Cells)
//self.assertEqual({1: 1, 3: 1}, dict_DistributionOfCardinalOfCoarseElements)
//np.testing.assert_almost_equal(np.array([1, 1, 1, 0]), fine_Cell_indices_To_Coarse_Cell_Indices[0])
}


int test_SwapFineCell_Deletion_of_Coarse_Element() {

    int iLevel = 0;
    long iFineCell = 3;
    long iOrigineCoarseCell = 0;
    long iDestinationCoarseCell = 1;

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({0, 1, 2});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};

    unordered_set<long> set_removedCoarseCells = swapFineCell(iFineCell, iOrigineCoarseCell, iDestinationCoarseCell,
                                                              dict_Coarse_Elem,
                                                              dict_Card_Coarse_Cells,
                                                              dict_DistributionOfCardinalOfCoarseElements,
                                                              fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(set_removedCoarseCells.size() == 1);
    assert(set_removedCoarseCells.count(0) == 1);

    assert(dict_Coarse_Elem.size() == 1);
    assert(dict_Coarse_Elem.count(1) == 1);
//    cout<<"dict_Coarse_Elem[0].size() "<<dict_Coarse_Elem[0].size()<<endl;
    assert(dict_Coarse_Elem[1].size() == 4);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);
    assert(dict_Coarse_Elem[1].count(3) == 1);


    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[4].size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(1) == 1);


    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);


    long ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 1};
    for (int i = 0; i < 4; i++) {
//        cout<<i<<" "<<fine_Cell_indices_To_Coarse_Cell_Indices[i]<<endl;
        assert(ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[i] == fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_computeConnectedComponent_9_Squares() {
//"""
//9 squares (3*3)
//:return:
//"""
//matrix = {(0, 0): 1,
//(0, 1): 1.0,
//(0, 3): 1.0,
//(1, 0): 1.0,
//(1, 1): 1.0,
//(1, 2): 1.0,
//(1, 4): 1.0,
//(2, 1): 1.0,
//(2, 2): 1.0,
//(2, 5): 1.0,
//(3, 0): 1.0,
//(3, 3): 1.0,
//(3, 4): 1.0,
//(3, 6): 1.0,
//(4, 1): 1.0,
//(4, 3): 1.0,
//(4, 4): 1.0,
//(4, 5): 1.0,
//(4, 7): 1.0,
//(5, 2): 1.0,
//(5, 4): 1.0,
//(5, 5): 1.0,
//(5, 8): 1.0,
//(6, 3): 1.0,
//(6, 6): 1.0,
//(6, 7): 1.0,
//(7, 4): 1.0,
//(7, 6): 1.0,
//(7, 7): 1.0,
//(7, 8): 1.0,
//(8, 5): 1.0,
//(8, 7): 1.0,
//(8, 8): 1.0,
//}

    int nbCells = 9;
//newMatrix = Util.updateOldDictToNewDict(matrix, nbCells)
//row_ptr, col_ind, values = Util.createCRSFromDict_New(newMatrix)
    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    unordered_set<long> listInitialCoarseCell({0, 1, 2, 3, 4, 5, 6, 7, 8});

    list<unordered_set<long>> listOfConnectedSet = computeConnectedComponent(listInitialCoarseCell, row_ptr, col_ind);
    assert(listOfConnectedSet.size() == 1);
    assert(listOfConnectedSet.front().size() == 9);
    assert(listOfConnectedSet.front().count(0) == 1);
    assert(listOfConnectedSet.front().count(1) == 1);
    assert(listOfConnectedSet.front().count(2) == 1);
    assert(listOfConnectedSet.front().count(3) == 1);
    assert(listOfConnectedSet.front().count(4) == 1);
    assert(listOfConnectedSet.front().count(5) == 1);
    assert(listOfConnectedSet.front().count(6) == 1);
    assert(listOfConnectedSet.front().count(7) == 1);
    assert(listOfConnectedSet.front().count(8) == 1);
//    cout<<"ok"<<endl;
    unordered_set<long> listInitialCoarseCell_2({0, 1, 2, 6, 7, 8});
    list<unordered_set<long>> listOfConnectedSet_2 = computeConnectedComponent(listInitialCoarseCell_2, row_ptr,
                                                                               col_ind);
//    cout<<"end computeConnectedComponent"<<endl;
//    cout<<"listOfConnectedSet_2.size() "<<listOfConnectedSet_2.size()<<endl;
//    cout<<"\nlistOfConnectedSet_2:"<<endl;
//    for (auto i : listOfConnectedSet_2.front()) {
//        cout<<i<<", ";
//    }
//    cout<<endl;

    assert(listOfConnectedSet_2.size() == 2);
    assert(listOfConnectedSet_2.front().size() == 3);
    assert(listOfConnectedSet_2.front().count(8) == 1);
    assert(listOfConnectedSet_2.front().count(6) == 1);
    assert(listOfConnectedSet_2.front().count(7) == 1);

//    cout<<"\nlistOfConnectedSet_2:"<<endl;
//    for (auto i : listOfConnectedSet_2.back()) {
//        cout<<i<<", ";
//    }
//    cout<<endl;


    assert(listOfConnectedSet_2.back().size() == 3);
    assert(listOfConnectedSet_2.back().count(2) == 1);
    assert(listOfConnectedSet_2.back().count(0) == 1);
    assert(listOfConnectedSet_2.back().count(1) == 1);


    unordered_set<long> listInitialCoarseCell_3({0, 2, 4, 6, 8});
    list<unordered_set<long>> listOfConnectedSet_3 = computeConnectedComponent(listInitialCoarseCell_3, row_ptr,
                                                                               col_ind);
    cout << "end computeConnectedComponent" << endl;
    cout << "listOfConnectedSet_3.size() " << listOfConnectedSet_3.size() << endl;
    assert(listOfConnectedSet_3.size() == 5);
    int ref[5] = {8, 0, 2, 4, 6};
    int i = 0;
    for (auto iSet: listOfConnectedSet_3) {
//        cout << *iSet.begin()<<endl;
        assert(iSet.size() == 1);
        assert(*iSet.begin() == ref[i]);
        i++;
    }
//    exit(1);
    unordered_set<long> listInitialCoarseCell_4({0, 2, 4, 1, 8});
    list<unordered_set<long>> listOfConnectedSet_4 = computeConnectedComponent(listInitialCoarseCell_4, row_ptr,
                                                                               col_ind);
    cout << "end computeConnectedComponent" << endl;
    cout << "listOfConnectedSet_4.size() " << listOfConnectedSet_4.size() << endl;
    cout << "listOfConnectedSet_4.front().size() " << listOfConnectedSet_4.front().size() << endl;
    assert(listOfConnectedSet_4.size() == 2);
    assert(listOfConnectedSet_4.back().size() == 4);
    assert(listOfConnectedSet_4.back().count(0) == 1);
    assert(listOfConnectedSet_4.back().count(1) == 1);
    assert(listOfConnectedSet_4.back().count(4) == 1);
    assert(listOfConnectedSet_4.back().count(2) == 1);

    assert(listOfConnectedSet_4.front().size() == 1);
    assert(listOfConnectedSet_4.front().count(8) == 1);


    return 1;
}


int test_createCoarseCell_Case_1() {

    int iLevel = 0;
    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices);

    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Coarse_Elem[2].size() == 4);
    assert(dict_Coarse_Elem[2].count(4) == 1);
    assert(dict_Coarse_Elem[2].count(5) == 1);
    assert(dict_Coarse_Elem[2].count(6) == 1);
    assert(dict_Coarse_Elem[2].count(7) == 1);
//    self.assertEqual({0: [3], 1: [1, 2, 0], 2: [4, 5, 6, 7]}, dict_Coarse_Elem)

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_Card_Coarse_Cells[4].size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

//    self.assertEqual({1: {0}, 3: {1}, 4: {2}}, dict_Card_Coarse_Cells)
    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}


int test_createCoarseCell_Case_1_isMutable_False() {
// Ex case 2
    int iLevel = 0;
    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices, false);

    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_createCoarseCell_Case_3() {

    unordered_set<long> l({4, 5, 6});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices);

    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 7);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Coarse_Elem[2].size() == 3);
    assert(dict_Coarse_Elem[2].count(4) == 1);
    assert(dict_Coarse_Elem[2].count(5) == 1);
    assert(dict_Coarse_Elem[2].count(6) == 1);
//    assert(dict_Coarse_Elem[2].count(7) == 1);
//    self.assertEqual({0: [3], 1: [1, 2, 0], 2: [4, 5, 6, 7]}, dict_Coarse_Elem)

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);

//    self.assertEqual({1: {0}, 3: {1}, 4: {2}}, dict_Card_Coarse_Cells)
    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);


//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_createCoarseCell_Delayed_Case_1() {

    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices, true, true);

    assert(indCoarseElement == 2);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 8);


//    self.assertEqual({0: [3], 1: [1, 2, 0], 2: [4, 5, 6, 7]}, dict_Coarse_Elem)
//    self.assertEqual({1: {0}, 3: {1}, 4: {2}}, dict_Card_Coarse_Cells)
//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
//    np.testing.assert_almost_equal(np.array([True, True, True, True, True, True, True, True, ]),
//                                   isFineCellAgglomerated_tmp[0])
//
//    np.testing.assert_almost_equal(np.array([1, 1, 1, 0, 2, 2, 2, 2]),
//            fine_Cell_indices_To_Coarse_Cell_Indices[0])

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Coarse_Elem[2].size() == 4);
    assert(dict_Coarse_Elem[2].count(4) == 1);
    assert(dict_Coarse_Elem[2].count(5) == 1);
    assert(dict_Coarse_Elem[2].count(6) == 1);
    assert(dict_Coarse_Elem[2].count(7) == 1);

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_Card_Coarse_Cells[4].size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_2[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}


int test_createCoarseCell_Delayed_Case_1_isMutable_False() {

    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices, false, true);

    assert(indCoarseElement == 2);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);

    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_Card_Coarse_Cells[4].size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_2[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}


int test_createCoarseCell_Delayed_Case_3() {

    unordered_set<long> l({4, 5, 6});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices, true, true);

    assert(indCoarseElement == 2);
    assert(numberOfFineAgglomeratedCells_tmp == 7);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);


//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 7);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);

    assert(dict_Card_Coarse_Cells[3].size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);


    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);


//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_2[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}

int test_agglomerate_Isotropic_CreateDelayedCoarseCells_Case_1() {

    unordered_set<long> l1({4, 5, 6, 7});
    unordered_set<long> l2({8, 9, 10});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[11] = {1, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[11] = {true, true, true, true, false, false, false, false, false, false, false};

    // Creation of the first cell l1:
    //===============================
    createCoarseCell(l1, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices, true, true);

    assert(indCoarseElement == 2);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

    bool ref_isFineCellAgglomerated_tmp[11] = {true, true, true, true, true, true, true, true, false, false, false,};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[11] = {1, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1};
    for (int i = 0; i < 11; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Creation of the first cell l2:
    //===============================
    createCoarseCell(l2, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices, true, true);

    assert(indCoarseElement == 2);
    assert(numberOfFineAgglomeratedCells_tmp == 11);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

    bool ref_isFineCellAgglomerated_tmp_2[11] = {true, true, true, true, true, true, true, true, true, true, true};
//    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[11] = {1, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1};
    for (int i = 0; i < 11; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_2[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    list<unordered_set<long>> delayedCoarseCells;
    delayedCoarseCells.push_back(l1);
    delayedCoarseCells.push_back(l2);
    agglomerate_Isotropic_CreateDelayedCoarseCells(dict_Coarse_Elem,
                                                   dict_Card_Coarse_Cells,
                                                   delayedCoarseCells, indCoarseElement, fine_Cell_indices_To_Coarse_Cell_Indices);

    assert(indCoarseElement == 4);
    assert(numberOfFineAgglomeratedCells_tmp == 11);

    assert(dict_Coarse_Elem.size() == 4);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Coarse_Elem[2].size() == 4);
    assert(dict_Coarse_Elem[2].count(4) == 1);
    assert(dict_Coarse_Elem[2].count(5) == 1);
    assert(dict_Coarse_Elem[2].count(6) == 1);
    assert(dict_Coarse_Elem[2].count(7) == 1);

    assert(dict_Coarse_Elem[3].size() == 3);
    assert(dict_Coarse_Elem[3].count(8) == 1);
    assert(dict_Coarse_Elem[3].count(9) == 1);
    assert(dict_Coarse_Elem[3].count(10) == 1);

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);

    assert(dict_Card_Coarse_Cells[3].size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(3) == 1);

    assert(dict_Card_Coarse_Cells[4].size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp_3[11] = {true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[11] = {1, 1, 1, 0, 2, 2, 2, 2, 3, 3, 3};
    for (int i = 0; i < 11; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_3[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}


int test_splitNonConnectedCoarseCell_9_Squares_1() {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    unordered_set<long> listInitialCoarseCell({0, 1, 2, 3, 4, 5, 6, 7, 8});
    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 3, 1, 3, 3, 3, 2, 3, 1};

    long indCoarseElement = 4;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

//    dict_Coarse_Cells = dict([(0,[0]), (1,[2, 8]), (2,[6]), (3,[1, 3, 4, 5, 7])])
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0});
    dict_Coarse_Cells[1] = unordered_set<long>({2, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({6});
    dict_Coarse_Cells[3] = unordered_set<long>({1, 3, 4, 5, 7});

//    dict_Card_Coarse_Cells = dict([(1, {0, 2}), (2, {1}), (5, {3})])
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 2});
    dict_Card_Coarse_Cells[2] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({3});

//    dict_DistributionOfCardinalOfCoarseElements = dict([(1, 2), (2, 1), (5, 1)])
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 2;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    long iCoarseCell = 1;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);

// print "dict_Coarse_Cells", dict_Coarse_Cells
// print "dict_Card_Coarse_Cells", dict_Card_Coarse_Cells
// print "dict_DistributionOfCardinalOfCoarseElements", dict_DistributionOfCardinalOfCoarseElements
// print "isFineCellAgglomerated", isFineCellAgglomerated
// print "fine_Cell_indices_To_Coarse_Cell_Indices", fine_Cell_indices_To_Coarse_Cell_Indices
    assert(5 == indCoarseElement);

    assert(dict_Coarse_Cells.size() == 5);
    assert(dict_Coarse_Cells[0].size() == 1);
    assert(dict_Coarse_Cells[1].size() == 1);
    assert(dict_Coarse_Cells[2].size() == 1);
    assert(dict_Coarse_Cells[3].size() == 5);
    assert(dict_Coarse_Cells[4].size() == 1);

    assert(dict_Coarse_Cells[0].count(0) == 1);

//    for (auto i: dict_Coarse_Cells[1]) {
//        cout << i << endl;
//    }

    assert(dict_Coarse_Cells[1].count(2) == 1);

    assert(dict_Coarse_Cells[2].count(6) == 1);

    assert(dict_Coarse_Cells[3].count(1) == 1);
    assert(dict_Coarse_Cells[3].count(3) == 1);
    assert(dict_Coarse_Cells[3].count(4) == 1);
    assert(dict_Coarse_Cells[3].count(5) == 1);
    assert(dict_Coarse_Cells[3].count(7) == 1);

    assert(dict_Coarse_Cells[4].count(8) == 1);


//    assert({0:[0], 1:[8], 2:[6], 3:[1, 3, 4, 5, 7], 4:[2]}, dict_Coarse_Cells)
//    assert(dict([(1, {0, 1, 2, 4}), (5, {3})]), dict_Card_Coarse_Cells)
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 4);
    assert(dict_Card_Coarse_Cells[5].size() == 1);

    assert(dict_Card_Coarse_Cells[1].count(0) == 1);
    assert(dict_Card_Coarse_Cells[1].count(1) == 1);
    assert(dict_Card_Coarse_Cells[1].count(2) == 1);
    assert(dict_Card_Coarse_Cells[1].count(4) == 1);

    assert(dict_Card_Coarse_Cells[5].count(3) == 1);


//    assert({1: 4, 5: 1}, dict_DistributionOfCardinalOfCoarseElements)
    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 4);
    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 1);
    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 3, 1, 3, 3, 3, 2, 3, 4};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
//        cout<< fine_Cell_indices_To_Coarse_Cell_Indices[i]<<endl;
    }

//    np.testing.assert_almost_equal(np.array([True, True, True, True, True, True, True, True, True]),
//                                   isFineCellAgglomerated)
//    np.testing.assert_almost_equal(np.array([0, 3, 4, 3, 3, 3, 2, 3, 1]), fine_Cell_indices_To_Coarse_Cell_Indices[0])
    return 1;
}

int test_splitNonConnectedCoarseCell_9_Squares_2() {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    unordered_set<long> listInitialCoarseCell({0, 1, 2, 3, 4, 5, 6, 7, 8});
    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 2, 2, 2, 1, 2, 1};

    long indCoarseElement = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

//    dict_Coarse_Cells = dict([(0, [0, 2]), (1, [6, 8]), (2, [1, 3, 4, 5, 7])])
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 2});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 3, 4, 5, 7});

//  dict_Card_Coarse_Cells = dict([(2, {0, 1}), (5, {3})])
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

//  dict_DistributionOfCardinalOfCoarseElements = dict([(2, 2), (5, 1)])
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 2;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    long iCoarseCell = 1;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);


    assert(4 == indCoarseElement);

    assert(dict_Coarse_Cells.size() == 4);
    assert(dict_Coarse_Cells[0].size() == 2);
    assert(dict_Coarse_Cells[1].size() == 1);
    assert(dict_Coarse_Cells[2].size() == 5);
    assert(dict_Coarse_Cells[3].size() == 1);

    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(2) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);

    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(3) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);
    assert(dict_Coarse_Cells[2].count(7) == 1);

    assert(dict_Coarse_Cells[3].count(8) == 1);

//    self.assertEqual(dict([(1, {1, 3}), (5, {3}), (2, {0})]), dict_Card_Coarse_Cells)

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 2);
    assert(dict_Card_Coarse_Cells[2].size() == 1);
    assert(dict_Card_Coarse_Cells[5].size() == 1);

    assert(dict_Card_Coarse_Cells[1].count(1) == 1);
    assert(dict_Card_Coarse_Cells[1].count(3) == 1);

    assert(dict_Card_Coarse_Cells[2].count(0) == 1);
    assert(dict_Card_Coarse_Cells[5].count(2) == 1);


//  self.assertEqual({1: 2, 2: 1, 5: 1}, dict_DistributionOfCardinalOfCoarseElements)
    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 1);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 2, 2, 2, 1, 2, 3};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

//    np.testing.assert_almost_equal(np.array([True, True, True, True, True, True, True, True, True]),
//                                   isFineCellAgglomerated)
//    np.testing.assert_almost_equal(np.array([0, 3, 4, 3, 3, 3, 2, 3, 1]), fine_Cell_indices_To_Coarse_Cell_Indices[0])
    iCoarseCell = 0;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(5 == indCoarseElement);

//    self.assertEqual({0: [2], 1: [8], 2: [1, 3, 4, 5, 7], 3: [6], 4:[0]}, dict_Coarse_Cells)
    assert(dict_Coarse_Cells.size() == 5);
    assert(dict_Coarse_Cells[0].size() == 1);
    assert(dict_Coarse_Cells[1].size() == 1);
    assert(dict_Coarse_Cells[2].size() == 5);
    assert(dict_Coarse_Cells[3].size() == 1);
    assert(dict_Coarse_Cells[4].size() == 1);

    assert(dict_Coarse_Cells[0].count(0) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);

    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(3) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);
    assert(dict_Coarse_Cells[2].count(7) == 1);

    assert(dict_Coarse_Cells[3].count(8) == 1);
    assert(dict_Coarse_Cells[4].count(2) == 1);

//   self.assertEqual(dict([(1, {1, 3, 0, 4}), (5, {3})]), dict_Card_Coarse_Cells)

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 4);
    assert(dict_Card_Coarse_Cells[5].size() == 1);

    assert(dict_Card_Coarse_Cells[1].count(0) == 1);
    assert(dict_Card_Coarse_Cells[1].count(1) == 1);
    assert(dict_Card_Coarse_Cells[1].count(3) == 1);
    assert(dict_Card_Coarse_Cells[1].count(4) == 1);

    assert(dict_Card_Coarse_Cells[5].count(2) == 1);


//  self.assertEqual({1: 4, 5: 1}, dict_DistributionOfCardinalOfCoarseElements)
    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 4);
    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 1);

    bool ref_isFineCellAgglomerated_tmp_2[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[9] = {0, 2, 4, 2, 2, 2, 1, 2, 3};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated[i] == ref_isFineCellAgglomerated_tmp_2[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}


int test_splitNonConnectedCoarseCell_9_Squares_3() {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};

    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 1, 2, 0, 1, 2, 1};

    long indCoarseElement = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???


//  dict_Coarse_Cells = dict([(0, [0, 2, 5]), (1, [3, 6, 8]), (2, [1, 4, 7])])
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 2, 5});
    dict_Coarse_Cells[1] = unordered_set<long>({3, 6, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});

//  dict_Card_Coarse_Cells = dict([(3, {0, 1, 2})])
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2});

//  dict_DistributionOfCardinalOfCoarseElements = dict([(3, 3)])
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 3;

    long iCoarseCell = 1;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);


    assert(4 == indCoarseElement);


//    self.assertEqual({0: [0, 2, 5], 1: [3, 6], 2: [1, 4, 7], 3: [8]}, dict_Coarse_Cells)
    assert(dict_Coarse_Cells.size() == 4);
    assert(dict_Coarse_Cells[0].size() == 3);
    assert(dict_Coarse_Cells[1].size() == 2);
    assert(dict_Coarse_Cells[2].size() == 3);
    assert(dict_Coarse_Cells[3].size() == 1);

    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(2) == 1);
    assert(dict_Coarse_Cells[0].count(5) == 1);

    assert(dict_Coarse_Cells[1].count(3) == 1);
    assert(dict_Coarse_Cells[1].count(6) == 1);

    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(7) == 1);

    assert(dict_Coarse_Cells[3].count(8) == 1);

//  self.assertEqual(dict([(1, {3}), (3, {0, 2}), (2, {1})]), dict_Card_Coarse_Cells)

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[3].size() == 2);
    assert(dict_Card_Coarse_Cells[2].size() == 1);

    assert(dict_Card_Coarse_Cells[1].count(3) == 1);

    assert(dict_Card_Coarse_Cells[2].count(1) == 1);

    assert(dict_Card_Coarse_Cells[3].count(0) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);


//  self.assertEqual({1: 1, 2: 1, 3: 2}, dict_DistributionOfCardinalOfCoarseElements)

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 1, 2, 0, 1, 2, 3};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_splitNonConnectedCoarseCell_9_Squares_5() {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};

    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 1, 2, 1, 0, 0, 1};

    long indCoarseElement = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???



    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 2, 6, 7});
    dict_Coarse_Cells[1] = unordered_set<long>({3, 5, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4});

//  dict_Card_Coarse_Cells = dict([(3, {0, 1, 2})])
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[2] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({0});

//  dict_DistributionOfCardinalOfCoarseElements = dict([(3, 3)])
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long iCoarseCell = 0;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);


    assert(5 == indCoarseElement);


    assert(dict_Coarse_Cells.size() == 5);
    assert(dict_Coarse_Cells[0].size() == 2);
    assert(dict_Coarse_Cells[1].size() == 3);
    assert(dict_Coarse_Cells[2].size() == 2);
    assert(dict_Coarse_Cells[3].size() == 1);
    assert(dict_Coarse_Cells[4].size() == 1);

    assert(dict_Coarse_Cells[0].count(6) == 1);
    assert(dict_Coarse_Cells[0].count(7) == 1);

    assert(dict_Coarse_Cells[1].count(3) == 1);
    assert(dict_Coarse_Cells[1].count(5) == 1);
    assert(dict_Coarse_Cells[1].count(8) == 1);

    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);

    assert(dict_Coarse_Cells[3].count(0) == 1);

    assert(dict_Coarse_Cells[4].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 2);
    assert(dict_Card_Coarse_Cells[3].size() == 2);
    assert(dict_Card_Coarse_Cells[2].size() == 1);

    assert(dict_Card_Coarse_Cells[1].count(3) == 1);
    assert(dict_Card_Coarse_Cells[1].count(4) == 1);

    assert(dict_Card_Coarse_Cells[2].count(0) == 1);
    assert(dict_Card_Coarse_Cells[2].count(2) == 1);

    assert(dict_Card_Coarse_Cells[3].count(1) == 1);


//  self.assertEqual({1: 1, 2: 1, 3: 2}, dict_DistributionOfCardinalOfCoarseElements)

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {3, 2, 4, 1, 2, 1, 0, 0, 1};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_removeDeletedCoarseCells_Case_1() {


    long numberOfCoarseCells = 2;
    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
//    dict_Card_Coarse_Cells = {1: [0], 3: [1]}
    long fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};
    unordered_set<long> set_removedCoarseCells;

    removeDeletedCoarseCells_v3(dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                fine_Cell_indices_To_Coarse_Cell_Indices, set_removedCoarseCells, numberOfCoarseCells);
    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);
    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);
//self.assertEqual({0: [3], 1: [1, 2, 0]}, dict_Coarse_Elem)
//    self.assertEqual({1: [0], 3: [1]}, dict_Card_Coarse_Cells)
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);

    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};
    for (int i = 0; i < 4; i++) {
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_removeDeletedCoarseCells_Deletion_of_Coarse_Element() {


    long numberOfCoarseCells = 2;
    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});

    long fine_Cell_indices_To_Coarse_Cell_Indices[3] = {1, 1, 1};
    unordered_set<long> set_removedCoarseCells({0});

    removeDeletedCoarseCells_v3(dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                fine_Cell_indices_To_Coarse_Cell_Indices, set_removedCoarseCells, numberOfCoarseCells);
    assert(numberOfCoarseCells == 1);
    assert(dict_Coarse_Elem.size() == 1);
    assert(dict_Coarse_Elem[0].size() == 3);
    assert(dict_Coarse_Elem[0].count(0) == 1);
    assert(dict_Coarse_Elem[0].count(1) == 1);
    assert(dict_Coarse_Elem[0].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(0) == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_removeDeletedCoarseCells_Deletion_of_Coarse_Element_Case_2() {


    long numberOfCoarseCells = 8;
    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[1] = unordered_set<long>({0});
    dict_Coarse_Elem[3] = unordered_set<long>({1});
    dict_Coarse_Elem[4] = unordered_set<long>({2});
    dict_Coarse_Elem[6] = unordered_set<long>({3});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({1, 3, 4, 6});

    long fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 3, 4, 6};
    unordered_set<long> set_removedCoarseCells({0, 2, 5, 7});

    removeDeletedCoarseCells_v3(dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                fine_Cell_indices_To_Coarse_Cell_Indices, set_removedCoarseCells, numberOfCoarseCells);
    assert(numberOfCoarseCells == 4);
    assert(dict_Coarse_Elem.size() == 4);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(0) == 1);

    assert(dict_Coarse_Elem[1].size() == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);

    assert(dict_Coarse_Elem[2].size() == 1);
    assert(dict_Coarse_Elem[2].count(2) == 1);

    assert(dict_Coarse_Elem[3].size() == 1);
    assert(dict_Coarse_Elem[3].count(3) == 1);


    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[1].size() == 4);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);
    assert(dict_Card_Coarse_Cells[1].count(1) == 1);
    assert(dict_Card_Coarse_Cells[1].count(2) == 1);
    assert(dict_Card_Coarse_Cells[1].count(3) == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_removeDeletedCoarseCells_Deletion_of_Coarse_Element_Case_3() {


    long numberOfCoarseCells = 7;
    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[1] = unordered_set<long>({0, 4, 9});
    dict_Coarse_Elem[3] = unordered_set<long>({3, 5, 7});
    dict_Coarse_Elem[4] = unordered_set<long>({2, 6, 8});
    dict_Coarse_Elem[6] = unordered_set<long>({1, 10});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({6});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1, 3, 4});

    long fine_Cell_indices_To_Coarse_Cell_Indices[11] = {1, 6, 4, 3, 1, 3, 4, 3, 4, 1, 6};
    unordered_set<long> set_removedCoarseCells({0, 2, 5});

    removeDeletedCoarseCells_v3(dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                fine_Cell_indices_To_Coarse_Cell_Indices, set_removedCoarseCells, numberOfCoarseCells);

    assert(numberOfCoarseCells == 4);
    assert(dict_Coarse_Elem.size() == 4);
    assert(dict_Coarse_Elem[0].size() == 3);
    assert(dict_Coarse_Elem[0].count(0) == 1);
    assert(dict_Coarse_Elem[0].count(4) == 1);
    assert(dict_Coarse_Elem[0].count(9) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(3) == 1);
    assert(dict_Coarse_Elem[1].count(5) == 1);
    assert(dict_Coarse_Elem[1].count(7) == 1);

    assert(dict_Coarse_Elem[2].size() == 3);
    assert(dict_Coarse_Elem[2].count(2) == 1);
    assert(dict_Coarse_Elem[2].count(6) == 1);
    assert(dict_Coarse_Elem[2].count(8) == 1);

    assert(dict_Coarse_Elem[3].size() == 2);
    assert(dict_Coarse_Elem[3].count(1) == 1);
    assert(dict_Coarse_Elem[3].count(10) == 1);


    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[2].size() == 1);
    assert(dict_Card_Coarse_Cells[2].count(3) == 1);

    assert(dict_Card_Coarse_Cells[3].size() == 3);
    assert(dict_Card_Coarse_Cells[3].count(0) == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[11] = {0, 3, 2, 1, 0, 1, 2, 1, 2, 0, 3};
    for (int i = 0; i < 11; i++) {
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_False() {
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

    for (auto i:dict_Neighbours_Of_Seed) {
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual({1: 1, 2: 2, 3: 1, 4: 2, 5: 3, 6: 2, 7: 3}, dict_Neighbours_Of_Seed)
//    self.assertEqual(4, maxOrderOfNeighbourhood)
    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
    cout << "\nCall of choice_Of_Agglomerated_Cells()" << endl;
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

    assert(set_of_fine_cells_for_Current_Coarse_Cell.size() == 4);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(0) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(1) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(3) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(4) == 1);

    assert(numberOfFineAgglomeratedCells_tmp == 4);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, false, true, true, false, false, false, false};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
    }
    cout << "dict_Neighbours_Of_Seed.size() " << dict_Neighbours_Of_Seed.size() << endl;
    assert(dict_Neighbours_Of_Seed.size() == 4);
    assert(dict_Neighbours_Of_Seed[2] == 2);
    assert(dict_Neighbours_Of_Seed[5] == 3);
    assert(dict_Neighbours_Of_Seed[6] == 2);
    assert(dict_Neighbours_Of_Seed[7] == 3);
    assert(listOfSeeds.size() == 4);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());
    assert(!listOfSeeds[2].empty());
    assert(listOfSeeds[2].front() == 2);
    assert(listOfSeeds[2].back() == 6);
    assert(listOfSeeds[3].empty());
    return 1;
}


int test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True() {
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

    for (auto i:dict_Neighbours_Of_Seed) {
        assert(ref[i.first] == i.second);
    }

    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
//    cout<<"\nCall of choice_Of_Agglomerated_Cells()"<<endl;
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

    assert(set_of_fine_cells_for_Current_Coarse_Cell.size() == 4);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(0) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(1) == 1);
//    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(2)==1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(3) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(6) == 1);
    //0,1, 2, 3 est ok aussi!

    assert(numberOfFineAgglomeratedCells_tmp == 4);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, false, true, false, false, true, false, false};
    for (int i = 0; i < 9; i++) {
        cout << i << " " << isFineCellAgglomerated_tmp[i] << endl;
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
    }

    assert(dict_Neighbours_Of_Seed.size() == 4);
    assert(dict_Neighbours_Of_Seed[4] == 2);
    assert(dict_Neighbours_Of_Seed[5] == 3);
    assert(dict_Neighbours_Of_Seed[2] == 2);
    assert(dict_Neighbours_Of_Seed[7] == 3);


    assert(listOfSeeds.size() == 4);
    assert(!listOfSeeds[0].empty());
    assert(listOfSeeds[0].front() == 4);
    assert(listOfSeeds[1].empty());
    assert(!listOfSeeds[2].empty());
    assert(listOfSeeds[2].front() == 2);
    assert(listOfSeeds[3].empty());
    return 1;
}


int test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_False_GoalCard_8_neighbours_4() {
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

    int iLevel = 1;
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


    for (auto i:dict_Neighbours_Of_Seed) {
        cout << i.first << " " << i.second << endl;
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual({1: 1, 2: 2, 3: 1, 4: 2, 5: 3, 6: 2, 7: 3}, dict_Neighbours_Of_Seed)
//    self.assertEqual(4, maxOrderOfNeighbourhood)
    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
    cout << "\nCall of choice_Of_Agglomerated_Cells()" << endl;
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

    assert(set_of_fine_cells_for_Current_Coarse_Cell.size() == 8);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(0) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(1) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(2) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(3) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(4) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(5) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(6) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(7) == 1);

    assert(numberOfFineAgglomeratedCells_tmp == 8);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, false};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
    }
//    cout<<"dict_Neighbours_Of_Seed.size() "<<dict_Neighbours_Of_Seed.size()<<endl;

    assert(dict_Neighbours_Of_Seed.size() == 1);
    assert(dict_Neighbours_Of_Seed[8] == 4);

    assert(listOfSeeds.size() == 4);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());

    assert(!listOfSeeds[2].empty());
    assert(listOfSeeds[2].front() == 8);
    assert(listOfSeeds[3].empty());
    return 1;
}


int test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True_GoalCard_8_neighbours_4() {
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

    int iLevel = 1;
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
    for (auto i:dict_Neighbours_Of_Seed) {
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual({1: 1, 2: 2, 3: 1, 4: 2, 5: 3, 6: 2, 7: 3}, dict_Neighbours_Of_Seed)
//    self.assertEqual(4, maxOrderOfNeighbourhood)
    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
    cout << "\nCall of choice_Of_Agglomerated_Cells()" << endl;
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

    assert(set_of_fine_cells_for_Current_Coarse_Cell.size() == 8);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(0) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(1) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(2) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(3) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(4) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(5) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(6) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(7) == 1);

    assert(numberOfFineAgglomeratedCells_tmp == 8);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, false};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
    }
    cout << "dict_Neighbours_Of_Seed.size() " << dict_Neighbours_Of_Seed.size() << endl;

    assert(dict_Neighbours_Of_Seed.size() == 1);
    assert(dict_Neighbours_Of_Seed[8] == 4);

    assert(listOfSeeds.size() == 4);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());

    assert(!listOfSeeds[2].empty());
    assert(listOfSeeds[2].front() == 8);
    assert(listOfSeeds[3].empty());
    return 1;
}


int test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True_GoalCard_8() {
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

    int iLevel = 1;
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
    for (auto i:dict_Neighbours_Of_Seed) {
        assert(ref[i.first] == i.second);
    }
//    self.assertEqual({1: 1, 2: 2, 3: 1, 4: 2, 5: 3, 6: 2, 7: 3}, dict_Neighbours_Of_Seed)
//    self.assertEqual(4, maxOrderOfNeighbourhood)
    double volumes[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[9] = {2, 1, 2, 1, 0, 1, 2, 1, 2};

    long numberOfFineAgglomeratedCells_tmp = 0;
//    cout<<"\nCall of choice_Of_Agglomerated_Cells()"<<endl;
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

    assert(set_of_fine_cells_for_Current_Coarse_Cell.size() == 8);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(0) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(1) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(2) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(3) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(4) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(5) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(6) == 1);
    assert(set_of_fine_cells_for_Current_Coarse_Cell.count(7) == 1);

    assert(numberOfFineAgglomeratedCells_tmp == 8);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, false};
    for (int i = 0; i < 9; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
    }
//    cout<<"dict_Neighbours_Of_Seed.size() "<<dict_Neighbours_Of_Seed.size()<<endl;

    assert(dict_Neighbours_Of_Seed.size() == 0);

    assert(listOfSeeds.size() == 4);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());

    assert(!listOfSeeds[2].empty());
    assert(listOfSeeds[2].front() == 8);
    assert(listOfSeeds[3].empty());
    return 1;
}


int test_Agglomerate_Isotropic_createCoarseCell_Case_1() {

    int iLevel = 0;
    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    list<unordered_set<long>> delayedCoarseCells;

    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells);
    assert(delayedCoarseCells.empty());

    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Coarse_Elem[2].size() == 4);
    assert(dict_Coarse_Elem[2].count(4) == 1);
    assert(dict_Coarse_Elem[2].count(5) == 1);
    assert(dict_Coarse_Elem[2].count(6) == 1);
    assert(dict_Coarse_Elem[2].count(7) == 1);
//    self.assertEqual({0: [3], 1: [1, 2, 0], 2: [4, 5, 6, 7]}, dict_Coarse_Elem)

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_Card_Coarse_Cells[4].size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

//    self.assertEqual({1: {0}, 3: {1}, 4: {2}}, dict_Card_Coarse_Cells)
    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_Agglomerate_Isotropic_createCoarseCell_Case_1_isMutable_False() {
// Ex case 2
    int iLevel = 0;
    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    list<unordered_set<long>> delayedCoarseCells;
    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells,
                                           false);

    assert(delayedCoarseCells.empty());

    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_Agglomerate_Isotropic_createCoarseCell_Case_3() {

    unordered_set<long> l({4, 5, 6});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    list<unordered_set<long>> delayedCoarseCells;

    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells);
    assert(delayedCoarseCells.empty());

    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 7);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Coarse_Elem[2].size() == 3);
    assert(dict_Coarse_Elem[2].count(4) == 1);
    assert(dict_Coarse_Elem[2].count(5) == 1);
    assert(dict_Coarse_Elem[2].count(6) == 1);
//    assert(dict_Coarse_Elem[2].count(7) == 1);
//    self.assertEqual({0: [3], 1: [1, 2, 0], 2: [4, 5, 6, 7]}, dict_Coarse_Elem)

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);

//    self.assertEqual({1: {0}, 3: {1}, 4: {2}}, dict_Card_Coarse_Cells)
    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);


//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_1() {

    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    list<unordered_set<long>> delayedCoarseCells;
    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells,
                                           true, true);
//    cout<<"after agglomerate_Isotropic_createCoarseCell"<<endl;

    assert(indCoarseElement == 2);

    assert(numberOfFineAgglomeratedCells_tmp == 8);
    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    assert(!delayedCoarseCells.empty());
    assert(delayedCoarseCells.front().size() == 4);
//    for (auto i:delayedCoarseCells.front())
//    {
//        cout<<i<<", ";
//    }
//    cout<<endl;
    assert(delayedCoarseCells.front().count(4) == 1);
    assert(delayedCoarseCells.front().count(5) == 1);
    assert(delayedCoarseCells.front().count(6) == 1);
    assert(delayedCoarseCells.front().count(7) == 1);

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                             indCoarseElement, fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Coarse_Elem[2].size() == 4);
    assert(dict_Coarse_Elem[2].count(4) == 1);
    assert(dict_Coarse_Elem[2].count(5) == 1);
    assert(dict_Coarse_Elem[2].count(6) == 1);
    assert(dict_Coarse_Elem[2].count(7) == 1);

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_Card_Coarse_Cells[4].size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_2[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}


int test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_1_isMutable_False() {

    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    list<unordered_set<long>> delayedCoarseCells;
    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells,
                                           false, true);
    assert(!delayedCoarseCells.empty());
    assert(delayedCoarseCells.front().size() == 4);
    assert(delayedCoarseCells.front().count(4) == 1);
    assert(delayedCoarseCells.front().count(5) == 1);
    assert(delayedCoarseCells.front().count(6) == 1);
    assert(delayedCoarseCells.front().count(7) == 1);


    assert(indCoarseElement == 2);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 8);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);

    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_Card_Coarse_Cells[4].size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_2[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}


int test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_3() {

    unordered_set<long> l({4, 5, 6});

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    long indCoarseElement = 2;
    long numberOfFineAgglomeratedCells_tmp = 4;
    bool isFineCellAgglomerated_tmp[8] = {true, true, true, true, false, false, false, false};

    list<unordered_set<long>> delayedCoarseCells;
    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells,
                                           true, true);

    assert(!delayedCoarseCells.empty());
    assert(delayedCoarseCells.front().size() == 3);
    assert(delayedCoarseCells.front().count(4) == 1);
    assert(delayedCoarseCells.front().count(5) == 1);
    assert(delayedCoarseCells.front().count(6) == 1);

    assert(indCoarseElement == 2);
    assert(numberOfFineAgglomeratedCells_tmp == 7);

    assert(dict_Coarse_Elem.size() == 2);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);


    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);


//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);
    assert(indCoarseElement == 3);
    assert(numberOfFineAgglomeratedCells_tmp == 7);

    assert(dict_Coarse_Elem.size() == 3);
    assert(dict_Coarse_Elem[0].size() == 1);
    assert(dict_Coarse_Elem[0].count(3) == 1);

    assert(dict_Coarse_Elem[1].size() == 3);
    assert(dict_Coarse_Elem[1].count(0) == 1);
    assert(dict_Coarse_Elem[1].count(1) == 1);
    assert(dict_Coarse_Elem[1].count(2) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].size() == 1);
    assert(dict_Card_Coarse_Cells[1].count(0) == 1);

    assert(dict_Card_Coarse_Cells[3].size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);


    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);


//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, -1};
    for (int i = 0; i < 8; i++) {
        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_2[i]);
        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}


int test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_0() {

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

    assert(seed == 0);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());
    assert(listOfSeeds[2].empty());
    assert(!listOfSeeds[3].empty());
    assert(listOfSeeds[3].size() == 7);
    assert(listOfSeeds[3].front() == 2);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 6);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 8);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 18);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 20);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 24);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 26);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].empty());

    delete[] isFineCellAgglomerated;
    return 1;
}

int test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_2() {

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

    assert(seed == 2);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());
    assert(listOfSeeds[2].empty());
    assert(!listOfSeeds[3].empty());
    assert(listOfSeeds[3].size() == 6);
    assert(listOfSeeds[3].front() == 6);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 8);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 18);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 20);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 24);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].front() == 26);
    listOfSeeds[3].pop();
    assert(listOfSeeds[3].empty());

    delete[] isFineCellAgglomerated;
    return 1;
}


int test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Ridge_1() {

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

    long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated, isOnRidge,
                                                     isOnValley);
//    cout<<"seed "<<seed<<endl;
    assert(seed == 25);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());
    assert(listOfSeeds[2].empty());
    assert(listOfSeeds[3].empty());
    delete[] isFineCellAgglomerated;
    return 1;
}

int test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Ridge_1_V2() {

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

    assert(seed == 1);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());
    assert(listOfSeeds[2].empty());
    assert(listOfSeeds[2].empty());
    delete[] isFineCellAgglomerated;
    return 1;
}

int test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Valley_4() {

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

    assert(seed == 4);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());
    assert(listOfSeeds[2].empty());
    assert(listOfSeeds[2].empty());
    delete[] isFineCellAgglomerated;
    return 1;
}

int test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Valley_10() {

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

    assert(seed == 10);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());
    assert(listOfSeeds[2].empty());
    assert(listOfSeeds[2].empty());
    delete[] isFineCellAgglomerated;
    return 1;
}

int test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Valley_13() {

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

    assert(seed == 13);
    assert(listOfSeeds[0].empty());
    assert(listOfSeeds[1].empty());
    assert(listOfSeeds[2].empty());
    assert(listOfSeeds[2].empty());
    delete[] isFineCellAgglomerated;
    return 1;
}


int test_remove_Too_Small_Cells_v2_12_Squares_8_isotropicCell_1_anisotropic() {
    //"""
    //9 squares (3*3)
    //4 is anisotropic
    //The biggest face area is between 1 and 4
    //isotropic fine cell 4 is added to anisotropic cell 2.
    //Coarse cell 6 is remove.
    //:return:
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 4, 0, 6, 4, 1, 3, 5, 1, 3, 5};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;// wrong value in 2D but for test!
    long indCoarseCell = 7;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[6] = unordered_set<long>({4});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[1] = unordered_set<long>({6});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;

    remove_Too_Small_Cells_v2(thresholdCard,
                              fineCellIndicesToCoarseCellIndices,
                              indCoarseCell,
                              row_ptr, col_ind,
                              values,
                              dict_Coarse_Cells,
                              dict_Card_Coarse_Cells,
                              dict_DistributionOfCardinalOfCoarseElements);
    assert(indCoarseCell == 6);
    assert(dict_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements.empty());

    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 4, 0, 2, 4, 1, 3, 5, 1, 3, 5};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}


int test_remove_Too_Small_Cells_v2_12_Squares_8_isotropicCell_1_anisotropic_case_2() {
    //"""
    //9 squares (3*3)
    //4 is anisotropic
    //The biggest face area is between 1 and 4
    //isotropic fine cell 4 is added to anisotropic cell 2.
    //Coarse cell 6 is remove.
    //:return:
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1.2, 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1.2, 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 4, 0, 6, 4, 1, 3, 5, 1, 3, 5};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;// wrong value in 2D but for test!
    long indCoarseCell = 7;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[6] = unordered_set<long>({4});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[1] = unordered_set<long>({6});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;

    remove_Too_Small_Cells_v2(thresholdCard,
                              fineCellIndicesToCoarseCellIndices,
                              indCoarseCell,
                              row_ptr, col_ind,
                              values,
                              dict_Coarse_Cells,
                              dict_Card_Coarse_Cells,
                              dict_DistributionOfCardinalOfCoarseElements);
    assert(indCoarseCell == 6);
    assert(dict_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements.empty());

    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 4, 0, 3, 4, 1, 3, 5, 1, 3, 5};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}


int test_remove_Too_Small_Cells_v2_12_Squares_9_isotropicCell() {
    //"""
    //9 squares (3*3)
    //4 is anisotropic
    //The biggest face area is between 1 and 4
    //isotropic fine cell 4 is added to anisotropic cell 2.
    //Coarse cell 6 is remove.
    //:return:
    //"""
    cout << "\n\n\n" << endl;
    cout << "============================>" << endl;
    cout << "\n\n\n" << endl;
    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 4,
                                                   0, 5, 4,
                                                   1, 3, 2,
                                                   1, 3, 2};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;// wrong value in 2D but for test!
    long indCoarseCell = 6;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 9});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 8, 11});
    dict_Coarse_Cells[3] = unordered_set<long>({7, 10});
    dict_Coarse_Cells[4] = unordered_set<long>({2, 5});
    dict_Coarse_Cells[5] = unordered_set<long>({4});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[1] = unordered_set<long>({5});
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 1, 3, 4});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[2] = 4;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    remove_Too_Small_Cells_v2(thresholdCard,
                              fineCellIndicesToCoarseCellIndices,
                              indCoarseCell,
                              row_ptr, col_ind,
                              values,
                              dict_Coarse_Cells,
                              dict_Card_Coarse_Cells,
                              dict_DistributionOfCardinalOfCoarseElements);
    assert(indCoarseCell == 2);
    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 2);
    assert(dict_Coarse_Cells.count(0) == 1);
    assert(dict_Coarse_Cells[0].size() == 6);

//    for(auto i:dict_Coarse_Cells[0]){
//        cout<<i<<", "<<endl;
//    }
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(2) == 1);
    assert(dict_Coarse_Cells[0].count(5) == 1);
    assert(dict_Coarse_Cells[0].count(8) == 1);
    assert(dict_Coarse_Cells[0].count(11) == 1);

    assert(dict_Coarse_Cells.count(1) == 1);
    assert(dict_Coarse_Cells[1].size() == 6);

    assert(dict_Coarse_Cells[1].count(3) == 1);
    assert(dict_Coarse_Cells[1].count(4) == 1);
    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(9) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[6] == 2);

    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}

int test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_0_anisotropic() {

//    cout<<"call of test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_0_anisotropic"<<endl;
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    unordered_set<long> listInitialCoarseCell({0, 1, 2, 3, 4, 5, 6, 7, 8});
    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fineCellIndicesToCoarseCellIndices[9] = {1, 0, 3, 1, 2, 3, 1, 3, 3};


    int thresholdCard = 2;  // wrong valu in 2D but for test!
    long indCoarseCell = 4;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[2] = unordered_set<long>({4});
    dict_Coarse_Cells[3] = unordered_set<long>({2, 5, 7, 8});

//  dict_Card_Coarse_Cells = dict([(1, {2}), (3, {1}), (4, {3})])
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({3});

//  dict_DistributionOfCardinalOfCoarseElements = dict([(1, 1), (3, 1), (4, 1)])
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    assert(3 == indCoarseCell);

    assert(dict_Coarse_Cells.size() == 2);
    assert(dict_Coarse_Cells[1].size() == 3);
    assert(dict_Coarse_Cells[2].size() == 5);

    assert(dict_Coarse_Cells[1].count(0) == 1);
    assert(dict_Coarse_Cells[1].count(3) == 1);
    assert(dict_Coarse_Cells[1].count(6) == 1);

    assert(dict_Coarse_Cells[2].count(2) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);
    assert(dict_Coarse_Cells[2].count(7) == 1);
    assert(dict_Coarse_Cells[2].count(8) == 1);

//    self.assertEqual(dict([(3, {1}), (5, {2})]), dict_Card_Coarse_Cells)
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[5].size() == 1);

    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[5].count(2) == 1);


//    self.assertEqual({3: 1, 5: 1}, dict_DistributionOfCardinalOfCoarseElements)

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 0, 2, 1, 2, 2, 1, 2, 2};
    for (int i = 0; i < 9; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_4() {

//    cout<<"call of test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_0_anisotropic"<<endl;
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    long fineCellIndicesToCoarseCellIndices[9] = {1, 2, 4, 1, 0, 4, 1, 3, 4};


    int thresholdCard = 4;  // wrong valu in 2D but for test!
    long indCoarseCell = 4;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({4});

//  dict_Card_Coarse_Cells = dict([(1, {2}), (3, {1}), (4, {3})])
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

//  dict_DistributionOfCardinalOfCoarseElements = dict([(1, 1), (3, 1), (4, 1)])
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    assert(3 == indCoarseCell);

    assert(dict_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements.empty());

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 2, 4, 1, 3, 4, 1, 3, 4};
    for (int i = 0; i < 9; i++) {
        cout << i << " " << fineCellIndicesToCoarseCellIndices[i] << endl;
        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_splitNonConnectedCoarseCell_9_Squares_Case_3() {

//    cout<<"call of test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_0_anisotropic"<<endl;
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    long fineCellIndicesToCoarseCellIndices[9] = {1, 0, 2, 1, 0, 2, 1, 2, 2};


    int thresholdCard = 2;  // wrong valu in 2D but for test!
    long indCoarseCell = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({1, 4});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    assert(3 == indCoarseCell);

    assert(dict_Coarse_Cells.size() == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);

    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[2].size() == 1);
    assert(dict_Card_Coarse_Cells[2].count(0) == 1);

    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 0, 2, 1, 0, 2, 1, 2, 2};
    for (int i = 0; i < 9; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_splitNonConnectedCoarseCell_9_Squares_Too_small_Cell_size_2() {

//    cout<<"call of test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_0_anisotropic"<<endl;
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    long fineCellIndicesToCoarseCellIndices[9] = {1, 0, 2, 1, 0, 2, 1, 2, 2};


    int thresholdCard = 2;  // wrong valu in 2D but for test!
    long indCoarseCell = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({1, 4});
    dict_Coarse_Cells[1] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 5, 7, 8});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    assert(2 == indCoarseCell);

    assert(dict_Coarse_Cells.size() == 2);
    for (auto iT: dict_Coarse_Cells[0]) {
        cout << iT << ", ";
    }
    cout << endl;
    assert(dict_Coarse_Cells[0].size() == 3);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(6) == 1);

    assert(dict_Coarse_Cells[1].size() == 6);
    assert(dict_Coarse_Cells[1].count(1) == 1);
    assert(dict_Coarse_Cells[1].count(2) == 1);
    assert(dict_Coarse_Cells[1].count(4) == 1);
    assert(dict_Coarse_Cells[1].count(5) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(8) == 1);

    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[3].size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(0) == 1);
    assert(dict_Card_Coarse_Cells[6].size() == 1);
    assert(dict_Card_Coarse_Cells[6].count(1) == 1);


    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[6] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 1, 1, 0, 1, 1, 0, 1, 1};
    for (int i = 0; i < 9; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}


int test_splitNonConnectedCoarseCell_9_Squares_Too_small_Cell_size_2_case_2() {

//    cout<<"call of test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_0_anisotropic"<<endl;
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    long fineCellIndicesToCoarseCellIndices[9] = {2, 1, 0, 2, 1, 0, 2, 0, 0};


    int thresholdCard = 2;  // wrong valu in 2D but for test!
    long indCoarseCell = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({1, 4});
    dict_Coarse_Cells[2] = unordered_set<long>({0, 3, 6});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    assert(2 == indCoarseCell);

    assert(dict_Coarse_Cells.size() == 1);
    for (auto iT: dict_Coarse_Cells[1]) {
        cout << iT << ", ";
    }
    cout << endl;
    assert(dict_Coarse_Cells[1].size() == 5);
    assert(dict_Coarse_Cells[1].count(0) == 1);
    assert(dict_Coarse_Cells[1].count(1) == 1);
    assert(dict_Coarse_Cells[1].count(3) == 1);
    assert(dict_Coarse_Cells[1].count(4) == 1);
    assert(dict_Coarse_Cells[1].count(6) == 1);

    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[5].size() == 1);
    assert(dict_Card_Coarse_Cells[5].count(1) == 1);


    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 1, 0, 1, 1, 0, 1, 0, 0};
    for (int i = 0; i < 9; i++) {
        cout << i << " " << fineCellIndicesToCoarseCellIndices[i] << endl;
        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_makeSmallCellBigger_12_Squares_12_isotropic_Cells() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2, 0, 2, 2, 1, 1, 2, 1, 1, 2};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 3;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 7, 9, 10], 2: [2, 4, 5, 8, 11]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}


    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 8, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);
    assert(indCoarseCell == 3);
    assert(numberOfFineAgglomeratedCells == 12);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(9) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);

    assert(dict_Coarse_Cells[2].count(2) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);
    assert(dict_Coarse_Cells[2].count(8) == 1);
    assert(dict_Coarse_Cells[2].count(11) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(0) == 1);
    assert(dict_Card_Coarse_Cells[4].count(1) == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 3);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 2, 0, 0, 2, 1, 1, 2, 1, 1, 2};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}

int test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_2() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2,
                                                   0, 2, 2,
                                                   1, 3, 3,
                                                   1, 1, 3};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 4;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 7, 9, 10], 2: [2, 4, 5, 8, 11]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}


    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 9, 10});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5});
    dict_Coarse_Cells[3] = unordered_set<long>({7, 8, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2, 3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 4;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);

    assert(indCoarseCell == 4);
    assert(numberOfFineAgglomeratedCells == 12);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 4);

    assert(dict_Coarse_Cells[0].size() == 4);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(6) == 1);


//    for (auto i:dict_Coarse_Cells[3]) {
//        cout<<i<<endl;
//    }
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(9) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);

    assert(dict_Coarse_Cells[2].count(2) == 1);
    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);

    assert(dict_Coarse_Cells[3].count(8) == 1);
    assert(dict_Coarse_Cells[3].count(11) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 3);

//    for (auto i:dict_Card_Coarse_Cells[3]) {
//        cout<<i<<endl;
//    }

    assert(dict_Card_Coarse_Cells[2].count(3) == 1);

    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);

    assert(dict_Card_Coarse_Cells[4].count(0) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 0,
                                                             2, 0, 1, 3, 1,
                                                             1, 3};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}


int test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_3() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2,
                                                   0, 2, 2,
                                                   2, 2, 1,
                                                   1, 1, 1};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 3;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [8, 9, 10, 11], 2: [2, 4, 5, 6, 7]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({8, 9, 10, 11});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 6, 7});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);
//    cout<<"dict_Coarse_Cells[0]: [";
//    for(auto i :dict_Coarse_Cells[0]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"dict_Coarse_Cells[1]: [";
//    for(auto i :dict_Coarse_Cells[1]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"dict_Coarse_Cells[2]: [";
//    for(auto i :dict_Coarse_Cells[2]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    assert(indCoarseCell == 3);
    assert(numberOfFineAgglomeratedCells == 12);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(2) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);

    assert(dict_Coarse_Cells[1].count(8) == 1);
    assert(dict_Coarse_Cells[1].count(9) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);
    assert(dict_Coarse_Cells[1].count(11) == 1);

    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);
    assert(dict_Coarse_Cells[2].count(6) == 1);
    assert(dict_Coarse_Cells[2].count(7) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(0) == 1);
    assert(dict_Card_Coarse_Cells[4].count(1) == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 3);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1};
    for (int i = 0; i < 12; i++) {

//        cout<<i<<" "<<fineCellIndicesToCoarseCellIndices[i]<<endl;
        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}

int test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_4() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 1,
                                                   0, 2, 1,
                                                   0, 2, 1,
                                                   3, 3, 3};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 4;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {2: [1, 4, 7], 3: [9, 10, 11]}
//    dict_Card_Coarse_Cells = {3: {2, 3}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 2}


    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({2, 3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 2;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);


//    cout<<"dict_Coarse_Cells[2]: [";
//    for(auto i :dict_Coarse_Cells[2]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"dict_Coarse_Cells[3]: [";
//    for(auto i :dict_Coarse_Cells[3]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
    assert(indCoarseCell == 4);
    assert(numberOfFineAgglomeratedCells == 12);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 2);

    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(7) == 1);

    assert(dict_Coarse_Cells[3].count(9) == 1);
    assert(dict_Coarse_Cells[3].count(10) == 1);
    assert(dict_Coarse_Cells[3].count(11) == 1);

//    cout<<"dict_Coarse_Cells[3]: [";
//    for(auto i :dict_Coarse_Cells[3]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);
    assert(dict_Card_Coarse_Cells[3].count(3) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);


    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 1, 0, 2, 1, 0, 2, 1, 3, 3, 3};
    for (int i = 0; i < 12; i++) {
//        cout<<i<<", "<<fineCellIndicesToCoarseCellIndices[i]<<endl;
        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
//    exit(1);

    return 1;
}

int test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_5() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 1, 1,
                                                   0, 2, 1,
                                                   0, 2, 1,
                                                   3, 3, 3};

    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 4;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {0: [0, 3, 6], 1: [1, 2, 5, 8], 2: [4, 7], 3: [9, 10, 11]}
//    dict_Card_Coarse_Cells = {2: {2}, 3: {0, 3}, 4: {1}}
//    dict_DistributionOfCardinalOfCoarseElements = {2: 1, 3: 2, 4: 1}


    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 5, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({4, 7});
    dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[2] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 3});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});


    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);
//    cout<<"dict_Coarse_Cells[0]: [";
//    for(auto i :dict_Coarse_Cells[0]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"dict_Coarse_Cells[1]: [";
//    for(auto i :dict_Coarse_Cells[1]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"dict_Coarse_Cells[2]: [";
//    for(auto i :dict_Coarse_Cells[2]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    assert(indCoarseCell == 3);
    assert(numberOfFineAgglomeratedCells == 12);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);
    assert(dict_Coarse_Cells[0].count(6) == 1);

    assert(dict_Coarse_Cells[1].count(1) == 1);
    assert(dict_Coarse_Cells[1].count(2) == 1);
    assert(dict_Coarse_Cells[1].count(5) == 1);
    assert(dict_Coarse_Cells[1].count(8) == 1);

    assert(dict_Coarse_Cells[2].count(7) == 1);
    assert(dict_Coarse_Cells[2].count(9) == 1);
    assert(dict_Coarse_Cells[2].count(10) == 1);
    assert(dict_Coarse_Cells[2].count(11) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[4].count(0) == 1);
    assert(dict_Card_Coarse_Cells[4].count(1) == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 3);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 1, 1, 0, 0, 1, 0, 2, 1, 2, 2, 2};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}


int test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2, 0, 2, 2, 1, 1, 2, 1, 1, 2};

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 7, 9, 10], 2: [2, 4, 5, 8, 11]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}

    long indCoarseCell = 3;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 8, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);

    assert(indCoarseCell == 3);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(9) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);

    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(2) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);
    assert(dict_Coarse_Cells[2].count(8) == 1);
    assert(dict_Coarse_Cells[2].count(11) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[2].count(0) == 1);
    assert(dict_Card_Coarse_Cells[4].count(1) == 1);
    assert(dict_Card_Coarse_Cells[6].count(2) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[6] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 2, 2, 1, 1, 2, 1, 1, 2};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}

int test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells_Case_2() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2,
                                                   0, 2, 2,
                                                   1, 3, 3,
                                                   1, 1, 3};



//            dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 9, 10], 2: [2, 4, 5], 3: [7, 8, 11]}
//            dict_Card_Coarse_Cells = {3: {0, 1, 2, 3}}
//            dict_DistributionOfCardinalOfCoarseElements = {3: 4}

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 7, 9, 10], 2: [2, 4, 5, 8, 11]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}

    long indCoarseCell = 4;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 9, 10});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5});
    dict_Coarse_Cells[3] = unordered_set<long>({7, 8, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2, 3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 4;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);
//    self.assertEqual(4, indCoarseCell)
//    self.assertEqual({0: [0, 3], 1: [6, 9, 10, 7], 2: [2, 4, 5, 1], 3: [8, 11]}, dict_Coarse_Cells)


    assert(indCoarseCell == 4);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 4);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(9) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);

    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(2) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);

    assert(dict_Coarse_Cells[3].count(8) == 1);
    assert(dict_Coarse_Cells[3].count(11) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[2].count(0) == 1);
    assert(dict_Card_Coarse_Cells[2].count(3) == 1);

    assert(dict_Card_Coarse_Cells[4].count(1) == 1);
    assert(dict_Card_Coarse_Cells[4].count(2) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 2);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 2, 2, 1, 1, 3, 1, 1, 3};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}


int test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells_Case_3() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2,
                                                   0, 2, 2,
                                                   2, 2, 1,
                                                   1, 1, 1};

    long indCoarseCell = 3;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({8, 9, 10, 11});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 6, 7});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);
//    cout<<"indCoarseCell "<<indCoarseCell<<endl;
//    for(auto i:dict_Coarse_Cells[0]){
//        cout<<i<<", "<<endl;
//    }
//    cout<<endl;
//    for(auto i:dict_Coarse_Cells[1]){
//        cout<<i<<", "<<endl;
//    }
//    cout<<endl;

    assert(indCoarseCell == 2);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 2);
    assert(dict_Coarse_Cells[0].count(9) == 1);
    assert(dict_Coarse_Cells[0].count(10) == 1);
    assert(dict_Coarse_Cells[0].count(11) == 1);

    assert(dict_Coarse_Cells[1].count(0) == 1);
    assert(dict_Coarse_Cells[1].count(1) == 1);
    assert(dict_Coarse_Cells[1].count(2) == 1);
    assert(dict_Coarse_Cells[1].count(3) == 1);
    assert(dict_Coarse_Cells[1].count(4) == 1);
    assert(dict_Coarse_Cells[1].count(5) == 1);
    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(8) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(0) == 1);
    assert(dict_Card_Coarse_Cells[9].count(1) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[9] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}


int test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells_Case_4() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 1,
                                                   0, 2, 1,
                                                   0, 2, 1,
                                                   3, 3, 3};

    long indCoarseCell = 4;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({2, 3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 2;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);

    assert(indCoarseCell == 4);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 2);
    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(4) == 1);
    assert(dict_Coarse_Cells[2].count(7) == 1);

    assert(dict_Coarse_Cells[3].count(9) == 1);
    assert(dict_Coarse_Cells[3].count(10) == 1);
    assert(dict_Coarse_Cells[3].count(11) == 1);


    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);
    assert(dict_Card_Coarse_Cells[3].count(3) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 1, 0, 2, 1, 0, 2, 1, 3, 3, 3};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}


int test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells_Case_5() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 1, 1,
                                                   0, 2, 1,
                                                   0, 2, 1,
                                                   3, 3, 3};


    long indCoarseCell = 4;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 5, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({4, 7});
    dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[2] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 3});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);


//    self.assertEqual(3, indCoarseCell)
//    self.assertEqual({0: [0, 3, 6], 1: [1, 2, 5, 8, 4, 7], 2: [9, 10, 11]}, dict_Coarse_Cells)
//    self.assertEqual({3: set([0, 2]), 6: set([1])}, dict_Card_Coarse_Cells)
//    self.assertEqual({3: 2, 6: 1}, dict_DistributionOfCardinalOfCoarseElements)
//    np.testing.assert_almost_equal(np.array([True, True, True, True, True, True, True, True, True, True, True, True]),
//                                   isFineCellAgglomerated)
//    np.testing.assert_almost_equal(np.array([0, 1, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2]), fineCellIndicesToCoarseCellIndices[0])

    assert(indCoarseCell == 3);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(6) == 1);

    assert(dict_Coarse_Cells[1].count(1) == 1);
    assert(dict_Coarse_Cells[1].count(2) == 1);
    assert(dict_Coarse_Cells[1].count(4) == 1);
    assert(dict_Coarse_Cells[1].count(5) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(8) == 1);

    assert(dict_Coarse_Cells[2].count(9) == 1);
    assert(dict_Coarse_Cells[2].count(10) == 1);
    assert(dict_Coarse_Cells[2].count(11) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(0) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);
    assert(dict_Card_Coarse_Cells[6].count(1) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[6] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 1, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}

int test_agglomerate_Isotropic_Correction_Too_Big_Cells_12_Squares_12_isotropic_Cells() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 0,
                                                   0, 0, 2,
                                                   3, 1, 1,
                                                   1, 1, 1};

    int goalCard = 4;
    long indCoarseCell = 4;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

    assert(!dict_Card_Coarse_Cells.empty());
//    cout<<"dict_Card_Coarse_Cells.size() "<<dict_Card_Coarse_Cells.size()<<endl;
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[1].count(2) == 1);
    assert(dict_Card_Coarse_Cells[1].count(3) == 1);
    assert(dict_Card_Coarse_Cells[5].count(0) == 1);
    assert(dict_Card_Coarse_Cells[5].count(1) == 1);

//    cout<<"\ndict_Coarse_Cells[0]: [";
//    for(auto i:dict_Coarse_Cells[0]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"\ndict_Coarse_Cells[1]: [";
//    for(auto i:dict_Coarse_Cells[1]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"\ndict_Coarse_Cells[2]: [";
//    for(auto i:dict_Coarse_Cells[2]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"\ndict_Coarse_Cells[3]: [";
//    for(auto i:dict_Coarse_Cells[3]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    agglomerate_Isotropic_Correction_Too_Big_Cells(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                                   row_ptr, col_ind,
                                                   dict_DistributionOfCardinalOfCoarseElements,
                                                   fineCellIndicesToCoarseCellIndices,
                                                   indCoarseCell,
                                                   goalCard, false);

//    self.assertEqual(4, indCoarseCell)
//    self.assertEqual({0: [0, 1, 3, 4], 1: [7, 8, 10, 11], 2: [5, 2], 3: [6, 9]}, dict_Coarse_Cells)
//    self.assertEqual({2: set([2, 3]), 4: set([0, 1])}, dict_Card_Coarse_Cells)
//    self.assertEqual({2: 2, 4: 2}, dict_DistributionOfCardinalOfCoarseElements)
//    np.testing.assert_almost_equal(np.array([]), fineCellIndicesToCoarseCellIndices[0])
//    cout<<"\ndict_Coarse_Cells[0]: [";
//    for(auto i:dict_Coarse_Cells[0]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//
//    cout<<"\ndict_Coarse_Cells[1]: [";
//    for(auto i:dict_Coarse_Cells[1]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"\ndict_Coarse_Cells[2]: [";
//    for(auto i:dict_Coarse_Cells[2]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"\ndict_Coarse_Cells[3]: [";
//    for(auto i:dict_Coarse_Cells[3]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//
//    cout<<"\ndict_Card_Coarse_Cells[2]: [";
//    for(auto i:dict_Card_Coarse_Cells[2]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

//    cout<<"\ndict_Card_Coarse_Cells[3]: [";
//    for(auto i:dict_Card_Coarse_Cells[3]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

//    cout<<"\ndict_Card_Coarse_Cells[4]: [";
//    for(auto i:dict_Card_Coarse_Cells[4]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"Keys dict_Card_Coarse_Cells: [";
//    for(auto iKV:dict_Card_Coarse_Cells)
//    {
//        cout<<iKV.first<<", ";
//    }
//    cout<<"]"<<endl;
    assert(indCoarseCell == 4);

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 4);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);

    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(8) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);
    assert(dict_Coarse_Cells[1].count(11) == 1);

    assert(dict_Coarse_Cells[2].count(5) == 1);
    assert(dict_Coarse_Cells[2].count(2) == 1);

    assert(dict_Coarse_Cells[3].count(6) == 1);
    assert(dict_Coarse_Cells[3].count(9) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
//    cout << "dict_Card_Coarse_Cells.size() " << dict_Card_Coarse_Cells.size() << endl;
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[2].count(2) == 1);
    assert(dict_Card_Coarse_Cells[2].count(3) == 1);
    assert(dict_Card_Coarse_Cells[4].count(0) == 1);
    assert(dict_Card_Coarse_Cells[4].count(1) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 2);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 2, 0, 0, 2, 3, 1, 1, 3, 1, 1};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    return 1;
}

int test_agglomerate_Isotropic_Correction_Too_Big_Cells_12_Squares_12_isotropic_Cells_Case_2() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 1,
                                                   0, 1, 1,
                                                   1, 1, 1,
                                                   2, 2, 1};

    int goalCard = 4;
    long indCoarseCell = 3;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

    agglomerate_Isotropic_Correction_Too_Big_Cells(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                                   row_ptr, col_ind,
                                                   dict_DistributionOfCardinalOfCoarseElements,
                                                   fineCellIndicesToCoarseCellIndices,
                                                   indCoarseCell,
                                                   goalCard, false);

    assert(indCoarseCell == 3);
    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);
    cout << "dict_Coarse_Cells[2]: [";
    for (auto i :dict_Coarse_Cells[2]) {
        cout << i << ", ";
    }
    cout << "]" << endl;
    assert(dict_Coarse_Cells[0].size() == 4);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(6) == 1);


    assert(dict_Coarse_Cells[1].size() == 5);
    assert(dict_Coarse_Cells[1].count(2) == 1);
    assert(dict_Coarse_Cells[1].count(4) == 1);
    assert(dict_Coarse_Cells[1].count(5) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(8) == 1);

    assert(dict_Coarse_Cells[2].count(9) == 1);
    assert(dict_Coarse_Cells[2].count(10) == 1);
    assert(dict_Coarse_Cells[2].count(11) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);
    assert(dict_Card_Coarse_Cells[4].count(0) == 1);
    assert(dict_Card_Coarse_Cells[5].count(1) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2};
    for (int i = 0; i < 12; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
//    exit(1);
    return 1;
}

int test_compute_Dicts_From_FineCellIndicesToCoarseCellIndices() {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 1,
                                                   0, 1, 1,
                                                   1, 1, 1,
                                                   2, 2, 1};

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

//
//    self.assertEqual({0: [0, 1, 3], 1: [2, 4, 5, 6, 7, 8, 11], 2: [9, 10]}, dict_Coarse_Cells)
//    self.assertEqual({3: set([0]), 2: {2}, 7: {1}}, dict_Card_Coarse_Cells)
//    self.assertEqual({2: 1, 3: 1, 7: 1}, dict_DistributionOfCardinalOfCoarseElements)

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);

    assert(dict_Coarse_Cells[1].count(2) == 1);
    assert(dict_Coarse_Cells[1].count(4) == 1);
    assert(dict_Coarse_Cells[1].count(5) == 1);
    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(7) == 1);
    assert(dict_Coarse_Cells[1].count(8) == 1);
    assert(dict_Coarse_Cells[1].count(11) == 1);

    assert(dict_Coarse_Cells[2].count(9) == 1);
    assert(dict_Coarse_Cells[2].count(10) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 3);
    assert(dict_Card_Coarse_Cells[3].count(0) == 1);
    assert(dict_Card_Coarse_Cells[2].count(2) == 1);
    assert(dict_Card_Coarse_Cells[7].count(1) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[7] == 1);

    return 1;
}


int test_agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo_1_cell() {
    // MGridGen Test case
    long matrixAdj_CRS_row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long matrixAdj_CRS_col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                                      7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                                      14};
    double matrixAdj_CRS_values[51] = {6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
                                       6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
                                       2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
                                       3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
                                       3.16227766, 7.};

    int nbCells = 15;
    double volumes[15] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[15] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2};
    long fineCellIndicesToCoarseCellIndices[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>();
    listOfSeeds[3] = queue<long>();

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

    long indCoarseCell = 1;
    int minCard = 3;
    int maxCard = 5;
    bool checks = true;
    bool verbose = true;
    long numberOfFineAgglomeratedCells = nbCells;
    int nbSizes = 2;
    long sizes[2] = {16, 51};
    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    cout << "\ndict_Coarse_Cells[0]: [";
    for (auto i:dict_Coarse_Cells[0]) {
        cout << i << ", ";
    }
    cout << "]" << endl;
    assert(indCoarseCell == 2);
    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 2);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(2) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);
    assert(dict_Coarse_Cells[0].count(5) == 1);
    assert(dict_Coarse_Cells[0].count(7) == 1);
    assert(dict_Coarse_Cells[0].count(8) == 1);
    assert(dict_Coarse_Cells[0].count(9) == 1);
    assert(dict_Coarse_Cells[0].count(10) == 1);
    assert(dict_Coarse_Cells[0].count(12) == 1);
    assert(dict_Coarse_Cells[0].count(13) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(11) == 1);
    assert(dict_Coarse_Cells[1].count(14) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[12].count(0) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[12] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[15] = {0, 0, 0,
                                                             0, 0, 0,
                                                             1, 0, 0,
                                                             0, 0, 1,
                                                             0, 0, 1};
    for (int i = 0; i < 15; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    cout << "\ndict_Coarse_Cells[2]: [";
    for (auto i:dict_Coarse_Cells[2]) {
        cout << i << ", ";
    }
    cout << "]" << endl;
    assert(indCoarseCell == 3);
    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(2) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);
    assert(dict_Coarse_Cells[0].count(5) == 1);
    assert(dict_Coarse_Cells[0].count(8) == 1);
    assert(dict_Coarse_Cells[0].count(10) == 1);
    assert(dict_Coarse_Cells[0].count(13) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(11) == 1);
    assert(dict_Coarse_Cells[1].count(14) == 1);

    assert(dict_Coarse_Cells[2].count(7) == 1);
    assert(dict_Coarse_Cells[2].count(9) == 1);
    assert(dict_Coarse_Cells[2].count(12) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);
    assert(dict_Card_Coarse_Cells[9].count(0) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);
    assert(dict_DistributionOfCardinalOfCoarseElements[9] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[15] = {0, 0, 0,
                                                               0, 0, 0,
                                                               1, 2, 0,
                                                               2, 0, 1,
                                                               2, 0, 1};
    for (int i = 0; i < 15; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }

    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

//    cout<<"\ndict_Coarse_Cells[0]: [";
//    for(auto i:dict_Coarse_Cells[0]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"\ndict_Coarse_Cells[1]: [";
//    for(auto i:dict_Coarse_Cells[1]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//
//    cout<<"\ndict_Coarse_Cells[2]: [";
//    for(auto i:dict_Coarse_Cells[2]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//
//    cout<<"\ndict_Coarse_Cells[3]: [";
//    for(auto i:dict_Coarse_Cells[3]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    assert(indCoarseCell == 4);
    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 4);
    assert(dict_Coarse_Cells[0].count(2) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);
    assert(dict_Coarse_Cells[0].count(5) == 1);
    assert(dict_Coarse_Cells[0].count(8) == 1);
    assert(dict_Coarse_Cells[0].count(10) == 1);
    assert(dict_Coarse_Cells[0].count(13) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(11) == 1);
    assert(dict_Coarse_Cells[1].count(14) == 1);

    assert(dict_Coarse_Cells[2].count(7) == 1);
    assert(dict_Coarse_Cells[2].count(9) == 1);
    assert(dict_Coarse_Cells[2].count(12) == 1);

    assert(dict_Coarse_Cells[3].count(0) == 1);
    assert(dict_Coarse_Cells[3].count(1) == 1);
    assert(dict_Coarse_Cells[3].count(3) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
    assert(dict_Card_Coarse_Cells[3].count(2) == 1);
    assert(dict_Card_Coarse_Cells[3].count(3) == 1);
    assert(dict_Card_Coarse_Cells[6].count(0) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 3);
    assert(dict_DistributionOfCardinalOfCoarseElements[6] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_3[15] = {3, 3, 0,
                                                               3, 0, 0,
                                                               1, 2, 0,
                                                               2, 0, 1,
                                                               2, 0, 1};
    for (int i = 0; i < 15; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_3[i]);
    }
    return 1;

}

int test_agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo_1_cell_v2() {

    // MGridGen Test case
    long matrixAdj_CRS_row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long matrixAdj_CRS_col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                                      7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                                      14};
    double matrixAdj_CRS_values[51] = {6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
                                       6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
                                       2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
                                       3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
                                       3.16227766, 7.};

    int nbCells = 15;
    double volumes[15] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[15] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2};
    long fineCellIndicesToCoarseCellIndices[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>();
    listOfSeeds[3] = queue<long>();

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

    long indCoarseCell = 1;
    int minCard = 5;
    int maxCard = 5;
    bool checks = true;
    bool verbose = false;
    long numberOfFineAgglomeratedCells = nbCells;
    int nbSizes = 2;
    long sizes[2] = {16, 51};
    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    cout << "\ndict_Coarse_Cells[0]: [";
    for (auto i:dict_Coarse_Cells[0]) {
        cout << i << ", ";
    }
    cout << "]" << endl;
    cout << "\ndict_Coarse_Cells[1]: [";
    for (auto i:dict_Coarse_Cells[1]) {
        cout << i << ", ";
    }
    cout << "]" << endl;

    assert(indCoarseCell == 2);
    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 2);
    assert(dict_Coarse_Cells[0].count(0) == 1);
    assert(dict_Coarse_Cells[0].count(1) == 1);
    assert(dict_Coarse_Cells[0].count(2) == 1);
    assert(dict_Coarse_Cells[0].count(3) == 1);
    assert(dict_Coarse_Cells[0].count(4) == 1);
    assert(dict_Coarse_Cells[0].count(5) == 1);
    assert(dict_Coarse_Cells[0].count(7) == 1);
    assert(dict_Coarse_Cells[0].count(8) == 1);
    assert(dict_Coarse_Cells[0].count(9) == 1);
    assert(dict_Coarse_Cells[0].count(12) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);
    assert(dict_Coarse_Cells[1].count(11) == 1);
    assert(dict_Coarse_Cells[1].count(13) == 1);
    assert(dict_Coarse_Cells[1].count(14) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 2);
    assert(dict_Card_Coarse_Cells[5].count(1) == 1);
    assert(dict_Card_Coarse_Cells[10].count(0) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements[10] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[15] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1};
//    cout<<"\nfineCellIndicesToCoarseCellIndices: [";
//    for(auto i:fineCellIndicesToCoarseCellIndices){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    for (int i = 0; i < 15; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    cout << "\ndict_Coarse_Cells[0]: [";
    for (auto i:dict_Coarse_Cells[0]) {
        cout << i << ", ";
    }
    cout << "]" << endl;
    cout << "\ndict_Coarse_Cells[1]: [";
    for (auto i:dict_Coarse_Cells[1]) {
        cout << i << ", ";
    }
    cout << "]" << endl;
    cout << "\ndict_Coarse_Cells[2]: [";
    for (auto i:dict_Coarse_Cells[2]) {
        cout << i << ", ";
    }
    cout << "]" << endl;

    cout << "indCoarseCell " << indCoarseCell << endl;
    assert(indCoarseCell == 3);
    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 3);

    assert(dict_Coarse_Cells[0].count(4) == 1);
    assert(dict_Coarse_Cells[0].count(7) == 1);
    assert(dict_Coarse_Cells[0].count(8) == 1);
    assert(dict_Coarse_Cells[0].count(9) == 1);
    assert(dict_Coarse_Cells[0].count(12) == 1);

    assert(dict_Coarse_Cells[1].count(6) == 1);
    assert(dict_Coarse_Cells[1].count(10) == 1);
    assert(dict_Coarse_Cells[1].count(11) == 1);
    assert(dict_Coarse_Cells[1].count(13) == 1);
    assert(dict_Coarse_Cells[1].count(14) == 1);

    assert(dict_Coarse_Cells[2].count(0) == 1);
    assert(dict_Coarse_Cells[2].count(1) == 1);
    assert(dict_Coarse_Cells[2].count(2) == 1);
    assert(dict_Coarse_Cells[2].count(3) == 1);
    assert(dict_Coarse_Cells[2].count(5) == 1);

    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[5].count(0) == 1);
    assert(dict_Card_Coarse_Cells[5].count(1) == 1);
    assert(dict_Card_Coarse_Cells[5].count(2) == 1);

    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 3);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[15] = {2, 2, 2, 2, 0, 2, 1, 0, 0, 0, 1, 1, 0, 1, 1};

    for (int i = 0; i < 15; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
    }
    return 1;
}


int test_agglomerate_Isotropic_First_Step_Box_5x5x5() {

    // without CGNS
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62, 66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132, 138, 144, 150, 155, 161, 167,
                                  172, 177, 183, 189, 194, 200, 206, 212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278, 282, 287, 293, 299, 304, 309, 315,
                                  321, 326, 330, 335, 340, 344};

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
                                        2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5,
                                        30., 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
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

    double volumes[64] = {12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125};

    bool isFineCellAgglomerated[64];
    for (int i = 0; i < 64; i++) {
        isFineCellAgglomerated[i] = false;
    }
    long fineCellIndicesToCoarseCellIndices[64];
    for (int i = 0; i < 64; i++) {
        fineCellIndicesToCoarseCellIndices[i] = -1;
    }

    int goalCard = 8;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    unordered_set<long> isOnRidge = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    unordered_set<long> isOnValley = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    unordered_set<long> isOnCorner = {0, 3, 12, 15, 48, 51, 60, 63};

    int isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int maxCard = 10;

    int numberOfInts = 4;

    long indCoarseCell = 0;

    long sizes[4] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells};
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];
//    long isOnValley_size = sizes[4];
//    long isOnRidge_size = sizes[5];
//    long isOnCorner_size = sizes[6];

    agglomerate_Isotropic_First_Step(sizes,
                                     adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
                                     dict_DistributionOfCardinalOfCoarseElements,
                                     dict_Coarse_Cells,
                                     dict_Card_Coarse_Cells,
            //numberOfFineCells,
                                     isOnValley,
                                     isOnRidge,
                                     isOnCorner,
//                                     indCoarseCell,
//                                     numberOfFineAgglomeratedCells,
                                     isFineCellAgglomerated,
                                     isOnFineBnd,
                                     fineCellIndicesToCoarseCellIndices,
                                     delayedCoarseCells,
                                     goalCard,
                                     thresholdCard,
                                     maxCard);
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];
    assert(sizes[0] == 64);
    assert(sizes[1] == 344);
    cout << "sizes[2] " << sizes[2] << endl;
    assert(sizes[2] == 8);//indCoarseCell
    assert(sizes[3] == 64);//indCoarseCell
    for (int i_cc = 0; i_cc < 8; i_cc++) {
        cout << "dict_Coarse_Cells[" << i_cc << "]: [";
        for (auto i:dict_Coarse_Cells[i_cc]) {
            cout << i << ", ";
        }
        cout << "]" << endl;
    }

    assert(!dict_Coarse_Cells.empty());
    assert(dict_Coarse_Cells.size() == 8);

    assert(dict_Coarse_Cells[0].count(42) == 1);
    assert(dict_Coarse_Cells[0].count(43) == 1);
    assert(dict_Coarse_Cells[0].count(46) == 1);
    assert(dict_Coarse_Cells[0].count(47) == 1);
    assert(dict_Coarse_Cells[0].count(58) == 1);
    assert(dict_Coarse_Cells[0].count(59) == 1);
    assert(dict_Coarse_Cells[0].count(62) == 1);
    assert(dict_Coarse_Cells[0].count(63) == 1);

    assert(dict_Coarse_Cells[1].count(40) == 1);
    assert(dict_Coarse_Cells[1].count(41) == 1);
    assert(dict_Coarse_Cells[1].count(44) == 1);
    assert(dict_Coarse_Cells[1].count(45) == 1);
    assert(dict_Coarse_Cells[1].count(56) == 1);
    assert(dict_Coarse_Cells[1].count(57) == 1);
    assert(dict_Coarse_Cells[1].count(60) == 1);
    assert(dict_Coarse_Cells[1].count(61) == 1);

    assert(dict_Coarse_Cells[2].count(34) == 1);
    assert(dict_Coarse_Cells[2].count(35) == 1);
    assert(dict_Coarse_Cells[2].count(38) == 1);
    assert(dict_Coarse_Cells[2].count(39) == 1);
    assert(dict_Coarse_Cells[2].count(50) == 1);
    assert(dict_Coarse_Cells[2].count(51) == 1);
    assert(dict_Coarse_Cells[2].count(54) == 1);
    assert(dict_Coarse_Cells[2].count(55) == 1);


    assert(dict_Coarse_Cells[5].count(8) == 1);
    assert(dict_Coarse_Cells[5].count(9) == 1);
    assert(dict_Coarse_Cells[5].count(12) == 1);
    assert(dict_Coarse_Cells[5].count(13) == 1);
    assert(dict_Coarse_Cells[5].count(24) == 1);
    assert(dict_Coarse_Cells[5].count(25) == 1);
    assert(dict_Coarse_Cells[5].count(28) == 1);
    assert(dict_Coarse_Cells[5].count(29) == 1);

    assert(dict_Coarse_Cells[4].count(10) == 1);
    assert(dict_Coarse_Cells[4].count(11) == 1);
    assert(dict_Coarse_Cells[4].count(14) == 1);
    assert(dict_Coarse_Cells[4].count(15) == 1);
    assert(dict_Coarse_Cells[4].count(26) == 1);
    assert(dict_Coarse_Cells[4].count(27) == 1);
    assert(dict_Coarse_Cells[4].count(30) == 1);
    assert(dict_Coarse_Cells[4].count(31) == 1);

    assert(dict_Coarse_Cells[3].count(32) == 1);
    assert(dict_Coarse_Cells[3].count(33) == 1);
    assert(dict_Coarse_Cells[3].count(36) == 1);
    assert(dict_Coarse_Cells[3].count(37) == 1);
    assert(dict_Coarse_Cells[3].count(48) == 1);
    assert(dict_Coarse_Cells[3].count(49) == 1);
    assert(dict_Coarse_Cells[3].count(52) == 1);
    assert(dict_Coarse_Cells[3].count(53) == 1);

    assert(dict_Coarse_Cells[6].count(2) == 1);
    assert(dict_Coarse_Cells[6].count(3) == 1);
    assert(dict_Coarse_Cells[6].count(6) == 1);
    assert(dict_Coarse_Cells[6].count(7) == 1);
    assert(dict_Coarse_Cells[6].count(18) == 1);
    assert(dict_Coarse_Cells[6].count(19) == 1);
    assert(dict_Coarse_Cells[6].count(22) == 1);
    assert(dict_Coarse_Cells[6].count(23) == 1);

    assert(dict_Coarse_Cells[7].count(0) == 1);
    assert(dict_Coarse_Cells[7].count(1) == 1);
    assert(dict_Coarse_Cells[7].count(4) == 1);
    assert(dict_Coarse_Cells[7].count(5) == 1);
    assert(dict_Coarse_Cells[7].count(16) == 1);
    assert(dict_Coarse_Cells[7].count(17) == 1);
    assert(dict_Coarse_Cells[7].count(20) == 1);
    assert(dict_Coarse_Cells[7].count(21) == 1);


    assert(!dict_Card_Coarse_Cells.empty());
    assert(dict_Card_Coarse_Cells.size() == 1);
    assert(dict_Card_Coarse_Cells[8].count(0) == 1);
    assert(dict_Card_Coarse_Cells[8].count(1) == 1);
    assert(dict_Card_Coarse_Cells[8].count(2) == 1);
    assert(dict_Card_Coarse_Cells[8].count(3) == 1);
    assert(dict_Card_Coarse_Cells[8].count(4) == 1);
    assert(dict_Card_Coarse_Cells[8].count(5) == 1);
    assert(dict_Card_Coarse_Cells[8].count(6) == 1);
    assert(dict_Card_Coarse_Cells[8].count(7) == 1);
    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
    assert(dict_DistributionOfCardinalOfCoarseElements[8] == 8);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 5, 5, 4, 4, 7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 5, 5, 4, 4, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1,
                                                             0, 0, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0};
//    cout<<"\nfineCellIndicesToCoarseCellIndices: [";
//    for(auto i:fineCellIndicesToCoarseCellIndices){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    for (int i = 0; i < 64; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_agglomerate_Isotropic_One_Level_v_2_Box_5x5x5() {

    // without CGNS
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62, 66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132, 138, 144, 150, 155, 161, 167,
                                  172, 177, 183, 189, 194, 200, 206, 212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278, 282, 287, 293, 299, 304, 309, 315,
                                  321, 326, 330, 335, 340, 344};

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
                                        2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5,
                                        30., 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
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

    double volumes[64] = {12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125};

    bool isFineCellAgglomerated[64];
    for (int i = 0; i < 64; i++) {
        isFineCellAgglomerated[i] = false;
    }
    long fineCellIndicesToCoarseCellIndices[64];
    for (int i = 0; i < 64; i++) {
        fineCellIndicesToCoarseCellIndices[i] = -1;
    }

    unordered_set<long> isOnRidge = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    unordered_set<long> isOnValley = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    unordered_set<long> isOnCorner = {0, 3, 12, 15, 48, 51, 60, 63};

    int isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int minCard = 6;
    int goalCard = 8;
    int maxCard = 10;

    int numberOfInts = 4;

    long indCoarseCell = 0;

    long sizes[4] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells};

    bool checks = true;
    bool verbose = true;

//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];


    agglomerate_Isotropic_One_Level_v_2(sizes,
                                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                                        fineCellIndicesToCoarseCellIndices,
                                        isFineCellAgglomerated,

                                        isOnValley,
                                        isOnRidge,
                                        isOnCorner,

                                        isOnFineBnd,
                                        minCard,
                                        goalCard,
                                        maxCard,
                                        thresholdCard,
                                        checks,
                                        verbose);
    assert(sizes[0] == 64);
    assert(sizes[1] == 344);
    assert(sizes[2] == 8);//indCoarseCell
    assert(sizes[3] == 64);//numberOfFineAgglomeratedCells



    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 5, 5, 4, 4, 7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 5, 5, 4, 4, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1,
                                                             0, 0, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0};
//    cout<<"\nfineCellIndicesToCoarseCellIndices: [";
//    for(auto i:fineCellIndicesToCoarseCellIndices){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    for (int i = 0; i < 64; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_agglomerate_Isotropic_One_Level_v_2_Box_5x5x5_goalCard_9() {

    // without CGNS
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62, 66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132, 138, 144, 150, 155, 161, 167,
                                  172, 177, 183, 189, 194, 200, 206, 212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278, 282, 287, 293, 299, 304, 309, 315,
                                  321, 326, 330, 335, 340, 344};

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
                                        2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5,
                                        30., 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
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

    double volumes[64] = {12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125};

    bool isFineCellAgglomerated[64];
    for (int i = 0; i < 64; i++) {
        isFineCellAgglomerated[i] = false;
    }
    long fineCellIndicesToCoarseCellIndices[64];
    for (int i = 0; i < 64; i++) {
        fineCellIndicesToCoarseCellIndices[i] = -1;
    }

    unordered_set<long> isOnRidge = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    unordered_set<long> isOnValley = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    unordered_set<long> isOnCorner = {0, 3, 12, 15, 48, 51, 60, 63};

    int isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                           3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int minCard = 6;
    int goalCard = 9;
    int maxCard = 10;

    int numberOfInts = 4;

    long indCoarseCell = 0;

    long sizes[4] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells};

    bool checks = false;
    bool verbose = false;

//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];

    agglomerate_Isotropic_One_Level_v_2(sizes,
                                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                                        fineCellIndicesToCoarseCellIndices,
                                        isFineCellAgglomerated,

                                        isOnValley,
                                        isOnRidge,
                                        isOnCorner,

                                        isOnFineBnd,
                                        minCard,
                                        goalCard,
                                        maxCard,
                                        thresholdCard,
                                        checks,
                                        verbose);
    assert(sizes[0] == 64);
    assert(sizes[1] == 344);
//    cout<<"sizes[2] "<<sizes[2]<<endl;
    assert(sizes[2] == 7);//indCoarseCell
    assert(sizes[3] == 64);//numberOfFineAgglomeratedCells



    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {3, 6, 6, 6, 5, 6, 6, 4, 5, 5, 4, 4, 5, 5, 4, 4, 3, 6, 6, 6, 5, 6, 6, 4, 5, 5, 4, 4, 5, 5, 4, 4, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1,
                                                             0, 0, 3, 3, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0};
//    cout << "\nfineCellIndicesToCoarseCellIndices: [";
//    for (auto i:fineCellIndicesToCoarseCellIndices) {
//        cout << i << ", ";
//    }
//    cout << "]" << endl;

    for (int i = 0; i < 64; i++) {

        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
    return 1;
}

int test_computeAnisotropicLine_1_AnisotropicCell() {

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

//
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
    assert(!isAgglomerationLines);
    cout << "fineAgglomerationLines" << endl;
    long numberOfAnisotropicLinesPOne_size = sizes[8];
    agglomerationLines_size = sizes[9];

    assert(numberOfAnisotropicLinesPOne_size == 2);
    assert(agglomerationLines_size == 0);
    long ref_agglomerationLines_Idx[2] = {0, 0};
//    cout<<"\nfineCellIndicesToCoarseCellIndices: [";
//    for(auto i:fineCellIndicesToCoarseCellIndices){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    for (int i = 0; i < 2; i++) {

        assert(agglomerationLines_Idx[i] == ref_agglomerationLines_Idx[i]);
    }
    delete[]agglomerationLines_Idx;
    delete[]agglomerationLines;
    return 1;
}

int test_computeAnisotropicLine_Box_5x5x5_iso_and_Aniso_MG_1_level() {
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
//    long sizes[5] = {numberOfFineCells, adjMatrix_col_ind_size, numberOfFineAnisotropicCompliantCells,
//                     numberOfAnisotropicLinesPOne_size, agglomerationLines_size};

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
    assert(isAggloLines);
//    cout<<"After call of computeAnisotropicLine"<<endl;
    //agglomerationLines_Idx, agglomerationLines = Util.convertFineAgglomerationLinesToFineAgglomerationLines_arrays(numberOfFineCells, fineAgglomerationLines)
    assert(numberOfFineAnisotropicCompliantCells == 64);
    assert(17 == sizes[8]);
    assert(48 == sizes[9]);
    long ref_agglomerationLines_Idx[17] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48};
    long ref_agglomerationLines[48] = {0, 16, 32,
                                       15, 31, 47,
                                       3, 19, 35,
                                       12, 28, 44, 2, 18, 34, 1, 17, 33, 11, 27, 43, 8, 24, 40, 7, 23, 39, 4, 20, 36, 14, 30, 46, 13, 29, 45, 6, 22, 38, 5, 21, 37, 10, 26, 42, 9, 25, 41};
//    cout << "\nagglomerationLines: [";
//    for (int i_line = 0; i_line<48; i_line ++){
//        cout << agglomerationLines[i_line] << ", ";
//    }
    cout << "]" << endl;
    for (int i = 0; i < 17; i++) {
        assert(ref_agglomerationLines_Idx[i] == agglomerationLines_Idx[i]);
    }

    for (int i = 0; i < 48; i++) {
        cout << agglomerationLines[i] << ", ";
        if ((i + 1) % 3 == 0) {
            cout << endl;
        }
        assert(ref_agglomerationLines[i] == agglomerationLines[i]);
    }
    cout << endl;
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
    return 1;
}


int test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle() {

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

//    long sizes_2[5] = {numberOfFineCells, adjMatrix_col_ind_size_2, numberOfFineAnisotropicCompliantCells_2,
//                       numberOfAnisotropicLinesPOne_size_2, agglomerationLines_size_2};
    long sizes[10] = {numberOfFineCells, adjMatrix_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      numberOfFineAnisotropicCompliantCells, agglomerationLines_Idx_size, agglomerationLines_size};

//    cout<<"Before call of computeAnisotropicLine"<<endl;
    computeAnisotropicLine(sizes,
                           adjMatrix_row_ptr,
                           adjMatrix_col_ind,
                           adjMatrix_areaValues,
                           arrayOfFineAnisotropicCompliantCells,
                           agglomerationLines_Idx,
                           agglomerationLines,
                           verbose);
//    cout<<"After call of computeAnisotropicLine"<<endl;
    //agglomerationLines_Idx, agglomerationLines = Util.convertFineAgglomerationLinesToFineAgglomerationLines_arrays(numberOfFineCells, fineAgglomerationLines)
    assert(numberOfFineAnisotropicCompliantCells == 24);
//    cout<<"sizes_2[3] "<<sizes_2[3]<< endl;
//    cout<<"sizes_2[4] "<<sizes_2[4]<< endl;
    assert(3 == sizes[8]);  // number of anisotropic lines
    assert(24 == sizes[9]);  // number of cells in these lines
    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 24;
    long ref_agglomerationLines_Idx[3] = {0, 12, 24};
    long ref_agglomerationLines[24] = {22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0, 23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
//        cout<<agglomerationLines_Idx_2[i]<<", ";
        assert(ref_agglomerationLines_Idx[i] == agglomerationLines_Idx[i]);
    }
//    cout<<endl;

//    cout << "\nagglomerationLines: [";
//    for (int i_line = 0; i_line<24; i_line ++){
//        cout << agglomerationLines[i_line] << ", ";
//    }
//    cout<<"]"<<endl;

    for (int i = 0; i < ref_agglomerationLines_size; i++) {
//        cout<<agglomerationLines_2[i]<<", ";
//        if ((i+1)%12==0)
//        {
//            cout<<endl;
//        }
        assert(ref_agglomerationLines[i] == agglomerationLines[i]);
    }
//    cout<<endl;
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
    return 1;
}

int test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle_2() {

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


//    cout<<"Before call of computeAnisotropicLine"<<endl;
    computeAnisotropicLine(sizes,
                           adjMatrix_row_ptr,
                           adjMatrix_col_ind,
                           adjMatrix_areaValues,
                           arrayOfFineAnisotropicCompliantCells,
                           agglomerationLines_Idx,
                           agglomerationLines,
                           verbose);
//    cout<<"After call of computeAnisotropicLine"<<endl;
    //agglomerationLines_Idx, agglomerationLines = Util.convertFineAgglomerationLinesToFineAgglomerationLines_arrays(numberOfFineCells, fineAgglomerationLines)
    assert(numberOfFineAnisotropicCompliantCells == 24);
//    cout<<"sizes_2[3] "<<sizes_2[3]<< endl;
//    cout<<"sizes_2[4] "<<sizes_2[4]<< endl;
    assert(3 == sizes[8]);  // number of anisotropic lines
    assert(24 == sizes[9]);  // number of cells in these lines
    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 24;
    long ref_agglomerationLines_Idx[3] = {0, 12, 24};
    long ref_agglomerationLines[24] = {23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
//        cout<<agglomerationLines_Idx_2[i]<<", ";
        assert(ref_agglomerationLines_Idx[i] == agglomerationLines_Idx[i]);
    }
//    cout<<endl;
    for (int i = 0; i < ref_agglomerationLines_size; i++) {
//        cout<<agglomerationLines_2[i]<<", ";
//        if ((i+1)%12==0)
//        {
//            cout<<endl;
//        }
        assert(ref_agglomerationLines[i] == agglomerationLines[i]);
    }
//    cout<<endl;
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
    return 1;
}

int test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle_Iso_at_both_ends() {

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


//    cout<<"Before call of computeAnisotropicLine"<<endl;
    computeAnisotropicLine(sizes,
                           adjMatrix_row_ptr,
                           adjMatrix_col_ind,
                           adjMatrix_areaValues,
                           arrayOfFineAnisotropicCompliantCells,
                           agglomerationLines_Idx,
                           agglomerationLines,
                           verbose);
//    cout<<"After call of computeAnisotropicLine"<<endl;
//agglomerationLines_Idx, agglomerationLines = Util.convertFineAgglomerationLinesToFineAgglomerationLines_arrays(numberOfFineCells, fineAgglomerationLines)
    assert(numberOfFineAnisotropicCompliantCells == 18);
//    cout<<"sizes_3[3] "<<sizes_3[3]<< endl;
//    cout<<"sizes_3[4] "<<sizes_3[4]<< endl;
    assert(3 == sizes[8]);  // number of anisotropic lines
    assert(18 == sizes[9]);  // number of cells in these lines
    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 18;
    long ref_agglomerationLines_Idx[3] = {0, 9, 18};
    long ref_agglomerationLines[18] = {3, 5, 7, 9, 11, 13, 15, 17, 19, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
//        cout<<agglomerationLines_Idx[i]<<", ";
        assert(ref_agglomerationLines_Idx[i] == agglomerationLines_Idx[i]);
    }
//    cout<<endl;
    for (int i = 0; i < ref_agglomerationLines_size; i++) {
//        cout<<agglomerationLines[i]<<", ";
//        if ((i+1)%9==0)
//        {
//            cout<<endl;
//        }
        assert(ref_agglomerationLines[i] == agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
    return 1;
}


int test_agglomerate_Anisotropic_One_Level() {

    long numberOfFineCells = 10;
    long numberOfFineAgglomeratedCells = 0;
    long indCoarseCell = 0;

    long fineAgglomerationLines_array_Idx_size = 5;
    long fineAgglomerationLines_array_Idx[5] = {0, 3, 4, 6, 10};
    long fineAgglomerationLines_array[10] = {0, 1, 5, 2, 3,
                                             6, 4, 7, 8, 9};

    long fineCellToCoarseCell[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    bool isFineCellAgglomerated[10] = {false, false, false, false, false, false, false, false, false, false};
    long AnisotropicCompliantCoarseCells_size = 10;
    long sizes[5] = {fineAgglomerationLines_array_Idx_size, numberOfFineCells, numberOfFineAgglomeratedCells, indCoarseCell, AnisotropicCompliantCoarseCells_size};

    long AnisotropicCompliantCoarseCells_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    agglomerate_Anisotropic_One_Level_without_list_lines(sizes,
                                                         fineAgglomerationLines_array_Idx, fineAgglomerationLines_array,
                                                         fineCellToCoarseCell,
                                                         isFineCellAgglomerated,
                                                         AnisotropicCompliantCoarseCells_array);

    fineAgglomerationLines_array_Idx_size = sizes[0];
    numberOfFineCells = sizes[1];
    numberOfFineAgglomeratedCells = sizes[2];
    indCoarseCell = sizes[3];
    AnisotropicCompliantCoarseCells_size = sizes[4];

    assert(numberOfFineAgglomeratedCells == 9);
    assert(indCoarseCell == 4);
    long ref_fineCellToCoarseCell[10] = {0, 0, -1, 1, 2, 0, 1, 2, 3, 3};
    for (int i = 0; i < numberOfFineCells; i++) {
//        cout <<fineCellToCoarseCell[i]<<", ";
        assert(ref_fineCellToCoarseCell[i] == fineCellToCoarseCell[i]);
    }
//    cout<<endl;

    long ref_AnisotropicCompliantCoarseCells_array[4] = {0, 1, 2, 3};
    for (int i = 0; i < AnisotropicCompliantCoarseCells_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_AnisotropicCompliantCoarseCells_array[i] == AnisotropicCompliantCoarseCells_array[i]);
    }
//    cout<<endl;
    bool ref_isFineCellAgglomerated[10] = {true, true, false, true, true, true, true, true, true, true};
    for (int i = 0; i < numberOfFineCells; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_isFineCellAgglomerated[i] == isFineCellAgglomerated[i]);
    }
//    cout<<endl;
    assert(fineAgglomerationLines_array_Idx_size - 1 == 3);

    long ref_fineAgglomerationLines_array_Idx[4] = {0, 1, 2, 4};
    for (int i = 0; i < fineAgglomerationLines_array_Idx_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_fineAgglomerationLines_array_Idx[i] == fineAgglomerationLines_array_Idx[i]);
    }
//    cout<<endl;
    long ref_fineAgglomerationLines_array[4] = {0, 1, 2, 3};
    for (int i = 0; i < fineAgglomerationLines_array_Idx[fineAgglomerationLines_array_Idx_size - 1]; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_fineAgglomerationLines_array[i] == fineAgglomerationLines_array[i]);
    }
//    cout<<endl;
    return 1;
}

int test_agglomerate_Anisotropic_One_Level_2() {

    long numberOfFineCells = 21;
    long numberOfFineAgglomeratedCells = 0;
    long indCoarseCell = 1;

    long fineAgglomerationLines_array_Idx_size = 8;
    long fineAgglomerationLines_array_Idx[8] = {0, 6, 9, 10, 12, 16, 21, 21};
    long fineAgglomerationLines_array[21] = {15, 16, 17, 18, 19,
                                             20, 0, 1, 5, 2,
                                             3, 6, 4, 7, 8,
                                             9, 10, 11, 12, 13,
                                             14};

    long fineCellToCoarseCell[21] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    bool isFineCellAgglomerated[21] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    long AnisotropicCompliantCoarseCells_size = 21;
    long sizes[5] = {fineAgglomerationLines_array_Idx_size, numberOfFineCells, numberOfFineAgglomeratedCells, indCoarseCell, AnisotropicCompliantCoarseCells_size};

    long AnisotropicCompliantCoarseCells_array[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    agglomerate_Anisotropic_One_Level_without_list_lines(sizes,
                                                         fineAgglomerationLines_array_Idx, fineAgglomerationLines_array,
                                                         fineCellToCoarseCell,
                                                         isFineCellAgglomerated,
                                                         AnisotropicCompliantCoarseCells_array);

    fineAgglomerationLines_array_Idx_size = sizes[0];
    numberOfFineCells = sizes[1];
    numberOfFineAgglomeratedCells = sizes[2];
    indCoarseCell = sizes[3];
    AnisotropicCompliantCoarseCells_size = sizes[4];

    assert(numberOfFineAgglomeratedCells == 20);
    assert(indCoarseCell == 10);
    long ref_fineCellToCoarseCell[21] = {4, 4, -1, 5, 6, 4, 5, 6, 7, 7, 8, 8, 9, 9, 9, 1, 1, 2, 2, 3, 3};
    for (int i = 0; i < numberOfFineCells; i++) {
//        cout <<fineCellToCoarseCell[i]<<", ";
        assert(ref_fineCellToCoarseCell[i] == fineCellToCoarseCell[i]);
    }
//    cout<<endl;

    long ref_AnisotropicCompliantCoarseCells_array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < AnisotropicCompliantCoarseCells_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_AnisotropicCompliantCoarseCells_array[i] == AnisotropicCompliantCoarseCells_array[i]);
    }
//    cout<<endl;
    bool ref_isFineCellAgglomerated[21] = {true, true, false, true, true, true, true, true, true, true,
                                           true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < numberOfFineCells; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_isFineCellAgglomerated[i] == isFineCellAgglomerated[i]);
    }
//    cout<<endl;
    assert(fineAgglomerationLines_array_Idx_size - 1 == 5);

    long ref_fineAgglomerationLines_array_Idx[6] = {0, 3, 4, 5, 7, 9};
    for (int i = 0; i < fineAgglomerationLines_array_Idx_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_fineAgglomerationLines_array_Idx[i] == fineAgglomerationLines_array_Idx[i]);
    }
//    cout<<endl;
    long ref_fineAgglomerationLines_array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < fineAgglomerationLines_array_Idx[fineAgglomerationLines_array_Idx_size - 1]; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_fineAgglomerationLines_array[i] == fineAgglomerationLines_array[i]);
    }
//    cout<<endl;
    return 1;
}

int test_agglomerate_Anisotropic_One_Level_Box_5x5x5_Aniso_MG_1_level() {

    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long indCoarseCell = 0;

    long fineAgglomerationLines_array_Idx_size = 17;
    long fineAgglomerationLines_array_Idx[17] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36,
                                                 40, 44, 48, 52, 56, 60, 64};
    long fineAgglomerationLines_array[64] = {5, 21, 37, 53, 6, 22, 38, 54, 9, 25,
                                             41, 57, 10, 26, 42, 58, 49, 33, 17, 1,
                                             50, 34, 18, 2, 52, 36, 20, 4, 55, 39,
                                             23, 7, 56, 40, 24, 8, 59, 43, 27, 11,
                                             61, 45, 29, 13, 62, 46, 30, 14, 48, 32,
                                             16, 0, 51, 35, 19, 3, 60, 44, 28, 12,
                                             63, 47, 31, 15};

    long *fineCellToCoarseCell = new long[64];
    bool *isFineCellAgglomerated = new bool[64];
    long *AnisotropicCompliantCoarseCells_array = new long[64];
    for (int i = 0; i < 64; i++) {
        fineCellToCoarseCell[i] = -1;
        isFineCellAgglomerated[i] = false;
        AnisotropicCompliantCoarseCells_array[i] = i;
    }

    long AnisotropicCompliantCoarseCells_size = 64;
    long sizes[5] = {fineAgglomerationLines_array_Idx_size, numberOfFineCells, numberOfFineAgglomeratedCells, indCoarseCell, AnisotropicCompliantCoarseCells_size};


    agglomerate_Anisotropic_One_Level_without_list_lines(sizes,
                                                         fineAgglomerationLines_array_Idx, fineAgglomerationLines_array,
                                                         fineCellToCoarseCell,
                                                         isFineCellAgglomerated,
                                                         AnisotropicCompliantCoarseCells_array);

    fineAgglomerationLines_array_Idx_size = sizes[0];
    numberOfFineCells = sizes[1];
    numberOfFineAgglomeratedCells = sizes[2];
    indCoarseCell = sizes[3];
    AnisotropicCompliantCoarseCells_size = sizes[4];
    cout << "numberOfFineAgglomeratedCells " << numberOfFineAgglomeratedCells << endl;
    cout << "indCoarseCell " << indCoarseCell << endl;
    assert(numberOfFineAgglomeratedCells == 64);
    assert(indCoarseCell == 32);
    long ref_fineCellToCoarseCell[64] = {25, 9, 11, 27,
                                         13, 0, 2, 15,
                                         17, 4, 6, 19,
                                         29, 21, 23, 31,

                                         25, 9, 11, 27,
                                         13, 0, 2, 15,
                                         17, 4, 6, 19,
                                         29, 21, 23, 31,

                                         24, 8, 10, 26,
                                         12, 1, 3, 14,
                                         16, 5, 7, 18,
                                         28, 20, 22, 30,

                                         24, 8, 10, 26,
                                         12, 1, 3, 14,
                                         16, 5, 7, 18,
                                         28, 20, 22, 30};
    for (int i = 0; i < numberOfFineCells; i++) {
        assert(ref_fineCellToCoarseCell[i] == fineCellToCoarseCell[i]);
    }

    long ref_AnisotropicCompliantCoarseCells_array[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                                                          17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    for (int i = 0; i < AnisotropicCompliantCoarseCells_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_AnisotropicCompliantCoarseCells_array[i] == AnisotropicCompliantCoarseCells_array[i]);
    }
//    cout<<endl;
    bool *ref_isFineCellAgglomerated = new bool[64];
    for (int i = 0; i < numberOfFineCells; i++) {
        ref_isFineCellAgglomerated[i] = true;
    }
    for (int i = 0; i < numberOfFineCells; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_isFineCellAgglomerated[i] == isFineCellAgglomerated[i]);
    }
//    cout<<endl;
    assert(fineAgglomerationLines_array_Idx_size - 1 == 16);

    long ref_fineAgglomerationLines_array_Idx[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    for (int i = 0; i < fineAgglomerationLines_array_Idx_size; i++) {
        // cout <<AnisotropicCompliantCoarseCells_array[i]<<", ";
        assert(ref_fineAgglomerationLines_array_Idx[i] == fineAgglomerationLines_array_Idx[i]);
    }
    long ref_fineAgglomerationLines_array[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                                 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                                                 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    for (int i = 0; i < fineAgglomerationLines_array_Idx[fineAgglomerationLines_array_Idx_size - 1]; i++) {
        assert(ref_fineAgglomerationLines_array[i] == fineAgglomerationLines_array[i]);
    }

    delete[] fineCellToCoarseCell;
    delete[] isFineCellAgglomerated;
    delete[] AnisotropicCompliantCoarseCells_array;
    delete[] ref_isFineCellAgglomerated;

    return 1;
}


