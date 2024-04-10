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

/** @brief Type of aspect-ratio. Notation:
 * - \f$ fc \f$ = Fine Cell, \f$ CC \f$ = Coarse cell = \f$ \{fc_i\}_i \f$:\n
 * - Distance: \f$ d(x,y) \f$
 * - Measure: in 3D, volume of the CC, \f$ vol(CC) \f$\n
 * - Radius: approximation of the characteristic length
 *   \f$ \rho(CC) = \sqrt[dim]{vol(CC)} \f$\n
 * - Barycenter: \f$ x^b_{CC} = \sum_{fc \in CC} vol_{fc} x^b_{fc} \f$\n
 * - Diameter:
 *   \f$ diam = max_{fc_i, fc_j \in CC} d(x^b_{fc_i}, x^b_{fc_j}) \f$\n
 * - Perimeter: external weights, \f$ \pi(CC) \f$\n
 * - Internal weights: sum of the surfaces of the internal facets,
 *   \f$ \sigma(CC) \f$
 */
enum CoMMAAspectRatioT : CoMMAIntT {
  /** Diameter over radius, \f$ AR = \frac{diam(CC)}{\rho(CC)} \f$ */
  DIAMETER_OVER_RADIUS = 0,
  /** Diameter over minimum edge (\f$ =min_{fc_i, fc_j \in CC}
   * d(x^b_{fc_i}, x^b_{fc_j}) \f$)
   */
  DIAMETER_OVER_MIN_EDGE,
  /** Diameter, \f$ AR = diam(CC) \f$ */
  DIAMETER,
  /** One over the measure (e.g., volume) of the cell,
   * \f$ AR = \frac{1}{vol(CC)} \f$
   */
  ONE_OVER_MEASURE,
  /** One over the internal weights, \f$ AR = \frac{1}{\sigma(CC)} \f$ */
  ONE_OVER_INTERNAL_WEIGHTS,
  /** Perimeter over radius, \f$ AR = \frac{\pi(CC)}{\rho(CC)} \f$ */
  PERIMETER_OVER_RADIUS,
  /** External weights, that is, perimeter, \f$ AR = \pi(CC) \f$ */
  EXTERNAL_WEIGHTS,
  /** Maximum FC-center distance from barycenter over radius */
  MAX_BARY_DIST_OVER_RADIUS,
  /** Maximum over minimum FC-center distance from barycenter */
  MAX_OVER_MIN_BARY_DIST,
  /** Algebraic-like perimeter over measure, that is, external weights over
   * cell weight
   */
  ALGEBRAIC_PERIMETER_OVER_MEASURE,
};

}  // end namespace comma

#endif
