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

#include <unordered_set>
#include <unordered_map>
#include <list>
#include <cassert>
#include <vector>

#include "Queue.h"
#include "CoMMATypes.h"

using namespace std;

/** @brief Class representing the pool of all the seeds for creating a coarse
 * cell  */

template <typename CoMMAIndexType, typename CoMMAIntType>
class Seeds_Pool {

 protected:
  /** @brief Number of cells */
  CoMMAIndexType _number_of_cells;
  /** @brief set of cells on corner */
  unordered_set<CoMMAIndexType> _is_on_corner;
  /** @brief set of cells on ridge */
  unordered_set<CoMMAIndexType> _is_on_ridge;
  /** @brief set of cells on valley */
  unordered_set<CoMMAIndexType> _is_on_valley;
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
  CoMMAIntType _init_bnd_level;
  /** @brief List of deque of seeds. For each identifier we have the available
   * seeds
   *  0 : interior (no face on the edge of the domain)
   *  1 : valley (one face on the edge of the domain)
   *  2 : ridge (two faces on the edge of the domain)
   *  3 : corner (three faces on the edge of the domain)
  */
  vector<Queue<CoMMAIndexType>> _l_of_seeds;

 public:
  /** @brief Constructor */
  Seeds_Pool(CoMMAIndexType number_of_cells,
             unordered_map<CoMMAIndexType, CoMMAIntType> &d_is_on_bnd) {
    /*
    :param number_of_cells: number of cells in the graph
    :param d_is_on_bnd: dictionary i_c to bnd value
    :param is_on_corner: set of indices of cells in corner
    :param is_on_ridge: set of indices of cells in ridge
    :param is_on_valley: set of indices of cells in valley
    :param init_bnd_level: level of bnd (i.e. corner, ridge, valley) we want to
    put initially in the pool. Default 3
    */
    // We want the seed to be chosen preferably in the corner, then ridges,
    // valleys and then interiors.
    // The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
    // to minimizes the too small cells!
    // is_on_bnd table can then contains 4 values:
    // 0 : interior (no face on the edge of the domain)
    // 1 : valley (one face on the edge of the domain)
    // 2 : ridge (two faces on the edge of the domain)
    // 3 : corner (three faces on the edge of the domain)
    _number_of_cells = number_of_cells;
    // The size 4 corresponds to 0 : interior, 1 : valley, 2 : ridge, 3 : corner
    _l_of_seeds = vector<Queue<CoMMAIndexType>>(CoMMACellT::N_CELL_TYPES);
    assert(!d_is_on_bnd.empty());
    _d_is_on_bnd = d_is_on_bnd;  // Useful for seed choice
    for (auto kv_fc : d_is_on_bnd) {
      CoMMAIndexType i_fc = kv_fc.first;
      CoMMAIntType i_fc_bnd = kv_fc.second;
      if (i_fc_bnd >= CoMMACellT::CORNER) {
        _d_is_on_bnd[i_fc] = CoMMACellT::CORNER;
        _is_on_corner.insert(i_fc);
      } else if (i_fc_bnd == CoMMACellT::RIDGE) {
        _is_on_ridge.insert(i_fc);
      } else if (i_fc_bnd == CoMMACellT::VALLEY) {
        _is_on_valley.insert(i_fc);
      }
    }
    // initialization of l_of_seeds
    if (_is_on_corner.size() > 0) {
      for (auto iFC : _is_on_corner) {
        _l_of_seeds[CoMMACellT::CORNER].push(iFC);
      }
    }

    if (_is_on_ridge.size() > 0) {
      for (auto iFC : _is_on_ridge) {
        _l_of_seeds[CoMMACellT::RIDGE].push(iFC);
      }
    }
    if (_is_on_valley.size() > 0) {
      for (auto iFC : _is_on_valley) {
        _l_of_seeds[CoMMACellT::VALLEY].push(iFC);
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
    //
    // Choose a correct seed from the fc pool list_of_seeds beyond not
    // agglomerated fc.
    //:param a_is_fc_agglomerated:
    //:return:
    //
    // We choose preferably the corners, then the ridges, then the valley, and
    // finally interior cells:
    // see NIA (Mavriplis uses Wall and farfield only)
    // Exactly the inverse of the order of the list. For this reason we proceed
    // with l--
    for (CoMMAIntType i_l = CoMMACellT::CORNER; i_l >= CoMMACellT::INTERIOR; i_l--) {
      CoMMAIndexType seed = spoil_seed(i_l, a_is_fc_agglomerated);
      if (seed != -1) {
        return (seed);
      } else {
        continue;
      }
    }
    for (CoMMAIndexType i = 0; i < _number_of_cells; i++) {
      if (!a_is_fc_agglomerated[i]) {
        return (i);
      }
    }
    // Avoid warning
    return -1;
  };

  /** @brief Given the index of the cell it returns the number of faces on the
   * boundary analysing the dictionary of faces on boundary cell
   * @param[in] i_fc global index of the fine cell
   * @return the number of faces
  */
  CoMMAIntType boundary_value(const CoMMAIndexType &i_fc) {
    auto i_fc_finder = _d_is_on_bnd.find(i_fc);
    if (i_fc_finder != _d_is_on_bnd.end()) {
      return _d_is_on_bnd[i_fc];
    } else {
      return CoMMACellT::INTERIOR;
    }
  };

  /** @brief We fill the deque of new seed and the dictionary on boundary
   * @param[in] l_new_seed is a list of the index of new seeds
   * @param[in] is_extremal_vertex flag to define if we are at the extremity of
   * a graph*/
  void update(const list<CoMMAIndexType> &l_new_seeds,
              bool is_extremal_vertex = false) {
    if (!is_extremal_vertex) {
      for (const CoMMAIndexType &i_new_seed : l_new_seeds) {
        // the value of isOnBnd[_i_lvl-1][i_new_seed] may be strictly bigger
        // than
        // 3, in case of partitioning
        // via Metis or Scotch.
        CoMMAIntType value_is_on_bnd = CoMMACellT::INTERIOR;
        auto i_new_seed_finder = _d_is_on_bnd.find(i_new_seed);
        if (i_new_seed_finder != _d_is_on_bnd.end()) {
          value_is_on_bnd = _d_is_on_bnd[i_new_seed];
        }
        // Update of isOnBnd to avoid value > 3
        if (value_is_on_bnd >= CoMMACellT::CORNER) {
          value_is_on_bnd = CoMMACellT::CORNER;
          _d_is_on_bnd[i_new_seed] = CoMMACellT::CORNER;
        }
        _l_of_seeds[value_is_on_bnd].push(i_new_seed);
      }
    } else {
      // here we add as bnd point at the extremity of graph, where
      // eccentricity=diameter
      for (const CoMMAIndexType &i_new_seed : l_new_seeds) {
        _d_is_on_bnd[i_new_seed] = CoMMACellT::EXTREME;
        _l_of_seeds[CoMMACellT::CORNER].push(i_new_seed);
      }
    }
  };

  /** @brief Given the default levels we define if the list of the targeted
   * level is empty
   * @param[in] i_level level of the defined list */
  bool is_empty(CoMMAIntType i_level = 0) {
    assert(CoMMACellT::INTERIOR <= i_level);
    assert(i_level <= CoMMACellT::CORNER);
    if (_l_of_seeds.size() > 0) {
      for (CoMMAIntType i = CoMMACellT::CORNER; i > i_level -1; i--) {
        if (_l_of_seeds[i].top() != -1) {
          return false;
        }
      }
    }
    return true;
  };
  /** @brief We define if the dictionary of boundary is empty or already filled.
   */
  inline bool is_initialized() { return !_d_is_on_bnd.empty(); }
  /** @brief Spoil seeds of one particular category (on_corner, on_valley,
   * on_ridge, interior)
   *  @param[in] i_l the category
   *  @return the seed or -1 in case the seed of a particular category are
   * finished
  */
  CoMMAIndexType spoil_seed(const CoMMAIntType &i_l,
                            const vector<bool> &a_is_fc_agglomerated) {
    CoMMAIndexType seed = _l_of_seeds[i_l].pop();
    if (seed == -1) {
      return (seed);
    } else {
      while (a_is_fc_agglomerated[seed] == true) {
        seed = _l_of_seeds[i_l].pop();
        if (seed == -1) {
          return (seed);
        }
      }
    }
    return (seed);
  };
  /** @brief Dictionary of number of faces on boundary per cell*/
  unordered_map<CoMMAIndexType, CoMMAIntType> _d_is_on_bnd;
};
#endif  // COMMA_PROJECT_SEEDS_POOL_H
