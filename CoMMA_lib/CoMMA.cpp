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

#include <type_traits>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "CoMMA.h"

#include "CoMMATypes.h"

using CoMMASignedIndexT = make_signed<CoMMAIndexT>::type;
using namespace pybind11::literals; // Use _a for args literals

PYBIND11_MODULE(CoMMA, module_handle) {
  module_handle.doc() =
    "CoMMA is an agglomeration library\n"
    "Functions:\n"
    " - agglomerate_one_level: agglomerate a fine mesh\n"
    " - compute_neighbourhood_based_wall_distance: compute distance from a given wall\n"
    "\n"
    "Types to use with numpy:\n"
    " - IndexType: type for indices\n"
    " - WeightType: type for weights\n"
    " - IntType: type for integers";
  module_handle.attr("__version__") = "1.2",
  module_handle.attr("IndexType") = "ulong",
  module_handle.attr("WeightType") = "double",
  module_handle.attr("IntType") = "int",
  module_handle.def(
      "agglomerate_one_level",
      [](// Dual graph
         const vector<CoMMAIndexT> &adjMatrix_row_ptr,
         const vector<CoMMAIndexT> &adjMatrix_col_ind,
         const vector<CoMMAWeightT> &adjMatrix_areaValues,
         const vector<CoMMAWeightT> &volumes,

         // Additional info about the mesh
         const vector<vector<CoMMAWeightT>> centers,
         const vector<CoMMAWeightT> &priority_weights,
         const vector<CoMMAIndexT> &anisotropicCompliantCells,
         const vector<CoMMAIntT> &n_bnd_faces,

         // Anisotropy related info
         bool build_anisotropic_lines,
         bool is_anisotropic,
         bool odd_line_length,
         CoMMAWeightT threshold_anisotropy,

         // Seed ordering
         const CoMMAIntT seed_ordering_type,

         // Outputs
         vector<CoMMAIndexT> fc_to_cc,                // Out
         vector<CoMMAIndexT> agglomerationLines_Idx,  // In & out
         vector<CoMMAIndexT> agglomerationLines,      // In & out

         // Tuning of the algorithms
         bool correction,
         CoMMAIntT dimension,
         CoMMAIntT goal_card,
         CoMMAIntT min_card,
         CoMMAIntT max_card,
         CoMMAIntT singular_card_thresh,
         CoMMAIntT fc_choice_iter,
         const CoMMAIntT type_of_isotropic_agglomeration) {
        agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
            adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes, centers,
            priority_weights, anisotropicCompliantCells, n_bnd_faces,
            build_anisotropic_lines, is_anisotropic, odd_line_length, threshold_anisotropy, seed_ordering_type,
            fc_to_cc, agglomerationLines_Idx, agglomerationLines, correction,
            dimension, goal_card, min_card, max_card, singular_card_thresh, fc_choice_iter, type_of_isotropic_agglomeration);
        return std::make_tuple(fc_to_cc, agglomerationLines_Idx,
                               agglomerationLines);
      },
      "Given a CSR graph-representation of a fine mesh, agglomerate it",
      // Register arguments
      "adjMatrix_row_ptr"_a, "adjMatrix_col_ind"_a, "adjMatrix_areaValues"_a,
      "volumes"_a, "centers"_a, "priority_weights"_a, "anisotropicCompliantCells"_a,
      "n_bnd_faces"_a, "build_anisotropic_lines"_a, "is_anisotropic"_a,
      "odd_line_length"_a, "threshold_anisotropy"_a, "seed_ordering_type"_a,
      "fc_to_cc"_a, "agglomerationLines_Idx"_a, "agglomerationLines"_a,
      "correction"_a, "dimension"_a, "goal_card"_a, "min_card"_a, "max_card"_a,
      "singular_card_thresh"_a = 1, "fc_choice_iter"_a = 1, "type_of_isotropic_agglomeration"_a = 0
      );
  module_handle.def(
      "compute_neighbourhood_based_wall_distance",
      [](const vector<CoMMAIndexT>& neigh_idxs,
         const vector<CoMMAIndexT>& neighs,
         const vector<CoMMAIndexT>& wall) {
        vector<CoMMASignedIndexT> dist{};
        compute_neighbourhood_based_wall_distance<CoMMAIndexT,CoMMASignedIndexT>(
                neigh_idxs, neighs, wall, dist);
        return dist;
      },
      "Compute the neighbourhood-based distance from a given wall",
      "neigh_idxs"_a, "neighs"_a, "wall"_a);
}
