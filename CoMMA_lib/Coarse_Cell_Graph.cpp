//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Coarse_Cell_Graph.h"

Coarse_Cell_Graph::Coarse_Cell_Graph(Dual_Graph &fc_graph,
                                     int verbose)
        : _fc_graph(fc_graph), _verbose(verbose) {
    //==================
    // Input datas
    //==================
     // Initialization of the vector of the agglomerated cells
     // as false at the beginning, are all not agglomerated.
     vector<bool> tmp(fc_graph.number_of_cells, false);
    _a_is_fc_agglomerated = tmp;

    //==================
    // temporary fields
    //==================
    // field that identify the index of the course cell in the 
    // fine cell (and retrived as the output)
    _fc_2_cc = vector<long>(fc_graph.number_of_cells, -1);
}

Coarse_Cell_Graph::~Coarse_Cell_Graph() {
    if (!_d_isotropic_cc.empty()) {
        for (auto i_kv:_d_isotropic_cc) {
            delete i_kv.second;
        }
    }
}



long Coarse_Cell_Graph::cc_create_a_cc(const unordered_set<long> &s_fc,
                                       bool is_anisotropic,
                                       bool is_creation_delayed) {

// Create a course cell from the fine cells and update the fc_2_cc tree.
    assert((!is_anisotropic) || (!is_creation_delayed));

    for (const long &i_fc :s_fc) {
        if (_fc_2_cc[i_fc] != -1) {
            cerr << "fc " << i_fc << " is already assigned to i_cc " << _fc_2_cc[i_fc] << endl;
        }
        assert(_fc_2_cc[i_fc] == -1);
    }

    bool is_mutable = true;
    if (is_anisotropic) {

        assert(!is_creation_delayed);
        _d_anisotropic_cc[_cc_counter] = unordered_set<long>(s_fc);  //TODO check there is a copy???

        is_mutable = false;
    }

    if (!is_creation_delayed) {
        // We create the cc right now.
        // Everything is updated:
        // if is_mutable: dict_cc, dict_card_cc, dict_compactness_2_cc
        // in both case: number_of_fine_agglomerated_cells_tmp, isFineCellAgglomerated_tmp, _fc_2_cc,
        if (is_mutable) {
            // the cell can be modified afterwards and is thus defined in dict_cc and dict_card_cc, dict_compactness_2_cc, dict_cc_to_compactness
            // Update of dict_cc:
            //==================
            Coarse_Cell *new_cc = new Coarse_Cell(_fc_graph,_cc_counter, s_fc);
            _d_isotropic_cc[_cc_counter] = new_cc;

            // Update of dict_Card_Coarse_Cells:
            //==================
            unsigned short int card = (*new_cc).__card;
            if (_d_card_2_cc.count(card) > 0) {
                _d_card_2_cc[card].insert(_cc_counter);
            } else {
                _d_card_2_cc[card] = unordered_set<long>({_cc_counter});
            }

            // Update of compactness informations:
            //####################################
            assert((*new_cc).is_connected());

            unsigned short int deg_compactness = (*new_cc).__compactness;  // (*this)._fc_graph.compute_min_fc_compactness_inside_a_cc(s_fc)
            if (_d_compactness_2_cc.count(deg_compactness) > 0) {
                //if deg_compactness in (*this)._d_compactness_2_cc:
                _d_compactness_2_cc[deg_compactness].insert(_cc_counter);
            } else {
               _d_compactness_2_cc[deg_compactness] = unordered_set<long>({_cc_counter});
            }

        }

        // Update of _associatedCoarseCellNumber the output of the current function agglomerate
	// _fc_2_cc is filled with _cc_counter
        for (const long &i_fc :s_fc) {
            // Only if not isCreationDelayed:
            // TODO not check this in production
            if (_fc_2_cc[i_fc] != -1) {
                cerr << "fc " << i_fc << " is Already assigned fine cell" << endl;
            }
            assert(_fc_2_cc[i_fc] == -1);
            _fc_2_cc[i_fc] = _cc_counter;
        }
        // Update of the number of CC
        //####################################
        _cc_counter++; // Only if not isCreationDelayed
    } else {
        // We do not create the coarse cell yet.
        // As this coarse cell will be soon deleted, we want its coarse index to be the greater possible.
        // Only isFineCellAgglomerated_tmp, number_of_fine_agglomerated_cells_tmp and
        // dict_DistributionOfCardinalOfCoarseElements are modified!
        _delayed_cc.push_back(s_fc);
        // raise ValueError("TO DO")
    }

    for (const long &i_fc :s_fc) {
        if (!_a_is_fc_agglomerated[i_fc]) {
            // Rq: initialise to False pour chaque niveau dans agglomerate(...)
            _a_is_fc_agglomerated[i_fc] = true;
            _nb_of_agglomerated_fc++;
        }
    }
    return (_cc_counter - 1);
}

void Coarse_Cell_Graph::fill_cc_neighbouring() {

    for (auto &i_k_v: _d_isotropic_cc) {
        (*i_k_v.second).fill_cc_neighbouring(_fc_2_cc);
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
                                                  const long i_origin_cc,
                                                  const long i_destination_cc) {


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
    if (_verbose) {
        cout << "Swap CC: {";
        for (const long &i_fc: s_fc) {
            cout << i_fc << ", ";
        }
        cout << "} " << i_origin_cc << " " << i_destination_cc << endl;
    }

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

    // 1) We remove i_origin_cc and i_destination_cc from _d_card_2_cc and _d_compactness_2_cc
    //========================================================================================
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
        delete _d_isotropic_cc[i_origin_cc];
        set_removed_cc.insert(i_origin_cc);
        _d_isotropic_cc.erase(i_origin_cc);

    } else {

        if ((*origin_cc).is_connected()) {
            // 3) We add i_origin_cc back with updated infos.
            //===============================================
            __add_a_cc(i_origin_cc);
        } else {
            // TODO Merge this with cc_split_non_connected_cc(...)
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
                if (_d_isotropic_cc.count(i_cc) > 0) {
                    // deal with _d_isotropic_cc
                    assert(_s_cc_to_remove.count(i_cc) == 0);
                    dict_old_cc_to_new_cc[i_cc] = new_i_cc;

                    delete _d_isotropic_cc[new_i_cc];

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
                    }
                    _d_anisotropic_cc.erase(i_cc);
                    new_i_cc++;


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
    assert(is_agglomeration_done());

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
            cerr << "Coarse_Cell_Graph::_check_consistency() ";
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
        if (!_d_anisotropic_cc.empty()) {
            for (auto &i_k_v: _d_anisotropic_cc) {
                if (nb_cc < i_k_v.first) {
                    nb_cc = i_k_v.first;
                }
            }
        }
        if (!_s_cc_to_remove.empty()) {
            for (const long i_fc: _s_cc_to_remove) {
                if (nb_cc < i_fc) {
                    nb_cc = i_fc;
                }
            }
        }
        if (_cc_counter != nb_cc + 1) {
            cout << "_cc_counter " << _cc_counter << "\tnb_cc + 1: " << nb_cc + 1 << endl;
        }
        assert(_cc_counter == nb_cc + 1);
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


unordered_map<long, unsigned short> Coarse_Cell_Graph::compute_nb_faces_in_common_faces_with_cc_neighbourhood(const long i_fc) const {
    unordered_map<long, unsigned short> d_n_cc;
    for (const long &i_fc_neighbour: _fc_graph.get_neighbours(i_fc)) {
        if (i_fc != i_fc_neighbour) {
            long i_cc_n = _fc_2_cc[i_fc_neighbour];
            if (d_n_cc.count(i_cc_n)) {
                d_n_cc[i_cc_n] += 1;
            } else {
                d_n_cc[i_cc_n] = 1;
            }
        }
    }
    return d_n_cc;
}

unordered_map<long, unordered_set<long>> Coarse_Cell_Graph::compute_dict_faces_in_common_faces_with_cc_neighbourhood(const long &i_fc) const {


    // return Dict[i_cc:{i_fc_neighbour, ...}]

    // TODO est-ce qu'il n'y aurait pas moyen de trier d'une manière ou d'une autre les voisins grossiers?
    // TODO Tas?

    // TODO  remove compute_nb_faces_in_common_faces_with_cc_neighbourhood() et juste récupérer len()???
    unordered_map<long, unordered_set<long>> d_n_cc;
    for (const long &i_fc_neighbour :_fc_graph.get_neighbours(i_fc)) {
        if (i_fc != i_fc_neighbour) {
            long i_cc = _fc_2_cc[i_fc_neighbour];
            if (is_isotropic_cc(i_cc)) {
                // We want only isotropic neighbours
                if (d_n_cc.count(i_cc)) {
                    d_n_cc[i_cc].insert(i_fc_neighbour);
                } else {
                    d_n_cc[i_cc] = {i_fc_neighbour};
                }
            }
        }
    }
    return d_n_cc;
}

void Coarse_Cell_Graph::compute_nb_faces_in_common_faces_with_cc_neighbourhood_w_unwanted_fc(const long &i_fc,
                                                                                             const unordered_set<long> &s_unwanted_fc,
                                                                                             unordered_set<long> &set_argmax_number_common_faces,
                                                                                             unordered_map<long, unsigned short> &dict_adjacent_cc
) const {

/**
 * Computes the number of faces in contact of every cc neighours
 *       For every fine cell, i_fc, inside i_cc,
 *       we look for which coarse neighbour shares the most common faces with it.
 *
 *  return: the set of arg_max and the dict of neighbouring cc
 */

    unsigned short max_number_common_faces = 0;

    // We process every neighbour of i_fc
    for (const long &i_fc_neighbour:_fc_graph.get_neighbours(i_fc)) {
        if (i_fc_neighbour != i_fc && (s_unwanted_fc.count(i_fc_neighbour) == 0)) {
            // second term is to check that the neighbour does not belong to current incomplete cc.
            long i_cc_neighbour = _fc_2_cc[i_fc_neighbour];
            if (is_isotropic_cc(i_cc_neighbour)) {
                // we add the coarse cell containing the fine cell i_cc_neighbour in dict_adjacent_cc
                if (dict_adjacent_cc.count(i_cc_neighbour)) {
                    dict_adjacent_cc[i_cc_neighbour] += 1;
                } else {
                    dict_adjacent_cc[i_cc_neighbour] = 1;
                }
                // We search the neighborhood with the largest number of faces
                if (dict_adjacent_cc[i_cc_neighbour] > max_number_common_faces) {
                    max_number_common_faces = dict_adjacent_cc[i_cc_neighbour];
                    set_argmax_number_common_faces = {i_cc_neighbour};
                } else if (dict_adjacent_cc[i_cc_neighbour] == max_number_common_faces) {
                    set_argmax_number_common_faces.insert(i_cc_neighbour);
                }
            }
        }
    }
}

unordered_set<long> Coarse_Cell_Graph::get_s_isotropic_cc_of_size(unsigned short size_min, unsigned short size_max) {
    /**
        * :param size_min: included
        * :param size_max: not included
        */

    if (_d_isotropic_cc.empty()) {
        return unordered_set<long>();
    }
    unordered_set<long> s_cc;
    unsigned short max_card = 0;
    if (size_max == 0) {
        for (const auto &i_k_v :_d_card_2_cc) {
            if (max_card < i_k_v.first) {
                max_card = i_k_v.first;
            }
        }
        size_max = max_card + 1;
    }

    for (unsigned short i_size = size_min; i_size < size_max; i_size++) {
        if (_d_card_2_cc.count(i_size)) {
//            s_cc.update(self._d_card_2_cc[i_size])
            for (const long &i_cc: _d_card_2_cc[i_size]) {
                s_cc.insert(i_cc);
            }
        }


    }

    return s_cc;
}

void Coarse_Cell_Graph::correction_make_small_cc_bigger(const unsigned short &min_card,
                                                        const unsigned short &goal_card,
                                                        const unsigned short &threshold_card,
                                                        bool verbose) {
    /**
     * We try take make too small cc bigger
     */

    if (verbose) {
        cout << "Call of __makeSmallCellBigger" << endl;
    }

    unordered_set<long> s_i_cc = get_s_isotropic_cc_of_size(threshold_card + 1, min_card);

    // We process every cc of cardinal s.t. threshold_card + 1 <=card <min_card
    for (const long &i_cc :s_i_cc) {
        if (verbose) {
            cerr << "\t\t i_cc " << i_cc;
        }

        // Special case: it can happened that a coarse cell is whole "eaten" in the process it is
        // added to set_removedCoarseCells.
        if (_d_isotropic_cc.count(i_cc) == 0) {
            // Anisotropic cell
            continue;
        }
        Coarse_Cell *cc = _d_isotropic_cc[i_cc];
        unordered_set<long> s_fc = (*cc).get_s_fc();
        // search of the fine cell at the "root" of the coarse cell, i.e. the fine cell with the most
        // faces in common with its coarse cell.
        long seed = _fc_graph._compute_subgraph_root(s_fc);
        if (verbose) {
            cerr << "\t\t seed " << seed << endl;
        }

        // From this cell (seed) we try to add extra fine cells!
        // Building of the neighbourhood:
        // Watch out: different of Dual_graph.compute_neighbourhood(...) in the selection of neighbours
        short nb_of_order_of_neighbourhood = 3;
        vector<bool> v_is_fc_agglomerated_in_isotropic_cc(_fc_graph.number_of_cells, false);  // Attention, there is a "!"/not in compute_neighbourhood_of_cc(...)
        for (const auto &i_k_v:_d_anisotropic_cc) {
            for (const long &i_fc:i_k_v.second) {
                v_is_fc_agglomerated_in_isotropic_cc[i_fc] = true;
            }
        }
        unordered_set<long> s_seed = {seed};
        unordered_map<long, short> d_neighbours_of_seed = {};
        _fc_graph.compute_neighbourhood_of_cc(s_seed,
                                              nb_of_order_of_neighbourhood,
                                              d_neighbours_of_seed,
                                              goal_card,
                                              v_is_fc_agglomerated_in_isotropic_cc);

        // We remove all fine cell already contained in the current coarse element
        for (const long &i_fc : (*cc).get_s_fc()) {
            if (d_neighbours_of_seed.count(i_fc)) {
                d_neighbours_of_seed.erase(i_fc);
            }
        }

        // The goal is now to add fc to the current cc
        //=============================================

        // Number of fine cells constituting the current coarse cell in construction.
        unsigned short size_current_cc = (*cc).__card;

        // set of fc for current cc:
        unordered_set<long> s_of_fc_for_current_cc = unordered_set<long>((*cc).get_s_fc());  // conscious copy

        // If no neighbour is found for seed: this case happened only when isotropic cell is surrounded by anisotropic cells.
        if (d_neighbours_of_seed.empty()) {
            continue;
        }
        assert(goal_card > size_current_cc);
        unsigned short nb_fc_to_add = goal_card - size_current_cc;

        assert(d_neighbours_of_seed.size() >= nb_fc_to_add);

        // we note that i_cc has been modified
        unordered_set<long> s_of_modified_cc = {i_cc};

        // Choice of the fine cells to agglomerate
        while (size_current_cc < goal_card) {

            unsigned short max_faces_in_common = 0;
            long arg_max_faces_in_common = -1;
            bool is_default_value_arg_max = true;

            // For every fine cell in the neighbourhood:
            for (const auto &i_k_v:d_neighbours_of_seed) {

                const long &i_fc = i_k_v.first;

                // On teste toutes les nouvelles cellules possibles pour prendre celle qui minimise localement s_fc'Aspect Ratio.
                if (arg_max_faces_in_common == -1) {
                    arg_max_faces_in_common = i_fc;
                }

                if (is_fc_agglomerated_in_isotropic_cc(i_fc)) {

                    // Est ce que la cellule grossiere associee a la cellule fine i_fc existe et est mutable?
                    unsigned short number_faces_in_common = _fc_graph.compute_degree_of_node_in_subgraph(i_fc, (*cc).get_s_fc());

                    // print "i_fc", i_fc, number_faces_in_common, ' in Coarse_Cell', self._Fine_Cell_indices_To_Coarse_Cell_Indices[_i_lvl][i_fc], dict_Coarse_Elem[ self._Fine_Cell_indices_To_Coarse_Cell_Indices[_i_lvl][i_fc]]
                    unsigned short order = d_neighbours_of_seed[i_fc];

                    // TODO This version seems good but refactorisation to do: perhaps it is not needed to update every new possible coarse cell aspect ratio?
                    // TODO also need to remove the list of minAR, argminAR, etc.
                    if (number_faces_in_common >= max_faces_in_common) {
                        if (number_faces_in_common == max_faces_in_common) {
                            if (order <= d_neighbours_of_seed[arg_max_faces_in_common]) {
                                // order is the "distance" of the current cell to the seed
                                unordered_set<long> s_current_tmp = unordered_set<long>(s_of_fc_for_current_cc);  // copy
                                s_current_tmp.insert(i_fc);
                                long i_cc_n = _fc_2_cc[i_fc];
                                assert(_d_isotropic_cc.count(i_cc_n));
                                unordered_set<long> tmp_set_neighbour = unordered_set<long>((*_d_isotropic_cc[i_cc_n]).get_s_fc());  // conscious copy
                                tmp_set_neighbour.erase(i_fc);

                                // Checks that we don't break connectivity of any element:
                                // s_current_tmp nor tmp_set_neighbour
                                if (_fc_graph.check_connectivity(s_current_tmp) && _fc_graph.check_connectivity(tmp_set_neighbour)) {
                                    // The second condition asserts the connectivity of the coarse element.
                                    arg_max_faces_in_common = i_fc;
                                    is_default_value_arg_max = false;
                                    // The number of face in common is the same no need to touch it
                                }
                            }
                        } else {
                            unordered_set<long> s_current_tmp = unordered_set<long>(s_of_fc_for_current_cc);  // copy
                            s_current_tmp.insert(i_fc);
                            long i_cc_n = _fc_2_cc[i_fc];
                            assert(_d_isotropic_cc.count(i_cc_n));
                            unordered_set<long> tmp_set_neighbour = unordered_set<long>((*_d_isotropic_cc[i_cc_n]).get_s_fc());  // conscious copy
                            tmp_set_neighbour.erase(i_fc);
                            // Checks that we don't break connectivity of any element:
                            // s_current_tmp nor tmp_set_neighbour
                            if (_fc_graph.check_connectivity(s_current_tmp) && _fc_graph.check_connectivity(tmp_set_neighbour)) {
                                // The second condition asserts the connectivity of the coarse element.
                                // Case :number_faces_in_common > max_faces_in_common:
                                max_faces_in_common = number_faces_in_common;
                                arg_max_faces_in_common = i_fc;
                                is_default_value_arg_max = false;
                                // The number of face in common is the same no need to touch it
                            }
                        }
                    }
                }
            }
            if (!is_default_value_arg_max) {
                // s_of_fc_for_current_cc.append(arg_max_faces_in_common)
                size_current_cc += 1;

                d_neighbours_of_seed.erase(arg_max_faces_in_common);
                long i_old_cc = _fc_2_cc[arg_max_faces_in_common];
                unordered_set<long> s = {arg_max_faces_in_common};
                unordered_set<long> set_tmp = cc_swap_fc(s, i_old_cc, i_cc);
                s_of_modified_cc.insert(i_old_cc);

                if (set_tmp.size() > 0) {
                    long i_cc_to_delete = *set_tmp.begin();
                    assert(set_tmp.size() == 1);
                    assert(_s_cc_to_remove.count(i_cc_to_delete) == 0);
                    _s_cc_to_remove.insert(i_cc_to_delete);
                }
            } else {
                break;
            }
        }

        // Phase de verification!
        // Check if we don't break connectivity in neighbouring cc.
        for (const long &i_cc_2 :s_of_modified_cc) {
            if (is_isotropic_cc(i_cc_2)) {  // i_cc_2 cell may have been eaten!
                s_fc = (*_d_isotropic_cc[i_cc_2]).get_s_fc();
                if (!_fc_graph.check_connectivity(s_fc)) {
// print "Treatment of non connected cell", i_cc_2, "s_fc=", s_fc
                    cc_split_non_connected_cc(i_cc_2);
                }
            }
        }
    }

    if (verbose) {
        cout << "End of __makeSmallCellBigger" << endl;
    }
}


void Coarse_Cell_Graph::cc_split_non_connected_cc(const long &i_cc) {

    /**
     * As a non connected coarse cell has been created (indexed i_cc,
     * index of the non-connected coarse cell
     */

    // print( "\nsplitNonConnectedCoarseCell"
    assert(_d_isotropic_cc.count(i_cc));
    vector<unordered_set<long>> l_of_s_connected_component = _fc_graph.compute_connected_components((*_d_isotropic_cc[i_cc]).get_s_fc());
    // print( "l_of_s_connected_component", l_of_s_connected_component
    unsigned short arg_max = 0;
    unsigned short max_length = 0;
    for (unsigned short i_l = 0; i_l < l_of_s_connected_component.size(); i_l++) {
        if (l_of_s_connected_component[i_l].size() >= max_length) {
            max_length = l_of_s_connected_component[i_l].size();
            arg_max = i_l;
        }
    }
    // print( "arg_max",arg_max
    for (unsigned short i_l = 0; i_l < l_of_s_connected_component.size(); i_l++) {

        // print( "i_l", i_l
        if (i_l != arg_max) {
            unordered_set<long> empty_set = unordered_set<long>({});
            cc_create_a_cc(empty_set);
            unordered_set<long> s_removed_cc = cc_swap_fc(l_of_s_connected_component[i_l], i_cc, _cc_counter - 1);
            assert(s_removed_cc.size() == 0);

        }


    }
    // Check the split cell (cc_counter - 1)
    assert(_fc_graph.check_connectivity(_d_isotropic_cc[_cc_counter - 1]->get_s_fc()));

    // Check the original coarse cell
    assert(_fc_graph.check_connectivity(_d_isotropic_cc[i_cc]->get_s_fc()));
}


unordered_map<unsigned short, long> Coarse_Cell_Graph::compute_d_distribution_of_cardinal_of_isotropic_cc() {
    unordered_map<unsigned short, long> d_distr_card_cc;
    for (const auto &i_k_v : _d_isotropic_cc) {
        // We get the reference coarse cell cardinality
        unsigned short card = (*_d_isotropic_cc[i_k_v.first]).__card;
        if (d_distr_card_cc.count(card)) {
        // If is alreafy inside +1 otherwise we initialize to 1
            d_distr_card_cc[card] += 1;
        } else {
            d_distr_card_cc[card] = 1;
        }

    }
    return d_distr_card_cc;
}

void Coarse_Cell_Graph::correction_reduce_too_big_cc(const unsigned short &goal_card,
                                                     const unsigned short &verbose) {

    // TODO Add variable for cardMin et cardMax.
    // TODO add the same optimisation as for correction_swap

    // Process every isotropic cc
    for (const long &i_cc :get_s_isotropic_cc_of_size(goal_card)) {

        // copy because the cc may be modify during the process: cc_swap_fc
        bool is_change = true;
        while (is_change) {


            // TODO really costly operation. We recompute everything every time
            // TODO we should only work on the neighbour of the remove i_fc
            is_change = false;
            Coarse_Cell *cc = _d_isotropic_cc[i_cc];
            // print("\t\t", i_cc, cc.get_s_fc())
            unordered_set<long> s_leaves = (*cc).compute_s_leaves();

            for (const long &i_fc : s_leaves) {


                unsigned short min_card_neighbour = numeric_limits<unsigned short>::max();
                long arg_min_card_neighbour = -1;
                vector<long> a_neighbours = _fc_graph.get_neighbours(i_fc);

                for (const long &i_fc_n:a_neighbours) {
                    if (i_fc_n != i_fc && (*cc).get_s_fc().count(i_fc_n) == 0) {


                        const long &i_cc_n = _fc_2_cc[i_fc_n];
                        if (is_isotropic_cc(i_cc_n)) {
                            Coarse_Cell *cc_n = _d_isotropic_cc[i_cc_n];
                            unsigned short size_neighbour = (*cc_n).__card;
                            if (size_neighbour < goal_card) {
                                if (size_neighbour < min_card_neighbour) {
                                    min_card_neighbour = size_neighbour;
                                    arg_min_card_neighbour = i_cc_n;
                                }
                            }
                        }
                    }
                }

                if (min_card_neighbour < goal_card) {

                    unordered_set<long> s_fc_to_swap = {i_fc};
                    unordered_set<long> set_tmp = cc_swap_fc(s_fc_to_swap, i_cc, arg_min_card_neighbour);

                    // assert(not _s_cc_to_remove & set_tmp);
                    // _s_cc_to_remove.update(set_tmp);

                    if (set_tmp.size() > 0) {
                        long i_cc_to_delete = *set_tmp.begin();
                        assert(set_tmp.size() == 1);
                        assert(_s_cc_to_remove.count(i_cc_to_delete) == 0);
                        _s_cc_to_remove.insert(i_cc_to_delete);
                    }

                    if (!set_tmp.empty()) {
                        // print("set_tmp", set_tmp)
                        is_change = false;
                        break;
                    }
                    is_change = true;
                }
            }
        }
    }
    if (_verbose) {
        cout << "After Destruction too big cells" << endl;
        //print("dict_DistributionOfCardinalOfCoarseElements", self.d_distribution_of_cardinal_of_isotropic_cc)
    }
}

void Coarse_Cell_Graph::correction_remove_too_small_cc(const unsigned short &threshold_card) {
    /**
     *  For every too small cc, we split it in fc and try to give it to the "optimal" cc neighbour.
     */

    // Treatment of too small cells:
    // 1<= card(Coarse_Cell_v2) <= threshold_card
    for (unsigned short i_cc_size = 1; i_cc_size < threshold_card + 1; i_cc_size++) {
    // We check if it exist this determined cardinality
        if (_d_card_2_cc.count(i_cc_size)) {
    // temporary set of Coarse Cell with a given cardinality
            unordered_set<long> s_temporary(_d_card_2_cc[i_cc_size]);
    // We cycle on the coarse cells
            for (const long &i_cc :s_temporary) {
                assert(is_isotropic_cc(i_cc));
    // We point on the actual coarse cell
                Coarse_Cell *cc = _d_isotropic_cc[i_cc];
    // We get the set of Fine Cells of the current coarse cell
                unordered_set<long> s_fc = (*cc).get_s_fc();
    // We check if we realize the condition of the threshold_card (minimum cardinality to
    // consider the coarse cell acceptable)
                if (s_fc.size() > threshold_card) {
                    continue;
                }
    // We process every fine cell separately
                int iCount = 0;
    // vector fc in cc
                vector<long> l_fc_in_i_cc(s_fc.size());
    // temp set
                unordered_set<long> set_tmp(s_fc);
    // Vector fill 
                for (const long &i_fc:s_fc) {
                    l_fc_in_i_cc[iCount] = i_fc;
                    iCount++;
                }
    // Size of oribilan cc
                unsigned short size_of_original_cc = l_fc_in_i_cc.size();
    // Initialize untreated_fc
                unordered_set<long> untreated_fc;
    // Cycle on the fine cells of the coarse cells
                for (unsigned short i = 0; i < l_fc_in_i_cc.size(); i++) {
                    const long &i_fc = l_fc_in_i_cc[i];
    // We process every neighbour of i_fc
                    unordered_set<long> set_argmax_number_common_faces;
                    unordered_map<long, unsigned short> dict_adjacent_cc;

                    unordered_set<long> s_union(set_tmp);
                    for (const long &i:untreated_fc) {
                        s_union.insert(i);
                    }

                    compute_nb_faces_in_common_faces_with_cc_neighbourhood_w_unwanted_fc(i_fc,
                                                                                         s_union,
                                                                                         set_argmax_number_common_faces,
                                                                                         dict_adjacent_cc);
                    //=================
                    // Three cases:
                    //=================

                    // 1) No cc neighbour:
                    //====================
                    if (set_argmax_number_common_faces.size() < 1) {
                        // No cc neighbour found!
                        // We can be in this situation when a fine cell is included inside the rest of fine cells.
                        // Putting it to the end should solve the problem!
                        // This problem could occur several times.
                        // Imagine a cc in "line" enclosed in anisotropic cells!
                        // In the worst case scenario in the order of treatment of fc, we may need to
                        // add (size_of_original_cc -1) times the most enclosed fc.

                        int counter = 0;
                        for (const long &i_tmp: l_fc_in_i_cc) {
                            if (i_tmp == i_fc) {
                                counter++;
                            }
                        }

                        if (counter <= size_of_original_cc) {
                            l_fc_in_i_cc.push_back(i_fc);
                            continue;
                        } else {
                            // Problematic case!
                            // the current incomplete cell has no (isotropic) neighbour!
                            untreated_fc.insert(i_fc);
                        }
                    }
                        // 2) One "optimal" cc neighbour:
                    else if (set_argmax_number_common_faces.size() == 1) {

                        // pop du premier!
                        // arg_max = set_argmax_number_common_faces.pop()
                        long arg_max = *set_argmax_number_common_faces.begin();
                        set_argmax_number_common_faces.erase(arg_max);

                        // Attention: do not replace self._fc_2_cc[i_fc] by i_cc because after a swap that disconnect a cc
                        // a new one is created.
                        unordered_set<long> s_fc_to_swap = {i_fc};
                        unordered_set<long> set_tmp_2 = cc_swap_fc(s_fc_to_swap, _fc_2_cc[i_fc], arg_max);

                        if (set_tmp_2.size() > 0) {
                            long i_cc_to_delete = *set_tmp_2.begin();
                            assert(set_tmp_2.size() == 1);
                            assert(_s_cc_to_remove.count(i_cc_to_delete) == 0);
                            _s_cc_to_remove.insert(i_cc_to_delete);
                        }
                    } else {


                        // 3) More than one neighbour:
                        // We choose the smallest coarse neighbour.
                        // TODO Is it the better choice????

                        long arg_min = numeric_limits<long>::max();
                        unsigned short size_min = numeric_limits<unsigned short>::max();

                        // TODO Is it the better choice????
                        // We choose the smallest coarse neighbour.
                        for (const long &i_c : set_argmax_number_common_faces) {
                            // reminder dict_Coarse_Elem contains only isotropic agglomeration
                            if ((*_d_isotropic_cc[i_c]).__card < size_min) {
                                size_min = (*_d_isotropic_cc[i_c]).__card;
                                arg_min = i_c;
                            }
                        }

                        long arg_max = arg_min;
                        // Attention: do not replace self._fc_2_cc[i_fc] by i_cc because after a swap that disconnect a cc
                        // a new one is created.
                        unordered_set<long> s_fc_to_swap = {i_fc};
                        unordered_set<long> set_tmp_2 = cc_swap_fc(s_fc_to_swap, _fc_2_cc[i_fc], arg_max);

                        if (set_tmp_2.size() > 0) {
                            long i_cc_to_delete = *set_tmp_2.begin();
                            assert(set_tmp_2.size() == 1);
                            assert(_s_cc_to_remove.count(i_cc_to_delete) == 0);
                            _s_cc_to_remove.insert(i_cc_to_delete);
                        }
// // We add the current fine cell to the smallest neighbouring coarse cell!
                    }
                    set_tmp.erase(i_fc);


                }

                if (!untreated_fc.empty()) {

                    // Maybe untreated_fc are not connected?
                    if (_fc_graph.check_connectivity(untreated_fc)) {

                        // A fc alone:
                        if (untreated_fc.size() == 1) {

                            long i_fc = *untreated_fc.begin();//l[0];

//                            i_fc = untreated_fc.pop()

                            // We look for the fc that shares the maximum area with the current fc (i_fc)
                            long arg_fc_max = -1;
                            double max_area = 0.0;

                            const vector<long> &v_neighbours = _fc_graph.get_neighbours(i_fc);
                            const vector<double> &v_weights = _fc_graph.get_weights(i_fc);

                            assert(v_neighbours.size() == v_weights.size());

                            for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {
                                // We process every neighbour of i_fc
                                long i_fc_n = v_neighbours[i_n];
                                double i_w_fc_n = v_weights[i_n];

                                if ((i_fc_n != i_fc) && i_w_fc_n > max_area) {
                                    max_area = i_w_fc_n;
                                    arg_fc_max = i_fc_n;
                                }
                            }

                            // TODO make a swap function for anisotropic cells
                            // No swap here because we deal with only anisotropic fine cells!
                            // They are not registrered in Dicts...
                            // TODO Change this manage anisotropic cc
                            if (arg_fc_max != -1) {
                                long i_aniso_cc = _fc_2_cc[arg_fc_max];
                                // TODO use update????
                                // Update of self._d_card_2_cc:
                                unsigned short size = 1;
                                unsigned short compactness = 0;

                                // 1) We remove the cell from i_origin_cc
                                assert(_d_card_2_cc.count(size));
                                assert(_d_card_2_cc[size].count(i_cc));
                                assert(_d_isotropic_cc.count(i_cc));

                                assert(_d_compactness_2_cc.count(compactness));

                                _d_card_2_cc[size].erase(i_cc);
                                if (_d_card_2_cc[size].empty()) {
                                    _d_card_2_cc.erase(size);
                                }
                                _d_compactness_2_cc[compactness].erase(i_cc);
                                if (_d_compactness_2_cc[compactness].empty()) {
                                    _d_compactness_2_cc.erase(compactness);
                                }
                                // TODO TROUBlE we don't modify the cardinal of the host cell.
                                _d_isotropic_cc.erase(i_cc);

                                assert(_s_cc_to_remove.count(i_cc) == 0);
                                _s_cc_to_remove.insert(i_cc);

                                _fc_2_cc[i_fc] = i_aniso_cc;
                                // print(i_fc, i_aniso_cc)
                                _d_anisotropic_cc[i_aniso_cc].insert(i_fc);
//                                cout << "Treatment: " << i_fc << " is in anisotropic CC: " << i_aniso_cc << endl;
                            }

                        } else {
                            // Nothing to do the coarse cell already exists
                            // We have done our best!
                            if (_verbose) {
                                cout << "UntreatedCells are connected Pfff! {";
                                for (const long &i_fc :untreated_fc) {
                                    cout << i_fc << ", ";
                                }
                                cout << "} and original cell {";
                                for (const long &i_fc :l_fc_in_i_cc) {
                                    cout << i_fc << ", ";
                                }
                                cout << "}" << endl;
                            }
                        }


                    } else {
                        cout << "Problematic non connected untreated cells" << endl;
                        cout << "UntreatedCells {";
                        for (const long &i_fc :untreated_fc) {
                            cout << i_fc << ", ";
                        }
                        cout << "} and original cell {";
                        for (const long &i_fc :l_fc_in_i_cc) {
                            cout << i_fc << ", ";
                        }
                        cout << "}" << endl;
//                        print("l=", untreated_fc, "and original cell", l_fc_in_i_cc)
//                        raise ValueError("Problematic case! Untreated cells are not connected!")
                        exit(1);
                    }
                }

            }
        }
    }
}

void Coarse_Cell_Graph::cc_remove_deleted_cc(unordered_set<long> set_removed_cc) {

    /*
      Deletion of some cc and renumbering of all cc
      :param set_removed_cc: set of index of cc to remove
      :return: None
    */
    long new_cc;
    assert(_delayed_cc.size() > 0);
    if (set_removed_cc.size() > 0) {
        if (_d_isotropic_cc.size() > 0) {
            for (auto i_cc : set_removed_cc) {
                if (_d_isotropic_cc.count(i_cc) > 0) {
                    // Check that self._d_isotropic_cc[i_cc] is empty
                    cerr << "test " << _d_isotropic_cc.count(i_cc) << endl;
                    if (_d_isotropic_cc.count(i_cc) > 0)
                        cerr << "Cc" << i_cc << "is not empty : " << _d_isotropic_cc[i_cc]->get_s_fc().size() << endl;
                    assert(_d_isotropic_cc.count(i_cc) == 0);

                }
            }

            long min_cc = *min_element(set_removed_cc.begin(), set_removed_cc.end());
            unordered_map<long, long> dict_old_cc_to_new_cc;
            new_cc = min_cc;

            for (long i_cc = min_cc + 1; i_cc < _cc_counter; i_cc++) {
                if (_d_isotropic_cc.count(i_cc) > 0) {
                    dict_old_cc_to_new_cc[i_cc] = new_cc;
                    _d_isotropic_cc[new_cc] = _d_isotropic_cc[i_cc];
                    for (auto i_fc : _d_isotropic_cc[new_cc]->get_s_fc())
                        _fc_2_cc[i_fc] = new_cc;
                    unsigned short int i_cc_size = _d_isotropic_cc[i_cc]->__card;
                    _d_card_2_cc[i_cc_size].erase(i_cc);
                    _d_card_2_cc[i_cc_size].insert(new_cc);

                    unsigned short int compactness = _d_isotropic_cc[i_cc]->__compactness;

                    _d_compactness_2_cc[compactness].erase(i_cc);
                    _d_compactness_2_cc[compactness].insert(new_cc);

                    _d_isotropic_cc.erase(i_cc);
                    new_cc++;
                }
            }

        } else
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

void Coarse_Cell_Graph::cc_update_cc(unordered_set<long> set_of_fc_to_add,
                                     long i_target_cc) {
//Add (not yet agglomerated) fine cells to a already created cc of index i_target_cc.
//          Update a cc with some (not yet agglomerated) fine cells.

    if (is_isotropic_cc(i_target_cc)) {
        unsigned short int old_card = get_cc_cardinal(i_target_cc);
        Coarse_Cell *target_cc = _d_isotropic_cc[i_target_cc];
        unsigned short int old_deg_compactness = target_cc->__compactness;

        // update of the definition of the cc
        target_cc->add_fc(set_of_fc_to_add, _fc_2_cc);

        unsigned short int new_card = target_cc->__card;

        // Update of dict_Card_Coarse_Cells:
        _d_card_2_cc[new_card].insert(i_target_cc);

        assert(_d_card_2_cc.count(old_card) > 0 && _d_card_2_cc[old_card].count(i_target_cc) > 0);

        _d_card_2_cc[old_card].erase(i_target_cc);
        if (_d_card_2_cc[old_card].size() == 0)
            _d_card_2_cc.erase(old_card);

        if (_d_compactness_2_cc.size() > 0) {
            _d_compactness_2_cc[old_deg_compactness].erase(i_target_cc);
            if (_d_compactness_2_cc[old_deg_compactness].empty()) {
                _d_compactness_2_cc.erase(old_deg_compactness);
            }
            unsigned short int deg_compactness = target_cc->__compactness;
            _d_compactness_2_cc[deg_compactness].insert(i_target_cc);
        }
    } else {
        _d_anisotropic_cc[i_target_cc].insert(set_of_fc_to_add.begin(), set_of_fc_to_add.end());
    }

    // Update of the associated cc number the output of the current function agglomerate
    for (const long i_fc:set_of_fc_to_add) {

        assert(!_a_is_fc_agglomerated[i_fc]);
        _a_is_fc_agglomerated[i_fc] = true;  // Rk: initialized at false in agglomerate(...)
        _fc_2_cc[i_fc] = i_target_cc;
    }

    _nb_of_agglomerated_fc += set_of_fc_to_add.size();
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
    for (auto i_fc_neighbour: _fc_graph.get_neighbours(i_fc)) {
        if (i_fc != i_fc_neighbour) {
            long i_cc = _fc_2_cc[i_fc_neighbour];
            if (is_isotropic_cc(i_cc)) {
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

void Coarse_Cell_Graph::correction_swap_leaf_fc_v2(int nb_iteration, Coarse_Cell_Graph *ccg_l_m_one, int verbose) {

    //TODO remove ccg_l_m_one usage: no more MG correction!
    //TODO work only on border fc!!!
    unordered_set<long> set_removed_coarse_cells_l_m_one;
    int iteration(0);
    bool is_at_least_one_swap(true);

    while (iteration < nb_iteration && is_at_least_one_swap) {

        //TODO: improvement Work only on fc at the edge of cc...
        if (_verbose) {
            cerr << "iteration " << iteration << endl;
        }
        is_at_least_one_swap = false;
        int nb_of_swap(0);
        // TODO je crois qu'il le faisait dans un ordre aleatoire sur les fc dans MGridGen.
        //Process every isotropic coarse cells

        // conscious copy
        unordered_set<long> s_i_cc;
        for (const auto i_cc_k_v: _d_isotropic_cc)
            s_i_cc.insert(i_cc_k_v.first);

        for (const long i_cc:s_i_cc) {

            Coarse_Cell *cc = _d_isotropic_cc[i_cc];
            vector<long> v_s_fc;
            unordered_set<long> s_leaves = cc->compute_s_leaves();
            unordered_set<long> neigbours = cc->get_s_fc_w_outer_neighbours(2);
            unordered_set<long> s_fc;
            // compute s_fc = s_leaves&neigbours
            for (const long i_fc:s_leaves) {
                if (neigbours.count(i_fc)) {
                    s_fc.insert(i_fc);
                }

            }
//            set_intersection(s_leaves.begin(), s_leaves.end(), neigbours.begin(), neigbours.end(), v_s_fc.begin());
//            unordered_set<long> s_fc(v_s_fc.begin(), v_s_fc.end());
            for (const long i_fc : s_fc) {

                unordered_map<long, unordered_set<long>> d_n_fc = compute_dict_faces_in_common_faces_with_cc_neighbourhood(i_fc);
                // sort wrt number of edge
                // TODO sorting... you can't sort a map!
                vector<pair<long, unordered_set<long>>> d_n_fc_v(d_n_fc.begin(), d_n_fc.end());
                // can't sort the map?
                sort(d_n_fc_v.begin(), d_n_fc_v.end(), compare_size);

                vector<long> l_keys_i_cc;
                for (const auto d_n_fc_k_v : d_n_fc_v) {
                    l_keys_i_cc.push_back(d_n_fc_k_v.first);
                }

                unsigned short i_last_key = l_keys_i_cc.size() - 1;
                auto it = find(l_keys_i_cc.begin(), l_keys_i_cc.end(), i_cc);
                int index = distance(l_keys_i_cc.begin(), it);
                if (d_n_fc.count(i_cc) == 0 || index < i_last_key) {

                    /* Is there a better cc for i_fc.
                       First condition, a cc of size one.
                       Second condition, i_cc is not the best cc for i_fc : l_keys_i_cc[i_last_key] is better*/
                    long argmax_nb_common_faces_n = l_keys_i_cc[i_last_key];
                    unsigned short max_nb_common_faces_n = d_n_fc[l_keys_i_cc[i_last_key]].size();
                    unsigned short min_size(1);
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

                        if (ccg_l_m_one != NULL) {
                            long i_cc_l_m_one = ccg_l_m_one->_fc_2_cc[i_fc];
                            unordered_set<long> s_cc;
                            for (auto i_fc_n: d_n_fc[l_keys_i_cc[i_last_key]])
                                s_cc.insert(ccg_l_m_one->_fc_2_cc[i_fc_n]);
                            if (s_cc.size() == 1) {
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

                            } else {
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
                        nb_of_swap++;
                        is_at_least_one_swap = true;
                        // assert ccg_l_m_one.check_data_consistency_and_connectivity()
                        // assert self.check_data_consistency_and_connectivity()
                    }

                }
            }
        }
        iteration++;
        if (_verbose) {
            cerr << "nb_of_swap = " << nb_of_swap << endl;
            cerr << "After iteration" << iteration << endl;
            // cerr << "Dict_distribution_of_cardinal_of_cc" << d_distribution_of_cardinal_of_isotropic_cc << endl;
        }
    }
    if (set_removed_coarse_cells_l_m_one.size() > 0) {
        cerr << "set_removed_coarse_cells_l_m_one ";
        for (const long elt : set_removed_coarse_cells_l_m_one)
            cerr << elt << " ";
        cerr << endl;
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


bool Coarse_Cell_Graph::is_cc_grid_not_structured(short goal_card) {

    // All fc are agglomerated
    assert(_nb_of_agglomerated_fc == _fc_graph.number_of_cells);

    if (!_d_anisotropic_cc.empty()) {
        return true;
    } else {
        short goal_compactness = _fc_graph._dimension;

        for (const auto &i_k_v :_d_isotropic_cc) {
            if ((*i_k_v.second).__compactness != goal_compactness) {
                return true;
            }
        }
        if (!_delayed_cc.empty()) {
            return true;
        }
        return _fc_graph.s_anisotropic_compliant_cells.empty();
    }
}

void Coarse_Cell_Graph::correction_main_triconnected(short min_neighbourhood_correction_step,
                                                     short goal_card,
                                                     int verbose) {
    // Not yet implemented
}


void Coarse_Cell_Graph::print_d_distribution_of_cardinal_of_isotropic_cc() {

    unordered_map<unsigned short, long> d_dist = compute_d_distribution_of_cardinal_of_isotropic_cc();
    cout << "{ ";
    for (auto &i_k_v : d_dist) {
        cout << i_k_v.first << ": " << i_k_v.second << ", ";
    }
    cout << "}" << endl;
}
