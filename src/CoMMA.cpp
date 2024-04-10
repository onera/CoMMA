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

#include "CoMMA/CoMMA.h"

#include <pybind11/attr.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "CoMMA/CoMMAConfig.h"
#include "CoMMA/CoMMADefs.h"
#include "CoMMA/Util.h"

#define CoMMA_xstr(s) CoMMA_str(s)
#define CoMMA_str(s) #s

using namespace comma;  // NOLINT
using namespace pybind11::literals;  // NOLINT Use _a for args literals
namespace py = pybind11;
using namespace std;  // NOLINT

using CoMMASignedIndexT = make_signed<CoMMAIndexT>::type;

// NOLINTNEXTLINE
PYBIND11_MODULE(CoMMA, module_handle) {
  module_handle.doc() =
    "CoMMA is an agglomeration library\n"
    "Functions:\n"
    " - agglomerate_one_level: agglomerate a fine mesh\n"
    " - compute_neighbourhood_based_wall_distance: compute distance from a "
    "given wall\n"
    "\n"
    "Types to use with numpy (suggestion):\n"
    " - IndexType: type for indices\n"
    " - WeightType: type for weights\n"
    " - IntType: type for integers";
  module_handle.attr("__version__") = CoMMA_xstr(CoMMA_VERSION);

  // It's a bit hard to convert from C type to numpy type,
  // https://numpy.org/doc/stable/user/basics.types.html so we use just
  // suggestions
  module_handle.attr("IndexType") = "ulong";
  module_handle.attr("WeightType") = "double";
  module_handle.attr("IntType") = "int";

  // Exporting enumerator to be used in python
  // https://stackoverflow.com/questions/47893832/pybind11-global-level-enum
  // https://pybind11.readthedocs.io/en/stable/classes.html?highlight=enum#enumerations-and-internal-types
  // https://github.com/pybind/pybind11/blob/master/tests/test_enum.py
  // https://github.com/pybind/pybind11/blob/master/tests/test_enum.cpp
  py::enum_<CoMMANeighbourhoodT>(
    module_handle,
    "Neighbourhood",
    "Type of neighbourhood of a coarse cell considered when agglomerating"
  )
    .value(
      "EXTENDED",
      CoMMANeighbourhoodT::EXTENDED,
      "All neighbours of the coarse cell"
    )
    .value(
      "PURE_FRONT",
      CoMMANeighbourhoodT::PURE_FRONT,
      "Only neighbours of the last added fine cell"
    )
    .export_values();
  py::enum_<CoMMASeedsPoolT>(
    module_handle, "SeedsPool", "Type of seeds pool ordering"
  )
    .value(
      "BOUNDARY",
      CoMMASeedsPoolT::BOUNDARY_PRIORITY,
      "The number of boundary faces has higher priority"
    )
    .value(
      "NEIGHBOURHOOD",
      CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY,
      "Neighbours of already agglomerated coarse cells have higher priority)"
    )
    .value(
      "BOUNDARY_POINT_INIT",
      CoMMASeedsPoolT::BOUNDARY_PRIORITY_ONE_POINT_INIT,
      "The number of boundary faces has higher priority, and initialize with one point only then let evolve"
    )
    .value(
      "NEIGHBOURHOOD_POINT_INIT",
      CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY_ONE_POINT_INIT,
      "Neighbours of already agglomerated coarse cells have higher priority, and initialize with one point only then let evolve"
    )
    .export_values();
  py::enum_<CoMMAAspectRatioT>(module_handle, "AR", "Type of aspect-ratio")
    .value(
      "DIAMETER_OVER_RADIUS",
      CoMMAAspectRatioT::DIAMETER_OVER_RADIUS,
      "Diameter over radius"
    )
    .value(
      "DIAMETER_OVER_MIN_EDGE",
      CoMMAAspectRatioT::DIAMETER_OVER_MIN_EDGE,
      "Diameter over minimum edge"
    )
    .value("DIAMETER", CoMMAAspectRatioT::DIAMETER, "Diameter")
    .value(
      "ONE_OVER_MEASURE",
      CoMMAAspectRatioT::ONE_OVER_MEASURE,
      "One over the measure (e.g., volume) of the cell"
    )
    .value(
      "ONE_OVER_INTERNAL_WEIGHTS",
      CoMMAAspectRatioT::ONE_OVER_INTERNAL_WEIGHTS,
      "One over the internal weights"
    )
    .value(
      "PERIMETER_OVER_RADIUS",
      CoMMAAspectRatioT::PERIMETER_OVER_RADIUS,
      "Perimeter over radius"
    )
    .value(
      "EXTERNAL_WEIGHTS",
      CoMMAAspectRatioT::EXTERNAL_WEIGHTS,
      "External weights, that is, perimeter"
    )
    .value(
      "MAX_BARY_DIST_OVER_RADIUS",
      CoMMAAspectRatioT::MAX_BARY_DIST_OVER_RADIUS,
      "Maximum FC-center distance from barycenter over radius"
    )
    .value(
      "MAX_OVER_MIN_BARY_DIST",
      CoMMAAspectRatioT::MAX_OVER_MIN_BARY_DIST,
      "Maximum over minimum FC-center distance from barycenter"
    )
    .value(
      "ALGEBRAIC_PERIMETER_OVER_MEASURE",
      CoMMAAspectRatioT::ALGEBRAIC_PERIMETER_OVER_MEASURE,
      "Algebraic-like perimeter over measure, that is, external weights over cell weight"
    )
    .export_values();

  // Main function
  module_handle.def(
    "agglomerate_one_level",
    [](  // Dual graph
      const vector<CoMMAIndexT> &adjMatrix_row_ptr,
      const vector<CoMMAIndexT> &adjMatrix_col_ind,
      const vector<CoMMAWeightT> &adjMatrix_areaValues,
      const vector<CoMMAWeightT> &volumes,

      // Additional info about the mesh
      const vector<vector<CoMMAWeightT>> &centers,
      const vector<CoMMAWeightT> &priority_weights,
      const vector<CoMMAIndexT> &anisotropicCompliantCells,
      const vector<CoMMAIntT> &n_bnd_faces,

      // Anisotropy related info
      bool build_anisotropic_lines,
      bool is_anisotropic,
      bool odd_line_length,
      CoMMAWeightT threshold_anisotropy,

      // Seed ordering
      CoMMAIntT seed_ordering_type,

      // Outputs
      vector<CoMMAIndexT> fc_to_cc,  // Out
      vector<CoMMAIndexT> agglomerationLines_Idx,  // In & out
      vector<CoMMAIndexT> agglomerationLines,  // In & out

      // Tuning of the algorithms
      bool correction,
      CoMMAIntT dimension,
      CoMMAIntT goal_card,
      CoMMAIntT min_card,
      CoMMAIntT max_card,
      CoMMAIntT aspect_ratio,
      CoMMAIntT singular_card_thresh,
      optional<CoMMAIndexT> max_cells_in_line,
      CoMMAIntT fc_choice_iter,
      CoMMAIntT type_of_isotropic_agglomeration
    ) {
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        adjMatrix_row_ptr,
        adjMatrix_col_ind,
        adjMatrix_areaValues,
        volumes,
        centers,
        priority_weights,
        anisotropicCompliantCells,
        n_bnd_faces,
        build_anisotropic_lines,
        is_anisotropic,
        odd_line_length,
        threshold_anisotropy,
        static_cast<CoMMASeedsPoolT>(seed_ordering_type),
        fc_to_cc,
        agglomerationLines_Idx,
        agglomerationLines,
        correction,
        dimension,
        goal_card,
        min_card,
        max_card,
        static_cast<CoMMAAspectRatioT>(aspect_ratio),
        singular_card_thresh,
        max_cells_in_line,
        fc_choice_iter,
        static_cast<CoMMANeighbourhoodT>(type_of_isotropic_agglomeration)
      );
      return std::make_tuple(
        fc_to_cc, agglomerationLines_Idx, agglomerationLines
      );
    },
    "Given a CSR graph-representation of a fine mesh, agglomerate it",
    // Register arguments
    "adjMatrix_row_ptr"_a,
    "adjMatrix_col_ind"_a,
    "adjMatrix_areaValues"_a,
    "volumes"_a,
    "centers"_a,
    "priority_weights"_a,
    "anisotropicCompliantCells"_a,
    "n_bnd_faces"_a,
    "build_anisotropic_lines"_a,
    "is_anisotropic"_a,
    "odd_line_length"_a,
    "threshold_anisotropy"_a,
    "seed_ordering_type"_a,
    "fc_to_cc"_a,
    "agglomerationLines_Idx"_a,
    "agglomerationLines"_a,
    "correction"_a,
    "dimension"_a,
    "goal_card"_a,
    "min_card"_a,
    "max_card"_a,
    "aspect_ratio"_a = CoMMAAspectRatioT::DIAMETER_OVER_RADIUS,
    "singular_card_thresh"_a = 1,
    "max_cells_in_line"_a = std::nullopt,
    "fc_choice_iter"_a = 1,
    "type_of_isotropic_agglomeration"_a = CoMMANeighbourhoodT::EXTENDED
  );

  module_handle.def(
    "compute_neighbourhood_based_wall_distance",
    [](
      const vector<CoMMAIndexT> &neigh_idxs,
      const vector<CoMMAIndexT> &neighs,
      const vector<CoMMAIndexT> &wall
    ) {
      std::vector<CoMMASignedIndexT> dist{};
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMASignedIndexT>(
        neigh_idxs, neighs, wall, dist
      );
      return dist;
    },
    "Compute the neighbourhood-based distance from a given wall",
    "neigh_idxs"_a,
    "neighs"_a,
    "wall"_a
  );

  module_handle.def(
    "filter_cells_by_n_edges",
    [](
      const std::vector<CoMMAIndexT> &indices,
      const std::vector<CoMMAIntT> &n_bnd_faces,
      const std::unordered_set<CoMMAIntT> allowed
    ) {
      std::vector<CoMMAIndexT> filtered{};
      filter_cells_by_n_edges(indices, n_bnd_faces, allowed, filtered);
      return filtered;
    },
    "Filter cells according to their number of edges",
    "indices"_a,
    "n_bnd_face"_a,
    "allowed"_a
  );
}

#undef CoMMA_str
#undef CoMMA_xstr
