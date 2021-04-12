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


long Coarse_Cell_Graph::cc_create_a_cc(unordered_set<long> &s_fc,
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

    if (_d_isotropic_cc.count(i_cc)){
        return (*_d_isotropic_cc[i_cc]).__compactness;
    }
    else{
        unsigned short int c = _fc_graph.compute_min_fc_compactness_inside_a_cc(_d_anisotropic_cc[i_cc]);
        if(c != (unsigned short int)(_d_anisotropic_cc[i_cc].size() > 1)) {
            cerr<<"Warning anisotropic CC of compactness >1"<< c<<endl;
        }
        return (unsigned short int)((_d_anisotropic_cc[i_cc].size()) > 1);
    }


}