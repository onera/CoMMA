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

#include <optional>
#include <vector>

#include "CoMMA/Agglomerator.h"
#include "CoMMA/CoMMA.h"
#include "CoMMA/CoMMADefs.h"
#include "DualGraphExamples.h"
#include "catch2/catch.hpp"
#include "test_defs.h"

using namespace comma;  // NOLINT
using namespace std;  // NOLINT

#define SING_CARD_THRESH 1
#define MAX_CELLS_IN_LINE std::nullopt
#define CELL_COUPLING_MAX CoMMACellCouplingT::MAX_WEIGHT
#define FORCE_DIRECTION true

// NOLINTNEXTLINE
SCENARIO(
  "Test the anisotropic agglomeration for small cases", "[Anisotropic]"
) {
  GIVEN("We load the anisotropic mesh structure") {
    const DualGEx_aniso Data = DualGEx_aniso();
    shared_ptr<SeedsPoolT> const seeds_pool =
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
      Data.arrayOfFineAnisotropicCompliantCells
    );
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    const CoMMAWeightT aniso_thresh{2.};
    const bool build_lines = true;
    const vector<CoMMAIndexT> agglomerationLines_Idx{};
    const vector<CoMMAIndexT> agglomerationLines{};
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      aniso_thresh,
      agglomerationLines_Idx,
      agglomerationLines,
      Data.weights,
      build_lines,
      ODD_LINE_LENGTH,
      MAX_CELLS_IN_LINE,
      Data.dim,
      CELL_COUPLING_MAX,
      FORCE_DIRECTION
    );

    WHEN(
      "We proceed with the agglomeration of the anisotropic lines (we gather them and later we agglomerate)"
    ) {
      aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
      THEN("We have a number of agglomeration lines != 0") {
        REQUIRE(aniso_agg._nb_lines[0] != 0);
      }
    }
  }
  GIVEN(
    "We load the anisotropic mesh structure, but there is no anisotropic cell"
  ) {
    /* ATTENTION: This test produces (prints) a warning, twice */
    const DualGEx_aniso Data = DualGEx_aniso();
    WHEN("We consider no anisotropic compliant cell") {
      shared_ptr<SeedsPoolT> const seeds_pool =
        make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
      vector<CoMMAIndexT> v_aniso = {};
      shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc,
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.n_bnd_faces,
        Data.dim,
        v_aniso
      );
      shared_ptr<CCContainerT> const cc_graph =
        make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
      const CoMMAWeightT aniso_thresh{-2.};
      const bool build_lines = true;
      const vector<CoMMAIndexT> agglomerationLines_Idx{};
      const vector<CoMMAIndexT> agglomerationLines{};
      Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
        fc_graph,
        cc_graph,
        seeds_pool,
        aniso_thresh,
        agglomerationLines_Idx,
        agglomerationLines,
        Data.weights,
        build_lines,
        ODD_LINE_LENGTH,
        MAX_CELLS_IN_LINE,
        Data.dim,
        CELL_COUPLING_MAX,
        FORCE_DIRECTION
      );

      THEN(
        "There is no need to agglomerate anisotropically since no line can be built"
      ) {
        REQUIRE(!aniso_agg._should_agglomerate);
        REQUIRE(aniso_agg._nb_lines[0] == 0);
        THEN("Even if one tries to agglomerate anisotropically, nothing happens"
        ) {
          aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
          REQUIRE(cc_graph->get_number_of_fc_agglomerated() == 0);
        }
      }
    }
    WHEN("We consider a too high threshold") {
      shared_ptr<SeedsPoolT> const seeds_pool =
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
        Data.arrayOfFineAnisotropicCompliantCells
      );
      shared_ptr<CCContainerT> const cc_graph =
        make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
      const CoMMAWeightT aniso_thresh{10000.};
      const bool build_lines = true;
      const vector<CoMMAIndexT> agglomerationLines_Idx{};
      const vector<CoMMAIndexT> agglomerationLines{};
      Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
        fc_graph,
        cc_graph,
        seeds_pool,
        aniso_thresh,
        agglomerationLines_Idx,
        agglomerationLines,
        Data.weights,
        build_lines,
        ODD_LINE_LENGTH,
        MAX_CELLS_IN_LINE,
        Data.dim,
        CELL_COUPLING_MAX,
        FORCE_DIRECTION
      );

      THEN(
        "There is no need to agglomerate anisotropically since no line can be built"
      ) {
        REQUIRE(!aniso_agg._should_agglomerate);
        REQUIRE(aniso_agg._nb_lines[0] == 0);
        THEN("Even if one tries to agglomerate anisotropically, nothing happens"
        ) {
          aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
          REQUIRE(cc_graph->get_number_of_fc_agglomerated() == 0);
        }
      }
    }
    WHEN("We consider only anisotropic lines composed of only one cell") {
      shared_ptr<SeedsPoolT> const seeds_pool =
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
        Data.arrayOfFineAnisotropicCompliantCells
      );
      shared_ptr<CCContainerT> const cc_graph =
        make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
      const CoMMAWeightT aniso_thresh{10000.};
      const bool build_lines = false;
      vector<CoMMAIndexT> agglomerationLines_Idx{0, 1, 2};
      vector<CoMMAIndexT> agglomerationLines{1, 2};
      Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
        fc_graph,
        cc_graph,
        seeds_pool,
        aniso_thresh,
        agglomerationLines_Idx,
        agglomerationLines,
        Data.weights,
        build_lines,
        ODD_LINE_LENGTH,
        MAX_CELLS_IN_LINE,
        Data.dim,
        CELL_COUPLING_MAX,
        FORCE_DIRECTION
      );

      THEN(
        "There is no need to agglomerate anisotropically since no line can be built"
      ) {
        REQUIRE(!aniso_agg._should_agglomerate);
        REQUIRE(aniso_agg._nb_lines[0] == 0);
        THEN("Even if one tries to agglomerate anisotropically, nothing happens"
        ) {
          aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
          REQUIRE(cc_graph->get_number_of_fc_agglomerated() == 0);
        }
        THEN("Even if one tries to get the output lines, they come back empty"
        ) {
          aniso_agg.export_anisotropic_lines(
            1, agglomerationLines_Idx, agglomerationLines
          );
          REQUIRE(agglomerationLines_Idx.empty());
          REQUIRE(agglomerationLines.empty());
        }
      }
    }
  }
  GIVEN("We load the anisotropic mesh structure, but only a cell is anisotropic"
  ) {
    /* ATTENTION: This test produces (prints) a warning */
    const DualGEx_aniso Data = DualGEx_aniso();
    shared_ptr<SeedsPoolT> const seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    vector<CoMMAIndexT> v_aniso = {0};
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
      Data.nb_fc,
      Data.adjMatrix_row_ptr,
      Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues,
      Data.volumes,
      Data.centers,
      Data.n_bnd_faces,
      Data.dim,
      v_aniso
    );
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    const CoMMAWeightT aniso_thresh{-2.};
    const bool build_lines = true;
    const vector<CoMMAIndexT> agglomerationLines_Idx{};
    const vector<CoMMAIndexT> agglomerationLines{};
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      aniso_thresh,
      agglomerationLines_Idx,
      agglomerationLines,
      Data.weights,
      build_lines,
      ODD_LINE_LENGTH,
      MAX_CELLS_IN_LINE,
      Data.dim,
      CELL_COUPLING_MAX,
      FORCE_DIRECTION
    );
    THEN(
      "There is no need to agglomerate anisotropically since no line can be built"
    ) {
      REQUIRE(!aniso_agg._should_agglomerate);
      REQUIRE(aniso_agg._nb_lines[0] == 0);
      THEN("Even if one tries to agglomerate anisotropically, nothing happens"
      ) {
        aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
        REQUIRE(cc_graph->get_number_of_fc_agglomerated() == 0);
      }
    }
  }
  GIVEN(
    "We load a 4by7 quad 2D mesh, we provide 4 lines but one is just 1-cell long"
  ) {
    /* ATTENTION: This test produces (prints) a warning */
    const DualGEx_aniso_3cell Data = DualGEx_aniso_3cell();
    shared_ptr<SeedsPoolT> const seeds_pool =
      make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    vector<CoMMAIndexT> v_aniso = {0};
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
      Data.nb_fc,
      Data.adjMatrix_row_ptr,
      Data.adjMatrix_col_ind,
      Data.adjMatrix_areaValues,
      Data.volumes,
      Data.centers,
      Data.n_bnd_faces,
      Data.dim,
      v_aniso
    );
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    const CoMMAWeightT aniso_thresh{-2.};
    const bool build_lines = false;
    const vector<CoMMAIndexT> agglomerationLines_Idx{0, 1, 6, 11, 16};
    // clang-format off
    const vector<CoMMAIndexT> agglomerationLines{
      0,  // This one won't be built, too short
      11, 10, 9, 8, 7,
      16, 14, 12, 13, 15,
      20, 22, 23, 21, 19
    };
    // clang-format on
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      aniso_thresh,
      agglomerationLines_Idx,
      agglomerationLines,
      Data.weights,
      build_lines,
      ODD_LINE_LENGTH,
      MAX_CELLS_IN_LINE,
      Data.dim,
      CELL_COUPLING_MAX,
      FORCE_DIRECTION
    );
    THEN("Only 3 lines are built") {
      REQUIRE(aniso_agg._nb_lines[0] == 3);
      REQUIRE(aniso_agg._should_agglomerate);
    }
  }

#define CHECK2CELLS(f2c, a, b) ((f2c)[(a)].value() == (f2c)[(b)].value())
#define CHECK3CELLS(f2c, a, b, c) \
  (CHECK2CELLS(f2c, a, b) && CHECK2CELLS(f2c, b, c))
#define CHECK4CELLS(f2c, a, b, c, d) \
  (CHECK2CELLS(f2c, a, b) && CHECK2CELLS(f2c, b, c) && CHECK2CELLS(f2c, c, d))
  GIVEN(
    "We load a 4by7 quad 2D mesh which has 4 anisotropic lines each of length 5 cells and"
    " a seeds pool with boundary priority with full initialization"
  ) {
    const DualGEx_aniso_3cell Data = DualGEx_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    const bool build_lines = true;
    const vector<CoMMAIndexT> agglomerationLines_Idx{};
    const vector<CoMMAIndexT> agglomerationLines{};
    shared_ptr<SeedsPoolT> const seeds_pool =
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
      Data.arrayOfFineAnisotropicCompliantCells
    );
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      aniso_thresh,
      agglomerationLines_Idx,
      agglomerationLines,
      Data.weights,
      build_lines,
      ODD_LINE_LENGTH,
      MAX_CELLS_IN_LINE,
      Data.dim,
      CELL_COUPLING_MAX,
      FORCE_DIRECTION
    );
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> iso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      CoMMANeighbourhoodT::EXTENDED,
      FC_ITER,
      Data.dim
    );
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      aniso_agg.update_seeds_pool();
      THEN(
        "After the anisotropic agglomeration, the seeds pool is not empty and does not need an initialization"
      ) {
        REQUIRE(!seeds_pool->is_empty());
        REQUIRE(!seeds_pool->need_initialization(cc_graph->_is_fc_agglomerated)
        );
      }
      THEN(
        "Having chosen a priority by boundary, the seed is a corner who is not a neighbour"
      ) {
        REQUIRE(
          seeds_pool->choose_new_seed(cc_graph->_is_fc_agglomerated).value()
          == 24
        );
      }
      iso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("There are two isotropic coarse cells") {
        REQUIRE(CHECK4CELLS(f2c, 5, 6, 24, 25));
        REQUIRE(CHECK4CELLS(f2c, 17, 18, 27, 26));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2"
      ) {
        REQUIRE(CHECK2CELLS(f2c, 0, 1));
        REQUIRE(CHECK2CELLS(f2c, 11, 10));
        REQUIRE(CHECK2CELLS(f2c, 16, 14));
        REQUIRE(CHECK2CELLS(f2c, 20, 22));
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
        REQUIRE(CHECK3CELLS(f2c, 2, 3, 4));
        REQUIRE(CHECK3CELLS(f2c, 9, 8, 7));
        REQUIRE(CHECK3CELLS(f2c, 12, 13, 15));
        REQUIRE(CHECK3CELLS(f2c, 23, 21, 19));
      }
      THEN(
        "The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
        " and the priority weights"
      ) {
        REQUIRE(f2c[0].value() == 0);
        REQUIRE(f2c[2].value() == 1);
        REQUIRE(f2c[9].value() == 3);
        REQUIRE(f2c[11].value() == 2);
        REQUIRE(f2c[13].value() == 5);
        REQUIRE(f2c[16].value() == 4);
        REQUIRE(f2c[20].value() == 6);
        REQUIRE(f2c[23].value() == 7);
      }
    }
  }

  GIVEN(
    "We load a 4by7 quad 2D mesh which has 4 anisotropic lines each of length 5 cells and"
    " a seeds pool with boundary priority with full initialization but we forbid odd lines"
  ) {
    const DualGEx_aniso_3cell Data = DualGEx_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    const bool build_lines = true;
    vector<CoMMAIndexT> const agglomerationLines_Idx{};
    vector<CoMMAIndexT> const agglomerationLines{};
    const shared_ptr<SeedsPoolT> seeds_pool =
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
      Data.arrayOfFineAnisotropicCompliantCells
    );
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      aniso_thresh,
      agglomerationLines_Idx,
      agglomerationLines,
      Data.weights,
      build_lines,
      false,
      MAX_CELLS_IN_LINE,
      Data.dim,
      CELL_COUPLING_MAX,
      FORCE_DIRECTION
    );
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> iso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      CoMMANeighbourhoodT::EXTENDED,
      FC_ITER,
      Data.dim
    );
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      aniso_agg.update_seeds_pool();
      THEN(
        "After the anisotropic agglomeration, the seeds pool is not empty and does not need an initialization"
      ) {
        REQUIRE(!seeds_pool->is_empty());
        REQUIRE(!seeds_pool->need_initialization(cc_graph->_is_fc_agglomerated)
        );
      }
      THEN(
        "Having chosen a priority by boundary, the seed is a corner who is not a neighbour"
      ) {
        REQUIRE(
          seeds_pool->choose_new_seed(cc_graph->_is_fc_agglomerated).value()
          == 24
        );
      }
      iso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("There are three isotropic coarse cells") {
        REQUIRE(CHECK4CELLS(f2c, 5, 6, 24, 25));
        REQUIRE(CHECK4CELLS(f2c, 17, 18, 27, 26));
        REQUIRE(CHECK4CELLS(f2c, 4, 7, 15, 19));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2"
      ) {
        REQUIRE(CHECK2CELLS(f2c, 0, 1));
        REQUIRE(CHECK2CELLS(f2c, 2, 3));
        REQUIRE(CHECK2CELLS(f2c, 9, 8));
        REQUIRE(CHECK2CELLS(f2c, 11, 10));
        REQUIRE(CHECK2CELLS(f2c, 12, 13));
        REQUIRE(CHECK2CELLS(f2c, 16, 14));
        REQUIRE(CHECK2CELLS(f2c, 20, 22));
        REQUIRE(CHECK2CELLS(f2c, 23, 21));
      }
      THEN("The coarse-cell numbering the anisotropy and the priority weights"
      ) {
        REQUIRE(f2c[0].value() == 0);
        REQUIRE(f2c[2].value() == 1);
        REQUIRE(f2c[4].value() == 10);
        REQUIRE(f2c[9].value() == 3);
        REQUIRE(f2c[11].value() == 2);
        REQUIRE(f2c[13].value() == 5);
        REQUIRE(f2c[16].value() == 4);
        REQUIRE(f2c[20].value() == 6);
        REQUIRE(f2c[23].value() == 7);
        REQUIRE(f2c[24].value() == 8);
        REQUIRE(f2c[27].value() == 9);
      }
    }
  }

  GIVEN(
    "We load a 4by7 quad 2D mesh [stretched wrt other similar cases] and consider all cells as anisotropic"
  ) {
#if 0
We stretch a little the top two layer of cells so that they are a rectangle (instead of a square) with the height
greater than the width. This implies that the max connection would be horizontal, whereas the anisotropic lines grow
vertically: in this case, we test that the construction take into account the direction.
We also have to modify the weights so that the seeds of the lines are at the bottom of the mesh which will ensure that
the line grows vertically
#endif
    DualGEx_aniso_3cell Data = DualGEx_aniso_3cell();
    const vector<CoMMAIndexT> bottom_layer = {0, 11, 16, 20};
    const vector<CoMMAIndexT> first_stretched_layer = {5, 6, 17, 18};
    const vector<CoMMAIndexT> second_stretched_layer = {24, 25, 26, 27};
    for (const auto &idx : first_stretched_layer) {
      // Stretching center
      Data.centers[idx][1] += 0.5;
      for (CoMMAIndexT i = Data.adjMatrix_row_ptr[idx];
           i < Data.adjMatrix_row_ptr[idx + 1];
           ++i) {
        const auto neigh = Data.adjMatrix_col_ind[i];
        if (find(
              first_stretched_layer.begin(), first_stretched_layer.end(), neigh
            )
            != first_stretched_layer.end()) {
          // Stretch the face
          Data.adjMatrix_areaValues[i] += 0.5;
        }
      }
    }
    // Same for second layer
    for (const auto &idx : second_stretched_layer) {
      // Stretching center
      Data.centers[idx][1] += 1.0;
      for (CoMMAIndexT i = Data.adjMatrix_row_ptr[idx];
           i < Data.adjMatrix_row_ptr[idx + 1];
           ++i) {
        const auto neigh = Data.adjMatrix_col_ind[i];
        if (find(
              second_stretched_layer.begin(),
              second_stretched_layer.end(),
              neigh
            )
            != second_stretched_layer.end()) {
          // Stretch the face
          Data.adjMatrix_areaValues[i] += 0.5;
        }
      }
    }
    const auto max_w = *max_element(Data.weights.begin(), Data.weights.end());
    for (const auto idx : bottom_layer)
      Data.weights[idx] = max_w + 1.;
    const CoMMAWeightT aniso_thresh{-4.
    };  // Negative so that every cell is considered
    const bool build_lines = true;
    const vector<CoMMAIndexT> agglomerationLines_Idx{};
    const vector<CoMMAIndexT> agglomerationLines{};
    shared_ptr<SeedsPoolT> const seeds_pool =
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
      Data.arrayOfFineAnisotropicCompliantCells
    );
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      aniso_thresh,
      agglomerationLines_Idx,
      agglomerationLines,
      Data.weights,
      build_lines,
      ODD_LINE_LENGTH,
      MAX_CELLS_IN_LINE,
      Data.dim,
      CELL_COUPLING_MAX,
      FORCE_DIRECTION
    );
    const Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
      iso_agg(
        fc_graph,
        cc_graph,
        seeds_pool,
        CoMMANeighbourhoodT::EXTENDED,
        FC_ITER,
        Data.dim
      );
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      THEN("All cells have been agglomerated") {
        REQUIRE(
          Data.nb_fc
          == static_cast<CoMMAIndexT>(count(
            cc_graph->_is_fc_agglomerated.begin(),
            cc_graph->_is_fc_agglomerated.end(),
            true
          ))
        );
      }
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("The anisotropic coarse cells at the bottom are of cardinality 2") {
        REQUIRE(CHECK2CELLS(f2c, 0, 1));
        REQUIRE(CHECK2CELLS(f2c, 2, 3));
        REQUIRE(CHECK2CELLS(f2c, 9, 8));
        REQUIRE(CHECK2CELLS(f2c, 11, 10));
        REQUIRE(CHECK2CELLS(f2c, 12, 13));
        REQUIRE(CHECK2CELLS(f2c, 16, 14));
        REQUIRE(CHECK2CELLS(f2c, 20, 22));
        REQUIRE(CHECK2CELLS(f2c, 23, 21));
      }
      THEN("The anisotropic coarse cells at the top are of cardinality 3") {
        REQUIRE(CHECK3CELLS(f2c, 4, 5, 24));
        REQUIRE(CHECK3CELLS(f2c, 7, 6, 25));
        REQUIRE(CHECK3CELLS(f2c, 15, 17, 26));
        REQUIRE(CHECK3CELLS(f2c, 19, 18, 27));
      }
      THEN("The coarse-cell numbering the anisotropy and the priority weights"
      ) {
        REQUIRE(f2c[0].value() == 0);
        REQUIRE(f2c[2].value() == 1);
        REQUIRE(f2c[4].value() == 2);
        REQUIRE(f2c[11].value() == 3);
        REQUIRE(f2c[9].value() == 4);
        REQUIRE(f2c[7].value() == 5);
        REQUIRE(f2c[16].value() == 6);
        REQUIRE(f2c[12].value() == 7);
        REQUIRE(f2c[15].value() == 8);
        REQUIRE(f2c[20].value() == 9);
        REQUIRE(f2c[21].value() == 10);
        REQUIRE(f2c[19].value() == 11);
      }
    }
  }

  GIVEN(
    "We load a 4by7 quad 2D mesh which has 4 anisotropic lines each of length 5 cells and"
    " a seeds pool with neighbourhood priority and priority weights for inverse numbering"
  ) {
    const DualGEx_aniso_3cell Data = DualGEx_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    const bool build_lines = true;
    const vector<CoMMAIndexT> agglomerationLines_Idx{};
    const vector<CoMMAIndexT> agglomerationLines{};
    vector<CoMMAWeightT> wei(Data.nb_fc);
    iota(wei.begin(), wei.end(), 0);
    shared_ptr<Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>> const
      seeds_pool = make_shared<Seeds_Pool_Neighbourhood_Priority<
        CoMMAIndexT,
        CoMMAWeightT,
        CoMMAIntT>>(Data.n_bnd_faces, wei, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
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
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      aniso_thresh,
      agglomerationLines_Idx,
      agglomerationLines,
      wei,
      build_lines,
      ODD_LINE_LENGTH,
      MAX_CELLS_IN_LINE,
      Data.dim,
      CELL_COUPLING_MAX,
      FORCE_DIRECTION
    );
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> iso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      CoMMANeighbourhoodT::EXTENDED,
      FC_ITER,
      Data.dim
    );
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, wei, false);
      aniso_agg.update_seeds_pool();
      THEN(
        "After the anisotropic agglomeration, the seeds pool is not empty and does not need an initialization"
      ) {
        REQUIRE(!seeds_pool->is_empty());
        REQUIRE(!seeds_pool->need_initialization(cc_graph->_is_fc_agglomerated)
        );
      }
      THEN(
        "Having chosen a neighbourhood by boundary, the seed is a neighbour of the first line"
      ) {
        REQUIRE(
          seeds_pool->choose_new_seed(cc_graph->_is_fc_agglomerated).value()
          == 18
        );
      }
      iso_agg.agglomerate_one_level(4, 4, 4, wei, false);
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("There are two isotropic coarse cells") {
        REQUIRE(CHECK4CELLS(f2c, 5, 6, 24, 25));
        REQUIRE(CHECK4CELLS(f2c, 17, 18, 27, 26));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2"
      ) {
        REQUIRE(CHECK2CELLS(f2c, 0, 1));
        REQUIRE(CHECK2CELLS(f2c, 11, 10));
        REQUIRE(CHECK2CELLS(f2c, 16, 14));
        REQUIRE(CHECK2CELLS(f2c, 20, 22));
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
        REQUIRE(CHECK3CELLS(f2c, 2, 3, 4));
        REQUIRE(CHECK3CELLS(f2c, 9, 8, 7));
        REQUIRE(CHECK3CELLS(f2c, 12, 13, 15));
        REQUIRE(CHECK3CELLS(f2c, 23, 21, 19));
      }
      THEN(
        "The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
        " and the priority weights"
      ) {
        REQUIRE(f2c[0].value() == 6);
        REQUIRE(f2c[2].value() == 7);
        REQUIRE(f2c[9].value() == 5);
        REQUIRE(f2c[11].value() == 4);
        REQUIRE(f2c[13].value() == 3);
        REQUIRE(f2c[16].value() == 2);
        REQUIRE(f2c[20].value() == 0);
        REQUIRE(f2c[23].value() == 1);
      }
    }
  };

  GIVEN(
    "We load a 4by7 quad 2D mesh which has 4 anisotropic lines each of length 5 cells and"
    " we simulate a restart (not first agglomeration, we already have the lines)"
  ) {
    const DualGEx_aniso_3cell Data = DualGEx_aniso_3cell();
    const CoMMAWeightT aniso_thresh{2.};
    const bool build_lines = false;
    const vector<CoMMAIndexT> agglomerationLines_Idx = {0, 5, 10, 15, 20};
    // clang-format off
    const vector<CoMMAIndexT> agglomerationLines = {
      0, 1, 2, 3, 4,
      11, 10, 9, 8, 7,
      16, 14, 12, 13, 15,
      20, 22, 23, 21, 19};
    // clang-format on
    shared_ptr<SeedsPoolT> const seeds_pool =
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
      Data.arrayOfFineAnisotropicCompliantCells
    );
    shared_ptr<CCContainerT> const cc_graph =
      make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> aniso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      aniso_thresh,
      agglomerationLines_Idx,
      agglomerationLines,
      Data.weights,
      build_lines,
      ODD_LINE_LENGTH,
      MAX_CELLS_IN_LINE,
      Data.dim,
      CELL_COUPLING_MAX,
      FORCE_DIRECTION
    );
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> iso_agg(
      fc_graph,
      cc_graph,
      seeds_pool,
      CoMMANeighbourhoodT::EXTENDED,
      FC_ITER,
      Data.dim
    );
    WHEN("We initialize the agglomerator") {
      THEN("The anisotropic lines have been read correctly and in order") {
        const auto n_lines = agglomerationLines_Idx.size() - 1;
        REQUIRE(static_cast<CoMMAIndexT>(n_lines) == aniso_agg._nb_lines[0]);
        auto read_line = aniso_agg._v_lines[0].cbegin();
        for (auto ref_line = agglomerationLines_Idx.cbegin() + 1;
             ref_line != agglomerationLines_Idx.cend();
             ++ref_line, ++read_line) {
          // I am not sure how to treat this...
          // NOLINTNEXTLINE
          for (auto ref_fc = agglomerationLines.cbegin() + (*(ref_line - 1));
               ref_fc != agglomerationLines.cbegin() + (*ref_line);
               ++ref_fc) {
            REQUIRE(
              find((*read_line)->cbegin(), (*read_line)->cend(), *ref_fc)
              != (*read_line)->cend()
            );
          }
        }
      }
    }
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      aniso_agg.update_seeds_pool();
      iso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("There are two isotropic coarse cells") {
        REQUIRE(CHECK4CELLS(f2c, 5, 6, 24, 25));
        REQUIRE(CHECK4CELLS(f2c, 17, 18, 27, 26));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2"
      ) {
        REQUIRE(CHECK2CELLS(f2c, 0, 1));
        REQUIRE(CHECK2CELLS(f2c, 11, 10));
        REQUIRE(CHECK2CELLS(f2c, 16, 14));
        REQUIRE(CHECK2CELLS(f2c, 20, 22));
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
        REQUIRE(CHECK3CELLS(f2c, 2, 3, 4));
        REQUIRE(CHECK3CELLS(f2c, 9, 8, 7));
        REQUIRE(CHECK3CELLS(f2c, 12, 13, 15));
        REQUIRE(CHECK3CELLS(f2c, 23, 21, 19));
      }
      THEN(
        "The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
        " and the priority weights"
      ) {
        REQUIRE(f2c[0].value() == 0);
        REQUIRE(f2c[2].value() == 1);
        REQUIRE(f2c[9].value() == 3);
        REQUIRE(f2c[11].value() == 2);
        REQUIRE(f2c[13].value() == 5);
        REQUIRE(f2c[16].value() == 4);
        REQUIRE(f2c[20].value() == 6);
        REQUIRE(f2c[23].value() == 7);
      }
    }
  }
#undef CHECK2CELLS
#undef CHECK3CELLS
#undef CHECK4CELLS
}

#define OUTCHECK2CELLS(f2c, a, b) ((f2c)[(a)] == (f2c)[(b)])
#define OUTCHECK3CELLS(f2c, a, b, c) \
  (OUTCHECK2CELLS(f2c, a, b) && OUTCHECK2CELLS(f2c, b, c))
#define OUTCHECK4CELLS(f2c, a, b, c, d)                   \
  (OUTCHECK2CELLS(f2c, a, b) && OUTCHECK2CELLS(f2c, b, c) \
   && OUTCHECK2CELLS(f2c, c, d))

SCENARIO("Test the anisotropic line computations", "[Anisotropic lines]") {
  GIVEN("a 4by7 quad 2D mesh which has 4 anisotropic lines") {
    const DualGEx_aniso_3cell Data = DualGEx_aniso_3cell();
    WHEN("We agglomerate with neighbourhood priority") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool aniso = true, build_lines = true, odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAWeightT aniso_thr = 4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.arrayOfFineAnisotropicCompliantCells,
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
      THEN("There are two isotropic coarse cells") {
        REQUIRE(OUTCHECK4CELLS(fc2cc, 5, 6, 24, 25));
        REQUIRE(OUTCHECK4CELLS(fc2cc, 17, 18, 27, 26));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2"
      ) {
        REQUIRE(OUTCHECK2CELLS(fc2cc, 0, 1));
        REQUIRE(OUTCHECK2CELLS(fc2cc, 11, 10));
        REQUIRE(OUTCHECK2CELLS(fc2cc, 16, 14));
        REQUIRE(OUTCHECK2CELLS(fc2cc, 20, 22));
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
        REQUIRE(OUTCHECK3CELLS(fc2cc, 2, 3, 4));
        REQUIRE(OUTCHECK3CELLS(fc2cc, 9, 8, 7));
        REQUIRE(OUTCHECK3CELLS(fc2cc, 12, 13, 15));
        REQUIRE(OUTCHECK3CELLS(fc2cc, 23, 21, 19));
      }
      THEN(
        "The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
        " and the priority weights"
      ) {
        REQUIRE(fc2cc[0] == 0);
        REQUIRE(fc2cc[2] == 1);
        REQUIRE(fc2cc[9] == 3);
        REQUIRE(fc2cc[11] == 2);
        REQUIRE(fc2cc[13] == 5);
        REQUIRE(fc2cc[16] == 4);
        REQUIRE(fc2cc[20] == 6);
        REQUIRE(fc2cc[23] == 7);
      }
    }
    WHEN(
      "We agglomerate with neighbourhood priority and limit the line length to 3"
    ) {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc), alines_idx{}, alines{};
      const bool aniso = true, build_lines = true, odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4, min_card = 4, max_card = 4;
      const CoMMAIndexT max_line_length = 3;
      const CoMMAWeightT aniso_thr = 4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
        Data.adjMatrix_row_ptr,
        Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues,
        Data.volumes,
        Data.centers,
        Data.weights,
        Data.arrayOfFineAnisotropicCompliantCells,
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
        max_card,
        SING_CARD_THRESH,
        max_line_length
      );
      THEN("There are four isotropic coarse cells") {
        REQUIRE(OUTCHECK4CELLS(fc2cc, 5, 6, 24, 25));
        REQUIRE(OUTCHECK4CELLS(fc2cc, 17, 18, 27, 26));
        REQUIRE(OUTCHECK4CELLS(fc2cc, 3, 4, 7, 8));
        REQUIRE(OUTCHECK4CELLS(fc2cc, 13, 15, 19, 21));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 3"
      ) {
        REQUIRE(OUTCHECK3CELLS(fc2cc, 0, 1, 2));
        REQUIRE(OUTCHECK3CELLS(fc2cc, 11, 10, 9));
        REQUIRE(OUTCHECK3CELLS(fc2cc, 16, 14, 12));
        REQUIRE(OUTCHECK3CELLS(fc2cc, 20, 22, 23));
      }
      THEN(
        "The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
        " and the priority weights"
      ) {
        REQUIRE(fc2cc[0] == 0);
        REQUIRE(fc2cc[11] == 1);
        REQUIRE(fc2cc[16] == 2);
        REQUIRE(fc2cc[20] == 3);
      }
    }
  }
}

#undef OUTCHECK2CELLS
#undef OUTCHECK3CELLS
#undef OUTCHECK4CELLS
