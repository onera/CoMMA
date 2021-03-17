#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator.h"
#include "gtest/gtest.h"

TEST(CoarseCellGraph_TestSuite, splitNonConnectedCoarseCell_9_Squares_1) {

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
    ASSERT_EQ(5, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({2});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({ 6});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({1, 3, 4, 5, 7});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({ 8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 1, 2, 4});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({3});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 4;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 3, 1, 3, 3, 3, 2, 3, 4};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, splitNonConnectedCoarseCell_9_Squares_2) {
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

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 2});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 3, 4, 5, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({ 8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({1, 3});
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);


//    assert(dict_Coarse_Cells.size() == 4);
//    assert(dict_Coarse_Cells[0].size() == 2);
//    assert(dict_Coarse_Cells[1].size() == 1);
//    assert(dict_Coarse_Cells[2].size() == 5);
//    assert(dict_Coarse_Cells[3].size() == 1);
//
//    assert(dict_Coarse_Cells[0].count(0) == 1);
//    assert(dict_Coarse_Cells[0].count(2) == 1);
//
//    assert(dict_Coarse_Cells[1].count(6) == 1);
//
//    assert(dict_Coarse_Cells[2].count(1) == 1);
//    assert(dict_Coarse_Cells[2].count(3) == 1);
//    assert(dict_Coarse_Cells[2].count(4) == 1);
//    assert(dict_Coarse_Cells[2].count(5) == 1);
//    assert(dict_Coarse_Cells[2].count(7) == 1);
//
//    assert(dict_Coarse_Cells[3].count(8) == 1);
//
////    self.assertEqual(dict([(1, {1, 3}), (5, {3}), (2, {0})]), dict_Card_Coarse_Cells)
//
//    assert(dict_Card_Coarse_Cells.size() == 3);
//    assert(dict_Card_Coarse_Cells[1].size() == 2);
//    assert(dict_Card_Coarse_Cells[2].size() == 1);
//    assert(dict_Card_Coarse_Cells[5].size() == 1);
//
//    assert(dict_Card_Coarse_Cells[1].count(1) == 1);
//    assert(dict_Card_Coarse_Cells[1].count(3) == 1);
//
//    assert(dict_Card_Coarse_Cells[2].count(0) == 1);
//    assert(dict_Card_Coarse_Cells[5].count(2) == 1);
//
//
////  self.assertEqual({1: 2, 2: 1, 5: 1}, dict_DistributionOfCardinalOfCoarseElements)
//    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
//    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 2);
//    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 1);
//    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 1);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 2, 2, 2, 1, 2, 3};
    for (int i = 0; i < 9; i++) {
//        assert(isFineCellAgglomerated[i] == ref_isFineCellAgglomerated_tmp[i]);
//        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);

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
    ASSERT_EQ(5, indCoarseElement);

    ref_dict_Coarse_Cells[0] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 3, 4, 5, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({ 8});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({ 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    ref_dict_Card_Coarse_Cells.erase(2);
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 1, 3, 4});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    ref_dict_DistributionOfCardinalOfCoarseElements.erase(2);
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 4;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);



//    self.assertEqual({0: [2], 1: [8], 2: [1, 3, 4, 5, 7], 3: [6], 4:[0]}, dict_Coarse_Cells)
//    assert(dict_Coarse_Cells.size() == 5);
//    assert(dict_Coarse_Cells[0].size() == 1);
//    assert(dict_Coarse_Cells[1].size() == 1);
//    assert(dict_Coarse_Cells[2].size() == 5);
//    assert(dict_Coarse_Cells[3].size() == 1);
//    assert(dict_Coarse_Cells[4].size() == 1);
//
//    assert(dict_Coarse_Cells[0].count(0) == 1);
//
//    assert(dict_Coarse_Cells[1].count(6) == 1);
//
//    assert(dict_Coarse_Cells[2].count(1) == 1);
//    assert(dict_Coarse_Cells[2].count(3) == 1);
//    assert(dict_Coarse_Cells[2].count(4) == 1);
//    assert(dict_Coarse_Cells[2].count(5) == 1);
//    assert(dict_Coarse_Cells[2].count(7) == 1);
//
//    assert(dict_Coarse_Cells[3].count(8) == 1);
//    assert(dict_Coarse_Cells[4].count(2) == 1);
//
////   self.assertEqual(dict([(1, {1, 3, 0, 4}), (5, {3})]), dict_Card_Coarse_Cells)
//
//    assert(dict_Card_Coarse_Cells.size() == 2);
//    assert(dict_Card_Coarse_Cells[1].size() == 4);
//    assert(dict_Card_Coarse_Cells[5].size() == 1);
//
//    assert(dict_Card_Coarse_Cells[1].count(0) == 1);
//    assert(dict_Card_Coarse_Cells[1].count(1) == 1);
//    assert(dict_Card_Coarse_Cells[1].count(3) == 1);
//    assert(dict_Card_Coarse_Cells[1].count(4) == 1);
//
//    assert(dict_Card_Coarse_Cells[5].count(2) == 1);
//
//
////  self.assertEqual({1: 4, 5: 1}, dict_DistributionOfCardinalOfCoarseElements)
//    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 2);
//    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 4);
//    assert(dict_DistributionOfCardinalOfCoarseElements[5] == 1);

    bool ref_isFineCellAgglomerated_tmp_2[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[9] = {0, 2, 4, 2, 2, 2, 1, 2, 3};
    for (int i = 0; i < 9; i++) {
//        assert(isFineCellAgglomerated[i] == ref_isFineCellAgglomerated_tmp_2[i]);
//        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, splitNonConnectedCoarseCell_9_Squares_3) {

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

}
TEST(CoarseCellGraph_TestSuite, splitNonConnectedCoarseCell_9_Squares_5) {

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
}
