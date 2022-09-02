//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_AGGLOMERATOR_H
#define COMMA_PROJECT_AGGLOMERATOR_H

#include "Dual_Graph.h"
#include "Coarse_Cell_Graph.h"
#include "Util.h"
#include "First_Order_Neighbourhood.h"

// forward definition to keep the agglomerators in one file

class Agglomerator_Anisotropic;
class Agglomerator_Isotropic;
class Agglomerator_Biconnected;

/** @brief A class responsible to do the interface between the different kinds
 * of
 *  agglomerator
 *  @author Alberto Remigi and Nicolas Lantos
 */
class Agglomerator {
 public:
  /** @brief The constructor of the interface
   *  @param[in] graph    *Dual Graph* object that determines the connectivity
   * of the matrix
   *  @param[in] dimension the dimension of the problem*/
  Agglomerator(Dual_Graph &graph, Coarse_Cell_Graph &cc_graph,
               int dimension = 3);
  /** @brief The destructor of the class */
  virtual ~Agglomerator() = default;
  /** @brief Accessor to retrive the fine cells to coarse cells from the coarse
   * cell graphs class
   */
  vector<long> get_fc_2_cc() { return _cc_graph->_fc_2_cc; }
  /** @brief Pure virtual function which implementation is specified in the
   * related child classes
     * and that defines the agglomeration of one level.
     * param[in] goal_card goal cardinality of the coarse cell
     * param[in] min_card minimum cardinality of the coarse cell
     * param[in] max_card maximum cardinality of the coarse cell
     * param[in] correction_steps it defines if corrections for the isotropic
   * algorithm are activated or not, for anisotropic algorithm not taken into
   * account.
     */
  virtual void agglomerate_one_level(const short goal_card,
                                     const short min_card, const short max_card,
                                     int correction_steps) = 0;

 protected:
  /** @brief dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3
   * -> 3D)*/
  int _dimension;
  /** @brief boolean to define if it is anisotropic or not. It is set as default
   * to false.
   *  @todo: check if we can get rid of _is_anisotropic variable*/
  bool _is_anisotropic = false;
  /** @brief minimum number of neighborhood we extend to search the neighborhood
   * in the greedy algorithm. Set as default to 3.*/
  short _min_neighbourhood = 3;
  /** @brief minimum cardinality. Set as default to -1 (meaning the maximum
   * possible).*/
  short _min_card = -1;
  /** @brief maximum cardinality. Set as default to -1 (meaning the maximum
   * possible)*/
  short _max_card = -1;
  /** @brief Goal cardinality. Set as default to -1 (meaning the maximum
   * possible)*/
  short _goal_card = -1;
  /** @brief Threshold cardinality. Set as default to -1 (meaning the maximum
   * possible)*/
  short _threshold_card = -1;
  /** @brief List of number of cells per coarse cell created.*/
  vector<long> _l_nb_of_cells;
  /** @brief Dual_Graph object detemining Fine cells graph and hence the
   * connectivity.*/
  Dual_Graph _fc_graph;
  /** @brief pointer to Coarse Cell Graph element */
  Coarse_Cell_Graph *_cc_graph;
};

/** @brief Agglomerator_Anisotropic class is a child class of the Agglomerator
 * class
 * that specializes the implementation to the case of Anisotropic
 * agglomeration.*/
class Agglomerator_Anisotropic : public Agglomerator {
  // Constructor
 public:
  /** @brief Constructor. The constructor takes as arguments the same arguments
   * of the father and
   * in this way activates also the constructor of the base class.*/
  Agglomerator_Anisotropic(Dual_Graph &graph, Coarse_Cell_Graph &cc_graph,
                           int dimension = 3);
  /** @brief Destructor*/
  ~Agglomerator_Anisotropic() {};

  /** @brief Specialization of the pure virtual function to the class
   * Agglomerator_Anisotropic.
     * We add the override key as a guard to possible mistakes:
     * https://stackoverflow.com/questions/46446652/is-there-any-point-in-using-override-when-overriding-a-pure-virtual-function*/
  void agglomerate_one_level(const short goal_card, const short min_card,
                             const short max_card,
                             int correction_steps) override;

  /** @brief Accessor (for this reason it is public)
   *  Function that returns the vector of agglomeration lines
   *  @param[in] level of the agglomeration process into the Multigrid algorithm
   *  @param[out] agglo_lines_array_idx Each element points to a particular
   * element in the
   *  vector agglo_lines_array. This is due to the storing structure.
   *  @param[out] agglo_lines_array Array storing all the element of the
   * anisotropic
   *  lines.
   *  */
  void get_agglo_lines(int level, vector<long> &agglo_lines_array_idx,
                       vector<long> &agglo_lines_array);
  /** @brief Vector of number of Anisotropic agglomeration lines per level*/
  vector<long> _v_nb_lines;
  /** @brief _v_lines : Agglomeration lines structure:
  * vector : level
  * forward list : identifier of the line
  * deque : line cells
  * e.g _v_lines[0] --> agglomeration lines at the finest level*/
  vector<vector<deque<long> *>> _v_lines;

 protected:
  /** @brief Function that for the current agglomerator, it creates the  */
  void create_all_anisotropic_cc_wrt_agglomeration_lines();
  /** @brief Vector of set of the anisotropic compliant of fine cells*/
  vector<unordered_set<long>> _v_of_s_anisotropic_compliant_fc;
};

/** @brief Agglomerator_Isotropic class is a child class of the Agglomerator
 * class
 * that specializes the implementation to the case of Isotropic agglomeration.*/
class Agglomerator_Isotropic : public Agglomerator {
 public:
  /** @brief Constructor. The constructor takes as arguments the same arguments
  * of the father and
  * in this way activates also the constructor of the base class.*/
  Agglomerator_Isotropic(Dual_Graph &graph, Coarse_Cell_Graph &cc_graph,
                         int dimension = 3);
  /** @brief Destructor*/
  ~Agglomerator_Isotropic() {};
  /** @brief The task of the function is to set the parameters of
   * dermine the cardinality limits with respect to the parameters passed
   * @param[in] goal_card goal cardinality of the coarse cell (set as default to
   * -1 indicating in our case
   * the maximum value)
   * @param[in] min_card minimum cardinality of the coarse cell(set as default
   * to -1 indicating in our case
   * the maximum value)
   * @param[in] max_card maximum cardinality of the coarse cell(set as default
   * to -1 indicating in our case
   * the maximum value)*/
  void set_agglomeration_parameter(short goal_card = -1, short min_card = -1,
                                   short max_card = -1);
  /** @brief Specialization of the pure virtual function to the class
   * Agglomerator_Isotropic.
      * We add the override key as a guard to possible mistakes:
      * https://stackoverflow.com/questions/46446652/is-there-any-point-in-using-override-when-overriding-a-pure-virtual-function
      * The function must be called later by the derived class
   * Agglomerator_Biconnected and Agglomerator Triconnected in order to
      * specialize the implementation of the choose optimal_cc. For further
   * information
      * about the structure, have a look at :
      * http://www.cplusplus.com/forum/general/31851/
      * The pseudo-code considers the while function and the agglomeration
   * process is not completed until all the cells are not
      * agglomerated. Hence:
      * - we choose a new seed
      * - we check with a specific algorithm the neighbouring cells to
   * agglomerate to the seed
      * - we create a new coarse cell (using the apposite method in cc_graph)
      * */
  void agglomerate_one_level(const short goal_card, const short min_card,
                             const short max_card,
                             int correction_steps) override;
  /** @brief Pure virtual function that must be implemented in child classes to
   * define the optimal coarse cell
   *  @param[in] seed the seed cell to start cumulate the other cells
   *  @param[in] compactness the compactness is defined as the minimum number of
   * neighbour of a fine cell inside
   *  a coarse cell
   */
  virtual unordered_set<long> choose_optimal_cc_and_update_seed_pool(
      const long seed, short &compactness) = 0;
};

/** @brief Child class of Agglomerator Isotropic where is implemented a specific
 * biconnected algorithm for the agglomeration. We call it biconnected case, but
 * it is the greedy algorithm in reality.
 */
class Agglomerator_Biconnected : public Agglomerator_Isotropic {
 public:
  /** @brief Constructor of the class. No specific implementation, it
   * instantiates the
   * base class Agglomerator_Isotropic */
  Agglomerator_Biconnected(Dual_Graph &graph, Coarse_Cell_Graph &cc_graph,
                           int dimension = 3);
  /** @brief Destructor*/
  ~Agglomerator_Biconnected() {};
  /** @brief Specialization of the pure virtual function in the parent class, to
   * be used in couple with the
   * Agglomerate_one_level of the Agglomerator_Isotropic */
  unordered_set<long> choose_optimal_cc_and_update_seed_pool(
      const long seed, short &compactness) override;

 protected:
  /** @brief Method that inside the biconnected algorith, checked in the
   * choose_optimal_cc_and_update_seed_pool
   * function all the possible exception, computes the best fine cells to add to
   * the coarse cell.
   */
  void compute_best_fc_to_add(
      Dual_Graph &graph, unordered_set<long> fon,
      const unordered_map<long, short> &dict_neighbours_of_seed,
      const bool &is_order_primary, const double &cc_surf, const double &vol_cc,
      const unordered_set<long> &s_of_fc_for_current_cc, long &argmin_ar,
      short &max_faces_in_common, double &min_ar_surf, double &min_ar_vol);
};

#endif  // COMMA_PROJECT_AGGLOMERATOR_H
