//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Agglomerator.h"


Agglomerator::Agglomerator(Dual_Graph &graph,
                           unsigned short int verbose,
                           bool is_visu_data_stored,
                           int dimension,
                           bool checks
) : __verbose(verbose), __checks(checks), __dimension(dimension), __fc_graphs(graph) {

    if (((*this).__dimension != 2) && ((*this).__dimension != 3)) {
        cerr << "Wrong definition of dimension !=2 and !=3" << endl;
        assert(((*this).__dimension == 2) || ((*this).__dimension == 3));
    }
    if ((*this).__dimension == 2) {
        __min_neighbourhood = 2;
    } else {
        __min_neighbourhood = 3;
    }


// for every defined level (1 by default), contains the number of cells
// e.g. (*this).__l_nb_of_cells[0]= number of cells on finest level
//      (*this).__l_nb_of_cells[1]= number of cells on the first coarse level
    (*this).__l_nb_of_cells.push_back(graph.number_of_cells);

    __cc_graphs = NULL;

    // Anisotropic agglomeration datas:
    // For every level, we have a set containing the admissible cells for anisotropy cell number:
    // For level 0, it is the cell number of prism or hexahedron ...
    __v_of_s_anisotropic_compliant_fc = {};
    __v_nb_lines = {};
    __v_lines = {};

    //=================
    // Visualization:
    //=================
    (*this).__is_visu_data_stored = is_visu_data_stored;

}


void Agglomerator::_set_agglomeration_parameter(
        bool is_anisotropic,
        string kind_of_agglomerator,
        short goal_card,
        short min_card,
        short max_card) {

    __is_anisotropic = is_anisotropic;
    if (__is_anisotropic) {
        __v_of_s_anisotropic_compliant_fc = vector<unordered_set<long>>(2);
        __v_of_s_anisotropic_compliant_fc[0] = __fc_graphs.s_anisotropic_compliant_cells;
        __v_nb_lines = vector<unsigned long>(2);
        __v_lines = vector<forward_list<deque<long> *> >(2);
    }

    __kind_of_agglomerator = kind_of_agglomerator;
    // print("Call of agglomerator {}".format((*this).__kind_of_agglomerator))
    unordered_map<unsigned short int, unsigned short int> d_default_min_card = {{2, 3},
                                                                                {3, 6}};
    unordered_map<unsigned short int, unsigned short int> d_default_max_card = {{2, 5},
                                                                                {3, 10}};
    unordered_map<unsigned short int, unsigned short int> d_default_goal_card = {{2, 4},
                                                                                 {3, 8}};
    unordered_map<unsigned short int, unsigned short int> d_default_threshold_card = {{2, 2},
                                                                                      {3, 3}};

    // Definition of (*this).__min_card
    if (min_card == -1) {
        (*this).__min_card = d_default_min_card[(*this).__dimension];
    } else {
        (*this).__min_card = min_card;
    }

    // Definition of (*this).__max_card
    if (max_card == -1) {
        (*this).__max_card = d_default_max_card[(*this).__dimension];
    } else {
        (*this).__max_card = max_card;
    }

    // Definition of (*this).__goal_card
    if (goal_card == -1) {
        __goal_card = d_default_goal_card[(*this).__dimension];
    } else {
        __goal_card = goal_card;
    }

    // Definition of (*this).__threshold_card
    (*this).__threshold_card = d_default_threshold_card[(*this).__dimension];
}


void Agglomerator::__agglomerate_one_level(
        vector<long> debug_only_fc_to_cc,
        short int debug_only_steps // arbitrary value greater than 7
) {

    /**
     * Agglomerate one level
     */

    if (!debug_only_fc_to_cc.empty()) {
        assert(debug_only_steps == -1);
    }
    Coarse_Cell_Graph *ccg;
    if (debug_only_fc_to_cc.empty()) {

        // General case
        // Creation of the CCG
        __cc_graphs = new Coarse_Cell_Graph(__fc_graphs);


        // TODO On pourrait ne l'allouer qu'une seule fois!

        // Is the agglomeration anisotropic?
        if (__is_anisotropic) {
            _agglomerate_one_level_anisotropic_part();
        }

        _agglomerate_one_level_isotropic_part(debug_only_steps);
    } else {

        // For debug purpose:
        __cc_graphs = new Coarse_Cell_Graph(__fc_graphs, __verbose, debug_only_fc_to_cc);
        (*__cc_graphs).fill_cc_neighbouring();
        __l_nb_of_cells.push_back((*__cc_graphs)._cc_counter);
    }
}


unordered_set<long> Agglomerator::_choose_optimal_cc_and_update_seed_pool(Coarse_Cell_Graph &cc_graph,
                                                                          const long &seed,
                                                                          unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                                          const unsigned short goal_card,
                                                                          const unsigned short max_card,
                                                                          string kind_of_agglomerator,
                                                                          unsigned short &compactness,
                                                                          bool is_order_primary,
// _correction_split_too_big_cc_in_two(...)
                                                                          bool increase_neighbouring,
// _correction_split_too_big_cc_in_two(...)
                                                                          unordered_set<long> set_of_fc_for_current_cc

) {
    /**
     * The goal of this function is to choose from a pool of neighbour the better one to build a compact coarse cell
     * Called by :
     * - self._create_optimal_cc_v2(...)
     * - self._correction_split_too_big_cc_in_two(...)
     * - self._correction_triconnected_sub(...)
     * :param is_order_primary: modify the order of priority for agglomeration. False is the default value and the
    number of face in common is primary. True, the order of neighbourhood is primary
     */
    _set_agglomeration_parameter(false, kind_of_agglomerator, goal_card, -1, max_card);
//    graph = cc_graph._fc_graph
    // Definition of the current cc
    // remark: does contain seed
    unordered_set<long> s_current_cc;

    // l_fc_added with chronological order
// remark: does not contain seed


    if (kind_of_agglomerator == "basic") {

        s_current_cc = __choose_optimal_cc_basic_v2_sub(seed,
                                                        dict_neighbours_of_seed,
                                                        compactness,
                                                        is_order_primary);
    } else if (kind_of_agglomerator == "triconnected") {
//        s_current_cc = __choose_optimal_cc_triconnected(cc_graph,
//                                                        seed,
//                                                        dict_neighbours_of_seed,
//                                                        goal_card,
//                                                        max_card,
//                                                        compactness,
//                                                        increase_neighbouring,
//                                                        set_of_fc_for_current_cc);
        cout << " Not yet implemented!" << endl;
        exit(1);
    } else {
        exit(1);
    }
// Remark: chosen CC may not be exactly of size self.__goal_card
//
//===========================
// Create of list_of_seeds:
//===========================
    list<long> l_of_new_seed = __create_list_of_seeds(cc_graph, seed, dict_neighbours_of_seed, s_current_cc);

    //===========================
    // Update of list_of_seeds:
    //===========================
    (*cc_graph._fc_graph.seeds_pool).update(l_of_new_seed);

    return s_current_cc;
}

// TODO rename this afterwards.
unordered_set<long> Agglomerator::_choose_optimal_cc_and_update_seed_pool_v2(const long seed,
                                                                             unsigned short &compactness,
                                                                             bool is_order_primary, // _correction_split_too_big_cc_in_two(...)
                                                                             bool increase_neighbouring// _correction_split_too_big_cc_in_two(...)
) {


    /**
     * The goal of this function is to choose from a pool of neighbour the better one to build a compact coarse cell
     * Called by :
     * - self._create_optimal_cc_v2(...)
     * - self._correction_split_too_big_cc_in_two(...)
     * - self._correction_triconnected_sub(...)
     * :param is_order_primary: modify the order of priority for agglomeration. False is the default value and the
     *  number of face in common is primary. True, the order of neighbourhood is primary
     */
    assert(__goal_card != -1);  // self.__goal_card has been initialized

    // Definition of the current cc
    // remark: does contain seed
    unordered_set<long> s_current_cc = {};
    unordered_map<long, unsigned short> d_n_of_seed;
    if (__kind_of_agglomerator == "basic") {
        s_current_cc = __choose_optimal_cc_basic_v2(seed,
                                                    d_n_of_seed,
                                                    compactness,
                                                    is_order_primary);
//        cout << "_choose_optimal_cc_and_update_seed_pool_v2 s_current_cc.size() " << s_current_cc.size() << endl;
//        cout<<"Not yet implemented:__choose_optimal_cc_basic_v2(cc_graph, seed,is_order_primary);"<<endl;
//        exit(1);
    } else if (__kind_of_agglomerator == "triconnected") {
        s_current_cc = __choose_optimal_cc_triconnected_v2(seed,
                                                           d_n_of_seed,
                                                           compactness,
                                                           increase_neighbouring);
    } else {
        exit(1);
    }
    // Remark: chosen CC may not be exactly of size self.__goal_card

    //====================================
    // Create of l_of_new_seeds:
    //====================================
    list<long> l_of_new_seeds = __create_list_of_seeds(*__cc_graphs, seed, d_n_of_seed, s_current_cc);

    //====================================
    // Update of list_of_seeds:
    //====================================
    (*__fc_graphs.seeds_pool).update(l_of_new_seeds);

    return s_current_cc;
}

unordered_set<long> Agglomerator::__choose_optimal_cc_basic_v2_sub(
        const long seed,
        unordered_map<long, unsigned short> &dict_neighbours_of_seed,
        unsigned short &compactness,
        const bool is_order_primary) {

    // Number of fine cells constituting the current coarse cell in construction.
    unsigned short size_current_cc = 1; // CC contains only one cell: the seed

    // set of fc for current cc:
    unordered_set<long> s_of_fc_for_current_cc = {seed};

    // On dit que les voisins sont OK
    // (pas de test pour l'instant sur leur pertinence: wall Far field, aspect Ratio...)
    // On les declare visite/agglomere
    // Rq: Mavriplis ne fait pas d'optimisation de choix dans les cellules fines!
    // C'est fait seulement dans le choix de la seed

    // Tant que la cellule agglomeree courante n'est pas complete
    // On a trop de voisin pour pas assez de place dans l'element grossier en construction.
    // while size_current_cc + len(listOfNeighborsForAgglomeration) < self.__maximumSizeOfAgglomeratedElement:

    // We build the cc by adding to the seed the fc that minimizes the AR at every step (local optimization)
    unsigned short min_size = __goal_card;

    // Computation of the initial aspect ratio: we need cc_surf: i.e. the external area (perimeter in 2D and sum of external faces in 3D) and volume
    double cc_surf = 0.0;
    vector<double> a_neighbours_weights = __fc_graphs.get_weights(seed);
    unsigned short nb_neighbours = __fc_graphs.get_nb_of_neighbours(seed);
    for (const double &w:a_neighbours_weights) {
        cc_surf += w;
    }

    // volume of cc is at first the volume of the seed.
    double vol_cc = __fc_graphs._volumes[seed];

    // This dictionary is used to store the eligible cc: i.e. its size is inside the permitted range.
    // This is useful to track back our step if needed.

    unordered_map<unsigned short, pair<unordered_set<long>, unordered_map<long, int>>> dict_cc_in_creation;
    //    Dict[int, List[Set[int], Dict[int, int]]]
    // dict_cc_in_creation[size_current_cc] = [set(s_of_fc_for_current_cc),
    //                                         {index of the last added fc: order (neighbourhood) of the last added fc}]
    double min_external_faces = numeric_limits<double>::max();
    unsigned short arg_min_external_faces = min_size;

    unsigned short max_ind = min(__max_card, (short) (dict_neighbours_of_seed.size() + 1));
    int number_of_external_faces_current_cc = nb_neighbours + (*__fc_graphs.seeds_pool).boundary_value(seed) - 1;
    // print("\nseed=", seed)

    unordered_set<long> s_neighbours_of_seed = d_keys_to_set(dict_neighbours_of_seed);

    First_Order_Neighbourhood f_o_neighbourhood = First_Order_Neighbourhood(s_neighbours_of_seed);

    vector<long> v = __fc_graphs.get_neighbours(seed);
    unordered_set<long> s_up(v.begin(), v.end());

    unordered_set<long> fon = f_o_neighbourhood.update(seed, s_up);

    // Choice of the fine cells to agglomerate
    while (size_current_cc < max_ind) {

        long argmin_ar = -1;
        double min_ar_surf = numeric_limits<double>::max();
        double min_ar_vol = numeric_limits<double>::max();
        unsigned short max_faces_in_common = 0;

        __compute_best_fc_to_add(__fc_graphs,
                                 fon,
                                 dict_neighbours_of_seed,
                                 is_order_primary,
                                 cc_surf,
                                 vol_cc,
                                 s_of_fc_for_current_cc,
                                 argmin_ar,
                                 max_faces_in_common,
                                 min_ar_surf,
                                 min_ar_vol);


        number_of_external_faces_current_cc += __fc_graphs.get_nb_of_neighbours(argmin_ar)
                                               + (*__fc_graphs.seeds_pool).boundary_value(argmin_ar) - 1 - 2 * max_faces_in_common;

        size_current_cc++;
        s_of_fc_for_current_cc.insert(argmin_ar);

        // if the constructed cc is eligible, i.e. its size is inside the permitted range
        // we store it inside dict_cc_in_creation
        // This choice is based on the idea that the smallest cc (w.r.t. card) is may be not the one that minimized
        // the number of external faces.
        if ((min_size <= size_current_cc) || size_current_cc == max_ind) {

            if (number_of_external_faces_current_cc <= min_external_faces) {

                min_external_faces = number_of_external_faces_current_cc;
                arg_min_external_faces = size_current_cc;
            }

            unordered_map<long, int> new_dict;
            new_dict[argmin_ar] = dict_neighbours_of_seed[argmin_ar];

            pair<unordered_set<long>, unordered_map<long, int>> p = make_pair(s_of_fc_for_current_cc, new_dict);
            dict_cc_in_creation[size_current_cc] = p;

//            dict_cc_in_creation[size_current_cc] = [
//                    set(s_of_fc_for_current_cc),
//                        {
//                            argmin_ar:
//                            dict_neighbours_of_seed[argmin_ar]
//                        }];
        }

        // Update of cc_surf and vol_cc with the new fc added
        cc_surf = min_ar_surf;
        vol_cc = min_ar_vol;

        // Remove added fc from the available neighbours
        dict_neighbours_of_seed.erase(argmin_ar);

        vector<long> v = __fc_graphs.get_neighbours(argmin_ar);
        unordered_set<long> s_up(v.begin(), v.end());
        fon = f_o_neighbourhood.update(argmin_ar, s_up);
    }
    s_of_fc_for_current_cc = dict_cc_in_creation[arg_min_external_faces].first;

    // If we do not chose the biggest cc, we put the useless fc back to the pool
    for (long i_s = arg_min_external_faces + 1; i_s < max_ind + 1; i_s++) {
        // for all size of Cell from arg_min_external_faces+1 to  min(max_card, len(dict_neighbours_of_seed) + 1) + 1
        //dict_neighbours_of_seed.
        //            update(dict_cc_in_creation[i_s][1])
        // Merge/update:
        for (auto iKV:dict_cc_in_creation[i_s].second) {
            dict_neighbours_of_seed[iKV.first] = iKV.second;
        }


        // print("arg_min_external_faces + 1", arg_min_external_faces + 1, "max_ind + 1", max_ind + 1)
        // raise ValueError
    }

    assert(arg_min_external_faces == s_of_fc_for_current_cc.size());
    compactness = __fc_graphs.compute_min_fc_compactness_inside_a_cc(s_of_fc_for_current_cc);
    return s_of_fc_for_current_cc;
}


list<long> Agglomerator::__create_list_of_seeds(const Coarse_Cell_Graph &cc_graph,
                                                const long &seed,
                                                const unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                unordered_set<long> s_current_cc) {
    // TODO Change type return? vector?
    // TODO test this

    //===========================
    // Update of list_of_seeds:
    //===========================
    //   a) with dict_neighbours_of_seed or
    //   b) by computing the neighbourhood of the current CC.

    list<long> l_of_new_seed;
    if (!dict_neighbours_of_seed.empty()) {

        unsigned short size = dict_neighbours_of_seed.size();
//        l_of_new_seed.resize(size);
        // if dict_neighbours_of_seed is not empty
        // Reminder: dict_neighbours_of_seed is here the pool of cell neighbouring the previous seed!
        for (auto &i_k_v : dict_neighbours_of_seed) {
            if (i_k_v.second <= 2) {
                l_of_new_seed.push_back(i_k_v.first);
            }
        }

        unsigned short i_k = 3;
        while (l_of_new_seed.empty()) {
            // We put FC in the l_of_new_seed according to its order of neighbouring of previous seed.
            // order 1 or 2 then 3, then 4 and so on.
            for (auto &i_k_v : dict_neighbours_of_seed) {
                if (i_k_v.second <= i_k) {
                    l_of_new_seed.push_back(i_k_v.first);
                }
            }
            i_k++;
        }
    } else {
        // else dict_neighbours_of_seed is empty: we used every neighbour!
        // if list_of_seeds is empty, we look for new seeds to add to the list_of_seeds.
        if ((*cc_graph._fc_graph.seeds_pool).is_empty()) {
            // if list_of_seeds is empty
            // we look if there is some neighbour to the current fc:
            // s_fc_neighbours_of_cc = set()
            // we remove seed because we already update its first neighbours.
            unordered_set<long> tmp_set(s_current_cc);  // copy needed because the set is used inside ccg
            tmp_set.erase(seed);

            // We add to s_fc_neighbours_of_cc all the neighbours of FC included in s_fc_for_current_cc without seed
            for (auto &i_fc: tmp_set) {
                vector<long> a_neighbours = cc_graph._fc_graph.get_neighbours(i_fc);
                for (const long &i_fc_n: a_neighbours) {
                    if (cc_graph.is_fc_not_already_agglomerated(i_fc_n)) {
                        // _a_is_fc_agglomerated is up-to-date.
                        // remark: we don't care i_fc_n == i_fc.
                        l_of_new_seed.push_back(i_fc_n);
                    }
                }
            }
        }
    }
    return l_of_new_seed;
}

void Agglomerator::__compute_best_fc_to_add(Dual_Graph &graph,
                                            unordered_set<long> fon,
                                            const unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                            const bool &is_order_primary,
                                            const double &cc_surf,
                                            const double &vol_cc,
                                            const unordered_set<long> &s_of_fc_for_current_cc,
                                            long &argmin_ar,
                                            unsigned short &max_faces_in_common,
                                            double &min_ar_surf,
                                            double &min_ar_vol
) {


    double min_ar = numeric_limits<double>::max();
    long arg_max_faces_in_common = -1;

    // For every fc in the neighbourhood:
    // we update the new aspect ratio
    // verifier que fon est un sous ensemble de dict_neighbours_of_seed
    // assert fon.issubset(dict_neighbours_of_seed.keys())
    for (const long &i_fc: fon) {  // we test every possible new cells to chose the one that locally

        // minimizes the Aspect Ratio.
        if (arg_max_faces_in_common == -1) {
            arg_max_faces_in_common = i_fc;
        }

        // update of the vol
        double new_ar_vol = vol_cc + graph._volumes[i_fc];

        unsigned short number_faces_in_common = 0;
        bool is_fc_adjacent_to_any_cell_of_the_cc = false;
        double new_ar_surf = cc_surf;

        __compute_ar_surf_adjacency_nb_face_common(graph, i_fc, s_of_fc_for_current_cc,
                                                   new_ar_surf,
                                                   is_fc_adjacent_to_any_cell_of_the_cc,
                                                   number_faces_in_common);

//        cout << i_fc << "\t" << new_ar_surf << "\t" << is_fc_adjacent_to_any_cell_of_the_cc << "\t" << number_faces_in_common <<
//             endl;
        double new_ar = pow(new_ar_surf, 1.5) / new_ar_vol;

        // TODO useful???
        const unsigned short &order = dict_neighbours_of_seed.at(i_fc); // [i_fc] is not const.

// TODO This version seems good but refactorisation to do: perhaps it is not needed to update every new possible coarse cell aspect ratio?
// TODO also need to remove the list of min_ar, argmin_ar, etc.
        if (number_faces_in_common >= max_faces_in_common or is_order_primary) {  // if is_order_primary is True the order of
// neighbourhood is primary
            if (number_faces_in_common == max_faces_in_common or is_order_primary) {

                if (order <= dict_neighbours_of_seed.at(arg_max_faces_in_common)) {  // [arg_max_faces_in_common] is not const.
                    if (order == dict_neighbours_of_seed.at(arg_max_faces_in_common)) {
                        if (new_ar < min_ar and is_fc_adjacent_to_any_cell_of_the_cc) {
// The second condition asserts the connectivity of the coarse element.
                            min_ar = new_ar;
                            argmin_ar = i_fc;
                            min_ar_surf = new_ar_surf;
                            min_ar_vol = new_ar_vol;

                            arg_max_faces_in_common = i_fc;
// The number of face in common is the same no need to touch it
                        }
                    } else {
// Case :number_faces_in_common == max_faces_in_common and order < dict_neighbours_of_seed[arg_max_faces_in_common]:
                        arg_max_faces_in_common = i_fc;
                        min_ar = new_ar;
                        argmin_ar = i_fc;
                        min_ar_surf = new_ar_surf;
                        min_ar_vol = new_ar_vol;
// The number of face in common is the same no need to touch it
                    }
                }
            } else {
// Case :number_faces_in_common > max_faces_in_common:
                max_faces_in_common = number_faces_in_common;
                arg_max_faces_in_common = i_fc;
                min_ar = new_ar;
                argmin_ar = i_fc;
                min_ar_surf = new_ar_surf;
                min_ar_vol = new_ar_vol;
            }
        }
    }
}

unordered_set<long> Agglomerator::__choose_optimal_cc_triconnected_v2(long seed,
                                                                      unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                                      unsigned short &compactness,
                                                                      const bool increase_neighbouring,
                                                                      short min_neighbourhood) {
    if (__verbose) {
        cout << "__choose_optimal_cc_triconnected_v2" << endl;
    }

    // set of fc for current cc:
    unordered_set<long> s_fc_for_current_cc = {seed};

//    graph = cc_graph._fc_graph
//    test_func = cc_graph.is_fc_not_already_agglomerated

    short deg_seed = __fc_graphs.compute_degree_of_node_not_a(seed, (*__cc_graphs)._a_is_fc_agglomerated);

// If seed is a leaf, we look for a new seed in its neighbourhood.
//================================================================
    long old_seed = -1;
    unordered_set<long> s_old_seed;

// TODO Is it wise?
// if deg_seed<self._dimension:
    if (deg_seed == 1) {

        while (deg_seed == 1) {

// We get the only neighbour if it is not already agglomerated:
            for (const long i_n : __fc_graphs.get_neighbours(seed)) {
                if (i_n != seed && (*__cc_graphs).is_fc_not_already_agglomerated(i_n)) {

                    old_seed = seed;
                    s_old_seed.insert(seed);
// we artificially say that the old seed is agglomerated. It will be remove later.
                    (*__cc_graphs)._a_is_fc_agglomerated[old_seed] = true;

                    seed = i_n;  // i_n is the new seed
                    break;
                }
            }
            deg_seed = __fc_graphs.compute_degree_of_node_not_a(seed, (*__cc_graphs)._a_is_fc_agglomerated);
//            deg_seed = graph.compute_degree_of_node(seed, test_func)
            if (deg_seed == 0) {

                for (long i_fc:s_old_seed) {
                    (*__cc_graphs)._a_is_fc_agglomerated[i_fc] = false;
                    s_fc_for_current_cc.insert(i_fc);  // s_fc_for_current_cc.update(s_old_seed)
                }
                s_fc_for_current_cc.insert(seed);  // add new_seed

                compactness = short(old_seed != -1);
                dict_neighbours_of_seed.clear();
                return s_fc_for_current_cc;

            } else {
//                cout << "attention diff there was a s_fc_for_current_cc.pop()" << endl;
                s_fc_for_current_cc.erase(old_seed);  // pop the old_seed
                s_fc_for_current_cc.insert(seed);  // add new_seed
            }
        }
    }

// Compute self.__min_neighbourhood order neighbourhood:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// La il faut prendre un voisinage plus petit que 3 pour le niveau grossier >=2 car on agglomere beaucoup beaucoup.
//
// Puis ajouter les corrections pour optimiser l'aspect ratio'
    unsigned short max_order_of_neighbourhood = __min_neighbourhood;
//    d_n_of_seed, max_order_of_neighbourhood =
    __fc_graphs.compute_neighbourhood_of_cc(s_fc_for_current_cc,
                                            max_order_of_neighbourhood,   //in and out
                                            dict_neighbours_of_seed,
                                            __max_card,
                                            (*__cc_graphs)._a_is_fc_agglomerated);

    if (!s_old_seed.empty()) {
        for (const long i_fc:s_old_seed) {
            (*__cc_graphs)._a_is_fc_agglomerated[i_fc] = false;
        }
    }

// If no neighbour is found for seed: this case happened only when isotropic cell is surrounded
// by anisotropic cells.
    if (dict_neighbours_of_seed.empty()) {

// dict_neighbours_of_seed is empty, i.e: the cc is not complete
// and no neighbour are available!
        compactness = 0;
        return s_fc_for_current_cc;
    }

// TODO remove this
// A neighbourhood is found but it is too small!
    if (dict_neighbours_of_seed.size() + s_fc_for_current_cc.size() < __goal_card) {

        // Not enough available neighbours: creation of a (too small) coarse cell.

        // s_fc_for_current_cc.update(d_n_of_seed)
        for (const auto i_k_v: dict_neighbours_of_seed) {
            s_fc_for_current_cc.insert(i_k_v.first);
        }

        // s_fc_for_current_cc.update(s_old_seed)
        for (const long i_fc: s_old_seed) {
            s_fc_for_current_cc.insert(i_fc);
        }

        bool is_creation_delayed = s_fc_for_current_cc.size() <= __threshold_card;
        if (is_creation_delayed) {
            dict_neighbours_of_seed.clear();
            compactness = 0;
            return s_fc_for_current_cc;
        } else {
            // TODO clean this: only one return!!!
            dict_neighbours_of_seed.clear();
            compactness = __dimension;
            return s_fc_for_current_cc;
        }

// TODO deal with that?
// return s_fc_for_current_cc, graph.compute_min_fc_compactness_inside_a_cc(s_fc_for_current_cc)
    }

    if (__dimension == 2) {
//        s_fc_for_current_cc, compactness, d_n_of_seed = self.__choose_optimal_cc_triconnected_2D(graph,
//                                                                                                 cc_graph,
//                                                                                                 seed,
//                                                                                                 dict_neighbours_of_seed,
//                                                                                                 increase_neighbouring,
//                                                                                                 min_neighbourhood)
        s_fc_for_current_cc = __choose_optimal_cc_triconnected_2D(seed,
                                                                  dict_neighbours_of_seed,
                                                                  compactness,
                                                                  increase_neighbouring,
                                                                  min_neighbourhood);
    } else {

//        s_fc_for_current_cc, compactness, d_n_of_seed = self.__choose_optimal_cc_triconnected_3D(graph,
//                                                                                                 cc_graph,
//                                                                                                 seed,
//                                                                                                 d_n_of_seed,
//                                                                                                 increase_neighbouring,
//                                                                                                 s_fc_for_current_cc)
//        cout << "triconnected_3D is not yet implemented!" << endl;
        s_fc_for_current_cc = __choose_optimal_cc_triconnected_2D(seed,
                                                                  dict_neighbours_of_seed,
                                                                  compactness,
                                                                  increase_neighbouring,
                                                                  min_neighbourhood);
    }
    if (__verbose) {
        cout << "__choose_optimal_cc_triconnected_v2 seed= " << seed << "d_n_of_seed: {";
        for (auto i_k_v : dict_neighbours_of_seed) {
            cout << "{" << i_k_v.first << ", " << i_k_v.second << "}, ";
        }
        cout << "}" << endl;
    }
// print("__choose_optimal_cc_triconnected_v2:", seed, "d_n_of_seed", d_n_of_seed)

// Post treatment of old_seed
    if (!s_old_seed.empty()) {
        for (const long i_fc:s_old_seed) {
            if (dict_neighbours_of_seed.count(i_fc)) {
                dict_neighbours_of_seed.erase(i_fc);
            }
//s_fc_for_current_cc.update(s_old_seed);
            s_fc_for_current_cc.insert(i_fc);
        }
// be careful here that compactness is not correct, but it is a way to force creation.
        compactness = __dimension;
    }

// print("seed", seed, "old_seed", old_seed, s_fc)
    return s_fc_for_current_cc;//, compactness, d_n_of_seed
}


void Agglomerator::initialize_l_cc_graphs_for_tests_only(Coarse_Cell_Graph *ccg) {
//    __l_cc_graphs.push_back(ccg);
    __cc_graphs = ccg;
}


void Agglomerator::__compute_ar_surf_adjacency_nb_face_common(
        const Dual_Graph &graph,
        const long &i_fc,
        const unordered_set<long> &s_of_fc_for_current_cc,
        double &new_ar_surf,
        bool &is_fc_adjacent_to_any_cell_of_the_cc,
        unsigned short &number_faces_in_common) const {

    // Computation of the new aspect ratio of the tested coarse element
    vector<long> v_neighbours = graph.get_neighbours(i_fc);
    vector<double> v_weights = graph.get_weights(i_fc);
    assert(v_neighbours.size() == v_weights.size());

    for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {

        long i_fc_n = v_neighbours[i_n];
        double i_w_fc_n = v_weights[i_n];

        if (i_fc_n == i_fc) {  // Boundary surface
            new_ar_surf += i_w_fc_n;
        } else if (s_of_fc_for_current_cc.count(i_fc_n) == 0) {
            new_ar_surf += i_w_fc_n;
        } else {
            is_fc_adjacent_to_any_cell_of_the_cc = true;
            new_ar_surf -= i_w_fc_n;
            number_faces_in_common++;
        }
    }

}

void Agglomerator::agglomerate_one_level(const bool is_anisotropic,
                                         unsigned long nb_aniso_agglo_lines,
                                         forward_list<deque<long> *> &anisotropic_lines,
                                         string kind_of_agglomerator,
                                         const short goal_card,
                                         const short min_card,
                                         const short max_card,
                                         vector<long> debug_only_fc_to_cc,
                                         const short debug_only_steps
) {
    cout << "\nCall of Agglomerator::agglomerate_one_level " << endl;
    cout << "is_anisotropic " << is_anisotropic << endl;
    cout << "goal_card " << goal_card << endl;
    cout << "min_card " << min_card << endl;
    cout << "max_card " << max_card << endl;
    /**
     * Main function of the agglomerator.
     *
     * For the current zone, we agglomerate fine cells to create coarse cells.
     * :param nb_of_coarse_level: number of coarse level to create
     * :param is_anisotropic: do we want an anisotropic agglomeration (True) or an isotropic (False)?
     * :param kind_of_agglomerator: do we want a basic agglomeration or a triconnected one?
     * :param goal_card: goal cardinal of the coarse cells
     * :param min_card: minimum cardinal of the coarse cells
     * :param max_card: maximum cardinal of the coarse cells
     */
    _set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);
    cout << "__goal_card " << __goal_card << endl;
    cout << "__min_card " << __min_card << endl;
    cout << "__max_card " << __max_card << endl;

    if (!debug_only_fc_to_cc.empty()) {

        // For debug purpose only:
        assert(nb_aniso_agglo_lines == 0);  // Up to now we do not manage anisotropic lines for debugging.
        __agglomerate_one_level(debug_only_fc_to_cc);

    } else {
        if (__is_anisotropic) {
            __v_nb_lines[0] = nb_aniso_agglo_lines;
            __v_lines[0] = anisotropic_lines;

            unsigned long nb_agglomeration_lines = 0;
            forward_list<deque<long> *>::iterator fLIt;
            for (fLIt = __v_lines[0].begin(); fLIt != __v_lines[0].end(); fLIt++) {
                nb_agglomeration_lines++;
            }
            assert(nb_agglomeration_lines == __v_nb_lines[0]);
        }

        __agglomerate_one_level(debug_only_fc_to_cc,
                                debug_only_steps);
    }

    assert((*__cc_graphs).check_cc_consistency());

//    if (agglo_lines_l != NULL) {
//
//        agglo_lines_l_m_one_array_idx, agglo_lines_l_m_one_array = convert_agglo_lines_to_agglomeration_lines_arrays(nb_lines_l, aniso_lines_l_m_one)
//
//        convert_agglo_lines_to_agglomeration_lines_arrays(
//        const long &nb_fc,
//        const unsigned long &nb_lines,
//        forward_list<deque<long> *> *agglo_lines,
//        long *sizes,
//        long *agglo_lines_array_idx,
//        long *agglo_lines_array);
//    }
//
//    if (agglo_lines_l_p_one != NULL) {
//        agglo_lines_l_array_idx, agglo_lines_l_array = u.convert_fine_agglomeration_lines_to_fine_agglomeration_lines_arrays(
//                ccg.nb_cc, aniso_lines_l)
//
//    }
//
//
//    return get_nb_cells_at_level(1), ccg.fc_2_cc, \
//                   agglo_lines_l_m_one_array_idx, agglo_lines_l_m_one_array, \
//                   agglo_lines_l_array_idx, agglo_lines_l_array;
}

bool Agglomerator::is_agglomeration_anisotropic() {
    return __cc_graphs->is_agglomeration_anisotropic();
}

void Agglomerator::get_agglo_lines(int level,  //TODO Change to short
                                   long *sizes,
                                   long *agglo_lines_array_idx,
                                   long *agglo_lines_array) {
    //remark: sizes[2]={nb_agglomeration_lines +1, nb_fc_in_agglomeration_lines}
    assert(__cc_graphs->is_agglomeration_anisotropic());
    assert(level == 0 || level == 1);

    convert_agglo_lines_to_agglomeration_lines_arrays(__v_nb_lines[level],
                                                      __v_lines[level],
                                                      sizes,
                                                      agglo_lines_array_idx,
                                                      agglo_lines_array);
}

void Agglomerator::__create_all_anisotropic_cc_wrt_agglomeration_lines() {
    /**
     * process along all agglomeration_lines to create every anisotropic cc.
     * Generate also the coarse agglomeration lines and the set of compliant anisotropic cc
     */
    //TODO Comprendre quand est-ce qu'on supprime une ligne d'agglomeration?

    // list of set of hexaedric or prismatic cell number (for level 0)
    __v_of_s_anisotropic_compliant_fc[1] = {};

    // Process of every agglomeration lines:
    forward_list<deque<long> *>::iterator fLIt;
    for (fLIt = __v_lines[1].begin(); fLIt != __v_lines[1].end(); fLIt++) {

        deque<long> *line_lvl_p_one = new deque<long>();
        long line_size = (**fLIt).size();
        if (line_size <= 1) {

            // the agglomeration_line is empty.
            *fLIt = line_lvl_p_one;
            continue;
        }

        long i_count = 0;
//        cout<<"Agglo_Line seed "<<(**fLIt)[i_count]<<endl;
        bool is_anisotropic = true;
        long i_cc;
        while (i_count + 2 <= line_size) {
            const long i_fc = (**fLIt)[i_count];
            const long i_fc_p_one = (**fLIt)[i_count + 1];

            unordered_set<long> s_fc = {i_fc, i_fc_p_one};
            i_cc = (*__cc_graphs).cc_create_a_cc(s_fc, is_anisotropic);
            line_lvl_p_one->push_back(i_cc);
            // Checks that the agglomerated fc is indeed an anisotropic fc".
            assert(__v_of_s_anisotropic_compliant_fc[0].count(i_fc));
            assert(__v_of_s_anisotropic_compliant_fc[0].count(i_fc_p_one));

            // the new cc is anisotropic compliant:
            __v_of_s_anisotropic_compliant_fc[1].insert(i_cc);
            i_count += 2;
        }

        // if i_count < len(line): there is a fc left!
        // i.e. the agglomeration line was of odd size.
        // 2 situations: si la cellule abandonnee est contigue a la zone Euler, c'est OK.
        //               sinon, c'est la merde!
        if (i_count < line_size) {
            // Problematic   Check! it may happen that the line touches the farfield
            // This is correct!
            // for example: RAE 2D case, Boxes iso_and_aniso
            // // check
            // ind = matrixAdj_CRS_row_ptr[cell]
            // ind_p_one = matrixAdj_CRS_row_ptr[cell + 1]
            // isOnBoundary = False
            // for i in range(ind, ind_p_one):
            //     indNeighbourCell = matrixAdj_CRS_col_ind[i]
            //     if indNeighbourCell == cell:
            //         isOnBoundary = True
            // assert not isOnBoundary, "The left alone anisotropic cell to agglomerate is on boundary"
            // // End Check
            // We add this fc to the last coarse element (thus of cardinal 3)
            (*__cc_graphs).cc_update_cc({(**fLIt)[i_count]}, i_cc);
        }
//        self.__lines[i_line] = line_lvl_p_one
        delete *fLIt;
        *fLIt = line_lvl_p_one;
    }
}

void Agglomerator::_agglomerate_one_level_isotropic_part(const short debug_only_steps) {
    /**
     *  isotropic agglomeration of one level of mesh.
     *  We do not necessary agglomerate all neighbouring cells.
     */

    // I) First try: we work on every fC
    //====================================
    _agglomerate_sub_sub_isotropic_first_step();

    if ((*__cc_graphs).is_cc_grid_not_structured(__goal_card)) {

//        cout<<"Not yet implemented:_correction_main_triconnected(*__cc_graphs);"<<endl;
//        exit(1);
        _correction_main_triconnected();
    }

    (*__cc_graphs).cc_create_all_delayed_cc();
    (*__cc_graphs).fill_cc_neighbouring();
    // print "After First Step : dict_Distribution_Of_Cardinal_Of_CC", dict_Distribution_Of_Cardinal_Of_CC

    // TODO ATTENTION AU CHANGEMENT DE LISTE AVEC LE DELETE!
    // TODO voir TP python. Peut etre faire une boucle tant que non vide...
    // II) Correction: treatment of incomplete cells!
    if (__kind_of_agglomerator == "basic" && (*__cc_graphs).is_cc_grid_not_structured(__goal_card)) {
        if (__verbose) {
            cout << "Call of self._correction_main_basic(...)" << endl;
        }
        _correction_main_basic(debug_only_steps);
    }

    (*__cc_graphs).cc_renumber();
    __l_nb_of_cells.push_back((*__cc_graphs)._cc_counter);
    assert((*__cc_graphs).check_data_consistency_and_connectivity());
}


void Agglomerator::_agglomerate_one_level_anisotropic_part() {

/**
 * Agglomerate one level of mesh for boundary layer region/ anisotropic part of the mesh.
 *       Uses the self.__lines.
 */

    if (__v_lines[0].empty()) {

        // The anisotropic lines are only computed on the original (finest) mesh.
        long nb_agglomeration_lines(0);
        __v_lines[0] = __fc_graphs.compute_anisotropic_line_v2(nb_agglomeration_lines);  // finest level!!!
        __v_nb_lines[0] = nb_agglomeration_lines;
    }
    // This is not the first generation of a coarse level.
    // The anisotropic lines are given as input.

    // Copy of the current agglomeration_lines as a backup for visualization purpose.
    __v_lines[1] = copy_agglomeration_lines(__v_lines[0]);
    __v_nb_lines[1] = __v_nb_lines[0];

    // Asserts that __v_lines[1] exists:
    if (!__v_lines[1].empty()) {
        __create_all_anisotropic_cc_wrt_agglomeration_lines();
    }
//    if self.__is_visu_data_stored and self.__lines:
//    // return self.__d_visu_anisotropic_agglomeration_lines_to_delete[i_lvl], self.__lines[:]
//    return tmp_backup_lines, self.__lines[:]
//    else:
//    return None, self.__lines[:]

}

void Agglomerator::_agglomerate_sub_sub_isotropic_first_step() {


/**
 * First try to build an agglomeration.
 * From the boundary of the domain, (corner, ridge or valley) we build agglomerated cells of cardinal goal_card
 */
    long nb_of_fc = __l_nb_of_cells[0];

    while ((*__cc_graphs).get_number_of_fc_agglomerated() < nb_of_fc) {

        // 1) Choose a new seed
        //====================================
        long seed = (*__cc_graphs).choose_new_seed();
        if (__verbose) {
            cout << "seed = " << seed << "\t" << (*__cc_graphs).get_number_of_fc_agglomerated() << endl;
        }


        // 2) Computation of optimal coarse cell
        //====================================

        _create_optimal_cc_v2(seed);
        // print("\n",cc_graph._cc_counter-1, seed, cc_graph._d_isotropic_cc[cc_graph._cc_counter-1].get_s_fc())

    }
}


// Agglomerator stuff:
// difference between _create_optimal_cc and _create_optimal_cc_v2 is that the computation of the neighbourhood is
// included inside  for _create_optimal_cc_v2
void Agglomerator::_create_optimal_cc_v2(const long seed) {

    /**
     * main function for isotropic agglomeration: it chooses the "optimal" fc for a given seed.
     * The goal of this function is to choose from a pool of neighbours the best CC to build
     * If there is not enough fc available in dict_neighbours_of_seed we create an incomplete cc: i.e. too small.
     *
     * -> Called from self._agglomerate_sub_sub_isotropic_first_step(...):
     * - Creates a cc.
     *
     */
    unsigned short compactness = 0;
    unordered_set<long> set_current_cc = _choose_optimal_cc_and_update_seed_pool_v2(seed,
                                                                                    compactness);

    // Creation of cc:
    //====================================
    bool is_anistropic = false;
    bool is_creation_delayed = compactness < __dimension;
    if (__verbose) {
        cout << "seed = " << seed << "\t{";

        for (long i_fc:set_current_cc) {
            cout << i_fc << ", ";
        }
        cout << "}" << endl;
    }
    (*__cc_graphs).cc_create_a_cc(set_current_cc, is_anistropic, is_creation_delayed);
}


unordered_set<long> Agglomerator::__choose_optimal_cc_basic_v2(const long seed,
                                                               unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                               unsigned short &compactness,
                                                               const bool is_order_primary) {
    if (__verbose) {
        cout << "Call of __choose_optimal_cc_basic_v2" << endl;
    }

    // Number of fine cells constituting the current coarse cell in construction.
    unsigned short size_current_cc = 1; // CC contains only one cell: the seed

    // set of fc for current cc:
    unordered_set<long> s_fc_for_current_cc = {seed};

    unordered_set<long> s_seeds = {seed};

    //TODO: refactor compute_neighbourhood_of_cc with  Agglomerator::__choose_optimal_cc_triconnected

    // Compute self.__min_neighbourhood order neighbourhood:
    //======================================================
    unsigned short max_order_of_neighbourhood = __min_neighbourhood;
    __fc_graphs.compute_neighbourhood_of_cc(s_seeds,
                                            max_order_of_neighbourhood,   //in and out
                                            dict_neighbours_of_seed,
                                            __max_card,
                                            (*__cc_graphs)._a_is_fc_agglomerated);


    // If no neighbour is found for seed: this case happened only when isotropic cell is surrounded
    // by anisotropic cells.
    if (dict_neighbours_of_seed.empty()) {
        // d_n_of_seed is empty, i.e: the cc is not complete
        // and no neighbour are available!
        compactness = 0;
        return s_fc_for_current_cc;
    }

    // TODO remove this
    if ((dict_neighbours_of_seed.size() + s_fc_for_current_cc.size()) < __goal_card) {
        // Not enough available neighbour: creation of a (too small) coarse cell.
        // s_fc_for_current_cc.update(d_n_of_seed)
        for (auto &i_k_v : dict_neighbours_of_seed) {
            s_fc_for_current_cc.insert(i_k_v.first);
        }

        bool is_creation_delayed = (s_fc_for_current_cc.size() <= __threshold_card);
        if (is_creation_delayed) {

            compactness = 0;
            return s_fc_for_current_cc;
        } else {
            compactness = __dimension;
            return s_fc_for_current_cc;
        }
    }

    unordered_set<long> s_fc = __choose_optimal_cc_basic_v2_sub(seed,
                                                                dict_neighbours_of_seed,
                                                                compactness,
                                                                is_order_primary);
//    cout << "s_fc.size() " << s_fc.size() << endl;
    return s_fc;
}


void Agglomerator::_correction_main_basic(const short debug_only_steps) {
    ;

/**
 * :param debug_only_steps: 0 no correction, 1 first step only, ... 6: all 6 correction steps. 10 (arbitrary strictly greater than 6) all steps
 */
    short dbg_only_steps = debug_only_steps;
    if (debug_only_steps == -1) {
        dbg_only_steps = 10.;
    }
    const short nb_iteration = 4;

    if (__verbose) {
        cout << "\nCorrection_main_basic" << endl;
        cout << "===========================" << endl;
    }

    for (short i = 0; i < nb_iteration; i++) {
        if (__verbose) {
            cout << "\n\n\n" << endl;
            cout << "\t==========================================" << endl;
            cout << "\tITERATION\t" << i << endl;
            cout << "\tnb_cc      :\t" << (*__cc_graphs)._cc_counter;
            cout << "\tnb_cc iso  :\t" << (*__cc_graphs)._d_isotropic_cc.size();
            cout << "\tnb_cc aniso:\t" << ((*__cc_graphs)._cc_counter - (*__cc_graphs)._d_isotropic_cc.size());
            cout << "\n\td_distribution_of_cardinal_of_cc: ";
            (*__cc_graphs).print_d_distribution_of_cardinal_of_isotropic_cc();
        }

        // Step 1:
        //======================================================
        if (dbg_only_steps >= 1) {


            if (__verbose) {
                cout << "\n\t// Step 1: remove_too_small_cc" << endl;
                cout << "\t//=============================" << endl;
            }

            // Step One: too small coarse cells (1<= size<are agglomerated, fine cell by fine cell
            // to their "better" neighbour. 1<= Card <= self.__threshold

            (*__cc_graphs).correction_remove_too_small_cc(__threshold_card);
            assert((*__cc_graphs).check_data_consistency_and_connectivity());

            if (__verbose) {
//                print("\t\t----> Correction After step one: dict_DistributionOfCardinalOfCoarseElements",
//                      cc_graph.d_distribution_of_cardinal_of_isotropic_cc)
                cout << "\t\t----> Correction After step one: distribution_of_cardinal_of_isotropic_cc ";
                (*__cc_graphs).print_d_distribution_of_cardinal_of_isotropic_cc();
            }
        }

        // Step 2:
        //======================================================
        if (dbg_only_steps >= 2) {

            if (__verbose) {
                cout << "\n\t// Step 2: make_small_cc_bigger" << endl;
                cout << "\t//==============================" << endl;
            }
            (*__cc_graphs).correction_make_small_cc_bigger(__threshold_card,
                                                           __min_card,
                                                           __goal_card,
                                                           __verbose);
            if (__verbose) {

                cout << "\t\t----> end Step 2: distribution_of_cardinal_of_isotropic_cc ";
                (*__cc_graphs).print_d_distribution_of_cardinal_of_isotropic_cc();

            }

            assert((*__cc_graphs).check_data_consistency_and_connectivity());
        }

        // Step 3:
        //======================================================
        if (dbg_only_steps >= 3) {

            if (__verbose) {

                cout << "\n\t// Step 3: swap leaf" << endl;
                cout << "\t//==============================" << endl;
            }

            // Swap for every fine cell.
            (*__cc_graphs).correction_swap_leaf_fc_v2();

            if (__verbose) {

                cout << "\t\t----> end Step 3: distribution_of_cardinal_of_isotropic_cc ";
                (*__cc_graphs).print_d_distribution_of_cardinal_of_isotropic_cc();

            }

            assert((*__cc_graphs).check_data_consistency_and_connectivity());
        }
        // Step 4:
        //======================================================
        if (dbg_only_steps >= 4) {

            if (__verbose) {

                cout << "\n\t// Step 4: correction_reduce_too_big_cc" << endl;
                cout << "\t//==============================" << endl;
            }


            // Step four: Too big cells are corrected!
            // We look for leaves in too big cc and give it to the smallest neighbour without any further consideration
            // (no card, or anything else)
            (*__cc_graphs).correction_reduce_too_big_cc(__goal_card, __verbose);

            if (__verbose) {

                cout << "\t\t----> end Step 4: distribution_of_cardinal_of_isotropic_cc ";
                (*__cc_graphs).print_d_distribution_of_cardinal_of_isotropic_cc();

            }

            assert((*__cc_graphs).check_data_consistency_and_connectivity());

        }


        // Step 5:
        //======================================================
        if (dbg_only_steps >= 5) {

            if (__verbose) {

                cout << "\n\t// Step 5: split_too_big_cc_in_two" << endl;
                cout << "\t//=================================" << endl;
            }

//            cout << "Not yet implemented" << endl;
//            exit(1);
            _correction_split_too_big_cc_in_two();

            if (__verbose) {

                cout << "\t\t----> end Step 5: distribution_of_cardinal_of_isotropic_cc ";
                (*__cc_graphs).print_d_distribution_of_cardinal_of_isotropic_cc();

            }
            assert((*__cc_graphs).check_data_consistency_and_connectivity());
        }
        // Step 6:
        //======================================================
        if (dbg_only_steps >= 6) {

            if (__verbose) {

                cout << "\n\t// Step 6: remove_too_small_cc" << endl;
                cout << "\t//===============================" << endl;
            }
            (*__cc_graphs).correction_remove_too_small_cc(__threshold_card);

            if (__verbose) {

                cout << "\t\t----> end Step 6: distribution_of_cardinal_of_isotropic_cc ";
                (*__cc_graphs).print_d_distribution_of_cardinal_of_isotropic_cc();

            }
            assert((*__cc_graphs).check_data_consistency_and_connectivity());
        }
    }
}

void Agglomerator::_correction_main_triconnected() {
    (*__cc_graphs).correction_main_triconnected(__min_neighbourhood_correction_step,
                                                __goal_card,
                                                __verbose);
}


// Coarse Graph
void Agglomerator::_correction_split_too_big_cc_in_two() {

/**
 *  Can't be transfered to CCG because of the _choose_optimal_cc_and_update_seed_pool call
 */

    if ((*__cc_graphs).is_agglomeration_done()) {

        unordered_set<long> s_cc = (*__cc_graphs).get_s_isotropic_cc_of_size(__max_card + 1);

        if (__verbose) {
            cout << "\n\n\n\t\t_correction_split_too_big_cc_in_two" << endl;
            cout << "\t\t=======================================================\n" << endl;
            cout << "\t\ts_cc: {";
            for (const long i_cc:s_cc) {
                cout << i_cc << ", ";
            }
            cout << "}" << endl;
        }


        for (const long i_cc :s_cc) {

            unordered_set<long> s_fc = (*(*__cc_graphs)._d_isotropic_cc[i_cc]).get_s_fc();
            vector<long> v_fc;
            for (const long i_fc :s_fc) {
                v_fc.push_back(i_fc);
            }
            long seed = -1;
            unordered_map<long, queue<long> *> d_spanning_tree = __fc_graphs.find_seed_via_frontal_method(seed, v_fc);

            if (__verbose) {
                cout << "\n\t\tToo Big Cell: " << i_cc << " {";
                for (const long i_cc:s_fc) {
                    cout << i_cc << ", ";
                }
                cout << "}" << endl;

                cout << "\t\t\tSeed " << seed << endl;
                for (auto i : d_spanning_tree) {
                    cout << "\t\t\t" << i.first << ":\t";
                    print_queue(*d_spanning_tree[i.first]);
                }
            }


            // We remove from the s_of_fc the cells of degree greater than (or equal to) 3.
            __fc_graphs.remove_separating_vertex(seed, d_spanning_tree, s_fc);
            //Rk: s_fc is modified.

            for (auto iPairDict:d_spanning_tree) {
                delete iPairDict.second;
            }

            if (__verbose) {
                cout << "\t\t\tAfter remove_separating_vertex" << endl;
                cout << "\t\t\ts_fc: {";
                for (const long i_fc:s_fc) {
                    cout << i_fc << ", ";
                }
                cout << "}" << endl;
            }


            // Computation of the neighbourhood:

            unordered_set<long> s_seeds = {seed};
            unsigned short max_order_of_neighbourhood = 3;
            unordered_map<long, unsigned short> d_neighbours_of_seed = {};
            __fc_graphs.compute_neighbourhood_of_cc(s_seeds,
                                                    max_order_of_neighbourhood,
                                                    d_neighbours_of_seed,
                                                    __max_card,
                                                    (*__cc_graphs)._a_is_fc_agglomerated,
                                                    s_fc);

            if (__verbose) {

                cout << "\t\t\tdict_neighbours_of_seed: {";
                for (const auto i_k_v:d_neighbours_of_seed) {
                    cout << i_k_v.first << ":" << i_k_v.second << ", ";
                }
                cout << "}" << endl;
            }

            if (d_neighbours_of_seed.empty()) {

                // If d_neighbours_of_seed is empty
                // We transfer the seed to one of its coarse neighbour.
                vector<long> a_fc_neighbours = __fc_graphs.get_neighbours(seed);

                for (const long i_fc_n : a_fc_neighbours) {
//                    original_s_fc = cc_graph.get_cc(i_cc).get_s_fc();
                    unordered_set<long> original_s_fc = (*(*__cc_graphs)._d_isotropic_cc[i_cc]).get_s_fc();
                    if (i_fc_n != seed && original_s_fc.count(i_fc_n) == 0) {
                        long i_cc_n = (*__cc_graphs)._fc_2_cc[i_fc_n];
                        if ((*__cc_graphs).is_isotropic_cc(i_cc_n)) {
                            unordered_set<long> s_fc_n = (*(*__cc_graphs)._d_isotropic_cc[i_cc_n]).get_s_fc();
                            if (s_fc_n.size() < __max_card) {

                                unordered_set<long> s_removed_cc = (*__cc_graphs).cc_swap_fc(s_seeds, i_cc, i_cc_n);
                                assert(s_removed_cc.size() == 0);
                                break;
                            }
                        }
                    }
                }
            } else {
                unsigned short compactness;
                // dict_neighbours_of_seed is not empty:
                unordered_set<long> s_of_fc_for_current_cc = _choose_optimal_cc_and_update_seed_pool((*__cc_graphs),
                                                                                                     seed,
                                                                                                     d_neighbours_of_seed,
                                                                                                     __min_card,
                                                                                                     __max_card,
                                                                                                     __kind_of_agglomerator,
                                                                                                     compactness,
                                                                                                     true,
                                                                                                     false);
                if (__verbose) {
                    cout << "s_of_fc_for_current_cc: {";
                    for (const long i_fc:s_of_fc_for_current_cc) {
                        cout << i_fc << ", ";
                    }
                    cout << "}" << endl;
                }

                // Creation of an empty coarse cell:
                unordered_set<long> empty_set = unordered_set<long>({});
                long ind_of_current_cc = (*__cc_graphs).cc_create_a_cc(empty_set);
                // ind_of_current_cc is the index of the newly created cc

                // We want that the new cell to be the smallest of the two.
                short size_original_i_cc = (*(*__cc_graphs)._d_isotropic_cc[i_cc]).get_s_fc().size();
                short size_new_cc = s_of_fc_for_current_cc.size();

                if (size_new_cc > size_original_i_cc - size_new_cc) {

                    unordered_set<long> s_complementary = (*(*__cc_graphs)._d_isotropic_cc[i_cc]).get_s_fc();
                    // s_complementary is the original cc.

                    // s_complementary = cc_graph.get_cc(i_cc).get_s_fc() - s_of_fc_for_current_cc
                    for (const long i_fc :s_of_fc_for_current_cc) {
                        if (s_complementary.count(i_fc)) {
                            s_complementary.erase(i_fc);
                        }
                    }

                    unordered_set<long> s_removed_cc = (*__cc_graphs).cc_swap_fc(s_complementary, i_cc, ind_of_current_cc);
                    if (__verbose) {
                        cout << "Cell " << i_cc << ": {";
                        for (const long i_fc:s_of_fc_for_current_cc) {
                            cout << i_fc << ", ";
                        }
                        cout << "} Cell " << ind_of_current_cc << ": {";
                        for (const long i_fc:s_complementary) {
                            cout << i_fc << ", ";
                        }
                        cout << "}" << endl;
                    }
                    assert(s_removed_cc.empty());

                } else {
                    // size_new_cc <= size_original_i_cc - size_new_cc
                    unordered_set<long> s_removed_cc = (*__cc_graphs).cc_swap_fc(s_of_fc_for_current_cc, i_cc, ind_of_current_cc);

                    unordered_set<long> s_complementary = (*(*__cc_graphs)._d_isotropic_cc[i_cc]).get_s_fc();
                    // s_complementary = cc_graph.get_cc(i_cc).get_s_fc() - s_of_fc_for_current_cc
                    for (const long i_fc :s_of_fc_for_current_cc) {
                        if (s_complementary.count(i_fc)) {
                            s_complementary.erase(i_fc);
                        }
                    }
                    if (__verbose) {
                        cout << "Cell " << i_cc << ": {";
//                        probleme ici on devrait avoir la cc 3 0,1,2
                        for (const long i_fc:s_complementary) {
                            cout << i_fc << ", ";
                        }
                        cout << "} Cell " << ind_of_current_cc << " : {";
                        for (const long i_fc:s_of_fc_for_current_cc) {
                            cout << i_fc << ", ";
                        }
                        cout << "}" << endl;
                    }

                    assert(s_removed_cc.empty());
                }


                assert(__fc_graphs.check_connectivity((*(*__cc_graphs)._d_isotropic_cc[ind_of_current_cc]).get_s_fc()));

            }

            if (__checks) {
                assert(__fc_graphs.check_connectivity((*(*__cc_graphs)._d_isotropic_cc[i_cc]).get_s_fc()));
                assert((*__cc_graphs).check_data_consistency_and_connectivity());
            }
        }

    }
}


unordered_set<long> Agglomerator::__choose_optimal_cc_triconnected_2D(long seed,
                                                                      unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                                      unsigned short &compactness,
                                                                      const bool increase_neighbouring,
                                                                      short min_neighbourhood) {


    // TODO In fact it is not triconnected as we are in 2D but biconnected. Change this????

    // l_of_fc = list(d_neighbours_of_seed)
    list<long> l_of_fc;//(dict_neighbours_of_seed.size());// = list(d_neighbours_of_seed)
    unordered_set<long> s_of_fc;
    long i_count = 0;
    for (auto i_k_v :dict_neighbours_of_seed) {
//        l_of_fc[i_count]=i_k_v.first;
        l_of_fc.push_back(i_k_v.first);
//        i_count++;
        s_of_fc.insert(i_k_v.first);  //s_of_fc: Set[int] = set(l_of_fc)

    }
    unordered_set<long> s_of_fc_for_current_cc = {seed};
    short i_neighbourhood_min;
    if (min_neighbourhood == -1) {

        i_neighbourhood_min = __min_neighbourhood;

    } else {

        i_neighbourhood_min = min_neighbourhood;

    }

    short i_neighbourhood_max = __max_neighbourhood;

    short i_neighbour = i_neighbourhood_min;  // Initial size

    short deg_seed = __fc_graphs.compute_degree_of_node_in_subgraph(seed, s_of_fc);

//    cout << "check a copy is done!" << endl;
// TODO: change type to unordered_map<long, short>?
    unordered_map<long, unsigned short> d_neighbours_of_seed = dict_neighbours_of_seed;
    unordered_map<long, unsigned short> d_neighbours_of_seed_n_minus_one = dict_neighbours_of_seed;

    if (increase_neighbouring) {
        assert(s_of_fc.size() + 1 >= __goal_card);
    }

    // This is current criteria goal_card
    unordered_map<short, unordered_set<long>> d_connected;  // TODO  Pas tres utilise! utile?
    unordered_map<short, unordered_set<long>> d_biconnected;

    // The first neighbourhood is connected
//    cout << "check a copy is done!" << endl;
    d_connected[i_neighbourhood_min] = s_of_fc_for_current_cc;  // copy needed
    d_connected[i_neighbourhood_min].insert(seed);

    //d_connected[i_neighbourhood_min].update(set(d_neighbours_of_seed.keys()))
    for (const auto i_kv: dict_neighbours_of_seed) {
        d_connected[i_neighbourhood_min].insert(i_kv.first);
    }

    bool inc_neighbouring = true;

    unordered_set<long> s_fc_for_cc;
//    unsigned short compactness_of_cc = 0;
    bool is_cc_found = false;

    bool is_correction_step = false;

//    list<long> l_of_fc;  // TODO can we replace this by a set??? i.e. does order matter?
    while (i_neighbour < i_neighbourhood_max && inc_neighbouring && (!is_cc_found)) {

        // We increase the neighbourhood while we have not found a biconnected component.
//        cout<< "\t\ti_neighbour "<< i_neighbour<<endl;
        inc_neighbouring = increase_neighbouring;

        // Increase neighbourhood:
        if (i_neighbour != i_neighbourhood_min) {

            // if it is not the first step (because the neighbourhood has already been computed)
            if (!is_correction_step) {
//                cout << "Increase Neighbourhood " << i_neighbour << endl;
                unsigned short max_order_of_neighbourhood = i_neighbour;
                unordered_set<long> s_seed = {seed};
                d_neighbours_of_seed.clear();
                __fc_graphs.compute_neighbourhood_of_cc(s_seed,
                                                        max_order_of_neighbourhood,   //in and out
                                                        d_neighbours_of_seed,
                                                        __max_card,
                                                        (*__cc_graphs)._a_is_fc_agglomerated);

                // l_of_fc = list(d_neighbours_of_seed)
                l_of_fc.clear();
                for (auto i_k_v :d_neighbours_of_seed) {
                    l_of_fc.push_back(i_k_v.first);
                }
            } else {

                unsigned short max_order_of_neighbourhood = i_neighbour;
                d_neighbours_of_seed.clear();
                __fc_graphs.compute_neighbourhood_of_cc(s_of_fc_for_current_cc,
                                                        max_order_of_neighbourhood,   //in and out
                                                        d_neighbours_of_seed,
                                                        __max_card,
                                                        (*__cc_graphs)._a_is_fc_agglomerated);


                // l_of_fc = list(d_neighbours_of_seed)
                l_of_fc.clear();
                for (auto i_k_v :d_neighbours_of_seed) {
                    l_of_fc.push_back(i_k_v.first);
                }
                l_of_fc.remove(seed);  // because the seed is in the neighbourhood of CC

                // l_of_fc.extend(s_of_fc_for_current_cc)
                for (const long i_fc :s_of_fc_for_current_cc) {
                    l_of_fc.push_back(i_fc);
                }
            }

            if (compare_maps_keys(d_neighbours_of_seed, d_neighbours_of_seed_n_minus_one)) {
                // trouble: no more available cells. in the neighbourhood of seed
                break;
            }
        }
        l_of_fc.push_back(seed);
        is_correction_step = false;

        unordered_set<long> s_bic_component_g = __compute_bic_components_from_s_connected_vertices_containing_seed(seed,
                                                                                                                   l_of_fc,
                                                                                                                   is_correction_step,
                                                                                                                   s_of_fc_for_current_cc);
        // TODO remove this! this makes no sense in 2D!
        if ((!s_bic_component_g.empty()) && deg_seed >= 3) {

            // we have a biconnected component containing seed and deg_seed >= 3
            // i.e. may be it belongs to a tric-component: we have to check:

            s_fc_for_cc = __compute_tric_components_from_s_bic_vertices(s_bic_component_g,
                                                                        is_correction_step,
                                                                        seed,
                                                                        compactness,
                                                                        s_of_fc_for_current_cc);

            if (!s_fc_for_cc.empty()) {
                // Exit from while loop
                is_cc_found = true;
                break;
            }

            // if not is_triconnected_component_found...
            // backup biconnected CC
            d_biconnected[i_neighbour] = s_bic_component_g;  //copy...

        } else if ((!s_bic_component_g.empty()) && deg_seed < 3) {

            // Deg of seed in the subgraph is lower than 3. We cannot found any triconnected component:
            is_cc_found = true;
            s_fc_for_cc = s_bic_component_g;
            compactness = 2;
        }
        i_neighbour += 1;
        d_neighbours_of_seed_n_minus_one = d_neighbours_of_seed;
    }
    if (!is_cc_found) {

        // we are here if no triconnected cc is found.
        // print "\n\ttrouble: no more available cells. No triconnected Coarse cell available"
        if (!d_biconnected.empty()) {

            // print "\tdict_biconnected", dict_biconnected
            // We choose the biconnected CC, if any, with the smallest cardinal
            for (short i_n = i_neighbourhood_min; i_n < i_neighbourhood_max; i_n++) {
                if (d_biconnected.count(i_n) > 0 && (d_biconnected[i_n].size() >= __min_card)) {

                    is_cc_found = true;
                    s_fc_for_cc = d_biconnected[i_n];  //copy!
                    compactness = 2;
                    break;
                }
            }
        }
        if (!is_cc_found) {
            s_fc_for_cc = d_connected[i_neighbourhood_min];
            compactness = 1;
        }
    }
    // // Cleaning of d_neighbours_of_seed:
    // // We remove fc which belongs to the selected cc s_fc_for_cc:
    // for i_fc in s_fc_for_cc:
    //     if i_fc in d_neighbours_of_seed:
    //         del d_neighbours_of_seed[i_fc]
    //
    // // To do improve this if possible?
    // // we remove from dict
    // l_neighbours_of_seed = list(d_neighbours_of_seed.keys())
    // for i_fc in l_neighbours_of_seed:
    //     s_n = set(graph.get_neighbours(i_fc))
    //     if not s_n & s_fc_for_cc:
    //         del d_neighbours_of_seed[i_fc]
    //     else:
    //         // Little trick to keep the current behaviour of basic agglomerator
    //         d_neighbours_of_seed[i_fc] = 2
    __fc_graphs.clean_d_neighbours_of_seed(s_fc_for_cc, d_neighbours_of_seed);

    // print("__choose_optimal_cc_triconnected_2D:", s_fc_for_cc, " d_n_of_seed", d_neighbours_of_seed)
//    compactness = compactness_of_cc;
//    dict_neighbours_of_seed = d_neighbours_of_seed;
    dict_neighbours_of_seed.clear();
    for (auto i_kv : d_neighbours_of_seed) {
        dict_neighbours_of_seed[i_kv.first] = i_kv.second;
    }
    return s_fc_for_cc;
}

unordered_set<long> Agglomerator::__compute_bic_components_from_s_connected_vertices_containing_seed(long seed,
                                                                                                     const list<long> &l_of_fc,
                                                                                                     bool is_correction_step,
                                                                                                     unordered_set<long> s_of_fc_for_current_cc) {


    // TODO mettre dans un set seed pour avoir un seul traitement entre seed et s_of_fc_for_current_cc ca ferait sauter la correction step.

    // TODO change type of l_of_fc????
    unordered_set<long> s_fc = {};
    for (const long i_fc : l_of_fc) {
        s_fc.insert(i_fc);
    }
    vector<long> l_to_g;
    // Building of a triconnected_graph Object:
    Triconnected_graph *tric_graph_l_connected = __generate_triconnected_graph(s_fc, l_to_g);


    list<unordered_set<long>> l_biconnected_component;
    bool is_biconnected = BCC_NR((*tric_graph_l_connected), l_biconnected_component);
    delete tric_graph_l_connected;
    unordered_set<long> s_biconnected_component_g;

    // We select the biconnected component containing seed or s_of_fc_for_current_cc if any
    for (unordered_set<long> compo : l_biconnected_component) {
        unordered_set<long> s_compo;
        for (const long i_l: compo) {
            s_compo.insert(l_to_g[i_l]);  // Local to global numbering
        }

        // TODO Choose it more wisely
        if (!is_correction_step) {
            if (s_compo.count(seed)) {
                // We select the Biconnected Component containing the seed:
//                s_biconnected_component_g = s_compo;
//                break;
                return s_compo;
            }
        } else {
            // we check that every fc in s_of_fc_for_current_cc belong to the current biconnected component: s_compo
            bool is_ok = true;
            for (long i_fc:s_of_fc_for_current_cc) {
                if (s_compo.count(i_fc) == 0) {
                    is_ok = false;
                    break;
                }
            }
            if (is_ok && s_compo.count(seed)) {
                return s_compo;
            } else {
                // no biconnected component...
                // We return an empty set.
                return s_biconnected_component_g;
            }
        }

    }

    return s_biconnected_component_g;
}

unordered_set<long> Agglomerator::__compute_tric_components_from_s_bic_vertices(unordered_set<long> s_bic_component_g,
                                                                                bool is_correction_step,
                                                                                long seed,
                                                                                unsigned short &compactness,
                                                                                unordered_set<long> s_of_fc_for_current_cc) {

    vector<long> l_of_biconnected_node(s_bic_component_g.size());  // indices from local to global
//    int i_count = 0;
//    for (long i_fc : s_bic_component_g) {
//        l_of_biconnected_node[i_count] = i_fc;
//        i_count++;
//    }
    Triconnected_graph *tric_graph = __generate_triconnected_graph(s_bic_component_g, l_of_biconnected_node);

    list<unordered_set<long>> l_tric_components_local = {};
    (*tric_graph).computeTriconnectivity(l_tric_components_local);
    delete tric_graph;

    // Global numbering
    list<unordered_set<long>> l_tric_component_global = {};
    for (auto compo : l_tric_components_local) {
        unordered_set<long> s_compo;
        for (long i_l : compo) {
            s_compo.insert(l_of_biconnected_node[i_l]);
        }
        l_tric_component_global.push_back(s_compo);
    }

    // print "\tTriconnected Graph_triconnected", l_tric_component_global
    unordered_set<long> s_fc_for_cc;
    compactness = 0;
    for (auto set_comp:l_tric_component_global) {

        if (!is_correction_step) {
            if (set_comp.count(seed)) {
                // we have found a triconnected component containing seed
                // print "Triconnected", len(set_comp)
                // print "\t\tiNeigh", i_neighbour, "size CC", len(set_comp), ", size Neigh", len(l_of_fc)
                // return set_comp, 3
                compactness = 3;
                return set_comp;
            }
        } else {
            bool is_ok = true;
            for (long i_fc:s_of_fc_for_current_cc) {
                if (set_comp.count(i_fc) == 0) {
                    is_ok = false;
                    break;
                }
            }
            if (is_ok && set_comp.count(seed)) {
                compactness = 3;
                return set_comp;
            }
        }
    }

    return s_fc_for_cc;
}

Triconnected_graph *Agglomerator::__generate_triconnected_graph(const unordered_set<long> &set_biconnected_component_g,
                                                                vector<long> &l_to_g) const {

    vector<long> row_ptr_l;
    vector<long> col_ind_l;
    vector<double> values_l;
    vector<long> g_to_l;

    __fc_graphs.compute_local_crs_subgraph_from_global_crs(set_biconnected_component_g,
                                                           row_ptr_l, col_ind_l, values_l,
                                                           g_to_l, l_to_g);

    return new Triconnected_graph(row_ptr_l, col_ind_l, values_l);
}