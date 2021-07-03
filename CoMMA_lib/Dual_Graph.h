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
#include "Metrics_And_Datas.h"

using namespace std;

struct pair_hash {
    inline std::size_t operator()(const std::pair<int, int> &v) const {
        return v.first * 31 + v.second;
    }
};

class Dual_Graph {

public :
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

    // Attributes
    int number_of_cells;
    int _dimension;
    int _verbose;
    const vector<long> _m_CRS_Row_Ptr;
    const vector<long> _m_CRS_Col_Ind;
    const vector<double> _m_CRS_Values;
    const vector<double> _volumes;

    // TODO manage this. A pointer is not what I want.
    Seeds_Pool *seeds_pool;
    unordered_set<long> s_anisotropic_compliant_cells;
    int _lines_size = 0;
    forward_list<deque<long> *> _lines;

    // Methods

    unsigned short get_nb_of_neighbours(long i_c);

    vector<long> get_neighbours(const long &i_c) const;

    vector<double> get_weights(const long &i_c) const;

    bool check_connectivity(unordered_set<long> s_fc, int verbose = 0);

    bool compute_anisotropic_line(long *sizes,
                                  long *arrayOfFineAnisotropicCompliantCells,
                                  long *agglomerationLines_Idx,
                                  long *agglomerationLines,
                                  bool verbose);

    forward_list<deque<long> *> compute_anisotropic_line_v2(long &nb_agglomeration_lines);

    unordered_map<long, double> __compute_d_anisotropic_fc(vector<double> &maxArray);

    unordered_map<long, unordered_set<pair<long, long>, pair_hash>> _compute_d_cut_edges(unordered_map<long, unordered_set<long>> d_cc);

    //unordered_map<long, unordered_set<Tuple>> _compute_d_cut_edges(unordered_map<long, unordered_set<long>> d_cc);
    long _compute_subgraph_root(unordered_set<long> s_fc);

    void clean_d_neighbours_of_seed(unordered_set<long> s_fc, unordered_map<long, unsigned short> &d_neighbours_of_seed);

    unsigned short int compute_degree_of_node_in_subgraph(int i_fc, unordered_set<long> s_of_fc);

    vector<double> compute_aspect_ratio();

    vector<double> compute_aspect_ratio_and_characteristics(double &min, double &max, double &mean, double &sd, double &median);

    void compute_aspect_ratio_characteristics(double &min, double &max, double &mean, double &sd, double &median, double &min_aniso, double &max_aniso, double &mean_aniso, double &sd_aniso,
                                              double &median_aniso);

    double compute_average_fc_compactness_inside_a_cc(unordered_set<long> set_of_fc);

    void compute_breadth_first_search(unordered_set<long> set_of_fc, long current_seed, unordered_map<long, long> dict_inv_list_of_fc, vector<long> color, long &max_color,
                                      unordered_set<long> &s_fc_max_color, unordered_map<long, vector<long>> &d_spanning_tree);

    void compute_breadth_first_search_v2(unordered_set<long> set_of_fc, long current_seed, vector<long> &predecessor, long &i_depth, unordered_map<long, vector<long> > &d_spanning_tree);

    unsigned short int compute_min_fc_compactness_inside_a_cc(unordered_set<long> &s_fc);

    unordered_map<long, unsigned short int> compute_fc_compactness_inside_a_cc(unordered_set<long> &s_fc);

    vector<unordered_set<long>> compute_connected_components(const unordered_set<long> &s_initial_cc);

    vector<double> compute_a_visualization_data(Available_Datas data_type);

    vector<double> compute_cells_color(vector<double> a_cells_color);

    void compute_mesh_characteristics(int &nb_cells, int &nb_iso, int &nb_aniso);

    void compute_characteristics(Available_Characteristic characteristic, double &min, double &max, double &mean, double &sd, double &median, double &min_aniso, double &max_aniso, double &mean_aniso,
                                 double &sd_aniso, double &median_aniso, int &nb_cells, int &nb_iso, int &nb_aniso);

    vector<long> get_vertices_iterable();

    unordered_map<long, vector<long> > compute_d_cc(vector<long> fc_2_cc);

    unordered_map<long, unordered_set<pair<long, long>, pair_hash>> compute_d_cc_edges(vector<long> fc_2_cc);

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
                                     unsigned short &nb_of_order_of_neighbourhood,
                                     unordered_map<long, unsigned short> &d_n_of_seed,
                                     const unsigned short max_card,
                                     vector<bool> &is_fc_agglomerated_tmp,
                                     unordered_set<long> s_of_constrained_fc = {});
};


#endif //COMMA_PROJECT_DUAL_GRAPH_H
