//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_COARSE_CELL_GRAPH_H
#define COMMA_PROJECT_COARSE_CELL_GRAPH_H

#include "Dual_Graph.h"
#include "Coarse_Cell.h"

class Coarse_Cell_Graph {
public:
    Coarse_Cell_Graph(Dual_Graph &fc_graph,
                      int verbose = 0,
                      vector<long> debug_only_fc_to_cc = vector<long>(),
                      forward_list<deque<long> *> *debug_a_anisotropic_lines = NULL);

    long cc_create_a_cc(const unordered_set<long> &s_fc,
                        bool is_anisotropic = false,
                        bool is_creation_delayed = false);

    void fill_cc_neighbouring();

    unordered_map<long, unordered_set<long>> get_d_cc_all();

    unordered_map<long, unordered_set<long>> get_d_cc_iso();

    unordered_map<long, unordered_set<long>> get_d_cc_aniso();

    unordered_map<unsigned short int, long> get_d_distribution_of_cardinal_of_isotropic_cc();

    unsigned short int get_cc_compactness(const long &i_cc);

    void cc_create_all_delayed_cc();

    unordered_set<long> cc_swap_fc(unordered_set<long> &s_fc,
                                   const long &i_origin_cc,
                                   const long &i_destination_cc);

    inline bool is_anisotropic_cc(const long &i_cc) const {
        return (!_d_anisotropic_cc.empty()) && (_d_anisotropic_cc.count(i_cc));
    }

    inline bool is_isotropic_cc(const long &i_cc) const {
        return (!_d_isotropic_cc.empty()) && (_d_isotropic_cc.count(i_cc));
    }

    inline bool is_fc_agglomerated_in_isotropic_cc(const long &i_fc) const {
        return _d_isotropic_cc.count(_fc_2_cc[i_fc]);
    }

    inline bool is_fc_not_already_agglomerated(const long &i_fc) const {
        return !_a_is_fc_agglomerated[i_fc];
    }

    inline bool is_agglomeration_done() {
        /**
         * The computation and/or recovery of cc depends of the data structure
         */
        return _nb_of_agglomerated_fc == _fc_graph.number_of_cells;
    }

    inline bool is_agglomeration_isotropic() {
        /**
         * The computation and/or recovery of cc depends of the data structure
         */
        return !_d_isotropic_cc.empty();
    }

    inline bool is_agglomeration_anisotropic() {
        /**
         * The computation and/or recovery of cc depends of the data structure
         */
        return !_d_anisotropic_cc.empty();
    }


    void __remove_an_isotropic_cc(const long &i_cc);

    void _cc_update_neighbours(const long &i_fc,
                               const unordered_set<long> &s_fc,
                               const long &i_origin_cc,
                               const long &i_dest_cc);

    void __add_a_cc(const long &i_cc);

    void cc_renumber();

    void _update_cc_neighbour(long min_cc, unordered_map<long, long> dict_old_cc_to_new_cc);

    bool check_cc_consistency();

    bool _check_consistency();

    bool __check_s_cc_to_remove_are_isotropic();

    bool check_data_consistency_and_connectivity();

    unordered_map<long, unsigned short> compute_nb_faces_in_common_faces_with_cc_neighbourhood(const long i_fc) const;

    unordered_map<long, unordered_set<long>> compute_dict_faces_in_common_faces_with_cc_neighbourhood(const long &i_fc) const;

    void compute_nb_faces_in_common_faces_with_cc_neighbourhood_w_unwanted_fc(const long &i_fc,
                                                                              const unordered_set<long> &s_unwanted_fc,
                                                                              unordered_set<long> &set_argmax_number_common_faces,
                                                                              unordered_map<long, unsigned short> &dict_adjacent_cc) const;

    unordered_set<long> get_s_isotropic_cc_of_size(unsigned short size_min = 0, unsigned short size_max = 0);

    void correction_make_small_cc_bigger(const unsigned short &min_card,
                                         const unsigned short &goal_card,
                                         const unsigned short &threshold_card,
                                         bool verbose);

    void cc_split_non_connected_cc(const long& i_cc);

    unordered_map<unsigned short, long> compute_d_distribution_of_cardinal_of_isotropic_cc();

    //protected:
    int _verbose;

    Dual_Graph _fc_graph;

    //==================
    // Temporary datas
    //==================
    unordered_set<long> _s_cc_to_remove;

    long _nb_of_agglomerated_fc = 0;  // number of (already) agglomerated fine cells
    vector<bool> _a_is_fc_agglomerated;

    vector<unordered_set<long>> _delayed_cc; // list of set of fc for too small cells. There are build at the end
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

    unordered_map<long, Coarse_Cell *> _d_isotropic_cc;
    // Contains the CC index: list of FC, that can be modified (not anisotropic CC, and not "boundary" CC!

    unordered_map<long, unordered_set<long>> _d_anisotropic_cc; // Dict of indices of anisotropic CC

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
