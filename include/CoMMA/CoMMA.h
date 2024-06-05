#ifndef COMMA_PROJECT_COMMA_H
#define COMMA_PROJECT_COMMA_H

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

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#include "CoMMA/Agglomerator.h"
#include "CoMMA/Args.h"
#include "CoMMA/CoMMADefs.h"
#include "CoMMA/Coarse_Cell_Container.h"
#include "CoMMA/Dual_Graph.h"
#include "CoMMA/Seeds_Pool.h"

namespace comma {

/// \cond DO_NOT_DOCUMENT
#define CHECK_INT_TYPE(intT, label)                                    \
  static_assert(                                                       \
    std::numeric_limits<intT>::is_integer,                             \
    "CoMMA works with integer types, but " #intT " (" label ") is not" \
  )
/// \endcond

/** @brief Maximum allowed iterations for the iterative algorithm, see
 * \ref Agglomerator_Iterative
 */
constexpr CoMMAIntT iter_agglo_max_iter = 4;

/** @brief Main function of the agglomerator, it is used as an interface
 * to build up all the agglomeration process. The result will be the definition
 * of the agglomerated cells \p fc_to_cc.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 * @param[in] adjMatrix_row_ptr the row pointer of the CRS representation
 * @param[in] adjMatrix_col_ind the column index of the CRS representation
 * @param[in] adjMatrix_areaValues the weight of the CRS representation (in
 * CoMMA case will be the area of the faces that in the graph representation are
 * the edges between two nodes represented by the cell centers.
 * @param[in] volumes The volumes of the cells
 * @param[in] centers Cell centers
 * @param[in] priority_weights Weights used to set the order telling where to
 * start agglomerating. The higher the weight, the higher the priority
 * @param[in] anisotropicCompliantCells List of cells which have to be looked
 * for anisotropy
 * @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
 * @param[in] build_anisotropic_lines Whether lines joining the anisotropic
 * cells should be built, otherwise, if the anisotropic agglomeration is
 * activated, the lines should be provided, see \p agglomerationLines_Idx and \p
 * agglomerationLines
 * @param[in] is_anisotropic Whether to consider an anisotropic agglomeration
 * @param[in] odd_line_length Whether anisotropic lines with odd length are
 * allowed
 * @param[in] threshold_anisotropy Value of the aspect-ratio above which a cell
 * is considered as anisotropic. If negative, all compliant cells are considered
 * as anisotropic
 * @param[in] seed_ordering_type Type of ordering for the seeds of the coarse
 * cells. Possible values (see \ref CoMMASeedsPoolT):
 * - 0: The number of boundary faces has highest priority
 * - 1: The neighbourhood has highest priority (neighbours of coarse cells have
 *      priority)
 * - 10: The number of boundary faces has highest priority, and initialize with
 * one point only then let evolve
 * - 11: The neighbourhood has highest priority, and initialize with one point
 * only then let evolve
 * @param[out] fc_to_cc Vector telling the ID of the coarse cell to which a fine
 * cell belongs after agglomeration. The vector is cleared and then resized to
 * the right size.
 * @param[in,out] agglomerationLines_Idx Connectivity for the agglomeration
 * lines: each element points to a particular element in the vector \p
 * agglomerationLines. The vector is cleared and then resized to the right size.
 * @param[in,out] agglomerationLines Vector storing all the elements of the
 * anisotropic lines. The vector is cleared and then resized to the right size.
 * @param[in] correction Whether to apply correction step (avoid isolated cells)
 * after agglomeration
 * @param[in] dimension Dimensionality of the problem, 2- or 3D
 * @param[in] goal_card Desired cardinality of the coarse cells (might not be
 * ensured)
 * @param[in] min_card Minimum cardinality accepted for the coarse cells
 * @param[in] max_card Maximum cardinality accepted for the coarse cells
 * @param[in] aspect_ratio Type of aspect-ratio (see \ref CoMMAAspectRatioT)
 * @param[in] singular_card_thresh (optional, default=1) Cardinality below which
 * a coarse is considered as singular, hence, compliant for correction
 * @param[in] max_cells_in_line [Optional] Maximum number of cells in an
 * anisotropic line; when this value is reached, all reaming cells are
 * discarded, hence considered as isotropic
 * @param[in] fc_choice_iter (optional, default=1) Number of iterations allowed
 * for the algorithm choosing which fine cell to add next. The cost grows
 * exponentially, hence use small values.
 * @param[in] neighbourhood_type (optional, default=Extended) Type of
 * neighbourhood to use when growing a coarse cell. See \ref CoMMANeighbourhoodT
 * for more details. Two alternatives:
 * - Extended: requested with 0, standard algorithm where we consider every
 * neighbour of the coarse cell as candidate.
 * - Pure Front Advancing: requested with 1, only direct neighbours of the last
 * added cell are candidates.
 * @throw invalid_argument whenever dimension is not 2 nor 3, cardinalities are
 * smaller than 1 or not in order, line building is disabled but lines are not
 * provided, or number of iterations is negative or greater than
 * \ref iter_agglo_max_iter.
 *
 * @copyright Copyright © 2024 ONERA
 * @author Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * @contributor Karim Anemiche
 * @license This project is released under the Mozilla Public License 2.0, see
 * https://mozilla.org/MPL/2.0/
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
void agglomerate_one_level(
  // Dual graph:
  const std::vector<CoMMAIndexType> &adjMatrix_row_ptr,
  const std::vector<CoMMAIndexType> &adjMatrix_col_ind,
  const std::vector<CoMMAWeightType> &adjMatrix_areaValues,
  const std::vector<CoMMAWeightType> &volumes,

  // Additional info about the mesh
  const std::vector<std::vector<CoMMAWeightType>> &centers,
  const std::vector<CoMMAWeightType> &priority_weights,
  const std::vector<CoMMAIndexType> &anisotropicCompliantCells,
  const std::vector<CoMMAIntType> &n_bnd_faces,

  // Anisotropy related info
  bool build_anisotropic_lines,
  bool is_anisotropic,
  bool odd_line_length,
  CoMMAWeightType threshold_anisotropy,

  // Seed ordering
  CoMMASeedsPoolT seed_ordering_type,

  // Outputs
  std::vector<CoMMAIndexType> &fc_to_cc,  // Out
  std::vector<CoMMAIndexType> &agglomerationLines_Idx,  // In & out
  std::vector<CoMMAIndexType> &agglomerationLines,  // In & out

  // Tuning of the algorithms
  bool correction,
  CoMMAIntType dimension,
  CoMMAIntType goal_card,
  CoMMAIntType min_card,
  CoMMAIntType max_card,
  CoMMAAspectRatioT aspect_ratio = CoMMAAspectRatioT::DIAMETER_OVER_RADIUS,
  CoMMAIntType singular_card_thresh = 1,
  std::optional<CoMMAIndexType> max_cells_in_line = std::nullopt,
  CoMMAIntType fc_choice_iter = 1,
  CoMMANeighbourhoodT neighbourhood_type = CoMMANeighbourhoodT::EXTENDED
) {
  // NOTATION
  //======================================
  // fc = Fine Cell
  // cc = Coarse Cell

  // USEFUL SHORTCUTS
  //======================================
  using SeedsPoolType =
    Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  using DualGraphType =
    Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  using CCContainerType =
    Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  using IsotropicPtr = std::unique_ptr<
    Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>;

  // SANITY CHECKS
  //======================================
  CHECK_INT_TYPE(CoMMAIndexType, "first template argument");
  CHECK_INT_TYPE(CoMMAIntType, "third template argument");
  if (!(dimension == 2 || dimension == 3))
    throw std::invalid_argument("CoMMA - Error: dimension must be 2 or 3");
  if (min_card <= 1 || goal_card <= 1 || max_card <= 1)
    throw std::invalid_argument(
      "CoMMA - Error: Cardinalities must be greater than 1"
    );
  if (!(min_card <= goal_card && goal_card <= max_card))
    throw std::invalid_argument(
      "CoMMA - Error: Cardinalities must be in order (min <= goal <= max)"
    );
  if (fc_choice_iter < 1)
    throw std::invalid_argument(
      "CoMMA - Error: the number of iteration for the choice of the fine "
      "cells must be at least 1"
    );
  if (fc_choice_iter > iter_agglo_max_iter)
    throw std::invalid_argument(
      "CoMMA - Error: the number of iteration for the choice of the fine "
      "cells must be at most "
      + std::to_string(iter_agglo_max_iter)
    );
  if (adjMatrix_row_ptr.empty()
      || adjMatrix_row_ptr.back()
        != static_cast<CoMMAIndexType>(adjMatrix_col_ind.size())
      || adjMatrix_row_ptr.back()
        != static_cast<CoMMAIndexType>(adjMatrix_areaValues.size()))
    throw std::invalid_argument(
      "CoMMA - Error: bad CRS graph (sizes do not match)"
    );
  if (is_anisotropic) {
    if (build_anisotropic_lines) {
      if (anisotropicCompliantCells.empty()) {
        std::cout << "CoMMA - Warning: building anisotropic line requested, no "
                     "compliant cells provided. Switching off anisotropy."
                  << std::endl;
      }
      if (max_cells_in_line.has_value() && max_cells_in_line.value() <= 0) {
        std::cout << "CoMMA - Requested a negative or null maximum number of "
                     "cells in line. Dropping the limit."
                  << std::endl;
        max_cells_in_line = std::nullopt;
      }
    } else {
      // Anisotropic lines are provided
      if (agglomerationLines_Idx.size() < 2 || agglomerationLines.empty()) {
        std::cout
          << "CoMMA - Warning: usage of input anisotropic line requested, "
             "but arguments are not enough / invalid to define them. "
             "Switching off anisotropy."
          << std::endl;
        is_anisotropic = false;
      } else if (agglomerationLines_Idx.back()
                 != static_cast<CoMMAIndexType>(agglomerationLines.size())) {
        throw std::invalid_argument(
          "CoMMA - Error: bad anisotropic lines definition (sizes do not "
          "match)"
        );
      }
    }
  }
  auto sing_thresh = singular_card_thresh;
  if (singular_card_thresh <= 0) {
    throw std::invalid_argument(
      "CoMMA - Error: Threshold cardinality for singular cells should be "
      "greater than zero"
    );
  }
  if (singular_card_thresh >= min_card) {
    std::cout
      << "CoMMA - Warning: Threshold cardinality is equal or larger than "
         "minimum cardinality. Changing it to this latter value."
      << std::endl;
    sing_thresh = min_card - 1;
  }

  // SIZES CAST
  //======================================
  const auto nb_fc = static_cast<CoMMAIndexType>(adjMatrix_row_ptr.size() - 1);

  // BOUNDARY FACES
  //======================================
  // Sometimes partitioners give a number of boundary faces higher than the
  // physical one. We fix this
  const CoMMAIntType expected_max_n_bnd = dimension;
  std::vector<CoMMAIntType> fixed_n_bnd_faces(n_bnd_faces.size());
  std::replace_copy_if(
    n_bnd_faces.begin(),
    n_bnd_faces.end(),
    fixed_n_bnd_faces.begin(),
    [expected_max_n_bnd](auto n) { return n > expected_max_n_bnd; },
    expected_max_n_bnd
  );

  // SEED POOL
  //======================================
  // Object providing the order of agglomeration
  std::shared_ptr<SeedsPoolType> seeds_pool = nullptr;
  switch (seed_ordering_type) {
    case CoMMASeedsPoolT::BOUNDARY_PRIORITY:
      seeds_pool = std::make_shared<Seeds_Pool_Boundary_Priority<
        CoMMAIndexType,
        CoMMAWeightType,
        CoMMAIntType>>(fixed_n_bnd_faces, priority_weights, false);
      break;
    case CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY:
      seeds_pool = std::make_shared<Seeds_Pool_Neighbourhood_Priority<
        CoMMAIndexType,
        CoMMAWeightType,
        CoMMAIntType>>(fixed_n_bnd_faces, priority_weights, false);
      break;
    case CoMMASeedsPoolT::BOUNDARY_PRIORITY_ONE_POINT_INIT:
      seeds_pool = std::make_shared<Seeds_Pool_Boundary_Priority<
        CoMMAIndexType,
        CoMMAWeightType,
        CoMMAIntType>>(fixed_n_bnd_faces, priority_weights, true);
      break;
    case CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY_ONE_POINT_INIT:
      seeds_pool = std::make_shared<Seeds_Pool_Neighbourhood_Priority<
        CoMMAIndexType,
        CoMMAWeightType,
        CoMMAIntType>>(fixed_n_bnd_faces, priority_weights, true);
      break;
    default:
      throw std::invalid_argument("CoMMA - Error: Seeds pool type unsupported");
  }

  // DUAL GRAPH
  //======================================
  // Object containing the graph representation and related info in a convenient
  // structure
  std::shared_ptr<DualGraphType> fc_graph = std::make_shared<DualGraphType>(
    nb_fc,
    adjMatrix_row_ptr,
    adjMatrix_col_ind,
    adjMatrix_areaValues,
    volumes,
    centers,
    fixed_n_bnd_faces,
    dimension,
    anisotropicCompliantCells
  );

  // COARSE CELL CONTAINER
  //======================================
  // Preparing the object that will contain all the coarse cells
  std::shared_ptr<CCContainerType> cc_graph =
    std::make_shared<CCContainerType>(fc_graph, sing_thresh);

  // AGGLOMERATION OF ANISOTROPIC CELLS
  //======================================
  // @todo maybe re-refactor the class agglomerator to allow the implicit upcast
  // like the biconnected case
  // The agglomerator anisotropic is not called with the implicit upcasting
  // pointing because of the initialization of
  // the anisotropic lines.
  // for more information look at:
  // https://stackoverflow.com/questions/19682402/initialize-child-object-on-a-pointer-to-parent
  // About constructors when upcasting:
  // https://www.reddit.com/r/learnprogramming/comments/1wopf6/java_which_constructor_is_called_when_upcasting/
  if (is_anisotropic) {
    // Build anisotropic agglomerator
    Agglomerator_Anisotropic<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>
      aniso_agg(
        fc_graph,
        cc_graph,
        seeds_pool,
        dimension,
        threshold_anisotropy,
        agglomerationLines_Idx,
        agglomerationLines,
        priority_weights,
        build_anisotropic_lines,
        odd_line_length,
        max_cells_in_line
      );

    // Agglomerate anisotropic cells only
    aniso_agg.agglomerate_one_level(
      goal_card, min_card, max_card, priority_weights, false
    );

    // Put anisotropic lines into the output parameters
    // (Info about level of the line: WARNING! here 1 it means that we give it
    // back lines in the new global index, 0 the old)
    aniso_agg.export_anisotropic_lines(
      1, agglomerationLines_Idx, agglomerationLines
    );
  } else {
    seeds_pool->initialize();
  }

  // AGGLOMERATION OF ISOTROPIC CELLS
  //======================================
  // We define here the type of Agglomerator
  IsotropicPtr agg = nullptr;
  // TODO: maybe pass to a switch when another agglomerator will be implemented
  if (fc_choice_iter > 1) {
    agg = std::make_unique<
      Agglomerator_Iterative<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
      fc_graph,
      cc_graph,
      seeds_pool,
      dimension,
      aspect_ratio,
      neighbourhood_type,
      fc_choice_iter
    );
  } else {
    agg = std::make_unique<
      Agglomerator_Biconnected<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
      fc_graph,
      cc_graph,
      seeds_pool,
      dimension,
      aspect_ratio,
      neighbourhood_type,
      fc_choice_iter
    );
  }
  // Agglomerate
  agg->agglomerate_one_level(
    goal_card, min_card, max_card, priority_weights, correction
  );
  // FILLING FC TO CC (it is a property of the cc_graph but retrieved through an
  // helper of the agglomerator)
  fc_to_cc.clear();
  fc_to_cc.reserve(cc_graph->_fc_2_cc.size());
  std::transform(
    cc_graph->_fc_2_cc.begin(),
    cc_graph->_fc_2_cc.end(),
    std::back_inserter(fc_to_cc),
    [](const auto &f2c) { return f2c.value(); }
  );
}

/** @brief Main function of the agglomerator, it is used as an interface
 * to build up all the agglomeration process. The result will be the definition
 * of the agglomerated cells \p fc_to_cc.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 * @param[in] graph Definition of the graph, see GraphArgs.
 * @param[in] agglo Parametrization of the isotropic agglomeration algorithm,
 * see AgglomerationArgs.
 * @param[in] aniso Parametrization of the anisotropic agglomeration algorithm,
 * see AnisotropicArgs.
 * @param[out] fc_to_cc Vector telling the ID of the coarse cell to which a fine
 * cell belongs after agglomeration.
 * @param[in,out] aniso_lines_indices Connectivity for the agglomeration lines
 * @param[in,out] aniso_lines Vector storing all the elements of the
 * anisotropic lines
 *
 * @note This version is just a wrapper around the other agglomerate_one_level
 * @copyright Copyright © 2024 ONERA
 * @author Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * @contributor Karim Anemiche
 * @license This project is released under the Mozilla Public License 2.0, see
 * https://mozilla.org/MPL/2.0/
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
inline void agglomerate_one_level(
  const GraphArgs<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &graph,
  const AgglomerationArgs<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &agglo,
  const AnisotropicArgs<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &aniso,
  std::vector<CoMMAIndexType> &fc_to_cc,  // Out
  std::vector<CoMMAIndexType> &aniso_lines_indices,  // In & out
  std::vector<CoMMAIndexType> &aniso_lines  // In & out
) {
  agglomerate_one_level<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
    graph.connectivity_indices,
    graph.connectivity,
    graph.connectivity_weights,
    graph.volumes,
    graph.centers,
    graph.priority_weights,
    aniso.anisotropicCompliantCells,
    graph.n_bnd_faces,
    aniso.build_lines,
    aniso.is_anisotropic,
    aniso.odd_line_length,
    aniso.threshold_anisotropy,
    agglo.seed_ordering_type,
    fc_to_cc,
    aniso_lines_indices,
    aniso_lines,
    agglo.correction,
    graph.dimension,
    agglo.goal_card,
    agglo.min_card,
    agglo.max_card,
    agglo.aspect_ratio,
    agglo.singular_card_thresh,
    aniso.max_cells_in_line,
    agglo.fc_choice_iter,
    agglo.neighbourhood_type
  );
}

#undef CHECK_INT_TYPE

}  // end namespace comma

#endif
