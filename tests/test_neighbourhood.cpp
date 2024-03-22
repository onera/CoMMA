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

#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CoMMA/Neighbourhood.h"
#include "DualGraphExamples.h"
#include "catch2/catch.hpp"
#include "test_defs.h"

using namespace comma;  // NOLINT
using namespace std;  // NOLINT

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
      Data.arrayOfFineAnisotropicCompliantCells
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
      THEN("Sizes are good") {
        REQUIRE(neighs[0].size() == 4);
        REQUIRE(neighs[1].size() == 8);
        REQUIRE(neighs[2].size() == 12);
      }
      THEN("First order is good") {
        REQUIRE(CONTAINS_(neighs[0], 17));
        REQUIRE(CONTAINS_(neighs[0], 23));
        REQUIRE(CONTAINS_(neighs[0], 25));
        REQUIRE(CONTAINS_(neighs[0], 31));
      }
      THEN("Second order is good") {
        REQUIRE(CONTAINS_(neighs[1], 10));
        REQUIRE(CONTAINS_(neighs[1], 16));
        REQUIRE(CONTAINS_(neighs[1], 18));
        REQUIRE(CONTAINS_(neighs[1], 22));
        REQUIRE(CONTAINS_(neighs[1], 26));
        REQUIRE(CONTAINS_(neighs[1], 30));
        REQUIRE(CONTAINS_(neighs[1], 32));
        REQUIRE(CONTAINS_(neighs[1], 38));
      }
      THEN("Third order is good") {
        REQUIRE(CONTAINS_(neighs[2], 3));
        REQUIRE(CONTAINS_(neighs[2], 9));
        REQUIRE(CONTAINS_(neighs[2], 11));
        REQUIRE(CONTAINS_(neighs[2], 15));
        REQUIRE(CONTAINS_(neighs[2], 19));
        REQUIRE(CONTAINS_(neighs[2], 21));
        REQUIRE(CONTAINS_(neighs[2], 27));
        REQUIRE(CONTAINS_(neighs[2], 29));
        REQUIRE(CONTAINS_(neighs[2], 33));
        REQUIRE(CONTAINS_(neighs[2], 37));
        REQUIRE(CONTAINS_(neighs[2], 39));
        REQUIRE(CONTAINS_(neighs[2], 45));
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
      THEN("Sizes are good") {
        REQUIRE(neighs[0].size() == 3);
        REQUIRE(neighs[1].size() == 3);
        REQUIRE(neighs[2].size() == 5);
      }
      THEN("First order is good") {
        REQUIRE(CONTAINS_(neighs[0], 17));
        REQUIRE(CONTAINS_(neighs[0], 23));
        REQUIRE(CONTAINS_(neighs[0], 25));
      }
      THEN("Second order is good") {
        REQUIRE(CONTAINS_(neighs[1], 18));
        REQUIRE(CONTAINS_(neighs[1], 22));
        REQUIRE(CONTAINS_(neighs[1], 26));
      }
      THEN("Third order is good") {
        REQUIRE(CONTAINS_(neighs[2], 11));
        REQUIRE(CONTAINS_(neighs[2], 15));
        REQUIRE(CONTAINS_(neighs[2], 19));
        REQUIRE(CONTAINS_(neighs[2], 21));
        REQUIRE(CONTAINS_(neighs[2], 27));
      }
    }  // WHEN PREVIOUS AGGLOMERATION
    WHEN("We compute the neighbourhood of a coarse cell") {
      vector<bool> agglomerated = vector<bool>(Data.nb_fc, false);
      const unordered_set<CoMMAIndexT> cc = {16, 17, 18, 23, 24};
      THEN("The whole neighbourhood is returned if no cell is agglomerated") {
        auto neighs = fc_graph.get_neighbourhood_of_cc(cc, agglomerated);
        REQUIRE(neighs.size() == 9);
        REQUIRE(CONTAINS_(neighs, 15));
        REQUIRE(CONTAINS_(neighs, 22));
        REQUIRE(CONTAINS_(neighs, 30));
        REQUIRE(CONTAINS_(neighs, 31));
        REQUIRE(CONTAINS_(neighs, 25));
        REQUIRE(CONTAINS_(neighs, 19));
        REQUIRE(CONTAINS_(neighs, 11));
        REQUIRE(CONTAINS_(neighs, 10));
        REQUIRE(CONTAINS_(neighs, 9));
      }
      agglomerated[15] = true;
      agglomerated[9] = true;
      agglomerated[19] = true;
      THEN(
        "If some cells are agglomerated, then they do not appear in the neighbourhood"
      ) {
        auto neighs = fc_graph.get_neighbourhood_of_cc(cc, agglomerated);
        REQUIRE(neighs.size() == 6);
        REQUIRE(CONTAINS_(neighs, 22));
        REQUIRE(CONTAINS_(neighs, 30));
        REQUIRE(CONTAINS_(neighs, 31));
        REQUIRE(CONTAINS_(neighs, 25));
        REQUIRE(CONTAINS_(neighs, 11));
        REQUIRE(CONTAINS_(neighs, 10));
      }
    }
  }
  GIVEN(
    "We have a 7x7 Cartesian 2D matrix, a standard Neighbourhood for 24 a one given by the creator"
  ) {
#define CONTAINS_1STEL_(cont, obj)                                      \
  (find_if((cont).begin(), (cont).end(), CoMMAPairFindFirstBasedT(obj)) \
   != (cont).end())
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
      Data.arrayOfFineAnisotropicCompliantCells
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
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
        REQUIRE(CONTAINS_(fon, 31));
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
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
        REQUIRE(CONTAINS_(fon, 31));
      }
    }
    neighbourhood.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      const auto &fon = neighbourhood.get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE(!CONTAINS_(fon, 31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE(CONTAINS_(fon, 30));
        REQUIRE(CONTAINS_(fon, 32));
        REQUIRE(CONTAINS_(fon, 38));
      }
    }
    c_neighbourhood->update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31 to the creator-provided neighbourhood") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE(!CONTAINS_(fon, 31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE(CONTAINS_(fon, 30));
        REQUIRE(CONTAINS_(fon, 32));
        REQUIRE(CONTAINS_(fon, 38));
      }
    }
    auto copy_c_neighbourhood = neigh_crtor.clone(c_neighbourhood);
    // We update after the copy to see if that's really a copy
    c_neighbourhood->update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 31 to the copy of the creator-provided neighbourhood") {
      const auto fon = copy_c_neighbourhood->get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE(!CONTAINS_(fon, 31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE(CONTAINS_(fon, 30));
        REQUIRE(CONTAINS_(fon, 32));
        REQUIRE(CONTAINS_(fon, 38));
      }
    }
    neighbourhood.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      const auto &fon = neighbourhood.get_candidates();
      THEN("Cell 38 is no more in the neighbourhood") {
        REQUIRE(!CONTAINS_(fon, 38));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
        REQUIRE(CONTAINS_(fon, 30));
        REQUIRE(CONTAINS_(fon, 32));
      }
      THEN(
        "Direct neighbours of 31 are NOT in the neighbourhood (max order neighbourhood)"
      ) {
        REQUIRE(!CONTAINS_(fon, 37));
        REQUIRE(!CONTAINS_(fon, 39));
        REQUIRE(!CONTAINS_(fon, 45));
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
      Data.arrayOfFineAnisotropicCompliantCells
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
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
        REQUIRE(CONTAINS_(fon, 31));
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
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
        REQUIRE(CONTAINS_(fon, 31));
      }
    }
    neighbourhood.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      const auto &fon = neighbourhood.get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE(CONTAINS_(fon, 30));
        REQUIRE(CONTAINS_(fon, 32));
        REQUIRE(CONTAINS_(fon, 38));
      }
      const auto &prev_fon = neighbourhood.get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE(!CONTAINS_1STEL_(prev_fon, 31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE(CONTAINS_1STEL_(prev_fon, 17));
        REQUIRE(CONTAINS_1STEL_(prev_fon, 23));
        REQUIRE(CONTAINS_1STEL_(prev_fon, 25));
      }
    }
    c_neighbourhood->update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31 to the creator-provided neighbourhood") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE(CONTAINS_(fon, 30));
        REQUIRE(CONTAINS_(fon, 32));
        REQUIRE(CONTAINS_(fon, 38));
      }
      const auto prev_fon =
        dynamic_pointer_cast<
          Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
          c_neighbourhood
        )
          ->get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE(!CONTAINS_1STEL_(prev_fon, 31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE(CONTAINS_1STEL_(prev_fon, 17));
        REQUIRE(CONTAINS_1STEL_(prev_fon, 23));
        REQUIRE(CONTAINS_1STEL_(prev_fon, 25));
      }
    }
    auto copy_c_neighbourhood = neigh_crtor.clone(c_neighbourhood);
    // We update after the copy to see if that's really a copy
    c_neighbourhood->update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 31 to the copy of the creator-provided neighbourhood") {
      const auto fon = copy_c_neighbourhood->get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE(CONTAINS_(fon, 30));
        REQUIRE(CONTAINS_(fon, 32));
        REQUIRE(CONTAINS_(fon, 38));
      }
      const auto prev_fon =
        dynamic_pointer_cast<
          Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
          copy_c_neighbourhood
        )
          ->get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE(!CONTAINS_1STEL_(prev_fon, 31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE(CONTAINS_1STEL_(prev_fon, 17));
        REQUIRE(CONTAINS_1STEL_(prev_fon, 23));
        REQUIRE(CONTAINS_1STEL_(prev_fon, 25));
      }
    }
    neighbourhood.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      const auto &fon = neighbourhood.get_candidates();
      THEN(
        "Direct neighbours of 31 are NOT in the neighbourhood (max order neighbourhood)"
      ) {
        REQUIRE(!CONTAINS_(fon, 37));
        REQUIRE(!CONTAINS_(fon, 39));
        REQUIRE(!CONTAINS_(fon, 45));
      }
      THEN(
        "First ever computed neighbourhood is returned since no direct neighbours were added"
      ) {
        REQUIRE(CONTAINS_(fon, 17));
        REQUIRE(CONTAINS_(fon, 23));
        REQUIRE(CONTAINS_(fon, 25));
      }
      const auto &prev_fon = neighbourhood.get_neighbours_by_level(1);
      THEN("Cell 38 is no more in the previous neighbourhood") {
        REQUIRE(!CONTAINS_1STEL_(prev_fon, 38));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE(CONTAINS_1STEL_(prev_fon, 30));
        REQUIRE(CONTAINS_1STEL_(prev_fon, 32));
      }
      const auto &prev_fon_2 = neighbourhood.get_neighbours_by_level(2);
      THEN(
        "Older neighbours are still in the second-to-last neighbourhood (which happens to be the first ever, hence the current)"
      ) {
        REQUIRE(CONTAINS_1STEL_(prev_fon_2, 17));
        REQUIRE(CONTAINS_1STEL_(prev_fon_2, 23));
        REQUIRE(CONTAINS_1STEL_(prev_fon_2, 25));
      }
    }
#undef CONTAINS_1STEL_
  }
}
