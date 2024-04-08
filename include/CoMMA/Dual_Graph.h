#ifndef COMMA_PROJECT_DUAL_GRAPH_H
#define COMMA_PROJECT_DUAL_GRAPH_H

/*
 * CoMMA
 *
 * Copyright © 2024 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cassert>
#include <climits>
#include <deque>
#include <functional>
#include <limits>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CoMMA/Seeds_Pool.h"

namespace comma {

/** @brief An interface class responsible of storing the cell centered dual
 * graph and of acting on it (it is an interface for the global Dual_Graph and
 * the Subgraph)
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Graph {
public:
  /** @brief Type for containers of indices */
  using ContainerIndexType = std::vector<CoMMAIndexType>;
  /** @brief Type for containers of weights */
  using ContainerWeightType = std::vector<CoMMAWeightType>;
  /** @brief Type for constant iterators of containers of indices */
  using ContainerIndexConstIt = typename ContainerIndexType::const_iterator;
  /** @brief Type for constant iterators of containers of weights */
  using ContainerWeightConstIt = typename ContainerWeightType::const_iterator;

  /** @brief Constructor of the class
   *  @param[in] nb_c Number of cells
   *  @param[in] m_crs_row_ptr The row pointer of the CRS representation
   *  @param[in] m_crs_col_ind The column index of the CRS representation
   *  @param[in] m_crs_values The weight of the CRS representation (in CoMMA
   * case will be the area of the faces that in the graph representation are the
   * edges between two nodes represented by the cell centers.
   *  @param[in] volumes The volumes of the cells
   */
  Graph(
    const CoMMAIndexType &nb_c,
    const ContainerIndexType &m_crs_row_ptr,
    const ContainerIndexType &m_crs_col_ind,
    const ContainerWeightType &m_crs_values,
    const ContainerWeightType &volumes
  ) :
    _number_of_cells(nb_c),
    _m_CRS_Row_Ptr(m_crs_row_ptr),
    _m_CRS_Col_Ind(m_crs_col_ind),
    _m_CRS_Values(m_crs_values),
    _volumes(volumes) {
    _visited.resize(_number_of_cells);
    fill(_visited.begin(), _visited.end(), false);
  }

  /** @brief Destructor of the class */
  virtual ~Graph() = default;

  /** @brief Number of nodes in the Graph (it corresponds to the number of cells
   * in the subgraph or the dual graph. */
  CoMMAIndexType _number_of_cells;

  /** @brief Helper vector for the DFS */
  std::vector<bool> _visited;

  /** @brief Vector of row pointer of CRS representation */
  ContainerIndexType _m_CRS_Row_Ptr;

  /** @brief Vector of column index of CRS representation */
  ContainerIndexType _m_CRS_Col_Ind;

  /** @brief Vector of area weight of CRS representation */
  ContainerWeightType _m_CRS_Values;

  /** @brief Vector of volumes */
  ContainerWeightType _volumes;

  /** @brief Depth First Search (DFS) recursive function
   *  @param[in] i_fc Index of the node to print
   */
  void DFS(const CoMMAIndexType &i_fc) {
    _visited[i_fc] = true;
    for (auto it = neighbours_cbegin(i_fc); it != neighbours_cend(i_fc); ++it) {
      if (!_visited[*it]) {
        DFS(*it);
      }
    }
  }

  /** @brief Breadth First Search (BFS) function
   *  @param[in] root Root of the spanning tree
   */
  void BFS(const CoMMAIndexType &root) {
    std::deque<CoMMAIndexType> coda;
    ContainerIndexType path;
    coda.push_back(root);
    std::vector<bool> visited(_number_of_cells, false);
    visited[root] = true;
    std::vector<std::optional<CoMMAIndexType>> prev(
      _number_of_cells, std::nullopt
    );
    while (!coda.empty()) {
      CoMMAIndexType node = coda.front();
      coda.pop_front();
      for (auto it = neighbours_cbegin(node); it != neighbours_cend(node);
           ++it) {
        if (!visited[*it]) {
          coda.push_pack(*it);
          visited[*it] = true;
          prev[it] = node;
        }
      }
    }
    // to print the inverse path
    CoMMAIndexType retro = prev[_number_of_cells - 1];
    while (retro.has_value()) {
      path.push_back(retro);
      retro = prev[retro];
    }
    reverse(path.begin(), path.end());
    //     for (CoMMAIntType i = 0; i < _number_of_cells; i++) {
    //            std::cout<<"BFS"<<path[i]<<std::endl;
    //     }
  }

  /** @brief Retrieve the number of neighbours
   *  @param[in] i_c Index of the cell
   *  @return Number of neighbours of the given cell.
   */
  inline CoMMAIntType get_nb_of_neighbours(CoMMAIndexType i_c) const {
    // Return the number of neighbours of the ith cell
    return _m_CRS_Row_Ptr[i_c + 1] - _m_CRS_Row_Ptr[i_c];
  }

  /** @brief Based on the CRS representation retrieves the neighbours of the
   * cell given as an input.
   * @param[in] i_c Index of the cell
   * @return vector of the neighbours.
   */
  ContainerIndexType get_neighbours(const CoMMAIndexType &i_c) const {
    // given the index of a cell return the neighbourhoods of this cell
    CoMMAIndexType ind = _m_CRS_Row_Ptr[i_c];
    CoMMAIndexType ind_p_one = _m_CRS_Row_Ptr[i_c + 1];
    // insert the values of the CRS_value from begin+ind (pointed to the face)
    // till the next pointed one, so related to all the connected areas (and
    // hence to the faces)
    ContainerIndexType result(
      _m_CRS_Col_Ind.begin() + ind, _m_CRS_Col_Ind.begin() + ind_p_one
    );
    return result;
  }

  /** @brief Get constant pointer to the first neighbour of cell \p i_c
   *  @param[in] i_c Index of the cell
   *  @return The pointer
   */
  inline ContainerIndexConstIt neighbours_cbegin(const CoMMAIndexType &i_c
  ) const {
    return _m_CRS_Col_Ind.cbegin() + _m_CRS_Row_Ptr[i_c];
  }

  /** @brief Get constant pointer to the element following the last neighbour of
   * cell \p i_c
   *  @param[in] i_c Index of the cell
   *  @return The pointer
   */
  inline ContainerIndexConstIt neighbours_cend(const CoMMAIndexType &i_c
  ) const {
    return _m_CRS_Col_Ind.cbegin() + _m_CRS_Row_Ptr[i_c + 1];
  }

  /** @brief Based on the area of the faces composing the cell given as an
   * input, we retrieve the faces connecting the given cell with the
   * neighbourhood that can be described also as the weight of the graph
   * @param[in] i_c Index of the cell
   * @return Vector of weights associated to the cell.
   */
  inline ContainerWeightType get_weights(const CoMMAIndexType &i_c) const {
    // Given the index of a cell, return the value of the faces connected
    CoMMAIndexType ind = _m_CRS_Row_Ptr[i_c];
    CoMMAIndexType ind_p_one = _m_CRS_Row_Ptr[i_c + 1];
    // insert the values of the CRS_value from begin+ind (pointed to the face)
    // till the next pointed one, so related to all the connected areas (and
    // hence to the faces)
    ContainerWeightType result(
      _m_CRS_Values.begin() + ind, _m_CRS_Values.begin() + ind_p_one
    );
    return result;
  }

  /** @brief Get constant pointer to the first neighbour of cell \p i_c
   *  @param[in] i_c Index of the cell
   *  @return The pointer
   */
  inline ContainerWeightConstIt weights_cbegin(const CoMMAIndexType &i_c
  ) const {
    return _m_CRS_Values.cbegin() + _m_CRS_Row_Ptr[i_c];
  }

  /** @brief Get constant pointer to the element following the last neighbour of
   * cell \p i_c
   *  @param[in] i_c Index of the cell
   *  @return The pointer
   */
  inline ContainerWeightConstIt weights_cend(const CoMMAIndexType &i_c) const {
    return _m_CRS_Values.cbegin() + _m_CRS_Row_Ptr[i_c + 1];
  }

  /** @brief Check the connectivity of the graph.
   * @return True if the graph is connected, false if it is not connected
   */
  bool check_connectivity() {
    for (auto i = decltype(_number_of_cells){0}; i < _number_of_cells; ++i) {
      _visited.push_back(false);
    }
    if (_number_of_cells == 1) {
      return (true);
    }
    DFS(_m_CRS_Col_Ind[0]);
    for (auto i = decltype(_number_of_cells){0}; i < _number_of_cells; ++i) {
      if (!_visited[i]) {
        return (false);
      }
    }
    return (true);
  }

  /** @brief Compute the minimum compactness of fine cells inside a coarse cell.
   *   @param[in] s_fc Set of fine cells to analyse
   *   @return the compactness of the fine cell
   */
  CoMMAIntType compute_min_fc_compactness_inside_a_cc(
    const std::unordered_set<CoMMAIndexType> &s_fc
  ) const {
    // Compute Compactness of a cc
    // Be careful: connectivity is assumed
    if (s_fc.size() > 1) {
      std::unordered_map<CoMMAIndexType, CoMMAIntType> dict_fc_compactness =
        compute_fc_compactness_inside_a_cc(s_fc);
      if (dict_fc_compactness.empty()) {
        return 0;
      }
      return min_element(
               dict_fc_compactness.begin(),
               dict_fc_compactness.end(),
               [](const auto &left, const auto &right) {
                 return left.second < right.second;
               }
      )->second;
    }
    return 0;
  }

  /** @brief Compute the dictionary of compactness of fine cells inside a coarse
   * cell.
   *   @param[in] s_fc Set of fine cells to analyse
   *   @return the dictionary associating a fine cell in the coarse cell with
   * its compactness
   */
  std::unordered_map<CoMMAIndexType, CoMMAIntType>
  compute_fc_compactness_inside_a_cc(
    const std::unordered_set<CoMMAIndexType> &s_fc
  ) const {
    std::unordered_map<CoMMAIndexType, CoMMAIntType> dict_fc_compactness;
    if (!s_fc.empty()) {
      if (s_fc.size() == 1) {
        dict_fc_compactness[*s_fc.begin()] = 0;
      } else {
        for (const CoMMAIndexType &i_fc : s_fc) {
          dict_fc_compactness[i_fc] = count_if(
            this->_m_CRS_Col_Ind.cbegin() + this->_m_CRS_Row_Ptr[i_fc],
            this->_m_CRS_Col_Ind.cbegin() + this->_m_CRS_Row_Ptr[i_fc + 1],
            [&](const auto &neigh) { return s_fc.count(neigh) > 0; }
          );
        }
      }
    }
    return dict_fc_compactness;
  }
};

/** @brief A class implementing the CRS subgraph representation. It is used in
 * the framework of CoMMA for the implementation of the CSR representation of
 * the coarse cells.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Subgraph : public Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief Parent class */
  using BaseClass = Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Type for containers of indices */
  using typename BaseClass::ContainerIndexType;
  /** @brief Type for containers of weights */
  using typename BaseClass::ContainerWeightType;

  /** @brief Constructor of the class
   *  @param[in] nb_c Cardinality of the CC, that is the number of fine cells
   * composing the CC
   *  @param[in] m_crs_row_ptr The row pointer of the CRS representation
   *  @param[in] m_crs_col_ind The column index of the CRS representation
   *  @param[in] m_crs_values The weight of the CRS representation (in CoMMA
   * case will be the area of the faces that in the graph representation are the
   * edges between two nodes represented by the cell centers.
   *  @param[in] volumes The volumes of the cells
   *  @param[in] mapping_l_to_g Mapping between the local (to the CC) numbering
   * to global numbering
   *  @param[in] is_isotropic Whether the cell is isotropic
   */
  Subgraph(
    const CoMMAIndexType &nb_c,
    const ContainerIndexType &m_crs_row_ptr,
    const ContainerIndexType &m_crs_col_ind,
    const ContainerWeightType &m_crs_values,
    const ContainerWeightType &volumes,
    const ContainerIndexType &mapping_l_to_g,
    const bool &is_isotropic
  ) :
    Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
      nb_c, m_crs_row_ptr, m_crs_col_ind, m_crs_values, volumes
    ),
    _is_isotropic(is_isotropic),
    _cardinality(static_cast<CoMMAIntType>(nb_c)),
    _mapping_l_to_g(mapping_l_to_g) {
    // Compactness computation
    _compactness = static_cast<CoMMAIntType>(nb_c);
    for (auto c = decltype(nb_c){0}; c < nb_c; ++c) {
      const auto n_neighs =
        static_cast<CoMMAIntType>(m_crs_row_ptr[c + 1] - m_crs_row_ptr[c]);
      if (n_neighs < _compactness) {
        _compactness = n_neighs;
      }
    }
  }

  /** @brief Destructor of the class */
  ~Subgraph() override = default;

  /** @brief Whether it originates from an isotropic cell. */
  bool _is_isotropic;

  /** @brief Cardinality of the given subgraph, alias the number of nodes
   * contained
   */
  CoMMAIntType _cardinality;

  /** @brief Compactness of the given subgraph. The compactness is the minimum
   * number of internal neighbours
   */
  CoMMAIntType _compactness;

  /** @brief Mapping from the local number of node to the global. Being a
   * subgraph this variable connect the local index of the node
   * with the global one
   */
  ContainerIndexType _mapping_l_to_g;

  /** @brief Insert a node in the subgraph and add it to the mapping the
   *  @param[in] v_neigh Vector of the neighbours to be added. The neighbours
   * must be given in the global indexing system.
   *  @param[in] i_fc Global index of the node
   *  @param[in] volume Volume of the cell
   *  @param[in] weight Vector of the area of the faces of the given cells to be
   * added.
   */
  void insert_node(
    const ContainerIndexType &v_neigh,
    const CoMMAIndexType &i_fc,
    const CoMMAWeightType &volume,
    const ContainerWeightType &weight
  ) {
    // Use the mapping
    // local vector of neighbourhood
    ContainerIndexType v_l_neigh{};
    // @todo this solution clearly help in the connection of the subnode BUT can
    // bring to instability and errors.
    for (const auto &elem : v_neigh) {
      const auto low1 =
        std::find(_mapping_l_to_g.begin(), _mapping_l_to_g.end(), elem);
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
      // insert the node and the weight (we have an iterator for this and
      // remember that at edge is associated one weight) look at here
      // https://stackoverflow.com/questions/71299247/inserting-an-element-in-given-positions-more-than-one-of-a-vector/71299304#71299304
      // to understand why we re-initialize.
      this->_m_CRS_Col_Ind.insert(
        this->_m_CRS_Col_Ind.begin() + this->_m_CRS_Row_Ptr[elem], local_index
      );
      this->_m_CRS_Values.insert(
        this->_m_CRS_Values.begin() + this->_m_CRS_Row_Ptr[elem],
        weight[iter_weight]
      );
      // We modify the row pointer as far it is related with what we have done
      // before
      for (auto it = this->_m_CRS_Row_Ptr.begin() + elem;
           it != this->_m_CRS_Row_Ptr.end();
           ++it) {
        ++(*it);
      }
      // we do the same.
      this->_m_CRS_Col_Ind.insert(this->_m_CRS_Col_Ind.end(), elem);
      this->_m_CRS_Values.insert(
        this->_m_CRS_Values.end(), weight[iter_weight]
      );
      // We increment the weight flag iterator
      ++iter_weight;
    }
    this->_m_CRS_Row_Ptr.push_back(*row_end + v_neigh.size());
    this->_volumes.push_back(volume);
    _mapping_l_to_g.push_back(i_fc);
  }

  /** @brief Remove a node from the CRS representation and automatically adjust
   * the mapping
   *  @param[in] elemento Global index of the node to be deleted.
   */
  void remove_node(const CoMMAIndexType &elemento) {
    // Pass to the local
    auto low =
      std::find(_mapping_l_to_g.begin(), _mapping_l_to_g.end(), elemento);
    const CoMMAIndexType i_fc = low - _mapping_l_to_g.begin();
    // Getting neighbours, store them before erasing
    ContainerIndexType v_neigh = this->get_neighbours(i_fc);

    // weight iterator for erasing in the weight vector
    auto pos_col = this->_m_CRS_Col_Ind.begin();
    auto pos_Values = this->_m_CRS_Values.begin();
    for (const auto &elem : v_neigh) {
      CoMMAIndexType ind = this->_m_CRS_Row_Ptr[elem];
      CoMMAIndexType ind_p_one = this->_m_CRS_Row_Ptr[elem + 1];
      // Constant to keep track and erase the weight
      for (auto it = pos_col + ind; it != pos_col + ind_p_one; ++it) {
        if (*it == i_fc) {
          this->_m_CRS_Col_Ind.erase(it);
          // define the exact position of the element for the processing of the
          // weight later.
          this->_m_CRS_Values.erase(pos_Values + (it - pos_col));
          // for each found i decrease the successive of 1 for the offset
          for (auto it_bis = this->_m_CRS_Row_Ptr.begin() + elem + 1;
               it_bis != this->_m_CRS_Row_Ptr.end();
               ++it_bis) {
            *it_bis = *it_bis - 1;
          }
        }
      }
    }
    // reduce the row ptr value of the deleted value
    // do the same with i_fc
    CoMMAIndexType ind = this->_m_CRS_Row_Ptr[i_fc];
    CoMMAIndexType ind_p_one = this->_m_CRS_Row_Ptr[i_fc + 1];
    for (auto it = pos_col + ind; it != pos_col + ind_p_one; ++it) {
      this->_m_CRS_Col_Ind.erase(it);
      // define the exact position of the element for the processing of the
      // weight later.
      this->_m_CRS_Values.erase(pos_Values + (it - pos_col));
      // for each found i decrease the successive of 1 for the offset
      for (auto it_bis = this->_m_CRS_Row_Ptr.begin() + i_fc + 1;
           it_bis != this->_m_CRS_Row_Ptr.end();
           ++it_bis) {
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
    // now we do not have nomore our node, but we must create a mapping between
    // the before and now, and translate it in the col_ind and update the
    // mapping with the global graph mapping for the renumbering of the nodes
    std::vector<std::optional<CoMMAIndexType>> internal_mapping;
    internal_mapping.reserve(this->_m_CRS_Row_Ptr.size() + 1);
    CoMMAIndexType indice = 0;
    for (auto ix = decltype(this->_m_CRS_Row_Ptr.size()){0};
         ix < this->_m_CRS_Row_Ptr.size() + 1;
         ++ix) {
      if (static_cast<decltype(i_fc)>(ix) != i_fc) {
        internal_mapping.push_back(indice);
        indice++;
      } else {
        internal_mapping.push_back(std::nullopt);
      }
    }
    for (auto &actual : this->_m_CRS_Col_Ind) {
      assert(internal_mapping[actual].has_value());
      actual = internal_mapping[actual].value();
    }
  }
};

/** @brief A class implementing the CRS global graph representation of the
 * global mesh
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Dual_Graph : public Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief Parent class */
  using BaseClass = Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Type for containers of indices */
  using typename BaseClass::ContainerIndexType;
  /** @brief Type for containers of weights */
  using typename BaseClass::ContainerWeightType;
  /** @brief Type of pair */
  using CoMMAPairType = std::pair<CoMMAIndexType, CoMMAWeightType>;
  /** @brief Type of set of pairs */
  using CoMMASetOfPairType =
    std::set<CoMMAPairType, CustomPairGreaterFunctor<CoMMAPairType>>;

  /** @brief Constructor of the class
   *  @param[in] nb_c Number of cells
   *  @param[in] m_crs_row_ptr The row pointer of the CRS representation
   *  @param[in] m_crs_col_ind The column index of the CRS representation
   *  @param[in] m_crs_values The weight of the CRS representation (in CoMMA
   * case will be the area of the faces that in the graph representation are the
   * edges between two nodes represented by the cell centers.
   *  @param[in] volumes The volumes of the cells
   *  @param[in] centers Cell centers
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell
   * has
   *  @param[in] dimension Dimensionality of the problem, 2- or 3D
   *  @param[in] anisotropic_compliant_fc Set of compliant fc cells (in the
   * most of the case all)
   */
  Dual_Graph(
    const CoMMAIndexType &nb_c,
    const ContainerIndexType &m_crs_row_ptr,
    const ContainerIndexType &m_crs_col_ind,
    const ContainerWeightType &m_crs_values,
    const ContainerWeightType &volumes,
    const std::vector<std::vector<CoMMAWeightType>> &centers,
    const std::vector<CoMMAIntType> &n_bnd_faces,
    const CoMMAIntType dimension,
    const ContainerIndexType &anisotropic_compliant_fc
  ) :
    Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
      nb_c, m_crs_row_ptr, m_crs_col_ind, m_crs_values, volumes
    ),
    _n_bnd_faces(n_bnd_faces),
    _s_anisotropic_compliant_cells(
      anisotropic_compliant_fc.begin(), anisotropic_compliant_fc.end()
    ),
    _centers(centers) {
    // Function to compute the aspect-ratio
    _compute_AR = dimension == 2
      ? [](
          const CoMMAWeightType min_s, const CoMMAWeightType max_s
        ) -> CoMMAWeightType { return max_s / min_s; }
    : [](
        const CoMMAWeightType min_s, const CoMMAWeightType max_s
      ) -> CoMMAWeightType { return sqrt(max_s / min_s); };
  }

  /** @brief Destructor of the class */
  ~Dual_Graph() override = default;

  /** @brief Vector telling how many boundary faces each cell has */
  const std::vector<CoMMAIntType> &_n_bnd_faces;

  /** @brief Elements that are checked if they are anisotropic. If an element
   * satisfies the condition for being anisotropic (typically, AR > threshold)
   * but it not in this set, it will not considered as anisotropic. We use a set
   * to ensure uniqueness
   */
  const std::unordered_set<CoMMAIndexType> _s_anisotropic_compliant_cells;

  /** @brief Vector of cell centers */
  const std::vector<std::vector<CoMMAWeightType>> &_centers;

  /** @brief Function which computes the aspect-ratio from the minimum and
   * maximum faces\n In 3D: \f$ AR = sqrt(\frac{max_{surf}}{min_{surf}}) \f$\n
   *  In 2D: \f$ AR = \frac{max_{surf}}{min_{surf}} \f$\n
   *  (Recall that in 2D a face is actually an edge)
   */
  std::function<CoMMAWeightType(const CoMMAWeightType, const CoMMAWeightType)>
    _compute_AR;

  /** @brief Return how many boundary faces a certain cell has
   *  @param[in] idx_c Index of the cell
   *  @return the number of boundary faces
   */
  inline CoMMAIntType get_n_boundary_faces(const CoMMAIndexType idx_c) const {
    return _n_bnd_faces[idx_c];
  }

  /** @brief Whether a cell is on the boundary
   *  @param[in] idx_c Index of the cell
   *  @return Whether a cell is on the boundary
   */
  inline bool is_on_boundary(const CoMMAIndexType idx_c) const {
    return _n_bnd_faces[idx_c] > 0;
  }

  /** @brief Tag cells as anisotropic if their aspect-ratio is over a given
   * threshold and order them according to given priority
   *  @param[out] max_weights Array of the maximum weight: the biggest area of
   * the faces composing the given fine cell
   *  @param[out] is_anisotropic Vector of length equal to the total number of
   * cell telling whether a cell is anisotropic
   *  @param[out] aniso_seeds_pool Container containing the anisotropic cells in
   * the order they should be considered when computing the lines
   *  @param[in] threshold_anisotropy Value of the aspect ratio above which a
   *  cell is considered anisotropic. If negative, all compliant cells are
   *  considered as anisotropic
   * @param[in] priority_weights Weights used to set the order telling where to
   * start agglomerating. The higher the weight, the higher the priority
   *  @param[in] preserving if 0 does not hit only the BL prism to preserve the
   * boundary layer otherwise 2 for 2D or 3 for the 3D to preserve the BL only
   * in the anisotropic agglomeration
   */
  void tag_anisotropic_cells(
    ContainerWeightType &max_weights,
    std::vector<bool> &is_anisotropic,
    std::deque<CoMMAIndexType> &aniso_seeds_pool,
    const CoMMAWeightType threshold_anisotropy,
    const ContainerWeightType &priority_weights,
    const CoMMAIndexType preserving
  ) {
    CoMMASetOfPairType aniso_w_weights{};
    if (threshold_anisotropy < 0) {
      for (const CoMMAIndexType i_fc : _s_anisotropic_compliant_cells) {
        aniso_w_weights.emplace(i_fc, priority_weights[i_fc]);
        max_weights[i_fc] =
          *(max_element(this->weights_cbegin(i_fc), this->weights_cend(i_fc)));
      }
    } else {
      for (const CoMMAIndexType i_fc : _s_anisotropic_compliant_cells) {
        CoMMAWeightType min_weight =
          std::numeric_limits<CoMMAWeightType>::max();
        CoMMAWeightType max_weight = 0.0;

        // computation of min_weight, max_weight for the current cell
        // Process of every faces/Neighbours and compute for the current cell
        // the neighbourhood and the area associated with the neighbourhood
        // cells
        const ContainerIndexType v_neighbours = this->get_neighbours(i_fc);
        const ContainerWeightType v_weights = this->get_weights(i_fc);

        assert(v_neighbours.size() == v_weights.size());
        auto nb_neighbours = v_neighbours.size();

        for (auto i_n = decltype(nb_neighbours){0}; i_n < nb_neighbours;
             i_n++) {
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

        max_weights[i_fc] = max_weight;
        // Compute the aspect-ratio and add cell to list if necessary
        const auto ar = _compute_AR(min_weight, max_weight);
        // Anisotropy criteria for the line Admissibility
        if (ar >= threshold_anisotropy) {
          switch (preserving) {
            case 2:
              if ((is_on_boundary(i_fc) > 0 && nb_neighbours == 3)
                  || nb_neighbours == 4)
                aniso_w_weights.emplace(i_fc, priority_weights[i_fc]);
              break;
            case 3:
              if ((is_on_boundary(i_fc) > 0 && nb_neighbours == 5)
                  || nb_neighbours == 6)
                aniso_w_weights.emplace(i_fc, priority_weights[i_fc]);
            case 0:
            default:
              // If the ratio is more than the given threshold of the biggest
              // with the smallest cell, add it
              aniso_w_weights.emplace(i_fc, priority_weights[i_fc]);
              break;
          }  // End switch
        }  // End if ar
      }  // End for compliant cells
    }  // End if threshold

    // Build result
    for (const auto &[i_fc, w] : aniso_w_weights) {
      is_anisotropic[i_fc] = true;
      aniso_seeds_pool.emplace_back(i_fc);
    }
  }

  /** @brief Getter that returns the number of cells
   *  @return number of cells
   */
  inline CoMMAIntType get_nb_cells() const { return this->_number_of_cells; }

  /** @brief Get the fine cells neighbours of a coarse cell
   *   @param[in] s_seeds Set of seeds for which the neighbourhood should be
   * computed. Generally they are the fine cells composing the coarse cell for
   * which we are trying to compute the neighbourhood.
   *   @param[in] is_fc_agglomerated_tmp Vector reporting the already
   * agglomerated cell, useful in the algorithm
   *   @return The set of neighbours
   */
  inline std::unordered_set<CoMMAIndexType> get_neighbourhood_of_cc(
    const std::unordered_set<CoMMAIndexType> &s_seeds,
    const std::vector<bool> &is_fc_agglomerated_tmp
  ) const {
    std::unordered_set<CoMMAIndexType> cc_neighs{};
    for (const auto fc : s_seeds)
      for (auto n = this->neighbours_cbegin(fc); n != this->neighbours_cend(fc);
           ++n)
        if (!is_fc_agglomerated_tmp[*n] && s_seeds.find(*n) == s_seeds.end())
          // If not agglomerated and not part of the coarse cell
          cc_neighs.insert(*n);
    return cc_neighs;
  }

  /** @brief Compute the dictionary of compactness of fine cells inside a coarse
   * cell.
   *   @param[in] s_seeds Set of seeds for which the neighbourhood must be
   * computed. Generally they are the fine cells composing the coarse cell for
   * which we are trying to compute the neighbourhood.
   *   @param[in] nb_of_order_of_neighbourhood order Of the neighbourhood at
   * which we want to extend the dictionary
   *   @param[out] d_n_of_seed Dictionary of the neighbourhood given as an
   * output. The key of the associative structure is the index of the fine cell,
   * the value is the order of the distance.
   *   @param[in] max_card Maximum cardinality
   *   @param[in] is_fc_agglomerated_tmp Vector reporting the already
   * agglomerated cell, useful in the algorithm
   */
  void compute_neighbourhood_of_cc(
    const std::unordered_set<CoMMAIndexType> &s_seeds,
    CoMMAIntType &nb_of_order_of_neighbourhood,
    std::unordered_map<CoMMAIndexType, CoMMAIntType> &d_n_of_seed,
    const CoMMAIntType max_card,
    const std::vector<bool> &is_fc_agglomerated_tmp
  ) const {
    // Basic checks
    assert(max_card > 1);
    // dict of FC with the order of neighbouring from seed
    std::unordered_map<CoMMAIndexType, CoMMAIntType> d_n_of_order_o_m_one;
    // we initialize for seeds where order is 0
    for (const CoMMAIndexType &i_fc : s_seeds) {
      d_n_of_order_o_m_one[i_fc] = 0;
    }

    CoMMAIntType i_order = 1;

    while ((i_order < nb_of_order_of_neighbourhood + 1)
           || static_cast<CoMMAIntType>(
                d_n_of_seed.size() + d_n_of_order_o_m_one.size()
              )
             < max_card) {
      std::unordered_map<CoMMAIndexType, CoMMAIntType> d_n_of_order_o;

      // If here, add elements of previous elements to output dictionary
      for (auto id_M_one : d_n_of_order_o_m_one) {
        d_n_of_seed[id_M_one.first] = id_M_one.second;
      }

      for (const auto &i_k_v : d_n_of_order_o_m_one) {
        // For all the cells in the previous neighbourhood order...

        CoMMAIndexType seed_tmp = i_k_v.first;

        for (auto i_fc_n = this->neighbours_cbegin(seed_tmp);
             i_fc_n != this->neighbours_cend(seed_tmp);
             ++i_fc_n) {
          // For all the neighbours of current seed...

          if ((d_n_of_seed.count(*i_fc_n) == 0)
              && ((!is_fc_agglomerated_tmp[*i_fc_n]))) {
            // If not yet in the final dictionary and not yet agglomerated...

            if (d_n_of_order_o.count(*i_fc_n) == 0) {
              // If not yet in the current neighbourhood order...
              // a fc can be access via multiple ways. We look for the quickest
              if (d_n_of_order_o_m_one.count(*i_fc_n)) {
                // If it was already in the previous neighbourhood order

                if (i_order < d_n_of_order_o_m_one[*i_fc_n]) {
                  // If current order smaller than the previous one
                  // ...update the order
                  d_n_of_order_o[*i_fc_n] = i_order;
                }
              } else {
                // ...add the neighbour
                d_n_of_order_o[*i_fc_n] = i_order;
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
    }  // End of while
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

}  // end namespace comma

#endif  // COMMA_PROJECT_DUAL_GRAPH_H
