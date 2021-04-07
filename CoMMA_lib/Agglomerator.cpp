//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Agglomerator.h"

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

    if(!debug_only_fc_to_cc.empty()){
        assert(debug_only_steps==-1);
    }
    Coarse_Cell_Graph* ccg;
    if(debug_only_fc_to_cc.empty()) {

        // General case
        // Creation of the CCG
        // ccg = CCG_w_CC.Coarse_Cell_Graph_w_CC(graph, i_lvl=i_lvl)
        // ccg = CCG_1.Coarse_Cell_Graph_v1(graph, i_lvl=i_lvl)
//        ccg = self._initialize_ccg(graph, i_lvl = i_lvl);
        ccg= new Coarse_Cell_Graph(graph);


        // TODO On pourrait ne l'allouer qu'une seule fois!

//        // Is the agglomeration anisotropic?
//        if(self.__is_anisotropic){
//            aniso_lines_l_m_one, aniso_lines_l = self._agglomerate_one_level_anisotropic_part(i_lvl,
//                                                                                              ccg,
//                                                                                              aniso_lines);
//        }
//
//        self._agglomerate_one_level_isotropic_part(i_lvl, ccg, debug_only_steps = debug_only_steps)
    }
    else {

// For debug purpose:

// ccg = CCG_1.Coarse_Cell_Graph_v1(graph, i_lvl=i_lvl, debug_only_fc_to_cc=debug_only_fc_to_cc)
// ccg = CCG_w_CC.Coarse_Cell_Graph_w_CC(graph, i_lvl=i_lvl, debug_only_fc_to_cc=debug_only_fc_to_cc)
//        ccg = self._initialize_ccg(graph, i_lvl = i_lvl, debug_only_fc_to_cc = debug_only_fc_to_cc)
//        ccg.fill_cc_neighbouring()
//        self.__l_nb_of_cells.append(ccg.nb_cc)
cout<<"toto";
        ccg= new Coarse_Cell_Graph(graph);
    }
    return ccg;//, aniso_lines_l_m_one, aniso_lines_l;

}