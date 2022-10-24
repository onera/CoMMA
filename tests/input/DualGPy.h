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
class DualGPy_cube_4;
class DualGPy_quad_4;

class DualGPy {

    public:
    // Variables definitiion from DualGPy configuration you can find in the README
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 2,  5,  9, 13, 17, 21, 24, 28, 32, 35, 38, 41, 44, 46, 49, 52, 54, 56, 59, 62, 64};

    vector<CoMMAIndexT> adjMatrix_col_ind ={ 1,  9,  0,  2,  6,  1,  3,  5,  8,  2,  4,  7, 11,  3,  5, 12, 14,  2,  4,  6, 15,  1,  5, 16, 3,  8, 10, 19,  2,  7,  9, 18,  0,  8, 17,  7, 11, 20,  3, 10, 12,  4, 11, 13, 12, 14,  4, 13,15,  5, 14, 16,  6, 15,  9, 18,  8, 17, 19,  7, 18, 20, 10, 19};
    vector<CoMMAIndexT> isOnFineBnd ={1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues ={1.41421356, 1,1,1,  1 ,1,1,   1      ,       1,         1,         1,         1,1,         1,         1,         1,         1,         1,1,        1,         1,         1,         1,         1,1,         1,         1,         1,         1,         1,1,         1};
    vector<CoMMAWeightT> volumes ={0.5, 0.5, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1};
  // number of faces
    CoMMAIndexT nb_fc = static_cast<CoMMAIndexT>(adjMatrix_row_ptr.size()-1);
    CoMMAIndexT adj_matrix_row_ptr_size = nb_fc + 1;
    CoMMAIndexT adj_matrix_col_ind_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    CoMMAIndexT adj_matrix_areas_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, CoMMAIntT> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;
    vector<bool> a_is_fc_agglomerated;
    DualGPy(){
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
      if (isOnFineBnd[i] > 0){
        fill_value<true,CoMMAIntT,const CoMMAIndexT>(d_is_on_bnd[i],isOnFineBnd[i]);
    }
    }
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
    for (CoMMAIndexT i_a_c_fc = 0; i_a_c_fc < nb_fc; i_a_c_fc++) {
        s_anisotropic_compliant_fc.insert(arrayOfFineAnisotropicCompliantCells[i_a_c_fc]);
    }
     vector<bool> tmp(nb_fc, false);
     a_is_fc_agglomerated = tmp;
};
};


class DualGPy_aniso {

    public:
    // Variables definitiion from DualGPy configuration you can find in the README (NOT YET IMPLEMENTED)
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0, 3, 7, 11, 15, 18, 22, 27, 32, 37, 41, 45, 50, 55, 60, 64, 68, 73, 78, 83, 87, 90, 94, 98, 102, 105, 109, 114, 119, 124, 128, 133, 139, 145, 151, 156, 161, 167, 173, 179, 184, 189, 195, 201, 207, 212, 216, 221, 226, 231, 235, 239, 244, 249, 254, 258, 263, 269, 275, 281, 286, 291, 297, 303, 309, 314, 319, 325, 331, 337, 342, 346, 351, 356, 361, 365, 369, 374, 379, 384, 388, 393, 399, 405, 411, 416, 421, 427, 433, 439, 444, 449, 455, 461, 467, 472, 476, 481, 486, 491, 495, 498, 502, 506, 510, 513, 517, 522, 527, 532, 536, 540, 545, 550, 555, 559, 563, 568, 573, 578, 582, 585, 589, 593, 597, 600};

    vector<CoMMAIndexT> adjMatrix_col_ind ={1, 5, 25, 0, 2, 6, 26, 1, 3, 7, 27, 2, 4, 8, 28, 3, 9, 29, 0, 6, 10, 30, 1, 5, 7, 11, 31, 2, 6, 8, 12, 32, 3, 7, 9, 13, 33, 4, 8, 14, 34, 5, 11, 15, 35, 6, 10, 12, 16, 36, 7, 11, 13, 17, 37, 8, 12, 14, 18, 38, 9, 13, 19, 39, 10, 16, 20, 40, 11, 15, 17, 21, 41, 12, 16, 18, 22, 42, 13, 17, 19, 23, 43, 14, 18, 24, 44, 15, 21, 45, 16, 20, 22, 46, 17, 21, 23, 47, 18, 22, 24, 48, 19, 23, 49, 0, 26, 30, 50, 1, 25, 27, 31, 51, 2, 26, 28, 32, 52, 3, 27, 29, 33, 53, 4, 28, 34, 54, 5, 25, 31, 35, 55, 6, 26, 30, 32, 36, 56, 7, 27, 31, 33, 37, 57, 8, 28, 32, 34, 38, 58, 9, 29, 33, 39, 59, 10, 30, 36, 40, 60, 11, 31, 35, 37, 41, 61, 12, 32, 36, 38, 42, 62, 13, 33, 37, 39, 43, 63, 14, 34, 38, 44, 64, 15, 35, 41, 45, 65, 16, 36, 40, 42, 46, 66, 17, 37, 41, 43, 47, 67, 18, 38, 42, 44, 48, 68, 19, 39, 43, 49, 69, 20, 40, 46, 70, 21, 41, 45, 47, 71, 22, 42, 46, 48, 72, 23, 43, 47, 49, 73, 24, 44, 48, 74, 25, 51, 55, 75, 26, 50, 52, 56, 76, 27, 51, 53, 57, 77, 28, 52, 54, 58, 78, 29, 53, 59, 79, 30, 50, 56, 60, 80, 31, 51, 55, 57, 61, 81, 32, 52, 56, 58, 62, 82, 33, 53, 57, 59, 63, 83, 34, 54, 58, 64, 84, 35, 55, 61, 65, 85, 36, 56, 60, 62, 66, 86, 37, 57, 61, 63, 67, 87, 38, 58, 62, 64, 68, 88, 39, 59, 63, 69, 89, 40, 60, 66, 70, 90, 41, 61, 65, 67, 71, 91, 42, 62, 66, 68, 72, 92, 43, 63, 67, 69, 73, 93, 44, 64, 68, 74, 94, 45, 65, 71, 95, 46, 66, 70, 72, 96, 47, 67, 71, 73, 97, 48, 68, 72, 74, 98, 49, 69, 73, 99, 50, 76, 80, 100, 51, 75, 77, 81, 101, 52, 76, 78, 82, 102, 53, 77, 79, 83, 103, 54, 78, 84, 104, 55, 75, 81, 85, 105, 56, 76, 80, 82, 86, 106, 57, 77, 81, 83, 87, 107, 58, 78, 82, 84, 88, 108, 59, 79, 83, 89, 109, 60, 80, 86, 90, 110, 61, 81, 85, 87, 91, 111, 62, 82, 86, 88, 92, 112, 63, 83, 87, 89, 93, 113, 64, 84, 88, 94, 114, 65, 85, 91, 95, 115, 66, 86, 90, 92, 96, 116, 67, 87, 91, 93, 97, 117, 68, 88, 92, 94, 98, 118, 69, 89, 93, 99, 119, 70, 90, 96, 120, 71, 91, 95, 97, 121, 72, 92, 96, 98, 122, 73, 93, 97, 99, 123, 74, 94, 98, 124, 75, 101, 105, 76, 100, 102, 106, 77, 101, 103, 107, 78, 102, 104, 108, 79, 103, 109, 80, 100, 106, 110, 81, 101, 105, 107, 111, 82, 102, 106, 108, 112, 83, 103, 107, 109, 113, 84, 104, 108, 114, 85, 105, 111, 115, 86, 106, 110, 112, 116, 87, 107, 111, 113, 117, 88, 108, 112, 114, 118, 89, 109, 113, 119, 90, 110, 116, 120, 91, 111, 115, 117, 121, 92, 112, 116, 118, 122, 93, 113, 117, 119, 123, 94, 114, 118, 124, 95, 115, 121, 96, 116, 120, 122, 97, 117, 121, 123, 98, 118, 122, 124, 99, 119, 123 };
    vector<CoMMAIndexT> isOnFineBnd ={3, 2, 2, 2, 3, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 3, 2, 2, 2, 3, 2, 1, 1, 1, 2, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 2, 1, 1, 1, 2, 3, 2, 2, 2, 3, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 3, 2, 2, 2, 3 };
    vector<CoMMAWeightT> adjMatrix_areaValues ={0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 1.0, 0.05788845944824356, 0.05788845944824356, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.15735714739564882, 0.15735714739564882, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 0.42774107434374375, 0.42774107434374375, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 1.162720789674148, 1.162720789674148, 1.0, 1.0, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 1.0, 3.1606027941427888, 3.1606027941427888};
    vector<CoMMAWeightT> volumes ={0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.05788845944824356, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.15735714739564882, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 0.42774107434374375, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 1.162720789674148, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888, 3.1606027941427888};
  // number of faces
    CoMMAIndexT nb_fc = static_cast<CoMMAIndexT>(adjMatrix_row_ptr.size()-1);
    CoMMAIndexT adj_matrix_row_ptr_size = nb_fc + 1;
    CoMMAIndexT adj_matrix_col_ind_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    CoMMAIndexT adj_matrix_areas_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, CoMMAIntT> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_aniso(){
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
      if (isOnFineBnd[i] > 0){
        fill_value<true,CoMMAIntT,const CoMMAIndexT>(d_is_on_bnd[i],isOnFineBnd[i]);
    }
    }
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
    for (CoMMAIndexT i_a_c_fc = 0; i_a_c_fc < nb_fc; i_a_c_fc++) {
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
    CoMMAIndexT nb_fc = static_cast<CoMMAIndexT>(adjMatrix_row_ptr.size()-1);
    CoMMAIndexT adj_matrix_row_ptr_size = nb_fc + 1;
    CoMMAIndexT adj_matrix_col_ind_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    CoMMAIndexT adj_matrix_areas_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, CoMMAIntT> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_minimal(){
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
      if (isOnFineBnd[i] > 0){
        fill_value<true,CoMMAIntT,const CoMMAIndexT>(d_is_on_bnd[i],isOnFineBnd[i]);
    }
    }
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
    for (CoMMAIndexT i_a_c_fc = 0; i_a_c_fc < nb_fc; i_a_c_fc++) {
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
    CoMMAIndexT nb_fc = static_cast<CoMMAIndexT>(adjMatrix_row_ptr.size()-1);
    CoMMAIndexT adj_matrix_row_ptr_size = nb_fc + 1;
    CoMMAIndexT adj_matrix_col_ind_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    CoMMAIndexT adj_matrix_areas_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, CoMMAIntT> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_quad_4(){
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
      if (isOnFineBnd[i] > 0){
        fill_value<true,CoMMAIntT,const CoMMAIndexT>(d_is_on_bnd[i],isOnFineBnd[i]);
    }
    }
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
    for (CoMMAIndexT i_a_c_fc = 0; i_a_c_fc < nb_fc; i_a_c_fc++) {
        s_anisotropic_compliant_fc.insert(arrayOfFineAnisotropicCompliantCells[i_a_c_fc]);
    }
    vector<bool> a_is_fc_agglomerated(nb_fc, false);

};
};

class DualGPy_quad_7 {

    public:
    // Variables definitiion from DualGPy configuration you can find in the README
    vector<CoMMAIndexT> adjMatrix_row_ptr = {0, 2, 5, 8, 11, 14, 17, 19, 22, 26, 30, 34, 38, 42, 45, 48, 52, 56, 60, 64, 68, 71, 74, 78, 82, 86, 90, 94, 97, 100, 104, 108, 112, 116, 120, 123, 126, 130, 134, 138, 142, 146, 149, 151, 154, 157, 160, 163, 166, 168};
    vector<CoMMAIndexT> adjMatrix_col_ind = {1, 7, 0, 2, 8, 1, 3, 9, 2, 4, 10, 3, 5, 11, 4, 6, 12, 5, 13, 0, 8, 14, 1, 7, 9, 15, 2, 8, 10, 16, 3, 9, 11, 17, 4, 10, 12, 18, 5, 11, 13, 19, 6, 12, 20, 7, 15, 21, 8, 14, 16, 22, 9, 15, 17, 23, 10, 16, 18, 24, 11, 17, 19, 25, 12, 18, 20, 26, 13, 19, 27, 14, 22, 28, 15, 21, 23, 29, 16, 22, 24, 30, 17, 23, 25, 31, 18, 24, 26, 32, 19, 25, 27, 33, 20, 26, 34, 21, 29, 35, 22, 28, 30, 36, 23, 29, 31, 37, 24, 30, 32, 38, 25, 31, 33, 39, 26, 32, 34, 40, 27, 33, 41, 28, 36, 42, 29, 35, 37, 43, 30, 36, 38, 44, 31, 37, 39, 45, 32, 38, 40, 46, 33, 39, 41, 47, 34, 40, 48, 35, 43, 36, 42, 44, 37, 43, 45, 38, 44, 46, 39, 45, 47, 40, 46, 48, 41, 47};
    vector<CoMMAIndexT> isOnFineBnd = {2, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
    vector<CoMMAWeightT> volumes = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
  // number of faces
    CoMMAIndexT nb_fc = static_cast<CoMMAIndexT>(adjMatrix_row_ptr.size()-1);
    CoMMAIndexT adj_matrix_row_ptr_size = nb_fc + 1;
    CoMMAIndexT adj_matrix_col_ind_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    CoMMAIndexT adj_matrix_areas_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, CoMMAIntT> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_quad_7(){
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
      if (isOnFineBnd[i] > 0){
        fill_value<true,CoMMAIntT,const CoMMAIndexT>(d_is_on_bnd[i],isOnFineBnd[i]);
    }
    }
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
    for (CoMMAIndexT i_a_c_fc = 0; i_a_c_fc < nb_fc; i_a_c_fc++) {
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
    CoMMAIndexT nb_fc = static_cast<CoMMAIndexT>(adjMatrix_row_ptr.size()-1);
    CoMMAIndexT adj_matrix_row_ptr_size = nb_fc + 1;
    CoMMAIndexT adj_matrix_col_ind_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    CoMMAIndexT adj_matrix_areas_size = static_cast<CoMMAIndexT>(adjMatrix_col_ind.size());
    unordered_map<CoMMAIndexT, CoMMAIntT> d_is_on_bnd;
    vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells = vector<CoMMAIndexT>(nb_fc);
    unordered_set<CoMMAIndexT> s_anisotropic_compliant_fc;

    DualGPy_cube_4(){
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
      if (isOnFineBnd[i] > 0){
        fill_value<true,CoMMAIntT,const CoMMAIndexT>(d_is_on_bnd[i],isOnFineBnd[i]);
    }
    }
    for (CoMMAIndexT i = 0; i < nb_fc; i++) {
        arrayOfFineAnisotropicCompliantCells[i] = i;
    }
    for (CoMMAIndexT i_a_c_fc = 0; i_a_c_fc < nb_fc; i_a_c_fc++) {
        s_anisotropic_compliant_fc.insert(arrayOfFineAnisotropicCompliantCells[i_a_c_fc]);
    }
    vector<bool> a_is_fc_agglomerated(nb_fc, false);

};
};

#endif
