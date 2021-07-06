#include "../CoMMA_lib/CoMMA.h"

//#include "MGridGen_Dual_Graph.h"
#include "gtest/gtest.h"


TEST(CoMMA_TestSuite, agglomerate_one_level_basic_MGridGen) {

    // MGridGen Test case
    int nb_fc = 15;
    long adjMatrix_col_ind_size = 51;

    long adjMatrix_row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long adjMatrix_col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                                  7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                                  14};
    double adjMatrix_areaValues[51] = {6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
                                       6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
                                       2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
                                       3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
                                       3.16227766, 7.};

    double volumes[15] = {2., 1., 2., 1., 2., 1., 2., 1., 0.5, 2., 1., 3., 3., 3., 1.5};
    long isOnBnd[15] = {1, 1, 0, 1, 1,
                        0, 0, 0, 0, 0,
                        0, 0, 2, 1, 2};

    long numberOfFineAgglomeratedCells = 0;  //TODO remove this

    long isOnRidge_size = 5;
    long isOnValley_size = 2;
    long isOnCorner_size = 0;

    long array_isOnRidge[5] = {0, 1, 3, 4, 13};
    long array_isOnValley[2] = {12, 14};
    long array_isOnCorner[0] = {};


    // Initialization of datas for agglomeration
    ////////////////////////////////////////////
    long fc_to_cc[nb_fc];
    for (int i = 0; i < nb_fc; i++) {
        fc_to_cc[i] = -1;
    }

    long indCoarseCell = 0;

    // Parameters for agglomeration
    ////////////////////////////////////////////
    int minCard = -1;
    int goalCard = -1;
    int maxCard = -1;

    long checks = 1;
    long verbose = 0;

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

    long isFirstAgglomeration = 1;  //TODO remove this
    long isAnisotropic = 0;
    long dimension = 2;

    long arrayOfFineAnisotropicCompliantCells_size = nb_fc;
    long agglomerationLines_Idx_size = nb_fc;
    long agglomerationLines_size = nb_fc;

    long sizes[10] = {nb_fc, adjMatrix_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
                          arrayOfFineAnisotropicCompliantCells,

                          isOnBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomerationLines_Idx,
                          agglomerationLines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);
//    cout << "sizes[0] " << sizes[0] << endl;
//    cout << "sizes[1] " << sizes[1] << endl;
//    cout << "sizes[2] " << sizes[2] << endl;
//    cout << "sizes[3] " << sizes[3] << endl;
//    cout << "sizes[4] " << sizes[4] << endl;
//    cout << "sizes[5] " << sizes[5] << endl;
//    cout << "sizes[6] " << sizes[6] << endl;
//    cout << "sizes[7] " << sizes[7] << endl;
//    cout << "sizes[8] " << sizes[8] << endl;
//    cout << "sizes[9] " << sizes[9] << endl;

    EXPECT_EQ(nb_fc, sizes[0]);
    EXPECT_EQ(adjMatrix_col_ind_size, sizes[1]);
    EXPECT_EQ(4, sizes[2]);//indCoarseCell
//    EXPECT_EQ(15, sizes[3]);//numberOfFineAgglomeratedCells
//    EXPECT_EQ(2, sizes[4]);//isOnValley_size
//    EXPECT_EQ(5, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(0, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(15, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
//    EXPECT_EQ(15, sizes[8]);//agglomerationLines_Idx_size
//    EXPECT_EQ(15, sizes[9]);//agglomerationLines_size

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[15] = {1, 1, 1, 1, 2, 2, 1, 3, 2, 3, 2, 0, 3, 0, 0};
//    cout<<endl;
    for (int i = 0; i < nb_fc; i++) {
//        cout<<fc_to_cc[i]<<", ";
//        cout<<i<<", "<<fc_to_cc[i]<<" ref= "<<ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]<<endl;
        EXPECT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fc_to_cc[i]);
    }
}

TEST(CoMMA_TestSuite, agglomerate_one_level_3D_basic_Box_5x5x5_Isotropic_goalCard_8) {

    // 64 Hexahedra
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62,
                                  66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132,
                                  138, 144, 150, 155, 161, 167, 172, 177, 183, 189, 194, 200, 206,
                                  212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278,
                                  282, 287, 293, 299, 304, 309, 315, 321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind[344] = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7,
                                   19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10, 22, 3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25,
                                   6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28, 9, 12,
                                   13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32,
                                   1, 16, 17, 18, 21, 33, 2, 17, 18, 19, 22, 34, 3, 18, 19, 23, 35,
                                   4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26,
                                   38, 7, 19, 22, 23, 27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26,
                                   29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24,
                                   28, 29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30,
                                   31, 47, 16, 32, 33, 36, 48, 17, 32, 33, 34, 37, 49, 18, 33, 34, 35,
                                   38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38,
                                   41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40,
                                   41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39,
                                   42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42,
                                   45, 46, 47, 62, 31, 43, 46, 47, 63, 32, 48, 49, 52, 33, 48, 49, 50,
                                   53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49,
                                   52, 53, 54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52,
                                   56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58, 59, 62, 43, 55,
                                   58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63,
                                   47, 59, 62, 63};

    double adjMatrix_areaValues[344] = {75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 75., 25., 25., 25., 50., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25.,
                                        25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25., 75., 25.,
                                        25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        75., 25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 50., 25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 50., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25., 50., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 50., 25., 25., 25., 50., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50.,
                                        25., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 75.};

    double volumes[64] = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125.};


    long fc_to_cc[64];
    for (int i = 0; i < 64; i++) {
        fc_to_cc[i] = -1;
    }

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long isOnFineBnd[64] = {3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3};

    int minCard = -1;
    int goalCard = -1;
    int maxCard = -1;

    long indCoarseCell = 0;  //TODO remove this

    long checks = 1;
    long verbose = 0;

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

    long isFirstAgglomeration = 1;
    long isAnisotropic = 0;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = 64;
    long agglomerationLines_Idx_size = 64;
    long agglomerationLines_size = 64;

    long sizes[10] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                          arrayOfFineAnisotropicCompliantCells,

                          isOnFineBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomerationLines_Idx,
                          agglomerationLines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);

    EXPECT_EQ(64, sizes[0]);
    EXPECT_EQ(344, sizes[1]);
    EXPECT_EQ(8, sizes[2]);//indCoarseCell
//    EXPECT_EQ(24, sizes[4]);//isOnValley_size
//    EXPECT_EQ(24, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(8, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(64, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
//    EXPECT_EQ(64, sizes[8]);//agglomerationLines_Idx_size
//    EXPECT_EQ(64, sizes[9]);//agglomerationLines_size

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2,
                                                             4, 4, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4};
//    cout << "\nfc_to_cc: [";
//    for (auto i:fc_to_cc) {
//        cout << i << ", ";
//    }
//    cout << "]" << endl;

    for (int i = 0; i < 64; i++) {

        EXPECT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fc_to_cc[i]);
    }
}

TEST(CoMMA_TestSuite, agglomerate_one_level_3D_basic_Box_5x5x5_Isotropic_goalCard_9) {

    // 64 Hexahedra
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62,
                                  66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132,
                                  138, 144, 150, 155, 161, 167, 172, 177, 183, 189, 194, 200, 206,
                                  212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278,
                                  282, 287, 293, 299, 304, 309, 315, 321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind[344] = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7,
                                   19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10, 22, 3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25,
                                   6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28, 9, 12,
                                   13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32,
                                   1, 16, 17, 18, 21, 33, 2, 17, 18, 19, 22, 34, 3, 18, 19, 23, 35,
                                   4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26,
                                   38, 7, 19, 22, 23, 27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26,
                                   29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24,
                                   28, 29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30,
                                   31, 47, 16, 32, 33, 36, 48, 17, 32, 33, 34, 37, 49, 18, 33, 34, 35,
                                   38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38,
                                   41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40,
                                   41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39,
                                   42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42,
                                   45, 46, 47, 62, 31, 43, 46, 47, 63, 32, 48, 49, 52, 33, 48, 49, 50,
                                   53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49,
                                   52, 53, 54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52,
                                   56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58, 59, 62, 43, 55,
                                   58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63,
                                   47, 59, 62, 63};

    double adjMatrix_areaValues[344] = {75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 75., 25., 25., 25., 50., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25.,
                                        25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25., 75., 25.,
                                        25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        75., 25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 50., 25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 50., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25., 50., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 50., 25., 25., 25., 50., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50.,
                                        25., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 75.};

    double volumes[64] = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125.};


    long fc_to_cc[64];
    for (int i = 0; i < 64; i++) {
        fc_to_cc[i] = -1;
    }

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long isOnFineBnd[64] = {3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3};

    int minCard = -1;
    int goalCard = 9;
    int maxCard = -1;

    long indCoarseCell = 0;  //TODO remove this

    long checks = 1;
    long verbose = 0;

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

    long isFirstAgglomeration = 1;
    long isAnisotropic = 0;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = 64;
    long agglomerationLines_Idx_size = 64;
    long agglomerationLines_size = 64;

    long sizes[10] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                          arrayOfFineAnisotropicCompliantCells,

                          isOnFineBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomerationLines_Idx,
                          agglomerationLines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);

    EXPECT_EQ(64, sizes[0]);
    EXPECT_EQ(344, sizes[1]);

    EXPECT_EQ(8, sizes[2]);//indCoarseCell
//    EXPECT_EQ(24, sizes[4]);//isOnValley_size
//    EXPECT_EQ(24, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(8, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(64, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
//    EXPECT_EQ(64, sizes[8]);//agglomerationLines_Idx_size
//    EXPECT_EQ(64, sizes[9]);//agglomerationLines_size

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {2, 2, 4, 4,
                                                             2, 2, 4, 4,
                                                             2, 2, 1, 1,
                                                             0, 0, 1, 1,
            //
                                                             2, 2, 4, 4, 2, 2, 4, 4, 0, 0, 1, 1, 0, 0, 1, 1, 6, 6, 6, 7, 6, 6, 7, 7, 3, 3, 5, 5, 3, 3, 5, 5, 6, 6, 6, 7, 6, 6, 7, 7, 3, 3, 5, 5, 3, 3,
                                                             5, 5};
//    cout << "\nfc_to_cc: [";
//    for (auto i:fc_to_cc) {
//        cout << i << ", ";
//    }
//    cout << "]" << endl;

    for (int i = 0; i < 64; i++) {

        EXPECT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fc_to_cc[i]);
    }
}

TEST(CoMMA_TestSuite, agglomerate_one_level_3D_basic_Box_5x5x5_anisotropic_goalCard_8) {

    // 64 Hexahedra
    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62,
                                  66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132,
                                  138, 144, 150, 155, 161, 167, 172, 177, 183, 189, 194, 200, 206,
                                  212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278,
                                  282, 287, 293, 299, 304, 309, 315, 321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind[344] = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7,
                                   19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10, 22, 3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25,
                                   6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28, 9, 12,
                                   13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32,
                                   1, 16, 17, 18, 21, 33, 2, 17, 18, 19, 22, 34, 3, 18, 19, 23, 35,
                                   4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26,
                                   38, 7, 19, 22, 23, 27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26,
                                   29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24,
                                   28, 29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30,
                                   31, 47, 16, 32, 33, 36, 48, 17, 32, 33, 34, 37, 49, 18, 33, 34, 35,
                                   38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38,
                                   41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40,
                                   41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39,
                                   42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42,
                                   45, 46, 47, 62, 31, 43, 46, 47, 63, 32, 48, 49, 52, 33, 48, 49, 50,
                                   53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49,
                                   52, 53, 54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52,
                                   56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58, 59, 62, 43, 55,
                                   58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63,
                                   47, 59, 62, 63};

    double adjMatrix_areaValues[344] = {75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 75., 25., 25., 25., 50., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25.,
                                        25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25., 75., 25.,
                                        25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        75., 25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 50., 25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 50., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25.,
                                        25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25., 50., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 50., 25., 25., 25., 50., 25., 25., 25., 25., 25.,
                                        25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50.,
                                        25., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                                        50., 25., 25., 25., 25., 75.};

    double volumes[64] = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125.};


    long fc_to_cc[64];
    for (int i = 0; i < 64; i++) {
        fc_to_cc[i] = -1;
    }

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long isOnFineBnd[64] = {3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3};

    int minCard = -1;
    int goalCard = -1;
    int maxCard = -1;

    long indCoarseCell = 0;  //TODO remove this

    long checks = 1;
    long verbose = 0;

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

    long isFirstAgglomeration = 1;
    long isAnisotropic = 1;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = 64;
    long agglomerationLines_Idx_size = 64;
    long agglomerationLines_size = 64;

    long sizes[10] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                          arrayOfFineAnisotropicCompliantCells,

                          isOnFineBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomerationLines_Idx,
                          agglomerationLines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);

    EXPECT_EQ(64, sizes[0]);
    EXPECT_EQ(344, sizes[1]);
    EXPECT_EQ(8, sizes[2]);//indCoarseCell
//    EXPECT_EQ(24, sizes[4]);//isOnValley_size
//    EXPECT_EQ(24, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(8, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(64, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
//    EXPECT_EQ(64, sizes[8]);//agglomerationLines_Idx_size
//    EXPECT_EQ(64, sizes[9]);//agglomerationLines_size

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2,
                                                             4, 4, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4};
    cout << "\nfc_to_cc: [";
    for (auto i:fc_to_cc) {
        cout << i << ", ";
    }
    cout << "]" << endl;

    for (int i = 0; i < 64; i++) {

        EXPECT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fc_to_cc[i]);
    }
}

TEST(CoMMA_TestSuite, agglomerate_one_level_3D_basic_Box_5x5x5_aniso) {

    // 64 Hexahedra: anisotropic cells

    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62,
                                  66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132,
                                  138, 144, 150, 155, 161, 167, 172, 177, 183, 189, 194, 200, 206,
                                  212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278,
                                  282, 287, 293, 299, 304, 309, 315, 321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind[344] = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7,
                                   19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10, 22, 3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25,
                                   6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28, 9, 12,
                                   13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32,
                                   1, 16, 17, 18, 21, 33, 2, 17, 18, 19, 22, 34, 3, 18, 19, 23, 35,
                                   4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26,
                                   38, 7, 19, 22, 23, 27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26,
                                   29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24,
                                   28, 29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30,
                                   31, 47, 16, 32, 33, 36, 48, 17, 32, 33, 34, 37, 49, 18, 33, 34, 35,
                                   38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38,
                                   41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40,
                                   41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39,
                                   42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42,
                                   45, 46, 47, 62, 31, 43, 46, 47, 63, 32, 48, 49, 52, 33, 48, 49, 50,
                                   53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49,
                                   52, 53, 54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52,
                                   56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58, 59, 62, 43, 55,
                                   58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63,
                                   47, 59, 62, 63};

    double adjMatrix_areaValues[344] = {30., 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 27.5,
                                        2.5, 2.5, 25., 2.5, 30., 2.5, 25., 2.5, 27.5, 2.5, 2.5,
                                        25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5,
                                        2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5,
                                        25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5,
                                        2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 30., 2.5, 25.,
                                        2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5,
                                        2.5, 30., 25., 25., 5., 2.5, 2.5, 25., 25., 2.5, 2.5,
                                        2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5,
                                        5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5,
                                        2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25.,
                                        2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5,
                                        25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5,
                                        25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 2.5, 5., 25., 25., 5., 2.5, 2.5,
                                        25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5,
                                        2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5,
                                        2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5,
                                        2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25.,
                                        2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                                        25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 5., 25.,
                                        25., 30., 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5,
                                        27.5, 2.5, 2.5, 25., 2.5, 30., 2.5, 25., 2.5, 27.5, 2.5,
                                        2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25.,
                                        2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 27.5, 2.5,
                                        2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25.,
                                        2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 30., 2.5,
                                        25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25.,
                                        2.5, 2.5, 30.};

    double volumes[64] = {12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5,
                          12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5};


    long fc_to_cc[64];
    for (int i = 0; i < 64; i++) {
        fc_to_cc[i] = -1;
    }

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long isOnFineBnd[64] = {3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3};

    int minCard = -1;
    int goalCard = -1;
    int maxCard = -1;

    long indCoarseCell = 0;  //TODO remove this

    long checks = 1;
    long verbose = 0;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    long *arrayOfFineAnisotropicCompliantCells = new long[64];
    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long *agglomeration_lines_idx = new long[64];
    long *agglomeration_lines = new long[64];

    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long isFirstAgglomeration = 1;
    long isAnisotropic = 1;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = 64;
    long agglomerationLines_Idx_size = 64;
    long agglomerationLines_size = 64;

    long sizes[10] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                          arrayOfFineAnisotropicCompliantCells,

                          isOnFineBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomeration_lines_idx,
                          agglomeration_lines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);

    EXPECT_EQ(64, sizes[0]);
    EXPECT_EQ(344, sizes[1]);

    EXPECT_EQ(32, sizes[2]);  // indCoarseCell
//    EXPECT_EQ(24, sizes[4]);//isOnValley_size
//    EXPECT_EQ(24, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(8, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(64, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {1, 11, 13, 3, 9, 29, 31, 21, 15, 27, 25, 23, 5, 17, 19, 7,
                                                             1, 11, 13, 3, 9, 29, 31, 21, 15, 27, 25, 23, 5, 17, 19, 7,
            //
                                                             0, 10, 12, 2, 8, 28, 30, 20, 14, 26, 24, 22, 4, 16, 18, 6,
                                                             0, 10, 12, 2, 8, 28, 30, 20, 14, 26, 24, 22, 4, 16, 18, 6};

    cout << "\nfc_to_cc: [";
    for (auto i:fc_to_cc) {
        cout << i << ", ";
    }
    cout << "]" << endl;

    for (int i = 0; i < 64; i++) {

        EXPECT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fc_to_cc[i]);
    }

    // Check agglomeration lines level: l+1
    //======================================

    EXPECT_EQ(17, sizes[8]);//agglomerationLines_Idx_size
    EXPECT_EQ(32, sizes[9]);//agglomerationLines_size

    long ref_agglo_lines_array_idx[17] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    long ref_agglo_lines_array[64] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

    cout << endl;
    for (long i_fc = 0; i_fc < sizes[8]; i_fc++) {
        cout << agglomeration_lines_idx[i_fc] << ", ";
    }
    cout << endl;
    for (long i_fc = 0; i_fc < sizes[9]; i_fc++) {
        cout << agglomeration_lines[i_fc] << ", ";
    }
    cout << endl;
    for (int i = 0; i < sizes[8]; i++) {
        EXPECT_EQ(ref_agglo_lines_array_idx[i], agglomeration_lines_idx[i]);
    }

    for (int i = 0; i < sizes[9]; i++) {
        EXPECT_EQ(ref_agglo_lines_array[i], agglomeration_lines[i]);
    }
}

TEST(CoMMA_TestSuite, agglomerate_one_level_3D_basic_Box_5x5x5_aniso_iso) {

    // 64 Hexahedra: anisotropic cells

    long numberOfFineCells = 64;
    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[65] = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62,
                                  66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132,
                                  138, 144, 150, 155, 161, 167, 172, 177, 183, 189, 194, 200, 206,
                                  212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278,
                                  282, 287, 293, 299, 304, 309, 315, 321, 326, 330, 335, 340, 344};

    long adjMatrix_col_ind[344] = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7,
                                   19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10, 22, 3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25,
                                   6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28, 9, 12,
                                   13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32,
                                   1, 16, 17, 18, 21, 33, 2, 17, 18, 19, 22, 34, 3, 18, 19, 23, 35,
                                   4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26,
                                   38, 7, 19, 22, 23, 27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26,
                                   29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24,
                                   28, 29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30,
                                   31, 47, 16, 32, 33, 36, 48, 17, 32, 33, 34, 37, 49, 18, 33, 34, 35,
                                   38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38,
                                   41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40,
                                   41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39,
                                   42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42,
                                   45, 46, 47, 62, 31, 43, 46, 47, 63, 32, 48, 49, 52, 33, 48, 49, 50,
                                   53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49,
                                   52, 53, 54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52,
                                   56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58, 59, 62, 43, 55,
                                   58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63,
                                   47, 59, 62, 63};

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

    double volumes[64] = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                          125., 125., 125., 125., 125., 125., 125., 125., 125.};


    long fc_to_cc[64];
    for (int i = 0; i < 64; i++) {
        fc_to_cc[i] = -1;
    }

    long array_isOnRidge[24] = {1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62};
    long array_isOnValley[24] = {5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58};
    long array_isOnCorner[8] = {0, 3, 12, 15, 48, 51, 60, 63};

    long isOnRidge_size = 24;
    long isOnValley_size = 24;
    long isOnCorner_size = 8;

    long isOnFineBnd[64] = {3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            2, 1, 1, 2,
                            1, 0, 0, 1,
                            1, 0, 0, 1,
                            2, 1, 1, 2,
            //
                            3, 2, 2, 3,
                            2, 1, 1, 2,
                            2, 1, 1, 2,
                            3, 2, 2, 3};

    int minCard = -1;
    int goalCard = -1;
    int maxCard = -1;

    long indCoarseCell = 0;  //TODO remove this

    long checks = 1;
    long verbose = 0;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    long *arrayOfFineAnisotropicCompliantCells = new long[64];
    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long *agglomeration_lines_idx = new long[64];
    long *agglomeration_lines = new long[64];

    for (int i = 0; i < 64; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long isFirstAgglomeration = 1;
    long isAnisotropic = 1;
    long dimension = 3;

    long arrayOfFineAnisotropicCompliantCells_size = 64;
    long agglomerationLines_Idx_size = 64;
    long agglomerationLines_size = 64;

    long sizes[10] = {64, 344, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                          arrayOfFineAnisotropicCompliantCells,

                          isOnFineBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomeration_lines_idx,
                          agglomeration_lines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);

    EXPECT_EQ(64, sizes[0]);
    EXPECT_EQ(344, sizes[1]);

    EXPECT_EQ(18, sizes[2]);  // indCoarseCell
//    EXPECT_EQ(24, sizes[4]);//isOnValley_size
//    EXPECT_EQ(24, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(8, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(64, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[64] = {0, 5, 4, 1, 6, 13, 12, 10, 7, 15, 14, 11, 2, 9, 8, 3,
                                                             0, 5, 4, 1, 6, 13, 12, 10, 7, 15, 14, 11, 2, 9, 8, 3,
                                                             0, 5, 4, 1, 6, 13, 12, 10, 7, 15, 14, 11, 2, 9, 8, 3,
                                                             17, 17, 17, 17, 16, 16, 17, 17, 16, 16, 16, 17, 16, 16, 16, 17};

//    cout << "\nfc_to_cc: [";
//    for (auto i:fc_to_cc) {
//        cout << i << ", ";
//    }
//    cout << "]" << endl;

    for (int i = 0; i < 64; i++) {

        EXPECT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fc_to_cc[i]);
    }

    // Check agglomeration lines level: l+1
    //======================================

    EXPECT_EQ(17, sizes[8]);//agglomerationLines_Idx_size
    EXPECT_EQ(16, sizes[9]);//agglomerationLines_size

    long ref_agglo_lines_array_idx[17] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    long ref_agglo_lines_array[64] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

//    cout << endl;
//    for (long i_fc = 0; i_fc < sizes[8]; i_fc++) {
//        cout << agglomeration_lines_idx[i_fc] << ", ";
//    }
//    cout << endl;
//    for (long i_fc = 0; i_fc < sizes[9]; i_fc++) {
//        cout << agglomeration_lines[i_fc] << ", ";
//    }
//    cout << endl;
    for (int i = 0; i < sizes[8]; i++) {
        EXPECT_EQ(ref_agglo_lines_array_idx[i], agglomeration_lines_idx[i]);
    }

    for (int i = 0; i < sizes[9]; i++) {
        EXPECT_EQ(ref_agglo_lines_array[i], agglomeration_lines[i]);
    }
}

TEST(CoMMA_TestSuite, agglomerate_one_level_3D_basic_Box_5x5x5_aniso_lvl_2_agglo_lines_recomputed) {

    // 64 Hexahedra: anisotropic cells

    const long nb_fc = 32;
    const long adjMatrix_row_ptr_size = 160;

    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[33] = {0, 6, 12, 18, 24, 30, 36, 42, 48, 53, 58, 63, 68,
                                  73, 78, 83, 88, 93, 98, 103, 108, 113, 118, 123, 128, 132,
                                  136, 140, 144, 148, 152, 156, 160};

    long adjMatrix_col_ind[adjMatrix_row_ptr_size] = {0, 1, 2, 4, 9, 13, 0, 1, 3, 5, 8, 12, 0, 2, 3, 6, 11,
                                                      15, 1, 2, 3, 7, 10, 14, 0, 4, 5, 6, 17, 21, 1, 4, 5, 7,
                                                      16, 20, 2, 4, 6, 7, 19, 23, 3, 5, 6, 7, 18, 22, 1, 8, 9,
                                                      10, 24, 0, 8, 9, 11, 25, 3, 8, 10, 11, 26, 2, 9, 10, 11, 27,
                                                      1, 12, 13, 16, 24, 0, 12, 13, 17, 25, 3, 14, 15, 18, 26, 2, 14,
                                                      15, 19, 27, 5, 12, 16, 17, 28, 4, 13, 16, 17, 29, 7, 14, 18, 19,
                                                      30, 6, 15, 18, 19, 31, 5, 20, 21, 22, 28, 4, 20, 21, 23, 29, 7,
                                                      20, 22, 23, 30, 6, 21, 22, 23, 31, 8, 12, 24, 25, 9, 13, 24, 25,
                                                      10, 14, 26, 27, 11, 15, 26, 27, 16, 20, 28, 29, 17, 21, 28, 29, 18,
                                                      22, 30, 31, 19, 23, 30, 31};

    double adjMatrix_areaValues[adjMatrix_row_ptr_size] = {25., 25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 5.,
                                                           25., 25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 25.,
                                                           25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 5., 25.,
                                                           25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 30., 25., 5.,
                                                           5., 5., 25., 30., 5., 5., 5., 5., 30., 25., 5., 5., 5.,
                                                           25., 30., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5., 5.,
                                                           5., 30., 25., 5., 5., 5., 25., 30., 5., 5., 5., 5., 30.,
                                                           25., 5., 5., 5., 25., 30., 5., 5., 5., 30., 25., 5., 5.,
                                                           5., 25., 30., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5.,
                                                           5., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5., 5., 5.,
                                                           35., 25., 5., 5., 25., 35., 5., 5., 35., 25., 5., 5., 25.,
                                                           35., 5., 5., 35., 25., 5., 5., 25., 35., 5., 5., 35., 25.,
                                                           5., 5., 25., 35.};

    double volumes[nb_fc] = {25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                             25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                             25., 25., 25., 25., 25., 25.};


    long fc_to_cc[nb_fc];
    for (int i = 0; i < nb_fc; i++) {
        fc_to_cc[i] = -1;
    }

    long array_isOnRidge[24] = {0, 1, 2, 3,
                                4, 5, 6, 7};
    long array_isOnValley[24] = {8, 9, 10, 11,
                                 12, 13, 14, 15,
                                 16, 17, 18, 19,
                                 20, 21, 22, 23
    };
    long array_isOnCorner[8] = {24, 25, 26, 27,
                                28, 29, 30, 31};

    long isOnRidge_size = 8;
    long isOnValley_size = 16;
    long isOnCorner_size = 8;

    long isOnFineBnd[nb_fc] = {1, 1, 1, 1,
                               1, 1, 1, 1,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               3, 3, 3, 3,
                               3, 3, 3, 3};

    int minCard = -1;
    int goalCard = -1;
    int maxCard = -1;

    long indCoarseCell = 0;  //TODO remove this

    long checks = 1;
    long verbose = 0;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    const long arrayOfFineAnisotropicCompliantCells_size = 32;
    long *arrayOfFineAnisotropicCompliantCells = new long[nb_fc];
    for (int i = 0; i < arrayOfFineAnisotropicCompliantCells_size; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    long *agglomeration_lines_idx = new long[nb_fc];
    long *agglomeration_lines = new long[nb_fc];

    long isFirstAgglomeration = 1;
    long isAnisotropic = 1;
    long dimension = 3;


    long agglomerationLines_Idx_size = nb_fc;
    long agglomerationLines_size = nb_fc;

    long sizes[10] = {nb_fc, adjMatrix_row_ptr_size, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                          arrayOfFineAnisotropicCompliantCells,

                          isOnFineBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomeration_lines_idx,
                          agglomeration_lines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);

    EXPECT_EQ(nb_fc, sizes[0]);
    EXPECT_EQ(adjMatrix_row_ptr_size, sizes[1]);

    EXPECT_EQ(16, sizes[2]);  // indCoarseCell
//    EXPECT_EQ(24, sizes[4]);//isOnValley_size
//    EXPECT_EQ(24, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(8, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(64, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
    long ref_fc_to_cc[nb_fc] = {12, 12, 14, 14, 13, 13, 15, 15, 6, 6, 11, 11, 7, 7, 5, 5, 10, 10, 9, 9, 4, 4, 8, 8, 0, 0, 1, 1, 2, 2, 3, 3};

    cout << "\nfc_to_cc: [";
    for (auto i:fc_to_cc) {
        cout << i << ", ";
    }
    cout << "]" << endl;

    for (int i = 0; i < 32; i++) {

        EXPECT_EQ(ref_fc_to_cc[i], fc_to_cc[i]);
    }

    // Check agglomeration lines level: l+1
    //======================================

    EXPECT_EQ(17, sizes[8]);//agglomerationLines_Idx_size
    EXPECT_EQ(16, sizes[9]);//agglomerationLines_size

    long ref_agglo_lines_array_idx[17] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    long ref_agglo_lines_array[64] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

//    cout << endl;
//    for (long i_fc = 0; i_fc < sizes[8]; i_fc++) {
//        cout << agglomeration_lines_idx[i_fc] << ", ";
//    }
//    cout << endl;
//    for (long i_fc = 0; i_fc < sizes[9]; i_fc++) {
//        cout << agglomeration_lines[i_fc] << ", ";
//    }
//    cout << endl;
    for (int i = 0; i < sizes[8]; i++) {
        EXPECT_EQ(ref_agglo_lines_array_idx[i], agglomeration_lines_idx[i]);
    }

    for (int i = 0; i < sizes[9]; i++) {
        EXPECT_EQ(ref_agglo_lines_array[i], agglomeration_lines[i]);
    }
}

TEST(CoMMA_TestSuite, agglomerate_one_level_3D_basic_Box_5x5x5_aniso_lvl_2) {

    const long nb_fc = 32;
    const long adjMatrix_row_ptr_size = 160;

    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[33] = {0, 6, 12, 18, 24, 30, 36, 42, 48, 53, 58, 63, 68,
                                  73, 78, 83, 88, 93, 98, 103, 108, 113, 118, 123, 128, 132,
                                  136, 140, 144, 148, 152, 156, 160};
    long adjMatrix_col_ind[adjMatrix_row_ptr_size] = {0, 1, 2, 4, 9, 13, 0, 1, 3, 5, 8, 12, 0, 2, 3, 6, 11,
                                                      15, 1, 2, 3, 7, 10, 14, 0, 4, 5, 6, 17, 21, 1, 4, 5, 7,
                                                      16, 20, 2, 4, 6, 7, 19, 23, 3, 5, 6, 7, 18, 22, 1, 8, 9,
                                                      10, 24, 0, 8, 9, 11, 25, 3, 8, 10, 11, 26, 2, 9, 10, 11, 27,
                                                      1, 12, 13, 16, 24, 0, 12, 13, 17, 25, 3, 14, 15, 18, 26, 2, 14,
                                                      15, 19, 27, 5, 12, 16, 17, 28, 4, 13, 16, 17, 29, 7, 14, 18, 19,
                                                      30, 6, 15, 18, 19, 31, 5, 20, 21, 22, 28, 4, 20, 21, 23, 29, 7,
                                                      20, 22, 23, 30, 6, 21, 22, 23, 31, 8, 12, 24, 25, 9, 13, 24, 25,
                                                      10, 14, 26, 27, 11, 15, 26, 27, 16, 20, 28, 29, 17, 21, 28, 29, 18,
                                                      22, 30, 31, 19, 23, 30, 31};
    double adjMatrix_areaValues[adjMatrix_row_ptr_size] = {25., 25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 5.,
                                                           25., 25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 25.,
                                                           25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 5., 25.,
                                                           25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 30., 25., 5.,
                                                           5., 5., 25., 30., 5., 5., 5., 5., 30., 25., 5., 5., 5.,
                                                           25., 30., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5., 5.,
                                                           5., 30., 25., 5., 5., 5., 25., 30., 5., 5., 5., 5., 30.,
                                                           25., 5., 5., 5., 25., 30., 5., 5., 5., 30., 25., 5., 5.,
                                                           5., 25., 30., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5.,
                                                           5., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5., 5., 5.,
                                                           35., 25., 5., 5., 25., 35., 5., 5., 35., 25., 5., 5., 25.,
                                                           35., 5., 5., 35., 25., 5., 5., 25., 35., 5., 5., 35., 25.,
                                                           5., 5., 25., 35.};
    double volumes[nb_fc] = {25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                             25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                             25., 25., 25., 25., 25., 25.};

    long fc_to_cc[nb_fc];
    for (int i = 0; i < nb_fc; i++) {
        fc_to_cc[i] = -1;
    }

    long array_isOnRidge[24] = {0, 1, 2, 3,
                                4, 5, 6, 7};
    long array_isOnValley[24] = {8, 9, 10, 11,
                                 12, 13, 14, 15,
                                 16, 17, 18, 19,
                                 20, 21, 22, 23
    };
    long array_isOnCorner[8] = {24, 25, 26, 27,
                                28, 29, 30, 31};

    long isOnRidge_size = 8;
    long isOnValley_size = 16;
    long isOnCorner_size = 8;

    long isOnFineBnd[nb_fc] = {1, 1, 1, 1,
                               1, 1, 1, 1,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               3, 3, 3, 3,
                               3, 3, 3, 3};

    int minCard = -1;
    int goalCard = -1;
    int maxCard = -1;

    long indCoarseCell = 0;  //TODO remove this

    long checks = 1;
    long verbose = 0;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    const long arrayOfFineAnisotropicCompliantCells_size = 32;
    long *arrayOfFineAnisotropicCompliantCells = new long[nb_fc];
    for (int i = 0; i < arrayOfFineAnisotropicCompliantCells_size; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    // Agglomeration lines:
    //=====================
    const long agglomerationLines_Idx_size = 17;
    const long agglomerationLines_size = 32;

    long agglomeration_lines_idx[agglomerationLines_Idx_size] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    long agglomeration_lines[agglomerationLines_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

    long isFirstAgglomeration = 0;
    long isAnisotropic = 1;
    long dimension = 3;


    long sizes[10] = {nb_fc, adjMatrix_row_ptr_size, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                          arrayOfFineAnisotropicCompliantCells,

                          isOnFineBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomeration_lines_idx,
                          agglomeration_lines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);

    EXPECT_EQ(nb_fc, sizes[0]);
    EXPECT_EQ(adjMatrix_row_ptr_size, sizes[1]);

    EXPECT_EQ(16, sizes[2]);  // indCoarseCell
//    EXPECT_EQ(24, sizes[4]);//isOnValley_size
//    EXPECT_EQ(24, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(8, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(64, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
    long ref_fc_to_cc[nb_fc] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15};

//    cout << "\nfc_to_cc: [";
//    for (auto i:fc_to_cc) {
//        cout << i << ", ";
//    }
//    cout << "]" << endl;

    for (int i = 0; i < 32; i++) {

        EXPECT_EQ(ref_fc_to_cc[i], fc_to_cc[i]);
    }

    // Check agglomeration lines level: l+1
    //======================================

    EXPECT_EQ(17, sizes[8]);//agglomerationLines_Idx_size
    EXPECT_EQ(16, sizes[9]);//agglomerationLines_size

    long ref_agglo_lines_array_idx[17] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    long ref_agglo_lines_array[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

//    cout << endl;
//    for (long i_fc = 0; i_fc < sizes[8]; i_fc++) {
//        cout << agglomeration_lines_idx[i_fc] << ", ";
//    }
//    cout << endl;
//    for (long i_fc = 0; i_fc < sizes[9]; i_fc++) {
//        cout << agglomeration_lines[i_fc] << ", ";
//    }
//    cout << endl;
    for (int i = 0; i < sizes[8]; i++) {
        EXPECT_EQ(ref_agglo_lines_array_idx[i], agglomeration_lines_idx[i]);
    }

    for (int i = 0; i < sizes[9]; i++) {
        EXPECT_EQ(ref_agglo_lines_array[i], agglomeration_lines[i]);
    }
}

TEST(CoMMA_TestSuite, agglomerate_one_level_3D_basic_Box_5x5x5_aniso_lvl_2_case_2) {

    const long nb_fc = 32;
    const long adjMatrix_row_ptr_size = 160;

    long numberOfFineAgglomeratedCells = 0;
    long adjMatrix_row_ptr[33] = {0, 6, 12, 18, 24, 30, 36, 42, 48, 53, 58, 63, 68,
                                  73, 78, 83, 88, 93, 98, 103, 108, 113, 118, 123, 128, 132,
                                  136, 140, 144, 148, 152, 156, 160};
    long adjMatrix_col_ind[adjMatrix_row_ptr_size] = {0, 1, 2, 4, 9, 13, 0, 1, 3, 5, 8, 12, 0, 2, 3, 6, 11,
                                                      15, 1, 2, 3, 7, 10, 14, 0, 4, 5, 6, 17, 21, 1, 4, 5, 7,
                                                      16, 20, 2, 4, 6, 7, 19, 23, 3, 5, 6, 7, 18, 22, 1, 8, 9,
                                                      10, 24, 0, 8, 9, 11, 25, 3, 8, 10, 11, 26, 2, 9, 10, 11, 27,
                                                      1, 12, 13, 16, 24, 0, 12, 13, 17, 25, 3, 14, 15, 18, 26, 2, 14,
                                                      15, 19, 27, 5, 12, 16, 17, 28, 4, 13, 16, 17, 29, 7, 14, 18, 19,
                                                      30, 6, 15, 18, 19, 31, 5, 20, 21, 22, 28, 4, 20, 21, 23, 29, 7,
                                                      20, 22, 23, 30, 6, 21, 22, 23, 31, 8, 12, 24, 25, 9, 13, 24, 25,
                                                      10, 14, 26, 27, 11, 15, 26, 27, 16, 20, 28, 29, 17, 21, 28, 29, 18,
                                                      22, 30, 31, 19, 23, 30, 31};
    double adjMatrix_areaValues[adjMatrix_row_ptr_size] = {25., 25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 5.,
                                                           25., 25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 25.,
                                                           25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 5., 5., 25.,
                                                           25., 5., 5., 5., 5., 25., 25., 5., 5., 5., 30., 25., 5.,
                                                           5., 5., 25., 30., 5., 5., 5., 5., 30., 25., 5., 5., 5.,
                                                           25., 30., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5., 5.,
                                                           5., 30., 25., 5., 5., 5., 25., 30., 5., 5., 5., 5., 30.,
                                                           25., 5., 5., 5., 25., 30., 5., 5., 5., 30., 25., 5., 5.,
                                                           5., 25., 30., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5.,
                                                           5., 5., 5., 30., 25., 5., 5., 5., 25., 30., 5., 5., 5.,
                                                           35., 25., 5., 5., 25., 35., 5., 5., 35., 25., 5., 5., 25.,
                                                           35., 5., 5., 35., 25., 5., 5., 25., 35., 5., 5., 35., 25.,
                                                           5., 5., 25., 35.};
    double volumes[nb_fc] = {25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                             25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                             25., 25., 25., 25., 25., 25.};

    long fc_to_cc[nb_fc];
    for (int i = 0; i < nb_fc; i++) {
        fc_to_cc[i] = -1;
    }

    long array_isOnRidge[24] = {0, 1, 2, 3,
                                4, 5, 6, 7};
    long array_isOnValley[24] = {8, 9, 10, 11,
                                 12, 13, 14, 15,
                                 16, 17, 18, 19,
                                 20, 21, 22, 23
    };
    long array_isOnCorner[8] = {24, 25, 26, 27,
                                28, 29, 30, 31};

    long isOnRidge_size = 8;
    long isOnValley_size = 16;
    long isOnCorner_size = 8;

    long isOnFineBnd[nb_fc] = {1, 1, 1, 1,
                               1, 1, 1, 1,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               2, 2, 2, 2,
                               3, 3, 3, 3,
                               3, 3, 3, 3};

    int minCard = -1;
    int goalCard = -1;
    int maxCard = -1;

    long indCoarseCell = 0;  //TODO remove this

    long checks = 1;
    long verbose = 0;

    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
    const long arrayOfFineAnisotropicCompliantCells_size = 32;
    long *arrayOfFineAnisotropicCompliantCells = new long[nb_fc];
    for (int i = 0; i < arrayOfFineAnisotropicCompliantCells_size; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }

    // Agglomeration lines:
    //=====================
    const long agglomerationLines_Idx_size = 17;
    const long agglomerationLines_size = 30;

    long agglomeration_lines_idx[agglomerationLines_Idx_size] = {0, 1, 3, 5, 7, 9, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
    long agglomeration_lines[agglomerationLines_size] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

    long isFirstAgglomeration = 0;
    long isAnisotropic = 1;
    long dimension = 3;

    long sizes[10] = {nb_fc, adjMatrix_row_ptr_size, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};


    agglomerate_one_level(sizes,
                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,

                          arrayOfFineAnisotropicCompliantCells,

                          isOnFineBnd,
                          array_isOnValley,
                          array_isOnRidge,
                          array_isOnCorner,

                          isFirstAgglomeration,
                          isAnisotropic,

                          fc_to_cc,

                          agglomeration_lines_idx,
                          agglomeration_lines,

                          dimension,
                          goalCard,
                          minCard,
                          maxCard,
                          checks,
                          verbose);

    EXPECT_EQ(nb_fc, sizes[0]);
    EXPECT_EQ(adjMatrix_row_ptr_size, sizes[1]);

    EXPECT_EQ(16, sizes[2]);  // indCoarseCell
//    EXPECT_EQ(24, sizes[4]);//isOnValley_size
//    EXPECT_EQ(24, sizes[5]);//isOnRidge_size
//    EXPECT_EQ(8, sizes[6]);//isOnCorner_size
//    EXPECT_EQ(64, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
    long ref_fc_to_cc[nb_fc] = {15, 15, 0, 0, 1, 1, 2, 2, 3, 3, 14, 14, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};

    cout << "\nfc_to_cc: [";
    for (auto i:fc_to_cc) {
        cout << i << ", ";
    }
    cout << "]" << endl;

    for (int i = 0; i < 32; i++) {

        EXPECT_EQ(ref_fc_to_cc[i], fc_to_cc[i]);
    }

    // Check agglomeration lines level: l+1
    //======================================

    EXPECT_EQ(17, sizes[8]);//agglomerationLines_Idx_size
    EXPECT_EQ(14, sizes[9]);//agglomerationLines_size

    long ref_agglo_lines_array_idx[17] = {0, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    long ref_agglo_lines_array[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    cout << endl;
    for (long i_fc = 0; i_fc < sizes[8]; i_fc++) {
        cout << agglomeration_lines_idx[i_fc] << ", ";
    }
    cout << endl;
    for (long i_fc = 0; i_fc < sizes[9]; i_fc++) {
        cout << agglomeration_lines[i_fc] << ", ";
    }
    cout << endl;
    for (int i = 0; i < sizes[8]; i++) {
        EXPECT_EQ(ref_agglo_lines_array_idx[i], agglomeration_lines_idx[i]);
    }

    for (int i = 0; i < sizes[9]; i++) {
        EXPECT_EQ(ref_agglo_lines_array[i], agglomeration_lines[i]);
    }
}



//TEST(CoMMA_TestSuite, agglomerateOneLevel_disconnected) {
//
//    // MGridGen Test case
//    int nb_fc = 10;
//    long adjMatrix_col_ind_size = 24;
//
//    long adjMatrix_row_ptr[11] = {0, 3, 5, 7, 9, 11, 12, 15, 18, 21, 24};
//    long adjMatrix_col_ind[24] = {0, 1, 2, 0, 1,
//                                  0, 2, 3, 4, 3,
//                                  4, 5, 6, 7, 8,
//                                  6, 7, 9, 6, 8, 9, 7, 8, 9};
//    double adjMatrix_areaValues[24] = {1., 1., 1., 1., 2., 1., 2., 2., 1., 1., 2., 3., 2., 1., 1., 1., 2.,
//                                       1., 1., 2., 1., 1., 1., 2.};
//
//    double volumes[10] = {1.0, 1.0, 1.0, 1.0, 1.0,
//                          1.0, 1.0, 1.0, 1.0, 1.0};
//    long isOnBnd[10] = {1, 2, 2, 2, 2,
//                        3, 2, 2, 2, 2};
//
//    long numberOfFineAgglomeratedCells = 0;
//
//    long isOnRidge_size = 1;
//    long isOnValley_size = 8;
//    long isOnCorner_size = 1;
//
//    long array_isOnRidge[1] = {0};
//    long array_isOnValley[8] = {1, 2, 3, 4, 6, 7, 8, 9};
//    long array_isOnCorner[1] = {5};
//
//
//    // Initialization of datas for agglomeration
//    ////////////////////////////////////////////
//    bool isFineCellAgglomerated[nb_fc];
//    for (int i = 0; i < nb_fc; i++) {
//        isFineCellAgglomerated[i] = false;
//    }
//    long fc_to_cc[nb_fc];
//    for (int i = 0; i < nb_fc; i++) {
//        fc_to_cc[i] = -1;
//    }
//
//    long indCoarseCell = 0;
//
//    // Parameters for agglomeration
//    ////////////////////////////////////////////
//    int thresholdCard = 3;
//    int minCard = 3;
//    int goalCard = 4;
//    int maxCard = 5;
//
//    long checks = 1;
//    long verbose = 0;
//
//    // initialization of arrayOfFineAnisotropicCompliantCells: prismatic and hexaedric cells
//    long *arrayOfFineAnisotropicCompliantCells = new long[nb_fc];
//    for (int i = 0; i < nb_fc; i++) {
//        arrayOfFineAnisotropicCompliantCells[i] = i;
//    }
//
//    long *agglomerationLines_Idx = new long[nb_fc];
//    long *agglomerationLines = new long[nb_fc];
//
//    for (int i = 0; i < nb_fc; i++) {
//        agglomerationLines_Idx[i] = 0;
//        agglomerationLines[i] = 0;
//    }
//
//    long isFirstAgglomeration = 1;
//    long isAnisotropic = 0;
//    long dimension = 2;
//
//    long arrayOfFineAnisotropicCompliantCells_size = nb_fc;
//    long agglomerationLines_Idx_size = nb_fc;
//    long agglomerationLines_size = nb_fc;
//
//    long sizes[10] = {nb_fc, adjMatrix_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, isOnValley_size, isOnRidge_size, isOnCorner_size,
//                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};
//
//
//    agglomerate_one_level(sizes,
//                          adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
//
//                          arrayOfFineAnisotropicCompliantCells,
//
//                          isOnBnd,
//                          array_isOnValley,
//                          array_isOnRidge,
//                          array_isOnCorner,
//
//                          isFirstAgglomeration,
//                          isAnisotropic,
//
//                          fc_to_cc,
//
//                          agglomerationLines_Idx,
//                          agglomerationLines,
//
//                          dimension,
//                          goalCard,
//                          minCard,
//                          maxCard,
//                          checks,
//                          verbose);
////    cout << "sizes[0] " << sizes[0] << endl;
////    cout << "sizes[1] " << sizes[1] << endl;
////    cout << "sizes[2] " << sizes[2] << endl;
////    cout << "sizes[3] " << sizes[3] << endl;
////    cout << "sizes[4] " << sizes[4] << endl;
////    cout << "sizes[5] " << sizes[5] << endl;
////    cout << "sizes[6] " << sizes[6] << endl;
////    cout << "sizes[7] " << sizes[7] << endl;
////    cout << "sizes[8] " << sizes[8] << endl;
////    cout << "sizes[9] " << sizes[9] << endl;
//
//    ASSERT_EQ(nb_fc, sizes[0] );
//    ASSERT_EQ(adjMatrix_col_ind_size, sizes[1]);
//    ASSERT_EQ(4, sizes[2]);//indCoarseCell
//    ASSERT_EQ(nb_fc, sizes[3]);//numberOfFineAgglomeratedCells
//    ASSERT_EQ(8, sizes[4]);//isOnValley_size
//    ASSERT_EQ(1, sizes[5]);//isOnRidge_size
//    ASSERT_EQ(1, sizes[6]);//isOnCorner_size
//    ASSERT_EQ(10, sizes[7]);//arrayOfFineAnisotropicCompliantCells_size
//    ASSERT_EQ(10, sizes[8]);//agglomerationLines_Idx_size
//    ASSERT_EQ(10, sizes[9]);//agglomerationLines_size
//
//    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[15] = {0, 0, 0, 3, 3, 2, 1, 1, 1, 1};
////    cout<<endl;
//    for (int i = 0; i < nb_fc; i++) {
////        cout << fc_to_cc[i] << ", ";
////        cout<<i<<", "<<fc_to_cc[i]<<" ref= "<<ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]<<endl;
//        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fc_to_cc[i]  );
//    }
//}

