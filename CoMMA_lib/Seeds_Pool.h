//
// Created by Nicolas Lantos on 22/03/2021.
//

#ifndef COMMA_PROJECT_SEEDS_POOL_H
#define COMMA_PROJECT_SEEDS_POOL_H

#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <deque>
#include <queue>
#include <vector>
#include <list>


using namespace std;

/** @brief Class representing the pool of all the seeds for creating a coarse cell  */

class Seeds_Pool {

public :
/** @brief Constructor */
    Seeds_Pool(int number_of_cells,
               unordered_map<long, int> &d_is_on_bnd,
               unordered_set<long> is_on_corner = unordered_set<long>({}),
               unordered_set<long> is_on_ridge = unordered_set<long>({}),
               unordered_set<long> is_on_valley = unordered_set<long>({}),
               int init_bnd_level = 3);
/** @brief Number of cells */
    int number_of_cells;
/** @brief Dictionary of number of faces on boundary per cell*/
    unordered_map<long, int> d_is_on_bnd;
/** @brief set of cells on corner */
    unordered_set<long> is_on_corner;
/** @brief set of cells on ridge */
    unordered_set<long> is_on_ridge;
/** @brief set of cells on valley */
    unordered_set<long> is_on_valley;
/** @brief level of boundary we want to put initially in the pool (default is 3)
 *  We want the seed to be chosen preferably in the corner, then ridges, valleys and then interiors.
 *  The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
 *  to minimizes the too small cells!
 *  0 : interior (no face on the edge of the domain)
 *  1 : valley (one face on the edge of the domain)
 *  2 : ridge (two faces on the edge of the domain)
 *  3 : corner (three faces on the edge of the domain)
*/
    int init_bnd_level;
/** @brief List of deque of seeds. For each identifier we have the available seeds
 *  0 : interior (no face on the edge of the domain)
 *  1 : valley (one face on the edge of the domain)
 *  2 : ridge (two faces on the edge of the domain)
 *  3 : corner (three faces on the edge of the domain)
*/
    vector<deque<long>> l_deque_of_seeds;
/** @brief Choose a new seed in the pool
 *  @param[in] a_is_fc_agglomerated vector of boolean with fine cells agglomerated (true) or not agglomerated (false)
 *  @return New seed
 */
    long choose_new_seed(const vector<bool> a_is_fc_agglomerated);

/** @brief Given the index of the cell it returns the number of faces on the boundary analysing the
 * dictionaty of faces on boundary cell
 * @param[in] i_fc global index of the fine cell
 * @return the number of faces
*/
    int boundary_value(const long i_fc);
/** @brief We fill the deque of new seed and the dictionary on boundary
 * @param[in] l_new_seed is a list of the index of new seeds
 * @param[in] is_extremal_vertex flag to define if we are at the extremity of a graph   */
    void update(list<long> l_new_seeds, bool is_extremal_vertex = false);
/** @brief Given the default levels we define if the list of the targeted level is empty
 * @param[in] i_level level of the defined list */
    bool is_empty(int i_level=0);
/** @brief We define if the dictionary of boundary is empty or already filled. */
    inline bool is_initialized(){
	    return !d_is_on_bnd.empty();
    }
};

#endif //COMMA_PROJECT_SEEDS_POOL_H
