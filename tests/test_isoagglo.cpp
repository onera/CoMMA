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

#include <unordered_set>
#include <vector>

#include "Agglomerator.h"
#include "catch2/catch.hpp"
#include "input/DualGPy.h"
#include "test_defs.h"

using namespace comma;
using namespace std;

SCENARIO("Test the Isotropic agglomeration for small 3D cases", "[Isotropic]") {
  GIVEN("We load the Isotropic mesh structure") {
    const DualGPy_cube_4 Data = DualGPy_cube_4();
    shared_ptr<SeedsPoolT> seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
      Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces,
      Data.dim, Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg = make_unique<
      Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER,
      Data.dim);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(8, 8, 8, Data.weights, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        const auto &fccc = cc_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {
          0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0,
          1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7,
          6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value() == fc2cc_req[i]);
        }
      }
    }
    WHEN(
      "We agglomerate the mesh with a biconnected agglomerator and we try to correct") {
      agg->agglomerate_one_level(8, 8, 8, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {
          0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0,
          1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7,
          6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value() == fc2cc_req[i]);
        }
      }
    }

    shared_ptr<CCContainerT> cc_PF_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg_PF = make_unique<
      Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      fc_graph, cc_PF_graph, seeds_pool, CoMMANeighbourhoodT::PURE_FRONT,
      FC_ITER, Data.dim);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(8, 8, 8, Data.weights, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        const auto &fccc = cc_PF_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {
          0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0,
          1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7,
          6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value() == fc2cc_req[i]);
        }
      }
    }
    WHEN(
      "We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct") {
      agg_PF->agglomerate_one_level(8, 8, 8, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_PF_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {
          0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0,
          1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7,
          6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value() == fc2cc_req[i]);
        }
      }
    }

    WHEN("We compute the aspect-ratio of a coarse cell on the boundary") {
      constexpr CoMMAWeightT eps = 1e-10;
      // In
      unordered_set<CoMMAIndexT> cc = {0, 1, 4, 5};
      CoMMAWeightT tmp_diam{-1.}, tmp_vol{-1.};
      CoMMAWeightT cc_diam = sqrt(2.), cc_vol = 4.;
      // Out
      CoMMAIntT shared_faces;
      CoMMAWeightT ar;
      CoMMAWeightT ref_diam = sqrt(3.), ref_vol = 5.;
      CoMMAWeightT ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("New coarse cell has 1 shared face") {
        agg->compute_next_cc_features(
          17, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      cc.insert(17);
      cc_diam = ref_diam, cc_vol = ref_vol;
      ref_vol = 6.;  // ref_diam does not change
      ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("New coarse cell has 2 shared face") {
        agg->compute_next_cc_features(
          21, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 2);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      cc.insert(21);
      cc.insert(20);
      cc_vol = 7.;  // cc_diam does not change
      ref_vol = 8.;  // ref_diam does not change
      ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("New coarse cell has 3 shared face") {
        agg->compute_next_cc_features(
          16, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 3);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
    }  // Aspect ratio
  }
}

SCENARIO("Test the Isotropic agglomeration for small 2D cases", "[Isotropic]") {
  GIVEN("We load the Isotropic mesh structure") {
    const DualGPy_quad_4 Data = DualGPy_quad_4();
    shared_ptr<SeedsPoolT> seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
      Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces,
      Data.dim, Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg = make_unique<
      Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER,
      Data.dim);
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
      "We agglomerate the mesh with a biconnected agglomerator and we try to correct") {
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

    shared_ptr<CCContainerT> cc_PF_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg_PF = make_unique<
      Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
      fc_graph, cc_PF_graph, seeds_pool, CoMMANeighbourhoodT::PURE_FRONT,
      FC_ITER, Data.dim);
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
      "We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct") {
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
      const CoMMAWeightT eps = 1e-10;
      // In
      unordered_set<CoMMAIndexT> cc = {0, 1};
      CoMMAWeightT cc_diam = 1., cc_vol = 2.;
      // Out
      CoMMAWeightT tmp_diam{-1.}, tmp_vol{-1.};
      CoMMAIntT shared_faces;
      CoMMAWeightT ar;
      CoMMAWeightT ref_diam = sqrt(2.), ref_vol = 3.;
      CoMMAWeightT ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("L-shaped coarse cell, 1 shared face") {
        agg->compute_next_cc_features(
          5, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      ref_diam = 2.;
      ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("I-shaped coarse cell, 1 shared face") {
        agg->compute_next_cc_features(
          2, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      cc.insert(5);
      cc_diam = sqrt(2.), cc_vol = 3.;
      ref_diam = cc_diam, ref_vol = 4.;
      ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("Squared coarse cell, 2 shared faces") {
        agg->compute_next_cc_features(
          4, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 2);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
    }
  };
  GIVEN("A 3x2 mesh of slightly stretched (x1.75) rectangles") {
    const DualGPy_T_shaped Data = DualGPy_T_shaped();
    shared_ptr<SeedsPoolT> seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
      Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces,
      Data.dim, Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph =
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
      "We agglomerate from the bottom central cell with a standard agglomerator, we get a T-shaped coarse cell") {
      const CoMMAIntT fc_iter = 1;
      auto agg = Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, fc_iter,
        Data.dim);
      agg.set_agglomeration_parameter(card, card, card);
      auto cc =
        agg.choose_optimal_cc_and_update_seeds_pool(seed, Data.weights, comp);
      THEN("Compactness is well computed") { REQUIRE(comp == 1); }
      THEN("The cell is T-shaped") {
        REQUIRE(found_(cc, seed));
        REQUIRE(found_(cc, 0));
        REQUIRE(found_(cc, 2));
        REQUIRE(found_(cc, 4));
      }
    }
    WHEN(
      "We agglomerate from the bottom central cell with a 2-steps iterative agglomerator, we get a square coarse cell") {
      const CoMMAIntT fc_iter = 2;
      auto agg = Agglomerator_Iterative<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, fc_iter,
        Data.dim);
      agg.set_agglomeration_parameter(card, card, card);
      auto cc =
        agg.choose_optimal_cc_and_update_seeds_pool(seed, Data.weights, comp);
      THEN("Compactness is well computed") { REQUIRE(comp == 2); }
      THEN("The cell is a square") {
        REQUIRE(found_(cc, seed));
        REQUIRE(found_(cc, 0));
        REQUIRE(found_(cc, 3));
        REQUIRE(found_(cc, 4));
      }
    }
  }
}
