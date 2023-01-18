#ifndef COMMA_PROJECT_AGGLOMERATOR_H
#define COMMA_PROJECT_AGGLOMERATOR_H

/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cmath>
#include <deque>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include "Coarse_Cell_Container.h"
#include "Dual_Graph.h"
#include "Util.h"
#include "Neighbourhood.h"

// How to pass parameters from base class
// https://stackoverflow.com/questions/9692675/c-constructor-where-parameters-are-used-by-base-class-constructor
// https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-base-class-constructor
// Error handling from
// https://weseetips.wordpress.com/tag/exception-from-constructor-initializer-list/

/** @brief A class responsible to do the interface between the different kinds
 * of agglomerator
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator {
 public:
  /** @brief Function which computes the aspect-ratio from a diameter and a volume
   *  In 3D: \f$ AR = diam_{CC} / \sqrt{vol_{CC}} \f$
   *  In 2D: \f$ AR = diam_{CC} / \sqrt[3]{vol_{CC}} \f$
   *  (Recall that in 2D the volume is the surface)
  */
  function<CoMMAWeightType(const CoMMAWeightType, const CoMMAWeightType)> _compute_AR;

  /** @brief The constructor of the interface
   *  @param[in] graph Dual_Graph object that determines the connectivity
   * of the matrix
   *  @param[in] cc_graph Container for the coarse cells
   *  @param[in] seeds_pool Seeds_Pool object giving the order in which the fine cells
   * should be considered when agglomerating
   *  @param[in] dimension the dimension of the problem
   */
  Agglomerator(shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> graph,
               shared_ptr<Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType,
                                     CoMMAIntType>> cc_graph,
               shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> seeds_pool,
               CoMMAIntType dimension = 3)
      : _dimension(dimension), _fc_graph(graph), _cc_graph(cc_graph),
      _seeds_pool(seeds_pool) {
    if ((_dimension != 2) && (_dimension != 3)) {
      throw range_error("dimension can only be 2 or 3");
    }
    if (_dimension == 2) {
      _min_neighbourhood = 2;
      _compute_AR = [](const CoMMAWeightType diam, const CoMMAWeightType area)
                        -> CoMMAWeightType { return diam / sqrt(area); };
    } else {
      _min_neighbourhood = 3;
      _compute_AR = [](const CoMMAWeightType diam, const CoMMAWeightType vol)
                        -> CoMMAWeightType { return diam / cbrt(vol); };
    }
    _l_nb_of_cells.push_back(graph->_number_of_cells);
  }

  /** @brief The destructor of the class */
  virtual ~Agglomerator() = default;

  /** @brief Accessor to retrieve the fine cells to coarse cells from the coarse
   * cell graphs class
   */
  inline vector<CoMMAIndexType> get_fc_2_cc() const { return _cc_graph->_fc_2_cc; }

  /** @brief Pure virtual function which implementation is specified in the
   * related child classes and that defines the agglomeration of one level.
   * @param[in] goal_card goal cardinality of the coarse cell
   * @param[in] min_card minimum cardinality of the coarse cell
   * @param[in] max_card maximum cardinality of the coarse cell
   * @param[in] priority_weights Weights used to set the order telling where to start
   * agglomerating. The higher the weight, the higher the priority
   * @param[in] correction_steps it defines if corrections for the isotropic
   * algorithm are activated or not, for anisotropic algorithm not taken into
   * account.
   */
  virtual void agglomerate_one_level(const CoMMAIntType goal_card,
                                     const CoMMAIntType min_card,
                                     const CoMMAIntType max_card,
                                     const vector<CoMMAWeightType> &priority_weights,
                                     bool correction_steps) = 0;

 protected:
  /** @brief dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3
   * -> 3D)*/
  CoMMAIntType _dimension;
  /** @brief minimum number of neighbourhood we extend to search the neighbourhood
   * in the greedy algorithm. Set as default to 3.*/
  CoMMAIntType _min_neighbourhood = 3;
  /** @brief minimum cardinality. Set as default to 0 (meaning the maximum
   * possible).*/
  CoMMAIntType _min_card = 0;
  /** @brief maximum cardinality. Set as default to 0 (meaning the maximum
   * possible)*/
  CoMMAIntType _max_card = 0;
  /** @brief Goal cardinality. Set as default to 0 (meaning the maximum
   * possible)*/
  CoMMAIntType _goal_card = 0;
  /** @brief Threshold cardinality. Set as default to 0 (meaning the maximum
   * possible)*/
  CoMMAIntType _threshold_card = 0;
  /** @brief List of number of cells per coarse cell created.*/
  vector<CoMMAIndexType> _l_nb_of_cells;
  /** @brief Dual_Graph object determining Fine cells graph and hence the
   * connectivity.*/
  shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> _fc_graph;
  /** @brief pointer to Coarse Cell Graph element */
  shared_ptr<Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType,
                                   CoMMAIntType>> _cc_graph;
  /** @brief Seeds_Pool object giving the order in which the fine cells should
   * be considered when agglomerating
   **/
  shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> _seeds_pool;

};

/** @brief Agglomerator_Anisotropic class is a child class of the Agglomerator
 * class that specializes the implementation to the case of Anisotropic
 * agglomeration.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 **/
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Anisotropic
    : public Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
 public:

  /** @brief Container for an anisotropic line */
  using AnisotropicLine = deque<CoMMAIndexType>;

  /** @brief (Shared) Pointer to an anisotropic line */
  using AnisotropicLinePtr = shared_ptr<AnisotropicLine>;

  /** @brief Constructor.
   *  @param[in] graph Dual_Graph object that determines the connectivity
   * of the matrix
   *  @param[in] cc_graph Container for the coarse cells
   *  @param[in] seeds_pool Seeds_Pool object giving the order in which the fine cells
   * should be considered when agglomerating
   *  @param[in] agglomerationLines_Idx Connectivity for the agglomeration lines: each
   * element points to a particular element in the vector \p agglomerationLines
   *  @param[in] agglomerationLines Vector storing all the elements of the
   * anisotropic lines
   *  @param[in] threshold_anisotropy Value of the aspect-ratio above which a cell is
   * considered as anisotropic
   *  @param[in] is_first_agglomeration Whether the current is the first agglomeration
   * meaning that anisotropic lines should be built
   *  @param[in] dimension the dimension of the problem
   */
  Agglomerator_Anisotropic(
      shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> graph,
      shared_ptr<Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType,
                                       CoMMAIntType>> cc_graph,
      shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> seeds_pool,
      const CoMMAWeightType threshold_anisotropy,
      const vector<CoMMAIndexType> &agglomerationLines_Idx,
      const vector<CoMMAIndexType> &agglomerationLines,
      const bool is_first_agglomeration,
      CoMMAIntType dimension = 3)
      : Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            graph, cc_graph, seeds_pool, dimension) {
    // for every defined level (1 by default), contains the number of cells
    // e.g. _l_nb_of_cells[0]= number of cells on finest level
    //      _l_nb_of_cells[1]= number of cells on the first coarse level
    this->_l_nb_of_cells.push_back(graph->_number_of_cells);
    // For every level, we have a set containing the admissible cells for
    // anisotropy cell number:
    // For level 0, it is the cell number of prism or hexahedron ...
    this->_v_of_s_anisotropic_compliant_fc =
        vector<unordered_set<CoMMAIndexType>>(2);
    this->_v_of_s_anisotropic_compliant_fc[0] =
        this->_fc_graph->_s_anisotropic_compliant_cells;

    this->_nb_lines = vector<CoMMAIndexType>(2);
    this->_v_lines = vector<vector<AnisotropicLinePtr>>(2);

    if (!is_first_agglomeration) {
      // case in which we have already agglomerated one level and hence we have
      // already agglomeration lines available; no need to recreate them.
#if 0
A brief remark about what follows: It used to be something like:
for (CoMMAIndexType i = agglomerationLines_Idx.size() - 2; i > -1; i--) {
  new_line = deque<CoMMAIndexType>(agglomerationLines.begin() + agglomerationLines_Idx[i],
                                   agglomerationLines.begin() + agglomerationLines_Idx[i + 1]);
}
However, if CoMMAIndexType were to be *unsigned*, then i > -1 (or i >=0) would lead
to an infinite loop. So we had to switch to pointers, and backwards ones since we loop
starting from the end.
With indices, we are used to constructions like "from idx[i] to idx[i + 1]". However,
using *backwards* pointers that translates into "from (*ptr) to (*(ptr - 1))"
#endif
      for (auto idx_ptr = agglomerationLines_Idx.rbegin() + 1;
           idx_ptr != agglomerationLines_Idx.rend(); idx_ptr++) {
        this->_v_lines[0].push_back(
            make_shared<AnisotropicLine>(
              agglomerationLines.begin() + (*idx_ptr),
              agglomerationLines.begin() + (*(idx_ptr - 1))
            )
        );
        this->_nb_lines[0] += 1;
      }
    }

    assert(threshold_anisotropy > 0);
    this->_threshold_anisotropy =
        (threshold_anisotropy < 1)
            ? static_cast<CoMMAWeightType>(1. / threshold_anisotropy)
            : threshold_anisotropy;
  }

  /** @brief Destructor*/
  virtual ~Agglomerator_Anisotropic() = default;

  /** @brief Specialization of the pure virtual function to the class
   * Agglomerator_Anisotropic.
   * We add the override key as a guard to possible mistakes:
   * https://stackoverflow.com/questions/46446652/is-there-any-point-in-using-override-when-overriding-a-pure-virtual-function
   **/
  void agglomerate_one_level(const CoMMAIntType goal_card,
                             const CoMMAIntType min_card,
                             const CoMMAIntType max_card,
                             const vector<CoMMAWeightType> &priority_weights,
                             bool correction_steps) override {
    // Unused parameters
    CoMMAUnused(goal_card);
    CoMMAUnused(min_card);
    CoMMAUnused(max_card);
    CoMMAUnused(priority_weights);
    CoMMAUnused(correction_steps);

    // if the finest agglomeration line is not computed, hence compute it
    // (REMEMBER! We compute the agglomeration lines only on the finest level,
    // the other one are stored only for visualization purpose)
    if (this->_v_lines[0].empty()) {
      // The anisotropic lines are only computed on the original (finest) mesh.
      this->compute_anisotropic_lines();  // finest level!!!
    }

    // In case the if is not realized, this is not the first generation of a
    // coarse level.
    // The anisotropic lines are given as input.
    // Copy of the current agglomeration_lines as a backup for visualization
    // purpose.

    this->_v_of_s_anisotropic_compliant_fc[1] = {};

    // Setting up lambda function that will perform the loop. This is needed since we
    // want to loop forwards or backwards according to some runtime value
    // See, e.g., https://stackoverflow.com/a/56133699/12152457
    auto loop_line = [&](auto begin, auto end) {
      AnisotropicLinePtr line_lvl_p_one = make_shared<AnisotropicLine>();
      // TODO here is necessary for the cc_create_a_cc but maybe we need in
      // some way to change that.
      const bool is_anisotropic = true;
      for (auto line_it = begin; line_it != end; line_it += 2) {
        // we agglomerate cells along the agglomeration line, hence we have to
        // go through the faces and agglomerate two faces together
        // Here we have to consider a special case when we have an odd number of cells:
        // THIS IS FUNDAMENTAL FOR THE CONVERGENCE OF THE MULTIGRID ALGORITHM
        unordered_set<CoMMAIndexType> s_fc = {*line_it, *(line_it + 1)};
        if (distance(line_it, end) == 3) {
          // If only three cells left, agglomerate them
          s_fc.insert(*(line_it + 2));
          line_it++;
        }
        // We create the coarse cell
        const CoMMAIndexType i_cc =
          this->_cc_graph->cc_create_a_cc(s_fc, is_anisotropic);
        line_lvl_p_one->push_back(i_cc);
        this->_v_of_s_anisotropic_compliant_fc[1].insert(i_cc);
      }

      this->_v_lines[1].push_back(line_lvl_p_one);
    }; // End lambda def

    // Process of every agglomeration lines:
    for (auto line_ptr = this->_v_lines[0].begin(); line_ptr != this->_v_lines[0].end();
         line_ptr++) {
      // We iterate on the anisotropic lines of a particular level (the level 1,
      // where they were copied from level 0).
      // We create a pointer to an empty deque for the line + 1, and hence for
      // the next level of agglomeration
      auto line = **line_ptr;
      if (line.size() <= 1) {
        // the agglomeration_line is empty and hence the iterator points again
        // to the empty deque, updating what is pointed by it and hence __v_lines[1]
        // (each time we iterate on the line, a new deque line_lvl_p_one is defined)
        continue;
      }
      // We start agglomerating from the head or the tail of the line according to
      // which of the two has more boundary faces
      const bool forward_line =
        this->_fc_graph->get_n_boundary_faces(line.front()) >=
          this->_fc_graph->get_n_boundary_faces(line.back());

      if (forward_line)
        loop_line(line.begin(), line.end());
      else
        loop_line(line.rbegin(), line.rend());

    }
  }

  /** @brief Function that prepares the anisotropic lines for output
   *  @param[in] level of the agglomeration process into the Multigrid algorithm
   *  @param[out] agglo_lines_array_idx Each element points to a particular
   * element in the vector agglo_lines_array. This is due to the storing structure.
   *  @param[out] agglo_lines_array Array storing all the element of the
   * anisotropic lines.
   **/
  void get_agglo_lines(CoMMAIntType level,
                       vector<CoMMAIndexType> &agglo_lines_array_idx,
                       vector<CoMMAIndexType> &agglo_lines_array) const {
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
    for (const auto &line_ptr : this->_v_lines[level]) {
      const auto line = *line_ptr; // Convenience
      const CoMMAIndexType size_of_line = line.size();
      // This vector store the end of a line and the start of a new anisotropic
      // line
      // WARNING! We are storing the anisotropic lines in a vector so we need a
      // way to point to the end of a line and the starting of a new one.
      agglo_lines_array_idx.push_back(size_of_line +
                                      number_of_fc_in_agglomeration_lines);
      // Here we store the index of the cell.
      for (const auto cell : line) {
        agglo_lines_array.push_back(cell);
      }
      number_of_fc_in_agglomeration_lines += size_of_line;
    }
  }

  /** @brief Vector of number of Anisotropic agglomeration lines per level*/
  vector<CoMMAIndexType> _nb_lines;

  /** @brief _v_lines : Agglomeration lines structure:
  * vector : level
  * forward list : identifier of the line
  * deque : line cells
  * e.g _v_lines[0] --> agglomeration lines at the finest level*/
  vector<vector<AnisotropicLinePtr>> _v_lines;

 protected:

  /** @brief Computes the anisotropic lines at the first level (only called at
   * the first level of agglomeration). Two main steps are performed:
   * 1) Look for anisotropic cells (via the dual graph)
   * 2) Build anisotropic lines
   */
  void compute_anisotropic_lines() {
    set<CoMMAIndexType> anisotropic_fc;
    // It is the max_weight, hence the maximum area among the faces composing the cell.
    // Used to recognized the face
    vector<CoMMAWeightType> maxArray(this->_fc_graph->_number_of_cells, 0.0);
    // Computation of the anisotropic cell, alias of the cells for which the
    // ratio between the face with maximum area and the face with minimum area
    // is more than a given threshold.
    this->_fc_graph->compute_anisotropic_fc(maxArray, anisotropic_fc,
                                            _threshold_anisotropy, 0);
    // Map to address if the cell has been added to a line
    unordered_map<CoMMAIndexType, bool> has_been_treated;
    for (auto &i_fc : anisotropic_fc) {
      has_been_treated[i_fc] = false;
    }
    // size of the line
    this->_nb_lines[0] = 0;
    // we cycle on all the anisotropic cells identified before
    for (auto &i_fc : anisotropic_fc) {
      // seed from where we start the deck
      if (has_been_treated[i_fc]) {
        // If the cell has been already treated, continue to the next
        // anisotropic cell in the unordered map
        continue;
      }
      // we save the primal seed for the opposite direction check that will
      // happen later
      const auto primal_seed = i_fc;
      // seed to be considered to add or not a new cell to the line
      CoMMAIndexType seed = primal_seed;
      // Create the new line
      AnisotropicLinePtr cur_line = make_shared<AnisotropicLine>();
      // we add the first seed
      cur_line->push_back(seed);
      has_been_treated[seed] = true;
      // Flag to determine end of line
      bool end = false;
      // Flag to determine if we arrived at the end of an extreme of a line
      bool opposite_direction_check = false;
      // Start the check from the seed
      // while the line is not ended
      while (!end) {
        // for the seed (that is updated each time end!= true) we fill the
        // neighbours and the weights
        const vector<CoMMAIndexType> v_neighbours = this->_fc_graph->get_neighbours(seed);
        const vector<CoMMAWeightType> v_w_neighbours = this->_fc_graph->get_weights(seed);
        // vector of the candidates to continue the line
        vector<CoMMAIndexType> candidates;
        for (auto i = decltype(v_neighbours.size()){0}; i < v_neighbours.size(); i++) {
          if (v_neighbours[i] != seed
              // Avoid the seed (it should not happen, but better safe than sorry)
                and anisotropic_fc.count(v_neighbours[i]) != 0
                // if anisotropic cell...
                  and v_w_neighbours[i] > 0.90 * maxArray[seed]
                  // ...and if along the max interface...
                      and !has_been_treated[v_neighbours[i]]
                ) {  // ...and if not treated
            candidates.push_back(v_neighbours[i]);
          }
        }  // end for loop
        // case we have only 1 candidate to continue the line
        if (candidates.size() == 1) {
          // we can add to the actual deque
          if (!opposite_direction_check) {
            cur_line->push_back(candidates[0]);
          } else {
            cur_line->push_front(candidates[0]);
          }
          // update the seed to the actual candidate
          seed = candidates[0];
          // the candidate (new seed) has been treated
          has_been_treated[seed] = true;
        }
        // case we have more than one candidate
        else if (candidates.size() > 1) {
          // we cycle on candidates
          /** @todo Not properly efficient. We risk to do twice the operations
        * (we overwrite the seed). This is not proper */
          for (auto &element : candidates) {
            // if has been treated ==> we check the next candidate
            if (has_been_treated[element]) {
              continue;
            } else {
              // if has not been treated, the opposite direction flag
              // is not active? ==> push back
              if (!opposite_direction_check) {
                cur_line->push_back(element);
                seed = element;
                has_been_treated[element] = true;
                // It break otherwise we risk to add 2 (problematic with primal
                // seed)
                // It is what is done in Mavriplis
                // https://scicomp.stackexchange.com/questions/41830/anisotropic-lines-identification-algorithm
                break;
              } else {  // if it is active push front
                cur_line->push_front(element);
                seed = element;
                has_been_treated[element] = true;
                break;
              }
              // we update the seed and the has been treated
            }
          }
        }  // end elseif
        // 0 candidate, we are at the end of the line or at the end of one direction
        else /*if (candidates.size() == 0)*/ {
          if (opposite_direction_check) {
            end = true;
          } else {
            seed = primal_seed;
            opposite_direction_check = true;
          }
        }
      }
      // we push the deque to the list if are bigger than 1
      if (cur_line->size() > 1) {
        this->_v_lines[0].push_back(cur_line);
        this->_nb_lines[0] += 1;
      }
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
 * agglomeration.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 **/
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Isotropic
    : public Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
 public:
  /** @brief NeighbourhoodCreator type (base class) */
  using NeighbourhoodCreatorBaseType = NeighbourhoodCreator<CoMMAIndexType, CoMMAWeightType,
                                                            CoMMAIntType>;
  /** @brief NeighbourhoodExtendedCreator type (derived class) */
  using NeighbourhoodCreatorExtType = NeighbourhoodExtendedCreator<CoMMAIndexType, CoMMAWeightType,
                                                                   CoMMAIntType>;
  /** @brief NeighbourhoodPureFrontCreator type (derived class) */
  using NeighbourhoodCreatorPFType = NeighbourhoodPureFrontCreator<CoMMAIndexType, CoMMAWeightType,
                                                                   CoMMAIntType>;
  /** @brief Number of iterations allowed for the algorithm choosing which fine cell
   * to add next
   */
  CoMMAIntType _fc_iter;

  /** @brief Creator responsible for neighborhood objects */
  shared_ptr<NeighbourhoodCreatorBaseType> _neigh_crtor;

  /** @brief Constructor. The constructor takes as arguments the same arguments
  * of the father and in this way activates also the constructor of the base class.
  **/
  Agglomerator_Isotropic(
      shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> graph,
      shared_ptr<Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType,
                                       CoMMAIntType>> cc_graph,
      shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> seeds_pool,
      CoMMAIntType agglo_type, CoMMAIntType fc_iter, CoMMAIntType dimension = 3)
      : Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            graph, cc_graph, seeds_pool, dimension), _fc_iter(fc_iter) {
    if (agglo_type == CoMMANeighbourhoodT::EXTENDED)
      _neigh_crtor = make_shared<NeighbourhoodCreatorExtType>();
    else
      _neigh_crtor = make_shared<NeighbourhoodCreatorPFType>();
  }

  /** @brief Destructor*/
  virtual ~Agglomerator_Isotropic() = default;

  /** @brief The task of the function is to set the parameters of
   * determine the cardinality limits with respect to the parameters passed
   * @param[in] goal_card goal cardinality of the coarse cell (set as default to
   * 0 indicating in our case the maximum value)
   * @param[in] min_card minimum cardinality of the coarse cell(set as default
   * to 0 indicating in our case the maximum value)
   * @param[in] max_card maximum cardinality of the coarse cell(set as default
   * to 0 indicating in our case the maximum value)
   **/
  void set_agglomeration_parameter(CoMMAIntType goal_card = 0,
                                   CoMMAIntType min_card  = 0,
                                   CoMMAIntType max_card  = 0) {
    // Note[RM]: I tried make the following const static but ended up
    // with some SEGFAULT with Intel possibly linked to the following
    // https://stackoverflow.com/a/36406774
    unordered_map<CoMMAIntType, CoMMAIntType>
                          d_default_min_card = {{2, 3}, {3, 6}};
    unordered_map<CoMMAIntType, CoMMAIntType>
                          d_default_max_card = {{2, 5}, {3, 10}};
    unordered_map<CoMMAIntType, CoMMAIntType>
                          d_default_goal_card = {{2, 4}, {3, 8}};
    unordered_map<CoMMAIntType, CoMMAIntType>
                          d_default_threshold_card = {{2, 2}, {3, 3}};
    // Definition of _min_card
    if (min_card == 0) {
      this->_min_card = d_default_min_card.at(this->_dimension);
    } else {
      this->_min_card = min_card;
    }

    // Definition of _max_card
    if (max_card == 0) {
      this->_max_card = d_default_max_card.at(this->_dimension);
    } else {
      this->_max_card = max_card;
    }
    // Definition of _goal_card
    if (goal_card == 0) {
      this->_goal_card = d_default_goal_card.at(this->_dimension);
    } else {
      this->_goal_card = goal_card;
    }

    // Definition of _threshold_card
    this->_threshold_card = d_default_threshold_card.at(this->_dimension);
  }

  /** @brief Specialization of the pure virtual function to the class
   * Agglomerator_Isotropic.
   * We add the override key as a guard to possible mistakes:
   * https://stackoverflow.com/questions/46446652/is-there-any-point-in-using-override-when-overriding-a-pure-virtual-function
   * The function must be called later by the derived class in order to
   * specialize the implementation of the choose optimal_cc. For further
   * information about the structure, have a look at :
   * http://www.cplusplus.com/forum/general/31851/
   * The pseudo-code considers the while function and the agglomeration
   * process is not completed until all the cells are not agglomerated. Hence:
   * - we choose a new seed
   * - we check with a specific algorithm the neighbouring cells to
   * agglomerate to the seed
   * - we create a new coarse cell (using the apposite method in cc_graph)
   */
  void agglomerate_one_level(const CoMMAIntType goal_card,
                             const CoMMAIntType min_card,
                             const CoMMAIntType max_card,
                             const vector<CoMMAWeightType> &priority_weights,
                             bool correction_steps) override {
    set_agglomeration_parameter(goal_card, min_card, max_card);
    // We define a while for which we control the number of agglomerated cells
    CoMMAIntType compactness = 0;
    CoMMAIndexType nb_of_fc = this->_l_nb_of_cells[0];
    while (this->_cc_graph->get_number_of_fc_agglomerated() < nb_of_fc) {
      // 1) Choose a new seed
      auto seed = this->_seeds_pool->choose_new_seed(
          this->_cc_graph->_a_is_fc_agglomerated);
      assert(seed.has_value());
      // 2) Choose the set of Coarse Cells with the specification of the
      // algorithm in the children class
      const unordered_set<CoMMAIndexType> set_current_cc =
          choose_optimal_cc_and_update_seed_pool(seed.value(), compactness,
                                                 priority_weights);
      // 3)  Creation of cc:
      bool is_anistropic = false;
      // Check if delay the agglomeration based on compactness written during
      // the optimal cc choice process. Remember the compactness is the minimum
      // degree in the coarse cell.
      //bool is_creation_delayed = compactness < this->_dimension;
      bool is_creation_delayed = false;
      this->_cc_graph->cc_create_a_cc(set_current_cc, is_anistropic,
                                      is_creation_delayed);
    }
    // When we exit from this process all the cells are agglomerated, apart the
    // delayed ones
    // We proceed in creating the delayed ones
    this->_cc_graph->cc_create_all_delayed_cc();
    // Correct if necessary
    if (correction_steps) {
      this->_cc_graph->correct(this->_max_card);
    }
    this->_l_nb_of_cells.push_back(this->_cc_graph->_cc_counter);
  }

  /** @brief Approximate the value of a boundary face using the known internal
   * faces. It uses a (geometric) average, so the result is correct only if the cell
   * is a regular polygon
   * @param int_faces Vector of the surfaces of the internal faces
   * @return An approximation of the surface of a boundary face
   */
  inline CoMMAWeightType estimate_boundary_face(
      const vector<CoMMAWeightType> &int_faces) const {
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
   * agglomerated (Current coarse cell means without \p i_fc)
   *  @param[in] i_fc Index of the fine cell to add to the coarse cell
   *  @param[in] cc_diam (Approximation of the) Diameter of the current coarse cell
   *  @param[in] cc_vol Volume of the current coarse cell
   *  @param[in] fc_of_cc Index of the fine cells already agglomerated in the coarse
   *  cell
   *  @param[out] shared_faces Number of faces shared by the fine cell with the
   *  current coarse cell
   *  @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   *  @param[out] new_diam (Approximation of the) Diameter of the (final) coarse cell
   *  @param[out] new_vol Volume of the (final) coarse cell
   */
  inline void compute_next_cc_features(const CoMMAIndexType i_fc,
    const CoMMAWeightType cc_diam, const CoMMAWeightType cc_vol,
    const unordered_set<CoMMAIndexType> &fc_of_cc,
    // out
    CoMMAIntType &shared_faces, CoMMAWeightType &aspect_ratio,
    CoMMAWeightType &new_diam, CoMMAWeightType &new_vol) const {
    // Compute shared faces
    shared_faces = 0;
    const vector<CoMMAIndexType> v_neighbours = this->_fc_graph->get_neighbours(i_fc);
    for (const auto i_n : v_neighbours) {
      if (i_n != i_fc && (fc_of_cc.count(i_n) != 0))
        shared_faces++;
    }

    // Compute new diameter
    const vector<CoMMAWeightType>& cen_fc = this->_fc_graph->_centers[i_fc];
    CoMMAWeightType max_diam = cc_diam;
    for (const auto i_fc_cc : fc_of_cc) {
      const CoMMAWeightType d = euclidean_distance<CoMMAWeightType>(
          cen_fc, this->_fc_graph->_centers[i_fc_cc]);
      if (d > max_diam)
        max_diam = d;
    } // for i_fc_cc
    new_diam = max_diam;

    new_vol = cc_vol + this->_fc_graph->_volumes[i_fc];

    aspect_ratio = this->_compute_AR(new_diam, new_vol);
  }

  /** @brief Pure virtual function that must be implemented in child classes to
   * define the optimal coarse cell
   *  @param[in] seed the seed cell to start cumulate the other cells
   *  @param[in] compactness the compactness is defined as the minimum number of
   * neighbour of a fine cell inside a coarse cell
   *  @param[in] priority_weights Weights used to set the order telling where to start
   * agglomerating. The higher the weight, the higher the priority
   */
  virtual unordered_set<CoMMAIndexType> choose_optimal_cc_and_update_seed_pool(
      const CoMMAIndexType seed, CoMMAIntType &compactness,
      const vector<CoMMAWeightType> &priority_weights) = 0;

};

/** @brief Child class of Agglomerator Isotropic where is implemented a specific
 * biconnected algorithm for the agglomeration. We call it biconnected case, but
 * it is the greedy algorithm in reality.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Biconnected
    : public Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType,
                                    CoMMAIntType> {
 public:
  /** @brief Constructor of the class. No specific implementation, it
   * instantiates the base class Agglomerator_Isotropic
   **/
  Agglomerator_Biconnected(
      shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> graph,
      shared_ptr<Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType,
                                       CoMMAIntType>> cc_graph,
      shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> seeds_pool,
      CoMMAIntType agglo_type, CoMMAIntType fc_iter, CoMMAIntType dimension = 3)
      : Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            graph, cc_graph, seeds_pool, agglo_type, fc_iter, dimension) {
    // no particular constructor
  }

  /** @brief Destructor*/
  virtual ~Agglomerator_Biconnected() = default;

 /** @brief Specialization of the pure virtual function in the parent class, to
   * be used in couple with the Agglomerate_one_level of the Agglomerator_Isotropic
   */
  unordered_set<CoMMAIndexType> choose_optimal_cc_and_update_seed_pool(
      const CoMMAIndexType seed, CoMMAIntType &compactness,
      const vector<CoMMAWeightType> &priority_weights) override {
    bool is_order_primary = false;
    // The goal of this function is to choose from a pool of neighbour the
    // better one to build a compact coarse cell
    assert(this->_goal_card > 1);  // _goal_card has been initialized
    // OUTPUT: Definition of the current cc, IT WILL BE GIVEN AS AN OUTPUT
    unordered_set<CoMMAIndexType> s_current_cc = {seed};
    // Dictionary of the neighbourhood of the seed, the key is the global index
    // of the cell and the value the order of distance from the seed (1 order
    // direct neighbourhood, 2 order etc.)
    unordered_map<CoMMAIndexType, CoMMAIntType> d_n_of_seed;
    // Number of fine cells constituting the current coarse cell in
    // construction.
    CoMMAIntType size_current_cc = 1;  // CC contains only one cell: the seed
    // set to 3 as default we set to this value the maximum order to which we
    // search to compose the coarse cell
    CoMMAIntType max_order_of_neighbourhood =
      max(this->_min_neighbourhood, this->_max_card / this->_dimension);

    // We fill the d_n_of_seeds considering the initial seed passed
    this->_fc_graph->compute_neighbourhood_of_cc(
        s_current_cc, max_order_of_neighbourhood,  // in and out
        d_n_of_seed,  // output, the function fills the dictionary
        this->_max_card,
        // anisotropic cells agglomerated (not to take into account in the
        // process)
        this->_cc_graph->_a_is_fc_agglomerated);

    // We get the number of neighbourhoods
    CoMMAIntType nb_neighbours = this->_fc_graph->get_nb_of_neighbours(seed);
    // return the area of the face connected to the seed
    vector<CoMMAWeightType> neighbours_weights =
        this->_fc_graph->get_weights(seed);

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
        // minimum number of neighbourhood of a connected cell
        compactness = this->_dimension;
        // TODO: CHECK THAT, it is not better to be substituted with number of
        // neighbourhood?
      }
    }
    else {
      // If we passed the two previous checks, the minimum size is the goal
      // cardinality required
      // TODO: CHECK THAT, if the goal is 2, the minimum size would be 3?
      // ARGUABLE! Let's think to 3
      CoMMAIntType min_size = this->_min_card;
      // Computation of the initial aspect ratio
      CoMMAWeightType diam_cc{-1.};
      // CC in construction
      decltype(s_current_cc) tmp_cc = {seed};
      // volume of cc is at first the volume of the seed.
      CoMMAWeightType vol_cc = this->_fc_graph->_volumes[seed];
      // This dictionary is used to store the eligible cc: i.e. its size is
      // inside the permitted range. This is useful to track back our step if
      // needed. [size of the current, [cell set, d_n_of seed]]
      unordered_map<CoMMAIntType,
                    pair<unordered_set<CoMMAIndexType>,
                         unordered_map<CoMMAIndexType, CoMMAIntType>>>
          dict_cc_in_creation;
      CoMMAIntType min_external_faces = numeric_limits<CoMMAIntType>::max();
      CoMMAIntType max_compact = 0;
      CoMMAIntType arg_min_card = min_size;
      // Here we define the exact dimension of the coarse cell as the min
      // between the max cardinality given as an input (remember the constructor
      // choice in case of -1) and the dictionary of the boundary cells, it
      // means the total number of neighbourhood cells until the order we have
      // given (as default 3, so until the third order)
      CoMMAIntType max_ind =
          min(this->_max_card, static_cast<CoMMAIntType>(d_n_of_seed.size() + 1));
      // We add the faces that are on boundary calling the method of seed pool.
      CoMMAIntType number_of_external_faces_current_cc =
          nb_neighbours + this->_fc_graph->get_n_boundary_faces(seed) - 1;
      // d_keys_to_set from Util.h, it takes the keys of the unordered map and
      // create an unordered set. The unordered set is representing hence all
      // the neighbours of seed until a given order.
      unordered_set<CoMMAIndexType> s_neighbours_of_seed =
          d_keys_to_set<CoMMAIndexType, CoMMAIntType>(d_n_of_seed);
      // Build the class neighbourhood
      auto neighbourhood = this->_neigh_crtor->create(
          s_neighbours_of_seed, priority_weights, this->_dimension);
      // Generate the candidates cells in the neighbourhood of the given seed
      neighbourhood->update(seed, this->_fc_graph->get_neighbours(seed));

      // Choice of the fine cells to agglomerate we enter in a while, we store
      // anyways all the possible coarse cells (not only the max dimension one)
      while (size_current_cc < max_ind) {
        // argmin_ar is the best fine cell to add
        CoMMAIndexType argmin_ar = 0; // Dummy initialization
        CoMMAWeightType min_ar_diam = numeric_limits<CoMMAWeightType>::max();
        CoMMAWeightType min_ar_vol = numeric_limits<CoMMAWeightType>::max();
        CoMMAIntType max_faces_in_common = 0;
        // We compute the best fine cell to add, based on the aspect
        // ratio and is given back in argmin_ar. It takes account also
        // the fine cells that has been added until now.
        // min(max_ind - size_current_cc, this->_fc_iter)
        this->compute_best_fc_to_add(min(max_ind - size_current_cc, this->_fc_iter),
                                     neighbourhood,
                                     d_n_of_seed, is_order_primary, diam_cc,
                                     vol_cc, tmp_cc, argmin_ar,
                                     // output
                                     max_faces_in_common, min_ar_diam, min_ar_vol);

        number_of_external_faces_current_cc +=
            this->_fc_graph->get_nb_of_neighbours(argmin_ar) +
            this->_fc_graph->get_n_boundary_faces(argmin_ar) - 1 -
            2 * max_faces_in_common;
        // we increase the cc
        size_current_cc++;
        tmp_cc.insert(argmin_ar);
        const CoMMAIntType cur_compact =
          this->_fc_graph->compute_min_fc_compactness_inside_a_cc(tmp_cc);

        // if the constructed cc is eligible, i.e. its size is inside the
        // permitted range we store it inside dict_cc_in_creation This choice is
        // based on the idea that the smallest cc (w.r.t. card) is may be not
        // the one that minimized the number of external faces. If this if is
        // satisfied it means we have an eligible cell
        if ((min_size <= size_current_cc) || size_current_cc == max_ind) {

          if ( cur_compact > max_compact ) {
            max_compact = cur_compact;
            min_external_faces = number_of_external_faces_current_cc;
            arg_min_card = size_current_cc;

          } else if (cur_compact == max_compact) {
            if ( (number_of_external_faces_current_cc < min_external_faces) ||
                  ( number_of_external_faces_current_cc == min_external_faces &&
                    arg_min_card != this->_goal_card) ) {
              min_external_faces = number_of_external_faces_current_cc;
              arg_min_card = size_current_cc;
            }
          }

          // We update the dictionary of eligible coarse cells
          unordered_map<CoMMAIndexType, CoMMAIntType> new_dict;
          new_dict[argmin_ar] = d_n_of_seed[argmin_ar];
          pair<unordered_set<CoMMAIndexType>,
               unordered_map<CoMMAIndexType, CoMMAIntType>> p =
              make_pair(tmp_cc, new_dict);
          dict_cc_in_creation[size_current_cc] = p;
        }

        // Update of diam_cc and vol_cc with the new fc added
        diam_cc = min_ar_diam;
        vol_cc = min_ar_vol;

        // Remove added fc from the available neighbours
        d_n_of_seed.erase(argmin_ar);

        neighbourhood->update(argmin_ar, this->_fc_graph->get_neighbours(argmin_ar));
      }

      // Selecting best CC to return
      s_current_cc = move(dict_cc_in_creation[arg_min_card].first);

      // If we do not chose the biggest cc, we put the useless fc back to the
      // pool
      for (auto i_s = arg_min_card + 1; i_s < max_ind + 1; i_s++) {
        // for all size of Cell from arg_min_card+1 to  min(max_card,
        // len(d_n_of_seed) + 1) + 1
        // d_n_of_seed.
        for (auto iKV : dict_cc_in_creation[i_s].second) {
          d_n_of_seed[iKV.first] = iKV.second;
        }
      }

      // Updating Seed Pool with the neighbours of the final CC. Strategy:
      // - Compute the direct neighbours of the CC (not yet agglomerated)
      // - Insert in the queue starting with those of lowest neighbourhood order wrt
      //   to the original seed
      // - If more than one cell with the same order, use priority weights
      const auto cc_neighs = this->_fc_graph->get_neighbourhood_of_cc(s_current_cc,
                                     this->_cc_graph->_a_is_fc_agglomerated);
      // Basically, like d_n_of_seed but with key and value swapped
      map<CoMMAIntType, unordered_set<CoMMAIndexType>> neighs_by_order{};
      // For those that were outside max neighbourhood order
      unordered_set<CoMMAIndexType> neighs_not_found{};
      for (const auto &s : cc_neighs) {
        if (d_n_of_seed.find(s) != d_n_of_seed.end())
          neighs_by_order[d_n_of_seed.at(s)].insert(s);
        else
          neighs_not_found.insert(s);
      }
      for (const auto &[o, neighs] : neighs_by_order)
        if (!neighs.empty())
          this->_seeds_pool->order_new_seeds_and_update(neighs);
      if (!neighs_not_found.empty())
        this->_seeds_pool->order_new_seeds_and_update(neighs_not_found);

      assert(arg_min_card == static_cast<CoMMAIntType>(s_current_cc.size()));

      // Computes the actual compactness of the coarse cell
      compactness =
          this->_fc_graph->compute_min_fc_compactness_inside_a_cc(s_current_cc);
    }  // end else
    return s_current_cc;
  }

  /** @brief Computes the best fine cells to add to the coarse cell. The choice
   * depends on: the aspect-ratio of the coarse cell (tries to minimize it), the
   * number of shared faces (tries to maximize it), the neighbourhood order (see
   * related argument)
   *  @param[in] fc_iter (Possibly unused) Number of iteration for the fine-cell
   *  research algorithm
   *  @param[in] neighbourhood Neighborhood object
   *  @param[in] d_n_of_seed Dictionary containing the cells to consider for the
   *  agglomeration with their neighbourhood order wrt to the original seed
   *  @param[in] is_order_primary If true, the neighbourhood order prevails on other
   *  criteria
   *  @param[in] diam_cc (Approximation of the) Diameter of the current coarse cell
   *  @param[in] vol_cc Volume of the current coarse cell
   *  @param[in] s_of_fc_for_current_cc Indices of the fine cells already
   *  agglomerated in the coarse cell
   *  @param[out] argmin_ar Index wrt to neighbours vector of the chosen fine cell
   *  @param[out] max_faces_in_common Number of faces shared between the chosen fine
   *  cell and the coarse cell
   *  @param[out] min_ar_diam (Approximation of the) Diameter of the coarse cell
   *  after the addition of the chosen fine cell
   *  @param[out] min_ar_vol Volume of the coarse cell after the addition of the
   *  chosen fine cell
   */
  virtual void compute_best_fc_to_add(
      const CoMMAIntType fc_iter,
      const shared_ptr<Neighbourhood<CoMMAIndexType, CoMMAWeightType,
                                     CoMMAIntType>> neighbourhood,
      const unordered_map<CoMMAIndexType, CoMMAIntType> &d_n_of_seed,
      const bool &is_order_primary, const CoMMAWeightType &diam_cc,
      const CoMMAWeightType &vol_cc,
      const unordered_set<CoMMAIndexType> &s_of_fc_for_current_cc,
      CoMMAIndexType &argmin_ar, CoMMAIntType &max_faces_in_common,
      CoMMAWeightType &min_ar_diam, CoMMAWeightType &min_ar_vol) const {
    CoMMAUnused(fc_iter);
    //  this function defines the best fine cells to add to create the coarse
    // cell for the current coarse cell considered
    CoMMAWeightType min_ar = numeric_limits<CoMMAWeightType>::max();
    const auto neighbours = neighbourhood->get_candidates();
    CoMMAIndexType arg_max_faces_in_common = neighbours[0];

    for (const auto &i_fc : neighbours) {
      // we test every possible new cell to chose the one that locally maximizes the
      // number of shared faces and/or minimizes the Aspect Ratio
      // Compute features of the CC obtained by adding i_fc
      CoMMAIntType number_faces_in_common = 0;
      CoMMAWeightType new_ar = numeric_limits<CoMMAWeightType>::min();
      CoMMAWeightType new_ar_diam = numeric_limits<CoMMAWeightType>::min();
      CoMMAWeightType new_ar_vol = numeric_limits<CoMMAWeightType>::min();
      this->compute_next_cc_features(i_fc, diam_cc, vol_cc, s_of_fc_for_current_cc,
          // out
          number_faces_in_common, new_ar, new_ar_diam, new_ar_vol);

      // Neighbourhood order of i_fc wrt to original seed of CC
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
                min_ar_diam = new_ar_diam;
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
              min_ar_diam = new_ar_diam;
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
          min_ar_diam = new_ar_diam;
          min_ar_vol = new_ar_vol;
        }
      }
    }
  }
};

/** @brief Child class of Agglomerator Isotropic where is implemented a specific
 * iterative algorithm for the search of fine cells
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
class Agglomerator_Iterative
    : public Agglomerator_Biconnected<CoMMAIndexType, CoMMAWeightType,
                                      CoMMAIntType> {
 public:
  /** @brief Constructor of the class. No specific implementation, it
   * instantiates the base class Agglomerator_Isotropic
   **/
  Agglomerator_Iterative(
      shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> graph,
      shared_ptr<Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType,
                                       CoMMAIntType>> cc_graph,
      shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>> seeds_pool,
      CoMMAIntType agglo_type, CoMMAIntType fc_iter, CoMMAIntType dimension = 3)
      : Agglomerator_Biconnected<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
            graph, cc_graph, seeds_pool, agglo_type, fc_iter, dimension) {
    // no particular constructor
  }

  /** @brief Destructor*/
  virtual ~Agglomerator_Iterative() = default;

  /** @brief Computes the best fine cells to add to the coarse cell. The choice
   * depends on: the aspect-ratio of the coarse cell (tries to minimize it), the
   * number of shared faces (tries to maximize it), the neighbourhood order (see
   * related argument)
   *  @param[in] fc_iter (Possibly unused) Number of iteration for the fine-cell
   *  research algorithm
   *  @param[in] neighbourhood Neighborhood object
   *  @param[in] d_n_of_seed Dictionary containing the cells to consider for the
   *  agglomeration with their neighbourhood order wrt to the original seed
   *  @param[in] is_order_primary If true, the neighbourhood order prevails on other
   *  criteria
   *  @param[in] diam_cc (Approximation of the) Diameter of the current coarse cell
   *  @param[in] vol_cc Volume of the current coarse cell
   *  @param[in] s_of_fc_for_current_cc Indices of the fine cells already
   *  agglomerated in the coarse cell
   *  @param[out] argmin_ar Index wrt to neighbours vector of the chosen fine cell
   *  @param[out] max_faces_in_common Number of faces shared between the chosen fine
   *  cell and the coarse cell
   *  @param[out] min_ar_diam (Approximation of the) Diameter of the coarse cell
   *  after the addition of the chosen fine cell
   *  @param[out] min_ar_vol Volume of the coarse cell after the addition of the
   *  chosen fine cell
   */
  void compute_best_fc_to_add(
      const CoMMAIntType fc_iter,
      const shared_ptr<Neighbourhood<CoMMAIndexType, CoMMAWeightType,
                                     CoMMAIntType>> neighbourhood,
      const unordered_map<CoMMAIndexType, CoMMAIntType> &d_n_of_seed,
      const bool &is_order_primary, const CoMMAWeightType &diam_cc,
      const CoMMAWeightType &vol_cc,
      const unordered_set<CoMMAIndexType> &s_of_fc_for_current_cc,
      CoMMAIndexType &argmin_ar, CoMMAIntType &max_faces_in_common,
      CoMMAWeightType &min_ar_diam, CoMMAWeightType &min_ar_vol) const override {
    CoMMAIndexType outer_argmax_faces{0};
    CoMMAIntType ref_max_faces = max_faces_in_common;
    CoMMAWeightType outer_ar = numeric_limits<CoMMAWeightType>::max();
    for (const auto &i_fc : neighbourhood->get_candidates()) {
      auto cur_neighbourhood = this->_neigh_crtor->clone(neighbourhood);
      CoMMAWeightType inner_ar{-1.};
      CoMMAIntType inner_max_faces_in_common{0};
      CoMMAWeightType inner_min_ar_diam = numeric_limits<CoMMAWeightType>::max();
      CoMMAWeightType inner_min_ar_vol{0.};
      this->compute_next_cc_features(i_fc, diam_cc, vol_cc, s_of_fc_for_current_cc,
              inner_max_faces_in_common, inner_ar, inner_min_ar_diam, inner_min_ar_vol);
      cur_neighbourhood->update(i_fc, this->_fc_graph->get_neighbours(i_fc));
      unordered_set<CoMMAIndexType> cur_fc{s_of_fc_for_current_cc.begin(),
                                           s_of_fc_for_current_cc.end()};
      cur_fc.insert(i_fc);
      // Store value of mother cell
      const CoMMAIntType ref_inner_faces = inner_max_faces_in_common;

      if (fc_iter > 1) {
        CoMMAIndexType cur_argmin{0};
        CoMMAIntType cur_max_faces_in_common{0};
        CoMMAWeightType cur_min_ar_diam = numeric_limits<CoMMAWeightType>::max();
        CoMMAWeightType cur_min_ar_vol{0.};
        CoMMAWeightType cur_min_ar{0.};
        this->compute_best_fc_to_add(fc_iter-1, cur_neighbourhood, d_n_of_seed,
                                     is_order_primary, inner_min_ar_diam,
                                     inner_min_ar_vol, cur_fc,
                                     // output
                                     cur_argmin, cur_max_faces_in_common,
                                     cur_min_ar_diam, cur_min_ar_vol);
        // We just keep the min AR and the max faces in common
        if (cur_max_faces_in_common > inner_max_faces_in_common) {
          inner_max_faces_in_common = cur_max_faces_in_common;
        } else if (cur_max_faces_in_common == inner_max_faces_in_common &&
                   cur_min_ar < inner_ar) {
          inner_ar = cur_min_ar;
        }
      }

      const CoMMAIntType &order = d_n_of_seed.at(i_fc);

      // TODO This version seems good but refactorisation to do: perhaps it is
      // not needed to update every new possible coarse cell aspect ratio?
      // TODO also need to remove the list of min_ar, argmin_ar, etc.
      if (inner_max_faces_in_common >=
          ref_max_faces or
              is_order_primary) {  // if is_order_primary is True the order of
                                   // neighbourhood is primary
        if (inner_max_faces_in_common == ref_max_faces or is_order_primary) {
          // If the number of faces in common is the same, let's see whether it's
          // worth to update or not

          if (order <= d_n_of_seed.at(outer_argmax_faces)) {
            // [outer_argmax_faces] is not const.
            if (order == d_n_of_seed.at(outer_argmax_faces)) {
              if (inner_ar < outer_ar and inner_max_faces_in_common > 0 ) {
                // The second condition asserts the connectivity of the coarse
                // element.
                argmin_ar = i_fc;
                // Outer AR is the min AR of the children, but since diameter and
                // volume are used in the next step, we keep those of the mother cell...
                outer_ar = inner_ar;
                min_ar_diam = inner_min_ar_diam;
                min_ar_vol = inner_min_ar_vol;
                // ... same for faces in common
                max_faces_in_common = ref_inner_faces;

                outer_argmax_faces = i_fc;
              }
            } else {
              // Case :inner_max_faces_in_common == ref_max_faces and order <
              // dict_neighbours_of_seed[outer_argmax_faces]:
              outer_argmax_faces = i_fc;
              argmin_ar = i_fc;
              // Outer AR is the min AR of the children, but since diameter and
              // volume are used in the next step, we keep those of the mother cell...
              outer_ar = inner_ar;
              min_ar_diam = inner_min_ar_diam;
              min_ar_vol = inner_min_ar_vol;
              // ... same for faces in common
              max_faces_in_common = ref_inner_faces;
            }
          }
        } else {
          // Case inner_max_faces_in_common > ref_max_faces:
          // -> Just update and see what comes next
          ref_max_faces = inner_max_faces_in_common;
          outer_argmax_faces = i_fc;
          argmin_ar = i_fc;
          // Outer AR is the min AR of the children, but since diameter and
          // volume are used in the next step, we keep those of the mother cell...
          outer_ar = inner_ar;
          min_ar_diam = inner_min_ar_diam;
          min_ar_vol = inner_min_ar_vol;
          // ... same for faces in common
          max_faces_in_common = ref_inner_faces;
        }
      }
    } // for i_fc
  }

};

#endif  // COMMA_PROJECT_AGGLOMERATOR_H
