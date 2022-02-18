#include "Tree.h"

Tree::Tree(shared_ptr<node> &root):_root(root){};

void Tree::insertSon(const long &father_index,const long &index, const double &volume, const int &root){
     shared_ptr<node> insertion(new node(index,volume));
     shared_ptr<node> u_p_father;
     if (root==1){u_p_father = _root;
     assert(u_p_father->_index == father_index);}
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
     u_p_father->_sonc = u_p_father->_sonc + 1;
     cout<<u_p_father->_sonc<<endl;
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

void Tree::deleteNode(const long &value){
     delete_node(_root->_left_son_idx,value);
};


void Tree::delete_node(shared_ptr<node> &searched_node,const long &value){
     if (searched_node == nullptr){return;}
     if(searched_node->_index==value){
            // case 0: leftest node
            if (searched_node->_left_idx == nullptr){ 
               searched_node->_father->_sonc--;
               searched_node->_father->_left_son_idx = searched_node->_right_idx;
               searched_node->_right_idx->_left_idx = nullptr;
               }
            // case 1: rightest node
            else if (searched_node->_right_idx == nullptr){
               searched_node->_father->_sonc = searched_node->_father->_sonc-1; 
               searched_node->_left_idx->_right_idx.reset();
               }
           else{ 
               searched_node->_father->_sonc--;
               searched_node->_left_idx->_right_idx = searched_node->_right_idx;
               searched_node->_right_idx->_left_idx = searched_node->_left_idx;
              }
     return;
     } 
     delete_node(searched_node->_right_idx,value);
};

void Tree::print(){
     print_nodes(_root);
};


void Tree::print_nodes(shared_ptr<node> &node){
     if (node==nullptr) {return;}
     cout<<"node"<<node->_index<<endl;
     print_nodes(node->_left_son_idx);
     print_nodes(node->_right_idx);
};

