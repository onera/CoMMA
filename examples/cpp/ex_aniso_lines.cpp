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
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "CoMMA/Args.h"
#include "CoMMA/CoMMA.h"
#include "CoMMA/CoMMAConfig.h"
#include "CoMMA/CoMMADefs.h"

using IndexT = comma::CoMMAIndexT;
using IntT = comma::CoMMAIntT;
using WeightT = comma::CoMMAWeightT;

/** @brief Class with the definition of a mesh, taken from
 * ../scripts/ex_aniso_lines.py
 */
class Ex_Dual_Graph {
public:
  const IntT dim = 2;
  // clang-format off
  const std::vector<IndexT> adj_idx = {
    0, 2, 5, 8, 11, 14, 16, 19, 23, 27, 31, 35, 38, 42, 46, 50, 54, 57, 60, 62,
    65, 67, 70, 73, 76
  };
  const std::vector<IndexT> adj = {
    1, 11, 0, 2, 10, 1, 3, 9, 2, 4, 8, 3, 5, 7, 4, 6, 5, 7, 17, 4, 8, 6, 15, 3,
    9, 7, 13, 2, 10, 8, 12, 1, 11, 9, 14, 0, 10, 16, 9, 14, 13, 23, 8, 12, 15,
    21, 10, 16, 12, 22, 7, 13, 17, 19, 11, 14, 20, 6, 15, 18, 17, 19, 15, 21,
    18, 16, 22, 13, 23, 19, 14, 20, 23, 12, 22, 21
  };
  const std::vector<WeightT> areas = {
    1. , 0.2, 1. , 1. , 0.2, 1. , 1. , 0.2, 1. , 1. , 0.2, 1. , 1. , 0.2, 1. ,
    1. , 1. , 1. , 1. , 0.2, 1. , 1. , 0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. , 1. ,
    0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. , 0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. , 1. ,
    0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. , 1. , 0.2, 0.2, 1. , 0.2, 1. , 1. , 1. ,
    1. , 1. , 0.2, 1. , 1. , 0.2, 1. , 0.2, 1. , 1. , 0.2, 1. , 1. , 0.2, 1. ,
    1.
  };
  const std::vector<WeightT> volumes = {
    0.2, 0.2, 0.2, 0.2, 0.2, 1. , 1. , 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2,
    0.2, 0.2, 1. , 1. , 0.2, 0.2, 0.2, 0.2, 0.2
  };
  const std::vector<std::vector<WeightT>> centers = {
    {0.5, 0.1}, {0.5, 0.3}, {0.5, 0.5}, {0.5, 0.7}, {0.5, 0.9}, {0.5, 1.5},
    {1.5, 1.5}, {1.5, 0.9}, {1.5, 0.7}, {1.5, 0.5}, {1.5, 0.3}, {1.5, 0.1},
    {2.5, 0.5}, {2.5, 0.7}, {2.5, 0.3}, {2.5, 0.9}, {2.5, 0.1}, {2.5, 1.5},
    {3.5, 1.5}, {3.5, 0.9}, {3.5, 0.1}, {3.5, 0.7}, {3.5, 0.3}, {3.5, 0.5}
  };
  const std::vector<IntT> n_bnd_faces = {
    2, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 2, 1, 2, 1, 1, 1
  };
  const std::vector<IndexT> compliant_cells = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23
  };
  const std::vector<WeightT> priority = {
    24., 23., 22., 21., 20., 19., 18., 17., 16., 15., 14., 13., 12., 11., 10.,
    9., 8., 7., 6., 5., 4., 3., 2., 1.
  };
  // clang-format on

  Ex_Dual_Graph() = default;
};

// Global variables for tuning the agglomeration
const Ex_Dual_Graph graph{};
constexpr bool is_aniso = true;
constexpr bool build_lines = true;
constexpr IntT min_card = 4;
constexpr IntT goal_card = 4;
constexpr IntT max_card = 4;
constexpr bool correction = false;
constexpr WeightT aniso_thresh = 1.5;
constexpr bool odd_line_length = true;
constexpr comma::CoMMAAspectRatioT AR =
  comma::CoMMAAspectRatioT::DIAMETER_OVER_RADIUS;
constexpr comma::CoMMASeedsPoolT seed_order =
  comma::CoMMASeedsPoolT::BOUNDARY_PRIORITY;
constexpr IntT sing_card = 1;
constexpr std::optional<IntT> max_cells_in_line = std::nullopt;
constexpr IntT fc_iter = 1;
constexpr comma::CoMMANeighbourhoodT neigh =
  comma::CoMMANeighbourhoodT::EXTENDED;

// Reference results
const std::vector<IndexT> ref_f2c = {0, 0, 1, 1, 1, 8, 8, 3, 3, 3, 2, 2,
                                     5, 5, 4, 5, 4, 8, 8, 7, 6, 7, 6, 7};
const std::vector<IndexT> ref_alines_idx = {0, 2, 4, 6, 8};
const std::vector<IndexT> ref_alines = {0, 1, 2, 3, 4, 5, 6, 7};

template<typename T>
inline void compare_and_print(
  const std::string &label, const T &left, const T &right
) {
  std::cout << "* " << label << ": " << (left == right ? "OK" : "KO")
            << std::endl;
}

/** @brief Test main function and compare the results with reference ones. */
void ex_agglomerate_one_level() {
  // Prepare output containers
  std::vector<IndexT> f2c{}, alines_idx{}, alines{};

  // Call main function
  comma::agglomerate_one_level<IndexT, WeightT, IntT>(
    graph.adj_idx,
    graph.adj,
    graph.areas,
    graph.volumes,
    graph.centers,
    graph.priority,
    graph.compliant_cells,
    graph.n_bnd_faces,
    build_lines,
    is_aniso,
    odd_line_length,
    aniso_thresh,
    seed_order,
    f2c,
    alines_idx,
    alines,
    correction,
    graph.dim,
    goal_card,
    min_card,
    max_card,
    // Args with defaults
    AR,
    sing_card,
    max_cells_in_line,
    fc_iter,
    neigh
  );

  // Compare results
  std::cout << "Test full signature:" << std::endl;
  compare_and_print("Aniso lines indices", alines_idx, ref_alines_idx);
  compare_and_print("Aniso lines", alines, ref_alines);
  compare_and_print("Agglomeration", f2c, ref_f2c);
}

/** @brief Test main function using signature with arguments and compare the
 * results with reference ones.
 */
void ex_agglomerate_one_level_args() {
  // Prepare arguments objects
  const comma::GraphArgs<IndexT, WeightT, IntT> graph_args(
    graph.adj_idx,
    graph.adj,
    graph.areas,
    graph.volumes,
    graph.centers,
    graph.priority,
    graph.n_bnd_faces,
    graph.dim
  );
  const comma::AgglomerationArgs<IndexT, WeightT, IntT> agglo_args(
    goal_card,
    min_card,
    max_card,
    correction,
    seed_order,
    // Args with defaults
    AR,
    sing_card,
    fc_iter,
    neigh
  );
  const comma::AnisotropicArgs<IndexT, WeightT, IntT> aniso_args(
    is_aniso,
    graph.compliant_cells,
    // Args with defaults
    build_lines,
    odd_line_length,
    aniso_thresh,
    max_cells_in_line
  );

  // Prepare output containers
  std::vector<IndexT> f2c{}, alines_idx{}, alines{};

  // Call main function
  comma::agglomerate_one_level<IndexT, WeightT, IntT>(
    graph_args, agglo_args, aniso_args, f2c, alines_idx, alines
  );

  // Compare results
  std::cout << "Test args signature:" << std::endl;
  compare_and_print("Aniso lines indices", alines_idx, ref_alines_idx);
  compare_and_print("Aniso lines", alines, ref_alines);
  compare_and_print("Agglomeration", f2c, ref_f2c);
}

int main() {
  ex_agglomerate_one_level();
  std::cout << std::endl;
  ex_agglomerate_one_level_args();
  return 0;
}
