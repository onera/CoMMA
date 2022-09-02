//
// Created by Nicolas Lantos on 22/03/2021.
//

#ifndef COMMA_PROJECT_SEEDS_POOL_H
#define COMMA_PROJECT_SEEDS_POOL_H
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <cassert>
#include <vector>
#include "Queue.h"

using namespace std;

/** @brief Class representing the pool of all the seeds for creating a coarse
 * cell  */

class Seeds_Pool {

 protected:
  /** @brief Number of cells */
  int _number_of_cells;
  /** @brief set of cells on corner */
  unordered_set<long> _is_on_corner;
  /** @brief set of cells on ridge */
  unordered_set<long> _is_on_ridge;
  /** @brief set of cells on valley */
  unordered_set<long> _is_on_valley;
  /** @brief level of boundary we want to put initially in the pool (default is
   * 3)
   *  We want the seed to be chosen preferably in the corner, then ridges,
   * valleys and then interiors.
   *  The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
   *  to minimizes the too small cells!
   *  0 : interior (no face on the edge of the domain)
   *  1 : valley (one face on the edge of the domain)
   *  2 : ridge (two faces on the edge of the domain)
   *  3 : corner (three faces on the edge of the domain)
  */
  int _init_bnd_level;
  /** @brief List of deque of seeds. For each identifier we have the available
   * seeds
   *  0 : interior (no face on the edge of the domain)
   *  1 : valley (one face on the edge of the domain)
   *  2 : ridge (two faces on the edge of the domain)
   *  3 : corner (three faces on the edge of the domain)
  */
  vector<Queue<long>> _l_of_seeds;

 public:
  /** @brief Constructor */
  Seeds_Pool(int number_of_cells, unordered_map<long, int> &d_is_on_bnd);

  /** @brief Destructor */
  ~Seeds_Pool() {};
  /** @brief Choose a new seed in the pool
   *  @param[in] a_is_fc_agglomerated vector of boolean with fine cells
   * agglomerated (true) or not agglomerated (false)
   *  @return New seed
   */

  long choose_new_seed(const vector<bool> &a_is_fc_agglomerated);

  /** @brief Given the index of the cell it returns the number of faces on the
   * boundary analysing the
   * dictionaty of faces on boundary cell
   * @param[in] i_fc global index of the fine cell
   * @return the number of faces
  */
  int boundary_value(const long &i_fc);
  /** @brief We fill the deque of new seed and the dictionary on boundary
   * @param[in] l_new_seed is a list of the index of new seeds
   * @param[in] is_extremal_vertex flag to define if we are at the extremity of
   * a graph   */
  void update(const list<long> &l_new_seeds, bool is_extremal_vertex = false);
  /** @brief Given the default levels we define if the list of the targeted
   * level is empty
   * @param[in] i_level level of the defined list */
  bool is_empty(int i_level = 0);
  /** @brief We define if the dictionary of boundary is empty or already filled.
   */
  inline bool is_initialized() { return !_d_is_on_bnd.empty(); }
  /** @brief Spoil seeds of one particular category (on_corner, on_valley,
   * on_ridge, interior)
   *  @param[in] i_l the category
   *  @return the seed or -1 in case the seed of a particular category are
   * finished
  */
  long spoil_seed(const int &i_l, const vector<bool> &a_is_fc_agglomerated);
  /** @brief Dictionary of number of faces on boundary per cell*/
  unordered_map<long, int> _d_is_on_bnd;
};

#endif  // COMMA_PROJECT_SEEDS_POOL_H
