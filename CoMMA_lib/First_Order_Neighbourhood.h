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

/** @brief Class representing the first order neighborhood of a given cell in
 * the graph.
 * Mind that no information about the element being already agglomerated or not
 * is known here.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class First_Order_Neighbourhood {
  public:
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

  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] weights Weights used to set up the order of the neighbors to visit
   **/
  First_Order_Neighbourhood(const unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
                            const vector<CoMMAWeightType> &weights) :
      _s_neighbours_of_seed(move(s_neighbours_of_seed)), _weights(weights), _s_fc() { }

  /** @brief Method that updates the first order neighborhood, by updating the
   * front. Given the new_fc, if is in the neighbours, it is deleted and are
   * added the s_new_neighbours in in the set of neighbours given in the
   * constructor.
   * @param[in] new_fc new fine cell to be added to the set of fine cells
   * @param[in] new_neighbours vector of the new neighbours to be analysed
   * @return A vector of candidates for the fine cell to agglomerate
   */
  virtual vector<CoMMAIndexType> update(
      const CoMMAIndexType new_fc, const vector<CoMMAIndexType> &new_neighbours) = 0;

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
};

/** @brief Class representing the first order neighborhood of a given cell in
 * the graph. In this derived class the neighborhood is extended, meaning that
 * all the neighbors seen so far are candidates.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class First_Order_Neighbourhood_Extended : public First_Order_Neighbourhood<CoMMAIndexType,
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
   *  @param[in] weights Weights used to set up the order of the neighbors to visit
   *  @param[in] dimension Dimension of the problem
   **/
  First_Order_Neighbourhood_Extended(const unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
                                     const vector<CoMMAWeightType> &weights) :
      First_Order_Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
        s_neighbours_of_seed, weights), _first_order_set() {}

  /** @brief Current neighbors, i.e. candidates */
  CoMMASetOfPairType _first_order_set;

  /** @brief Method that updates the first order neighborhood, by updating the
   * front. Given the new_fc, if is in the neighbours, it is deleted and are
   * added the s_new_neighbours in in the set of neighbours given in the
   * constructor.
   * @param[in] new_fc new fine cell to be added to the set of fine cells
   * @param[in] new_neighbours vector of the new neighbours to be analysed
   * @return A vector of candidates for the fine cell to agglomerate
   */
  vector<CoMMAIndexType> update(const CoMMAIndexType new_fc,
                                const vector<CoMMAIndexType> &new_neighbours) override {
    // Add new_fc to current CC and remove it from previous neighborhoods
    this->_s_fc.insert(new_fc);
    if (!this->_first_order_set.empty()) {
      // There is erase_if for sets in C++20
      //erase_if(_first_order_set, [&new_fc](const CoMMAPairType &p){
      //                              return p.first == new_fc;});
      auto it = find_if(this->_first_order_set.begin(), this->_first_order_set.end(),
                     CoMMAPairFindFirstBasedType(new_fc));
                     //[&new_fc](const CoMMAPairType &p){return p.first == new_fc;});
      assert(it != _first_order_set.end()); // It must be there!
      this->_first_order_set.erase(it);
    }

    // Compute the set of direct neighbors allowed by original neighborhood-order
    for (const CoMMAIndexType& i_fc : new_neighbours) {
      if ( (find_if(_first_order_set.begin(), _first_order_set.end(),
                   CoMMAPairFindFirstBasedType(i_fc))
                   //[&i_fc](const CoMMAPairType &p){return p.first == i_fc;})
            == _first_order_set.end()) &&
           (this->_s_fc.count(i_fc) == 0) &&
           (this->_s_neighbours_of_seed.count(i_fc) > 0) ) {
        // If not yet in the FON, not yet in the coarse cell and among the
        // allowed neighbours, insert
        this->_first_order_set.emplace(i_fc, this->_weights[i_fc]);
      }
    }
    return vector_of_first_elements<decltype(this->_first_order_set)>(this->_first_order_set);
  }
};

/** @brief Class representing the first order neighborhood of a given cell in
 * the graph. In this derived class the neighborhood is 'pure front-advancing',
 * meaning that the next candidates are only the direct neighbors of the last
 * added cell
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class First_Order_Neighbourhood_Pure_Front : public First_Order_Neighbourhood<CoMMAIndexType,
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
   *  @param[in] weights Weights used to set up the order of the neighbors to visit
   *  @param[in] dimension Dimension of the problem
   **/
  First_Order_Neighbourhood_Pure_Front(const unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
                                       const vector<CoMMAWeightType> &weights,
                                       CoMMAIntType dimension) :
      First_Order_Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
        s_neighbours_of_seed, weights), _q_fon(), _dimension(dimension) {}

  /** @brief History of the first-order-neighborhoods of the fine cells recently
   * agglomerated */
  deque<CoMMASetOfPairType> _q_fon;

  /** @brief dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3
   * -> 3D)*/
  CoMMAIntType _dimension;

  /** @brief Method that updates the first order neighborhood, by updating the
   * front. Given the new_fc, if is in the neighbours, it is deleted and are
   * added the s_new_neighbours in in the set of neighbours given in the
   * constructor.
   * @param[in] new_fc new fine cell to be added to the set of fine cells
   * @param[in] new_neighbours vector of the new neighbours to be analysed
   * @return A vector of candidates for the fine cell to agglomerate
   */
  vector<CoMMAIndexType> update(const CoMMAIndexType new_fc,
                                const vector<CoMMAIndexType> &new_neighbours) override {
    // Add new_fc to current CC and remove it from previous neighborhoods
    this->_s_fc.insert(new_fc);
    for (auto &fon : this->_q_fon) {
      // There is erase_if for sets in C++20
      //erase_if(fon, [&new_fc](const CoMMAPairType &p){return p.first == new_fc;});
      auto it = find_if(fon.begin(), fon.end(),
                        CoMMAPairFindFirstBasedType(new_fc));
                        //[&new_fc](const CoMMAPairType &p){return p.first == new_fc;});
      if (it != fon.end())
        fon.erase(it);
    }

    // Compute the set of direct neighbors allowed by original neighborhood-order
    CoMMASetOfPairType curr_set = CoMMASetOfPairType();
    for (const CoMMAIndexType& i_fc : new_neighbours) {
      if ( (this->_s_fc.count(i_fc) == 0) &&
           (this->_s_neighbours_of_seed.count(i_fc) > 0) ) {
        // If not yet in coarse cell and among the allowed neighbours, insert
        curr_set.emplace(i_fc, this->_weights[i_fc]);
      }
    }

    this->_q_fon.push_front(curr_set);

    // Now, see which FON to return. Here is the strategy:
    // If most recent FON is not empty, return it. If not, check the oldest FON: if
    // not empty return it, otherwise check the previous FON. If empty, check the
    // second oldest, and so on...
    // We grant ourselves one exception...
    if ( this->_q_fon.size() <= static_cast<decltype(this->_q_fon.size())>(this->_dimension) ) {
      // If at the (very) beginning of the agglomeration, still consider every
      // possible neighbor. This will allow to obtain nice quads from quads
      // TODO[RM]: I think this workaround is needed because we are not able to
      // compute exactly the AR; if we ever we will be able we should try to remove
      // it
      for (auto prev_fon = this->_q_fon.begin() + 1; prev_fon != this->_q_fon.end(); ++prev_fon)
        curr_set.insert(prev_fon->begin(), prev_fon->end());
      return vector_of_first_elements<decltype(curr_set)>(curr_set);
    }
    else {
      auto cur_front = decltype(this->_q_fon.size()){0};
      auto cur_back  = decltype(this->_q_fon.size()){this->_q_fon.size() - 1};
      while (cur_front <= cur_back) {
        typename decltype(this->_q_fon)::iterator it =
          this->_q_fon.begin() + (cur_front++);
        if ( !it->empty() )
          return vector_of_first_elements<typename decltype(this->_q_fon)::value_type>(*it);
        it =  this->_q_fon.begin() + (cur_back--);
        if ( !it->empty() )
          return vector_of_first_elements<typename decltype(this->_q_fon)::value_type>(*it);
      }
    }
    // If everything fails return an empty set
    return vector<CoMMAIndexType>();
  }

};

#endif  // COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
