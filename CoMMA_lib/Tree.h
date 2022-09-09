#ifndef COMMA_PROJECT_TREE_H
#define COMMA_PROJECT_TREE_H

/*
 *  * CoMMA
    * Copyright © 2022 ONERA
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

#include <vector>
#include <cstddef>
#include <memory>
#include <iostream>
#include <cassert>

using namespace std;

class Tree;

/** @brief Node data structure that represent a node of the tree*/
struct node {
  node(long index, double volume) : _index(index), _volume(volume) {};
  /** @brief index of the cell*/
  long _index;
  /** @brief volume*/
  double _volume;
  /** @brief number of son*/
  int _sonc = 0;
  /** @brief shared pointer to the father node */
  shared_ptr<node> _father;
  /** @brief shared pointer to the left element */
  shared_ptr<node> _left_idx;
  /** @brief shared pointer to the right element */
  shared_ptr<node> _right_idx;
  /** @brief shared pointer to the left element */
  shared_ptr<node> _left_son_idx;
};

/** @brief Tree structure that represent a coarse cell, the fine cell and the
 * neighbors to them*/
class Tree {
 public:
  /** @brief Constructor*/
  Tree(shared_ptr<node> &root);
  ~Tree() {};
  shared_ptr<node> _root;
  void insertSon(const long &father_index, const long &index,
                 const double &volume, const int &root);
  shared_ptr<node> search(shared_ptr<node> &node, const long &value);
  shared_ptr<node> transverse(shared_ptr<node> &node);
  void deleteNode(const long &value);
  void delete_node(shared_ptr<node> &searched_node, const long &value);
  void print();
  void print_nodes(shared_ptr<node> &node);
};

#endif  // COMMA_PROJECT_TREE_H
