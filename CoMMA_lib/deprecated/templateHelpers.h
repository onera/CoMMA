#ifndef COMMA_PROJECT_TEMPLATE_HELPERS_H
#define COMMA_PROJECT_TEMPLATE_HELPERS_H

/*
 *  * CoMMA
    * Copyright © 2023 ONERA
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

template <bool condition, typename typeA, typename typeB>
void fill_value(typeA &A, typeB &B) {
  if
    constexpr(condition) { A = B; }
}

#endif
