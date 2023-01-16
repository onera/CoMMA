#ifndef COMMA_PROJECT_TREE_H
#define COMMA_PROJECT_TREE_H

/*
 *  * CoMMA
    * Copyright © 2023 ONERA
    *
    * Authors and Architects : Alberto Remigi and Nicolas Lantos
    *
    * Contributors : Riccardo Milani
    *
    * This program is free software: you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation, either version 3 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
    * You should have received a copy of the GNU General Public License
    * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Node;
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Tree;

using namespace std;

/** @brief Node data structure that represent a node of the tree
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Node {
  public:
  Node(CoMMAIndexType index, CoMMAWeightType volume) : _index(index), _volume(volume) {};
  /** @brief index of the cell*/
  CoMMAIndexType _index;
  /** @brief volume*/
  CoMMAWeightType _volume;
  /** @brief number of son*/
  CoMMAIntType _sonc = 0;
  /** @brief shared pointer to the father node */
  shared_ptr<Node> _father;
  /** @brief shared pointer to the left element */
  shared_ptr<Node> _left_idx;
  /** @brief shared pointer to the right element */
  shared_ptr<Node> _right_idx;
  /** @brief shared pointer to the left element */
  shared_ptr<Node> _left_son_idx;
};

/** @brief Tree structure that represent a coarse cell, the fine cell and the
 * neighbours to them
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Tree {
 public:
  /** @brief Type of node for the current tree */
  using NodeType = Node<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;

  /** @brief Constructor*/
  Tree(shared_ptr<NodeType> &root) : _root(root) {}

  ~Tree() {};

  shared_ptr<NodeType> _root;

  void insertSon(const CoMMAIndexType &father_index, const CoMMAIndexType &index,
                 const CoMMAWeightType &volume, const CoMMAIntType &root) {
    shared_ptr<NodeType> insertion(new NodeType(index, volume));
    shared_ptr<NodeType> u_p_father;
    if (root == 1) {
      u_p_father = _root;
      assert(u_p_father->_index == father_index);
    } else {
      u_p_father = search(_root->_left_son_idx, father_index);
    }
    assert(u_p_father != nullptr);
    insertion->_father = u_p_father;
    shared_ptr<NodeType> left_idx;
    left_idx = transverse(u_p_father->_left_son_idx);
    if (left_idx == nullptr) {
      u_p_father->_left_son_idx = insertion;
    } else {
      insertion->_left_idx = left_idx;
      left_idx->_right_idx = insertion;
    }
    u_p_father->_sonc = u_p_father->_sonc + 1;
    //cout << u_p_father->_sonc << endl;
  }
  shared_ptr<NodeType> search(shared_ptr<NodeType> &node, const CoMMAIndexType &value) {
    if (node->_index == value && node->_father != nullptr) {
      return node;
    }
    if (node == nullptr || node->_right_idx == nullptr) {
      return nullptr;
    }
    return (search(node->_right_idx, value));
  }

  shared_ptr<NodeType> transverse(shared_ptr<NodeType> &node) {
    if (node == nullptr || node->_right_idx == nullptr) {
      return node;
    }
    return (transverse(node->_right_idx));
  }

  void deleteNode(const CoMMAIndexType &value) {delete_node(_root->_left_son_idx, value);}

  void delete_node(shared_ptr<NodeType> &searched_node, const CoMMAIndexType &value) {
    if (searched_node == nullptr) {
      return;
    }
    if (searched_node->_index == value) {
      // case 0: leftest node
      if (searched_node->_left_idx == nullptr) {
        searched_node->_father->_sonc--;
        searched_node->_father->_left_son_idx = searched_node->_right_idx;
        searched_node->_right_idx->_left_idx = nullptr;
      }
      // case 1: rightest node
      else if (searched_node->_right_idx == nullptr) {
        searched_node->_father->_sonc = searched_node->_father->_sonc - 1;
        searched_node->_left_idx->_right_idx.reset();
      } else {
        searched_node->_father->_sonc--;
        searched_node->_left_idx->_right_idx = searched_node->_right_idx;
        searched_node->_right_idx->_left_idx = searched_node->_left_idx;
      }
      return;
    }
    delete_node(searched_node->_right_idx, value);
  }

  void print() { print_nodes(_root); }

  void print_nodes(shared_ptr<NodeType> &node) {
    if (node == nullptr) {
      return;
    }
    //cout << "node" << node->_index << endl;
    print_nodes(node->_left_son_idx);
    print_nodes(node->_right_idx);
  }
};

#endif  // COMMA_PROJECT_TREE_H
