#include <iostream>
#include <cassert>
#include <unordered_set>
#include "Agglomerator.h"
#include "Agglomerator_Isotropic.h"
#include "tests.h"
using namespace std;


int main() {
    cout<<"toto"<<endl;
    std::cout << "Hello, World!" << std::endl;


//    unordered_map<long, double> m;
//    m[0] = 19.3;
//    m[1] = 0.3;
//    m[2] = 1.3;
//    m[3] = 2.3;
//
//    multimap<double, long> dst = flip_map(m);
//    multimap<double, long>::iterator it;
//    multimap<double, long>::reverse_iterator rIt;
//
//    for (it = dst.begin();it!=dst.end(); it++)
//    {
//        cout<<it->first<<" : "<<it->second <<endl;
//    }
//
//    for (rIt = dst.rbegin();rIt!=dst.rend(); rIt++)
//    {
//        cout<<rIt->first<<" : "<<rIt->second <<endl;
//    }
//    deque<long> dQ;
//    dQ.push_back(5);
////    long value =
////            find(dQ.begin(), dQ.end(), 5);
//
//    forward_list<int> toto;
//    cout<<"max_size "<< toto.max_size()<<endl;
//    toto.push_front(5);
//    toto.push_front(6);
//    toto.push_front(7);
//    forward_list<int>::iterator fLIt;
//    for (fLIt = toto.begin();fLIt!=toto.end(); fLIt++)
//    {
//        cout<<*fLIt<<" ";
//    }
//    cout<<"front= "<< toto.front()<<endl;





//


    test_FindSeedViaFrontalMethod_025();
    test_FindSeedViaFrontalMethod_10265813119();
    test_FindSeedViaFrontalMethod_136111314();
    test_FindSeedViaFrontalMethod_520810131411();
    test_FindSeedViaFrontalMethod_145208101311();
    test_FindSeedViaFrontalMethod_457810();
    test_FindSeedViaFrontalMethod_205();
    test_FindSeedViaFrontalMethod_All();
    test_FindSeedViaFrontalMethod_All_shuffled();
    test_FindSeedViaFrontalMethod_Case_2();
    test_FindSeedViaFrontalMethod_Case_3_seed_0();
    test_FindSeedViaFrontalMethod_Case_3_seed_1();
    test_FindSeedViaFrontalMethod_Case_3_seed_6();
    test_FindSeedViaFrontalMethod_Case_3_seed_8();
    test_FindSeedViaFrontalMethod_Case_4_random();
    test_FindSeedViaFrontalMethod_Case_5_random();
    test_FindSeedViaFrontalMethod_Case_6();
    test_FindSeedViaFrontalMethod_Case_6_seed_6();
    test_Computation_Of_Neighbourhood();
    test_Computation_Of_Neighbourhood_FineCellsConstraints();
    test_Computation_Of_Neighbourhood_FineCellsConstraints_11();
    test_Computation_Of_Neighbourhood_FineCellsConstraints_118();
    test_Computation_Of_Neighbourhood_FineCellsConstraints_118_maxCard();
    test_Computation_Of_Neighbourhood_FineCellsConstraints_1184();
    test_Computation_Of_Neighbourhood_FineCellsConstraints_13();
    test_ComputeNumberOfCommonFaces();

//    unordered_set<int> set_1;
//    set_1.insert(1);
//    set_1.insert(2);
//    set_1.insert(19);
//    set_1.insert(25);
//    unordered_set<int> set_2(set_1);
//    set_2.insert(999);
//    assert(set_1.count(999)==0);
//    assert(set_2.count(999)==1);
//
//    set_2 = set_1;
////
//    for (auto j :set_2){
//        cout<<j<<", ";
//    }cout<<endl;
//
//    int iFineCell = *set_2.begin();
//    cout<<"iFineCell "<<iFineCell<<endl;
//    set_2.erase(set_2.begin());
//    for (auto j :set_2){
//        cout<<j<<", ";
//    }cout<<endl;

    test_CheckConnectivity_MGridGenDoc_example();
    test_PartsList();
//    cout<<"\ntest_PartsList_length_1"<<endl;
    test_PartsList_length_1();
//    cout<<"\ntest_PartsList_length_2"<<endl;
    test_PartsList_length_2();
//    cout<<"\ntest_PartsList_length_3"<<endl;
    test_PartsList_length_3();
//
//    unordered_set<int> set_1;
//    set_1.insert(1);
//    set_1.insert(2);
//    set_1.insert(19);
//    set_1.insert(25);
//    set_1.erase(3);
//    for (auto j :set_1){
//        cout<<j<<", ";
//    }
//    cout<<endl;
//    cout<<"test_RemoveSeparatingVertex_205"<<endl;
    test_RemoveSeparatingVertex_205();
//    cout<<"test_RemoveSeparatingVertex_136111314"<<endl;
    test_RemoveSeparatingVertex_136111314();
    test_RemoveSeparatingVertex_10265813119();
//    cout<<"======================================================================="<<endl;
//    cout<<"test_RemoveSeparatingVertex_All"<<endl;
    test_RemoveSeparatingVertex_All();
    test_RemoveSeparatingVertex_Case_2();
    test_RemoveSeparatingVertex_Case_3_seed_0();
    test_RemoveSeparatingVertex_Case_4_random();
    test_RemoveSeparatingVertex_Case_5_random();
    test_RemoveSeparatingVertex_Case_6();

    test_RemoveSeparatingVertex_Case_7_seed_0();
    test_RemoveSeparatingVertex_Case_7_seed_1();
    test_RemoveSeparatingVertex_Case_7_seed_9();
    test_RemoveSeparatingVertex_Case_8_random();
    test_RemoveSeparatingVertex_Case_9_random();
//    cout<<"\nCall of test_SwapFineCell_Case_1"<<endl;
    test_SwapFineCell_Case_1();
//    cout<<"\nCall of test_SwapFineCell_Deletion_of_Coarse_Element"<<endl;
    test_SwapFineCell_Deletion_of_Coarse_Element();
//    cout<<"\nCall of test_computeConnectedComponent_9_Squares"<<endl;
    test_computeConnectedComponent_9_Squares();


    test_createCoarseCell_Case_1();
    test_createCoarseCell_Case_1_isMutable_False();

    test_createCoarseCell_Case_3();
    test_createCoarseCell_Delayed_Case_1();
    test_createCoarseCell_Delayed_Case_1_isMutable_False();
    test_createCoarseCell_Delayed_Case_3();
//    cout<<"\nCall of test_splitNonConnectedCoarseCell_9_Squares_1"<<endl;

    test_splitNonConnectedCoarseCell_9_Squares_1();
    test_splitNonConnectedCoarseCell_9_Squares_2();
    test_splitNonConnectedCoarseCell_9_Squares_3();
    cout<<"\nATTENTION TREAT this case!"<<endl;
    cout<<"\nCall of test_splitNonConnectedCoarseCell_9_Squares_5"<<endl;
//    test_splitNonConnectedCoarseCell_9_Squares_5();

//    cout<<"\nCall of test_removeDeletedCoarseCells_Case_1"<<endl;
    test_removeDeletedCoarseCells_Case_1();
    test_removeDeletedCoarseCells_Deletion_of_Coarse_Element();
    test_removeDeletedCoarseCells_Deletion_of_Coarse_Element_Case_2();
    test_removeDeletedCoarseCells_Deletion_of_Coarse_Element_Case_3();
//    cout<<"\nCall of test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_False"<<endl;
    test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_False();
//    cout<<"\nCall of test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True"<<endl;
    test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True();
//    cout<<"\nCall of test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_False_GoalCard_8_neighbours_4"<<endl;
    test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_False_GoalCard_8_neighbours_4();
//    cout<<"\nCall of test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True_GoalCard_8_neighbours_4"<<endl;
    test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True_GoalCard_8_neighbours_4();
    test_choice_Of_Agglomerated_Cells_9_Squares_isOrderPrimary_True_GoalCard_8();
//    cout<<"\nCall of test_Agglomerate_Isotropic_createCoarseCell_Case_1"<<endl;
    test_Agglomerate_Isotropic_createCoarseCell_Case_1();
//    cout<<"\nCall of test_Agglomerate_Isotropic_createCoarseCell_Case_1_isMutable_False"<<endl;
    test_Agglomerate_Isotropic_createCoarseCell_Case_1_isMutable_False();
//    cout<<"\nCall of test_Agglomerate_Isotropic_createCoarseCell_Case_3"<<endl;
    test_Agglomerate_Isotropic_createCoarseCell_Case_3();
//    cout<<"\nCall of test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_1"<<endl;
    test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_1();
//    cout<<"\nCall of test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_1_isMutable_False"<<endl;
    test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_1_isMutable_False();
//    cout<<"\nCall of test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_3"<<endl;
    test_Agglomerate_Isotropic_createCoarseCell_Delayed_Case_3();
    test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_0();
    test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_2();
    test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Ridge_1();
    test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Ridge_1_V2();
    test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Valley_4();
    test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Valley_10();
    test_agglomerate_Isotropic_Choice_Of_Seed_Case_Seed_Valley_13();




//    unordered_set<int> set1{3, 1, 4, 6, 5, 9};
//    unordered_set<int> set2;
//    set2 = set1;
//    set2.erase(3);
//    cout<<"\nset1"<<endl;
//    for (auto i :set1) {
//        cout << i << ", ";
//    }
//
//    cout << endl;
//
//    cout<<"\nset2"<<endl;
//    for (auto i :set2) {
//        cout << i << ", ";
//    }
//
//    cout << endl;
    test_remove_Too_Small_Cells_v2_12_Squares_8_isotropicCell_1_anisotropic();
    test_remove_Too_Small_Cells_v2_12_Squares_8_isotropicCell_1_anisotropic_case_2();
    test_remove_Too_Small_Cells_v2_12_Squares_9_isotropicCell();

    test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_0_anisotropic();
    test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_4();
    test_splitNonConnectedCoarseCell_9_Squares_Case_3();
    test_splitNonConnectedCoarseCell_9_Squares_Too_small_Cell_size_2();
    test_splitNonConnectedCoarseCell_9_Squares_Too_small_Cell_size_2_case_2();

    test_makeSmallCellBigger_12_Squares_12_isotropic_Cells();
//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;

    test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_2();
//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_3"<<endl;
    test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_3();
//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_4"<<endl;
    test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_4();
//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_5"<<endl;
    test_makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_5();
    cout<<"\n\n\n"<<endl;
    cout<<"=================================================================="<<endl;
    cout<<"test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells"<<endl;

    test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells();
    test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells_Case_2();
    test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells_Case_3();
    test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells_Case_4();
    test_agglomerate_Isotropic_Correction_Swap_12_Squares_12_isotropic_Cells_Case_5();

    test_compute_Dicts_From_FineCellIndicesToCoarseCellIndices();

    test_agglomerate_Isotropic_Correction_Too_Big_Cells_12_Squares_12_isotropic_Cells();
    test_agglomerate_Isotropic_Correction_Too_Big_Cells_12_Squares_12_isotropic_Cells_Case_2();

//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo_1_cell"<<endl;
    test_agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo_1_cell();
    test_agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo_1_cell_v2();

    test_agglomerate_Isotropic_CreateDelayedCoarseCells_Case_1();

    test_agglomerate_Isotropic_First_Step_Box_5x5x5();
    test_agglomerate_Isotropic_One_Level_v_2_Box_5x5x5();

//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_agglomerate_Isotropic_One_Level_v_2_Box_5x5x5_goalCard_9"<<endl;
    test_agglomerate_Isotropic_One_Level_v_2_Box_5x5x5_goalCard_9();

//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_computeAnisotropicLine_1_AnisotropicCell"<<endl;
    test_computeAnisotropicLine_1_AnisotropicCell();

//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_computeAnisotropicLine_Box_5x5x5_iso_and_Aniso_MG_1_level"<<endl;
    test_computeAnisotropicLine_Box_5x5x5_iso_and_Aniso_MG_1_level();

//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle"<<endl;
    test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle();

//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle_2"<<endl;
    test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle_2();

//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle_Iso_at_both_ends"<<endl;
    test_computeAnisotropicLine_MostAnisotropicCellInTheMiddle_Iso_at_both_ends();
    test_agglomerate_Anisotropic_One_Level();
    test_agglomerate_Anisotropic_One_Level_2();
    test_agglomerate_Anisotropic_One_Level_Box_5x5x5_Aniso_MG_1_level();

    cout<<"\n\n\n"<<endl;
    cout<<"=================================================================="<<endl;
    cout<<"test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_8"<<endl;
    test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_8();
    test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_9();
    test_agglomerateOneLevel_Box_5x5x5_Anisotropic();
    cout<<"Tout s'est bien fini!"<<endl;
    return 0;
}

