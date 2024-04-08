#ifndef COMMA_PROJECT_COMMA_TYPES_H
#define COMMA_PROJECT_COMMA_TYPES_H

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

#include "CoMMA/CoMMAConfig.h"

namespace comma {

/** @brief Type of an element according to its boundary faces / edges
 *  The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
 */
enum CoMMACellT : CoMMAIntT {
  INTERIOR = 0,  ///< Interior cell, no boundary faces
  VALLEY = 1,  ///< Valley, one boundary face
  RIDGE = 2,  ///< Ridge, two boundary faces
  CORNER = 3,  ///< Corners, three boundary faces
  EXTREME = 4,  ///< Extreme value, should not be used

  N_CELL_TYPES = 4  ///< Total number of values
};

/** @brief Type of neighbourhood (of a coarse cell) considered when
 * agglomerating
 */
enum CoMMANeighbourhoodT : CoMMAIntT {
  EXTENDED = 0,  ///< Extended, all neighbours of the coarse cell
  PURE_FRONT = 1  ///< Pure front, only neighbours of the last added fine cell
};

/** @brief Type of seeds pool ordering */
enum CoMMASeedsPoolT : CoMMAIntT {
  /** The number of boundary faces has highest priority */
  BOUNDARY_PRIORITY = 0,
  /** The neighbourhood has highest priority (neighbours of coarse cells have
   * priority)
   */
  NEIGHBOURHOOD_PRIORITY = 1,
  /** The number of boundary faces has highest priority, and initialize with one
   * point only then let evolve
   */
  BOUNDARY_PRIORITY_ONE_POINT_INIT = 10,
  /** The neighbourhood has highest priority, and initialize with one point only
   * then let evolve
   */
  NEIGHBOURHOOD_PRIORITY_ONE_POINT_INIT = 11
};

/** @brief Type of aspect-ratio */
enum CoMMAAspectRatioT : CoMMAIntT {
  /** @brief  */
  DIAMETER_OVER_RADIUS = 0,
  /** @brief  */
  DIAMETER_OVER_MIN_EDGE,
  /** @brief  */
  DIAMETER,
  /** @brief  */
  ONE_OVER_MEASURE,
  /** @brief  */
  ONE_OVER_INTERNAL_WEIGHTS,
  /** @brief  */
  PERIMETER_OVER_RADIUS,
  /** @brief  */
  EXTERNAL_WEIGHTS,
  /** @brief  */
  MAX_BARY_DIST_OVER_RADIUS,
  /** @brief  */
  MAX_OVER_MIN_BARY_DIST,
  /** @brief  */
  ALGEBRAIC_PERIMETER_OVER_MEASURE,
};

}  // end namespace comma

#endif
