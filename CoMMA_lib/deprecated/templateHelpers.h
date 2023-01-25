#ifndef COMMA_PROJECT_TEMPLATE_HELPERS_H
#define COMMA_PROJECT_TEMPLATE_HELPERS_H

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

/** @brief Assign from type to another only if a compile-time condition is verified
 *  @tparam condition Boolean telling if the assignment should be done. It has to be
 *  a \p constexpr
 *  @tparam typeA Type of the destination of the assignment
 *  @tparam typeB Type of the source of the assignment
 *  @param[in] A Destination of the assignment
 *  @param[in] B Source of the assignment
 *  @deprecated Not used anymore
 */
template <bool condition, typename typeA, typename typeB>
void fill_value(typeA &A, typeB &B) {
  if constexpr(condition) { A = B; }
}

#endif
