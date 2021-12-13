//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Coarse_Cell.h"

Coarse_Cell::Coarse_Cell(Dual_Graph &fc_graph,
                         long i_cc,
                         const unordered_set<long> &s_fc,
                         bool is_isotropic,
                         bool is_delayed) : __dim(fc_graph._dimension), __is_isotropic(is_isotropic), __is_delayed(is_delayed) {
    // input set of fc is empty!
//    assert(!s_fc.empty());
    __fc_graph = &fc_graph;
    __i_cc = i_cc;
    __card = s_fc.size();

    for (const long &i_fc : s_fc) {
        __s_fc.insert(i_fc);
    }
    __compactness = __card;  //we initialize (*this).__compactness with a big value
    for (const long &i_fc : s_fc) {

        assert(i_fc < fc_graph.number_of_cells);
        volume += fc_graph._volumes[i_fc];

        vector<long> v_neighbours = fc_graph.get_neighbours(i_fc);
        vector<double> v_weights = fc_graph.get_weights(i_fc);
        assert(v_neighbours.size() == v_weights.size());

        for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {
            long i_fc_n = v_neighbours[i_n];
            double i_w_fc_n = v_weights[i_n];
            if (s_fc.count(i_fc_n) > 0) {
                if (i_fc == i_fc_n) {

                    // Here we put boundary information inside a variable and remove it of d_def: which contains so
                    // only real neighbour
                    __boundary_area += i_w_fc_n;

                } else {
                    __d_def[i_fc][i_fc_n] = i_w_fc_n;
                }
            } else {
                __tmp_fc_fine_cut_edges[i_fc][i_fc_n] = i_w_fc_n;
            }
        }

        // __compactness is the min degree of a vertex inside a cc.
        assert (SHRT_MAX > __d_def[i_fc].size());
        unsigned short int tmp_degree = __d_def[i_fc].size();
        if (tmp_degree < __compactness) {
            __compactness = tmp_degree;
        }
        if (__d_compactness_to_s_fc.count(tmp_degree) > 0) {
            __d_compactness_to_s_fc[tmp_degree].insert(i_fc);
        } else {
            __d_compactness_to_s_fc[tmp_degree] = {i_fc};
        }
    }

    if (__compactness == __dim) {
        _is_finalized = true;
    }

}

bool Coarse_Cell::is_connected() {

    if (!__is_connectivity_up_to_date) {
       __is_connected = __check_connectivity();
       __is_connectivity_up_to_date = true;
    }
    return __is_connected;

}

bool Coarse_Cell::__check_connectivity(
        int verbose) {
/**
 * Checks connectivity of the coarse cell
 * :param verbose: level of output
 * :return: True or False
 */

    // We assume that the size of the coarse is small <USHRT_MAX
    //TODO smartly manage this: i.e. at creation and when we add a cell
    //TODO also checks positivity when we remove a fc!
    assert(__card < USHRT_MAX);

    if (__card <= 1) {
        return true;
    }

    //list_fc = list((*this).__d_def.keys())
    unsigned short int nb_fc = __d_def.size();
    long list_fc[nb_fc];
    unsigned short int i = 0;
    for (auto iKV:__d_def) {
        list_fc[i++] = iKV.first;
    }

    __is_already_connected.clear();
    __is_already_connected.insert(0);
    unordered_set<long> set_next = {list_fc[0]};

    unsigned short int nb_connected_cells = 1;
    unordered_map<long, unsigned short int> dict_global_to_local;

    for (unsigned short int i_loc_fc = 0; i_loc_fc < __card; i_loc_fc++) {
        dict_global_to_local[list_fc[i_loc_fc]] = i_loc_fc;
    }

    while (nb_connected_cells < __card) {

        if (!set_next.empty()) {
            // i_fc = set_next.pop()
            long i_fc = *set_next.begin();
            set_next.erase(set_next.begin());

            for (auto &i_k_v :__d_def[i_fc]) {

                long i_fc_n = i_k_v.first;
                if (dict_global_to_local.count(i_fc_n) > 0) {
                    if (__is_already_connected.count(dict_global_to_local[i_fc_n]) == 0) {
                        // dict_global_to_local[i_fc_n] not in (*this).__is_already_connected
                        set_next.insert(i_fc_n);
                        nb_connected_cells += 1;
                        __is_already_connected.insert(dict_global_to_local[i_fc_n]);
                    }
                }

            }


        } else {
            break;
        }

    }
    return (nb_connected_cells == __card);
}


void Coarse_Cell::fill_cc_neighbouring(vector<long> &fc_2_cc) {

    // TODO To update to avoid to recompute everything each time

    if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc.empty()) {
        __is_cc_neighbourhood_build = true;
        if (__tmp_fc_fine_cut_edges.empty()) {
            unordered_set<long> s_fc;

            for (auto &i_k_v :__d_def) {
                s_fc.insert(i_k_v.first);
            }
            for (const long &i_fc :s_fc) {
//                (*this).__tmp_fc_fine_cut_edges[i_fc] = dict();
                vector<long> v_neighbours = (*__fc_graph).get_neighbours(i_fc);
                vector<double> v_weights = (*__fc_graph).get_weights(i_fc);

                assert(v_neighbours.size() == v_weights.size());

                for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {
                    long i_fc_n = v_neighbours[i_n];
                    double i_w_fc_n = v_weights[i_n];

                    //                    for i_fc_n, i_w_fc_n in zip((*this).__fc_graph.get_neighbours(i_fc),
                    //                (*this).__fc_graph.get_weights(i_fc)):
                    if (s_fc.count(i_fc_n) == 0) {
                        __tmp_fc_fine_cut_edges[i_fc][i_fc_n] = i_w_fc_n;
                    }
                }
            }
        }
        for (auto &i_k_v :__tmp_fc_fine_cut_edges) {

            long i_fc = i_k_v.first;

//            if ((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc)==0){
////                (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc] = dict()
//            }



            for (auto &j_k_v : __tmp_fc_fine_cut_edges[i_fc]) {
                long j_fc = j_k_v.first;
                __add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc,
                                                                       fc_2_cc[j_fc],
                                                                       j_fc,
                                                                       __tmp_fc_fine_cut_edges[i_fc][j_fc]);
            }
            // remove dict entry if its empty:
            if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].empty()) {
                __d_i_fc_to_j_cc_neighbourhood_to_j_fc.erase(i_fc);
            }
        }
        // computation of d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour
        __compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour();

// TODO we could theoretically delete (*this).__tmp_fc_fine_cut_edges but it break some test cases where there are
//  multiple calls to fill_cc_neighbouring.
// (*this).__tmp_fc_fine_cut_edges = None  // (*this).__tmp_fc_fine_cut_edges is no more useful.
    }

}

// Method to add to the map
void Coarse_Cell::__add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(long i_fc, long j_cc, long j_fc, double j_fc_area) {
    assert(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].count(j_fc) == 0);
    __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc][j_fc] = j_fc_area;
}

void Coarse_Cell::__compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour() {
    d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.clear();
    for (auto &i_k_v : __d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
        long i_fc = i_k_v.first;
        for (auto &j_k_v : __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]) {
            long j_cc = j_k_v.first;
            unsigned short int nb_fine_edges_wrt_cc = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].size();
            if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].count(i_fc) > 0) {
                d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].insert(j_cc);
            } else {
                d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc] = unordered_set<long>{j_cc};
            }
        }
    }
}

unordered_set<long> Coarse_Cell::get_s_cc_neighbours() {

    unordered_set<long> s_cc;
    for (auto &i_k_v : __d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
//        cout<<"i_fc "<< i_k_v.first<<endl;
        for (auto &j_k_v : __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_k_v.first]) {
            s_cc.insert(j_k_v.first);
        }
    }
    return s_cc;
}

unordered_set<long> Coarse_Cell::compute_s_leaves() {
    /**
     * We look for vertices of degree one (or zero) in the cc.
     * :return: set of leaves:
     */
    unordered_set<long> s_leaves;

    for (const long &i_fc: (*this).__s_fc) {
//        long i_fc = i_k_v.first;
        if ((*this).__d_def.count(i_fc) == 0) {
            s_leaves.insert(i_fc);
        } else {
            unsigned short int nb_common_faces_i_fc = (*this).__d_def[i_fc].size();

            // if a cc composed of only one fc then the fc is a leaf : nb_common_faces_i_fc==0
            if (nb_common_faces_i_fc < 2) {
                s_leaves.insert(i_fc);
            }
        }
    }

    return s_leaves;
}

void Coarse_Cell::add_fc(unordered_set<long> s_fc_to_add,
                         vector<long> fc_2_cc) {

    if (!s_fc_to_add.empty()) {

        // Update of cardinal
        __card += s_fc_to_add.size();

        // Attention, fc_2_cc has not yet been updated:
        // we want the old i_cc associated with each fc to add
        __is_connectivity_up_to_date = false;

        // i_fc are new fc to add
        for (const long &i_fc : s_fc_to_add) {
           // We add the additional cell
            __s_fc.insert(i_fc);
           // We get the neighbours of the fine cell
            vector<long> v_neighbours = (*__fc_graph).get_neighbours(i_fc);
            vector<double> v_weights = (*__fc_graph).get_weights(i_fc);
            assert(v_neighbours.size() == v_weights.size());
            // We add the volume to the volumes
            volume += __fc_graph->_volumes[i_fc];
            for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {
                long i_fc_n = v_neighbours[i_n];
                double i_w_fc_n = v_weights[i_n];
                if (__s_fc.count(i_fc_n) > 0) {
                    // if i_fc_n in s_def:
                    // i_fc_n are fc already in current cc.
                    if (i_fc == i_fc_n) {
                        // Here we put boundary information inside a variable and remove it of d_def: which contains so
                        // only real neighbour
                        __boundary_area += i_w_fc_n;
                    } else {

                        // update of __d_def
                        __d_def[i_fc][i_fc_n] = i_w_fc_n;
                        __d_def[i_fc_n][i_fc] = i_w_fc_n;

                        // update of compactness
                        if (s_fc_to_add.count(i_fc_n) == 0) {
                            unsigned short int old_compactness = __d_def[i_fc_n].size() - 1;
                            unsigned short int new_compactness = __d_def[i_fc_n].size();

                            __d_compactness_to_s_fc[old_compactness].erase(i_fc_n);
                            if (__d_compactness_to_s_fc[old_compactness].empty()) {
                                __d_compactness_to_s_fc.erase(old_compactness);
                            }
                            if (__d_compactness_to_s_fc.count(new_compactness) > 0) {
                                __d_compactness_to_s_fc[new_compactness].insert(i_fc_n);
                            } else {
                                __d_compactness_to_s_fc[new_compactness] = unordered_set<long>({i_fc_n});
                            }
                        }
                        if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc_n) == 0)

                            assert(__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc_n) > 0);
                        long &j_cc = fc_2_cc[i_fc]; 
                        __delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc_n,
                                                                                                      j_cc,
                                                                                                      i_fc);
                        __delete_and_propagate_deletion__tmp_fc_fine_cut_edges(i_fc_n, i_fc);
                    }
                } else {
                    __tmp_fc_fine_cut_edges[i_fc][i_fc_n] = i_w_fc_n;
                    long &j_cc_n = fc_2_cc[i_fc_n];  // We want the old cc index of i_fc
                    __add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, j_cc_n, i_fc_n,
                                                                           i_w_fc_n);
                }
            }
        }

        // Update of compactness for new fc
        //////////////////////////////////////////////////////////////////////
        for (const long &i_fc : s_fc_to_add) {
            unsigned short int compactness = 0;
            if (__d_def.count(i_fc) > 0) {
                compactness = __d_def[i_fc].size();
            }
            if (__d_compactness_to_s_fc.count(compactness) > 0) {
                __d_compactness_to_s_fc[compactness].insert(i_fc);
            } else {
                __d_compactness_to_s_fc[compactness] = unordered_set<long>({i_fc});
            }

        }

        __compute_compactness();
        // TODO ne pas tout recalculer.
        __compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour();
    }
}

void Coarse_Cell::__delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(const long &i_fc,
                                                                                        const long &j_cc,
                                                                                        const long &j_fc) {
    __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].erase(j_fc);
    if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].empty()) {
        __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].erase(j_cc);
    }
    if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].empty()) {
        __d_i_fc_to_j_cc_neighbourhood_to_j_fc.erase(i_fc);
    }

}

void Coarse_Cell::__delete_and_propagate_deletion__tmp_fc_fine_cut_edges(const long &i_fc,
                                                                         const long &j_fc) {

    __tmp_fc_fine_cut_edges[i_fc].erase(j_fc);
    if (__tmp_fc_fine_cut_edges[i_fc].empty()) {
        __tmp_fc_fine_cut_edges.erase(i_fc);
    }
}


unordered_set<long> Coarse_Cell::get_s_fc_w_outer_neighbours(unsigned short int min_degree) {
    unordered_set<long> s_fc;
    for (auto &i_k_v : (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour) {
        unsigned short int i_d = i_k_v.first;
        if (i_d >= min_degree) {
            for (auto &j_k_v : (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_d]) {
//                set((*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_d].keys()));
                s_fc.insert(j_k_v.first);
            }

        }
    }
    return s_fc;
}

//// KARIM:
bool Coarse_Cell::check_consistency(vector<long> &fc_2_cc) {

/**
 * check_consistency between __d_i_fc_to_j_cc_neighbourhood_to_j_fc and d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour
 */

    for (auto &i_k_v: __d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
        long i_fc = i_k_v.first;
        for (auto &j_k_v: __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]) {
            long j_cc = j_k_v.first;
            if (fc_2_cc.size() > 0) {
                for (auto &k_k_v: __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc]) {
                    long j_fc = k_k_v.first;
                    if (fc_2_cc[j_fc] != j_cc) {
                        cerr << "\t\t\t\ti_fc " << i_fc << "j_fc" << j_fc << "j_cc" << j_cc << "fc_2_cc" << fc_2_cc[j_fc] << endl;
                    }
                    assert(fc_2_cc[j_fc] == j_cc);
                }
            }
            long nb_edges = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].size();
            assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(nb_edges) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end());
            // assert nb in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour

            assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_edges].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_edges].end());
            // assert i_fc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb]

            assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_edges][i_fc].find(j_cc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_edges][i_fc].end());
            // assert j_cc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb][i_fc]
        }
    }

    for (auto &i_degree_k_v : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour) {
        unsigned short int i_degree = i_degree_k_v.first;
        for (auto &i_k_v : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree]) {
            long i_fc = i_k_v.first;
            for (auto &j_cc : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc]) {
                if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc.find(i_fc) == __d_i_fc_to_j_cc_neighbourhood_to_j_fc.end()) {
                    // i_fc not in self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc
                    cout << "check_consistency a :\t i_fc : " << i_fc << " j_cc " << j_cc << endl;
                    return false;
                } else if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].find(j_cc) == __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].end()) {
                    //if j_cc not in self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]:
                    cout << "check_consistency b :\t i_fc : " << i_fc << " j_cc " << j_cc << endl;
                    return false;
                } else if (i_degree != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].size()) {
                    //if i_degree != len(self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc]):
                    cout << "check_consistency c :\t i_fc : " << i_fc << " j_cc " << j_cc << endl;
                    return false;
                } else {
                    return true;
                }
            }
        }
    }
    return true;
}


void Coarse_Cell::__remove_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(unsigned short int nb_fine_edges_wrt_cc, long i_fc, long i_cc_old) {
    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(nb_fine_edges_wrt_cc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end());
    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].end());
    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].find(i_cc_old) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].end());

    d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].erase(i_cc_old);

    if (nb_fine_edges_wrt_cc > 1) {
        unsigned short int new_nb = nb_fine_edges_wrt_cc - 1;
        // we add the fact that  i_cc_old has still new_nb edges with current cc
        if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(new_nb) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end()) {
            if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb].end())
                d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].insert(i_cc_old);
            else
                d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc] = {i_cc_old};
        } else {
            d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc]; // = {i_fc : {i_cc_old}};
            d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].insert(i_cc_old);
        }
    }

    if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].size() == 0) {
        d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].erase(i_fc);
        if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].empty()) {
            d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.erase(nb_fine_edges_wrt_cc);
        }
    }

}


void Coarse_Cell::__add_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(unsigned short int nb_fine_edges_wrt_cc, long i_fc, long i_cc) {

    if (nb_fine_edges_wrt_cc > 0) {
        // Checks
        assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(nb_fine_edges_wrt_cc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end());
        assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].end());
        assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].find(i_cc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].end());


        d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].erase(i_cc);
        if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].empty()) {
            d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].erase(i_fc);
            if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].empty())
                d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.erase(nb_fine_edges_wrt_cc);
        }
    } else
        assert(nb_fine_edges_wrt_cc == 0);

    unsigned short int new_nb = nb_fine_edges_wrt_cc + 1;

    if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.count(new_nb) > 0) {
        if (d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb].count(i_fc) > 0) {
            assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].count(i_cc) == 0);
            d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].insert(i_cc);
        } else
            d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc] = {i_cc};
    } else {
        d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb];
        d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc];
        d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].insert(i_cc);
    }
}

void Coarse_Cell::update_cc_neighbour(long i_fc, long i_fc_n, long i_cc_old, long i_cc_new) {
    unsigned short int nb_fine_edges_wrt_cc_old = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old].size();
    unsigned short int nb_fine_edges_wrt_cc_new(0);

    if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].find(i_cc_new) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].end())
        nb_fine_edges_wrt_cc_new = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_new].size();

    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(nb_fine_edges_wrt_cc_old) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end());
    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old].end());
    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old][i_fc].find(i_cc_old) !=
           d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old][i_fc].end());


    __remove_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(nb_fine_edges_wrt_cc_old, i_fc, i_cc_old);
    __add_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(nb_fine_edges_wrt_cc_new, i_fc, i_cc_new);

    assert(__d_i_fc_to_j_cc_neighbourhood_to_j_fc.find(i_fc) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc.end());
    assert(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].find(i_cc_old) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].end());
    assert(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old].find(i_fc_n) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old].end());
    double area = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old][i_fc_n];
    __add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, i_cc_new, i_fc_n, area);
    __delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, i_cc_old, i_fc_n);

}

void Coarse_Cell::remove_fc(unordered_set<long> s_fc_to_remove, vector<long> fc_2_cc) {
    __is_connectivity_up_to_date = false;
    __is_cc_neighbourhood_build = false;

    __card -= s_fc_to_remove.size();

    // TODO usefull?
    unordered_set<long> s_modified_fc;
    // Update of __d_def;

    for (auto i_fc : s_fc_to_remove) {
        unsigned short int compactness = __d_def[i_fc].size();
        __d_compactness_to_s_fc[compactness].erase(i_fc);
        if (__d_compactness_to_s_fc[compactness].empty())
            __d_compactness_to_s_fc.erase(compactness);
    }

    for (const long &i_fc : s_fc_to_remove) {

        vector<long> v_neighbours = (*(*this).__fc_graph).get_neighbours(i_fc);
        vector<double> v_weights = (*(*this).__fc_graph).get_weights(i_fc);
        assert(v_neighbours.size() == v_weights.size());

        (*this).volume -= (*(*this).__fc_graph)._volumes[i_fc];
        //TODO remove this!
      //  unordered_set<long> s_tmp;
        for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {

            long i_fc_n = v_neighbours[i_n];
            double i_w_fc_n = v_weights[i_n];
            //TODO remove this!
           // if (s_tmp.count(i_fc_n) != 0) {
            //    continue;
           // }

            if ((*this).__s_fc.count(i_fc_n) > 0) {
                /* i_fc_n are fc already in current cc.
                   no need to take care of i_fc_n in s_fc_to_remove because they will be deleted later via del __d_def[i_fc]*/
                if (i_fc == i_fc_n) {
                    /* Here we remove boundary information inside a variable and remove it of d_def: which contains so
                       only real neighbour*/
                    __boundary_area -= i_w_fc_n;
                } else {
                    // TODO usefull?
                    if (s_fc_to_remove.count(i_fc_n) == 0) {
                        s_modified_fc.insert(i_fc_n);
                    }

                    // update __d_def
                    __d_def[i_fc].erase(i_fc_n);
                    __d_def[i_fc_n].erase(i_fc);
                    // update of compactness of i_fc_n
                    if (s_fc_to_remove.count(i_fc_n) == 0) {
                        // i_fc_n is in the current cc and will not be removed.
                        unsigned short int old_compactness_i_fc_n = __d_def[i_fc_n].size() + 1;
                        unsigned short int new_compactness_i_fc_n = __d_def[i_fc_n].size();

                        // update of __d_compactness_to_s_fc
                        __d_compactness_to_s_fc[old_compactness_i_fc_n].erase(i_fc_n);
                        if (__d_compactness_to_s_fc[old_compactness_i_fc_n].empty())
                            __d_compactness_to_s_fc.erase(old_compactness_i_fc_n);

                        if (__d_compactness_to_s_fc.count(new_compactness_i_fc_n) > 0)
                            __d_compactness_to_s_fc[new_compactness_i_fc_n].insert(i_fc_n);
                        else
                            __d_compactness_to_s_fc[new_compactness_i_fc_n] = {i_fc_n};
                    }

                    long j_cc = fc_2_cc[i_fc];

//                    if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc) > 0)
//                        __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc_n];

                    __add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc_n, j_cc, i_fc, i_w_fc_n);
                    (*this).__tmp_fc_fine_cut_edges[i_fc_n][i_fc] = i_w_fc_n;
                }
            } else {
                // update of d_i_fc_to_j_cc_neighbourhood_to_j_fc
                long j_cc_n = fc_2_cc[i_fc_n];
                __delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, j_cc_n, i_fc_n);
                (*this).__delete_and_propagate_deletion__tmp_fc_fine_cut_edges(i_fc, i_fc_n);
            }
            //TODO remove this!
           // s_tmp.insert(i_fc_n);
        }
        __d_def.erase(i_fc);
        __s_fc.erase(i_fc);
    }

    // Update of compactness
    for (const long &i_fc : s_modified_fc) {

        unsigned short int compactness = __d_def[i_fc].size();
        if (__d_compactness_to_s_fc.count(compactness) > 0)
            __d_compactness_to_s_fc[compactness].insert(i_fc);
        else
            __d_compactness_to_s_fc[compactness] = {i_fc};
    }
    __compute_compactness();
//    for (int i_comp = 0; i_comp < 4; i_comp++) {
//        if (__d_compactness_to_s_fc.count(i_comp) && !__d_compactness_to_s_fc[i_comp].empty()) {
//            __compactness = i_comp;
//            break;
//        }
//    }

    // TODO ne pas tout recalculer
    __compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour();
}

unordered_set<long> Coarse_Cell::get_s_fc() {
    unordered_set<long> s_fc;
    for (const long &i_fc : __s_fc)
        s_fc.insert(i_fc);

    return s_fc;
}

unordered_set<long> *Coarse_Cell::get_s_fc_v2() {
    unordered_set<long> *s_fc = new unordered_set<long>();
    for (const long &i_fc : __s_fc)
        (*s_fc).insert(i_fc);

    return s_fc;
}

unordered_map<long, unordered_map<long, unordered_map<long, double>>> Coarse_Cell::get_d_fc_w_outer_neighbours_j_cc_and_s_j_fc() {
    return __d_i_fc_to_j_cc_neighbourhood_to_j_fc;
}

void Coarse_Cell::update_icc(long new_i_cc) {
    __i_cc = new_i_cc;
}

void Coarse_Cell::update_cc_neighbour_renumbering(unordered_map<long, long> dict_old_cc_to_new_cc) {
    // print("Call of update_cc_neighbour_renumbering")
    for (auto &i_k_v : __d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
        long i_fc = i_k_v.first;
        // Useful to avoid collision: by bad luck a new i_cc can be the same as an old one
        vector<long> l_i_cc_sorted(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].size()); // conscious copy
        long i = 0;
        for (auto &j_k_v : __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]) {
            l_i_cc_sorted[i++] = j_k_v.first;
        }
        sort(l_i_cc_sorted.begin(), l_i_cc_sorted.end());

        for (const long &i_cc_old :l_i_cc_sorted) {
            if (dict_old_cc_to_new_cc.count(i_cc_old) > 0) {
                long i_cc_new = dict_old_cc_to_new_cc[i_cc_old];
                __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_new] = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old];
                __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].erase(i_cc_old);
            }
        }
    }
    for (auto& i_k_v :d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour) {

        unsigned short i_degree = i_k_v.first;
        for (auto& j_k_v :d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree]) {
            long i_fc = j_k_v.first;
            vector<long> l_i_cc_sorted(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc].size()); // conscious copy
            long i = 0;
            for (const long &j_cc : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc]) {
                l_i_cc_sorted[i++] = j_cc;
            }
            sort(l_i_cc_sorted.begin(), l_i_cc_sorted.end());
            for(const long& i_cc_old : l_i_cc_sorted){
                if (dict_old_cc_to_new_cc.count(i_cc_old) > 0) {
                    long i_cc_new = dict_old_cc_to_new_cc[i_cc_old];
                    d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc].erase(i_cc_old);
                    d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc].insert(i_cc_new);
                }
            }
        }


    }
}
