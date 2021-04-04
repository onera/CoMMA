#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator_anisotropic.h"
#include "gtest/gtest.h"


TEST(Utils_TestSuite, PartsList){

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq);
//    for (auto l:listOfSet) {
//        for (auto i: l) {
//            cout << i << ", ";
//        }
//        cout << endl;
//    }

    EXPECT_EQ(7,listOfSet.size());
    //ASSERT_EQ(
}

TEST(Utils_TestSuite, PartsList_length_1){

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq, 1);

//    for (auto l:listOfSet) {
//        for (auto i: l) {
//            cout << i << ", ";
//        }
//        cout << endl;
//    }
    EXPECT_EQ(3,listOfSet.size());
    //ASSERT_EQ(
}

TEST(Utils_TestSuite, PartsList_length_2){

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq, 2);
    EXPECT_EQ(3,listOfSet.size());
    //ASSERT_EQ(
}

TEST(Utils_TestSuite, PartsList_length_3){

    vector<long> seq({0, 3, 1});
    list<unordered_set<long>> listOfSet = partsList(seq, 3);
    EXPECT_EQ(1,listOfSet.size());

}

//queue<unordered_set<long>> partsList(vector<long> seq, int length)

TEST(Utils_TestSuite, CheckConnectivity_MGridGenDoc_example){
    long row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                        7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11, 14};

    long sizes[3] = {16, 51, 0};
    unordered_set<long> datas({0, 1, 3});
    ASSERT_TRUE(checkConnectivity_w_set(datas, row_ptr, col_ind));

    datas = {3, 1, 0};
    ASSERT_TRUE(checkConnectivity_w_set(datas, row_ptr, col_ind));

    datas = {1, 3, 0};
    ASSERT_TRUE(checkConnectivity_w_set(datas, row_ptr, col_ind));

    unordered_set<long> datas2({0, 2});
    ASSERT_TRUE(checkConnectivity_w_set(datas2, row_ptr, col_ind));

    datas2 = {2, 0};
    ASSERT_TRUE(checkConnectivity_w_set(datas2, row_ptr, col_ind));

    datas2 = {0, 6};
    ASSERT_FALSE(checkConnectivity_w_set(datas2, row_ptr, col_ind));

    datas2 = {6, 0};
    ASSERT_FALSE(checkConnectivity_w_set(datas2, row_ptr, col_ind));

    unordered_set<long> datas3({0, 1, 2, 3, 4, 5, 6, 7, 14});
    ASSERT_FALSE(checkConnectivity_w_set(datas3, row_ptr, col_ind));

    datas3 = {0, 1, 2, 3, 4, 5, 6, 7, 11};
    ASSERT_TRUE(checkConnectivity_w_set(datas3, row_ptr, col_ind));

    unordered_set<long> datas4({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    ASSERT_TRUE(checkConnectivity_w_set(datas4, row_ptr, col_ind));

    unordered_set<long> datas5({14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
    ASSERT_TRUE(checkConnectivity_w_set(datas5, row_ptr, col_ind));
}


TEST(Utils_TestSuite, ComputeNumberOfCommonFaces){

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

    ASSERT_EQ(1,computeNumberOfCommonFaces(0, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(1,computeNumberOfCommonFaces(1, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(2,computeNumberOfCommonFaces(2, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(0,computeNumberOfCommonFaces(3, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(2,computeNumberOfCommonFaces(4, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(3,computeNumberOfCommonFaces(5, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(2,computeNumberOfCommonFaces(6, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));
    ASSERT_EQ(2,computeNumberOfCommonFaces(11, iCC, row_ptr, col_ind, fine_Cell_indices_To_C_C_Indices));

}


TEST(Utils_TestSuite, read_agglomeration_datas_from_file){

    // Datas from test_agglomerate.test_agglomerateOneLevel_disconnected(...)
    string filename = "/Users/lantos/CLionProjects/CoMMA/Google_tests/tests/0_Inputs/Datas_Agglomeration_10_24.txt";
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


    // MGridGen_Dual_Graph Test case
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
    ASSERT_EQ(ref_numberOfFineCells , numberOfFineCells);
    ASSERT_EQ(ref_adjMatrix_col_ind_size , adjMatrix_col_ind_size);

    ASSERT_EQ(ref_isFirstAgglomeration , isFirstAgglomeration_long[0]);
    ASSERT_EQ(ref_isAnisotropic , isAnisotropic_long[0]);

    ASSERT_EQ(ref_dimension , dimension[0]);
    ASSERT_EQ(ref_goalCard , goalCard[0]);
    ASSERT_EQ(ref_minCard , minCard[0]);
    ASSERT_EQ(ref_maxCard , maxCard[0]);

    for (int i = 0; i < numberOfFineCells + 1; i++) {
        ASSERT_EQ(ref_adjMatrix_row_ptr[i], adjMatrix_row_ptr[i]);
    }
    for (int i = 0; i < ref_adjMatrix_col_ind_size; i++) {
        ASSERT_EQ(ref_adjMatrix_col_ind[i] , adjMatrix_col_ind[i]);
    }

    for (int i = 0; i < ref_adjMatrix_col_ind_size; i++) {
        ASSERT_EQ(ref_adjMatrix_areaValues[i] , adjMatrix_areaValues[i]);
    }

    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_volumes[i] , volumes[i]);
    }

    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_arrayOfFineAnisotropicCompliantCells[i] , arrayOfFineAnisotropicCompliantCells[i]);
    }
    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_isOnBnd[i] , isOnFineBnd_l[i]);
    }
    for (int i = 0; i < ref_isOnValley_size; i++) {
        ASSERT_EQ(ref_array_isOnValley[i] , array_isOnValley[i]);
    }

    for (int i = 0; i < ref_isOnRidge_size; i++) {
        ASSERT_EQ(ref_array_isOnRidge[i] , array_isOnRidge[i]);
    }
    for (int i = 0; i < ref_isOnCorner_size; i++) {
        ASSERT_EQ(ref_array_isOnCorner[i] , array_isOnCorner[i]);
    }
    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i] , agglomerationLines_Idx[i]);
    }
    for (int i = 0; i < numberOfFineCells; i++) {
        ASSERT_EQ(ref_agglomerationLines[i] , agglomerationLines[i]);
    }
}

TEST(Utils_TestSuite, DISABLED_read_agglomeration_datas_from_file_and_agglomerate){

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
//        cout << "\tnumberOfFineCells\t" << numberOfFineCells << endl;
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
            ASSERT_TRUE(i<sizes[2]);
        }
//        cout << "]" << endl;

        //    for (int i = 0; i < 64; i++) {
        //
        //        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
        //    }
    }
}



TEST(Utils_TestSuite, compute_Dicts_From_FineCellIndicesToCoarseCellIndices){
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
    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({0, 1, 3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({2, 4, 5, 6, 7, 8, 11});
    ref_dict_Coarse_Elem[2] = unordered_set<long>({9,10});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({2});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[7] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[7] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

//    assert(!dict_Coarse_Cells.empty());
//    assert(dict_Coarse_Cells.size() == 3);
//    assert(dict_Coarse_Cells[0].count(0) == 1);
//    assert(dict_Coarse_Cells[0].count(1) == 1);
//    assert(dict_Coarse_Cells[0].count(3) == 1);
//
//    assert(dict_Coarse_Cells[1].count(2) == 1);
//    assert(dict_Coarse_Cells[1].count(4) == 1);
//    assert(dict_Coarse_Cells[1].count(5) == 1);
//    assert(dict_Coarse_Cells[1].count(6) == 1);
//    assert(dict_Coarse_Cells[1].count(7) == 1);
//    assert(dict_Coarse_Cells[1].count(8) == 1);
//    assert(dict_Coarse_Cells[1].count(11) == 1);
//
//    assert(dict_Coarse_Cells[2].count(9) == 1);
//    assert(dict_Coarse_Cells[2].count(10) == 1);

//    assert(!dict_Card_Coarse_Cells.empty());
//    assert(dict_Card_Coarse_Cells.size() == 3);
//    assert(dict_Card_Coarse_Cells[3].count(0) == 1);
//    assert(dict_Card_Coarse_Cells[2].count(2) == 1);
//    assert(dict_Card_Coarse_Cells[7].count(1) == 1);

//    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
//    assert(dict_DistributionOfCardinalOfCoarseElements[2] == 1);
//    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 1);
//    assert(dict_DistributionOfCardinalOfCoarseElements[7] == 1);
}
