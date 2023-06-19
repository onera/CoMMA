#include "catch2/catch.hpp"

#include "test_defs.h"
#include "input/DualGPy.h"

using namespace std;

SCENARIO("Test the correction in 2D", "[Isotropic Correction]") {
  GIVEN("We load the Minimal Isotropic mesh structure") {
    const DualGPy_minimal Data = DualGPy_minimal();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    auto agg = make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
    // COMPLETE THE TEST
    WHEN("We proceed with the Isotropic agglomeration") {
      agg->agglomerate_one_level(2, 2, 2, Data.weights, true);

      THEN("No cells are left with cardinality 1") {
        for (auto i = cc_graph->_ccs.begin(); i != cc_graph->_ccs.end(); i++) {
          REQUIRE(i->second->_cardinality != 1);
        }
      }
    }
  };
#define fc_in_cc(graph, fc, cc) graph->_fc_2_cc[fc].value() == cc
  GIVEN("A simple 8-cell Cartesian grid") {
    const DualGPy_correction Data = DualGPy_correction();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1") {
      cc_graph->create_cc({0,4,5}, 1);
      cc_graph->create_cc({1}, 0);
      cc_graph->create_cc({2,3,6,7}, 2);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 1));
        REQUIRE(fc_in_cc(cc_graph, 2, 2));
        REQUIRE(fc_in_cc(cc_graph, 3, 2));
        REQUIRE(fc_in_cc(cc_graph, 6, 2));
        REQUIRE(fc_in_cc(cc_graph, 7, 2));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cell has been agglomerated") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 1));
        REQUIRE(fc_in_cc(cc_graph, 3, 1));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
      }
    }
    WHEN("We perform the same test as above, but test whats happens if the singular cell is the first one") {
      cc_graph->create_cc({1}, 0);
      cc_graph->create_cc({0,4,5}, 1);
      cc_graph->create_cc({2,3,6,7}, 2);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 0, 1));
        REQUIRE(fc_in_cc(cc_graph, 4, 1));
        REQUIRE(fc_in_cc(cc_graph, 5, 1));
        REQUIRE(fc_in_cc(cc_graph, 2, 2));
        REQUIRE(fc_in_cc(cc_graph, 3, 2));
        REQUIRE(fc_in_cc(cc_graph, 6, 2));
        REQUIRE(fc_in_cc(cc_graph, 7, 2));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cell has been agglomerated") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 1));
        REQUIRE(fc_in_cc(cc_graph, 3, 1));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
      }
    }
    WHEN("We perform the same test as above, but test whats happens if the singular cell is the last one") {
      cc_graph->create_cc({0,4,5}, 1);
      cc_graph->create_cc({2,3,6,7}, 2);
      cc_graph->create_cc({1}, 0);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 1));
        REQUIRE(fc_in_cc(cc_graph, 3, 1));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 1, 2));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cell has been agglomerated") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 1));
        REQUIRE(fc_in_cc(cc_graph, 3, 1));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
      }
    }
  }
  GIVEN("A simple 3x3 Cartesian grid") {
    const DualGPy_quad_3 Data = DualGPy_quad_3();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1 "
         "and two coarse cells with cardinality 5 and 3") {
      cc_graph->create_cc({0,1,2,5,8}, 1);
      cc_graph->create_cc({4}, 0);
      cc_graph->create_cc({3,6,7}, 1);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 0));
        REQUIRE(fc_in_cc(cc_graph, 3, 2));
        REQUIRE(fc_in_cc(cc_graph, 4, 1));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 6, 2));
        REQUIRE(fc_in_cc(cc_graph, 7, 2));
        REQUIRE(fc_in_cc(cc_graph, 8, 0));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cell has been agglomerated "
           "to the coarse cell which has an increased in the compactness (no matter the max cardinality)") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 0));
        REQUIRE(fc_in_cc(cc_graph, 3, 1));
        REQUIRE(fc_in_cc(cc_graph, 4, 1));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 0));
      }
    }
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1 "
         "and three coarse cells with cardinality 4, 2, and 2") {
      cc_graph->create_cc({0,3}, 1);
      cc_graph->create_cc({6,7}, 1);
      cc_graph->create_cc({4}, 0);
      cc_graph->create_cc({1,2,5,8}, 1);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 3));
        REQUIRE(fc_in_cc(cc_graph, 2, 3));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 2));
        REQUIRE(fc_in_cc(cc_graph, 5, 3));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 3));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cell has been agglomerated "
           "to the coarse cell with which it shares the most faces (no matter the max cardinality)") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 2));
        REQUIRE(fc_in_cc(cc_graph, 2, 2));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 2));
        REQUIRE(fc_in_cc(cc_graph, 5, 2));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 2));
      }
    }
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1 "
         "and 3 cells with cardinality 4, 2, and 2") {
      cc_graph->create_cc({0,1,3,4}, 2);
      cc_graph->create_cc({7,8}, 1);
      cc_graph->create_cc({6}, 0);
      cc_graph->create_cc({2,5}, 1);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 3));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 3));
        REQUIRE(fc_in_cc(cc_graph, 6, 2));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 1));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cell has been agglomerated "
           "to the coarse cell with the lowest cardinality") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 2));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 2));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 1));
      }
    }
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1 "
         "and four coarse cells with cardinality 2") {
      cc_graph->create_cc({0,3}, 1);
      cc_graph->create_cc({6,7}, 1);
      cc_graph->create_cc({4}, 0);
      cc_graph->create_cc({5,8}, 1);
      cc_graph->create_cc({1,2}, 1);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 4));
        REQUIRE(fc_in_cc(cc_graph, 2, 4));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 2));
        REQUIRE(fc_in_cc(cc_graph, 5, 3));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 3));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cell has been agglomerated "
           "to the coarse cell with lower ID (here, the coarse cells have equivalent features "
           "wrt to the fine cell, hence we cannot choose)") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 3));
        REQUIRE(fc_in_cc(cc_graph, 2, 3));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 2));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 2));
      }
    }
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1 "
         "and four coarse cells with cardinality 2, one of which is anisotropic") {
      cc_graph->create_cc({0,3}, 1, true);
      cc_graph->create_cc({6,7}, 1);
      cc_graph->create_cc({4}, 0);
      cc_graph->create_cc({5,8}, 1);
      cc_graph->create_cc({1,2}, 1);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 4));
        REQUIRE(fc_in_cc(cc_graph, 2, 4));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 2));
        REQUIRE(fc_in_cc(cc_graph, 5, 3));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 3));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cell has been agglomerated "
           "to the coarse ISOTROPIC cell with lower ID") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 3));
        REQUIRE(fc_in_cc(cc_graph, 2, 3));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 1));
        REQUIRE(fc_in_cc(cc_graph, 5, 2));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 2));
      }
    }
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1 "
         "and four coarse cells with cardinality 2, all of which are anisotropic") {
      cc_graph->create_cc({0,3}, 1, true);
      cc_graph->create_cc({6,7}, 1, true);
      cc_graph->create_cc({4}, 0);
      cc_graph->create_cc({5,8}, 1, true);
      cc_graph->create_cc({1,2}, 1, true);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 4));
        REQUIRE(fc_in_cc(cc_graph, 2, 4));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 2));
        REQUIRE(fc_in_cc(cc_graph, 5, 3));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 3));
      }
      cc_graph->correct(4);
      THEN("Nothing changes after correction because we do not agglomerate to anisotropic cells") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 4));
        REQUIRE(fc_in_cc(cc_graph, 2, 4));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 2));
        REQUIRE(fc_in_cc(cc_graph, 5, 3));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 3));
      }
    }
    WHEN("We agglomerate (manually) leaving two coarse cells with cardinality 1") {
      cc_graph->create_cc({0,1}, 1);
      cc_graph->create_cc({2}, 0);
      cc_graph->create_cc({6}, 0);
      cc_graph->create_cc({3,4,7}, 1);
      cc_graph->create_cc({5,8}, 1);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 1));
        REQUIRE(fc_in_cc(cc_graph, 3, 3));
        REQUIRE(fc_in_cc(cc_graph, 4, 3));
        REQUIRE(fc_in_cc(cc_graph, 5, 4));
        REQUIRE(fc_in_cc(cc_graph, 6, 2));
        REQUIRE(fc_in_cc(cc_graph, 7, 3));
        REQUIRE(fc_in_cc(cc_graph, 8, 4));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cells have been agglomerated") {
        REQUIRE(fc_in_cc(cc_graph, 0, 0));
        REQUIRE(fc_in_cc(cc_graph, 1, 0));
        REQUIRE(fc_in_cc(cc_graph, 2, 0));
        REQUIRE(fc_in_cc(cc_graph, 3, 1));
        REQUIRE(fc_in_cc(cc_graph, 4, 1));
        REQUIRE(fc_in_cc(cc_graph, 5, 2));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 2));
      }
    }
    WHEN("We agglomerate (manually) leaving three coarse cells with cardinality 1") {
      cc_graph->create_cc({3,4,5}, 1);
      cc_graph->create_cc({6,7,8}, 1);
      cc_graph->create_cc({0}, 0);
      cc_graph->create_cc({1}, 0);
      cc_graph->create_cc({2}, 0);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph, 0, 2));
        REQUIRE(fc_in_cc(cc_graph, 1, 3));
        REQUIRE(fc_in_cc(cc_graph, 2, 4));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 1));
      }
      cc_graph->correct(4);
      THEN("Once the correction has been performed, the isolated cells have been agglomerated together") {
        REQUIRE(fc_in_cc(cc_graph, 0, 2));
        REQUIRE(fc_in_cc(cc_graph, 1, 2));
        REQUIRE(fc_in_cc(cc_graph, 2, 2));
        REQUIRE(fc_in_cc(cc_graph, 3, 0));
        REQUIRE(fc_in_cc(cc_graph, 4, 0));
        REQUIRE(fc_in_cc(cc_graph, 5, 0));
        REQUIRE(fc_in_cc(cc_graph, 6, 1));
        REQUIRE(fc_in_cc(cc_graph, 7, 1));
        REQUIRE(fc_in_cc(cc_graph, 8, 1));
      }
    }
    WHEN("We agglomerate (manually) and request a high singular threshold") {
      shared_ptr<CCContainerT> cc_graph_3thresh = make_shared<CCContainerT>(fc_graph, 3);
      cc_graph_3thresh->create_cc({0,1}, 1);
      cc_graph_3thresh->create_cc({2}, 0);
      cc_graph_3thresh->create_cc({3,4,5,6,7,8}, 2);
      THEN("We recover the forced order") {
        REQUIRE(fc_in_cc(cc_graph_3thresh, 0, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 1, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 2, 1));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 3, 2));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 4, 2));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 5, 2));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 6, 2));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 7, 2));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 8, 2));
      }
      cc_graph_3thresh->correct(9);
      THEN("Once the correction has been performed, we got only one cell") {
        REQUIRE(fc_in_cc(cc_graph_3thresh, 0, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 1, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 2, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 3, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 4, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 5, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 6, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 7, 0));
        REQUIRE(fc_in_cc(cc_graph_3thresh, 8, 0));
      }
    }
  }
  GIVEN("A simple 3x3 Cartesian grid to which we add an extra unconnected cell to"
        " simulate pathological partitions") {
    DualGPy_quad_3 Data = DualGPy_quad_3();
    // Adding the extra cell
    Data.nb_fc = 10;
    Data.adjMatrix_row_ptr.push_back(24);
    Data.n_bnd_faces.push_back(0);
    Data.volumes.push_back(1.);
    Data.centers.push_back({-10, -10});
    Data.weights.push_back(-1.);
    Data.arrayOfFineAnisotropicCompliantCells.push_back(9);
    //
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph, SING_CARD_THRESH);
    WHEN("We agglomerate (without correction)") {
      auto agg =
        make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
            fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
      agg->agglomerate_one_level(4, 2, 6, Data.weights, false);
      const auto &unconnected_cell = cc_graph->_fc_2_cc.back();
      THEN("The isolated cell is singular") {
        REQUIRE(unconnected_cell.has_value());
        REQUIRE(count_if(cc_graph->_fc_2_cc.begin(), cc_graph->_fc_2_cc.end(),
                         [&](const auto &fc){
                              return fc.has_value()
                                     && fc.value() == unconnected_cell.value();
                          }) == 1);
      }
      THEN("If we try to correct, the cell remain isolated") {
        cc_graph->correct(4);
        REQUIRE(count_if(cc_graph->_fc_2_cc.begin(), cc_graph->_fc_2_cc.end(),
                         [&](const auto &fc){
                              return fc.has_value()
                                     && fc.value() == unconnected_cell.value();
                          }) == 1);
      }
    }
    WHEN("We agglomerate (with correction)") {
      auto agg =
        make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
            fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
      agg->agglomerate_one_level(4, 2, 6, Data.weights, true);
      const auto &unconnected_cell = cc_graph->_fc_2_cc.back();
      THEN("The isolated cell is singular") {
        REQUIRE(unconnected_cell.has_value());
        REQUIRE(count_if(cc_graph->_fc_2_cc.begin(), cc_graph->_fc_2_cc.end(),
                         [&](const auto &fc){
                              return fc.has_value()
                                     && fc.value() == unconnected_cell.value();
                          }) == 1);
      }
    }
  }
#undef fc_in_cc
}
