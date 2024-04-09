#ifndef COMMA_PROJECT_PARAMS_H
#define COMMA_PROJECT_PARAMS_H

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

#include <optional>
#include <vector>

#include "CoMMA/CoMMADefs.h"

namespace comma {

/** @brief Convenient class holding arguments defining the graph
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class GraphArgs {
public:
  /** @brief Indices of the CSR representation of the graph */
  const std::vector<CoMMAIndexType> &connectivity_indices;
  /** @brief Values of the CSR representation of the graph */
  const std::vector<CoMMAIndexType> &connectivity;
  /** @brief Weights of the CSR representation of the graph */
  const std::vector<CoMMAWeightType> &connectivity_weights;
  /** @brief Volumes of the cells */
  const std::vector<CoMMAWeightType> &volumes;
  /** @brief Centers of the cells */
  const std::vector<std::vector<CoMMAWeightType>> &centers;
  /** @brief Priority weights */
  const std::vector<CoMMAWeightType> &priority_weights;
  /** @brief Number of boundary faces per cell */
  const std::vector<CoMMAIntType> &n_bnd_faces;
  /** @brief Dimensionality of the problem, 2- or 3D */
  CoMMAIntType dimension;

  /** @brief Default constructor */
  GraphArgs() = default;

  /** @brief Constructor
   * @param[in] connectivity_indices Indices of the CSR representation of the
   * graph.
   * @param[in] connectivity Values of the CSR representation of the graph.
   * @param[in] connectivity_weights Weights of the CSR representation of the
   * graph.
   * @param[in] volumes Volumes of the cells.
   * @param[in] centers Centers of the cells.
   * @param[in] priority_weights Priority weights.
   * @param[in] n_bnd_faces Number of boundary faces per cell.
   * @param[in] dimension Dimensionality of the problem, 2- or 3D.
   */
  GraphArgs(
    const std::vector<CoMMAIndexType> &connectivity_indices,
    const std::vector<CoMMAIndexType> &connectivity,
    const std::vector<CoMMAWeightType> &connectivity_weights,
    const std::vector<CoMMAWeightType> &volumes,
    const std::vector<std::vector<CoMMAWeightType>> &centers,
    const std::vector<CoMMAWeightType> &priority_weights,
    const std::vector<CoMMAIntType> &n_bnd_faces,
    CoMMAIntType dimension
  ) :
    connectivity_indices(connectivity_indices),
    connectivity(connectivity),
    connectivity_weights(connectivity_weights),
    volumes(volumes),
    centers(centers),
    priority_weights(priority_weights),
    n_bnd_faces(n_bnd_faces),
    dimension(dimension) {}
};

/** @brief Convenient class holding arguments for the parametrization of the
 * agglomeration algorithm.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class AgglomerationArgs {
public:
  /** @brief Desired cardinality of the coarse cells */
  CoMMAIntType goal_card;
  /** @brief Minimum cardinality accepted for the coarse cells */
  CoMMAIntType min_card;
  /** @brief Maximum cardinality accepted for the coarse cells */
  CoMMAIntType max_card;
  /** @brief Whether to apply correction step (avoid isolated cells)
   * after agglomeration
   */
  bool correction{};  // clang-tidy complains without {}
  /** @brief Type of ordering for the seeds of the coarse cells (see \ref
   * CoMMASeedsPoolT)
   */
  CoMMASeedsPoolT seed_ordering_type{};
  /** @brief Cardinality below which a coarse is considered as singular, hence,
   * compliant for correction */
  CoMMAIntType singular_card_thresh;
  /** @brief Number of iterations allowed for the algorithm choosing which fine
   * cell to add next. The cost grows exponentially, hence use small values.
   */
  CoMMAIntType fc_choice_iter;
  /** @brief Type of neighbourhood to use when growing a coarse cell. See \ref
   * CoMMANeighbourhoodT for more details.
   */
  CoMMANeighbourhoodT neighbourhood_type{};

  /** @brief Default constructor */
  AgglomerationArgs() = default;

  /** @brief Constructor
   * @param[in] goal_card Desired cardinality of the coarse cells (might not be
   * ensured)
   * @param[in] min_card Minimum cardinality accepted for the coarse cells
   * @param[in] max_card Maximum cardinality accepted for the coarse cells
   * @param[in] correction Whether to apply correction step (avoid isolated
   * cells) after agglomeration
   * @param[in] seed_ordering_type Type of ordering for the seeds of the coarse
   * cells (see \ref CoMMASeedsPoolT):
   * @param[in] singular_card_thresh (optional, default=1) Cardinality below
   * which a coarse is considered as singular, hence, compliant for correction
   * @param[in] fc_choice_iter (optional, default=1) Number of iterations
   * allowed for the algorithm choosing which fine cell to add next. The cost
   * grows exponentially, hence use small values.
   * @param[in] neighbourhood_type (optional, default=Extended) Type of
   * neighbourhood to use when growing a coarse cell, see \ref
   * CoMMANeighbourhoodT for more details.
   */
  AgglomerationArgs(
    CoMMAIntType goal_card,
    CoMMAIntType min_card,
    CoMMAIntType max_card,
    bool correction,
    CoMMASeedsPoolT seed_ordering_type,
    CoMMAIntType singular_card_thresh = 1,
    CoMMAIntType fc_choice_iter = 1,
    CoMMANeighbourhoodT neighbourhood_type = CoMMANeighbourhoodT::EXTENDED
  ) :
    goal_card(goal_card),
    min_card(min_card),
    max_card(max_card),
    correction(correction),
    seed_ordering_type(seed_ordering_type),
    singular_card_thresh(singular_card_thresh),
    fc_choice_iter(fc_choice_iter),
    neighbourhood_type(neighbourhood_type) {}
};

/** @brief Convenient class holding arguments for the parametrization of the
 * anisotropic agglomeration algorithm.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class AnisotropicArgs {
public:
  /** @brief Whether to consider an anisotropic agglomeration */
  bool is_anisotropic;
  /** @brief List of cells which have to be looked for anisotropy */
  const std::vector<CoMMAIndexType> &anisotropicCompliantCells;
  /** @brief Whether lines joining the anisotropic cells should be built */
  bool build_lines;
  /** @brief  Whether anisotropic lines with odd length are allowed */
  bool odd_line_length;
  /** @brief Value of the aspect-ratio above which a cell is considered as
   * anisotropic
   */
  CoMMAWeightType threshold_anisotropy;
  /** @brief Maximum number of cells in an anisotropic line */
  std::optional<CoMMAIndexType> max_cells_in_line = std::nullopt;

  /** @brief Constructor
   * @param[in] is_anisotropic Whether to consider an anisotropic agglomeration.
   * @param[in] anisotropicCompliantCells List of cells which have to be looked
   * for anisotropy.
   * @param[in] build_lines Whether lines joining the anisotropic cells should
   * be built. Default: true.
   * @param[in] odd_line_length Whether anisotropic lines with odd length are
   * allowed. Default: true.
   * @param[in] threshold_anisotropy Value of the aspect-ratio above which a
   * cell is considered as anisotropic. Default: 4.0
   * @param[in] max_cells_in_line Maximum number of cells in an anisotropic
   * line. Default: no limit.
   */
  AnisotropicArgs(
    bool is_anisotropic,
    const std::vector<CoMMAIndexType> &anisotropicCompliantCells,
    bool build_lines = true,
    bool odd_line_length = true,
    CoMMAWeightType threshold_anisotropy = 4.,
    std::optional<CoMMAIndexType> max_cells_in_line = std::nullopt
  ) :
    is_anisotropic(is_anisotropic),
    anisotropicCompliantCells(anisotropicCompliantCells),
    build_lines(build_lines),
    odd_line_length(odd_line_length),
    threshold_anisotropy(threshold_anisotropy),
    max_cells_in_line(max_cells_in_line) {}
};

}  // end namespace comma

#endif
