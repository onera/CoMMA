//
// Created by Nicolas Lantos on 31/03/2021.
//

#ifndef COMMA_PROJECT_DUAL_GRAPH_H
#define COMMA_PROJECT_DUAL_GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <cmath>
#include <deque>
#include<queue>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <forward_list>
#include "Seeds_Pool.h"
#include "Util.h"
#include <iostream>
#include <climits>
#include <utility>
#include <numeric>

using namespace std;

/** @brief trick from
https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
 */
struct pair_hash {
    inline std::size_t operator()(const std::pair<int, int> &v) const {
        return v.first * 31 + v.second;
    }
};

/** @brief A class responsible of storing the cell centered dual graph
 * and of acting on it (in other words it implements the methods that complements
 *  the agglomeration, like the detection of anisotropic lines
 *  @author Alberto Remigi and Nicolas Lantos
 */
class Dual_Graph {

public :
/** @brief Constructor of the class
 *  @param[in] m_crs_row_ptr the row pointer of the CRS representation
 *  @param[in] m_crs_col_ind the column index of the CRS representation
 *  @param[in] m_crs_value the weight of the CRS representation (in CoMMA case will be the area
 *  of the faces that in the graph representation are the edges between two nodes represented
 *  by the cell centers.
 *  @param[in] volumes The volumes of the cells
 *  @param[in] d_is_on_bnd The dictionary of cells on boundary. In particular, it is built in CoMMA function
 *  and for each global index of cell on boundary it pairs the number of faces on boundary.
 *  @param[in]  is_on_corner see Mavriplis
 *  @param[in]  is_on_ridge see Mavriplis
 *  @param[in]  is_on_valley see Mavriplis
 *  @param[in]  s_anisotropic_compliant_fc set of complianr fc cells (in the most of the case all)
 *  @param[in]  verbose flag to have more information for debugging
 *  @param[in]  dimension  3 or 2 (by default 3)
 *
 */
    Dual_Graph(long nb_c,
               const vector<long> &m_crs_row_ptr,
               const vector<long> &m_crs_col_ind,
               const vector<double> &m_crs_values,
               const vector<double> &volumes,
               unordered_map<long, int> &d_is_on_bnd,
               unordered_set<long> is_on_corner = unordered_set<long>({}),
               unordered_set<long> is_on_ridge = unordered_set<long>({}),
               unordered_set<long> is_on_valley = unordered_set<long>({}),
               unordered_set<long> s_anisotropic_compliant_fc = unordered_set<long>({}),
               int verbose = 0,
               int dim = 3
    );

/** @brief Destructor of the class 
 *
 */ 
    ~Dual_Graph() {
        cout << "Delete Dual_Graph" << endl;
    }

/** @brief Number of cells in the mesh */ 
    int number_of_cells;
/** @brief Dimension of the problem */ 
    int _dimension;
/** @brief Flag to have more information for debugging */ 
    int _verbose;
/** @brief Vector of row pointer of CRS representation (member variable different from the unordered
 * set passed as a reference in input) */ 
    const vector<long> _m_CRS_Row_Ptr;
/** @brief Vector of column index of CRS representation (member variable different from the unordered
 * set passed as a reference in input) */ 
    const vector<long> _m_CRS_Col_Ind;
/** @brief Vector of area weight of CRS representation (member variable different from the unordered
 * set passed as a reference in input) */ 
    const vector<double> _m_CRS_Values;
/** @brief Vector of volumes (member variable different from the unordered
 * set passed as a reference in input) */ 
    const vector<double> _volumes;
/** @brief Seeds pool object pointer*/ 
    Seeds_Pool *seeds_pool;
/** @brief Member unordered set of compliant cells*/ 
    unordered_set<long> s_anisotropic_compliant_cells;
/** @brief Anisotropic lines list size*/ 
    int _lines_size = 0;
/** @brief List of deaue containing the anisotropic lines*/ 
    forward_list<deque<long> *> _lines;

    unsigned short get_nb_of_neighbours(long i_c);

    vector<long> get_neighbours(const long &i_c) const;

    vector<double> get_weights(const long &i_c) const;

    bool check_connectivity(unordered_set<long> s_fc, int verbose = 0);

    forward_list<deque<long> *> compute_anisotropic_line(long &nb_agglomeration_lines);

    void compute_d_anisotropic_fc(vector<double> &maxArray, unordered_map<long,double> &d_anisotropic_fc, unordered_map<long,double> &d_isotropic_fc);

    long _compute_subgraph_root(unordered_set<long> s_fc);

    void clean_d_neighbours_of_seed(unordered_set<long> s_fc, unordered_map<long, unsigned short> &d_neighbours_of_seed);

    unsigned short int compute_degree_of_node_in_subgraph(int i_fc, unordered_set<long> s_of_fc);

    vector<double> compute_aspect_ratio();

    vector<double> compute_aspect_ratio_and_characteristics(double &min, double &max, double &mean, double &sd, double &median);

    void compute_aspect_ratio_characteristics(double &min, double &max, double &mean, double &sd, double &median, double &min_aniso, double &max_aniso, double &mean_aniso, double &sd_aniso,
                                              double &median_aniso);

    void compute_breadth_first_search(unordered_set<long> set_of_fc, long current_seed, unordered_map<long, long> dict_inv_list_of_fc, vector<long> color, long &max_color,
                                      unordered_set<long> &s_fc_max_color, unordered_map<long, vector<long>> &d_spanning_tree);

    void compute_breadth_first_search_v2(unordered_set<long> set_of_fc, long current_seed, vector<long> &predecessor, long &i_depth, unordered_map<long, vector<long> > &d_spanning_tree);

    unsigned short int compute_min_fc_compactness_inside_a_cc(unordered_set<long> &s_fc);

    unordered_map<long, unsigned short int> compute_fc_compactness_inside_a_cc(unordered_set<long> &s_fc);

    vector<unordered_set<long>> compute_connected_components(const unordered_set<long> &s_initial_cc);

    unsigned short int compute_degree_of_node(int i_fc, bool (*test_function)(int) = nullptr);

    short compute_degree_of_node_not_a(const long &i_fc, vector<bool> a);

    void compute_local_crs_subgraph_from_global_crs(unordered_set<long> set_of_node, vector<long> &row_ptr_l, vector<long> &col_ind_l, vector<double> &values_l, vector<long> &g_to_l,
                                                    vector<long> &l_to_g) const;

    unordered_set<long> compute_s_leaves(unordered_set<long> s_fc);

    int compute_distance_from_seed_to_first_vertex_of_degree_2(long seed, unordered_map<long, queue<long> *> dict_Covering_Tree);

    unordered_map<long, queue<long> *> find_seed_via_frontal_method(long &max_seed,
                                                                    vector<long> listOfFineCells);

    void remove_separating_vertex(long seed,
                                  unordered_map<long, queue<long> *> &d_spanning_tree,
                                  unordered_set<long> &s_fc,
                                  long verbose = 0);


    void compute_neighbourhood_of_cc(const unordered_set<long> s_seeds,
                                     short &nb_of_order_of_neighbourhood,
                                     unordered_map<long, short> &d_n_of_seed,
                                     const short max_card,
                                     vector<bool> &is_fc_agglomerated_tmp,
                                     unordered_set<long> s_of_constrained_fc = {});
};


#endif //COMMA_PROJECT_DUAL_GRAPH_H
