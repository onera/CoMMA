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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "CoMMA.h"

#include "CoMMATypes.h"

PYBIND11_MODULE(CoMMA, module_handle) {
  module_handle.doc() = "CoMMA is an agglomeration library";
  module_handle.def(
      "agglomerate_one_level",
      [](const vector<CoMMAIndexT> &adjMatrix_row_ptr,
         const vector<CoMMAIndexT> &adjMatrix_col_ind,
         const vector<CoMMAWeightT> &adjMatrix_areaValues,
         const vector<CoMMAWeightT> &volumes,
         const vector<vector<CoMMAWeightT>> centers,
         const vector<CoMMAWeightT> &priority_weights,

         // Indices of compliant cc
         const vector<CoMMAIndexT> &arrayOfFineAnisotropicCompliantCells,

         // boundaries
         const vector<CoMMAIntT> &n_bnd_faces,

         // Agglomeration argument
         bool isFirstAgglomeration, bool is_anisotropic,
         CoMMAWeightT threshold_anisotropy,
         const CoMMAIntT seed_ordering_type,

         // Outputs
         vector<CoMMAIndexT> fc_to_cc,                // Out
         vector<CoMMAIndexT> agglomerationLines_Idx,  // In & out
         vector<CoMMAIndexT> agglomerationLines,      // In & out

         // Args with default value
         bool correction, CoMMAIntT dimension, CoMMAIntT goal_card,
         CoMMAIntT min_card, CoMMAIntT max_card,
         CoMMAIntT fc_choice_iter,
         const CoMMAIntT type_of_isotropic_agglomeration) {
        agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
            adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes, centers,
            priority_weights, arrayOfFineAnisotropicCompliantCells, n_bnd_faces,
            isFirstAgglomeration, is_anisotropic, threshold_anisotropy, seed_ordering_type,
            fc_to_cc, agglomerationLines_Idx, agglomerationLines, correction,
            dimension, goal_card, min_card, max_card, fc_choice_iter, type_of_isotropic_agglomeration);
        return std::make_tuple(fc_to_cc, agglomerationLines_Idx,
                               agglomerationLines);
      });
}
