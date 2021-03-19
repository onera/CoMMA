//
// Created by Nicolas Lantos on 18/10/2017.
//
#include "Agglomerator_Isotropic.h"
#include "Agglomerator.h"
#include<assert.h>
#include<iostream>


using namespace std;


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
//    cout << "numberOfFineAgglomeratedCells " << numberOfFineAgglomeratedCells << endl;
//    cout << "indCoarseCell " << indCoarseCell << endl;
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


int test_read_agglomeration_datas_from_file() {

    // Datas from test_agglomerate.test_agglomerateOneLevel_disconnected(...)
    string filename = "/Users/lantos/CLionProjects/CoMMA_lib/0_Outputs/Datas_Agglomeration_10_24.txt";
    long *fineCellToCoarseCell = new long[64];
    long sizes[10];
    long *adjMatrix_row_ptr = NULL;
    long *adjMatrix_col_ind = NULL;
    double *adjMatrix_areaValues = NULL;
    double *volumes = NULL;
    long *arrayOfFineAnisotropicCompliantCells = NULL;
    long *isOnFineBnd_l = NULL;
    long *array_isOnValley = NULL;
    long *array_isOnRidge = NULL;
    long *array_isOnCorner = NULL;

    long *isFirstAgglomeration_long = new long[1];
    long *isAnisotropic_long = new long[1];
    long *agglomerationLines_Idx = NULL;
    long *agglomerationLines = NULL;

    long *dimension = new long[1];
    long *goalCard = new long[1];
    long *minCard = new long[1];
    long *maxCard = new long[1];

    read_agglomeration_datas_from_file(filename,
                                       sizes,
                                       adjMatrix_row_ptr,
                                       adjMatrix_col_ind,
                                       adjMatrix_areaValues,
                                       volumes,

                                       arrayOfFineAnisotropicCompliantCells,

                                       isOnFineBnd_l,
                                       array_isOnValley,
                                       array_isOnRidge,
                                       array_isOnCorner,
                                       isFirstAgglomeration_long,
                                       isAnisotropic_long,
                                       agglomerationLines_Idx,
                                       agglomerationLines,
                                       dimension, goalCard, minCard, maxCard
    );

    long numberOfFineCells = sizes[0];
    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
    long adjMatrix_col_ind_size = sizes[1];
    long adjMatrix_areaValues_size = sizes[1];

    // Rmk: sizes[2] ==indCoarseCell
    long numberOfFineAgglomeratedCells = sizes[3];
    long isOnValley_size = sizes[4];
    long isOnRidge_size = sizes[5];
    long isOnCorner_size = sizes[6];
    long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
    long agglomerationLines_Idx_size = sizes[8];
    long agglomerationLines_size = sizes[9];


    // MGridGen Test case
    int ref_numberOfFineCells = 10;
    long ref_adjMatrix_col_ind_size = 24;

    long ref_adjMatrix_row_ptr[11] = {0, 3, 5, 7, 9, 11, 12, 15, 18, 21, 24};
    long ref_adjMatrix_col_ind[24] = {0, 1, 2, 0, 1,
                                      0, 2, 3, 4, 3,
                                      4, 5, 6, 7, 8,
                                      6, 7, 9, 6, 8, 9, 7, 8, 9};
    double ref_adjMatrix_areaValues[24] = {1., 1., 1., 1., 2., 1., 2., 2., 1., 1., 2., 3., 2., 1., 1., 1., 2.,
                                           1., 1., 2., 1., 1., 1., 2.};

    double ref_volumes[10] = {1.0, 1.0, 1.0, 1.0, 1.0,
                              1.0, 1.0, 1.0, 1.0, 1.0};
    long ref_isOnBnd[10] = {1, 2, 2, 2, 2,
                            3, 2, 2, 2, 2};

    long ref_agglomerationLines_Idx[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    long ref_agglomerationLines[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    long *ref_arrayOfFineAnisotropicCompliantCells = new long[ref_numberOfFineCells];
    for (int i = 0; i < ref_numberOfFineCells; i++) {
        ref_arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long ref_isOnRidge_size = 1;
    long ref_isOnValley_size = 8;
    long ref_isOnCorner_size = 1;

    long ref_array_isOnRidge[1] = {0};
    long ref_array_isOnValley[8] = {1, 2, 3, 4, 6, 7, 8, 9};
    long ref_array_isOnCorner[1] = {5};

    long ref_isFirstAgglomeration = 1;
    long ref_isAnisotropic = 0;
    long ref_dimension = 2;

    int ref_thresholdCard = 3;
    int ref_minCard = 3;
    int ref_goalCard = 4;
    int ref_maxCard = 5;

    // Checks
    assert(ref_numberOfFineCells == numberOfFineCells);
    assert(ref_adjMatrix_col_ind_size == adjMatrix_col_ind_size);

    assert(ref_isFirstAgglomeration == isFirstAgglomeration_long[0]);
    assert(ref_isAnisotropic == isAnisotropic_long[0]);

    assert(ref_dimension == dimension[0]);
    assert(ref_goalCard == goalCard[0]);
    assert(ref_minCard == minCard[0]);
    assert(ref_maxCard == maxCard[0]);

    for (int i = 0; i < numberOfFineCells + 1; i++) {
        assert(ref_adjMatrix_row_ptr[i] == adjMatrix_row_ptr[i]);
    }
    for (int i = 0; i < ref_adjMatrix_col_ind_size; i++) {
        assert(ref_adjMatrix_col_ind[i] == adjMatrix_col_ind[i]);
    }

    for (int i = 0; i < ref_adjMatrix_col_ind_size; i++) {
        assert(ref_adjMatrix_areaValues[i] == adjMatrix_areaValues[i]);
    }

    for (int i = 0; i < numberOfFineCells; i++) {
        assert(ref_volumes[i] == volumes[i]);
    }

    for (int i = 0; i < numberOfFineCells; i++) {
        assert(ref_arrayOfFineAnisotropicCompliantCells[i] == arrayOfFineAnisotropicCompliantCells[i]);
    }
    for (int i = 0; i < numberOfFineCells; i++) {
        assert(ref_isOnBnd[i] == isOnFineBnd_l[i]);
    }
    for (int i = 0; i < ref_isOnValley_size; i++) {
        assert(ref_array_isOnValley[i] == array_isOnValley[i]);
    }

    for (int i = 0; i < ref_isOnRidge_size; i++) {
        assert(ref_array_isOnRidge[i] == array_isOnRidge[i]);
    }
    for (int i = 0; i < ref_isOnCorner_size; i++) {
        assert(ref_array_isOnCorner[i] == array_isOnCorner[i]);
    }
    for (int i = 0; i < numberOfFineCells; i++) {
        assert(ref_agglomerationLines_Idx[i] == agglomerationLines_Idx[i]);
    }
    for (int i = 0; i < numberOfFineCells; i++) {
        assert(ref_agglomerationLines[i] == agglomerationLines[i]);
    }

    return 1;
}


int test_read_agglomeration_datas_from_file_and_agglomerate() {

    // Datas from test_agglomerate.test_agglomerateOneLevel_disconnected(...)
    //    string filename = "/Users/lantos/CLionProjects/CoMMA_lib/0_Outputs/Datas_Agglomeration_10_24.txt";
    string input_folder = "/Users/lantos/CLionProjects/CoMMA_lib/0_Inputs/Agglo_datas/";
//    string filename = "/Users/lantos/CLionProjects/CoMMA_lib/0_Inputs/Agglo_datas/Datas_Agglomeration_51410_295612.txt";
    const int nb_domain = 64;
    std::string a_filenames[nb_domain] = {"Datas_Agglomeration_106103_610148.txt",
                                          "Datas_Agglomeration_73753_415900.txt",
                                          "Datas_Agglomeration_77958_448746.txt",
                                          "Datas_Agglomeration_84877_480698.txt",
                                          "Datas_Agglomeration_106112_602136.txt",
                                          "Datas_Agglomeration_73941_409958.txt",
                                          "Datas_Agglomeration_78133_454500.txt",
                                          "Datas_Agglomeration_84932_483364.txt",
                                          "Datas_Agglomeration_51410_295612.txt",
                                          "Datas_Agglomeration_74680_418108.txt",
                                          "Datas_Agglomeration_78139_448332.txt",
                                          "Datas_Agglomeration_85440_481472.txt",
                                          "Datas_Agglomeration_61784_356906.txt",
                                          "Datas_Agglomeration_74858_421406.txt",
                                          "Datas_Agglomeration_78291_442628.txt",
                                          "Datas_Agglomeration_85463_489092.txt",
                                          "Datas_Agglomeration_63389_360748.txt",
                                          "Datas_Agglomeration_75354_428408.txt",
                                          "Datas_Agglomeration_78309_442878.txt",
                                          "Datas_Agglomeration_85992_487072.txt",
                                          "Datas_Agglomeration_70808_399850.txt",
                                          "Datas_Agglomeration_75589_434930.txt",
                                          "Datas_Agglomeration_78697_432460.txt",
                                          "Datas_Agglomeration_86162_492854.txt",
                                          "Datas_Agglomeration_71626_405406.txt",
                                          "Datas_Agglomeration_75651_435656.txt",
                                          "Datas_Agglomeration_80083_447122.txt",
                                          "Datas_Agglomeration_86320_496618.txt",
                                          "Datas_Agglomeration_72042_418242.txt",
                                          "Datas_Agglomeration_75716_425486.txt",
                                          "Datas_Agglomeration_80952_458358.txt",
                                          "Datas_Agglomeration_86365_491656.txt",
                                          "Datas_Agglomeration_72378_411388.txt",
                                          "Datas_Agglomeration_76606_431576.txt",
                                          "Datas_Agglomeration_82012_469728.txt",
                                          "Datas_Agglomeration_86691_493422.txt",
                                          "Datas_Agglomeration_72853_423128.txt",
                                          "Datas_Agglomeration_76680_435458.txt",
                                          "Datas_Agglomeration_82426_475268.txt",
                                          "Datas_Agglomeration_87388_497946.txt",
                                          "Datas_Agglomeration_72885_416994.txt",
                                          "Datas_Agglomeration_77017_433482.txt",
                                          "Datas_Agglomeration_82677_472674.txt",
                                          "Datas_Agglomeration_87573_492128.txt",
                                          "Datas_Agglomeration_73062_410084.txt",
                                          "Datas_Agglomeration_77119_441708.txt",
                                          "Datas_Agglomeration_83187_483764.txt",
                                          "Datas_Agglomeration_88571_506834.txt",
                                          "Datas_Agglomeration_73217_409458.txt",
                                          "Datas_Agglomeration_77696_448110.txt",
                                          "Datas_Agglomeration_84047_472582.txt",
                                          "Datas_Agglomeration_89687_507460.txt",
                                          "Datas_Agglomeration_73510_416028.txt",
                                          "Datas_Agglomeration_77848_446462.txt",
                                          "Datas_Agglomeration_84304_474282.txt",
                                          "Datas_Agglomeration_90140_524436.txt",
                                          "Datas_Agglomeration_73557_417550.txt",
                                          "Datas_Agglomeration_77884_449022.txt",
                                          "Datas_Agglomeration_84407_480058.txt",
                                          "Datas_Agglomeration_92001_533656.txt",
                                          "Datas_Agglomeration_73636_421836.txt",
                                          "Datas_Agglomeration_77926_446600.txt",
                                          "Datas_Agglomeration_84529_478592.txt",
                                          "Datas_Agglomeration_99365_569388.txt"};

    for (int i = 1; i < 2/*nb_domain*/; i++) {

        std::cout <<"\nDomain "<<i<<" "<< a_filenames[i] << "\n";
        string filename = input_folder + a_filenames[i];
        long sizes[10];
        long *adjMatrix_row_ptr = NULL;
        long *adjMatrix_col_ind = NULL;
        double *adjMatrix_areaValues = NULL;
        double *volumes = NULL;
        long *arrayOfFineAnisotropicCompliantCells = NULL;
        long *isOnFineBnd_l = NULL;
        long *array_isOnValley = NULL;
        long *array_isOnRidge = NULL;
        long *array_isOnCorner = NULL;

        long *isFirstAgglomeration_long = new long[1];
        long *isAnisotropic_long = new long[1];
        long *agglomerationLines_Idx = NULL;
        long *agglomerationLines = NULL;

        long *dimension = new long[1];
        long *goalCard = new long[1];
        long *minCard = new long[1];
        long *maxCard = new long[1];

        read_agglomeration_datas_from_file(filename,
                                           sizes,
                                           adjMatrix_row_ptr,
                                           adjMatrix_col_ind,
                                           adjMatrix_areaValues,
                                           volumes,

                                           arrayOfFineAnisotropicCompliantCells,

                                           isOnFineBnd_l,
                                           array_isOnValley,
                                           array_isOnRidge,
                                           array_isOnCorner,
                                           isFirstAgglomeration_long,
                                           isAnisotropic_long,
                                           agglomerationLines_Idx,
                                           agglomerationLines,
                                           dimension, goalCard, minCard, maxCard
        );

        long numberOfFineCells = sizes[0];
        cout << "\tnumberOfFineCells\t" << numberOfFineCells << endl;
        long adjMatrix_row_ptr_size = numberOfFineCells + 1;
        long adjMatrix_col_ind_size = sizes[1];
        long adjMatrix_areaValues_size = sizes[1];

        // Rmk: sizes[2] ==indCoarseCell
        long numberOfFineAgglomeratedCells = sizes[3];
        long isOnValley_size = sizes[4];
        long isOnRidge_size = sizes[5];
        long isOnCorner_size = sizes[6];
        long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
        long agglomerationLines_Idx_size = sizes[8];
        long agglomerationLines_size = sizes[9];

        bool isFineCellAgglomerated[numberOfFineCells];
        for (int i = 0; i < numberOfFineCells; i++) {
            isFineCellAgglomerated[i] = false;
        }
        long fineCellIndicesToCoarseCellIndices[numberOfFineCells];
        for (int i = 0; i < numberOfFineCells; i++) {
            fineCellIndicesToCoarseCellIndices[i] = -1;
        }

        long indCoarseCell = 0;

        long checks = 1;
        long verbose = 0;

        // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells

        agglomerateOneLevel(sizes,
                            adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                            arrayOfFineAnisotropicCompliantCells,

                            isOnFineBnd_l,
                            array_isOnValley,
                            array_isOnRidge,
                            array_isOnCorner,

                            isFirstAgglomeration_long[0],
                            isAnisotropic_long[0],

                            fineCellIndicesToCoarseCellIndices,

                            agglomerationLines_Idx,
                            agglomerationLines,

                            dimension[0],
                            goalCard[0],
                            minCard[0],
                            maxCard[0],
                            checks,
                            verbose);

//        cout << "\tsizes[0] " << sizes[0] << endl;
        cout << "\tsizes[1] " << sizes[1];
        cout << "\tsizes[2] " << sizes[2];
        cout << "\tsizes[3] " << sizes[3];
        cout << "\tsizes[4] " << sizes[4];
        cout << "\tsizes[5] " << sizes[5];
        cout << "\tsizes[6] " << sizes[6];
        cout << "\tsizes[7] " << sizes[7];
        cout << "\tsizes[8] " << sizes[8];
        cout << "\tsizes[9] " << sizes[9] << endl;

        //    assert(sizes[0] == 64);
        //    assert(sizes[1] == 344);
        //    assert(sizes[2] == 8);//indCoarseCell
        //    assert(sizes[3] == 64);//numberOfFineAgglomeratedCells
        //    assert(sizes[4] == 24);//isOnValley_size
        //    assert(sizes[5] == 24);//isOnRidge_size
        //    assert(sizes[6] == 8);//isOnCorner_size
        //    assert(sizes[7] == 64);//arrayOfFineAnisotropicCompliantCells_size
        //    assert(sizes[8] == 64);//agglomerationLines_Idx_size
        //    assert(sizes[9] == 64);//agglomerationLines_size

//        cout << "\nfineCellIndicesToCoarseCellIndices: [";
        for (auto i:fineCellIndicesToCoarseCellIndices) {
//            cout << i << ", ";
            assert(i<sizes[2]);
        }
//        cout << "]" << endl;

        //    for (int i = 0; i < 64; i++) {
        //
        //        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
        //    }
    }
    return 1;
}