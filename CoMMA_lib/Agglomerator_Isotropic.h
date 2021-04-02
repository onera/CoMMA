//
// Created by Nicolas Lantos on 18/10/2017.
//

#ifndef AGGLOMERATOR_ISOTROPIC_H
#define AGGLOMERATOR_ISOTROPIC_H

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <string>

using namespace std;

void agglomerateOneLevel(long *sizes,

                         long *adjMatrix_row_ptr,
                         long *adjMatrix_col_ind,
                         double *adjMatrix_areaValues,
                         double *volumes,

                         long *arrayOfFineAnisotropicCompliantCells,

                         long *isOnFineBnd,
                         long *array_isOnValley,
                         long *array_isOnRidge,
                         long *array_isOnCorner,

                         long isFirstAgglomeration,
                         long isAnisotropic,

                         long *fineCellToCoarseCell,

                         long *agglomerationLines_Idx,
                         long *agglomerationLines,

                         long dimension = 3,
                         long goalCard = -1,
                         long minCard = -1,
                         long maxCard = -3,
                         long checks = 0,
                         long verbose = 0);


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
                                         bool verbose);

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
                                      int maxCard);

void agglomerate_Isotropic_CreateDelayedCoarseCells(unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                                    unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                    list<unordered_set<long>> delayedCoarseCells,
                                                    long &indCoarseCell,
                                                    long *fineCellToCoarseCell);


unordered_map<long, queue<long> *> findSeedViaFrontalMethod(int numberOfInts, long *sizes,
                                                            vector<long> listOfFineCells,
                                                            long *matrixAdj_CRS_row_ptr,
                                                            long *matrixAdj_CRS_col_ind);

int computeDistanceFromSeedTofirstVertexOfDegree2(long seed, unordered_map<long, queue<long> *> dict_Covering_Tree);

unordered_map<long, int> computation_Of_Neighbourhood(long seed, int numberOfOrderOfNeighbourhood,
                                                      long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind,
                                                      int maxCard,
                                                      bool *isFineCellAgglomerated_tmp,
                                                      unordered_set<long> *setOfFineCells = nullptr);

int computeNumberOfCommonFaces(long iFine, long iCoarse,
                               long *matrixAdj_CRS_row_ptr,
                               long *matrixAdj_CRS_col_ind,
                               long *fine_Cell_indices_To_Coarse_Cell_Indices);

int removeSeparatingVertex(long seed, unordered_map<long, queue<long> *> dict_ConnectivityTree,
                           unordered_set<long> &setOfFineCells,
                           long *matrixAdj_CRS_row_ptr,
                           long *matrixAdj_CRS_col_ind, long verbose = 0);

//bool check_connectivity(vector<long> listFineCells, long* matrixAdj_CRS_row_ptr, long* matrixAdj_CRS_col_ind, int verbose = 0);
bool checkConnectivity_w_set(unordered_set<long> listFineCells, long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind, int verbose = 0);


list<unordered_set<long>> partsList(vector<long> seq, int length = 0);

unordered_set<long> swapFineCell(long iFineCell, long iOrigineCoarseCell, long iDestinationCoarseCell,
                                 unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                 unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                 unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                 long *fineCellToCoarseCell);


void splitNonConnectedCoarseCell(long &indCoarseElement,
                                 long &numberOfFineAgglomeratedCells,
                                 long &iCoarseCell,
                                 unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                 unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                 unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                 long *matrixAdj_CRS_row_ptr,
                                 long *matrixAdj_CRS_col_ind,
                                 bool *isFineCellAgglomerated,
                                 long *fine_Cell_indices_To_Coarse_Cell_Indices);

list<unordered_set<long>> computeConnectedComponent(
        unordered_set<long> listInitialCoarseCell, long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind);

void createCoarseCell(unordered_set<long> l,
                      unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                      unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                      unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                      long &indCoarseElement,
                      long &numberOfFineAgglomeratedCells_tmp,
                      bool *isFineCellAgglomerated_tmp,
                      long *Fine_Cell_indices_To_Coarse_Cell_Indices,
                      bool isMutable = true,
                      bool isCreationDelayed = false);

void createADelayedCoarseCell(unordered_set<long> l,
                              unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                              unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                              long &indCoarseElement,
                              long *Fine_Cell_indices_To_Coarse_Cell_Indices);

void removeDeletedCoarseCells_v3(unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                 unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                 long *fine_Cell_indices_To_Coarse_Cell_Indices,
                                 unordered_set<long> set_removedCoarseCells,
                                 long &numberOfCoarseCells);

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
                                                 bool isOrderPrimary = false);

void agglomerate_Isotropic_createCoarseCell(unordered_set<long> l,
                                            unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                            unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                            unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                            long &indCoarseCell,
                                            long &numberOfFineAgglomeratedCells,
                                            bool *isFineCellAgglomerated,
                                            long *fineCellToCoarseCell,
                                            list<unordered_set<long>> &delayedCoarseCells,
                                            bool isMutable = true,
                                            bool isCreationDelayed = false);

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
                                                        int maxCard);

long agglomerate_Isotropic_Choice_Of_Seed(vector<queue<long>> &listOfSeeds,
                                          long numberOfFineCells,
                                          const bool *isFineCellAgglomerated,
                                          unordered_set<long> isOnRidge,
                                          unordered_set<long> isOnValley);

void remove_Too_Small_Cells_v2(int thresholdCard,
                               long *fineCellIndicesToCoarseCellIndices,
                               long &indCoarseCell,
                               long *matrixAdj_CRS_row_ptr,
                               long *matrixAdj_CRS_col_ind,
                               double *matrixAdj_CRS_values,
                               unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                               unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                               unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements);

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
                         bool verbose);

void agglomerate_Isotropic_Correction_Swap(unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                           unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                           long *matrixAdj_CRS_row_ptr,
                                           long *matrixAdj_CRS_col_ind,
                                           unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                           long &indCoarseCell,
                                           long numberOfFineCells,
                                           long *fineCellToCoarseCell,
                                           bool verbose);

void agglomerate_Isotropic_Correction_Too_Big_Cells(unordered_map<long, unordered_set<long>> &dict_Coarse_Elem,
                                                    unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                    long *matrixAdj_CRS_row_ptr,
                                                    long *matrixAdj_CRS_col_ind,
                                                    unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements,
                                                    long *fineCellToCoarseCell,
                                                    long &indCoarseCell,
                                                    int goalCard,
                                                    bool verbose);

void compute_Dicts_From_FineCellIndicesToCoarseCellIndices(long nbOfFineCells,
                                                           long *fineCellIndicesToCoarseCellIndices,
                                                           unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                                           unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                           unordered_map<int, long> &dict_DistributionOfCardinalOfCoarseElements);

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
                                                                 bool verbose);


void agglomerate_Isotropic_CheckConsistancyDictCoarseCells(unordered_map<long, unordered_set<long>> &dict_Coarse_Cells,
                                                           unordered_map<int, unordered_set<long>> &dict_Card_Coarse_Cells,
                                                           long fineCellIndicesToCoarseCellIndices_size,
                                                           long *fineCellIndicesToCoarseCellIndices);

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
                                                  bool verbose);

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
                               long verbose_long);

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
                                        );

long read_long_array(std::fstream &stream, long*&array);
long read_double_array(std::fstream &stream,
                       double *&array);

long read_long(std::fstream &stream);


#endif AGGLOMERATOR_ISOTROPIC_H
