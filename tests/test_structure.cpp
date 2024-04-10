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

#define CATCH_CONFIG_MAIN

#include <optional>
#include <vector>

#include "CoMMA/Args.h"
#include "CoMMA/CoMMA.h"
#include "DualGraphExamples.h"
#include "catch2/catch.hpp"
#include "test_defs.h"

using namespace comma;  // NOLINT
using namespace std;  // NOLINT

#define MAX_CELLS_IN_LINE std::nullopt

SCENARIO("Test of a structure", "[structure]") {
  GIVEN("A simple graph, and we build the Dual Graph") {
    const DualGEx Data = DualGEx();
    // Construction of the Dual Graph element
    shared_ptr<SeedsPoolT> seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
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
    shared_ptr<CCContainerT> cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    // Check the effective length
    WHEN("We try to access to the member variables") {
      class test :
        public Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> {
      public:
        test(
          shared_ptr<DualGraphT> &graph,
          shared_ptr<CCContainerT> &cc_graph,
          shared_ptr<SeedsPoolT> &seeds_pool,
          CoMMAIntT dimension
        ) :
          Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
            graph,
            cc_graph,
            seeds_pool,
            dimension,
            CoMMANeighbourhoodT::EXTENDED,
            FC_ITER
          ){};

        CoMMAIntT test_variable() { return (this->_threshold_card); }
      };

      THEN("We see that the agglomeration is not set, hence set to 0") {
        test agg = test(fc_graph, cc_graph, seeds_pool, 2);
        const CoMMAIntT testing = agg.test_variable();
        REQUIRE(testing == 0);
      }
    }
    WHEN("We try to access to Define the cardinality") {
      class test :
        public Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> {
      public:
        test(
          shared_ptr<DualGraphT> &graph,
          shared_ptr<CCContainerT> &cc_graph,
          shared_ptr<SeedsPoolT> &seeds_pool,
          CoMMAIntT dimension
        ) :
          Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
            graph,
            cc_graph,
            seeds_pool,
            dimension,
            CoMMANeighbourhoodT::EXTENDED,
            FC_ITER
          ){};

        CoMMAIntT thres() { return (_threshold_card); }

        CoMMAIntT max() { return (_max_card); }

        CoMMAIntT min() { return (_min_card); }
      };

      THEN("We see that the cardinality passes from 0 to 2") {
        test agg = test(fc_graph, cc_graph, seeds_pool, 2);

        REQUIRE(agg.thres() == 0);
        REQUIRE(agg.max() == 0);
        REQUIRE(agg.min() == 0);
        agg.set_agglomeration_parameter(2, 2, 2);
        REQUIRE(agg.thres() == 2);
        REQUIRE(agg.max() == 2);
        REQUIRE(agg.min() == 2);
      }
    }
  }
}

SCENARIO("Test of main function", "[structure]") {
  // The following tests are basically a copy of test_isoagglo but with the
  // main function
  GIVEN("A simple 4x4 Cartesian mesh") {
    const DualGEx_quad_4 Data = DualGEx_quad_4();
    const vector<CoMMAIndexT> ref_results(
      {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3}
    );
    WHEN("We agglomerate with neighbourhood priority") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool aniso = false, build_lines = true, odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAWeightT aniso_thr = -4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.anisoCompliantCells,
        Data.n_bnd_faces,
        build_lines,
        aniso,
        odd_length,
        aniso_thr,
        seed,
        fc2cc,
        alines_idx,
        alines,
        correction,
        Data.dim,
        goal_card,
        min_card,
        max_card
      );
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; ++i)
          REQUIRE(fc2cc[i] == ref_results[i]);
      }
    }
    // It is the same test as above but we use the API with parameter classes
    WHEN("We agglomerate with neighbourhood priority and the parameter API") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool is_aniso = false, correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY;
      const GraphArgs<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> graph(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.n_bnd_faces,
        Data.dim
      );
      const AgglomerationArgs<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> agglo(
        goal_card, min_card, max_card, correction, seed
      );
      const AnisotropicArgs<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso(
        is_aniso, Data.anisoCompliantCells
      );
      // Agglomerate
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        graph, agglo, aniso, fc2cc, alines_idx, alines
      );
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; ++i)
          REQUIRE(fc2cc[i] == ref_results[i]);
      }
    }
    WHEN("We agglomerate with boundary priority") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool aniso = false, build_lines = true, odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAWeightT aniso_thr = -4.;
      const auto seed = CoMMASeedsPoolT::BOUNDARY_PRIORITY;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.anisoCompliantCells,
        Data.n_bnd_faces,
        build_lines,
        aniso,
        odd_length,
        aniso_thr,
        seed,
        fc2cc,
        alines_idx,
        alines,
        correction,
        Data.dim,
        goal_card,
        min_card,
        max_card
      );
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; ++i)
          REQUIRE(fc2cc[i] == ref_results[i]);
      }
    }
    WHEN("We agglomerate with neighbourhood priority one point initialization"
    ) {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool aniso = false, build_lines = true, odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAWeightT aniso_thr = -4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY_ONE_POINT_INIT;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.anisoCompliantCells,
        Data.n_bnd_faces,
        build_lines,
        aniso,
        odd_length,
        aniso_thr,
        seed,
        fc2cc,
        alines_idx,
        alines,
        correction,
        Data.dim,
        goal_card,
        min_card,
        max_card
      );
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; ++i)
          REQUIRE(fc2cc[i] == ref_results[i]);
      }
    }
    WHEN("We agglomerate with boundary priority one point initialization") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool aniso = false, build_lines = true, odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAWeightT aniso_thr = -4.;
      const auto seed = CoMMASeedsPoolT::BOUNDARY_PRIORITY_ONE_POINT_INIT;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.anisoCompliantCells,
        Data.n_bnd_faces,
        build_lines,
        aniso,
        odd_length,
        aniso_thr,
        seed,
        fc2cc,
        alines_idx,
        alines,
        correction,
        Data.dim,
        goal_card,
        min_card,
        max_card
      );
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; ++i)
          REQUIRE(fc2cc[i] == ref_results[i]);
      }
    }
  }
  GIVEN("a 4by7 quad 2D mesh which has 4 anisotropic lines") {
    const DualGEx_aniso_3cell Data = DualGEx_aniso_3cell();
    const vector<CoMMAIndexT> ref_results({0, 0, 1, 1, 1, 8, 8, 3, 3, 3,
                                           2, 2, 5, 5, 4, 5, 4, 9, 9, 7,
                                           6, 7, 6, 7, 8, 8, 9, 9});
    // The following tests are basically a copy of test_isoagglo but with the
    // main function
    WHEN(
      "We agglomerate with anisotropy activated, boundary priority and, full initialization"
    ) {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool aniso = true, build_lines = true, odd_length = ODD_LINE_LENGTH,
                 correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAWeightT aniso_thr = 4.;
      const auto seed = CoMMASeedsPoolT::BOUNDARY_PRIORITY;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.anisoCompliantCells,
        Data.n_bnd_faces,
        build_lines,
        aniso,
        odd_length,
        aniso_thr,
        seed,
        fc2cc,
        alines_idx,
        alines,
        correction,
        Data.dim,
        goal_card,
        min_card,
        max_card
      );
      THEN("The result is 4 anisotropic lines and 2 isotropic cells") {
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; ++i)
          REQUIRE(fc2cc[i] == ref_results[i]);
      }
    }
    WHEN(
      "We agglomerate with anisotropy activated, boundary priority and, full initialization using the parameter classes"
    ) {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool is_aniso = true, build_lines = true,
                 odd_length = ODD_LINE_LENGTH, correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAWeightT aniso_thr = 4.;
      const auto seed = CoMMASeedsPoolT::BOUNDARY_PRIORITY;
      const GraphArgs<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> graph(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.n_bnd_faces,
        Data.dim
      );
      const AgglomerationArgs<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> agglo(
        goal_card, min_card, max_card, correction, seed
      );
      const AnisotropicArgs<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso(
        is_aniso, Data.anisoCompliantCells, build_lines, odd_length, aniso_thr
      );
      // Agglomerate
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        graph, agglo, aniso, fc2cc, alines_idx, alines
      );
      THEN("The result is 4 anisotropic lines and 2 isotropic cells") {
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; ++i)
          REQUIRE(fc2cc[i] == ref_results[i]);
      }
    }
    WHEN("We agglomerate with bad argument") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool aniso = true, build_lines = true, odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAWeightT aniso_thr = 4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY;
      // clang-format off
      // Off to highlight which parameter should be responsible for the throw
      THEN("CoMMA throws if invalid arguments") {
        // Bad dimension: only 2 or 3
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              5,
              goal_card, min_card, max_card)
        );
        vector<CoMMAIndexT> tmp = {};
        // Bad graph definition: indirection
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              tmp,
              Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, goal_card, min_card, max_card)
        );
        tmp.push_back(2);
        // Bad graph definition: indirection
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              tmp,
              Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes, Data.centers,
              Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces, build_lines,
              aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction, Data.dim,
              goal_card, min_card, max_card)
        );
        // Bad graph definition: values do not correspond to indirection
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr,
              tmp,
              Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.weights,
              Data.anisoCompliantCells, Data.n_bnd_faces, build_lines, aniso,
              odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction, Data.dim, goal_card,
              min_card, max_card)
        );
        // Bad graph definition: values do not correspond to indirection
        const vector<CoMMAWeightT> tmp_w = {};
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
              tmp_w,
              Data.volumes, Data.centers, Data.weights, Data.anisoCompliantCells,
              Data.n_bnd_faces, build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx,
              alines, correction, Data.dim, goal_card, min_card, max_card)
        );
        // Bad cardinality: min higher than goal
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, goal_card,
              goal_card + 1,
              max_card)
        );
        // Bad cardinality: goal cardinality is 0
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim,
              0,
              min_card, max_card)
        );
        // Bad anisotropic line definition
        vector<CoMMAIndexT> tmp_idx = {0, 0};
        vector<CoMMAIndexT> tmp_lines = {4};
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces,
              false,
              aniso, odd_length, aniso_thr, seed, fc2cc,
              tmp_idx, tmp_lines,
              correction, Data.dim, goal_card, min_card, max_card)
        );
        // Bad cardinality: singular cardinality is 0
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, goal_card, min_card, max_card,
              0)
        );
        // Bad iteration number: is 0
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, goal_card, min_card, max_card, SING_CARD_THRESH, MAX_CELLS_IN_LINE,
              0)
        );
        // Bad iteration number: greater than threshold
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.anisoCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, goal_card, min_card, max_card, SING_CARD_THRESH, MAX_CELLS_IN_LINE,
              comma::iter_agglo_max_iter + 1)
        );
      }
      // clang-format on
    }
  }
}
