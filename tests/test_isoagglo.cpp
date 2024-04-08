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

#include <unordered_set>
#include <vector>

#include "CoMMA/Agglomerator.h"
#include "DualGraphExamples.h"
#include "catch2/catch.hpp"
#include "test_defs.h"

using namespace comma;  // NOLINT
using namespace std;  // NOLINT

#define CHECK_CELL_FEATURES(this, that, eps)                                 \
  REQUIRE(EQUAL_UP_TO(this._diam, that._diam, eps));                         \
  REQUIRE(EQUAL_UP_TO(this._min_edge, that._min_edge, eps));                 \
  REQUIRE(EQUAL_UP_TO(this._measure, that._measure, eps));                   \
  REQUIRE(EQUAL_UP_TO(this._external_weights, that._external_weights, eps)); \
  REQUIRE(EQUAL_UP_TO(this._internal_weights, that._internal_weights, eps)); \
  REQUIRE(this._n_internal_faces == that._n_internal_faces);

SCENARIO("Test the Isotropic agglomeration for small 3D cases", "[Isotropic]") {
  GIVEN("We load the Isotropic mesh structure") {
    const DualGEx_cube_4 Data = DualGEx_cube_4();
    shared_ptr<SeedsPoolT> const seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
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
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg = make_unique<
      Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      fc_graph,
      cc_graph,
      seeds_pool,
      DEFAULT_AR,
      CoMMANeighbourhoodT::EXTENDED,
      FC_ITER,
      Data.dim
    );
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(8, 8, 8, Data.weights, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        const auto &fccc = cc_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2,
                                         2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2,
                                         3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5,
                                         5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5,
                                         4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value() == fc2cc_req[i]);
        }
      }
    }
    WHEN(
      "We agglomerate the mesh with a biconnected agglomerator and we try to correct"
    ) {
      agg->agglomerate_one_level(8, 8, 8, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2,
                                         2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2,
                                         3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5,
                                         5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5,
                                         4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value() == fc2cc_req[i]);
        }
      }
    }

    shared_ptr<CCContainerT> const cc_PF_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg_PF = make_unique<
      Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      fc_graph,
      cc_PF_graph,
      seeds_pool,
      DEFAULT_AR,
      CoMMANeighbourhoodT::PURE_FRONT,
      FC_ITER,
      Data.dim
    );
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(8, 8, 8, Data.weights, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        const auto &fccc = cc_PF_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2,
                                         2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2,
                                         3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5,
                                         5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5,
                                         4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value() == fc2cc_req[i]);
        }
      }
    }
    WHEN(
      "We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct"
    ) {
      agg_PF->agglomerate_one_level(8, 8, 8, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_PF_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2,
                                         2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2,
                                         3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5,
                                         5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5,
                                         4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value() == fc2cc_req[i]);
        }
      }
    }

    WHEN("We compute the aspect-ratio of a coarse cell on the boundary") {
      constexpr CoMMAWeightT eps = 1e-10;
      // In
      unordered_set<CoMMAIndexT> cc = {0, 1, 4, 5};
      CellFeatures<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> tmp_feats{};
      CellFeatures<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> cc_feats{
        4., 16., 4., 4, 1., sqrt(2.)
      };
      // Out
      CoMMAIntT shared_faces{};
      CoMMAWeightT ar{};
      CellFeatures<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> ref_feats{
        5., 20., 5., 5, 1., sqrt(3.)
      };
      CoMMAWeightT ref_ar = agg->_compute_AR(ref_feats);
      THEN("New coarse cell has 1 shared face") {
        agg->compute_next_cc_features(
          17, cc_feats, cc, shared_faces, ar, tmp_feats
        );
        REQUIRE(shared_faces == 1);
        CHECK_CELL_FEATURES(ref_feats, tmp_feats, eps);
        REQUIRE(EQUAL_UP_TO(ref_ar, ar, eps));
      }
      cc.insert(17);
      cc_feats = ref_feats;  // OK copy
      ref_feats._measure = 6.;
      // ref_diam does not change
      ref_feats._external_weights = 22.;
      ref_feats._internal_weights = 7.;
      ref_feats._n_internal_faces = 7;
      ref_ar = agg->_compute_AR(ref_feats);
      THEN("New coarse cell has 2 shared face") {
        agg->compute_next_cc_features(
          21, cc_feats, cc, shared_faces, ar, tmp_feats
        );
        REQUIRE(shared_faces == 2);
        CHECK_CELL_FEATURES(ref_feats, tmp_feats, eps);
        REQUIRE(EQUAL_UP_TO(ref_ar, ar, eps));
      }
      cc.insert(21);
      cc.insert(20);
      cc_feats._measure = 7.;
      // cc_diam does not change
      cc_feats._external_weights = 24.;
      cc_feats._internal_weights = 9.;
      cc_feats._n_internal_faces = 9;
      ref_feats._measure = 8.;
      // ref_diam does not change
      ref_feats._external_weights = 24.;
      ref_feats._internal_weights = 12.;
      ref_feats._n_internal_faces = 12;
      ref_ar = agg->_compute_AR(ref_feats);
      THEN("New coarse cell has 3 shared face") {
        agg->compute_next_cc_features(
          16, cc_feats, cc, shared_faces, ar, tmp_feats
        );
        REQUIRE(shared_faces == 3);
        CHECK_CELL_FEATURES(ref_feats, tmp_feats, eps);
        REQUIRE(EQUAL_UP_TO(ref_ar, ar, eps));
      }
    }  // Aspect ratio
  }
}

SCENARIO("Test the Isotropic agglomeration for small 2D cases", "[Isotropic]") {
  GIVEN("We load the Isotropic mesh structure") {
    const DualGEx_quad_4 Data = DualGEx_quad_4();
    shared_ptr<SeedsPoolT> const seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
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
    const shared_ptr<CCContainerT> cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg = make_unique<
      Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      fc_graph,
      cc_graph,
      seeds_pool,
      DEFAULT_AR,
      CoMMANeighbourhoodT::EXTENDED,
      FC_ITER,
      Data.dim
    );
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(4, 4, 4, Data.weights, false);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        const auto &fccc = cc_graph->_fc_2_cc;
        REQUIRE(fccc[0].value() == 0);
        REQUIRE(fccc[1].value() == 0);
        REQUIRE(fccc[2].value() == 1);
        REQUIRE(fccc[3].value() == 1);
        REQUIRE(fccc[4].value() == 0);
        REQUIRE(fccc[5].value() == 0);
        REQUIRE(fccc[6].value() == 1);
        REQUIRE(fccc[7].value() == 1);
        REQUIRE(fccc[8].value() == 2);
        REQUIRE(fccc[9].value() == 2);
        REQUIRE(fccc[10].value() == 3);
        REQUIRE(fccc[11].value() == 3);
        REQUIRE(fccc[12].value() == 2);
        REQUIRE(fccc[13].value() == 2);
        REQUIRE(fccc[14].value() == 3);
        REQUIRE(fccc[15].value() == 3);
      }
    }
    WHEN(
      "We agglomerate the mesh with a biconnected agglomerator and we try to correct"
    ) {
      agg->agglomerate_one_level(4, 4, 4, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_graph->_fc_2_cc;
        REQUIRE(fccc[0].value() == 0);
        REQUIRE(fccc[1].value() == 0);
        REQUIRE(fccc[2].value() == 1);
        REQUIRE(fccc[3].value() == 1);
        REQUIRE(fccc[4].value() == 0);
        REQUIRE(fccc[5].value() == 0);
        REQUIRE(fccc[6].value() == 1);
        REQUIRE(fccc[7].value() == 1);
        REQUIRE(fccc[8].value() == 2);
        REQUIRE(fccc[9].value() == 2);
        REQUIRE(fccc[10].value() == 3);
        REQUIRE(fccc[11].value() == 3);
        REQUIRE(fccc[12].value() == 2);
        REQUIRE(fccc[13].value() == 2);
        REQUIRE(fccc[14].value() == 3);
        REQUIRE(fccc[15].value() == 3);
      }
    }

    const shared_ptr<CCContainerT> cc_PF_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg_PF = make_unique<
      Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      fc_graph,
      cc_PF_graph,
      seeds_pool,
      DEFAULT_AR,
      CoMMANeighbourhoodT::PURE_FRONT,
      FC_ITER,
      Data.dim
    );
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(4, 4, 4, Data.weights, false);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        // Nothing changes with respect to the case of the standard Biconnected
        // We have a trick that make it work as it should
        const auto &fccc = cc_PF_graph->_fc_2_cc;
        REQUIRE(fccc[0].value() == 0);
        REQUIRE(fccc[1].value() == 0);
        REQUIRE(fccc[2].value() == 1);
        REQUIRE(fccc[3].value() == 1);
        REQUIRE(fccc[4].value() == 0);
        REQUIRE(fccc[5].value() == 0);
        REQUIRE(fccc[6].value() == 1);
        REQUIRE(fccc[7].value() == 1);
        REQUIRE(fccc[8].value() == 2);
        REQUIRE(fccc[9].value() == 2);
        REQUIRE(fccc[10].value() == 3);
        REQUIRE(fccc[11].value() == 3);
        REQUIRE(fccc[12].value() == 2);
        REQUIRE(fccc[13].value() == 2);
        REQUIRE(fccc[14].value() == 3);
        REQUIRE(fccc[15].value() == 3);
      }
    }
    WHEN(
      "We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct"
    ) {
      agg_PF->agglomerate_one_level(4, 4, 4, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto fccc = cc_PF_graph->_fc_2_cc;
        REQUIRE(fccc[0].value() == 0);
        REQUIRE(fccc[1].value() == 0);
        REQUIRE(fccc[2].value() == 1);
        REQUIRE(fccc[3].value() == 1);
        REQUIRE(fccc[4].value() == 0);
        REQUIRE(fccc[5].value() == 0);
        REQUIRE(fccc[6].value() == 1);
        REQUIRE(fccc[7].value() == 1);
        REQUIRE(fccc[8].value() == 2);
        REQUIRE(fccc[9].value() == 2);
        REQUIRE(fccc[10].value() == 3);
        REQUIRE(fccc[11].value() == 3);
        REQUIRE(fccc[12].value() == 2);
        REQUIRE(fccc[13].value() == 2);
        REQUIRE(fccc[14].value() == 3);
        REQUIRE(fccc[15].value() == 3);
      }
    }

    WHEN("We compute the aspect-ratio of a coarse cell") {
      constexpr CoMMAWeightT eps = 1e-10;
      // In
      unordered_set<CoMMAIndexT> cc = {0, 1};
      CellFeatures<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> cc_feats{
        2., 6., 1., 1, 1., 1.
      };
      // Out
      CellFeatures<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> tmp_feats{};
      CoMMAIntT shared_faces{};
      CoMMAWeightT ar{};
      CellFeatures<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> ref_feats{
        3., 8., 2., 2, 1., sqrt(2.)
      };
      CoMMAWeightT ref_ar = agg->_compute_AR(ref_feats);
      THEN("L-shaped coarse cell, 1 shared face") {
        agg->compute_next_cc_features(
          5, cc_feats, cc, shared_faces, ar, tmp_feats
        );
        REQUIRE(shared_faces == 1);
        CHECK_CELL_FEATURES(ref_feats, tmp_feats, eps);
        REQUIRE(EQUAL_UP_TO(ref_ar, ar, eps));
      }
      ref_feats._diam = 2.;
      ref_ar = agg->_compute_AR(ref_feats);
      THEN("I-shaped coarse cell, 1 shared face") {
        agg->compute_next_cc_features(
          2, cc_feats, cc, shared_faces, ar, tmp_feats
        );
        REQUIRE(shared_faces == 1);
        CHECK_CELL_FEATURES(ref_feats, tmp_feats, eps);
        REQUIRE(EQUAL_UP_TO(ref_ar, ar, eps));
      }
      cc.insert(5);
      cc_feats._diam = sqrt(2.);
      cc_feats._measure = 3.;
      cc_feats._external_weights = 8.;
      cc_feats._internal_weights = 2.;
      cc_feats._n_internal_faces = 2;
      //
      ref_feats._diam = cc_feats._diam;
      ref_feats._measure = 4.;
      ref_feats._external_weights = 8.;
      ref_feats._internal_weights = 4.;
      ref_feats._n_internal_faces = 4;
      ref_ar = agg->_compute_AR(ref_feats);
      THEN("Squared coarse cell, 2 shared faces") {
        agg->compute_next_cc_features(
          4, cc_feats, cc, shared_faces, ar, tmp_feats
        );
        REQUIRE(shared_faces == 2);
        CHECK_CELL_FEATURES(ref_feats, tmp_feats, eps);
        REQUIRE(EQUAL_UP_TO(ref_ar, ar, eps));
      }
    }
  }
  GIVEN("A 3x2 mesh of slightly stretched (x1.75) rectangles") {
    const DualGEx_T_shaped Data = DualGEx_T_shaped();
    const shared_ptr<SeedsPoolT> seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
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
    const shared_ptr<CCContainerT> cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    const CoMMAIntT card = 4;
    CoMMAIntT comp = 0;
#if 0
The mesh has been created so that an I-shaped CC leads to a higher AR than an
L-shaped one. However, if the seed is a corner, the additional check in
choose_best_fc regarding the neighbourhood order, lead us to prefer the L-shaped
cell. We start here from the bottom middle so that the above check is not strict and
let the algorithm choose the cell according to the AR.
The standard algorithm will thus pick the I-shaped CC (leading to a T-shaped final
CC). The iterative, on the contrary, will pick the L-shaped CC (leading to a square
final CC)
#endif
    const CoMMAIndexT seed = 1;
    WHEN(
      "We agglomerate from the bottom central cell with a standard agglomerator, we get a T-shaped coarse cell"
    ) {
      const CoMMAIntT fc_iter = 1;
      auto agg = Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        fc_graph,
        cc_graph,
        seeds_pool,
        DEFAULT_AR,
        CoMMANeighbourhoodT::EXTENDED,
        fc_iter,
        Data.dim
      );
      agg.set_agglomeration_parameter(card, card, card);
      auto cc =
        agg.choose_optimal_cc_and_update_seeds_pool(seed, Data.weights, comp);
      THEN("Compactness is well computed") { REQUIRE(comp == 1); }
      THEN("The cell is T-shaped") {
        REQUIRE(CONTAINS_(cc, seed));
        REQUIRE(CONTAINS_(cc, 0));
        REQUIRE(CONTAINS_(cc, 2));
        REQUIRE(CONTAINS_(cc, 4));
      }
    }
    WHEN(
      "We agglomerate from the bottom central cell with a 2-steps iterative agglomerator, we get a square coarse cell"
    ) {
      const CoMMAIntT fc_iter = 2;
      auto agg = Agglomerator_Iterative<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        fc_graph,
        cc_graph,
        seeds_pool,
        DEFAULT_AR,
        CoMMANeighbourhoodT::EXTENDED,
        fc_iter,
        Data.dim
      );
      agg.set_agglomeration_parameter(card, card, card);
      auto cc =
        agg.choose_optimal_cc_and_update_seeds_pool(seed, Data.weights, comp);
      THEN("Compactness is well computed") { REQUIRE(comp == 2); }
      THEN("The cell is a square") {
        REQUIRE(CONTAINS_(cc, seed));
        REQUIRE(CONTAINS_(cc, 0));
        REQUIRE(CONTAINS_(cc, 3));
        REQUIRE(CONTAINS_(cc, 4));
      }
    }
  }
}
