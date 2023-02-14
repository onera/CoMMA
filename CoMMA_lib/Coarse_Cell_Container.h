#ifndef COMMA_PROJECT_COARSE_CELL_CONTAINER_H
#define COMMA_PROJECT_COARSE_CELL_CONTAINER_H

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

#include <algorithm>
#include <cassert>
#include <deque>
#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Coarse_Cell.h"
#include "Dual_Graph.h"

using namespace std;

/** @brief Class implementing a custom container where the coarse cells are stored.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Coarse_Cell_Container {

 public:

  /** @brief Type for a custom map */
  using CustomMapItT = typename map<
    CoMMAIndexType, shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType,
                                        CoMMAIntType>>>::iterator;

  /** @brief Create a Coarse_Cell_Container
   *  @param[in] fc_graph Input element Dual_Graph to work on the seeds choice
   * and the seeds pool
   */
  Coarse_Cell_Container(
      shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> &fc_graph)
      : _cc_vec(), _fc_graph(fc_graph), _cc_counter(0),
        _fc_2_cc(fc_graph->_number_of_cells, nullopt),
        _a_is_fc_agglomerated(fc_graph->_number_of_cells, false),
        _nb_of_agglomerated_fc(0), _delayed_cc(), _singular_cc() {}

  /** @brief Destructor */
  ~Coarse_Cell_Container() = default;

  /** @brief Map container of the CSR representation of the coarse cells */
  map<CoMMAIndexType,
      shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>>
      _cc_vec;

  /** @brief Dual graph representation */
  shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> _fc_graph;

  /** @brief Number of coarse cells */
  CoMMAIndexType _cc_counter = 0;

  /** @brief Output vector identifying to which coarse cell the fine cell belongs */
  vector<optional<CoMMAIndexType>> _fc_2_cc;

  /** @brief Vector of boolean telling whether a fine cell has been agglomerated */
  vector<bool> _a_is_fc_agglomerated;

  /** @brief Helper to get the member variable that defines the number of
   * agglomerated fine cells
   *  @return The number of fine cells agglomerated so far
   */
  inline CoMMAIndexType get_number_of_fc_agglomerated() const {
    return (_nb_of_agglomerated_fc);
  }

  /** @brief Helper to get the number of coarse cells
   *  @return The number of coarse cells created so far
   */
  inline CoMMAIndexType get_nb_of_cc() const {
    return _cc_counter;
  }

  /** @brief Retrieve the indexes of the neighbouring coarse cells to a given
   * fine cell in a coarse cell (excluding the given coarse cell in which the
   * fine cell is)
   *  @param[in] i_fc index of the fine cell inside the coarse cell to be
   * analysed
   *  @param[in] i_cc index of the coarse cell in which the fine cell is in
   *  @return vector of the index of the coarse cells
   */
  vector<CoMMAIndexType> get_neighs_cc(const CoMMAIndexType &i_fc,
                                       const CoMMAIndexType &i_cc) const {
    const vector<CoMMAIndexType> neigh = _fc_graph->get_neighbours(i_fc);
    vector<CoMMAIndexType> result;
    for (const CoMMAIndexType &elem : neigh) {
      const auto cc = _fc_2_cc[elem].value();
      if (cc != i_cc &&
          find(result.begin(), result.end(), cc) == result.end()) {
        result.push_back(cc);
      }
    }
    return (result);
  }

#if 0
Not used anymore but we leave it for example purposes
  /** @brief Remove a coarse cell from the mapping
   * @param[in] elim Iterator to the element to eliminate
   * @return Iterator to the next element
   */
  CustomMapItT remove_cc(CustomMapItT elim) {
    // we delete the element and we obtained the pointer to the next element in
    // memory
    CustomMapItT it = _cc_vec.erase(elim);
    // update value of the other nodes
    for (auto i = it; i != _cc_vec.end(); i++) {
      for (auto const &i_fc : i->second->_mapping_l_to_g) {
        _fc_2_cc[i_fc] = (i->first) - 1;
      }
      auto node = _cc_vec.extract(i);
      if (!node.empty()) {
        node.key() = (i->first) - 1;
        _cc_vec.insert(move(node));
      }
    }
    // return pointer to the next element
    return (it);
  }
#endif

  /** @brief Implementation of the correction. In this version it implements the
   * correction of singular cells (if one cell is alone after the agglomeration
   * step is agglomerated to a neighbouring cell)
   * @param max_card Maximum cardinality allowed (CoMMA might still be beyond this
   * value)
   */
  void correct(const CoMMAIntType max_card) {
    // We use it to understand if we have succeeded in the correction
    set<typename decltype(_singular_cc)::value_type> removed_cc{};
    for (const auto& i_cc : _singular_cc) {
      // We enter in the property of the subgraph of being 1
      // and we consider what happens. Remember that second because
      // we are checking the subgraph
      //auto current_cc = _cc_vec[i_cc];
      auto i_fc = _cc_vec[i_cc]->_mapping_l_to_g[0];
      // Get the cc neighs of the given fine cell
      const vector<CoMMAIndexType> neighs = get_neighs_cc(i_fc, i_cc);
      if(!neighs.empty()) {
        // now we have the neighbourhood cc cell, we can access to them and
        // control the characteristics
        const auto cc_idx = select_best_cc_to_agglomerate(i_fc, neighs, max_card);
        // If the condition is verified we add the cell to the identified cc
        // and we remove it from the current cc
        // if we failed we go on, it is life, so we agglomerate to the nearest
        // cell (the first one of the vector). At this point, we do not check if
        // the max cardinality has been reached or not, otherwise we might leave
        // the isolated cell isolated
        const auto new_cc = cc_idx.has_value() ? cc_idx.value() : neighs[0];
        auto neig_cc = _cc_vec[new_cc];
        // first we assign to the fc_2_cc the new cc (later it will be
        // renumbered considering the deleted cc)
        _fc_2_cc[i_fc] = new_cc;
        neig_cc->insert_node(_fc_graph->get_neighbours(i_fc),
                             i_fc, _fc_graph->_volumes[i_fc],
                             _fc_graph->get_weights(i_fc));
        _cc_vec.erase(i_cc);
        removed_cc.emplace(i_cc);
      }
      // If the cell has no neighbour (this could happen when the partitioning does
      // not give a connected partition), unfortunately, there is nothing that we
      // can do. We just skip it
    }

    // Now update the ID if necessary
    if (!removed_cc.empty()) {
      auto new_ID = *removed_cc.begin();
      // Starting from the CC just after the first removed singular cell, update all
      // cells. Looking for new_ID-1 than doing ++ avoid case of consecutive singular
      // cells
      for(auto &it_cc = ++_cc_vec.find(new_ID - 1); it_cc != _cc_vec.end();
          ++it_cc, ++new_ID) {
        // Update fine cells
        for (auto const &i_fc : it_cc->second->_mapping_l_to_g) {
          _fc_2_cc[i_fc] = new_ID;
        }
        // Update coarse cell ID
        auto node = _cc_vec.extract(it_cc);
        if (!node.empty()) {
          node.key() = new_ID;
          _cc_vec.insert(move(node));
        }
      }
    }

  }

  /** @brief Choose among the neighbouring coarse cells, the one to which a fine cell
   * should be assigned to. We prefer the coarse cell which shares the most faces
   * with the fine cell. Otherwise, we look at the cardinality and choose the coarse
   * cell with the smallest one
   * @param[in] fc Index of the fine cell
   * @param[in] neigh Neighbouring coarse cells
   * @param[in] max_card Maximum cardinality allowed (CoMMA might still be beyond this
   * value)
   * @return The index of the chosen coarse cell
   */
  optional<CoMMAIndexType> select_best_cc_to_agglomerate(
      const CoMMAIndexType fc, const vector<CoMMAIndexType> &neigh,
      const CoMMAIntType max_card) const {
    CoMMAUnused(max_card);
    unordered_map<CoMMAIndexType, CoMMAIntType> card{};
    unordered_map<CoMMAIndexType, CoMMAIntType> shared_faces{};
    unordered_map<CoMMAIndexType, bool> compact_increase{};
    card.reserve(neigh.size());
    shared_faces.reserve(neigh.size());
    compact_increase.reserve(neigh.size());
    CoMMAIntType min_card = numeric_limits<CoMMAIntType>::max();
    deque<CoMMAIndexType> argmin_card{};
    CoMMAIntType max_shared_f{0};
    deque<CoMMAIndexType> argmax_shared_f{};
    deque<CoMMAIndexType> argtrue_compact{};
    // Loop on neighbours to compute their features
    for (const auto & cc_idx : neigh) {
      const auto n_cc = _cc_vec.at(cc_idx);
      if (n_cc->_compactness > 0 && n_cc->_is_isotropic &&
          //n_cc->_cardinality < max_card &&
          n_cc->_cardinality >= 2) {
        // On second thought, let us consider also cells with max cardinality since
        // the number of faces could be important to ensure compactness of the coarse
        // cell
        const auto cur_card = n_cc->_cardinality;
        card[cc_idx] = cur_card;
        if (cur_card < min_card) {
          min_card = cur_card;
          argmin_card.clear();
          argmin_card.push_back(cc_idx);
        } else if (cur_card == min_card) {
          argmin_card.push_back(cc_idx);
        }
        // @TODO: merge computation of shared faces and compactness?
        const auto cur_sf = get_shared_faces(fc, n_cc);
        shared_faces[cc_idx] = cur_sf;
        if (cur_sf > max_shared_f) {
          max_shared_f = cur_sf;
          argmax_shared_f.clear();
          argmax_shared_f.push_back(cc_idx);
        } else if (cur_sf == max_shared_f) {
          argmax_shared_f.push_back(cc_idx);
        }
        if (new_cell_increases_compactness(fc, n_cc)) {
          compact_increase[cc_idx] = true;
          argtrue_compact.push_back(cc_idx);
        } else {
          compact_increase[cc_idx] = false;
        }
      }
    }
    // Now, it's time to choose the best neighbours. Priority is given to those which:
    // 1 - Increase the degree of compactness
    if (!argtrue_compact.empty()) {
      // Sort so that, in the end, if nothing worked, we rely on ID numbering
      sort(argtrue_compact.begin(), argtrue_compact.end());
      CoMMAIndexType ret_cc{argtrue_compact[0]};
      CoMMAIntType cur_max{shared_faces[ret_cc]};
      // If more than one, maximize shared faces and/or minimize cardinality
      for (const auto & idx : argtrue_compact) {
        const auto cur_shf = shared_faces[idx];
        if (cur_shf > cur_max) {
          cur_max = cur_shf;
          ret_cc = idx;
        } else if (cur_shf == cur_max && card[idx] < card[ret_cc]) {
          ret_cc = idx;
        }
      }
      return ret_cc;
    }
    // 2 - Maximize the number of shared faces
    if (!argmax_shared_f.empty()) {
      // Sort so that, in the end, if nothing worked, we rely on ID numbering
      sort(argmax_shared_f.begin(), argmax_shared_f.end());
      CoMMAIndexType ret_cc{argmax_shared_f[0]};
      CoMMAIntType cur_min{card[ret_cc]};
      // ..but let's see if among all the cells there is one with smaller cardinality
      for (const auto & idx : argmax_shared_f) {
        if (card[idx] < cur_min) {
          ret_cc = idx;
          cur_min = card[ret_cc];
        }
      }
      return ret_cc;
    }
    // 3 - Minimize the cardinality
    if (!argmin_card.empty()) {
      // We should never need to come here...
      // @TODO: I'm not sure what I could consider here to decide which cell to
      // return. The aspect-ratio maybe? In the mean time, I return the one with the lowest ID
      return *min_element(argmin_card.begin(), argmin_card.end());
    }
    // If everything failed, return dummy
    return nullopt;
  }

  /** @brief Compute the number of faces shared between a fine cell and a coarse one
   * @param[in] fc Index of the fine cell
   * @param[in] cc Subgraph representing the coarse cell
   * @return The number of shared faces
   */
  inline CoMMAIntType get_shared_faces(const CoMMAIndexType fc,
      const shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> cc) const {
    const auto n_fc_cc = cc->_number_of_cells;
    CoMMAIntType shared_faces{0};
    // I am not 100% sure that mapping is perfect hence I prefer loop using indices
    for (auto i_fc_cc = decltype(n_fc_cc){0}; i_fc_cc < n_fc_cc; ++i_fc_cc) {
      const auto fc_neighs = _fc_graph->get_neighbours(cc->_mapping_l_to_g[i_fc_cc]);
      shared_faces += count(fc_neighs.begin(), fc_neighs.end(), fc);
    }
    return shared_faces;
  }

  /** @brief Tell if the addition of a new fine cell increase the compactness degree
   * of a coarse cell
   * @param[in] fc Index of the fine cell
   * @param[in] cc Subgraph representing the coarse cell
   * @return A boolean
   */
  inline bool new_cell_increases_compactness(const CoMMAIndexType fc,
      const shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> cc) const {
    // Set of faces in the CC
    unordered_set<CoMMAIndexType> tmp_cc{cc->_mapping_l_to_g.begin(),
                                         cc->_mapping_l_to_g.end()};
    tmp_cc.insert(fc);
    return _fc_graph->compute_min_fc_compactness_inside_a_cc(tmp_cc) > cc->_compactness;
  }

  /** @brief It creates a coarse cell based on the set of fine cells given as an
   * input
   * @param[in] s_fc Set of fine cells passed as a reference
   * @param[in] is_anisotropic Boolean that tells if we are in an anisotropic
   * case or not
   * @param[in] is_creation_delayed Based on the agglomerator instruction we
   * define if we delay or not the agglomeration
   * @return Global identifier of the coarse cell
   */
  CoMMAIndexType cc_create_a_cc(
      const unordered_set<CoMMAIndexType> &s_fc, bool is_anisotropic = false,
      bool is_creation_delayed = false) {
    // Create a course cell from the fine cells and update the fc_2_cc tree.
    assert((!is_anisotropic) || (!is_creation_delayed));
    // error handling
    CoMMAWeightType vol_cc = 0;
    for (const auto &i_fc : s_fc) {
      if (!_a_is_fc_agglomerated[i_fc]) {
        // Rq: initialise to False pour chaque niveau dans agglomerate(...)
        _a_is_fc_agglomerated[i_fc] = true;
        _nb_of_agglomerated_fc++;
      }
    }
    for (const auto &i_fc : s_fc) {
      vol_cc = vol_cc + _fc_graph->_volumes[i_fc];
      assert(!_fc_2_cc[i_fc].has_value());
    }
    // Anisotropic case
    bool is_mutable = true;
    if (is_anisotropic) {
      assert(!is_creation_delayed);
      auto new_cc = make_shared<
          Coarse_Cell<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
          _fc_graph, _cc_counter, s_fc, false);
      // we collect the various cc_graph, where the index in the vector is the
      // i_cc
      _cc_vec.insert(pair<
          CoMMAIndexType,
          shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>>(
          _cc_counter, new_cc->_cc_graph));
      is_mutable = false;
    }
    if (!is_creation_delayed) {
      // We create the cc right now.
      // Everything is updated:
      if (is_mutable) {
        // the cell can be modified afterwards and is thus defined in dict_cc
        // and dict_card_cc, dict_compactness_2_cc, dict_cc_to_compactness
        // Update of dict_cc:
        //==================
        auto new_cc = make_shared<
            Coarse_Cell<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
            _fc_graph, _cc_counter, s_fc);
        // we collect the various cc_graph, where the index in the vector is the
        // i_cc
        _cc_vec.insert(
            pair<CoMMAIndexType,
                 shared_ptr<
                     Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>>(
                _cc_counter, new_cc->_cc_graph));
        if (s_fc.size() == 1)
          _singular_cc.emplace_back(_cc_counter);

        // Update of compactness informations:
        //####################################
        assert(new_cc->is_connected());
      }
      // Update of _associatedCoarseCellNumber the output of the current
      // function agglomerate _fc_2_cc is filled with _cc_counter
      for (const auto &i_fc : s_fc) {
        // Only if not isCreationDelayed:
        assert(!_fc_2_cc[i_fc].has_value());
        _fc_2_cc[i_fc] = _cc_counter;
      }
      // Update of the number of CC
      //####################################
      _cc_counter++;  // Only if not isCreationDelayed
    } else {
      // We do not create the coarse cell yet.
      // As this coarse cell will be soon deleted, we want its coarse index to
      // be the greater possible.
      // Only isFineCellAgglomerated_tmp, number_of_fine_agglomerated_cells_tmp
      // and dict_DistributionOfCardinalOfCoarseElements are modified!
      _delayed_cc.push_back(s_fc);
    }
    return (_cc_counter - 1);
  }

  /** @brief Creates all the delayed coarse cell. It works only when the delayed
   * cell flag is activated in the agglomerator
   */
  inline void cc_create_all_delayed_cc() {
    for (const unordered_set<CoMMAIndexType> &s_fc : _delayed_cc) {
      cc_create_a_cc(s_fc);
    }
    _delayed_cc.clear();
  }

  /** @brief Checks if the fine cell is already or not agglomerated
   * @param[in] i_fc Global index of the fine cell to analyse
   * @return true or false with respect to the answer
   */
  inline bool is_fc_not_already_agglomerated(const CoMMAIndexType &i_fc) const {
    return !_a_is_fc_agglomerated[i_fc];
  }

 protected:
  /** @brief Number of agglomerated fine cells */
  CoMMAIndexType _nb_of_agglomerated_fc = 0;

  /** @brief Vector of the set of fine cells composing the too small coarse
   * cells that will be built at the end of the agglomeration process
   */
  vector<unordered_set<CoMMAIndexType>> _delayed_cc;

  /** @brief Set of singular coarse cells, that is, composed of only one fine cell */
  deque<CoMMAIndexType> _singular_cc;
};

#endif  // COMMA_PROJECT_COARSE_CELL_GRAPH_H
