//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_COARSE_CELL_H
#define COMMA_PROJECT_COARSE_CELL_H

#include "Dual_Graph.h"
#include <memory>

/** @brief Class implementing a Coarse_Cell object.
 * @param[in] fc_graph Dual_Graph object from where are taken the set of fine cells to create the 
 * coarse cell.
 * @param[in] global index of the coarse cell
 * @param[in] s_fc unordered set of fine cells constituting the coarse cell
 * @param[in] is_isotropic boolean describing if the cell is coming from an isotropic agglomeration process
 * or an anisotropic agglomeration process. The default value is set to true.
 * @param[in] is_delayed boolean determing if the agglomeration process is delayed or not. The default value is set
 * to true */

class Coarse_Cell {
public:
    Coarse_Cell(Dual_Graph &fc_graph,
                long i_cc,
                const unordered_set<long> &s_fc,
                bool is_isotropic = true,
                bool is_delayed = false
    );

    ~Coarse_Cell() {
    };
/** @brief Method that return a boolean determing if the Coarse Cell is defined by a connected sub-graph or not.
 *  @return true if the subgraph is connected, false if the subgraph is not connected*/
    bool is_connected();

/** @brief mapping vector. The position of the index is the local node, the value is the global*/
    vector<long> _mapping_g_to_l;
/** @brief The row pointer of the CSR representation of the subgraph*/
    vector<long> _adjMatrix_row_ptr;
/** @brief The column index representation of the CSR representation*/
    vector<long> _adjMatrix_col_ind;
/** @brief The area value of the internal fine cells*/
    vector<double> _adjMatrix_areaValues;
/** @brief The volumes of the internal fine cells*/
    vector<double> _volumes;
/** @brief shared pointer of the subgraph structure (CSR representation)*/
    shared_ptr<Subgraph> _cc_graph;
/** @brief function to build the local CSR subgraph representation
 * @return a vector representing the local to global mapping.*/
    vector<long> build_CRS();

/** @brief   The fine cell i_fc has edge to j_cc coarse cell via edges: (i_fc, j_fc) for some j_fc
 *  i.e. i_fc in self.__icc, j_fc in j_cc 
 *  i_fc and j_cc are related to two different coarse
 *  cells and connected thrpugh the outer face of j_fc.
  * i_fc  | j_cc | i_cc | Area |
 * ------------- | -------------|----------|--------|
 *  8  | 0 | 4 |1 | 
 *  9 | 0 | 5 | 1 |
 *  6 | 0 | 5 | 1 |
 *  2 | 0 | 1 | 1 |
 *  \image html dict_neigh.png*/



 
   unordered_map<long, unordered_map<long, unordered_map<long, double>>> __d_i_fc_to_j_cc_neighbourhood_to_j_fc;
/** @brief Dictionary where it is represented the neighborhood of a given fine cells with respect to the external
 * neighborhood to the coarse cell to which is owned. E.g the table for the cell 0 of the example (the table is completed for all the cells in the code
 * here we give as example only one part):
   * i_fc  | j_fc | Area |
 * ------------- | -------------|----------|
 *  4  | 8 | 1 | 
 *  5 | 9 | 1 |
 *  5 | 6 | 1 | 
 *  1 | 2 | 1 |
 *  \image html aretes.png*/
    unordered_map<long, unordered_map<long, double>> __tmp_fc_fine_cut_edges;
/** @brief helper to retrive the set of fine cells with an outer neighborhood*/
    unordered_set<long> get_s_fc_w_outer_neighbours(unsigned short int min_degree = 0);
/** @brief variable defining ig the cell is finalized or not */
    bool _is_finalized = false;
/** @brief Helper to add to the dictionary the value related. i_fc and j_cc are related to two different coarse
 * cells and connected thrpugh the outer face of j_fc.
 * i_fc  | j_cc | i_cc | Area |
 * ------------- | -------------|----------|--------|
 *  8  | 0 | 4 |1 | 
 *  9 | 0 | 5 | 1 |
 *  6 | 0 | 5 | 1 |
 *  2 | 0 | 1 | 1 |
 *  \image html dict_neigh.png*/
 

    void __add_to__d_i_fc_to_j_cc_neighbourhood_to_j_fc(long i_fc, long j_cc, long j_fc, double j_fc_area);
/** @brief By cycling on the dictionary of the  connections fine cells coarse cells neighborhoods it gives back
 * the set with the global index of the coarse cell neighborhood.
 */
    unordered_set<long> get_s_cc_neighbours();
/** @brief Dictionary where it is represented the neighborhood of a given fine cells with respect to the INTERNAL (opposite of the __tmp_fc_fine_cut_edges structure)
 * neighborhood to the coarse cell to which is owned. E.g the table for the cell 0 of the example (the table is completed for all the cells in the code, here we give as example
 * only one part):
   * i_fc  | j_fc | Area |
 * ------------- | -------------|----------|
 *  4  | 0 | 1 | 
 *  4 | 5 | 1 |
 *  5 | 1 | 1 | 
 *  5 | 4 | 1 |
 *  0 | 1 | 1 |
 *  0 | 4 | 1 |
 *  1 | 0 | 1 |
 *  1 | 5 | 1 |
 *  \image html aretes.png*/

    unordered_map<long, unordered_map<long, double>> __d_def; 
/** @brief Dictionary of the degree of the fine cell with respect to the coarse cell. The dictionary 
 * will be composed like this:
 * nb_edges |i_fc  | j_cc (set) | 
 * ------------- | -------------|----------|
 *  2  | 6 | 1,0 | 
 *  2 | 9 | 1,0 | 
 *  1 | 7 | 1 | 
 *  \image html ar_deg.png*/
 



    unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>> d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour;
/** @brief fill the dictionary for the present Coarse Cell*/
    void __compute_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour();
/** @brief erase completely the value related to i_fc,j_cc,j_fc */
    void __delete_and_propagate_deletion__d_i_fc_to_j_cc_neighbourhood_to_j_fc(const long &i_fc,
                                                                               const long &j_cc,
                                                                               const long &j_fc);

/** @brief erase completely the value related to i_fc,j_fc */
    void __delete_and_propagate_deletion__tmp_fc_fine_cut_edges(const long &i_fc,
                                                                const long &j_fc);

/** @brief With this function we fill the dictionary of neighborhoodi and we build up the coarse cell neighnorhood.*/
    void fill_cc_neighbouring(vector<long> &fc_2_cc);
/** @brief Add a set of fine cells to the actual coarse cell.
 *  @param[in] s_fc_to_add set of fine cells to add.
 *  @param[in] fc_2_cc vector of the global number of coarse cells assigned to the fine cells.*/
    void add_fc(unordered_set<long> s_fc_to_add, vector<long> fc_2_cc);
/** @brief Compute the compactness of a coarse cell */
    inline void __compute_compactness(){
        unsigned short int max_comp = 0;
        for (auto &i_k_v :__d_compactness_to_s_fc) {
            if (max_comp < i_k_v.first) {
                max_comp = i_k_v.first;
            }
        }
        for (unsigned short int i_comp = 0; i_comp < max(max_comp + 1, 4); i_comp++) {
            if (__d_compactness_to_s_fc.count(i_comp) > 0 && !__d_compactness_to_s_fc[i_comp].empty()) {
                __compactness = i_comp;
                break;
            }
        }
    }
    void __remove_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(unsigned short int nb_fine_edges_wrt_cc, long i_fc, long i_cc_old);

    void __add_one_to_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour(unsigned short int nb_fine_edges_wrt_cc, long i_fc, long i_cc);



    unordered_set<long> compute_s_leaves();

    bool check_consistency(vector<long> &fc_2_cc);

    void update_cc_neighbour(long i_fc, long i_fc_n, long i_cc_old, long i_cc_new);

    unordered_set<long> get_s_fc();  //Karim
    unordered_set<long> *get_s_fc_v2();  //Mine

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> get_d_fc_w_outer_neighbours_j_cc_and_s_j_fc();

    void remove_fc(unordered_set<long> s_fc_to_remove, vector<long> fc_2_cc);
    void update_icc(long new_i_cc);

    void update_cc_neighbour_renumbering(unordered_map<long, long> dict_old_cc_to_new_cc);
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
    // if cc is of size 1, the __d_def map will not work
    unordered_set<long> __s_fc; 
     
    unordered_map<unsigned short int, unordered_set<long>> __d_compactness_to_s_fc; // TODO ordered dict????

    unordered_map<long, double> __d_i_fc_to_volume;  // volumes associated with each inner fc.

   /** @brief Total boundary area */
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

};


#endif //COMMA_PROJECT_COARSE_CELL_H
