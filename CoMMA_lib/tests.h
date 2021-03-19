//
// Created by Nicolas Lantos on 18/10/2017.
//

#ifndef TEST_TESTS_H
#define TEST_TESTS_H

int test_ComputeNumberOfCommonFaces();
int test_CheckConnectivity_MGridGenDoc_example();

int test_computeConnectedComponent_9_Squares();

int test_compute_Dicts_From_FineCellIndicesToCoarseCellIndices();

int test_agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo_1_cell();
int test_agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo_1_cell_v2();

int test_agglomerate_Isotropic_CreateDelayedCoarseCells_Case_1();

int test_agglomerate_Isotropic_First_Step_Box_5x5x5();
int test_agglomerate_Isotropic_One_Level_v_2_Box_5x5x5();
int test_agglomerate_Isotropic_One_Level_v_2_Box_5x5x5_goalCard_9();


int test_computeAnisotropicLine_1_AnisotropicCell();
int test_computeAnisotropicLine_Box_5x5x5_iso_and_Aniso_MG_1_level();

int test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle();
int test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle_2();

int test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle_Iso_at_both_ends();

int test_agglomerate_Anisotropic_One_Level();
int test_agglomerate_Anisotropic_One_Level_2();
int test_agglomerate_Anisotropic_One_Level_Box_5x5x5_Aniso_MG_1_level();
int test_read_agglomeration_datas_from_file();
int test_read_agglomeration_datas_from_file_and_agglomerate();

#endif //TEST_TESTS_H
