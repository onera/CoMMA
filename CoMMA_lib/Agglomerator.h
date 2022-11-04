#ifndef COMMA_PROJECT_AGGLOMERATOR_H
#define COMMA_PROJECT_AGGLOMERATOR_H

/*
 *  * CoMMA
    * Copyright © 2022 ONERA
    *
    * Authors and Architects : Alberto Remigi and Nicolas Lantos
    *
    * Contributors : Riccardo Milani
    *
    * This program is free software: you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation, either version 3 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
    * You should have received a copy of the GNU General Public License
    * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cmath>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <iterator>

#include "Dual_Graph.h"
#include "Coarse_Cell_Container.h"
#include "Util.h"
#include "First_Order_Neighbourhood.h"

// forward definition to keep the agglomerators in one file

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Anisotropic;
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Isotropic;
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Biconnected;
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Pure_Front;

//"""
// Main Class of the CoMMA library, containing the methods to operate the
// agglomeration
// process. The constructor require the following parameters:
//:param Dual_Graph: the struct Dual Graph
//:param dimension: dimension of the problem
//"""

// How to pass parameters from base class
// https://stackoverflow.com/questions/9692675/c-constructor-where-parameters-are-used-by-base-class-constructor
// https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-base-class-constructor
// Error handling from
// https://weseetips.wordpress.com/tag/exception-from-constructor-initializer-list/

/** @brief A class responsible to do the interface between the different kinds
 * of agglomerator
 *  @author Alberto Remigi and Nicolas Lantos
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator {
 public:
  /** @brief Function which computes the aspect-ratio from a surface and a volume
   *  In 3D: \f$ AR = (surf_{CC})^{3/2} / vol_{CC} \f$
   *  In 2D: \f$ AR = surf_{CC}^2 / vol_{CC} \f$
   *  (Recall that in 2D the surface is the perimeter and the volume is the surface)
  */
  function<CoMMAWeightType(const CoMMAWeightType, const CoMMAWeightType)> _compute_AR;

  /** @brief The constructor of the interface
   *  @param[in] graph    *Dual Graph* object that determines the connectivity
   * of the matrix
   *  @param[in] dimension the dimension of the problem*/
  Agglomerator(Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &graph,
               Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType,
                                     CoMMAIntType> &cc_graph,
               CoMMAIntType dimension = 3)
      : _dimension(dimension), _fc_graph(graph), _cc_graph(&cc_graph) {
    if ((_dimension != 2) && (_dimension != 3)) {
      throw range_error("dimension can only be 2 or 3");
    }
    if (_dimension == 2) {
      _min_neighbourhood = 2;
      _compute_AR = [](const CoMMAWeightType perim, const CoMMAWeightType area)
                        -> CoMMAWeightType { return perim*perim / area; };
    } else {
      _min_neighbourhood = 3;
      _compute_AR = [](const CoMMAWeightType surf, const CoMMAWeightType vol)
                        -> CoMMAWeightType { return sqrt(surf*surf*surf) / vol; };
    }
    _l_nb_of_cells.push_back(graph._number_of_cells);
  }

  /** @brief The destructor of the class */
  virtual ~Agglomerator() = default;

  /** @brief Accessor to retrieve the fine cells to coarse cells from the coarse
   * cell graphs class
   */
  vector<CoMMAIndexType> get_fc_2_cc() { return _cc_graph->_fc_2_cc; }

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
  virtual void agglomerate_one_level(const CoMMAIntType goal_card,
                                     const CoMMAIntType min_card,
                                     const CoMMAIntType max_card,
                                     bool correction_steps) = 0;

 protected:
  /** @brief dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3
   * -> 3D)*/
  CoMMAIntType _dimension;
  /** @brief boolean to define if it is anisotropic or not. It is set as default
   * to false.
   *  @todo: check if we can get rid of _is_anisotropic variable*/
  bool _is_anisotropic = false;
  /** @brief minimum number of neighborhood we extend to search the neighborhood
   * in the greedy algorithm. Set as default to 3.*/
  CoMMAIntType _min_neighbourhood = 3;
  /** @brief minimum cardinality. Set as default to -1 (meaning the maximum
   * possible).*/
  CoMMAIntType _min_card = -1;
  /** @brief maximum cardinality. Set as default to -1 (meaning the maximum
   * possible)*/
  CoMMAIntType _max_card = -1;
  /** @brief Goal cardinality. Set as default to -1 (meaning the maximum
   * possible)*/
  CoMMAIntType _goal_card = -1;
  /** @brief Threshold cardinality. Set as default to -1 (meaning the maximum
   * possible)*/
  CoMMAIntType _threshold_card = -1;
  /** @brief List of number of cells per coarse cell created.*/
  vector<CoMMAIndexType> _l_nb_of_cells;
  /** @brief Dual_Graph object determining Fine cells graph and hence the
   * connectivity.*/
  Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> _fc_graph;
  /** @brief pointer to Coarse Cell Graph element */
  Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> *
      _cc_graph;
};

/** @brief Agglomerator_Anisotropic class is a child class of the Agglomerator
 * class that specializes the implementation to the case of Anisotropic
 * agglomeration.*/
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Anisotropic
    : public Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
  // Constructor
 public:
  /** @brief Constructor. The constructor takes as arguments the same arguments
   * of the father and
   * in this way activates also the constructor of the base class.*/
  Agglomerator_Anisotropic(
      Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &graph,
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &
          cc_graph,
      const CoMMAWeightType threshold_anisotropy, CoMMAIntType dimension = 3)
      : Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            graph, cc_graph, dimension) {
    // for every defined level (1 by default), contains the number of cells
    // e.g. _l_nb_of_cells[0]= number of cells on finest level
    //      _l_nb_of_cells[1]= number of cells on the first coarse level
    this->_l_nb_of_cells.push_back(graph._number_of_cells);
    // For every level, we have a set containing the admissible cells for
    // anisotropy cell number:
    // For level 0, it is the cell number of prism or hexahedron ...
    this->_v_of_s_anisotropic_compliant_fc =
        vector<unordered_set<CoMMAIndexType>>(2);
    this->_v_of_s_anisotropic_compliant_fc[0] =
        this->_fc_graph._s_anisotropic_compliant_cells;
    this->_v_nb_lines = vector<CoMMAIndexType>(2);
    this->_v_lines = vector<vector<deque<CoMMAIndexType> *>>(2);

    assert(threshold_anisotropy > 0);
    this->_threshold_anisotropy =
        (threshold_anisotropy < 1)
            ? static_cast<CoMMAWeightType>(1. / threshold_anisotropy)
            : threshold_anisotropy;
  }

  /** @brief Destructor*/
  ~Agglomerator_Anisotropic() {};

  /** @brief Specialization of the pure virtual function to the class
   * Agglomerator_Anisotropic.
     * We add the override key as a guard to possible mistakes:
     * https://stackoverflow.com/questions/46446652/is-there-any-point-in-using-override-when-overriding-a-pure-virtual-function*/
  void agglomerate_one_level(const CoMMAIntType goal_card,
                             const CoMMAIntType min_card,
                             const CoMMAIntType max_card,
                             bool correction_steps) override {
    // Unused parameters
    (void)goal_card;
    (void)min_card;
    (void)max_card;
    (void)correction_steps;
    // if the finest agglomeration line is not computed, hence compute it
    // (REMEMBER! We compute the agglomeration lines
    // only on the finest level, the other one are stored only for visualization
    // purpose
    if (this->_v_lines[0].empty()) {
      // The anisotropic lines are only computed on the original (finest) mesh.
      CoMMAIndexType nb_agglomeration_lines(0);
      this->_v_lines[0] = this->_fc_graph.compute_anisotropic_line(
          this->_threshold_anisotropy,
          nb_agglomeration_lines);  // finest level!!!
      this->_v_nb_lines[0] = nb_agglomeration_lines;
    }
    // In case the if is not realized, this is not the first generation of a
    // coarse level.
    // The anisotropic lines are given as input.
    // Copy of the current agglomeration_lines as a backup for visualization
    // purpose.
    create_all_anisotropic_cc_wrt_agglomeration_lines();
  }

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
  /** @todo maybe delete the aggl_lines_sizes here. Not so sure that is useful.
   * Maybe only for statistics.  */
  void get_agglo_lines(CoMMAIntType level,
                       vector<CoMMAIndexType> &agglo_lines_array_idx,
                       vector<CoMMAIndexType> &agglo_lines_array) {
    // If at the level of agglomeration "level" the vector containing the number of
    // lines is empty, hence it means no line has been found at the current
    // level.
    // variable cumulating the number of fine cells in the agglomeration lines
    // of the current level
    CoMMAIndexType number_of_fc_in_agglomeration_lines = 0;
    agglo_lines_array_idx.clear();
    agglo_lines_array.clear();
    agglo_lines_array_idx.push_back(0);
    // We cycle over the line (in _v_lines)
    for (auto &line : (this->_v_lines[level])) {
      CoMMAIndexType size_of_line = (*line).size();
      // This vector store the end of a line and the start of a new anisotropic
      // line
      // WARNING! We are storing the anisotropic lines in a vector so we need a
      // way to point to the end of a line and the starting of a new one.
      agglo_lines_array_idx.push_back(size_of_line +
                                      number_of_fc_in_agglomeration_lines);
      // Here we store the index of the cell.
      for (CoMMAIndexType i = 0; i < size_of_line; i++) {
        agglo_lines_array.push_back((*line)[i]);
      }
      number_of_fc_in_agglomeration_lines += size_of_line;
    }
  }

  /** @brief Vector of number of Anisotropic agglomeration lines per level*/
  vector<CoMMAIndexType> _v_nb_lines;

  /** @brief _v_lines : Agglomeration lines structure:
  * vector : level
  * forward list : identifier of the line
  * deque : line cells
  * e.g _v_lines[0] --> agglomeration lines at the finest level*/
  vector<vector<deque<CoMMAIndexType> *>> _v_lines;

 protected:
  /** @brief Function that for the current agglomerator, it creates the coarse
   * cells following the anisotropic lines */
  void create_all_anisotropic_cc_wrt_agglomeration_lines() {
    // list of set of hexaedric or prismatic cell number (for level 0)
    this->_v_of_s_anisotropic_compliant_fc[1] = {};

    // Setting up lambda function that will perform the loop. This is needed since we
    // want to loop forwards or backwards according to some runtime value
    // See, e.g., https://stackoverflow.com/a/56133699/12152457
    auto loop_line = [&](auto begin, auto end) {
      deque<CoMMAIndexType> *line_lvl_p_one = new deque<CoMMAIndexType>();
      // TODO here is necessary for the cc_create_a_cc but maybe we need in
      // some way to change that.
      const bool is_anisotropic = true;
      for (auto deqIt = begin; deqIt != end; deqIt += 2) {
        // we agglomerate cells along the agglomeration line, hence we have to
        // go through the faces and agglomerate two faces together, getting to
        // cardinal 2
        // ANISOTROPIC: we agglomerate 2 by 2, hence the anisotropic
        // agglomeration will provoke agglomerated coarse cells of cardinality
        // 2.
        // Here we have to consider the two different case in which we have an
        // odd number of cells.
        // THIS IS FUNDAMENTAL FOR THE CONVERGENCE OF THE MULTIGRID ALGORITHM
        unordered_set<CoMMAIndexType> s_fc = {*deqIt, *(deqIt + 1)};
        if (distance(deqIt, end) == 3) {
          // If only three cells left, agglomerate them
          s_fc.insert(*(deqIt + 2));
          deqIt++;
        }
        // We create the coarse cell
        const CoMMAIndexType i_cc =
          (*(this->_cc_graph)).cc_create_a_cc(s_fc, is_anisotropic);
        line_lvl_p_one->push_back(i_cc);
        this->_v_of_s_anisotropic_compliant_fc[1].insert(i_cc);
      }

      this->_v_lines[1].push_back(line_lvl_p_one);

    }; // End lambda def

    // Process of every agglomeration lines:
    for (auto fLIt = this->_v_lines[0].begin(); fLIt != this->_v_lines[0].end();
         fLIt++) {
      // We iterate on the anisotropic lines of a particular level (the level 1,
      // where they were copied from level 0.
      // We create a pointer to an empty deque for the line + 1, and hence for
      // the next level of agglomeration
      // We check the line size for the pointed line by the iterator
      //     CoMMAIndexType line_size = (**fLIt).size();
      auto actual_deque = **fLIt;
      if (actual_deque.size() <= 1) {
        // the agglomeration_line is empty and hence the iterator points again
        // to the empty deque, updating what is pointed by it and hence __v_lines[1]
        // (each time we iterate on the line, a new deque line_lvl_p_one is defined)
        continue;
      }
      // We start agglomerating from the head or the tail of the line according to
      // which of the two has more boundary faces
      const bool forward_line =
        (this->_fc_graph._seeds_pool).get_n_boundary_faces(actual_deque.front()) >=
          (this->_fc_graph._seeds_pool).get_n_boundary_faces(actual_deque.back());

      if (forward_line)
        loop_line(actual_deque.begin(), actual_deque.end());
      else
        loop_line(actual_deque.rbegin(), actual_deque.rend());

    }
  }

  /** @brief Vector of set of the anisotropic compliant of fine cells*/
  vector<unordered_set<CoMMAIndexType>> _v_of_s_anisotropic_compliant_fc;

  /** @brief Value of the aspect ration above which a cell is considered
   * anisotropic */
  CoMMAWeightType _threshold_anisotropy;
};

/** @brief Agglomerator_Isotropic class is a child class of the Agglomerator
 * class that specializes the implementation to the case of Isotropic
 * agglomeration.*/
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Isotropic
    : public Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
 public:
  /** @brief Constructor. The constructor takes as arguments the same arguments
  * of the father and
  * in this way activates also the constructor of the base class.*/
  Agglomerator_Isotropic(
      Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &graph,
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &
          cc_graph,
      CoMMAIntType dimension = 3)
      : Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            graph, cc_graph, dimension) {
    // no particular constructor
  }

  /** @brief Destructor*/
  ~Agglomerator_Isotropic() {};

  /** @brief The task of the function is to set the parameters of
   * determine the cardinality limits with respect to the parameters passed
   * @param[in] goal_card goal cardinality of the coarse cell (set as default to
   * -1 indicating in our case
   * the maximum value)
   * @param[in] min_card minimum cardinality of the coarse cell(set as default
   * to -1 indicating in our case
   * the maximum value)
   * @param[in] max_card maximum cardinality of the coarse cell(set as default
   * to -1 indicating in our case
   * the maximum value)*/
  void set_agglomeration_parameter(CoMMAIntType goal_card = -1,
                                   CoMMAIntType min_card = -1,
                                   CoMMAIntType max_card = -1) {
    unordered_map<CoMMAIntType, CoMMAIntType> d_default_min_card = {{2, 3},
                                                                    {3, 6}};
    unordered_map<CoMMAIntType, CoMMAIntType> d_default_max_card = {{2, 5},
                                                                    {3, 10}};
    unordered_map<CoMMAIntType, CoMMAIntType> d_default_goal_card = {{2, 4},
                                                                     {3, 8}};
    unordered_map<CoMMAIntType, CoMMAIntType> d_default_threshold_card = {
        {2, 2}, {3, 3}};

    // Definition of _min_card
    if (min_card == -1) {
      this->_min_card = d_default_min_card[this->_dimension];
    } else {
      this->_min_card = min_card;
    }

    // Definition of _max_card
    if (max_card == -1) {
      this->_max_card = d_default_max_card[this->_dimension];
    } else {
      this->_max_card = max_card;
    }
    // Definition of _goal_card
    if (goal_card == -1) {
      this->_goal_card = d_default_goal_card[this->_dimension];
    } else {
      this->_goal_card = goal_card;
    }

    // Definition of _threshold_card
    this->_threshold_card = d_default_threshold_card[this->_dimension];
  }

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
   */
  void agglomerate_one_level(const CoMMAIntType goal_card,
                             const CoMMAIntType min_card,
                             const CoMMAIntType max_card,
                             bool correction_steps) override {
    set_agglomeration_parameter(goal_card, min_card, max_card);
    // We define a while for which we control the number of agglomerated cells
    CoMMAIntType compactness = 0;
    CoMMAIndexType nb_of_fc = this->_l_nb_of_cells[0];
    while (this->_cc_graph->get_number_of_fc_agglomerated() < nb_of_fc) {
      // 1) Choose a new seed
      CoMMAIndexType seed = (this->_fc_graph._seeds_pool).choose_new_seed(
          this->_cc_graph->_a_is_fc_agglomerated);
      // 2) Choose the set of Coarse Cells with the specification of the
      // algorithm
      // in the children class (triconnected or biconnected)
      unordered_set<CoMMAIndexType> set_current_cc =
          choose_optimal_cc_and_update_seed_pool(seed, compactness);
      // 3)  Creation of cc:
      bool is_anistropic =
          false;  // needed the create_cc of the coarse cell class
      // Check if delay the agglomeration based on compactness written during
      // the optimal cc choice process. Remember the compactness is the minimum
      // degree in
      // the coarse cell.
      //    	 bool is_creation_delayed = compactness < _dimension;
      bool is_creation_delayed = false;
      this->_cc_graph->cc_create_a_cc(set_current_cc, is_anistropic,
                                      is_creation_delayed);
    }
    // When we exit from this process all the cell are agglomerated, apart the
    // delayed one
    // We proceed in creating the delayed one
    this->_cc_graph->cc_create_all_delayed_cc();
    if (correction_steps) {
      this->_cc_graph->correct(this->_max_card);
    }
    this->_l_nb_of_cells.push_back(this->_cc_graph->_cc_counter);
  }

  /** @brief Approximate the value of a boundary face using the known internal
   * faces. It uses a (geometric) average, so the result is correct only if the cell
   * is a regular polygon
   * @param int_face Vector of the surfaces of the internal faces
   * @return An approximation of the surface of a boundary face
   */
  inline CoMMAWeightType estimate_boundary_face(
      const vector<CoMMAWeightType> &int_faces) {
    // Approximate with an average of the internal faces
    // We could choose many kinds of average, e.g. arithmetic or geometric, I
    // honestly don't know if one is better then the other...
    // Here, we use the geometric one, which should be less sensitive to outliers
    return pow(accumulate(int_faces.begin(), int_faces.end(),
                              CoMMAWeightType{1.}, multiplies<>()),
               CoMMAWeightType{1.} / int_faces.size());
  }

  /** @brief Computes features of the CC obtained by adding a given fine cell. The
   * features are Aspect-Ratio and number of face shared with other cells already
   * agglomerated (Current coarse cell means without i_fc)
   *  @param[in] i_fc Index of the fine cell to add to the coarse cell
   *  @param[in] cc_surf Surface of the current coarse cell
   *  @param[in] cc_vol Volume of the current coarse cell
   *  @param[in] fc_of_cc Index of the fine cells already agglomerated in the coarse
   *  cell
   *  @param[out] shared_faces Number of faces shared by the fine cell with the
   *  current coarse cell
   *  @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   */
  inline void compute_next_cc_features(const CoMMAIndexType i_fc,
    const CoMMAWeightType cc_surf, const CoMMAWeightType cc_vol,
    const unordered_set<CoMMAIndexType> &fc_of_cc,
    // out
    CoMMAIntType &shared_faces, CoMMAWeightType &aspect_ratio,
    CoMMAWeightType &new_surf, CoMMAWeightType &new_vol) {

    shared_faces = 0;
    new_surf = cc_surf;
    vector<CoMMAIndexType> v_neighbours = this->_fc_graph.get_neighbours(i_fc);
    vector<CoMMAWeightType> v_weights = this->_fc_graph.get_weights(i_fc);
    assert(v_neighbours.size() == v_weights.size());

    for (auto i_n = decltype(v_neighbours.size()){0}; i_n < v_neighbours.size(); i_n++) {

      const CoMMAIndexType i_fc_n = v_neighbours[i_n];
      const CoMMAWeightType i_w_fc_n = v_weights[i_n];

      if (i_fc_n == i_fc) {
        // This can never happen!!! -> See below
        // Boundary surface
        new_surf += i_w_fc_n;
      } else if (fc_of_cc.count(i_fc_n) == 0) {
        // i_fc_n is not yet in CC -> ext surface
        new_surf += i_w_fc_n;
      } else {
        new_surf -= i_w_fc_n;
        shared_faces++;
      }
    }

    const CoMMAIntType n_bdry_f =
      (this->_fc_graph._seeds_pool).get_n_boundary_faces(i_fc);
    if (n_bdry_f > 0) {
      new_surf += n_bdry_f * this->estimate_boundary_face(v_weights);
    }

    // Return parameters
    new_vol = cc_vol + this->_fc_graph._volumes[i_fc];
    // AR is the non-dimensional ratio between the surface and the volume
    aspect_ratio = this->_compute_AR(new_surf, new_vol);

  }

  /** @brief Pure virtual function that must be implemented in child classes to
   * define the optimal coarse cell
   *  @param[in] seed the seed cell to start cumulate the other cells
   *  @param[in] compactness the compactness is defined as the minimum number of
   * neighbour of a fine cell inside
   *  a coarse cell
   */
  virtual unordered_set<CoMMAIndexType> choose_optimal_cc_and_update_seed_pool(
      const CoMMAIndexType seed, CoMMAIntType &compactness) = 0;
};

/** @brief Child class of Agglomerator Isotropic where is implemented a specific
 * biconnected algorithm for the agglomeration. We call it biconnected case, but
 * it is the greedy algorithm in reality.
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Biconnected
    : public Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType,
                                    CoMMAIntType> {
 public:
  /** @brief Constructor of the class. No specific implementation, it
   * instantiates the
   * base class Agglomerator_Isotropic */
  Agglomerator_Biconnected(
      Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &graph,
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &
          cc_graph,
      CoMMAIntType dimension = 3)
      : Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            graph, cc_graph, dimension) {
    // no particular constructor
  }

  /** @brief Destructor*/
  ~Agglomerator_Biconnected() {};

  /** @brief Specialization of the pure virtual function in the parent class, to
   * be used in couple with the
   * Agglomerate_one_level of the Agglomerator_Isotropic */
  unordered_set<CoMMAIndexType> choose_optimal_cc_and_update_seed_pool(
      const CoMMAIndexType seed, CoMMAIntType &compactness) override {
    bool is_order_primary = false;
    // The goal of this function is to choose from a pool of neighbour the
    // better one to build a compact coarse cell
    assert(this->_goal_card != -1);  // _goal_card has been initialized
    // OUTPUT: Definition of the current cc, IT WILL BE GIVEN AS AN OUTPUT
    unordered_set<CoMMAIndexType> s_current_cc = {seed};
    // Dictionary of the neighborhood of the seed, the key is the global index
    // of the cell and the value the order of distance from the seed (1 order
    // direct neighborhood, 2 order etc.)
    unordered_map<CoMMAIndexType, CoMMAIntType> d_n_of_seed;
    // Number of fine cells constituting the current coarse cell in
    // construction.
    CoMMAIntType size_current_cc = 1;  // CC contains only one cell: the seed
    // set to 3 as default we set to this value the maximum order to which we
    // search to compose the coarse cell
    CoMMAIntType max_order_of_neighbourhood = this->_min_neighbourhood;

    // We fill the d_n_of_seeds considering the initial seed passed
    this->_fc_graph.compute_neighbourhood_of_cc(
        s_current_cc, max_order_of_neighbourhood,  // in and out
        d_n_of_seed,  // output, the function fills the dictionary
        this->_max_card,
        // anisotropic cells agglomerated (not to take into account in the
        // process)
        this->_cc_graph->_a_is_fc_agglomerated);

    // We get the number of neighborhoods
    CoMMAIntType nb_neighbours = this->_fc_graph.get_nb_of_neighbours(seed);
    // return the area of the face connected to the seed
    vector<CoMMAWeightType> neighbours_weights =
        this->_fc_graph.get_weights(seed);

    // If no neighbour is found for seed: this case happened only when isotropic
    // cell is surrounded by anisotropic cells.
    if (d_n_of_seed.empty()) {
      // d_n_of_seed is empty, i.e: the cc is not complete and no neighbour are
      // available!
      compactness = 0;
    }
    else if (static_cast<CoMMAIntType>(d_n_of_seed.size() + s_current_cc.size()) < this->_goal_card) {
      // Not enough available neighbour, the dictionary of the available cells
      // size summed with the current cell size is not enough to reach the goal
      // cardinality: creation of a (too small) coarse cell.
      // We add ALL the cells of the dictionary to the set of current coarse cell.
      for (auto &i_k_v : d_n_of_seed) {
        s_current_cc.insert(i_k_v.first);
      }
      // We check as a consequence the threshold cardinality that is a minimum
      // limit
      bool is_creation_delayed = (static_cast<CoMMAIntType>(s_current_cc.size()) <= this->_threshold_card);
      if (is_creation_delayed) {
        compactness = 0;  // Return
      } else {
        // minimum number of neighborhood of a connected cell
        compactness = this->_dimension;
        // TODO: CHECK THAT, it is not better to be substituted with number of
        // neighborhood?
      }
    }
    else {
      // If we passed the two previous checks, the minimum size is the goal
      // cardinality required
      // TODO: CHECK THAT, if the goal is 2, the minimum size would be 3?
      // ARGUABLE! Let's think to 3
      CoMMAIntType min_size = this->_goal_card;
      // Computation of the initial aspect ratio: we need cc_surf: i.e. the
      // external area (perimeter in 2D and sum of external faces in 3D) and
      // volume
      CoMMAWeightType cc_surf = accumulate(neighbours_weights.begin(),
          neighbours_weights.begin(), CoMMAWeightType{0});
      const CoMMAIntType n_bdry_f =
        (this->_fc_graph._seeds_pool).get_n_boundary_faces(seed);
      if (n_bdry_f > 0) {
        cc_surf += n_bdry_f * this->estimate_boundary_face(neighbours_weights);
      }
      // volume of cc is at first the volume of the seed.
      CoMMAWeightType vol_cc = this->_fc_graph._volumes[seed];
      // This dictionary is used to store the eligible cc: i.e. its size is
      // inside the permitted range. This is useful to track back our step if
      // needed. [size of the current, [cell set, d_n_of seed]]
      unordered_map<CoMMAIntType,
                    pair<unordered_set<CoMMAIndexType>,
                         unordered_map<CoMMAIndexType, CoMMAIntType>>>
          dict_cc_in_creation;
      CoMMAIntType min_external_faces = numeric_limits<CoMMAIntType>::max();
      CoMMAIntType arg_min_external_faces = min_size;
      // Here we define the exact dimension of the coarse cell as the min
      // between the max cardinality given as an input (remember the constructor
      // choice in case of -1) and the dictionary of the boundary cells, it
      // means the total number of neighborhood cells until the order we have
      // given (as default 3, so until the third order)
      CoMMAIntType max_ind =
          min(this->_max_card, static_cast<CoMMAIntType>(d_n_of_seed.size() + 1));
      // We add the faces that are on boundary calling the method of seed pool.
      CoMMAIntType number_of_external_faces_current_cc =
          nb_neighbours + this->_fc_graph._seeds_pool.boundary_value(seed) - 1;
      // d_keys_to_set from Util.h, it takes the keys of the unordered map and
      // create an unordered set. The unordered set is representing hence all
      // the neighbors of seed until a given order.
      unordered_set<CoMMAIndexType> s_neighbours_of_seed =
          d_keys_to_set<CoMMAIndexType, CoMMAIntType>(d_n_of_seed);
      // Build the class first order neighborhood
      First_Order_Neighbourhood<CoMMAIndexType, CoMMAIntType> f_o_neighbourhood =
          First_Order_Neighbourhood<CoMMAIndexType, CoMMAIntType>(
              s_neighbours_of_seed, this->_dimension,false);
      // Generate the set of the first order neighborhood to the given seed
      unordered_set<CoMMAIndexType> fon =
        f_o_neighbourhood.update(seed, this->_fc_graph.get_neighbours(seed));

      // Choice of the fine cells to agglomerate we enter in a while, we store
      // anyways all the possible coarse cells (not only the max dimension one)
      while (size_current_cc < max_ind) {
        // argmin_ar is the best fine cell to add
        CoMMAIndexType argmin_ar = -1;
        CoMMAWeightType min_ar_surf = numeric_limits<CoMMAWeightType>::max();
        CoMMAWeightType min_ar_vol = numeric_limits<CoMMAWeightType>::max();
        CoMMAIntType max_faces_in_common = 0;
        // We compute the best fine cell to add, based on the aspect
        // ratio and is given back in argmin_ar. It takes account also
        // the fine cells that has been added until now.
        compute_best_fc_to_add(fon, d_n_of_seed, is_order_primary, cc_surf,
                               vol_cc, s_current_cc, argmin_ar,  // output
                               max_faces_in_common, min_ar_surf, min_ar_vol);

        number_of_external_faces_current_cc +=
            this->_fc_graph.get_nb_of_neighbours(argmin_ar) +
            this->_fc_graph._seeds_pool.boundary_value(argmin_ar) - 1 -
            2 * max_faces_in_common;
        // we increase the cc
        size_current_cc++;
        s_current_cc.insert(argmin_ar);

        // if the constructed cc is eligible, i.e. its size is inside the
        // permitted range we store it inside dict_cc_in_creation This choice is
        // based on the idea that the smallest cc (w.r.t. card) is may be not
        // the one that minimized the number of external faces. If this if is
        // satisfied it means we have an eligible cell
        if ((min_size <= size_current_cc) || size_current_cc == max_ind) {

          if (number_of_external_faces_current_cc <= min_external_faces) {

            min_external_faces = number_of_external_faces_current_cc;
            arg_min_external_faces = size_current_cc;
          }

          // We update the dictionary of eligible coarse cells
          unordered_map<CoMMAIndexType, CoMMAIntType> new_dict;
          new_dict[argmin_ar] = d_n_of_seed[argmin_ar];
          pair<unordered_set<CoMMAIndexType>,
               unordered_map<CoMMAIndexType, CoMMAIntType>> p =
              make_pair(s_current_cc, new_dict);
          dict_cc_in_creation[size_current_cc] = p;
        }

        // Update of cc_surf and vol_cc with the new fc added
        cc_surf = min_ar_surf;
        vol_cc = min_ar_vol;

        // Remove added fc from the available neighbours
        d_n_of_seed.erase(argmin_ar);

        fon = f_o_neighbourhood.update(argmin_ar,
            this->_fc_graph.get_neighbours(argmin_ar));
      }

      s_current_cc = dict_cc_in_creation[arg_min_external_faces].first;

      // If we do not chose the biggest cc, we put the useless fc back to the
      // pool
      for (auto i_s = arg_min_external_faces + 1; i_s < max_ind + 1; i_s++) {
        // for all size of Cell from arg_min_external_faces+1 to  min(max_card,
        // len(d_n_of_seed) + 1) + 1
        // d_n_of_seed.
        for (auto iKV : dict_cc_in_creation[i_s].second) {
          d_n_of_seed[iKV.first] = iKV.second;
        }
      }
      assert(arg_min_external_faces == static_cast<CoMMAIntType>(s_current_cc.size()));
      // Computes the actual compactness of the coarse cell
      compactness =
          this->_fc_graph.compute_min_fc_compactness_inside_a_cc(s_current_cc);
    }  // end else
    // Update seeds
    // Create of l_of_new_seed:
    list<CoMMAIndexType> l_of_new_seed;
    if (!d_n_of_seed.empty()) {

      // CoMMAIntType size = d_n_of_seed.size();
      // l_of_new_seed.resize(size);
      // if d_n_of_seed is not empty
      // Reminder: d_n_of_seed is here the pool of cell neighbouring the
      // previous seed!
      for (auto &i_k_v : d_n_of_seed) {
        if (i_k_v.second <= 2) {
          l_of_new_seed.push_back(i_k_v.first);
        }
      }
      // if list of new seeds is still empty we go to order 3
      CoMMAIntType i_k = 3;
      while (l_of_new_seed.empty()) {
        // We put FC in the l_of_new_seed according to its order of neighbouring
        // of previous seed.
        // order 1 or 2 then 3, then 4 and so on.
        for (auto &i_k_v : d_n_of_seed) {
          if (i_k_v.second <= i_k) {
            l_of_new_seed.push_back(i_k_v.first);
          }
        }
        i_k++;
      }
    } else if (this->_fc_graph._seeds_pool.is_empty()) {
      // else d_n_of_seed is empty: we used every neighbour!
      // if list_of_seeds is empty, we look for new seeds to add to the
      // list_of_seeds.
      // if list_of_seeds is empty we look if there is some neighbour to the
      // current fc:
      // s_fc_neighbours_of_cc = set()
      // we remove seed because we already update its first neighbours.
      // A copy needed because the set is used inside ccg
      unordered_set<CoMMAIndexType> tmp_set(s_current_cc);
      tmp_set.erase(seed);

      // We add to s_fc_neighbours_of_cc all the neighbours of FC included in
      // s_fc_for_current_cc without seed
      for (auto &i_fc : tmp_set) {
        vector<CoMMAIndexType> a_neighbours =
            this->_fc_graph.get_neighbours(i_fc);
        for (const CoMMAIndexType &i_fc_n : a_neighbours) {
          if ((*(this->_cc_graph)).is_fc_not_already_agglomerated(i_fc_n)) {
            // _a_is_fc_agglomerated is up-to-date.
            // remark: we don't care i_fc_n == i_fc.
            l_of_new_seed.push_back(i_fc_n);
          }
        }
      }
    }
    // Update of list_of_seeds:
    (this->_fc_graph._seeds_pool).update(l_of_new_seed);
    return s_current_cc;
  }

 protected:
  /** @brief Method that inside the biconnected algorithm, checked in the
   * choose_optimal_cc_and_update_seed_pool function all the possible exception,
   * computes the best fine cells to add to the coarse cell.
   */
  void compute_best_fc_to_add(
      unordered_set<CoMMAIndexType> fon,
      const unordered_map<CoMMAIndexType, CoMMAIntType> &d_n_of_seed,
      const bool &is_order_primary, const CoMMAWeightType &cc_surf,
      const CoMMAWeightType &vol_cc,
      const unordered_set<CoMMAIndexType> &s_of_fc_for_current_cc,
      CoMMAIndexType &argmin_ar, CoMMAIntType &max_faces_in_common,
      CoMMAWeightType &min_ar_surf, CoMMAWeightType &min_ar_vol) {
    //  this function defines the best fine cells to add to create the coarse
    // cell for the current coarse cell considered
    CoMMAWeightType min_ar = numeric_limits<CoMMAWeightType>::max();
    CoMMAIndexType arg_max_faces_in_common = -1;

    // For every fc in the neighbourhood:
    // we update the new aspect ratio
    // we verify that fon is a sub member of the dict of seeds
    for (const CoMMAIndexType &i_fc : fon) {
      // we test every possible new cells to chose the one that locally
      // minimizes the Aspect Ratio at the first fine cell of the fon.
      if (arg_max_faces_in_common == -1) {
        arg_max_faces_in_common = i_fc;
      }

      // Compute features of the CC obtained by adding i_fc
      CoMMAIntType number_faces_in_common = 0;
      CoMMAWeightT new_ar = numeric_limits<CoMMAWeightType>::min();
      CoMMAWeightT new_ar_surf = numeric_limits<CoMMAWeightType>::min();
      CoMMAWeightT new_ar_vol = numeric_limits<CoMMAWeightType>::min();
      this->compute_next_cc_features(i_fc, cc_surf,
          vol_cc,
          s_of_fc_for_current_cc,
          // out
          number_faces_in_common, new_ar, new_ar_surf, new_ar_vol);

      // Neighborhood order of i_fc wrt to original seed of CC
      // [i_fc] is not const the method at returns the reference to the value of the
      // key i_fc.
      const CoMMAIntType &order = d_n_of_seed.at(i_fc);

      // TODO This version seems good but refactorisation to do: perhaps it is
      // not needed to update every new possible coarse cell aspect ratio?
      // TODO also need to remove the list of min_ar, argmin_ar, etc.
      if (number_faces_in_common >=
          max_faces_in_common or
              is_order_primary) {  // if is_order_primary is True the order of
                                   // neighbourhood is primary
        if (number_faces_in_common == max_faces_in_common or is_order_primary) {
          // If the number of faces in common is the same, let's see whether it's
          // worth to update or not

          if (order <= d_n_of_seed.at(arg_max_faces_in_common)) {
            // [arg_max_faces_in_common] is not const.
            if (order == d_n_of_seed.at(arg_max_faces_in_common)) {
              if (new_ar < min_ar and number_faces_in_common > 0 ) {
                // The second condition asserts the connectivity of the coarse
                // element.
                min_ar = new_ar;
                argmin_ar = i_fc;
                min_ar_surf = new_ar_surf;
                min_ar_vol = new_ar_vol;

                arg_max_faces_in_common = i_fc;
                // The number of face in common is the same no need to touch it
              }
            } else {
              // Case :number_faces_in_common == max_faces_in_common and order <
              // dict_neighbours_of_seed[arg_max_faces_in_common]:
              arg_max_faces_in_common = i_fc;
              min_ar = new_ar;
              argmin_ar = i_fc;
              min_ar_surf = new_ar_surf;
              min_ar_vol = new_ar_vol;
              // The number of face in common is the same no need to touch it
            }
          }
        } else {
          // Case number_faces_in_common > max_faces_in_common:
          // -> Just update and see what comes next
          max_faces_in_common = number_faces_in_common;
          arg_max_faces_in_common = i_fc;
          min_ar = new_ar;
          argmin_ar = i_fc;
          min_ar_surf = new_ar_surf;
          min_ar_vol = new_ar_vol;
        }
      }
    }
  }
};
/** @brief Child class of Agglomerator Isotropic where is implemented a specific
 * biconnected algorithm for the agglomeration. We call it biconnected case, but
 * it is the greedy algorithm in reality.
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Pure_Front
    : public Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType,
                                    CoMMAIntType> {
 public:
  /** @brief Constructor of the class. No specific implementation, it
   * instantiates the
   * base class Agglomerator_Isotropic */
  Agglomerator_Pure_Front(
      Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &graph,
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &
          cc_graph,
      CoMMAIntType dimension = 3)
      : Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            graph, cc_graph, dimension) {
    // no particular constructor
  }

  /** @brief Destructor*/
  ~Agglomerator_Pure_Front() {};

  /** @brief Specialization of the pure virtual function in the parent class, to
   * be used in couple with the
   * Agglomerate_one_level of the Agglomerator_Isotropic */
  unordered_set<CoMMAIndexType> choose_optimal_cc_and_update_seed_pool(
      const CoMMAIndexType seed, CoMMAIntType &compactness) override {
    bool is_order_primary = false;
    // The goal of this function is to choose from a pool of neighbour the
    // better one to build a compact coarse cell
    assert(this->_goal_card != -1);  // _goal_card has been initialized
    // OUTPUT: Definition of the current cc, IT WILL BE GIVEN AS AN OUTPUT
    unordered_set<CoMMAIndexType> s_current_cc = {seed};
    // Dictionary of the neighborhood of the seed, the key is the global index
    // of the cell and the value the order of distance from the seed (1 order
    // direct neighborhood, 2 order etc.)
    unordered_map<CoMMAIndexType, CoMMAIntType> d_n_of_seed;
    // Number of fine cells constituting the current coarse cell in
    // construction.
    CoMMAIntType size_current_cc = 1;  // CC contains only one cell: the seed
    // set to 3 as default we set to this value the maximum order to which we
    // search to compose the coarse cell
    CoMMAIntType max_order_of_neighbourhood = this->_min_neighbourhood;

    // We fill the d_n_of_seeds considering the initial seed passed
    this->_fc_graph.compute_neighbourhood_of_cc(
        s_current_cc, max_order_of_neighbourhood,  // in and out
        d_n_of_seed,  // output, the function fills the dictionary
        this->_max_card,
        // anisotropic cells agglomerated (not to take into account in the
        // process)
        this->_cc_graph->_a_is_fc_agglomerated);

    // We get the number of neighborhoods
    CoMMAIntType nb_neighbours = this->_fc_graph.get_nb_of_neighbours(seed);
    // return the area of the face connected to the seed
    vector<CoMMAWeightType> neighbours_weights =
        this->_fc_graph.get_weights(seed);

    // If no neighbour is found for seed: this case happened only when isotropic
    // cell is surrounded by anisotropic cells.
    if (d_n_of_seed.empty()) {
      // d_n_of_seed is empty, i.e: the cc is not complete and no neighbour are
      // available!
      compactness = 0;
    }
    else if (static_cast<CoMMAIntType>(d_n_of_seed.size() + s_current_cc.size()) < this->_goal_card) {
      // Not enough available neighbour, the dictionary of the available cells
      // size summed with the current cell size is not enough to reach the goal
      // cardinality: creation of a (too small) coarse cell.
      // We add ALL the cells of the dictionary to the set of current coarse cell.
      for (auto &i_k_v : d_n_of_seed) {
        s_current_cc.insert(i_k_v.first);
      }
      // We check as a consequence the threshold cardinality that is a minimum
      // limit
      bool is_creation_delayed = (static_cast<CoMMAIntType>(s_current_cc.size()) <= this->_threshold_card);
      if (is_creation_delayed) {
        compactness = 0;  // Return
      } else {
        // minimum number of neighborhood of a connected cell
        compactness = this->_dimension;
        // TODO: CHECK THAT, it is not better to be substituted with number of
        // neighborhood?
      }
    }
    else {
      // If we passed the two previous checks, the minimum size is the goal
      // cardinality required
      // TODO: CHECK THAT, if the goal is 2, the minimum size would be 3?
      // ARGUABLE! Let's think to 3
      CoMMAIntType min_size = this->_goal_card;
      // Computation of the initial aspect ratio: we need cc_surf: i.e. the
      // external area (perimeter in 2D and sum of external faces in 3D) and
      // volume
      CoMMAWeightType cc_surf = accumulate(neighbours_weights.begin(),
          neighbours_weights.begin(), CoMMAWeightType{0});
      const CoMMAIntType n_bdry_f =
        (this->_fc_graph._seeds_pool).get_n_boundary_faces(seed);
      if (n_bdry_f > 0) {
        cc_surf += n_bdry_f * this->estimate_boundary_face(neighbours_weights);
      }
      // volume of cc is at first the volume of the seed.
      CoMMAWeightType vol_cc = this->_fc_graph._volumes[seed];
      // This dictionary is used to store the eligible cc: i.e. its size is
      // inside the permitted range. This is useful to track back our step if
      // needed. [size of the current, [cell set, d_n_of seed]]
      unordered_map<CoMMAIntType,
                    pair<unordered_set<CoMMAIndexType>,
                         unordered_map<CoMMAIndexType, CoMMAIntType>>>
          dict_cc_in_creation;
      CoMMAIntType min_external_faces = numeric_limits<CoMMAIntType>::max();
      CoMMAIntType arg_min_external_faces = min_size;
      // Here we define the exact dimension of the coarse cell as the min
      // between the max cardinality given as an input (remember the constructor
      // choice in case of -1) and the dictionary of the boundary cells, it
      // means the total number of neighborhood cells until the order we have
      // given (as default 3, so until the third order)
      CoMMAIntType max_ind =
          min(this->_max_card, static_cast<CoMMAIntType>(d_n_of_seed.size() + 1));
      // We add the faces that are on boundary calling the method of seed pool.
      CoMMAIntType number_of_external_faces_current_cc =
          nb_neighbours + this->_fc_graph._seeds_pool.boundary_value(seed) - 1;
      // d_keys_to_set from Util.h, it takes the keys of the unordered map and
      // create an unordered set. The unordered set is representing hence all
      // the neighbors of seed until a given order.
      unordered_set<CoMMAIndexType> s_neighbours_of_seed =
          d_keys_to_set<CoMMAIndexType, CoMMAIntType>(d_n_of_seed);
      // Build the class first order neighborhood
      First_Order_Neighbourhood<CoMMAIndexType, CoMMAIntType> f_o_neighbourhood =
          First_Order_Neighbourhood<CoMMAIndexType, CoMMAIntType>(
              s_neighbours_of_seed, this->_dimension,true);
      // Generate the set of the first order neighborhood to the given seed
      unordered_set<CoMMAIndexType> fon =
        f_o_neighbourhood.update(seed, this->_fc_graph.get_neighbours(seed));

      // Choice of the fine cells to agglomerate we enter in a while, we store
      // anyways all the possible coarse cells (not only the max dimension one)
      while (size_current_cc < max_ind) {
        // argmin_ar is the best fine cell to add
        CoMMAIndexType argmin_ar = -1;
        CoMMAWeightType min_ar_surf = numeric_limits<CoMMAWeightType>::max();
        CoMMAWeightType min_ar_vol = numeric_limits<CoMMAWeightType>::max();
        CoMMAIntType max_faces_in_common = 0;
        // We compute the best fine cell to add, based on the aspect
        // ratio and is given back in argmin_ar. It takes account also
        // the fine cells that has been added until now.
        compute_best_fc_to_add(fon, d_n_of_seed, is_order_primary, cc_surf,
                               vol_cc, s_current_cc, argmin_ar,  // output
                               max_faces_in_common, min_ar_surf, min_ar_vol);

        number_of_external_faces_current_cc +=
            this->_fc_graph.get_nb_of_neighbours(argmin_ar) +
            this->_fc_graph._seeds_pool.boundary_value(argmin_ar) - 1 -
            2 * max_faces_in_common;
        // we increase the cc
        size_current_cc++;
        s_current_cc.insert(argmin_ar);

        // if the constructed cc is eligible, i.e. its size is inside the
        // permitted range we store it inside dict_cc_in_creation This choice is
        // based on the idea that the smallest cc (w.r.t. card) is may be not
        // the one that minimized the number of external faces. If this if is
        // satisfied it means we have an eligible cell
        if ((min_size <= size_current_cc) || size_current_cc == max_ind) {

          if (number_of_external_faces_current_cc <= min_external_faces) {

            min_external_faces = number_of_external_faces_current_cc;
            arg_min_external_faces = size_current_cc;
          }

          // We update the dictionary of eligible coarse cells
          unordered_map<CoMMAIndexType, CoMMAIntType> new_dict;
          new_dict[argmin_ar] = d_n_of_seed[argmin_ar];
          pair<unordered_set<CoMMAIndexType>,
               unordered_map<CoMMAIndexType, CoMMAIntType>> p =
              make_pair(s_current_cc, new_dict);
          dict_cc_in_creation[size_current_cc] = p;
        }

        // Update of cc_surf and vol_cc with the new fc added
        cc_surf = min_ar_surf;
        vol_cc = min_ar_vol;

        // Remove added fc from the available neighbours
        d_n_of_seed.erase(argmin_ar);

        fon = f_o_neighbourhood.update(argmin_ar,
            this->_fc_graph.get_neighbours(argmin_ar));
      }

      s_current_cc = dict_cc_in_creation[arg_min_external_faces].first;

      // If we do not chose the biggest cc, we put the useless fc back to the
      // pool
      for (auto i_s = arg_min_external_faces + 1; i_s < max_ind + 1; i_s++) {
        // for all size of Cell from arg_min_external_faces+1 to  min(max_card,
        // len(d_n_of_seed) + 1) + 1
        // d_n_of_seed.
        for (auto iKV : dict_cc_in_creation[i_s].second) {
          d_n_of_seed[iKV.first] = iKV.second;
        }
      }
      assert(arg_min_external_faces == static_cast<CoMMAIntType>(s_current_cc.size()));
      // Computes the actual compactness of the coarse cell
      compactness =
          this->_fc_graph.compute_min_fc_compactness_inside_a_cc(s_current_cc);
    }  // end else
    // Update seeds
    // Create of l_of_new_seed:
    list<CoMMAIndexType> l_of_new_seed;
    if (!d_n_of_seed.empty()) {

      // CoMMAIntType size = d_n_of_seed.size();
      // l_of_new_seed.resize(size);
      // if d_n_of_seed is not empty
      // Reminder: d_n_of_seed is here the pool of cell neighbouring the
      // previous seed!
      for (auto &i_k_v : d_n_of_seed) {
        if (i_k_v.second <= 2) {
          l_of_new_seed.push_back(i_k_v.first);
        }
      }
      // if list of new seeds is still empty we go to order 3
      CoMMAIntType i_k = 3;
      while (l_of_new_seed.empty()) {
        // We put FC in the l_of_new_seed according to its order of neighbouring
        // of previous seed.
        // order 1 or 2 then 3, then 4 and so on.
        for (auto &i_k_v : d_n_of_seed) {
          if (i_k_v.second <= i_k) {
            l_of_new_seed.push_back(i_k_v.first);
          }
        }
        i_k++;
      }
    } else if (this->_fc_graph._seeds_pool.is_empty()) {
      // else d_n_of_seed is empty: we used every neighbour!
      // if list_of_seeds is empty, we look for new seeds to add to the
      // list_of_seeds.
      // if list_of_seeds is empty we look if there is some neighbour to the
      // current fc:
      // s_fc_neighbours_of_cc = set()
      // we remove seed because we already update its first neighbours.
      // A copy needed because the set is used inside ccg
      unordered_set<CoMMAIndexType> tmp_set(s_current_cc);
      tmp_set.erase(seed);

      // We add to s_fc_neighbours_of_cc all the neighbours of FC included in
      // s_fc_for_current_cc without seed
      for (auto &i_fc : tmp_set) {
        vector<CoMMAIndexType> a_neighbours =
            this->_fc_graph.get_neighbours(i_fc);
        for (const CoMMAIndexType &i_fc_n : a_neighbours) {
          if ((*(this->_cc_graph)).is_fc_not_already_agglomerated(i_fc_n)) {
            // _a_is_fc_agglomerated is up-to-date.
            // remark: we don't care i_fc_n == i_fc.
            l_of_new_seed.push_back(i_fc_n);
          }
        }
      }
    }
    // Update of list_of_seeds:
    (this->_fc_graph._seeds_pool).update(l_of_new_seed);
    return s_current_cc;
  }

 protected:
  /** @brief Method that inside the biconnected algorithm, checked in the
   * choose_optimal_cc_and_update_seed_pool function all the possible exception,
   * computes the best fine cells to add to the coarse cell.
   */
  void compute_best_fc_to_add(
      unordered_set<CoMMAIndexType> fon,
      const unordered_map<CoMMAIndexType, CoMMAIntType> &d_n_of_seed,
      const bool &is_order_primary, const CoMMAWeightType &cc_surf,
      const CoMMAWeightType &vol_cc,
      const unordered_set<CoMMAIndexType> &s_of_fc_for_current_cc,
      CoMMAIndexType &argmin_ar, CoMMAIntType &max_faces_in_common,
      CoMMAWeightType &min_ar_surf, CoMMAWeightType &min_ar_vol) {
    //  this function defines the best fine cells to add to create the coarse
    // cell for the current coarse cell considered
    CoMMAWeightType min_ar = numeric_limits<CoMMAWeightType>::max();
    CoMMAIndexType arg_max_faces_in_common = -1;

    // For every fc in the neighbourhood:
    // we update the new aspect ratio
    // we verify that fon is a sub member of the dict of seeds
    for (const CoMMAIndexType &i_fc : fon) {
      // we test every possible new cells to chose the one that locally
      // minimizes the Aspect Ratio at the first fine cell of the fon.
      if (arg_max_faces_in_common == -1) {
        arg_max_faces_in_common = i_fc;
      }

      // Compute features of the CC obtained by adding i_fc
      CoMMAIntType number_faces_in_common = 0;
      CoMMAWeightT new_ar = numeric_limits<CoMMAWeightType>::min();
      CoMMAWeightT new_ar_surf = numeric_limits<CoMMAWeightType>::min();
      CoMMAWeightT new_ar_vol = numeric_limits<CoMMAWeightType>::min();
      this->compute_next_cc_features(i_fc, cc_surf,
          vol_cc,
          s_of_fc_for_current_cc,
          // out
          number_faces_in_common, new_ar, new_ar_surf, new_ar_vol);

      // Neighborhood order of i_fc wrt to original seed of CC
      // [i_fc] is not const the method at returns the reference to the value of the
      // key i_fc.
      const CoMMAIntType &order = d_n_of_seed.at(i_fc);

      // TODO This version seems good but refactorisation to do: perhaps it is
      // not needed to update every new possible coarse cell aspect ratio?
      // TODO also need to remove the list of min_ar, argmin_ar, etc.
      if (number_faces_in_common >=
          max_faces_in_common or
              is_order_primary) {  // if is_order_primary is True the order of
                                   // neighbourhood is primary
        if (number_faces_in_common == max_faces_in_common or is_order_primary) {
          // If the number of faces in common is the same, let's see whether it's
          // worth to update or not

          if (order <= d_n_of_seed.at(arg_max_faces_in_common)) {
            // [arg_max_faces_in_common] is not const.
            if (order == d_n_of_seed.at(arg_max_faces_in_common)) {
              if (new_ar < min_ar and number_faces_in_common > 0 ) {
                // The second condition asserts the connectivity of the coarse
                // element.
                min_ar = new_ar;
                argmin_ar = i_fc;
                min_ar_surf = new_ar_surf;
                min_ar_vol = new_ar_vol;

                arg_max_faces_in_common = i_fc;
                // The number of face in common is the same no need to touch it
              }
            } else {
              // Case :number_faces_in_common == max_faces_in_common and order <
              // dict_neighbours_of_seed[arg_max_faces_in_common]:
              arg_max_faces_in_common = i_fc;
              min_ar = new_ar;
              argmin_ar = i_fc;
              min_ar_surf = new_ar_surf;
              min_ar_vol = new_ar_vol;
              // The number of face in common is the same no need to touch it
            }
          }
        } else {
          // Case number_faces_in_common > max_faces_in_common:
          // -> Just update and see what comes next
          max_faces_in_common = number_faces_in_common;
          arg_max_faces_in_common = i_fc;
          min_ar = new_ar;
          argmin_ar = i_fc;
          min_ar_surf = new_ar_surf;
          min_ar_vol = new_ar_vol;
        }
      }
    }
  }
};

#endif  // COMMA_PROJECT_AGGLOMERATOR_H
