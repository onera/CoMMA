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

#include <memory>
#include <map>

#include "Dual_Graph.h"
#include "Coarse_Cell.h"

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
using MapIterator = typename map<
    CoMMAIndexType, shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType,
                                        CoMMAIntType>>>::iterator;

/** @brief Create a Coarse Cell Graph, hence it is the
 *  container of the Coarse Cells created and
 *  of the operation we can do on them.
 *  @param[in] Dual_Graph input element Dual_Graph to work on the seeds choice
 * and the seeds pool
 *  @param[in] verbose it defines the verbose parameters
*/
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Coarse_Cell_Container {

 public:
  /** @brief Constructor*/
  Coarse_Cell_Container(
      const Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &fc_graph)
      : _fc_graph(fc_graph) {
    // Initialization of the vector of the agglomerated cells
    // as false at the beginning, are all not agglomerated.
    /** @todo Change to a more meaningful name */
    vector<bool> tmp(fc_graph._number_of_cells, false);
    _a_is_fc_agglomerated = tmp;
    // field that identify the index of the coarse cells in the
    // fine cell (and retrieved as the output)
    _fc_2_cc = vector<CoMMAIndexType>(fc_graph._number_of_cells, -1);
  };

  /** @brief Destructor*/
  ~Coarse_Cell_Container() {};
  /** @brief Function that return if the coarse cell structure created is
   * unstructured or structured (in dependence of the anisotropic  cells found)
   * @param[in] goal_card goal cardinality, useful to check if in case of non
   * presence of anisotropic cells we reached * the goal cardinality for all
   * the coarse cells created
   */
  bool is_cc_grid_not_structured(CoMMAIntType goal_card = -1);
  /** @brief Helper to get the member variable that defines the number of
   * agglomerated fine cells
   **/
  inline CoMMAIndexType get_number_of_fc_agglomerated() {
    return (_nb_of_agglomerated_fc);
  };

  /** @brief Helper to get the number of coarse cells */
  inline CoMMAIndexType get_nb_of_cc() {
    return _cc_counter;
  };
  /** @brief map container of the CSR representation of the coarse cells */
  map<CoMMAIndexType,
      shared_ptr<Subgraph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>>
      _cc_vec;
  /** @brief Retrieve the indexes of the neighbouring coarse cells to a given
   * fine cell in a coarse cell (excluding the given coarse cell in which the
   * fine cell is)
   *  @param[in] i_fc index of the fine cell inside the coarse cell to be
   * analysed
   *  @param[in] i_cc index of the coarse cell in which the fine cell is in
   *  @return vector of the index of the coarse cells
   **/
  vector<CoMMAIndexType> get_neigh_cc(const CoMMAIndexType &i_fc,
                                      const CoMMAIndexType &i_cc) {
    vector<CoMMAIndexType> neigh = _fc_graph.get_neighbours(i_fc);
    vector<CoMMAIndexType> result;
    for (const CoMMAIndexType &elem : neigh) {
      if (_fc_2_cc[elem] != i_cc) {
        result.push_back(_fc_2_cc[elem]);
      }
    }
    return (result);
  };
  /** @brief Update the member variable of the memebr fc2cc based on the mapping
   * given
   *  @param[in] mapping mapping on old fc2cc to the new fc2cc
   **/
  void update_fc_2_cc(const vector<CoMMAIndexType> &mapping) {
    for (auto &elem : _fc_2_cc) {
      // we substitute in elem the mapped element
      elem = mapping[elem];
    }
  };

  /** @brief Remove a coarse cell from the mapping
   * @param[in] elim iterator to the element to eliminate
   * @return iterator to the next element
   **/
  MapIterator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> remove_cc(
      MapIterator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> elim) {
    // we delete the element and we obtainer the pointer to the next element in
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
  };
  /** @brief Implementation of the Correction. In this version it implements the
   * correction of singular cells (if one cell is alone after the agglomeration
   * step is agglomerated to a neighbouring cell)
   **/
  void correct() {
    // initializing vector neigh_cc
    vector<CoMMAIndexType> neigh;
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
        neigh = get_neigh_cc(i_fc, i_cc);
        if(!neigh.empty()){
        // now we have the neighborhood cc cell, we can access to them and
        // control the characteristics
        for (auto const &elem : neigh) {
          auto neig_cc = _cc_vec[elem];
          if (neig_cc->_compactness > 0 && neig_cc->_cardinality >= 2 &&
              neig_cc->_is_isotropic) {
            // If the condition is verified we add the cell to the identified cc
            // and we remove it from the current cc
            // first we assign to the fc_2_cc the new cc (later it will be
            // renumbered considering the deleted cc)
            _fc_2_cc[i_fc] = elem;
            vector<CoMMAIndexType> fine_neigh = _fc_graph.get_neighbours(i_fc);
            vector<CoMMAWeightType> fine_weights = _fc_graph.get_weights(i_fc);
            neig_cc->insert_node(fine_neigh, i_fc, _fc_graph._volumes[i_fc],
                                 fine_weights);
            current_cc->remove_node(i_fc);
            // the new it point directly to the next element in the map
            it = remove_cc(it);
            break;
          }
        }
        // if we failed we go on, it is life, so we agglomerate to the nearest
        // cell (the first one of the vector
        if (it == it_old) {
          auto const elem = neigh[0];
          auto neig_cc = _cc_vec[elem];
          _fc_2_cc[i_fc] = elem;
          vector<CoMMAIndexType> fine_neigh = _fc_graph.get_neighbours(i_fc);
          vector<CoMMAWeightType> fine_weights = _fc_graph.get_weights(i_fc);
          neig_cc->insert_node(fine_neigh, i_fc, _fc_graph._volumes[i_fc],
                               fine_weights);
          current_cc->remove_node(i_fc);
          // the new it point directly to the next element in the map
          it = remove_cc(it);
        }
       } else {++it;};
        end = _cc_vec.end();
        neigh.clear();
        it_old = it;
      } else {
        ++it;
      }
    }
  };

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
        assert((*new_cc).is_connected());
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
  };

  /** @brief Vector of boolean for which the length is the number of fine cells
  * and for which the value of i_fc cell is true when the cell is agglomerated in a
  * coarse cell
  **/
  vector<bool> _a_is_fc_agglomerated;

  /** @brief Creates all the delayed coarse cell. It works only when the delayed
   * cell flag is activated in the agglomerator
   **/
  inline void cc_create_all_delayed_cc() {
    for (const unordered_set<CoMMAIndexType> &s_fc : _delayed_cc) {
      cc_create_a_cc(s_fc);
    }
    _delayed_cc.clear();
  };

  /** @brief checks if the fine cell is already or not agglomerated
   * @param[in] i_fc global index of the fine cell to analyse
   * @return true or false with respect to the answer
   **/
  inline bool is_fc_not_already_agglomerated(const CoMMAIndexType &i_fc) const {
    return !_a_is_fc_agglomerated[i_fc];
  }

  /** @brief Dual graph representation.*/
  Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> _fc_graph;
  /** @brief Number of coarse cells in the Coarse cell Graph*/
  CoMMAIndexType _cc_counter = 0;
  /** @brief Output vector of the fine cells to the coarse cell. The vector
   * long as much as the fine cells have as a value the corresponding coarse
   * cell id
   **/
  vector<CoMMAIndexType> _fc_2_cc;

 protected:
  /** @brief Number of agglomerated fine cells */
  CoMMAIndexType _nb_of_agglomerated_fc = 0;
  /** @brief vector of the set of fine cells composing the too small coarse
   * cells that will be built at the end of the agglomeration process
   **/
  vector<unordered_set<CoMMAIndexType>> _delayed_cc;
};

#endif  // COMMA_PROJECT_COARSE_CELL_GRAPH_H
