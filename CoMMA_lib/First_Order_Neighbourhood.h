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
    _first_order_neighbourhood = {};
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
    _s_fc.insert(new_fc);
    if (_first_order_neighbourhood.count(new_fc)) {
      _first_order_neighbourhood.erase(new_fc);
    }
    for (const CoMMAIndexType& i_fc : s_new_neighbour) {
      if ( (_s_fc.count(i_fc) == 0) &&
           (_s_neighbours_of_seed.count(i_fc) > 0) ) {
        // If not yet in coarse cell and among the allowed neighbours, insert
        _first_order_neighbourhood.insert(i_fc);
      }
    }
    return _first_order_neighbourhood;
  }

  /** @brief Set of the fine cells composing the coarse cell */
  unordered_set<CoMMAIndexType> _s_fc;

  /** @brief Set of the neighbours of seed given as an input in the constructor.
   * Here, we can find all the neighbours of order up to a user-defined value
   * (by default is 3, this means neighbours of neighbours of neighbours) of the
   * initial seed. Hence, it holds the cells allowed to be agglomerated
   */
  unordered_set<CoMMAIndexType> _s_neighbours_of_seed;

  /** @brief Set of the first order neighborhood updated with the method update
   * of the class */
  unordered_set<CoMMAIndexType> _first_order_neighbourhood;
};

#endif  // COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
