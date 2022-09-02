//
// Created by Nicolas Lantos on 21/04/2021.
//

#ifndef COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
#define COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
#include <unordered_set>

using namespace std;
/** @brief Class representing the first order neighborhood of a given cell in
 * the graph.*/
class First_Order_Neighbourhood {
 public:
  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed set of the neighbours of the given cell
   * chosen as seed */
  First_Order_Neighbourhood(unordered_set<long> s_neighbours_of_seed);
  /** @brief Method that updates the first order neighborhood, by updating the
   * front. given the new_fc, if is
   * in the neiighbours, it is deleted and are added the s_new_neighbours in in
   * the set of neighbours given in the
   * constructor.
   * @param[in] new_fc new fine cell to be added to the set of fine cells
   * @param[in] s_new_neighbours set of the new neighbours to be analysed and to
   * be added to the __first_order_neighbourhood
   * member variable only if the element of the set are in the set of neighbours
   * of seed.*/
  unordered_set<long> update(long new_fc, unordered_set<long> s_new_neighbour);
  /** @brief Set of the fine cells composing the coarse cell */
  unordered_set<long> __s_fc;
  /** @brief Set of the neighbours of seed given as an input in the constructor
   */
  unordered_set<long> __s_neighbours_of_seed;
  /** @brief Set of the first order neighborhood updated with the method update
   * of the class */
  unordered_set<long> __first_order_neighbourhood;
};

#endif  // COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
