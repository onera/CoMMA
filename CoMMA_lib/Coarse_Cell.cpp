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
    return true;
//    for (auto &i_k_v:(*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc) {
//        long i_fc = i_k_v.first;
//        for (auto &j_k_v:(*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]) {
//            long j_cc = j_k_v.first;
//
//            if (fc_2_cc.size()>0) {
//                for (auto &k_k_v:(*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc]) {
//                    long j_fc= k_k_v.first;
//                    if(fc_2_cc[j_fc] != j_cc){
//                        cout<<"\t\t\t\ti_fc "<< i_fc<< "j_fc"<< j_fc<< "j_cc"<< j_cc<< "fc_2_cc"<< fc_2_cc[j_fc]);
//                    }
//                    assert(fc_2_cc[j_fc] == j_cc);
//                }
//            }
//            nb = len((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc])
//// if nb not in (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour:
//// print("\n" + str(i_fc) + "\t" + str(j_cc) + "\t" + str(nb))
//            assert
//                    nb
//            in
//            (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, str(i_fc) + "\t" + str(
//                    j_cc) + "\t" + str(nb)
//            assert
//                    i_fc
//            in
//            (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb]
//            // if j_cc not in (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb][i_fc]:
//            //     print("\ni_cc", (*this).__i_cc)
//            //     print(nb, i_fc, j_cc, (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour,  (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc])
//            assert
//                    j_cc
//            in
//            (*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[nb][i_fc]
//        }
//    }
//    for
//    i_degree
//    in(*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour:
//    for
//    i_fc
//    in(*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree]:
//    for
//    j_cc
//    in(*this).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour[i_degree][i_fc]:
//    if i_fc
//        not in(*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc:
//    print(i_fc, j_cc, i_fc, (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc)
//    return False
//    elif
//            j_cc
//    not in(*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc]:
//    print(i_fc, j_cc, i_fc, (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc])
//    return False
//    elif
//    i_degree != len((*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc][j_cc]):
//    print(i_fc, j_cc, i_fc, (*this).__d_i_fc_to_j_cc_neighbourhood_to_j_fc[i_fc], i_degree)
//    return False
//    else:
//    return True
//    return True

}