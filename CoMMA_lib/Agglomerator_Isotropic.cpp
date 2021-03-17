//
// Created by Nicolas Lantos on 18/10/2017.
//

#include "Agglomerator_Isotropic.h"
#include "Agglomerator.h"

#include <unordered_map>
#include <unordered_set>
#include <list>
//#include<vector>
#include<queue>
#include<iostream>
#include<math.h>
#include <assert.h>
#include <stdexcept>
#include <algorithm>
#include <exception>

#include <string>

#include <fstream>
#include <iomanip>

using namespace std;

void agglomerateOneLevel(long *sizes,

                         long *adjMatrix_row_ptr,
                         long *adjMatrix_col_ind,
                         double *adjMatrix_areaValues,
                         double *volumes,

                         long *arrayOfFineAnisotropicCompliantCells,

                         long *isOnFineBnd_l,
                         long *array_isOnValley,
                         long *array_isOnRidge,
                         long *array_isOnCorner,
                         long isFirstAgglomeration_long,
                         long isAnisotropic_long,

                         long *fineCellToCoarseCell,

                         long *agglomerationLines_Idx,
                         long *agglomerationLines,

                         long dimension,
                         long goalCard,
                         long minCard,
                         long maxCard,
                         long checks_long,
                         long verbose_long) {
    //"""
    // Main function of the agglomerator.
    // It agglomerates one MG level.
    //:param isAnisotropic:  [boolean], do we want an anisotropic agglomeration (True) or an isotropic (False)?
    //:param goalCard: [int] goal cardinal of the coarse cells
    //:param minCard: [int] minimum cardinal of the coarse cells
    //:param maxCard: [int] maximum cardinal of the coarse cells
    //:param checks: [boolean]
    //:param verbose: [boolean]
    //"""
    bool checks = checks_long == 1;
    bool verbose = verbose_long == 1;

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

    bool isFirstAgglomeration = isFirstAgglomeration_long == 1;
    bool isAnisotropic = isAnisotropic_long == 1;
    store_agglomeration_datas(sizes,
                              adjMatrix_row_ptr,
                              adjMatrix_col_ind,
                              adjMatrix_areaValues,
                              volumes,

                              arrayOfFineAnisotropicCompliantCells,

                              isOnFineBnd_l,
                              array_isOnValley,
                              array_isOnRidge,
                              array_isOnCorner,
                              isFirstAgglomeration_long, isAnisotropic_long, fineCellToCoarseCell,
                              agglomerationLines_Idx,
                              agglomerationLines, dimension, goalCard, minCard, maxCard, checks_long, verbose_long);

    // Initialization of isOnValley, isOnRidge, isOnCorner;
    // ATTENTION, we work on sets!
    unordered_set<long> isOnValley, isOnRidge, isOnCorner;
    for (long iOV = 0; iOV < isOnValley_size; iOV++) {
        isOnValley.insert(array_isOnValley[iOV]);
    }

    for (long iOR = 0; iOR < isOnRidge_size; iOR++) {
        isOnRidge.insert(array_isOnRidge[iOR]);
    }

    for (long iOC = 0; iOC < isOnCorner_size; iOC++) {
        isOnCorner.insert(array_isOnCorner[iOC]);
    }

//    long numberOfFineAnisotropicCompliantCells = 0;
//    if(!isFirstAgglomeration) {//    if(arrayOfFineAnisotropicCompliantCells!= NULL) {
//        numberOfFineAnisotropicCompliantCells = arrayOfFineAnisotropicCompliantCells_size;
//    }
    long numberOfFineAnisotropicCompliantCells = arrayOfFineAnisotropicCompliantCells_size;

    // Definition of minCard
    if (minCard == -1) {
        if (dimension == 2) {
            minCard = 3;
        } else {
            minCard = 6;
        }
    }

    // Definition of maxCard
    if (maxCard == -1) {
        if (dimension == 2) {
            maxCard = 5;
        } else {
            maxCard = 10;
        }
    }

    // Definition of goalCard
    if (goalCard == -1) {
        if (dimension == 2) {
            goalCard = 4;
        } else {
            goalCard = 8;
        }
    }

    // Definition of thresholdCard
    int thresholdCard;
    if (dimension == 2) {
        thresholdCard = 2;
    } else {
        thresholdCard = 3;
    }

    // Keep track of agglomerated fine cell
    long indCoarseCell = 0;
    numberOfFineAgglomeratedCells = 0;  // number of fine (already) agglomerated cells
    bool *isFineCellAgglomerated = new bool[numberOfFineCells];
    for (long i = 0; i < numberOfFineCells; i++) {
        isFineCellAgglomerated[i] = false;
    }



    // TODO On pourrait ne l'allouer qu'une seule fois!
    // fineAgglomerationLines = None
    // fineAgglomerationLines_for_visu = None

    //fineAgglomerationLines_array_Idx = None
    //fineAgglomerationLines_array = None

    long *fineAgglomerationLines_for_visu_array_Idx = NULL;
    long *fineAgglomerationLines_for_visu_array = NULL;

//    long* arrayOfCoarseAnisotropicCompliantCells = NULL;
    long numberOfAnisotropicLinesPOne_size = 0;
//    long agglomerationLines_size = 0;
    bool isAnisotropicLines = false;
    if (isAnisotropic) {
        if (isFirstAgglomeration) {
            numberOfAnisotropicLinesPOne_size = agglomerationLines_Idx_size;
            agglomerationLines_size = agglomerationLines_size;

//            cout << "Call of ALM.computeAnisotropicLine" << endl;
            isAnisotropicLines = computeAnisotropicLine(sizes,

                                                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues,

                                                        arrayOfFineAnisotropicCompliantCells,
                                                        agglomerationLines_Idx,
                                                        agglomerationLines,
                                                        verbose);

//            numberOfAnisotropicLinesPOne_size = sizes[3];  // number of agglomeration lines +1
//            agglomerationLines_size = sizes[4];
            numberOfAnisotropicLinesPOne_size = sizes[8];
            agglomerationLines_size = sizes[9];
//            cout << "numberOfAnisotropicLinesPOne_size " << numberOfAnisotropicLinesPOne_size << endl;
//            cout << "agglomerationLines_size " << agglomerationLines_size << endl;
//          Pas de resize, c'est pas possible avec un tableau
//            agglomerationLines_Idx.resize((numberOfAnisotropicLinesPOne_size,), refcheck = False)
//            agglomerationLines.resize((agglomerationLines_size,), refcheck = False)

            // For Visu only:
//            fineAgglomerationLines_for_visu_array_Idx = np.copy(agglomerationLines_Idx);
//            fineAgglomerationLines_for_visu_array = np.copy(agglomerationLines);
        }
        if (isAnisotropicLines) {

//            cout << "agglomerationLines_size " << agglomerationLines_size << endl;
//            long arrayOfCoarseAnisotropicCompliantCells_size = agglomerationLines_size; //np.shape(arrayOfFineAnisotropicCompliantCells)[0]
//            arrayOfCoarseAnisotropicCompliantCells = np.zeros((arrayOfCoarseAnisotropicCompliantCells_size,), dtype = int)
            agglomerationLines_Idx_size = sizes[8];
//            cout << "sizes[0]= " << sizes[0] << endl;
//            cout << "sizes[1]= " << sizes[1] << endl;
//            cout << "sizes[2]= " << sizes[2] << endl;
//            cout << "sizes[3]= " << sizes[3] << endl;
//            cout << "sizes[4]= " << sizes[4] << endl;
//            cout << "sizes[5]= " << sizes[5] << endl;
//            cout << "sizes[6]= " << sizes[6] << endl;
//            cout << "sizes[7]= " << sizes[7] << endl;
//            cout << "sizes[8]= " << sizes[8] << endl;
//            cout << "sizes[9]= " << sizes[9] << endl;

            long sizes_aniso[5] = {agglomerationLines_Idx_size, numberOfFineCells, numberOfFineAgglomeratedCells, indCoarseCell, -1};

            agglomerate_Anisotropic_One_Level_without_list_lines(sizes_aniso,
                                                                 agglomerationLines_Idx,
                                                                 agglomerationLines,
                                                                 fineCellToCoarseCell, isFineCellAgglomerated, arrayOfFineAnisotropicCompliantCells);//arrayOfCoarseAnisotropicCompliantCells);
//            cout << "End of agglomerate_Anisotropic_One_Level_without_list_lines" << endl;
//            agglomerationLines_Idx_size = sizes_aniso[0];
//            numberOfFineCells = sizes_aniso[1];
//            numberOfFineAgglomeratedCells = sizes_aniso[2];
//            indCoarseCell = sizes_aniso[3];
//            long arrayOfCoarseAnisotropicCompliantCells_size = sizes_aniso[4];
//            long numberOfFineCells = sizes[0];
//            long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//            long adjMatrix_col_ind_size = sizes[1];
//            long adjMatrix_areaValues_size = sizes[1];
//
//            // Rmk: sizes[2] ==indCoarseCell
//            long numberOfFineAgglomeratedCells = sizes[3];
//            long isOnValley_size = sizes[4];
//            long isOnRidge_size = sizes[5];
//            long isOnCorner_size = sizes[6];
//            long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
//            long agglomerationLines_Idx_size = sizes[8];
//            long agglomerationLines_size = sizes[9];
            sizes[2] = sizes_aniso[3];
            sizes[3] = sizes_aniso[2];
            sizes[7] = sizes_aniso[4];
            sizes[8] = sizes_aniso[0];
            sizes[9] = sizes[8] - 1;


//            if (agglomerationLines_Idx_size>0){
//
//            agglomerationLines_Idx.resize((agglomerationLines_Idx_size,), refcheck = False)
//            agglomerationLines.resize((agglomerationLines_Idx[agglomerationLines_Idx_size - 1],), refcheck = False)

//            if
//                arrayOfCoarseAnisotropicCompliantCells
//                        is
//                not None:
//            arrayOfCoarseAnisotropicCompliantCells.resize((arrayOfCoarseAnisotropicCompliantCells_size,), refcheck = False)
        }
    }

//    long sizes_iso[4] = {numberOfFineCells, adjMatrix_row_ptr_size, indCoarseCell, numberOfFineAgglomeratedCells};
//    cout<<"sizes[0]= "<<sizes[0]<<endl;
//    cout<<"sizes[1]= "<<sizes[1]<<endl;
//    cout<<"sizes[2]= "<<sizes[2]<<endl;
//    cout<<"sizes[3]= "<<sizes[3]<<endl;
//    cout<<"sizes[4]= "<<sizes[4]<<endl;
//    cout<<"sizes[5]= "<<sizes[5]<<endl;
//    cout<<"sizes[6]= "<<sizes[6]<<endl;
//    cout<<"sizes[7]= "<<sizes[7]<<endl;
//    cout<<"sizes[8]= "<<sizes[8]<<endl;
//    cout<<"sizes[9]= "<<sizes[9]<<endl;
    int *isOnFineBnd = new int[numberOfFineCells];
    for (long iL = 0; iL < numberOfFineCells; iL++) {
        isOnFineBnd[iL] = isOnFineBnd_l[iL];
    }
    agglomerate_Isotropic_One_Level_v_2(sizes,
                                        adjMatrix_row_ptr,
                                        adjMatrix_col_ind,
                                        adjMatrix_areaValues,
                                        volumes,
                                        fineCellToCoarseCell,
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
    // Rmk: sizes[2] ==indCoarseCell
//    long numberOfFineAgglomeratedCells = sizes[3];
//    long isOnValley_size = sizes[4];
//    long isOnRidge_size = sizes[5];
//    long isOnCorner_size = sizes[6];
//    long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
//    long agglomerationLines_Idx_size = sizes[8];
//    long agglomerationLines_size = sizes[9];

//    sizes[2] = sizes_iso[2];  //indCoarseCell
//    sizes[3] = sizes_iso[3];  //numberOfFineAgglomeratedCells
//    sizes[7] = arrayOfFineAnisotropicCompliantCells_size;
//    sizes[8] = arrayOfFineAnisotropicCompliantCells_size;
//    sizes[9] = arrayOfFineAnisotropicCompliantCells_size;
//    return indCoarseCell, fineAgglomerationLines_for_visu_array_Idx, fineAgglomerationLines_for_visu_array, agglomerationLines_Idx, \
//           agglomerationLines,  \
//           arrayOfCoarseAnisotropicCompliantCells
    delete[] isFineCellAgglomerated;
}

////Main function
void agglomerate_Isotropic_One_Level_v_2(long *sizes,

                                         long *matrixAdj_CRS_row_ptr,
                                         long *matrixAdj_CRS_col_ind,
                                         double *matrixAdj_CRS_values,
                                         double *volumes,

                                         long *fineCellToCoarseCell,
                                         bool *isFineCellAgglomerated,

                                         unordered_set<long> &isOnValley,
                                         unordered_set<long> &isOnRidge,
                                         unordered_set<long> &isOnCorner,
                                         int *isOnFineBnd,  //size numberOfFineCells
                                         int minCard,
                                         int goalCard,
                                         int maxCard,
                                         int thresholdCard,
                                         bool checks,
                                         bool verbose) {
//    isotropic agglomeration of one level of mesh.
//    V2: On n'agglomere pas necessairement toutes les cellules voisines
//      :param iLevel: level of Coarse grid to generate: 1 to nbOfCoarseLevel
//      :param matrixAdj_CRS_row_ptr:
//      :param matrixAdj_CRS_col_ind:
//      :param matrixAdj_CRS_values: unused up to now. We do not use any geometric information for isotropic agglo
//      :param volumes: np array of volume of every fine cell
//      :return:

    long numberOfFineCells = sizes[0];
    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
    long adjMatrix_col_ind_size = sizes[1];
    long adjMatrix_areaValues_size = sizes[1];

    long numberOfFineAgglomeratedCells = sizes[3];
//    long isOnValley_size = sizes[4];
//    long isOnRidge_size = sizes[5];
//    long isOnCorner_size = sizes[6];

    // Not mutualized with anisotropic agglomeration!!!!
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;  // Contains the coarse cells that can be modified (not anisotropic coarse cells (, and not "boundary" coarse cells! not sure what that mean!)
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;  // Contains the coarse cells that can be modified indexed by their cardinal i.e. {card: set of index of coarse cells of cardinal card}
    list<unordered_set<long>> delayedCoarseCells;
    // numberOfFineCells = np.shape(isFineCellAgglomerated)[0]
    // I) First try: we work on every fine cell
    //
    agglomerate_Isotropic_First_Step(sizes,
                                     matrixAdj_CRS_row_ptr,
                                     matrixAdj_CRS_col_ind,
                                     matrixAdj_CRS_values, volumes,
                                     dict_DistributionOfCardinalOfCoarseElements,
                                     dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                     isOnValley,
                                     isOnRidge,
                                     isOnCorner,
                                     isFineCellAgglomerated,
                                     isOnFineBnd,
                                     fineCellToCoarseCell,
                                     delayedCoarseCells,
                                     goalCard,
                                     thresholdCard,
                                     maxCard);
//    cout << "\t---->\tDelayedCoarseCells [";
//    for (auto iKV:delayedCoarseCells) {
//        cout << "\t{";
//        for (long iFC:iKV) {
//            cout << iFC << ", ";
//        }
//        cout << "}";
//    }
//    cout << "]" << endl;
    long indCoarseCell = sizes[2];
    agglomerate_Isotropic_CreateDelayedCoarseCells(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                                   delayedCoarseCells, indCoarseCell,
                                                   fineCellToCoarseCell);
    sizes[2] = indCoarseCell;
//    cout << "\nAfter First Step : " << endl;
//    cout << "Distribution of isotropic coarse cells : [";//, dict_DistributionOfCardinalOfCoarseElements
//    for (auto iKV:dict_DistributionOfCardinalOfCoarseElements) {
//        cout << "{" << iKV.first << ":" << iKV.second << "}";
//    }
//    cout << "]" << endl;
//    cout<<"\tdict_Coarse_Cells [";
//    for(auto iKV:dict_Coarse_Cells){
//        cout<<"\n\t"<<iKV.first<<": {";
//        for (long iFC:iKV.second)
//        {
//            cout<<iFC<<", ";
//        }
//        cout<<"}";
//    }
//    cout<<"]"<<endl;
//    if (checks) {
//        cout<<"\tChecks!"<<endl;
//        cout<<"\t\t consistancy"<<endl;
//        agglomerate_Isotropic_CheckConsistancyDictCoarseCells(dict_Coarse_Cells,
//                                                              dict_Card_Coarse_Cells, sizes[0], fineCellToCoarseCell);
//        cout<<"\t\t connectivity"<<endl;
//        // Phase de verification!
//        for (auto index :dict_Coarse_Cells) {
//
////                unordered_set<long> s = dict_CoarseCells[index];
//            if (!checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
//                checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, 1);
//                cout << "Error for coarse cell " << index.first << " [";
//                for (long iFC: index.second) {
//                    cout << iFC << ", ";
//                }
//                cout << "]" << endl;
//
//                throw logic_error("Connectivity Error");
//            }
//        }
//    }

    // TODO ATTENTION AU CHANGEMENT DE LISTE AVEC LE DELETE!
    // TODO voir TP python. Peut etre faitre une boucle tant que non vide...
    // II) Correction: treatment of incomplete cells!
    if (!dict_DistributionOfCardinalOfCoarseElements.empty()) {
        if (dict_DistributionOfCardinalOfCoarseElements.size() != 1 || dict_DistributionOfCardinalOfCoarseElements.count(goalCard) == 0) {

            agglomerate_Isotropic_Second_Step_Correction(6, sizes,
                                                         matrixAdj_CRS_row_ptr,
                                                         matrixAdj_CRS_col_ind, matrixAdj_CRS_values, volumes,
                                                         dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                                         dict_DistributionOfCardinalOfCoarseElements,
//                    indCoarseCell,
//                    numberOfFineAgglomeratedCells,
                                                         isFineCellAgglomerated,
                                                         fineCellToCoarseCell,
                                                         isOnFineBnd,
                                                         minCard,
                                                         goalCard,
                                                         maxCard,
                                                         thresholdCard,
                                                         checks,
                                                         verbose);
        }
    }
//    cout << "\nAfter Second Step : " << endl;
//    cout << "Distribution of isotropic coarse cells : [";//, dict_DistributionOfCardinalOfCoarseElements
//    for (auto iKV:dict_DistributionOfCardinalOfCoarseElements) {
//        cout << "{" << iKV.first << ":" << iKV.second << "}";
//    }
//    cout << "]" << endl;

    if (checks) {

        if (!dict_Coarse_Cells.empty()) {
            long nbCoarseElem = -1;
            for (auto iKVCC : dict_Coarse_Cells) {
                if (nbCoarseElem < iKVCC.first) {
                    nbCoarseElem = iKVCC.first;
                }
            }
//            list_tmp_tmp = dict_Coarse_Cells.keys();
//            nbCoarseElem = max(list_tmp_tmp)
            indCoarseCell = sizes[2];
//            cout << "indCoarseCell " << indCoarseCell << " nbCoarseElem " << nbCoarseElem << endl;
            assert(indCoarseCell == (nbCoarseElem + 1));
        }
        // Phase de verification!
        for (auto index:dict_Coarse_Cells) {
            unordered_set<long> l = index.second;
            if (!checkConnectivity_w_set(l, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                checkConnectivity_w_set(l, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, verbose = 1);
                cout << "Error for coarse cell " << index.first << " [";
                for (long iFC: index.second) {
                    cout << iFC << ", ";
                }
                cout << "]" << endl;

                throw logic_error("Connectivity Error");
            }
        }


    }
}


void agglomerate_Isotropic_First_Step(long *sizes,
                                      long *matrixAdj_CRS_row_ptr,
                                      long *matrixAdj_CRS_col_ind,
                                      double *matrixAdj_CRS_values,
                                      double *volumes,
                                      unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseCells,
                                      unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                      unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
        // numberOfFineCells,
                                      unordered_set<long> &isOnValley,
                                      unordered_set<long> &isOnRidge,
                                      unordered_set<long> &isOnCorner,
        // indCoarseCell,
        // numberOfFineAgglomeratedCells,
                                      bool *isFineCellAgglomerated,
                                      int *isOnFineBnd,
                                      long *fineCellToCoarseCell,
                                      list<unordered_set<long>> &delayedCoarseCells,
                                      int goalCard,
                                      int thresholdCard,
                                      int maxCard) {

    // First try to build an agglomeration.
    //  From the boundary of the domain, (corner, ridge or valley) we build agglomerated cells of cardinal __goalCard
    //        :param iLevel: coarse level
    //        :param matrixAdj_CRS_row_ptr: matrix of adjacency of the dual graph
    //        :param matrixAdj_CRS_col_ind: matrix of adjacency of the dual graph
    //        :param matrixAdj_CRS_values: matrix of adjacency of the dual graph
    //        :param volumes: np.array of volume of fines cells (iLevel -1)
    //        :param dict_DistributionOfCardinalOfCoarseCells: dictionary associating the cardinal number to the number of coarse cells of this size.
    //  :param dict_Coarse_Cells:
    //  :param dict_Card_Coarse_Cells:
    //  :return:
    //

    // We want the seed to be chosen preferably in the corner, then ridges, valleys and then interiors.
    // The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
    // to minimizes the too small cells!
    // IsOnBnd table can then contains 4 values:
    // 0 : interior
    // 1 : valey
    // 2 : ridge
    // 3 : corner

    // The size 4 corresponds to 0 : interior, 1 : valey, 2 : ridge, 3 : corner
    long numberOfFineCells = sizes[0];
    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
    long adjMatrix_col_ind_size = sizes[1];
    long adjMatrix_areaValues_size = sizes[1];
    long indCoarseCell = sizes[2];
    long numberOfFineAgglomeratedCells = sizes[3];
    vector<queue<long>> listOfSeeds(4);
    for (int i = 0; i < 4; i++) {
        listOfSeeds[i] = queue<long>();
    }


    int numberOfOrderOfNeighbourhood = 3;

    if (isOnCorner.size() > 0) {
        for (auto iFC: isOnCorner) {
            listOfSeeds[3].push(iFC);
        }

    }

    while (numberOfFineAgglomeratedCells < numberOfFineCells) {
//        cout << "===> numberOfFineAgglomeratedCells " << numberOfFineAgglomeratedCells << endl;
        // 1) Choice of seed
        //////////////////////////////////////
        // no dict in or out
        long seed = agglomerate_Isotropic_Choice_Of_Seed(listOfSeeds, numberOfFineCells, isFineCellAgglomerated, isOnRidge, isOnValley);

        // 2) Computation of neighbourhood
        //////////////////////////////////////////////////////////////////
        // dict: out
        unordered_map<long, int> dict_Neighbours_Of_Seed = computation_Of_Neighbourhood(seed,
                                                                                        numberOfOrderOfNeighbourhood,
                                                                                        matrixAdj_CRS_row_ptr,
                                                                                        matrixAdj_CRS_col_ind, maxCard,
                                                                                        isFineCellAgglomerated);

        // 3) Computation of optimal coarse cell
        //////////////////////////////////////////////////////////////////////////////
        // dict: in and out

        agglomerate_Isotropic_Choice_Of_Agglomerated_Cells(seed, listOfSeeds,
                                                           dict_Neighbours_Of_Seed,
                                                           matrixAdj_CRS_row_ptr,
                                                           matrixAdj_CRS_col_ind,
                                                           matrixAdj_CRS_values, volumes,
                                                           dict_Coarse_Cells,
                                                           dict_Card_Coarse_Cells,
                                                           dict_DistributionOfCardinalOfCoarseCells,
                                                           indCoarseCell,
                                                           numberOfFineAgglomeratedCells,
                                                           isFineCellAgglomerated,
                                                           fineCellToCoarseCell,
                                                           delayedCoarseCells,
                                                           isOnFineBnd,
                                                           goalCard, thresholdCard, maxCard);
    }
    sizes[2] = indCoarseCell;
    sizes[3] = numberOfFineAgglomeratedCells;

}


void agglomerate_Isotropic_CreateDelayedCoarseCells(unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                                    unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                    list<unordered_set<long>> delayedCoarseCells,
                                                    long &indCoarseCell,
                                                    long *fineCellToCoarseCell) {

    for (auto listCoarseCell : delayedCoarseCells) {
        createADelayedCoarseCell(listCoarseCell, dict_Coarse_Cells, dict_Card_Coarse_Cells, indCoarseCell, fineCellToCoarseCell);

    }
}


unordered_map<long, queue<long> *> findSeedViaFrontalMethod(int numberOfInts, long *sizes,
                                                            vector<long> listOfFineCells,
                                                            long *matrixAdj_CRS_row_ptr,
                                                            long *matrixAdj_CRS_col_ind) {


//"""
//This function finds the fine cells in the list listOfFineCells that defined the biggest distance of the covering tree.
//:param listOfFineCells:
//:param matrixAdj_CRS_row_ptr:
//:param matrixAdj_CRS_col_ind:
//:return:
//"""
    long numberOfFineCells = sizes[0];
    long matrixAdj_CRS_col_ind_size = sizes[1];

    long nbIteration = 5;

    long seed = listOfFineCells[0];
    int argSeed = 0;  //TODO Check int/Long
    unordered_set<long> possibleSeed;
    int iteration = 0;

    long max_seed = -1;
    int max_dist = -1;
    int max_length = -1;
    unordered_map<long, queue<long> *> max_dict;

    int nbOfCells = listOfFineCells.size();
    unordered_map<long, int> dict_inv_listOfFineCells;
    unordered_set<long> setOfFineCells(nbOfCells);
    for (int i = 0; i < nbOfCells; i++) {
        dict_inv_listOfFineCells[listOfFineCells[i]] = i;
        setOfFineCells.insert(listOfFineCells[i]);
    }


    vector<int> colour = vector<int>(nbOfCells);//-1 * np.ones((nbOfCells,), dtype=int)

    // Loop to find a good seed.
    // Many iteration are needed.
    while (iteration < nbIteration) {

        // print "\niter", iteration
        unordered_map<long, queue<long> *> dict_ConnectivityTree;

        // parcours en profondeur
        //Initialisation of the vector to -1;
        for (int i = 0; i < nbOfCells; i++) {
            colour[i] = -1;
        }

        colour[argSeed] = 0;
        int maxColour = 0;

        queue<long> queueOfNewSeed = queue<long>({seed});

        long old_seed = seed;

        while (!queueOfNewSeed.empty()) {

            seed = queueOfNewSeed.front();
//            cout<<iteration<<" seed "<<seed<<endl;
            queueOfNewSeed.pop();
            argSeed = dict_inv_listOfFineCells[seed];

            long ind = matrixAdj_CRS_row_ptr[seed];
            long ind_p_one = matrixAdj_CRS_row_ptr[seed + 1];
            for (long iNCell = ind; iNCell < ind_p_one; ++iNCell) // Process of Neighbours
            {
                long indNeighborCell = matrixAdj_CRS_col_ind[iNCell];
//                cout<<"\t"<<indNeighborCell<<endl;
                if ((indNeighborCell != seed) && (setOfFineCells.count(indNeighborCell) == 1)) {

                    int arg = dict_inv_listOfFineCells[indNeighborCell];

                    if (colour[arg] == -1) {
                        colour[arg] = colour[argSeed] + 1;
                        queueOfNewSeed.push(indNeighborCell);
                        //cout<<"queueOfNewSeed.front() "<<queueOfNewSeed.front()<<endl;
                        if (maxColour < colour[argSeed] + 1) {
                            maxColour = colour[argSeed] + 1;
                        }
                    }
                    // building of connectivity tree:
                    if (dict_ConnectivityTree.count(seed) == 1) {
                        (*dict_ConnectivityTree[seed]).push(indNeighborCell);
                    } else {
                        dict_ConnectivityTree[seed] = new queue<long>({indNeighborCell});
                    }

                }
            }
        }
        if (max_length <= maxColour) {  // max length c'est la longueur max pour toutes les iterations

            int dist = computeDistanceFromSeedTofirstVertexOfDegree2(old_seed, dict_ConnectivityTree);
            if (max_length < maxColour) {
                possibleSeed.clear();
            }
            max_length = maxColour;  // max length c'est la longueur max pour toutes les iterations

            if (max_dist <= dist) {
                max_seed = old_seed;
                max_dist = dist;

                // Deep copy:
                //unordered_map<long, queue<long>*> max_dict;
                //max_dict.erase();
                for (auto iPairMD:max_dict) {
                    delete iPairMD.second;
                }
                for (auto iPairDict:dict_ConnectivityTree) {
                    max_dict[iPairDict.first] = iPairDict.second;
//                    while(!(*iPair.second).empty())
//                    {
//                        (* max_dict[iPair.first]).push((*iPair.second).front());
//                        (*iPair.second).pop();
//                    }
                }
                //max_dict = copy.deepcopy(dict_ConnectivityTree);
            } else {
                // Destruction of the pointer to queue
                for (auto iPairDict:dict_ConnectivityTree) {
                    delete iPairDict.second;
                }
            }
        }
        unordered_set<long> set_max_New_Seed;
        for (int i = 0; i < nbOfCells; ++i) {
            if (colour[i] == maxColour) {
                set_max_New_Seed.insert(listOfFineCells[i]);
            }
        }

        if (set_max_New_Seed.size() == 1) {

            for (auto s : set_max_New_Seed) {
                seed = s;  //set_max_New_Seed.pop();
            }

            if (possibleSeed.count(seed) == 0) {
                argSeed = dict_inv_listOfFineCells[seed];
                possibleSeed.insert(old_seed);
            } else {
                sizes[2] = max_seed;
                return max_dict;
            }
        } else {
            bool isAllSeedTested = true;

            for (auto newSeed: set_max_New_Seed) {

                if (possibleSeed.count(newSeed) == 0) {
                    isAllSeedTested = false;
                    seed = newSeed;
                    argSeed = dict_inv_listOfFineCells[seed];
                    possibleSeed.insert(old_seed);
                }
            }
            if (isAllSeedTested) {
                //seed = max_seed;
                sizes[2] = max_seed;
                return max_dict;
            }

        }
        iteration += 1;

        // print "maxColour", maxColour
        // print "maxDist", max_dist, "seed", max_seed, "dict", max_dict
//        return max_seed, max_dict;
    }
    sizes[2] = max_seed;
    return max_dict;
}

int computeDistanceFromSeedTofirstVertexOfDegree2(long seed, unordered_map<long, queue<long> *> dict_Covering_Tree) {
    // TODO Cette fonction ne me parait pas ausi utile que pourrait le laisser penser son nom.
    // TODO En gros vu les arbres passes en arg, le retour est toujours 0 ou 1.
    // TODO et le nom n'est pas top, car le degre peut etre de plus de 2.
    long iter_seed = seed;
    int dist = 0;
    while (dict_Covering_Tree.count(iter_seed) == 1) {

        if ((*dict_Covering_Tree[iter_seed]).size() == 1) {
            iter_seed = (*dict_Covering_Tree[iter_seed]).front();
            dist += 1;
        } else {
            return dist;
        }
    }
    return dist;
}


unordered_map<long, int> computation_Of_Neighbourhood(long seed, int numberOfOrderOfNeighbourhood,
                                                      long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind,
                                                      int maxCard,
                                                      bool *isFineCellAgglomerated_tmp,
                                                      unordered_set<long> *setOfFineCells) {
//    cout<<"Call of computation_Of_Neighbourhood"<<endl;
    // This function computes the neighbourhood of a seed passed as argument.
    // It looks in the neighbourhood of order at least numberOfOrderOfNeighbourhood, but if the size of the set of neighbour
    // is too small (<maxCard), we look in higher order neighbourhood.
    //
    unordered_map<long, int> dict_Neighbours_Of_Seed;  // set (with unicity) des indices des cellules du 1er voisinage de seed
    unordered_map<long, int> dict_Neighbours_Of_Order_O_M_One;
    dict_Neighbours_Of_Order_O_M_One[seed] = 0;

    int iOrder = 1;
    bool isSetOfFineCellsTmp = false;
    if (setOfFineCells == nullptr) {
//        cout<<"setOfFineCells== nullptr "<<endl;
        isSetOfFineCellsTmp = true;
        setOfFineCells = new unordered_set<long>;
    }
//    else{
//        cout<<"setOfFineCells.size() "<<(*setOfFineCells).size()<<endl;
//    }
    // for iOrder in xrange(1, numberOfOrderOfNeighbourhood+1):
    while ((iOrder < numberOfOrderOfNeighbourhood + 1) ||
           (dict_Neighbours_Of_Seed.size() + dict_Neighbours_Of_Order_O_M_One.size()) < maxCard) {
        unordered_map<long, int> dict_Neighbours_Of_Order_O;
        //dict_Neighbours_Of_Seed.update(dict_Neighbours_Of_Order_O_M_One)
        for (auto id_M_one:dict_Neighbours_Of_Order_O_M_One) {
            dict_Neighbours_Of_Seed[id_M_one.first] = id_M_one.second;
        }
        for (auto seed_tmp : dict_Neighbours_Of_Order_O_M_One) {
            long ind = matrixAdj_CRS_row_ptr[seed_tmp.first];  // Usefull to find neighbours of seed
            long ind_p_one = matrixAdj_CRS_row_ptr[seed_tmp.first + 1]; // Usefull to find neighbours of seed
            for (long i = ind; i < ind_p_one; i++) {
                long indCell = matrixAdj_CRS_col_ind[i];
                if ((dict_Neighbours_Of_Seed.count(indCell) == 0) &&
                    ((!isFineCellAgglomerated_tmp[indCell] || !(*setOfFineCells).empty()))) {
                    if (dict_Neighbours_Of_Order_O.count(indCell) == 0) {
                        if (!(*setOfFineCells).empty()) {
                            if ((*setOfFineCells).count(indCell) == 1) {
                                dict_Neighbours_Of_Order_O[indCell] = iOrder;
                            }
                        } else {
                            dict_Neighbours_Of_Order_O[indCell] = iOrder;
                        }
                    }
                }
            }
        }

        // Exit condition
        if (dict_Neighbours_Of_Order_O.empty()) {
            // No more neighbours available:
            break;
        }

        //dict_Neighbours_Of_Order_O_M_One = dict_Neighbours_Of_Order_O;

        // Copy
        dict_Neighbours_Of_Order_O_M_One.clear();
        for (auto id:dict_Neighbours_Of_Order_O) {
            dict_Neighbours_Of_Order_O_M_One[id.first] = id.second;
        }
        iOrder += 1;
    }
    // Update of dict_Neighbours_Of_Seed
    //dict_Neighbours_Of_Seed.update(dict_Neighbours_Of_Order_O_M_One)
    for (auto id_M_one:dict_Neighbours_Of_Order_O_M_One) {
        dict_Neighbours_Of_Seed[id_M_one.first] = id_M_one.second;
    }

    // We remove the seed from the neighbours of seed
    // dict_Neighbours_Of_Seed.remove(seed)
    dict_Neighbours_Of_Seed.erase(seed);
    if (isSetOfFineCellsTmp) {
        delete setOfFineCells;
    }
//    cout << "iOrderMax " << iOrder << endl;
    return dict_Neighbours_Of_Seed;
}


int computeNumberOfCommonFaces(long iFine, long iCoarse,
                               long *matrixAdj_CRS_row_ptr,
                               long *matrixAdj_CRS_col_ind,
                               long *fine_Cell_indices_To_Coarse_Cell_Indices) {
    int nbCommonFaces = 0;

    long ind = matrixAdj_CRS_row_ptr[iFine];
    long ind_p_one = matrixAdj_CRS_row_ptr[iFine + 1];
    long indOfFineNeighbor = 0;
    for (int iN = ind; iN < ind_p_one; iN++) { //We process every neighbour of iFine
        indOfFineNeighbor = matrixAdj_CRS_col_ind[iN];
        if ((indOfFineNeighbor != iFine) && (fine_Cell_indices_To_Coarse_Cell_Indices[indOfFineNeighbor] == iCoarse)) {
            nbCommonFaces += 1;
        }
    }
    return nbCommonFaces;
}

int removeSeparatingVertex(long seed, unordered_map<long, queue<long> *> dict_ConnectivityTree,
                           unordered_set<long> &setOfFineCells,
                           long *matrixAdj_CRS_row_ptr,
                           long *matrixAdj_CRS_col_ind, long verbose) {

    //The bipartition of the too big cell is a complex problem.
    //This is a stupid version of a solution.
    //The goal is to remove from setOfFineCells the vertices that may split the coarse in more than two parts.
    //
    //:param seed:
    //:param dict_ConnectivityTree:
    //:param setOfFineCells:
    //:param matrixAdj_CRS_row_ptr:
    //:param matrixAdj_CRS_col_ind:
    //:return:
    //"""
    // We remove from the setOfFineCells the cells of degree greater than (or equal to) 3.

    long iter_seed = seed;
    unordered_set<long> firstCoarseCell;  // it was list we try set
    unordered_set<long> secondCoarseCell(setOfFineCells);  // it was list we try set = list(setOfFineCells)
    unordered_set<long> setRemovedCells;
    // print "setOfFineCells", setOfFineCells
    // If seed is not on a cycle, we move as much as possible until we face a vertex of third degree (Cycle).
    while (dict_ConnectivityTree.count(iter_seed) == 1) {
        if ((*dict_ConnectivityTree[iter_seed]).size() == 1) {
            firstCoarseCell.insert(iter_seed);
            secondCoarseCell.erase(iter_seed);
            iter_seed = (*dict_ConnectivityTree[iter_seed]).front();
        } else {
            break;
        }

    }
    if (verbose) {
        cout << "iter_seed " << iter_seed << " dict_ConnectivityTree.count(iter_seed) "
             << dict_ConnectivityTree.count(iter_seed) << endl;
        cout << "[";
        for (auto i : firstCoarseCell) {
            cout << i << ", ";
        }
        cout << "]";
        cout << "[";
        for (auto i : secondCoarseCell) {
            cout << i << ", ";
        }
        cout << "]" << endl;
    }
    if (dict_ConnectivityTree.count(iter_seed) == 1) {

        unordered_set<long> setL({iter_seed});  // = set([iter_seed])
        unordered_set<long> setLPlusOne;

        while (!setL.empty()) {

//            print "\nsetL", setL
            if (verbose) {
                cout << "\tsetL = [";

                for (auto iFineCell: setL) {
                    cout << iFineCell << ", ";
                }
                cout << "]" << endl;
            }
            unordered_set<long> setLCellsToRemove;
            unordered_set<long> tmp_SetL(setL);  // copy: we will remove some element during the computation
            if (verbose) {
                cout << "\ttmp_SetL = [";

                for (auto iFineCell: tmp_SetL) {
                    cout << iFineCell << ", ";
                }
                cout << "]" << endl;
            }
            for (int iLength = 1; iLength < setL.size() + 1; iLength++) {
                if (verbose) {
                    cout << "\t\tiLength " << iLength << endl;
                }
                vector<long> vector_tmp_SetL(tmp_SetL.size());
                int i = 0;
                for (auto iFC : tmp_SetL) {
                    vector_tmp_SetL[i] = iFC;
                    ++i;
                }
                list<unordered_set<long>> listOfListL = partsList(vector_tmp_SetL, iLength);
                // print "iLength", iLength, "listOfListL", listOfListL
                if (verbose) {
                    cout << "\t\tlistOfListL" << endl;
                }
                for (auto iSubList : listOfListL) {
                    if (verbose) {
                        cout << "\t\t\tiSubList = [";

                        for (auto iFcell : iSubList) {
                            cout << iFcell << ", ";
                        }
                        cout << "]" << endl;
                    }
                    unordered_set<long> tmp_set(secondCoarseCell);
                    for (auto iCell:iSubList) {
                        tmp_set.erase(iCell);
                    }
                    if (verbose) {
                        cout << "\t\t\ttmp_set = [";

                        for (auto iFcell : tmp_set) {
                            cout << iFcell << ", ";
                        }
                        cout << "]" << endl;
                    }
//                    vector<long> v_tmp_list(tmp_set.size());
//                    int i = 0;
//                    for (auto iCell :tmp_set) {
//                        v_tmp_list[i] = iCell;
//                        ++i;
//                    }
                    if (verbose) {
                        cout << "\t\t\t=== 4" << endl;
                    }
                    if (!checkConnectivity_w_set(tmp_set, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                        if (verbose) {
                            cout << "\t\t\t!checkConnectivity" << endl;
                        }
                        for (auto iCell : iSubList) {

                            if (tmp_SetL.count(iCell)) {
                                tmp_SetL.erase(iCell);
                            }
                            if (setOfFineCells.count(iCell)) {
                                setLCellsToRemove.insert(iCell);
                            }
                        }
                    }
                    if (verbose) {
                        cout << "\t\t\t=== 5" << endl;
                    }
                }
            }
            if (verbose) {
                cout << "\t=== 6" << endl;
            }


            // cout<< "setLCellsToRemove"<<endl;
            // setLCellsToRemove
            unordered_set<long> setL_Minus_setLCellsToRemove(setL);
            for (long iCell : setLCellsToRemove) {
                setL_Minus_setLCellsToRemove.erase(iCell);
            }
            if (verbose) {
                cout << "=== 7" << endl;
            }
            // Construction of the next level of the tree
            for (long iCell : setL_Minus_setLCellsToRemove) {
                if (verbose) { cout << "iCell " << iCell << endl; }
                if (dict_ConnectivityTree.count(iCell) == 1) {
                    if (verbose) { cout << "=== 8" << endl; }
                    long iN = (*dict_ConnectivityTree[iCell]).front();
                    if (verbose) { cout << "iN " << iN << endl; }
                    while (!(*dict_ConnectivityTree[iCell]).empty()) {
                        if (verbose) { cout << "iN " << iN << endl; }
                        (*dict_ConnectivityTree[iCell]).pop();
                        if ((setL.count(iN) == 0) && (firstCoarseCell.count(iN) == 0) &&
                            (setRemovedCells.count(iN) == 0)) {
                            if (verbose) { cout << "setLPlusOne.insert(" << iN << ")" << endl; }
                            setLPlusOne.insert(iN);
                        }
                        iN = (*dict_ConnectivityTree[iCell]).front();

                    }
                }
            }
            if (verbose) { cout << "Update" << endl; }
            for (auto id_Cell:setLCellsToRemove) {
                setRemovedCells.insert(id_Cell);  //setRemovedCells.merge(setLCellsToRemove);
            }

            //setOfFineCells -= setLCellsToRemove
            for (long iCell : setLCellsToRemove) {
                if (verbose) { cout << "setOfFineCells.erase(" << iCell << ")" << endl; }
                setOfFineCells.erase(iCell);
            }

            setL_Minus_setLCellsToRemove = setL;
            for (long iCell : setLCellsToRemove) {
                setL_Minus_setLCellsToRemove.erase(iCell);
            }

            for (auto iC :setL_Minus_setLCellsToRemove) {
                firstCoarseCell.insert(iC);
                secondCoarseCell.erase(iC);

            }
            if (verbose) { cout << "=== 9" << endl; }
            // print firstCoarseCell, secondCoarseCell
            // print setL, setLPlusOne
            if (verbose) {
                cout << "setLPlusOne" << endl;

                cout << "[";
                for (auto id_Ce:setLPlusOne) {
                    cout << id_Ce << ", ";
                }
                cout << "]" << endl;
            }
            setL = setLPlusOne;
            // listOfListL = partsList(list(setL))
            setLPlusOne.clear(); // = set([])
            if (verbose) {
                cout << "setL = [";

                for (auto id_Ce:setL) {
                    cout << id_Ce << ", ";
                }
                cout << "]" << endl;
                cout << "setLPlusOne = [";
                for (auto id_Ce:setLPlusOne) {
                    cout << id_Ce << ", ";
                }
                cout << "]" << endl;
                cout << "=== 10\n\n\n" << endl;
            }
        }
    }

    return 0;
}

list<unordered_set<long>> partsList(vector<long> seq, int length) {
    // generates all subparts of a list:
    list<unordered_set<long>> p;
    int i = 1;
    int iMax = pow(2, seq.size()) - 1;

    while (i <= iMax) {
        unordered_set<long> s;
        int j = 0, jmax = seq.size() - 1;
        while (j <= jmax) {
            if (((i >> j) & 1) == 1) {
                s.insert(seq[j]);
            }
            j += 1;
        }
        if (length > 0) {
            if (s.size() == length) {
                p.push_back(s);
            }
        } else {
            p.push_back(s);
        }
        i += 1;
    }
    return p;
}


//bool checkConnectivity(vector<long> listFineCells,
//                       long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind, int verbose) {
//
//
////"""
////Checks connectivity of the coarse cell
////        :param listFineCells: List of fine cells defining the coarse element.
////:param matrixAdj_CRS_row_ptr: dual graph information see MgridGen data structure
////        :param matrixAdj_CRS_col_ind: dual graph information see MgridGen data structure
////        :return: True or False
////"""
//
//    int size = listFineCells.size();
//    if (size <= 1) {
//        return true;
//    }
//
//    bool *isAlreadyConnected = new bool[size];
//    for (int i = 0; i < size; i++) {
//        isAlreadyConnected[i] = false;
//    }
//    isAlreadyConnected[0] = true;
//    unordered_set<long> setNext({listFineCells[0]});
//    int nbConnectedCells = 1;
//    unordered_map<long, int> dict_GlobalToLocal;
//    for (int indFineCell = 0; indFineCell < size; indFineCell++) {
//        dict_GlobalToLocal[listFineCells[indFineCell]] = indFineCell;
//    }
//
//
//    while (nbConnectedCells < size) {
//        if (!setNext.empty()) {
//            long iFineCell = *setNext.begin();
//            setNext.erase(setNext.begin());
//            long ind = matrixAdj_CRS_row_ptr[iFineCell];  // Usefull to find neighbours of seed
//            long ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1]; // Usefull to find neighbours of seed
//            for (long iFC = ind; iFC < ind_p_one; iFC++) {
//                long iFCellNeighbour = matrixAdj_CRS_col_ind[iFC];
//                if (dict_GlobalToLocal.count(iFCellNeighbour) == 1) {
//                    if ((iFCellNeighbour != iFineCell) && (!isAlreadyConnected[dict_GlobalToLocal[iFCellNeighbour]])) {
//                        setNext.insert(iFCellNeighbour);
//                        nbConnectedCells += 1;
//                        isAlreadyConnected[dict_GlobalToLocal[iFCellNeighbour]] = true;
//                    }
//                }
//            }
//        } else {
//            break;
//        }
//    }
//    if ((verbose) && (nbConnectedCells != size)) {
//        cout << "listFineCells " << endl;
//        int iCount = 0;
//        for (auto i : listFineCells) {
//            cout << i << " : " << isAlreadyConnected[iCount] << endl;
//            iCount++;
//        }
//
//        cout << "nbConnectedCells " << nbConnectedCells << " size " << size << endl;
//    }
//    delete[] isAlreadyConnected;
//    if ((verbose) && (nbConnectedCells != size)) {
//        cout << "UnconnectedCell: [";
//        for (int i = 0; i < listFineCells.size(); i++) {
//            cout << listFineCells[i] << ", ";
//        }
//        cout << "]" << endl;
//    }
//    return nbConnectedCells == size;
//
//}

bool checkConnectivity_w_set(unordered_set<long> setFineCells, long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind, int verbose) {

//"""
//Checks connectivity of the coarse cell
//        :param listFineCells: List of fine cells defining the coarse element.
//:param matrixAdj_CRS_row_ptr: dual graph information see MgridGen data structure
//        :param matrixAdj_CRS_col_ind: dual graph information see MgridGen data structure
//        :return: True or False
//"""
    unordered_map<long, bool> map_isAlreadyConnected;
    int size = setFineCells.size();
    if (size <= 1) {
        return true;
    }
    for (long iFC :setFineCells) {
        map_isAlreadyConnected[iFC] = false;
    }
    long front = *setFineCells.begin();
    map_isAlreadyConnected[front] = true;

    unordered_set<long> setNext({front});
    int nbConnectedCells = 1;

    while (nbConnectedCells < size) {

        if (!setNext.empty()) {
            long iFineCell = *setNext.begin();
            setNext.erase(setNext.begin());
            long ind = matrixAdj_CRS_row_ptr[iFineCell];  // Usefull to find neighbours of seed
            long ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1]; // Usefull to find neighbours of seed
            for (long iFC = ind; iFC < ind_p_one; iFC++) {
                long iFCellNeighbour = matrixAdj_CRS_col_ind[iFC];
                if (map_isAlreadyConnected.count(iFCellNeighbour) == 1) {
                    if ((iFCellNeighbour != iFineCell) && (!map_isAlreadyConnected[iFCellNeighbour])) {
                        setNext.insert(iFCellNeighbour);
                        nbConnectedCells += 1;
                        map_isAlreadyConnected[iFCellNeighbour] = true;
                    }
                }
            }
        } else {
            break;
        }
    }
    if ((verbose) && (nbConnectedCells != size)) {
        cout << "setFineCells " << endl;
        int iCount = 0;
        for (auto i : setFineCells) {
            cout << i << " : " << map_isAlreadyConnected[i] << endl;
            iCount++;
        }

        cout << "nbConnectedCells " << nbConnectedCells << " size " << size << endl;
    }

    return nbConnectedCells == size;

}

//unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
//
//unordered_map<long, list<long>> dict_Coarse_Cells;  // Contains the coarse cells that can be modified (not anisotropic coarse cells (, and not "boundary" coarse cells! not sure what that mean!)
//unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;  // Contains the coarse cells that can be modified indexed by their cardinal i.e. {card: set of index of coarse cells of cardinal card}
//list<list<long>> delayedCoarseCells;

unordered_set<long> swapFineCell(long iFineCell, long iOrigineCoarseCell, long iDestinationCoarseCell,
                                 unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                 unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                 unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                 long *fineCellToCoarseCell) {
//
//This function swaps the fine cell iFineCell which belongs to iOrigineCoarseCell coarse cell to iDestinationCoarseCell coarse cell.
//Warning: NO CONNECTIVITY CHECK in the old coarse element. BE CAREFULL!
//
//:param iLevel:
//:param iFineCell:
//:param iOrigineCoarseCell:
//:param iDestinationCoarseCell:
//:param dict_Coarse_Elem:
//:param dict_Card_Coarse_Cells:
//:param dict_DistributionOfCardinalOfCoarseElements:
//:param fineCellToCoarseCell:
//:return:
//
// TODO gerer si on detruit une cellule!
// TODO Verifier qu'on ne presuppose pas la connectivity!!!!

//    cout << "Swap " << iFineCell << " from " << iOrigineCoarseCell << " to " << iDestinationCoarseCell << endl;
    unordered_set<long> set_removedCoarseCells;

    int size = dict_Coarse_Elem[iOrigineCoarseCell].size();
    assert(size > 0);
    // Update of dict_Card_Coarse_Cells:

    // 1) We remove the cell from iOrigineCoarseCell
    assert(dict_Card_Coarse_Cells.count(size) == 1);
    assert(dict_DistributionOfCardinalOfCoarseElements.count(size));

    assert(dict_Card_Coarse_Cells[size].count(iOrigineCoarseCell));

    dict_Card_Coarse_Cells[size].erase(iOrigineCoarseCell);
    if (dict_Card_Coarse_Cells[size].empty()) {
        dict_Card_Coarse_Cells.erase(size);
    }

    dict_DistributionOfCardinalOfCoarseElements[size] -= 1;
    if (dict_DistributionOfCardinalOfCoarseElements[size] == 0) {
        dict_DistributionOfCardinalOfCoarseElements.erase(size);
    }

    if (size - 1 != 0) {
        if (dict_Card_Coarse_Cells.count(size - 1) == 1) {
            dict_Card_Coarse_Cells[size - 1].insert(iOrigineCoarseCell);
        } else {
            dict_Card_Coarse_Cells[size - 1] = unordered_set<long>({iOrigineCoarseCell});
        }

        if (dict_DistributionOfCardinalOfCoarseElements.count(size - 1)) {
            dict_DistributionOfCardinalOfCoarseElements[size - 1] += 1;
        } else {
            dict_DistributionOfCardinalOfCoarseElements[size - 1] = 1;
        }

        dict_Coarse_Elem[iOrigineCoarseCell].erase(iFineCell);

    } else {
        dict_Coarse_Elem.erase(iOrigineCoarseCell);
        set_removedCoarseCells.insert(iOrigineCoarseCell);
    }


    // 2) We add it to iDestinationCoarseCell
    int sizeDest = dict_Coarse_Elem[iDestinationCoarseCell].size();
    dict_Card_Coarse_Cells[sizeDest].erase(iDestinationCoarseCell);

    if (dict_Card_Coarse_Cells[sizeDest].empty()) {
        dict_Card_Coarse_Cells.erase(sizeDest);
    }

    if (dict_Card_Coarse_Cells.count(sizeDest + 1) == 1) {
        dict_Card_Coarse_Cells[sizeDest + 1].insert(iDestinationCoarseCell);
    } else {

        dict_Card_Coarse_Cells[sizeDest + 1] = unordered_set<long>({iDestinationCoarseCell});
    }

    dict_Coarse_Elem[iDestinationCoarseCell].insert(iFineCell);
    fineCellToCoarseCell[iFineCell] = iDestinationCoarseCell;

    // Add to compute the distribution of cardinal of cell
    int tmp_size = dict_Coarse_Elem[iDestinationCoarseCell].size();
    dict_DistributionOfCardinalOfCoarseElements[tmp_size - 1] -= 1;
    if (dict_DistributionOfCardinalOfCoarseElements[tmp_size - 1] == 0) {
        dict_DistributionOfCardinalOfCoarseElements.erase(tmp_size - 1);
    }
    if (dict_DistributionOfCardinalOfCoarseElements.count(tmp_size) == 1) {
        dict_DistributionOfCardinalOfCoarseElements[tmp_size] += 1;
    } else {
        dict_DistributionOfCardinalOfCoarseElements[tmp_size] = 1;
    }

    return set_removedCoarseCells;
}


void splitNonConnectedCoarseCell(long &indCoarseElement,
                                 long &numberOfFineAgglomeratedCells,
                                 long &iCoarseCell,
                                 unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                 unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                 unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                 long *matrixAdj_CRS_row_ptr,
                                 long *matrixAdj_CRS_col_ind,
                                 bool *isFineCellAgglomerated,
                                 long *fine_Cell_indices_To_Coarse_Cell_Indices) {
//    cout<<"Call of splitNonConnectedCoarseCell"<<endl;
//    cout<<"Unconnected Coarse Cell: [";
//    for (auto iFC : dict_Coarse_Cells[iCoarseCell]){
//        cout<<iFC<<", ";
//    }
//    cout<<"]"<<endl;
    //As a non connected coarse cell has been created (indexed iCoarseCell,
    //:param iCoarseCell:  index of the non-connex coarse cell

    list<unordered_set<long>> listOfSetConnectedComponent = computeConnectedComponent(dict_Coarse_Cells[iCoarseCell],
                                                                                      matrixAdj_CRS_row_ptr,
                                                                                      matrixAdj_CRS_col_ind);
//    cout<<" Coarse Cell 1: [";
//    for (auto iFC : listOfSetConnectedComponent.front()){
//        cout<<iFC<<", ";
//    }
//    cout<<"]"<<endl;
//
//    cout<<"Coarse Cell 2: [";
//    for (auto iFC : listOfSetConnectedComponent.back()){
//        cout<<iFC<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"indCoarseElement "<<indCoarseElement<<endl;
//    if (listOfSetConnectedComponent.size() > 2) {
//        throw invalid_argument("The coarse cell is splitted in more than 2");
//    }

    int argMax = 0;
    int maxLength = 0;

    // we could use only maxLength, but if the splitted coarse cell has the same size, there may be a problem, so we keep iL.
    int iL = 0;
    for (auto iSet :listOfSetConnectedComponent) {
        if (iSet.size() >= maxLength) {
            maxLength = iSet.size();

            argMax = iL;
        }
        iL++;
    }

    iL = 0;
    for (auto iSet :listOfSetConnectedComponent) {

        if (iL != argMax) {

            // print "iL != argMax"
            // print "Creation of coarse cell", indCoarseElement -1
            // Creation of an empty coarse cell:
            unordered_set<long> emptySet;
            createCoarseCell(emptySet, dict_Coarse_Cells, dict_Card_Coarse_Cells,
                             dict_DistributionOfCardinalOfCoarseElements, indCoarseElement,
                             numberOfFineAgglomeratedCells, isFineCellAgglomerated,
                             fine_Cell_indices_To_Coarse_Cell_Indices, true);

            vector<long> v(iSet.size());
            int i = 0;
            for (auto iFineCell:iSet) {

//                cout<<"\tSwap of fine cell "<< iFineCell<<" from "<<   iCoarseCell<<" to "<< indCoarseElement - 1<<endl;
                swapFineCell(iFineCell, iCoarseCell, indCoarseElement - 1, dict_Coarse_Cells,
                             dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                             fine_Cell_indices_To_Coarse_Cell_Indices);
//                cout<<"\tEND Swap of fine cell "<< iFineCell<<" from "<<   iCoarseCell<<" to "<< indCoarseElement - 1<<endl;
                v[i] = iFineCell;
                i++;
            }
            // Check the splitted cell (indCoarseElement - 1)
            assert(checkConnectivity_w_set(dict_Coarse_Cells[indCoarseElement - 1], matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind));


        }
        iL++;
    }
    // TODO Remove this in production
//    // Check the splitted cell (indCoarseElement - 1)
//    assert checkConnectivity(dict_Coarse_Cells[indCoarseElement - 1], matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)
//
//    // Check the original coarse cell
//    assert checkConnectivity(dict_Coarse_Cells[iCoarseCell], matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind);`
//    vector<long> v(dict_Coarse_Cells[iCoarseCell].size());
//    int i = 0;
//    for (auto iFineCell:dict_Coarse_Cells[iCoarseCell]) {
//        v[i] = iFineCell;
//        i++;
//    }
// Check the original coarse cell
//    cout<<"Final check on iCoarseCell "<<iCoarseCell<<endl;
    assert(checkConnectivity_w_set(dict_Coarse_Cells[iCoarseCell], matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind));
//    cout<<"End of Call of splitNonConnectedCoarseCell"<<endl;
}


list<unordered_set<long>> computeConnectedComponent(
        unordered_set<long> listInitialCoarseCell, long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind) {
    //can't do a set of set for hashing reason...
//"""
//With a non connected list of fine cell (listInitialCoarseCell), we compute connected components of the graph.
//:param listInitialCoarseCell: list of fine cells composing the initial coarse cell (not connected).
//:param matrixAdj_CRS_row_ptr: description of adjacency matrix
//        :param matrixAdj_CRS_col_ind: description of adjacency matrix
//        :return: a set of connected component (stored as frozenset)
//"""
    // TODO rename listOfConnectedSet to setsOfConnectedSet
    list<unordered_set<long>> listOfConnectedSet;

    int sizeCC = listInitialCoarseCell.size();
    if (sizeCC <= 1) {
//        listOfConnectedSet.insert(listInitialCoarseCell);  // a little strange as we insert empty set... This shhould not happend
//        return listOfConnectedSet;  //TODO generate an error instead!
        throw invalid_argument("Receive an empty arg listInitialCoarseCell");
    }
    bool *isAlreadyConnected = new bool[sizeCC];
    for (int i = 0; i < sizeCC; i++) {
        isAlreadyConnected[i] = false;
    }
    isAlreadyConnected[0] = true;
    long iFineCell_first = *listInitialCoarseCell.begin();
    unordered_set<long> setNext({iFineCell_first});
    int nbConnectedCells = 1;
    unordered_map<long, int> dict_GlobalToLocal;
//    for (int indFineCell=0;indFineCell<sizeCC;indFineCell++) {
//        dict_GlobalToLocal[listInitialCoarseCell[indFineCell]] = indFineCell;
//    }
    int indFineCell = 0;
    for (auto iCoarseCell : listInitialCoarseCell) {
        dict_GlobalToLocal[iCoarseCell] = indFineCell;
        indFineCell++;
    }
    unordered_set<long> setOfFineCell({iFineCell_first});
    while (nbConnectedCells < sizeCC) {

        if (!setNext.empty()) {
//            cout << "!setNext.empty()" << endl;
            long iFineCell = *setNext.begin();  // setNext.pop();
//            cout << "iFineCell " << iFineCell << endl;
            setNext.erase(setNext.begin());     // setNext.pop( );
//            cout << "setNext" << endl;
            for (auto iF:setNext) {
                cout << iF << ", ";
            }
//            cout << endl;
            long ind = matrixAdj_CRS_row_ptr[iFineCell];  // Usefull to find neighbours of seed
            long ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1];  // Usefull to find neighbours of seed
            for (long iFC = ind; iFC < ind_p_one; iFC++) {
                long iFCellNeighbour = matrixAdj_CRS_col_ind[iFC];
//                cout << "iFCellNeighbour " << iFCellNeighbour << endl;
                if (dict_GlobalToLocal.count(iFCellNeighbour) == 1) {
                    if ((iFCellNeighbour != iFineCell) && (!isAlreadyConnected[dict_GlobalToLocal[iFCellNeighbour]])) {
//                        cout << "setNext.insert " << iFCellNeighbour << endl;
                        setNext.insert(iFCellNeighbour);
                        nbConnectedCells += 1;
                        isAlreadyConnected[dict_GlobalToLocal[iFCellNeighbour]] = true;
                        setOfFineCell.insert(iFCellNeighbour);
                    }
                }
            }
        } else {
//            cout << "setNext.empty()" << endl;
            // End of a connected component
            listOfConnectedSet.push_back(setOfFineCell);
            long newSeed = -1;  // This is the new seed of the new connected component
            for (auto gTL:dict_GlobalToLocal) {
                if (!isAlreadyConnected[dict_GlobalToLocal[gTL.first]]) {
                    newSeed = gTL.first;
                    isAlreadyConnected[gTL.second] = true;
                    nbConnectedCells += 1;
                    break;
                }
            }

//            for(int i=0; i<sizeCC; i++)
//            {
//                if (!isAlreadyConnected[i]){
//                    argNewSeed = i;
//                    isAlreadyConnected[i] = true;
//                    nbConnectedCells += 1;
//                    break;
//                }
//            }
//            long newSeed = *listInitialCoarseCell.find(argNewSeed);
//            cout << "newSeed " << newSeed << endl;
            // Creation of a new connected component
            setOfFineCell = unordered_set<long>({newSeed});
            setNext = unordered_set<long>({newSeed});
        }
    }
    // We add the last connected component
    listOfConnectedSet.push_back(setOfFineCell);

    return listOfConnectedSet;
}


//unordered_map<long, unordered_set<long>>& dict_Coarse_Cells,
//        unordered_map<int, unordered_set<long>>& dict_Card_Coarse_Cells,
//unordered_map<int, long>& dict_DistributionOfCardinalOfCoarseElements,

void createCoarseCell(unordered_set<long> l,
                      unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                      unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                      unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                      long &indCoarseElement,
                      long &numberOfFineAgglomeratedCells_tmp,
                      bool *isFineCellAgglomerated_tmp,
                      long *Fine_Cell_indices_To_Coarse_Cell_Indices,
                      bool isMutable,
                      bool isCreationDelayed) {
//"""
//:param l:
//:param dict_Coarse_Elem:
//:param dict_Card_Coarse_Cells:
//:param dict_DistributionOfCardinalOfCoarseElements:
//:param indCoarseElement:
//:param numberOfFineAgglomeratedCells_tmp:
//:param isFineCellAgglomerated_tmp:
//:param Fine_Cell_indices_To_Coarse_Cell_Indices:
//:param isMutable: if True, the cell can be modified afterwards and is thus defined in  dict_Coarse_Elem and
//        dict_Card_Coarse_Cells
//        :param isCreationDelayed: the numbering of the coarse cell will be done latter
//        (for algorithmic consideration as the smallest cells wil be the first deleted.) Rk: if a cell is created with isCreationDelayed==True, it is necessary mutable.
//:return:
//"""
    if (!isCreationDelayed) {
        if (isMutable) {
            dict_Coarse_Elem[indCoarseElement] = l;

            // Update of dict_Card_Coarse_Cells:
            int card = l.size();
            // TODO Change dict_Card_Coarse_Cells[card] from list to set!
            if (dict_Card_Coarse_Cells.count(card)) {

                dict_Card_Coarse_Cells[card].insert(indCoarseElement);

            } else {

                dict_Card_Coarse_Cells[card] = {indCoarseElement};
            }
        }

        // Update of _associatedCoarseCellNumber the output of the current function agglomerate
        for (auto iFineCell : l) {
            if (!isFineCellAgglomerated_tmp[iFineCell]) {
                isFineCellAgglomerated_tmp[iFineCell] = true;  // Rq: initialise a False pour chaque niveau dans agglomerate(...)
                numberOfFineAgglomeratedCells_tmp += 1;
            }

            // TODO not check this in production
            assert(Fine_Cell_indices_To_Coarse_Cell_Indices[iFineCell] == -1);
//            , "Cell " + str(iFineCell) + " is Already assigned fine cell"
            Fine_Cell_indices_To_Coarse_Cell_Indices[iFineCell] = indCoarseElement;
        }
        indCoarseElement += 1;

        // Computation the distribution of cardinal cells (remove the old one and add the new one)
        int tmp_size = l.size();
        if (dict_DistributionOfCardinalOfCoarseElements.count(tmp_size)) {
            dict_DistributionOfCardinalOfCoarseElements[tmp_size] += 1;
        } else {
            dict_DistributionOfCardinalOfCoarseElements[tmp_size] = 1;
        }
    } else {
        // We do not create the coarse cell yet.
        // As this coarse cell will be soon deleted, we want its coarse index to be the greater possible.
        // Only isFineCellAgglomerated_tmp, numberOfFineAgglomeratedCells_tmp and
        // dict_DistributionOfCardinalOfCoarseElements are modified!

        // Update of _associatedCoarseCellNumber the output of the current function agglomerate
        for (auto iFineCell : l) {
            if (!isFineCellAgglomerated_tmp[iFineCell]) {
                isFineCellAgglomerated_tmp[iFineCell] = true;  // Rq: initialise a False pour chaque niveau dans agglomerate(...)
                numberOfFineAgglomeratedCells_tmp += 1;
            }
        }
        // Computation the distribution of cardinal cells (remove the old one and add the new one)
        int tmp_size = l.size();
        if (dict_DistributionOfCardinalOfCoarseElements.count(tmp_size) == 1) {
            dict_DistributionOfCardinalOfCoarseElements[tmp_size] += 1;
        } else {
            dict_DistributionOfCardinalOfCoarseElements[tmp_size] = 1;
        }


    }
}


void createADelayedCoarseCell(unordered_set<long> l,
                              unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                              unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                              long &indCoarseElement,
                              long *Fine_Cell_indices_To_Coarse_Cell_Indices) {

    dict_Coarse_Elem[indCoarseElement] = l;

    // Update of dict_Card_Coarse_Cells:
    int card = l.size();
    // TODO Change dict_Card_Coarse_Cells[card] from list to set!
    if (dict_Card_Coarse_Cells.count(card)) {
        dict_Card_Coarse_Cells[card].insert(indCoarseElement);
    } else {
        dict_Card_Coarse_Cells[card] = {indCoarseElement};
    }

    // Update of _associatedCoarseCellNumber the output of the current function agglomerate
    for (auto iFineCell : l) {
        // TODO not check this in production
        assert (Fine_Cell_indices_To_Coarse_Cell_Indices[iFineCell] ==
                -1);  // , "Cell " + str(iFineCell) + " is Already assigned fine cell"
        Fine_Cell_indices_To_Coarse_Cell_Indices[iFineCell] = indCoarseElement;
    }
    indCoarseElement += 1;
}


void remove_Too_Small_Cells_v2(int thresholdCard,
                               long *fineCellIndicesToCoarseCellIndices,
                               long &indCoarseCell,
                               long *matrixAdj_CRS_row_ptr,
                               long *matrixAdj_CRS_col_ind,
                               double *matrixAdj_CRS_values,
                               unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                               unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                               unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements) {

    unordered_set<long> set_removedCoarseCells;

    // Treatment of too small cells:
    // 1<= card(CoarseCell) <= thresholdCard
    for (int iSize = 1; iSize < thresholdCard + 1; iSize++) {
//        cout<<"iSize "<<iSize<<endl;
        if (dict_Card_Coarse_Cells.count(iSize)) {

            unordered_set<long> temporarySet;
            //        = dict_Card_Coarse_Cells[iSize].copy()
            for (long iCC: dict_Card_Coarse_Cells[iSize]) {
                temporarySet.insert(iCC);
            }

            // The list of coarse cell of card iSize is not empty:
            for (long iCoarseCell :temporarySet) {
//                cout<<"\tLoop iCoarseCell "<<iCoarseCell<<endl;

                // We process every fine cell separately
                int sizeOfOriginalCoarseCell = dict_Coarse_Elem[iCoarseCell].size();
                vector<long> temporaryList_CoarseElement(sizeOfOriginalCoarseCell);
                unordered_set<long> set_tmp;
                int iCount = 0;
                for (long iFC:dict_Coarse_Elem[iCoarseCell]) {
                    temporaryList_CoarseElement[iCount] = iFC;
                    set_tmp.insert(iFC);
                    iCount++;
                }

                unordered_set<long> unTreatedCells;
                int size_temporaryList_CoarseElement = sizeOfOriginalCoarseCell;
                for (int iIFineCell = 0; iIFineCell < size_temporaryList_CoarseElement; iIFineCell++) {
//                for (auto iFineCell : temporaryList_CoarseElement) {
                    long iFineCell = temporaryList_CoarseElement[iIFineCell];
//                    cout<<"\t\tLoop iFineCell "<<iFineCell<<endl;
                    // For every fine cell, iFineCell, inside iCoarseCell,
                    // we look for which coarse neighbour shares the most common faces with it.
                    unordered_map<long, int> dict_AdjacentCoarseCells;
                    int maxNumberCommonFaces = 0;
                    unordered_set<long> set_argmaxNumberCommonFaces;
                    long ind = matrixAdj_CRS_row_ptr[iFineCell];
                    long ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1];

                    // We process every neighbour of iFineCell
                    for (long i = ind; i < ind_p_one; i++) {

                        long ind_of_fine_neighbor = matrixAdj_CRS_col_ind[i];

                        // Union of two sets:
                        unordered_set<long> set_union_set_tmp_and_unTreatedCells(set_tmp);
                        for (auto iFC_tmp:unTreatedCells) {
                            set_union_set_tmp_and_unTreatedCells.insert(iFC_tmp);
                        }// to avoid to treat untreatedcells
                        if ((ind_of_fine_neighbor != iFineCell) &&
                            (set_union_set_tmp_and_unTreatedCells.count(ind_of_fine_neighbor) == 0)) {
                            // second term is to check that the neighbour does not belong to current incomplete
                            // coarse cell.
                            long ind_of_coarse_neighbor = fineCellIndicesToCoarseCellIndices[ind_of_fine_neighbor];

                            if (dict_Coarse_Elem.count(ind_of_coarse_neighbor) == 1) {
                                // if ind_of_coarse_neighbor is an isotropic coarse cell!
                                // we add the coarse cell containing the fine cell ind_of_coarse_neighbor in
                                // dict_AdjacentCoarseCells
                                if (dict_AdjacentCoarseCells.count(ind_of_coarse_neighbor) == 1) {
                                    dict_AdjacentCoarseCells[ind_of_coarse_neighbor] += 1;
                                } else {
                                    dict_AdjacentCoarseCells[ind_of_coarse_neighbor] = 1;
                                }
                                // On essaye de reperer le voisin qui a le plus grand nombre de face commune
                                // pour le rattacher a lui.
                                if (dict_AdjacentCoarseCells[ind_of_coarse_neighbor] > maxNumberCommonFaces) {

                                    maxNumberCommonFaces = dict_AdjacentCoarseCells[ind_of_coarse_neighbor];
                                    set_argmaxNumberCommonFaces.clear();
                                    set_argmaxNumberCommonFaces.insert(ind_of_coarse_neighbor);
                                } else if (dict_AdjacentCoarseCells[ind_of_coarse_neighbor] == maxNumberCommonFaces) {
                                    set_argmaxNumberCommonFaces.insert(ind_of_coarse_neighbor);
                                }
                            }
                        }
                    }
                    // Three cases:
                    ////////////////////////////
                    // 1) No neighbour:
                    if (set_argmaxNumberCommonFaces.size() < 1) {
//                        cout<<"\t\t\tCase 1 "<<endl;
                        // No neighbour found!
                        // We can be in this situation when a fine cell is included inside the rest of fine cells.
                        // Putting it to the end should solve the problem!
                        int counter = 0;
                        for (auto i_tmp: temporaryList_CoarseElement) {
                            if (i_tmp == iFineCell) {
                                counter++;
                            }
                        }
//                        // we want the last index of iFineCell in temporaryList_CoarseElement
//                        int ind_last_seen_iFineCell = -1;
//                        for (int i = 0; i < size_temporaryList_CoarseElement; i++) {
//                            if (temporaryList_CoarseElement[i] == iFineCell) {
//                                ind_last_seen_iFineCell = i;
//                            }
//                        }

                        // ind_last_seen_iFineCell should not be -1
                        // if ind_last_seen_iFineCell is the last index, we are in trouble-> untreated cell, otherwise another fc was added to temporaryList_CoarseElement
                        // may be it will solve the problem.
                        if (counter < sizeOfOriginalCoarseCell) {
//                        if ( (counter < 2)) {
                            temporaryList_CoarseElement.push_back(iFineCell);
                            size_temporaryList_CoarseElement++;
//                            cout<<"temporaryList_CoarseElement.size() "<<temporaryList_CoarseElement.size()<<endl;
                            continue;
                        } else {
                            // Problematic case!
                            // the current incompleted cell has no (isotropic) neighbour!
//                            cout<<"add of "<<iFineCell<< " to unTreatedCells "<<unTreatedCells.size()<<endl;
                            unTreatedCells.insert(iFineCell);
                        }
                    }
                        // 2) One neighbour:
                    else if (set_argmaxNumberCommonFaces.size() == 1) {
//                        cout<<"\t\t\tCase 2 "<<endl;
                        // pop du premier!
                        int argMax = *set_argmaxNumberCommonFaces.begin();
                        set_argmaxNumberCommonFaces.erase(argMax);

                        unordered_set<long> set_tmp_2 = swapFineCell(iFineCell, iCoarseCell,
                                                                     argMax,
                                                                     dict_Coarse_Elem,
                                                                     dict_Card_Coarse_Cells,
                                                                     dict_DistributionOfCardinalOfCoarseElements,
                                                                     fineCellIndicesToCoarseCellIndices);
                        //Update: set_removedCoarseCells.update(set_tmp_2);
                        for (auto i_set_tmp_2: set_tmp_2) {

                            set_removedCoarseCells.insert(i_set_tmp_2);
                        }

                    }
                        // 3) More than one neighbour:
                    else {
//                        cout<<"\t\t\tCase 3 "<<endl;

                        // More than one neighbour!

                        long argMin = numeric_limits<long>::max();
                        int sizeMin = numeric_limits<int>::max();

                        // TODO Is it the better choice????
                        // We choose the smallest coarse neighbour.
                        for (long iC : set_argmaxNumberCommonFaces) {

                            // reminder dict_Coarse_Elem contains only isotropic agglomeration
                            if (dict_Coarse_Elem[iC].size() < sizeMin) {
                                sizeMin = dict_Coarse_Elem[iC].size();
                                argMin = iC;
                            }
                        }
                        long argMax = argMin;
//                        cout<<"\t\t\t\tswapFineCell "<<iFineCell<< " from "<<iCoarseCell<<" to "<<argMax<<endl;
                        unordered_set<long> set_tmp_2 = swapFineCell(iFineCell, iCoarseCell,
                                                                     argMax,
                                                                     dict_Coarse_Elem,
                                                                     dict_Card_Coarse_Cells,
                                                                     dict_DistributionOfCardinalOfCoarseElements,
                                                                     fineCellIndicesToCoarseCellIndices);

                        //Update: set_removedCoarseCells.update(set_tmp_2);
                        for (auto i_set_tmp_2: set_tmp_2) {
                            set_removedCoarseCells.insert(i_set_tmp_2);
                        }
                    }
//                    cout<<"set_tmp.erase(iFineCell); "<<iFineCell<<endl;
                    set_tmp.erase(iFineCell);
                }

                if (!unTreatedCells.empty()) {
//                    cout<< " unTreatedCells= [";
//                    for (auto iU: unTreatedCells){
//                        cout<<iU<<" ,";
//                    }
//                    cout<<"]"<<endl;

//                    vector<long> l(unTreatedCells.size());
//                    int i_U_Count = 0;
//                    for (auto i_Untreated : unTreatedCells) {
//                        l[i_U_Count] = i_Untreated;
//                        i_U_Count++;
//                    }

                    // Maybe unTreatedCells are not connected?
                    bool isConnex = checkConnectivity_w_set(unTreatedCells, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind);
//                    cout<<"isConnex "<<isConnex<<endl;
                    // if not isConnex:
                    //     // len(l)>1
                    //     dict_IsConnected = dict()
                    //     iCell = l[0]
                    //     dict_IsConnected[iCell] = True
                    //     setNext = {}
                    //     while len(dict_IsConnected) < len(l):
                    //         ind = matrixAdj_CRS_row_ptr[iCell]
                    //         ind_p_one = matrixAdj_CRS_row_ptr[iCell + 1]
                    //         for( i in xrange(ind,
                    //                         ind_p_one):  // We process every neighbour of iFineCell
                    //             ind_of_fine_neighbor = matrixAdj_CRS_col_ind[i]
                    //             if ind_of_fine_neighbor != iCell and ind_of_fine_neighbor not in dict_IsConnected and ind_of_fine_neighbor in l:
                    //                 // We find a neighbour inside l!
                    //                 setNext.add(ind_of_fine_neighbor)
                    //                 dict_IsConnected[ind_of_fine_neighbor] = True
                    //         if setNext:
                    //             iCell = setNext.pop()
                    //         else:
                    //             break
                    //     if len(dict_IsConnected) == len(l):
                    //         isConnex = True

                    if (isConnex) {

                        if (unTreatedCells.size() == 1) {

                            long iFineCell = *unTreatedCells.begin();//l[0];
                            long ind = matrixAdj_CRS_row_ptr[iFineCell];
                            long ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1];
                            long argFineMax = -1;
                            long maxArea = 0.0;

                            // We process every neighbour of iFineCell
                            for (long i = ind; i < ind_p_one; i++) {
                                long ind_of_fine_neighbor = matrixAdj_CRS_col_ind[i];

                                if (ind_of_fine_neighbor != iFineCell) {
                                    if (matrixAdj_CRS_values[i] > maxArea) {
                                        maxArea = matrixAdj_CRS_values[i];
                                        argFineMax = ind_of_fine_neighbor;
                                    }
                                }
                            }

                            if (argFineMax != -1) {
                                long iAnisoCC = fineCellIndicesToCoarseCellIndices[argFineMax];

                                // Update of dict_Card_Coarse_Cells:
                                int size = 1;

                                // 1) We remove the cell from iOrigineCoarseCell
                                assert(dict_Card_Coarse_Cells.count(size) == 1);
                                assert(dict_DistributionOfCardinalOfCoarseElements.count(size) == 1);
                                assert(dict_Card_Coarse_Cells[size].count(iCoarseCell) == 1);
                                assert(dict_Coarse_Elem.count(iCoarseCell) == 1);


                                dict_Card_Coarse_Cells[size].erase(iCoarseCell);
                                if (dict_Card_Coarse_Cells[size].empty()) {
                                    dict_Card_Coarse_Cells.erase(size);
//                                   del dict_Card_Coarse_Cells[size]
                                }
                                dict_DistributionOfCardinalOfCoarseElements[size] -= 1;
                                if (dict_DistributionOfCardinalOfCoarseElements[size] == 0) {
                                    dict_DistributionOfCardinalOfCoarseElements.erase(
                                            size);  // del dict_DistributionOfCardinalOfCoarseElements[size]
                                }
                                dict_Coarse_Elem.erase(iCoarseCell);  // del dict_Coarse_Elem[iCoarseCell]
                                set_removedCoarseCells.insert(iCoarseCell);

                                fineCellIndicesToCoarseCellIndices[iFineCell] = iAnisoCC;

                            }
                            // print "Treatment:", iFineCell, "is in anisotropic CC:", iAnisoCC
                        }
//                        else {
//                            // Nothing to do the coarse cell already exists
//                            // print "UntreatedCells are connected Pfff!", l,"and original cell", set(temporaryList_CoarseElement)
//                            pass
//                        }
                    } else {
                        cout << "Problematic non connected untreated cells" << endl;
                        cout << "l= [";
                        for (auto i :unTreatedCells) {
                            cout << i << ", ";
                        }
                        cout << "]" << endl;
                        cout << "and original cell= [";
                        for (auto i :temporaryList_CoarseElement) {
                            cout << i << ", ";
                        }
                        cout << "]" << endl;
                        throw logic_error("Problematic case! Untreated cells are not connected!");
                    }
                }
            }
        }
    }
//    cout<<"set_removedCoarseCells.empty() "<<set_removedCoarseCells.empty()<<endl;
    if (!set_removedCoarseCells.empty()) {
        removeDeletedCoarseCells_v3(
                dict_Coarse_Elem,
                dict_Card_Coarse_Cells,
                fineCellIndicesToCoarseCellIndices,
                set_removedCoarseCells,
                indCoarseCell);
    }

}


void removeDeletedCoarseCells_v3(unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                 unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                 long *fine_Cell_indices_To_Coarse_Cell_Indices,
                                 unordered_set<long> set_removedCoarseCells,
                                 long &numberOfCoarseCells) {
//    cout<<"\nCall of removeDeletedCoarseCells_v3"<<endl;
    if (!set_removedCoarseCells.empty()) {

        long newCC;
        if (!dict_Coarse_Elem.empty()) {
            long minCoarseCell = numeric_limits<long>::max();
            for (auto iFC : set_removedCoarseCells) {
                if (iFC < minCoarseCell) {
                    minCoarseCell = iFC;
                }
            }
//            cout<<"minCoarseCell "<<minCoarseCell<<endl;

            unordered_map<long, long> dict_Old_CC_To_newCC;
            newCC = minCoarseCell;
            for (long iCoarseCell = minCoarseCell + 1; iCoarseCell < numberOfCoarseCells; iCoarseCell++) {
                if (dict_Coarse_Elem.count(iCoarseCell) == 1) {

                    dict_Old_CC_To_newCC[iCoarseCell] = newCC;
                    dict_Coarse_Elem[newCC] = dict_Coarse_Elem[iCoarseCell];
                    for (auto iFineCell:dict_Coarse_Elem[newCC]) {
                        fine_Cell_indices_To_Coarse_Cell_Indices[iFineCell] = newCC;
                    }
                    int iCSize = dict_Coarse_Elem[iCoarseCell].size();
                    dict_Card_Coarse_Cells[iCSize].erase(iCoarseCell);
                    dict_Card_Coarse_Cells[iCSize].insert(dict_Old_CC_To_newCC[iCoarseCell]);
                    dict_Coarse_Elem.erase(iCoarseCell);
                    newCC += 1;
                }
            }
        } else {
            newCC = numberOfCoarseCells - set_removedCoarseCells.size();
        }

        numberOfCoarseCells -= set_removedCoarseCells.size();
//        cout<<"numberOfCoarseCells "<<numberOfCoarseCells<< " newCC "<<newCC<<endl;
        assert(numberOfCoarseCells == newCC); //, "Problem consistancy in number of Coarse cells " + str(numberOfCoarseCells) +" " +         str(newCC)
//        return dict_Coarse_Elem, dict_Card_Coarse_Cells, numberOfCoarseCells
    }
}

//unordered_map<long, list<unordered_set<long>>>
unordered_set<long> choice_Of_Agglomerated_Cells(long seed,
                                                 vector<queue<long>> &listOfSeeds,
                                                 unordered_map<long, int> &dict_Neighbours_Of_Seed,
                                                 long *matrixAdj_CRS_row_ptr,
                                                 long *matrixAdj_CRS_col_ind,
                                                 double *matrixAdj_CRS_values,
                                                 double *volumes,
                                                 int goalCard,
                                                 int maxCard,
                                                 bool *isFineCellAgglomerated_tmp,
                                                 int *isOnFineBnd,
                                                 long &numberOfFineAgglomeratedCells_tmp,
                                                 bool isOrderPrimary) {
//"""
//The goal of this function is to chose from a pool of neighbour the better one to build a compact coarse cell
//:param isOrderPrimary: modify the order of priority for agglomeration. False is the default value and the number of
//face in common is primary. True, the order of neighbourhood is primary
//"""

    // Number of fine cells constituting the current coarse cell in construction.
    int size_Current_Coarse_Cell = 1;  // contains the seed
    unordered_set<long> set_of_fine_cells_for_Current_Coarse_Cell({seed});

    int nbFCellsToAdd = goalCard - size_Current_Coarse_Cell;
    // print "len(dict_Neighbours_Of_Seed)", len(dict_Neighbours_Of_Seed), nbFCellsToAdd
    // assert len(dict_Neighbours_Of_Seed) >= nbFCellsToAdd

    // On dit que les voisins sont OK
    // (pas de test pour l'instant sur leur pertinence: wall Far field, aspect Ratio...)
    // On les declare visite/agglomere
    // Rq: Mavriplis ne fait pas d'optimisation de choix dans les cellules fines!
    // C'est fait seulement dans le choix de la seed
    //

    // TODO Check that the Coarse element is connex!
    // Tant que la cellule agglomeree courante n'est pas complete
    // On a trop de voisin pour pas assez de place dans l'element grossier en construction.
    // while size_Current_Coarse_Cell + len(listOfNeighborsForAgglomeration) < self.__maximumSizeOfAgglomeratedElement:

    int minSize = goalCard;

    // Computation of the initial aspect ratio: we need surf and volume
    double coarseElement_surf = 0.0;
    long ind = matrixAdj_CRS_row_ptr[seed];
    long ind_p_one = matrixAdj_CRS_row_ptr[seed + 1];
    for (long i = ind; i < ind_p_one; i++) {
        coarseElement_surf += matrixAdj_CRS_values[i];
    }
    double coarseElement_vol = volumes[seed];

    isFineCellAgglomerated_tmp[seed] = true;
    unordered_set<long> tmp_set;  //ex tmp_list
    unordered_map<long, pair<unordered_set<long>, unordered_map<long, int>>> dict_Coarse_Cells_in_Creation;
    // Choice of the fine cells to agglomerate
    // while size_Current_Coarse_Cell < self.__goalCard:
    double minExternalFaces = numeric_limits<double>::max();
    int argMinExternalFaces = minSize;

    int maxInd = min(maxCard, int(dict_Neighbours_Of_Seed.size()) + 1);
    int numberOfExternalFacesCurrentCoarseCell =
            matrixAdj_CRS_row_ptr[seed + 1] - matrixAdj_CRS_row_ptr[seed] + isOnFineBnd[seed] - 1;
//    cout << "numberOfExternalFacesCurrentCoarseCell " << numberOfExternalFacesCurrentCoarseCell << endl;
    while (size_Current_Coarse_Cell < maxInd) {

        double minAR = numeric_limits<double>::max();
        double minAR_surf = numeric_limits<double>::max();
        double minAR_vol = numeric_limits<double>::max();
        long argminAR = -1;

        int maxFacesInCommon = 0;
        long argMaxFacesInCommon = -1;

        // For every fine cell in the neighbourhood:
        for (auto iKeyValue: dict_Neighbours_Of_Seed) {      // we test every possible new cells to chose the one that localy
            // minimizes the Aspect Ratio.
            long iFinerCell = iKeyValue.first;
            int order = iKeyValue.second;


            if (argMaxFacesInCommon == -1) {
                argMaxFacesInCommon = iFinerCell;
            }

            bool isFinerCellAdjacentToAnyCellOfTheCoarseElement = false;
            double new_AR_surf = coarseElement_surf;
            double new_AR_vol = coarseElement_vol + volumes[iFinerCell];

            int numberFacesInCommon = 0;

            // Computation of the new aspect ratio of the tested coarse element
            ind = matrixAdj_CRS_row_ptr[iFinerCell];
            ind_p_one = matrixAdj_CRS_row_ptr[iFinerCell + 1];
            for (long i = ind; i < ind_p_one; i++)  // loop on neighbours
            {
                long indCell = matrixAdj_CRS_col_ind[i];
                if (indCell == iFinerCell) {                        // Boundary surface
                    new_AR_surf += matrixAdj_CRS_values[i];
                } else if (set_of_fine_cells_for_Current_Coarse_Cell.count(indCell) == 0) {
                    new_AR_surf += matrixAdj_CRS_values[i];
                } else {
                    isFinerCellAdjacentToAnyCellOfTheCoarseElement = true;
                    new_AR_surf -= matrixAdj_CRS_values[i];
                    numberFacesInCommon += 1;
                }
            }
            double new_AR = pow(new_AR_surf, 1.5) / new_AR_vol;

//            order = dict_Neighbours_Of_Seed[iFinerCell];

            // TODO This version seems good but refactorisation to do: perhaps it is not needed to compute every new possible coarse cell aspect ratio?
            // TODO also need to remove the list of minAR, argminAR, etc.
            if (numberFacesInCommon >= maxFacesInCommon or isOrderPrimary) {  // if isOrderPrimary is True the order of
                // neighbourhood is primary
                if (numberFacesInCommon == maxFacesInCommon or isOrderPrimary) {
                    if (order <= dict_Neighbours_Of_Seed[argMaxFacesInCommon]) {
                        if (order == dict_Neighbours_Of_Seed[argMaxFacesInCommon]) {
                            if ((new_AR < minAR) && isFinerCellAdjacentToAnyCellOfTheCoarseElement) {
                                // The second condition asserts the connectivity of the coarse element.
                                minAR = new_AR;
                                argminAR = iFinerCell;
                                minAR_surf = new_AR_surf;
                                minAR_vol = new_AR_vol;

                                argMaxFacesInCommon = iFinerCell;
                                // The number of face in common is the same no need to touch it
                            }
                        } else {
                            // Case :numberFacesInCommon == maxFacesInCommon and order < dict_Neighbours_Of_Seed[argMaxFacesInCommon]:
                            argMaxFacesInCommon = iFinerCell;
                            minAR = new_AR;
                            argminAR = iFinerCell;
                            minAR_surf = new_AR_surf;
                            minAR_vol = new_AR_vol;
                            // The number of face in common is the same no need to touch it
                        }
                    }
                } else {
                    // Case :numberFacesInCommon > maxFacesInCommon:
                    maxFacesInCommon = numberFacesInCommon;
                    argMaxFacesInCommon = iFinerCell;
                    minAR = new_AR;
                    argminAR = iFinerCell;
                    minAR_surf = new_AR_surf;
                    minAR_vol = new_AR_vol;
                }
            }
        }
//        print "argminAR", argminAR
        numberOfExternalFacesCurrentCoarseCell +=
                matrixAdj_CRS_row_ptr[argminAR + 1] - matrixAdj_CRS_row_ptr[argminAR] + isOnFineBnd[argminAR] - 1 -
                2 * maxFacesInCommon;
        size_Current_Coarse_Cell += 1;
        set_of_fine_cells_for_Current_Coarse_Cell.insert(argminAR);

        if (((minSize <= size_Current_Coarse_Cell) && (size_Current_Coarse_Cell <= maxInd)) ||
            size_Current_Coarse_Cell == maxInd) {
            if (numberOfExternalFacesCurrentCoarseCell <= minExternalFaces) {
                minExternalFaces = numberOfExternalFacesCurrentCoarseCell;
                argMinExternalFaces = size_Current_Coarse_Cell;
            }
            unordered_map<long, int> new_dict;
            new_dict[argminAR] = dict_Neighbours_Of_Seed[argminAR];

            pair<unordered_set<long>, unordered_map<long, int>> p = make_pair(
                    set_of_fine_cells_for_Current_Coarse_Cell, new_dict);
            dict_Coarse_Cells_in_Creation[size_Current_Coarse_Cell] = p;

        }
        coarseElement_surf = minAR_surf;
        coarseElement_vol = minAR_vol;
        dict_Neighbours_Of_Seed.erase(argminAR);
        isFineCellAgglomerated_tmp[argminAR] = true;
        tmp_set.insert(argminAR);
    }
//    cout << "end Loop while" << endl;
    // assert size_Current_Coarse_Cell == self.__goalCard, \
    //     "Pb: wrong number of fine cells in Current Coarse Cell" + str(set_of_fine_cells_for_Current_Coarse_Cell)
    // print "dict_Coarse_Cells_in_Creation",dict_Coarse_Cells_in_Creation
    set_of_fine_cells_for_Current_Coarse_Cell = dict_Coarse_Cells_in_Creation[argMinExternalFaces].first;
//    cout << "before Loop iS" << endl;
    for (long iS = argMinExternalFaces + 1; iS < maxInd + 1; iS++) {

        // Merge/update:
        for (auto iKV:dict_Coarse_Cells_in_Creation[iS].second) {
            dict_Neighbours_Of_Seed[iKV.first] = iKV.second;
        }
//        dict_Neighbours_Of_Seed.update(dict_Coarse_Cells_in_Creation[iS].second);
        long removedFCell = (*dict_Coarse_Cells_in_Creation[iS].second.begin()).first;  // keys()[0];
        isFineCellAgglomerated_tmp[removedFCell] = false;
        tmp_set.erase(removedFCell);
    }
//    cout << "after Loop iS" << endl;
    // update of nb_of_Agglomerated_cells and self._isFineCellAgglomerated_tmp
    numberOfFineAgglomeratedCells_tmp += argMinExternalFaces;

    // Update of listOfSeeds: a) with dict_Neighbours_Of_Seed or b) by computing the neighbourhood of the current
    // coarse cell.
    if (!dict_Neighbours_Of_Seed.empty()) {

//        cout << "!dict_Neighbours_Of_Seed.empty()" << endl;
        // if( dict_Neighbours_Of_Seed is not empty
        // Reminder: dict_Neighbours_Of_Seed is here the pool of cell neighbouring the previous seed!
        unordered_set<long> setOfNewSeed;
        for (auto iKV : dict_Neighbours_Of_Seed) {
            if (iKV.second <= 2) {
                setOfNewSeed.insert(iKV.first);
            }
        }
//        cout << "\nSetOfNewSeed:" << endl;
//        cout << "[";
//        for (auto i:setOfNewSeed) {
//            cout << i << ", ";
//        }
//        cout << "]" << endl;
//            setOfNewSeed = [k         for k in dict_Neighbours_Of_Seed if dict_Neighbours_Of_Seed[k] <= 2]
        int iK = 3;
        while (setOfNewSeed.empty()) {

//                setOfNewSeed = [k for          k in dict_Neighbours_Of_Seed if dict_Neighbours_Of_Seed[k] <= iK]
            for (auto iKV : dict_Neighbours_Of_Seed) {
                if (iKV.second <= iK) {
                    setOfNewSeed.insert(iKV.first);
                }
            }
            iK += 1;
        }
        //print "setOfNewSeed", setOfNewSeed
        for (auto iNewSeed: setOfNewSeed) {
            // TODO make self._isOnBnd private!
            // the value of isOnBnd[iLevel-1][iNewSeed] may be strictly bigger than 3, in case of partitionning
            // via Metis or Scotch.
            int valueIsOnBnd = isOnFineBnd[iNewSeed];
            if (valueIsOnBnd >= 3) {
                valueIsOnBnd = 3;
                isOnFineBnd[iNewSeed] = 3;
            }
            listOfSeeds[valueIsOnBnd].push(iNewSeed);
        }
//        cout << "END of !dict_Neighbours_Of_Seed.empty()" << endl;
    } else {
//        cout << "dict_Neighbours_Of_Seed.empty()" << endl;
        // else dict_Neighbours_Of_Seed is empty: on a utilise tous les voisins!
        // On en cherche d'autres!
        bool isEmpty = true;
        for (int i = 3; i > -1; i--) {

            //vector<queue<long>> listOfSeeds
            if (!listOfSeeds.empty()) {
                if (!listOfSeeds[i].empty()) {
                    isEmpty = false;
                }
            }
        }
        if (isEmpty) {


            // if( listOfSeeds is empty
            // we look if there is some neighbour to the current fine cells:
            unordered_set<long> set_Neighbours_Of_Seed;
            for (long iFineCell : tmp_set) {
                ind = matrixAdj_CRS_row_ptr[iFineCell];
                ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1];
                for (long i = ind; i < ind_p_one; i++) {

                    long indCell = matrixAdj_CRS_col_ind[i];
                    if ((indCell != iFineCell) && (!isFineCellAgglomerated_tmp[indCell])) {
                        set_Neighbours_Of_Seed.insert(indCell);
                    }
                }
            }
            // listOfSeeds.extend(set_Neighbours_Of_Seed)
            for (long iNewSeed : set_Neighbours_Of_Seed) {
                // TODO make self._isOnBnd private!
                listOfSeeds[isOnFineBnd[iNewSeed]].push(iNewSeed);
            }

        }
    }
    return set_of_fine_cells_for_Current_Coarse_Cell;
}

void agglomerate_Isotropic_Choice_Of_Agglomerated_Cells(long seed,
                                                        vector<queue<long>> &listOfSeeds,
                                                        unordered_map<long, int> &dict_Neighbours_Of_Seed,
                                                        long *matrixAdj_CRS_row_ptr,
                                                        long *matrixAdj_CRS_col_ind,
                                                        double *matrixAdj_CRS_values,
                                                        double *volumes,
                                                        unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                                        unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                        unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                                        long &indCoarseCell,
                                                        long &numberOfFineAgglomeratedCells,
                                                        bool *isFineCellAgglomerated,
                                                        long *fineCellToCoarseCell,
                                                        list<unordered_set<long>> &delayedCoarseCells,
                                                        int *isOnFineBnd,
                                                        int goalCard,
                                                        int thresholdCard,
                                                        int maxCard) {
    //"""
    //The goal of this function is to chose from a pool of neighbour the better one to build a compact coarse cell
    //        :param iLevel: current level of coarse grid
    //        :param seed:
    //:param listOfSeeds:
    //:param dict_Neighbours_Of_Seed:
    //:param maxOrderOfNeighbourhood:
    //:param matrixAdj_CRS_row_ptr:
    //:param matrixAdj_CRS_col_ind:
    //:param matrixAdj_CRS_values:
    //:param volumes:
    //:param dict_Coarse_Elem:
    //:param dict_Card_Coarse_Cells:
    //:param dict_DistributionOfCardinalOfCoarseElements:
    //:return:
    //"""

    unordered_set<long> list_of_fine_cells_for_Current_Coarse_Cell({seed});  // TODO Transformer en set?

    // If no neighbour is found for seed: this case happened only when isotropic cell is surrounded
    // by anisotropic cells.
    if (dict_Neighbours_Of_Seed.empty()) {

        // dict_Neighbours_Of_Seed is empty, i.e: l'element agglo n'est pas complet et il n'y a plus de voisins disponibles!
        agglomerate_Isotropic_createCoarseCell(list_of_fine_cells_for_Current_Coarse_Cell,
                                               dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                               dict_DistributionOfCardinalOfCoarseElements,
                                               indCoarseCell,
                                               numberOfFineAgglomeratedCells,
                                               isFineCellAgglomerated,
                                               fineCellToCoarseCell,
                                               delayedCoarseCells,
                                               true,
                                               list_of_fine_cells_for_Current_Coarse_Cell.size() <= thresholdCard);
    } else if (dict_Neighbours_Of_Seed.size() + 1 < goalCard) {
        // Not enough available neighbour: creation of a (too small) coarse cell.
        for (auto iKV : dict_Neighbours_Of_Seed) {
            list_of_fine_cells_for_Current_Coarse_Cell.insert(iKV.first);
//            list_of_fine_cells_for_Current_Coarse_Cell.extend(list(dict_Neighbours_Of_Seed.keys()))
        }


        agglomerate_Isotropic_createCoarseCell(list_of_fine_cells_for_Current_Coarse_Cell,
                                               dict_Coarse_Elem,
                                               dict_Card_Coarse_Cells,
                                               dict_DistributionOfCardinalOfCoarseElements,
                                               indCoarseCell,
                                               numberOfFineAgglomeratedCells,
                                               isFineCellAgglomerated,
                                               fineCellToCoarseCell,
                                               delayedCoarseCells,
                                               true,
                                               list_of_fine_cells_for_Current_Coarse_Cell.size() <= thresholdCard);

    } else {
        list_of_fine_cells_for_Current_Coarse_Cell = choice_Of_Agglomerated_Cells(seed,
                                                                                  listOfSeeds,
                                                                                  dict_Neighbours_Of_Seed,
                                                                                  matrixAdj_CRS_row_ptr,
                                                                                  matrixAdj_CRS_col_ind,
                                                                                  matrixAdj_CRS_values,
                                                                                  volumes,
                                                                                  goalCard,  // could probably be __minCard but it breaks structured agglomeration
                                                                                  maxCard,
                                                                                  isFineCellAgglomerated,
                                                                                  isOnFineBnd,
                                                                                  numberOfFineAgglomeratedCells);

        agglomerate_Isotropic_createCoarseCell(list_of_fine_cells_for_Current_Coarse_Cell, dict_Coarse_Elem,
                                               dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                                               indCoarseCell,
                                               numberOfFineAgglomeratedCells,
                                               isFineCellAgglomerated,
                                               fineCellToCoarseCell,
                                               delayedCoarseCells);

    }
}


void agglomerate_Isotropic_createCoarseCell(unordered_set<long> l,
                                            unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                            unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                            unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                            long &indCoarseCell,
                                            long &numberOfFineAgglomeratedCells,
                                            bool *isFineCellAgglomerated,
                                            long *fineCellToCoarseCell,
                                            list<unordered_set<long>> &delayedCoarseCells,
                                            bool isMutable,
                                            bool isCreationDelayed) {
    if (!isCreationDelayed) {
        createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                         dict_DistributionOfCardinalOfCoarseElements, indCoarseCell,
                         numberOfFineAgglomeratedCells, isFineCellAgglomerated,
                         fineCellToCoarseCell, isMutable);
    } else {

        // If isCreationDelayed == True
        createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                         dict_DistributionOfCardinalOfCoarseElements, indCoarseCell,
                         numberOfFineAgglomeratedCells, isFineCellAgglomerated,
                         fineCellToCoarseCell,
                         isMutable,
                         isCreationDelayed);

        delayedCoarseCells.push_back(l);
    }
}


long agglomerate_Isotropic_Choice_Of_Seed(vector<queue<long>> &listOfSeeds,
                                          long numberOfFineCells,
                                          const bool *isFineCellAgglomerated,
                                          unordered_set<long> isOnRidge,
                                          unordered_set<long> isOnValley) {
    //
    //Chose a correct seed in the fine cell pool of not agglomerated cells.
    //:param listOfSeeds: list of 4 deques. Deques are eventually reduced but no add.
    //:param numberOfFineCells: Usefull if the listOfSeeds contains no correct seed (not already agglomerated!)
    //:return: a correct seed (not already agglomerated!)
    //"""
    // We chose preferably the corners, then the ridges, then the valey, and finaly interior cells:
    // see NIA (Mavriplis uses Wall and farfield only)

    long seed = -1;
    for (int iL = 3; iL > -1; iL--) {
        if (!listOfSeeds[iL].empty()) {
            seed = listOfSeeds[iL].front();
            listOfSeeds[iL].pop();
            while (isFineCellAgglomerated[seed]) {
                if (!listOfSeeds[iL].empty()) {
                    seed = listOfSeeds[iL].front();
                    listOfSeeds[iL].pop();
                } else {
                    break;
                }
            }
            if (isFineCellAgglomerated[seed]) {
                continue; // no correct new seed, so we try iL--
            } else {
                break;  // a new seed no need to try iL--;
            }
        }
    }
    // if no seed were found in listOfSeeds... we look in ridges or valley
    if (seed == -1 or isFineCellAgglomerated[seed]) {
        for (int iL = 2; iL > -1; iL--) {
            // we check in _isOnRidge/_isOnValley if anything is available?
            if (iL == 2) {
                if (!isOnRidge.empty()) {
                    seed = *isOnRidge.begin();
                    isOnRidge.erase(seed);
                    while (isFineCellAgglomerated[seed]) {
                        if (!isOnRidge.empty()) {
                            seed = *isOnRidge.begin();
                            isOnRidge.erase(seed);
//                            seed = isOnRidge.pop()
                        } else {
                            break;
                        }
                    }
                    break;
                }
            } else if (iL == 1) {
                if (!isOnValley.empty()) {
                    seed = *isOnValley.begin();
                    isOnValley.erase(seed);
                    while (isFineCellAgglomerated[seed]) {
                        if (!isOnValley.empty()) {
                            seed = *isOnValley.begin();
                            isOnValley.erase(seed);
                        } else {
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    // if no seed were found in listOfSeeds nor in in ridges or valley we take the first one!
    if ((seed == -1) || (isFineCellAgglomerated[seed])) {
        // We do not have a correct seed:
        for (int i = 0; i < numberOfFineCells; i++) {
            if (!isFineCellAgglomerated[i]) {
                seed = i;
                break;
            }
        }
    }
    return seed;

}


void makeSmallCellBigger(unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                         unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                         long *matrixAdj_CRS_row_ptr,
                         long *matrixAdj_CRS_col_ind,
                         unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                         long &indCoarseCell,
                         long &numberOfFineAgglomeratedCells,
                         bool *isFineCellAgglomerated,
                         long *fineCellToCoarseCell,
                         int minCard,
                         int goalCard,
                         int thresholdCard,
                         bool verbose) {
    // TODO rename dict_Coarse_Elem dictCoarseCells
    if (verbose) {
        cout << "Call of __makeSmallCellBigger" << endl;
    }

    unordered_set<long> set_removedCoarseCells;

    for (int iSize = thresholdCard + 1; iSize < minCard; iSize++) {

        if (dict_Card_Coarse_Cells.count(iSize) == 1) {


            unordered_set<long> copyOfdict_Card_Coarse_Cells_iSize = dict_Card_Coarse_Cells[iSize];
            // For every coarse cell of size iSize
            for (auto iCoarseCell: copyOfdict_Card_Coarse_Cells_iSize) {
//                cout<<"\n\tiCoarseCell "<<iCoarseCell<<endl;
                // Special case: it can happened that a coarse cell is whole "eaten" in the process it is
                // added to set_removedCoarseCells.
                if (dict_Coarse_Elem.count(iCoarseCell) == 0) {
                    continue;
                }
                // print "\nInitial Coarse cell ", iCoarseCell, dict_Coarse_Elem[iCoarseCell]

                // search of the fine cell at the "root" of the coarse cell, i.e. the fine cell with the most
                // faces in common with its coarse cell.
                int maxNumberCommonFaces = -1;
                long argMaxNumberCommonFaces = -1;
                for (long iFineCell: dict_Coarse_Elem[iCoarseCell]) {
                    int nbCommonFaces_iFineCell = computeNumberOfCommonFaces(iFineCell, iCoarseCell,
                                                                             matrixAdj_CRS_row_ptr,
                                                                             matrixAdj_CRS_col_ind,
                                                                             fineCellToCoarseCell);
                    if (nbCommonFaces_iFineCell > maxNumberCommonFaces) {
                        maxNumberCommonFaces = nbCommonFaces_iFineCell;
                        argMaxNumberCommonFaces = iFineCell;

                    }
                }

                long seed = argMaxNumberCommonFaces;
                // From this cell (seed) we try to add extra fine cells!

                // Building of the neighbourhood:
                // TODO Replace this with self.__agglomerate_Isotropic_Computation_Of_Neighbourhood() if possible???
                int numberOfOrderOfNeighbourhood = 3;
                unordered_map<long, int> dict_Neighbours_Of_Seed;  // set (with unicity) des indices des cellules du 1er voisinage de seed
                unordered_map<long, int> dict_Neighbours_Of_Order_O_M_One;
                dict_Neighbours_Of_Order_O_M_One[seed] = 0;

                int iOrder = 1;

                // for iOrder in xrange(1, numberOfOrderOfNeighbourhood+1):
                while ((iOrder < numberOfOrderOfNeighbourhood + 1) ||
                       (dict_Neighbours_Of_Seed.size() + dict_Neighbours_Of_Order_O_M_One.size() < goalCard)) {

                    unordered_map<long, int> dict_Neighbours_Of_Order_O;

                    // dict_Neighbours_Of_Seed.update(dict_Neighbours_Of_Order_O_M_One)
                    for (auto iKV_O_M_One:dict_Neighbours_Of_Order_O_M_One) {
                        dict_Neighbours_Of_Seed[iKV_O_M_One.first] = iKV_O_M_One.second;
                    }
                    for (auto seed_tmp :dict_Neighbours_Of_Order_O_M_One) {

                        long ind = matrixAdj_CRS_row_ptr[seed_tmp.first];            // Usefull to find neighbours of seed
                        long ind_p_one = matrixAdj_CRS_row_ptr[seed_tmp.first +
                                                               1];  // Usefull to find neighbours of seed
                        for (long i = ind; i < ind_p_one; i++) {
                            long indFCellNeighbour = matrixAdj_CRS_col_ind[i];
                            if ((indFCellNeighbour != seed_tmp.first) &&
                                (dict_Coarse_Elem.count(fineCellToCoarseCell[indFCellNeighbour]) == 1)) {
                                // The second part is to avoid the work with anisotropic cells
                                if (dict_Neighbours_Of_Seed.count(indFCellNeighbour) ==
                                    0) {  // We take all cells even if they are already agglomerated to another coarse neighbour!
                                    if (dict_Neighbours_Of_Order_O.count(indFCellNeighbour) == 0) {
                                        dict_Neighbours_Of_Order_O[indFCellNeighbour] = iOrder;
                                    }
                                }
                            }
                        }
                    }
                    // Exit condition
                    if (dict_Neighbours_Of_Order_O.empty()) {
                        // No more neighbours available:
                        break;
                    }

                    dict_Neighbours_Of_Order_O_M_One = dict_Neighbours_Of_Order_O;  //copy
                    iOrder += 1;
                }

                // Update of dict_Neighbours_Of_Seed
                // dict_Neighbours_Of_Seed.update(dict_Neighbours_Of_Order_O_M_One)
                for (auto iKV : dict_Neighbours_Of_Order_O_M_One) {
                    dict_Neighbours_Of_Seed[iKV.first] = iKV.second;
                }
                // print "dict_Neighbours_Of_Seed 1 ", dict_Neighbours_Of_Seed
                int maxOrderOfNeighbourhood = iOrder;

                // We remove all fine cell already contained in the current coarse element
                for (long iFC :dict_Coarse_Elem[iCoarseCell]) {
                    if (dict_Neighbours_Of_Seed.count(iFC) == 1) {
                        dict_Neighbours_Of_Seed.erase(iFC);
                    }
                }
                // print iCoarseCell, "size", len(dict_Coarse_Elem[iCoarseCell]), "add", len(dict_Neighbours_Of_Seed), "dict=", dict_Neighbours_Of_Seed
                // print "dict_Neighbours_Of_Seed 2 ", dict_Neighbours_Of_Seed
                // On ajoute des cellules fines a notre cellule grossiere courante
                //////////////////////////////////////////////////////////////////

                // Number of fine cells constituting the current coarse cell in construction.
//                unordered_set<long> list_of_fine_cells_for_Current_Coarse_Cell = dict_Coarse_Elem[iCoarseCell];
                int size_Current_Coarse_Cell = dict_Coarse_Elem[iCoarseCell].size();

                // If no neighbour is found for seed: this case happened only when isotropic cell is surrounded
                // by anisotropic cells.
                if (dict_Neighbours_Of_Seed.empty()) {
                    // print "\nInitial Coarse cell ", iCoarseCell, dict_Coarse_Elem[iCoarseCell]
                    // print "Trouble dict_Neighbours_Of_Seed is empty for seed " + str(seed)
                    // raise ValueError("Trouble dict_Neighbours_Of_Seed is empty for seed " + str(seed))
                    continue;
                }

                int nbFCellsToAdd = goalCard - size_Current_Coarse_Cell;
                assert(dict_Neighbours_Of_Seed.size() >= nbFCellsToAdd);
                unordered_set<long> setOfModifiedCoarseCells = {iCoarseCell};

                // Choice of the fine cells to agglomerate
                while (size_Current_Coarse_Cell < goalCard) {
//                    cout<<"\n => Size of current coarse cell "<<size_Current_Coarse_Cell<<endl;
//                    cout<<"[";
//                    for (auto iC: dict_Coarse_Elem[iCoarseCell]){
//                        cout<<iC<<", ";
//                    }
//                    cout<<"]"<<endl;

                    int maxFacesInCommon = 0;
                    long argMaxFacesInCommon = -1;
                    bool isDefaultValue_argMax = true;
                    // For every fine cell in the neighbourhood:
                    for (auto iKV : dict_Neighbours_Of_Seed) {   // On teste toutes les nouvelles cellules possibles pour prendre celle qui minimise localement l'Aspect Ratio.

                        long iFinerCell = iKV.first;
                        if (argMaxFacesInCommon == -1) {
                            argMaxFacesInCommon = iFinerCell;
                        }

                        if (dict_Coarse_Elem.count(fineCellToCoarseCell[iFinerCell]) == 1) {
                            // Est ce que la cellule grossiere associee a la cellule fine iFinerCell existe et est mutable?
                            // numberFacesInCommon = self.__computeNumberOfCommonFaces(iFinerCell, iCoarseCell, iLevel,
                            //                                                         matrixAdj_CRS_row_ptr,
                            //                                                         matrixAdj_CRS_col_ind,
                            //                                                         dict_Coarse_Elem)
                            int numberFacesInCommon = computeNumberOfCommonFaces(iFinerCell, iCoarseCell,
                                                                                 matrixAdj_CRS_row_ptr,
                                                                                 matrixAdj_CRS_col_ind,
                                                                                 fineCellToCoarseCell);
                            // print "iFinerCell", iFinerCell, numberFacesInCommon, ' in CoarseCell', self._Fine_Cell_indices_To_Coarse_Cell_Indices[iLevel][iFinerCell], dict_Coarse_Elem[ self._Fine_Cell_indices_To_Coarse_Cell_Indices[iLevel][iFinerCell]]
//                            cout<< "\t\tiFinerCell "<< iFinerCell<<" "<<numberFacesInCommon<< " in CoarseCell "<<fineCellToCoarseCell[iFinerCell]<<endl;//, dict_Coarse_Elem[ self._Fine_Cell_indices_To_Coarse_Cell_Indices[iLevel][iFinerCell]]
                            int order = dict_Neighbours_Of_Seed[iFinerCell];

                            // TODO This version seems good but refactorisation to do: perhaps it is not needed to compute every new possible coarse cell aspect ratio?
                            // TODO also need to remove the list of minAR, argminAR, etc.
                            if (numberFacesInCommon >= maxFacesInCommon) {
                                if (numberFacesInCommon == maxFacesInCommon) {
                                    if (order <= dict_Neighbours_Of_Seed[argMaxFacesInCommon]) {
//                                        cout<<"IF numberFacesInCommon "<<numberFacesInCommon<<" iFinerCell "<<iFinerCell<<endl;
                                        //Creation of tmp_list_Current vector
//                                        vector<long> tmp_list_Current(dict_Coarse_Elem[iCoarseCell].size());
//                                        int i_tmp = 0;
//                                        for (auto iFC:dict_Coarse_Elem[iCoarseCell]) {
//                                            tmp_list_Current[i_tmp] = iFC;
//                                            i_tmp++;
//                                        }
//                                        tmp_list_Current.push_back(iFinerCell);

                                        //Creation of tmp_list_Neighbour vector
//                                        vector<long> tmp_list_Neighbour(
//                                                dict_Coarse_Elem[fineCellToCoarseCell[iFinerCell]].size() - 1);
//                                        i_tmp = 0;
//                                        for (auto iFC:dict_Coarse_Elem[fineCellToCoarseCell[iFinerCell]]) {
//                                            if (iFC != iFinerCell) {
//                                                tmp_list_Neighbour[i_tmp] = iFC;
//                                                i_tmp++;
//                                            }
//                                        }
                                        unordered_set<long> tmp_set_Current = dict_Coarse_Elem[iCoarseCell];
                                        tmp_set_Current.insert(iFinerCell);
                                        unordered_set<long> tmp_set_Neighbour = dict_Coarse_Elem[fineCellToCoarseCell[iFinerCell]];
                                        tmp_set_Neighbour.erase(iFinerCell);
                                        // print "iFinerCell", iFinerCell
                                        // tmp_list_Neighbour.erase(iFinerCell);
                                        if ((checkConnectivity_w_set(tmp_set_Current, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) &&
                                            (checkConnectivity_w_set(tmp_set_Neighbour, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind))) {
                                            // The second condition asserts the connectivity of the coarse element.
                                            argMaxFacesInCommon = iFinerCell;
                                            isDefaultValue_argMax = false;
                                            // The number of face in common is the same no need to touch it
                                        }
                                    }
                                } else {
//                                    cout<<"Else numberFacesInCommon "<<numberFacesInCommon<<" iFinerCell "<<iFinerCell<<endl;
//                                    tmp_list_Current = list_of_fine_cells_for_Current_Coarse_Cell[:]
//                                    tmp_list_Current.append(iFinerCell);
//                                    tmp_list_Neighbour = dict_Coarse_Elem[fineCellToCoarseCell[iFinerCell]][:];
//                                    tmp_list_Neighbour.remove(iFinerCell);
                                    //Creation of tmp_list_Current vector
                                    vector<long> tmp_list_Current(dict_Coarse_Elem[iCoarseCell].size());
                                    int i_tmp = 0;
                                    for (auto iFC:dict_Coarse_Elem[iCoarseCell]) {
                                        tmp_list_Current[i_tmp] = iFC;
                                        i_tmp++;
                                    }
                                    tmp_list_Current.push_back(iFinerCell);
//                                    cout<<"\t\ttmp_list_Current= [";
//                                    for (int i=0; i<tmp_list_Current.size(); i++)
//                                    {
//                                        cout<<tmp_list_Current[i]<<", ";
//                                    }
//                                    cout<<"]"<<endl;
                                    //Creation of tmp_list_Neighbour vector
                                    vector<long> tmp_list_Neighbour(dict_Coarse_Elem[fineCellToCoarseCell[iFinerCell]].size() - 1);
                                    i_tmp = 0;
                                    for (auto iFC:dict_Coarse_Elem[fineCellToCoarseCell[iFinerCell]]) {
                                        if (iFC != iFinerCell) {
                                            tmp_list_Neighbour[i_tmp] = iFC;
                                            i_tmp++;
                                        }
                                    }
//                                    cout<<"\t\ttmp_list_Neighbour= [";
//                                    for (int i=0; i<tmp_list_Neighbour.size(); i++)
//                                    {
//                                        cout<<tmp_list_Neighbour[i]<<", ";
//                                    }
//                                    cout<<"]"<<endl;
                                    unordered_set<long> tmp_set_Current = dict_Coarse_Elem[iCoarseCell];
                                    tmp_set_Current.insert(iFinerCell);
                                    unordered_set<long> tmp_set_Neighbour = dict_Coarse_Elem[fineCellToCoarseCell[iFinerCell]];
                                    tmp_set_Neighbour.erase(iFinerCell);

                                    if ((checkConnectivity_w_set(tmp_set_Current, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind))
                                        && (checkConnectivity_w_set(tmp_set_Neighbour, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind))) {

                                        // Case :numberFacesInCommon > maxFacesInCommon:
                                        maxFacesInCommon = numberFacesInCommon;
                                        argMaxFacesInCommon = iFinerCell;
                                        isDefaultValue_argMax = false;
//                                        cout<<"\t\targMaxFacesInCommon "<<iFinerCell<<endl;
                                    }
//                                    else{
//                                        cout<<"\t\t\tProbleme"<<endl;
// }
                                }
                            }
                        }
                    }
                    if (!isDefaultValue_argMax) {
                        // list_of_fine_cells_for_Current_Coarse_Cell.append(argMaxFacesInCommon)
                        size_Current_Coarse_Cell += 1;

                        dict_Neighbours_Of_Seed.erase(argMaxFacesInCommon);
                        long iOldCoarseCell = fineCellToCoarseCell[argMaxFacesInCommon];
                        // dict_Coarse_Elem[iOldCoarseCell].remove(argMaxFacesInCommon)
//                        cout<<"Swap fine Cell "<<argMaxFacesInCommon<<" from "<<iOldCoarseCell<<" to "<<iCoarseCell<<endl;
                        unordered_set<long> set_tmp = swapFineCell(argMaxFacesInCommon, iOldCoarseCell, iCoarseCell,
                                                                   dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                                                   dict_DistributionOfCardinalOfCoarseElements,
                                                                   fineCellToCoarseCell);
                        setOfModifiedCoarseCells.insert(iOldCoarseCell);
                        for (auto iST:set_tmp) {
                            set_removedCoarseCells.insert(iST);
                        }
                    } else {
                        break;
                    }
                    // set_removedCoarseCells.update(set_tmp);
                }
                // print "iCoarseCell", iCoarseCell

                // Phase de verification!
                for (long iCC: setOfModifiedCoarseCells) {

                    if (dict_Coarse_Elem.count(iCC) == 1) {  // iCC cell may have been eaten!
//                        cout<<"test of CC "<<iCC<<endl;
//                        vector<long> l(dict_Coarse_Elem[iCC].size());
//                        int i_iFC_tmp = 0;
//                        for (long iFC_tmp : dict_Coarse_Elem[iCC]) {
//                            l[i_iFC_tmp] = iFC_tmp;
//                            i_iFC_tmp++;
//                        }
                        if (!checkConnectivity_w_set(dict_Coarse_Elem[iCC], matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                            // print "Treatment of non connected cell", iCC, "l=", l
                            splitNonConnectedCoarseCell(indCoarseCell,
                                                        numberOfFineAgglomeratedCells, iCC,
                                                        dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                                        dict_DistributionOfCardinalOfCoarseElements,
                                                        matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind,
                                                        isFineCellAgglomerated,
                                                        fineCellToCoarseCell);
                        }
                    }
                }
//                assert((size_Current_Coarse_Cell == goalCard);
                //"Pb: wrong number of fine cells in Current Coarse Cell" + str(
                // list_of_fine_cells_for_Current_Coarse_Cell)
            }
        }
    }
    // print "Final Coarse cell ", iCoarseCell, dict_Coarse_Elem[iCoarseCell], "set_removedCoarseCells", set_removedCoarseCells
    if (!set_removedCoarseCells.empty()) {
        removeDeletedCoarseCells_v3(dict_Coarse_Elem,
                                    dict_Card_Coarse_Cells,
                                    fineCellToCoarseCell,
                                    set_removedCoarseCells,
                                    indCoarseCell);
    }
    if (verbose) {
        cout << "End of __makeSmallCellBigger" << endl;
    }
//    return dict_Coarse_Elem, dict_Card_Coarse_Cells, indCoarseCell, numberOfFineAgglomeratedCells
}


void agglomerate_Isotropic_Correction_Swap(unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                           unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                           long *matrixAdj_CRS_row_ptr,
                                           long *matrixAdj_CRS_col_ind,
                                           unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                           long &indCoarseCell,
                                           long numberOfFineCells,
                                           long *fineCellToCoarseCell,
                                           bool verbose) {

    int nbIteration = 5;
    unordered_set<long> set_removedCoarseCells;

    int iteration = 0;
    bool isSwap = true;
    while ((iteration < nbIteration) and (isSwap)) {
        if (verbose) {
            cout << "\niteration" << iteration << endl;
        }
        isSwap = false;
        int nbOfSwap = 0;
        for (long iFineCell = 0; iFineCell < numberOfFineCells; iFineCell++) {

            long iCoarse = fineCellToCoarseCell[iFineCell];
            if (dict_Coarse_Elem.count(iCoarse) == 1) { //We work only on isotropic cells!

                int nbCommonFaces_iFineCell = computeNumberOfCommonFaces(iFineCell, iCoarse,
                                                                         matrixAdj_CRS_row_ptr,
                                                                         matrixAdj_CRS_col_ind,
                                                                         fineCellToCoarseCell);
                if (nbCommonFaces_iFineCell <= 1) {

                    // the fine cell is attached to the current coarse cell with only one face
                    // Computation of the neighbourhood:
                    unordered_set<long> setNeighbour;
                    long ind = matrixAdj_CRS_row_ptr[iFineCell];
                    long ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1];
                    for (long i = ind; i < ind_p_one; i++) {  // We process every fine neighbour of indFineCell
                        long indOfFineNeighbor = matrixAdj_CRS_col_ind[i];
                        if ((indOfFineNeighbor != iFineCell) && (dict_Coarse_Elem[iCoarse].count(indOfFineNeighbor) == 0) && (dict_Coarse_Elem.count(fineCellToCoarseCell[indOfFineNeighbor]) == 1)) {
                            setNeighbour.insert(fineCellToCoarseCell[indOfFineNeighbor]);
                        }
                    }

                    long argmaxNbCommonFaces_N = -1;
                    int maxNbCommonFaces_N = -1;
                    for (long iCoarseNeighbour: setNeighbour) {

                        int nbCommonFaces_N = computeNumberOfCommonFaces(iFineCell, iCoarseNeighbour, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, fineCellToCoarseCell);
                        // cardN = len(dict_Coarse_Elem[iCoarseNeighbour])
                        // print "Neighbour: iCell", iFineCell, "iCoarseNeighbour", iCoarseNeighbour, "Card=", cardN, "NbCommonFaces", nbCommonFaces_N
                        if (maxNbCommonFaces_N < nbCommonFaces_N) {
                            maxNbCommonFaces_N = nbCommonFaces_N;
                            argmaxNbCommonFaces_N = iCoarseNeighbour;
                        }
                    }
                    // Pourquoi il y a ca????

                    // TODO Essayer de retrouver a quoi ca sert!!!
                    // if iteration == 0:
                    //     minSize = 2
                    // else:
                    //     minSize = 1
                    int minSize = 1;
                    if (maxNbCommonFaces_N > minSize) {
                        // print "\niCell", iFineCell, " C=", iCoarse, "Card=", cardCurrent, "NbCommonFaces", nbCommonFaces_iFineCell
                        // print "On propose d'echanger la cellule ",iFineCell, "appartenant a iC", iCoarse, "pour la donner a ", argmaxNbCommonFaces_N, " car ",maxNbCommonFaces_N-1,  "de faces en plus en commun"
                        unordered_set<long> set_tmp = swapFineCell(iFineCell, iCoarse, argmaxNbCommonFaces_N, dict_Coarse_Elem, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                                                                   fineCellToCoarseCell);
//                        set_removedCoarseCells.update(set_tmp);
                        for (auto i_S : set_tmp) {
                            set_removedCoarseCells.insert(i_S);
                        }
                        nbOfSwap += 1;
                        isSwap = true;
                    }
                }
            }
        }
        iteration += 1;
        if (verbose) {
            cout << "nbOfSwap = " << nbOfSwap << endl;
            // print "Neighbour: iCell", indFineCell, "iCoarseNeighbour", iCoarseNeighbour, "Card=", cardN, "NbCommonFaces", nbCommonFaces_N
            cout << "After iteration " << iteration << endl;
            cout << "dict_DistributionOfCardinalOfCoarseElements [";
            for (auto i :dict_DistributionOfCardinalOfCoarseElements) {
                cout << "{" << i.first << " : " << i.second << "} ";
            }
            cout << endl;
        }
    }
    if (!set_removedCoarseCells.empty()) {
        removeDeletedCoarseCells_v3(dict_Coarse_Elem, dict_Card_Coarse_Cells, fineCellToCoarseCell, set_removedCoarseCells, indCoarseCell);
    }

}

void agglomerate_Isotropic_Correction_Too_Big_Cells(unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                                    unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                    long *matrixAdj_CRS_row_ptr,
                                                    long *matrixAdj_CRS_col_ind,
                                                    unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                                    long *fineCellToCoarseCell,
                                                    long &indCoarseCell,
                                                    int goalCard,
                                                    bool verbose) {

    // TODO Add variable for cardMin et cardMax.
    // TODO do not work on every cell with a if, but instead use dict_Card_Coarse_Cells...
    unordered_set<long> set_removedCoarseCells;
    for (long iCoarseCell = 0; iCoarseCell < dict_Coarse_Elem.size(); iCoarseCell++) {
//        cout<<"\n=================>  iCoarseCell"<<iCoarseCell<<endl;
        if (dict_Coarse_Elem.count(iCoarseCell) == 1) {
            if (dict_Coarse_Elem[iCoarseCell].size() > goalCard) {
                unordered_set<long> tmp_copy = dict_Coarse_Elem[iCoarseCell];
                for (long iFineCell: tmp_copy) {
//                    cout<<"\tiFineCell "<<iFineCell<<endl;
                    int nbCommonFaces_iFineCell = computeNumberOfCommonFaces(iFineCell, iCoarseCell, matrixAdj_CRS_row_ptr,
                                                                             matrixAdj_CRS_col_ind,
                                                                             fineCellToCoarseCell);
                    if (nbCommonFaces_iFineCell == 1) {
                        // Neighbours:
                        // setNeighbour = set()
                        int minCardNeighbour = numeric_limits<int>::max();
                        long argMinCardNeighbour = -1;
                        long ind = matrixAdj_CRS_row_ptr[iFineCell];
                        long ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1];
                        for (long i = ind; i < ind_p_one; i++) {
                            long indOfFineNeighbor = matrixAdj_CRS_col_ind[i];
                            if ((indOfFineNeighbor != iFineCell) && (dict_Coarse_Elem[iCoarseCell].count(indOfFineNeighbor) == 0)) {

                                long iCoarseNeighbour = fineCellToCoarseCell[indOfFineNeighbor];
                                // TODO cette fonction etait appelee, mais sans utilisation du retour??? Comprendre!
                                if (dict_Coarse_Elem.count(iCoarseNeighbour)) {
                                    int sizeNeighbour = dict_Coarse_Elem[iCoarseNeighbour].size();
                                    if (sizeNeighbour < goalCard) {
                                        if (minCardNeighbour > sizeNeighbour) {
                                            minCardNeighbour = sizeNeighbour;
                                            argMinCardNeighbour = iCoarseNeighbour;
                                        }
                                    }
                                }
                            }

                        }

                        if (minCardNeighbour < goalCard) {
                            unordered_set<long> set_tmp = swapFineCell(iFineCell, iCoarseCell, argMinCardNeighbour,
                                                                       dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                                                       dict_DistributionOfCardinalOfCoarseElements,
                                                                       fineCellToCoarseCell);
                            for (auto iST: set_tmp) {
                                set_removedCoarseCells.insert(iST);
                            }

                        }
                    }
                }
            }
        }
    }
    if (verbose) {
        cout << "After Destruction too big cells!" << endl;
//        cout<<"dict_DistributionOfCardinalOfCoarseElements "<< dict_DistributionOfCardinalOfCoarseElements<<endl;
    }
    if (set_removedCoarseCells.empty()) {
        removeDeletedCoarseCells_v3(dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                    fineCellToCoarseCell,
                                    set_removedCoarseCells,
                                    indCoarseCell);
    }
}

void agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(int Nbsizes,
                                                                 long *sizes,
                                                                 vector<queue<long>> &listOfSeeds,
                                                                 unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                                                 unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                                 long *matrixAdj_CRS_row_ptr,
                                                                 long *matrixAdj_CRS_col_ind,
                                                                 double *matrixAdj_CRS_values,
                                                                 double *volumes,
                                                                 unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                                                 long &indCoarseCell,
                                                                 long *fineCellToCoarseCell,
                                                                 long numberOfFineAgglomeratedCells,
                                                                 bool *isFineCellAgglomerated,
                                                                 int *isOnFineBnd,
                                                                 int minCard,
                                                                 int maxCard,
                                                                 bool checks,
                                                                 bool verbose) {

    int maxSize = numeric_limits<int>::min();

    for (auto iKV : dict_Card_Coarse_Cells) {
        if (maxSize < iKV.first) {
            maxSize = iKV.first;
        }
    }
//    cout << "maxSize " << maxSize << endl;
//    int maxSize = max(dict_Card_Coarse_Cells.keys());
    if (verbose) {
        cout << "\n\n\n__agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo maxSize= " << maxSize << endl;
    }
    // 1 <= card(CoarseCell) <= self.__threshold
    for (int iSize = maxCard + 1; iSize < maxSize + 1; iSize++) {
        if (dict_Card_Coarse_Cells.count(iSize) == 1) {

            if (verbose) {
                cout << "\n==========================\n iSize= " << iSize << endl;
            }

            unordered_set<long> tmp_set_Coarse_Cell_Of_Size_iSize = dict_Card_Coarse_Cells[iSize];
            for (long iCoarseCell: tmp_set_Coarse_Cell_Of_Size_iSize) {


                unordered_set<long> setOfFineCells = dict_Coarse_Elem[iCoarseCell];

                vector<long> listOfFineCells(setOfFineCells.size());
                int iCount = 0;
                for (auto iSOFC : setOfFineCells) {

                    listOfFineCells[iCount] = iSOFC;
                    iCount++;
                }

                if (verbose) {
                    cout << "\nToo Big Cell: " << iCoarseCell << endl;
                    cout << "setOfFineCells= [";
                    for (auto i : setOfFineCells) {
                        cout << i << ", ";
                    }
                    cout << "]" << endl;
                }

                long localSizes[3] = {sizes[0], sizes[1], 0};

                unordered_map<long, queue<long> *> dict_ConnectivityTree = findSeedViaFrontalMethod(3, localSizes, listOfFineCells, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind);

                long seed = localSizes[2];
                if (verbose) {
                    cout << "seed " << seed << endl;
//                    << " dict_ConnectivityTree :"<< dict_ConnectivityTree<<endl;
                }

                // We remove from the setOfFineCells the cells of degree greater than (or equal to) 3.
                long iter_seed = seed;
//                if (dict_ConnectivityTree[iter_seed]->size() >= 2) {
//                    if (verbose) {
//                        cout << "Cycle! " << iCoarseCell << setOfFineCells << endl;
//                    }
//                }

                removeSeparatingVertex(seed, dict_ConnectivityTree, setOfFineCells, matrixAdj_CRS_row_ptr,
                                       matrixAdj_CRS_col_ind);
//                if(verbose) {
//                    cout << "setOfFineCells " << setOfFineCells << endl;
//                }
                // Computation of the neighbourhood:
                unordered_map<long, int> dict_Neighbours_Of_Seed = computation_Of_Neighbourhood(seed, 3, matrixAdj_CRS_row_ptr,
                                                                                                matrixAdj_CRS_col_ind,
                                                                                                maxCard, isFineCellAgglomerated,
                                                                                                &setOfFineCells);
                if (verbose) {
                    cout << "dict_Neighbours_Of_Seed ["; //<< dict_Neighbours_Of_Seed<<endl;
                    for (auto iKV: dict_Neighbours_Of_Seed) {
                        cout << "(" << iKV.first << ", " << iKV.second << ") ";
                    }
                    cout << "]" << endl;
                }

                if (dict_Neighbours_Of_Seed.empty()) {
                    // If dict_Neighbours_Of_Seed is empty
                    // We transfert the seed to one of its coarse neighbour.
                    // print seed, dict_Coarse_Elem[iCoarseCell]
                    long ind = matrixAdj_CRS_row_ptr[seed];  // Usefull to find neighbours of seed
                    long ind_p_one = matrixAdj_CRS_row_ptr[seed + 1];  // Usefull to find neighbours of seed
                    for (long i = ind; i < ind_p_one; i++) {

                        long indCellNeighbour = matrixAdj_CRS_col_ind[i];
                        if ((indCellNeighbour != seed) && (dict_Coarse_Elem[iCoarseCell].count(indCellNeighbour) == 0)) {
                            long indCoarseCellNeighbour = fineCellToCoarseCell[indCellNeighbour];
                            if (dict_Coarse_Elem.count(indCoarseCellNeighbour) == 1) {
                                if (dict_Coarse_Elem[indCoarseCellNeighbour].size() < maxCard) {
                                    swapFineCell(seed, iCoarseCell, indCoarseCellNeighbour,
                                                 dict_Coarse_Elem,
                                                 dict_Card_Coarse_Cells,
                                                 dict_DistributionOfCardinalOfCoarseElements,
                                                 fineCellToCoarseCell);
                                    break;
                                }
                            }
                        }
                    }
                } else {
                    // dict_Neighbours_Of_Seed is not empty:

                    // TODO remove numberOfFineAgglomeratedCells
                    unordered_set<long> listOfFineCellsForCurrentCoarseCell = choice_Of_Agglomerated_Cells(seed, listOfSeeds,
                                                                                                           dict_Neighbours_Of_Seed,
                                                                                                           matrixAdj_CRS_row_ptr,
                                                                                                           matrixAdj_CRS_col_ind,
                                                                                                           matrixAdj_CRS_values, volumes,
                                                                                                           minCard,
                                                                                                           maxCard,
                                                                                                           isFineCellAgglomerated,
                                                                                                           isOnFineBnd,
                                                                                                           numberOfFineAgglomeratedCells,
                                                                                                           true);
//                    if(verbose) {
//                        cout << "listOfFineCellsForCurrentCoarseCell " << listOfFineCellsForCurrentCoarseCell << endl;
//                    }
                    unordered_set<long> l;
                    // Creation of an empty coarse cell:
                    createCoarseCell(l, dict_Coarse_Elem, dict_Card_Coarse_Cells,
                                     dict_DistributionOfCardinalOfCoarseElements, indCoarseCell,
                                     numberOfFineAgglomeratedCells,
                                     isFineCellAgglomerated,
                                     fineCellToCoarseCell, true);

                    // We want that the new cell to be the smallest of the two.
                    int sizeOriginalICoarseCell = listOfFineCells.size();
                    int sizeNewCoarseCell = listOfFineCellsForCurrentCoarseCell.size();
                    if (sizeNewCoarseCell > sizeOriginalICoarseCell - sizeNewCoarseCell) {
                        unordered_set<long> complementary_list;  // the smallest !
                        for (long i:dict_Coarse_Elem[iCoarseCell]) {
                            if (listOfFineCellsForCurrentCoarseCell.count(i) == 0) {
                                complementary_list.insert(i);
                            }
                        }
                        for (long iFineCell : complementary_list) {
                            swapFineCell(iFineCell, iCoarseCell, indCoarseCell - 1,
                                         dict_Coarse_Elem,
                                         dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                                         fineCellToCoarseCell);
                        }
                    } else {
                        for (long iFineCell : listOfFineCellsForCurrentCoarseCell) {
                            swapFineCell(iFineCell, iCoarseCell, indCoarseCell - 1,
                                         dict_Coarse_Elem,
                                         dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements,
                                         fineCellToCoarseCell);
                        }
                    }
// if self._checks:
//     if not Util.checkConnectivity(dict_Coarse_Elem[indCoarseCell - 1],
//                                   matrixAdj_CRS_row_ptr,
//                                   matrixAdj_CRS_col_ind):
//         print "ERROR", indCoarseCell - 1, dict_Coarse_Elem[indCoarseCell - 1]
//
//         raise ValueError

                    assert(checkConnectivity_w_set(dict_Coarse_Elem[indCoarseCell - 1], matrixAdj_CRS_row_ptr,
                                                   matrixAdj_CRS_col_ind));
                    //, "ERROR " + str(indCoarseCell - 1) + " " + str(dict_Coarse_Elem[indCoarseCell - 1])
                }
                if (checks) {
                    assert(checkConnectivity_w_set(dict_Coarse_Elem[iCoarseCell], matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind));
                }
            }
        }
    }
}


void compute_Dicts_From_FineCellIndicesToCoarseCellIndices(long nbOfFineCells,
                                                           long *fineCellIndicesToCoarseCellIndices,
                                                           unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                                           unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                           unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements) {

    for (long iFC = 0; iFC < nbOfFineCells; iFC++) {
        long iCC = fineCellIndicesToCoarseCellIndices[iFC];
        //iCC in enumerate(fineCellIndicesToCoarseCellIndices)
        if (dict_Coarse_Cells.count(iCC) == 1) {
            dict_Coarse_Cells[iCC].insert(iFC);
        } else {
            dict_Coarse_Cells[iCC] = {iFC};
        }
    }

    for (auto iKV_CC : dict_Coarse_Cells) {
        int size = iKV_CC.second.size();
        if (dict_Card_Coarse_Cells.count(size) == 1) {
            dict_Card_Coarse_Cells[size].insert(iKV_CC.first);
            dict_DistributionOfCardinalOfCoarseElements[size] += 1;
        } else {
            dict_Card_Coarse_Cells[size] = {iKV_CC.first};
            dict_DistributionOfCardinalOfCoarseElements[size] = 1;
        }
    }

}

void agglomerate_Isotropic_Second_Step_Correction(int numberOfInts, long *sizes,
                                                  long *matrixAdj_CRS_row_ptr,
                                                  long *matrixAdj_CRS_col_ind,
                                                  double *matrixAdj_CRS_values,
                                                  double *volumes,
                                                  unordered_map<long, unordered_set<long>> &dict_CoarseCells,
                                                  unordered_map<int, unordered_set<long>> &dict_CardCoarseCells,
                                                  unordered_map<int, long> &dict_DistribOfCardOfCoarseCells,

                                                  bool *isFineCellAgglomerated,
                                                  long *fineCellToCoarseCell,
                                                  int *isOnFineBnd,
                                                  int minCard,
                                                  int goalCard,
                                                  int maxCard,
                                                  int thresholdCard,
                                                  bool checks,
                                                  bool verbose) {
//    cout << "\nagglomerate_Isotropic_Second_Step_Correction    " << endl;

    long indCoarseCell = sizes[2];
    long numberOfFineAgglomeratedCells = sizes[3];

    int nbIteration = 4;
    for (int i = 0; i < nbIteration; i++) {
//        cout << "\n\n\n=======================================================================" << endl;
//        cout << "ITERATION    " << i << endl;

        if (verbose) {
            cout << "\n\n\n=======================================================================" << endl;
            cout << "ITERATION    " << i << endl;
            cout << "dict_DistributionOfCardinalOfCoarseElements: [";
            for (auto iKV:dict_DistribOfCardOfCoarseCells) {
                cout << "{" << iKV.first << ", " << iKV.second << "} ";
            }
            cout << "]" << endl;
        }

        // Step One: too small coarse cells (1<= size<are agglomerated, fine cell by fine cell
        // to their "better" neighbour. 1<= Card <= self.__threshold

        remove_Too_Small_Cells_v2(thresholdCard,
                                  fineCellToCoarseCell,
                                  indCoarseCell,
                                  matrixAdj_CRS_row_ptr,
                                  matrixAdj_CRS_col_ind,
                                  matrixAdj_CRS_values,
                                  dict_CoarseCells,
                                  dict_CardCoarseCells,
                                  dict_DistribOfCardOfCoarseCells);

        if (checks) {
//            cout << "\tChecks!" << endl;
//            cout << "\t\t consistancy" << endl;
            agglomerate_Isotropic_CheckConsistancyDictCoarseCells(dict_CoarseCells,
                                                                  dict_CardCoarseCells, sizes[0], fineCellToCoarseCell);
//            cout << "\t\t connectivity" << endl;
            // Phase de verification!
            for (auto index :dict_CoarseCells) {

//                unordered_set<long> s = dict_CoarseCells[index];
                if (!checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                    checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, 1);
                    cout << "Error for coarse cell " << index.first << " [";
                    for (long iFC: index.second) {
                        cout << iFC << ", ";
                    }
                    cout << "]" << endl;

                    throw logic_error("Connectivity Error");
                }
            }
        }
        // print "dict_DistributionOfCardinalOfCoarseElements", dict_DistributionOfCardinalOfCoarseElements
        if (verbose) {
            cout << "Correction After step one: dict_DistributionOfCardinalOfCoarseElements [";
            for (auto iKV:dict_DistribOfCardOfCoarseCells) {
                cout << "{" << iKV.first << ", " << iKV.second << "}, ";
            }
            cout << "]" << endl;

        }
        // print "\nCall of __makeSmallCellBigger"
        // print "dict_Coarse_Elem", dict_Coarse_Elem
        // Step Two: // threshold <= Card <= minCard

        makeSmallCellBigger(dict_CoarseCells, dict_CardCoarseCells, matrixAdj_CRS_row_ptr,
                            matrixAdj_CRS_col_ind, dict_DistribOfCardOfCoarseCells,
                            indCoarseCell,
                            numberOfFineAgglomeratedCells,
                            isFineCellAgglomerated,
                            fineCellToCoarseCell,
                            minCard,
                            goalCard, thresholdCard, verbose);

        if (verbose) {
            cout << "Correction After step 2: Distribution of C C  [";
            for (auto iKV:dict_DistribOfCardOfCoarseCells) {
                cout << "{" << iKV.first << ", " << iKV.second << "}, ";
            }
            cout << "]" << endl;

        }
        if (checks) {
            agglomerate_Isotropic_CheckConsistancyDictCoarseCells(dict_CoarseCells,
                                                                  dict_CardCoarseCells, sizes[0], fineCellToCoarseCell);
            // Phase de verification!
            for (auto index :dict_CoarseCells) {

//                unordered_set<long> s = dict_CoarseCells[index];
                if (!checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                    checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, 1);
                    cout << "Error for coarse cell " << index.first << " [";
                    for (long iFC: index.second) {
                        cout << iFC << ", ";
                    }
                    cout << "]" << endl;

                    throw logic_error("Connectivity Error");
                }
            }
        }
        // print "\nAFTER __makeSmallCellBigger"
        // print "dict_Coarse_Elem", dict_Coarse_Elem
        // Step Two: Swap for every fine cell.
        agglomerate_Isotropic_Correction_Swap(dict_CoarseCells, dict_CardCoarseCells,
                                              matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind,
                                              dict_DistribOfCardOfCoarseCells, indCoarseCell,
                                              sizes[0],
                                              fineCellToCoarseCell,
                                              verbose);

        if (verbose) {
            cout << "Correction After step 3: Distribution of C C\"  [";
            for (auto iKV:dict_DistribOfCardOfCoarseCells) {
                cout << "{" << iKV.first << ", " << iKV.second << "}, ";
            }
            cout << "]" << endl;

        }

        if (checks) {
            agglomerate_Isotropic_CheckConsistancyDictCoarseCells(dict_CoarseCells,
                                                                  dict_CardCoarseCells, sizes[0], fineCellToCoarseCell);
            // Phase de verification!
            for (auto index :dict_CoarseCells) {

//                unordered_set<long> s = dict_CoarseCells[index];
                if (!checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                    checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, 1);
                    cout << "Error for coarse cell " << index.first << " [";
                    for (long iFC: index.second) {
                        cout << iFC << ", ";
                    }
                    cout << "]" << endl;

                    throw logic_error("Connectivity Error");
                }
            }
        }

        // Step Three: Too big cells are corrected!
        agglomerate_Isotropic_Correction_Too_Big_Cells(dict_CoarseCells, dict_CardCoarseCells,
                                                       matrixAdj_CRS_row_ptr,
                                                       matrixAdj_CRS_col_ind,
                                                       dict_DistribOfCardOfCoarseCells,
                                                       fineCellToCoarseCell,
                                                       indCoarseCell,
                                                       goalCard, verbose);

        if (checks) {
            agglomerate_Isotropic_CheckConsistancyDictCoarseCells(dict_CoarseCells,
                                                                  dict_CardCoarseCells, sizes[0], fineCellToCoarseCell);
            // Phase de verification!
            for (auto index :dict_CoarseCells) {

//                unordered_set<long> s = dict_CoarseCells[index];
                if (!checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                    checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, 1);
                    cout << "Error for coarse cell " << index.first << " [";
                    for (long iFC: index.second) {
                        cout << iFC << ", ";
                    }
                    cout << "]" << endl;

                    throw logic_error("Connectivity Error");
                }
            }
        }

        if (verbose) {
            cout << "Correction After step 4: Distribution of C C\"  [";
            for (auto iKV:dict_DistribOfCardOfCoarseCells) {
                cout << "{" << iKV.first << ", " << iKV.second << "}, ";
            }
            cout << "]" << endl;
        }

        vector<queue<long>> listOfSeeds(4);
        for (int i = 0; i < 4; i++) {
            listOfSeeds[i] = queue<long>();
        }

        agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(6, sizes, listOfSeeds,
                                                                    dict_CoarseCells,
                                                                    dict_CardCoarseCells,
                                                                    matrixAdj_CRS_row_ptr,
                                                                    matrixAdj_CRS_col_ind,
                                                                    matrixAdj_CRS_values, volumes,
                                                                    dict_DistribOfCardOfCoarseCells,
                                                                    indCoarseCell,
                                                                    fineCellToCoarseCell,
                                                                    numberOfFineAgglomeratedCells,
                                                                    isFineCellAgglomerated,
                                                                    isOnFineBnd,
                                                                    minCard,
                                                                    maxCard,
                                                                    checks,
                                                                    verbose);

        if (checks) {
            agglomerate_Isotropic_CheckConsistancyDictCoarseCells(dict_CoarseCells,
                                                                  dict_CardCoarseCells, sizes[0], fineCellToCoarseCell);
            // Phase de verification!
            for (auto index :dict_CoarseCells) {

                if (!checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                    checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, 1);
                    cout << "Error for coarse cell " << index.first << " [";
                    for (long iFC: index.second) {
                        cout << iFC << ", ";
                    }
                    cout << "]" << endl;

                    throw logic_error("Connectivity Error");
                }
            }
        }
        if (verbose) {
            cout << "Correction After step 5: Distribution of C C\"  [";
            for (auto iKV:dict_DistribOfCardOfCoarseCells) {
                cout << "{" << iKV.first << ", " << iKV.second << "}, ";
            }
            cout << "]" << endl;
        }

//        cout << "remove_Too_Small_Cells_v2" << endl;
        remove_Too_Small_Cells_v2(thresholdCard,
                                  fineCellToCoarseCell,
                                  indCoarseCell,
                                  matrixAdj_CRS_row_ptr,
                                  matrixAdj_CRS_col_ind,
                                  matrixAdj_CRS_values,
                                  dict_CoarseCells,
                                  dict_CardCoarseCells,
                                  dict_DistribOfCardOfCoarseCells);

        if (checks) {
            agglomerate_Isotropic_CheckConsistancyDictCoarseCells(dict_CoarseCells,
                                                                  dict_CardCoarseCells, sizes[0], fineCellToCoarseCell);
            // Phase de verification!
            for (auto index :dict_CoarseCells) {

                if (!checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind)) {
                    checkConnectivity_w_set(index.second, matrixAdj_CRS_row_ptr, matrixAdj_CRS_col_ind, 1);
                    cout << "Error for coarse cell " << index.first << " [";
                    for (long iFC: index.second) {
                        cout << iFC << ", ";
                    }
                    cout << "]" << endl;

                    throw logic_error("Connectivity Error");
                }
            }
        }
        if (verbose) {
            cout << "Correction After step 6 deletion too small cells: Distribution of C C\"  [";
            for (auto iKV:dict_DistribOfCardOfCoarseCells) {
                cout << "{" << iKV.first << ", " << iKV.second << "}, ";
            }
            cout << "]" << endl;
        }

    }
    sizes[2] = indCoarseCell;
    sizes[3] = numberOfFineAgglomeratedCells;
}


// TODO remove calls to this function for production
void agglomerate_Isotropic_CheckConsistancyDictCoarseCells(unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                                           unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                           long fineCellIndicesToCoarseCellIndices_size,
                                                           long *fineCellIndicesToCoarseCellIndices) {
//"""
//We check that the data are consistant between dict_Coarse_Cells, dict_Card_Coarse_Cells and fineCellIndicesToCoarseCellIndices
//
//        :param iLevel:
//:param dict_Coarse_Cells:
//:param dict_Card_Coarse_Cells:
//:param fineCellIndicesToCoarseCellIndices:
//:return:
//"""
    for (auto iKV : dict_Coarse_Cells) {

        long iCC = iKV.first;
        for (long iFC: dict_Coarse_Cells[iCC]) {
            assert(fineCellIndicesToCoarseCellIndices[iFC] == iCC);
        }
        int size_iCC = dict_Coarse_Cells[iCC].size();
        assert(dict_Card_Coarse_Cells.count(size_iCC) == 1);
        assert(dict_Card_Coarse_Cells[size_iCC].count(iCC) == 1);
    }
    for (auto iKV : dict_Card_Coarse_Cells) {
        int iSize = iKV.first;
        for (long iCC :dict_Card_Coarse_Cells[iSize]) {
            assert(dict_Coarse_Cells.count(iCC) == 1);
            assert(dict_Coarse_Cells[iCC].size() == iSize);
        }
    }
//    cout << "dict_Coarse_Cells [";
//    for (auto iKV:dict_Coarse_Cells) {
//        cout << "\n" << iKV.first << ": {";
//        for (long iFC:iKV.second) {
//            cout << iFC << ", ";
//        }
//        cout << "}";
//    }
//    cout << "]" << endl;
    for (long iF = 0; iF < fineCellIndicesToCoarseCellIndices_size; iF++) {
        long iC = fineCellIndicesToCoarseCellIndices[iF];
        if (dict_Coarse_Cells.count(iC) == 1) {
            // if isotropic cell...
            if (dict_Coarse_Cells[iC].count(iF) == 0) {
                cout << "iC not in dict_Coarse_Cells iF: " << iF << " iC " << iC << " l [";
                for (long iFC : dict_Coarse_Cells[iC]) {
                    cout << iFC << ", ";
                }
                cout << "]" << endl;
            }
            assert(dict_Coarse_Cells[iC].count(iF) == 1);
        }
    }
}


void store_agglomeration_datas(long *sizes,

                               long *adjMatrix_row_ptr,
                               long *adjMatrix_col_ind,
                               double *adjMatrix_areaValues,
                               double *volumes,

                               long *arrayOfFineAnisotropicCompliantCells,

                               long *isOnFineBnd_l,
                               long *array_isOnValley,
                               long *array_isOnRidge,
                               long *array_isOnCorner,
                               long isFirstAgglomeration_long,
                               long isAnisotropic_long,

                               long *fineCellToCoarseCell,

                               long *agglomerationLines_Idx,
                               long *agglomerationLines,

                               long dimension,
                               long goalCard,
                               long minCard,
                               long maxCard,
                               long checks_long,
                               long verbose_long) {

//    cout << "Call of store_agglomeration_datas" << endl;
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

    bool isFirstAgglomeration = isFirstAgglomeration_long == 1;
    bool isAnisotropic = isAnisotropic_long == 1;
    string filename = "/Users/lantos/CLionProjects/CoMMA_lib/0_Outputs/Datas_Agglomeration_";
    ofstream myfile;
    myfile.open(filename + to_string(numberOfFineCells) + "_" + to_string(adjMatrix_col_ind_size) + ".txt");

    myfile << "adjMatrix_row_ptr; ";
    myfile << to_string(numberOfFineCells + 1) + "; ";
    for (int i = 0; i < numberOfFineCells + 1; i++) {
        myfile << to_string(adjMatrix_row_ptr[i]) << "; ";
    }
    myfile << endl;

    myfile << "adjMatrix_col_ind; ";
    myfile << to_string(adjMatrix_col_ind_size) + "; ";
    for (int i = 0; i < adjMatrix_col_ind_size; i++) {
        myfile << adjMatrix_col_ind[i] << "; ";
    }
    myfile << endl;

    myfile << "adjMatrix_areaValues; ";
    myfile << to_string(adjMatrix_areaValues_size) + "; ";
    for (int i = 0; i < adjMatrix_areaValues_size; i++) {
        myfile<< std::scientific<< adjMatrix_areaValues[i] << "; ";
    }
    myfile << endl;

    myfile << "volumes; ";
    myfile << to_string(numberOfFineCells) + "; ";
    for (int i = 0; i < numberOfFineCells; i++) {
        myfile <<std::scientific<< volumes[i] << "; ";
    }
    myfile << endl;
    myfile << "arrayOfFineAnisotropicCompliantCells; ";
    myfile << to_string(numberOfFineCells) + "; ";
    for (int i = 0; i < numberOfFineCells; i++) {
        myfile << to_string(arrayOfFineAnisotropicCompliantCells[i]) << "; ";
    }
    myfile << endl;


    myfile << "isOnFineBnd_l; ";
    myfile << to_string(numberOfFineCells) + "; ";
    for (int i = 0; i < numberOfFineCells; i++) {
        myfile << to_string(isOnFineBnd_l[i]) << "; ";
    }
    myfile << endl;

    myfile << "array_isOnValley; ";
    myfile << to_string(isOnValley_size) + "; ";
    for (int i = 0; i < isOnValley_size; i++) {
        myfile << to_string(array_isOnValley[i]) << "; ";
    }
    myfile << endl;

    myfile << "array_isOnRidge; ";
    myfile << to_string(isOnRidge_size) + "; ";
    for (int i = 0; i < isOnRidge_size; i++) {
        myfile << to_string(array_isOnRidge[i]) << "; ";
    }
    myfile << endl;

    myfile << "array_isOnCorner; ";
    myfile << to_string(isOnCorner_size) + "; ";
    for (int i = 0; i < isOnCorner_size; i++) {
        myfile << to_string(array_isOnCorner[i]) << "; ";
    }
    myfile << endl;

    myfile << "isFirstAgglomeration_long; ";
    myfile << "1; ";
    myfile << to_string(isFirstAgglomeration_long) + "; ";
    myfile << endl;

    myfile << "isAnisotropic_long; ";
    myfile << "1; ";
    myfile << to_string(isAnisotropic_long) + "; ";
    myfile << endl;

    myfile << "agglomerationLines_Idx; ";
    myfile << to_string(agglomerationLines_Idx_size) + "; ";
    for (int i = 0; i < agglomerationLines_Idx_size; i++) {
        myfile << to_string(agglomerationLines_Idx[i]) << "; ";
    }
    myfile << endl;

    myfile << "agglomerationLines; ";
    myfile << to_string(agglomerationLines_size) + "; ";
    for (int i = 0; i < agglomerationLines_size; i++) {
        myfile << to_string(agglomerationLines[i]) << "; ";
    }
    myfile << endl;

    myfile << "dimension; ";
    myfile << "1; ";
    myfile << to_string(dimension) + "; ";
    myfile << endl;

    myfile << "goalCard; ";
    myfile << "1; ";
    myfile << to_string(goalCard) + "; ";
    myfile << endl;

    myfile << "minCard; ";
    myfile << "1; ";
    myfile << to_string(minCard) + "; ";
    myfile << endl;

    myfile << "maxCard; ";
    myfile << "1; ";
    myfile << to_string(maxCard) + "; ";
    myfile << endl;

    myfile.close();
}

void read_agglomeration_datas_from_file(string filename,
                                        long *sizes,
                                        long *&adjMatrix_row_ptr,
                                        long *&adjMatrix_col_ind,
                                        double *&adjMatrix_areaValues,
                                        double *&volumes,

                                        long *&arrayOfFineAnisotropicCompliantCells,

                                        long *&isOnFineBnd_l,
                                        long *&array_isOnValley,
                                        long *&array_isOnRidge,
                                        long *&array_isOnCorner,
                                        long *isFirstAgglomeration_long,
                                        long *isAnisotropic_long,

                                        long *&agglomerationLines_Idx,
                                        long *&agglomerationLines,

                                        long *dimension,
                                        long *goalCard,
                                        long *minCard,
                                        long *maxCard
) {

//    cout << "\nCall of read_agglomeration_datas" << endl;
    fstream my_file;
    my_file.open(filename, ios::in);
    if (my_file.is_open()) {   //checking whether the file is open
        string tp;

        sizes[0] = read_long_array(my_file, adjMatrix_row_ptr) - 1;
//        cout << "\nadjMatrix_row_ptr= " << sizes[0] << endl;
//        for (int i = 0; i < sizes[0] + 1; i++) {
//            cout << adjMatrix_row_ptr[i] << " ";
//        }
//        cout<<endl;
        sizes[1] = read_long_array(my_file, adjMatrix_col_ind);
//                cout << "\nadjMatrix_col_ind= " << sizes[1] << endl;
//        for (int i = 0; i < sizes[1]; i++) {
//            cout << adjMatrix_col_ind[i] << " ";
//        }
//        cout << endl;

        read_double_array(my_file, adjMatrix_areaValues);
//        for (int i = 0; i < sizes[1]; i++) {
//            cout << adjMatrix_areaValues[i] << " ";
//        }
//        cout << endl;

//                cout << "sizes[1]= " << sizes[1] << endl;
//        cout << "adjMatrix_areaValues"<<endl;
//        for (int i = 0; i < sizes[1]; i++) {
//            cout << adjMatrix_areaValues[i] << " ";
//        }
//        cout << endl;

        read_double_array(my_file, volumes);
//        cout << "volumes"<<endl;
//        for (int i = 0; i < sizes[0]; i++) {
//            cout << volumes[i] << " ";
//        }
//        cout << endl;

        sizes[7] = read_long_array(my_file, arrayOfFineAnisotropicCompliantCells);

        read_long_array(my_file, isOnFineBnd_l);
//        for (int i = 0; i < sizes[0]; i++) {
//            cout << isOnFineBnd_l[i] << " ";
//        }
//        cout<<endl;

        sizes[4] = read_long_array(my_file, array_isOnValley);
//        cout << "array_isOnValley "<<sizes[7]<<":";
//        for (int i = 0; i < sizes[7]; i++) {
//            cout << array_isOnValley[i] << " ";
//        }
        cout << endl;
        sizes[5] = read_long_array(my_file, array_isOnRidge);
        sizes[6] = read_long_array(my_file, array_isOnCorner);
        isFirstAgglomeration_long[0] = read_long(my_file);
        isAnisotropic_long[0] = read_long(my_file);

        sizes[8] = read_long_array(my_file, agglomerationLines_Idx);
        sizes[9] = read_long_array(my_file, agglomerationLines);

        dimension[0] = read_long(my_file);
//        cout<<"dimension[0]"<<dimension[0]<<endl;
        goalCard[0] = read_long(my_file);
//        cout<<"goalCard[0]"<<goalCard[0]<<endl;
        minCard[0] = read_long(my_file);
//        cout<<"minCard[0]"<<minCard[0]<<endl;
        maxCard[0] = read_long(my_file);
//        cout<<"maxCard[0]"<<maxCard[0]<<endl;


        my_file.close();   //close the file object.
    } else {
        cout << "Fail to read file " << filename << endl;
    }
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//
//    // Rmk: sizes[2] ==indCoarseCell
//    long numberOfFineAgglomeratedCells = sizes[3];
//    long isOnValley_size = sizes[4];
//    long isOnRidge_size = sizes[5];
//    long isOnCorner_size = sizes[6];
//    long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
//    long agglomerationLines_Idx_size = sizes[8];
//    long agglomerationLines_size = sizes[9];
//
//    bool isFirstAgglomeration = isFirstAgglomeration_long == 1;
//    bool isAnisotropic = isAnisotropic_long == 1;

//    myfile << "adjMatrix_row_ptr; ";
//    myfile << to_string(numberOfFineCells) + "; ";
//    for (int i = 0; i < numberOfFineCells; i++) {
//        myfile << to_string(adjMatrix_row_ptr[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "adjMatrix_col_ind; ";
//    myfile << to_string(adjMatrix_col_ind_size) + "; ";
//    for (int i = 0; i < adjMatrix_col_ind_size; i++) {
//        myfile << to_string(adjMatrix_col_ind[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "adjMatrix_areaValues; ";
//    myfile << to_string(adjMatrix_areaValues_size) + "; ";
//    for (int i = 0; i < adjMatrix_areaValues_size; i++) {
//        myfile << to_string(adjMatrix_areaValues[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "volumes; ";
//    myfile << to_string(numberOfFineCells) + "; ";
//    for (int i = 0; i < numberOfFineCells; i++) {
//        myfile << to_string(volumes[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "isOnFineBnd_l; ";
//    myfile << to_string(numberOfFineCells) + "; ";
//    for (int i = 0; i < numberOfFineCells; i++) {
//        myfile << to_string(isOnFineBnd_l[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "array_isOnValley; ";
//    myfile << to_string(isOnValley_size) + "; ";
//    for (int i = 0; i < isOnValley_size; i++) {
//        myfile << to_string(array_isOnValley[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "array_isOnRidge; ";
//    myfile << to_string(isOnRidge_size) + "; ";
//    for (int i = 0; i < isOnRidge_size; i++) {
//        myfile << to_string(array_isOnRidge[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "array_isOnCorner; ";
//    myfile << to_string(isOnCorner_size) + "; ";
//    for (int i = 0; i < isOnCorner_size; i++) {
//        myfile << to_string(array_isOnCorner[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "isFirstAgglomeration_long; ";
//    myfile << to_string(isFirstAgglomeration_long) + "; ";
//    myfile << endl;
//
//    myfile << "isAnisotropic_long; ";
//    myfile << to_string(isAnisotropic_long) + "; ";
//    myfile << endl;
//
//    myfile << "agglomerationLines_Idx; ";
//    myfile << to_string(agglomerationLines_Idx_size) + "; ";
//    for (int i = 0; i < agglomerationLines_Idx_size; i++) {
//        myfile << to_string(agglomerationLines_Idx[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "agglomerationLines; ";
//    myfile << to_string(agglomerationLines_size) + "; ";
//    for (int i = 0; i < agglomerationLines_size; i++) {
//        myfile << to_string(agglomerationLines[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "dimension; ";
//    myfile << to_string(dimension) + "; ";
//    myfile << endl;
//
//    myfile << "goalCard; ";
//    myfile << to_string(goalCard) + "; ";
//    myfile << endl;
//
//    myfile << "minCard; ";
//    myfile << to_string(minCard) + "; ";
//    myfile << endl;
//
//    myfile << "maxCard; ";
//    myfile << to_string(maxCard) + "; ";
//    myfile << endl;
//
//    myfile.close();
}

long read_long_array(std::fstream &stream,
                     long *&array) {

    string tp;

    // 16 entries
    char delim = ';';
    getline(stream, tp, delim);
    tp.erase(std::remove(tp.begin(), tp.end(), '\n'), tp.end());
//    cout <<"\nName="<< tp << "\t";
    getline(stream, tp, delim);
    int size = std::stoi(tp);
    array = new long[size];
    for (int j = 0; j < size; j++) {
        getline(stream, tp, delim);
        array[j] = std::stoi(tp);
    }
    return size;
}

long read_double_array(std::fstream &stream,
                       double *&array) {

    string tp;

    char delim = ';';
    getline(stream, tp, delim);
    tp.erase(std::remove(tp.begin(), tp.end(), '\n'), tp.end());
    getline(stream, tp, delim);
    int size = std::stoi(tp);
    array = new double[size];
    for (int j = 0; j < size; j++) {
        getline(stream, tp, delim);
        array[j] = std::stod(tp);
    }

    return size;
}

long read_long(std::fstream &stream) {

    string tp;
    char delim = ';';
    getline(stream, tp, delim);
    tp.erase(std::remove(tp.begin(), tp.end(), '\n'), tp.end());

    getline(stream, tp, delim);
    getline(stream, tp, delim);

    return std::stoi(tp);
}