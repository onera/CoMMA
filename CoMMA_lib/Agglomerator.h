//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_AGGLOMERATOR_H
#define COMMA_PROJECT_AGGLOMERATOR_H

#include "Dual_Graph.h"

class Agglomerator {

public :
    Agglomerator(Dual_Graph &graph,
                 int verbose = 0,
                 bool is_visu_data_stored = false,
                 int dimension = 3,
                 bool checks = false
    );
private:
    int __verbose;
    bool __checks;

    // Agglomerator parameters:
    //=========================
    int __dimension;
    int __nb_of_coarse_level=-1;
    bool __is_anisotropic = false;  // TODO is it useful?  Is agglomeration anisotropic of isotropic
    string __kind_of_agglomerator;  // which kind of agglomerator: basic or triconnected

    short int __min_neighbourhood = 3;
    short int __max_neighbourhood = 6;

    short int __min_neighbourhood_correction_step=1;
    short int __max_neighbourhood_correction_step=3;
    short int __min_card      =-1;
    short int __max_card = -1;
    short int __goal_card = -1;
    short int __threshold_card = -1;

    vector<long> __l_nb_of_cells;
    vector<unordered_set<long>> __l_of_s_anisotropic_compliant_fc;
    vector<deque<long>> __lines;  // Agglomeration lines.

    // for every defined level (1 by default), contains the associated fine/coarse graph
    // e.g. self.__l_fc_graphs[0]= finest dual graph
    //      self.__l_fc_graphs[1]= first level coarse graph
//    vector<Dual_Graph> __l_fc_graphs;
//    vector<Dual_Graph> __l_cc_graphs;
};


#endif //COMMA_PROJECT_AGGLOMERATOR_H
