//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_COARSE_CELL_GRAPH_H
#define COMMA_PROJECT_COARSE_CELL_GRAPH_H

#include <memory>
#include <map>

#include "Dual_Graph.h"
#include "Coarse_Cell.h"
/** @brief Create a Coarse Cell Graph, hence it is the
 *  container of the Coarse Cells created and
 *  of the operation we can do on them.
 *  @param[in] Dual_Graph input element Dual_Graph to work on the seeds choice
 * and the seeds pool
 *  @param[in] verbose it defines the verbose parameters
*/
class Coarse_Cell_Graph {

 public:
  /** @brief Constructor*/
  Coarse_Cell_Graph(const Dual_Graph<long, double> &fc_graph, int verbose = 0);
  /** @brief Destructor*/
  ~Coarse_Cell_Graph();
  /** @brief Function that return if the coarse cell structure created is
   * unstructured or structured (in dependence
   * of the anisotropic  cells found)
   * @param[in] goal_card goal cardinality, useful to check if in case of non
   * presence of anisotropic cells we reached
   * the goal cardinality for all the coarse cells created */
  bool is_cc_grid_not_structured(short goal_card = -1);
  /** @brief Helper to get the member variable that defines the number of
   * agglomerated fine cells */
  inline long get_number_of_fc_agglomerated() {
    return (_nb_of_agglomerated_fc);
  }

  /** @brief Helper to get the number of coarse cells */
  inline long get_nb_of_cc() { return _cc_counter; }
  /** @brief map container of the CSR representation of the coarse cells */
  map<long, shared_ptr<Subgraph<long, double>>> _cc_vec;
  /** @brief Retrieve the indexes of the neighbouring coarse cells to a given
   * fine cell in a coarse cell (excluding the
   *  given coarse cell in which the fine cell is)
   *  @param[in] i_fc index of the fine cell inside the coarse cell to be
   * analysed
   *  @param[in] i_cc index of the coarse cell in which the fine cell is in
   *  @return vector of the index of the coarse cells
   */
  vector<long> get_neigh_cc(const long &i_fc, const long &i_cc);
  void update_fc_2_cc(const vector<long> &mapping);
  map<long, shared_ptr<Subgraph<long, double>>>::iterator remove_cc(
      map<long, shared_ptr<Subgraph<long, double>>>::iterator elim);
  void correct(const long &max_card);
  /** @brief It creates a coarse cell based on the set of fine cells given as an
   * input
   * @param[in] s_fc set of fine cells passed as a reference
   * @param[in] is_anisotropic boolean that tells if we are in an anisotropic
   * case or not
   * @param[in] is_creation_delayed based on the agglomerator instruction we
   * define if we delay or not the agglomeration
   * @return a long with the global identifier of the coarse cell.*/
  long cc_create_a_cc(const unordered_set<long> &s_fc,
                      bool is_anisotropic = false,
                      bool is_creation_delayed = false);
  /** @brief Vector of boolean for which the length is the number of fine cells
  * and for which the value of i_fc cell is true
  * when the cell is agglomerated in a coarse cell */
  vector<bool> _a_is_fc_agglomerated;

  unordered_map<long, unordered_set<long>> get_d_cc_all();

  unordered_map<long, unordered_set<long>> get_d_cc_iso();

  unordered_map<long, unordered_set<long>> get_d_cc_aniso();

  unordered_map<unsigned short int, long>
      get_d_distribution_of_cardinal_of_isotropic_cc();

  void cc_create_all_delayed_cc();

  inline bool is_anisotropic_cc(const long &i_cc) const {
    return (!_d_anisotropic_cc.empty()) && (_d_anisotropic_cc.count(i_cc));
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
    return _nb_of_agglomerated_fc == _fc_graph.get_nb_cells();
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
  /** @brief Dual graph representation.*/
  Dual_Graph<long, double> _fc_graph;
  /** @brief Number of coarse cells in the Coarse cell Graph*/
  long _cc_counter = 0;
  /** @brief Output vector of the fine cells to the coarse cell. The vector
   * long as much as the fine cells have as a value the corresponding coarse
   * cell id*/
  vector<long> _fc_2_cc;
  void print_d_distribution_of_cardinal_of_isotropic_cc();

 protected:
  /** @brief Variable where are recorded the isotropic coarse cells. Index is
   * the global index of the coarse
   * cell, key is the pointer to a Coarse_Cell element. */
  unordered_map<long, shared_ptr<Coarse_Cell>> _d_isotropic_cc;
  /** @brief Number of agglomerated fine cells */
  long _nb_of_agglomerated_fc = 0;
  /** @brief vector of the set of fine cells composing the too small coarse
   * cells that will be built at the end of the agglomeration
   * process */
  vector<unordered_set<long>> _delayed_cc;
  // because they will be the first to be deleted.
  /** @brief Variable where are recorded the anisotropic coarse cells. Index is
   * the global index of the coarse
   * cell, key is the pointer to a Coarse_Cell element. */
  unordered_map<long, unordered_set<long>> _d_anisotropic_cc;

  unordered_map<unsigned short, long>
      compute_d_distribution_of_cardinal_of_isotropic_cc();

  int _verbose;
};

#endif  // COMMA_PROJECT_COARSE_CELL_GRAPH_H
