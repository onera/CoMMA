//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Coarse_Cell.h"
#include <climits>

Coarse_Cell::Coarse_Cell(Dual_Graph &fc_graph,
                         long i_cc,
                         unordered_set<long> &s_fc,
                         bool is_isotropic,
                         bool is_delayed) : __dim(fc_graph._dimension), __is_isotropic(is_isotropic), __is_delayed(is_delayed) {

    (*this).__fc_graph = &fc_graph;
    (*this).__card = s_fc.size();
    (*this).__compactness = (*this).__card;  //we initialize (*this).__compactness with a big value
    for (long i_fc : s_fc) {
        assert(i_fc < fc_graph.number_of_cells);

        (*this).__d_i_fc_to_volume[i_fc] = fc_graph._volumes[i_fc];
        (*this).volume += fc_graph._volumes[i_fc];
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
                    (*this).__boundary_area += i_w_fc_n;

                } else {
                    (*this).__d_def[i_fc][i_fc_n] = i_w_fc_n;
                }
            } else {
                (*this).__tmp_fc_fine_cut_edges[i_fc][i_fc_n] = i_w_fc_n;
            }
        }

        // __compactness is the min degree of a vertex inside a cc.
        assert (SHRT_MAX > (*this).__d_def[i_fc].size());
        unsigned short int tmp_degree = (*this).__d_def[i_fc].size();
        if (tmp_degree < (*this).__compactness) {
            (*this).__compactness = tmp_degree;
        }
        if ((*this).__d_compactness_to_s_fc.count(tmp_degree) > 0) {
            (*this).__d_compactness_to_s_fc[tmp_degree].insert(i_fc);
        } else {
            (*this).__d_compactness_to_s_fc[tmp_degree] = {i_fc};
        }


    }

    if ((*this).__compactness == (*this).__dim) {
        (*this)._is_finalized = true;
    }
    (*this).__i_cc = i_cc;
}

bool Coarse_Cell::is_connected() {

    if (not(*this).__is_connectivity_up_to_date) {
        (*this).__is_connected = (*this).__check_connectivity();
        (*this).__is_connectivity_up_to_date = true;
    }
    return (*this).__is_connected;

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
    assert((*this).__card < USHRT_MAX);

    if ((*this).__card <= 1) {
        return true;
    }

    //list_fc = list((*this).__d_def.keys())
    unsigned short int nb_fc = (*this).__d_def.size();
    long list_fc[nb_fc];
    unsigned short int i = 0;
    for (auto iKV:(*this).__d_def) {
        list_fc[i++] = iKV.first;
    }

    (*this).__is_already_connected.clear();
    (*this).__is_already_connected.insert(0);
    unordered_set<long> set_next = {list_fc[0]};

    unsigned short int nb_connected_cells = 1;
    unordered_map<long, unsigned short int> dict_global_to_local;

    for (unsigned short int i_loc_fc = 0; i_loc_fc < (*this).__card; i_loc_fc++) {
        dict_global_to_local[list_fc[i_loc_fc]] = i_loc_fc;
    }

    while (nb_connected_cells < (*this).__card) {

        if (!set_next.empty()) {
            // i_fc = set_next.pop()
            long i_fc = *set_next.begin();
            set_next.erase(set_next.begin());

            for (auto &i_k_v :(*this).__d_def[i_fc]) {

                long i_fc_n = i_k_v.first;
                if (dict_global_to_local.count(i_fc_n) > 0) {
                    if ((*this).__is_already_connected.count(dict_global_to_local[i_fc_n]) == 0) {
                        // dict_global_to_local[i_fc_n] not in (*this).__is_already_connected
                        set_next.insert(i_fc_n);
                        nb_connected_cells += 1;
                        (*this).__is_already_connected.insert(dict_global_to_local[i_fc_n]);
                    }
                }

            }


        } else {
            break;
        }

    }
    if (verbose and nb_connected_cells != (*this).__card) {
        cout << "\n set_fc_original: {";
        for (auto iKV:(*this).__d_def) {
            cout << iKV.first << ", ";
        }
        cout << "}\nis_already_connected: { ";
        for (auto i_fc:(*this).__is_already_connected) {
            cout << i_fc << ", ";
        }
        cout << "}\nnb_connected_cells = " << nb_connected_cells << " size of cc= " << (*this).__card << endl;
    }
    return (nb_connected_cells == (*this).__card);
}


void Coarse_Cell::fill_cc_neighbouring(vector<long> &fc_2_cc) {

    // TODO Important prevoir un update pour eviter de tout recalculer a chaque fois.

    if ((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc.empty()) {
        (*this).__is_cc_neighbourhood_build = true;
//        (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc = dict();
        if ((*this).__tmp_fc_fine_cut_edges.empty()) {
            unordered_set<long> s_fc;// = set((*this).__d_def.keys())

            for (auto &i_k_v :(*this).__d_def) {
                s_fc.insert(i_k_v.first);
            }
            for (const long &i_fc :s_fc) {
//                (*this).__tmp_fc_fine_cut_edges[i_fc] = dict();
                vector<long> v_neighbours = (*(*this).__fc_graph).get_neighbours(i_fc);
                vector<double> v_weights = (*(*this).__fc_graph).get_weights(i_fc);

                assert(v_neighbours.size() == v_weights.size());

                for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {
                    long i_fc_n = v_neighbours[i_n];
                    double i_w_fc_n = v_weights[i_n];

                    //                    for i_fc_n, i_w_fc_n in zip((*this).__fc_graph.get_neighbours(i_fc),
                    //                (*this).__fc_graph.get_weights(i_fc)):
                    if (s_fc.count(i_fc_n) == 0) {
                        (*this).__tmp_fc_fine_cut_edges[i_fc][i_fc_n] = i_w_fc_n;
                    }
                }
            }
        }
        for (auto &i_k_v :(*this).__tmp_fc_fine_cut_edges) {

            long i_fc = i_k_v.first;

//            if ((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc)==0){
////                (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc] = dict()
//            }



            for (auto &j_k_v : (*this).__tmp_fc_fine_cut_edges[i_fc]) {
                long j_fc = j_k_v.first;
                (*this).__add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc,
                                                                       fc_2_cc[j_fc],
                                                                       j_fc,
                                                                       (*this).__tmp_fc_fine_cut_edges[i_fc][j_fc]);
            }
            // remove dict entry if its empty:
            if ((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].empty()) {
                (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc.erase(i_fc);
            }
        }
        // computation of d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour
        (*this).__compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour();

// TODO we could theoretically delete (*this).__tmp_fc_fine_cut_edges but it break some test cases where there are
//  multiple calls to fill_cc_neighbouring.
// (*this).__tmp_fc_fine_cut_edges = None  // (*this).__tmp_fc_fine_cut_edges is no more useful.
    }

}

void Coarse_Cell::__add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(long i_fc, long j_cc, long j_fc, double j_fc_area) {
//    if(j_cc not in (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]){
//        (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc] = dict()
//
//    }
    assert((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].count(j_fc) == 0);
    (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc][j_fc] = j_fc_area;
}

void Coarse_Cell::__compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour() {

/**
 * Compute (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour from (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc
 */
    (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.clear();
    for (auto &i_k_v : (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
        long i_fc = i_k_v.first;
        for (auto &j_k_v : (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]) {
            long j_cc = j_k_v.first;
            unsigned short int nb_fine_edges_wrt_cc = (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].size();
//            if ((*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.count(nb_fine_edges_wrt_cc)){
////                (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc] = dict()
//            }
            if ((*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].count(i_fc) > 0) {
                (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].insert(j_cc);
            } else {
                (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc] = unordered_set<long>{j_cc};
            }
        }
    }
}

unordered_set<long> Coarse_Cell::get_s_cc_neighbours() {

    unordered_set<long> s_cc;
    for (auto &i_k_v : (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
//        cout<<"i_fc "<< i_k_v.first<<endl;
        for (auto &j_k_v : (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_k_v.first]) {
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
    for (auto &i_k_v : (*this).__d_def) {
        long i_fc = i_k_v.first;
        unsigned short int nb_common_faces_i_fc = (*this).__d_def[i_fc].size();
        assert(((*this).__d_def.size() == 1) || nb_common_faces_i_fc > 0);
        // if a cc composed of only one fc then the fc is a leaf : nb_common_faces_i_fc==0
        if (nb_common_faces_i_fc < 2) {
            s_leaves.insert(i_fc);
        }
    }
    return s_leaves;
}

void Coarse_Cell::add_fc(unordered_set<long> s_fc_to_add,
                         vector<long> fc_2_cc) {

    // attention, fc_2_cc has not yet been updated:
    // we want the old i_cc associated with each fc to add
    (*this).__is_connectivity_up_to_date = false;

    // Update of cardinal
    //////////////////////////////////////////////
    (*this).__card += s_fc_to_add.size();

    // i_fc are new fc to add
    for (const long &i_fc : s_fc_to_add) {

        // print("add_fc", i_fc)
        // (*this).__d_def[i_fc] = dict()  // subgraph describing the cc
        vector<long> v_neighbours = (*(*this).__fc_graph).get_neighbours(i_fc);
        vector<double> v_weights = (*(*this).__fc_graph).get_weights(i_fc);

        assert(v_neighbours.size() == v_weights.size());

        for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {
            long i_fc_n = v_neighbours[i_n];
            double i_w_fc_n = v_weights[i_n];

            //            for
            //    i_fc_n, i_w_fc_n
            //    in
            //    zip((*this).__fc_graph.get_neighbours(i_fc), (*this).__fc_graph.get_weights(i_fc)):
            //// if i_fc_n in s_def:
            if ((*this).__d_def.count(i_fc_n) > 0) {
                // i_fc_n are fc already in current cc.
                if (i_fc == i_fc_n) {
                    // Here we put boundary information inside a variable and remove it of d_def: which contains so
                    // only real neighbour
                    (*this).__boundary_area += i_w_fc_n;
                } else {

                    // update of __d_def
                    (*this).__d_def[i_fc][i_fc_n] = i_w_fc_n;
                    (*this).__d_def[i_fc_n][i_fc] = i_w_fc_n;

                    // update of compactness
                    if (s_fc_to_add.count(i_fc_n) == 0) {
                        unsigned short int old_compactness = (*this).__d_def[i_fc_n].size() - 1;
                        unsigned short int new_compactness = (*this).__d_def[i_fc_n].size();

                        (*this).__d_compactness_to_s_fc[old_compactness].erase(i_fc_n);
                        if ((*this).__d_compactness_to_s_fc[old_compactness].empty()) {
                            (*this).__d_compactness_to_s_fc.erase(old_compactness);
                        }
                        if ((*this).__d_compactness_to_s_fc.count(new_compactness) > 0) {
                            (*this).__d_compactness_to_s_fc[new_compactness].insert(i_fc_n);
                        } else {
                            (*this).__d_compactness_to_s_fc[new_compactness] = unordered_set<long>({i_fc_n});
                        }
                    }
                    // update of __d_i_fc_to_j_cc_neighbourhood_to_j_fc:
                    // i_fc_n must be at the "boundary of current cc.
//                    if((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc_n)==0){
//
//                    }

//                    print(i_fc_n, (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc)
//                    print('assert i_fc_n in (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc, str(i_fc_n)')
                    assert((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc_n) > 0);
//                    in(*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc, str(i_fc_n)
                    // il se peut que la cc precedente etait de taille 1.
                    long &j_cc = fc_2_cc[i_fc];  // We want the old cc index of i_fc
                    // print(i_fc_n, j_cc,i_fc)
                    (*this).__delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc_n,
                                                                                                  j_cc,
                                                                                                  i_fc);
                }
            } else {
                // update of d_i_fc_to_j_cc_neighbourhood_to_j_fc
                long &j_cc_n = fc_2_cc[i_fc_n];  // We want the old cc index of i_fc
//                if ((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc)){
//                (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc] = dict()
//                }
                (*this).__add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, j_cc_n, i_fc_n,
                                                                       i_w_fc_n);
            }
        }
    }

    // Update of compactness for new fc
    //////////////////////////////////////////////////////////////////////
    for (const long &i_fc : s_fc_to_add) {

        unsigned short int compactness = (*this).__d_def[i_fc].size();
        if ((*this).__d_compactness_to_s_fc.count(compactness)) {
            (*this).__d_compactness_to_s_fc[compactness].insert(i_fc);
        } else {
            (*this).__d_compactness_to_s_fc[compactness] = unordered_set<long>({i_fc});
        }
    }
    unsigned short int max_comp = 0;
    for (auto &i_k_v :(*this).__d_compactness_to_s_fc) {
        if (max_comp < i_k_v.first) {
            max_comp = i_k_v.first;
        }
    }
//    unsigned short int max_comp = max((*this).__d_compactness_to_s_fc.keys())

    for (unsigned short int i_comp = 0; i_comp < max(max_comp + 1, 4); i_comp++) {
        if ((*this).__d_compactness_to_s_fc.count(i_comp) > 0 && !(*this).__d_compactness_to_s_fc[i_comp].empty()) {
            (*this).__compactness = i_comp;
            break;
        }
    }

    // TODO ne pas tout recalculer.
    (*this).__compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour();
}


void Coarse_Cell::__delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(const long &i_fc,
                                                                                        const long &j_cc,
                                                                                        const long &j_fc) {
    // print("__delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc", i_fc, j_cc, j_fc)
    // print((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc])
    (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].erase(j_fc);
    if ((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].empty()) {
        (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].erase(j_cc);
    }
    if ((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].empty()) {
        (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc.erase(i_fc);
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

bool Coarse_Cell::check_consistency(vector<long> &fc_2_cc) {
/**
 * check_consistency between __d_i_fc_to_j_cc_neighbourhood_to_j_fc and d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour
 */

  for(auto &i_k_v: __d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
    long i_fc = i_k_v.first;
    for(auto &j_k_v: __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]) {
      long j_cc = j_k_v.first;
      if(fc_2_cc.size() > 0) {
	for(auto & k_k_v: __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc]) {
	  long j_fc = k_k_v.first;
	  if(fc_2_cc[j_fc] != j_cc) {
	    cout << "\t\t\t\ti_fc "<< i_fc << "j_fc" <<  j_fc << "j_cc" << j_cc << "fc_2_cc" << fc_2_cc[j_fc] << endl;
	  }
	  assert(fc_2_cc[j_fc] == j_cc);
	}
      }
      long nb = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].size();
      assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(nb) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end());
      assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb].end());
      assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb][i_fc].find(j_cc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb][i_fc].end());
    }
  }

  for(auto &i_degree_k_v : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour) {
    unsigned short int i_degree = i_degree_k_v.first;
    for(auto &i_k_v : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree]) {
      long i_fc = i_k_v.first;
      for(auto &j_cc : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc]) {
	if(__d_i_fc_to_j_cc_neighbourhood_to_j_fc.find(i_fc) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc.end()) {
	  cout << "i_fc : " << i_fc << " j_cc " << j_cc << endl; 
	  return false;
	}
	else if(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].find(j_cc) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].end()) {
	  cout << "i_fc : " << i_fc << " j_cc " << j_cc << endl;
	  return false;
	}
	else if(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].find(i_degree) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc].end()) {
	  cout << "i_fc : " << i_fc << " j_cc " << j_cc << endl;
	  return false;
	}
	else {
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

  if(nb_fine_edges_wrt_cc > 1) {
    unsigned short int new_nb = nb_fine_edges_wrt_cc - 1;
    // we add the fact that  i_cc_old has still new_nb edges with current cc
    if(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(new_nb) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end()) {
      if(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb].end())
	d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].insert(i_cc_old);
      else
	d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc] = {i_cc_old};
    }
    else {
      d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc]; // = {i_fc : {i_cc_old}};
      d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].insert(i_cc_old);
    }
  }

  if(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].size() == 0)
    d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].erase(i_fc);
}

/*
    def __remove_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(self, nb_fine_edges_wrt_cc, i_fc, i_cc_old):

        assert nb_fine_edges_wrt_cc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour
        assert i_fc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc]
        assert i_cc_old in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc]

        self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].remove(i_cc_old)

        if nb_fine_edges_wrt_cc > 1:
            new_nb = nb_fine_edges_wrt_cc - 1
            # we add the fact that  i_cc_old has still new_nb edges with current cc
            if new_nb in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour:
                if i_fc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb]:
                    # print(i_fc, self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb])
                    self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].add(i_cc_old)
                else:
                    self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc] = {i_cc_old}
            else:
                self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb] = {i_fc: {i_cc_old}}
        # else nothing to do

        if not self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc]:
            self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].pop(i_fc)
*/

void Coarse_Cell::__add_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(unsigned short int nb_fine_edges_wrt_cc, long i_fc, long i_cc) {
  // Checks
  if(nb_fine_edges_wrt_cc > 0) {
    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(nb_fine_edges_wrt_cc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end());
    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].end());
    assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].find(i_cc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].end());
    if(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].empty()) {
      d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].erase(i_fc);
      if(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].empty())
	d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.erase(nb_fine_edges_wrt_cc);
    }
  }
  else
    assert(nb_fine_edges_wrt_cc == 0);

  unsigned short int new_nb = nb_fine_edges_wrt_cc + 1;

  if(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.count(new_nb) > 0) {
    if(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb].count(i_fc) > 0) {
      assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].count(i_cc) > 0);
      d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].insert(i_cc);
    }
    else
      d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc] = {i_cc};
  }
  else {
    d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb];
    d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc];
    d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].insert(i_cc);
  }
}
/*
    def __add_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(self,
                                                                        nb_fine_edges_wrt_cc: int,
                                                                        i_fc: int,
                                                                        i_cc: int):

        # Checks
        if nb_fine_edges_wrt_cc > 0:
            assert nb_fine_edges_wrt_cc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour
            assert i_fc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc]
            assert i_cc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc]
            self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc].remove(i_cc)
            if not self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc][i_fc]:
                self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc].pop(i_fc)
                if not self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc]:
                    self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.pop(nb_fine_edges_wrt_cc)

        else:
            assert nb_fine_edges_wrt_cc == 0
        new_nb = nb_fine_edges_wrt_cc + 1
        if new_nb in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour:
            # assert i_fc not in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb]
            if i_fc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb]:
                assert i_cc not in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc]
                self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc].add(i_cc)
            else:
                self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb][i_fc] = {i_cc}
        else:
            self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[new_nb] = {i_fc: {i_cc}}
*/


void Coarse_Cell::update_cc_neighbour(long i_fc, long i_fc_n, long i_cc_old, long i_cc_new) {
    unsigned short int nb_fine_edges_wrt_cc_old = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old].size();
  unsigned short int nb_fine_edges_wrt_cc_new(0);

  if(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].find(i_cc_new) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].end())
    nb_fine_edges_wrt_cc_new = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_new].size();
  assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.find(nb_fine_edges_wrt_cc_old) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour.end());
  assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old].find(i_fc) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old].end());
  assert(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old][i_fc].find(i_cc_old) != d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old][i_fc].end());
  

  __remove_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(nb_fine_edges_wrt_cc_old, i_fc, i_cc_old);
  __add_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(nb_fine_edges_wrt_cc_new, i_fc, i_cc_new);
  
  assert(__d_i_fc_to_j_cc_neighbourhood_to_j_fc.find(i_fc) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc.end());
  assert(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].find(i_cc_old) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].end());
  assert(__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old].find(i_fc_n) != __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old].end());
  double area = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old][i_fc_n];
  __add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, i_cc_new, i_fc_n, area);
  
    
}

/*
    def update_cc_neighbour(self, i_fc, i_fc_n, i_cc_old, i_cc_new):
        """
        Useful for swap
        :param i_fc:
        :param i_fc_n:
        :param i_cc_old:
        :param i_cc_new:
        :return:
        """
        # print("Call of update_cc_neighbour", i_fc, i_fc_n, i_cc_old, i_cc_new)
        # print("Call of update_cc_neighbour", self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc)

        nb_fine_edges_wrt_cc_old = len(self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old])
        if i_cc_new in self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]:
            nb_fine_edges_wrt_cc_new = len(self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_new])
        else:
            nb_fine_edges_wrt_cc_new = 0
        assert nb_fine_edges_wrt_cc_old in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour
        assert i_fc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old]
        assert i_cc_old in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old][i_fc]
        # self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old][i_fc].remove(i_cc_old)
        self.__remove_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(nb_fine_edges_wrt_cc_old, i_fc,
                                                                                i_cc_old)
        self.__add_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(nb_fine_edges_wrt_cc_new, i_fc, i_cc_new)
        # self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb_fine_edges_wrt_cc_old][i_fc].add(i_cc_new)

        assert i_fc in self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc
        assert i_cc_old in self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]
        assert i_fc_n in self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old]
        area = self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old][i_fc_n]
        self.__add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, i_cc_new, i_fc_n, area)
        self.__delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, i_cc_old, i_fc_n)

        # assert self.__check_consistancy()
*/

void Coarse_Cell::update_cc_neighbour_renumbering(unordered_map<long,long> dict_old_cc_to_new_cc) {
  cout << "Call of update_cc_neighbour_renumbering" << endl;
  for(auto &i_fc_k_v : __d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
    long i_fc = i_fc_k_v.first;
    vector<long> l_i_cc_sorted;
    for(auto &i_cc_k_v : __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc])
      l_i_cc_sorted.push_back(i_cc_k_v.first);
    sort(l_i_cc_sorted.begin(), l_i_cc_sorted.end());
    for(auto & i_cc_old : l_i_cc_sorted) {
      if(dict_old_cc_to_new_cc.count(i_cc_old) > 0) {
	long i_cc_new = dict_old_cc_to_new_cc[i_cc_old];
	__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_new] = __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old];
	__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].erase(i_cc_old);
      }
    }    
  }

  for(auto &i_degree_k_v : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour) {
    unsigned short int i_degree = i_degree_k_v.first;
    for(auto &i_fc_k_v : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree]) {
      vector<long> l_i_cc_sorted;
      long i_fc = i_fc_k_v.first;
      for(auto& i_cc : d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc])
	l_i_cc_sorted.push_back(i_cc);
      sort(l_i_cc_sorted.begin(), l_i_cc_sorted.end());
      for(auto i_cc_old : l_i_cc_sorted) {
	if (dict_old_cc_to_new_cc.count(i_cc_old) > 0) {
	  long i_cc_new = dict_old_cc_to_new_cc[i_cc_old];
	  d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc].erase(i_cc_old);
	  d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc].insert(i_cc_new);
	}
      }
    }
  }
}

/*
    def update_cc_neighbour_renumbering(self, dict_old_cc_to_new_cc: Dict[int, int]):

        # print("Call of update_cc_neighbour_renumbering")
        for i_fc in self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc:
            # Useful to avoid collision: by bad luck a new i_cc can be the same as an old one
            l_i_cc_sorted = list(self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc])
            l_i_cc_sorted.sort()
            for i_cc_old in l_i_cc_sorted:  # conscious copy
                if i_cc_old in dict_old_cc_to_new_cc:
                    i_cc_new = dict_old_cc_to_new_cc[i_cc_old]
                    self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_new] = \
                        self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][i_cc_old]
                    self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc].pop(i_cc_old)

        for i_degree in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour:
            for i_fc in self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree]:
                l_i_cc_sorted = list(self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc])
                l_i_cc_sorted.sort()
                for i_cc_old in l_i_cc_sorted:  # conscious copy
                    if i_cc_old in dict_old_cc_to_new_cc:
                        i_cc_new = dict_old_cc_to_new_cc[i_cc_old]
                        self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc].remove(i_cc_old)
                        self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc].add(i_cc_new)

*/

void Coarse_Cell::remove_fc(unordered_set<long> s_fc_to_remove, vector<long> fc_2_cc) {
  __is_connectivity_up_to_date = false;
  __is_cc_neighbourhood_build = false;

  __card -= s_fc_to_remove.size();

  // TODO usefull?
  unordered_set<long> s_modified_fc;
  // Update of __d_def;

  for(auto i_fc : s_fc_to_remove) {
    unsigned short int compactness = __d_def[i_fc].size();
    __d_compactness_to_s_fc[compactness].erase(i_fc);
    if(__d_compactness_to_s_fc[compactness].empty())
      __d_compactness_to_s_fc.erase(compactness);
  }

  for(auto i_fc : s_fc_to_remove) {
    auto i_fc_n_ptr = __fc_graph->get_neighbours(i_fc).begin();
    auto i_w_fc_n_ptr = __fc_graph->get_weights(i_fc).begin();
    for(;i_fc_n_ptr != __fc_graph->get_neighbours(i_fc).end() && i_w_fc_n_ptr != __fc_graph->get_weights(i_fc).end() ; i_w_fc_n_ptr++, i_fc_n_ptr++) {
      long i_fc_n = *i_fc_n_ptr;
      long i_w_fc_n = * i_w_fc_n_ptr;
      if(__d_def.count(i_fc_n)) {
	/* i_fc_n are fc already in current cc.
	   no need to take care of i_fc_n in s_fc_to_remove because they will be deleted later via del __d_def[i_fc]*/
	if (i_fc == i_fc_n) {
	  /* Here we remove boundary information inside a variable and remove it of d_def: which contains so
	     only real neighbour*/
	  __boundary_area -= i_w_fc_n;
	}
	else {
	  // TODO usefull?
	  if(s_fc_to_remove.count(i_fc_n) > 0)
	    s_modified_fc.insert(i_fc_n);

	  // update __d_def
	  __d_def[i_fc].erase(i_fc_n);
	  __d_def[i_fc_n].erase(i_fc);
	  // update of compactness of i_fc_n
	  if(s_fc_to_remove.count(i_fc) > 0) {
	    // i_fc_n is in the current cc and will not be removed.
	    unsigned short int old_compactness_i_fc_n = __d_def[i_fc_n].size() + 1;
	    unsigned short int new_compactness_i_fc_n = __d_def[i_fc_n].size();

	    // update of __d_compactness_to_s_fc
	    __d_compactness_to_s_fc[old_compactness_i_fc_n].erase(i_fc_n);
	    if(!__d_compactness_to_s_fc[old_compactness_i_fc_n].empty())
	      __d_compactness_to_s_fc.erase(old_compactness_i_fc_n);

	    if(__d_compactness_to_s_fc.count(new_compactness_i_fc_n) > 0)
	      __d_compactness_to_s_fc[new_compactness_i_fc_n].insert(i_fc_n);
	    else
	      __d_compactness_to_s_fc[new_compactness_i_fc_n] = {i_fc_n};
	  }

	  long j_cc = fc_2_cc[i_fc];

	  if (__d_i_fc_to_j_cc_neighbourhood_to_j_fc.count(i_fc) > 0)
	    __d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc_n];

	  __add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc_n, j_cc, i_fc, i_w_fc_n);

	}
      }
      else {
	// update of d_i_fc_to_j_cc_neighbourhood_to_j_fc
	long j_cc_n = fc_2_cc[i_fc_n];
	__delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc, j_cc_n, i_fc_n);
      }
    }
    __d_def.erase(i_fc);
  }

  // Update of compactness

  for(auto & i_fc : s_modified_fc) {
    unsigned short int compactness = __d_def[i_fc].size();
    if(__d_compactness_to_s_fc.count(compactness) > 0)
      __d_compactness_to_s_fc[compactness].insert(i_fc);
    else
      __d_compactness_to_s_fc[compactness] = {i_fc};
  }

  for(int i_comp = 0 ; i_comp < 4 ; i_comp++) {
    if(__d_compactness_to_s_fc.count(i_comp) && !__d_compactness_to_s_fc[i_comp].empty()) {
      __compactness = i_comp;
      break;
    }
  }

  // TODO ne pas tout recalculer
  __compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour();
}

/*
    def remove_fc(self, s_fc_to_remove: Set[int],
                  fc_2_cc: Optional[np.ndarray] = None):
        # TODO tester volume and boundary
        self.__is_connectivity_up_to_date = False
        self.__is_cc_neighbourhood_build = False

        # Update of cardinal
        #######################
        self.card -= len(s_fc_to_remove)

        # TODO usefull?
        s_modified_fc = set()
        # Update of self.__d_def
        #######################
        # i_fc are a fc to remove
        for i_fc in s_fc_to_remove:
            compactness = len(self.__d_def[i_fc])
            self.__d_compactness_to_s_fc[compactness].remove(i_fc)
            if not self.__d_compactness_to_s_fc[compactness]:
                self.__d_compactness_to_s_fc.pop(compactness)

        for i_fc in s_fc_to_remove:

            for i_fc_n, i_w_fc_n in zip(self.__fc_graph.get_neighbours(i_fc), self.__fc_graph.get_weights(i_fc)):
                if i_fc_n in self.__d_def:
                    # i_fc_n are fc already in current cc.
                    # no need to take care of i_fc_n in s_fc_to_remove because they will be deleted later via del self.__d_def[i_fc]
                    if i_fc == i_fc_n:
                        # Here we remove boundary information inside a variable and remove it of d_def: which contains so
                        # only real neighbour
                        self.__boundary_area -= i_w_fc_n
                    else:

                        # TODO usefull?
                        if i_fc_n not in s_fc_to_remove:
                            s_modified_fc.add(i_fc_n)

                        # update of __d_def
                        del self.__d_def[i_fc][i_fc_n]
                        del self.__d_def[i_fc_n][i_fc]

                        # update of compactness of i_fc_n
                        if i_fc_n not in s_fc_to_remove:
                            # i_fc_n is in the current cc and will not be removed.
                            old_compactness_i_fc_n = len(self.__d_def[i_fc_n]) + 1
                            new_compactness_i_fc_n = len(self.__d_def[i_fc_n])

                            # update of self.__d_compactness_to_s_fc
                            self.__d_compactness_to_s_fc[old_compactness_i_fc_n].remove(i_fc_n)
                            if not self.__d_compactness_to_s_fc[old_compactness_i_fc_n]:
                                self.__d_compactness_to_s_fc.pop(old_compactness_i_fc_n)
                            if new_compactness_i_fc_n in self.__d_compactness_to_s_fc:
                                self.__d_compactness_to_s_fc[new_compactness_i_fc_n].add(i_fc_n)
                            else:
                                self.__d_compactness_to_s_fc[new_compactness_i_fc_n] = {i_fc_n}

                        j_cc = fc_2_cc[i_fc]  # We want the old cc index of i_fc

                        if i_fc_n not in self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc:
                            self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc_n] = dict()

                        self.__add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc_n,
                                                                            j_cc,
                                                                            i_fc,
                                                                            i_w_fc_n)
                else:
                    # update of d_i_fc_to_j_cc_neighbourhood_to_j_fc
                    j_cc_n = fc_2_cc[i_fc_n]  # We want the old cc index of i_fc
                    self.__delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(i_fc,
                                                                                               j_cc_n,
                                                                                               i_fc_n)
            # we delete the entry i_fc of the graph.
            del self.__d_def[i_fc]

        # Update of compactness
        #######################
        for i_fc in s_modified_fc:

            compactness = len(self.__d_def[i_fc])
            if compactness in self.__d_compactness_to_s_fc:
                self.__d_compactness_to_s_fc[compactness].add(i_fc)
            else:
                self.__d_compactness_to_s_fc[compactness] = {i_fc}

        for i_comp in range(4):
            if i_comp in self.__d_compactness_to_s_fc and self.__d_compactness_to_s_fc[i_comp]:
                self.__compactness = i_comp
                break

        # TODO ne pas tout recalculer.
        self.__compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour()
*/

unordered_set<long> Coarse_Cell::get_s_fc() {
  unordered_set<long> s_fc;
  for(auto &k_v : __d_def)
    s_fc.insert(k_v.first);
  return s_fc;
}

/*
    def get_s_fc(self) -> Set[int]:
        return set(self.__d_def.keys())
*/

unordered_map<long, unordered_map<long, unordered_map<long, double>>> Coarse_Cell::get_d_fc_w_outer_neighbours_j_cc_and_s_j_fc() {
  return __d_i_fc_to_j_cc_neighbourhood_to_j_fc;
}

/*
    def get_d_fc_w_outer_neighbours_j_cc_and_s_j_fc(self) -> Dict[int, Dict[int, Dict[int, float]]]:
        return self.__d_i_fc_to_j_cc_neighbourhood_to_j_fc
*/
