#ifndef COMMA_PROJECT_COARSE_CELL_H
#define COMMA_PROJECT_COARSE_CELL_H

/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Dual_Graph.h"

using namespace std;

/** @brief Class describing a coarse cell.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Coarse_Cell {
 public:
  /** @brief Constructor of the class
   * @param[in] fc_graph Dual_Graph object from where are taken the set of fine
   * cells to create the coarse cell.
   * @param[in] i_cc Index of the coarse cell
   * @param[in] s_fc Unordered set of fine cells constituting the coarse cell
   * @param[in] compactness Compactness degree of the CC
   * @param[in] is_isotropic (default = true) boolean describing if the cell is coming from an
   * isotropic agglomeration process or an anisotropic agglomeration process.
   */
  Coarse_Cell(
      shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> fc_graph,
      CoMMAIndexType i_cc, const unordered_set<CoMMAIndexType> &s_fc,
      CoMMAIntType compactness, bool is_isotropic = true)
      : _idx(i_cc), _fc_graph(fc_graph), _compactness(compactness),
      _is_isotropic(is_isotropic), _is_connected(false),
      _is_connectivity_up_to_date(false), _s_fc(s_fc) {
    _cardinality = static_cast<CoMMAIntType>(s_fc.size());
    // initialization vectors
    CoMMAIndexType position = 0;
    vector<CoMMAWeightType> volumes;
    vector<CoMMAWeightType> CSR_vals{};
    vector<CoMMAIndexType> CSR_row = {0};
    vector<CoMMAIndexType> CSR_col{};
    vector<CoMMAIndexType> col_ind{};
    vector<CoMMAIndexType> mapping{};
    for (const CoMMAIndexType &i_fc : _s_fc) {
      // we add to the mapping the i_fc
      mapping.push_back(i_fc);
      // get neighbours and the weights associated
      const vector<CoMMAIndexType> neigh = _fc_graph->get_neighbours(i_fc);
      const vector<CoMMAWeightType> area = _fc_graph->get_weights(i_fc);
      for (auto it = neigh.begin(); it != neigh.end(); ++it) {
        if (find(_s_fc.begin(), _s_fc.end(), *it) != _s_fc.end()) {
          ++position;
          col_ind.push_back(*it);
          CSR_vals.push_back(area[it - neigh.begin()]);
        }
      }
      CSR_row.push_back(position);
      volumes.push_back(_fc_graph->_volumes[i_fc]);
    }

    // Map in the local subgraph
    for (auto it = col_ind.begin(); it != col_ind.end(); ++it) {
      auto indx = find(mapping.begin(), mapping.end(), *it);
      CSR_col.push_back(indx - mapping.begin());
    }

    _cc_graph =
        make_shared<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
            s_fc.size(), CSR_row, CSR_col,
            CSR_vals, volumes, mapping, is_isotropic);
  }

  /** @brief Destructor of the class */
  ~Coarse_Cell() = default;

  /** @brief Index of the coarse cell (It seems to be unused, but useful to have) */
  CoMMAIndexType _idx;

  /** @brief shared pointer of the subgraph structure (CSR representation) */
  shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> _cc_graph;

  /** @brief The global dual graph */
  shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> _fc_graph;

  /** @brief Compactness degree of the CC */
  CoMMAIntType _compactness;

  /** @brief Number of FC in the CC */
  CoMMAIntType _cardinality;

  /** @brief Whether the cell isotropic or anisotropic */
  bool _is_isotropic;

  /** @brief Whether the cell connected */
  bool _is_connected;

  /** @brief Whether the connectivity has been checked */
  bool _is_connectivity_up_to_date;

  /** @brief Set of fine cells composing the Coarse cell */
  unordered_set<CoMMAIndexType> _s_fc;

  /** @brief Method that return a boolean determining if the coarse cell is
   * defined by a connected sub-graph or not.
   *  @return true if the subgraph is connected, false if the subgraph is not
   * connected
   */
  inline bool is_connected() {
    if (!_is_connectivity_up_to_date) {
      _is_connected = _cc_graph->check_connectivity();
      _is_connectivity_up_to_date = true;
    }
    return _is_connected;
  }

  /** @brief Insert a FC in the CC (and update sub-graph if necessary)
   *  @param[in] i_fc Index of the fine cell to add
   */
  inline void insert_cell(const CoMMAIndexType i_fc) {
    _s_fc.insert(i_fc);
    ++_cardinality;
    _cc_graph->insert_node(_fc_graph->get_neighbours(i_fc),
                           i_fc, _fc_graph->_volumes[i_fc],
                           _fc_graph->get_weights(i_fc));
    _compactness = _fc_graph->compute_min_fc_compactness_inside_a_cc(_s_fc);
  }

};

#endif  // COMMA_PROJECT_COARSE_CELL_H
