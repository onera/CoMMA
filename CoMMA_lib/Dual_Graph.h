#ifndef COMMA_PROJECT_DUAL_GRAPH_H
#define COMMA_PROJECT_DUAL_GRAPH_H

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

#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <vector>
#include <algorithm>
#include <limits>
#include <climits>
#include <functional>

#include "Queue.h"
#include "Seeds_Pool.h"

using namespace std;

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Subgraph;
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Dual_Graph;

/** @brief An interface class responsible of storing the cell centered dual
 * graph and of acting on it (it is an interface for the global Dual Graph and
 * the Subgraph)
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Graph {
 public:
  /** @brief Constructor of the class
   *  @param[in] m_crs_row_ptr the row pointer of the CRS representation
   *  @param[in] m_crs_col_ind the column index of the CRS representation
   *  @param[in] m_crs_value the weight of the CRS representation (in CoMMA case
   * will be the area of the faces that in the graph representation are the edges
   * between two nodes represented by the cell centers.
   *  @param[in] volumes The volumes of the cells
   */
  Graph(const CoMMAIndexType &nb_c, const vector<CoMMAIndexType> &m_crs_row_ptr,
        const vector<CoMMAIndexType> &m_crs_col_ind,
        const vector<CoMMAWeightType> &m_crs_values,
        const vector<CoMMAWeightType> &volumes)
      : _number_of_cells(nb_c),
        _m_CRS_Row_Ptr(m_crs_row_ptr),
        _m_CRS_Col_Ind(m_crs_col_ind),
        _m_CRS_Values(m_crs_values),
        _volumes(volumes) {
    _visited.resize(_number_of_cells);
    fill(_visited.begin(), _visited.end(), false);
  }

  ~Graph() {}
  /** @brief Number of nodes in the Graph (it corresponds to the number of cells
   * in the subgraph or the dual graph. */
  CoMMAIndexType _number_of_cells;

  /** @brief helper vector for the DFS*/
  vector<bool> _visited;

  /** @brief Vector of row pointer of CRS representation (member variable
   * different from the unordered
   * set passed as a reference in input) */
  vector<CoMMAIndexType> _m_CRS_Row_Ptr;

  /** @brief Vector of column index of CRS representation (member variable
   * different from the unordered
   * set passed as a reference in input) */
  vector<CoMMAIndexType> _m_CRS_Col_Ind;

  /** @brief Vector of area weight of CRS representation (member variable
   * different from the unordered
   * set passed as a reference in input) */
  vector<CoMMAWeightType> _m_CRS_Values;

  /** @brief Vector of volumes (member variable different from the unordered
   * set passed as a reference in input) */
  vector<CoMMAWeightType> _volumes;

  /** @brief Depth First Search (DFS) recursive function
   *  @param[in] i_fc index of the node to print*/
  void DFS(const CoMMAIndexType &i_fc) {
    _visited[i_fc] = true;
    vector<CoMMAIndexType> v_neigh;
    v_neigh = get_neighbours(i_fc);
    for (auto &it : v_neigh) {
      if (!_visited[it]) {
        DFS(it);
      }
    }
  }

  /** @brief Breadth First Search (BFS) function
   *  @param[in] root  root of the spanning tree
   */
  void BFS(const CoMMAIndexType &root) {
    Queue<CoMMAIndexType> coda;
    vector<CoMMAIndexType> v_neigh;
    vector<CoMMAIndexType> path;
    coda.push(root);
    vector<bool> visited(_number_of_cells, false);
    visited[root] = true;
    vector<CoMMAIndexType> prev(_number_of_cells, -1);
    while (!coda.empty()) {
      CoMMAIndexType node = coda.front();
      coda.pop();
      v_neigh = get_neighbours(node);
      for (auto &it : v_neigh) {
        if (!visited[it]) {
          coda.push(it);
          visited[it] = true;
          prev[it] = node;
        }
      }
    }
    // to print the inverse path
    CoMMAIndexType retro = prev[_number_of_cells - 1];
    while (retro != -1) {
      path.push_back(retro);
      retro = prev[retro];
    }
    reverse(path.begin(), path.end());
    //     for (CoMMAIntType i = 0; i < _number_of_cells; i++) {
    //            cout<<"BFS"<<path[i]<<endl;
    //     }
  }

  /** @brief Retrieve the number of neighbours
   *  @param[in] i_c index of the cell
   *  @return number of neighbors of the given cell.
   */
  inline CoMMAIntType get_nb_of_neighbours(CoMMAIndexType i_c) const {
    // Return the number of neighbours of the ith cell
    return _m_CRS_Row_Ptr[i_c + 1] - _m_CRS_Row_Ptr[i_c];
  }

  /** @brief Based on the CRS representation retrieves the neighbours of the
   * cell given as an input.
   * @param[in] i_c index of the cell to check the neighbours
   * @return vector of the neighbors.
   */
  vector<CoMMAIndexType> get_neighbours(const CoMMAIndexType &i_c) const {
    // given the index of a cell return the neighborhoods of this cell
    CoMMAIndexType ind = _m_CRS_Row_Ptr[i_c];
    CoMMAIndexType ind_p_one = _m_CRS_Row_Ptr[i_c + 1];
    // insert the values of the CRS_value from begin+ind (pointed to the face) till
    // the next pointed one, so related to all the connected areas (and hence to the
    // faces)
    vector<CoMMAIndexType> result(_m_CRS_Col_Ind.begin() + ind,
                                  _m_CRS_Col_Ind.begin() + ind_p_one);
    return result;
  }

  /** @brief Based on the area of the faces composing the cell given as an
   * input, we retrieve the faces connecting the given cell with the
   * neighborhood that can be described also as the weight of the graph
   * @return vector of weight associated to the cell.
   */
  vector<CoMMAWeightType> get_weights(const CoMMAIndexType &i_c) const {
    // Given the index of a cell, return the value of the faces connected
    CoMMAIndexType ind = _m_CRS_Row_Ptr[i_c];
    CoMMAIndexType ind_p_one = _m_CRS_Row_Ptr[i_c + 1];
    // insert the values of the CRS_value from begin+ind (pointed to the face) till
    // the next pointed one, so related to all the connected areas (and hence to the
    // faces)
    vector<CoMMAWeightType> result(_m_CRS_Values.begin() + ind,
                                   _m_CRS_Values.begin() + ind_p_one);
    return result;
  }

  /** @brief Check the connectivity of the given Graph.
   * @return True if the graph is connected, false if it is not connected
   */
  bool check_connectivity() {
    for (CoMMAIntType i = 0; i < _number_of_cells; i++) {
      _visited.push_back(false);
    }
    if (_number_of_cells == 1) {
      return (true);
    }
    DFS(_m_CRS_Col_Ind[0]);
    for (CoMMAIntType i = 0; i < _number_of_cells; i++) {
      if (_visited[i] == false) {
        return (false);
      }
    }
    return (true);
  }
};

/** @brief A class implementing the CRS subgraph representation. It is used in
 * the framework of CoMMA for the implementation of the CSR representation of
 * the coarse cells.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 */

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Subgraph : public Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
 public:
  /** @brief Constructor of the class
   *  @param[in] m_crs_row_ptr the row pointer of the CRS representation
   *  @param[in] m_crs_col_ind the column index of the CRS representation
   *  @param[in] m_crs_value the weight of the CRS representation (in CoMMA case
   * will be the area of the faces that in the graph representation are the edges
   * between two nodes represented by the cell centers.
   *  @param[in] volumes The volumes of the cells
   */
  Subgraph(const CoMMAIndexType &nb_c,
           const vector<CoMMAIndexType> &m_crs_row_ptr,
           const vector<CoMMAIndexType> &m_crs_col_ind,
           const vector<CoMMAWeightType> &m_crs_values,
           const vector<CoMMAWeightType> &volumes,
           const vector<CoMMAIndexType> &mapping_l_to_g,
           const bool &is_isotropic)
      : Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            nb_c, m_crs_row_ptr, m_crs_col_ind, m_crs_values, volumes),
        _is_isotropic(is_isotropic),
        _mapping_l_to_g(mapping_l_to_g) {
    // Definition degree
    CoMMAIndexType pos_old = 0;
    CoMMAIndexType degree = 0;
    for (const CoMMAIndexType &elem : m_crs_row_ptr) {
      degree = elem - pos_old;
      if (degree > _compactness) {
        _compactness = degree;
      }
      pos_old = elem;
    }
    // Definition cardinality
    _cardinality = nb_c;
  }

  ~Subgraph() {}

  /** @brief it originates from an isotropic cell.*/
  bool _is_isotropic;

  /** @brief Cardinality of the given subgraph, alias the number of nodes
   * contained*/
  CoMMAIntType _cardinality = 0;

  /** @brief Compactness of the given subgraph */
  CoMMAIntType _compactness = 0;

  /** @brief Mapping from the local number of node to the global. Being a
   * subgraph this variable connect the local index of the node
   * with the global one
   */
  vector<CoMMAIndexType> _mapping_l_to_g;

  /** @brief Insert a node in the subgraph and add it to the mapping the
   *  @param[in] v_neigh vector of the neighbors to be added. The neighbors must
   * be given in the global indexing system.
   *  @param[in] i_fc global index of the node
   *  @param[in] volume Volume of the cell
   *  @param[in] weight vector of the area of the faces of the given cells to be
   * added.
   */
  void insert_node(vector<CoMMAIndexType> &v_neigh, const CoMMAIndexType &i_fc,
                   const CoMMAWeightType &volume,
                   const vector<CoMMAWeightType> &weight) {
    // Use the mapping
    // local vector of neighborhood
    vector<CoMMAIndexType> v_l_neigh;
    typename vector<CoMMAIndexType>::iterator low1;
    // @todo this solution clearly help in the connection of the subnode BUT can
    // bring to instability and errors.
    for (const auto &elem : v_neigh) {
      low1 = find(_mapping_l_to_g.begin(), _mapping_l_to_g.end(), elem);
      if (low1 != _mapping_l_to_g.end()) {
        v_l_neigh.push_back(low1 - _mapping_l_to_g.begin());
      }
    }
    // variable to add weight for each face
    CoMMAIntType iter_weight = 0;
    CoMMAIndexType local_index = this->_m_CRS_Row_Ptr.size() - 1;
    // initialization pointers for insertion, pointing to the first element of
    // each
    auto row_end = this->_m_CRS_Row_Ptr.end() - 1;

    // cycle on the set of neighbours
    for (const auto &elem : v_l_neigh) {
      // insert the node and the weight (we have an iterator for this and remember
      // that at edge is associated one weight) look at here
      // https://stackoverflow.com/questions/71299247/inserting-an-element-in-given-positions-more-than-one-of-a-vector/71299304#71299304
      // to understand why we re-initialize.
      this->_m_CRS_Col_Ind.insert(
          this->_m_CRS_Col_Ind.begin() + this->_m_CRS_Row_Ptr[elem],
          local_index);
      this->_m_CRS_Values.insert(
          this->_m_CRS_Values.begin() + this->_m_CRS_Row_Ptr[elem],
          weight[iter_weight]);
      // We modify the row pointer as far it is related with what we have done
      // before
      for (auto it = this->_m_CRS_Row_Ptr.begin() + elem;
           it != this->_m_CRS_Row_Ptr.end(); ++it) {
        ++(*it);
      }
      // we do the same.
      this->_m_CRS_Col_Ind.insert(this->_m_CRS_Col_Ind.end(), elem);
      this->_m_CRS_Values.insert(this->_m_CRS_Values.end(),
                                 weight[iter_weight]);
      // We increment the weight flag iterator
      ++iter_weight;
    }
    this->_m_CRS_Row_Ptr.push_back(*row_end + v_neigh.size());
    this->_volumes.push_back(volume);
    _mapping_l_to_g.push_back(i_fc);
  }

  /** @brief Remove a node from the CRS representation and automatically adjust
   * the mapping
   *  @param[in] elemento global index of the node to be deleted.
   */
  void remove_node(const CoMMAIndexType &elemento) {
    // Pass to the local
    auto low = find(_mapping_l_to_g.begin(), _mapping_l_to_g.end(), elemento);
    CoMMAIndexType i_fc = low - _mapping_l_to_g.begin();
    // initialize starting indices
    CoMMAIndexType ind;
    CoMMAIndexType ind_p_one;
    // getting neighbours
    vector<CoMMAIndexType> v_neigh = this->get_neighbours(i_fc);

    // weight iterator for erasing in the weight vector
    typename vector<CoMMAWeightType>::iterator weight_it;
    auto pos_col = this->_m_CRS_Col_Ind.begin();
    auto pos_Values = this->_m_CRS_Values.begin();
    CoMMAIndexType k;
    // mapping for the renumbering of the nodes
    vector<CoMMAIndexType> internal_mapping;
    for (const auto &elem : v_neigh) {
      ind = this->_m_CRS_Row_Ptr[elem];
      ind_p_one = this->_m_CRS_Row_Ptr[elem + 1];
      // Constant to keep track and erase the weight
      for (auto it = pos_col + ind; it != pos_col + ind_p_one; ++it) {
        if (*it == i_fc) {
          this->_m_CRS_Col_Ind.erase(it);
          // define the exact position of the element for the processing of the
          // weight later.
          k = it - pos_col;
          weight_it = pos_Values + k;
          this->_m_CRS_Values.erase(weight_it);
          // for each found i decrease the successive of 1 for the offset
          for (auto it_bis = this->_m_CRS_Row_Ptr.begin() + elem + 1;
               it_bis != this->_m_CRS_Row_Ptr.end(); ++it_bis) {
            *it_bis = *it_bis - 1;
          }
        }
      }
    }
    // reduce the row ptr value of the deleted value
    // do the same with i_fc
    ind = this->_m_CRS_Row_Ptr[i_fc];
    ind_p_one = this->_m_CRS_Row_Ptr[i_fc + 1];
    for (auto it = pos_col + ind; it != pos_col + ind_p_one; ++it) {
      this->_m_CRS_Col_Ind.erase(it);
      // define the exact position of the element for the processing of the
      // weight later.
      k = it - pos_col;
      weight_it = pos_Values + k;
      this->_m_CRS_Values.erase(weight_it);
      // for each found i decrease the successive of 1 for the offset
      for (auto it_bis = this->_m_CRS_Row_Ptr.begin() + i_fc + 1;
           it_bis != this->_m_CRS_Row_Ptr.end(); ++it_bis) {
        *it_bis = *it_bis - 1;
      }
    }
    // Get rid of the col element
    auto Col_pointer = this->_m_CRS_Row_Ptr.begin() + i_fc;
    // Modify the mapping
    auto mapping_pointer = _mapping_l_to_g.begin() + i_fc;
    auto volumes_pointer = this->_volumes.begin() + i_fc;
    this->_volumes.erase(volumes_pointer);
    this->_m_CRS_Row_Ptr.erase(Col_pointer);
    _mapping_l_to_g.erase(mapping_pointer);
    // now we do not have nomore our node, but we must create a mapping between the
    // before and now, and translate it in the col_ind and update the mapping with
    // the global graph
    CoMMAIndexType indice = 0;
    // to cycle on the main vector
    CoMMAIndexType ix = 0;
    while (ix != static_cast<CoMMAIntType>(this->_m_CRS_Row_Ptr.size()) + 1) {
      if (ix != i_fc) {
        internal_mapping.push_back(indice);
        indice++;
      } else {
        internal_mapping.push_back(-1);
      }
      ++ix;
    }
    for (auto &actual : this->_m_CRS_Col_Ind) {
      actual = internal_mapping[actual];
    }
  }
};

/** @brief A class implementing the CRS global graph representation of the
 * global mesh
 *  @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Dual_Graph : public Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {

 public:
  /** @brief Constructor of the class
   *  @param[in] m_crs_row_ptr the row pointer of the CRS representation
   *  @param[in] m_crs_col_ind the column index of the CRS representation
   *  @param[in] m_crs_value the weight of the CRS representation (in CoMMA case
   * will be the area of the faces that in the graph representation are the edges
   * between two nodes represented by the cell centers.
   *  @param[in] volumes The volumes of the cells
   *  @param[in] seeds_pool The seeds pool structure
   *  @param[in] s_anisotropic_compliant_fc set of compliant fc cells (in the
   * most of the case all)
   */
  Dual_Graph(const CoMMAIndexType &nb_c,
             const vector<CoMMAIndexType> &m_crs_row_ptr,
             const vector<CoMMAIndexType> &m_crs_col_ind,
             const vector<CoMMAWeightType> &m_crs_values,
             const vector<CoMMAWeightType> &volumes,
             const vector<vector<CoMMAWeightType>> &centers,
             const Seeds_Pool<CoMMAIndexType, CoMMAIntType> &seeds_pool,
             const CoMMAIntType dimension,
             const unordered_set<CoMMAIndexType> &s_anisotropic_compliant_fc =
                 unordered_set<CoMMAIndexType>({}))
      : Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            nb_c, m_crs_row_ptr, m_crs_col_ind, m_crs_values, volumes),
        _seeds_pool(seeds_pool), _centers(centers) {
    if (s_anisotropic_compliant_fc.size() > 0) {
      _s_anisotropic_compliant_cells = s_anisotropic_compliant_fc;
    } else {
      // Default initialization of s_anisotropic_compliant_cells
      for (CoMMAIndexType i = 0; i < this->_number_of_cells; i++) {
        _s_anisotropic_compliant_cells.insert(i);
      }
    }

    // Function to compute the aspect-ratio
    _compute_AR = dimension == 2 ?
        [](const CoMMAWeightType min_s, const CoMMAWeightType max_s)
                        -> CoMMAWeightType { return max_s / min_s; } :
        [](const CoMMAWeightType min_s, const CoMMAWeightType max_s)
                        //-> CoMMAWeightType { return max_s / min_s; };
                        -> CoMMAWeightType { return sqrt(max_s / min_s); };
  }

  /** @brief Destructor of the class */
  ~Dual_Graph() {}

  /** @brief Member seeds pool variable */
  Seeds_Pool<CoMMAIndexType, CoMMAIntType> _seeds_pool;

  /** @brief Member unordered set of compliant cells*/
  unordered_set<CoMMAIndexType> _s_anisotropic_compliant_cells;

  /** @brief Vector of cell centers */
  vector<vector<CoMMAWeightType>> _centers;

  /** @brief Function which computes the aspect-ratio from the minimum and maximum
   * faces
   *  In 3D: \f$ AR = sqrt(max_{surf} / min_{surf}) \f$
   *  In 2D: \f$ AR = max_{surf} / min_{surf} \f$
   *  (Recall that in 2D a face is actually an edge)
   */
  function<CoMMAWeightType(const CoMMAWeightType, const CoMMAWeightType)> _compute_AR;

  /** @brief Tag cells as anisotropic if their aspect-ratio is over a given
   * threshold
   *  @param[out] maxArray Array of the maximum weight: the biggest area of the
   * faces composing the given fine cell
   *  @param[out] anisotropic_fc Set of fine cells tagged as anisotropic
   *  @param[in] threshold_anisotropy Value of the aspect ration above which a
   *  cell is considered anisotropic
   *  @param[in] preserving if 0 does not hit only the BL prism to preserve the
   * boundary layer otherwise 2 for 2D or 3 for the 3D to preserve the BL only
   * in the anisotropic agglomeration
   */
  void compute_anisotropic_fc(
      vector<CoMMAWeightType> &maxArray,
      unordered_set<CoMMAIndexType> &anisotropic_fc,
      const CoMMAWeightType threshold_anisotropy,
      const CoMMAIndexType preserving) {
    // Process of every compliant fine cells (it is a member variable, so it is
    // not passed to the function):
    for (const CoMMAIndexType i_fc : _s_anisotropic_compliant_cells) {
      CoMMAWeightType min_weight = numeric_limits<CoMMAWeightType>::max();
      CoMMAWeightType max_weight = 0.0;

      // computation of min_weight, max_weight for the current cell
      // Process of every faces/Neighbours and compute for the current cell the
      // neighborhood and the area associated with the neighborhood cells
      const vector<CoMMAIndexType> v_neighbours = this->get_neighbours(i_fc);
      const vector<CoMMAWeightType> v_weights = this->get_weights(i_fc);

      assert(v_neighbours.size() == v_weights.size());
      auto nb_neighbours = v_neighbours.size();

      for (auto i_n = decltype(nb_neighbours){0}; i_n < nb_neighbours; i_n++) {
        // to avoid special case where the boundary value are stored
        if (v_neighbours[i_n] != i_fc) {
          const CoMMAWeightType i_w_fc_n = v_weights[i_n];
          if (max_weight < i_w_fc_n) {
            max_weight = i_w_fc_n;
          }
          if (min_weight > i_w_fc_n) {
            min_weight = i_w_fc_n;
          }
        } else {
          nb_neighbours--;
        }
      }

      maxArray[i_fc] = max_weight;
      // Compute the aspect-ratio and add cell to list if necessary
      const auto ar = _compute_AR(min_weight, max_weight);
      if (preserving == 0) {
        // Anisotropy criteria for the line Admissibility
        if (ar >= threshold_anisotropy) {
          // If the ratio is more than the given threshold of the biggest with the
          // smallest cell, add it
          anisotropic_fc.insert(i_fc);
        }
      } else if (preserving == 2) {
        if (ar >= threshold_anisotropy) {
          if (_seeds_pool._d_is_on_bnd.count(i_fc) && nb_neighbours == 3) {
            anisotropic_fc.insert(i_fc);
          } else if (nb_neighbours == 4) {
            anisotropic_fc.insert(i_fc);
          }
        }
      } else if (preserving == 3) {
        if (ar >= threshold_anisotropy) {
          if (_seeds_pool._d_is_on_bnd.count(i_fc) && nb_neighbours == 5) {
            anisotropic_fc.insert(i_fc);
          } else if (nb_neighbours == 6) {
            anisotropic_fc.insert(i_fc);
          }
        }
      }
    } // End for compliant cells
  }

  /** @brief Compute the minimum compactness of fine cells inside a coarse cell.
  *   @param[in] s_fc set of fine cells to analyse
  *   @return the compactness of the fine cell
  */
  CoMMAIntType compute_min_fc_compactness_inside_a_cc(
      unordered_set<CoMMAIndexType> &s_fc) const {
    // Compute Compactness of a cc
    // Be careful: connectivity is assumed
    if (s_fc.size() > 1) {
      unordered_map<CoMMAIndexType, CoMMAIntType> dict_fc_compactness =
          compute_fc_compactness_inside_a_cc(s_fc);
      if (dict_fc_compactness.empty()) {
        return 0;
      }
      CoMMAIntType min_comp = numeric_limits<CoMMAIntType>::max();
      for (auto &i_k_v : dict_fc_compactness) {
        if (i_k_v.second < min_comp) {
          min_comp = i_k_v.second;
        }
      }
      return min_comp;
    } else {
      return 0;
    }
  }

  /** @brief Compute the dictionary of compactness of fine cells inside a coarse
  * cell.
  *   @param[in] s_fc set of fine cells to analyse
  *   @return the dictionary associating a fine cell in the coarse cell with its
  * compactness
  */
  unordered_map<CoMMAIndexType, CoMMAIntType>
  compute_fc_compactness_inside_a_cc(unordered_set<CoMMAIndexType> &s_fc) const {
    unordered_map<CoMMAIndexType, CoMMAIntType> dict_fc_compactness;
    if (s_fc.size() > 1) {

      // for every fc constituting a cc
      for (const CoMMAIndexType &i_fc : s_fc) {

        const vector<CoMMAIndexType> v_neighbours = this->get_neighbours(i_fc);
        for (const CoMMAIndexType &i_fc_n : v_neighbours) {
          if ((s_fc.count(i_fc_n) > 0) && (i_fc != i_fc_n)) {
            if (dict_fc_compactness.count(i_fc) > 0) {
              dict_fc_compactness[i_fc]++;
            } else {
              dict_fc_compactness[i_fc] = 1;
            }
          }
        }
        if (dict_fc_compactness.count(i_fc) == 0) {
          dict_fc_compactness[i_fc] = 0;
        }
      }
    }
    return dict_fc_compactness;
  }

  /** @brief Getter that returns the number of cells
   *  @return number of cells
   */
  inline CoMMAIntType get_nb_cells() const { return this->_number_of_cells; }

  /** @brief Compute the dictionary of compactness of fine cells inside a coarse
  * cell.
  *   @param[in] s_seeds set of seeds for which the neighborhood must be
  * computed. Generally they are the fine cells composing the coarse cell for
  * which we are trying to compute the neighborhood.
  *   @param[in] nb_of_order_of_neighbourhood order of the neighborhood at which
  * we want to extend the dictionary
  *   @param[out] d_n_of_seed dictionary of the neighborhood given as an output.
  * The key of the associative structure is the index of the fine cell, the
  * value is the order of the distance.
  *   @param[in] max_card maximum cardinality
  *   @param[in] is_fc_agglomerated_tmp vector reporting the already
  * agglomerated cell, useful in the algorithm
  */
  void compute_neighbourhood_of_cc(
      const unordered_set<CoMMAIndexType> s_seeds,
      CoMMAIntType &nb_of_order_of_neighbourhood,
      unordered_map<CoMMAIndexType, CoMMAIntType> &d_n_of_seed,
      const CoMMAIntType max_card, vector<bool> &is_fc_agglomerated_tmp) {
    // Basic checks
    assert(max_card != -1);
    // dict of FC with the order of neighbouring from seed
    unordered_map<CoMMAIndexType, CoMMAIntType> d_n_of_order_o_m_one;
    // we initialize for seeds where order is 0
    for (const CoMMAIndexType &i_fc : s_seeds) {
      d_n_of_order_o_m_one[i_fc] = 0;
    }

    CoMMAIntType i_order = 1;

    while ((i_order < nb_of_order_of_neighbourhood + 1) ||
           static_cast<CoMMAIntT>(d_n_of_seed.size() + d_n_of_order_o_m_one.size()) < max_card) {

      unordered_map<CoMMAIndexType, CoMMAIntType> d_n_of_order_o;

      // If here, add elements of previous elements to output dictionary
      for (auto id_M_one : d_n_of_order_o_m_one) {
        d_n_of_seed[id_M_one.first] = id_M_one.second;
      }

      for (const auto &i_k_v : d_n_of_order_o_m_one) {
        // For all the cells in the previous neighborhood order...

        CoMMAIndexType seed_tmp = i_k_v.first;

        for (const CoMMAIndexType &i_fc_n : this->get_neighbours(seed_tmp)) {
          // For all the neighbours of current seed...

          if ((d_n_of_seed.count(i_fc_n) == 0) &&
              ((!is_fc_agglomerated_tmp[i_fc_n]))) {
            // If not yet in the final dictionary and not yet agglomerated...

            if (d_n_of_order_o.count(i_fc_n) == 0) {
              // If not yet in the current neighborhood order...
              // a fc can be access via multiple ways. We look for the quickest
              if (d_n_of_order_o_m_one.count(i_fc_n)) {
                // If it was already in the previous neighborhood order

                if (i_order < d_n_of_order_o_m_one[i_fc_n]) {
                  // If current order smaller than the previous one
                  // ...update the order
                  d_n_of_order_o[i_fc_n] = i_order;
                }
              } else {
                // ...add the neighbour
                d_n_of_order_o[i_fc_n] = i_order;
              }
            }
          }
        }
      }

      // Exit condition (while)
      if (d_n_of_order_o.empty()) {
        // No more neighbours available:
        break;
      }

      // Update previous order
      d_n_of_order_o_m_one.clear();
      for (auto id : d_n_of_order_o) {
        d_n_of_order_o_m_one[id.first] = id.second;
      }
      i_order++;
    } // End of while
    // Update of d_n_of_seed
    // d_n_of_seed.update(d_n_of_order_o_m_one)
    for (auto id_M_one : d_n_of_order_o_m_one) {
      d_n_of_seed[id_M_one.first] = id_M_one.second;
    }

    // We remove the seed from the neighbours of seed
    for (const CoMMAIndexType &i_fc : s_seeds) {
      d_n_of_seed.erase(i_fc);
    }

    nb_of_order_of_neighbourhood = i_order;
  }
};

#endif  // COMMA_PROJECT_DUAL_GRAPH_H
