#ifndef COMMA_PROJECT_COMMA_TYPES_H
#define COMMA_PROJECT_COMMA_TYPES_H

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

/// Type of an index (used by agglomerator CoMMA for edges and nodes).
using CoMMAIndexT = long;

/// Type of floating-point values (used by agglomerator CoMMA for this edge
/// weights).
using CoMMAWeightT = double;

/// Type of integer for cardinality of coarse cells, dimension, distances,
/// orders...
using CoMMAIntT = int;

/** @brief Type of an element according to its boundary faces / edges
 *  The terms come from the NIA paper: Nishikawa, Diskin, Thomas...
 *  to minimizes the too small cells!
 *  0 : interior (no face on the edge of the domain)
 *  1 : valley (one face on the edge of the domain)
 *  2 : ridge (two faces on the edge of the domain)
 *  3 : corner (three faces on the edge of the domain)
 */
enum CoMMACellT : CoMMAIntT {
  INTERIOR     = 0,
  VALLEY       = 1,
  RIDGE        = 2,
  CORNER       = 3,
  EXTREME      = 4,
  N_CELL_TYPES = 4
};

/** @brief Type of Isotropic Agglomerator
 *  0 : Biconnected from Mavriplis Paper
 *  1 : Pure front where a pure front advancing method is implemented
 */
enum CoMMAAgglT : CoMMAIntT {
  BICONNECTED     = 0,
  PURE_FRONT      = 1,
};

#endif
