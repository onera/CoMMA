//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Coarse_Cell_Graph.h"

Coarse_Cell_Graph::Coarse_Cell_Graph(Dual_Graph &fc_graph,
                                     int verbose,
                                     vector<long> debug_only_fc_to_cc,
                                     forward_list<deque<long> *> *debug_a_anisotropic_lines)
        : _fc_graph(fc_graph), _verbose(verbose) {
    //==================
    // Input datas
    //==================
    vector<bool> tmp(fc_graph.number_of_cells, false);
    (*this)._a_is_fc_agglomerated = tmp;

    //==================
    // temporary fields
    //==================
    (*this)._fc_2_cc = vector<long>(fc_graph.number_of_cells, -1);
    if (!debug_only_fc_to_cc.empty()) {


        if (((*this)._verbose > 0) && (debug_only_fc_to_cc.size() != (*this)._fc_2_cc.size())) {
            cerr << "mismatched sizes of debug_only_fc_to_cc " << debug_only_fc_to_cc.size() << " and of (*this)._fc_2_cc " << (*this)._fc_2_cc.size() << endl;
        }
        assert(debug_only_fc_to_cc.size() == (*this)._fc_2_cc.size());
        unordered_map<long, unordered_set<long>> d_cc;
        long i_fc = 0;
        long nb_cc = 0;  //computes the max of i_cc
        for (const long &i_cc:debug_only_fc_to_cc) {
            if (nb_cc < i_cc) {
                nb_cc = i_cc;
            }

            if (d_cc.count(i_cc) > 0) {
                d_cc[i_cc].insert(i_fc);
            } else {
                d_cc[i_cc] = unordered_set<long>({i_fc});
            }
        }

        unordered_set<long> s_anisotropic_cc;
        if (debug_a_anisotropic_lines != NULL) {

            for (auto &i_l :(*debug_a_anisotropic_lines)) {
                for (const long &i_cc :(*i_l)) {
                    s_anisotropic_cc.insert(i_cc);
                }
            }
        }
        nb_cc++;
        //Creation of Coarse cells:
        for (long i_cc = 0; i_cc < nb_cc; i_cc++) {
            (*this).cc_create_a_cc(d_cc[i_cc], s_anisotropic_cc.count(i_cc) > 0);
        }
        (*this)._fc_2_cc = debug_only_fc_to_cc;
        (*this).fill_cc_neighbouring();
//        np.copyto((*this)._fc_2_cc, debug_only_fc_to_cc)
    }
}

long Coarse_Cell_Graph::cc_create_a_cc(const unordered_set<long> &s_fc,
                                       bool is_anisotropic,
                                       bool is_creation_delayed) {

// print("cc_create_a_cc", s_fc, "is_creation_delayed", is_creation_delayed)
    assert((!is_anisotropic) || (!is_creation_delayed));

    for (const long &i_fc :s_fc) {
        if ((*this)._fc_2_cc[i_fc] != -1) {
            cerr << "fc " << i_fc << " is already assigned to i_cc " << (*this)._fc_2_cc[i_fc] << endl;
        }
        assert((*this)._fc_2_cc[i_fc] == -1);
    }

    bool is_mutable = true;
    if (is_anisotropic) {

        assert(!is_creation_delayed);
        (*this)._d_anisotropic_cc[(*this)._cc_counter] = unordered_set<long>(s_fc);  //TODO check there is a copy???

        is_mutable = false;
    }

    if (!is_creation_delayed) {
        // We create the cc right now.
        // Everything is updated:
//

        // if is_mutable: dict_cc, dict_card_cc, dict_compactness_2_cc
//
        // in both case: number_of_fine_agglomerated_cells_tmp, isFineCellAgglomerated_tmp, _fc_2_cc,
        if (is_mutable) {
            // the cell can be modified afterwards and is thus defined in dict_cc and dict_card_cc, dict_compactness_2_cc, dict_cc_to_compactness
            // Update of dict_cc:
            //###################################
            // (*this)._d_isotropic_cc[(*this)._cc_counter] = new_cc = Coarse_Cell_v1((*this)._fc_graph, (*this)._cc_counter,
            //                                                                    s_fc)
            Coarse_Cell *new_cc = new Coarse_Cell((*this)._fc_graph, (*this)._cc_counter, s_fc);
            (*this)._d_isotropic_cc[(*this)._cc_counter] = new_cc;

            // Update of dict_Card_Coarse_Cells:
            //###################################
            unsigned short int card = (*new_cc).__card;
            if ((*this)._d_card_2_cc.count(card) > 0) {
                //if card in (*this)._d_card_2_cc:
                (*this)._d_card_2_cc[card].insert((*this)._cc_counter);
            } else {
                (*this)._d_card_2_cc[card] = unordered_set<long>({(*this)._cc_counter});
            }

            // Update of compactness informations:
            //####################################
            // TODO peut-etre eviter de recalculer. On l'avait avant dans le choix de la CC.
            assert((*new_cc).is_connected());

            unsigned short int deg_compactness = (*new_cc).__compactness;  // (*this)._fc_graph.compute_min_fc_compactness_inside_a_cc(s_fc)
            if ((*this)._d_compactness_2_cc.count(deg_compactness) > 0) {
                //if deg_compactness in (*this)._d_compactness_2_cc:
                (*this)._d_compactness_2_cc[deg_compactness].insert((*this)._cc_counter);
            } else {
                (*this)._d_compactness_2_cc[deg_compactness] = unordered_set<long>({(*this)._cc_counter});
            }

        }

        // Update of _associatedCoarseCellNumber the output of the current function agglomerate
        for (const long &i_fc :s_fc) {
            // Only if not isCreationDelayed:
            // TODO not check this in production
            if ((*this)._fc_2_cc[i_fc] != -1) {
                cerr << "fc " << i_fc << " is Already assigned fine cell" << endl;
            }
            assert((*this)._fc_2_cc[i_fc] == -1);
            (*this)._fc_2_cc[i_fc] = (*this)._cc_counter;
        }
        // Update of the number of CC
        //####################################
        (*this)._cc_counter++; // Only if not isCreationDelayed
    } else {
        // We do not create the coarse cell yet.
        // As this coarse cell will be soon deleted, we want its coarse index to be the greater possible.
        // Only isFineCellAgglomerated_tmp, number_of_fine_agglomerated_cells_tmp and
        // dict_DistributionOfCardinalOfCoarseElements are modified!
        (*this)._delayed_cc.push_back(s_fc);
        // raise ValueError("TO DO")
    }

    for (const long &i_fc :s_fc) {
        if (!(*this)._a_is_fc_agglomerated[i_fc]) {
            // Rq: initialise to False pour chaque niveau dans agglomerate(...)
            (*this)._a_is_fc_agglomerated[i_fc] = true;
            (*this)._nb_of_agglomerated_fc++;
        }
    }
    return (*this)._cc_counter - 1;
}

void Coarse_Cell_Graph::fill_cc_neighbouring() {

    for (auto &i_k_v: (*this)._d_isotropic_cc) {
        (*i_k_v.second).fill_cc_neighbouring((*this)._fc_2_cc);
    }
}

unordered_map<long, unordered_set<long>> Coarse_Cell_Graph::get_d_cc_iso() {

    // Watch out that delayed_cc may not be empty , i.e. not yet created and numbered!
    unordered_map<long, unordered_set<long>> d;

    for (auto &i_k_v : _d_isotropic_cc) {
        long i_cc = i_k_v.first;
        unordered_set<long> tmp(i_k_v.second->get_s_fc());
        d[i_cc] = tmp;
    }
    return d;
}

unordered_map<long, unordered_set<long>> Coarse_Cell_Graph::get_d_cc_aniso() {

    // Watch out that delayed_cc may not be empty , i.e. not yet created and numbered!
    unordered_map<long, unordered_set<long>> d;

    for (auto &i_k_v : _d_anisotropic_cc) {
        long i_cc = i_k_v.first;
        d[i_cc] = i_k_v.second;
    }
    return d;
}

unordered_map<long, unordered_set<long>> Coarse_Cell_Graph::get_d_cc_all() {
    // assert not (*this)._delayed_cc`
    // Watch out that delayed_cc may not be empty , i.e. not yet created and numbered!
    unordered_map<long, unordered_set<long>> d;

    for (auto &i_k_v : _d_isotropic_cc) {
        long i_cc = i_k_v.first;
//        for (auto &i  : i_k_v.second->get_s_fc()) {
//            cout << i << ", ";
//        }
//        cout << endl;
        unordered_set<long> tmp(i_k_v.second->get_s_fc());
        d[i_cc] = tmp;
//        if (*d)[i_cc]==NULL;

//        (*d)[i_cc] = (*(* i_k_v.second).get_s_fc());
    }
    for (auto &i_k_v : _d_anisotropic_cc) {
        long i_cc = i_k_v.first;
        d[i_cc] = i_k_v.second;
    }
//    (*d).insert((*this)._d_anisotropic_cc.begin(), (*this)._d_anisotropic_cc.end());
//    (*d).merge((*this)._d_anisotropic_cc);
    return d;
}

unordered_map<unsigned short int, long> Coarse_Cell_Graph::get_d_distribution_of_cardinal_of_isotropic_cc() {

    unordered_map<unsigned short int, long> d_distr_card_cc;
    for (auto &i_k_v : _d_isotropic_cc) {
        long i_cc = i_k_v.first;
        unsigned short int card = _d_isotropic_cc[i_cc]->__card;
        if (d_distr_card_cc.count(card)) {
            d_distr_card_cc[card] += 1;
        } else {
            d_distr_card_cc[card] = 1;
        }
    }
    return d_distr_card_cc;
}

unsigned short int Coarse_Cell_Graph::get_cc_compactness(const long &i_cc) {
//    cout<< _d_isotropic_cc.count(i_cc)<<endl;

    if (_d_isotropic_cc.count(i_cc)) {
//        cout<<"True"<<endl;
        return (*_d_isotropic_cc[i_cc]).__compactness;
    } else if (_d_anisotropic_cc.count(i_cc)) {
//        cout<<"False"<<endl;
        unsigned short int c = _fc_graph.compute_min_fc_compactness_inside_a_cc(_d_anisotropic_cc[i_cc]);
        if (c != (unsigned short int) (_d_anisotropic_cc[i_cc].size() > 1)) {
            cerr << "get_cc_compactness: Warning anisotropic CC of compactness >1 " << c << endl;
        }
        return (unsigned short int) ((_d_anisotropic_cc[i_cc].size()) > 1);
    } else {
        //Delayed cc
        unsigned short int c = _fc_graph.compute_min_fc_compactness_inside_a_cc(_delayed_cc[i_cc]);
        return c;
    }
}

void Coarse_Cell_Graph::cc_create_all_delayed_cc() {
    for (const unordered_set<long> &s_fc: _delayed_cc) {
        cc_create_a_cc(s_fc);
    }
    _delayed_cc.clear();
}

void Coarse_Cell_Graph::__remove_an_isotropic_cc(const long &i_cc) {

/**
 * We remove the cc of index i_cc
 */
    unsigned short int cc_size = (*_d_isotropic_cc[i_cc]).__card;

    // a) remove the origin cc from _d_card_2_cc

    assert(_d_card_2_cc.count(cc_size));
    assert(_d_card_2_cc[cc_size].count(i_cc));

    unsigned short int cc_compactness = (*_d_isotropic_cc[i_cc]).__compactness;

    _d_card_2_cc[cc_size].erase(i_cc);
    if (_d_card_2_cc[cc_size].empty()) {

        // no more cells of size cc_size: we delete the entry
        _d_card_2_cc.erase(cc_size);
    }

    // b) remove the origin cc from _d_compactness_2_cc
    _d_compactness_2_cc[cc_compactness].erase(i_cc);
    if (_d_compactness_2_cc[cc_compactness].empty()) {
        // no more cells of size cc_size: we delete the entry
        _d_compactness_2_cc.erase(cc_compactness);
    }
}

unordered_set<long> Coarse_Cell_Graph::cc_swap_fc(unordered_set<long> &s_fc,
                                                  const long &i_origin_cc,
                                                  const long &i_destination_cc) {


/***
 * This function swaps the fine cell i_fc which belongs to i_origin_cc cc to i_destination_cc cc.
 * Connectivity is checked. if the swap leads to a disconnected cc, the cc is split
 *
 * :param s_fc: set of fine cells to swap
 * :param i_origin_cc: index of the cc containing initially the fine cells
 * :param i_destination_cc: index of the cc that will contain  the fine cells.
 * :return: the set of cc to remove
 *
 */
// TODO Verifier qu'on ne presuppose pas la connectivity!!!!

    // print("cc_swap_fc ", s_fc, i_origin_cc, i_destination_cc)
    if (!is_isotropic_cc(i_origin_cc) or !(is_isotropic_cc(i_destination_cc))) {
        cerr << "Swap with non isotropic CC: {";
        for (const long &i_fc: s_fc) {
            cerr << i_fc << ", ";
        }
        cerr << "} " << i_origin_cc << " " << i_destination_cc << endl;
    }
    assert (is_isotropic_cc(i_origin_cc));
    assert (is_isotropic_cc(i_destination_cc));

    Coarse_Cell *origin_cc = _d_isotropic_cc[i_origin_cc];
    Coarse_Cell *destination_cc = _d_isotropic_cc[i_destination_cc];
    for (const long &i_fc : s_fc) {
        assert((*origin_cc).get_s_fc().count(i_fc));
    }

    // if we remove all the fine cells from a cc this cc will be deleted.
    unordered_set<long> set_removed_cc;

    // 1) We remove i_origin_cc
    //==========================
    __remove_an_isotropic_cc(i_origin_cc);
    __remove_an_isotropic_cc(i_destination_cc);

    // 2) We update i_origin_cc
    //==========================

    (*destination_cc).add_fc(s_fc, _fc_2_cc);
    unordered_set<long> union_s_fc = (*destination_cc).get_s_fc();
    for (const long &i_fc : (*origin_cc).get_s_fc()) {
        union_s_fc.insert(i_fc);
    }
    for (const long &i_fc : s_fc) {
        // Initialisation of s_fc_to_update_i_fc:
        unordered_set<long> s_fc_to_update_i_fc;
        for (const long &i_fc_n : _fc_graph.get_neighbours(i_fc)) {
            if (union_s_fc.count(i_fc_n) == 0) {
                s_fc_to_update_i_fc.insert(i_fc_n);
            }
        }

        _cc_update_neighbours(i_fc,
                              s_fc_to_update_i_fc,
                              i_origin_cc,
                              i_destination_cc);
        _fc_2_cc[i_fc] = i_destination_cc;
    }
    (*origin_cc).remove_fc(s_fc, _fc_2_cc);

    if ((*origin_cc).__card == 0) {

        // The cc indexed i_origin_cc disapears.
        set_removed_cc.insert(i_origin_cc);
        _d_isotropic_cc.erase(i_origin_cc);
    } else {
        if ((*origin_cc).is_connected()) {
            // 3) We add i_origin_cc back with updated infos.
            //===============================================
            __add_a_cc(i_origin_cc);
        } else {
            // the modified cc is disconnected
            vector<unordered_set<long>> l_of_s_connected_component = _fc_graph.compute_connected_components((*origin_cc).get_s_fc());
            unsigned short int arg_max = 0;

            //Computation of max_length:
            unsigned short int max_length = 0;
            for (unsigned short i_v = 0; i_v < l_of_s_connected_component.size(); i_v++) {
                if (l_of_s_connected_component[i_v].size() >= max_length) {
                    max_length = l_of_s_connected_component[i_v].size();
                    arg_max = i_v;
                }
            }

            // We leave the biggest part inplace and create new cc for the rest.
            for (unsigned short i_v = 0; i_v < l_of_s_connected_component.size(); i_v++) {
                if (i_v != arg_max) {
                    // All the smallest connected components:
                    unordered_set<long> empty_set = unordered_set<long>({});
                    const long i_dest_cc = cc_create_a_cc(empty_set);
                    // Remark: i_dest_cc = _cc_counter - 1
                    // we remove arrays  referencing the new empty cc
                    __remove_an_isotropic_cc(i_dest_cc);
                    (*_d_isotropic_cc[i_dest_cc]).add_fc(l_of_s_connected_component[i_v], _fc_2_cc);
                    for (const long &i_fc: l_of_s_connected_component[i_v]) {

                        // Initialisation of s_fc_to_update_i_fc:
                        unordered_set<long> s_fc_to_update_i_fc;
                        for (const long &i_fc_n : _fc_graph.get_neighbours(i_fc)) {
                            if (l_of_s_connected_component[i_v].count(i_fc_n) == 0) {
                                s_fc_to_update_i_fc.insert(i_fc_n);
                            }
                        }

                        _cc_update_neighbours(i_fc,
                                              s_fc_to_update_i_fc,
                                              i_origin_cc,
                                              i_dest_cc);
                        // ATTENTION A CA
                        _fc_2_cc[i_fc] = i_dest_cc;
                    }

                    (*origin_cc).remove_fc(l_of_s_connected_component[i_v], _fc_2_cc);

                    __add_a_cc(i_dest_cc);
                }
            }

            __add_a_cc(i_origin_cc);

            // Check the split cell (cc_counter - 1)
            // TODO Remove this
            assert(is_isotropic_cc(_cc_counter - 1));
            assert((*_d_isotropic_cc[_cc_counter - 1]).is_connected());
            assert(_fc_graph.check_connectivity((*_d_isotropic_cc[_cc_counter - 1]).get_s_fc()));

            // Check the original coarse cell
            assert(is_isotropic_cc(i_origin_cc));
            assert((*_d_isotropic_cc[i_origin_cc]).is_connected());
            assert(_fc_graph.check_connectivity((*_d_isotropic_cc[i_origin_cc]).get_s_fc()));
        }
    }
    __add_a_cc(i_destination_cc);

    assert(set_removed_cc.size() <= 1);
    return set_removed_cc;

//    return unordered_set<long>();
}


void Coarse_Cell_Graph::_cc_update_neighbours(const long &i_fc,
                                              const unordered_set<long> &s_fc,
                                              const long &i_origin_cc,
                                              const long &i_dest_cc) {


/**
 * Due to swap_fc, we update cc informations(d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour and
 *                                          __d_i_fc_to_j_cc_neighbourhood_to_j_fc) in cc neighbours of i_fc.
 * :param i_fc: index of current fc.
 * :param s_fc: set of fc, neighbours of i_fc, that needs to be updated (i.e. not inside the current cc)
 * :param i_origin_cc: old number of cc that used to include i_fc
 * :param i_dest_cc: new number of cc that includes i_fc
 * :return: None
 */
    if (_verbose) {
        cerr << "_cc_update_neighbours " << i_fc << " {";
        for (const long &i: s_fc) {
            cerr << i << ", ";
        }
        cerr << "} " << i_origin_cc << " " << i_dest_cc << endl;
    }


    for (const long &i_fc_n: s_fc) {
        if (is_fc_agglomerated_in_isotropic_cc(i_fc_n)) {
            const long &i_cc_n = _fc_2_cc[i_fc_n];
            Coarse_Cell *cc_n = _d_isotropic_cc[i_cc_n];
            (*cc_n).update_cc_neighbour(i_fc_n, i_fc, i_origin_cc, i_dest_cc);
        }

    }
}

void Coarse_Cell_Graph::__add_a_cc(const long &i_cc) {
/**
 * Remark: _d_isotropic_cc[i_cc] has already been updated
 */
// a) we update data with the new size of the original cc.

    Coarse_Cell *cc = _d_isotropic_cc[i_cc];
    assert((*cc).is_connected());

    unsigned short new_cc_size = (*cc).__card;

    assert(new_cc_size != 0);

    // The origin cc still exists.
    if (_d_card_2_cc.count(new_cc_size)) {
        _d_card_2_cc[new_cc_size].insert(i_cc);
    } else {
        _d_card_2_cc[new_cc_size] = {i_cc};
    }

    // computation of the compactness of the updated cc:
    unsigned short deg_compactness = (*cc).__compactness;
    if (_d_compactness_2_cc.count(deg_compactness)) {
        _d_compactness_2_cc[deg_compactness].insert(i_cc);
    } else {
        _d_compactness_2_cc[deg_compactness] = {i_cc};
    }
}

void Coarse_Cell_Graph::cc_renumber() {
/**
 * Renumbering of all cc
 * _s_cc_to_remove is only useful to know the first index to be renumbered and the number of deleted cc.
 * :return: None
 */


    // ajouter le tableau des renumerotation
    // ancien-> nouveau.
    if (!_s_cc_to_remove.empty()) {

        // print("_s_cc_to_remove", _s_cc_to_remove)
        unordered_map<long, long> dict_old_cc_to_new_cc;
        long min_cc;
        long new_i_cc;
        if (!_d_isotropic_cc.empty()) {

            assert(__check_s_cc_to_remove_are_isotropic());

            // we look for the smallest index of cc to be removed.
            min_cc = _cc_counter;
            for (const long &i: _s_cc_to_remove) {
                if (min_cc > i) {
                    min_cc = i;
                }
            }

            new_i_cc = min_cc;
            for (long i_cc = min_cc + 1; i_cc < _cc_counter; i_cc++) {
                if (_d_isotropic_cc.count(i_cc)) {
                    // deal with _d_isotropic_cc
                    assert(_s_cc_to_remove.count(i_cc) == 0);
                    dict_old_cc_to_new_cc[i_cc] = new_i_cc;

                    _d_isotropic_cc[new_i_cc] = _d_isotropic_cc[i_cc];
                    Coarse_Cell *new_cc = _d_isotropic_cc[new_i_cc];
                    (*new_cc).update_icc(new_i_cc);
                    for (const long &i_fc :(*new_cc).get_s_fc()) {

                        assert(_fc_2_cc[i_fc] == i_cc);
                        _fc_2_cc[i_fc] = new_i_cc;
                    }

                    unsigned short i_cc_size = (*_d_isotropic_cc[i_cc]).__card;
                    _d_card_2_cc[i_cc_size].erase(i_cc);
                    _d_card_2_cc[i_cc_size].insert(new_i_cc);

                    unsigned short compactness = (*_d_isotropic_cc[i_cc]).__compactness;
                    _d_compactness_2_cc[compactness].erase(i_cc);
                    _d_compactness_2_cc[compactness].insert(new_i_cc);

                    _d_isotropic_cc.erase(i_cc);
                    // _d_isotropic_cc.pop(i_cc)
                    new_i_cc++;
                } else {
                    assert(_s_cc_to_remove.count(i_cc));
                }
            }
        } else if (!_d_anisotropic_cc.empty()) {

            // we look for the smallest index of cc to be removed.
            min_cc = _cc_counter;
            for (const long &i: _s_cc_to_remove) {
                if (min_cc > i) {
                    min_cc = i;
                }
            }
//            min_cc = min(_s_cc_to_remove)
            new_i_cc = min_cc;
            for (long i_cc = min_cc + 1; i_cc < _cc_counter; i_cc++) {

                if (_d_anisotropic_cc.count(i_cc)) {
                    dict_old_cc_to_new_cc[i_cc] = new_i_cc;
                    _d_anisotropic_cc[new_i_cc] = _d_anisotropic_cc[i_cc];
                    for (const long &i_fc :_d_anisotropic_cc[new_i_cc]) {
                        _fc_2_cc[i_fc] = new_i_cc;

                        _d_anisotropic_cc.erase(i_cc);
                        new_i_cc++;
                    }

                }
// new_i_cc = _cc_counter - len(_s_cc_to_remove)
            }
        } else {
            exit(1);
        }
        _update_cc_neighbour(min_cc, dict_old_cc_to_new_cc);

        _cc_counter -= _s_cc_to_remove.size();
        if (_cc_counter != new_i_cc) {
            cerr << "Problem consistency in number of Coarse cells " << _cc_counter << " " << new_i_cc << endl;
        }
        assert(_cc_counter == new_i_cc);

    }
    _s_cc_to_remove.clear();

}


bool Coarse_Cell_Graph::__check_s_cc_to_remove_are_isotropic() {

    for (const long &i_cc : _s_cc_to_remove) {
        if (_d_isotropic_cc.count(i_cc)) {
            // Check that _d_isotropic_cc[i_cc] is empty
            if (_d_isotropic_cc[i_cc] != NULL) {
                cerr << "Cc " << i_cc << " is not empty: {";
                for (const long &i: (*_d_isotropic_cc[i_cc]).get_s_fc()) {
                    cerr << i << ", ";
                }
                cerr << "}" << endl;
            }
            assert((*_d_isotropic_cc[i_cc]).get_s_fc().empty());
        }
    }
    return true;
}


void Coarse_Cell_Graph::_update_cc_neighbour(long min_cc, unordered_map<long, long> dict_old_cc_to_new_cc) {

    /**
     * Update cc index for the cc neighbours of renumbered cc.
     */

    // be careful that self._cc_counter is currently before the deletion of removed cc.
    // for i_cc in range(min_cc + 1, self._cc_counter):
    unordered_set<long> s_cc_to_update;
    for (const auto &i_k_v: dict_old_cc_to_new_cc) {
        // we process it from smaller to bigger to avoid rewrite data and the alea in set.
        const long i_cc = i_k_v.first;
        const long i_cc_new = i_k_v.second;
        if (_d_isotropic_cc.count(i_cc_new)) {
            Coarse_Cell *cc_new = _d_isotropic_cc[i_cc_new];
            (*cc_new).update_cc_neighbour_renumbering(dict_old_cc_to_new_cc);
            unordered_set<long> s_cc = (*cc_new).get_s_cc_neighbours();
            s_cc_to_update.insert(s_cc.begin(), s_cc.end());
//            s_cc_to_update.update((*cc_new).get_s_cc_neighbours())

        }
    }

    // We take care of cc that are neighbour of renumbered cc.
    for (const auto &i_k_v : dict_old_cc_to_new_cc) {
        if (s_cc_to_update.count(i_k_v.second)) {
            s_cc_to_update.erase(i_k_v.second);
        }
    }
//    s_cc_to_update -= set(dict_old_cc_to_new_cc.values());
    // print("s_cc_to_update", s_cc_to_update)
    for (const long &i_cc : s_cc_to_update) {
        if (_d_isotropic_cc.count(i_cc)) {
            (*_d_isotropic_cc[i_cc]).update_cc_neighbour_renumbering(dict_old_cc_to_new_cc);
        }

    }
}

// TODO remove this
bool Coarse_Cell_Graph::check_cc_consistency() {
    // print("check_cc_consistency")
    for (auto &i_k_v: _d_isotropic_cc) {
        // print(i_cc)
        Coarse_Cell *cc = _d_isotropic_cc[i_k_v.first];
        if (!(*cc).check_consistency(_fc_2_cc)) {
            return false;
        }
    }
    return true;
}

bool Coarse_Cell_Graph::_check_consistency() {

/**
 *  We check that the data are consistant between dict_cc, dict_Card_Coarse_Cells and fc_to_cc
 */

    bool result = true;


    //=========================
    // Check isotropic part
    //=========================

    for (const auto &i_k_v : _d_isotropic_cc) {
        const long &i_cc = i_k_v.first;
        Coarse_Cell *cc = _d_isotropic_cc[i_cc];
        for (const long &i_fc : (*cc).get_s_fc()) {
            result = result && _fc_2_cc[i_fc] == i_cc;
        }

        unsigned short card = (*cc).__card;
        result = result && _d_card_2_cc.count(card);
        result = result && _d_card_2_cc[card].count(i_cc);

        unsigned short compactness = (*cc).__compactness;
        result = result && (_d_compactness_2_cc.count(compactness) && _d_compactness_2_cc[compactness].count(i_cc));

        result = result && (*cc).check_consistency(_fc_2_cc);
        if (!result) {
            cerr << "Trouble of consistency in isotropic cc " << i_cc << endl;
//           print(i_cc, (*cc).get_s_fc(), self._d_compactness_2_cc)
        }
    }
    //=========================
    // Check anisotropic part
    //=========================
    for (const auto &i_k_v : _d_anisotropic_cc) {
        const long &i_cc = i_k_v.first;
        for (const long &i_fc : _d_anisotropic_cc[i_cc]) {
            result = result && _fc_2_cc[i_fc] == i_cc;
        }
        if (!result) {
            cerr << "Trouble of consistency in anisotropic cc " << i_cc << endl;
        }
    }
    if (!result) {
        cerr << "Failed after step 1" << endl;
    }

    //=========================
    // Check _d_card_2_cc
    //=========================
    for (const auto &i_k_v : _d_card_2_cc) {
        const unsigned short &i_size = i_k_v.first;
        for (const long &i_cc : _d_card_2_cc[i_size]) {
            result = result && _d_isotropic_cc.count(i_cc);
            result = result && (*_d_isotropic_cc[i_cc]).__card == i_size;
        }
    }
    if (!result) {
        cerr << "Failed after step 2" << endl;
    }

    //=========================
    // Check _fc_2_cc
    //=========================

    for (long i_fc = 0; i_fc < _fc_graph.number_of_cells; i_fc++) {
        long i_cc = _fc_2_cc[i_fc];
        if (_d_isotropic_cc.count(i_cc)) {

            // i_cc is an isotropic cell
            if ((*_d_isotropic_cc[i_cc]).get_s_fc().count(i_fc) == 0) {
                cerr << "i_cc not in dict_cc i_fc: " << i_fc << " i_cc " << i_cc << endl;
//                print("i_cc not in dict_cc i_fc:", i_fc, "i_cc", i_cc, "l",
//                      self._d_isotropic_cc[i_cc].get_s_fc())

            }
            result = result && (*_d_isotropic_cc[i_cc]).get_s_fc().count(i_fc);

        } else if (_d_anisotropic_cc.count(i_cc)) {

            // i_cc is an anisotropic cell
            if (_d_anisotropic_cc[i_cc].count(i_fc) == 0) {
                cerr << "i_cc not in dict_cc i_fc: " << i_fc << " i_cc " << i_cc << endl;
//                print("i_cc not in dict_cc i_fc:", i_fc, "i_cc", i_cc, "l", self._d_anisotropic_cc[i_cc])

            }
            result = result && _d_anisotropic_cc[i_cc].count(i_fc);
        } else {
            cerr << "Unknown i_cc: " << i_cc << " i_fc " << i_cc << endl;
            exit(1);

        }
    }
    if (!result) {
        cerr << "Failed after step 3" << endl;
    }

    return result;
}

// TODO remove calls to this function for production
bool Coarse_Cell_Graph::check_data_consistency_and_connectivity() {

    assert(is_agglomeration_done());

    //=========================
    // checks the number of cc: i.e. no delayed cc left
    //=========================

    if (is_agglomeration_isotropic()) {

        long nb_cc = 0;
        for (auto &i_k_v: _d_isotropic_cc) {
            if (nb_cc < i_k_v.first) {
                nb_cc = i_k_v.first;
            }
        }
        assert(_cc_counter == nb_cc+1);
        bool result = _check_consistency();
        if (!result) {
            cerr << "storage in unconsistent" << endl;
            return false;
        }

        //=========================
        // Connectivity checking
        //=========================
        for (auto &i_k_v: _d_isotropic_cc) {
            long i_cc = i_k_v.first;
            Coarse_Cell *cc = i_k_v.second;
            const unordered_set<long> &s_fc = (*cc).get_s_fc();
            if (!_fc_graph.check_connectivity(s_fc)) {
                _fc_graph.check_connectivity(s_fc, true);  // We force output in case of trouble!
                cerr << "Error for Coarse cell " << i_cc << endl;
//                print("Error at level", self._i_lvl, "for coarse cell", index, s_of_fc)
                result = false;
            }
        }
        return result;
    } else {
        bool result = _check_consistency();
        return result && is_agglomeration_anisotropic();
    }

}

