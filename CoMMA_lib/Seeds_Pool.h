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

#include <set>
#include <unordered_map>
#include <list>
#include <cassert>
#include <deque>
#include <queue>
#include <utility>
#include <vector>

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
  vector<CoMMAPriorityQueueType> _l_of_seeds;

 public:
  /** @brief Vector of number of faces on boundary per cell*/
  vector<CoMMAIntType> _n_bnd_faces;

  /** @brief Constructor
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
   *  @param[in] priority_weights Weights used to set the order of the seed to choose
   **/
  Seeds_Pool(const vector<CoMMAIntType> &n_bnd_faces,
             const vector<CoMMAWeightType> &priority_weights) {
    // We copy it since we might modified it
    _n_bnd_faces = n_bnd_faces;
    // The size 4 corresponds to 0 : interior, 1 : valley, 2 : ridge, 3 : corner
    _l_of_seeds = vector<CoMMAPriorityQueueType>(CoMMACellT::N_CELL_TYPES);
    for (auto i_fc = decltype(n_bnd_faces.size()){0}; i_fc < n_bnd_faces.size(); ++i_fc) {
      const CoMMAIntType i_fc_bnd = n_bnd_faces[i_fc];
      assert(i_fc_bnd >= CoMMACellT::INTERIOR);
      if (i_fc_bnd >= CoMMACellT::CORNER) {
        _n_bnd_faces[i_fc] = CoMMACellT::CORNER;
        _l_of_seeds[CoMMACellT::CORNER].emplace(
                                i_fc, priority_weights[i_fc]);
      } else if (i_fc_bnd == CoMMACellT::RIDGE) {
        _l_of_seeds[CoMMACellT::RIDGE].emplace(
                                i_fc, priority_weights[i_fc]);
      } else if (i_fc_bnd == CoMMACellT::VALLEY) {
        _l_of_seeds[CoMMACellT::VALLEY].emplace(
                                i_fc, priority_weights[i_fc]);
      } else if (i_fc_bnd == CoMMACellT::INTERIOR) {
        _l_of_seeds[CoMMACellT::INTERIOR].emplace(
                                i_fc, priority_weights[i_fc]);
      }
    }
  };

  /** @brief Destructor */
  ~Seeds_Pool() {};

  /** @brief Choose a new seed in the pool
   *  @param[in] a_is_fc_agglomerated vector of boolean with fine cells
   * agglomerated (true) or not agglomerated (false)
   *  @return New seed
   */
  CoMMAIndexType choose_new_seed(const vector<bool> &a_is_fc_agglomerated) {
    // Choose a correct seed from the fc pool list_of_seeds beyond not
    // agglomerated fc.
    // We choose preferably the corners, then the ridges, then the valley, and
    // finally interior cells:
    // see NIA (Mavriplis uses Wall and farfield only)
    // Exactly the inverse of the order of the list. For this reason we proceed
    // from the back
    for (auto prio_q = _l_of_seeds.rbegin(); prio_q != _l_of_seeds.rend(); ++prio_q) {
      for (; !prio_q->empty() && a_is_fc_agglomerated[prio_q->top().first]; prio_q->pop()) {
        // If there still is something and it's already agglomerated, then keep on searching
        // [Intentionally leaving this empty]
      }
      if (!prio_q->empty()) {
        const auto seed = prio_q->top().first;
        prio_q->pop();
        return seed;
      }
    }
    return -1;
  };

  /** @brief Given the default levels we define if the list of the targeted
   * level is empty
   * @param[in] i_level level of the defined list */
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
  };

  /** @brief Whether the dictionary of boundary is empty or already filled.
   *  @return A bool
   */
  inline bool is_initialized() const { return !_n_bnd_faces.empty(); }

  /** @brief Return how many boundary faces a certain cell has
   *  @param[in] idx_f Index of the cell
   *  @return the number of boundary faces
   */
  inline CoMMAIntType get_n_boundary_faces(const CoMMAIndexT idx_c) const {
    return _n_bnd_faces[idx_c];
  }
  /** @brief Whether a cell is on the boundary
   *  @param[in] idx_f Index of the cell
   *  @return Whether a cell is on the boundary
   */
  inline bool is_on_boundary(const CoMMAIndexT idx_c) const {
    return _n_bnd_faces[idx_c] > 0;
  }
};
#endif  // COMMA_PROJECT_SEEDS_POOL_H
