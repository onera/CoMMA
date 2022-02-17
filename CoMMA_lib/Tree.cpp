#include "Tree.h"

Tree::Tree(shared_ptr<node> &root):_root(root){};

void Tree::insertSon(const long &father_index,const long &index, const double &volume, const int &root){
     shared_ptr<node> insertion(new node(index,volume));
     shared_ptr<node> u_p_father;
     if (root==1){u_p_father = _root;}
     else{u_p_father = search(_root->_left_son_idx,father_index);}
     assert(u_p_father!=nullptr);
     insertion->_father = u_p_father;
     shared_ptr<node> left_idx;
     left_idx = transverse(u_p_father->_left_son_idx);
     if (left_idx == nullptr){
        u_p_father -> _left_son_idx = insertion;
     } else {
        insertion->_left_idx = left_idx; 
        left_idx->_right_idx = insertion;
     }
     u_p_father->_sonc++;
};

shared_ptr<node> Tree::transverse(shared_ptr<node> &node){
    if (node == nullptr || node->_right_idx == nullptr) {return node;}
    return(transverse(node->_right_idx));
};

shared_ptr<node> Tree::search(shared_ptr<node> &node, const long &value){ 
    if (node->_index == value && node->_father!=nullptr) {return node;} 
    if (node == nullptr || node->_right_idx == nullptr) {return nullptr;}
    return(search(node->_right_idx,value));
};

void Tree::delete_node(shared_ptr<node> &searched_node){};

void Tree::print(){};

