#ifndef COMMA_PROJECT_COARSE_CELL_H
#define COMMA_PROJECT_COARSE_CELL_H

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

#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Dual_Graph.h"

/** @brief Class implementing a Coarse_Cell object.
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
   * @param[in] s_fc unordered set of fine cells constituting the coarse cell
   * @param[in] is_isotropic boolean describing if the cell is coming from an
   * isotropic agglomeration process or an anisotropic agglomeration process.
   * The default value is set to true.
   */
  Coarse_Cell(
      shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> fc_graph,
      CoMMAIndexType i_cc, const unordered_set<CoMMAIndexType> &s_fc,
      bool is_isotropic = true)
      : _idx(i_cc), _fc_graph(fc_graph), _is_isotropic(is_isotropic),
      _is_connected(false), _is_connectivity_up_to_date(false), _s_fc(s_fc) {
    // It also initializes _fc_volumes, _adjMatrix_row_ptr, _adjMatrix_col_ind, and
    // _adjMatrix_areaValues
    build_local_CRS();

    _cc_graph =
        make_shared<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
            s_fc.size(), _adjMatrix_row_ptr, _adjMatrix_col_ind,
            _adjMatrix_areaValues, _fc_volumes, _mapping_g_to_l, is_isotropic);
  }

  /** @brief Destructor of the class */
  ~Coarse_Cell() = default;

  /** @brief Index of the coarse cell (It seems to be unused, but it useful to have) */
  CoMMAIndexType _idx;

  /** @brief mapping vector. The position of the index is the local node, the
   * value is the global
   */
  vector<CoMMAIndexType> _mapping_g_to_l;

  /** @brief The row pointer of the CSR representation of the subgraph */
  vector<CoMMAIndexType> _adjMatrix_row_ptr;

  /** @brief The column index representation of the CSR representation */
  vector<CoMMAIndexType> _adjMatrix_col_ind;

  /** @brief The area value of the internal fine cells */
  vector<CoMMAWeightType> _adjMatrix_areaValues;

  /** @brief The volumes of the internal fine cells */
  vector<CoMMAWeightType> _fc_volumes;

  /** @brief shared pointer of the subgraph structure (CSR representation) */
  shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> _cc_graph;

  /** @brief The global dual graph*/
  shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> _fc_graph;

  /** @brief Is the cell isotropic or anisotropic */
  bool _is_isotropic;

  /** @brief is the cell connected */
  bool _is_connected;

  /** @brief the connectivity has been checked */
  bool _is_connectivity_up_to_date;

  /** @brief Set of fine cells composing the Coarse cell */
  unordered_set<CoMMAIndexType> _s_fc;

  /** @brief Method that return a boolean determining if the Coarse Cell is
   * defined by a connected sub-graph or not.
   *  @return true if the subgraph is connected, false if the subgraph is not
   * connected
   **/
  inline bool is_connected() {
    if (!_is_connectivity_up_to_date) {
      _is_connected = _cc_graph->check_connectivity();
      _is_connectivity_up_to_date = true;
    }
    return _is_connected;
  }

  /** @brief function to build the local CSR subgraph representation. It initializes
   * several members related to the subgraph
   */
  inline void build_local_CRS() {
    // initialization vectors
    CoMMAIndexType position = 0;
    vector<CoMMAWeightType> weight{};
    vector<CoMMAIndexType> row_ptr = {0};
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
          weight.push_back(area[it - neigh.begin()]);
        }
      }
      row_ptr.push_back(position);
      _fc_volumes.push_back(_fc_graph->_volumes[i_fc]);
    }

    _adjMatrix_row_ptr = move(row_ptr);

    // Map in the local subgraph
    for (auto it = col_ind.begin(); it != col_ind.end(); ++it) {
      auto indx = find(mapping.begin(), mapping.end(), *it);
      _adjMatrix_col_ind.push_back(indx - mapping.begin());
    }

    _adjMatrix_areaValues = move(weight);

    _mapping_g_to_l = move(mapping);
  }
};

#endif  // COMMA_PROJECT_COARSE_CELL_H
