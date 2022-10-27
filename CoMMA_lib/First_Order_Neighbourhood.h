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

#include <unordered_set>
#include <vector>
#include <deque>

using namespace std;

/** @brief Class representing the first order neighborhood of a given cell in
 * the graph.
 * Mind that no information about the element being already agglomerated or not
 * is known here.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 */
template <typename CoMMAIndexType>
class First_Order_Neighbourhood {
 public:
  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed set of the neighbours of the given cell
   * chosen as seed */
  First_Order_Neighbourhood(
      unordered_set<CoMMAIndexType> s_neighbours_of_seed) {
    _s_fc = {};                                    // definition of the cc
    _s_neighbours_of_seed = s_neighbours_of_seed;  // defined once and for all
    _q_fon = {};
  }

  /** @brief Method that updates the first order neighborhood, by updating the
   * front. Given the new_fc, if is in the neighbours, it is deleted and are
   * added the s_new_neighbours in in the set of neighbours given in the
   * constructor.
   * @param[in] new_fc new fine cell to be added to the set of fine cells
   * @param[in] s_new_neighbours set of the new neighbours to be analysed and to
   * be added to the _first_order_neighbourhood member variable only if the
   * element of the set are in the set of neighbours of seed.
   */
  unordered_set<CoMMAIndexType> update(
      CoMMAIndexType new_fc, vector<CoMMAIndexType> s_new_neighbour) {

    // Add new_fc to current CC and remove it from previous neighborhoods
    _s_fc.insert(new_fc);
    for (auto &fon : _q_fon)
      fon.erase(new_fc);

    // Compute the set of direct neighbors allowed by original neighborhood-order
    unordered_set<CoMMAIndexType> curr_set = unordered_set<CoMMAIndexType>();
    for (const CoMMAIndexType& i_fc : s_new_neighbour) {
      if ( (_s_fc.count(i_fc) == 0) &&
           (_s_neighbours_of_seed.count(i_fc) > 0) ) {
        // If not yet in coarse cell and among the allowed neighbours, insert
        curr_set.insert(i_fc);
      }
    }

    // Add current set to the queue
    _q_fon.push_front(curr_set);

    // Now, see which FON to return. Here is the strategy:
    // If most recent FON is not empty, return it. If not, check the oldest FON: if
    // not empty return it, otherwise check the previous FON. If empty, check the
    // second oldest, and so on...
    auto cur_front = decltype(_q_fon.size()){0};
    auto cur_back  = decltype(_q_fon.size()){_q_fon.size() - 1};
    while (cur_front <= cur_back) {
      typename deque<unordered_set<CoMMAIndexType>>::iterator it =
        _q_fon.begin() + (cur_front++);
      if ( !it->empty() )
        return *it;
      it =  _q_fon.begin() + (cur_back--);
      if ( !it->empty() )
        return *it;
    }
    // If everything failed, return empty set
    return {};
  }

  /** @brief Set of the fine cells composing the coarse cell */
  unordered_set<CoMMAIndexType> _s_fc;

  /** @brief Set of the neighbours of seed given as an input in the constructor.
   * Here, we can find all the neighbours of order up to a user-defined value
   * (by default is 3, this means neighbours of neighbours of neighbours) of the
   * initial seed. Hence, it holds the cells allowed to be agglomerated
   */
  unordered_set<CoMMAIndexType> _s_neighbours_of_seed;

  /** @brief History of the first-order-neighborhoods of the fine cells recently
   * agglomerated */
  deque<unordered_set<CoMMAIndexType>> _q_fon;
};

#endif  // COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
