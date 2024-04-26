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
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "CoMMA/ARComputer.h"
#include "CoMMA/Dual_Graph.h"
#include "DualGraphExamples.h"
#include "test_defs.h"

using namespace comma;  // NOLINT
using namespace std;  // NOLINT
using Catch::Matchers::Approx;
using Catch::Matchers::WithinAbs;

using ARCT = ARComputer<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using CFT = CellFeatures<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;

#define CHECK_BASE_CELL_FEATURES(this, that, eps)                 \
  REQUIRE_THAT((this)._measure, WithinAbs((that)._measure, eps)); \
  REQUIRE((this)._n_internal_faces == (that)._n_internal_faces);

#define CHECK_WEIGHTS_CELL_FEATURES(this, that, eps)                   \
  REQUIRE_THAT(                                                        \
    (this)._external_weights, WithinAbs((that)._external_weights, eps) \
  );                                                                   \
  REQUIRE_THAT(                                                        \
    (this)._internal_weights, WithinAbs((that)._internal_weights, eps) \
  );

#define CHECK_GEOM_CELL_FEATURES(this, that, eps)                 \
  REQUIRE_THAT((this)._sq_diam, WithinAbs((that)._sq_diam, eps)); \
  REQUIRE_THAT((this)._sq_min_edge, WithinAbs((that)._sq_min_edge, eps));

SCENARIO("Test CellFeatures", "[CellFeatures]") {
  GIVEN("A 2D 4x4 Cartesian mesh") {
    const DualGEx_quad_4 Data = DualGEx_quad_4();
    shared_ptr<DualGraphT> const graph = make_shared<DualGraphT>(
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
    WHEN("We create a CellFeatures object using the dual graph") {
      constexpr CoMMAIndexT idx = 0;
      const CFT feats(idx, graph);
      const vector<CoMMAWeightT> ref_center = {0.5, 0.5};
      THEN("Quantities are as expected") {
        REQUIRE_THAT(feats._measure, WithinAbs(1., eps));
        REQUIRE_THAT(feats._external_weights, WithinAbs(4., eps));
        REQUIRE_THAT(feats._sum_centers, Approx(ref_center).margin(eps));
        REQUIRE(feats._external_facets.at(idx) == 4);
      }
    }  // When
  }  // Given
}  // Scenario

SCENARIO("Test aspect-ration computations", "[ARComputer]") {  // NOLINT
  GIVEN("A 2D 4x4 Cartesian mesh") {
    const DualGEx_quad_4 Data = DualGEx_quad_4();
    shared_ptr<DualGraphT> graph = make_shared<DualGraphT>(
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
    const map<CoMMAAspectRatioT, shared_ptr<ARCT>> all_ARCs({
      {DIAMETER_OVER_RADIUS,
       make_shared<ARDiamOverRadius<CoMMAIndexT, CoMMAWeightT, CoMMAIntT, 2>>(
         graph
       )},
      {DIAMETER_OVER_MIN_EDGE,
       make_shared<ARDiamOverMinEdge<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
         graph
       )},
      {DIAMETER,
       make_shared<ARDiameter<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(graph)},
      {ONE_OVER_MEASURE,
       make_shared<AROverMeasure<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(graph)},
      {ONE_OVER_INTERNAL_WEIGHTS,
       make_shared<AROverInternalWeights<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
         graph
       )},
      {PERIMETER_OVER_RADIUS,
       make_shared<
         ARExternalWeightOverRadius<CoMMAIndexT, CoMMAWeightT, CoMMAIntT, 2>>(
         graph
       )},
      {EXTERNAL_WEIGHTS,
       make_shared<ARExternalWeights<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
         graph
       )},
      {MAX_BARY_DIST_OVER_RADIUS,
       make_shared<
         ARMaxBaryDistanceOverRadius<CoMMAIndexT, CoMMAWeightT, CoMMAIntT, 2>>(
         graph
       )},
      {MAX_OVER_MIN_BARY_DIST,
       make_shared<
         ARMaxOverMinBaryDistance<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
         graph, 1e-12
       )},
      {ALGEBRAIC_PERIMETER_OVER_MEASURE,
       make_shared<
         ARExternalWeightOverRadius<CoMMAIndexT, CoMMAWeightT, CoMMAIntT, 1>>(
         graph
       )},
    });
    WHEN("We create a cell and try to add some fine-cells") {
      CoMMAIndexT idx = 0, new_fc = 1;
      unordered_set<CoMMAIndexT> cc = {idx};
      CFT feats(idx, graph);
      CFT tmp_feats{};
      CFT ref_feats(2., 6., 1., 1, 1., 1., {2., 1.}, {{idx, 3}, {new_fc, 3}});
      CoMMAIntT n_shared_faces{};
      CoMMAWeightT ar{};
      // Case with basic features
      all_ARCs.at(DIAMETER)->compute_and_update_features(
        new_fc, feats, cc, n_shared_faces, ar, tmp_feats
      );
      THEN("2 cells - Base features OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_GEOM_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE(n_shared_faces == 1);
      }
      // Case with weights features
      all_ARCs.at(EXTERNAL_WEIGHTS)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("2 cells - Weights features are OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_WEIGHTS_CELL_FEATURES(tmp_feats, ref_feats, eps);
      }
      // Case with barycentric features
      all_ARCs.at(MAX_OVER_MIN_BARY_DIST)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("2 cells - barycenter info is OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE_THAT(
          ref_feats._sum_centers, Approx(tmp_feats._sum_centers).margin(eps)
        );
        for (auto &[idx, nef] : tmp_feats._external_facets) {
          REQUIRE(nef == ref_feats._external_facets.at(idx));
        }
      }  // Then

      // Adding one cell
      cc.insert(new_fc);
      feats = ref_feats;
      new_fc = 5;
      ref_feats._measure += graph->_volumes[new_fc];
      ref_feats._external_weights = 8.;
      ref_feats._internal_weights += 1.;
      ref_feats._n_internal_faces += 1;
      // ref_feats._sq_min_edge = 1.: // Does not change
      ref_feats._sq_diam = 2.;
      for (auto xyz = decltype(ref_feats._sum_centers.size()){0};
           xyz < ref_feats._sum_centers.size();
           ++xyz)
        ref_feats._sum_centers[xyz] +=
          graph->_volumes[new_fc] * graph->_centers[new_fc][xyz];
      ref_feats._external_facets.insert({new_fc, 3});
      ref_feats._external_facets.at(1) -= 1;
      // Case with basic features
      all_ARCs.at(DIAMETER)->compute_and_update_features(
        new_fc, feats, cc, n_shared_faces, ar, tmp_feats
      );
      THEN("3 cells - Base features OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_GEOM_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE(n_shared_faces == 1);
      }
      // Case with weights features
      all_ARCs.at(EXTERNAL_WEIGHTS)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("3 cells - Weights features are OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_WEIGHTS_CELL_FEATURES(tmp_feats, ref_feats, eps);
      }
      // Case with barycentric features
      all_ARCs.at(MAX_OVER_MIN_BARY_DIST)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("3 cells - barycenter info is OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE_THAT(
          tmp_feats._sum_centers, Approx(ref_feats._sum_centers).margin(eps)
        );
        for (auto &[idx, nef] : tmp_feats._external_facets) {
          REQUIRE(nef == ref_feats._external_facets.at(idx));
        }
      }  // Then

      // Adding one cell
      cc.insert(new_fc);
      feats = ref_feats;
      new_fc = 4;
      ref_feats._measure += graph->_volumes[new_fc];
      // ref_feats._external_weights = 8.; // Does not change
      ref_feats._internal_weights += 2.;
      ref_feats._n_internal_faces += 2;
      // ref_feats._sq_min_edge = 1.: // Does not change
      // ref_feats._sq_diam = 2.; // Does not change
      for (auto xyz = decltype(ref_feats._sum_centers.size()){0};
           xyz < ref_feats._sum_centers.size();
           ++xyz)
        ref_feats._sum_centers[xyz] +=
          graph->_volumes[new_fc] * graph->_centers[new_fc][xyz];
      for (auto &ef : ref_feats._external_facets)
        ef.second = 2;
      ref_feats._external_facets.insert({new_fc, 2});
      // Case with basic features
      all_ARCs.at(DIAMETER)->compute_and_update_features(
        new_fc, feats, cc, n_shared_faces, ar, tmp_feats
      );
      THEN("4 cells - Base features OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_GEOM_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE(n_shared_faces == 2);
      }
      // Case with weights features
      all_ARCs.at(EXTERNAL_WEIGHTS)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("4 cells - Weights features are OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_WEIGHTS_CELL_FEATURES(tmp_feats, ref_feats, eps);
      }
      // Case with barycentric features
      all_ARCs.at(MAX_OVER_MIN_BARY_DIST)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("4 cells - barycenter info is OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE_THAT(
          tmp_feats._sum_centers, Approx(ref_feats._sum_centers).margin(eps)
        );
        for (auto &[idx, nef] : tmp_feats._external_facets) {
          REQUIRE(nef == ref_feats._external_facets.at(idx));
        }
      }  // Then

      const map<CoMMAAspectRatioT, CoMMAWeightT> expected_AR({
        // Squared
        {DIAMETER_OVER_RADIUS, ref_feats._sq_diam / ref_feats._measure},
        {DIAMETER_OVER_MIN_EDGE, ref_feats._sq_diam / ref_feats._sq_min_edge},
        {DIAMETER, ref_feats._sq_diam},
        {ONE_OVER_MEASURE, 1. / ref_feats._measure},
        {ONE_OVER_INTERNAL_WEIGHTS, 1. / ref_feats._internal_weights},
        // Squared
        {PERIMETER_OVER_RADIUS,
         _sq(ref_feats._external_weights) / ref_feats._measure},
        {EXTERNAL_WEIGHTS, ref_feats._external_weights},
        // Squared
        {MAX_BARY_DIST_OVER_RADIUS, 2 * _sq(0.5) / ref_feats._measure},
        {MAX_OVER_MIN_BARY_DIST, 1.},
        {ALGEBRAIC_PERIMETER_OVER_MEASURE,
         ref_feats._external_weights / ref_feats._measure},
      });
      THEN("All AR-computers give the expected results") {
        for (const auto &[label, ARC] : all_ARCs) {
          ARC->compute_and_update_features(
            new_fc, feats, cc, n_shared_faces, ar, tmp_feats
          );
          REQUIRE_THAT(ar, WithinAbs(expected_AR.at(label), eps));
        }
      }

      // Now testing 9 cells: in this case, the barycenter is the center of a
      // FC, we check that the barycenter-based computations are fine
      for (auto &fc : {2, 4, 6, 8, 9})
        cc.insert(fc);
      new_fc = 10;
      feats._external_weights = ref_feats._external_weights = 9.0;
      feats._measure = 8., ref_feats._measure = 9.;
      feats._n_internal_faces = 10, ref_feats._n_internal_faces = 12;
      feats._internal_weights = 10., ref_feats._internal_weights = 12.;
      feats._sq_diam = ref_feats._sq_diam = 8.;
      feats._sq_min_edge = ref_feats._sq_min_edge = 1.;
      ref_feats._sum_centers[0] = ref_feats._sum_centers[1] = 9. * 1.5;
      feats._sum_centers[0] =
        ref_feats._sum_centers[0] - graph->_centers[new_fc][0];
      feats._sum_centers[1] =
        ref_feats._sum_centers[1] - graph->_centers[new_fc][1];
      feats._external_facets.clear();
      feats._external_facets = {
        {0, 2}, {1, 1}, {2, 2}, {4, 1}, {5, 0}, {6, 2}, {8, 2}, {9, 2}
      };
      ref_feats._external_facets = feats._external_facets;
      ref_feats._external_facets.at(6) = ref_feats._external_facets.at(9) = 1;
      ref_feats._external_facets.insert({10, 2});
      // Case with basic features
      all_ARCs.at(DIAMETER)->compute_and_update_features(
        new_fc, feats, cc, n_shared_faces, ar, tmp_feats
      );
      THEN("9 cells - Base features OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_GEOM_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE(n_shared_faces == 2);
      }
      // Case with weights features
      all_ARCs.at(EXTERNAL_WEIGHTS)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("9 cells - Weights features are OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_WEIGHTS_CELL_FEATURES(tmp_feats, ref_feats, eps);
      }
      // Case with barycentric features
      all_ARCs.at(MAX_OVER_MIN_BARY_DIST)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("9 cells - barycenter info is OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE_THAT(
          tmp_feats._sum_centers, Approx(ref_feats._sum_centers).margin(eps)
        );
        for (auto &[idx, nef] : tmp_feats._external_facets) {
          REQUIRE(nef == ref_feats._external_facets.at(idx));
        }
        // max(bary_dist) = sqrt(2)
        // min(bary_dist) = 1 It should be zero since the barycenter is the
        // center of a cell, but we have a threshold
        // Here, it is not squared
        REQUIRE_THAT(ar, WithinAbs(sqrt(2.), eps));
      }  // Then

    }  // When
  }  // Given

  GIVEN("A 3D 4x4x4 Cartesian mesh") {
    const DualGEx_cube_4 Data = DualGEx_cube_4();
    shared_ptr<DualGraphT> graph = make_shared<DualGraphT>(
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
    const map<CoMMAAspectRatioT, shared_ptr<ARCT>> all_ARCs({
      {DIAMETER_OVER_RADIUS,
       make_shared<ARDiamOverRadius<CoMMAIndexT, CoMMAWeightT, CoMMAIntT, 3>>(
         graph
       )},
      {DIAMETER_OVER_MIN_EDGE,
       make_shared<ARDiamOverMinEdge<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
         graph
       )},
      {DIAMETER,
       make_shared<ARDiameter<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(graph)},
      {ONE_OVER_MEASURE,
       make_shared<AROverMeasure<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(graph)},
      {ONE_OVER_INTERNAL_WEIGHTS,
       make_shared<AROverInternalWeights<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
         graph
       )},
      {PERIMETER_OVER_RADIUS,
       make_shared<
         ARExternalWeightOverRadius<CoMMAIndexT, CoMMAWeightT, CoMMAIntT, 3>>(
         graph
       )},
      {EXTERNAL_WEIGHTS,
       make_shared<ARExternalWeights<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
         graph
       )},
      {MAX_BARY_DIST_OVER_RADIUS,
       make_shared<
         ARMaxBaryDistanceOverRadius<CoMMAIndexT, CoMMAWeightT, CoMMAIntT, 3>>(
         graph
       )},
      {MAX_OVER_MIN_BARY_DIST,
       make_shared<
         ARMaxOverMinBaryDistance<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
         graph, 1e-12
       )},
      {ALGEBRAIC_PERIMETER_OVER_MEASURE,
       make_shared<
         ARExternalWeightOverRadius<CoMMAIndexT, CoMMAWeightT, CoMMAIntT, 1>>(
         graph
       )},
    });
    WHEN("We create a cell and try to add some fine-cells") {
      // In
      unordered_set<CoMMAIndexT> cc = {0, 1, 4, 5};
      CoMMAIndexT new_fc = 17;
      CFT feats{
        4.,
        16.,
        4.,
        4,
        1.,
        sqrt(2.),
        {4., 4., 2.},
        {{0, 4}, {1, 4}, {4, 4}, {5, 4}}
      };
      CFT ref_feats{
        5., 20., 5., 5, 1., sqrt(3.), feats._sum_centers, feats._external_facets
      };
      CFT tmp_feats{};
      CoMMAIntT n_shared_faces{};
      CoMMAWeightT ar{};
      for (auto xyz = decltype(ref_feats._sum_centers.size()){0};
           xyz < ref_feats._sum_centers.size();
           ++xyz)
        ref_feats._sum_centers[xyz] +=
          graph->_volumes[new_fc] * graph->_centers[new_fc][xyz];
      ref_feats._external_facets.at(1)--;
      ref_feats._external_facets.insert({new_fc, 5});
      // Case with basic features
      all_ARCs.at(DIAMETER)->compute_and_update_features(
        new_fc, feats, cc, n_shared_faces, ar, tmp_feats
      );
      THEN("5 cells - Base features OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_GEOM_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE(n_shared_faces == 1);
      }
      // Case with weights features
      all_ARCs.at(EXTERNAL_WEIGHTS)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("5 cells - Weights features are OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_WEIGHTS_CELL_FEATURES(tmp_feats, ref_feats, eps);
      }
      // Case with barycentric features
      all_ARCs.at(MAX_OVER_MIN_BARY_DIST)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("5 cells - barycenter info is OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE_THAT(
          ref_feats._sum_centers, Approx(tmp_feats._sum_centers).margin(eps)
        );
        for (auto &[idx, nef] : tmp_feats._external_facets) {
          REQUIRE(nef == ref_feats._external_facets.at(idx));
        }
      }  // Then

      cc.insert(new_fc);
      new_fc = 21;
      feats = ref_feats;  // OK copy
      ref_feats._measure = 6.;
      // ref_diam does not change
      ref_feats._external_weights = 22.;
      ref_feats._internal_weights = 7.;
      ref_feats._n_internal_faces = 7;
      ref_feats._external_facets.at(5)--;
      ref_feats._external_facets.at(17)--;
      ref_feats._external_facets.insert({new_fc, 4});
      for (auto xyz = decltype(ref_feats._sum_centers.size()){0};
           xyz < ref_feats._sum_centers.size();
           ++xyz)
        ref_feats._sum_centers[xyz] +=
          graph->_volumes[new_fc] * graph->_centers[new_fc][xyz];
      // Case with basic features
      all_ARCs.at(DIAMETER)->compute_and_update_features(
        new_fc, feats, cc, n_shared_faces, ar, tmp_feats
      );
      THEN("6 cells - Base features OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_GEOM_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE(n_shared_faces == 2);
      }
      // Case with weights features
      all_ARCs.at(EXTERNAL_WEIGHTS)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("6 cells - Weights features are OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_WEIGHTS_CELL_FEATURES(tmp_feats, ref_feats, eps);
      }
      // Case with barycentric features
      all_ARCs.at(MAX_OVER_MIN_BARY_DIST)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("6 cells - barycenter info is OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE_THAT(
          tmp_feats._sum_centers, Approx(ref_feats._sum_centers).margin(eps)
        );
        for (auto &[idx, nef] : tmp_feats._external_facets) {
          REQUIRE(nef == ref_feats._external_facets.at(idx));
        }
      }  // Then

      cc.insert(new_fc), cc.insert(20);
      feats._measure = 7.;
      // cc_diam does not change
      feats._external_weights = 24.;
      feats._internal_weights = 9.;
      feats._n_internal_faces = 9;
      feats._sum_centers = ref_feats._sum_centers;
      for (auto xyz = decltype(ref_feats._sum_centers.size()){0};
           xyz < ref_feats._sum_centers.size();
           ++xyz) {
        feats._sum_centers[xyz] +=
          graph->_volumes[20] * graph->_centers[20][xyz];
        ref_feats._sum_centers[xyz] +=
          graph->_volumes[20] * graph->_centers[20][xyz]
          + graph->_volumes[16] * graph->_centers[16][xyz];
      }
      ref_feats._external_facets.insert({20, 4});
      ref_feats._external_facets.at(new_fc)--;
      ref_feats._external_facets.at(4)--;
      feats._external_facets = ref_feats._external_facets;
      new_fc = 16;
      ref_feats._external_facets.insert({new_fc, 3});
      for (auto &[idx, nef] : ref_feats._external_facets)
        nef = 3;
      //
      ref_feats._measure = 8.;
      // ref_diam does not change
      ref_feats._external_weights = 24.;
      ref_feats._internal_weights = 12.;
      ref_feats._n_internal_faces = 12;

      // Case with basic features
      all_ARCs.at(DIAMETER)->compute_and_update_features(
        new_fc, feats, cc, n_shared_faces, ar, tmp_feats
      );
      THEN("8 cells - Base features OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_GEOM_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE(n_shared_faces == 3);
      }
      // Case with weights features
      all_ARCs.at(EXTERNAL_WEIGHTS)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("8 cells - Weights features are OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_WEIGHTS_CELL_FEATURES(tmp_feats, ref_feats, eps);
      }
      // Case with barycentric features
      all_ARCs.at(MAX_OVER_MIN_BARY_DIST)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("8 cells - barycenter info is OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE_THAT(
          tmp_feats._sum_centers, Approx(ref_feats._sum_centers).margin(eps)
        );
        for (auto &[idx, nef] : tmp_feats._external_facets) {
          REQUIRE(nef == ref_feats._external_facets.at(idx));
        }
      }  // Then

      const map<CoMMAAspectRatioT, CoMMAWeightT> expected_AR({
        // Squared
        {DIAMETER_OVER_RADIUS, _cb(ref_feats._sq_diam) / _sq(ref_feats._measure)
        },
        {DIAMETER_OVER_MIN_EDGE, ref_feats._sq_diam / ref_feats._sq_min_edge},
        {DIAMETER, ref_feats._sq_diam},
        {ONE_OVER_MEASURE, 1. / ref_feats._measure},
        {ONE_OVER_INTERNAL_WEIGHTS, 1. / ref_feats._internal_weights},
        // Squared
        {PERIMETER_OVER_RADIUS,
         _cb(ref_feats._external_weights) / _sq(ref_feats._measure)},
        {EXTERNAL_WEIGHTS, ref_feats._external_weights},
        // Squared
        {MAX_BARY_DIST_OVER_RADIUS, _cb(3 * _sq(0.5)) / _sq(ref_feats._measure)
        },
        {MAX_OVER_MIN_BARY_DIST, 1.},
        {ALGEBRAIC_PERIMETER_OVER_MEASURE,
         ref_feats._external_weights / ref_feats._measure},
      });
      THEN("All AR-computers give the expected results") {
        for (const auto &[label, ARC] : all_ARCs) {
          ARC->compute_and_update_features(
            new_fc, feats, cc, n_shared_faces, ar, tmp_feats
          );
          REQUIRE_THAT(ar, WithinAbs(expected_AR.at(label), eps));
        }
      }
    }  // When

    WHEN("We create a cell and try to add some fine-cells") {
      // clang-format off
      constexpr CoMMAIndexT new_fc = 42;  // It was not on purpose
      const unordered_set<CoMMAIndexT> cc = {
        // Top layer
        40, 41, // new_fc, We will add it later
        36, 37, 38,
        32, 33, 34,
        // Middle layer
        24, 25, 26,
        20, 21, 22,
        16, 17, 18,
        // Bottom layer
         8,  9, 10,
         4,  5,  6,
         0,  1,  2
      };
      // clang-format on
      unordered_map<CoMMAIndexT, CoMMAIntT> ext_faces{};
      ext_faces.insert({21, 0});  // Internal face
      for (auto &valley : {5, 17, 20, 22, 25, 37})
        ext_faces.insert({valley, 1});
      for (auto &ridge : {1, 4, 6, 9, 16, 18, 24, 26, 33, 36, 38, 41})
        ext_faces.insert({ridge, 2});
      for (auto &corner : {0, 2, 8, 10, 32, 34, 40, 42})
        ext_faces.insert({corner, 3});
      constexpr CoMMAWeightT sum_centers = 27.0 * 1.5;
      const vector<CoMMAWeightT> true_bary = {
        sum_centers, sum_centers, sum_centers
      };

      const CFT ref_feats(
        9., 54., 54., 54, 1., sqrt(3. * _sq(4)), true_bary, ext_faces
      );
      CFT feats = ref_feats;
      feats._measure -= 1.;
      // external weights doe not change
      feats._internal_weights -= 3.;
      feats._n_internal_faces -= 3;
      for (auto xyz = decltype(feats._sum_centers.size()){0};
           xyz < feats._sum_centers.size();
           ++xyz)
        feats._sum_centers[xyz] -=
          graph->_volumes[new_fc] * graph->_centers[new_fc][xyz];
      for (auto &neigh_42 : {26, 38, 41})
        feats._external_facets.at(neigh_42) += 1;
      CFT tmp_feats{};
      CoMMAWeightT ar{};
      CoMMAIntT n_shared_faces{};
      // Case with basic features
      all_ARCs.at(DIAMETER)->compute_and_update_features(
        new_fc, feats, cc, n_shared_faces, ar, tmp_feats
      );
      THEN("27 cells - Base features OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_GEOM_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE(n_shared_faces == 3);
      }
      // Case with weights features
      all_ARCs.at(EXTERNAL_WEIGHTS)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("27 cells - Weights features are OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        CHECK_WEIGHTS_CELL_FEATURES(tmp_feats, ref_feats, eps);
      }
      // Case with barycentric features
      all_ARCs.at(MAX_OVER_MIN_BARY_DIST)
        ->compute_and_update_features(
          new_fc, feats, cc, n_shared_faces, ar, tmp_feats
        );
      THEN("27 cells - barycenter info is OK") {
        CHECK_BASE_CELL_FEATURES(tmp_feats, ref_feats, eps);
        REQUIRE_THAT(
          tmp_feats._sum_centers, Approx(ref_feats._sum_centers).margin(eps)
        );
        for (auto &[idx, nef] : tmp_feats._external_facets) {
          REQUIRE(nef == ref_feats._external_facets.at(idx));
        }
        // max(bary_dist) = sqrt(3)
        // min(bary_dist) = 1 It should be zero since the barycenter is the
        // center of a cell, but we have a threshold
        // Here, it is not squared
        REQUIRE_THAT(ar, WithinAbs(sqrt(3.), eps));
      }  // Then
    }  // When
  }  // Given
}  // Scenario
