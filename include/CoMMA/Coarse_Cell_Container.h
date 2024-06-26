#ifndef COMMA_PROJECT_COARSE_CELL_CONTAINER_H
#define COMMA_PROJECT_COARSE_CELL_CONTAINER_H

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
#include <deque>
#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "CoMMA/Coarse_Cell.h"
#include "CoMMA/Dual_Graph.h"

namespace comma {

/** @brief Class implementing a custom container where the coarse cells are
 * stored.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Coarse_Cell_Container {
public:
  /** @brief Type for a Coarse_Cell object */
  using CoarseCellType =
    Coarse_Cell<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;

  /** @brief Type for a shared pointer to a Dual_Graph object */
  using CoarseCellPtr = std::shared_ptr<CoarseCellType>;

  /** @brief Type for a shared pointer to a Dual_Graph object */
  using DualGraphPtr = typename CoarseCellType::DualGraphPtr;

  /** @brief Create a Coarse_Cell_Container
   *  @param[in] fc_graph Input element Dual_Graph to work on the seeds choice
   * and the seeds pool
   *  @param[in] singular_card_thresh Threshold value below which a coarse cell
   * is considered singular
   */
  Coarse_Cell_Container(
    DualGraphPtr &fc_graph, const CoMMAIntType singular_card_thresh
  ) :
    _ccs(),
    _fc_graph(fc_graph),
    _cc_counter(0),
    _fc_2_cc(fc_graph->_number_of_cells, std::nullopt),
    _is_fc_agglomerated(fc_graph->_number_of_cells, false),
    _sing_card_thresh(singular_card_thresh),
    _nb_of_agglomerated_fc(0),
    _delayed_cc(),
    _singular_cc() {}

  /** @brief Destructor */
  ~Coarse_Cell_Container() = default;

  /** @brief Map containing the CC and their ID */
  std::map<CoMMAIndexType, CoarseCellPtr> _ccs;

  /** @brief Dual graph representation */
  DualGraphPtr _fc_graph;

  /** @brief Number of coarse cells */
  CoMMAIndexType _cc_counter;

  /** @brief Output vector identifying to which coarse cell the fine cell
   * belongs */
  std::vector<std::optional<CoMMAIndexType>> _fc_2_cc;

  /** @brief Vector of boolean telling whether a fine cell has been agglomerated
   */
  std::vector<bool> _is_fc_agglomerated;

  /** @brief Minimum cardinality for receiver CC when correcting */
  CoMMAIntType _sing_card_thresh;

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
  inline CoMMAIndexType get_nb_of_cc() const { return _cc_counter; }

  /** @brief Retrieve the indexes of the neighbouring coarse cells to a given
   * fine cell in a coarse cell (excluding the given coarse cell in which the
   * fine cell is)
   *  @param[in] i_fc index of the fine cell inside the coarse cell to be
   * analysed
   *  @param[in] i_cc index of the coarse cell in which the fine cell is in
   *  @return vector of the index of the coarse cells
   */
  inline std::set<CoMMAIndexType> get_neighs_cc(
    const CoMMAIndexType &i_fc, const CoMMAIndexType &i_cc
  ) const {
    std::set<CoMMAIndexType> result;
    for (auto elem = _fc_graph->neighbours_cbegin(i_fc);
         elem != _fc_graph->neighbours_cend(i_fc);
         ++elem) {
      const auto cc = _fc_2_cc[*elem].value();
      if (cc != i_cc) result.insert(cc);
    }
    return result;
  }

#if 0
Not used anymore but we leave it for example purposes

  /** @brief Type for a custom map */
  using CustomMapItT = typename std::map<CoMMAIndexType, SubGraphPtr>::iterator;
  /** @brief Remove a coarse cell from the mapping
   * @param[in] elim Iterator to the element to eliminate
   * @return Iterator to the next element
   */
  CustomMapItT remove_cc(CustomMapItT elim) {
    // we delete the element and we obtained the pointer to the next element in
    // memory
    CustomMapItT it = _ccs.erase(elim);
    // update value of the other nodes
    for (auto i = it; i != _ccs.end(); i++) {
      for (auto const &i_fc : i->second->_mapping_l_to_g) {
        _fc_2_cc[i_fc] = (i->first) - 1;
      }
      auto node = _ccs.extract(i);
      if (!node.empty()) {
        node.key() = (i->first) - 1;
        _ccs.insert(std::move(node));
      }
    }
    // return pointer to the next element
    return (it);
  }
#endif

  /** @brief Implementation of the correction. In this version it implements the
   * correction of singular cells (if one cell is alone after the agglomeration
   * step is agglomerated to a neighbouring cell)
   * @param max_card Maximum cardinality allowed (CoMMA might still be beyond
   * this value)
   */
  void correct(const CoMMAIntType max_card) {
    // We use it to understand if we have succeeded in the correction
    std::set<typename decltype(_singular_cc)::value_type> removed_cc{};
    for (const auto &old_cc : _singular_cc) {
      // It might happen that we agglomerate to a singular cell so that the new
      // cell was singular when it was created but it is not any more
      auto &cur_cc = _ccs.at(old_cc);
      if (cur_cc->_cardinality <= _sing_card_thresh) {
        const auto &fcs = cur_cc->_s_fc;
        bool should_remove = false;
        std::unordered_map<CoMMAIndexType, std::set<CoMMAIndexType>>
          neighs_by_fc{};
        neighs_by_fc.reserve(cur_cc->_cardinality);
        for (const auto &i_fc : fcs) {
          neighs_by_fc.emplace(i_fc, get_neighs_cc(i_fc, old_cc));
        }
        if (cur_cc->_cardinality > 1) {
          // First try: agglomerate the whole coarse cell to another one
          std::set<CoMMAIndexType> glob_neighs = {};
          for (const auto &[i_fc, neighs] : neighs_by_fc) {
            glob_neighs.insert(neighs.begin(), neighs.end());
          }
          if (!glob_neighs.empty()) {
            std::optional<CoMMAIntType> new_compactness = std::nullopt;
            const auto new_cc = select_best_cc_to_agglomerate_whole(
              fcs, glob_neighs, max_card, new_compactness
            );
            if (new_cc.has_value()) {
              // first we assign to the fc_2_cc the new cc (later it will be
              // renumbered considering the deleted cc)
              for (const auto &i_fc : fcs) {
                _fc_2_cc[i_fc] = new_cc.value();
              }
              _ccs[new_cc.value()]->insert_cells(fcs, new_compactness);
              should_remove = true;
            }
          }
        }
        if (!should_remove) {
          // If here, we could not agglomerate the whole cell, hence we look
          // fine cell by fine cell
          for (const auto &[i_fc, neighs] : neighs_by_fc) {
            if (!neighs.empty()) {
              std::optional<CoMMAIntType> new_compactness = std::nullopt;
              const auto new_cc = select_best_cc_to_agglomerate(
                i_fc, neighs, max_card, new_compactness
              );
              if (new_cc.has_value()) {
                _fc_2_cc[i_fc] = new_cc.value();
                _ccs[new_cc.value()]->insert_cell(i_fc, new_compactness);
                should_remove = true;
              }
            }
            // If the cell has no neighbour (this could happen when the
            // partitioning does not give a connected partition), unfortunately,
            // there is nothing that we can do. We just skip it
          }
        }

        if (should_remove) {
          _ccs.erase(old_cc);
          removed_cc.emplace(old_cc);
        }
      }  // End if still singular
    }  // End loop over singular cells

    // Now update the ID if necessary
    if (!removed_cc.empty()) {
      auto new_ID = *(removed_cc.begin());
      // Starting from the CC just after the first removed singular cell, update
      // all cells. Looking for new_ID-1 than doing ++ avoid case of consecutive
      // singular cells. If the first removed cell was cell 0, then start from
      // the beginning
      auto it_cc = _ccs.begin();
      if (new_ID > 0) {
        it_cc = _ccs.find(new_ID - 1);
        ++it_cc;
      }
      for (; it_cc != _ccs.end(); ++it_cc, ++new_ID) {
        // Update fine cells
        for (auto const &i_fc : it_cc->second->_s_fc) {
          _fc_2_cc[i_fc] = new_ID;
        }
        // Update coarse cell ID
        auto node = _ccs.extract(it_cc);
        if (!node.empty()) {
          node.key() = new_ID;
          _ccs.insert(std::move(node));
        }
      }
    }
  }

  /** @brief Choose among the neighbouring coarse cells, the one to which a
   * singular coarse cell should be assigned to. This function is similar to
   * \ref select_best_cc_to_agglomerate but considers only the compactness and
   * the cardinality
   * @param[in] fcs Set of indices of the fine cells composing the coarse cell
   * to be agglomerated
   * @param[in] neighs Neighbouring coarse cells
   * @param[in] max_card Maximum cardinality allowed (CoMMA might still go
   * beyond this value)
   * @param[out] new_compactness Compactness degree of the CC if the result
   * would to be added
   * @return The index of the chosen coarse cell
   * @warning \p max_card might not be honored
   */
  std::optional<CoMMAIndexType> select_best_cc_to_agglomerate_whole(
    const std::unordered_set<CoMMAIndexType> &fcs,
    const std::set<CoMMAIndexType> &neighs,
    const CoMMAIntType max_card,
    std::optional<CoMMAIntType> &new_compactness
  ) const {
    CoMMAUnused(max_card);
    std::unordered_map<CoMMAIndexType, CoMMAIntType> card{};
    std::unordered_map<CoMMAIndexType, CoMMAIntType> compact{};
    const auto n_neighs = neighs.size();
    card.reserve(n_neighs);
    // Since in the end we sort, wouldn't it be better to just use set instead
    // of deque?
    std::deque<CoMMAIndexType> argtrue_compact{};
    // Loop on neighbours to compute their features
    for (const auto &cc_idx : neighs) {
      const auto n_cc = _ccs.at(cc_idx);
      if (n_cc->_is_isotropic) {
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        if (true /* n_cc->_cardinality < max_card */) {
          // On second thought, let us consider also cells with max cardinality
          // since the number of faces could be important to ensure compactness
          // of the coarse cell
          card[cc_idx] = n_cc->_cardinality;
          // Analysing compactness
          auto tmp_cc = n_cc->_s_fc;  // OK copy
          tmp_cc.insert(fcs.begin(), fcs.end());
          const auto new_cpt =
            _fc_graph->compute_min_fc_compactness_inside_a_cc(tmp_cc);
          compact[cc_idx] = new_cpt;
          if (new_cpt > n_cc->_compactness) {
            argtrue_compact.push_back(cc_idx);
          }
        }  // End compactness and cardinality
      }  // End if isotropic
    }
    if (!argtrue_compact.empty()) {
      // Sort so that, in the end, if nothing worked, we rely on ID numbering
      sort(argtrue_compact.begin(), argtrue_compact.end());
      CoMMAIndexType ret_cc{argtrue_compact[0]};
      CoMMAIntType cur_min{card[ret_cc]};
      // If more than one, maximize shared faces and/or minimize cardinality
      for (const auto &idx : argtrue_compact) {
        const auto cur_card = card[idx];
        if (cur_card < cur_min) {
          cur_min = cur_card;
          ret_cc = idx;
        }
      }
      new_compactness = compact.at(ret_cc);
      return ret_cc;
    }
    return std::nullopt;
  }

  /** @brief Choose among the neighbouring coarse cells, the one to which a fine
   * cell should be assigned to. We prefer the coarse cell which shares the most
   * faces with the fine cell. Otherwise, we look at the cardinality and choose
   * the coarse cell with the smallest one
   * @param[in] fc Index of the fine cell
   * @param[in] neighs Neighbouring coarse cells
   * @param[in] max_card Maximum cardinality allowed (CoMMA might still go
   * beyond this value)
   * @param[out] new_compactness Compactness degree of the CC if the result
   * would to be added
   * @return The index of the chosen coarse cell
   * @warning \p max_card might not be honored
   */
  std::optional<CoMMAIndexType> select_best_cc_to_agglomerate(
    const CoMMAIndexType fc,
    const std::set<CoMMAIndexType> &neighs,
    const CoMMAIntType max_card,
    std::optional<CoMMAIntType> &new_compactness
  ) const {
    CoMMAUnused(max_card);
    std::unordered_map<CoMMAIndexType, CoMMAIntType> card{};
    std::unordered_map<CoMMAIndexType, CoMMAIntType> shared_faces{};
    std::unordered_map<CoMMAIndexType, CoMMAIntType> compact{};
    const auto n_neighs = neighs.size();
    card.reserve(n_neighs);
    shared_faces.reserve(n_neighs);
    CoMMAIntType min_card = std::numeric_limits<CoMMAIntType>::max();
    CoMMAIntType max_shared_f{0};
    // Since in the end we sort, wouldn't it be better to just use set instead
    // of deque?
    std::deque<CoMMAIndexType> argmin_card{};
    std::deque<CoMMAIndexType> argmax_shared_f{};
    std::deque<CoMMAIndexType> argtrue_compact{};
    std::deque<CoMMAIndexType> iso_neighs{};
    // Loop on neighbours to compute their features
    for (const auto &cc_idx : neighs) {
      const auto n_cc = _ccs.at(cc_idx);
      if (n_cc->_is_isotropic) {
        iso_neighs.push_back(cc_idx);
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        if (true /* n_cc->_cardinality < max_card */) {
          // On second thought, let us consider also cells with max cardinality
          // since the number of faces could be important to ensure compactness
          // of the coarse cell
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
          // Analysing compactness
          auto tmp_cc = n_cc->_s_fc;  // OK copy
          tmp_cc.insert(fc);
          const auto new_cpt =
            _fc_graph->compute_min_fc_compactness_inside_a_cc(tmp_cc);
          compact[cc_idx] = new_cpt;
          if (new_cpt > n_cc->_compactness) {
            argtrue_compact.push_back(cc_idx);
          }
        }  // End compactness and cardinality
      }  // End if isotropic
    }
    // Now, it's time to choose the best neighbours. Priority is given to those
    // which: 1 - Increase the degree of compactness
    if (!argtrue_compact.empty()) {
      // Sort so that, in the end, if nothing worked, we rely on ID numbering
      sort(argtrue_compact.begin(), argtrue_compact.end());
      CoMMAIndexType ret_cc{argtrue_compact[0]};
      CoMMAIntType cur_max{shared_faces[ret_cc]};
      // If more than one, maximize shared faces and/or minimize cardinality
      for (const auto &idx : argtrue_compact) {
        const auto cur_shf = shared_faces[idx];
        if (cur_shf > cur_max) {
          cur_max = cur_shf;
          ret_cc = idx;
        } else if (cur_shf == cur_max && card[idx] < card[ret_cc]) {
          ret_cc = idx;
        }
      }
      new_compactness = compact.at(ret_cc);
      return ret_cc;
    }
    // 2 - Maximize the number of shared faces
    if (!argmax_shared_f.empty()) {
      // Sort so that, in the end, if nothing worked, we rely on ID numbering
      sort(argmax_shared_f.begin(), argmax_shared_f.end());
      CoMMAIndexType ret_cc{argmax_shared_f[0]};
      CoMMAIntType cur_min{card[ret_cc]};
      // ..but let's see if among all the cells there is one with smaller
      // cardinality
      for (const auto &idx : argmax_shared_f) {
        if (card[idx] < cur_min) {
          ret_cc = idx;
          cur_min = card[ret_cc];
        }
      }
      new_compactness = compact.at(ret_cc);
      return ret_cc;
    }
    // 3 - Minimize the cardinality
    if (!argmin_card.empty()) {
      // We should never need to come here...
      // @TODO: I'm not sure what I could consider here to decide which cell to
      // return. The aspect-ratio maybe? In the mean time, I return the one with
      // the lowest ID
      const auto ret_cc =
        *(min_element(argmin_card.begin(), argmin_card.end()));
      new_compactness = compact.at(ret_cc);
      return ret_cc;
    }
    // If everything failed, look through the neighbours
    if (!iso_neighs.empty()) return iso_neighs[0];
    // otherwise, there is nothing we can do
    return std::nullopt;
  }

  /** @brief Compute the number of faces shared between a fine cell and a coarse
   * one
   * @param[in] fc Index of the fine cell
   * @param[in] cc Pointer to the coarse cell
   * @return The number of shared faces
   */
  inline CoMMAIntType get_shared_faces(
    const CoMMAIndexType fc, const CoarseCellPtr cc
  ) const {
    CoMMAIntType shared_faces{0};
    for (const auto &i_fc : cc->_s_fc) {
      shared_faces += count(
        _fc_graph->neighbours_cbegin(i_fc), _fc_graph->neighbours_cend(i_fc), fc
      );
    }
    return shared_faces;
  }

  /** @brief It creates a coarse cell based on the set of fine cells given as
   * input
   * @param[in] s_fc Set of fine cells passed as a reference
   * @param[in] compactness Compactness degree of the CC
   * @param[in] is_anisotropic Boolean that tells if we are in an anisotropic
   * case or not
   * @param[in] is_creation_delayed Based on the agglomerator instruction we
   * define if we delay or not the agglomeration
   * @return Global identifier of the coarse cell
   */
  CoMMAIndexType create_cc(
    const std::unordered_set<CoMMAIndexType> &s_fc,
    const CoMMAIntType compactness,
    bool is_anisotropic = false,
    bool is_creation_delayed = false
  ) {
    // Create a course cell from the fine cells and update the fc_2_cc tree.
    assert((!is_anisotropic) || (!is_creation_delayed));
    for (const auto &i_fc : s_fc) {
      assert(!_fc_2_cc[i_fc].has_value());
      if (!_is_fc_agglomerated[i_fc]) {
        // Rq: initialise to False pour chaque niveau dans agglomerate(...)
        _is_fc_agglomerated[i_fc] = true;
        _nb_of_agglomerated_fc++;
      }
    }
    // Anisotropic case
    bool is_mutable = true;
    if (is_anisotropic) {
      // we collect the various cc, where the index in the vector is the i_cc
      _ccs[_cc_counter] = std::make_shared<CoarseCellType>(
        _fc_graph, _cc_counter, s_fc, compactness, !is_anisotropic
      );
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
        // we collect the various cc, where the index in the vector is the i_cc
        _ccs[_cc_counter] = std::make_shared<CoarseCellType>(
          _fc_graph, _cc_counter, s_fc, compactness
        );
        if (!is_anisotropic
            && static_cast<decltype(_sing_card_thresh)>(s_fc.size())
              <= _sing_card_thresh)
          _singular_cc.emplace_back(_cc_counter);
      }
      // Update of _associatedCoarseCellNumber the output of the current
      // function agglomerate _fc_2_cc is filled with _cc_counter
      for (const auto &i_fc : s_fc) {
        // Only if not isCreationDelayed:
        assert(!_fc_2_cc[i_fc].has_value());
        _fc_2_cc[i_fc] = _cc_counter;
      }
      // Update of the number of CC
      // ####################################
      _cc_counter++;  // Only if not isCreationDelayed
    } else {
      // We do not create the coarse cell yet.
      // As this coarse cell will be soon deleted, we want its coarse index to
      // be the greater possible.
      // Only isFineCellAgglomerated_tmp, number_of_fine_agglomerated_cells_tmp
      // and dict_DistributionOfCardinalOfCoarseElements are modified!
      _delayed_cc.emplace_back(s_fc, compactness);
    }
    return (_cc_counter - 1);
  }

  /** @brief Creates all the delayed coarse cell. It works only when the delayed
   * cell flag is activated in the agglomerator
   */
  inline void cc_create_all_delayed_cc() {
    for (const auto &[s_fc, cpt] : _delayed_cc) {
      create_cc(s_fc, cpt);
    }
    _delayed_cc.clear();
  }

protected:
  /** @brief Number of agglomerated fine cells */
  CoMMAIndexType _nb_of_agglomerated_fc = 0;

  /** @brief Vector of the set of fine cells composing the too small coarse
   * cells that will be built at the end of the agglomeration process and their
   * compactness degree
   */
  std::vector<std::pair<std::unordered_set<CoMMAIndexType>, CoMMAIntType>>
    _delayed_cc;

  /** @brief Set of singular coarse cells, that is, composed of only one fine
   * cell */
  std::deque<CoMMAIndexType> _singular_cc;
};

}  // end namespace comma

#endif  // COMMA_PROJECT_COARSE_CELL_GRAPH_H
