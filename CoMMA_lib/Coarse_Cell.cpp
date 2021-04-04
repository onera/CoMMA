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

//        (*this).__d_def[i_fc] = dict()  # subgraph describing the cc
//        (*this).__tmp_fc_fine_cut_edges[i_fc] = dict()

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

            for (auto  i_k_v :(*this).__d_def[i_fc]) {

                long i_fc_n=i_k_v.first;
                if(dict_global_to_local.count(i_fc_n)>0)
                {
                    if((*this).__is_already_connected.count(dict_global_to_local[i_fc_n])==0) {
                        // dict_global_to_local[i_fc_n] not in (*this).__is_already_connected
                        set_next.insert(i_fc_n);
                        nb_connected_cells += 1;
                        (*this).__is_already_connected.insert(dict_global_to_local[i_fc_n]);
                    }
                }

            }


        }
        else{
            break;
        }

    }
    if (verbose and nb_connected_cells != (*this).__card) {
        cout << "\n set_fc_original: {";
        for (auto iKV:(*this).__d_def) {
            cout<<iKV.first<<", ";
        }
        cout<<"}\nis_already_connected: { ";
        for (auto i_fc:(*this).__is_already_connected) {
            cout<<i_fc<<", ";
        }
        cout<<"}\nnb_connected_cells = "<<nb_connected_cells<< " size of cc= "<<(*this).__card<<endl;
    }
    return (nb_connected_cells == (*this).__card);
}
