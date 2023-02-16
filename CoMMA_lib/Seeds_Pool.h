#ifndef COMMA_PROJECT_SEEDS_POOL_H
#define COMMA_PROJECT_SEEDS_POOL_H

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
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <utility>
#include <vector>

#include "CoMMATypes.h"
#include "Util.h"

using namespace std;

/** @brief Functor performing the initialization of a seeds pool
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
struct SPInitializator {
  /** @brief Type of pair */
  using CoMMAPairType = pair<CoMMAIndexType, CoMMAWeightType>;
  /** @brief Type of queue which holds seeds */
  using CoMMAQueueType = deque<CoMMAIndexType>;
  /** @brief Type of set of pairs */
  using CoMMASetOfPairType = set<CoMMAPairType, CustomPairGreaterFunctor<CoMMAPairType>>;
  /** @brief Initialize a queue of a seeds pool
   *  @param[in] priority_weights Weights used to set the order of the seed to choose
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
   *  @param[in] max_bnd Reference value for boundary faces: only cells with
   *  boundaries higher than this are considered
   *  @param[out] q The queue to build
   */
  virtual void operator() (const vector<CoMMAWeightType> &priority_weights,
                           const vector<CoMMAIntType> &n_bnd_faces,
                           const CoMMAIntType max_bnd,
                           CoMMAQueueType &q) = 0;
};

/** @brief Functor performing the full initialization of a seeds pool
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
struct SPFullInitializator :
  public SPInitializator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
  /** @brief Parent class */
  using BaseClass = SPInitializator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Type of queue which holds seeds */
  using typename BaseClass::CoMMAQueueType;
  /** @brief Type of set of pairs */
  using typename BaseClass::CoMMASetOfPairType;
  /** @brief Initialize a queue of a seeds pool
   *  @param[in] priority_weights Weights used to set the order of the seed to choose
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
   *  @param[in] max_bnd Reference value for boundary faces: only cells with
   *  boundaries higher than this are considered
   *  @param[out] q The queue to build
   */
  void operator() (const vector<CoMMAWeightType> &priority_weights,
                   const vector<CoMMAIntType> &n_bnd_faces,
                   const CoMMAIntType max_bnd,
                   CoMMAQueueType &q) override {
    // Similar to function build_queue but without is_agglomerated
    // Using set to force order
    CoMMASetOfPairType tmp_set{};
    for (auto i_fc = decltype(n_bnd_faces.size()){0};
         i_fc < n_bnd_faces.size(); ++i_fc) {
      if (n_bnd_faces[i_fc] >= max_bnd)
        tmp_set.emplace(i_fc, priority_weights[i_fc]);
    }
    for (const auto &idx : tmp_set)
      q.push_back(idx.first);
  }
};

/** @brief Functor performing the one-point initialization of a seeds pool
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
struct SPOnePointInitializator :
  public SPInitializator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
  /** @brief Parent class */
  using BaseClass = SPInitializator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Type of pair */
  using typename BaseClass::CoMMAPairType;
  /** @brief Type of queue which holds seeds */
  using typename BaseClass::CoMMAQueueType;
  /** @brief Initialize a queue of a seeds pool
   *  @param[in] priority_weights Weights used to set the order of the seed to choose
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
   *  @param[in] max_bnd Reference value for boundary faces: only cells with
   *  boundaries higher than this are considered
   *  @param[out] q The queue to build
   */
  void operator() (const vector<CoMMAWeightType> &priority_weights,
                   const vector<CoMMAIntType> &n_bnd_faces,
                   const CoMMAIntType max_bnd,
                   CoMMAQueueType &q) override {
    CoMMAPairType cur_max{numeric_limits<CoMMAIndexType>::max(),
                          numeric_limits<CoMMAWeightType>::min()};
    const CustomPairGreaterFunctor<CoMMAPairType> comp_op{};
    for (auto i_fc = decltype(n_bnd_faces.size()){0};
         i_fc < n_bnd_faces.size(); ++i_fc) {
      const CoMMAPairType p{i_fc, priority_weights[i_fc]};
      if (n_bnd_faces[i_fc] >= max_bnd && comp_op(p, cur_max))
        cur_max = move(p);
    }
    q.push_back(cur_max.first);
  }
};

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

 protected:
  /** @brief List of deque of seeds. For each identifier we have the available
   * seeds. We want the seed to be chosen preferably in the corner, then ridges,
   * valleys and then interiors, see \ref CoMMACellT
   */
  vector<CoMMAQueueType> _l_of_seeds;

  /** @brief Weights used to set the order of the seed to choose */
  const vector<CoMMAWeightType> &_priority_weights;

  /** @brief Optional possibly containing the level (0,1,2,3, see
   * \ref Seeds_Pool._l_of_seeds) of the queue currently being spoiled
   */
  optional<CoMMAIntType> _cur_top_queue;

  /** @brief Pointer to a SPInitializator */
  shared_ptr<SPInitializator<CoMMAIndexType, CoMMAWeightType,
                             CoMMAIntType>> _initializator;

 public:
  /** @brief Vector of number of faces on boundary per cell */
  const vector<CoMMAIntType> &_n_bnd_faces;

  /** @brief Constructor
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
   *  @param[in] priority_weights Weights used to set the order of the seed to choose
   *  @param[in] one_point_init Whether the initialization should be done for the
   *  highest boundary level or just for one point
   */
  Seeds_Pool(const vector<CoMMAIntType> &n_bnd_faces,
             const vector<CoMMAWeightType> &priority_weights,
             const bool one_point_init) :
          _priority_weights(priority_weights), _cur_top_queue(nullopt),
          _n_bnd_faces(n_bnd_faces) {
    // The size 4 corresponds to 0 : interior, 1 : valley, 2 : ridge, 3 : corner
    _l_of_seeds = vector<CoMMAQueueType>(CoMMACellT::N_CELL_TYPES);

    _initializator = nullptr;
    if (one_point_init)
      _initializator = make_shared<
              SPOnePointInitializator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>();
    else
      _initializator = make_shared<
              SPFullInitializator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>();
  }

  /** @brief Destructor */
  virtual ~Seeds_Pool() = default;

  /** @brief Spoil a queue looking for an not-yet-agglomerated seed
   *  @param[in] is_fc_agglomerated Vector of boolean telling whether a face has
   * been agglomerated
   *  @param[out] queue The queue to spoil
   *  @return An optional which contains the new seed if found
   */
  inline optional<CoMMAIndexType> spoil_queue(const vector<bool> &is_fc_agglomerated,
                                              CoMMAQueueType &queue) {

    for (; !queue.empty() && is_fc_agglomerated[queue.front()];
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
   *  @param[in] is_agglomerated Vector of booleans telling whether fine cells are
   *  agglomerated
   *  @return New seed
   */
  virtual optional<CoMMAIndexType> choose_new_seed(const vector<bool> &is_agglomerated) = 0;

  /** @brief Add the provided seeds to a seeds pool queue according to the number of
   * boundary faces
   * @param[in] new_seeds Seeds to add
   * @warning new_seeds is supposed to be already ordered by the priority weight,
   * therefore no check will be performed
   **/
  virtual void update(const deque<CoMMAIndexType> &new_seeds) = 0;

  /** @brief Add the provided seeds to a seeds pool queue according to the number of
   * boundary faces. The seeds will be ordered considering their priority weights
   * before being added to the queue
   * @param[in] new_seeds Vector of seeds to add
   **/
  virtual void order_new_seeds_and_update(const unordered_set<CoMMAIndexType> &new_seeds) = 0;

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
   *  @param[in] expected_max Expected value. If found, function returns immediately.
   *  Defaulted to CoMMACellT::CORNER
   *  @return An optional which contains the number of boundary faces if there
   * is at least one cell left to agglomerate
   */
  inline optional<CoMMAIntType> get_highest_n_bnd_yet_to_agglomerate(
      const vector<bool> &is_fc_agglomerated,
      const CoMMAIntType expected_max = CoMMACellT::CORNER) const {
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

  /** @brief Given the default levels we define if the list of the targeted
   * level is empty
   * @param[in] i_level level of the defined list
   **/
  bool is_empty(CoMMAIntType i_level = CoMMACellT::N_CELL_TYPES) const {
    assert(CoMMACellT::INTERIOR <= i_level);
    assert(i_level <= CoMMACellT::N_CELL_TYPES);
    if (_l_of_seeds.size() > 0) {
      switch (i_level) {
        case CoMMACellT::INTERIOR:
          return _l_of_seeds[CoMMACellT::INTERIOR].empty();
        case CoMMACellT::VALLEY:
          return _l_of_seeds[CoMMACellT::VALLEY].empty();
        case CoMMACellT::RIDGE:
          return _l_of_seeds[CoMMACellT::RIDGE].empty();
        case CoMMACellT::CORNER:
          return _l_of_seeds[CoMMACellT::CORNER].empty();
        case CoMMACellT::N_CELL_TYPES:
        default:
          return _l_of_seeds[CoMMACellT::INTERIOR].empty() &&
                 _l_of_seeds[CoMMACellT::VALLEY].empty()   &&
                 _l_of_seeds[CoMMACellT::RIDGE].empty()    &&
                 _l_of_seeds[CoMMACellT::CORNER].empty();
      } /* Switch */
    }
    return true;
  }

  /** @brief Whether the seeds pool need to be initialized
   *  @param[in] is_agglomerated Vector of booleans telling whether fine cells are
   *  agglomerated
   *  @return A bool
   */
  virtual bool need_initialization(const vector<bool> &is_agglomerated) = 0;

  /** @brief Initialize the seeds pool via a call to its initializator */
  inline void initialize() {
    if (!_cur_top_queue.has_value())
      // Computing the max number of boundary faces. Only the related seed queue will
      // be filled at this initial stage
      _cur_top_queue = *(max_element(_n_bnd_faces.begin(), _n_bnd_faces.end()));
    (*this->_initializator)(this->_priority_weights, this->_n_bnd_faces,
                            this->_cur_top_queue.value(),
                            this->_l_of_seeds[this->_cur_top_queue.value()]);
  }

  /** @brief Setter for the top queue member
   *  @param[in] q_lvl Level of the new top queue
   */
  inline void set_top_queue(const CoMMAIntType q_lvl) { this->_cur_top_queue = q_lvl; }

};

/** @brief Class representing the pool of all the seeds for creating a coarse
 * cell. This derived class gives higher priority to cells that are on the border.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Seeds_Pool_Boundary_Priority
  : public Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
  public:

  /** @brief Base class */
  using SeedsPoolBaseType = Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Type of pair */
  using typename SeedsPoolBaseType::CoMMAPairType;
  /** @brief Type of set of pairs */
  using typename SeedsPoolBaseType::CoMMASetOfPairType;

  /** @brief Constructor
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
   *  @param[in] priority_weights Weights used to set the order of the seed to choose
   *  @param[in] one_point_init Whether the initialization should be done for the
   *  highest boundary level or just for one point
   **/
  Seeds_Pool_Boundary_Priority(const vector<CoMMAIntType> &n_bnd_faces,
                               const vector<CoMMAWeightType> &priority_weights,
                               const bool one_point_init) :
      Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
          n_bnd_faces, priority_weights, one_point_init) {}

  /** @brief Choose a new seed in the pool
   *  @param[in] is_agglomerated Vector of booleans telling whether fine cells are
   *  agglomerated
   *  @return New seed
   */
  optional<CoMMAIndexType> choose_new_seed(const vector<bool> &is_agglomerated) override {
    // Choose a correct seed from the fc pool list_of_seeds beyond not
    // agglomerated fc.
    // We choose preferably the corners, then the ridges, then the valley, and
    // finally interior cells:
    // see NIA (Mavriplis uses Wall and farfield only)
    // Exactly the inverse of the order of the list. For this reason we proceed
    // from the back
    if (this->_cur_top_queue.has_value()) {
      auto opt_seed = this->spoil_queue(is_agglomerated,
                                        this->_l_of_seeds[this->_cur_top_queue.value()]);
      if (opt_seed.has_value())
        return opt_seed.value();
      auto opt_top = this->get_highest_n_bnd_yet_to_agglomerate(
                        is_agglomerated, this->_cur_top_queue.value());
      if (opt_top.has_value()) {
        const auto cur_queue = opt_top.value();
        this->_cur_top_queue = cur_queue;
        // Could be the same top queue, but that's OK
        opt_seed = this->spoil_queue(is_agglomerated, this->_l_of_seeds[cur_queue]);
        if (opt_seed.has_value())
          return opt_seed.value();
        // If here, we already used everything that we had in the seeds pool but there are still
        // cells to agglomerate of the same type, hence we need to rebuild the queue from scratch
        this->build_queue(is_agglomerated, cur_queue);

        const auto seed = this->_l_of_seeds[cur_queue].front();
        this->_l_of_seeds[cur_queue].pop_front();
        return seed;
      }
      // If everything failed, set to null
      this->_cur_top_queue = nullopt;
    }
    return nullopt;
  }

  /** @brief Add the provided seeds to a seeds pool queue according to the number of
   * boundary faces. It updates the current top queue if necessary
   * @param[in] new_seeds Seeds to add
   * @warning new_seeds is supposed to be already ordered by the priority weight,
   * therefore no check will be performed
   **/
  inline void update(const deque<CoMMAIndexType> &new_seeds) override {
    if (!new_seeds.empty()) {
      CoMMAIntType max_bnd{0};
      for (const auto &s : new_seeds) {
        const auto n_bnd = this->_n_bnd_faces[s];
        if (n_bnd > max_bnd)
          max_bnd = n_bnd;
        auto &q = this->_l_of_seeds[n_bnd];
        if (find(q.begin(), q.end(), s) == q.end())
          q.push_back(s);
      }
      if (!this->_cur_top_queue.has_value() || max_bnd > this->_cur_top_queue.value())
        this->_cur_top_queue = max_bnd;
    }
  }

  /** @brief Add the provided seeds to a seeds pool queue according to the number of
   * boundary faces. The seeds will be ordered considering their priority weights
   * before being added to the queue
   * @param[in] new_seeds Vector of seeds to add
   **/
  inline void order_new_seeds_and_update(
      const unordered_set<CoMMAIndexType> &new_seeds) override {
    map<CoMMAIntType, CoMMASetOfPairType, greater<>> new_seeds_by_bnd;
    if (!new_seeds.empty()) {
      for (const auto s : new_seeds) {
        new_seeds_by_bnd[this->_n_bnd_faces[s]].emplace(s, this->_priority_weights[s]);
      }
      const auto max_bnd = new_seeds_by_bnd.begin()->first;
      if (!this->_cur_top_queue.has_value() || max_bnd > this->_cur_top_queue.value())
        this->_cur_top_queue = max_bnd;
      for (const auto & [n_bnd, seeds] : new_seeds_by_bnd) {
        auto &q = this->_l_of_seeds[n_bnd];
        for (const auto & [s, w] : seeds) {
          if (find(q.begin(), q.end(), s) == q.end())
            q.push_back(s);
        }
      }
    }
  }

  /** @brief Whether the seeds pool need to be initialized. It updates the top queue
   * if necessary
   *  @param[in] is_agglomerated Vector of booleans telling whether fine cells are
   *  agglomerated
   *  @return A bool
   */
  bool need_initialization(const vector<bool> &is_agglomerated) override {
    if (this->is_empty() || (!this->_cur_top_queue.has_value()) )
      return true;
    const auto max_bnd = this->get_highest_n_bnd_yet_to_agglomerate(is_agglomerated);
    if (max_bnd.has_value() && max_bnd.value() > this->_cur_top_queue.value() ) {
      this->_cur_top_queue = max_bnd.value();
      return true;
    }
    return false;
  }

};

/** @brief Class representing the pool of all the seeds for creating a coarse
 * cell. This derived class gives higher priority to cells that are neighbours of
 * already existing coarse cells
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Seeds_Pool_Neighbourhood_Priority
  : public Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
  public:

  /** @brief Base class */
  using SeedsPoolBaseType = Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Type of pair */
  using typename SeedsPoolBaseType::CoMMAPairType;
  /** @brief Type of set of pairs */
  using typename SeedsPoolBaseType::CoMMASetOfPairType;

  /** @brief Constructor
   *  @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
   *  @param[in] priority_weights Weights used to set the order of the seed to choose
   *  @param[in] one_point_init Whether the initialization should be done for the
   *  highest boundary level or just for one point
   **/
  Seeds_Pool_Neighbourhood_Priority(const vector<CoMMAIntType> &n_bnd_faces,
                                    const vector<CoMMAWeightType> &priority_weights,
                                    const bool one_point_init) :
      Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
          n_bnd_faces, priority_weights, one_point_init) {}

  /** @brief Choose a new seed in the pool
   *  @param[in] is_agglomerated Vector of booleans telling whether fine cells are
   *  agglomerated
   *  @return New seed
   */
  optional<CoMMAIndexType> choose_new_seed(const vector<bool> &is_agglomerated) override {
    // Choose a correct seed from the fc pool list_of_seeds beyond not
    // agglomerated fc.
    // We choose preferably the corners, then the ridges, then the valley, and
    // finally interior cells:
    // see NIA (Mavriplis uses Wall and farfield only)
    // Exactly the inverse of the order of the list. For this reason we proceed
    // from the back
    if (this->_cur_top_queue.has_value()) {

      for (auto q = this->_l_of_seeds.rbegin() +
                    (this->_l_of_seeds.size() - (this->_cur_top_queue.value() + 1));
           q != this->_l_of_seeds.rend(); ++q) {
        const auto opt_seed = this->spoil_queue(is_agglomerated, *q);
        if (opt_seed.has_value()) {
          this->_cur_top_queue = distance(q, this->_l_of_seeds.rend()) - 1;
          return opt_seed.value();
        }
      }

      // If not found, see which is the highest unfinished level
      const auto opt_top = this->get_highest_n_bnd_yet_to_agglomerate(
          is_agglomerated, this->_cur_top_queue.value());
      if (opt_top.has_value()) {
        const auto cur_queue = opt_top.value();
        this->_cur_top_queue = cur_queue;
        // Could be the same top queue, but that's OK
        const auto opt_seed = this->spoil_queue(is_agglomerated, this->_l_of_seeds[cur_queue]);
        if (opt_seed.has_value())
          return opt_seed.value();
        // If here, we already used everything that we had in the seeds pool but there are still
        // cells to agglomerate of the same type, hence we need to rebuild the queue from scratch
        this->build_queue(is_agglomerated, cur_queue);

        const auto seed = this->_l_of_seeds[cur_queue].front();
        this->_l_of_seeds[cur_queue].pop_front();
        return seed;
      }
      // If everything failed, set to null
      this->_cur_top_queue = nullopt;
    }
    return nullopt;
  }

  /** @brief Add the provided seeds to a seeds pool queue according to the number of
   * boundary faces
   * @param[in] new_seeds Seeds to add
   * @warning new_seeds is supposed to be already ordered by the priority weight,
   * therefore no check will be performed
   **/
  inline void update(const deque<CoMMAIndexType> &new_seeds) override {
    for (const auto &s : new_seeds) {
      // In order to the neighbourhood priority, we choose to append to the current
      // top queue, rather than switching queue
      const auto q_lvl = this->_cur_top_queue.has_value() ? min(this->_n_bnd_faces[s],
                                                                this->_cur_top_queue.value())
                                                          : this->_n_bnd_faces[s];
      auto &q = this->_l_of_seeds[q_lvl];
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
      const unordered_set<CoMMAIndexType> &new_seeds) override {
    map<CoMMAIntType, CoMMASetOfPairType, greater<>> new_seeds_by_bnd;
    for (const auto s : new_seeds) {
      new_seeds_by_bnd[this->_n_bnd_faces[s]].emplace(s, this->_priority_weights[s]);
    }
    for (auto & [n_bnd, seeds] : new_seeds_by_bnd) {
      // In order to the neighbourhood priority, we choose to append to the current
      // top queue, rather than switching queue
      const auto q_lvl = this->_cur_top_queue.has_value() ? min(n_bnd,
                                                                this->_cur_top_queue.value())
                                                          : n_bnd;
      auto &q = this->_l_of_seeds[q_lvl];
      for (const auto & [s, w] : seeds) {
        if (find(q.begin(), q.end(), s) == q.end())
          q.push_back(s);
      }
    }
  }

  /** @brief Whether the seeds pool need to be initialized
   *  @param[in] is_agglomerated vector of boolean with fine cells
   *  @return A bool
   */
  bool need_initialization(const vector<bool> &is_agglomerated) override {
    CoMMAUnused(is_agglomerated);
    return this->is_empty() || (!this->_cur_top_queue.has_value());
  }

};
#endif  // COMMA_PROJECT_SEEDS_POOL_H
