#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator_anisotropic.h"
#include "gtest/gtest.h"

TEST(CC_TestSuite, createCoarseCell_Case_1) {

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

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});
    ref_dict_Coarse_Elem[2] = unordered_set<long>({4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, 2};

    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

}

TEST(CC_TestSuite, createCoarseCell_Case_1_isMutable_False) {
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

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CC_TestSuite, createCoarseCell_Case_3) {

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

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(7, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});
    ref_dict_Coarse_Elem[2] = unordered_set<long>({4, 5, 6});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, 2, 2, 2, -1};
    for (int i = 0; i < 8; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CC_TestSuite, createCoarseCell_Delayed_Case_1) {

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

    ASSERT_EQ(2, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    ref_dict_Coarse_Elem[2] = unordered_set<long>({4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CC_TestSuite, createCoarseCell_Delayed_Case_1_isMutable_False) {

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

    ASSERT_EQ(2, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[8] = {1, 1, 1, 0, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);

    }

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    ref_dict_Coarse_Elem[2] = unordered_set<long>({4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({2});

    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);


//    assert(dict_Coarse_Elem.size() == 3);
//    assert(dict_Coarse_Elem[0].size() == 1);
//    assert(dict_Coarse_Elem[0].count(3) == 1);
//    assert(dict_Coarse_Elem[1].size() == 3);
//    assert(dict_Coarse_Elem[1].count(0) == 1);
//    assert(dict_Coarse_Elem[1].count(1) == 1);
//    assert(dict_Coarse_Elem[1].count(2) == 1);
//
//    assert(dict_Card_Coarse_Cells.size() == 3);
//    assert(dict_Card_Coarse_Cells[1].size() == 1);
//    assert(dict_Card_Coarse_Cells[1].count(0) == 1);
//    assert(dict_Card_Coarse_Cells[3].size() == 1);
//    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
//    assert(dict_Card_Coarse_Cells[4].size() == 1);
//    assert(dict_Card_Coarse_Cells[4].count(2) == 1);
//
//    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
//    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
//    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
//    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CC_TestSuite, createCoarseCell_Delayed_Case_3) {


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

    ASSERT_EQ(2, indCoarseElement);
    ASSERT_EQ(7, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

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

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(7, numberOfFineAgglomeratedCells_tmp);

    ref_dict_Coarse_Elem[2] = unordered_set<long>({4, 5, 6});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    // the map was already up to date.
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, -1};
    for (int i = 0; i < 8; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}


TEST(CC_TestSuite, SwapFineCell_Case_1) {

    int iLevel = 0;
    long iFineCell = 0;
    long iOrigineCoarseCell = 0;
    long iDestinationCoarseCell = 1;

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
//    assert(set_removedCoarseCells.size() == 0);
    ASSERT_EQ(0, set_removedCoarseCells.size());

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};
    for (int i = 0; i < 4; i++) {
//        assert(ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[i] == fine_Cell_indices_To_Coarse_Cell_Indices[i]);
        ASSERT_EQ(ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CC_TestSuite, SwapFineCell_Deletion_of_Coarse_Element) {

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
//    assert(set_removedCoarseCells.size() == 1);
//    assert(set_removedCoarseCells.count(0) == 1);
    unordered_set<long> ref_set_removedCoarseCells = unordered_set<long>({0});
    ASSERT_EQ(ref_set_removedCoarseCells, set_removedCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0, 3});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);


    long ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 1};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}
