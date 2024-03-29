#ifndef COMMA_PROJECT_AGGLOMERATOR_H
#define COMMA_PROJECT_AGGLOMERATOR_H

/*
 * CoMMA
 *
 * Copyright © 2024 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include "CoMMA/Coarse_Cell_Container.h"
#include "CoMMA/Dual_Graph.h"
#include "CoMMA/Neighbourhood.h"
#include "CoMMA/Util.h"

namespace comma {

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
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Agglomerator {
public:
  /** @brief Function which computes the aspect-ratio from a diameter and a
   * volume\n In 3D: \f$ AR = \frac{diam_{CC}}{\sqrt{vol_{CC}}} \f$ \n In 2D:
   * \f$ AR = \frac{diam_{CC}}{\sqrt[3]{vol_{CC}}} \f$ \n (Recall that in 2D the
   * volume is the surface)
   */
  std::function<CoMMAWeightType(const CoMMAWeightType, const CoMMAWeightType)>
    _compute_AR;

  /** @brief The constructor of the interface
   *  @param[in] graph Dual_Graph object that determines the connectivity
   * of the matrix
   *  @param[in] cc_graph Container for the coarse cells
   *  @param[in] seeds_pool Seeds_Pool object giving the order in which the fine
   * cells should be considered when agglomerating
   *  @param[in] dimension Dimension of the problem
   */
  Agglomerator(
    std::shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      graph,
    std::shared_ptr<
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      cc_graph,
    std::shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      seeds_pool,
    CoMMAIntType dimension = 3) :
      _dimension(dimension),
      _fc_graph(graph),
      _cc_graph(cc_graph),
      _seeds_pool(seeds_pool) {
    if ((_dimension != 2) && (_dimension != 3)) {
      throw std::range_error("dimension can only be 2 or 3");
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
  inline std::vector<CoMMAIndexType> get_fc_2_cc() const {
    return _cc_graph->_fc_2_cc;
  }

  /** @brief Pure virtual function which implementation is specified in the
   * related child classes and that defines the agglomeration of one level.
   * @param[in] goal_card goal cardinality of the coarse cell
   * @param[in] min_card minimum cardinality of the coarse cell
   * @param[in] max_card maximum cardinality of the coarse cell
   * @param[in] priority_weights Weights used to set the order telling where to
   * start agglomerating. The higher the weight, the higher the priority
   * @param[in] correction_steps it defines if corrections for the isotropic
   * algorithm are activated or not, for anisotropic algorithm not taken into
   * account.
   */
  virtual void agglomerate_one_level(
    const CoMMAIntType goal_card,
    const CoMMAIntType min_card,
    const CoMMAIntType max_card,
    const std::vector<CoMMAWeightType> &priority_weights,
    bool correction_steps) = 0;

protected:
  /** @brief Dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3
   * -> 3D)*/
  CoMMAIntType _dimension;
  /** @brief Minimum number of neighbourhood we extend to search the
   * neighbourhood in the greedy algorithm. Set as default to 3 */
  CoMMAIntType _min_neighbourhood = 3;
  /** @brief Minimum cardinality (default = 0, meaning, equal to the dimension)
   */
  CoMMAIntType _min_card = 0;
  /** @brief Maximum cardinality (default = 0, meaning, 5 or 10 for, resp.,
   * 2- and 3D
   */
  CoMMAIntType _max_card = 0;
  /** @brief Goal cardinality (default = 0, meaning, 4 or 8 for, resp.,
   * 2- and 3D
   */
  CoMMAIntType _goal_card = 0;
  /** @brief Threshold cardinality (default = 0, meaning, equal to the
   * dimension)
   */
  CoMMAIntType _threshold_card = 0;
  /** @brief List of number of cells per coarse cell created */
  std::vector<CoMMAIndexType> _l_nb_of_cells;
  /** @brief Dual_Graph object determining Fine cells graph and hence the
   * connectivity
   */
  std::shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
    _fc_graph;
  /** @brief pointer to Coarse_Cell_Container element
   */
  std::shared_ptr<
    Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
    _cc_graph;
  /** @brief Seeds_Pool object giving the order in which the fine cells should
   * be considered when agglomerating
   */
  std::shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
    _seeds_pool;
};

/** @brief Agglomerator_Anisotropic class is a child class of the Agglomerator
 * class that specializes the implementation to the case of Anisotropic
 * agglomeration.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Agglomerator_Anisotropic :
    public Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief Container for an anisotropic line */
  using AnisotropicLine = std::deque<CoMMAIndexType>;

  /** @brief (Shared) Pointer to an anisotropic line */
  using AnisotropicLinePtr = std::shared_ptr<AnisotropicLine>;

  /** @brief Type of pair */
  using CoMMAPairType = std::pair<CoMMAIndexType, CoMMAWeightType>;
  /** @brief Type of set of pairs */
  using CoMMASetOfPairType =
    std::set<CoMMAPairType, CustomPairGreaterFunctor<CoMMAPairType>>;

  /** @brief Constructor.
   *  @param[in] graph Dual_Graph object that determines the connectivity
   * of the matrix
   *  @param[in] cc_graph Container for the coarse cells
   *  @param[in] seeds_pool Seeds_Pool object giving the order in which the fine
   * cells should be considered when agglomerating
   *  @param[in] agglomerationLines_Idx Connectivity for the agglomeration
   * lines: each element points to a particular element in the vector \p
   * agglomerationLines
   *  @param[in] agglomerationLines Vector storing all the elements of the
   * anisotropic lines
   *  @param[in] threshold_anisotropy Value of the aspect-ratio above which a
   * cell is considered as anisotropic
   * @param[in] priority_weights Weights used to set the order telling where to
   * start agglomerating. The higher the weight, the higher the priority
   *  @param[in] build_lines Whether lines joining the anisotropic cells should
   * be built
   *  @param[in] odd_line_length Whether anisotropic lines with odd length are
   * allowed
   *  @param[in] max_cells_in_line Maximum number of cells in an anisotropic
   *  line; when this value is reached, all reaming cells are discarded, hence
   *  considered as isotropic
   *  @param[in] dimension Dimension of the problem
   */
  Agglomerator_Anisotropic(
    std::shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      graph,
    std::shared_ptr<
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      cc_graph,
    std::shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      seeds_pool,
    const CoMMAWeightType threshold_anisotropy,
    const std::vector<CoMMAIndexType> &agglomerationLines_Idx,
    const std::vector<CoMMAIndexType> &agglomerationLines,
    const std::vector<CoMMAWeightType> &priority_weights,
    const bool build_lines,
    const bool odd_line_length,
    const std::optional<CoMMAIndexType> max_cells_in_line,
    CoMMAIntType dimension = 3) :
      Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
        graph, cc_graph, seeds_pool, dimension),
      _should_agglomerate(true),
      _aniso_neighbours(),
      _odd_line_length(odd_line_length),
      _max_cells_in_line(max_cells_in_line) {
    // for every defined level (1 by default), contains the number of cells
    // e.g. _l_nb_of_cells[0]= number of cells on finest level
    //      _l_nb_of_cells[1]= number of cells on the first coarse level
    this->_l_nb_of_cells.push_back(graph->_number_of_cells);

    this->_nb_lines = std::vector<CoMMAIndexType>(2);
    this->_v_lines = std::vector<std::vector<AnisotropicLinePtr>>(2);

    if (build_lines) {
      const CoMMAWeightType thr =
        (threshold_anisotropy > 1 || threshold_anisotropy < 0)
          ? threshold_anisotropy
          : static_cast<CoMMAWeightType>(1. / threshold_anisotropy);
      // if the finest agglomeration line is not computed, hence compute it
      // (REMEMBER! We compute the agglomeration lines only on the (original)
      // finest level
      this->_should_agglomerate =
        this->build_anisotropic_lines(priority_weights, thr);
    } else {
      // case in which we have already agglomerated one level and hence we have
      // already agglomeration lines available; no need to recreate them.
      this->_nb_lines[0] = 0;
      for (auto idx_ptr = agglomerationLines_Idx.cbegin() + 1;
           idx_ptr != agglomerationLines_Idx.cend(); ++idx_ptr) {
        const auto ln_sz = *idx_ptr - (*(idx_ptr - 1));
        if (ln_sz > 1) {
          this->_v_lines[0].push_back(std::make_shared<AnisotropicLine>(
            agglomerationLines.cbegin() + (*(idx_ptr - 1)),
            agglomerationLines.cbegin() + (*idx_ptr)));
          this->_nb_lines[0]++;
        }
      }
      this->_should_agglomerate = this->_nb_lines[0] > 0;
      if (!this->_should_agglomerate)
        std::cout
          << "CoMMA - No anisotropic line was built (e.g., only one-cell "
             "lines). Skipping anisotropic agglomeration"
          << std::endl;
    }
  }

  /** @brief Destructor*/
  ~Agglomerator_Anisotropic() override = default;

  /** @brief Specialization of the pure virtual function to the class
   * Agglomerator_Anisotropic.
   * We add the override key as a guard to possible mistakes:
   * https://stackoverflow.com/questions/46446652/is-there-any-point-in-using-override-when-overriding-a-pure-virtual-function
   * @param[in] goal_card goal cardinality of the coarse cell
   * @param[in] min_card minimum cardinality of the coarse cell
   * @param[in] max_card maximum cardinality of the coarse cell
   * @param[in] priority_weights Weights used to set the order telling where to
   * start agglomerating. The higher the weight, the higher the priority
   * @param[in] correction_steps it defines if corrections for the isotropic
   * algorithm are activated or not, for anisotropic algorithm not taken into
   * account.
   * @note The input arguments are not used since the structure of the
   * anisotropic agglomerator is fixed: cells belonging to the same anisotropic
   * lines are agglomerated pairwise (by 3 on some rare occasions).
   */
  void agglomerate_one_level(
    const CoMMAIntType goal_card,
    const CoMMAIntType min_card,
    const CoMMAIntType max_card,
    const std::vector<CoMMAWeightType> &priority_weights,
    bool correction_steps) override {
    // Unused parameters
    CoMMAUnused(goal_card);
    CoMMAUnused(min_card);
    CoMMAUnused(max_card);
    CoMMAUnused(correction_steps);

    if (this->_should_agglomerate) {
      // Necessary for the create_cc but maybe we need in
      // some way to change that.
      constexpr bool is_anisotropic = true;
      constexpr CoMMAIntType compactness = 1;

      // Setting up lambda function that will perform the loop. This is needed
      // since we want to loop forwards or backwards according to some runtime
      // value See, e.g., https://stackoverflow.com/a/56133699/12152457
      auto loop_line = [&](auto begin, auto end) {
        AnisotropicLinePtr line_lvl_p_one = std::make_shared<AnisotropicLine>();
        typename decltype(this->_max_cells_in_line)::value_type n_cells{0};
        for (auto line_it = begin; line_it != end; line_it += 2) {
          if (
            this->_max_cells_in_line.has_value()
            && n_cells + 2 > this->_max_cells_in_line.value()) {
            // Max length reached, break
            break;
          }
          // we agglomerate cells along the agglomeration line, hence we have to
          // go through the faces and agglomerate two faces together
          // Here we have to consider a special case when we have an odd number
          // of cells: THIS IS FUNDAMENTAL FOR THE CONVERGENCE OF THE MULTIGRID
          // ALGORITHM
          std::unordered_set<CoMMAIndexType> s_fc = {*line_it, *(line_it + 1)};
          n_cells += 2;
          // We update the neighbours. At this stage, we do not check if it is
          // or will be agglomerated since there will be a cleaning step after
          // the anisotropic agglomeration
          this->_aniso_neighbours.insert(
            this->_aniso_neighbours.end(),
            this->_fc_graph->neighbours_cbegin(*line_it),
            this->_fc_graph->neighbours_cend(*line_it));
          this->_aniso_neighbours.insert(
            this->_aniso_neighbours.end(),
            this->_fc_graph->neighbours_cbegin(*(line_it + 1)),
            this->_fc_graph->neighbours_cend(*(line_it + 1)));
          if (
            std::distance(line_it, end) == 3
            || (
              this->_max_cells_in_line.has_value()
              && n_cells + 1 == this->_max_cells_in_line.value())) {
            if (this->_odd_line_length) {
              // If only three cells left, agglomerate them
              s_fc.insert(*(line_it + 2));
              n_cells++;
              this->_aniso_neighbours.insert(
                this->_aniso_neighbours.end(),
                this->_fc_graph->neighbours_cbegin(*(line_it + 2)),
                this->_fc_graph->neighbours_cend(*(line_it + 2)));
            }
            line_it++;  // Ensure to break the loop after current iteration
          }
          // We create the coarse cell
          const CoMMAIndexType i_cc =
            this->_cc_graph->create_cc(s_fc, compactness, is_anisotropic);
          line_lvl_p_one->push_back(i_cc);
        }

        this->_v_lines[1].push_back(line_lvl_p_one);
      };  // End lambda def

      // Process of every agglomeration lines:
      for (auto line_ptr = this->_v_lines[0].begin();
           line_ptr != this->_v_lines[0].end(); line_ptr++) {
        // We iterate on the anisotropic lines of a particular level (the level
        // 1, where they were copied from level 0). We create a pointer to an
        // empty deque for the line + 1, and hence for the next level of
        // agglomeration
        auto line = **line_ptr;
        if (line.size() <= 1) {
          // the agglomeration_line is empty and hence the iterator points again
          // to the empty deque, updating what is pointed by it and hence
          // __v_lines[1] (each time we iterate on the line, a new deque
          // line_lvl_p_one is defined)
          continue;
        }
        // We start agglomerating from the head or the tail of the line
        // according to which of the two has more boundary faces, or priority or
        // ID
        const auto l_fr = line.front(), l_bk = line.back();
        const auto bnd_fr = this->_fc_graph->get_n_boundary_faces(l_fr),
                   bnd_bk = this->_fc_graph->get_n_boundary_faces(l_bk);
        const auto w_fr = priority_weights[l_fr], w_bk = priority_weights[l_bk];
        const bool forward_line =
          bnd_fr > bnd_bk  // Greater boundaries...
          || (bnd_fr == bnd_bk &&
              (w_fr > w_bk                       // ...or greater priority...
               || (w_fr == w_bk && l_fr < l_bk)  // ..or smaller index
               ));

        if (forward_line)
          loop_line(line.cbegin(), line.cend());
        else
          loop_line(line.crbegin(), line.crend());

      }  // End loop on lines

      this->update_seeds_pool();

    }  // End if should agglomerate
    else
      // If it didn't agglomerate, initialize for sure
      this->_seeds_pool->initialize();
  }

  /** @brief Update the seeds pool with the neighbours of the anisotropic cells
   * agglomerated so far.
   */
  void update_seeds_pool() {
    if (!this->_aniso_neighbours.empty()) {
      // Example of erase taken from
      // https://en.cppreference.com/w/cpp/container/deque/erase
      for (auto it = this->_aniso_neighbours.begin();
           it != this->_aniso_neighbours.end();) {
        if (this->_cc_graph->_is_fc_agglomerated[*it])
          it = this->_aniso_neighbours.erase(it);
        else
          ++it;
      }
      if (!this->_aniso_neighbours.empty()) {
        // The master queue is the one of the first agglomerated cell:
        // It is important to set it in the case the user asked for
        // neighbourhood priority
        this->_seeds_pool->set_top_queue(this->_fc_graph->get_n_boundary_faces(
          this->_aniso_neighbours.front()));
        this->_seeds_pool->update(this->_aniso_neighbours);
      }
    }
    // Even if we have updated it, the seeds pool might need initialization, for
    // instance, if it was set up with boundary priority
    if (this->_seeds_pool->need_initialization(
          this->_cc_graph->_is_fc_agglomerated))
      this->_seeds_pool->initialize();
  }

  /** @brief Function that prepares the anisotropic lines for output
   *  @param[in] level of the agglomeration process into the Multigrid algorithm
   *  @param[out] aniso_lines_idx Connectivity for the agglomeration lines: each
   * element points to a particular element in the vector \p aniso_lines
   *  @param[out] aniso_lines Vector storing all the elements of the anisotropic
   * lines
   */
  void export_anisotropic_lines(
    CoMMAIntType level,
    std::vector<CoMMAIndexType> &aniso_lines_idx,
    std::vector<CoMMAIndexType> &aniso_lines) const {
    // Reset lines
    aniso_lines_idx.clear();
    aniso_lines.clear();
    if (this->_should_agglomerate && !this->_v_lines[level].empty()) {
      // If at the level of agglomeration "level" the vector containing the
      // number of lines is empty, hence it means no line has been found at the
      // current level. variable cumulating the number of fine cells in the
      // agglomeration lines of the current level
      CoMMAIndexType number_of_fc_in_agglomeration_lines = 0;
      aniso_lines_idx.push_back(0);
      // We cycle over the line (in _v_lines)
      for (const auto &line_ptr : this->_v_lines[level]) {
        const auto line = *line_ptr;  // Convenience
        const CoMMAIndexType size_of_line = line.size();
        // This vector store the end of a line and the start of a new
        // anisotropic line WARNING! We are storing the anisotropic lines in a
        // vector so we need a way to point to the end of a line and the
        // starting of a new one.
        aniso_lines_idx.push_back(
          size_of_line + number_of_fc_in_agglomeration_lines);
        // Here we store the index of the cell.
        for (const auto cell : line) {
          aniso_lines.push_back(cell);
        }
        number_of_fc_in_agglomeration_lines += size_of_line;
      }
    }
  }

  /** @brief Vector of number of Anisotropic agglomeration lines per level */
  std::vector<CoMMAIndexType> _nb_lines;

  /** @brief _v_lines : Agglomeration lines structure:
   * - vector : level\n
   * - forward list : identifier of the line\n
   * - deque : line cells\n
   * - e.g., _v_lines[0] --> agglomeration lines at the finest level
   */
  std::vector<std::vector<AnisotropicLinePtr>> _v_lines;

  /** @brief Whether agglomeration is possible: for instance, if anisotropy
   * requested but no anisotropic cells found, there is no actual need.
   */
  bool _should_agglomerate;

protected:
  /** @brief Build the anisotropic lines at the first level (only called at
   * the first level of agglomeration). Two main steps are performed:
   * 1. Tag anisotropic cells (via the dual graph)
   * 2. Build anisotropic lines
   *
   * @param[in] priority_weights Weights used to set the order telling where to
   * start
   * @param[in] threshold_anisotropy Value of the aspect-ratio above which a
   * cell is
   * @return whether at least one line was built
   */
  bool build_anisotropic_lines(
    const std::vector<CoMMAWeightType> &priority_weights,
    const CoMMAWeightType threshold_anisotropy) {
    std::deque<CoMMAIndexType> aniso_seeds_pool;
    // It is the max_weight, hence the maximum area among the faces composing
    // the cell. Used to recognized the face
    std::vector<CoMMAWeightType> max_weights(
      this->_fc_graph->_number_of_cells, 0.0);
    std::vector<bool> to_treat(this->_fc_graph->_number_of_cells, false);
    // Computation of the anisotropic cell, alias of the cells for which the
    // ratio between the face with maximum area and the face with minimum area
    // is more than a given threshold.
    this->_fc_graph->tag_anisotropic_cells(
      max_weights, to_treat, aniso_seeds_pool, threshold_anisotropy,
      priority_weights, 0);
    if (aniso_seeds_pool.empty()) {
      std::cout << "CoMMA - No anisotropic cell found. Skipping anisotropic "
                   "agglomeration"
                << std::endl;
      return false;
    }
    // Size might not be the dimension
    const auto pts_dim = this->_fc_graph->_centers[0].size();
    // size of the line
    this->_nb_lines[0] = 0;
    // we cycle on all the anisotropic cells identified before
    for (auto &i_fc : aniso_seeds_pool) {
      // seed from where we start building the line
      if (!to_treat[i_fc]) {
        // If the cell has been already treated, continue to the next
        // anisotropic cell
        continue;
      }
      // we save the primal seed for the opposite direction check that will
      // happen later
      const auto primal_seed = i_fc;
      std::optional<std::vector<CoMMAWeightType>> primal_dir = std::nullopt;
      // seed to be considered to add or not a new cell to the line
      CoMMAIndexType seed = primal_seed;
      // Create the new line
      AnisotropicLinePtr cur_line = std::make_shared<AnisotropicLine>();
      // we add the first seed
      cur_line->push_back(seed);
      to_treat[seed] = false;
      // Flag to determine end of line
      bool end = false;
      // Flag to determine if we arrived at the end of an extreme of a line
      bool opposite_direction_check = false;
      // Info about growth direction
      std::vector<CoMMAWeightType> prev_cen =
        this->_fc_graph->_centers[seed];  // OK copy
      std::vector<CoMMAWeightType> prev_dir(pts_dim);
      bool empty_line = true;
      // Start the check from the seed
      // while the line is not ended
      while (!end) {
        // for the seed (that is updated each time end!= true) we fill the
        // neighbours and the weights
        const std::vector<CoMMAIndexType> v_neighbours =
          this->_fc_graph->get_neighbours(seed);
        const std::vector<CoMMAWeightType> v_w_neighbours =
          this->_fc_graph->get_weights(seed);
        auto n_it = this->_fc_graph->neighbours_cbegin(seed);
        auto w_it = this->_fc_graph->weights_cbegin(seed);
        // std::vector of the candidates to continue the line
        CoMMASetOfPairType candidates;
        // If the line is long enough, we use the direction. Otherwise, we use
        // the weight. Putting a high-level if to reduce the branching inside
        // the loop over the neighbours.
        if (empty_line) {
          for (; n_it != this->_fc_graph->neighbours_cend(seed);
               ++n_it, ++w_it) {
            if (to_treat[*n_it] && *w_it > 0.90 * max_weights[seed]) {
              candidates.emplace(*w_it, *n_it);
            }
          }  // end for loop
        } else {
          // If not an empty line, we check the direction, see
          // !dot_deviate below
          for (; n_it != this->_fc_graph->neighbours_cend(seed);
               ++n_it, ++w_it) {
            if (to_treat[*n_it] && *w_it > 0.90 * max_weights[seed]) {
              std::vector<CoMMAWeightType> cur_dir(pts_dim);
              get_direction<CoMMAWeightType>(
                prev_cen, this->_fc_graph->_centers[*n_it], cur_dir);
              const auto dot = dot_product<CoMMAWeightType>(prev_dir, cur_dir);
              if (!dot_deviate<CoMMAWeightType>(dot))
                candidates.emplace(fabs(dot), *n_it);
            }
          }  // end for loop
        }
        if (!candidates.empty()) {
          // Even if we have more than one candidate, we choose just one
          // otherwise we risk to add 2 (problematic with primal seed)
          // It is what is done in Mavriplis
          // https://scicomp.stackexchange.com/questions/41830/anisotropic-lines-identification-algorithm
          /** @todo Not properly efficient. We risk to do twice the operations
           * (we overwrite the seed). This is not proper
           */
          // update the seed to the actual candidate
          seed = candidates.begin()->second;
          if (!opposite_direction_check) {
            cur_line->push_back(seed);
          } else {
            cur_line->push_front(seed);
          }
          to_treat[seed] = false;
          empty_line = false;
          const auto &cur_cen = this->_fc_graph->_centers[seed];
          get_direction<CoMMAWeightType>(prev_cen, cur_cen, prev_dir);
          prev_cen = cur_cen;  // this->_fc_graph->_centers[seed];
          if (!primal_dir.has_value()) primal_dir = prev_dir;
        }
        // 0 candidate, we are at the end of the line or at the end of one
        // direction
        else /*if (candidates.size() == 0)*/ {
          // Before giving up, let's try another thing: Doing the same things as
          // above with the only difference that we allow to move through any
          // faces and not only the maximum one but still checking for direction
          if (!empty_line) {
            // If not an empty line, we check the direction, see is_parallel
            // below
            for (auto it = this->_fc_graph->neighbours_cbegin(seed);
                 it != this->_fc_graph->neighbours_cend(seed); ++it) {
              if (to_treat[*it]) {
                std::vector<CoMMAWeightType> cur_dir(pts_dim);
                get_direction<CoMMAWeightType>(
                  prev_cen, this->_fc_graph->_centers[*it], cur_dir);
                const auto dot =
                  dot_product<CoMMAWeightType>(prev_dir, cur_dir);
                if (!dot_deviate<CoMMAWeightType>(dot))
                  candidates.emplace(fabs(dot), *it);
              }
            }  // end for loop
            if (!candidates.empty()) {
              // We found one! Keep going!
              seed = candidates.begin()->second;
              if (!opposite_direction_check) {
                cur_line->push_back(seed);
              } else {
                cur_line->push_front(seed);
              }
              to_treat[seed] = false;
              empty_line = false;
              const auto &cur_cen = this->_fc_graph->_centers[seed];
              get_direction<CoMMAWeightType>(prev_cen, cur_cen, prev_dir);
              prev_cen = cur_cen;  // this->_fc_graph->_centers[seed];
              if (!primal_dir.has_value()) primal_dir = prev_dir;
            } else {
              if (opposite_direction_check) {
                end = true;
              } else {
                seed = primal_seed;
                prev_dir = primal_dir.value();
                prev_cen = this->_fc_graph->_centers[seed];
                opposite_direction_check = true;
              }
            }
          }  // End last step check on neighbours
          else {
            end = true;
          }
        }  // End of no candidates case
      }  // End of a line
      // we push the deque to the list if are bigger than 1
      if (cur_line->size() > 1) {
        this->_v_lines[0].push_back(cur_line);
        this->_nb_lines[0] += 1;
      }
    }  // End of loop over anisotropic cells

    if (this->_nb_lines[0] == 0) {
      std::cout << "CoMMA - No anisotropic line was built (e.g., only isolated "
                   "anisotropic cells). Skipping anisotropic agglomeration"
                << std::endl;
      return false;
    }
    return true;
  }

  /** @brief Neighbours of the anisotropic cells agglomerated. They are used to
   * update the seeds pool
   */
  std::deque<CoMMAIndexType> _aniso_neighbours;

  /** @brief Whether anisotropic lines with odd length are allowed. */
  bool _odd_line_length;

  /** @brief Maximum number of cells in an anisotropic line; when this value is
   * reached, all reaming cells are discarded, hence considered as isotropic
   */
  std::optional<CoMMAIndexType> _max_cells_in_line;
};

/** @brief Agglomerator_Isotropic class is a child class of the Agglomerator
 * class that specializes the implementation to the case of Isotropic
 * agglomeration.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Agglomerator_Isotropic :
    public Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief NeighbourhoodCreator type (base class) */
  using NeighbourhoodCreatorBaseType =
    NeighbourhoodCreator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief NeighbourhoodExtendedCreator type (derived class) */
  using NeighbourhoodCreatorExtType =
    NeighbourhoodExtendedCreator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief NeighbourhoodPureFrontCreator type (derived class) */
  using NeighbourhoodCreatorPFType = NeighbourhoodPureFrontCreator<
    CoMMAIndexType,
    CoMMAWeightType,
    CoMMAIntType>;
  /** @brief Number of iterations allowed for the algorithm choosing which fine
   * cell to add next
   */
  CoMMAIntType _fc_iter;

  /** @brief Creator responsible for neighborhood objects */
  std::shared_ptr<NeighbourhoodCreatorBaseType> _neigh_crtor;

  /** @brief Constructor. The constructor takes as arguments the same arguments
   * of the father and in this way activates also the constructor of the base
   * class.
   *  @param[in] graph Dual_Graph object that determines the connectivity
   * of the matrix
   *  @param[in] cc_graph Container for the coarse cells
   *  @param[in] seeds_pool Seeds_Pool object giving the order in which the fine
   * cells should be considered when agglomerating
   *  @param[in] neighbourhood_type Type of neighbourhood to use when growing a
   * coarse cell. See \ref CoMMANeighbourhoodT for more details.
   *  @param[in] fc_iter Number of iterations allowed for the algorithm choosing
   * which fine cell to add next. The cost grows exponentially, hence use small
   * values.
   *  @param[in] dimension Dimension of the problem
   */
  Agglomerator_Isotropic(
    std::shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      graph,
    std::shared_ptr<
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      cc_graph,
    std::shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      seeds_pool,
    CoMMAIntType neighbourhood_type,
    CoMMAIntType fc_iter,
    CoMMAIntType dimension = 3) :
      Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
        graph, cc_graph, seeds_pool, dimension),
      _fc_iter(fc_iter) {
    if (neighbourhood_type == CoMMANeighbourhoodT::EXTENDED)
      _neigh_crtor = std::make_shared<NeighbourhoodCreatorExtType>();
    else
      _neigh_crtor = std::make_shared<NeighbourhoodCreatorPFType>();
  }

  /** @brief Destructor*/
  ~Agglomerator_Isotropic() override = default;

  /** @brief The task of the function is to set the parameters of
   * determine the cardinality limits with respect to the parameters passed
   * @param[in] goal_card goal cardinality of the coarse cell (set as default to
   * 0 indicating in our case the maximum value)
   * @param[in] min_card minimum cardinality of the coarse cell(set as default
   * to 0 indicating in our case the maximum value)
   * @param[in] max_card maximum cardinality of the coarse cell(set as default
   * to 0 indicating in our case the maximum value)
   */
  void set_agglomeration_parameter(
    CoMMAIntType goal_card = 0,
    CoMMAIntType min_card = 0,
    CoMMAIntType max_card = 0) {
    // Note[RM]: I tried make the following const static but ended up
    // with some SEGFAULT with Intel possibly linked to the following
    // https://stackoverflow.com/a/36406774
    std::unordered_map<CoMMAIntType, CoMMAIntType> d_default_min_card = {
      {2, 3}, {3, 6}};
    std::unordered_map<CoMMAIntType, CoMMAIntType> d_default_max_card = {
      {2, 5}, {3, 10}};
    std::unordered_map<CoMMAIntType, CoMMAIntType> d_default_goal_card = {
      {2, 4}, {3, 8}};
    std::unordered_map<CoMMAIntType, CoMMAIntType> d_default_threshold_card = {
      {2, 2}, {3, 3}};
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
   * \n The function calls function \ref choose_optimal_cc_and_update_seeds_pool
   * of the derived class. For further information about the structure, have a
   * look at: http://www.cplusplus.com/forum/general/31851/ \n The pseudo-code
   * considers the while function and the agglomeration process is not completed
   * until all the cells are not agglomerated. Hence, while there are still non
   * agglomerate fine cells, repeat the following steps:
   * 1. Choose a new seed
   * 2. Check with a specific algorithm the neighbouring cells to
   * agglomerate to the seed
   * 3. Create a new coarse cell (using the apposite method in cc_graph)
   * @param[in] goal_card goal cardinality of the coarse cell
   * @param[in] min_card minimum cardinality of the coarse cell
   * @param[in] max_card maximum cardinality of the coarse cell
   * @param[in] priority_weights Weights used to set the order telling where to
   * start agglomerating. The higher the weight, the higher the priority
   * @param[in] correction_steps it defines if corrections for the isotropic
   * algorithm are activated or not, for anisotropic algorithm not taken into
   * account.
   */
  void agglomerate_one_level(
    const CoMMAIntType goal_card,
    const CoMMAIntType min_card,
    const CoMMAIntType max_card,
    const std::vector<CoMMAWeightType> &priority_weights,
    bool correction_steps) override {
    set_agglomeration_parameter(goal_card, min_card, max_card);
    constexpr bool is_anistropic = false;
    // We define a while for which we control the number of agglomerated cells
    const CoMMAIndexType nb_of_fc = this->_l_nb_of_cells[0];
    while (this->_cc_graph->get_number_of_fc_agglomerated() < nb_of_fc) {
      // 1) Choose a new seed
      const auto seed = this->_seeds_pool->choose_new_seed(
        this->_cc_graph->_is_fc_agglomerated);
      assert(seed.has_value());
      // 2) Choose the set of Coarse Cells with the specification of the
      // algorithm in the children class
      CoMMAIntType compactness = 0;
      const std::unordered_set<CoMMAIndexType> set_current_cc =
        choose_optimal_cc_and_update_seeds_pool(
          seed.value(), priority_weights, compactness);
      // 3)  Creation of cc
      // We check that threshold cardinality is reached
      // const bool creation_delayed =
      // (static_cast<CoMMAIntType>(s_current_cc.size()) <=
      // this->_threshold_card);
      // @TODO: We prefer to have coarse cells created right now, it might be
      // interesting to explore if delaying was allowed
      constexpr bool is_creation_delayed = false;
      this->_cc_graph->create_cc(
        set_current_cc, compactness, is_anistropic, is_creation_delayed);
    }
    // When we exit from this process all the cells are agglomerated, apart the
    // delayed ones
    // We proceed in creating the delayed ones
    this->_cc_graph->cc_create_all_delayed_cc();
    // Correct if necessary
    if (correction_steps) { this->_cc_graph->correct(this->_max_card); }
    this->_l_nb_of_cells.push_back(this->_cc_graph->_cc_counter);
  }

  /** @brief Approximate the value of a boundary face using the known internal
   * faces. It uses a (geometric) average, so the result is correct only if the
   * cell is a regular polygon
   * @param int_faces Vector of the surfaces of the internal faces
   * @return An approximation of the surface of a boundary face
   */
  inline CoMMAWeightType estimate_boundary_face(
    const std::vector<CoMMAWeightType> &int_faces) const {
    // Approximate with an average of the internal faces
    // We could choose many kinds of average, e.g. arithmetic or geometric, I
    // honestly don't know if one is better then the other...
    // Here, we use the geometric one, which should be less sensitive to
    // outliers
    return pow(
      accumulate(
        int_faces.begin(), int_faces.end(), CoMMAWeightType{1.},
        std::multiplies<>()),
      CoMMAWeightType{1.} / int_faces.size());
  }

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio and number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc)
   *  @param[in] i_fc Index of the fine cell to add to the coarse cell
   *  @param[in] cc_diam (Approximation of the) Diameter of the current coarse
   * cell
   *  @param[in] cc_vol Volume of the current coarse cell
   *  @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   *  @param[out] shared_faces Number of faces shared by the fine cell with the
   *  current coarse cell
   *  @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   *  @param[out] new_diam (Approximation of the) Diameter of the (final) coarse
   * cell
   *  @param[out] new_vol Volume of the (final) coarse cell
   */
  inline void compute_next_cc_features(
    const CoMMAIndexType i_fc,
    const CoMMAWeightType cc_diam,
    const CoMMAWeightType cc_vol,
    const std::unordered_set<CoMMAIndexType> &fc_of_cc,
    // out
    CoMMAIntType &shared_faces,
    CoMMAWeightType &aspect_ratio,
    CoMMAWeightType &new_diam,
    CoMMAWeightType &new_vol) const {
    // Compute shared faces
    shared_faces = 0;
    for (auto it = this->_fc_graph->neighbours_cbegin(i_fc);
         it != this->_fc_graph->neighbours_cend(i_fc); ++it) {
      if (*it != i_fc && (fc_of_cc.count(*it) != 0)) shared_faces++;
    }

    // Compute new diameter
    const std::vector<CoMMAWeightType> &cen_fc =
      this->_fc_graph->_centers[i_fc];
    CoMMAWeightType max_diam = cc_diam * cc_diam;
    for (const auto i_fc_cc : fc_of_cc) {
      const auto dist = squared_euclidean_distance<CoMMAWeightType>(
        cen_fc, this->_fc_graph->_centers[i_fc_cc]);
      if (dist > max_diam) max_diam = dist;
    }  // for i_fc_cc
    new_diam = sqrt(max_diam);

    new_vol = cc_vol + this->_fc_graph->_volumes[i_fc];

    aspect_ratio = this->_compute_AR(new_diam, new_vol);
  }

  /** @brief Pure virtual function that must be implemented in child classes to
   * define the optimal coarse cell
   *  @param[in] seed Cell from which the agglomeration of the CC starts
   *  @param[in] priority_weights Weights used to set the order telling where to
   * start agglomerating. The higher the weight, the higher the priority
   *  @param[out] compactness Compactness of the final CC, that is, the minimum
   * number of neighbours of a fine cell inside a coarse cell
   *  @return the (unordered) set of the fine cells composing the coarse cell
   */
  virtual std::unordered_set<CoMMAIndexType>
  choose_optimal_cc_and_update_seeds_pool(
    const CoMMAIndexType seed,
    const std::vector<CoMMAWeightType> &priority_weights,
    CoMMAIntType &compactness) = 0;
};

/** @brief Child class of Agglomerator_Isotropic where is implemented a specific
 * biconnected algorithm for the agglomeration. We call it biconnected case, but
 * it is the greedy algorithm in reality.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Agglomerator_Biconnected :
    public Agglomerator_Isotropic<
      CoMMAIndexType,
      CoMMAWeightType,
      CoMMAIntType> {
public:
  /** @brief Constructor of the class. No specific implementation, it
   * instantiates the base class Agglomerator_Isotropic.
   *  @param[in] graph Dual_Graph object that determines the connectivity
   * of the matrix
   *  @param[in] cc_graph Container for the coarse cells
   *  @param[in] seeds_pool Seeds_Pool object giving the order in which the fine
   * cells should be considered when agglomerating
   *  @param[in] neighbourhood_type Type of neighbourhood to use when growing a
   * coarse cell. See \ref CoMMANeighbourhoodT for more details.
   *  @param[in] fc_iter Number of iterations allowed for the algorithm choosing
   * which fine cell to add next. The cost grows exponentially, hence use small
   * values.
   *  @param[in] dimension Dimension of the problem
   */
  Agglomerator_Biconnected(
    std::shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      graph,
    std::shared_ptr<
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      cc_graph,
    std::shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      seeds_pool,
    CoMMAIntType neighbourhood_type,
    CoMMAIntType fc_iter,
    CoMMAIntType dimension = 3) :
      Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
        graph, cc_graph, seeds_pool, neighbourhood_type, fc_iter, dimension) {
    // no particular constructor
  }

  /** @brief Destructor*/
  ~Agglomerator_Biconnected() override = default;

  /** @brief Specialization of the pure virtual function in the parent class, to
   * be used in couple with the agglomerate_one_level of the
   * Agglomerator_Isotropic.
   *  @param[in] seed Cell from which the agglomeration of the CC starts
   *  @param[in] priority_weights Weights used to set the order telling where to
   * start agglomerating. The higher the weight, the higher the priority
   *  @param[out] compactness Compactness of the final CC, that is, the minimum
   * number of neighbours of a fine cell inside a coarse cell
   *  @return the (unordered) set of the fine cells composing the coarse cell
   */
  std::unordered_set<CoMMAIndexType> choose_optimal_cc_and_update_seeds_pool(
    const CoMMAIndexType seed,
    const std::vector<CoMMAWeightType> &priority_weights,
    CoMMAIntType &compactness) override {
    bool is_order_primary = false;
    // The goal of this function is to choose from a pool of neighbour the
    // better one to build a compact coarse cell
    assert(this->_goal_card > 1);  // _goal_card has been initialized
    // OUTPUT: Definition of the current cc, IT WILL BE GIVEN AS AN OUTPUT
    std::unordered_set<CoMMAIndexType> s_current_cc = {seed};
    // Dictionary of the neighbourhood of the seed, the key is the global index
    // of the cell and the value the order of distance from the seed (1 order
    // direct neighbourhood, 2 order etc.)
    std::unordered_map<CoMMAIndexType, CoMMAIntType> d_n_of_seed;
    // Number of fine cells constituting the current coarse cell in
    // construction.
    CoMMAIntType size_current_cc = 1;  // CC contains only one cell: the seed
    // set to 3 as default we set to this value the maximum order to which we
    // search to compose the coarse cell
    CoMMAIntType max_order_of_neighbourhood =
      std::max(this->_min_neighbourhood, this->_max_card / this->_dimension);

    // We fill the d_n_of_seeds considering the initial seed passed
    this->_fc_graph->compute_neighbourhood_of_cc(
      s_current_cc,
      max_order_of_neighbourhood,  // in and out
      d_n_of_seed,  // output, the function fills the dictionary
      this->_max_card,
      // anisotropic cells agglomerated (not to take into account in the
      // process)
      this->_cc_graph->_is_fc_agglomerated);

    // If no neighbour is found for seed: this case happened only when isotropic
    // cell is surrounded by anisotropic cells.
    if (d_n_of_seed.empty()) {
      // d_n_of_seed is empty, i.e: the cc is not complete and no neighbour are
      // available!
      compactness = 0;
    } else if (
      static_cast<CoMMAIntType>(d_n_of_seed.size() + s_current_cc.size())
      < this->_goal_card) {
      // Not enough available neighbour, the dictionary of the available cells
      // size summed with the current cell size is not enough to reach the goal
      // cardinality: creation of a (too small) coarse cell.
      // We add ALL the cells of the dictionary to the set of current coarse
      // cell.
      for (auto &i_k_v : d_n_of_seed) {
        s_current_cc.insert(i_k_v.first);
      }
      compactness =
        this->_fc_graph->compute_min_fc_compactness_inside_a_cc(s_current_cc);
    } else {
      // If we passed the two previous checks, the minimum size is the goal
      // cardinality required
      // TODO: CHECK THAT, if the goal is 2, the minimum size would be 3?
      // ARGUABLE! Let's think to 3
      // Computation of the initial aspect ratio
      CoMMAWeightType diam_cc{-1.};
      // CC in construction
      decltype(s_current_cc) tmp_cc = {seed};
      // volume of cc is at first the volume of the seed.
      CoMMAWeightType vol_cc = this->_fc_graph->_volumes[seed];
      // This dictionary is used to store the eligible cc: i.e. its size is
      // inside the permitted range. This is useful to track back our step if
      // needed. [size of the current, [cell set, d_n_of seed]]
      std::unordered_map<
        CoMMAIntType, std::pair<
                        std::unordered_set<CoMMAIndexType>,
                        std::unordered_map<CoMMAIndexType, CoMMAIntType>>>
        dict_cc_in_creation;
      CoMMAIntType min_external_faces =
        std::numeric_limits<CoMMAIntType>::max();
      CoMMAIntType max_compact = 0;
      CoMMAIntType arg_min_card = this->_min_card;
      // Here we define the exact dimension of the coarse cell as the min
      // between the max cardinality given as an input (remember the constructor
      // choice in case of -1) and the dictionary of the boundary cells, it
      // means the total number of neighbourhood cells until the order we have
      // given (as default 3, so until the third order)
      const CoMMAIntType max_ind = std::min(
        this->_max_card, static_cast<CoMMAIntType>(d_n_of_seed.size() + 1));
      // This formula does not work
      CoMMAIntType number_of_external_faces_current_cc =
        this->_fc_graph->get_nb_of_neighbours(seed)
        + this->_fc_graph->get_n_boundary_faces(seed) - 1;
      // d_keys_to_set from Util.h, it takes the keys of the unordered map and
      // create an unordered set. The unordered set is representing hence all
      // the neighbours of seed until a given order.
      const std::unordered_set<CoMMAIndexType> s_neighbours_of_seed =
        d_keys_to_set<CoMMAIndexType, CoMMAIntType>(d_n_of_seed);
      // Build the class neighbourhood
      auto neighbourhood = this->_neigh_crtor->create(
        s_neighbours_of_seed, priority_weights, this->_dimension);
      // Compactness is used when choosing the best cell. We compute it
      // iteratively as the agglomeration advances.
      std::unordered_map<CoMMAIndexType, CoMMAIntType> cur_compact_by_fc{};
      cur_compact_by_fc.reserve(max_ind);
      cur_compact_by_fc[seed] = 0;
      constexpr auto second_less = [](const auto &left, const auto &right) {
        return left.second < right.second;
      };
      CoMMAIndexType next_cell = seed;  // Dummy initialization
      // Choice of the fine cells to agglomerate we enter in a while, we store
      // anyways all the possible coarse cells (not only the max dimension one)
      while (size_current_cc < max_ind) {
        // Update the neighbourhood and generate the candidates
        neighbourhood->update(
          next_cell, this->_fc_graph->get_neighbours(next_cell));
        next_cell = 0;  // Dummy initialization
        CoMMAWeightType min_ar_diam =
          std::numeric_limits<CoMMAWeightType>::max();
        CoMMAWeightType min_ar_vol =
          std::numeric_limits<CoMMAWeightType>::max();
        CoMMAIntType max_faces_in_common = 0;
        // We compute the best fine cell to add, based on the aspect
        // ratio and is given back in next_cell. It takes account also
        // the fine cells that has been added until now.
        // min(max_ind - size_current_cc, this->_fc_iter)
        this->compute_best_fc_to_add(
          std::min(max_ind - size_current_cc, this->_fc_iter), neighbourhood,
          d_n_of_seed, is_order_primary, diam_cc, vol_cc, tmp_cc, next_cell,
          // output
          max_faces_in_common, min_ar_diam, min_ar_vol);

        // This formula does not work
        number_of_external_faces_current_cc +=
          this->_fc_graph->get_nb_of_neighbours(next_cell)
          + this->_fc_graph->get_n_boundary_faces(next_cell) - 1
          - 2 * max_faces_in_common;
        // we increase the cc
        size_current_cc++;
        tmp_cc.insert(next_cell);

        // Update compactness
        CoMMAIntType argmin_compact{0};
        for (auto neigh = this->_fc_graph->neighbours_cbegin(next_cell);
             neigh != this->_fc_graph->neighbours_cend(next_cell); ++neigh) {
          if (tmp_cc.find(*neigh) != tmp_cc.end()) {
            ++argmin_compact;
            ++cur_compact_by_fc[*neigh];
          }
        }
        cur_compact_by_fc[next_cell] = argmin_compact;
        const CoMMAIntType cur_compact =
          min_element(
            cur_compact_by_fc.cbegin(), cur_compact_by_fc.cend(), second_less)
            ->second;
        // Equivalent to:
        // this->_fc_graph->compute_min_fc_compactness_inside_a_cc(tmp_cc);

        // if the constructed cc is eligible, i.e. its size is inside the
        // permitted range we store it inside dict_cc_in_creation This choice is
        // based on the idea that the smallest cc (w.r.t. card) is may be not
        // the one that minimized the number of external faces. If this if is
        // satisfied it means we have an eligible cell
        if (
          (this->_min_card <= size_current_cc) || size_current_cc == max_ind) {
          if (cur_compact > max_compact) {
            max_compact = cur_compact;
            min_external_faces = number_of_external_faces_current_cc;
            arg_min_card = size_current_cc;

          } else if (cur_compact == max_compact) {
            if (
              (number_of_external_faces_current_cc < min_external_faces) ||
              (number_of_external_faces_current_cc == min_external_faces &&
               arg_min_card != this->_goal_card)) {
              min_external_faces = number_of_external_faces_current_cc;
              arg_min_card = size_current_cc;
            }
          }

          // We update the dictionary of eligible coarse cells
          std::unordered_map<CoMMAIndexType, CoMMAIntType> new_dict;
          new_dict[next_cell] = d_n_of_seed[next_cell];
          std::pair<
            std::unordered_set<CoMMAIndexType>,
            std::unordered_map<CoMMAIndexType, CoMMAIntType>>
            tmp_pair = std::make_pair(tmp_cc, new_dict);
          dict_cc_in_creation[size_current_cc] = tmp_pair;
        }

        // Update of diam_cc and vol_cc with the new fc added
        diam_cc = min_ar_diam;
        vol_cc = min_ar_vol;

        // Remove added fc from the available neighbours
        d_n_of_seed.erase(next_cell);
      }

      // Selecting best CC to return
      s_current_cc = std::move(dict_cc_in_creation[arg_min_card].first);

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

      // Updating Seeds Pool with the neighbours of the final CC. Strategy:
      // - Compute the direct neighbours of the CC (not yet agglomerated)
      // - Insert in the queue starting with those of lowest neighbourhood order
      // wrt
      //   to the original seed
      // - If more than one cell with the same order, use priority weights
      const auto cc_neighs = this->_fc_graph->get_neighbourhood_of_cc(
        s_current_cc, this->_cc_graph->_is_fc_agglomerated);
      // Basically, like d_n_of_seed but with key and value swapped
      std::map<CoMMAIntType, std::unordered_set<CoMMAIndexType>>
        neighs_by_order{};
      // For those that were outside max neighbourhood order
      std::unordered_set<CoMMAIndexType> neighs_not_found{};
      for (const auto &neigh : cc_neighs) {
        if (d_n_of_seed.find(neigh) != d_n_of_seed.end())
          neighs_by_order[d_n_of_seed.at(neigh)].insert(neigh);
        else
          neighs_not_found.insert(neigh);
      }
      for (const auto &[o, neighs] : neighs_by_order)
        if (!neighs.empty())
          this->_seeds_pool->order_new_seeds_and_update(neighs);
      if (!neighs_not_found.empty())
        this->_seeds_pool->order_new_seeds_and_update(neighs_not_found);

      assert(arg_min_card == static_cast<CoMMAIntType>(s_current_cc.size()));

      compactness = max_compact;
    }  // end else
    return s_current_cc;
  }

  /** @brief Computes the best fine cell to add to the coarse cell. The choice
   * depends on: the number of shared faces (tries to maximize it), the
   * neighbourhood order (tries to minimize it), and the aspect-ratio of the
   * coarse cell (tries to minimize it).
   *  @param[in] fc_iter (Unused) Number of iteration for the fine cell
   *  research algorithm
   *  @param[in] neighbourhood Neighborhood object
   *  @param[in] d_n_of_seed Dictionary containing the cells to consider for the
   *  agglomeration with their neighbourhood order wrt to the original seed
   *  @param[in] is_order_primary If true, the neighbourhood order prevails on
   * other criteria
   *  @param[in] diam_cc (Approximation of the) Diameter of the current coarse
   * cell
   *  @param[in] vol_cc Volume of the current coarse cell
   *  @param[in] s_of_fc_for_current_cc Indices of the fine cells already
   *  agglomerated in the coarse cell
   *  @param[out] argmin_ar Index wrt to neighbours vector of the chosen fine
   * cell
   *  @param[out] max_faces_in_common Number of faces shared between the chosen
   * fine cell and the coarse cell
   *  @param[out] min_ar_diam (Approximation of the) Diameter of the coarse cell
   *  after the addition of the chosen fine cell
   *  @param[out] min_ar_vol Volume of the coarse cell after the addition of the
   *  chosen fine cell
   */
  virtual void compute_best_fc_to_add(
    const CoMMAIntType fc_iter,
    const std::shared_ptr<
      Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      neighbourhood,
    const std::unordered_map<CoMMAIndexType, CoMMAIntType> &d_n_of_seed,
    const bool &is_order_primary,
    const CoMMAWeightType &diam_cc,
    const CoMMAWeightType &vol_cc,
    const std::unordered_set<CoMMAIndexType> &s_of_fc_for_current_cc,
    CoMMAIndexType &argmin_ar,
    CoMMAIntType &max_faces_in_common,
    CoMMAWeightType &min_ar_diam,
    CoMMAWeightType &min_ar_vol) const {
    CoMMAUnused(fc_iter);
    //  this function defines the best fine cells to add to create the coarse
    // cell for the current coarse cell considered
    CoMMAWeightType min_ar = std::numeric_limits<CoMMAWeightType>::max();
    const auto neighbours = neighbourhood->get_candidates();
    CoMMAIndexType arg_max_faces_in_common = neighbours[0];

    for (const auto &i_fc : neighbours) {
      // we test every possible new cell to chose the one that locally maximizes
      // the number of shared faces and/or minimizes the Aspect Ratio Compute
      // features of the CC obtained by adding i_fc
      CoMMAIntType number_faces_in_common = 0;
      CoMMAWeightType new_ar = std::numeric_limits<CoMMAWeightType>::min();
      CoMMAWeightType new_ar_diam = std::numeric_limits<CoMMAWeightType>::min();
      CoMMAWeightType new_ar_vol = std::numeric_limits<CoMMAWeightType>::min();
      this->compute_next_cc_features(
        i_fc, diam_cc, vol_cc, s_of_fc_for_current_cc,
        // out
        number_faces_in_common, new_ar, new_ar_diam, new_ar_vol);

      // Neighbourhood order of i_fc wrt to original seed of CC
      // [i_fc] is not const the method at returns the reference to the value of
      // the key i_fc.
      const CoMMAIntType &order = d_n_of_seed.at(i_fc);

      // TODO This version seems good but refactorisation to do: perhaps it is
      // not needed to update every new possible coarse cell aspect ratio?
      // TODO also need to remove the list of min_ar, argmin_ar, etc.
      if (
        number_faces_in_common >= max_faces_in_common
        or is_order_primary) {  // if is_order_primary is True the order of
                                // neighbourhood is primary
        if (number_faces_in_common == max_faces_in_common or is_order_primary) {
          // If the number of faces in common is the same, let's see whether
          // it's worth to update or not

          if (order <= d_n_of_seed.at(arg_max_faces_in_common)) {
            // [arg_max_faces_in_common] is not const.
            if (order == d_n_of_seed.at(arg_max_faces_in_common)) {
              if (new_ar < min_ar and number_faces_in_common > 0) {
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

/** @brief Child class of Agglomerator_Isotropic which implements a specialized
 * iterative algorithm for the search of fine cells
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Agglomerator_Iterative :
    public Agglomerator_Biconnected<
      CoMMAIndexType,
      CoMMAWeightType,
      CoMMAIntType> {
public:
  /** @brief Constructor of the class. No specific implementation, it
   * instantiates the base class Agglomerator_Biconnected
   *  @param[in] graph Dual_Graph object that determines the connectivity
   * of the matrix
   *  @param[in] cc_graph Container for the coarse cells
   *  @param[in] seeds_pool Seeds_Pool object giving the order in which the fine
   * cells should be considered when agglomerating
   *  @param[in] neighbourhood_type Type of neighbourhood to use when growing a
   * coarse cell. See \ref CoMMANeighbourhoodT for more details.
   *  @param[in] fc_iter Number of iterations allowed for the algorithm choosing
   * which fine cell to add next. The cost grows exponentially, hence use small
   * values.
   *  @param[in] dimension Dimension of the problem
   */
  Agglomerator_Iterative(
    std::shared_ptr<Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      graph,
    std::shared_ptr<
      Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      cc_graph,
    std::shared_ptr<Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      seeds_pool,
    CoMMAIntType neighbourhood_type,
    CoMMAIntType fc_iter,
    CoMMAIntType dimension = 3) :
      Agglomerator_Biconnected<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
        graph, cc_graph, seeds_pool, neighbourhood_type, fc_iter, dimension) {
    // no particular constructor
  }

  /** @brief Destructor*/
  ~Agglomerator_Iterative() override = default;

  /** @brief Specialization of the parent function. This is an iterative
   * version. Computes the best fine cell to add to the coarse cell. The choice
   * depends on: the number of shared faces (tries to maximize it), the
   * neighbourhood order (tries to minimize it), and the aspect-ratio of the
   * coarse cell (tries to minimize it).
   *  @param[in] fc_iter Number of iteration for the fine-cell research
   * algorithm
   *  @param[in] neighbourhood Neighborhood object
   *  @param[in] d_n_of_seed Dictionary containing the cells to consider for the
   *  agglomeration with their neighbourhood order wrt to the original seed
   *  @param[in] is_order_primary If true, the neighbourhood order prevails on
   * other criteria
   *  @param[in] diam_cc (Approximation of the) Diameter of the current coarse
   * cell
   *  @param[in] vol_cc Volume of the current coarse cell
   *  @param[in] s_of_fc_for_current_cc Indices of the fine cells already
   *  agglomerated in the coarse cell
   *  @param[out] argmin_ar Index wrt to neighbours vector of the chosen fine
   * cell
   *  @param[out] max_faces_in_common Number of faces shared between the chosen
   * fine cell and the coarse cell
   *  @param[out] min_ar_diam (Approximation of the) Diameter of the coarse cell
   *  after the addition of the chosen fine cell
   *  @param[out] min_ar_vol Volume of the coarse cell after the addition of the
   *  chosen fine cell
   */
  void compute_best_fc_to_add(
    const CoMMAIntType fc_iter,
    const std::shared_ptr<
      Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
      neighbourhood,
    const std::unordered_map<CoMMAIndexType, CoMMAIntType> &d_n_of_seed,
    const bool &is_order_primary,
    const CoMMAWeightType &diam_cc,
    const CoMMAWeightType &vol_cc,
    const std::unordered_set<CoMMAIndexType> &s_of_fc_for_current_cc,
    CoMMAIndexType &argmin_ar,
    CoMMAIntType &max_faces_in_common,
    CoMMAWeightType &min_ar_diam,
    CoMMAWeightType &min_ar_vol) const override {
    CoMMAIndexType outer_argmax_faces{0};
    CoMMAIntType ref_max_faces = max_faces_in_common;
    CoMMAWeightType outer_ar = std::numeric_limits<CoMMAWeightType>::max();
    for (const auto &i_fc : neighbourhood->get_candidates()) {
      auto cur_neighbourhood = this->_neigh_crtor->clone(neighbourhood);
      CoMMAWeightType inner_ar{-1.};
      CoMMAIntType inner_max_faces_in_common{0};
      CoMMAWeightType inner_min_ar_diam =
        std::numeric_limits<CoMMAWeightType>::max();
      CoMMAWeightType inner_min_ar_vol{0.};
      this->compute_next_cc_features(
        i_fc, diam_cc, vol_cc, s_of_fc_for_current_cc,
        inner_max_faces_in_common, inner_ar, inner_min_ar_diam,
        inner_min_ar_vol);
      cur_neighbourhood->update(i_fc, this->_fc_graph->get_neighbours(i_fc));
      std::unordered_set<CoMMAIndexType> cur_fc{
        s_of_fc_for_current_cc.begin(), s_of_fc_for_current_cc.end()};
      cur_fc.insert(i_fc);
      // Store value of mother cell
      const CoMMAIntType ref_inner_faces = inner_max_faces_in_common;

      if (fc_iter > 1) {
        CoMMAIndexType cur_argmin{0};
        CoMMAIntType cur_max_faces_in_common{0};
        CoMMAWeightType cur_min_ar_diam =
          std::numeric_limits<CoMMAWeightType>::max();
        CoMMAWeightType cur_min_ar_vol{0.};
        CoMMAWeightType cur_min_ar{0.};
        this->compute_best_fc_to_add(
          fc_iter - 1, cur_neighbourhood, d_n_of_seed, is_order_primary,
          inner_min_ar_diam, inner_min_ar_vol, cur_fc,
          // output
          cur_argmin, cur_max_faces_in_common, cur_min_ar_diam, cur_min_ar_vol);
        // We just keep the min AR and the max faces in common
        if (cur_max_faces_in_common > inner_max_faces_in_common) {
          inner_max_faces_in_common = cur_max_faces_in_common;
        } else if (
          cur_max_faces_in_common == inner_max_faces_in_common
          && cur_min_ar < inner_ar) {
          inner_ar = cur_min_ar;
        }
      }

      const CoMMAIntType &order = d_n_of_seed.at(i_fc);

      // TODO This version seems good but refactorisation to do: perhaps it is
      // not needed to update every new possible coarse cell aspect ratio?
      // TODO also need to remove the list of min_ar, argmin_ar, etc.
      if (
        inner_max_faces_in_common >= ref_max_faces
        or is_order_primary) {  // if is_order_primary is True the order of
                                // neighbourhood is primary
        if (inner_max_faces_in_common == ref_max_faces or is_order_primary) {
          // If the number of faces in common is the same, let's see whether
          // it's worth to update or not

          if (order <= d_n_of_seed.at(outer_argmax_faces)) {
            // [outer_argmax_faces] is not const.
            if (order == d_n_of_seed.at(outer_argmax_faces)) {
              if (inner_ar < outer_ar and inner_max_faces_in_common > 0) {
                // The second condition asserts the connectivity of the coarse
                // element.
                argmin_ar = i_fc;
                // Outer AR is the min AR of the children, but since diameter
                // and volume are used in the next step, we keep those of the
                // mother cell...
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
              // volume are used in the next step, we keep those of the mother
              // cell...
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
          // volume are used in the next step, we keep those of the mother
          // cell...
          outer_ar = inner_ar;
          min_ar_diam = inner_min_ar_diam;
          min_ar_vol = inner_min_ar_vol;
          // ... same for faces in common
          max_faces_in_common = ref_inner_faces;
        }
      }
    }  // for i_fc
  }
};

}  // end namespace comma

#endif  // COMMA_PROJECT_AGGLOMERATOR_H
