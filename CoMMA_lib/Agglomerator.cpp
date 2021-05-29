//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Agglomerator.h"
#include "First_Order_Neighbourhood.h"

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


unordered_set<long> Agglomerator::_choose_optimal_cc_and_update_seed_pool(Coarse_Cell_Graph cc_graph,
                                                                          const long &seed,
                                                                          unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                                          const unsigned short goal_card,
                                                                          const unsigned short max_card,
                                                                          string kind_of_agglomerator,
                                                                          short &compactness,
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
        s_current_cc = __choose_optimal_cc_triconnected(cc_graph,
                                                        seed,
                                                        dict_neighbours_of_seed,
                                                        goal_card,
                                                        max_card,
                                                        compactness,
                                                        increase_neighbouring,
                                                        set_of_fc_for_current_cc);
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
        short compactness = -1;
        s_current_cc = __choose_optimal_cc_basic_v2(seed,
                                                    d_n_of_seed,
                                                    compactness,
                                                    is_order_primary);
//        cout << "_choose_optimal_cc_and_update_seed_pool_v2 s_current_cc.size() " << s_current_cc.size() << endl;
//        cout<<"Not yet implemented:__choose_optimal_cc_basic_v2(cc_graph, seed,is_order_primary);"<<endl;
//        exit(1);
    } else if (__kind_of_agglomerator == "triconnected") {
//        s_current_cc, compactness,
//                d_n_of_seed = self.__choose_optimal_cc_triconnected_v2(cc_graph,
//                                                                       seed,
//                                                                       increase_neighbouring)
        cout << "Not yet implemented:__choose_optimal_cc_triconnected_v2(...);" << endl;
        exit(1);
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
        short &compactness,
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


list<long> Agglomerator::__create_list_of_seeds(Coarse_Cell_Graph cc_graph,
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
    Dual_Graph graph = cc_graph._fc_graph;

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
        if ((*graph.seeds_pool).is_empty()) {
            // if list_of_seeds is empty
            // we look if there is some neighbour to the current fc:
            // s_fc_neighbours_of_cc = set()
            // we remove seed because we already update its first neighbours.
            unordered_set<long> tmp_set(s_current_cc);  // copy needed because the set is used inside ccg
            tmp_set.erase(seed);

            // We add to s_fc_neighbours_of_cc all the neighbours of FC included in s_fc_for_current_cc without seed
            for (auto &i_fc: tmp_set) {
                vector<long> a_neighbours = graph.get_neighbours(i_fc);
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

unordered_set<long> Agglomerator::__choose_optimal_cc_triconnected(
        Coarse_Cell_Graph cc_graph,
        const long &seed,
        unordered_map<long, unsigned short> &dict_neighbours_of_seed,
        const unsigned short goal_card,
        const unsigned short max_card,
        short &compactness,
        bool increase_neighbouring,
        unordered_set<long> s_of_fc_for_current_cc) {

//l_of_fc = list(d_neighbours_of_seed)
//
//# Number of fine cells constituting the current coarse cell in construction.
//if len(s_of_fc_for_current_cc) != 1:
//
//# TODO This seems to be not used???
//
//# this is a correction step
//is_correction_step = True
//
//s_of_fc_for_current_cc.remove(seed)
//l_of_fc.remove(seed)
//l_of_fc.extend(s_of_fc_for_current_cc)
//# increase_neighbouring = False
//i_neighbourhood_min = self.__min_neighbourhood_correction_step
//i_neighbourhood_max = self.__max_neighbourhood_correction_step
//
//else:
//
//# s_of_fc_for_current_cc contains only the seed
//is_correction_step = False
//i_neighbourhood_min = self.__min_neighbourhood
//i_neighbourhood_max = self.__max_neighbourhood
//
//cc_graph._a_is_fc_agglomerated[seed] = True  # the seed FC is agglomerated
//        cc_graph._nb_of_agglomerated_fc += 1
//
//s_of_fc: Set[int] = set(l_of_fc)
//i_neighbour = i_neighbourhood_min  # Initial size
//
//deg_seed: int = graph.compute_degree_of_node_in_subgraph(seed, s_of_fc)
//
//d_neighbours_of_seed_n_minus_one = d_neighbours_of_seed
//
//if increase_neighbouring and not is_correction_step:
//assert len(s_of_fc) + 1 >= goal_card
//# This is current criteria goal_card
//
//        dict_connected = dict()  # TODO  Pas tres utilise! utile?
//dict_biconnected = dict()
//is_triconnected_component_found = False
//
//# The first neighbourhood is connected
//dict_connected[i_neighbourhood_min] = set(s_of_fc_for_current_cc)  # copy needed
//dict_connected[i_neighbourhood_min].add(seed)
//
//inc_neighbouring = True
//while i_neighbour < i_neighbourhood_max and inc_neighbouring:
//
//# print "\t\ti_neighbour", i_neighbour,
//inc_neighbouring = increase_neighbouring
//
//if i_neighbour != i_neighbourhood_min:
//# if it is not the first step (because the neighbourhood has already been computed)
//if not is_correction_step:
//
//                    d_neighbours_of_seed, max_order_of_neighbourhood = \
//                        graph.compute_neighbourhood({seed},
//                                                    i_neighbour,
//                                                    test_function=cc_graph.is_fc_not_already_agglomerated,
//                                                    max_card=max_card,
//                                                    )
//                    l_of_fc = list(d_neighbours_of_seed)
//
//                else:
//
//                    d_neighbours_of_seed, max_order_of_neighbourhood = \
//                        graph.compute_neighbourhood(s_of_fc_for_current_cc,
//                                                    i_neighbour,
//                                                    test_function=cc_graph.is_fc_not_already_agglomerated,
//                                                    # is_fc_not_already_agglomerated_nor_seed,
//                                                    max_card=self.__max_card,  # __goal_card
//                                                    s_of_constrained_fc=None,
//                                                    )
//
//                    l_of_fc = list(d_neighbours_of_seed)
//                    l_of_fc.remove(seed)  # because the seed is in the neighbourhood of CC
//                    l_of_fc.extend(s_of_fc_for_current_cc)
//
//                if d_neighbours_of_seed == d_neighbours_of_seed_n_minus_one:
//                    # trouble: no more available cells. in the neighbourhood of seed
//                    break
//
//            l_of_fc.append(seed)
//            graph_l_connected = self.__generate_triconnected_graph(graph, l_of_fc)
//            # if graph.use_static_storage:
//            #     # building of a subgraph containing l_of_fc
//            #     row_ptr_l_connected, col_ind_l_connected, values_l_connected, _ = \
//            #         graph.compute_local_crs_subgraph_from_global_crs(l_of_fc)
//            #
//            #     # Restriction to Biconnected Subgraph
//            #     graph_l_connected = Graph_triconnected.Graph_triconnected(row_ptr=row_ptr_l_connected,
//            #                                                               col_ind=col_ind_l_connected,
//            #                                                               values=values_l_connected)
//            #
//            # else:
//            #     matrix_l, _ = graph.compute_local_crs_subgraph_from_global_crs(l_of_fc)
//            #     # Restriction to Biconnected Subgraph
//            #     graph_l_connected = Graph_triconnected.Graph_triconnected(adjacency_matrix=matrix_l)
//
//            is_biconnected_l, component_l = ug.BCC_NR(graph_l_connected)
//            # is_biconnected_l, component_l = graph_l_connected.BCC()
//
//            set_biconnected_component_g = None
//            for compo in component_l:
//                set_compo_g = set()
//                for i_l in compo:
//                    set_compo_g.add(l_of_fc[i_l])  # Local to global numbering
//
//                # TODO Choose it more wisely
//                if not is_correction_step:
//                    if seed in set_compo_g:
//                        set_biconnected_component_g = set_compo_g
//                        break
//                else:
//                    is_ok = True
//                    for i_fc in s_of_fc_for_current_cc:
//                        if i_fc not in set_compo_g:
//                            is_ok = False
//                            break
//                    if is_ok and seed in set_compo_g:
//                        set_biconnected_component_g = set_compo_g
//
//            if set_biconnected_component_g and deg_seed >= 3:
//
//                # we have a biconnected component containing seed.
//                list_of_node_biconnected = list(set_biconnected_component_g)  # indices from local to global
//                # if graph.use_static_storage:
//                #     row_ptr_l, col_ind_l, values_l, _ = graph.compute_local_crs_subgraph_from_global_crs(
//                #         set_biconnected_component_g)
//                #
//                #     graph_tri = Graph_triconnected.Graph_triconnected(row_ptr=row_ptr_l, col_ind=col_ind_l,
//                #                                                       values=values_l)
//                # else:
//                #     matrix_l, _ = graph.compute_local_crs_subgraph_from_global_crs(set_biconnected_component_g)
//                #     # Restriction to Biconnected Subgraph
//                #     graph_tri = Graph_triconnected.Graph_triconnected(adjacency_matrix=matrix_l)
//                graph_tri = self.__generate_triconnected_graph(graph, set_biconnected_component_g)
//
//                triconnected_components = graph_tri.computeTriconnectivity()
//
//                # Global numbering
//                triconnected_component_g = []
//                for compo in triconnected_components:
//                    set_compo_g = set()
//                    for i_l in compo:
//                        set_compo_g.add(list_of_node_biconnected[i_l])
//                    triconnected_component_g.append(set_compo_g)
//                # print "\tTriconnected Graph_triconnected", triconnected_component_g
//
//                for set_comp in triconnected_component_g:
//
//                    if not is_correction_step:
//
//                        if seed in set_comp:  # we have found a triconnected component containing seed
//                            # print "Triconnected", len(set_comp)
//                            # print "\t\tiNeigh", i_neighbour, "size CC", len(set_comp), ", size Neigh", len(l_of_fc)
//                            return set_comp, 3
//                    else:
//
//                        is_ok = True
//                        for i_fc in s_of_fc_for_current_cc:
//                            if i_fc not in set_comp:
//                                is_ok = False
//                                break
//                        if is_ok and seed in set_comp:
//                            return set_comp, 3
//
//                # if not is_triconnected_component_found...
//                # backup biconnected CC
//                dict_biconnected[i_neighbour] = list_of_node_biconnected
//
//            elif set_biconnected_component_g and deg_seed < 3:
//                # Deg of seed in the subgraph is lower than 3. We cannot found any triconnected component:
//                # print "Biconnected", len(set_biconnected_component_g)
//                return set_biconnected_component_g, 2
//            # else:
//            # # No biconnected component containing seed is found.
//            # dict_connected[i_neighbour] = s_of_fc
//
//            i_neighbour += 1
//            d_neighbours_of_seed_n_minus_one = d_neighbours_of_seed
//
//        if not is_triconnected_component_found:
//            # Treatment if non triconnected CC is found
//
//            # print "\n\ttrouble: no more available cells. No triconnected Coarse cell available"
//            if dict_biconnected:
//                # print "\tdict_biconnected", dict_biconnected
//                # We choose the biconnected CC, if any, with the smallest cardinal
//
//                for i_n in range(i_neighbourhood_min, i_neighbourhood_max):
//                    if i_n in dict_biconnected and len(dict_biconnected[i_n]) >= self.__min_card:
//                        s_1 = dict_biconnected[i_n]
//                        # print "Biconnected", len(s_1)
//                        return s_1, 2
//            # if dict_connected:
//            # for i_n in range(i_neighbourhood_min, i_neighbourhood_max):
//            #     if i_n in dict_connected:
//            #         # If we are here, we have no correct biconnected CC...
//            #         if increase_neighbouring or not is_correction_step:
//            #             assert len(dict_connected[i_n]) >= goal_card
//            #         s_1 = dict_connected[i_n]
//            #         print "Connected", len(s_1)
//            #         return s_1, 1
//            return dict_connected[i_neighbourhood_min], 1
    unordered_set<long> empty_set = {};
    return empty_set;
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

void Agglomerator::get_agglo_lines(int level,
                                   long *sizes,
                                   long *agglo_lines_array_idx,
                                   long *agglo_lines_array) {
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
        __v_lines[0] = __fc_graphs.compute_anisotropic_line_v2();  // finest level!!!

    }
    // This is not the first generation of a coarse level.
    // The anisotropic lines are given as input.

    // Copy of the current agglomeration_lines as a backup for visualization purpose.
    __v_lines[1] = copy_agglomeration_lines(__v_lines[0]);

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
    (*__cc_graphs).cc_create_a_cc(set_current_cc, is_anistropic, is_creation_delayed);
}


unordered_set<long> Agglomerator::__choose_optimal_cc_basic_v2(const long seed,
                                                               unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                               short &compactness,
                                                               const bool is_order_primary) {

//    cout << "Call of __choose_optimal_cc_basic_v2" << endl;

    // Number of fine cells constituting the current coarse cell in construction.
    unsigned short size_current_cc = 1; // CC contains only one cell: the seed

    // set of fc for current cc:
    unordered_set<long> s_fc_for_current_cc = {seed};

    unordered_set<long> s_seeds = {seed};

    // Compute self.__min_neighbourhood order neighbourhood:
    //======================================================
    unsigned short max_order_of_neighbourhood = __min_neighbourhood;
    __fc_graphs.compute_neighbourhood_of_cc(s_seeds,
                                            max_order_of_neighbourhood,   //in and out
                                            dict_neighbours_of_seed,
                                            __goal_card,
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
                short compactness = -1;
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