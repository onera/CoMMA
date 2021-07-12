#include <iostream>
#include <cassert>
#include <unordered_set>
//#include "Agglomerator_anisotropic.h"
//#include "Agglomerator_Isotropic.h"
//#include "Agglomerator_Isotropic.h"
#include "Agglomerator.h"

//#include "Google_tests/tests/0_Inputs/MGridGen_Dual_Graph.h"
using namespace std;


int main() {
    cout << "Beginning of tests" << endl;

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


//    cout<<"\n\n\n"<<endl;
//    cout<<"=================================================================="<<endl;
//    cout<<"MGridGen_agglomerate_one_level"<<endl;
//
//    vector<long> v_MGridGen_row_ptr = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
//    vector<long> v_MGridGen_col_ind = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4, 7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13,
//                          11, 14};
//
//    vector<double> v_MGridGen_values = {6.82842712, 2., 2., 2., 4.23606798,
//                         1., 2., 4., 2.23606798, 2.23606798,
//                         1., 4., 2.23606798, 6.82842712, 2.,
//                         2., 2.23606798, 2., 2., 1.,
//                         2.23606798, 2.23606798, 4., 2., 2., 2., 1., 2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2., 3.16227766, 2., 6.,
//                         3.16227766, 3.16227766, 3.60555128,
//                         11., 3.16227766, 3.16227766, 8., 3.16227766, 7.};
//
//
//    int MGridGen_nb_c = 15;
//    int MGridGen_col_ind_size = 51;
//    vector<double> v_MGridGen_volumes = {2., 1., 2., 1., 2., 1., 2., 1., 0.5, 2., 1., 3., 3., 3., 1.5};
//
//    unordered_map<long, int> MGridGen_d_is_on_bnd{{0,  1},
//                                                  {1,  1},
//                                                  {3,  1},
//                                                  {4,  1},
//                                                  {12, 2},
//                                                  {13, 1},
//                                                  {14, 2}
//    };
//
//    unordered_set<long> s_MGridGen_is_on_corner = {};
//    unordered_set<long> s_MGridGen_is_on_ridge = {12, 14};
//    unordered_set<long> s_MGridGen_is_on_valley = {0, 1, 3, 4, 13};
//
//    Dual_Graph* g = new Dual_Graph(MGridGen_nb_c,
//                       v_MGridGen_row_ptr,
//                       v_MGridGen_col_ind,
//                       v_MGridGen_values,
//                       v_MGridGen_volumes,
//                       MGridGen_d_is_on_bnd,
//                       unordered_set<long>({}),
//                       unordered_set<long>({}),
//                       unordered_set<long>({}),
//                       unordered_set<long>({}),
//                       0,
//                       2);
//
//
//    unsigned short int verbose = 0;
//    bool is_visu_data_stored = true;
//    int dimension = 2;
//    bool checks = true;
//
//    Agglomerator agg = Agglomerator((*g),
//                                    verbose,
//                                    is_visu_data_stored,
//                                    dimension,
//                                    checks);
//    forward_list<deque<long> *> anisotropic_lines = {};
//    agg.agglomerate_one_level(false, 0, anisotropic_lines);
//    //ASSERT_EQ(4, agg.get_nb_cc());
//    cout<<"agg.get_nb_cc() "<< agg.get_nb_cc()<<endl;
//
//    vector<long> ref_fc_2_cc = {1, 1, 1, 1, 2, 2, 1, 3, 2, 3, 2, 0, 3, 0, 0};
//    for (long i: agg.get_fc_2_cc())
//    {
//      cout<<i<<", ";
//    }
//    delete g;
//    cout<<"=================================================================="<<endl;

//    test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_8();
//    test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_9();
//    test_agglomerateOneLevel_Box_5x5x5_Anisotropic();
//
//    cout<<"\ntest_agglomerateOneLevel_MGridGen"<<endl;
//    test_agglomerateOneLevel_MGridGen();
//    test_agglomerateOneLevel_disconnected();

//    cout<<"Tout s'est bien fini!"<<endl;

    cout << "\n\n\n" << endl;
    cout << "==================================================================" << endl;
    cout << "box_5x5x5_iso_and_aniso" << endl;

    vector<long> v_row_ptr = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62,
                              66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132,
                              138, 144, 150, 155, 161, 167, 172, 177, 183, 189, 194, 200, 206,
                              212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278,
                              282, 287, 293, 299, 304, 309, 315, 321, 326, 330, 335, 340, 344};
    vector<long> v_col_ind = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7,
                              19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10,
                              22, 3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25,
                              6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28, 9, 12,
                              13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32,
                              1, 16, 17, 18, 21, 33, 2, 17, 18, 19, 22, 34, 3, 18, 19, 23, 35,
                              4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26,
                              38, 7, 19, 22, 23, 27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26,
                              29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24, 28,
                              29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30,
                              31, 47, 16, 32, 33, 36, 48, 17, 32, 33, 34, 37, 49, 18, 33, 34, 35,
                              38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38,
                              41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40,
                              41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39,
                              42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42,
                              45, 46, 47, 62, 31, 43, 46, 47, 63, 32, 48, 49, 52, 33, 48, 49, 50,
                              53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49,
                              52, 53, 54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52,
                              56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58, 59, 62, 43, 55,
                              58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63,
                              47, 59, 62, 63};

    vector<double> v_values = {30., 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 27.5, 2.5, 2.5, 25., 2.5, 30., 2.5, 25., 2.5, 27.5, 2.5,
                               2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 27.5,
                               2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 30.,
                               2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 27.5, 2.5, 25., 2.5, 2.5, 30., 25., 25., 5., 2.5, 2.5, 25., 25.,
                               2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                               25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                               2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5,
                               5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 5., 25., 25., 5.,
                               2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5,
                               2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25.,
                               25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5,
                               2.5, 25., 25., 2.5, 5., 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5, 2.5, 2.5, 25., 25., 2.5, 2.5,
                               5., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25.,
                               50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                               50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                               75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25., 75.};


    int nb_c = 64;
    int col_ind_size = 344;
    vector<double> v_volumes = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                                125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                                125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                                125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                                125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                                125., 125., 125., 125., 125., 125., 125., 125., 125.};

    int box_5x5x5_is_on_bnd[64] = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3, 2, 1, 1, 2, 1, 0,
                                   0, 1, 1, 0, 0, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1,
                                   2, 1, 1, 2, 3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};

    unordered_map<long, int> box_5x5x5_d_is_on_bnd;
    for (int i = 0; i < nb_c; i++) {
        if (box_5x5x5_is_on_bnd[i] > 0) {
            box_5x5x5_d_is_on_bnd[i] = box_5x5x5_is_on_bnd[i];
        }
    }


    Dual_Graph *g = new Dual_Graph(nb_c,
                                   v_row_ptr,
                                   v_col_ind,
                                   v_values,
                                   v_volumes,
                                   box_5x5x5_d_is_on_bnd,
                                   unordered_set<long>({}),
                                   unordered_set<long>({}),
                                   unordered_set<long>({}),
                                   unordered_set<long>({}),
                                   0,
                                   2);


    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    forward_list<deque<long> *> anisotropic_lines = {};
    agg.agglomerate_one_level(false, 0, anisotropic_lines, "triconnected");
    //ASSERT_EQ(4, agg.get_nb_cc());
    cout << "agg.get_nb_cc() " << agg.get_nb_cc() << endl;

    vector<long> ref_fc_2_cc = {1, 1, 1, 1, 2, 2, 1, 3, 2, 3, 2, 0, 3, 0, 0};
    for (long i: agg.get_fc_2_cc()) {
        cout << i << ", ";
    }
    delete g;
    cout << "==================================================================" << endl;

    return 0;
}
