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
class DualGPy_aniso;
class DualGPy_minimal;

class DualGPy {

    public:
    // Variables definitiion from DualGPy configuration you can find in the README
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 2,  5,  9, 13, 17, 21, 24, 28, 32, 35, 38, 41, 44, 46, 49, 52, 54, 56, 59, 62, 64};
   
    vector<CoMMAIndexT> adjMatrix_col_ind ={ 1,  9,  0,  2,  6,  1,  3,  5,  8,  2,  4,  7, 11,  3,  5, 12, 14,  2,  4,  6, 15,  1,  5, 16, 3,  8, 10, 19,  2,  7,  9, 18,  0,  8, 17,  7, 11, 20,  3, 10, 12,  4, 11, 13, 12, 14,  4, 13,15,  5, 14, 16,  6, 15,  9, 18,  8, 17, 19,  7, 18, 20, 10, 19};
    vector<CoMMAIndexT> isOnFineBnd ={1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues ={1.41421356, 1,1,1,  1 ,1,1,   1      ,       1,         1,         1,         1,1,         1,         1,         1,         1,         1,1,        1,         1,         1,         1,         1,1,         1,         1,         1,         1,         1,1,         1};
    vector<CoMMAWeightT> volumes ={0.5, 0.5, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1};
  // number of faces
    long nb_fc = static_cast<long>(adjMatrix_row_ptr.size()-1);
    long adj_matrix_row_ptr_size = nb_fc + 1;
    long adj_matrix_col_ind_size = static_cast<long>(adjMatrix_col_ind.size());
    long adj_matrix_areas_size = static_cast<long>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, int> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;
    vector<bool> a_is_fc_agglomerated;
    DualGPy(){
    for (int i = 0; i < nb_fc; i++) {
	    if (isOnFineBnd[i] > 0){
	    	fill_value<true,int,const long>(d_is_on_bnd[i],isOnFineBnd[i]);		
    }
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


class DualGPy_aniso {

    public:
    // Variables definitiion from DualGPy configuration you can find in the README (NOT YET IMPLEMENTED)
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 2,  5,  8, 10, 13, 17, 21, 24, 27, 31, 35, 38, 40, 43, 46, 48};
   
    vector<CoMMAIndexT> adjMatrix_col_ind ={ 1,  4,  0,  2,  5,  1,  3,  6,  2,  7,  0,  5,  8,  1,  4,  6,  9,  2,  5,  7, 10,  3,  6, 11, 4,  9, 12,  5,  8, 10, 13,  6,  9, 11, 14,  7, 10, 15,  8, 13,  9, 12, 14, 10, 13, 15, 11, 14};
    vector<CoMMAIndexT> isOnFineBnd ={2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues ={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 3, 1, 1, 3, 3, 1, 1, 3, 1, 1, 5, 1, 1, 5, 5, 1, 1, 5, 5, 1, 1, 5, 1, 1, 7, 1, 7, 7, 1, 7, 7, 1, 7 };
    vector<CoMMAWeightT> volumes ={1, 1, 1, 1, 3, 3, 3, 3, 5, 5, 5, 5, 7, 7, 7, 7};
  // number of faces
    long nb_fc = static_cast<long>(adjMatrix_row_ptr.size()-1);
    long adj_matrix_row_ptr_size = nb_fc + 1;
    long adj_matrix_col_ind_size = static_cast<long>(adjMatrix_col_ind.size());
    long adj_matrix_areas_size = static_cast<long>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, int> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_aniso(){
    for (int i = 0; i < nb_fc; i++) {
	    if (isOnFineBnd[i] > 0){
	    	fill_value<true,int,const long>(d_is_on_bnd[i],isOnFineBnd[i]);		
    }
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

class DualGPy_minimal {

    public:
    // Variables definitiion from DualGPy configuration you can find in the README
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 2, 5, 7, 10, 14, 17, 19, 22, 24};
   
    vector<CoMMAIndexT> adjMatrix_col_ind ={1, 3, 0, 2, 4, 1, 5, 0, 4, 6, 1, 3, 5, 7, 2, 4, 8, 3, 7, 4, 6, 8, 5, 7};
    vector<CoMMAIndexT> isOnFineBnd ={2, 1, 2, 1, 0, 1, 2, 1, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues ={1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    vector<CoMMAWeightT> volumes ={1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  // number of faces
    long nb_fc = static_cast<long>(adjMatrix_row_ptr.size()-1);
    long adj_matrix_row_ptr_size = nb_fc + 1;
    long adj_matrix_col_ind_size = static_cast<long>(adjMatrix_col_ind.size());
    long adj_matrix_areas_size = static_cast<long>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, int> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_minimal(){
    for (int i = 0; i < nb_fc; i++) {
	    if (isOnFineBnd[i] > 0){
	    	fill_value<true,int,const long>(d_is_on_bnd[i],isOnFineBnd[i]);		
    }
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

class DualGPy_quad_4 {

    public:
    // Variables definitiion from DualGPy configuration you can find in the README
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 2, 5, 8, 10, 13, 17, 21, 24, 27, 31, 35, 38, 40, 43, 46, 48};
   
    vector<CoMMAIndexT> adjMatrix_col_ind ={1, 4, 0, 2, 5, 1, 3, 6, 2, 7, 0, 5, 8, 1, 4, 6, 9, 2, 5, 7, 10, 3, 6, 11, 4, 9, 12, 5, 8, 10, 13, 6, 9, 11, 14, 7, 10, 15, 8, 13, 9, 12, 14, 10, 13, 15, 11, 14};
    vector<CoMMAIndexT> isOnFineBnd ={2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues ={1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    vector<CoMMAWeightT> volumes ={1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  // number of faces
    long nb_fc = static_cast<long>(adjMatrix_row_ptr.size()-1);
    long adj_matrix_row_ptr_size = nb_fc + 1;
    long adj_matrix_col_ind_size = static_cast<long>(adjMatrix_col_ind.size());
    long adj_matrix_areas_size = static_cast<long>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, int> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_quad_4(){
    for (int i = 0; i < nb_fc; i++) {
	    if (isOnFineBnd[i] > 0){
	    	fill_value<true,int,const long>(d_is_on_bnd[i],isOnFineBnd[i]);		
    }
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

class DualGPy_cube_4 {

    public:
    // Variables definitiion from DualGPy configuration you can find in the README
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 3, 7, 11, 14, 18, 23, 28, 32, 36, 41, 46, 50, 53, 57, 61, 64, 68, 73, 78, 82, 87, 93, 99, 104, 109, 115, 121, 126, 130, 135, 140, 144, 148, 153, 158, 162, 167, 173, 179, 184, 189, 195, 201, 206, 210, 215, 220, 224, 227, 231, 235, 238, 242, 247, 252, 256, 260, 265, 270, 274, 277, 281, 285, 288};
   
    vector<CoMMAIndexT> adjMatrix_col_ind ={1, 4, 16, 0, 2, 5, 17, 1, 3, 6, 18, 2, 7, 19, 0, 5, 8, 20, 1, 4, 6, 9, 21, 2, 5, 7, 10, 22, 3, 6, 11, 23, 4, 9, 12, 24, 5, 8, 10, 13, 25, 6, 9, 11, 14, 26, 7, 10, 15, 27, 8, 13, 28, 9, 12, 14, 29, 10, 13, 15, 30, 11, 14, 31, 0, 17, 20, 32, 1, 16, 18, 21, 33, 2, 17, 19, 22, 34, 3, 18, 23, 35, 4, 16, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26, 38, 7, 19, 22, 27, 39, 8, 20, 25, 28, 40, 9, 21, 24, 26, 29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 31, 43, 12, 24, 29, 44, 13, 25, 28, 30, 45, 14, 26, 29, 31, 46, 15, 27, 30, 47, 16, 33, 36, 48, 17, 32, 34, 37, 49, 18, 33, 35, 38, 50, 19, 34, 39, 51, 20, 32, 37, 40, 52, 21, 33, 36, 38, 41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 43, 55, 24, 36, 41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39, 42, 47, 59, 28, 40, 45, 60, 29, 41, 44, 46, 61, 30, 42, 45, 47, 62, 31, 43, 46, 63, 32, 49, 52, 33, 48, 50, 53, 34, 49, 51, 54, 35, 50, 55, 36, 48, 53, 56, 37, 49, 52, 54, 57, 38, 50, 53, 55, 58, 39, 51, 54, 59, 40, 52, 57, 60, 41, 53, 56, 58, 61, 42, 54, 57, 59, 62, 43, 55, 58, 63, 44, 56, 61, 45, 57, 60, 62, 46, 58, 61, 63, 47, 59, 62};
    vector<CoMMAIndexT> isOnFineBnd ={3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3, 2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2, 3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
    vector<CoMMAWeightT> adjMatrix_areaValues ={1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    vector<CoMMAWeightT> volumes ={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  // number of faces
    long nb_fc = static_cast<long>(adjMatrix_row_ptr.size()-1);
    long adj_matrix_row_ptr_size = nb_fc + 1;
    long adj_matrix_col_ind_size = static_cast<long>(adjMatrix_col_ind.size());
    long adj_matrix_areas_size = static_cast<long>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, int> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_cube_4(){
    for (int i = 0; i < nb_fc; i++) {
	    if (isOnFineBnd[i] > 0){
	    	fill_value<true,int,const long>(d_is_on_bnd[i],isOnFineBnd[i]);		
    }
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
 
