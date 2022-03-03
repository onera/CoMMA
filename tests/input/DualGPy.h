#include <memory>
#include "CoMMATypes.h"
#include "Agglomerator.h"
#include "Tree.h"
#include "CoMMA.h"
#include "templateHelpers.h"
#include "Bimap.h"

#ifndef DUALGPY_TEST_H
#define DUALGPY_TEST_H


class DualGPy;
class DualGPy_5;

class DualGPy {

    public:
    // Variables definitiion from DualGPy configuration you can find here:
    // https://github.com/albiremo/dualGPy
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 2,  5,  9, 13, 17, 21, 24, 28, 32, 35, 38, 41, 44, 46, 49, 52, 54, 56, 59, 62, 64};
   
    vector<CoMMAIndexT> adjMatrix_col_ind ={ 1,  9,  0,  2,  6,  1,  3,  5,  8,  2,  4,  7, 11,  3,  5, 12, 14,  2,  4,  6, 15,  1,  5, 16, 3,  8, 10, 19,  2,  7,  9, 18,  0,  8, 17,  7, 11, 20,  3, 10, 12,  4, 11, 13, 12, 14,  4, 13,15,  5, 14, 16,  6, 15,  9, 18,  8, 17, 19,  7, 18, 20, 10, 19};
    vector<CoMMAIndexT> isOnFineBnd ={1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2};
    vector<CoMMAIndexT> array_is_on_ridge ={13, 16, 17, 20};
    vector<CoMMAIndexT> array_is_on_valley ={0,  6,  9, 10, 11, 12, 14, 15, 18, 19};
    vector<CoMMAIndexT> array_is_on_corner; 
    vector<CoMMAWeightT> adjMatrix_areaValues ={1.41421356, 1,1,1,  1 ,1,1,   1      ,       1,         1,         1,         1,1,         1,         1,         1,         1,         1,1,        1,         1,         1,         1,         1,1,         1,         1,         1,         1,         1,1,         1};
    vector<CoMMAWeightT> volumes ={0.5, 0.5, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1};
  // number of faces
    long nb_fc = static_cast<long>(adjMatrix_row_ptr.size()-1);
    long adj_matrix_row_ptr_size = nb_fc + 1;
    long adj_matrix_col_ind_size = static_cast<long>(adjMatrix_col_ind.size());
    long adj_matrix_areas_size = static_cast<long>(adjMatrix_col_ind.size());
    long is_on_valley_size = static_cast<long>(array_is_on_valley.size());
    long is_on_ridge_size = static_cast<long>(array_is_on_ridge.size());
    long is_on_corner_size = static_cast<long>(array_is_on_corner.size());
    unordered_map<CoMMAIndexT, int> d_is_on_bnd;
    unordered_set<CoMMAIndexT> s_is_on_valley, s_is_on_ridge, s_is_on_corner;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;
    vector<bool> a_is_fc_agglomerated;
    DualGPy(){
    for (int i = 0; i < nb_fc; i++) {
	    if (isOnFineBnd[i] > 0){
	    	fill_value<true,int,const long>(d_is_on_bnd[i],isOnFineBnd[i]);		
    }
    }
    for (long i_o_v = 0; i_o_v < is_on_valley_size; i_o_v++) {
        s_is_on_valley.insert(array_is_on_valley[i_o_v]);
    }
    for (long i_o_r = 0; i_o_r < is_on_ridge_size; i_o_r++) {
        s_is_on_ridge.insert(array_is_on_ridge[i_o_r]);
    }
    for (long iOC = 0; iOC < is_on_corner_size; iOC++) {
        s_is_on_corner.insert(array_is_on_corner[iOC]);
    }
    for (int i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
    for (long i_a_c_fc = 0; i_a_c_fc < nb_fc; i_a_c_fc++) {
        s_anisotropic_compliant_fc.insert(arrayOfFineAnisotropicCompliantCells[i_a_c_fc]);
    }
     vector<bool> tmp(nb_fc, false);
     a_is_fc_agglomerated = tmp;
};

};

class DualGPy_5 {

    public:
    // Variables definitiion from DualGPy configuration you can find here:
    // https://github.com/albiremo/dualGPy
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 2,  5,  9, 13, 17, 21, 24, 28, 32, 35, 38, 41, 44, 46, 49, 52, 54, 56, 59, 62, 64};
   
    vector<CoMMAIndexT> adjMatrix_col_ind ={ 1,  9,  0,  2,  6,  1,  3,  5,  8,  2,  4,  7, 11,  3,  5, 12, 14,  2,  4,  6, 15,  1,  5, 16, 3,  8, 10, 19,  2,  7,  9, 18,  0,  8, 17,  7, 11, 20,  3, 10, 12,  4, 11, 13, 12, 14,  4, 13,15,  5, 14, 16,  6, 15,  9, 18,  8, 17, 19,  7, 18, 20, 10, 19};
    vector<CoMMAIndexT> isOnFineBnd ={1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2};
    vector<CoMMAIndexT> array_is_on_ridge ={13, 16, 17, 20};
    vector<CoMMAIndexT> array_is_on_valley ={0,  6,  9, 10, 11, 12, 14, 15, 18, 19};
    vector<CoMMAIndexT> array_is_on_corner; 
    vector<CoMMAWeightT> adjMatrix_areaValues ={1.41421356, 1,1,1,  1 ,1,1,   1      ,       1,         1,         1,         1,1,         1,         1,         1,         1,         1,1,        1,         1,         1,         1,         1,1,         1,         1,         1,         1,         1,1,         1};
    vector<CoMMAWeightT> volumes ={0.5, 0.5, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1};
  // number of faces
    long nb_fc = static_cast<long>(adjMatrix_row_ptr.size()-1);
    long adj_matrix_row_ptr_size = nb_fc + 1;
    long adj_matrix_col_ind_size = static_cast<long>(adjMatrix_col_ind.size());
    long adj_matrix_areas_size = static_cast<long>(adjMatrix_col_ind.size());
    long is_on_valley_size = static_cast<long>(array_is_on_valley.size());
    long is_on_ridge_size = static_cast<long>(array_is_on_ridge.size());
    long is_on_corner_size = static_cast<long>(array_is_on_corner.size());
    unordered_map<CoMMAIndexT, int> d_is_on_bnd;
    unordered_set<CoMMAIndexT> s_is_on_valley, s_is_on_ridge, s_is_on_corner;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_5(){
    for (int i = 0; i < nb_fc; i++) {
	    if (isOnFineBnd[i] > 0){
	    	fill_value<true,int,const long>(d_is_on_bnd[i],isOnFineBnd[i]);		
    }
    }
    for (long i_o_v = 0; i_o_v < is_on_valley_size; i_o_v++) {
        s_is_on_valley.insert(array_is_on_valley[i_o_v]);
    }
    for (long i_o_r = 0; i_o_r < is_on_ridge_size; i_o_r++) {
        s_is_on_ridge.insert(array_is_on_ridge[i_o_r]);
    }
    for (long iOC = 0; iOC < is_on_corner_size; iOC++) {
        s_is_on_corner.insert(array_is_on_corner[iOC]);
    }
    for (int i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
    for (long i_a_c_fc = 0; i_a_c_fc < nb_fc; i_a_c_fc++) {
        s_anisotropic_compliant_fc.insert(arrayOfFineAnisotropicCompliantCells[i_a_c_fc]);
    }
    vector<bool> a_is_fc_agglomerated(nb_fc, false);

};
};



#endif
 
