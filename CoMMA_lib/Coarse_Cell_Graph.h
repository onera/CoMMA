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
                      vector<long> debug_only_fc_to_cc = vector<long>(),
                      forward_list<deque<long> *> *debug_a_anisotropic_lines = NULL);

    long cc_create_a_cc(unordered_set<long> &s_fc,
                        bool is_anisotropic = false,
                        bool is_creation_delayed = false);
    void fill_cc_neighbouring();
//    unordered_map<long, unordered_set<long>*>* get_d_cc_all();
    unordered_map<long, unordered_set<long>> get_d_cc_all();

    unordered_map<unsigned short int, long> get_d_distribution_of_cardinal_of_isotropic_cc();
    unsigned short int get_cc_compactness(const long& i_cc);

    //protected:
    int _verbose;

    Dual_Graph _fc_graph;

    //==================
    // Temporary datas
    //==================
    unordered_set<long> _s_cc_to_remove;

    long _nb_of_agglomerated_fc = 0;  // number of (already) agglomerated fine cells
    vector<bool> _a_is_fc_agglomerated;

    vector<unordered_set<long>> _delayed_cc ; // list of set of fc for too small cells. There are build at the end
    // because they will be the first to be deleted.

    // TODO mutualized Anisotropic with isotropic.
    //    unordered_map<long, Coarse_Cell> dict_Coarse_Cells;

    //==================
    // Coarse Graph:
    //==================

    // Coarse cells building
    // Currently not mutualized with anisotropic agglomeration!!!!

    long _cc_counter = 0;  // Counter of cc (even the ones deleted)

    // TODO mutualized Anisotropic with isotropic.

    unordered_map<long, Coarse_Cell*> _d_isotropic_cc;
    // Contains the CC index: list of FC, that can be modified (not anisotropic CC, and not "boundary" CC!

    unordered_map<long, unordered_set<long>>  _d_anisotropic_cc; // Dict of indices of anisotropic CC

    // Pointers from caracteristics to cc index:
    unordered_map<unsigned short int, unordered_set<long>> _d_card_2_cc;  // Contains the CC that can be modified, indexed by their
    // cardinal i.e. {card: set of index of coarse cells of cardinal card}

    unordered_map<unsigned short int, unordered_set<long>> _d_compactness_2_cc; // Contains for every degree of compactness the list of
    // CC of this type i.e. {deg_Of_compactness: set of CC of compactness deg_Of_compactness}

    //==================
    // Output datas
    //==================
    vector<long> _fc_2_cc;
//    = -1 * np.ones(((*this)._fc_graph.number_of_cells,), dtype = np.
//    int)



};


#endif //COMMA_PROJECT_COARSE_CELL_GRAPH_H
