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
    this->number_of_cells = number_of_cells;
    this->init_bnd_level = init_bnd_level;
    this->l_deque_of_seeds = vector<deque<long>>(4);
    // The size 4 corresponds to 0 : interior, 1 : valley, 2 : ridge, 3 : corner
    for (int i = 0; i < 4; i++) {
        this->l_deque_of_seeds[i] = deque<long>();
    }
    assert(!d_is_on_bnd.empty());
//    assert isinstance(d_is_on_bnd, dict)
    this->number_of_cells = number_of_cells;
    this->d_is_on_bnd = d_is_on_bnd;  // Useful for seed choice
    //For 2D, the mesh is 3D with one layer. We do not count the 2 lateral plans.
    if (!is_on_corner.empty()) {
        for (auto i : is_on_corner) {
            this->is_on_corner.insert(i);  // Useful for initial seed choice
        }
        for (auto i : is_on_ridge) {
            this->is_on_ridge.insert(i);  // Useful for initial seed choice
        }
        for (auto i : is_on_valley) {
            this->is_on_valley.insert(i);  // Useful for initial seed choice
        }
    }
//    for (auto iPairDict:dict_ConnectivityTree) {
//        max_dict[iPairDict.first] = iPairDict.second;
    for (auto kv_fc:this->d_is_on_bnd) {
        //    self._d_is_on_bnd.keys():
        long i_fc = kv_fc.first;
        int i_fc_bnd = kv_fc.second;


        if (i_fc_bnd >= 3) {  // With partitioning, we may have cell with self._d_is_on_bnd[0] = 4 or 5
            this->d_is_on_bnd[i_fc] = 3;  // To avoid trouble with listOfSeed (which is of size 4)
            this->is_on_corner.insert(i_fc);
        } else if (i_fc_bnd == 2) {
            this->is_on_ridge.insert(i_fc);
        } else if (i_fc_bnd == 1) {
            this->is_on_valley.insert(i_fc);
        }
    }

    // initialization of self.l_deque_of_seeds
    if (this->init_bnd_level <= 3 && this->is_on_corner.size() > 0) {
        for (auto iFC: this->is_on_corner) {
            this->l_deque_of_seeds[3].push_back(iFC);
        }
        this->is_on_corner.clear();
    }

    // Not used by default, legacy issue
    if (this->init_bnd_level <= 2 && this->is_on_ridge.size() > 0) {
        for (auto iFC: this->is_on_ridge) {
            this->l_deque_of_seeds[2].push_back(iFC);
        }
        this->is_on_ridge.clear();
    }

    // Not used by default, legacy issue
    if (this->init_bnd_level <= 1 && this->is_on_valley.size() > 0) {
        for (auto iFC: this->is_on_valley) {
            this->l_deque_of_seeds[1].push_back(iFC);
        }
        this->is_on_valley.clear();
    }
}

long Seeds_Pool::choose_new_seed(const bool *a_is_fc_agglomerated) {
//
//Choose a correct seed from the fc pool list_of_seeds beyond not agglomerated fc.
//:param a_is_fc_agglomerated:
//:return:
//
//We choose preferably the corners, then the ridges, then the valley, and finally interior cells:
// see NIA (Mavriplis uses Wall and farfield only)

    long seed = -1;
    for (int i_l = 3; i_l > -1; i_l--) {
        if (!(this->l_deque_of_seeds[i_l]).empty()) {
            seed = this->l_deque_of_seeds[i_l].front();
            this->l_deque_of_seeds[i_l].pop_front();
            while (a_is_fc_agglomerated[seed]) {
                if (!this->l_deque_of_seeds[i_l].empty()) {
                    seed = this->l_deque_of_seeds[i_l].front();
                    this->l_deque_of_seeds[i_l].pop_front();
                } else {
                    break;
                }
            }
            if (a_is_fc_agglomerated[seed]) {
                continue;  // no correct new seed, so we try i_l--
            } else {
                break;  // a new seed no need to try i_l--
            }
        }
    }

    // if no seed were found in list_of_seeds... we look in ridges or valley
    if (seed == -1 or a_is_fc_agglomerated[seed]) {
        for (int i_l = 2; i_l > -1; i_l--) {
            // we check in this->is_on_ridge/this->is_on_valley if anything is available?
            if (i_l == 2) {
                if (!this->is_on_ridge.empty()) {
                    seed = (*this->is_on_ridge.begin());
                    this->is_on_ridge.erase(seed);
                    while (a_is_fc_agglomerated[seed]) {
                        if (!this->is_on_ridge.empty()) {
                            seed = *this->is_on_ridge.begin();
                            this->is_on_ridge.erase(seed);
                        } else {
                            break;
                        }
                    }
                    break;
                }
            } else if (i_l == 1) {
                if (!this->is_on_valley.empty()) {
                    seed = *this->is_on_valley.begin();
                    this->is_on_valley.erase(seed);
                    while (a_is_fc_agglomerated[seed]) {
                        if (!this->is_on_valley.empty()) {
                            seed = *this->is_on_valley.begin();
                            this->is_on_valley.erase(seed);
                        } else {
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    // if no seed were found in listOfSeeds nor in in ridges or valley we take the first one!
    // Useful if the listOfSeeds contains no correct seed (not already agglomerated!)
    if ((seed == -1) || (a_is_fc_agglomerated[seed])) {
        // We do not have a correct seed:
        for (int i = 0; i < this->number_of_cells; i++) {
            if (!a_is_fc_agglomerated[i]) {
                seed = i;
                break;
            }
        }
    }
    return seed;
}

int Seeds_Pool::boundary_value(const long i_fc) {
    auto i_fc_finder = this->d_is_on_bnd.find(i_fc);
    if (i_fc_finder != this->d_is_on_bnd.end()) {
        return this->d_is_on_bnd[i_fc];
    } else {
        return 0;
    }

}

void Seeds_Pool::update(
        list<long> l_new_seeds,
        bool is_extremal_vertex) {

    if (!is_extremal_vertex){
        for (long i_new_seed :l_new_seeds){
            // the value of isOnBnd[_i_lvl-1][i_new_seed] may be strictly bigger than 3, in case of partitionning
            // via Metis or Scotch.
            int value_is_on_bnd = 0;
            auto i_new_seed_finder = this->d_is_on_bnd.find(i_new_seed);
            if (i_new_seed_finder != this->d_is_on_bnd.end()) {
                value_is_on_bnd = this->d_is_on_bnd[i_new_seed];
            }
            // Update of isOnBnd to avoid value > 3
            if(value_is_on_bnd >= 3){
                value_is_on_bnd = 3;
                this->d_is_on_bnd[i_new_seed] = 3;
            }
            this->l_deque_of_seeds[value_is_on_bnd].push_back(i_new_seed);
        }
    }else {
        // here we add as bnd point at the extremity of graph, where eccentricity=diameter
        // TODO: useful?
        for (long i_new_seed :l_new_seeds) {
            this->d_is_on_bnd[i_new_seed] = 4;
            //self._update(3, i_new_seed)
            this->l_deque_of_seeds[3].push_back(i_new_seed);

        }
    }
}


bool Seeds_Pool::is_empty(int i_level){
    assert (0 <= i_level);
    assert (i_level <= 3);
    if(this->l_deque_of_seeds.size()>0)
    {
        for (int i=3;  i>i_level-1; i--){

            if(!this->l_deque_of_seeds[i].empty()) {
                return false;
            }
        }
    }
    return true;


}

bool Seeds_Pool::is_initialized(){
    return !d_is_on_bnd.empty();
}
