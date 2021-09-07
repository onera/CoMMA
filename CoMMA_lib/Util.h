//
// Created by Nicolas Lantos on 06/04/2021.
//

#ifndef COMMA_PROJECT_UTIL_H
#define COMMA_PROJECT_UTIL_H

#include <assert.h>
#include <deque>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <forward_list>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>


using namespace std;

void convert_agglomeration_lines_arrays_to_agglomeration_lines(
        const unsigned long fineAgglomerationLines_array_Idx_size,
        const unsigned long fineAgglomerationLines_array_size,
        vector<long> &fineAgglomerationLines_array_Idx,
        vector<long> &fineAgglomerationLines_array,
        long &nb_agglomeration_lines,
        forward_list<deque<long> *> &agglomeration_lines);

forward_list<deque < long> *> convertFineAgglomerationLines_Arrays_to_agglomeration_lines(
        unsigned long fineAgglomerationLines_array_Idx_size,
        unsigned long fineAgglomerationLines_array_size,
        long *fineAgglomerationLines_array_Idx,
        long *fineAgglomerationLines_array);

forward_list<deque < long> > convertFineAgglomerationLines_arraysToFineAgglomerationLines_without_pointers(
        unsigned long fineAgglomerationLines_array_Idx_size,
        unsigned long fineAgglomerationLines_array_size,
        long *fineAgglomerationLines_array_Idx,
        long *fineAgglomerationLines_array);
forward_list<int> test();

void convert_agglo_lines_to_agglomeration_lines_arrays(const long nb_lines,
                                                       forward_list<deque<long> *> const &agglo_lines,
                                                       long *sizes,
                                                       vector<long> &agglo_lines_array_idx,
                                                       vector<long> &agglo_lines_array);

list<unordered_set<long>> parts_list(vector<long> seq, int length);

inline unordered_set<long> d_keys_to_set(const unordered_map<long, unsigned short> &dict) {
    unordered_set<long> s_neighbours_of_seed = {};
    for (const auto &i_k_v : dict) {
        s_neighbours_of_seed.insert(i_k_v.first);
    }
    return s_neighbours_of_seed;
}

void print_queue(queue<long> q);

void print_agglomeration_lines(forward_list<deque<long> *> lines);

forward_list<deque<long> *> copy_agglomeration_lines(forward_list<deque<long> *>);

void clean_agglomeration_lines(forward_list<deque<long> *>);

bool compare_maps_keys(const unordered_map<long, unsigned short> &m_1, const unordered_map<long, unsigned short> &m_2);

list<unordered_set<long>> partsList(vector<long> seq, int length = 0);

bool checkConnectivity_w_set(unordered_set<long> listFineCells, long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind, int verbose = 0);

int computeNumberOfCommonFaces(long iFine, long iCoarse,
                               long *matrixAdj_CRS_row_ptr,
                               long *matrixAdj_CRS_col_ind,
                               long *fine_Cell_indices_To_Coarse_Cell_Indices);

void store_agglomeration_datas(long *sizes,
        // Dual graph:
                           const vector<long> &adjMatrix_row_ptr,
                           const vector<long> &adjMatrix_col_ind,
                           const vector<double> &adjMatrix_areaValues,
                           const vector<double> &volumes,


                           vector<long> &arrayOfFineAnisotropicCompliantCells,

                           const vector<long> &isOnFineBnd,
                           vector<long> &array_is_on_valley,
                           vector<long> &array_is_on_ridge,
                           vector<long> &array_is_on_corner,

        // Agglomeration argument
                           long isFirstAgglomeration_long,

                           long is_anisotropic_long,


                           vector<long> fineCellToCoarseCell,
                       
                           vector<long> &agglomerationLines_Idx,  // In & out
                           vector<long> &agglomerationLines,  // In & out



                               long dimension,
                               long goalCard,
                               long minCard,
                               long maxCard,
                               long checks_long,
                               long verbose_long); 


void read_agglomeration_datas_from_file(std::string filename,
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

long read_long_array(std::fstream &stream, long *&array);

long read_double_array(std::fstream &stream,
                       double *&array);

long read_long(std::fstream &stream);

#endif //COMMA_PROJECT_UTIL_H
