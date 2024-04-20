/*
 * CoMMA
 *
 * Copyright © 2024 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CoMMA/Neighbourhood.h"
#include "DualGraphExamples.h"
#include "test_defs.h"

using namespace comma;  // NOLINT
using namespace std;  // NOLINT
using Catch::Matchers::Contains;
using Catch::Matchers::UnorderedRangeEquals;

using CoMMAPairT = pair<CoMMAIndexT, CoMMAWeightT>;
using CoMMAPairFindFirstBasedT = PairFindFirstBasedFunctor<CoMMAPairT>;

// About the size
// NOLINTNEXTLINE
SCENARIO("Test neighbourhood computing", "[Neighbourhood]") {
  GIVEN("We have a 7x7 Cartesian 2D matrix") {
    const DualGEx_quad_7 Data = DualGEx_quad_7();
    const Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
      Data.nb_fc,
      Data.adjMatrix_row_ptr,
      Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues,
      Data.volumes,
      Data.centers,
      Data.n_bnd_faces,
      Data.dim,
      Data.anisoCompliantCells
    );
    const CoMMAIndexT seed = 24;
    CoMMAIntT neigh_order = 3;
    const unordered_set<CoMMAIndexT> s_seeds = {seed};
    const CoMMAIntT card = 4;
    WHEN("We compute neighbourhood of cell 24 (no cell is agglomerated)") {
      const vector<bool> agglomerated =
        vector<bool>(Data.volumes.size(), false);
      unordered_map<CoMMAIndexT, CoMMAIntT> d_n_of_seed;
      fc_graph.compute_neighbourhood_of_cc(
        s_seeds, neigh_order, d_n_of_seed, card, agglomerated
      );
      vector<set<CoMMAIndexT>> neighs = vector<set<CoMMAIndexT>>(neigh_order);
      for (auto [k, v] : d_n_of_seed)
        neighs[v - 1].insert(k);
      THEN("First order is good") {
        REQUIRE_THAT(neighs[0], UnorderedRangeEquals(CCT{17, 23, 25, 31}));
      }
      THEN("Second order is good") {
        REQUIRE_THAT(
          neighs[1], UnorderedRangeEquals(CCT{10, 16, 18, 22, 26, 30, 32, 38})
        );
      }
      THEN("Third order is good") {
        REQUIRE_THAT(
          neighs[2],
          UnorderedRangeEquals(CCT{3, 9, 11, 15, 19, 21, 27, 29, 33, 37, 39, 45}
          )
        );
      }
    }  // WHEN PREVIOUS AGGLOMERATION
    WHEN(
      "We compute neighbourhood of cell 24 (cell 10,16, 28-to-34 agglomerated)"
    ) {
      vector<bool> agglomerated = vector<bool>(Data.volumes.size(), false);
      agglomerated[10] = agglomerated[16] = true;
      for (int i = 28; i < 35; ++i)
        agglomerated[i] = true;
      unordered_map<CoMMAIndexT, CoMMAIntT> d_n_of_seed;
      fc_graph.compute_neighbourhood_of_cc(
        s_seeds, neigh_order, d_n_of_seed, card, agglomerated
      );
      vector<set<CoMMAIndexT>> neighs = vector<set<CoMMAIndexT>>(neigh_order);
      for (auto [k, v] : d_n_of_seed)
        neighs[v - 1].insert(k);
      THEN("First order is good") {
        REQUIRE_THAT(neighs[0], UnorderedRangeEquals(CCT{17, 23, 25}));
      }
      THEN("Second order is good") {
        REQUIRE_THAT(neighs[1], UnorderedRangeEquals(CCT{18, 22, 26}));
      }
      THEN("Third order is good") {
        REQUIRE_THAT(neighs[2], UnorderedRangeEquals(CCT{11, 15, 19, 21, 27}));
      }
    }  // WHEN PREVIOUS AGGLOMERATION
    WHEN("We compute the neighbourhood of a coarse cell") {
      vector<bool> agglomerated = vector<bool>(Data.nb_fc, false);
      const unordered_set<CoMMAIndexT> cc = {16, 17, 18, 23, 24};
      THEN("The whole neighbourhood is returned if no cell is agglomerated") {
        auto neighs = fc_graph.get_neighbourhood_of_cc(cc, agglomerated);
        REQUIRE_THAT(
          neighs, UnorderedRangeEquals(CCT{15, 22, 30, 31, 25, 19, 11, 10, 9})
        );
      }
      agglomerated[15] = true;
      agglomerated[9] = true;
      agglomerated[19] = true;
      THEN(
        "If some cells are agglomerated, then they do not appear in the neighbourhood"
      ) {
        auto neighs = fc_graph.get_neighbourhood_of_cc(cc, agglomerated);
        REQUIRE_THAT(neighs, UnorderedRangeEquals(CCT{22, 30, 31, 25, 11, 10}));
      }
    }
  }
  GIVEN(
    "We have a 7x7 Cartesian 2D matrix, a standard Neighbourhood for 24 a one given by the creator"
  ) {
    const DualGEx_quad_7 Data = DualGEx_quad_7();
    const Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
      Data.nb_fc,
      Data.adjMatrix_row_ptr,
      Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues,
      Data.volumes,
      Data.centers,
      Data.n_bnd_faces,
      Data.dim,
      Data.anisoCompliantCells
    );
    const CoMMAIndexT seed = 24;
    CoMMAIntT neigh_order = 2;
    const unordered_set<CoMMAIndexT> s_seeds = {seed};
    const CoMMAIntT card = 4;
    const vector<bool> agglomerated = vector<bool>(Data.volumes.size(), false);
    unordered_map<CoMMAIndexT, CoMMAIntT> d_n_of_seed;
    fc_graph.compute_neighbourhood_of_cc(
      s_seeds, neigh_order, d_n_of_seed, card, agglomerated
    );
    const unordered_set<CoMMAIndexT> s_neighbours_of_seed =
      d_keys_to_set<CoMMAIndexT, CoMMAIntT>(d_n_of_seed);
    Neighbourhood_Extended<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> neighbourhood(
      s_neighbours_of_seed, Data.weights
    );
    neighbourhood.update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first neighbourhood") {
      const auto &fon = neighbourhood.get_candidates();
      THEN("Only direct neighbours are in the neighbourhood") {
        REQUIRE_THAT(fon, UnorderedRangeEquals(CCT{17, 23, 25, 31}));
      }
    }
    // Testing the neighbourhood creator
    const NeighbourhoodExtendedCreator<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
      neigh_crtor{};
    auto c_neighbourhood =
      neigh_crtor.create(s_neighbours_of_seed, Data.weights, Data.dim);
    c_neighbourhood->update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first neighbourhood of the one got from the creator") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Only direct neighbours are in the neighbourhood") {
        REQUIRE_THAT(fon, UnorderedRangeEquals(CCT{17, 23, 25, 31}));
      }
    }
    neighbourhood.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      const auto &fon = neighbourhood.get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE_THAT(fon, !Contains(31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE_THAT(fon, Contains(17));
        REQUIRE_THAT(fon, Contains(23));
        REQUIRE_THAT(fon, Contains(25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE_THAT(fon, Contains(30));
        REQUIRE_THAT(fon, Contains(32));
        REQUIRE_THAT(fon, Contains(38));
      }
    }
    c_neighbourhood->update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31 to the creator-provided neighbourhood") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE_THAT(fon, !Contains(31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE_THAT(fon, Contains(17));
        REQUIRE_THAT(fon, Contains(23));
        REQUIRE_THAT(fon, Contains(25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE_THAT(fon, Contains(30));
        REQUIRE_THAT(fon, Contains(32));
        REQUIRE_THAT(fon, Contains(38));
      }
    }
    auto copy_c_neighbourhood = neigh_crtor.clone(c_neighbourhood);
    // We update after the copy to see if that's really a copy
    c_neighbourhood->update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 31 to the copy of the creator-provided neighbourhood") {
      const auto fon = copy_c_neighbourhood->get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE_THAT(fon, !Contains(31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE_THAT(fon, Contains(17));
        REQUIRE_THAT(fon, Contains(23));
        REQUIRE_THAT(fon, Contains(25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE_THAT(fon, Contains(30));
        REQUIRE_THAT(fon, Contains(32));
        REQUIRE_THAT(fon, Contains(38));
      }
    }
    neighbourhood.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      const auto &fon = neighbourhood.get_candidates();
      THEN("Cell 38 is no more in the neighbourhood") {
        REQUIRE_THAT(fon, !Contains(38));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE_THAT(fon, Contains(17));
        REQUIRE_THAT(fon, Contains(23));
        REQUIRE_THAT(fon, Contains(25));
        REQUIRE_THAT(fon, Contains(30));
        REQUIRE_THAT(fon, Contains(32));
      }
      THEN(
        "Direct neighbours of 31 are NOT in the neighbourhood (max order neighbourhood)"
      ) {
        REQUIRE_THAT(fon, !Contains(37));
        REQUIRE_THAT(fon, !Contains(39));
        REQUIRE_THAT(fon, !Contains(45));
      }
    }
  }
  GIVEN(
    "We have a 7x7 Cartesian 2D matrix and set up a Pure Front Neighbourhood for 24"
  ) {
    const DualGEx_quad_7 Data = DualGEx_quad_7();
    const Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
      Data.nb_fc,
      Data.adjMatrix_row_ptr,
      Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues,
      Data.volumes,
      Data.centers,
      Data.n_bnd_faces,
      Data.dim,
      Data.anisoCompliantCells
    );
    const CoMMAIndexT seed = 24;
    CoMMAIntT neigh_order = 2;
    const unordered_set<CoMMAIndexT> s_seeds = {seed};
    const CoMMAIntT card = 4;
    const vector<bool> agglomerated = vector<bool>(Data.volumes.size(), false);
    unordered_map<CoMMAIndexT, CoMMAIntT> d_n_of_seed;
    fc_graph.compute_neighbourhood_of_cc(
      s_seeds, neigh_order, d_n_of_seed, card, agglomerated
    );
    const unordered_set<CoMMAIndexT> s_neighbours_of_seed =
      d_keys_to_set<CoMMAIndexT, CoMMAIntT>(d_n_of_seed);
    Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
      neighbourhood(s_neighbours_of_seed, Data.weights, Data.dim);
    neighbourhood.update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first neighbourhood") {
      const auto &fon = neighbourhood.get_candidates();
      THEN("Only direct neighbours are in the neighbourhood") {
        REQUIRE_THAT(fon, UnorderedRangeEquals(CCT{17, 23, 25, 31}));
      }
    }
    // Testing the neighbourhood creator
    const NeighbourhoodPureFrontCreator<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
      neigh_crtor{};
    auto c_neighbourhood =
      neigh_crtor.create(s_neighbours_of_seed, Data.weights, Data.dim);
    c_neighbourhood->update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first neighbourhood of the one got from the creator") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Only direct neighbours are in the neighbourhood") {
        REQUIRE_THAT(fon, UnorderedRangeEquals(CCT{17, 23, 25, 31}));
      }
    }
    neighbourhood.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      const auto &fon = neighbourhood.get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE_THAT(fon, Contains(30));
        REQUIRE_THAT(fon, Contains(32));
        REQUIRE_THAT(fon, Contains(38));
      }
      const auto &prev_fon = neighbourhood.get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE_THAT(prev_fon, !Contains1stElem(31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE_THAT(prev_fon, Contains1stElem(17));
        REQUIRE_THAT(prev_fon, Contains1stElem(23));
        REQUIRE_THAT(prev_fon, Contains1stElem(25));
      }
    }
    c_neighbourhood->update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31 to the creator-provided neighbourhood") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE_THAT(fon, Contains(30));
        REQUIRE_THAT(fon, Contains(32));
        REQUIRE_THAT(fon, Contains(38));
      }
      const auto prev_fon =
        dynamic_pointer_cast<
          Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
          c_neighbourhood
        )
          ->get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE_THAT(prev_fon, !Contains1stElem(31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE_THAT(prev_fon, Contains1stElem(17));
        REQUIRE_THAT(prev_fon, Contains1stElem(23));
        REQUIRE_THAT(prev_fon, Contains1stElem(25));
      }
    }
    auto copy_c_neighbourhood = neigh_crtor.clone(c_neighbourhood);
    // We update after the copy to see if that's really a copy
    c_neighbourhood->update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 31 to the copy of the creator-provided neighbourhood") {
      const auto fon = copy_c_neighbourhood->get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE_THAT(fon, Contains(30));
        REQUIRE_THAT(fon, Contains(32));
        REQUIRE_THAT(fon, Contains(38));
      }
      const auto prev_fon =
        dynamic_pointer_cast<
          Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
          copy_c_neighbourhood
        )
          ->get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE_THAT(prev_fon, !Contains1stElem(31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE_THAT(prev_fon, Contains1stElem(17));
        REQUIRE_THAT(prev_fon, Contains1stElem(23));
        REQUIRE_THAT(prev_fon, Contains1stElem(25));
      }
    }
    neighbourhood.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      const auto &fon = neighbourhood.get_candidates();
      THEN(
        "Direct neighbours of 31 are NOT in the neighbourhood (max order neighbourhood)"
      ) {
        REQUIRE_THAT(fon, !Contains(37));
        REQUIRE_THAT(fon, !Contains(39));
        REQUIRE_THAT(fon, !Contains(45));
      }
      THEN(
        "First ever computed neighbourhood is returned since no direct neighbours were added"
      ) {
        REQUIRE_THAT(fon, Contains(17));
        REQUIRE_THAT(fon, Contains(23));
        REQUIRE_THAT(fon, Contains(25));
      }
      const auto &prev_fon = neighbourhood.get_neighbours_by_level(1);
      THEN("Cell 38 is no more in the previous neighbourhood") {
        REQUIRE_THAT(prev_fon, !Contains1stElem(38));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE_THAT(prev_fon, Contains1stElem(30));
        REQUIRE_THAT(prev_fon, Contains1stElem(32));
      }
      const auto &prev_fon_2 = neighbourhood.get_neighbours_by_level(2);
      THEN(
        "Older neighbours are still in the second-to-last neighbourhood (which happens to be the first ever, hence the current)"
      ) {
        REQUIRE_THAT(prev_fon_2, Contains1stElem(17));
        REQUIRE_THAT(prev_fon_2, Contains1stElem(23));
        REQUIRE_THAT(prev_fon_2, Contains1stElem(25));
      }
    }
  }
}
