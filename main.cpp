#include <iostream>
#include <cassert>
#include <unordered_set>
#include "Agglomerator_anisotropic.h"
#include "Agglomerator_Isotropic.h"
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
//    test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_8();
//    test_agglomerateOneLevel_Box_5x5x5_Isotropic_goalCard_9();
//    test_agglomerateOneLevel_Box_5x5x5_Anisotropic();
//
//    cout<<"\ntest_agglomerateOneLevel_MGridGen"<<endl;
//    test_agglomerateOneLevel_MGridGen();
//    test_agglomerateOneLevel_disconnected();

    cout<<"Tout s'est bien fini!"<<endl;
    return 0;
}

