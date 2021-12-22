//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_COARSE_CELL_GRAPH_H
#define COMMA_PROJECT_COARSE_CELL_GRAPH_H

#include "Dual_Graph.h"
#include "Coarse_Cell.h"

/** @brief Create a Coarse Cell Graph, hence it is the
 *  container of the Coarse Cells created and 
 *  of the operation we can do on them.
 *  @param[in] Dual_Graph input element Dual_Graph to work on the seeds choice and the seeds pool
 *  @param[in] verbose it defines the verbose parameters
*/
class Coarse_Cell_Graph {

public:
/** @brief Constructor*/
    Coarse_Cell_Graph(Dual_Graph &fc_graph,
                      int verbose = 0);
/** @brief Destructor*/
    ~Coarse_Cell_Graph();
/** @brief Function that return if the coarse cell structure created is unstructured or structured (in dependence
 * of the anisotropic  cells found)
 * @param[in] goal_card goal cardinality, useful to check if in case of non presence of anisotropic cells we reached
 * the goal cardinality for all the coarse cells created */
    bool is_cc_grid_not_structured(short goal_card = -1);
/** @brief Helper to get the member variable that defines the number of agglomerated fine cells */ 
    inline long get_number_of_fc_agglomerated() {
        return (_nb_of_agglomerated_fc);
    }
/** @brief Helper to choose the new seed based on the already aggomerated fine cells*/
    inline long choose_new_seed() {
        return (*_fc_graph.seeds_pool).choose_new_seed(_a_is_fc_agglomerated);
    }
/** @brief Helper to get the number of coarse cells */
    inline long get_nb_of_cc() {
        return _cc_counter;
    }
/** @brief It creates a coarse cell based on the set of fine cells given as an input 
 * @param[in] s_fc set of fine cells passed as a reference
 * @param[in] is_anisotropic boolean that tells if we are in an anisotropic case or not
 * @param[in] is_creation_delayed based on the agglomerator instruction we define if we delay or not the agglomeration
 * @return a long with the global identifier of the coarse cell.*/
    long cc_create_a_cc(const unordered_set<long> &s_fc,
                        bool is_anisotropic = false,
                        bool is_creation_delayed = false);
/** @brief Vector of boolean for which the length is the number of fine cells and for which the value of i_fc cell is true 
* when the cell is agglomerated in a coarse cell */
    vector<bool> _a_is_fc_agglomerated;




    void correction_main_triconnected(short min_neighbourhood_correction_step,
                                      short goal_card,
                                      int verbose = false);

/** @brief Return a dictionary where the key is the cqrdinality of an isotropic cell and the value is the number
 * of the coarse cell that have the given cardinality.*/
    unordered_map<unsigned short, long> compute_d_distribution_of_cardinal_of_isotropic_cc();

    void print_d_distribution_of_cardinal_of_isotropic_cc();
/** @brief Add a set of fine cells to the actual coarse cell and update the related dictionaries 
 * @ param[in] set_of_fc_to_add set of fine cells to be added
 * @ param[in] i_target_cc index of targetted coarse cells */
    void cc_update_cc(unordered_set<long> set_of_fc_to_add, long i_target_cc);
// Corrections
/** @brief It analyse the _d_card_2_cc cycling on the keys (hence the cardinality) */

    void correction_remove_too_small_cc(const unsigned short &threshold_card);
/** @brief functions called in the heart of correction_remove_too_small_cc 
 * given i_fc, inside i_cc, we look for which coarse neighbour shares the most common faces with it.
 * @param[in] i_fc index of the fine cells
 * @param[in] s_unwanted_fc set of unwanted fine cells*/
    void compute_nb_faces_in_common_faces_with_cc_neighbourhood_w_unwanted_fc(const long &i_fc,
                                                                              const unordered_set<long> &s_unwanted_fc,
                                                                              unordered_set<long> &set_argmax_number_common_faces,
                                                                              unordered_map<long, unsigned short> &dict_adjacent_cc) const;


    void fill_cc_neighbouring();

    unordered_map<long, unordered_set<long>> get_d_cc_all();

    unordered_map<long, unordered_set<long>> get_d_cc_iso();

    unordered_map<long, unordered_set<long>> get_d_cc_aniso();

    unordered_map<unsigned short int, long> get_d_distribution_of_cardinal_of_isotropic_cc();

    unsigned short int get_cc_compactness(const long &i_cc);

    void cc_create_all_delayed_cc();

    unordered_set<long> cc_swap_fc(unordered_set<long> &s_fc,
                                   const long i_origin_cc,
                                   const long i_destination_cc);

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


    void cc_renumber();


    bool check_cc_consistency();

    bool check_data_consistency_and_connectivity();

    unordered_map<long, unsigned short> compute_nb_faces_in_common_faces_with_cc_neighbourhood(const long i_fc) const;

    unordered_map<long, unordered_set<long>> compute_dict_faces_in_common_faces_with_cc_neighbourhood(const long &i_fc) const;

    unordered_set<long> get_s_isotropic_cc_of_size(unsigned short size_min = 0, unsigned short size_max = 0);

    void correction_make_small_cc_bigger(const unsigned short &min_card,
                                         const unsigned short &goal_card,
                                         const unsigned short &threshold_card,
                                         bool verbose = false);

    void correction_reduce_too_big_cc(const unsigned short &goal_card,
                                      const unsigned short &verbose = 0);


    void cc_split_non_connected_cc(const long &i_cc);


    void cc_remove_deleted_cc(unordered_set<long> set_removed_cc);

    unsigned short int get_cc_cardinal(long i_cc);

    unordered_map<long, unordered_set<long>> compute_dict_faces_in_common_faces_with_cc_neighbourhood(long i_fc);

    void correction_swap_leaf_fc_v2(int nb_iteration = 5, Coarse_Cell_Graph *ccg_l_m_one = NULL, int verbose = 1);


    // Dual Graph
    // Coarse cells building
    // Currently not mutualized with anisotropic agglomeration!!!!
    //
    Dual_Graph _fc_graph;
    // TODO mutualized Anisotropic with isotropic.


    long _cc_counter = 0;  // Counter of cc (even the ones deleted)

    // Output datas

    vector<long> _fc_2_cc;
//    = -1 * np.ones(((*this)._fc_graph.number_of_cells,), dtype = np.
//    int)




protected :
    /** @brief Variable where are recorded the isotropic coarse cells. Index is the global index of the coarse
     * cell, key is the pointer to a Coarse_Cell element. */
    unordered_map<long, Coarse_Cell *> _d_isotropic_cc;
     /** @brief Number of agglomerated fine cells */
    long _nb_of_agglomerated_fc = 0; 
    /** @brief vector of the set of fine cells composing the too small coarse cells that will be built at the end of the agglomeration
     * process */
    vector<unordered_set<long>> _delayed_cc; 
    // because they will be the first to be deleted.
    /** @brief Variable where are recorded the anisotropic coarse cells. Index is the global index of the coarse
     * cell, key is the pointer to a Coarse_Cell element. */
    unordered_map<long, unordered_set<long>> _d_anisotropic_cc;     
    /** @brief Cardinality counter of the coarse cell. Key is the cardinality value is an unordered set of thei coarse cells with a given cardinality
     */
    unordered_map<unsigned short int, unordered_set<long>> _d_card_2_cc;  
    /** @brief Dictionary that contains the compactness as a key and a set of the compactness cells as a value*/
    unordered_map<unsigned short int, unordered_set<long>> _d_compactness_2_cc; 

   
    void __remove_an_isotropic_cc(const long &i_cc);

    void _cc_update_neighbours(const long &i_fc,
                               const unordered_set<long> &s_fc,
                               const long &i_origin_cc,
                               const long &i_dest_cc);

    void __add_a_cc(const long &i_cc);



    void _update_cc_neighbour(long min_cc, unordered_map<long, long> dict_old_cc_to_new_cc);

    bool _check_consistency();

    bool __check_s_cc_to_remove_are_isotropic();

    int _verbose;

    // Temporary datas
    unordered_set<long> _s_cc_to_remove;

};


#endif //COMMA_PROJECT_COARSE_CELL_GRAPH_H
