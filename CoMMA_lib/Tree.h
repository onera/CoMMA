#ifndef COMMA_PROJECT_TREE_H
#define COMMA_PROJECT_TREE_H


#include<vector>
#include<memory>
#include<iostream>

using namespace std;

class Tree;

struct node{
    node(long index):_index(index){};
    long _index;
    // number of son
    int sonc;
    // vector of the pointers to the son
    vector<shared_ptr<node>> sons;
};


class Tree {
 public:
   Tree(const node &root);
   void insertSon(const long &index, const double &volume);
   void searchSon(const long &index, const double &volume);
   void deleteSon(const long &index, const double &volume);
   void print();
};


#endif //COMMA_PROJECT_TREE_H
