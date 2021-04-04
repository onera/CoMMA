//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_COARSE_CELL_H
#define COMMA_PROJECT_COARSE_CELL_H

#include "Dual_Graph.h"

class Coarse_Cell {
public:
    Coarse_Cell(Dual_Graph &fc_graph,
                long i_cc,
                unordered_set<long> &s_fc,
                bool is_isotropic = true,
                bool is_delayed = false
    );

    bool is_connected();

public:
    //protected
    bool _is_finalized = false;
public:
    //Private methods:
    bool __check_connectivity(int verbose = 0);

    //private Fields
    Dual_Graph *__fc_graph;
    short int __dim;
    bool __is_isotropic;
    bool __is_delayed;

    // Connectivity:
    bool __is_connected;
    bool __is_connectivity_up_to_date = false;//  # TODO useful for is_connected?

    unsigned short int __card;
    unsigned short int __compactness;

    unordered_map<long, unordered_map<long, double>> __d_def;  // subgraph describing the cc
    unordered_map<unsigned short int, unordered_set<long>> __d_compactness_to_s_fc; // TODO ordered dict????

    // TODO useful?
    unordered_map<long, double> __d_i_fc_to_volume;  // volumes associated with each inner fc.

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> __d_i_fc_to_j_cc_neighbourhood_to_j_fc;
    // The fine cell i_fc has edge to i_cc coarse cell via edges: (i_fc, j_fc) for some j_fc
    // i.e. i_fc in self.__icc, j_fc in icc

    // Only fine informations.
    // TODO useful after first fill_cc_neighbourhood?
    unordered_map<long, unordered_map<long, double>> __tmp_fc_fine_cut_edges;
    // Dict[i_fc: int, Dict[j_fc(out):int, surface_outer_face: float]] = dict()

    // Attention, this is a little bit complex:
    // the problem is that there is multiple outer degree:
    // for a inner fc:
    //
    //               # wrt fc        # wrt cc
    //#####################################################
    // fine edges    # Useless for   #  self.d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour
    //               # Agglomeration #
    //####################################################
    // coarse edges  #  No sense     #  Number Cc neighbours
    //
    unordered_map<long, unordered_map<long, unordered_set<long>>> d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour;
    // Dict[outer fine_cell_degree, Dict[int:i_fc, Set[int: j_cc]]]

    double __boundary_area = 0.0;

    double volume = 0.0;  // TODO useful?


    bool __is_cc_neighbourhood_build = false;//  # TODO useful????


    // USEFUL???
    // Pas clair
    //#########
    // Useless for debug only
    long __i_cc;
    // TODO useful? trouble with the use of card. The size may change during process
    // le remplacer par un set?
    unordered_set<unsigned short int> __is_already_connected;
    // self.__is_already_connected = np.zeros((self.card,), dtype=bool)

    //############################################
    // Primal_mesh
    //############################################
//    self.primal_mesh = primal_mesh
//    self.compute_convexity(s_fc)

};


#endif //COMMA_PROJECT_COARSE_CELL_H
