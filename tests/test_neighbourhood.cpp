#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "catch2/catch.hpp"

#include "test_defs.h"
#include "input/DualGPy.h"
#include "Neighbourhood.h"

using namespace std;

using CoMMAPairT = pair<CoMMAIndexT, CoMMAWeightT>;
using CoMMAPairFindFirstBasedT = PairFindFirstBasedFunctor<CoMMAPairT>;

SCENARIO("Test neighbourhood computing", "[Neighbourhood]") {
  GIVEN("We have a 7x7 Cartesian 2D matrix") {
    const DualGPy_quad_7 Data = DualGPy_quad_7();
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    CoMMAIndexT seed = 24;
    CoMMAIntT neigh_order = 3;
    unordered_set<CoMMAIndexT> s_seeds = {seed};
    CoMMAIntT card = 4;
    WHEN("We compute neighbourhood of cell 24 (no cell is agglomerated)") {
      vector<bool> agglomerated = vector<bool>(Data.volumes.size(), false);
      unordered_map<CoMMAIndexT, CoMMAIntT> d_n_of_seed;
      fc_graph.compute_neighbourhood_of_cc(s_seeds, neigh_order, d_n_of_seed,
          card, agglomerated);
      vector< set<CoMMAIndexT> > neighs = vector< set<CoMMAIndexT> >(neigh_order);
      for (auto [k, v] : d_n_of_seed)
        neighs[v-1].insert(k);
#define is_in_order(idx, order) neighs[order].count(idx) > 0
      THEN("Sizes are good") {
        REQUIRE(neighs[0].size() == 4);
        REQUIRE(neighs[1].size() == 8);
        REQUIRE(neighs[2].size() == 12);
      }
      THEN("First order is good") {
        REQUIRE(is_in_order(17,0));
        REQUIRE(is_in_order(23,0));
        REQUIRE(is_in_order(25,0));
        REQUIRE(is_in_order(31,0));
      }
      THEN("Second order is good") {
        REQUIRE(is_in_order(10,1));
        REQUIRE(is_in_order(16,1));
        REQUIRE(is_in_order(18,1));
        REQUIRE(is_in_order(22,1));
        REQUIRE(is_in_order(26,1));
        REQUIRE(is_in_order(30,1));
        REQUIRE(is_in_order(32,1));
        REQUIRE(is_in_order(38,1));
      }
      THEN("Third order is good") {
        REQUIRE(is_in_order(3,2));
        REQUIRE(is_in_order(9,2));
        REQUIRE(is_in_order(11,2));
        REQUIRE(is_in_order(15,2));
        REQUIRE(is_in_order(19,2));
        REQUIRE(is_in_order(21,2));
        REQUIRE(is_in_order(27,2));
        REQUIRE(is_in_order(29,2));
        REQUIRE(is_in_order(33,2));
        REQUIRE(is_in_order(37,2));
        REQUIRE(is_in_order(39,2));
        REQUIRE(is_in_order(45,2));
      }
    } // WHEN PREVIOUS AGGLOMERATION
    WHEN("We compute neighbourhood of cell 24 (cell 10,16, 28-to-34 agglomerated)") {
      vector<bool> agglomerated = vector<bool>(Data.volumes.size(), false);
      agglomerated[10] = agglomerated[16] = true;
      for (int i = 28; i < 35; ++i)
        agglomerated[i] = true;
      unordered_map<CoMMAIndexT, CoMMAIntT> d_n_of_seed;
      fc_graph.compute_neighbourhood_of_cc(s_seeds, neigh_order, d_n_of_seed,
          card, agglomerated);
      vector< set<CoMMAIndexT> > neighs = vector< set<CoMMAIndexT> >(neigh_order);
      for (auto [k, v] : d_n_of_seed)
        neighs[v-1].insert(k);
      THEN("Sizes are good") {
        REQUIRE(neighs[0].size() == 3);
        REQUIRE(neighs[1].size() == 3);
        REQUIRE(neighs[2].size() == 5);
      }
      THEN("First order is good") {
        REQUIRE(is_in_order(17,0));
        REQUIRE(is_in_order(23,0));
        REQUIRE(is_in_order(25,0));
      }
      THEN("Second order is good") {
        REQUIRE(is_in_order(18,1));
        REQUIRE(is_in_order(22,1));
        REQUIRE(is_in_order(26,1));
      }
      THEN("Third order is good") {
        REQUIRE(is_in_order(11,2));
        REQUIRE(is_in_order(15,2));
        REQUIRE(is_in_order(19,2));
        REQUIRE(is_in_order(21,2));
        REQUIRE(is_in_order(27,2));
      }
    } // WHEN PREVIOUS AGGLOMERATION
#undef is_in_order
#define is_in(i,s) s.find(i) != s.end()
    WHEN("We compute the neighbourhood of a coarse cell") {
      vector<bool> agglomerated = vector<bool>(Data.nb_fc, false);
      unordered_set<CoMMAIndexT> cc = {16, 17, 18, 23, 24};
      THEN("The whole neighbourhood is returned if no cell is agglomerated") {
        auto neighs = fc_graph.get_neighbourhood_of_cc(cc, agglomerated);
        REQUIRE(neighs.size() == 9);
        REQUIRE(is_in(15, neighs));
        REQUIRE(is_in(22, neighs));
        REQUIRE(is_in(30, neighs));
        REQUIRE(is_in(31, neighs));
        REQUIRE(is_in(25, neighs));
        REQUIRE(is_in(19, neighs));
        REQUIRE(is_in(11, neighs));
        REQUIRE(is_in(10, neighs));
        REQUIRE(is_in(9, neighs));
      }
      agglomerated[15] = true;
      agglomerated[9] = true;
      agglomerated[19] = true;
      THEN("If some cells are agglomerated, then they do not appear in the neighbourhood") {
        auto neighs = fc_graph.get_neighbourhood_of_cc(cc, agglomerated);
        REQUIRE(neighs.size() == 6);
        REQUIRE(is_in(22, neighs));
        REQUIRE(is_in(30, neighs));
        REQUIRE(is_in(31, neighs));
        REQUIRE(is_in(25, neighs));
        REQUIRE(is_in(11, neighs));
        REQUIRE(is_in(10, neighs));
      }
    }
#undef is_in
  }
  GIVEN("We have a 7x7 Cartesian 2D matrix, a standard Neighbourhood for 24 a one given by the creator") {
 #define found_1stEl_(cont, obj) check_(find_if, !=, cont, CoMMAPairFindFirstBasedT(obj))
 #define not_found_1stEl_(cont, obj) check_(find_if, ==, cont, CoMMAPairFindFirstBasedT(obj))
    const DualGPy_quad_7 Data = DualGPy_quad_7();
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    CoMMAIndexT seed = 24;
    CoMMAIntT neigh_order = 2;
    unordered_set<CoMMAIndexT> s_seeds = {seed};
    CoMMAIntT card = 4;
    vector<bool> agglomerated = vector<bool>(Data.volumes.size(), false);
    unordered_map<CoMMAIndexT, CoMMAIntT> d_n_of_seed;
    fc_graph.compute_neighbourhood_of_cc(s_seeds, neigh_order, d_n_of_seed,
        card, agglomerated);
    unordered_set<CoMMAIndexT> s_neighbours_of_seed =
        d_keys_to_set<CoMMAIndexT, CoMMAIntT>(d_n_of_seed);
    Neighbourhood_Extended<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> neighbourhood(
        s_neighbours_of_seed, Data.weights);
    neighbourhood.update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first neighbourhood") {
      const auto fon = neighbourhood.get_candidates();
      THEN("Only direct neighbours are in the neighbourhood") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
        REQUIRE(found_(fon, 31));
      }
    }
    // Testing the neighbourhood creator
    NeighbourhoodExtendedCreator<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
      neigh_crtor{};
    auto c_neighbourhood = neigh_crtor.create(s_neighbours_of_seed,
                                              Data.weights, Data.dim);
    c_neighbourhood->update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first neighbourhood of the one got from the creator") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Only direct neighbours are in the neighbourhood") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
        REQUIRE(found_(fon, 31));
      }
    }
    neighbourhood.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      const auto fon = neighbourhood.get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE(not_found_(fon, 31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
        REQUIRE(found_(fon, 38));
      }
    }
    c_neighbourhood->update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31 to the creator-provided neighbourhood") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE(not_found_(fon, 31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
        REQUIRE(found_(fon, 38));
      }
    }
    auto copy_c_neighbourhood = neigh_crtor.clone(c_neighbourhood);
    // We update after the copy to see if that's really a copy
    c_neighbourhood->update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 31 to the copy of the creator-provided neighbourhood") {
      const auto fon = copy_c_neighbourhood->get_candidates();
      THEN("Cell 31 is no more in the neighbourhood") {
        REQUIRE(not_found_(fon, 31));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
      }
      THEN("Direct neighbours of 31 are in the neighbourhood") {
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
        REQUIRE(found_(fon, 38));
      }
    }
    neighbourhood.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      const auto fon = neighbourhood.get_candidates();
      THEN("Cell 38 is no more in the neighbourhood") {
        REQUIRE(not_found_(fon, 38));
      }
      THEN("Old neighbours are still in the neighbourhood") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
      }
      THEN("Direct neighbours of 31 are NOT in the neighbourhood (max order neighbourhood)") {
        REQUIRE(not_found_(fon, 37));
        REQUIRE(not_found_(fon, 39));
        REQUIRE(not_found_(fon, 45));
      }
    }
  }
  GIVEN("We have a 7x7 Cartesian 2D matrix and set up a Pure Front Neighbourhood for 24") {
    const DualGPy_quad_7 Data = DualGPy_quad_7();
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    CoMMAIndexT seed = 24;
    CoMMAIntT neigh_order = 2;
    unordered_set<CoMMAIndexT> s_seeds = {seed};
    CoMMAIntT card = 4;
    vector<bool> agglomerated = vector<bool>(Data.volumes.size(), false);
    unordered_map<CoMMAIndexT, CoMMAIntT> d_n_of_seed;
    fc_graph.compute_neighbourhood_of_cc(s_seeds, neigh_order, d_n_of_seed,
        card, agglomerated);
    unordered_set<CoMMAIndexT> s_neighbours_of_seed =
        d_keys_to_set<CoMMAIndexT, CoMMAIntT>(d_n_of_seed);
    Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> neighbourhood(
        s_neighbours_of_seed, Data.weights, Data.dim);
    neighbourhood.update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first neighbourhood") {
      const auto fon = neighbourhood.get_candidates();
      THEN("Only direct neighbours are in the neighbourhood") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
        REQUIRE(found_(fon, 31));
      }
    }
    // Testing the neighbourhood creator
    NeighbourhoodPureFrontCreator<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
      neigh_crtor{};
    auto c_neighbourhood = neigh_crtor.create(s_neighbours_of_seed,
                                              Data.weights, Data.dim);
    c_neighbourhood->update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first neighbourhood of the one got from the creator") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Only direct neighbours are in the neighbourhood") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
        REQUIRE(found_(fon, 31));
      }
    }
    neighbourhood.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      const auto fon = neighbourhood.get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
        REQUIRE(found_(fon, 38));
      }
      const auto prev_fon = neighbourhood.get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE(not_found_1stEl_(prev_fon, 31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE(found_1stEl_(prev_fon, 17));
        REQUIRE(found_1stEl_(prev_fon, 23));
        REQUIRE(found_1stEl_(prev_fon, 25));
      }
    }
    c_neighbourhood->update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31 to the creator-provided neighbourhood") {
      const auto fon = c_neighbourhood->get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
        REQUIRE(found_(fon, 38));
      }
      const auto prev_fon = dynamic_pointer_cast<
        Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
          c_neighbourhood)->get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE(not_found_1stEl_(prev_fon, 31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE(found_1stEl_(prev_fon, 17));
        REQUIRE(found_1stEl_(prev_fon, 23));
        REQUIRE(found_1stEl_(prev_fon, 25));
      }
    }
    auto copy_c_neighbourhood = neigh_crtor.clone(c_neighbourhood);
    // We update after the copy to see if that's really a copy
    c_neighbourhood->update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 31 to the copy of the creator-provided neighbourhood") {
      const auto fon = copy_c_neighbourhood->get_candidates();
      THEN("Direct neighbours of 31 are in the current neighbourhood") {
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
        REQUIRE(found_(fon, 38));
      }
      const auto prev_fon = dynamic_pointer_cast<
        Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
          copy_c_neighbourhood)->get_neighbours_by_level(1);
      THEN("Cell 31 is no more in the previous neighbourhood") {
        REQUIRE(not_found_1stEl_(prev_fon, 31));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE(found_1stEl_(prev_fon, 17));
        REQUIRE(found_1stEl_(prev_fon, 23));
        REQUIRE(found_1stEl_(prev_fon, 25));
      }
    }
    neighbourhood.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      const auto fon = neighbourhood.get_candidates();
      THEN("Direct neighbours of 31 are NOT in the neighbourhood (max order neighbourhood)") {
        REQUIRE(not_found_(fon, 37));
        REQUIRE(not_found_(fon, 39));
        REQUIRE(not_found_(fon, 45));
      }
      THEN("First ever computed neighbourhood is returned since no direct neighbours were added") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
      }
      const auto prev_fon = neighbourhood.get_neighbours_by_level(1);
      THEN("Cell 38 is no more in the previous neighbourhood") {
        REQUIRE(not_found_1stEl_(prev_fon, 38));
      }
      THEN("Old neighbours are still in the previous neighbourhood") {
        REQUIRE(found_1stEl_(prev_fon, 30));
        REQUIRE(found_1stEl_(prev_fon, 32));
      }
      const auto prev_fon_2 = neighbourhood.get_neighbours_by_level(2);
      THEN("Older neighbours are still in the second-to-last neighbourhood (which happens to be the first ever, hence the current)") {
        REQUIRE(found_1stEl_(prev_fon_2, 17));
        REQUIRE(found_1stEl_(prev_fon_2, 23));
        REQUIRE(found_1stEl_(prev_fon_2, 25));
      }
    }
#undef found_1stEl_
#undef not_found_1stEl_
  }
}
