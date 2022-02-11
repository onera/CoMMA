//
// Created by Nicolas Lantos on 22/03/2021.
//
#include "Seeds_Pool.h"

Seeds_Pool::Seeds_Pool(int number_of_cells,
                       unordered_map<long, int> &d_is_on_bnd,
                       unordered_set<long> is_on_corner,
                       unordered_set<long> is_on_ridge,
                       unordered_set<long> is_on_valley,
                       int init_bnd_level) {
    /*
    :param number_of_cells: number of cells in the graph
    :param d_is_on_bnd: dictionary i_c to bnd value
    :param is_on_corner: set of indices of cells in corner
    :param is_on_ridge: set of indices of cells in ridge
    :param is_on_valley: set of indices of cells in valley
    :param init_bnd_level: level of bnd (i.e. corner, ridge, valley) we want to put initially in the pool. Default 3
    */
    // We want the seed to be chosen preferably in the corner, then ridges, valleys and then interiors.
    // The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
    // to minimizes the too small cells!
    // is_on_bnd table can then contains 4 values:
    // 0 : interior (no face on the edge of the domain)
    // 1 : valley (one face on the edge of the domain)
    // 2 : ridge (two faces on the edge of the domain)
    // 3 : corner (three faces on the edge of the domain)
    _number_of_cells = number_of_cells;
    _init_bnd_level = init_bnd_level;
    // The size 4 corresponds to 0 : interior, 1 : valley, 2 : ridge, 3 : corner
    _l_of_seeds = vector<Stack<long>>(4);
    assert(!d_is_on_bnd.empty());
//    assert isinstance(d_is_on_bnd, dict)
    _d_is_on_bnd = d_is_on_bnd;  // Useful for seed choice
    //For 2D, the mesh is 3D with one layer. We do not count the 2 lateral plans.
    // only in case of 3D we have on corner case and the ridge and valley will be filled
    // otherwise we get it through the boundary dictionary.
    if (is_on_ridge.empty() || is_on_valley.empty()){
       cout << "WARNING! Boundary values emty, check again" << endl;
    }
    for (auto i : is_on_corner) {
            _is_on_corner.insert(i);  // Useful for initial seed choice
        }
    for (auto i : is_on_ridge) {
            _is_on_ridge.insert(i);  // Useful for initial seed choice
        }
    for (auto i : is_on_valley) {
            _is_on_valley.insert(i);  // Useful for initial seed choice
        }
    // initialization of self.l_deque_of_seeds
    if (_init_bnd_level <= 3 && _is_on_corner.size() > 0) {
        for (auto iFC: _is_on_corner) {
           _l_of_seeds[3].push(iFC);
          _init_bnd_level--;
        }
    }

    // Not used by default, legacy issue
    if (_init_bnd_level <= 2 && _is_on_ridge.size() > 0) {
        for (auto iFC: _is_on_ridge) {
            _l_of_seeds[2].push(iFC);
            _init_bnd_level--;
        }
    }

    // Not used by default, legacy issue
    if (_init_bnd_level <= 1 && _is_on_valley.size() > 0) {
        for (auto iFC: _is_on_valley) {
            _l_of_seeds[1].push(iFC);
        }
    }
}

long Seeds_Pool::choose_new_seed(const vector<bool> &a_is_fc_agglomerated) {
//
//Choose a correct seed from the fc pool list_of_seeds beyond not agglomerated fc.
//:param a_is_fc_agglomerated:
//:return:
//
//We choose preferably the corners, then the ridges, then the valley, and finally interior cells:
// see NIA (Mavriplis uses Wall and farfield only)
// Exactly the inverse of the order of the list. For this reason we proceed with l--
//    long seed = -1;
  for (int i_l = 3; i_l > -1; i_l--) {
      if (_l_of_seeds[i_l].top()!=-1 && !a_is_fc_agglomerated[_l_of_seeds[i_l].top()]){
         return(_l_of_seeds[i_l].pop());
      }
      else
      { continue;} 
   }
   for (int i = 0; i < _number_of_cells; i++) {
            if (!a_is_fc_agglomerated[i]) {
                return(i);
            }
         }
}
int Seeds_Pool::boundary_value(const long &i_fc) {
    auto i_fc_finder = _d_is_on_bnd.find(i_fc);
    if (i_fc_finder != _d_is_on_bnd.end()) {
        return _d_is_on_bnd[i_fc];
    } else {
        return 0;
    }

}

void Seeds_Pool::update(
        const list<long> &l_new_seeds,
        bool is_extremal_vertex) {

    if (!is_extremal_vertex){
        for (long i_new_seed :l_new_seeds){
            // the value of isOnBnd[_i_lvl-1][i_new_seed] may be strictly bigger than 3, in case of partitionning
            // via Metis or Scotch.
            int value_is_on_bnd = 0;
            auto i_new_seed_finder = _d_is_on_bnd.find(i_new_seed);
            if (i_new_seed_finder != _d_is_on_bnd.end()) {
                value_is_on_bnd = _d_is_on_bnd[i_new_seed];
            }
            // Update of isOnBnd to avoid value > 3
            if(value_is_on_bnd >= 3){
                value_is_on_bnd = 3;
                _d_is_on_bnd[i_new_seed] = 3;
            }
            _l_of_seeds[value_is_on_bnd].push(i_new_seed);
        }
    }else {
        // here we add as bnd point at the extremity of graph, where eccentricity=diameter
        for (long i_new_seed :l_new_seeds) {
            _d_is_on_bnd[i_new_seed] = 4;
            _l_of_seeds[3].push(i_new_seed);

        }
    }
}


bool Seeds_Pool::is_empty(int i_level){
    assert (0 <= i_level);
    assert (i_level <= 3);
    if(_l_of_seeds.size()>0)
    {
        for (int i=3;  i>i_level-1; i--){
            if(_l_of_seeds[i].top()!=-1) {
                return false;
            }
        }
    }
    return true;
}
