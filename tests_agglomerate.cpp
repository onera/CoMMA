//
// Created by Nicolas Lantos on 18/10/2017.
//
//#include "gtest/gtest.h"
#include "Agglomerator_Isotropic.h"
#include "Agglomerator.h"
#include<assert.h>
#include<iostream>


using namespace std;

int test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_8() {

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

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                            2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                            2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                            3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int minCard = 6;
    int goalCard = 8;
    int maxCard = 10;

    long indCoarseCell = 0;

    long checks = 1;
    long verbose = 1;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    long *arrayOfFineAnisotropicCompliantCells = new long[64];
    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long *agglomerationLines_Idx = new long[64];
    long *agglomerationLines = new long[64];

    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];
    long isFirstAgglomeration = 1;
    long isAnisotropic = 0;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = 64;
    long agglomerationLines_Idx_size = 64;
    long agglomerationLines_size = 64;

    long sizes[10] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerateOneLevel(sizes,
                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                        arrayOfFineAnisotropicCompliantCells,

                        isOnFineBnd,
                        array_isOnValley,
                        array_isOnRidge,
                        array_isOnCorner,

                        isFirstAgglomeration,
                        isAnisotropic,

                        fineCellIndicesToCoarseCellIndices,

                        agglomerationLines_Idx,
                        agglomerationLines,

                        dimension,
                        goalCard,
                        minCard,
                        maxCard,
                        checks,
                        verbose);
    cout << "sizes[0] " << sizes[0] << endl;
    cout << "sizes[1] " << sizes[1] << endl;
    cout << "sizes[2] " << sizes[2] << endl;
    cout << "sizes[3] " << sizes[3] << endl;
    cout << "sizes[4] " << sizes[4] << endl;
    cout << "sizes[5] " << sizes[5] << endl;
    cout << "sizes[6] " << sizes[6] << endl;
    cout << "sizes[7] " << sizes[7] << endl;
    cout << "sizes[8] " << sizes[8] << endl;
    cout << "sizes[9] " << sizes[9] << endl;

    assert(sizes[0] == 64);
    assert(sizes[1] == 344);
    assert(sizes[2] == 8);//indCoarseCell
    assert(sizes[3] == 64);//numberOfFineAgglomeratedCells
    assert(sizes[4] == 24);//isOnValley_size
    assert(sizes[5] == 24);//isOnRidge_size
    assert(sizes[6] == 8);//isOnCorner_size
    assert(sizes[7] == 64);//arrayOfFineAnisotropicCompliantCells_size
    assert(sizes[8] == 64);//agglomerationLines_Idx_size
    assert(sizes[9] == 64);//agglomerationLines_size




    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 3, 3, 5, 5, 3, 3, 7, 7, 6, 6, 7, 7, 6, 6, 5, 5, 3, 3, 5, 5, 3, 3, 2, 2, 4, 4, 2, 2, 4, 4, 1, 1, 0, 0, 1, 1,
                                                             0, 0, 2, 2, 4, 4, 2, 2, 4, 4, 1, 1, 0, 0, 1, 1, 0, 0};
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


int test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_9() {

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

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                            2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                            2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                            3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int minCard = 6;
    int goalCard = 9;
    int maxCard = 10;

    long indCoarseCell = 0;


    long checks = 1;
    long verbose = 1;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    long *arrayOfFineAnisotropicCompliantCells = new long[64];
    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long *agglomerationLines_Idx = new long[64];
    long *agglomerationLines = new long[64];

    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];
    long isFirstAgglomeration = 1;
    long isAnisotropic = 0;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = 64;
    long agglomerationLines_Idx_size = 64;
    long agglomerationLines_size = 64;

    long sizes[10] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerateOneLevel(sizes,
                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                        arrayOfFineAnisotropicCompliantCells,

                        isOnFineBnd,
                        array_isOnValley,
                        array_isOnRidge,
                        array_isOnCorner,

                        isFirstAgglomeration,
                        isAnisotropic,

                        fineCellIndicesToCoarseCellIndices,

                        agglomerationLines_Idx,
                        agglomerationLines,

                        dimension,
                        goalCard,
                        minCard,
                        maxCard,
                        checks,
                        verbose);
//    cout<<"sizes[0] "<<sizes[0]<<endl;
//    cout<<"sizes[1] "<<sizes[1]<<endl;
//    cout<<"sizes[2] "<<sizes[2]<<endl;
//    cout<<"sizes[3] "<<sizes[3]<<endl;
//    cout<<"sizes[4] "<<sizes[4]<<endl;
//    cout<<"sizes[5] "<<sizes[5]<<endl;
//    cout<<"sizes[6] "<<sizes[6]<<endl;
//    cout<<"sizes[7] "<<sizes[7]<<endl;
//    cout<<"sizes[8] "<<sizes[8]<<endl;
//    cout<<"sizes[9] "<<sizes[9]<<endl;

    assert(sizes[0] == 64);
    assert(sizes[1] == 344);
    assert(sizes[2] == 7);//indCoarseCell
    assert(sizes[3] == 64);//numberOfFineAgglomeratedCells

    assert(sizes[4] == 24);//isOnValley_size
    assert(sizes[5] == 24);//isOnRidge_size
    assert(sizes[6] == 8);//isOnCorner_size
    assert(sizes[7] == 64);//arrayOfFineAnisotropicCompliantCells_size
    assert(sizes[8] == 64);//agglomerationLines_Idx_size
    assert(sizes[9] == 64);//agglomerationLines_size

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {2, 6, 6, 6, 5, 6, 6, 3, 5, 5, 3, 3, 5, 5, 3, 3, 2, 6, 6, 6, 5, 6, 6, 3, 5, 5, 3, 3, 5, 5, 3, 3, 2, 2, 4, 4, 2, 2, 4, 4, 1, 1, 0, 0, 1, 1,
                                                             0, 0, 2, 2, 4, 4, 2, 2, 4, 4, 1, 1, 0, 0, 1, 1, 0, 0};



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


int test_agglomerateOneLevel_Box_5x5x5_Anisotropic() {

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

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                            2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                            2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                            3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    list<unordered_set<long>> delayedCoarseCells;
    int thresholdCard = 3;
    int minCard = 6;
    int goalCard = 8;
    int maxCard = 10;

    long indCoarseCell = 0;


    long checks = 1;
    long verbose = 1;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    long *arrayOfFineAnisotropicCompliantCells = new long[64];
    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long *agglomerationLines_Idx = new long[64];
    long *agglomerationLines = new long[64];

    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//    long indCoarseCell = sizes[2];
//    long numberOfFineAgglomeratedCells = sizes[3];
    long isFirstAgglomeration = 1;
    long isAnisotropic = 1;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = 64;
    long agglomerationLines_Idx_size = 64;
    long agglomerationLines_size = 64;

    long sizes[10] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerateOneLevel(sizes,
                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                        arrayOfFineAnisotropicCompliantCells,

                        isOnFineBnd,
                        array_isOnValley,
                        array_isOnRidge,
                        array_isOnCorner,

                        isFirstAgglomeration,
                        isAnisotropic,

                        fineCellIndicesToCoarseCellIndices,

                        agglomerationLines_Idx,
                        agglomerationLines,

                        dimension,
                        goalCard,
                        minCard,
                        maxCard,
                        checks,
                        verbose);
    cout << "sizes[0] " << sizes[0] << endl;
    cout << "sizes[1] " << sizes[1] << endl;
    cout << "sizes[2] " << sizes[2] << endl;
    cout << "sizes[3] " << sizes[3] << endl;
    cout << "sizes[4] " << sizes[4] << endl;
    cout << "sizes[5] " << sizes[5] << endl;
    cout << "sizes[6] " << sizes[6] << endl;
    cout << "sizes[7] " << sizes[7] << endl;
    cout << "sizes[8] " << sizes[8] << endl;
    cout << "sizes[9] " << sizes[9] << endl;

    assert(sizes[0] == 64);
    assert(sizes[1] == 344);
    assert(sizes[2] == 18);//indCoarseCell
    assert(sizes[3] == 64);//numberOfFineAgglomeratedCells

    assert(sizes[4] == 24);//isOnValley_size
    assert(sizes[5] == 24);//isOnRidge_size
    assert(sizes[6] == 8);//isOnCorner_size
    assert(sizes[7] == 16);//arrayOfFineAnisotropicCompliantCells_size
    assert(sizes[8] == 17);//agglomerationLines_Idx_size
    assert(sizes[9] == 16);//agglomerationLines_size

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {0, 5, 4, 2, 9, 13, 12, 8, 7, 15, 14, 6, 3, 11, 10, 1, 0, 5, 4, 2, 9, 13, 12, 8, 7, 15, 14, 6, 3, 11, 10, 1, 0, 5, 4, 2, 9, 13, 12, 8, 7,
                                                             15, 14, 6, 3, 11, 10, 1, 17, 17, 17, 17, 17, 17, 16, 16, 17, 16, 16, 16, 17, 16, 16, 16};
//            1, 6, 5, 2,
//                                                             8, 13, 14, 9,
//                                                             10, 12, 15, 11,
//                                                             3, 4, 7, 0,
//
//                                                             1, 6, 5, 2,
//                                                             8, 13, 14, 9,
//                                                             10, 12, 15, 11,
//                                                             3, 4, 7, 0,
//
//                                                             1, 6, 5, 2,
//                                                             8, 13, 14, 9,
//                                                             10, 12, 15, 11,
//                                                             3, 4, 7, 0,
//
//                                                             16, 16, 16, 17,
//                                                             16, 16, 16, 17,
//                                                             16, 16, 17, 17,
//                                                             17, 17, 17, 17};



//    cout<<"\nfineCellIndicesToCoarseCellIndices: [";
//    for(auto i:fineCellIndicesToCoarseCellIndices){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    for (int i = 0; i < 64; i++) {
//        cout<<i<<", "<<fineCellIndicesToCoarseCellIndices[i]<<" ref= "<<ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]<<endl;
        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    long AnisotropicCompliantCoarseCells_size = sizes[7];

    long ref_AnisotropicCompliantCoarseCells_array[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    for (int i = 0; i < AnisotropicCompliantCoarseCells_size; i++) {
//        cout <<arrayOfFineAnisotropicCompliantCells[i]<<", ";
        assert(ref_AnisotropicCompliantCoarseCells_array[i] == arrayOfFineAnisotropicCompliantCells[i]);
    }
//    cout<<endl;

    long fineAgglomerationLines_array_Idx_size = sizes[8];
    assert(fineAgglomerationLines_array_Idx_size - 1 == 16);

    long ref_agglomerationLines_Idx[17] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    for (int i = 0; i < fineAgglomerationLines_array_Idx_size; i++) {
//        cout <<agglomerationLines_Idx[i]<<", ";
        assert(ref_agglomerationLines_Idx[i] == agglomerationLines_Idx[i]);
    }
    long ref_fineAgglomerationLines_array[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    for (int i = 0; i < agglomerationLines_Idx[fineAgglomerationLines_array_Idx_size - 1]; i++) {
        assert(ref_fineAgglomerationLines_array[i] == agglomerationLines[i]);
    }


    return 1;
}

int test_agglomerateOneLevel_MGridGen() {

    long nb_fc = 64;
    long adjMatrix_col_ind_size = 344;
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

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnFineBnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3,
                            2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                            2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2,
                            3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};

    // Initialization of datas for agglomeration
    ////////////////////////////////////////////
    bool isFineCellAgglomerated[nb_fc];
    for (int i = 0; i < nb_fc; i++) {
        isFineCellAgglomerated[i] = false;
    }
    long fineCellIndicesToCoarseCellIndices[nb_fc];
    for (int i = 0; i < nb_fc; i++) {
        fineCellIndicesToCoarseCellIndices[i] = -1;
    }

    long indCoarseCell = 0;

    // Parameters for agglomeration
    ////////////////////////////////////////////

    int thresholdCard = 3;
    int minCard = 6;
    int goalCard = 8;
    int maxCard = 10;

    long checks = 1;
    long verbose = 1;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    long *arrayOfFineAnisotropicCompliantCells = new long[nb_fc];
    for (int i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long *agglomerationLines_Idx = new long[nb_fc];
    long *agglomerationLines = new long[nb_fc];

    for (int i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long isFirstAgglomeration = 1;
    long isAnisotropic = 1;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = nb_fc;
    long agglomerationLines_Idx_size = nb_fc;
    long agglomerationLines_size = nb_fc;

    long sizes[10] = {nb_fc, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerateOneLevel(sizes,
                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                        arrayOfFineAnisotropicCompliantCells,

                        isOnFineBnd,
                        array_isOnValley,
                        array_isOnRidge,
                        array_isOnCorner,

                        isFirstAgglomeration,
                        isAnisotropic,

                        fineCellIndicesToCoarseCellIndices,

                        agglomerationLines_Idx,
                        agglomerationLines,

                        dimension,
                        goalCard,
                        minCard,
                        maxCard,
                        checks,
                        verbose);
    cout << "sizes[0] " << sizes[0] << endl;
    cout << "sizes[1] " << sizes[1] << endl;
    cout << "sizes[2] " << sizes[2] << endl;
    cout << "sizes[3] " << sizes[3] << endl;
    cout << "sizes[4] " << sizes[4] << endl;
    cout << "sizes[5] " << sizes[5] << endl;
    cout << "sizes[6] " << sizes[6] << endl;
    cout << "sizes[7] " << sizes[7] << endl;
    cout << "sizes[8] " << sizes[8] << endl;
    cout << "sizes[9] " << sizes[9] << endl;

    assert(sizes[0] == 64);
    assert(sizes[1] == 344);
    assert(sizes[2] == 18);//indCoarseCell
    assert(sizes[3] == 64);//numberOfFineAgglomeratedCells

    assert(sizes[4] == 24);//isOnValley_size
    assert(sizes[5] == 24);//isOnRidge_size
    assert(sizes[6] == 8);//isOnCorner_size
    assert(sizes[7] == 16);//arrayOfFineAnisotropicCompliantCells_size
    assert(sizes[8] == 17);//agglomerationLines_Idx_size
    assert(sizes[9] == 16);//agglomerationLines_size

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {0, 5, 4, 2, 9, 13, 12, 8, 7, 15, 14, 6, 3, 11, 10, 1, 0, 5, 4, 2, 9, 13, 12, 8, 7, 15, 14, 6, 3, 11, 10, 1, 0, 5, 4, 2, 9, 13, 12, 8, 7,
                                                             15, 14, 6, 3, 11, 10, 1, 17, 17, 17, 17, 17, 17, 16, 16, 17, 16, 16, 16, 17, 16, 16, 16};
//            1, 6, 5, 2,
//                                                             8, 13, 14, 9,
//                                                             10, 12, 15, 11,
//                                                             3, 4, 7, 0,
//
//                                                             1, 6, 5, 2,
//                                                             8, 13, 14, 9,
//                                                             10, 12, 15, 11,
//                                                             3, 4, 7, 0,
//
//                                                             1, 6, 5, 2,
//                                                             8, 13, 14, 9,
//                                                             10, 12, 15, 11,
//                                                             3, 4, 7, 0,
//
//                                                             16, 16, 16, 17,
//                                                             16, 16, 16, 17,
//                                                             16, 16, 17, 17,
//                                                             17, 17, 17, 17};



//    cout<<"\nfineCellIndicesToCoarseCellIndices: [";
//    for(auto i:fineCellIndicesToCoarseCellIndices){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    for (int i = 0; i < 64; i++) {
//        cout<<i<<", "<<fineCellIndicesToCoarseCellIndices[i]<<" ref= "<<ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]<<endl;
        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

    long AnisotropicCompliantCoarseCells_size = sizes[7];

    long ref_AnisotropicCompliantCoarseCells_array[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    for (int i = 0; i < AnisotropicCompliantCoarseCells_size; i++) {
//        cout <<arrayOfFineAnisotropicCompliantCells[i]<<", ";
        assert(ref_AnisotropicCompliantCoarseCells_array[i] == arrayOfFineAnisotropicCompliantCells[i]);
    }
//    cout<<endl;

    long fineAgglomerationLines_array_Idx_size = sizes[8];
    assert(fineAgglomerationLines_array_Idx_size - 1 == 16);

    long ref_agglomerationLines_Idx[17] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    for (int i = 0; i < fineAgglomerationLines_array_Idx_size; i++) {
//        cout <<agglomerationLines_Idx[i]<<", ";
        assert(ref_agglomerationLines_Idx[i] == agglomerationLines_Idx[i]);
    }
    long ref_fineAgglomerationLines_array[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    for (int i = 0; i < agglomerationLines_Idx[fineAgglomerationLines_array_Idx_size - 1]; i++) {
        assert(ref_fineAgglomerationLines_array[i] == agglomerationLines[i]);
    }


    return 1;
}


