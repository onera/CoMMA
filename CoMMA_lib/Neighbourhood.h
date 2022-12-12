#ifndef COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
#define COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H

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
#include <cassert>
#include <deque>
#include <queue>
#include <set>
#include <unordered_set>
#include <vector>

#include "Util.h"

using namespace std;

/** @brief Class representing the neighbourhood of a given cell in the graph.
 * Mind that no information about the element being already agglomerated or not
 * is known here.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Neighbourhood {
#if 0
Some remarks about the implementation. All the work is done in the function "update",
hence that is where we have to focus on. There are only two constraints for the
returned object: it should be an iterable, and the content (the indices of the neighbours)
must be ordered using their weights. An ordered set of pair of (index, weight) with
CustomPairGreaterFunctor as comparator would not work as because we want unique indices
and that would have been quite hard to achieve even if defining a custom operator==,
have a look here, https://stackoverflow.com/a/1114862
We still use sets, but we have an extra check before inserting whether there already is
a pair with the same index.
#endif

  public:
  /** @brief Type of pair */
  using CoMMAPairType = pair<CoMMAIndexType, CoMMAWeightType>;
  /** @brief Type of set of pairs */
  using CoMMASetOfPairType = set<CoMMAPairType, CustomPairGreaterFunctor<CoMMAPairType>>;
  /** @brief Type for container of candidates */
  using CandidatesContainerType = deque<CoMMAIndexType>;

  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] weights Weights used to set up the order of the neighbours to visit
   **/
  Neighbourhood(const unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
                            const vector<CoMMAWeightType> &weights) :
      _s_neighbours_of_seed(move(s_neighbours_of_seed)), _weights(weights), _s_fc(),
      _candidates() { }

  /** @brief Method that updates the neighbourhood. Given the new_fc, if is in the
   * neighbours, it is deleted. Then, the new neighbours are added as candidates
   * @param[in] new_fc new fine cell to be added to the set of fine cells
   * @param[in] new_neighbours vector of the new neighbours to be analysed
   */
  virtual void update(const CoMMAIndexType new_fc, const vector<CoMMAIndexType> &new_neighbours) = 0;

  /** @brief Get candidates that should be consider in the next step of the agglomeration
   *  @return The candidates
   */
  inline const CandidatesContainerType& get_candidates() const { return this->_candidates; }

  protected:
  /** @brief Set of the neighbours of seed given as an input in the constructor.
   * Here, we can find all the neighbours of order up to a user-defined value
   * (by default is 3, this means neighbours of neighbours of neighbours) of the
   * initial seed. Hence, it holds the cells allowed to be agglomerated
   */
  const unordered_set<CoMMAIndexType> _s_neighbours_of_seed;

  /** @brief Priority weights */
  const vector<CoMMAWeightType> &_weights;

  /** @brief Set of the fine cells composing the coarse cell */
  unordered_set<CoMMAIndexType> _s_fc;

  /** @brief Candidates that should be considered */
  CandidatesContainerType _candidates;

  /** @brief Get candidates that should be consider in the next step of the agglomeration
   *  @return The candidates
   */
  inline void extract_and_update_candidates(const CoMMASetOfPairType &candidates_w_weights) {
    for (const auto & [idx, w] : candidates_w_weights)
      this->_candidates.push_back(idx);
  }

};

/** @brief Class representing the neighbourhood of a given cell in the graph.
 * In this derived class the neighbourhood is extended, meaning that
 * all the neighbours seen so far are candidates.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Neighbourhood_Extended : public Neighbourhood<CoMMAIndexType,
                                                        CoMMAWeightType, CoMMAIntType> {
  public:
  /** @brief Type of pair */
  using CoMMAPairType = pair<CoMMAIndexType, CoMMAWeightType>;
  /** @brief Type of set of pairs */
  using CoMMASetOfPairType = set<CoMMAPairType, CustomPairGreaterFunctor<CoMMAPairType>>;
  /** @brief Functor used if find-like function relying only on first element of the pair */
  using CoMMAPairFindFirstBasedType = PairFindFirstBasedFunctor<CoMMAPairType>;

  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] weights Weights used to set up the order of the neighbours to visit
   *  @param[in] dimension Dimension of the problem
   **/
  Neighbourhood_Extended(const unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
                                     const vector<CoMMAWeightType> &weights) :
      Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
        s_neighbours_of_seed, weights) {}

  /** @brief Method that updates the neighbourhood. Given the new_fc, if is in the
   * neighbours, it is deleted. Then, the new neighbours are added as candidates
   * @param[in] new_fc new fine cell to be added to the set of fine cells
   * @param[in] new_neighbours vector of the new neighbours to be analysed
   */
  void update(const CoMMAIndexType new_fc, const vector<CoMMAIndexType> &new_neighbours) override {
    // Add new_fc to current CC and remove it from previous neighbourhoods
    this->_s_fc.insert(new_fc);
    auto new_fc_ptr = find(this->_candidates.begin(), this->_candidates.end(), new_fc);
    if (new_fc_ptr != this->_candidates.end())
      this->_candidates.erase(new_fc_ptr);

    // Compute the set of direct neighbours allowed by original neighbourhood-order
    CoMMASetOfPairType neighs;
    for (const CoMMAIndexType& i_fc : new_neighbours) {
      if ( (find(this->_candidates.begin(), this->_candidates.end(), i_fc)
            == this->_candidates.end()) &&
           (this->_s_fc.count(i_fc) == 0) &&
           (this->_s_neighbours_of_seed.count(i_fc) > 0) ) {
        // If not yet in the FON, not yet in the coarse cell and among the
        // allowed neighbours, insert
        neighs.emplace(i_fc, this->_weights[i_fc]);
      }
    }
    this->extract_and_update_candidates(neighs);
  }

};

/** @brief Class representing the neighbourhood of a given cell in the graph.
 * In this derived class the neighbourhood is 'pure front-advancing',
 * meaning that the next candidates are only the direct neighbours of the last
 * added cell
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Neighbourhood_Pure_Front : public Neighbourhood<CoMMAIndexType,
                                                        CoMMAWeightType, CoMMAIntType> {
  public:
  /** @brief Type of pair */
  using CoMMAPairType = pair<CoMMAIndexType, CoMMAWeightType>;
  /** @brief Type of set of pairs */
  using CoMMASetOfPairType = set<CoMMAPairType, CustomPairGreaterFunctor<CoMMAPairType>>;
  /** @brief Functor used if find-like function relying only on first element of the pair */
  using CoMMAPairFindFirstBasedType = PairFindFirstBasedFunctor<CoMMAPairType>;

  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] weights Weights used to set up the order of the neighbours to visit
   *  @param[in] dimension Dimension of the problem
   **/
  Neighbourhood_Pure_Front(const unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
                                       const vector<CoMMAWeightType> &weights,
                                       CoMMAIntType dimension) :
      Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
        s_neighbours_of_seed, weights), _q_neighs_w_weights(), _dimension(dimension) {}

  /** @brief Method that updates the neighbourhood. Given the new_fc, if is in the
   * neighbours, it is deleted. Then, the new neighbours are added as candidates
   * @param[in] new_fc new fine cell to be added to the set of fine cells
   * @param[in] new_neighbours vector of the new neighbours to be analysed
   */
  void update(const CoMMAIndexType new_fc, const vector<CoMMAIndexType> &new_neighbours) override {
    this->_candidates.clear();
    // Add new_fc to current CC and remove it from previous neighbourhoods
    this->_s_fc.insert(new_fc);
    for (auto &fon : this->_q_neighs_w_weights) {
      // There is erase_if for sets in C++20
      //erase_if(fon, [&new_fc](const CoMMAPairType &p){return p.first == new_fc;});
      auto it = find_if(fon.begin(), fon.end(),
                        CoMMAPairFindFirstBasedType(new_fc));
                        //[&new_fc](const CoMMAPairType &p){return p.first == new_fc;});
      if (it != fon.end())
        fon.erase(it);
    }

    // Compute the set of direct neighbours allowed by original neighbourhood-order
    CoMMASetOfPairType curr_set = CoMMASetOfPairType();
    for (const CoMMAIndexType& i_fc : new_neighbours) {
      if ( (this->_s_fc.count(i_fc) == 0) &&
           (this->_s_neighbours_of_seed.count(i_fc) > 0) ) {
        // If not yet in coarse cell and among the allowed neighbours, insert
        curr_set.emplace(i_fc, this->_weights[i_fc]);
      }
    }

    this->_q_neighs_w_weights.push_front(curr_set);

    // Now, see which FON to return. Here is the strategy:
    // If most recent FON is not empty, return it. If not, check the oldest FON: if
    // not empty return it, otherwise check the previous FON. If empty, check the
    // second oldest, and so on...
    // We grant ourselves one exception...
    if ( this->_q_neighs_w_weights.size() <=
         static_cast<decltype(this->_q_neighs_w_weights.size())>(this->_dimension) ) {
      // If at the (very) beginning of the agglomeration, still consider every
      // possible neighbour. This will allow to obtain nice quads from quads
      // TODO[RM]: I think this workaround is needed because we are not able to
      // compute exactly the AR; if we ever we will be able we should try to remove
      // it
      for (auto prev_fon = this->_q_neighs_w_weights.begin() + 1;
           prev_fon != this->_q_neighs_w_weights.end(); ++prev_fon)
        curr_set.insert(prev_fon->begin(), prev_fon->end());
      this->extract_and_update_candidates(curr_set);
    }
    else {
      auto cur_front = decltype(this->_q_neighs_w_weights.size()){0};
      auto cur_back  = decltype(this->_q_neighs_w_weights.size()){this->_q_neighs_w_weights.size() - 1};
      while (cur_front <= cur_back) {
        typename decltype(this->_q_neighs_w_weights)::iterator it =
          this->_q_neighs_w_weights.begin() + (cur_front++);
        if ( !it->empty() ) {
          this->extract_and_update_candidates(*it);
          break;
        }
        it =  this->_q_neighs_w_weights.begin() + (cur_back--);
        if ( !it->empty() ) {
          this->extract_and_update_candidates(*it);
          break;
        }
      }
    }
  }

  /** @brief Get the neighbours from a previous stage
   *  @param[in] lvl Stage identifier. 0 = current neighbours, 1 = previous neighbours,
   * 2 = second-to-last neighbours, etc...
   *  @return A constant reference to a set of pairs of neighbours and weights
   */
  inline const CoMMASetOfPairType& get_neighbours_by_level(const CoMMAIntType lvl) const {
    assert(lvl >= 0 && lvl < static_cast<CoMMAIntType>(this->_q_neighs_w_weights.size()));
    return this->_q_neighs_w_weights[lvl];
  }

  protected:
  /** @brief History of the first-order-neighbourhoods of the fine cells recently
   * agglomerated */
  deque<CoMMASetOfPairType> _q_neighs_w_weights;

  /** @brief dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3
   * -> 3D)*/
  CoMMAIntType _dimension;

};

#endif  // COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
