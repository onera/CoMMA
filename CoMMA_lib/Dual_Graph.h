//
// Created by Nicolas Lantos on 31/03/2021.
//

#ifndef COMMA_PROJECT_DUAL_GRAPH_H
#define COMMA_PROJECT_DUAL_GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <vector>
#include <algorithm>
#include <climits>

#include "Seeds_Pool.h"

using namespace std;

class Dual_Graph;
class Subgraph;

/** @brief An interface class responsible of storing the cell centered dual
 * graph
 * and of acting on it (it is an interface for the global Dual Graph and the
 * Subgraph)
 */
class Graph {
 public:
  /** @brief Constructor of the class
   *  @param[in] m_crs_row_ptr the row pointer of the CRS representation
   *  @param[in] m_crs_col_ind the column index of the CRS representation
   *  @param[in] m_crs_value the weight of the CRS representation (in CoMMA case
   * will be the area
   *  of the faces that in the graph representation are the edges between two
   * nodes represented
   *  by the cell centers.
   *  @param[in] volumes The volumes of the cells
   */
  Graph(const long &nb_c, const vector<long> &m_crs_row_ptr,
        const vector<long> &m_crs_col_ind, const vector<double> &m_crs_values,
        const vector<double> &volumes);
  ~Graph() {}
  /** @brief Number of nodes in the Graph (it corresponds to the number of cells
   * in the subgraph or the dual graph. */
  int _number_of_cells;
  /** @brief helper vector for the DFS*/
  vector<bool> _visited;
  /** @brief Dimension of the problem */
  short _dimension;
  /** @brief Vector of row pointer of CRS representation (member variable
   * different from the unordered
   * set passed as a reference in input) */
  vector<long> _m_CRS_Row_Ptr;
  /** @brief Vector of column index of CRS representation (member variable
   * different from the unordered
   * set passed as a reference in input) */
  vector<long> _m_CRS_Col_Ind;
  /** @brief Vector of area weight of CRS representation (member variable
   * different from the unordered
   * set passed as a reference in input) */
  vector<double> _m_CRS_Values;
  /** @brief Vector of volumes (member variable different from the unordered
   * set passed as a reference in input) */
  vector<double> _volumes;
  /** @brief Depth First Search (DFS) recursive function
   *  @param[in] i_fc index of the node to print*/
  void DFS(const long &i_fc);
  /** @brief Breadth First Search (BFS) function
   *  @param[in] root  root of the spanning tree*/
  void BFS(const long &root);
  /** @brief Retrive the number of neighbours
   *  @param[in] i_c index of the cell
   *  @return number of neighbors of the given cell.**/
  unsigned short get_nb_of_neighbours(long i_c);
  /** @brief Based on the CRS representation retrives the neighbours of the cell
   * given as an input.
   * @param[in] i_c index of the cell to check the neighbours
   * @return vector of the neighbors. **/
  vector<long> get_neighbours(const long &i_c) const;
  /** @brief Based on the area of the faces composing the cell given as an
   * input, we retrive the faces connecting the given cell with the neighborhood
   * that can be described also as the weight of the graph
   * @return vector of weight associated to the cell.**/
  vector<double> get_weights(const long &i_c) const;

  /** @brief Check the connectivity of the given Graph.
   * @return True if the graph is connected, false if it is not connected**/
  bool check_connectivity();
};

/** @brief A class implementing the CRS subgraph representation. It is used in
 * the framework of CoMMA for the implementation of the CSR representation of
 * the coarse cells.
 */

class Subgraph : public Graph {
 public:
  /** @brief Constructor of the class
   *  @param[in] m_crs_row_ptr the row pointer of the CRS representation
   *  @param[in] m_crs_col_ind the column index of the CRS representation
   *  @param[in] m_crs_value the weight of the CRS representation (in CoMMA case
   * will be the area
   *  of the faces that in the graph representation are the edges between two
   * nodes represented
   *  by the cell centers.
   *  @param[in] volumes The volumes of the cells
   */
  Subgraph(const long &nb_c, const vector<long> &m_crs_row_ptr,
           const vector<long> &m_crs_col_ind,
           const vector<double> &m_crs_values, const vector<double> &volumes,
           const vector<long> &mapping_l_to_g, const bool &is_isotropic);
  ~Subgraph() {
    //      cout << "Delete Dual_Graph" << endl;
  }
  /** @brief it originates from an isotropic cell.*/
  bool _is_isotropic;
  /** @brief Cardinality of the given subgraph, alias the number of nodes
   * contained*/
  long _cardinality = 0;
  /** @brief Compactness of the given subgraph */
  long _compactness = 0;
  /** @brief Mapping from the local number of node to the global. Being a
   * subgraph this variable connect the local index of the node
   * with the global one*/
  vector<long> _mapping_l_to_g;
  /** @brief Insert a node in the subgraph and add it to the mapping the
   *  @param[in] v_neigh vector of the neighbors to be added. The neighbors must
   * be given in the global indexing system.
   *  @param[in] i_fc global index of the node
   *  @param[in] volume Volume of the cell
   *  @param[in] weight vector of the area of the faces of the given cells to be
   * added.*/
  void insert_node(vector<long> &v_neigh, const long &i_fc,
                   const double &volume, const vector<double> &weight);
  /** @brief Remove a node from the CRS representation and automatically adjust
   * the mapping
   *  @param[in] elemento global index of the node to be deleted.*/
  void remove_node(const long &elemento);
};

/** @brief A class implementing the CRS global graph representation.
 */
class Dual_Graph : public Graph {

 public:
  /** @brief Constructor of the class
   *  @param[in] m_crs_row_ptr the row pointer of the CRS representation
   *  @param[in] m_crs_col_ind the column index of the CRS representation
   *  @param[in] m_crs_value the weight of the CRS representation (in CoMMA case
   * will be the area
   *  of the faces that in the graph representation are the edges between two
   * nodes represented
   *  by the cell centers.
   *  @param[in] volumes The volumes of the cells
   *  @param[in] seeds_pool The seeds pool structure
   *  @param[in]  s_anisotropic_compliant_fc set of complianr fc cells (in the
   * most of the case all)
   *  @param[in]  verbose flag to have more information for debugging
   *  @param[in]  dimension  3 or 2 (by default 3)
   */
  Dual_Graph(const long &nb_c, const vector<long> &m_crs_row_ptr,
             const vector<long> &m_crs_col_ind,
             const vector<double> &m_crs_values, const vector<double> &volumes,
             const Seeds_Pool<long> &seeds_pool,
             const unordered_set<long> &s_anisotropic_compliant_fc =
                 unordered_set<long>({}),
             int verbose = 0, short dim = 3);

  /** @brief Destructor of the class
   *
   */
  ~Dual_Graph() {
    //        cout << "Delete Dual_Graph" << endl;
  }
  /** @brief Member seeds pool variable */
  Seeds_Pool<long> _seeds_pool;
  /** @brief Flag to have more information for debugging */
  int _verbose;
  /** @brief Dimension of the problem*/
  short _dimension;
  /** @brief Member unordered set of compliant cells*/
  unordered_set<long> _s_anisotropic_compliant_cells;
  /** @brief Anisotropic lines list size*/
  int _lines_size = 0;
  /** @brief List of deaue containing the anisotropic lines*/
  vector<deque<long> *> _lines;

  /** @brief Computes the anisotropic lines at the first level (only called at
   * the first level of agglomeration)
   *  @param[in/out] nb_agglomeration_lines number of the agglomeration lines
   *  @return a forward list of pointers to the deque representing the lines
  */
  vector<deque<long> *> compute_anisotropic_line(long &nb_agglomeration_lines);
  /** @brief Computes the dictionqry of the anisotropic fine cells eligible for
   * the agglomeration lines
   *  @param[out] maxArray Array of the maximum weight: the biggest area of the
   * faces composing the given fine cell
   *  @param[out] d_anisotropic_fc dictionary (unordered_map) storing the
   * eligible anisotropic cells and the respective ratio between the max_weight
   * and the min_weight
   *  @param[out] d_isotropic_fc the same as the anisotropic but for the
   * isotropic cells
   *  @param[in] preserving if 0 does not hit only the BL prism to preserve the
   * boundary layer otherwise 2 for 2D or 3 for the 3D to preserve the BL only
   * in the anisotropic agglomeration*/
  void compute_d_anisotropic_fc(vector<double> &maxArray,
                                unordered_map<long, double> &d_anisotropic_fc,
                                unordered_map<long, double> &d_isotropic_fc,
                                const long preserving);

  /** @brief Compute the minimum compactness of fine cells inside a coarse cell.
  *   @param[in] s_fc set of fine cells to analyse
  *   @return the compactness of the fine cell
  */
  unsigned short int compute_min_fc_compactness_inside_a_cc(
      unordered_set<long> &s_fc);
  /** @brief Compute the dictionary of compactness of fine cells inside a coarse
  * cell.
  *   @param[in] s_fc set of fine cells to analyse
  *   @return the dictionary associating a fine cell in the coarse cell with its
  * compactness
  */
  unordered_map<long, unsigned short int> compute_fc_compactness_inside_a_cc(
      unordered_set<long> &s_fc);
  /** @brief Compute the dictionary of compactness of fine cells inside a coarse
  * cell.
  *   @param[in] s_seeds set of seeds for which the neighborhood must be
  * computed. Generally they are the fine cells composing the coarse cell for
  * which we are trying to compute the neighborhood.
  *   @param[in] nb_of_order_of_neighbourhood order of the neighborhood at which
  * we want to extend the dictionary
  *   @param[out] d_n_of_seed dictionary of the neighborhood given as an output.
  * The key of the associative structure is the index of the fine cell, the
  * value is the order of the distance.
  *   @param[in] max_card maximum cardinality
  *   @param[in] is_fc_agglomerated_tmp vector reporting the already
  * agglomerated cell, useful in the algorithm
  */

  void compute_neighbourhood_of_cc(const unordered_set<long> s_seeds,
                                   short &nb_of_order_of_neighbourhood,
                                   unordered_map<long, short> &d_n_of_seed,
                                   const short max_card,
                                   vector<bool> &is_fc_agglomerated_tmp);
};

#endif  // COMMA_PROJECT_DUAL_GRAPH_H
