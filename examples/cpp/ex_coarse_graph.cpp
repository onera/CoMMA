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
#include <vector>

#include "CoMMA/CoMMADefs.h"
#include "CoMMA/Util.h"

using IndexT = comma::CoMMAIndexT;
using IntT = comma::CoMMAIntT;
using WeightT = comma::CoMMAWeightT;
using std::vector;

int main() {
  // Definition of a 2D square 4x4 mesh
  // Copied from root/tests/DualGraphExamples.h::DualGEx_quad_4
  // clang-format off
  const vector<IndexT> f_adj_idx = {0, 2, 5, 8, 10, 13, 17, 21, 24, 27, 31, 35, 38, 40, 43, 46, 48};
  const vector<IndexT> f_adj = {1, 4, 0, 2, 5, 1, 3, 6, 2, 7, 0, 5, 8, 1, 4, 6, 9, 2, 5, 7, 10, 3, 6, 11, 4, 9, 12, 5, 8, 10, 13, 6, 9, 11, 14, 7, 10, 15, 8, 13, 9, 12, 14, 10, 13, 15, 11, 14};
  const vector<WeightT> f_weights = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  const vector<WeightT> f_volumes = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  const vector<vector<WeightT>> f_centers = {{0.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}, {3.5, 0.5}, {0.5, 1.5}, {1.5, 1.5}, {2.5, 1.5}, {3.5, 1.5}, {0.5, 2.5}, {1.5, 2.5}, {2.5, 2.5}, {3.5, 2.5}, {0.5, 3.5}, {1.5, 3.5}, {2.5, 3.5}, {3.5, 3.5}};
  const vector<WeightT> f_priority = {16., 15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1.};
  const vector<IntT> f_n_bnd = {2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1, 2, 1, 1, 2};
  // clang-format on
  // Example of result of agglomeration
  const vector<IndexT> f2c = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3};
  // Output - coarse graph
  vector<IndexT> c_adj_idx{};
  vector<IndexT> c_adj{};
  vector<WeightT> c_weights{};
  vector<WeightT> c_volumes{};
  vector<vector<WeightT>> c_centers{};
  vector<WeightT> c_priority{};
  vector<IntT> c_n_bnd{};
  comma::build_coarse_graph<IndexT, WeightT, IntT>(
    f2c,
    f_adj_idx,
    f_adj,
    f_weights,
    f_volumes,
    f_centers,
    f_priority,
    f_n_bnd,
    // Output
    c_adj_idx,
    c_adj,
    c_weights,
    c_volumes,
    c_centers,
    c_priority,
    c_n_bnd
  );
  // One can now reuse c_adj_idx, ... as input of comma::agglomerate_one_level
  return 0;
}
