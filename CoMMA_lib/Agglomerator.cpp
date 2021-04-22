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
) : __verbose(verbose), __checks(checks), __dimension(dimension) {

    if (((*this).__dimension != 2) && ((*this).__dimension != 3)) {
        cerr << "Wrong definition of dimension !=2 and !=3" << endl;
        assert(((*this).__dimension == 2) || ((*this).__dimension == 3));
    }

    // for every defined level (1 by default), contains the number of cells
    // e.g. (*this).__l_nb_of_cells[0]= number of cells on finest level
    //      (*this).__l_nb_of_cells[1]= number of cells on the first coarse level
    (*this).__l_nb_of_cells.push_back(graph.number_of_cells);

    // Anisotropic agglomeration datas:
    // For every level, we have a set containing the admissible cells for anisotropy cell number:
    // For level 0, it is the cell number of prism or hexahedron ...
    (*this).__l_of_s_anisotropic_compliant_fc.push_back(graph.s_anisotropic_compliant_cells);

    // for every defined level (1 by default), contains the associated fine/coarse graph
    // e.g. (*this).__l_fc_graphs[0]= finest dual graph
    //      (*this).__l_fc_graphs[1]= first level coarse graph
    (*this).__l_fc_graphs.push_back(graph);

//    (*this).__l_cc_graphs: Optional[List[ccg.Coarse_Cell_Graph]] = None

    //=================
    // Visualization:
    //=================
    (*this).__is_visu_data_stored = is_visu_data_stored;

}


void Agglomerator::_set_agglomeration_parameter(
        short int nb_of_coarse_level,
        bool is_anisotropic,
        string kind_of_agglomerator,
        unsigned short int goal_card,
        unsigned short int min_card,
        unsigned short int max_card) {

    (*this).__is_anisotropic = is_anisotropic;

    if (nb_of_coarse_level != -1) {
        (*this).__nb_of_coarse_level = nb_of_coarse_level;
    } else {
        (*this).__nb_of_coarse_level = 100;
    }

    (*this).__kind_of_agglomerator = kind_of_agglomerator;
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
    if (min_card == 0) {
        (*this).__min_card = d_default_min_card[(*this).__dimension];
    } else {
        (*this).__min_card = min_card;
    }

    // Definition of (*this).__max_card
    if (max_card == 0) {
        (*this).__max_card = d_default_max_card[(*this).__dimension];
    } else {
        (*this).__max_card = max_card;
    }

    // Definition of (*this).__goal_card
    if (goal_card == 0) {
        (*this).__goal_card = d_default_goal_card[(*this).__dimension];
    } else {
        (*this).__goal_card = goal_card;
    }

    // Definition of (*this).__threshold_card
    (*this).__threshold_card = d_default_threshold_card[(*this).__dimension];
}


Coarse_Cell_Graph *Agglomerator::__agglomerate_one_level(
        Dual_Graph graph,
        vector<long> debug_only_fc_to_cc,
        short int debug_only_steps,  // arbitrary value greater than 7
        forward_list<deque<long> *> *agglo_lines,
        forward_list<deque<long> *> *agglo_lines_p_one) {

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
        // ccg = CCG_w_CC.Coarse_Cell_Graph_w_CC(graph, i_lvl=i_lvl)
        // ccg = CCG_1.Coarse_Cell_Graph_v1(graph, i_lvl=i_lvl)
//        ccg = self._initialize_ccg(graph, i_lvl = i_lvl);
        ccg = new Coarse_Cell_Graph(graph);


        // TODO On pourrait ne l'allouer qu'une seule fois!

//        // Is the agglomeration anisotropic?
//        if(self.__is_anisotropic){
//            aniso_lines_l_m_one, aniso_lines_l = self._agglomerate_one_level_anisotropic_part(i_lvl,
//                                                                                              ccg,
//                                                                                              aniso_lines);
//        }
//
//        self._agglomerate_one_level_isotropic_part(i_lvl, ccg, debug_only_steps = debug_only_steps)
    } else {

// For debug purpose:

// ccg = CCG_1.Coarse_Cell_Graph_v1(graph, i_lvl=i_lvl, debug_only_fc_to_cc=debug_only_fc_to_cc)
// ccg = CCG_w_CC.Coarse_Cell_Graph_w_CC(graph, i_lvl=i_lvl, debug_only_fc_to_cc=debug_only_fc_to_cc)
//        ccg = self._initialize_ccg(graph, i_lvl = i_lvl, debug_only_fc_to_cc = debug_only_fc_to_cc)
//        ccg.fill_cc_neighbouring()
//        self.__l_nb_of_cells.append(ccg.nb_cc)
        cout << "toto";
        ccg = new Coarse_Cell_Graph(graph);
    }
    return ccg;//, aniso_lines_l_m_one, aniso_lines_l;

}


unordered_set<long> Agglomerator::_choose_optimal_cc_and_update_seed_pool(Coarse_Cell_Graph cc_graph,
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

//    graph = cc_graph._fc_graph
    // Definition of the current cc
    // remark: does contain seed
    unordered_set<long> s_current_cc;

    // l_fc_added with chronological order
    // remark: does not contain seed


    if (kind_of_agglomerator == "basic") {

        s_current_cc = __choose_optimal_cc_basic(cc_graph._fc_graph, seed,
                                                 dict_neighbours_of_seed,
                                                 goal_card,
                                                 max_card,
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

unordered_set<long> Agglomerator::__choose_optimal_cc_basic(
        Dual_Graph graph,
        const long &seed,
        unordered_map<long, unsigned short> &dict_neighbours_of_seed,
        const unsigned short goal_card,
        const unsigned short max_card,
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
    unsigned short min_size = goal_card;

    // Computation of the initial aspect ratio: we need cc_surf: i.e. the external area (perimeter in 2D and sum of external faces in 3D) and volume
    double cc_surf = 0.0;
    vector<double> a_neighbours_weights = graph.get_weights(seed);
    unsigned short nb_neighbours = graph.get_nb_of_neighbours(seed);
    for (const double &w:a_neighbours_weights) {
        cc_surf += w;
    }

    // volume of cc is at first the volume of the seed.
    double vol_cc = graph._volumes[seed];

    // This dictionary is used to store the eligible cc: i.e. its size is inside the permitted range.
    // This is useful to track back our step if needed.

    unordered_map<unsigned short, pair<unordered_set<long>, unordered_map<long, int>>> dict_cc_in_creation;
    //    Dict[int, List[Set[int], Dict[int, int]]]
    // dict_cc_in_creation[size_current_cc] = [set(s_of_fc_for_current_cc),
    //                                         {index of the last added fc: order (neighbourhood) of the last added fc}]
    double min_external_faces = numeric_limits<double>::max();
    int arg_min_external_faces = min_size;

    unsigned short max_ind = min(max_card, (unsigned short) (dict_neighbours_of_seed.size() + 1));
    int number_of_external_faces_current_cc = nb_neighbours + (*graph.seeds_pool).boundary_value(seed) - 1;
    // print("\nseed=", seed)

    unordered_set<long> s_neighbours_of_seed = d_keys_to_set(dict_neighbours_of_seed);

    First_Order_Neighbourhood f_o_neighbourhood = First_Order_Neighbourhood(s_neighbours_of_seed);

    vector<long> v = graph.get_neighbours(seed);
    unordered_set<long> s_up(v.begin(), v.end());

    unordered_set<long> fon = f_o_neighbourhood.update(seed, s_up);

    // Choice of the fine cells to agglomerate
    while (size_current_cc < max_ind) {

        long argmin_ar = -1;
        double min_ar_surf = numeric_limits<double>::max();
        double min_ar_vol = numeric_limits<double>::max();
        unsigned short max_faces_in_common = 0;

        __compute_best_fc_to_add(graph,
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


        number_of_external_faces_current_cc += graph.get_nb_of_neighbours(argmin_ar)
                                               + (*graph.seeds_pool).boundary_value(argmin_ar) - 1 - 2 * max_faces_in_common;

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

        vector<long> v = graph.get_neighbours(argmin_ar);
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
    compactness = graph.compute_min_fc_compactness_inside_a_cc(s_of_fc_for_current_cc);
    return s_of_fc_for_current_cc;
}


list<long> Agglomerator::__create_list_of_seeds(Coarse_Cell_Graph cc_graph,
                                                const long &seed,
                                                const unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                unordered_set<long> s_current_cc) {
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
        const unsigned short & order = dict_neighbours_of_seed.at(i_fc); // [i_fc] is not const.

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
        unsigned short &compactness,
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

void Agglomerator::initialize_l_cc_graphs_for_tests_only(Coarse_Cell_Graph &ccg) {
    __l_cc_graphs.push_back(ccg);
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
