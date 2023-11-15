/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <algorithm>
#include <deque>
#include <numeric>
#include <unordered_set>
#include <vector>

#include "CoMMATypes.h"
#include "Seeds_Pool.h"
#include "catch2/catch.hpp"
#include "input/DualGPy.h"

using namespace std;

SCENARIO("Test of the seeds pool", "[Seeds_Pool]") {
  GIVEN(
    "A 4x4x4 cube and a Seeds Pool which should ensure that the order respects the cell numbering") {
    const DualGPy_cube_4 Data = DualGPy_cube_4();
    Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
      seeds_pool(Data.n_bnd_faces, Data.weights, false);
    deque<CoMMAIndexT> corners{}, ridges{}, valleys{}, interior{};
    for (CoMMAIndexT i = 0; i < Data.nb_fc; ++i) {
      switch (Data.n_bnd_faces[i]) {
        case 3:
          corners.push_back(i);
          break;
        case 2:
          ridges.push_back(i);
          break;
        case 1:
          valleys.push_back(i);
          break;
        case 0:
          interior.push_back(i);
          break;
        default:
          continue;
      } /* Switch */
    } /* For */
    vector<bool> agglomerated(Data.nb_fc, false);
    WHEN("We create the seeds pool") {
      THEN("It is not initialized") {
        REQUIRE(seeds_pool.is_empty(0));
        REQUIRE(seeds_pool.is_empty(1));
        REQUIRE(seeds_pool.is_empty(2));
        REQUIRE(seeds_pool.is_empty(3));
        REQUIRE(seeds_pool.is_empty());
        REQUIRE(seeds_pool.need_initialization(agglomerated));
      }
    }
    seeds_pool.initialize();
    WHEN("We spoil the seed") {
      THEN("The order is respected") {
        for (auto i : corners) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : ridges) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : valleys) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : interior) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
      }
    }
    WHEN("We simply update the seeds pool") {
      // Spoil all the corners so that the seed is completely void
      for (auto i : corners) {
        seeds_pool.choose_new_seed(agglomerated);
        agglomerated[i] = true;
      }
      // In order: Internal, ridge, corner, valley
      deque<CoMMAIndexT> new_seeds = {21, 44, 63, 30};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.update(new_seeds);
      THEN("The order does not respect the priority but the boundary only") {
        auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(63 == opt_seed.value());
        agglomerated[63] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(44 == opt_seed.value());
        agglomerated[44] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(30 == opt_seed.value());
        agglomerated[30] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(21 == opt_seed.value());
        agglomerated[21] = true;
      }
      new_seeds = {10, 5, 6, 9};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.update(new_seeds);
      THEN(
        "The order respects the order of the input if all in the same queue") {
        for (auto i : new_seeds) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
      }
    }
    WHEN("We update the seeds pool by asking a reorder") {
      // Spoil all the corners so that the seed is completely void
      for (auto i : corners) {
        seeds_pool.choose_new_seed(agglomerated);
        agglomerated[i] = true;
      }
      // In order: Internal, ridge, corner, valley
      unordered_set<CoMMAIndexT> new_seeds = {21, 44, 63, 30};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.order_new_seeds_and_update(new_seeds);
      THEN("The order does not respect the priority but the boundary only") {
        auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(63 == opt_seed.value());
        agglomerated[63] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(44 == opt_seed.value());
        agglomerated[44] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(30 == opt_seed.value());
        agglomerated[30] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(21 == opt_seed.value());
        agglomerated[21] = true;
      }
      new_seeds = {10, 5, 6, 9};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.order_new_seeds_and_update(new_seeds);
      THEN("The order respects the priority all in the same queue") {
        auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(5 == opt_seed.value());
        agglomerated[5] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(6 == opt_seed.value());
        agglomerated[6] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(9 == opt_seed.value());
        agglomerated[9] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(10 == opt_seed.value());
        agglomerated[10] = true;
      }
    }
  }

  GIVEN(
    "A 4x4x4 cube and a Seeds Pool which should force an order reversed wrt the cell numbering") {
    const DualGPy_cube_4 Data = DualGPy_cube_4();
    vector<CoMMAWeightT> w(Data.nb_fc);
    iota(w.begin(), w.end(), 0);
    Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
      seeds_pool(Data.n_bnd_faces, w, false);
    deque<CoMMAIndexT> corners{}, ridges{}, valleys{}, interior{};
    for (CoMMAIndexT i = 0; i < Data.nb_fc; ++i) {
      switch (Data.n_bnd_faces[i]) {
        case 3:
          corners.push_front(i);
          break;
        case 2:
          ridges.push_front(i);
          break;
        case 1:
          valleys.push_front(i);
          break;
        case 0:
          interior.push_front(i);
          break;
        default:
          continue;
      } /* Switch */
    } /* For */
    vector<bool> agglomerated(Data.nb_fc, false);
    WHEN("We create the seeds pool") {
      THEN("It is not initialized") {
        REQUIRE(seeds_pool.need_initialization(agglomerated));
      }
    }
    seeds_pool.initialize();
    WHEN("We spoil the seed") {
      THEN("The order is respected") {
        for (auto i : corners) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : ridges) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : valleys) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : interior) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
      }
    }
  }
  GIVEN("A mesh with a hole and two corners") {
    const DualGPy_hole_w_corners Data = DualGPy_hole_w_corners();
    const Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
      Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces,
      Data.dim, Data.arrayOfFineAnisotropicCompliantCells);
    WHEN("We use a seeds pool with boundary priority") {
      Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, false);
      seeds_pool.initialize();
      // clang-format off
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 7, 5, 6};   // ...finally, interior cells in an order given by neighbourhood
      // clang-on
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto &s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end());  // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
    WHEN(
      "We use a seeds pool with boundary priority and with only one start point") {
      Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(
        Data.n_bnd_faces, Data.weights, true);
      seeds_pool.initialize();
#if 0
The one-point initialization would not any impact w.r.t. the standard one on this mesh
#endif
      // clang-format off
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 7, 5, 6};   // ...finally, interior cells in an order given by neighbourhood
      // clang-on
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto &s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end());  // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
    WHEN("We use a seeds pool with neighbourhood priority") {
      Seeds_Pool_Neighbourhood_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, false);
      seeds_pool.initialize();
      // clang-format off
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        4, 7, 5, 6,    // ...then, interior cells in an order given by neighbourhood
        8, 11, 9, 10}; // ...finally, inner boundary cells in an order given by neighbourhood
      // clang-on
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto &s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end());  // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
    WHEN(
      "We use a seeds pool with neighbourhood priority and with only one start point") {
      Seeds_Pool_Neighbourhood_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, true);
      seeds_pool.initialize();
      // clang-format off
      const vector<CoMMAIndexT> expected_order = {
        12,             // Starting point
        0, 3,           // Direct neighbours
        13,             // Another corner
        2, 1,           // Outer boundary cells
        4, 7, 6, 5,     // Interior cells
        8, 11, 10, 9};  // Inner boundary cells
      // clang-on
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto &s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end());  // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
  }
  GIVEN("A mesh with a hole and no corners") {
    const DualGPy_hole_no_corners Data = DualGPy_hole_no_corners();
    const Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
      Data.nb_fc,
      Data.adjMatrix_row_ptr,
      Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues,
      Data.volumes,
      Data.centers,
      Data.n_bnd_faces,
      Data.dim,
      Data.arrayOfFineAnisotropicCompliantCells);
    WHEN("We use a seeds pool with boundary priority") {
      Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(
        Data.n_bnd_faces, Data.weights, false);
      seeds_pool.initialize();
      // clang-format off
      const vector<CoMMAIndexT> expected_order = {
        0, 1, 2, 3, 8, 9, 10, 11, // First, outer boundary cells in an order given by the weights
        4, 5, 6, 7};              // ...finally, interior cells in an order given by neighbourhood
      // clang-on
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto &s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end());  // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
    WHEN("We use a seeds pool with boundary priority") {
      Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(
        Data.n_bnd_faces, Data.weights, true);
      seeds_pool.initialize();
      // clang-format off
      const vector<CoMMAIndexT> expected_order = {
        0,             // Starting point,
        1, 3, 2,       // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 5, 7, 6};   // ...finally, interior cells in an order given by neighbourhood
      // clang-on
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto &s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end());  // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
  }
}
