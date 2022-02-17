#ifndef COMMA_PROJECT_TREE_H
#define COMMA_PROJECT_TREE_H


#include<vector>
#include<memory>
#include<iostream>

using namespace std;

class Tree;

/** @brief Node data structure that represent a node of the tree*/
struct node{
    node(long index, double volume):_index(index),_volume(volume){};
    /** @brief index of the cell*/
    long _index;
    /** @brief volume*/
    double _volume;
    /** @brief number of son*/
    int _sonc;
    // vector of the pointers to the son
    vector<shared_ptr<node>> _sons;
};



/** @brief Tree structure that represent a coarse cell, the fine cell and the neighbors to them*/
class Tree {
 public:
/** @brief Constructor*/
   Tree(const node &root);
   void insertSon(const long &index, const double &volume);
   void searchSon(const long &index, const double &volume);
   void deleteSon(const long &index, const double &volume);
   void print();
};


#endif //COMMA_PROJECT_TREE_H
