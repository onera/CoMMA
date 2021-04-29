//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_AGGLOMERATOR_H
#define COMMA_PROJECT_AGGLOMERATOR_H

#include "Dual_Graph.h"
#include "Coarse_Cell_Graph.h"
#include "Util.h"

class Agglomerator {

public :
    Agglomerator(Dual_Graph &graph,
                 unsigned short int verbose = 0,
                 bool is_visu_data_stored = false,
                 int dimension = 3,
                 bool checks = false
    );

    ~Agglomerator() {
        if (__is_anisotropic) {
            delete __v_lines[0];
            delete __v_lines[1];

        }
    };

    void _set_agglomeration_parameter(
            bool is_anisotropic,
            string kind_of_agglomerator = "basic",
            unsigned short int goal_card = 0,
            unsigned short int min_card = 0,
            unsigned short int max_card = 0);

    Coarse_Cell_Graph *__agglomerate_one_level(
            vector<long> debug_only_fc_to_cc,
            short int debug_only_steps = -1  // arbitrary value greater than 7
    );

    unordered_set<long> _choose_optimal_cc_and_update_seed_pool(Coarse_Cell_Graph cc_graph,
                                                                const long &seed,
                                                                unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                                const unsigned short goal_card,
                                                                const unsigned short max_card,
                                                                string kind_of_agglomerator,
                                                                unsigned short &compactness,
                                                                bool is_order_primary = false,
                                                                bool increase_neighbouring = true,
                                                                unordered_set<long> set_of_fc_for_current_cc = {});

    unordered_set<long> __choose_optimal_cc_basic(
            Dual_Graph graph,
            const long &seed,
            unordered_map<long, unsigned short> &dict_neighbours_of_seed,
            const unsigned short goal_card,
            const unsigned short max_card,
            unsigned short &compactness,
            bool is_order_primary = false);

    list<long> __create_list_of_seeds(Coarse_Cell_Graph cc_graph,
                                      const long &seed,
                                      const unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                      unordered_set<long> s_current_cc);

    unordered_set<long> __choose_optimal_cc_triconnected(
            Coarse_Cell_Graph cc_graph,
            const long &seed,
            unordered_map<long, unsigned short> &dict_neighbours_of_seed,
            const unsigned short goal_card,
            const unsigned short max_card,
            unsigned short &compactness,
            bool increase_neighbouring,
            unordered_set<long> s_of_fc_for_current_cc);

    void initialize_l_cc_graphs_for_tests_only(Coarse_Cell_Graph *value);

    void __compute_ar_surf_adjacency_nb_face_common(const Dual_Graph &graph,
                                                    const long &i_fc,
                                                    const unordered_set<long> &s_of_fc_for_current_cc,
                                                    double &new_ar_surf,
                                                    bool &is_fc_adjacent_to_any_cell_of_the_cc,
                                                    unsigned short &number_faces_in_common) const;

    void __compute_best_fc_to_add(Dual_Graph &graph,
                                  unordered_set<long> fon,
                                  const unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                  const bool &is_order_primary,
                                  const double &cc_surf,
                                  const double &vol_cc,
                                  const unordered_set<long> &s_of_fc_for_current_cc,
                                  long &argmin_ar,
                                  unsigned short &max_faces_in_common,
                                  double &min_ar_surf,
                                  double &min_ar_vol);

    void agglomerate_one_level(const bool is_anisotropic,
                               string kind_of_agglomerator = "basic",
                               const short goal_card = -1,
                               const short min_card = -1,
                               const short max_card = -1,
                               unsigned long nb_aniso_agglo_lines = 0,
                               forward_list<deque<long> *> *anisotropic_lines = NULL,
                               vector<long> debug_only_fc_to_cc = {},
                               const short debug_only_steps = -1);

    //===========================
    // Accessors
    //===========================
    inline long get_nb_cc() {
        return (*__cc_graphs)._cc_counter;
    };

    void get_agglo_lines(int level,
                         long *sizes,
                         long *agglo_lines_array_idx,
                         long *agglo_lines_array);

    inline vector<long> get_fc_2_cc(){
        return (*__cc_graphs)._fc_2_cc;
    }

public:
    //private fields
    int __verbose;
    bool __checks;

    // Agglomerator parameters:
    //=========================
    int __dimension;
    bool __is_anisotropic = false;  // TODO is it useful?  Is agglomeration anisotropic of isotropic
    string __kind_of_agglomerator;  // which kind of agglomerator: basic or triconnected

    short int __min_neighbourhood = 3;
    short int __max_neighbourhood = 6;

    short int __min_neighbourhood_correction_step = 1;
    short int __max_neighbourhood_correction_step = 3;
    short int __min_card = -1;
    short int __max_card = -1;
    short int __goal_card = -1;
    short int __threshold_card = -1;

    vector<long> __l_nb_of_cells;

    Dual_Graph __fc_graphs;
    Coarse_Cell_Graph *__cc_graphs;

    //======================
    // anisotopic specifics:
    //======================
    vector<unordered_set<long>> __l_of_s_anisotropic_compliant_fc;

    // Anisotropic agglomeration lines:
    vector<unsigned long> __v_nb_lines;
    vector<forward_list<deque<long> *> *> __v_lines;

    //=================
    // Visualization:
    //=================
    bool __is_visu_data_stored;
};


#endif //COMMA_PROJECT_AGGLOMERATOR_H
