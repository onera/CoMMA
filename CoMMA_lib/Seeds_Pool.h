#ifndef COMMA_PROJECT_SEEDS_POOL_H
#define COMMA_PROJECT_SEEDS_POOL_H

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

#include <algorithm>
#include <set>
#include <unordered_map>
#include <list>
#include <cassert>
#include <deque>
#include <queue>
#include <utility>
#include <vector>
#include <optional>

#include "CoMMATypes.h"
#include "Util.h"

using namespace std;

/** @brief Class representing the pool of all the seeds for creating a coarse
 * cell
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Seeds_Pool {
 public:
  /** @brief Type of pair */
  using CoMMAPairType = pair<CoMMAIndexType, CoMMAWeightType>;
#if 0
  /** @brief Type of container of pairs */
  // Better to use a deque (rather than the default vector) because it has nice
  // insert / remove performances, and it's more closer to the philosophy we would
  // like to obtain
  using CoMMAContainerPairType = deque<CoMMAPairType>;
  /** @brief Type of priority queue with pairs */
  // A drawback of priority_queue to be aware of: it cannot be looped through
  // A workaround is explained on https://stackoverflow.com/questions/4484767/how-to-iterate-over-a-priority-queue
  // TODO: Maybe change to the advised make_heap (with vector or deque)
  using CoMMAPriorityQueueType = priority_queue<CoMMAPairType,
                                                CoMMAContainerPairType,
                                                CustomPairLessFunctor<CoMMAPairType>>;
#endif
  /** @brief Type of queue which holds seeds */
  using CoMMAQueueType = deque<CoMMAIndexType>;
  /** @brief Type of set of pairs */
  using CoMMASetOfPairType = set<CoMMAPairType, CustomPairGreaterFunctor<CoMMAPairType>>;
  /** @brief Functor used if find-like function relying only on first element of the pair */
  using CoMMAPairFindFirstBasedType = PairFindFirstBasedFunctor<CoMMAPairType>;

 protected:
  /** @brief List of deque of seeds. For each identifier we have the available
   * seeds
   *  We want the seed to be chosen preferably in the corner, then ridges,
   * valleys and then interiors.
   *  The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
   *  to minimizes the too small cells!
   *  0 : interior (no face on the edge of the domain)
   *  1 : valley (one face on the edge of the domain)
   *  2 : ridge (two faces on the edge of the domain)
   *  3 : corner (three faces on the edge of the domain)
  */
  vector<CoMMAQueueType> _l_of_seeds;

  /** @brief Weights used to set the order of the seed to choose */
  const vector<CoMMAWeightType> &_priority_weights;

  /** @brief Optional possibly containing the level (0,1,2,3, see above) of the
   * queue currently being spoiled
   */
  optional<CoMMAIntType> _cur_top_queue;

 public:
  /** @brief Vector of number of faces on boundary per cell */
  const vector<CoMMAIntType> &_n_bnd_faces;

  /** @brief Constructor
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
   *  @param[in] priority_weights Weights used to set the order of the seed to choose
   **/
  Seeds_Pool(const vector<CoMMAIntType> &n_bnd_faces,
             const vector<CoMMAWeightType> &priority_weights) :
          _priority_weights(priority_weights), _n_bnd_faces(n_bnd_faces) {
    // The size 4 corresponds to 0 : interior, 1 : valley, 2 : ridge, 3 : corner
    _l_of_seeds = vector<CoMMAQueueType>(CoMMACellT::N_CELL_TYPES);

    // Computing the max number of boundary faces. Only the related seed queue will
    // be filled at this initial stage
    const auto max_bnd = *(max_element(_n_bnd_faces.begin(), _n_bnd_faces.end()));
    _cur_top_queue = max_bnd;
    // Similar to function build_queue but without is_agglomerated
    // Using set to force order
    CoMMASetOfPairType tmp_set{};
    for (auto i_fc = decltype(_n_bnd_faces.size()){0}; i_fc < _n_bnd_faces.size(); ++i_fc) {
      if (_n_bnd_faces[i_fc] >= max_bnd)
        tmp_set.emplace(i_fc, priority_weights[i_fc]);
    }
    auto &top_queue = _l_of_seeds[max_bnd];
    for (const auto &idx : tmp_set)
      top_queue.push_back(idx.first);
  };

  /** @brief Destructor */
  ~Seeds_Pool() {};

  /** @brief Spoil a queue looking for an not-yet-agglomerated seed
   *  @param[in] is_fc_agglomerated Vector of boolean telling whether a face has
   * been agglomerated
   *  @param[in] i_q Index of the queue to spoil
   *  @return An optional which contains the new seed if found
   */
  inline optional<CoMMAIndexType> spoil_queue(const vector<bool> &a_is_fc_agglomerated,
      const CoMMAIntType i_q) {
    auto &queue = _l_of_seeds[i_q]; // Convenient
    for (; !queue.empty() && a_is_fc_agglomerated[queue.front()];
         queue.pop_front()) {
        // If there still is something and it's already agglomerated, then keep on searching
        // [Intentionally leaving this empty]
    }
    if (!queue.empty()) {
      const auto seed = queue.front();
      queue.pop_front();
      return seed;
    }
    return nullopt;
  }

  /** @brief Choose a new seed in the pool
   *  @param[in] a_is_fc_agglomerated vector of boolean with fine cells
   * agglomerated (true) or not agglomerated (false)
   *  @return New seed
   */
  optional<CoMMAIndexType> choose_new_seed(const vector<bool> &a_is_fc_agglomerated) {
    // Choose a correct seed from the fc pool list_of_seeds beyond not
    // agglomerated fc.
    // We choose preferably the corners, then the ridges, then the valley, and
    // finally interior cells:
    // see NIA (Mavriplis uses Wall and farfield only)
    // Exactly the inverse of the order of the list. For this reason we proceed
    // from the back
    if (_cur_top_queue.has_value()) {
      auto opt_seed = spoil_queue(a_is_fc_agglomerated, _cur_top_queue.value());
      if (opt_seed.has_value())
        return opt_seed.value();
      auto opt_top = get_highest_n_bnd_yet_to_agglomerate(a_is_fc_agglomerated,
                                                          _cur_top_queue.value());
      if (opt_top.has_value()) {
        const auto cur_queue = opt_top.value();
        _cur_top_queue = cur_queue;
        // Could be the same top queue, but that's OK
        opt_seed = spoil_queue(a_is_fc_agglomerated, cur_queue);
        if (opt_seed.has_value())
          return opt_seed.value();
        // If here, we already used everything that we had in the seed pool but there are still
        // cells to agglomerate of the same type, hence we need to rebuild the queue from scratch
        build_queue(a_is_fc_agglomerated, cur_queue);

        const auto seed = _l_of_seeds[cur_queue].front();
        _l_of_seeds[cur_queue].pop_front();
        return seed;
      }
      // If everything failed, set to null
      _cur_top_queue = nullopt;
    }
    return nullopt;
  };

  /** @brief Build the weight-ordered queue of seed for a given target level.
   * A set of (index, weight) pair with special comparator is used to enforced order, then
   * the indices are extracted
   *  @param[in] is_fc_agglomerated Vector of boolean telling whether a face has
   * been agglomerated
   *  @param[in] target Level of the queue to build. It corresponds to the number
   * of boundary faces
   */
  inline void build_queue(const vector<bool> &is_fc_agglomerated,
                          const CoMMAIntType target) {
    // Using set to force order
    CoMMASetOfPairType tmp_set{};
    for (auto i_fc = decltype(_n_bnd_faces.size()){0}; i_fc < _n_bnd_faces.size(); ++i_fc) {
      if (!is_fc_agglomerated[i_fc] && _n_bnd_faces[i_fc] == target)
        tmp_set.emplace(i_fc, _priority_weights[i_fc]);
    }
    for (const auto &idx : tmp_set)
     _l_of_seeds[target].push_back(idx.first);
  }

  /** @brief Compute the highest number of boundary faces of cells which are not
   * agglomerated yet
   *  @param[in] is_fc_agglomerated Vector of boolean telling whether a face has
   * been agglomerated
   *  @param[in] expected_max Expected value. If found, function returns immediately
   *  @return An optional which contains the number of boundary faces if there
   * is at least one cell left to agglomerate
   */
  inline optional<CoMMAIntType> get_highest_n_bnd_yet_to_agglomerate(
      const vector<bool> &is_fc_agglomerated, const CoMMAIntType expected_max) const {
    optional<CoMMAIntType> max_bnd = nullopt;
    for (auto i_fc = decltype(_n_bnd_faces.size()){0}; i_fc < _n_bnd_faces.size(); ++i_fc) {
      if (!is_fc_agglomerated[i_fc]) {
        const auto i_fc_bnd = _n_bnd_faces[i_fc];
        if (!max_bnd.has_value())
          max_bnd = i_fc_bnd;
        if (i_fc_bnd >= expected_max) {
          return i_fc_bnd;
        }
        else if (i_fc_bnd > max_bnd) {
          max_bnd = i_fc_bnd;
        }
      }
    }
    return max_bnd;
  }

  /** @brief Add the provided seeds to a seeds pool queue according to the number of
   * boundary faces
   * @param[in] new_seeds Vector of seeds to add
   * @warning new_seeds is supposed to be already ordered by the priority weight,
   * therefore no check will be performed
   **/
  inline void update(const vector<CoMMAIndexT> &new_seeds) {
    for (const auto &s : new_seeds) {
      auto &q = _l_of_seeds[_n_bnd_faces[s]];
      if (find(q.begin(), q.end(), s) == q.end())
        q.push_back(s);
    }
  }

  /** @brief Add the provided seeds to a seeds pool queue according to the number of
   * boundary faces. The seeds will be ordered considering their priority weights
   * before being added to the queue
   * @param[in] new_seeds Vector of seeds to add
   **/
  inline void order_new_seeds_and_update(
      const unordered_set<CoMMAIndexType> &new_seeds) {
    unordered_map<CoMMAIntT, CoMMASetOfPairType> new_seeds_by_bnd(4);
    for (const auto s : new_seeds) {
      new_seeds_by_bnd[_n_bnd_faces[s]].emplace(s, _priority_weights[s]);
    }
    for (auto & [n_bnd, seeds] : new_seeds_by_bnd) {
      auto &q = _l_of_seeds[n_bnd];
      for (const auto & [s, w] : seeds) {
        if (find(q.begin(), q.end(), s) == q.end())
          q.push_back(s);
      }
    }
  }

  /** @brief Given the default levels we define if the list of the targeted
   * level is empty
   * @param[in] i_level level of the defined list
   **/
  bool is_empty(CoMMAIntType i_level = CoMMACellT::INTERIOR) const {
    assert(CoMMACellT::INTERIOR <= i_level);
    assert(i_level <= CoMMACellT::CORNER);
    if (_l_of_seeds.size() > 0) {
      for (CoMMAIntType i = CoMMACellT::CORNER; i > i_level -1; i--) {
        if (!_l_of_seeds[i].empty()) {
          return false;
        }
      }
    }
    return true;
  }

  /** @brief Whether the dictionary of boundary is empty or already filled.
   *  @return A bool
   */
  inline bool is_initialized() const { return !_n_bnd_faces.empty(); }

};
#endif  // COMMA_PROJECT_SEEDS_POOL_H
