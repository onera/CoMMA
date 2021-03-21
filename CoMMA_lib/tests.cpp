//
// Created by Nicolas Lantos on 18/10/2017.
//
#include "Agglomerator_Isotropic.h"
#include "Agglomerator.h"
#include<assert.h>
#include<iostream>


using namespace std;





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




