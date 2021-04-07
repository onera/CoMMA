//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_COARSE_CELL_GRAPH_H
#define COMMA_PROJECT_COARSE_CELL_GRAPH_H

#include "Dual_Graph.h"
#include "Coarse_Cell.h"

class Coarse_Cell_Graph {
public:
    Coarse_Cell_Graph(Dual_Graph& fc_graph,
                      int verbose=0,
                      vector<long> debug_only_fc_to_cc = vector<long>()
                      //debug_a_anisotropic_lines: Optional[List[Deque]] = None,
    );

//protected:
    int _verbose;

    Dual_Graph _fc_graph;

    //#############################
    // Temporary datas
    //###########################
    unordered_set<long> _s_cc_to_remove;

    long _nb_of_agglomerated_fc = 0;  // number of (already) agglomerated fine cells
    vector<bool> _a_is_fc_agglomerated;

    vector<unordered_set<long>> _delayed_cc ; // list of set of fc for too small cells. There are build at the end
    // because they will be the first to be deleted.
    long _cc_counter = 0;  // Counter of cc (even the ones deleted)

    // TODO mutualized Anisotropic with isotropic.
    unordered_map<long, Coarse_Cell> dict_Coarse_Cells;



};


#endif //COMMA_PROJECT_COARSE_CELL_GRAPH_H
