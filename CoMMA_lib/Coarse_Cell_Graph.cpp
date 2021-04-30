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
        if (_cc_counter != new_i_cc)
        {
            cerr<<"Problem consistency in number of Coarse cells " << _cc_counter << " " <<new_i_cc<<endl;
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
                    cerr<<i<<", ";
                }
                cerr<<"}"<<endl;
            }
            assert((*_d_isotropic_cc[i_cc]).get_s_fc().empty());
        }
    }
    return true;
}


void Coarse_Cell_Graph::_update_cc_neighbour(long min_cc, unordered_map<long, long> dict_old_cc_to_new_cc){

    /**
     * Update cc index for the cc neighbours of renumbered cc.
     */

    // be careful that self._cc_counter is currently before the deletion of removed cc.
    // for i_cc in range(min_cc + 1, self._cc_counter):
    unordered_set<long> s_cc_to_update;
    for (const auto& i_k_v: dict_old_cc_to_new_cc)
    {
        // we process it from smaller to bigger to avoid rewrite data and the alea in set.
        const long i_cc=i_k_v.first;
        const long i_cc_new = i_k_v.second;
        if (_d_isotropic_cc.count(i_cc_new))
        {
            Coarse_Cell* cc_new = _d_isotropic_cc[i_cc_new];
            (*cc_new).update_cc_neighbour_renumbering(dict_old_cc_to_new_cc);
            unordered_set<long> s_cc = (*cc_new).get_s_cc_neighbours();
            s_cc_to_update.insert(s_cc.begin(), s_cc.end());
//            s_cc_to_update.update((*cc_new).get_s_cc_neighbours())

        }
    }

    // We take care of cc that are neighbour of renumbered cc.
    for (const auto &i_k_v : dict_old_cc_to_new_cc) {
        if (s_cc_to_update.count(i_k_v.second)){
            s_cc_to_update.erase(i_k_v.second);
        }
    }
//    s_cc_to_update -= set(dict_old_cc_to_new_cc.values());
    // print("s_cc_to_update", s_cc_to_update)
    for(const long& i_cc : s_cc_to_update) {
        if (_d_isotropic_cc.count(i_cc)){
            (*_d_isotropic_cc[i_cc]).update_cc_neighbour_renumbering(dict_old_cc_to_new_cc);
        }

    }
}

bool Coarse_Cell_Graph::check_cc_consistency(){
    // print("check_cc_consistency")
    for (auto& i_k_v: _d_isotropic_cc) {
        // print(i_cc)
        Coarse_Cell *cc = _d_isotropic_cc[i_k_v.first];
        if (!(*cc).check_consistency(_fc_2_cc)) {
            return false;
        }
    }
    return true;
}

void Coarse_Cell_Graph::cc_remove_deleted_cc(unordered_set<long> set_removed_cc) {

  /*
    Deletion of some cc and renumbering of all cc
    :param set_removed_cc: set of index of cc to remove
    :return: None
  */
  long new_cc;
  assert(_delayed_cc.size() > 0);
  if(set_removed_cc.size() > 0) {
    if(_d_isotropic_cc.size() > 0) {
      for(auto i_cc : set_removed_cc) {
	if (_d_isotropic_cc.count(i_cc) > 0) {
	  // Check that self._d_isotropic_cc[i_cc] is empty
	  cerr << "test " << _d_isotropic_cc.count(i_cc) << endl;
	  if (_d_isotropic_cc.count(i_cc) > 0)
	    cerr << "Cc" << i_cc << "is not empty : " << _d_isotropic_cc[i_cc]->get_s_fc().size() << endl;
	  assert(_d_isotropic_cc.count(i_cc) == 0); 
	  
	}
      }
      
      long min_cc = *min_element(set_removed_cc.begin(), set_removed_cc.end());
      unordered_map<long,long> dict_old_cc_to_new_cc;
      new_cc = min_cc;
      
      for(long i_cc = min_cc+1 ; i_cc < _cc_counter ; i_cc++) {
	if(_d_isotropic_cc.count(i_cc) > 0) {
	  dict_old_cc_to_new_cc[i_cc] = new_cc;
	  _d_isotropic_cc[new_cc] = _d_isotropic_cc[i_cc];
	  for(auto i_fc : _d_isotropic_cc[new_cc]->get_s_fc())
	    _fc_2_cc[i_fc] = new_cc;
	  unsigned short int i_cc_size = _d_isotropic_cc[i_cc]->__card;
	  _d_card_2_cc[i_cc_size].erase(i_cc);
	  _d_card_2_cc[i_cc_size].insert(new_cc);

	  unsigned short int compactness = _d_isotropic_cc[i_cc]->__compactness;

	  _d_compactness_2_cc[compactness].erase(i_cc);
	  _d_compactness_2_cc[compactness].insert(new_cc);

	  _d_isotropic_cc.erase(i_cc);
	  new_cc ++;
	}
      }

    }
    else
      new_cc = _cc_counter - set_removed_cc.size();
    _cc_counter -= set_removed_cc.size();
    if (_cc_counter != new_cc)
      cerr << "Problem consistency in number of Coarse cells " << _cc_counter << " " << new_cc << endl;
    assert(_cc_counter == new_cc);
  }
}

unsigned short int Coarse_Cell_Graph::get_cc_cardinal(long i_cc) {
  if (is_isotropic_cc(i_cc))
    return _d_isotropic_cc[i_cc]->__card;
  else
    return _d_anisotropic_cc[i_cc].size();
}

void Coarse_Cell_Graph::cc_update_cc_specifics(unordered_set<long> set_of_fc_to_add, long i_target_cc) {
  /*
        Update a cc with some (not yet agglomerated) fine cells.
   */

  if(is_isotropic_cc(i_target_cc)) {
    unsigned short int old_card = get_cc_cardinal(i_target_cc);
    Coarse_Cell * target_cc = _d_isotropic_cc[i_target_cc];
    unsigned short int old_deg_compactness = target_cc->__compactness;

    // update of the definition of the cc
    target_cc->add_fc(set_of_fc_to_add, _fc_2_cc);

    unsigned short int new_card = target_cc->__card;

    // Update of dict_Card_Coarse_Cells:
    _d_card_2_cc[new_card];
    _d_card_2_cc[new_card].insert(i_target_cc);

    assert(_d_card_2_cc.count(old_card) > 0 && _d_card_2_cc[old_card].count(i_target_cc) > 0);
      
    _d_card_2_cc[old_card].erase(i_target_cc);
    if(_d_card_2_cc[old_card].size() == 0)
      _d_card_2_cc.erase(old_card);

    if (_d_compactness_2_cc.size() > 0) {
      _d_compactness_2_cc[old_deg_compactness].erase(i_target_cc);
      // TODO peut-etre eviter de recalculer. On l'avait avant dans le choix de la CC.
      unsigned short int deg_compactness = target_cc->__compactness;
      _d_compactness_2_cc[deg_compactness];
      _d_compactness_2_cc[deg_compactness].insert(i_target_cc);

    }
  }
  else
    _d_anisotropic_cc[i_target_cc].insert(set_of_fc_to_add.begin(), set_of_fc_to_add.end());
}


unordered_map<long, unordered_set<long>> Coarse_Cell_Graph::compute_dict_faces_in_common_faces_with_cc_neighbourhood(long i_fc) {
  /*
    
    :param i_fc:
    :return: Dict[i_cc:{i_fc_neighbour, ...}]
  */
  // TODO est-ce qu'il n'y aurait pas moyen de trier d'une manière ou d'une autre les voisins grossiers?
  // TODO Tas?

  // TODO  remove compute_nb_faces_in_common_faces_with_cc_neighbourhood() et juste récupérer len()???
  unordered_map<long, unordered_set<long>> d_n_cc;
  for(auto i_fc_neighbour: _fc_graph.get_neighbours(i_fc)) {
    if (i_fc != i_fc_neighbour) {
      long i_cc = _fc_2_cc[i_fc_neighbour];
      if(is_isotropic_cc(i_cc)) {
	// We want only isotropic neighbours
	d_n_cc[i_cc];
	d_n_cc[i_cc].insert(i_fc_neighbour);
      }
    }
  }
  return d_n_cc;
}

bool compare_size(pair<long, unordered_set<long>> a, pair<long, unordered_set<long>> b) {
  return a.second.size() < b.second.size();
}

void Coarse_Cell_Graph::correction_swap_leaf_fc_v2(int nb_iteration, Coarse_Cell_Graph * ccg_l_m_one, int verbose) {
  /*TODO work only on border fc!!!
    set_removed_coarse_cells: Set[int] = set()*/
  unordered_set<long> set_removed_coarse_cells_l_m_one;
  int iteration(0);
  bool is_at_least_one_swap(true);
  // int count(0);
  while(iteration < nb_iteration && is_at_least_one_swap) {
    //TODO: improvement Work only on fc at the edge of cc...
    if (_verbose)
      cerr << "iteration " << iteration << endl;
    is_at_least_one_swap = false;
    int nb_of_swap(0);
    /* TODO je crois qu'il le faisait dans un ordre aleatoire sur les fc dans MGridGen.
       Process every isotropic coarse cells*/
    // conscious copy
    unordered_set<long> cc_keys;
    for(auto i_cc_k_v: _d_isotropic_cc)
      cc_keys.insert(i_cc_k_v.first);
    for(auto i_cc:cc_keys) {
      Coarse_Cell * cc = _d_isotropic_cc[i_cc];
      vector<long> v_s_fc;
      unordered_set<long> s_leaves = cc->compute_s_leaves();
      unordered_set<long> neigbours = cc->get_s_fc_w_outer_neighbours(2);

      set_intersection(s_leaves.begin(), s_leaves.end(), neigbours.begin(), neigbours.end(), v_s_fc.begin());
      unordered_set<long> s_fc(v_s_fc.begin(), v_s_fc.end());
      for(auto i_fc : s_fc) {
	unordered_map<long, unordered_set<long>> d_n_fc = compute_dict_faces_in_common_faces_with_cc_neighbourhood(i_fc);
	// sort wrt number of edge
	// TODO sorting... you can't sort a map!
	vector<pair<long, unordered_set<long>>> d_n_fc_v(d_n_fc.begin(), d_n_fc.end());
	// can't sort the map?
	sort(d_n_fc_v.begin(), d_n_fc_v.end(), compare_size);

	vector<long> l_keys_i_cc;
	for(auto d_n_fc_k_v : d_n_fc_v)
	  l_keys_i_cc.push_back(d_n_fc_k_v.first);
	unsigned short int i_last_key = l_keys_i_cc.size() - 1;
	auto it = find(l_keys_i_cc.begin(), l_keys_i_cc.end(), i_cc);
	int index = distance(l_keys_i_cc.begin(), it);
	if (d_n_fc.count(i_cc) || index < i_last_key) {
	  /* Is there a better cc for i_fc.
	     First condition, a cc of size one.
	     Second condition, i_cc is not the best cc for i_fc : l_keys_i_cc[i_last_key] is better*/
	  long argmax_nb_common_faces_n = l_keys_i_cc[i_last_key];
	  unsigned short int max_nb_common_faces_n = d_n_fc[l_keys_i_cc[i_last_key]].size();
	  unsigned short int min_size(1);
	  if (max_nb_common_faces_n > min_size) {
	    unordered_set<long> tmp;
	    tmp.insert(i_fc);
	    unordered_set<long> set_tmp = cc_swap_fc(tmp, i_cc, argmax_nb_common_faces_n);
	    // set_removed_coarse_cells.update(set_tmp)
	    assert(check_cc_consistency());
	    vector<long> inter;
	    set_intersection(_s_cc_to_remove.begin(), _s_cc_to_remove.end(), set_tmp.begin(), set_tmp.end(), inter.begin());
	    assert(inter.size() == 0);
	    _s_cc_to_remove.insert(set_tmp.begin(), set_tmp.end());
	    
	    // self.plot_cc("tmp_" + str(iteration) + "_" + str(nb_of_swap))
	    if (ccg_l_m_one != NULL) {
	      long i_cc_l_m_one = ccg_l_m_one->_fc_2_cc[i_fc];
	      unordered_set<long> s_cc;
	      for(auto i_fc_n: d_n_fc[l_keys_i_cc[i_last_key]])
		s_cc.insert(ccg_l_m_one->_fc_2_cc[i_fc_n]);
	      if(s_cc.size() == 1) {
		  long argmax_nb_common_faces_n_l_m_one = *s_cc.begin();
		  s_cc.erase(*s_cc.begin());
		  //print("l_m_one :Swap ", i_fc, i_cc_l_m_one, ccg_l_m_one.get_cc(i_cc_l_m_one),
		  //argmax_nb_common_faces_n_l_m_one, ccg_l_m_one.get_cc(argmax_nb_common_faces_n_l_m_one), )
		  //TODO : check arguments? 
		  //assert(ccg_l_m_one->check_cc_consistency(_fc_2_cc));
		  assert(ccg_l_m_one->check_cc_consistency());
		  // Swap lower level
		  unordered_set<long> tmp;
		  tmp.insert(i_fc);
		  unordered_set<long> set_tmp = ccg_l_m_one->cc_swap_fc(tmp, i_cc_l_m_one,
									argmax_nb_common_faces_n_l_m_one);
		  assert(ccg_l_m_one->check_cc_consistency());
		  set_removed_coarse_cells_l_m_one.insert(set_tmp.begin(), set_tmp.end());
		  
	      }
	      else {
		// TODO on fait pareil (i.e. on prend le résultat du pop), mais on doit pouvoir trouver mieux.
		long argmax_nb_common_faces_n_l_m_one = *s_cc.begin();
		s_cc.erase(*s_cc.begin());
		
		// print("l_m_one :Swap ", i_fc, i_cc_l_m_one, ccg_l_m_one.get_cc(i_cc_l_m_one),
		// argmax_nb_common_faces_n_l_m_one,
		// ccg_l_m_one.get_cc(argmax_nb_common_faces_n_l_m_one), )
		//assert ccg_l_m_one.check_cc_consistency()
		unordered_set<long> tmp;
		tmp.insert(i_fc);
		unordered_set<long> set_tmp = ccg_l_m_one->cc_swap_fc(tmp, i_cc_l_m_one,
                                                                     argmax_nb_common_faces_n_l_m_one);
		// assert ccg_l_m_one.check_cc_consistency()
		set_removed_coarse_cells_l_m_one.insert(set_tmp.begin(), set_tmp.end());
	      }
	    }
	    nb_of_swap ++;
	    is_at_least_one_swap = true;
	    // assert ccg_l_m_one.check_data_consistency_and_connectivity()
	    // assert self.check_data_consistency_and_connectivity()
	  }

	}
      }
    }
    //
    iteration ++;
    if(_verbose) {
      cerr << "nb_of_swap = "  << nb_of_swap << endl;
      cerr << "After iteration" << iteration << endl;
      // cerr << "Dict_distribution_of_cardinal_of_cc" << d_distribution_of_cardinal_of_isotropic_cc << endl;
    }
  }
  if(set_removed_coarse_cells_l_m_one.size() > 0) {
    cerr << "set_removed_coarse_cells_l_m_one ";
    for (auto elt : set_removed_coarse_cells_l_m_one)
      cerr << elt << " ";
    cerr<< endl;
    //print("set_removed_coarse_cells_l_m_one", set_removed_coarse_cells_l_m_one)
    //raise ValueError
  }
  /*
        # if set_removed_coarse_cells:
        #     print("set_removed_coarse_cells", set_removed_coarse_cells)
        #     self.cc_remove_deleted_cc(set_removed_coarse_cells)
        # raise ValueError
  */
  if (ccg_l_m_one != NULL)
    assert(ccg_l_m_one->check_cc_consistency());

}

