#ifndef COMMA_PROJECT_COARSE_CELL_CONTAINER_H
#define COMMA_PROJECT_COARSE_CELL_CONTAINER_H

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

#include <map>
#include <memory>

#include "Dual_Graph.h"
#include "Coarse_Cell.h"

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
using MapIterator = typename map<
    CoMMAIndexType, shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType,
                                        CoMMAIntType>>>::iterator;

/** @brief Class implementing a Coarse_Cell_Container object, that is where
 * the coarse cells are stored.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Coarse_Cell_Container {

 public:
  /** @brief Create a Coarse_Cell_Container
   *  @param[in] fc_graph Input element Dual_Graph to work on the seeds choice
   * and the seeds pool
   */
  Coarse_Cell_Container(
      const Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &fc_graph)
      : _cc_vec(), _fc_graph(fc_graph), _cc_counter(0),
        _fc_2_cc(fc_graph._number_of_cells, -1),
        _a_is_fc_agglomerated(fc_graph._number_of_cells, false),
        _nb_of_agglomerated_fc(0), _delayed_cc() {}

  /** @brief Destructor*/
  ~Coarse_Cell_Container() {}

  /** @brief map container of the CSR representation of the coarse cells */
  map<CoMMAIndexType,
      shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>>
      _cc_vec;

  /** @brief Dual graph representation */
  Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> _fc_graph;

  /** @brief Number of coarse cells */
  CoMMAIndexType _cc_counter = 0;

  /** @brief Output vector identifying to which coarse cell the fine cell belongs */
  vector<CoMMAIndexType> _fc_2_cc;

  /** @brief Vector of boolean telling whether a fine cell has been agglomerated */
  vector<bool> _a_is_fc_agglomerated;

  /** @brief Helper to get the member variable that defines the number of
   * agglomerated fine cells
   *  @return The number of fine cells agglomerated so far
   **/
  inline CoMMAIndexType get_number_of_fc_agglomerated() const {
    return (_nb_of_agglomerated_fc);
  }

  /** @brief Helper to get the number of coarse cells
   *  @return The number of coarse cells created so far
   **/
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
   **/
  vector<CoMMAIndexType> get_neigh_cc(const CoMMAIndexType &i_fc,
                                      const CoMMAIndexType &i_cc) const {
    const vector<CoMMAIndexType> neigh = _fc_graph.get_neighbours(i_fc);
    vector<CoMMAIndexType> result;
    for (const CoMMAIndexType &elem : neigh) {
      const auto cc = _fc_2_cc[elem];
      if (cc != i_cc &&
          find(result.begin(), result.end(), cc) == result.end()) {
        result.push_back(cc);
      }
    }
    return (result);
  }

  /** @brief Update the member variable of the member fc2cc based on the mapping
   * given
   *  @param[in] mapping mapping on old fc2cc to the new fc2cc
   **/
  void update_fc_2_cc(const vector<CoMMAIndexType> &mapping) const {
    for (auto &elem : _fc_2_cc) {
      // we substitute in elem the mapped element
      elem = mapping[elem];
    }
  }

  /** @brief Remove a coarse cell from the mapping
   * @param[in] elim iterator to the element to eliminate
   * @return iterator to the next element
   **/
  MapIterator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> remove_cc(
      MapIterator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> elim) {
    // we delete the element and we obtained the pointer to the next element in
    // memory
    MapIterator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> it =
        _cc_vec.erase(elim);
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

  /** @brief Implementation of the Correction. In this version it implements the
   * correction of singular cells (if one cell is alone after the agglomeration
   * step is agglomerated to a neighbouring cell)
   * @param max_card Maximum cardinality allowed
   **/
  void correct(const CoMMAIntType max_card) {
    // initializing vector neigh_cc
    // We cycle on the subgraphs of the bimap structure
    auto it = _cc_vec.begin();
    // We use it to understand if we have succeeded in the correction
    auto it_old = _cc_vec.begin();
    auto end = _cc_vec.end();
    while (it != end) {
      // We enter in the property of the subgraph of being 1
      // and we consider what happens. Remember that second because
      // we are checking the subgraph
      auto current_cc = it->second;
      auto i_cc = it->first;
      // check the isotropic cells with cardinality 1
      if (current_cc->_cardinality == 1 && current_cc->_is_isotropic) {
        // Get the cc neigh of the given fine cell
        auto i_fc = current_cc->_mapping_l_to_g[0];
        const vector<CoMMAIndexType> neigh = get_neigh_cc(i_fc, i_cc);
        if(!neigh.empty()) {
          // now we have the neighborhood cc cell, we can access to them and
          // control the characteristics
          const auto cc_idx = select_best_cc_to_agglomerate(i_fc, neigh, max_card);
          if (cc_idx.has_value()) {
            // If the condition is verified we add the cell to the identified cc
            // and we remove it from the current cc
            // first we assign to the fc_2_cc the new cc (later it will be
            // renumbered considering the deleted cc)
            _fc_2_cc[i_fc] = cc_idx.value();
            auto neig_cc = _cc_vec[cc_idx.value()];
            neig_cc->insert_node(_fc_graph.get_neighbours(i_fc),
                                 i_fc, _fc_graph._volumes[i_fc],
                                 _fc_graph.get_weights(i_fc));
            current_cc->remove_node(i_fc);
            // the new it point directly to the next element in the map
            it = remove_cc(it);
          } else {
            // if we failed we go on, it is life, so we agglomerate to the nearest
            // cell (the first one of the vector). At this point, we do not check if
            // the max cardinality has been reached or not, otherwise we might leave
            // the isolated cell isolated
            auto const elem = neigh[0];
            auto neig_cc = _cc_vec[elem];
            _fc_2_cc[i_fc] = elem;
            neig_cc->insert_node(_fc_graph.get_neighbours(i_fc),
                                 i_fc, _fc_graph._volumes[i_fc],
                                 _fc_graph.get_weights(i_fc));
            current_cc->remove_node(i_fc);
            // the new it point directly to the next element in the map
            it = remove_cc(it);
          }
        } else {
          // The cell has no neighbors. This could happen when the partitioning does
          // not give a connected partition. Unfortunately, there is nothing that we
          // can do. We just skip it
          ++it;
        }
        end = _cc_vec.end();
        it_old = it;
      } else {
        ++it;
      }
    }
  }

  /** @brief Choose among the neighboring coarse cells, the one to which a fine cell
   * should be assigned to. We prefer the coarse cell which shares the most faces
   * with the fine cell. Otherwise, we look at the cardinality and choose the coarse
   * cell with the smallest one
   * @param[in] fc Index of the fine cell
   * @param[in] cc Neighboring coarse cells
   * @param[in] max_card Maximum cardinality allowed
   * @return The index of the chosen coarse cell wrt to the given neighborhood
   */
  optional<CoMMAIndexType> select_best_cc_to_agglomerate(
      const CoMMAIndexType fc, const vector<CoMMAIndexType> &neigh,
      const CoMMAIntType max_card) const {
    CoMMAUnused(max_card);
    unordered_map<CoMMAIndexType, CoMMAIntType> card{};
    unordered_map<CoMMAIndexType, CoMMAIntT> shared_faces{};
    card.reserve(neigh.size());
    shared_faces.reserve(neigh.size());
    CoMMAIntType min_card = numeric_limits<CoMMAIntType>::max();
    deque<CoMMAIndexType> argmin_card{};
    CoMMAIntType max_shared_f{0};
    deque<CoMMAIndexType> argmax_shared_f{};
    for (auto n_idx = decltype(neigh.size()){0}; n_idx < neigh.size(); ++n_idx) {
      const auto n_cc = _cc_vec.at(neigh[n_idx]);
      if (n_cc->_compactness > 0 && n_cc->_is_isotropic &&
          //n_cc->_cardinality < max_card &&
          n_cc->_cardinality >= 2) {
        // On second thought, let us consider also cells with max cardinality since
        // the number of faces could be important to ensure compactness of the coarse
        // cell
        const auto cur_card = n_cc->_cardinality;
        card[n_idx] = cur_card;
        if (cur_card < min_card) {
          min_card = cur_card;
          argmin_card.clear();
          argmin_card.push_back(n_idx);
        } else if (cur_card == min_card) {
          argmin_card.push_back(n_idx);
        }
        const auto cur_sf = get_shared_faces(fc, n_cc);
        shared_faces[n_idx] = cur_sf;
        if (cur_sf > max_shared_f) {
          max_shared_f = cur_sf;
          argmax_shared_f.clear();
          argmax_shared_f.push_back(n_idx);
        } else if (cur_sf == max_shared_f) {
          argmax_shared_f.push_back(n_idx);
        }
      }
    }
    // Shared faces prevails in order to have more compact cells...
    if (!argmax_shared_f.empty()) {
      CoMMAIndexType ret_idx{argmax_shared_f[0]};
      CoMMAIntType cur_min{card[ret_idx]};
      // ..but let's see if among all the cells there is one with smaller cardinality
      for (auto i = decltype(argmax_shared_f.size()){1}; i < argmax_shared_f.size(); ++i)
        if (card[argmax_shared_f[i]] < cur_min)
          cur_min = argmax_shared_f[i];
      return ret_idx;
    }
    if (!argmin_card.empty()) {
      // @TODO: I'm not sure what I could consider here to decide which cell to
      // return. The aspect-ratio maybe? In the mean time, I return the first one
      return argmin_card[0];
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
      shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> cc) const {
    const auto n_fc_cc = cc->_number_of_cells;
    CoMMAIntType shared_faces{0};
    for (auto i_fc_cc = decltype(n_fc_cc){0}; i_fc_cc < n_fc_cc; ++i_fc_cc) {
      const auto fc_neighs = _fc_graph.get_neighbours(cc->_mapping_l_to_g[i_fc_cc]);
      shared_faces += count(fc_neighs.begin(), fc_neighs.end(), fc);
    }
    return shared_faces;
  }

  /** @brief It creates a coarse cell based on the set of fine cells given as an
   * input
   * @param[in] s_fc set of fine cells passed as a reference
   * @param[in] is_anisotropic boolean that tells if we are in an anisotropic
   * case or not
   * @param[in] is_creation_delayed based on the agglomerator instruction we
   * define if we delay or not the agglomeration
   * @return a long with the global identifier of the coarse cell
   **/
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
      vol_cc = vol_cc + _fc_graph._volumes[i_fc];
      assert(_fc_2_cc[i_fc] == -1);
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

        // Update of compactness informations:
        //####################################
        assert(new_cc->is_connected());
      }
      // Update of _associatedCoarseCellNumber the output of the current
      // function agglomerate _fc_2_cc is filled with _cc_counter
      for (const auto &i_fc : s_fc) {
        // Only if not isCreationDelayed:
        assert(_fc_2_cc[i_fc] == -1);
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
   **/
  inline void cc_create_all_delayed_cc() {
    for (const unordered_set<CoMMAIndexType> &s_fc : _delayed_cc) {
      cc_create_a_cc(s_fc);
    }
    _delayed_cc.clear();
  }

  /** @brief checks if the fine cell is already or not agglomerated
   * @param[in] i_fc global index of the fine cell to analyse
   * @return true or false with respect to the answer
   **/
  inline bool is_fc_not_already_agglomerated(const CoMMAIndexType &i_fc) const {
    return !_a_is_fc_agglomerated[i_fc];
  }

 protected:
  /** @brief Number of agglomerated fine cells */
  CoMMAIndexType _nb_of_agglomerated_fc = 0;

  /** @brief vector of the set of fine cells composing the too small coarse
   * cells that will be built at the end of the agglomeration process
   **/
  vector<unordered_set<CoMMAIndexType>> _delayed_cc;
};

#endif  // COMMA_PROJECT_COARSE_CELL_GRAPH_H
