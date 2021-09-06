//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_AGGLOMERATOR_H
#define COMMA_PROJECT_AGGLOMERATOR_H

#include "Dual_Graph.h"
#include "Coarse_Cell_Graph.h"
#include "Util.h"
#include "First_Order_Neighbourhood.h"
#include "Triconnected_graph.h"
#include "Util_graph.h"
    //"""
    // Main Class of the CoMMA library, containing the methods to operate the agglomeration
    // process. It is constructed and the methods are prevalently called from the function agglomerate_one_level in
    // CoMMA.cpp. The constructor require the following parameters:
    //:param Dual_Graph: the struct Dual Graph 
    //:param verbose: [boolean]
    //:param is_visu_data_stored: parameter to store visualization data
    //:param dimension: dimension of the problem
    //:param checks: helper for the checks 
    //"""
class Agglomerator {

public:
   // Constructor 
   Agglomerator(Dual_Graph &graph,
                 unsigned short int verbose = 0,
                 bool is_visu_data_stored = false,
                 int dimension = 3,
                 bool checks = false
    );
    // Destructor
    ~Agglomerator() {
        cout << "Delete Agglomerator" << endl;
        if (__is_anisotropic) {
            clean_agglomeration_lines(__v_lines[0]);
            clean_agglomeration_lines(__v_lines[1]);
        }
        if (__cc_graphs != NULL) {
            delete __cc_graphs->_fc_graph.seeds_pool;
            delete __cc_graphs;
        }

    };
    // Function to understand if the agglomeration is anisotropic
    bool is_agglomeration_anisotropic();


    // Main function that creates the agglomeration 
    void agglomerate_one_level(const bool is_anisotropic,
                               unsigned long nb_aniso_agglo_lines,
                               forward_list<deque<long> *> &anisotropic_lines,
                               string kind_of_agglomerator = "basic",
                               const short goal_card = -1,
                               const short min_card = -1,
                               const short max_card = -1,
                               vector<long> debug_only_fc_to_cc = {},
                               const short debug_only_steps = -1);

    // Accessors
    inline long get_nb_cc() {
        return (*__cc_graphs)._cc_counter;
    };

    void get_agglo_lines(int level,
                         long *sizes,
                         vector<long> &agglo_lines_array_idx,
                         vector<long> &agglo_lines_array);

    inline vector<long> get_fc_2_cc() {
        return (*__cc_graphs)._fc_2_cc;
    }

//protected:
    // Initialize the main parameters of the Agglomeration process
    void _set_agglomeration_parameter(
            bool is_anisotropic,
            string kind_of_agglomerator = "basic",
            short goal_card = -1,
            short min_card = -1,
            short max_card = -1);

    unordered_set<long> _choose_optimal_cc_and_update_seed_pool(Coarse_Cell_Graph &cc_graph,
                                                                const long &seed,
                                                                unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                                const unsigned short goal_card,
                                                                const unsigned short max_card,
                                                                string kind_of_agglomerator,
                                                                unsigned short &compactness,
                                                                bool is_order_primary = false,
                                                                bool increase_neighbouring = true,
                                                                unordered_set<long> set_of_fc_for_current_cc = {});

    unordered_set<long> __choose_optimal_cc_basic_v2_sub(
            const long seed,
            unordered_map<long, unsigned short> &dict_neighbours_of_seed,
            unsigned short &compactness,
            const bool is_order_primary = false);

    unordered_set<long> __choose_optimal_cc_basic_v2(const long seed,
                                                     unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                     unsigned short &compactness,
                                                     const bool is_order_primary = false);
    void __agglomerate_one_level(
            vector<long> debug_only_fc_to_cc,
            short int debug_only_steps = -1  // arbitrary value greater than 7
    );



    list<long> __create_list_of_seeds(const Coarse_Cell_Graph &cc_graph,
                                      const long &seed,
                                      const unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                      unordered_set<long> s_current_cc);

//    unordered_set<long> __choose_optimal_cc_triconnected(
//            Coarse_Cell_Graph cc_graph,
//            const long &seed,
//            unordered_map<long, unsigned short> &dict_neighbours_of_seed,
//            const unsigned short goal_card,
//            const unsigned short max_card,
//            short &compactness,
//            bool increase_neighbouring,
//            unordered_set<long> s_of_fc_for_current_cc);

    unordered_set<long> __choose_optimal_cc_triconnected_v2(long seed,
                                                            unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                            unsigned short &compactness,
                                                            const bool increase_neighbouring,
                                                            short min_neighbourhood = -1);

    unordered_set<long> __choose_optimal_cc_triconnected_2D(long seed,
                                                            unordered_map<long, unsigned short> &dict_neighbours_of_seed,
                                                            unsigned short &compactness,
                                                            const bool increase_neighbouring,
                                                            short min_neighbourhood);

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

    void _agglomerate_one_level_anisotropic_part();

    void _agglomerate_one_level_isotropic_part(const short debug_only_steps);

    void __create_all_anisotropic_cc_wrt_agglomeration_lines();


      // Agglomerator parameters:
    //=========================
    int __dimension;
    bool __is_anisotropic = false;  // TODO is it useful?  Is agglomeration anisotropic of isotropic
    string __kind_of_agglomerator;  // which kind of agglomerator: basic or triconnected

    short __min_neighbourhood = 3;
    short int __max_neighbourhood = 6;

    short __min_neighbourhood_correction_step = 1;
    short __max_neighbourhood_correction_step = 3;
    short __min_card = -1;
    short __max_card = -1;
    short __goal_card = -1;  // TODO Make it const!!
    short __threshold_card = -1;

    vector<long> __l_nb_of_cells;

    // TODO rename __fc_graphs to __fc_graph
    Dual_Graph __fc_graphs;

    // TODO rename __cc_graphs to __cc_graph
    Coarse_Cell_Graph *__cc_graphs; 

    //======================
    // anisotopic specifics:
    //======================
    vector<unordered_set<long>> __v_of_s_anisotropic_compliant_fc;

    // Anisotropic agglomeration lines:
    vector<unsigned long> __v_nb_lines;
    // __v_lines : Agglomeration lines structure:
    // vector : level
    // forward list : identifier of the line
    // deque : line cells
    // e.g __v_lines[0] --> agglomeration lines at the finest level
    vector<forward_list<deque<long> *>> __v_lines;  
    // We store the current agglomeration_lines and the coarse one for visualization purpose.

    //=================
    // Visualization:
    //=================
    bool __is_visu_data_stored;

  //private fields
    int __verbose;
    bool __checks;
    //    private part
    void _agglomerate_sub_sub_isotropic_first_step();

    void _create_optimal_cc_v2(const long seed);

    unordered_set<long> _choose_optimal_cc_and_update_seed_pool_v2(const long seed,
                                                                   unsigned short &compactness,
                                                                   bool is_order_primary = false,  // _correction_split_too_big_cc_in_two(...)
                                                                   bool increase_neighbouring = true  // _correction_split_too_big_cc_in_two(...)
    );

    void _correction_main_basic(const short debug_only_steps = 10);

    void _correction_main_triconnected();

    void _correction_split_too_big_cc_in_two();

    Triconnected_graph *__generate_triconnected_graph(const unordered_set<long> &set_biconnected_component_g,
                                                      vector<long> &l_to_g) const;

    unordered_set<long> __compute_bic_components_from_s_connected_vertices_containing_seed(long seed,
                                                                                           const list<long> &l_of_fc,
                                                                                           bool is_correction_step,
                                                                                           unordered_set<long> s_of_fc_for_current_cc);

    unordered_set<long> __compute_tric_components_from_s_bic_vertices(unordered_set<long> s_bic_component_g,
                                                                      bool is_correction_step,
                                                                      long seed,
                                                                      unsigned short &compactness,
                                                                      unordered_set<long> s_of_fc_for_current_cc);


};


#endif //COMMA_PROJECT_AGGLOMERATOR_H
