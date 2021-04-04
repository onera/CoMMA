#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator_anisotropic.h"
#include "gtest/gtest.h"

typedef unordered_map<long, unordered_set<long>> unorderedMap;
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

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0});
    dict_Coarse_Cells[1] = unordered_set<long>({2, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({6});
    dict_Coarse_Cells[3] = unordered_set<long>({1, 3, 4, 5, 7});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 2});
    dict_Card_Coarse_Cells[2] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({3});

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

    ASSERT_EQ(5, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({2});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({6});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({1, 3, 4, 5, 7});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({8});
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

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 2});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 3, 4, 5, 7});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

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


    ASSERT_EQ(4, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 2});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 3, 4, 5, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8});
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


    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 2, 2, 2, 1, 2, 3};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);

    }

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
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    ref_dict_Card_Coarse_Cells.erase(2);
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 1, 3, 4});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    ref_dict_DistributionOfCardinalOfCoarseElements.erase(2);
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 4;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp_2[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[9] = {0, 2, 4, 2, 2, 2, 1, 2, 3};
    for (int i = 0; i < 9; i++) {

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

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 2, 5});
    dict_Coarse_Cells[1] = unordered_set<long>({3, 6, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2});

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


    ASSERT_EQ(4, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 2, 5});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({3, 6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({3});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 2});
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 1, 2, 0, 1, 2, 3};
    for (int i = 0; i < 9; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
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

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[2] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    long iCoarseCell = 0;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);


    ASSERT_EQ(5, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({6, 7});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({3, 5, 8});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 4});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({3, 4});
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 2});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {3, 2, 4, 1, 2, 1, 0, 0, 1};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);

    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_3_isotropicCell_0_anisotropic) {

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


    int thresholdCard = 2;  // wrong value in 2D but for test!
    long indCoarseCell = 4;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[2] = unordered_set<long>({4});
    dict_Coarse_Cells[3] = unordered_set<long>({2, 5, 7, 8});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({3});

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

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 3, 6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 7, 8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 0, 2, 1, 2, 2, 1, 2, 2};
    for (int i = 0; i < 9; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_3_isotropicCell_4) {

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

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

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

    ASSERT_EQ(3, indCoarseCell);

    ASSERT_TRUE(dict_Coarse_Cells.empty());
    ASSERT_TRUE(dict_Card_Coarse_Cells.empty());
    ASSERT_TRUE(dict_DistributionOfCardinalOfCoarseElements.empty());

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 2, 4, 1, 3, 4, 1, 3, 4};
    for (int i = 0; i < 9; i++) {
//        cout << i << " " << fineCellIndicesToCoarseCellIndices[i] << endl;
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_Case_3) {

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

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({1, 4});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 0, 2, 1, 0, 2, 1, 2, 2};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_Too_small_Cell_size_2) {

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

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 4, 5, 7, 8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[6] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[6] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);


    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 1, 1, 0, 1, 1, 0, 1, 1};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_Too_small_Cell_size_2_case_2) {

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

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 3, 4, 6});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 1, 0, 1, 1, 0, 1, 0, 0};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_12_Squares_8_isotropicCell_1_anisotropic) {
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
    ASSERT_EQ(6, indCoarseCell);
    ASSERT_TRUE(dict_Coarse_Cells.empty());
    ASSERT_TRUE(dict_Card_Coarse_Cells.empty());
    ASSERT_TRUE(dict_DistributionOfCardinalOfCoarseElements.empty());

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 4, 0, 2, 4, 1, 3, 5, 1, 3, 5};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_12_Squares_8_isotropicCell_1_anisotropic_case_2) {

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
    ASSERT_EQ(6, indCoarseCell);
    ASSERT_TRUE(dict_Coarse_Cells.empty());
    ASSERT_TRUE(dict_Card_Coarse_Cells.empty());
    ASSERT_TRUE(dict_DistributionOfCardinalOfCoarseElements.empty());

    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 4, 0, 3, 4, 1, 3, 5, 1, 3, 5};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_12_Squares_9_isotropicCell) {
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
    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2, 5, 8, 11});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({3, 4, 6, 7, 9, 10});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[6] = unordered_set<long>({0, 1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[6] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells) {
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
    ASSERT_EQ(3, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3, 4});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2, 5, 8, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 3;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 2, 0, 0, 2, 1, 1, 2, 1, 1, 2};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_2) {
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

    ASSERT_EQ(4, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 4, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({7, 9, 10});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 2, 5});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({3});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1, 2});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 0, 2, 0, 1, 3, 1, 1, 3};
    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_3) {

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

    ASSERT_EQ(3, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2, 3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({8, 9, 10, 11});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 3;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1};
    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_4) {
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

    ASSERT_EQ(4, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({2, 3});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 1, 0, 2, 1, 0, 2, 1, 3, 3, 3};
    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_5) {

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

    ASSERT_EQ(3, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 4, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 5, 8});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({7, 9, 10, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 3;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 1, 1, 0, 0, 1, 0, 2, 1, 2, 2, 2};
    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}


TEST(CoarseCellGraph_TestSuite, removeDeletedCoarseCells_Case_1) {

    long numberOfCoarseCells = 2;
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});

    long fineCellIndicesToCoarseCellIndices[4] = {1, 1, 1, 0};
    unordered_set<long> set_removedCoarseCells;

    removeDeletedCoarseCells_v3(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                fineCellIndicesToCoarseCellIndices, set_removedCoarseCells, numberOfCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, removeDeletedCoarseCells_Deletion_of_Coarse_Element) {


    long numberOfCoarseCells = 2;
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});

    long fineCellIndicesToCoarseCellIndices[3] = {1, 1, 1};
    unordered_set<long> set_removedCoarseCells({0});

    removeDeletedCoarseCells_v3(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                fineCellIndicesToCoarseCellIndices, set_removedCoarseCells, numberOfCoarseCells);
    ASSERT_EQ(1, numberOfCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, removeDeletedCoarseCells_Deletion_of_Coarse_Element_Case_2) {

    long numberOfCoarseCells = 8;
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({0});
    dict_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Coarse_Cells[4] = unordered_set<long>({2});
    dict_Coarse_Cells[6] = unordered_set<long>({3});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({1, 3, 4, 6});

    long fineCellIndicesToCoarseCellIndices[4] = {1, 3, 4, 6};
    unordered_set<long> set_removedCoarseCells({0, 2, 5, 7});

    removeDeletedCoarseCells_v3(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                fineCellIndicesToCoarseCellIndices, set_removedCoarseCells, numberOfCoarseCells);
    ASSERT_EQ(4, numberOfCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({1});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({3});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 1, 2, 3});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, removeDeletedCoarseCells_Deletion_of_Coarse_Element_Case_3) {

    long numberOfCoarseCells = 7;
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({0, 4, 9});
    dict_Coarse_Cells[3] = unordered_set<long>({3, 5, 7});
    dict_Coarse_Cells[4] = unordered_set<long>({2, 6, 8});
    dict_Coarse_Cells[6] = unordered_set<long>({1, 10});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({6});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1, 3, 4});

    long fineCellIndicesToCoarseCellIndices[11] = {1, 6, 4, 3, 1, 3, 4, 3, 4, 1, 6};
    unordered_set<long> set_removedCoarseCells({0, 2, 5});

    removeDeletedCoarseCells_v3(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                fineCellIndicesToCoarseCellIndices, set_removedCoarseCells, numberOfCoarseCells);

    ASSERT_EQ(4, numberOfCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 4, 9});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({3, 5, 7});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2, 6, 8});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({1, 10});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({3});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[11] = {0, 3, 2, 1, 0, 1, 2, 1, 2, 0, 3};
    for (int i = 0; i < 11; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}


TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells) {

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

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 2, 4, 5, 8, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[6] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[6] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 2, 2, 1, 1, 2, 1, 1, 2};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells_Case_2) {
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


    ASSERT_EQ(4, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 2, 4, 5});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 3});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 2, 2, 1, 1, 3, 1, 1, 3};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells_Case_3) {
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

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({9, 10, 11});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2, 3, 4, 5, 6, 7, 8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[9] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[9] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells_Case_4) {

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

    ASSERT_EQ(4, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({2, 3});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 1, 0, 2, 1, 0, 2, 1, 3, 3, 3};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells_Case_5) {
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

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 4, 5, 7, 8});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({9, 10, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 2});
    ref_dict_Card_Coarse_Cells[6] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[6] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 1, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}


TEST(CoarseCellGraph_TestSuite, createCoarseCell_Case_1) {

    int iLevel = 0;
    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

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

    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells);
    ASSERT_TRUE(delayedCoarseCells.empty());

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

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

TEST(CoarseCellGraph_TestSuite, createCoarseCell_Case_1_isMutable_False) {

// Ex case 2
    int iLevel = 0;
    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

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
    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells,
                                           false);

    ASSERT_TRUE(delayedCoarseCells.empty());

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

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

TEST(CoarseCellGraph_TestSuite, createCoarseCell_Case_3) {


    unordered_set<long> l({4, 5, 6});

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

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

    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells);
    ASSERT_TRUE(delayedCoarseCells.empty());

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(7, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({4, 5, 6});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

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

TEST(CoarseCellGraph_TestSuite, createCoarseCell_Delayed_Case_1) {

    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

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
    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells,
                                           true, true);

    ASSERT_EQ(2, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

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


    ASSERT_FALSE(delayedCoarseCells.empty());
    list<unordered_set<long>> ref_delayedCoarseCells;
    ref_delayedCoarseCells.push_front(unordered_set<long>({4, 5, 6, 7}));
    ASSERT_EQ(ref_delayedCoarseCells, delayedCoarseCells);

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                             indCoarseElement, fine_Cell_indices_To_Coarse_Cell_Indices);
    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_2;
    ref_dict_Coarse_Cells_2[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells_2[1] = unordered_set<long>({0, 1, 2});
    ref_dict_Coarse_Cells_2[2] = unordered_set<long>({4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Coarse_Cells_2, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_2;
    ref_dict_Card_Coarse_Cells_2[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells_2[3] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells_2[4] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_2, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_2, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, createCoarseCell_Delayed_Case_1_isMutable_False) {

    unordered_set<long> l({4, 5, 6, 7});

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

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
    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells,
                                           false, true);

    ASSERT_FALSE(delayedCoarseCells.empty());
    list<unordered_set<long>> ref_delayedCoarseCells;
    ref_delayedCoarseCells.push_front(unordered_set<long>({4, 5, 6, 7}));
    ASSERT_EQ(ref_delayedCoarseCells, delayedCoarseCells);

    ASSERT_EQ(2, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

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
    createADelayedCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_2;
    ref_dict_Coarse_Cells_2[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells_2[1] = unordered_set<long>({0, 1, 2});
    ref_dict_Coarse_Cells_2[2] = unordered_set<long>({4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Coarse_Cells_2, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_2;
    ref_dict_Card_Coarse_Cells_2[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells_2[3] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells_2[4] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_2, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_2, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, 2};
    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, createCoarseCell_Delayed_Case_3) {

    unordered_set<long> l({4, 5, 6});

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

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
    agglomerate_Isotropic_createCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                           dict_DistributionOfCardinalOfCoarseElements,
                                           indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                                           isFineCellAgglomerated_tmp,
                                           fine_Cell_indices_To_Coarse_Cell_Indices,
                                           delayedCoarseCells,
                                           true, true);

    ASSERT_FALSE(delayedCoarseCells.empty());
    list<unordered_set<long>> ref_delayedCoarseCells;
    ref_delayedCoarseCells.push_front(unordered_set<long>({4, 5, 6}));
    ASSERT_EQ(ref_delayedCoarseCells, delayedCoarseCells);

    ASSERT_EQ(2, indCoarseElement);
    ASSERT_EQ(7, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

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
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    createADelayedCoarseCell(l, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                             indCoarseElement,
                             fine_Cell_indices_To_Coarse_Cell_Indices);

    ASSERT_EQ(3, indCoarseElement);
    ASSERT_EQ(7, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_2;
    ref_dict_Coarse_Cells_2[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells_2[1] = unordered_set<long>({0, 1, 2});
    ref_dict_Coarse_Cells_2[2] = unordered_set<long>({4, 5, 6});
    ASSERT_EQ(ref_dict_Coarse_Cells_2, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_2;
    ref_dict_Card_Coarse_Cells_2[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells_2[3] = unordered_set<long>({1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_2, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_2, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp_2[8] = {true, true, true, true, true, true, true, false};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[8] = {1, 1, 1, 0, 2, 2, 2, -1};
    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

}


TEST(CoarseCellGraph_TestSuite, Correction_Too_Big_Cells_12_Squares_12_isotropic_Cells) {

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

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_0;
    ref_dict_Card_Coarse_Cells_0[1] = unordered_set<long>({2, 3});
    ref_dict_Card_Coarse_Cells_0[5] = unordered_set<long>({0, 1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_0, dict_Card_Coarse_Cells);

    agglomerate_Isotropic_Correction_Too_Big_Cells(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                                   row_ptr, col_ind,
                                                   dict_DistributionOfCardinalOfCoarseElements,
                                                   fineCellIndicesToCoarseCellIndices,
                                                   indCoarseCell,
                                                   goalCard, false);

    ASSERT_EQ(4, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3, 4});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({7, 8, 10, 11});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2, 5});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({6, 9});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({2, 3});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0, 1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 2, 0, 0, 2, 3, 1, 1, 3, 1, 1};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
//        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Too_Big_Cells_12_Squares_12_isotropic_Cells_Case_2) {
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

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({2, 4, 5, 7, 8});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({9, 10, 11});

    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({2});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_SplitTooBigCoarseCellInTwo_1_cell) {

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

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 11, 14});

    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[12] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[12] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[15] = {0, 0, 0,
                                                             0, 0, 0,
                                                             1, 0, 0,
                                                             0, 0, 1,
                                                             0, 0, 1};
    for (int i = 0; i < 15; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
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

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_2;
    ref_dict_Coarse_Cells_2[0] = unordered_set<long>({0, 1, 2, 3, 4, 5, 8, 10, 13});
    ref_dict_Coarse_Cells_2[1] = unordered_set<long>({6, 11, 14});
    ref_dict_Coarse_Cells_2[2] = unordered_set<long>({7, 9, 12});
    ASSERT_EQ(ref_dict_Coarse_Cells_2, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_2;
    ref_dict_Card_Coarse_Cells_2[3] = unordered_set<long>({1, 2});
    ref_dict_Card_Coarse_Cells_2[9] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_2, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[3] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[9] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_2, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[15] = {0, 0, 0,
                                                               0, 0, 0,
                                                               1, 2, 0,
                                                               2, 0, 1,
                                                               2, 0, 1};
    for (int i = 0; i < 15; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fineCellIndicesToCoarseCellIndices[i]);
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

    ASSERT_EQ(4, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_3;
    ref_dict_Coarse_Cells_3[0] = unordered_set<long>({2, 4, 5, 8, 10, 13});
    ref_dict_Coarse_Cells_3[1] = unordered_set<long>({6, 11, 14});
    ref_dict_Coarse_Cells_3[2] = unordered_set<long>({7, 9, 12});
    ref_dict_Coarse_Cells_3[3] = unordered_set<long>({0, 1, 3});
    ASSERT_EQ(ref_dict_Coarse_Cells_3, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_3;
    ref_dict_Card_Coarse_Cells_3[3] = unordered_set<long>({1, 2, 3});
    ref_dict_Card_Coarse_Cells_3[6] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_3, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_3;
    ref_dict_DistributionOfCardinalOfCoarseElements_3[3] = 3;
    ref_dict_DistributionOfCardinalOfCoarseElements_3[6] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_3, dict_DistributionOfCardinalOfCoarseElements);

//    assert(indCoarseCell == 4);
//    assert(!dict_Coarse_Cells.empty());
//    assert(dict_Coarse_Cells.size() == 4);
//    assert(dict_Coarse_Cells[0].count(2) == 1);
//    assert(dict_Coarse_Cells[0].count(4) == 1);
//    assert(dict_Coarse_Cells[0].count(5) == 1);
//    assert(dict_Coarse_Cells[0].count(8) == 1);
//    assert(dict_Coarse_Cells[0].count(10) == 1);
//    assert(dict_Coarse_Cells[0].count(13) == 1);
//
//    assert(dict_Coarse_Cells[1].count(6) == 1);
//    assert(dict_Coarse_Cells[1].count(11) == 1);
//    assert(dict_Coarse_Cells[1].count(14) == 1);
//
//    assert(dict_Coarse_Cells[2].count(7) == 1);
//    assert(dict_Coarse_Cells[2].count(9) == 1);
//    assert(dict_Coarse_Cells[2].count(12) == 1);
//
//    assert(dict_Coarse_Cells[3].count(0) == 1);
//    assert(dict_Coarse_Cells[3].count(1) == 1);
//    assert(dict_Coarse_Cells[3].count(3) == 1);
//
//    assert(!dict_Card_Coarse_Cells.empty());
//    assert(dict_Card_Coarse_Cells.size() == 2);
//    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
//    assert(dict_Card_Coarse_Cells[3].count(2) == 1);
//    assert(dict_Card_Coarse_Cells[3].count(3) == 1);
//    assert(dict_Card_Coarse_Cells[6].count(0) == 1);
//
//    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
//    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 3);
//    assert(dict_DistributionOfCardinalOfCoarseElements[6] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_3[15] = {3, 3, 0,
                                                               3, 0, 0,
                                                               1, 2, 0,
                                                               2, 0, 1,
                                                               2, 0, 1};
    for (int i = 0; i < 15; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_3[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, Correction_SplitTooBigCoarseCellInTwo_1_cell_v2) {

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

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2, 3, 4, 5, 7, 8, 9, 12});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 10, 11, 13, 14});

    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[10] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements = {{5,  1},
                                                                                {10, 1}};
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[15] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1};

    for (int i = 0; i < 15; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
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

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_2;
    ref_dict_Coarse_Cells_2[0] = unordered_set<long>({4, 7, 8, 9, 12});
    ref_dict_Coarse_Cells_2[1] = unordered_set<long>({6, 10, 11, 13, 14});
    ref_dict_Coarse_Cells_2[2] = unordered_set<long>({0, 1, 2, 3, 5});

    ASSERT_EQ(ref_dict_Coarse_Cells_2, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_2;
    ref_dict_Card_Coarse_Cells_2[5] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_2, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[5] = 3;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_2, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[15] = {2, 2, 2, 2, 0, 2, 1, 0, 0, 0, 1, 1, 0, 1, 1};

    for (int i = 0; i < 15; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}


TEST(CoarseCellGraph_TestSuite, CreateDelayedCoarseCells_Case_1) {

    unordered_set<long> l1({4, 5, 6, 7});
    unordered_set<long> l2({8, 9, 10});

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

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
    createCoarseCell(l1, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices, true, true);

    ASSERT_EQ(2, indCoarseElement);
    ASSERT_EQ(8, numberOfFineAgglomeratedCells_tmp);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells = {{0, unordered_set<long>({3})},
                                                                      {1, unordered_set<long>({0, 1, 2})},};
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells = {{1, unordered_set<long>({0})},
                                                                          {3, unordered_set<long>({1})},};

    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[11] = {true, true, true, true, true, true, true, true, false, false, false,};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[11] = {1, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1};
    for (int i = 0; i < 11; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Creation of the first cell l2:
    //===============================
    createCoarseCell(l2, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                     indCoarseElement, numberOfFineAgglomeratedCells_tmp,
                     isFineCellAgglomerated_tmp,
                     fine_Cell_indices_To_Coarse_Cell_Indices, true, true);

    ASSERT_EQ(2, indCoarseElement);
    ASSERT_EQ(11, numberOfFineAgglomeratedCells_tmp);

    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);


//    assert(indCoarseElement == 2);
//    assert(numberOfFineAgglomeratedCells_tmp == 11);

//    assert(dict_Coarse_Cells.size() == 2);
//    assert(dict_Coarse_Cells[0].size() == 1);
//    assert(dict_Coarse_Cells[0].count(3) == 1);
//
//    assert(dict_Coarse_Cells[1].size() == 3);
//    assert(dict_Coarse_Cells[1].count(0) == 1);
//    assert(dict_Coarse_Cells[1].count(1) == 1);
//    assert(dict_Coarse_Cells[1].count(2) == 1);
//
//    assert(dict_Card_Coarse_Cells.size() == 2);
//    assert(dict_Card_Coarse_Cells[1].size() == 1);
//    assert(dict_Card_Coarse_Cells[1].count(0) == 1);
//
//
//    assert(dict_Card_Coarse_Cells[3].size() == 1);
//    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
//
//    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
//    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
//    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);
//    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

    bool ref_isFineCellAgglomerated_tmp_2[11] = {true, true, true, true, true, true, true, true, true, true, true};
//    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[11] = {1, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1};
    for (int i = 0; i < 11; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    // Real Creation:
    list<unordered_set<long>> delayedCoarseCells;
    delayedCoarseCells.push_back(l1);
    delayedCoarseCells.push_back(l2);
    agglomerate_Isotropic_CreateDelayedCoarseCells(dict_Coarse_Cells,
                                                   dict_Card_Coarse_Cells,
                                                   delayedCoarseCells, indCoarseElement, fine_Cell_indices_To_Coarse_Cell_Indices);

    ASSERT_EQ(4, indCoarseElement);
    ASSERT_EQ(11, numberOfFineAgglomeratedCells_tmp);

    unorderedMap ref_dict_Coarse_Cells_2 = {{0, unordered_set<long>({3})},
                                            {1, unordered_set<long>({0, 1, 2})},
                                            {2, unordered_set<long>({4, 5, 6, 7})},
                                            {3, unordered_set<long>({8, 9, 10})}};
    ASSERT_EQ(ref_dict_Coarse_Cells_2, dict_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_2 = {{1, 1},
                                                                                  {3, 2},
                                                                                  {4, 1}};
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_2, dict_DistributionOfCardinalOfCoarseElements);


//    assert(indCoarseElement == 4);
//    assert(numberOfFineAgglomeratedCells_tmp == 11);

//    assert(dict_Coarse_Cells.size() == 4);
//    assert(dict_Coarse_Cells[0].size() == 1);
//    assert(dict_Coarse_Cells[0].count(3) == 1);
//
//    assert(dict_Coarse_Cells[1].size() == 3);
//    assert(dict_Coarse_Cells[1].count(0) == 1);
//    assert(dict_Coarse_Cells[1].count(1) == 1);
//    assert(dict_Coarse_Cells[1].count(2) == 1);
//
//    assert(dict_Coarse_Cells[2].size() == 4);
//    assert(dict_Coarse_Cells[2].count(4) == 1);
//    assert(dict_Coarse_Cells[2].count(5) == 1);
//    assert(dict_Coarse_Cells[2].count(6) == 1);
//    assert(dict_Coarse_Cells[2].count(7) == 1);

//    assert(dict_Coarse_Cells[3].size() == 3);
//    assert(dict_Coarse_Cells[3].count(8) == 1);
//    assert(dict_Coarse_Cells[3].count(9) == 1);
//    assert(dict_Coarse_Cells[3].count(10) == 1);

    assert(dict_Card_Coarse_Cells.size() == 3);
//    assert(dict_Card_Coarse_Cells[1].size() == 1);
//    assert(dict_Card_Coarse_Cells[1].count(0) == 1);
//
//    assert(dict_Card_Coarse_Cells[3].size() == 2);
//    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
//    assert(dict_Card_Coarse_Cells[3].count(3) == 1);
//
//    assert(dict_Card_Coarse_Cells[4].size() == 1);
//    assert(dict_Card_Coarse_Cells[4].count(2) == 1);
//
//    assert(dict_DistributionOfCardinalOfCoarseElements.size() == 3);
//    assert(dict_DistributionOfCardinalOfCoarseElements[1] == 1);
//
//    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 2);
//
//    assert(dict_DistributionOfCardinalOfCoarseElements[4] == 1);

//    self.assertEqual({1: 1, 3: 1, 4: 1}, dict_DistributionOfCardinalOfCoarseElements)
    bool ref_isFineCellAgglomerated_tmp_3[11] = {true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[11] = {1, 1, 1, 0, 2, 2, 2, 2, 3, 3, 3};
    for (int i = 0; i < 11; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_3[i], isFineCellAgglomerated_tmp[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
//
//        assert(isFineCellAgglomerated_tmp[i] == ref_isFineCellAgglomerated_tmp_3[i]);
//        assert(fine_Cell_indices_To_Coarse_Cell_Indices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i]);
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
