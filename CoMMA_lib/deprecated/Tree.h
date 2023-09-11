#ifndef COMMA_PROJECT_TREE_H
#define COMMA_PROJECT_TREE_H

/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

/** @brief Node data structure that represent a node of the tree
 *  @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 *  @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 *  @tparam CoMMAIntType the CoMMA type for integers
 *  @deprecated Not used anymore
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Node {
  public:
  Node(CoMMAIndexType index, CoMMAWeightType volume) : _index(index), _volume(volume) {};
  /** @brief Index of the cell*/
  CoMMAIndexType _index;
  /** @brief Volume*/
  CoMMAWeightType _volume;
  /** @brief Number of son*/
  CoMMAIntType _sonc = 0;
  /** @brief Shared pointer to the father node */
  shared_ptr<Node> _father;
  /** @brief Shared pointer to the left element */
  shared_ptr<Node> _left_idx;
  /** @brief Shared pointer to the right element */
  shared_ptr<Node> _right_idx;
  /** @brief Shared pointer to the left element */
  shared_ptr<Node> _left_son_idx;
};

/** @brief Tree structure that represent a coarse cell, the fine cell and the
 * neighbours to them
 *  @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 *  @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 *  @tparam CoMMAIntType the CoMMA type for integers
 *  @deprecated Not used anymore
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Tree {
 public:
  /** @brief Type of node for the current tree */
  using NodeType = Node<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;

  /** @brief Constructor
   *  @param[in] root A Node which is the root of the Tree
   */
  Tree(shared_ptr<NodeType> &root) : _root(root) {}

  /** @brief Destructor */
  ~Tree() {};

  /** @brief The Node at the root of the tree */
  shared_ptr<NodeType> _root;

  /** @brief Insert a node as child of a given node
   *  @param[in] father_index Index of the parent node
   *  @param[in] index Index of the child
   *  @param[in] volume Volume of the child
   *  @param[in] root Whether it is at the root
   */
  void insertSon(const CoMMAIndexType &father_index, const CoMMAIndexType &index,
                 const CoMMAWeightType &volume, const CoMMAIntType &root) {
    shared_ptr<NodeType> insertion = make_shared<NodeType>(index, volume);
    auto u_p_father = root == 1 ? _root :
                                  search(_root->_left_son_idx, father_index);
    assert(u_p_father != nullptr);
    insertion->_father = u_p_father;
    auto left_idx = transverse(u_p_father->_left_son_idx);
    if (left_idx == nullptr) {
      u_p_father->_left_son_idx = insertion;
    } else {
      insertion->_left_idx = left_idx;
      left_idx->_right_idx = insertion;
    }
    u_p_father->_sonc = u_p_father->_sonc + 1;
    //cout << u_p_father->_sonc << endl;
  }

  /** @brief Look for a node
   *  @param[in] node Starting point
   *  @param[in] value Target
   *  @return a pointer (possibly null) to the target
   */
  shared_ptr<NodeType> search(shared_ptr<NodeType> &node, const CoMMAIndexType &value) {
    if (node->_index == value && node->_father != nullptr) {
      return node;
    }
    if (node == nullptr || node->_right_idx == nullptr) {
      return nullptr;
    }
    return (search(node->_right_idx, value));
  }

  /** @brief Traverse the tree
   *  @param[in] node Where to start the search
   *  @return a pointer to a leaf
   */
  shared_ptr<NodeType> transverse(shared_ptr<NodeType> &node) {
    if (node == nullptr || node->_right_idx == nullptr) {
      return node;
    }
    return (transverse(node->_right_idx));
  }

  /** @brief Delete a node
   *  @param[in] value Target
   */
  void deleteNode(const CoMMAIndexType &value) {delete_node(_root->_left_son_idx, value);}

  /** @brief Delete a node
   *  @param[in] searched_node Where to look
   *  @param[in] value Target
   */
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

  /** @brief Print the tree */
  void print() { print_nodes(_root); }

  /** @brief Print the branches starting from a given node
   *  @param[in] node Where to start
   */
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
