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

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <set>
#include <unordered_set>
#include <vector>

#include "CoMMA/CoMMADefs.h"
#include "CoMMA/Util.h"
#include "DualGraphExamples.h"
#include "test_defs.h"

using namespace comma;  // NOLINT
using namespace std;  // NOLINT
using Catch::Matchers::Equals;
using Catch::Matchers::SizeIs;
using Catch::Matchers::WithinAbs;
const auto EMPTY = Catch::Matchers::IsEmpty();

SCENARIO("Test neighbourhood-based wall-distance", "[Wall-distance]") {
  GIVEN("A 7x7 Cartesian 2D matrix") {
    const DualGEx_quad_7 Data = DualGEx_quad_7();
    const vector<CoMMAIndexT> wall = {
      0, 1, 2, 3, 4, 5, 6, 7, 14, 21, 28, 35, 42
    };
    // clang-format off
    // Cells grouped by distance wrt the wall
    const vector<vector<CoMMAIndexT>> distances = {
      {8, 9, 10, 11, 12, 13, 15, 22, 29, 36, 43},
      {16, 17, 18, 19, 20, 23, 30, 37, 44},
      {24, 25, 26, 27, 31, 38, 45},
      {32, 33, 34, 39, 46},
      {40, 41, 47}
    };
    // clang-format on
    WHEN("We compute the neighbourhood-based wall-distance") {
      vector<CoMMAIntT> dist{};
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAIntT>(
        Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, wall, dist
      );
      THEN("Wall") {
        for (const auto &cell : wall) {
          REQUIRE(dist[cell] == 0);
        }
      }
      THEN("First set of neighbours") {
        for (const auto &cell : distances[0]) {
          REQUIRE(dist[cell] == 1);
        }
      }
      THEN("Second set of neighbours") {
        for (const auto &cell : distances[1]) {
          REQUIRE(dist[cell] == 2);
        }
      }
      THEN("Third set of neighbours") {
        for (const auto &cell : distances[2]) {
          REQUIRE(dist[cell] == 3);
        }
      }
      THEN("Fourth set of neighbours") {
        for (const auto &cell : distances[3]) {
          REQUIRE(dist[cell] == 4);
        }
      }
      THEN("Fifth set of neighbours") {
        for (const auto &cell : distances[4]) {
          REQUIRE(dist[cell] == 5);
        }
      }
      THEN("Sixth set of neighbours") { REQUIRE(dist[48] == 6); }
    }

    WHEN("We compute the neighbourhood-based wall-distance (double type)") {
      vector<CoMMAWeightT> dist{};
      constexpr CoMMAWeightT eps = 1e-10;
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAWeightT>(
        Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, wall, dist
      );
      THEN("Wall") {
        for (const auto &cell : wall) {
          REQUIRE_THAT(dist[cell], WithinAbs(0., eps));
        }
      }
      THEN("First set of neighbours") {
        for (const auto &cell : distances[0]) {
          REQUIRE_THAT(dist[cell], WithinAbs(1., eps));
        }
      }
      THEN("Second set of neighbours") {
        for (const auto &cell : distances[1]) {
          REQUIRE_THAT(dist[cell], WithinAbs(2., eps));
        }
      }
      THEN("Third set of neighbours") {
        for (const auto &cell : distances[2]) {
          REQUIRE_THAT(dist[cell], WithinAbs(3., eps));
        }
      }
      THEN("Fourth set of neighbours") {
        for (const auto &cell : distances[3]) {
          REQUIRE_THAT(dist[cell], WithinAbs(4., eps));
        }
      }
      THEN("Fifth set of neighbours") {
        for (const auto &cell : distances[4]) {
          REQUIRE_THAT(dist[cell], WithinAbs(5., eps));
        }
      }
      THEN("Sixth set of neighbours") {
        REQUIRE_THAT(dist[48], WithinAbs(6., eps));
      }
    }
    WHEN("We compute the neighbourhood-based wall-distance wrt to an empty wall"
    ) {
      const vector<CoMMAIndexT> empty_wall{};
      vector<CoMMAWeightT> dist{};
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAWeightT>(
        Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, empty_wall, dist
      );
      THEN("All the distances are negative") {
        REQUIRE(all_of(dist.begin(), dist.end(), [](const auto &d) {
          return d < 0;
        }));
      }
    }
  }
}

SCENARIO("Test compactness computation", "[Compactness]") {
  GIVEN("A simple 3x3 Cartesian grid") {
    const DualGEx_quad_3 Data = DualGEx_quad_3();
    shared_ptr<DualGraphT> const fc_graph = make_shared<DualGraphT>(
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
    WHEN("We consider an empty coarse cell") {
      const unordered_set<CoMMAIndexT> cc = {};
      THEN("The single compactness value are right") {
        REQUIRE_THAT(fc_graph->compute_fc_compactness_inside_a_cc(cc), EMPTY);
      }
      THEN("The minimum is well identified") {
        REQUIRE(fc_graph->compute_min_fc_compactness_inside_a_cc(cc) == 0);
      }
    }
    WHEN("We consider a singular coarse cell") {
      const unordered_set<CoMMAIndexT> cc = {1};
      THEN("The single compactness value are right") {
        const auto cpt = fc_graph->compute_fc_compactness_inside_a_cc(cc);
        REQUIRE_THAT(cpt, SizeIs(1));
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
    const set<PairIntInt, CustomPairGreaterFunctor<PairIntInt>> st = {
      {1, 0}, {1, 0}, {1, 3}, {0, 3}
    };
    WHEN("We have a look at the set:") {
      THEN("The expected order is obtained") {
        auto it = st.begin();
        REQUIRE(*it == PairIntInt{0, 3});
        //
        it++;
        REQUIRE(*it == PairIntInt{1, 3});
        //
        it++;
        REQUIRE(*it == PairIntInt{1, 0});
      }
      THEN("Duplicates are not added") { REQUIRE_THAT(st, SizeIs(3)); }
    }
  }
  GIVEN("Some (int,int) pairs in a set with custom 'Less'") {
    const set<PairIntInt, CustomPairLessFunctor<PairIntInt>> st = {
      {1, 3}, {1, 0}, {1, 0}, {0, 3}
    };
    WHEN("We have a look at the set:") {
      THEN("The expected order is obtained") {
        auto it = st.begin();
        REQUIRE(*it == PairIntInt{1, 0});
        //
        it++;
        REQUIRE(*it == PairIntInt{1, 3});
        //
        it++;
        REQUIRE(*it == PairIntInt{0, 3});
      }
      THEN("Duplicates are not added") { REQUIRE_THAT(st, SizeIs(3)); }
    }
  }
}

SCENARIO("Test cell filtering", "[Cell filtering]") {
  GIVEN("A 2D 4x4 quad mesh") {
    const DualGEx_quad_4 Data = DualGEx_quad_4();
    std::vector<CoMMAIndexT> ref_res(Data.nb_fc);
    std::iota(ref_res.begin(), ref_res.end(), 0);
    WHEN("We provide no neighbours") {
      std::vector<CoMMAIndexT> filtered{};
      filter_cells_by_n_edges<CoMMAIndexT, CoMMAIntT>(
        Data.adjMatrix_row_ptr, Data.n_bnd_faces, {}, filtered
      );
      THEN("The result is empty") { REQUIRE_THAT(filtered, EMPTY); }
    }
    WHEN("We provide an impossible number of neighbours") {
      std::vector<CoMMAIndexT> filtered{};
      filter_cells_by_n_edges<CoMMAIndexT, CoMMAIntT>(
        Data.adjMatrix_row_ptr, Data.n_bnd_faces, {-1, 0}, filtered
      );
      THEN("The result is empty") { REQUIRE_THAT(filtered, EMPTY); }
    }
    WHEN("We ask for cells with 3 neighbours (that is, none)") {
      std::vector<CoMMAIndexT> filtered{};
      filter_cells_by_n_edges<CoMMAIndexT, CoMMAIntT>(
        Data.adjMatrix_row_ptr, Data.n_bnd_faces, {3}, filtered
      );
      THEN("The result is empty") { REQUIRE_THAT(filtered, EMPTY); }
    }
    WHEN("We ask for cells with 4 neighbours (that is, all)") {
      std::vector<CoMMAIndexT> filtered{};
      filter_cells_by_n_edges<CoMMAIndexT, CoMMAIntT>(
        Data.adjMatrix_row_ptr, Data.n_bnd_faces, {4}, filtered
      );
      std::sort(filtered.begin(), filtered.end());
      THEN("All cells are in the final results") {
        REQUIRE_THAT(filtered, SizeIs(Data.nb_fc));
        REQUIRE_THAT(filtered, Equals(ref_res));
      }
    }
    WHEN("We ask for cells with 3 or 4 neighbours (that is, all)") {
      std::vector<CoMMAIndexT> filtered{};
      filter_cells_by_n_edges<CoMMAIndexT, CoMMAIntT>(
        Data.adjMatrix_row_ptr, Data.n_bnd_faces, {3, 4}, filtered
      );
      std::sort(filtered.begin(), filtered.end());
      THEN("All cells are in the final results") {
        REQUIRE_THAT(filtered, SizeIs(Data.nb_fc));
        REQUIRE_THAT(filtered, Equals(ref_res));
      }
    }
  }
}
