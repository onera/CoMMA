#ifndef COMMA_PROJECT_TEMPLATE_HELPERS_H
#define COMMA_PROJECT_TEMPLATE_HELPERS_H

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

namespace comma {

/** @brief Assign from type to another only if a compile-time condition is
 * verified
 *  @tparam condition Boolean telling if the assignment should be done. It has
 * to be a \p constexpr
 *  @tparam typeA Type of the destination of the assignment
 *  @tparam typeB Type of the source of the assignment
 *  @param[in] a Destination of the assignment
 *  @param[in] b Source of the assignment
 *  @deprecated Not used anymore
 */
template<bool condition, typename typeA, typename typeB>
void fill_value(typeA &a, typeB &b) {
  if constexpr (condition) { a = b; }
}

}  // end namespace comma

#endif
