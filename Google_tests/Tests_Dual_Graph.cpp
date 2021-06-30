#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Agglomerator_anisotropic.h"
#include "../CoMMA_lib/Dual_Graph.h"

#include "MGridGen_Dual_Graph.h"
#include "Edge_7_Dual_Graph.h"
#include "Example_6_Dual_Graph.h"
#include "Example_7_Dual_Graph.h"
#include "Example_8_Dual_Graph.h"
#include "Example_9_Dual_Graph.h"
#include "Edge_8_Dual_Graph.h"
#include "Edge_9_Dual_Graph.h"
#include "Edge_9_3_Dual_Graph.h"
#include "Graphs_biconnected_Dual_Graph.h"
#include "Nine_squares_3x3_Dual_Graph.h"
#include "Box_5x5x5_Dual_Graph.h"
#include "Box_5x5x5_Aniso_Dual_Graph.h"
#include "Box_5x5x5_iso_and_Aniso_Dual_Graph.h"


#include "gtest/gtest.h"

//class box_5x5x5_iso_and_aniso : public ::testing::Test {
//    // filename_ini = "box_1_rect_5_d_F.hdf"
//    // input_directory = os.path.join((*this).cgns_path, os.path.join("0_Inputs", "0_Box"))
//
//protected:
//
//    Dual_Graph *g;
//    int box_5x5x5_number_of_cells;
//    int box_5x5x5_col_ind_size;
//    vector<long> v_box_5x5x5_row_ptr;
//    vector<long> v_box_5x5x5_col_ind;
//    vector<double> v_box_5x5x5_values;
//    vector<double> v_box_5x5x5_volumes;
//
//    virtual void SetUp() {
//        v_box_5x5x5_row_ptr = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62,
//                               66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132,
//                               138, 144, 150, 155, 161, 167, 172, 177, 183, 189, 194, 200, 206,
//                               212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278,
//                               282, 287, 293, 299, 304, 309, 315, 321, 326, 330, 335, 340, 344};
//
//        v_box_5x5x5_col_ind = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7,
//                               19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10,
//                               22, 3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25,
//                               6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28, 9, 12,
//                               13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32,
//                               1, 16, 17, 18, 21, 33, 2, 17, 18, 19, 22, 34, 3, 18, 19, 23, 35,
//                               4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26,
//                               38, 7, 19, 22, 23, 27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26,
//                               29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24, 28,
//                               29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30,
//                               31, 47, 16, 32, 33, 36, 48, 17, 32, 33, 34, 37, 49, 18, 33, 34, 35,
//                               38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38,
//                               41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40,
//                               41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39,
//                               42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42,
//                               45, 46, 47, 62, 31, 43, 46, 47, 63, 32, 48, 49, 52, 33, 48, 49, 50,
//                               53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49,
//                               52, 53, 54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52,
//                               56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58, 59, 62, 43, 55,
//                               58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63,
//                               47, 59, 62, 63};
//
//        v_box_5x5x5_values = {30., 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 30., 2.5, 25., 2.5, 27.5, 2.5,
//                              2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 27.5,
//                              2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 30.,
//                              2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25., 25.,
//                              2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
//                              25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
//                              2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5,
//                              5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 5., 25., 25., 5.,
//                              2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5,
//                              2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
//                              25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
//                              2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5,
//                              5., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25.,
//                              50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
//                              50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
//                              75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75.};
//
//        box_5x5x5_number_of_cells = 64;
//        box_5x5x5_col_ind_size = 344;
//
//        v_box_5x5x5_volumes = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
//                               125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
//                               125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
//                               125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
//                               125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
//                               125., 125., 125., 125., 125., 125., 125., 125., 125.};
//
//        int box_5x5x5_is_on_bnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3, 2, 1, 1, 2, 1, 0,
//                                       0, 1, 1, 0, 0, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1,
//                                       2, 1, 1, 2, 3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};
//
//        unordered_map<long, int> box_5x5x5_d_is_on_bnd;
//        for (int i = 0; i < box_5x5x5_number_of_cells; i++) {
//            box_5x5x5_d_is_on_bnd[i] = box_5x5x5_is_on_bnd[i];
//        }
//
//        g = new Dual_Graph(box_5x5x5_number_of_cells,
//                           v_box_5x5x5_row_ptr,
//                           v_box_5x5x5_col_ind,
//                           v_box_5x5x5_values,
//                           v_box_5x5x5_volumes,
//                           box_5x5x5_d_is_on_bnd);
//
//    }
//
//    virtual void TearDown() {
//        delete g;
////        delete v_box_5x5x5_row_ptr;
////        delete v_box_5x5x5_col_ind;
////        delete v_box_5x5x5_values;
//    }
//};

class box_2x12 : public ::testing::Test {

protected:

    Dual_Graph *g;
    int box_2x12_nb_fc;
    int box_2x12_number_of_anisotropic_compliant_fc;
    int box_2x12_col_ind_size;
    vector<long> v_box_2x12_row_ptr;
    vector<long> v_box_2x12_col_ind;
    vector<double> v_box_2x12_values;
    vector<double> v_box_2x12_volumes;

    virtual void SetUp() {
        v_box_2x12_row_ptr = {0, 3, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82, 86, 89, 92};

        v_box_2x12_col_ind = {0, 1, 2,  // 0
                              0, 1, 3,  // 1
                              0, 2, 3, 4,  // 2
                              1, 2, 3, 5,
                              2, 4, 5, 6,  // 4
                              3, 4, 5, 7,
                              4, 6, 7, 8,  // 6
                              5, 6, 7, 9,
                              6, 8, 9, 10,  // 8
                              7, 8, 9, 11,
                              8, 10, 11, 12,  // 10
                              9, 10, 11, 13,
                              10, 12, 13, 14,  // 12
                              11, 12, 13, 15,
                              12, 14, 15, 16,  // 14
                              13, 14, 15, 17,
                              14, 16, 17, 18,  // 16
                              15, 16, 17, 19,
                              16, 18, 19, 20,  // 18
                              17, 18, 19, 21,
                              18, 20, 21, 22,
                              19, 20, 21, 23,
                              20, 22, 23,
                              21, 22, 23};

        v_box_2x12_values = {32.5, 2.5, 25,
                             2.5, 32.5, 25,
                             25, 6.75, 2.25, 25,  // 2
                             25, 2.25, 6.75, 25,  // 3
                             25, 6.075, 2.025, 25,
                             25, 2.025, 6.075, 25,
                             25, 5.4675, 1.8225, 25, // 6
                             25, 1.8225, 5.4675, 25,
                             25, 4.92075, 1.64025, 25,
                             25, 1.64025, 4.92075, 25,
                             25, 4.428675, 1.476225, 25,  // 10
                             25, 1.476225, 4.428675, 25,
                             25, 4.8715425, 1.6238475, 25,  // 12
                             25, 1.6238475, 4.8715425, 25,
                             25, 5.35869675, 1.78623225, 25,// 14
                             25, 1.78623225, 5.35869675, 25,
                             25, 5.894566425, 1.964855475, 25,// 16
                             25, 1.964855475, 5.894566425, 25,
                             25, 6.4840230675, 2.1613410225, 25, // 18
                             25, 2.1613410225, 6.4840230675, 25,
                             25, 7.1324253743, 2.3774751248, 25, // 20
                             25, 2.3774751248, 7.1324253743, 25,
                             25, 32.5, 2.5,  // 22
                             25, 2.5, 32.5,};

        box_2x12_nb_fc = 24;
        box_2x12_col_ind_size = 92;

        v_box_2x12_volumes = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                              125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                              125., 125., 125., 125.,};

        int box_2x12_is_on_bnd[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0};

        unordered_map<long, int> box_2x12_d_is_on_bnd;
        for (int i = 0; i < box_2x12_nb_fc; i++) {
            box_2x12_d_is_on_bnd[i] = box_2x12_is_on_bnd[i];
        }

        box_2x12_number_of_anisotropic_compliant_fc = 24;
        unordered_set<long> s_anisotropic_compliant_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                          19, 20, 21, 22, 23};
        unordered_set<long> empty_set = unordered_set<long>({});
        g = new Dual_Graph(box_2x12_nb_fc,
                           v_box_2x12_row_ptr,
                           v_box_2x12_col_ind,
                           v_box_2x12_values,
                           v_box_2x12_volumes,
                           box_2x12_d_is_on_bnd,
                           empty_set,
                           empty_set,
                           empty_set,
                           s_anisotropic_compliant_fc);

    }

    virtual void TearDown() {
        delete g;
//        delete v_box_2x12_row_ptr;
//        delete v_box_2x12_col_ind;
//        delete v_box_2x12_values;
    }
};

class box_2x12_bis : public ::testing::Test {

protected:

    Dual_Graph *g;
    int box_2x12_nb_fc;
    int box_2x12_col_ind_size;
    vector<long> v_box_2x12_row_ptr;
    vector<long> v_box_2x12_col_ind;
    vector<double> v_box_2x12_values;
    vector<double> v_box_2x12_volumes;

    virtual void SetUp() {
        v_box_2x12_row_ptr = {0, 3, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82, 86, 89, 92};

        v_box_2x12_col_ind = {0, 1, 2,  // 0
                              0, 1, 3,  // 1
                              0, 2, 3, 4,  // 2
                              1, 2, 3, 5,
                              2, 4, 5, 6,  // 4
                              3, 4, 5, 7,
                              4, 6, 7, 8,  // 6
                              5, 6, 7, 9,
                              6, 8, 9, 10,  // 8
                              7, 8, 9, 11,
                              8, 10, 11, 12,  // 10
                              9, 10, 11, 13,
                              10, 12, 13, 14,  // 12
                              11, 12, 13, 15,
                              12, 14, 15, 16,  // 14
                              13, 14, 15, 17,
                              14, 16, 17, 18,  // 16
                              15, 16, 17, 19,
                              16, 18, 19, 20,  // 18
                              17, 18, 19, 21,
                              18, 20, 21, 22,
                              19, 20, 21, 23,
                              20, 22, 23,
                              21, 22, 23};

        v_box_2x12_values = {32.5, 2.5, 25,
                             2.5, 32.5, 25,
                             25, 6.75, 2.25, 25,  // 2
                             25, 2.25, 6.75, 25,  // 3
                             25, 6.075, 2.025, 25,
                             25, 2.025, 6.075, 25,
                             25, 5.4675, 1.8225, 25, // 6
                             25, 1.8225, 5.4675, 25,
                             25, 6.01425, 2.00475, 25,
                             25, 2.00475, 6.01425, 25,
                             25, 6.615675, 2.205225, 25,  // 10
                             25, 2.205225, 6.615675, 25,
                             25, 5.9541075, 1.9847025, 25,  // 12
                             25, 1.9847025, 5.9541075, 25,
                             25, 5.35869675, 1.78623225, 25,// 14
                             25, 1.78623225, 5.35869675, 25,
                             25, 5.894566425, 1.964855475, 25,// 16
                             25, 1.964855475, 5.894566425, 25,
                             25, 6.4840230675, 2.1613410225, 25, // 18
                             25, 2.1613410225, 6.4840230675, 25,
                             25, 7.1324253743, 2.3774751248, 25, // 20
                             25, 2.3774751248, 7.1324253743, 25,
                             25, 32.5, 2.5,  // 22
                             25, 2.5, 32.5,};

        box_2x12_nb_fc = 24;
        box_2x12_col_ind_size = 92;

        v_box_2x12_volumes = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                              125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                              125., 125., 125., 125.,};

        int box_2x12_is_on_bnd[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0};

        unordered_map<long, int> box_2x12_d_is_on_bnd;
        for (int i = 0; i < box_2x12_nb_fc; i++) {
            box_2x12_d_is_on_bnd[i] = box_2x12_is_on_bnd[i];
        }

        unordered_set<long> s_anisotropic_compliant_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                          19, 20, 21, 22, 23};
        unordered_set<long> empty_set = unordered_set<long>({});

        g = new Dual_Graph(box_2x12_nb_fc,
                           v_box_2x12_row_ptr,
                           v_box_2x12_col_ind,
                           v_box_2x12_values,
                           v_box_2x12_volumes,
                           box_2x12_d_is_on_bnd,
                           empty_set,
                           empty_set,
                           empty_set,
                           s_anisotropic_compliant_fc);

    }

    virtual void TearDown() {
        delete g;
//        delete v_box_2x12_row_ptr;
//        delete v_box_2x12_col_ind;
//        delete v_box_2x12_values;
    }
};

class box_2x12_ter : public ::testing::Test {

protected:

    Dual_Graph *g;
    int box_2x12_nb_fc;
    int box_2x12_col_ind_size;
    vector<long> v_box_2x12_row_ptr;
    vector<long> v_box_2x12_col_ind;
    vector<double> v_box_2x12_values;
    vector<double> v_box_2x12_volumes;

    virtual void SetUp() {
        v_box_2x12_row_ptr = {0, 3, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82, 86, 89, 92};

        v_box_2x12_col_ind = {0, 1, 2,  // 0
                              0, 1, 3,  // 1
                              0, 2, 3, 4,  // 2
                              1, 2, 3, 5,
                              2, 4, 5, 6,  // 4
                              3, 4, 5, 7,
                              4, 6, 7, 8,  // 6
                              5, 6, 7, 9,
                              6, 8, 9, 10,  // 8
                              7, 8, 9, 11,
                              8, 10, 11, 12,  // 10
                              9, 10, 11, 13,
                              10, 12, 13, 14,  // 12
                              11, 12, 13, 15,
                              12, 14, 15, 16,  // 14
                              13, 14, 15, 17,
                              14, 16, 17, 18,  // 16
                              15, 16, 17, 19,
                              16, 18, 19, 20,  // 18
                              17, 18, 19, 21,
                              18, 20, 21, 22,
                              19, 20, 21, 23,
                              20, 22, 23,
                              21, 22, 23};

        v_box_2x12_values = {100, 25, 25,
                             25, 100, 25,
                             25, 6.75, 2.25, 25,  // 2
                             25, 2.25, 6.75, 25,  // 3
                             25, 6.075, 2.025, 25,
                             25, 2.025, 6.075, 25,
                             25, 5.4675, 1.8225, 25, // 6
                             25, 1.8225, 5.4675, 25,
                             25, 6.01425, 2.00475, 25,
                             25, 2.00475, 6.01425, 25,
                             25, 6.615675, 2.205225, 25,  // 10
                             25, 2.205225, 6.615675, 25,
                             25, 5.9541075, 1.9847025, 25,  // 12
                             25, 1.9847025, 5.9541075, 25,
                             25, 5.35869675, 1.78623225, 25,// 14
                             25, 1.78623225, 5.35869675, 25,
                             25, 5.894566425, 1.964855475, 25,// 16
                             25, 1.964855475, 5.894566425, 25,
                             25, 6.4840230675, 2.1613410225, 25, // 18
                             25, 2.1613410225, 6.4840230675, 25,
                             25, 75, 25, 25, // 20
                             25, 25, 75, 25,
                             25, 100, 25,  // 22
                             25, 25, 100,};

        box_2x12_nb_fc = 24;
        box_2x12_col_ind_size = 92;

        v_box_2x12_volumes = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                              125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                              125., 125., 125., 125.,};

        int box_2x12_is_on_bnd[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0};

        unordered_map<long, int> box_2x12_d_is_on_bnd;
        for (int i = 0; i < box_2x12_nb_fc; i++) {
            box_2x12_d_is_on_bnd[i] = box_2x12_is_on_bnd[i];
        }

        long numberOfFineAnisotropicCompliantCells = 18;

        unordered_set<long> s_anisotropic_compliant_fc = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                          19};
        unordered_set<long> empty_set = unordered_set<long>({});

        g = new Dual_Graph(box_2x12_nb_fc,
                           v_box_2x12_row_ptr,
                           v_box_2x12_col_ind,
                           v_box_2x12_values,
                           v_box_2x12_volumes,
                           box_2x12_d_is_on_bnd,
                           empty_set,
                           empty_set,
                           empty_set,
                           s_anisotropic_compliant_fc);

    }

    virtual void TearDown() {
        delete g;
//        delete v_box_2x12_row_ptr;
//        delete v_box_2x12_col_ind;
//        delete v_box_2x12_values;
    }
};

TEST_F(Box_5x5x5_Dual_Graph, Dual_Graph_Box_5x5x5) {
    unordered_set<long> empty_set = unordered_set<long>({});
    unordered_set<long> ref_isOnRidge({1, 2, 4, 7, 8, 11, 13, 14, 16, 19, 28, 31, 32, 35, 44, 47, 49, 50, 52, 55, 56, 59, 61, 62});
    unordered_set<long> ref_isOnValley({5, 6, 9, 10, 17, 18, 20, 23, 24, 27, 29, 30, 33, 34, 36, 39, 40, 43, 45, 46, 53, 54, 57, 58});


    ASSERT_EQ(empty_set, g->seeds_pool->is_on_corner);
    ASSERT_EQ(ref_isOnRidge, g->seeds_pool->is_on_ridge);
    ASSERT_EQ(ref_isOnValley, g->seeds_pool->is_on_valley);

    for (int i = 0; i < box_5x5x5_number_of_cells + 1; i++) {
        ASSERT_EQ((v_box_5x5x5_row_ptr)[i], g->_m_CRS_Row_Ptr[i]);
    }
    ASSERT_EQ(344, (v_box_5x5x5_row_ptr)[box_5x5x5_number_of_cells]);
    for (int i = 0; i < (v_box_5x5x5_row_ptr)[box_5x5x5_number_of_cells]; i++) {
        ASSERT_EQ((v_box_5x5x5_col_ind)[i], g->_m_CRS_Col_Ind[i]);
    }
    for (int i = 0; i < (v_box_5x5x5_row_ptr)[box_5x5x5_number_of_cells]; i++) {
        ASSERT_EQ((v_box_5x5x5_values)[i], g->_m_CRS_Values[i]);
    }
}

TEST_F(MGridGen_Dual_Graph, Dual_Graph_MGridGen) {

    ASSERT_EQ(15, (*g).number_of_cells);
    unordered_set<long> empty_set = unordered_set<long>({});
    ASSERT_EQ(empty_set, (*g).seeds_pool->is_on_corner);
    ASSERT_EQ(empty_set, (*g).seeds_pool->is_on_ridge);
    ASSERT_EQ(s_MGridGen_is_on_valley, (*g).seeds_pool->is_on_valley);
    long myints_0[3] = {0, 1, 2};
    vector<long> ref_neighbours_0(myints_0, myints_0 + sizeof(myints_0) / sizeof(long));

    long myints_1[] = {0, 1, 3};
    vector<long> ref_neighbours_1(myints_1, myints_1 + sizeof(myints_1) / sizeof(long));

    ASSERT_EQ(ref_neighbours_0, (*g).get_neighbours(0));
    ASSERT_EQ(ref_neighbours_1, (*g).get_neighbours(1));

    ASSERT_EQ(3, (*g).get_nb_of_neighbours(0));
    ASSERT_EQ(3, (*g).get_nb_of_neighbours(1));

    double myints_n_0[] = {6.8284271199999997, 2., 2.};
    vector<double> ref_neighbours_n_0(myints_n_0, myints_n_0 + sizeof(myints_n_0) / sizeof(double));

    double myints_n_1[] = {2., 4.2360679799999996, 1.};
    vector<double> ref_neighbours_n_1(myints_n_1, myints_n_1 + sizeof(myints_n_1) / sizeof(double));

    for (int i = 0; i < ref_neighbours_n_0.size(); i++) {
        ASSERT_NEAR(ref_neighbours_n_0[i], (*g).get_weights(0)[i], 1e-10);
    }

    for (int i = 0; i < ref_neighbours_n_1.size(); i++) {
        ASSERT_NEAR(ref_neighbours_n_1[i], (*g).get_weights(1)[i], 1e-10);
    }
}

TEST_F(Nine_squares_3x3_Dual_Graph, Dual_Graph_MGridGen) {

    ASSERT_EQ(9, (*g).number_of_cells);

    vector<long> ref_neighbours_0 = {0, 1, 3};
    vector<long> ref_neighbours_1 = {0, 1, 2, 4};

    ASSERT_EQ(ref_neighbours_0, (*g).get_neighbours(0));
    ASSERT_EQ(ref_neighbours_1, (*g).get_neighbours(1));

    ASSERT_EQ(3, (*g).get_nb_of_neighbours(0));
    ASSERT_EQ(4, (*g).get_nb_of_neighbours(1));

    vector<double> ref_neighbours_n_0 = {1.0, 1., 1.};
    vector<double> ref_neighbours_n_1 = {1.0, 1.0, 1.0, 1.0,};

    for (int i = 0; i < ref_neighbours_n_0.size(); i++) {
        ASSERT_NEAR(ref_neighbours_n_0[i], (*g).get_weights(0)[i], 1e-10);
    }

    for (int i = 0; i < ref_neighbours_n_1.size(); i++) {
        ASSERT_NEAR(ref_neighbours_n_1[i], (*g).get_weights(1)[i], 1e-10);
    }

    vector<deque<long>> ref_l_deque_of_seeds(4);
    for (int i = 0; i < 4; i++) {
        ref_l_deque_of_seeds[i] = deque<long>();
    }
    ref_l_deque_of_seeds[2] = deque<long>({0, 2, 6, 8});
    ASSERT_EQ(ref_l_deque_of_seeds, (*(*g).seeds_pool).l_deque_of_seeds);

}

TEST_F(MGridGen_Dual_Graph, Dual_Graph_CheckConnectivity_MGridGenDoc_example) {

//    unordered_set<long> s_MGridGen_is_on_ridge = unordered_set<long>({12, 14});
    ASSERT_TRUE((*g).check_connectivity(unordered_set<long>({0, 1, 3})));
    ASSERT_TRUE((*g).check_connectivity(unordered_set<long>({3, 1, 0})));
    ASSERT_TRUE((*g).check_connectivity(unordered_set<long>({1, 3, 0})));
    ASSERT_TRUE((*g).check_connectivity(unordered_set<long>({0, 2})));
    ASSERT_TRUE ((*g).check_connectivity(unordered_set<long>({2, 0})));
    ASSERT_FALSE((*g).check_connectivity(unordered_set<long>({0, 6})));
    ASSERT_FALSE((*g).check_connectivity(unordered_set<long>({6, 0})));
    ASSERT_FALSE((*g).check_connectivity(unordered_set<long>({0, 1, 2, 3, 4, 5, 6, 7, 14})));
    ASSERT_TRUE((*g).check_connectivity(unordered_set<long>({0, 1, 2, 3, 4, 5, 6, 7, 11})));
    ASSERT_TRUE((*g).check_connectivity(unordered_set<long>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14})));
}

TEST(Dual_Graph_TestSuite, compute_anisotropic_line_1_AnisotropicCell) {

    long nb_c = 4;
    long adjMatrix_col_ind_size = 12;

    const vector<long> v_row_ptr = {0, 3, 6, 9, 12};
    const vector<long> v_col_ind = {0, 1, 2,
                                    0, 1, 3,
                                    0, 2, 3,
                                    1, 2, 3};

    const vector<double> v_values = {32.5, 2.5, 25,
                                     2.5, 32.5, 25,
                                     25, 32.5, 25,
                                     25, 25, 32.5};

    const vector<double> v_volumes = {1.0, 1.0, 1.0, 1.0};

    unordered_map<long, int> d_is_on_bnd_d;
    d_is_on_bnd_d[0] = 1;
    d_is_on_bnd_d[1] = 1;
    d_is_on_bnd_d[3] = 1;
    d_is_on_bnd_d[4] = 1;

    Dual_Graph g = Dual_Graph(nb_c,
                              v_row_ptr,
                              v_col_ind,
                              v_values,
                              v_volumes,
                              d_is_on_bnd_d);

    long arrayOfFineAnisotropicCompliantCells_size = 2;
    long arrayOfFineAnisotropicCompliantCells[2] = {0, 1};

    bool verbose = true;
    long *agglomerationLines_Idx = new long[nb_c];
    long *agglomerationLines = new long[nb_c];

    long indCoarseCell = 0;
    long nb_agglomerated_fc = 0;
    long agglomerationLines_Idx_size = nb_c;
    long agglomerationLines_size = nb_c;

    long sizes[10] = {nb_c, adjMatrix_col_ind_size, indCoarseCell, nb_agglomerated_fc, 0, 0, 0,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};
    bool is_agglometion_lines = g.compute_anisotropic_line(sizes,
                                                           arrayOfFineAnisotropicCompliantCells,
                                                           agglomerationLines_Idx,
                                                           agglomerationLines, verbose);

    ASSERT_FALSE(is_agglometion_lines);

    long nb_anisotropic_lines_p_one_size = sizes[8];
    agglomerationLines_size = sizes[9];

    ASSERT_EQ(2, nb_anisotropic_lines_p_one_size);
    ASSERT_EQ(0, agglomerationLines_size);
    long ref_agglomerationLines_Idx[2] = {0, 0};
    for (int i = 0; i < 2; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }
    delete[]agglomerationLines_Idx;
    delete[]agglomerationLines;
}

TEST(Dual_Graph_TestSuite, compute_anisotropic_line_v2_1_AnisotropicCell) {

    long nb_c = 4;
    long adjMatrix_col_ind_size = 12;

    const vector<long> v_row_ptr = {0, 3, 6, 9, 12};
    const vector<long> v_col_ind = {0, 1, 2,
                                    0, 1, 3,
                                    0, 2, 3,
                                    1, 2, 3};

    const vector<double> v_values = {32.5, 2.5, 25,
                                     2.5, 32.5, 25,
                                     25, 32.5, 25,
                                     25, 25, 32.5};

    const vector<double> v_volumes = {1.0, 1.0, 1.0, 1.0};

    unordered_map<long, int> d_is_on_bnd_d;
    d_is_on_bnd_d[0] = 1;
    d_is_on_bnd_d[1] = 1;
    d_is_on_bnd_d[3] = 1;
    d_is_on_bnd_d[4] = 1;

    unordered_set<long> s_anisotropic_compliant_fc = {0, 1};
    unordered_set<long> empty_set = {};

    Dual_Graph g = Dual_Graph(nb_c,
                              v_row_ptr,
                              v_col_ind,
                              v_values,
                              v_volumes,
                              d_is_on_bnd_d,
                              empty_set,
                              empty_set,
                              empty_set,
                              s_anisotropic_compliant_fc);

    forward_list<deque<long> *> lines = g.compute_anisotropic_line_v2();
    ASSERT_TRUE(lines.empty());
}

TEST_F(Box_5x5x5_Aniso_Dual_Graph, compute_anisotropic_line_Box_5x5x5_aniso_MG_1_level) {

    long numberOfFineAnisotropicCompliantCells = 64;
    long arrayOfFineAnisotropicCompliantCells[64] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                                     21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                                     31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
                                                     49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                                                     60, 61, 62, 63};

    long numberOfAnisotropicLinesPOne_size = box_5x5x5_number_of_cells;
    long agglomerationLines_size = box_5x5x5_number_of_cells;

    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    bool verbose = false;

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = box_5x5x5_number_of_cells;
    long agglomerationLines_Idx_size = box_5x5x5_number_of_cells;

    unordered_set<long> empty_set = unordered_set<long>({});
    unordered_set<long> ref_s_Box_5x5x5_is_on_valley = unordered_set<long>({57, 54, 45, 43, 40, 36, 27, 53, 24, 23, 39, 10, 20, 33, 18, 34, 5, 46, 17, 30, 6, 9, 58, 29});
    unordered_set<long> ref_s_Box_5x5x5_is_on_ridge = unordered_set<long>({56, 55, 52, 50, 49, 47, 44, 35, 11, 32, 61, 7, 28, 31, 2, 19, 16, 14, 59, 1, 13, 62, 4, 8});

    ASSERT_EQ(empty_set, (*(*g).seeds_pool).is_on_corner);
    ASSERT_EQ(ref_s_Box_5x5x5_is_on_ridge, (*(*g).seeds_pool).is_on_ridge);
    ASSERT_EQ(ref_s_Box_5x5x5_is_on_valley, (*(*g).seeds_pool).is_on_valley);


    long sizes[10] = {box_5x5x5_number_of_cells, box_5x5x5_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      box_5x5x5_number_of_cells, agglomerationLines_Idx_size, agglomerationLines_size};

    bool isAggloLines = g->compute_anisotropic_line(sizes,
                                                    arrayOfFineAnisotropicCompliantCells,
                                                    agglomerationLines_Idx,
                                                    agglomerationLines,
                                                    verbose);

    ASSERT_TRUE(isAggloLines);
    const long ref_agglo_lines_Idx_size = 17;
    const long ref_agglo_lines_size = 64;

    ASSERT_EQ(ref_agglo_lines_size, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(ref_agglo_lines_Idx_size, sizes[8]);  // number of anisotropic lines
    ASSERT_EQ(ref_agglo_lines_size, sizes[9]);  // number of cells in these lines

    long ref_agglo_lines_Idx[ref_agglo_lines_Idx_size] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64};
    long ref_agglo_lines[ref_agglo_lines_size] = {48, 32, 16, 0,
                                                  51, 35, 19, 3,
                                                  60, 44, 28, 12,
                                                  63, 47, 31, 15,
                                                  61, 45, 29, 13,
                                                  62, 46, 30, 14,
                                                  50, 34, 18, 2,
                                                  49, 33, 17, 1,
                                                  59, 43, 27, 11,
                                                  56, 40, 24, 8,
                                                  55, 39, 23, 7,
                                                  52, 36, 20, 4,
                                                  9, 25, 41, 57,
                                                  10, 26, 42, 58,
                                                  5, 21, 37, 53,
                                                  6, 22, 38, 54,
    };
    for (int i = 0; i < ref_agglo_lines_Idx_size; i++) {
        ASSERT_EQ(ref_agglo_lines_Idx[i], agglomerationLines_Idx[i]);
    }
    for (int i = 0; i < ref_agglo_lines_size; i++) {
//        cout << agglomerationLines[i] << ", ";
        ASSERT_EQ(ref_agglo_lines[i], agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;

}

TEST_F(Box_5x5x5_Aniso_Dual_Graph, compute_anisotropic_line_v2_Box_5x5x5_aniso_MG_1_level) {

    long numberOfAnisotropicLinesPOne_size = box_5x5x5_number_of_cells;
    long agglomerationLines_size = box_5x5x5_number_of_cells;

    bool verbose = false;

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = box_5x5x5_number_of_cells;
    long agglomerationLines_Idx_size = box_5x5x5_number_of_cells;

    unordered_set<long> empty_set = unordered_set<long>({});
    unordered_set<long> ref_s_Box_5x5x5_is_on_valley = unordered_set<long>({57, 54, 45, 43, 40, 36, 27, 53, 24, 23, 39, 10, 20, 33, 18, 34, 5, 46, 17, 30, 6, 9, 58, 29});
    unordered_set<long> ref_s_Box_5x5x5_is_on_ridge = unordered_set<long>({56, 55, 52, 50, 49, 47, 44, 35, 11, 32, 61, 7, 28, 31, 2, 19, 16, 14, 59, 1, 13, 62, 4, 8});

    ASSERT_EQ(empty_set, (*(*g).seeds_pool).is_on_corner);
    ASSERT_EQ(ref_s_Box_5x5x5_is_on_ridge, (*(*g).seeds_pool).is_on_ridge);
    ASSERT_EQ(ref_s_Box_5x5x5_is_on_valley, (*(*g).seeds_pool).is_on_valley);

    forward_list<deque<long> *> lines = g->compute_anisotropic_line_v2();
    ASSERT_FALSE(lines.empty());

    forward_list<deque<long> *> ref_lines;
    deque<long> *a = new deque<long>({54, 38, 22, 6,});
    ref_lines.push_front(a);

    a = new deque<long>({53, 37, 21, 5,});
    ref_lines.push_front(a);

    a = new deque<long>({58, 42, 26, 10,});
    ref_lines.push_front(a);

    a = new deque<long>({57, 41, 25, 9,});
    ref_lines.push_front(a);

    a = new deque<long>({59, 43, 27, 11,});
    ref_lines.push_front(a);

    a = new deque<long>({55, 39, 23, 7,});
    ref_lines.push_front(a);

    a = new deque<long>({62, 46, 30, 14});
    ref_lines.push_front(a);

    a = new deque<long>({61, 45, 29, 13});
    ref_lines.push_front(a);

    a = new deque<long>({52, 36, 20, 4});
    ref_lines.push_front(a);

    // 6
    a = new deque<long>({56, 40, 24, 8});
    ref_lines.push_front(a);

    // 5
    a = new deque<long>({49, 33, 17, 1,});
    ref_lines.push_front(a);

    a = new deque<long>({50, 34, 18, 2,});
    ref_lines.push_front(a);

    a = new deque<long>({63, 47, 31, 15});
    ref_lines.push_front(a);

    a = new deque<long>({51, 35, 19, 3,});
    ref_lines.push_front(a);

    a = new deque<long>({60, 44, 28, 12,});
    ref_lines.push_front(a);

    // 0
    a = new deque<long>({48, 32, 16, 0,});
    ref_lines.push_front(a);

    forward_list<deque<long> *>::iterator fLIt;
    forward_list<deque<long> *>::iterator fLIt_bis;
    fLIt_bis = ref_lines.begin();
    int count = 0;

    for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {
//        cout << "count " << count << endl;
        ASSERT_EQ(**fLIt, **fLIt_bis);
//        cout << "{";
//        for (auto i :(*(*fLIt))) {
//            cout << i << ", ";
//        }
//        cout << "}" << endl;
        count++;
        fLIt_bis++;
    }
    int nb_agglomeration_lines = count;

    long sizes[2] = {0, 0};
    long a_agglo_lines_idx[box_5x5x5_number_of_cells];  // not initialized
    long a_agglo_lines[box_5x5x5_number_of_cells];  // not initialized

    convert_agglo_lines_to_agglomeration_lines_arrays(nb_agglomeration_lines,
                                                      lines,
                                                      sizes,
                                                      a_agglo_lines_idx,
                                                      a_agglo_lines);

    const long ref_agglo_lines_Idx_size = 17;
    const long ref_agglo_lines_size = 64;

    ASSERT_EQ(ref_agglo_lines_size, box_5x5x5_number_of_cells);
    ASSERT_EQ(ref_agglo_lines_Idx_size, sizes[0]);  // number of anisotropic lines
    ASSERT_EQ(ref_agglo_lines_size, sizes[1]);  // number of cells in these lines

    long ref_agglo_lines_Idx[ref_agglo_lines_Idx_size] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64};
    long ref_agglo_lines[ref_agglo_lines_size] = {48, 32, 16, 0,
                                                  60, 44, 28, 12,
                                                  51, 35, 19, 3,
                                                  63, 47, 31, 15,
                                                  50, 34, 18, 2, 49, 33, 17, 1, 56, 40, 24, 8, 52, 36, 20, 4, 61, 45, 29, 13, 62, 46, 30, 14, 55, 39, 23, 7, 59, 43, 27, 11, 57, 41, 25, 9, 58, 42, 26,
                                                  10, 53, 37, 21, 5, 54, 38, 22, 6,
    };

    for (int i = 0; i < ref_agglo_lines_Idx_size; i++) {
//        cout<<"i "<<i<<"\t"<<a_agglo_lines_idx[i]<<endl;
        ASSERT_EQ(ref_agglo_lines_Idx[i], a_agglo_lines_idx[i]);
    }
    for (int i = 0; i < ref_agglo_lines_size; i++) {
//        cout << a_agglo_lines[i] << ", ";
        ASSERT_EQ(ref_agglo_lines[i], a_agglo_lines[i]);
    }

    clean_agglomeration_lines(lines);
    clean_agglomeration_lines(ref_lines);
}

TEST_F(box_5x5x5_iso_and_aniso, compute_anisotropic_line_Box_5x5x5_iso_and_Aniso_MG_1_level) {

    long numberOfFineAnisotropicCompliantCells = 64;
    long arrayOfFineAnisotropicCompliantCells[64] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                     19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
                                                     36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
                                                     53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};
    long numberOfAnisotropicLinesPOne_size = numberOfFineAnisotropicCompliantCells;
    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = box_5x5x5_number_of_cells;
    long agglomerationLines_Idx_size = box_5x5x5_number_of_cells;
    long agglomerationLines_size = numberOfFineAnisotropicCompliantCells;

    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    long verbose = 0;

    long sizes[10] = {box_5x5x5_number_of_cells, box_5x5x5_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      arrayOfFineAnisotropicCompliantCells_size, agglomerationLines_Idx_size, agglomerationLines_size};

    bool isAggloLines = g->compute_anisotropic_line(sizes,
                                                    arrayOfFineAnisotropicCompliantCells,
                                                    agglomerationLines_Idx,
                                                    agglomerationLines,
                                                    verbose);
    ASSERT_TRUE(isAggloLines);
    ASSERT_EQ(64, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(17, sizes[8]);
    ASSERT_EQ(48, sizes[9]);
    long ref_agglomerationLines_Idx[17] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48};
    long ref_agglomerationLines[48] = {0, 16, 32,
                                       15, 31, 47,
                                       3, 19, 35,
                                       12, 28, 44, 2, 18, 34, 1, 17, 33, 11, 27, 43, 8, 24, 40, 7, 23, 39, 4, 20, 36, 14, 30, 46, 13, 29, 45, 6, 22, 38, 5, 21, 37, 10, 26, 42, 9, 25, 41};
    for (int i = 0; i < 17; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }

    for (int i = 0; i < 48; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
}

TEST_F(box_5x5x5_iso_and_aniso, compute_anisotropic_line_v2_Box_5x5x5_iso_and_Aniso_MG_1_level) {

    forward_list<deque<long> *> lines = g->compute_anisotropic_line_v2();
    ASSERT_TRUE(!lines.empty());

    ASSERT_EQ(64, box_5x5x5_number_of_cells);


    unsigned long nb_agglomeration_lines = 0;
    forward_list<deque<long> *>::iterator fLIt;
    for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {
        nb_agglomeration_lines++;

    }

    long sizes[2] = {0, 0};
    long a_agglo_lines_idx[box_5x5x5_number_of_cells];  // not initialized
    long a_agglo_lines[box_5x5x5_number_of_cells];  // not initialized
    convert_agglo_lines_to_agglomeration_lines_arrays(nb_agglomeration_lines,
                                                      lines,
                                                      sizes,
                                                      a_agglo_lines_idx,
                                                      a_agglo_lines);
    ASSERT_EQ(17, sizes[0]);
    ASSERT_EQ(48, sizes[1]);
    long ref_agglomerationLines_Idx[17] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48};
    long ref_agglomerationLines[48] = {0, 16, 32,
                                       3, 19, 35,
                                       12, 28, 44,
                                       15, 31, 47,
                                       2, 18, 34,
                                       1, 17, 33,
                                       4, 20, 36,
                                       8, 24, 40,
                                       14, 30, 46,
                                       13, 29, 45,
                                       7, 23, 39,
                                       11, 27, 43,
                                       6, 22, 38,
                                       5, 21, 37,
                                       10, 26, 42,
                                       9, 25, 41};

    for (int i = 0; i < 17; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], a_agglo_lines_idx[i]);
    }

    for (int i = 0; i < 48; i++) {
//        cout << a_agglo_lines[i] << ", ";
        ASSERT_EQ(ref_agglomerationLines[i], a_agglo_lines[i]);
    }
}

TEST_F(box_2x12, compute_anisotropic_line_MostAnisotropicCellInTheMiddle) {


    long numberOfFineAnisotropicCompliantCells = 24;
    long arrayOfFineAnisotropicCompliantCells[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                     19, 20, 21, 22, 23};
    long numberOfAnisotropicLinesPOne_size = numberOfFineAnisotropicCompliantCells;
    long agglomerationLines_size = numberOfFineAnisotropicCompliantCells;

    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    bool verbose = false;

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = box_2x12_nb_fc;
    long agglomerationLines_Idx_size = box_2x12_nb_fc;

    long sizes[10] = {box_2x12_nb_fc, box_2x12_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      numberOfFineAnisotropicCompliantCells, agglomerationLines_Idx_size, agglomerationLines_size};

    bool isAggloLines = g->compute_anisotropic_line(sizes,
                                                    arrayOfFineAnisotropicCompliantCells,
                                                    agglomerationLines_Idx,
                                                    agglomerationLines,
                                                    verbose);

    ASSERT_TRUE(isAggloLines);
    ASSERT_EQ(24, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(3, sizes[8]);  // number of anisotropic lines
    ASSERT_EQ(24, sizes[9]);  // number of cells in these lines
    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 24;
    long ref_agglomerationLines_Idx[3] = {0, 12, 24};
    long ref_agglomerationLines[24] = {22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0, 23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }

    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
}

TEST_F(box_2x12, compute_anisotropic_line_v2_MostAnisotropicCellInTheMiddle) {

    forward_list<deque<long> *> lines = g->compute_anisotropic_line_v2();
    ASSERT_FALSE(lines.empty());

    unsigned long nb_agglomeration_lines = 0;
    forward_list<deque<long> *>::iterator fLIt;
    for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {
        nb_agglomeration_lines++;
    }

    long sizes[2] = {0, 0};
    long a_agglo_lines_idx[box_2x12_nb_fc];  // not initialized
    long a_agglo_lines[box_2x12_nb_fc];  // not initialized

    convert_agglo_lines_to_agglomeration_lines_arrays(nb_agglomeration_lines,
                                                      lines,
                                                      sizes,
                                                      a_agglo_lines_idx,
                                                      a_agglo_lines);

    ASSERT_EQ(24, box_2x12_nb_fc);
    ASSERT_EQ(3, sizes[0]);  // number of anisotropic lines
    ASSERT_EQ(24, sizes[1]);  // number of cells in these lines
    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 24;
    long ref_agglomerationLines_Idx[3] = {0, 12, 24};
    long ref_agglomerationLines[24] = {22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0, 23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], a_agglo_lines_idx[i]);
    }

    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], a_agglo_lines[i]);
    }
}

TEST_F(box_2x12_bis, compute_anisotropic_line_MostAnisotropicCellInTheMiddle_2) {

    long numberOfFineAnisotropicCompliantCells = 24;
    long arrayOfFineAnisotropicCompliantCells[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                     19, 20, 21, 22, 23};
    long numberOfAnisotropicLinesPOne_size = numberOfFineAnisotropicCompliantCells;
    long agglomerationLines_size = numberOfFineAnisotropicCompliantCells;

    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    bool verbose = false;

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = box_2x12_nb_fc;
    long agglomerationLines_Idx_size = box_2x12_nb_fc;

    long sizes[10] = {box_2x12_nb_fc, box_2x12_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      numberOfFineAnisotropicCompliantCells, agglomerationLines_Idx_size, agglomerationLines_size};

    bool isAggloLines = g->compute_anisotropic_line(sizes,
                                                    arrayOfFineAnisotropicCompliantCells,
                                                    agglomerationLines_Idx,
                                                    agglomerationLines,
                                                    verbose);

    ASSERT_TRUE(isAggloLines);
    ASSERT_EQ(24, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(3, sizes[8]);  // number of anisotropic lines
    ASSERT_EQ(24, sizes[9]);  // number of cells in these lines

    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 24;
    long ref_agglomerationLines_Idx[3] = {0, 12, 24};
    long ref_agglomerationLines[24] = {23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }
    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
}

TEST_F(box_2x12_bis, compute_anisotropic_line_v2_MostAnisotropicCellInTheMiddle_2) {

    forward_list<deque<long> *> lines = g->compute_anisotropic_line_v2();
    ASSERT_FALSE(lines.empty());

    long sizes[2] = {0, 0};
    long a_agglo_lines_idx[box_2x12_nb_fc];  // not initialized
    long a_agglo_lines[box_2x12_nb_fc];  // not initialized

    unsigned long nb_agglomeration_lines = 0;
    forward_list<deque<long> *>::iterator fLIt;
    for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {
        nb_agglomeration_lines++;
    }
    ASSERT_EQ(2, nb_agglomeration_lines);  // number of anisotropic lines

    convert_agglo_lines_to_agglomeration_lines_arrays(nb_agglomeration_lines,
                                                      lines,
                                                      sizes,
                                                      a_agglo_lines_idx,
                                                      a_agglo_lines);
    ASSERT_EQ(24, box_2x12_nb_fc);
    ASSERT_EQ(3, sizes[0]);  // number of anisotropic lines +1
    ASSERT_EQ(24, sizes[1]);  // number of cells in these lines

    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 24;
    long ref_agglomerationLines_Idx[3] = {0, 12, 24};
    long ref_agglomerationLines[24] = {22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0,
                                       23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], a_agglo_lines_idx[i]);
    }
    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], a_agglo_lines[i]);
    }
}

TEST_F(box_2x12_ter, compute_anisotropic_line_MostAnisotropicCellInTheMiddle_Iso_at_both_ends) {

    long numberOfFineAnisotropicCompliantCells = 18;
    long arrayOfFineAnisotropicCompliantCells[18] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                     19};
    long numberOfAnisotropicLinesPOne_size = numberOfFineAnisotropicCompliantCells;
    long agglomerationLines_size = numberOfFineAnisotropicCompliantCells;

    long *agglomerationLines_Idx = new long[numberOfAnisotropicLinesPOne_size];
    long *agglomerationLines = new long[numberOfAnisotropicLinesPOne_size];
    bool verbose = false;

    long indCoarseCell = 0;
    long numberOfFineAgglomeratedCells = 0;
    long arrayOfFineAnisotropicCompliantCells_size = box_2x12_nb_fc;
    long agglomerationLines_Idx_size = box_2x12_nb_fc;

    long sizes[10] = {box_2x12_nb_fc, box_2x12_col_ind_size, indCoarseCell, numberOfFineAgglomeratedCells, 0, 0, 0,
                      numberOfFineAnisotropicCompliantCells, agglomerationLines_Idx_size, agglomerationLines_size};

    bool isAggloLines = g->compute_anisotropic_line(sizes,
                                                    arrayOfFineAnisotropicCompliantCells,
                                                    agglomerationLines_Idx,
                                                    agglomerationLines,
                                                    verbose);

    ASSERT_TRUE(isAggloLines);
    ASSERT_EQ(18, numberOfFineAnisotropicCompliantCells);
    ASSERT_EQ(3, sizes[8]);  // number of anisotropic lines
    ASSERT_EQ(18, sizes[9]);  // number of cells in these lines

    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 18;
    long ref_agglomerationLines_Idx[3] = {0, 9, 18};
    long ref_agglomerationLines[18] = {3, 5, 7, 9, 11, 13, 15, 17, 19, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], agglomerationLines_Idx[i]);
    }
    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], agglomerationLines[i]);
    }
    delete[] agglomerationLines_Idx;
    delete[] agglomerationLines;
}

TEST_F(box_2x12_ter, compute_anisotropic_line_v2_MostAnisotropicCellInTheMiddle_Iso_at_both_ends) {

    forward_list<deque<long> *> lines = g->compute_anisotropic_line_v2();
    ASSERT_FALSE(lines.empty());

    ASSERT_EQ(18, g->s_anisotropic_compliant_cells.size());

    long sizes[2] = {0, 0};
    long a_agglo_lines_idx[box_2x12_nb_fc];  // not initialized
    long a_agglo_lines[box_2x12_nb_fc];  // not initialized

    unsigned long nb_agglomeration_lines = 0;
    forward_list<deque<long> *>::iterator fLIt;
    for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {
        nb_agglomeration_lines++;
    }
    ASSERT_EQ(2, nb_agglomeration_lines);  // number of anisotropic lines

    convert_agglo_lines_to_agglomeration_lines_arrays(nb_agglomeration_lines,
                                                      lines,
                                                      sizes,
                                                      a_agglo_lines_idx,
                                                      a_agglo_lines);

    ASSERT_EQ(3, sizes[0]);  // number of anisotropic lines
    ASSERT_EQ(18, sizes[1]);  // number of cells in these lines

    long ref_agglomerationLines_Idx_size = 3;
    long ref_agglomerationLines_size = 18;
    long ref_agglomerationLines_Idx[3] = {0, 9, 18};
    long ref_agglomerationLines[18] = {2, 4, 6, 8, 10, 12, 14, 16, 18,
                                       3, 5, 7, 9, 11, 13, 15, 17, 19};
    for (int i = 0; i < ref_agglomerationLines_Idx_size; i++) {
        ASSERT_EQ(ref_agglomerationLines_Idx[i], a_agglo_lines_idx[i]);
    }
    for (int i = 0; i < ref_agglomerationLines_size; i++) {
        ASSERT_EQ(ref_agglomerationLines[i], a_agglo_lines[i]);
    }
}

TEST_F(MGridGen_Dual_Graph, compute_min_fc_compactness_inside_a_cc_mgridgen) {

    unordered_set<long> s_fc = unordered_set<long>({0, 1, 2, 3, 6});
    ASSERT_EQ(2, (*g).compute_min_fc_compactness_inside_a_cc(s_fc));

    s_fc = unordered_set<long>({0, 1, 2, 3});
    ASSERT_EQ(1, (*g).compute_min_fc_compactness_inside_a_cc(s_fc));

    s_fc = unordered_set<long>({0});
    ASSERT_EQ(0, (*g).compute_min_fc_compactness_inside_a_cc(s_fc));

    s_fc = unordered_set<long>({4, 5, 7, 8, 9, 10});
    ASSERT_EQ(2, (*g).compute_min_fc_compactness_inside_a_cc(s_fc));
}

TEST_F(MGridGen_Dual_Graph, compute_fc_compactness_inside_a_cc_mgridgen) {

    unordered_set<long> s_fc = unordered_set<long>({0, 1, 2, 3, 6});
    unordered_map<long, unsigned short int> ref_d_fc_compactness = {{0, 2},
                                                                    {1, 2},
                                                                    {2, 2},
                                                                    {3, 2},
                                                                    {6, 2},
    };
    unordered_map<long, unsigned short int> d_fc_compactness = (*g).compute_fc_compactness_inside_a_cc(s_fc);
    ASSERT_EQ(ref_d_fc_compactness, d_fc_compactness);

    s_fc = unordered_set<long>({0, 1, 2, 3, 6, 5, 11});
    d_fc_compactness = (*g).compute_fc_compactness_inside_a_cc(s_fc);
    ref_d_fc_compactness = {{0,  2},
                            {1,  2},
                            {2,  3},
                            {3,  2},
                            {5,  1},
                            {6,  3},
                            {11, 1}
    };
    ASSERT_EQ(ref_d_fc_compactness, d_fc_compactness);

    s_fc = unordered_set<long>({0, 1, 2, 3, 6, 5, 11, 14});
    d_fc_compactness = (*g).compute_fc_compactness_inside_a_cc(s_fc);
    ref_d_fc_compactness = {{0,  2},
                            {1,  2},
                            {2,  3},
                            {3,  2},
                            {5,  1},
                            {6,  3},
                            {11, 2},
                            {14, 1}
    };
    ASSERT_EQ(ref_d_fc_compactness, d_fc_compactness);

    s_fc = unordered_set<long>({2, 6, 5, 11, 14});
    d_fc_compactness = (*g).compute_fc_compactness_inside_a_cc(s_fc);
    ref_d_fc_compactness = {{2,  2},
                            {5,  1},
                            {6,  2},
                            {11, 2},
                            {14, 1}
    };
    ASSERT_EQ(ref_d_fc_compactness, d_fc_compactness);
}

TEST_F(MGridGen_Dual_Graph, compute_average_fc_compactness_inside_a_cc_mgridgen) {

    unordered_set<long> s_fc = unordered_set<long>({0, 1, 2, 3, 6});
    ASSERT_EQ(2, (*g).compute_average_fc_compactness_inside_a_cc(s_fc));

    s_fc = unordered_set<long>({0, 1, 2, 3});
    ASSERT_EQ(1.5, (*g).compute_average_fc_compactness_inside_a_cc(s_fc));

    s_fc = unordered_set<long>({0});
    ASSERT_EQ(0, (*g).compute_average_fc_compactness_inside_a_cc({0}));

    s_fc = unordered_set<long>({4, 5, 7, 8, 9, 10});
    ASSERT_EQ(2.3333333333333335, (*g).compute_average_fc_compactness_inside_a_cc(s_fc));
}

TEST_F(Graphs_biconnected, compute_Degree_Of_Node_In_SubGraph) {

    unordered_set<long> s_fc = unordered_set<long>({0, 1, 2, 3, 4, 5, 6, 7});
    long seed = 0;
    ASSERT_EQ(3, (*g).compute_degree_of_node_in_subgraph(seed, s_fc));

    seed = 5;
    ASSERT_EQ(4, (*g).compute_degree_of_node_in_subgraph(seed, s_fc));

    seed = 0;
    s_fc = unordered_set<long>({2, 6, 3, 7});
    ASSERT_EQ(0, (*g).compute_degree_of_node_in_subgraph(seed, s_fc));

    seed = 0;
    s_fc = unordered_set<long>({1, 2, 6, 3, 7});
    ASSERT_EQ(1, (*g).compute_degree_of_node_in_subgraph(seed, s_fc));
}

TEST_F(MGridGen_Dual_Graph, compute_Degree_Of_Node_In_SubGraph_MGridGen) {

    unordered_set<long> s_fc_1 = unordered_set<long>({9, 4, 7, 14, 2, 0, 12, 13, 5, 11, 8, 10});

    ASSERT_EQ(1, (*g).compute_degree_of_node_in_subgraph(0, s_fc_1));
    ASSERT_EQ(1, (*g).compute_degree_of_node_in_subgraph(1, s_fc_1));
    ASSERT_EQ(2, (*g).compute_degree_of_node_in_subgraph(2, s_fc_1));
    ASSERT_EQ(0, (*g).compute_degree_of_node_in_subgraph(3, s_fc_1));
    ASSERT_EQ(2, (*g).compute_degree_of_node_in_subgraph(4, s_fc_1));
    ASSERT_EQ(3, (*g).compute_degree_of_node_in_subgraph(5, s_fc_1));
    ASSERT_EQ(2, (*g).compute_degree_of_node_in_subgraph(6, s_fc_1));
    ASSERT_EQ(2, (*g).compute_degree_of_node_in_subgraph(11, s_fc_1));
}

TEST_F(Nine_squares_3x3_Dual_Graph, compute_connected_component_9_squares) {


/**
 * 9 squares (3*3)
 */
    unordered_set<long> s_initial_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    vector<unordered_set<long>> l_of_connected_set = (*g).compute_connected_components(s_initial_fc);
    vector<unordered_set<long>> ref_connected_component = {s_initial_fc};
    ASSERT_EQ(ref_connected_component, l_of_connected_set);

    s_initial_fc = {0, 1, 2, 6, 7, 8};
    l_of_connected_set = (*g).compute_connected_components(s_initial_fc);
    ref_connected_component = {unordered_set<long>({6, 7, 8}), unordered_set<long>({0, 1, 2})};
    ASSERT_EQ(ref_connected_component, l_of_connected_set);

    s_initial_fc = {0, 2, 4, 6, 8};
    l_of_connected_set = (*g).compute_connected_components(s_initial_fc);
    ref_connected_component = {unordered_set<long>({8}), unordered_set<long>({6}), unordered_set<long>({4}), unordered_set<long>({2}), unordered_set<long>({0})};
    ASSERT_EQ(ref_connected_component, l_of_connected_set);

    s_initial_fc = {0, 2, 4, 1, 8};
    l_of_connected_set = (*g).compute_connected_components(s_initial_fc);
    ref_connected_component = {unordered_set<long>({8}), unordered_set<long>({0, 1, 2, 4})};
    ASSERT_EQ(ref_connected_component, l_of_connected_set);
}

TEST_F(Edge_7_Dual_Graph, find_seed_via_frontal_method_Case_2) {

    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(6, max_seed);   // correct answer 1, 3, 6

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[3] = new queue<long>({2, 4});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[6] = new queue<long>({4, 5});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
}

TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_025) {

    vector<long> listOfFineCells = {0, 2, 5};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(5, max_seed);   // correct answer 1, 3, 6

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({2});
    ref_max_dict[2] = new queue<long>({0, 5});
    ref_max_dict[5] = new queue<long>({2});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_10265813119) {

    vector<long> listOfFineCells = {10, 2, 6, 5, 8, 13, 11, 9};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(9, max_seed);   // correct answer 1, 3, 6

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[10] = new queue<long>({8, 9, 13});
    ref_max_dict[8] = new queue<long>({5, 10});
    ref_max_dict[9] = new queue<long>({10});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[5] = new queue<long>({2, 8});
    ref_max_dict[11] = new queue<long>({6, 13});
    ref_max_dict[2] = new queue<long>({5, 6});
    ref_max_dict[6] = new queue<long>({2, 11});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_136111314) {

    vector<long> listOfFineCells = {1, 3, 6, 11, 13, 14};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(13, max_seed);   // correct answer 1, 3, 6

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[1] = new queue<long>({3});
    ref_max_dict[14] = new queue<long>({11});
    ref_max_dict[3] = new queue<long>({1, 6});
    ref_max_dict[6] = new queue<long>({3, 11});
    ref_max_dict[11] = new queue<long>({6, 13, 14});
    ref_max_dict[13] = new queue<long>({11});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_520810131411) {

    vector<long> listOfFineCells = {5, 2, 0, 8, 10, 13, 14, 11};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(0, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[5] = new queue<long>({2, 8});
    ref_max_dict[2] = new queue<long>({0, 5});
    ref_max_dict[8] = new queue<long>({5, 10});
    ref_max_dict[0] = new queue<long>({2});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[10] = new queue<long>({8, 13});
    ref_max_dict[11] = new queue<long>({13, 14});
    ref_max_dict[14] = new queue<long>({11});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_145208101311) {

    vector<long> listOfFineCells = {14, 5, 2, 0, 8, 10, 13, 11};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(0, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[14] = new queue<long>({11});
    ref_max_dict[11] = new queue<long>({13, 14});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[0] = new queue<long>({2});
    ref_max_dict[10] = new queue<long>({8, 13});
    ref_max_dict[8] = new queue<long>({5, 10});
    ref_max_dict[5] = new queue<long>({2, 8});
    ref_max_dict[2] = new queue<long>({0, 5});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_457810) {

    vector<long> listOfFineCells = {8, 7, 5, 4, 10};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(10, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[8] = new queue<long>({5, 7, 10});
    ref_max_dict[5] = new queue<long>({4, 8});
    ref_max_dict[7] = new queue<long>({4, 8});
    ref_max_dict[10] = new queue<long>({8});
    ref_max_dict[4] = new queue<long>({5, 7});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_205) {

    vector<long> listOfFineCells = {2, 0, 5};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(5, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[2] = new queue<long>({0, 5});
    ref_max_dict[0] = new queue<long>({2,});
    ref_max_dict[5] = new queue<long>({2,});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_All) {

    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(12, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[10] = new queue<long>({8, 9, 13});
    ref_max_dict[7] = new queue<long>({4, 8, 9});
    ref_max_dict[12] = new queue<long>({9,});
    ref_max_dict[9] = new queue<long>({7, 10, 12});
    ref_max_dict[0] = new queue<long>({1, 2,});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[1] = new queue<long>({0, 3});
    ref_max_dict[14] = new queue<long>({11});
    ref_max_dict[2] = new queue<long>({0, 5, 6});
    ref_max_dict[3] = new queue<long>({1, 6});
    ref_max_dict[5] = new queue<long>({2, 4, 8});
    ref_max_dict[6] = new queue<long>({2, 3, 11});
    ref_max_dict[4] = new queue<long>({5, 7});
    ref_max_dict[8] = new queue<long>({5, 7, 10});
    ref_max_dict[11] = new queue<long>({6, 13, 14});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(Edge_9_3_Dual_Graph, find_seed_via_frontal_method_Case_3_seed_0) {

    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(7, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[3] = new queue<long>({2, 4, 8});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[8] = new queue<long>({3});
    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(Edge_9_3_Dual_Graph, find_seed_via_frontal_method_Case_3_seed_1) {

    vector<long> listOfFineCells = {1, 2, 3, 4, 5, 6, 7, 8};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(8, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[1] = new queue<long>({2, 7});
    ref_max_dict[2] = new queue<long>({1, 3, 4, 5});
    ref_max_dict[3] = new queue<long>({2, 4, 8});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[8] = new queue<long>({3});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(Edge_9_3_Dual_Graph, find_seed_via_frontal_method_Case_3_seed_6) {

    vector<long> listOfFineCells = {6, 7, 8, 1, 3, 5, 4, 0, 2};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(8, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[3] = new queue<long>({2, 4, 8});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[8] = new queue<long>({3});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(Edge_9_3_Dual_Graph, find_seed_via_frontal_method_Case_3_seed_8) {

    vector<long> listOfFineCells = {8, 1, 2, 3, 4, 5, 0, 6, 7};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(7, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[3] = new queue<long>({2, 4, 8});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[8] = new queue<long>({3});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(Edge_8_Dual_Graph, find_seed_via_frontal_method_Case_4_random) {

    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(7, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5});
    ref_max_dict[3] = new queue<long>({2, 4});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});


    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(Edge_9_Dual_Graph, find_seed_via_frontal_method_Case_5_random) {

    vector<long> listOfFineCells = {5, 0, 3, 4, 2, 7, 1, 6, 8};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(7, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 2, 5});
    ref_max_dict[1] = new queue<long>({0, 2, 7});
    ref_max_dict[2] = new queue<long>({0, 1, 3, 4, 5, 8});
    ref_max_dict[3] = new queue<long>({2, 4});
    ref_max_dict[4] = new queue<long>({2, 3, 5, 6});
    ref_max_dict[5] = new queue<long>({0, 2, 4, 6});
    ref_max_dict[6] = new queue<long>({4, 5});
    ref_max_dict[7] = new queue<long>({1});
    ref_max_dict[8] = new queue<long>({2});

    int i_count = 0;
    for (auto i : dict_ConnectivityTree) {
//        cout<< "i_count " << i_count++<<endl;
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(Example_6_Dual_Graph, find_seed_via_frontal_method_Case_6) {

    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(4, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 5, 8});
    ref_max_dict[1] = new queue<long>({0, 2});
    ref_max_dict[2] = new queue<long>({1, 6, 7});
    ref_max_dict[3] = new queue<long>({4, 6, 7});
    ref_max_dict[4] = new queue<long>({3, 7, 8});
    ref_max_dict[5] = new queue<long>({0, 8});
    ref_max_dict[6] = new queue<long>({2, 3, 7});
    ref_max_dict[7] = new queue<long>({2, 3, 4, 6, 8});
    ref_max_dict[8] = new queue<long>({0, 4, 5, 7});


    int i_count = 0;
    for (auto i : dict_ConnectivityTree) {
//        cout<< "i_count " << i_count++<<endl;
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(Example_6_Dual_Graph, find_seed_via_frontal_method_Case_6_seed_6) {

    vector<long> listOfFineCells = {6, 0, 1, 2, 3, 4, 5, 7, 8};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(4, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[0] = new queue<long>({1, 5, 8});
    ref_max_dict[1] = new queue<long>({0, 2});
    ref_max_dict[2] = new queue<long>({1, 6, 7});
    ref_max_dict[3] = new queue<long>({4, 6, 7});
    ref_max_dict[4] = new queue<long>({3, 7, 8});
    ref_max_dict[5] = new queue<long>({0, 8});
    ref_max_dict[6] = new queue<long>({2, 3, 7});
    ref_max_dict[7] = new queue<long>({2, 3, 4, 6, 8});
    ref_max_dict[8] = new queue<long>({0, 4, 5, 7});


    int i_count = 0;
    for (auto i : dict_ConnectivityTree) {
//        cout<< "i_count " << i_count++<<endl;
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}


TEST_F(MGridGen_Dual_Graph, find_seed_via_frontal_method_Case_All_shuffled) {

    vector<long> listOfFineCells = {9, 4, 7, 14, 2, 0, 12, 13, 5, 6, 1, 11, 3, 8, 10};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);

    EXPECT_EQ(12, max_seed);

    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[14] = new queue<long>({11});
    ref_max_dict[6] = new queue<long>({2, 3, 11});
    ref_max_dict[1] = new queue<long>({0, 3});
    ref_max_dict[3] = new queue<long>({1, 6});
    ref_max_dict[9] = new queue<long>({7, 10, 12});
    ref_max_dict[7] = new queue<long>({4, 8, 9});
    ref_max_dict[10] = new queue<long>({8, 9, 13});
    ref_max_dict[12] = new queue<long>({9,});
    ref_max_dict[4] = new queue<long>({5, 7});
    ref_max_dict[8] = new queue<long>({5, 7, 10});
    ref_max_dict[13] = new queue<long>({10, 11});
    ref_max_dict[0] = new queue<long>({1, 2,});
    ref_max_dict[5] = new queue<long>({2, 4, 8});
    ref_max_dict[11] = new queue<long>({6, 13, 14});
    ref_max_dict[2] = new queue<long>({0, 5, 6});

    for (auto i : dict_ConnectivityTree) {
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
//        cout << i.first << ": [";
//        while (!(*i.second).empty()) {
//            cout << (*i.second).front() << ", ";
//            (*i.second).pop();
//        }
//        cout << "]" << endl;
    }
    for (auto i : dict_ConnectivityTree) {
        delete ref_max_dict[i.first];
        delete dict_ConnectivityTree[i.first];
    }
}

TEST_F(MGridGen_Dual_Graph, remove_separating_vertex_all) {

    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, listOfFineCells);
    //    dict_ConnectivityTree = {0: [1], 2: [0, 6], 3: [1], 4: [5], 5: [2], 6: [2, 3], 7: [4, 8], 8: [5], 9: [7, 10], 10: [8, 13], 11: [6, 14], 12: [9], 13: [11]}
    ASSERT_EQ(12, max_seed);
    long new_seed = 12;

    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    unordered_set<long> ref_s_fc({0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13, 14});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Edge_7_Dual_Graph, remove_separating_vertex_Case_2) {

    vector<long> listOfFineCells = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    long new_seed = 1;
    unordered_map<long, queue<long> *> d_spanning_tree;
    d_spanning_tree[0] = new queue<long>({5});
    d_spanning_tree[1] = new queue<long>({0, 2});
    d_spanning_tree[2] = new queue<long>({3, 4});
    d_spanning_tree[5] = new queue<long>({6});

    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6});
    (*g).remove_separating_vertex(new_seed, d_spanning_tree, s_fc);

    unordered_set<long> ref_s_fc({0, 1, 2, 3, 5, 6});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(MGridGen_Dual_Graph, remove_separating_vertex_205) {

    vector<long> v_fc = {2, 0, 5};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(5, max_seed);

    long new_seed = 0;
    unordered_map<long, queue<long> *> dict_SpanningTree;
    dict_SpanningTree[0] = new queue<long>({2});
    dict_SpanningTree[2] = new queue<long>({5});

    unordered_set<long> s_fc({2, 0, 5});
    (*g).remove_separating_vertex(new_seed, dict_SpanningTree, s_fc);

    unordered_set<long> ref_s_fc({2, 0, 5});
    ASSERT_EQ(ref_s_fc, s_fc);
}


TEST_F(MGridGen_Dual_Graph, remove_separating_vertex_136111314) {

    vector<long> v_fc = {1, 3, 6, 11, 13, 14};
    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(13, max_seed);

    long new_seed = 1;

    unordered_set<long> s_fc({1, 3, 6, 11, 13, 14});
    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    unordered_set<long> ref_s_fc({1, 3, 6, 13, 14});
    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(MGridGen_Dual_Graph, remove_separating_vertex_10265813119) {

    vector<long> v_fc = {10, 2, 6, 5, 8, 13, 11, 9};
    unordered_set<long> s_fc({10, 2, 6, 5, 8, 13, 11, 9});
    unordered_set<long> ref_s_fc({2, 5, 6, 8, 9, 10, 11, 13});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(9, max_seed);

    long new_seed = 1;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}


TEST_F(Edge_9_3_Dual_Graph, remove_separating_vertex_10265813119) {

    vector<long> v_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8});
    unordered_set<long> ref_s_fc({0, 1, 2, 5, 6, 7, 8});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(7, max_seed);

    long new_seed = 7;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Edge_9_Dual_Graph, remove_separating_vertex_Case_5_random) {

    vector<long> v_fc = {5, 0, 3, 4, 2, 7, 1, 6, 8};
    unordered_set<long> s_fc({6, 7, 1, 3, 5, 4, 0, 2, 8});
    unordered_set<long> ref_s_fc({0, 1, 3, 5, 6, 7, 8});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(7, max_seed);

    long new_seed = 7;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Example_6_Dual_Graph, remove_separating_vertex_Case_6) {

    vector<long> v_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    unordered_set<long> s_fc({6, 7, 1, 3, 5, 4, 0, 2, 8});
    unordered_set<long> ref_s_fc({1, 3, 4, 5, 6, 7, 8});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(4, max_seed);

    long new_seed = 4;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Edge_8_Dual_Graph, remove_separating_vertex_Case_4_random) {

    vector<long> v_fc = {5, 0, 3, 4, 2, 7, 1, 6};
    unordered_set<long> s_fc({6, 7, 1, 3, 5, 4, 0, 2});
    unordered_set<long> ref_s_fc({0, 1, 2, 3, 5, 6, 7});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(7, max_seed);

    long new_seed = 7;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Example_7_Dual_Graph, remove_separating_vertex_Case_7_seed_0) {

    vector<long> v_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    unordered_set<long> ref_s_fc({0, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(5, max_seed);

    long new_seed = 5;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Example_7_Dual_Graph, remove_separating_vertex_Case_7_seed_1) {

    vector<long> v_fc = {1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    unordered_set<long> ref_s_fc({0, 1, 3, 4, 5, 6, 7, 8, 9, 10});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(3, max_seed);

    long new_seed = 3;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Example_7_Dual_Graph, remove_separating_vertex_Case_7_seed_9) {

    vector<long> v_fc = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};
    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    unordered_set<long> ref_s_fc({0, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(5, max_seed);

    long new_seed = 5;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Example_8_Dual_Graph, remove_separating_vertex_Case_8_random) {

    vector<long> v_fc = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};
    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    unordered_set<long> ref_s_fc({1, 2, 3, 4, 5, 6, 7, 10});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(7, max_seed);

    long new_seed = 7;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

TEST_F(Example_9_Dual_Graph, remove_separating_vertex_Case_9_random) {

    vector<long> v_fc = {9, 1, 0, 2, 3, 4, 5, 6, 7, 8, 10};
    unordered_set<long> s_fc({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    unordered_set<long> ref_s_fc({0, 1, 2, 3, 4, 6, 8, 9, 10});

    long max_seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(max_seed, v_fc);
    ASSERT_EQ(9, max_seed);

    long new_seed = 9;

    (*g).remove_separating_vertex(new_seed, dict_ConnectivityTree, s_fc);

    ASSERT_EQ(ref_s_fc, s_fc);
}

//TODO rename compute_neighbourhood_of_cc to compute_neighbourhood
TEST_F(MGridGen_Dual_Graph, compute_neighbourhood_of_cc_MGridGen_case_0) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated);

    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1,  1},
                                                                 {2,  1},
                                                                 {3,  2},
                                                                 {4,  3},
                                                                 {5,  2},
                                                                 {6,  2},
                                                                 {8,  3},
                                                                 {11, 3}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(4, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_neighbourhood_of_cc_MGridGen_case_1) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0, 1};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{2,  1},
                                                                 {3,  1},
                                                                 {4,  3},
                                                                 {5,  2},
                                                                 {6,  2},
                                                                 {8,  3},
                                                                 {11, 3}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(4, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_neighbourhood_of_cc_MGridGen_case_2) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0, 1, 2, 3, 6};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{4,  2},
                                                                 {5,  1},
                                                                 {7,  3},
                                                                 {8,  2},
                                                                 {10, 3},
                                                                 {11, 1},
                                                                 {13, 2},
                                                                 {14, 2}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(4, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_of_neighbourhood_of_cc_case_3_cc_not_connected) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0, 1, 11};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated
    );
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{2,  1},
                                                                 {3,  1},
                                                                 {4,  3},
                                                                 {5,  2},
                                                                 {6,  1},
                                                                 {8,  3},
                                                                 {9,  3},
                                                                 {10, 2},
                                                                 {13, 1},
                                                                 {14, 1}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(4, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_of_neighbourhood_of_cc_w_constraints) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_set<long> s_of_constrained_fc = {9, 4, 7, 14, 2, 0, 12, 13, 5, 6, 1, 11, 3, 8, 10};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated, s_of_constrained_fc);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1,  1},
                                                                 {2,  1},
                                                                 {3,  2},
                                                                 {4,  3},
                                                                 {5,  2},
                                                                 {6,  2},
                                                                 {8,  3},
                                                                 {11, 3}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(4, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_of_neighbourhood_of_cc_w_constraints_11) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_set<long> s_of_constrained_fc = {9, 4, 7, 2, 0, 12, 13, 5, 6, 1, 3, 8, 10};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated, s_of_constrained_fc);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1, 1},
                                                                 {2, 1},
                                                                 {3, 2},
                                                                 {4, 3},
                                                                 {5, 2},
                                                                 {6, 2},
                                                                 {8, 3}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(4, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_of_neighbourhood_of_cc_w_constraints_118) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_set<long> s_of_constrained_fc = {9, 4, 7, 2, 0, 12, 13, 5, 6, 1, 3, 10};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated, s_of_constrained_fc);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1, 1},
                                                                 {2, 1},
                                                                 {3, 2},
                                                                 {4, 3},
                                                                 {5, 2},
                                                                 {6, 2},
                                                                 {7, 4}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(5, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_of_neighbourhood_of_cc_w_constraints_118_max_card) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 10;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_set<long> s_of_constrained_fc = {9, 4, 7, 2, 0, 12, 13, 5, 6, 1, 3, 10};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated, s_of_constrained_fc);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1,  1},
                                                                 {2,  1},
                                                                 {3,  2},
                                                                 {4,  3},
                                                                 {5,  2},
                                                                 {6,  2},
                                                                 {7,  4},
                                                                 {9,  5},
                                                                 {10, 6},
                                                                 {12, 6}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(7, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_of_neighbourhood_of_cc_w_constraints_1184) {
//    Not enough cells due to restriction.

    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_set<long> s_of_constrained_fc = {9, 7, 2, 0, 12, 13, 5, 6, 1, 3, 10};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated, s_of_constrained_fc);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1, 1},
                                                                 {2, 1},
                                                                 {3, 2},
                                                                 {5, 2},
                                                                 {6, 2}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(3, number_of_order_of_neighbourhood);
}

TEST_F(MGridGen_Dual_Graph, compute_of_neighbourhood_of_cc_w_constraints_13) {


    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_set<long> s_of_constrained_fc = {9, 4, 7, 14, 2, 0, 12, 13, 5, 6, 11, 8, 10};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated, s_of_constrained_fc);
    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{2,  1},
                                                                 {4,  3},
                                                                 {5,  2},
                                                                 {6,  2},
                                                                 {7,  4},
                                                                 {8,  3},
                                                                 {10, 4},
                                                                 {11, 3},
                                                                 {13, 4},
                                                                 {14, 4}
    };
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    ASSERT_EQ(5, number_of_order_of_neighbourhood);
}

TEST_F(Graphs_biconnected, compute_Local_CRS_Subgraph_From_Global) {

    // Test 1
    //=======

    unordered_set<long> s_of_node = {0, 1, 2, 3, 4, 5, 6, 7};
    vector<long> row_ptr_l;
    vector<long> col_ind_l;
    vector<double> values_l;
    vector<long> g_to_l;
    vector<long> l_to_g;
    (*g).compute_local_crs_subgraph_from_global_crs(s_of_node, row_ptr_l,
                                                    col_ind_l,
                                                    values_l,
                                                    g_to_l,
                                                    l_to_g);
    vector<long> ref_col_ind_l = {1, 4, 5, 0, 2, 4, 5, 1, 3, 6, 7, 2, 6, 7, 0, 1, 5, 0, 1, 4, 6, 2, 3, 5, 7, 2, 3, 6};
    vector<double> ref_values_l = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    vector<long> ref_g_to_l = {7, 6, 5, 4, 3, 2, 1, 0};
    vector<long> ref_l_to_g = {7, 6, 5, 4, 3, 2, 1, 0};

    ASSERT_EQ((*g)._m_CRS_Row_Ptr, row_ptr_l);
    ASSERT_EQ(ref_col_ind_l, col_ind_l);
    ASSERT_EQ(ref_values_l, values_l);
    ASSERT_EQ(ref_g_to_l, g_to_l);
    ASSERT_EQ(ref_l_to_g, l_to_g);

    // Test 2
    //=======
    s_of_node = {0, 1, 2, 3};
    (*g).compute_local_crs_subgraph_from_global_crs(s_of_node, row_ptr_l,
                                                    col_ind_l,
                                                    values_l,
                                                    g_to_l,
                                                    l_to_g
    );
    vector<long> ref_row_ptr_l = {0, 1, 3, 5, 6};
    ref_col_ind_l = {1, 0, 2, 1, 3, 2};
    ref_values_l = {1, 1, 1, 1, 1, 1};
    ref_g_to_l = {3, 2, 1, 0, -1, -1, -1, -1};
    ref_l_to_g = {3, 2, 1, 0};
    for (int i = 0; i < s_of_node.size(); i++) {
        ASSERT_EQ(i, g_to_l[l_to_g[i]]);
    }
    ASSERT_EQ(ref_row_ptr_l, row_ptr_l);
    ASSERT_EQ(ref_col_ind_l, col_ind_l);
    ASSERT_EQ(ref_values_l, values_l);
    ASSERT_EQ(ref_g_to_l, g_to_l);
    ASSERT_EQ(ref_l_to_g, l_to_g);

    // Test 3
    //=======

    s_of_node = {0, 1, 4, 5};
    (*g).compute_local_crs_subgraph_from_global_crs(s_of_node, row_ptr_l,
                                                    col_ind_l,
                                                    values_l,
                                                    g_to_l, l_to_g);
    ref_row_ptr_l = {0, 3, 6, 9, 12};
    ref_col_ind_l = {1, 2, 3, 0, 2, 3, 0, 1, 3, 0, 1, 2};
    ref_values_l = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};
    ref_g_to_l = {3, 2, -1, -1, 1, 0, -1, -1};
    ref_l_to_g = {5, 4, 1, 0};

    ASSERT_EQ(ref_row_ptr_l, row_ptr_l);
    ASSERT_EQ(ref_col_ind_l, col_ind_l);
    ASSERT_EQ(ref_values_l, values_l);
    ASSERT_EQ(ref_g_to_l, g_to_l);
    ASSERT_EQ(ref_l_to_g, l_to_g);
    for (int i = 0; i < s_of_node.size(); i++) {
        ASSERT_EQ(i, g_to_l[l_to_g[i]]);
    }

    // Test 4
    //=======
    s_of_node = {7, 6, 2, 3};
    (*g).compute_local_crs_subgraph_from_global_crs(s_of_node, row_ptr_l,
                                                    col_ind_l,
                                                    values_l,
                                                    g_to_l,
                                                    l_to_g);
    ref_row_ptr_l = {0, 3, 6, 9, 12};
    ref_col_ind_l = {3, 2, 1, 3, 2, 0, 3, 0, 1, 2, 0, 1};
    ref_values_l = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};
    ref_g_to_l = {-1, -1, 1, 0, -1, -1, 2, 3};
    ref_l_to_g = {3, 2, 6, 7};

    ASSERT_EQ(ref_row_ptr_l, row_ptr_l);
    ASSERT_EQ(ref_col_ind_l, col_ind_l);
    ASSERT_EQ(ref_values_l, values_l);
    ASSERT_EQ(ref_g_to_l, g_to_l);
    ASSERT_EQ(ref_l_to_g, l_to_g);
    for (int i = 0; i < s_of_node.size(); i++) {
        ASSERT_EQ(i, g_to_l[l_to_g[i]]);
    }
    // Test 5
    //=======
    s_of_node = {7};
    (*g).compute_local_crs_subgraph_from_global_crs(s_of_node, row_ptr_l,
                                                    col_ind_l,
                                                    values_l,
                                                    g_to_l,
                                                    l_to_g);
    ref_row_ptr_l = {0, 1};
    ref_col_ind_l = {};
    ref_values_l = {};
    ref_g_to_l = {-1, -1, -1, -1, -1, -1, -1, 0};
    ref_l_to_g = {7};

    ASSERT_EQ(ref_row_ptr_l, row_ptr_l);
    ASSERT_EQ(ref_col_ind_l, col_ind_l);
    ASSERT_EQ(ref_values_l, values_l);
    ASSERT_EQ(ref_g_to_l, g_to_l);
    ASSERT_EQ(ref_l_to_g, l_to_g);
}

TEST_F(MGridGen_Dual_Graph, clean_d_neighbours_of_seed_case_0) {
    // Case 0: seed =0
    // and s_fc =  {0, 1, 2, 5}
    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated);

    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1,  1},
                                                                 {2,  1},
                                                                 {3,  2},
                                                                 {5,  2},
                                                                 {6,  2},
                                                                 {4,  3},
                                                                 {8,  3},
                                                                 {11, 3}};
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    unordered_set<long> s_fc = {0, 1, 2, 5};
    (*g).clean_d_neighbours_of_seed(s_fc, d_neighbours_of_seed);

    ref_dict_fc_n_of_seed = {{3, 2},
                             {6, 2},
                             {4, 2},
                             {8, 2}};
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);

}

TEST_F(MGridGen_Dual_Graph, clean_d_neighbours_of_seed_case_1) {
    // Case 1: seed =0
    // and s_fc =  {0, 1, 2, 3, 6}
    vector<bool> a_is_fc_agglomerated(15, false);

    unsigned short goal_card = 8;
    unsigned short number_of_order_of_neighbourhood = 3;
    unordered_set<long> s_seeds = {0};
    unordered_map<long, unsigned short> d_neighbours_of_seed = {};
    (*g).compute_neighbourhood_of_cc(s_seeds,
                                     number_of_order_of_neighbourhood,
                                     d_neighbours_of_seed,
                                     goal_card,
                                     a_is_fc_agglomerated);

    unordered_map<long, unsigned short> ref_dict_fc_n_of_seed = {{1,  1},
                                                                 {2,  1},
                                                                 {3,  2},
                                                                 {5,  2},
                                                                 {6,  2},
                                                                 {4,  3},
                                                                 {8,  3},
                                                                 {11, 3}};
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);
    unordered_set<long> s_fc = {0, 1, 2, 3, 6};
    (*g).clean_d_neighbours_of_seed(s_fc, d_neighbours_of_seed);

    ref_dict_fc_n_of_seed = {{5,  2},
                             {11, 2}};
    ASSERT_EQ(ref_dict_fc_n_of_seed, d_neighbours_of_seed);

}