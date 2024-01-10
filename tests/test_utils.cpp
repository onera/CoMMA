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

#include <set>
#include <unordered_set>
#include <vector>

#include "CoMMATypes.h"
#include "Util.h"
#include "catch2/catch.hpp"
#include "input/DualGPy.h"
#include "test_defs.h"

using namespace comma;
using namespace std;

SCENARIO("Test neighbourhood-based wall-distance", "[Wall-distance]") {
  GIVEN("A 7x7 Cartesian 2D matrix") {
    const DualGPy_quad_7 Data = DualGPy_quad_7();
    const vector<CoMMAIndexT> wall = {0, 1,  2,  3,  4,  5, 6,
                                      7, 14, 21, 28, 35, 42};
    WHEN("We compute the neighbourhood-based wall-distance") {
      vector<CoMMAIntT> dist{};
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAIntT>(
        Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, wall, dist);
      THEN("Wall") {
        for (const auto &cell : wall) {
          REQUIRE(dist[cell] == 0);
        }
      }
      THEN("First set of neighbours") {
        const vector<CoMMAIndexT> cells = {8,  9,  10, 11, 12, 13,
                                           15, 22, 29, 36, 43};
        for (const auto &cell : cells) {
          REQUIRE(dist[cell] == 1);
        }
      }
      THEN("Second set of neighbours") {
        const vector<CoMMAIndexT> cells = {16, 17, 18, 19, 20, 23, 30, 37, 44};
        for (const auto &cell : cells) {
          REQUIRE(dist[cell] == 2);
        }
      }
      THEN("Third set of neighbours") {
        const vector<CoMMAIndexT> cells = {24, 25, 26, 27, 31, 38, 45};
        for (const auto &cell : cells) {
          REQUIRE(dist[cell] == 3);
        }
      }
      THEN("Fourth set of neighbours") {
        const vector<CoMMAIndexT> cells = {32, 33, 34, 39, 46};
        for (const auto &cell : cells) {
          REQUIRE(dist[cell] == 4);
        }
      }
      THEN("Fifth set of neighbours") {
        const vector<CoMMAIndexT> cells = {40, 41, 47};
        for (const auto &cell : cells) {
          REQUIRE(dist[cell] == 5);
        }
      }
      THEN("Sixth set of neighbours") { REQUIRE(dist[48] == 6); }
    }

    WHEN("We compute the neighbourhood-based wall-distance (double type)") {
      vector<CoMMAWeightT> dist{};
      constexpr CoMMAWeightT eps = 1e-10;
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAWeightT>(
        Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, wall, dist);
      THEN("Wall") {
        for (const auto &cell : wall) {
          REQUIRE(equal_up_to(dist[cell], 0., eps));
        }
      }
      THEN("First set of neighbours") {
        const vector<CoMMAIndexT> cells = {8,  9,  10, 11, 12, 13,
                                           15, 22, 29, 36, 43};
        for (const auto &cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 1., eps));
        }
      }
      THEN("Second set of neighbours") {
        const vector<CoMMAIndexT> cells = {16, 17, 18, 19, 20, 23, 30, 37, 44};
        for (const auto &cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 2., eps));
        }
      }
      THEN("Third set of neighbours") {
        const vector<CoMMAIndexT> cells = {24, 25, 26, 27, 31, 38, 45};
        for (const auto &cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 3., eps));
        }
      }
      THEN("Fourth set of neighbours") {
        const vector<CoMMAIndexT> cells = {32, 33, 34, 39, 46};
        for (const auto &cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 4., eps));
        }
      }
      THEN("Fifth set of neighbours") {
        const vector<CoMMAIndexT> cells = {40, 41, 47};
        for (const auto &cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 5., eps));
        }
      }
      THEN("Sixth set of neighbours") {
        REQUIRE(equal_up_to(dist[48], 6., eps));
      }
    }
    WHEN(
      "We compute the neighbourhood-based wall-distance wrt to an empty wall") {
      const vector<CoMMAIndexT> empty_wall{};
      vector<CoMMAWeightT> dist{};
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAWeightT>(
        Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, empty_wall, dist);
      THEN("All the distances are negative") {
        for (const auto &d : dist) {
          REQUIRE(d < 0);
        }
      }
    }
  }
}

SCENARIO("Test compactness computation", "[Compactness]") {
  GIVEN("A simple 3x3 Cartesian grid") {
    const DualGPy_quad_3 Data = DualGPy_quad_3();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
      Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces,
      Data.dim, Data.arrayOfFineAnisotropicCompliantCells);
    WHEN("We consider an empty coarse cell") {
      const unordered_set<CoMMAIndexT> cc = {};
      THEN("The single compactness value are right") {
        REQUIRE(fc_graph->compute_fc_compactness_inside_a_cc(cc).empty());
      }
      THEN("The minimum is well identified") {
        REQUIRE(fc_graph->compute_min_fc_compactness_inside_a_cc(cc) == 0);
      }
    }
    WHEN("We consider a singular coarse cell") {
      const unordered_set<CoMMAIndexT> cc = {1};
      THEN("The single compactness value are right") {
        const auto cpt = fc_graph->compute_fc_compactness_inside_a_cc(cc);
        REQUIRE(cpt.size() == 1);
        REQUIRE(cpt.at(1) == 0);
      }
      THEN("The minimum is well identified") {
        REQUIRE(fc_graph->compute_min_fc_compactness_inside_a_cc(cc) == 0);
      }
    }
    WHEN("We consider an L-shaped coarse cell") {
      const unordered_set<CoMMAIndexT> cc = {6, 3, 0, 1, 2};
      THEN("The single compactness values are right") {
        const auto cpt = fc_graph->compute_fc_compactness_inside_a_cc(cc);
        REQUIRE(cpt.at(6) == 1);
        REQUIRE(cpt.at(3) == 2);
        REQUIRE(cpt.at(0) == 2);
        REQUIRE(cpt.at(1) == 2);
        REQUIRE(cpt.at(2) == 1);
      }
      THEN("The minimum is well identified") {
        REQUIRE(fc_graph->compute_min_fc_compactness_inside_a_cc(cc) == 1);
      }
    }
    WHEN("We consider the whole mesh as coarse cell") {
      const unordered_set<CoMMAIndexT> cc = {0, 1, 2, 3, 4, 5, 6, 7, 8};
      THEN("The single compactness values are right") {
        const auto cpt = fc_graph->compute_fc_compactness_inside_a_cc(cc);
        REQUIRE(cpt.at(0) == 2);
        REQUIRE(cpt.at(1) == 3);
        REQUIRE(cpt.at(2) == 2);
        REQUIRE(cpt.at(3) == 3);
        REQUIRE(cpt.at(4) == 4);
        REQUIRE(cpt.at(5) == 3);
        REQUIRE(cpt.at(6) == 2);
        REQUIRE(cpt.at(7) == 3);
        REQUIRE(cpt.at(8) == 2);
      }
      THEN("The minimum is well identified") {
        REQUIRE(fc_graph->compute_min_fc_compactness_inside_a_cc(cc) == 2);
      }
    }
  }
}

SCENARIO("Test custom pair comparison", "[Pair comparison]") {
  using PairIntInt = pair<int, int>;
  GIVEN("Some (int,int) pairs in a set with custom 'Greater'") {
    set<PairIntInt, CustomPairGreaterFunctor<PairIntInt>> s = {
      {1, 0}, {1, 0}, {1, 3}, {0, 3}};
    WHEN("We have a look at the set:") {
      THEN("The expected order is obtained") {
        auto it = s.begin();
        REQUIRE(it->first == 0);
        REQUIRE(it->second == 3);
        //
        it++;
        REQUIRE(it->first == 1);
        REQUIRE(it->second == 3);
        //
        it++;
        REQUIRE(it->first == 1);
        REQUIRE(it->second == 0);
      }
      THEN("Duplicates are not added") { REQUIRE(s.size() == 3); }
    }
  }
  GIVEN("Some (int,int) pairs in a set with custom 'Less'") {
    set<PairIntInt, CustomPairLessFunctor<PairIntInt>> s = {
      {1, 3}, {1, 0}, {1, 0}, {0, 3}};
    WHEN("We have a look at the set:") {
      THEN("The expected order is obtained") {
        auto it = s.begin();
        REQUIRE(it->first == 1);
        REQUIRE(it->second == 0);
        //
        it++;
        REQUIRE(it->first == 1);
        REQUIRE(it->second == 3);
        //
        it++;
        REQUIRE(it->first == 0);
        REQUIRE(it->second == 3);
      }
      THEN("Duplicates are not added") { REQUIRE(s.size() == 3); }
    }
  }
}
