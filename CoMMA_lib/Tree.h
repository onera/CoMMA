#ifndef COMMA_PROJECT_TREE_H
#define COMMA_PROJECT_TREE_H


#include<vector>
#include<cstddef>
#include<memory>
#include<iostream>
#include <cassert>

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
    /** @brief shared pointer to the father node */
    shared_ptr<node> _father;
    /** @brief shared pointer to the left element */ 
    shared_ptr<node> _left_idx;
    /** @brief shared pointer to the right element */
    shared_ptr<node> _right_idx;
    /** @brief shared pointer to the left element */ 
    shared_ptr<node> _left_son_idx;
};



/** @brief Tree structure that represent a coarse cell, the fine cell and the neighbors to them*/
class Tree {
 public:
/** @brief Constructor*/
   Tree(shared_ptr<node> &root);
   shared_ptr<node> _root;
   void insertSon(const long &father_index, const long &index, const double &volume,const int &root);
   shared_ptr<node> search(shared_ptr<node> &node,const long &value);
   shared_ptr<node> transverse(shared_ptr<node> &node);
   void delete_node(shared_ptr<node> &searched_node);
   void print();
};


#endif //COMMA_PROJECT_TREE_H
