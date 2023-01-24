#ifndef COMMA_PROJECT_COMMA_TYPES_H
#define COMMA_PROJECT_COMMA_TYPES_H

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

/// Type of an index (used by agglomerator CoMMA for edges and nodes).
using CoMMAIndexT = unsigned long;

/// Type of floating-point values (used by agglomerator CoMMA for this edge
/// weights).
using CoMMAWeightT = double;

/// Type of integer for cardinality of coarse cells, dimension, distances,
/// orders...
using CoMMAIntT = int;

/** @brief Type of an element according to its boundary faces / edges
 *  The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
 */
enum CoMMACellT : CoMMAIntT {
  INTERIOR     = 0, ///< Interior cell, no boundary faces
  VALLEY       = 1, ///< Valley, one boundary face
  RIDGE        = 2, ///< Ridge, two boundary faces
  CORNER       = 3, ///< Corners, three boundary faces
  EXTREME      = 4, ///< Extreme value, should not be used

  N_CELL_TYPES = 4  ///< Total number of values
};

/** @brief Type of neighbourhood (of a coarse cell) considered when agglomerating
 */
enum CoMMANeighbourhoodT : CoMMAIntT {
  EXTENDED   = 0, ///< Extended, all neighbours of the coarse cell
  PURE_FRONT = 1  ///< Pure front, only neighbours of the last added fine cell
};

/** @brief Type of seeds pool ordering */
enum CoMMASeedsPoolT : CoMMAIntT {
  /** The number of boundary faces has highest priority */
  BOUNDARY_PRIORITY                     = 0,
  /** The neighbourhood has highest priority (neighbours of coarse cells have
   * priority)
   */
  NEIGHBOURHOOD_PRIORITY                = 1,
  /** The number of boundary faces has highest priority, and initialize with one
   * point only then let evolve
   */
  BOUNDARY_PRIORITY_ONE_POINT_INIT      = 10,
  /** The neighbourhood has highest priority, and initialize with one point only
   * then let evolve
   */
  NEIGHBOURHOOD_PRIORITY_ONE_POINT_INIT = 11
};

#endif
