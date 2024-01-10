#ifndef COMMA_PROJECT_COARSE_CELL_H
#define COMMA_PROJECT_COARSE_CELL_H

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

#include <memory>
#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Dual_Graph.h"

namespace comma {

/** @brief Class describing a coarse cell.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Coarse_Cell {
public:
  /** @brief Type for a shared pointer to a Dual_Graph object */
  using DualGraphPtr =
    std::shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>;

  /** @brief Constructor of the class
   * @param[in] fc_graph Dual_Graph object from where are taken the set of fine
   * cells to create the coarse cell.
   * @param[in] i_cc Index of the coarse cell
   * @param[in] s_fc Unordered set of fine cells constituting the coarse cell
   * @param[in] compactness Compactness degree of the CC
   * @param[in] is_isotropic (default = true) boolean describing if the cell is
   * coming from an isotropic agglomeration process or an anisotropic
   * agglomeration process.
   */
  Coarse_Cell(
    DualGraphPtr fc_graph,
    CoMMAIndexType i_cc,
    const std::unordered_set<CoMMAIndexType> &s_fc,
    CoMMAIntType compactness,
    bool is_isotropic = true) :
      _idx(i_cc),
      _fc_graph(fc_graph),
      _compactness(compactness),
      _cardinality(static_cast<CoMMAIntType>(s_fc.size())),
      _is_isotropic(is_isotropic),
      _s_fc(s_fc) {}

  /** @brief Destructor of the class */
  virtual ~Coarse_Cell() = default;

  /** @brief Index of the coarse cell (It seems to be unused, but useful to
   * have) */
  CoMMAIndexType _idx;

  /** @brief The global dual graph */
  DualGraphPtr _fc_graph;

  /** @brief Compactness degree of the CC */
  CoMMAIntType _compactness;

  /** @brief Number of FC in the CC */
  CoMMAIntType _cardinality;

  /** @brief Whether the cell isotropic or anisotropic */
  bool _is_isotropic;

  /** @brief Set of fine cells composing the Coarse cell */
  std::unordered_set<CoMMAIndexType> _s_fc;

  /** @brief Method that return a boolean determining if the coarse cell is
   * defined by a connected sub-graph or not.
   *  @return true if the subgraph is connected, false if the subgraph is not
   * connected
   */
  inline bool is_connected() { return _compactness > 0; }

  /** @brief Insert a FC in the CC (and update sub-graph if necessary)
   *  @param[in] i_fc Index of the fine cell to add
   *  @param[in] new_compactness Optional, default void, giving the compactness
   * of the CC after the addition
   */
  virtual void insert_cell(
    const CoMMAIndexType i_fc,
    const std::optional<CoMMAIntType> new_compactness = std::nullopt) {
    _s_fc.insert(i_fc);
    ++_cardinality;
    _compactness = new_compactness.has_value()
                     ? new_compactness.value()
                     : _fc_graph->compute_min_fc_compactness_inside_a_cc(_s_fc);
  }

  /** @brief Insert several FC in the CC (and update sub-graph if necessary)
   *  @param[in] fcs Set of indices of the fine cells to add
   *  @param[in] new_compactness Optional, default void, giving the compactness
   * of the CC after the addition
   */
  virtual void insert_cells(
    const std::unordered_set<CoMMAIndexType> &fcs,
    const std::optional<CoMMAIntType> new_compactness = std::nullopt) {
    _s_fc.insert(fcs.begin(), fcs.end());
    _cardinality += fcs.size();
    _compactness = new_compactness.has_value()
                     ? new_compactness.value()
                     : _fc_graph->compute_min_fc_compactness_inside_a_cc(_s_fc);
  }
};

/** @brief Class describing a coarse cell with a full description, that is, it
 * also holds a subgraph describing how the fine cells are connected inside the
 * coarse one.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Coarse_Cell_Subgraph :
    Coarse_Cell<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief Type for the base Coarse_Cell class */
  using BaseClass = Coarse_Cell<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;

  /** @brief Type for a shared pointer to a Dual_Graph object */
  using typename BaseClass::DualGraphPtr;

  /** @brief Type for a Subgraph object */
  using SubGraphType = Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;

  /** @brief Type for a shared pointer to a Subgraph object */
  using SubGraphPtr = std::shared_ptr<SubGraphType>;

  /** @brief Constructor of the class
   * @param[in] fc_graph Dual_Graph object from where are taken the set of fine
   * cells to create the coarse cell.
   * @param[in] i_cc Index of the coarse cell
   * @param[in] s_fc Unordered set of fine cells constituting the coarse cell
   * @param[in] compactness Compactness degree of the CC
   * @param[in] is_isotropic (default = true) boolean describing if the cell is
   * coming from an isotropic agglomeration process or an anisotropic
   * agglomeration process.
   */
  Coarse_Cell_Subgraph(
    DualGraphPtr fc_graph,
    CoMMAIndexType i_cc,
    const std::unordered_set<CoMMAIndexType> &s_fc,
    CoMMAIntType compactness,
    bool is_isotropic = true) :
      BaseClass(fc_graph, i_cc, s_fc, compactness, is_isotropic),
      _is_connected(compactness > 0),
      _is_connectivity_up_to_date(true) {
    // initialization vectors
    CoMMAIndexType position = 0;
    std::vector<CoMMAWeightType> volumes;
    std::vector<CoMMAWeightType> CSR_vals{};
    std::vector<CoMMAIndexType> CSR_row = {0};
    std::vector<CoMMAIndexType> CSR_col{};
    std::vector<CoMMAIndexType> col_ind{};
    std::vector<CoMMAIndexType> mapping{};
    for (const CoMMAIndexType &i_fc : this->_s_fc) {
      // we add to the mapping the i_fc
      mapping.push_back(i_fc);
      // get neighbours and the weights associated
      const std::vector<CoMMAIndexType> neigh =
        this->_fc_graph->get_neighbours(i_fc);
      const std::vector<CoMMAWeightType> area =
        this->_fc_graph->get_weights(i_fc);
      for (auto it = neigh.begin(); it != neigh.end(); ++it) {
        if (
          std::find(this->_s_fc.begin(), this->_s_fc.end(), *it)
          != this->_s_fc.end()) {
          ++position;
          col_ind.push_back(*it);
          CSR_vals.push_back(area[it - neigh.begin()]);
        }
      }
      CSR_row.push_back(position);
      volumes.push_back(this->_fc_graph->_volumes[i_fc]);
    }

    // Map in the local subgraph
    for (auto it = col_ind.begin(); it != col_ind.end(); ++it) {
      auto indx = std::find(mapping.begin(), mapping.end(), *it);
      CSR_col.push_back(indx - mapping.begin());
    }

    _cc_graph = std::make_shared<SubGraphType>(
      s_fc.size(), CSR_row, CSR_col, CSR_vals, volumes, mapping, is_isotropic);
  }

  /** @brief Insert a FC in the CC (and update sub-graph if necessary)
   *  @param[in] i_fc Index of the fine cell to add
   *  @param[in] new_compactness Optional, default void, giving the compactness
   * of the CC after the addition
   */
  void insert_cell(
    const CoMMAIndexType i_fc,
    const std::optional<CoMMAIntType> new_compactness = std::nullopt) override {
    // As base class...
    this->_s_fc.insert(i_fc);
    ++this->_cardinality;
    this->_compactness =
      new_compactness.has_value()
        ? new_compactness.value()
        : this->_fc_graph->compute_min_fc_compactness_inside_a_cc(this->_s_fc);
    // ...but now add to the subgraph
    _cc_graph->insert_node(
      this->_fc_graph->get_neighbours(i_fc), i_fc,
      this->_fc_graph->_volumes[i_fc], this->_fc_graph->get_weights(i_fc));
  }

  /** @brief Insert several FC in the CC (and update sub-graph if necessary)
   *  @param[in] fcs Set of indices of the fine cells to add
   *  @param[in] new_compactness Optional, default void, giving the compactness
   * of the CC after the addition
   */
  void insert_cells(
    const std::unordered_set<CoMMAIndexType> &fcs,
    const std::optional<CoMMAIntType> new_compactness = std::nullopt) override {
    // As base class...
    this->_s_fc.insert(fcs.begin(), fcs.end());
    this->_cardinality += fcs.size();
    this->_compactness =
      new_compactness.has_value()
        ? new_compactness.value()
        : this->_fc_graph->compute_min_fc_compactness_inside_a_cc(this->_s_fc);
    // ...but now add to the subgraph
    for (const auto &i_fc : fcs) {
      _cc_graph->insert_node(
        this->_fc_graph->get_neighbours(i_fc), i_fc,
        this->_fc_graph->_volumes[i_fc], this->_fc_graph->get_weights(i_fc));
    }
  }

  /** @brief Analyse subgraph and update the connectivity */
  inline void update_connectivity() {
    _is_connected = _cc_graph->check_connectivity();
    _is_connectivity_up_to_date = true;
  }

  /** @brief Shared pointer of the subgraph structure (CSR representation) */
  SubGraphPtr _cc_graph;

  /** @brief Whether the cell connected */
  bool _is_connected;

  /** @brief Whether the connectivity has been checked */
  bool _is_connectivity_up_to_date;
};

}  // end namespace comma

#endif  // COMMA_PROJECT_COARSE_CELL_H
