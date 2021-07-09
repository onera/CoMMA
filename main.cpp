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
    cout<<"Beginning of tests"<<endl;

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


    cout<<"\n\n\n"<<endl;
    cout<<"=================================================================="<<endl;
    cout<<"test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_8"<<endl;

    vector<long> v_MGridGen_row_ptr = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    vector<long> v_MGridGen_col_ind = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4, 7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13,
                          11, 14};

    vector<double> v_MGridGen_values = {6.82842712, 2., 2., 2., 4.23606798,
                         1., 2., 4., 2.23606798, 2.23606798,
                         1., 4., 2.23606798, 6.82842712, 2.,
                         2., 2.23606798, 2., 2., 1.,
                         2.23606798, 2.23606798, 4., 2., 2., 2., 1., 2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2., 3.16227766, 2., 6.,
                         3.16227766, 3.16227766, 3.60555128,
                         11., 3.16227766, 3.16227766, 8., 3.16227766, 7.};


    int MGridGen_nb_c = 15;
    int MGridGen_col_ind_size = 51;
    vector<double> v_MGridGen_volumes = {2., 1., 2., 1., 2., 1., 2., 1., 0.5, 2., 1., 3., 3., 3., 1.5};

    unordered_map<long, int> MGridGen_d_is_on_bnd{{0,  1},
                                                  {1,  1},
                                                  {3,  1},
                                                  {4,  1},
                                                  {12, 2},
                                                  {13, 1},
                                                  {14, 2}
    };

    unordered_set<long> s_MGridGen_is_on_corner = {};
    unordered_set<long> s_MGridGen_is_on_ridge = {12, 14};
    unordered_set<long> s_MGridGen_is_on_valley = {0, 1, 3, 4, 13};

    Dual_Graph* g = new Dual_Graph(MGridGen_nb_c,
                       v_MGridGen_row_ptr,
                       v_MGridGen_col_ind,
                       v_MGridGen_values,
                       v_MGridGen_volumes,
                       MGridGen_d_is_on_bnd,
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
    agg.agglomerate_one_level(false, 0, anisotropic_lines);
    //ASSERT_EQ(4, agg.get_nb_cc());
    cout<<"agg.get_nb_cc() "<< agg.get_nb_cc()<<endl;

    vector<long> ref_fc_2_cc = {1, 1, 1, 1, 2, 2, 1, 3, 2, 3, 2, 0, 3, 0, 0};
    for (long i: agg.get_fc_2_cc())
    {
      cout<<i<<", ";
    }
    //cout<<"agg.get_nb_cc() "<< agg.get_nb_cc()<<endl;
    //ASSERT_EQ(ref_fc_2_cc, agg.get_fc_2_cc());
    delete g;
//    test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_8();
//    test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_9();
//    test_agglomerateOneLevel_Box_5x5x5_Anisotropic();
//
//    cout<<"\ntest_agglomerateOneLevel_MGridGen"<<endl;
//    test_agglomerateOneLevel_MGridGen();
//    test_agglomerateOneLevel_disconnected();

//    cout<<"Tout s'est bien fini!"<<endl;
    return 0;
}
