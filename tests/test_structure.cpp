#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catch2/catch.hpp"

// I input with DualGPy configuration the configuration you can
// find in the README of the library
#include "input/DualGPy.h"
#include "CoMMATypes.h"

#include "deprecated/Bimap.h"
#include "deprecated/Priority_Pair.h"
#include "deprecated/Queue.h"
#include "deprecated/Tree.h"

using namespace std;

#define equal_up_to(a,b,eps) (fabs(a - b) < eps)

using CoMMAPairT = pair<CoMMAIndexT, CoMMAWeightT>;
using CoMMAPairFindFirstBasedT = PairFindFirstBasedFunctor<CoMMAPairT>;
using PairValueTestT = int; // Leave this since we might try something than what usually found in CoMMA

SCENARIO("Test of a structure", "[structure]") {
  GIVEN("A simple graph, and we build the Dual Graph") {
    DualGPy Data = DualGPy();
    // Construction of the Dual Graph element
    Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 2,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> cc_graph(fc_graph);
    // Check the effective length
    WHEN("We try to access to the member variables") {
      class test : public Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> {
       public:
        test(Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &graph,
             Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &cc_graph,
             Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &seeds_pool, CoMMAIntT dimension)
            : Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(graph, cc_graph,
                                                                seeds_pool, dimension) {};

        CoMMAIntT test_variable() {
          return (this->_threshold_card);
        };
      };
      THEN("We see that the agglomeration is not set, hence set to 0") {
        test *agg = new test(fc_graph, cc_graph, seeds_pool, 2);
        CoMMAIntT testing = agg->test_variable();
        REQUIRE(testing == 0);
      }
    }
    WHEN("We try to access to Define the cardinality") {
      class test : public Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> {
       public:
        test(Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &graph,
             Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &cc_graph,
             Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &seeds_pool, CoMMAIntT dimension)
            : Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(graph, cc_graph,
                                                                seeds_pool, dimension) {};

        CoMMAIntT thres() {
          return (_threshold_card);
        };
        CoMMAIntT max() {
          return (_max_card);
        };
        CoMMAIntT min() {
          return (_min_card);
        };
      };
      THEN("We see that the cardinality passes from 0 to 2") {
        test *agg = new test(fc_graph, cc_graph, seeds_pool, 2);

        REQUIRE(agg->thres() == 0);
        REQUIRE(agg->max() == 0);
        REQUIRE(agg->min() == 0);
        agg->set_agglomeration_parameter(2, 2, 2);
        REQUIRE(agg->thres() == 2);
        REQUIRE(agg->max() == 2);
        REQUIRE(agg->min() == 2);
      }
    }
  }
}

SCENARIO("Test of the Queue", "[Queue]") {
  GIVEN("A simple Queue of CoMMAIndexT type") {
    Queue<long> st_long;
    WHEN("We push a certain number of elements") {
      st_long.push(0);
      st_long.push(1);
      st_long.push(2);
      THEN(
          "We can check that the operation on the stack are correctly "
          "realized") {

        REQUIRE(st_long.top() == 0);
        CoMMAIndexT top = st_long.pop();
        REQUIRE(top == 0);
        REQUIRE(st_long.top() == 1);
        top = st_long.pop();
        REQUIRE(top == 1);
        REQUIRE(st_long.top() == 2);
        top = st_long.pop();
        REQUIRE(top == 2);
        REQUIRE(st_long.top() == -1);
      }
    };
  }
}

SCENARIO("Test of the tree", "[Tree]") {
  GIVEN("The node") {
    shared_ptr<Node<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>
      A(new Node<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(2, 1));
    shared_ptr<Tree<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>
      Albero(new Tree<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(A));
    WHEN("We add something to the tree") {
      Albero->insertSon(2, 1, 1, 1);
      THEN("The tree has a child") {
        Albero->insertSon(2, 3, 1, 1);
        Albero->insertSon(2, 4, 1, 1);
        Albero->insertSon(4, 2, 1, 0);
        //Albero->print();
        REQUIRE(Albero->_root->_sonc == 3);
        Albero->deleteNode(4);
        //Albero->print();
        REQUIRE(Albero->_root->_sonc == 2);
      }
    };
  }
}

SCENARIO("Test of Priority Pair", "[Priority Pair]") {
  GIVEN("Some pairs") {
    Priority_Pair<PairValueTestT, PairValueTestT> p1{0,1}, p1bis{0,1},
      p2{3,0}, p3{0,0};
    WHEN("We compare two equal pairs") {
      THEN("Equal pairs are identified") {
        REQUIRE(p1 == p1bis);
      }
    }
    WHEN("We compare two pair with different first element") {
      THEN("The one with the greater first element is identified as smaller") {
        REQUIRE(p2 < p1);
      }
    }
    WHEN("We compare two pair with the same first element") {
      THEN("The one with the smaller first element is identified as smaller") {
        REQUIRE(p3 < p1);
      }
    }
  }
  GIVEN("A set of pairs") {
    set<Priority_Pair<PairValueTestT, PairValueTestT>> s =
      {{0,1}, {0,1}, {3,0}, {0,0}, {2,-1}};
    WHEN("We iterate over the set") {
      vector<PairValueTestT> fe1 = {3,  2, 0, 0};
      vector<PairValueTestT> se1 = {0, -1, 0, 1};
      THEN("The expected order is obtained") {
        for (auto it = s.begin(); it != s.end(); ++it) {
          const auto idx = distance(s.begin(), it);
          REQUIRE(it->first()  == fe1[idx]);
          REQUIRE(it->second() == se1[idx]);
        }
      }
    }
    WHEN("We add an item and iterate on the new set") {
      s.emplace(1,4);
      vector<int> fe2 = {3,  2, 1, 0, 0};
      vector<int> se2 = {0, -1, 4, 0, 1};
      THEN("The expected order is obtained") {
        for (auto it = s.begin(); it != s.end(); ++it) {
          const auto idx = distance(s.begin(), it);
          REQUIRE(it->first()  == fe2[idx]);
          REQUIRE(it->second() == se2[idx]);
        }
      }
    }
  }
}

SCENARIO("Test custom pair comparison", "[Pair comparison]") {
  using PairT = pair<PairValueTestT, PairValueTestT>;
  GIVEN("Some (int,int) pairs in a set with custom 'Greater'") {
    set<PairT, CustomPairGreaterFunctor<PairT>> s = {{1,0}, {1,0}, {1,3}, {0,3}};
    WHEN("We have a look at the set:") {
      THEN("The expected order is obtained") {
        auto it = s.begin();
        REQUIRE(it->first  == 0);
        REQUIRE(it->second == 3);
        //
        it++;
        REQUIRE(it->first  == 1);
        REQUIRE(it->second == 3);
        //
        it++;
        REQUIRE(it->first  == 1);
        REQUIRE(it->second == 0);
      }
      THEN("Duplicates are not added") {
        REQUIRE(s.size() == 3);
      }
    }
  }
  GIVEN("Some (int,int) pairs in a set with custom 'Less'") {
    set<PairT, CustomPairLessFunctor<PairT>> s = {{1,3}, {1,0}, {1,0}, {0,3}};
    WHEN("We have a look at the set:") {
      THEN("The expected order is obtained") {
        auto it = s.begin();
        REQUIRE(it->first  == 1);
        REQUIRE(it->second == 0);
        //
        it++;
        REQUIRE(it->first  == 1);
        REQUIRE(it->second == 3);
        //
        it++;
        REQUIRE(it->first  == 0);
        REQUIRE(it->second == 3);
      }
      THEN("Duplicates are not added") {
        REQUIRE(s.size() == 3);
      }
    }
  }
}

SCENARIO("Subgraph", "[Subgraph]") {
  GIVEN("We have the CSR representation") {
    vector<CoMMAIndexT> adjMatrix_row_ptr = {0, 2, 4, 7, 9, 10};
    vector<CoMMAIndexT> adjMatrix_col_ind = {1, 3, 0, 2, 1, 3, 4, 0, 2, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    vector<CoMMAWeightT> volumes = {1, 1, 1, 1, 1};
    WHEN("We build the graph") {
      vector<CoMMAIndexT> _mapping_l_to_g = {20, 30, 40, 50, 60};
      auto Marion = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
          5, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues,
          volumes, _mapping_l_to_g, true);
      THEN("We remove a node") { Marion->remove_node(50); }
      THEN("We add a node") {
        vector<CoMMAIndexT> v_neigh = {30, 20};
        CoMMAIndexT i_fc = 10;
        vector<CoMMAWeightT> weight = {1, 1};
        Marion->insert_node(v_neigh, i_fc, 1, weight);
        REQUIRE(Marion->check_connectivity() == true);
        REQUIRE(Marion->_mapping_l_to_g[5] == 10);
      }
    }
  };
}

SCENARIO("Test of the seed pool", "[Seed_Pool]") {
  GIVEN("A 4x4x4 cube and a Seed Pool which should ensure that the order respects the cell numbering") {
    DualGPy_cube_4 Data = DualGPy_cube_4();
    Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights);
    deque<CoMMAIndexT> corners{}, ridges{}, valleys{}, interior{};
    for (CoMMAIndexT i = 0; i < Data.nb_fc; ++i) {
      switch (Data.n_bnd_faces[i]) {
        case 3:
          corners.push_back(i);
          break;
        case 2:
          ridges.push_back(i);
          break;
        case 1:
          valleys.push_back(i);
          break;
        case 0:
          interior.push_back(i);
          break;
        default:
          continue;
      } /* Switch */
    } /* For */
    vector<bool> agglomerated(Data.nb_fc, false);
    WHEN("We spoil the seed") {
      THEN("The order is respected") {
        for (auto i : corners) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
        for (auto i : ridges) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
        for (auto i : valleys) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
        for (auto i : interior) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
      }
    }
    WHEN("We simply update the seed pool") {
      // Spoil all the corners so that the seed is completely void
      for (auto i : corners) {
        seeds_pool.choose_new_seed(agglomerated);
        agglomerated[i] = true;
      }
      // In order: Internal, ridge, corner, valley
      vector<CoMMAIndexT> new_seeds = {21, 44, 63, 30};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.update(new_seeds);
      THEN("The order does not respect the priority but the boundary only") {
        REQUIRE(63 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[63] = true;
        REQUIRE(44 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[44] = true;
        REQUIRE(30 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[30] = true;
        REQUIRE(21 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[21] = true;
      }
      new_seeds = {10, 5, 6, 9};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.update(new_seeds);
      THEN("The order respects the order of the input if all in the same queue") {
        for (auto i : new_seeds) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
      }
    }
    WHEN("We update the seed pool by asking a reorder") {
      // Spoil all the corners so that the seed is completely void
      for (auto i : corners) {
        seeds_pool.choose_new_seed(agglomerated);
        agglomerated[i] = true;
      }
      // In order: Internal, ridge, corner, valley
      unordered_set<CoMMAIndexT> new_seeds = {21, 44, 63, 30};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.order_new_seeds_and_update(new_seeds);
      THEN("The order does not respect the priority but the boundary only") {
        REQUIRE(63 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[63] = true;
        REQUIRE(44 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[44] = true;
        REQUIRE(30 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[30] = true;
        REQUIRE(21 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[21] = true;
      }
      new_seeds = {10, 5, 6, 9};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.order_new_seeds_and_update(new_seeds);
      THEN("The order respects the priority all in the same queue") {
        REQUIRE(5 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[5] = true;
        REQUIRE(6 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[6] = true;
        REQUIRE(9 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[9] = true;
        REQUIRE(10 == seeds_pool.choose_new_seed(agglomerated));
        agglomerated[10] = true;
      }
    }
  }

  GIVEN("A 4x4x4 cube and a Seed Pool which should force an order reversed wrt the cell numbering") {
    DualGPy_cube_4 Data = DualGPy_cube_4();
    vector<CoMMAWeightT> w(Data.nb_fc);
    iota(w.begin(), w.end(), 0);
    Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, w);
    deque<CoMMAIndexT> corners{}, ridges{}, valleys{}, interior{};
    for (CoMMAIndexT i = 0; i < Data.nb_fc; ++i) {
      switch (Data.n_bnd_faces[i]) {
        case 3:
          corners.push_front(i);
          break;
        case 2:
          ridges.push_front(i);
          break;
        case 1:
          valleys.push_front(i);
          break;
        case 0:
          interior.push_front(i);
          break;
        default:
          continue;
      } /* Switch */
    } /* For */
    vector<bool> agglomerated(Data.nb_fc, false);
    WHEN("We spoil the seed") {
      THEN("The order is respected") {
        for (auto i : corners) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
        for (auto i : ridges) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
        for (auto i : valleys) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
        for (auto i : interior) {
          REQUIRE(i == seeds_pool.choose_new_seed(agglomerated));
          agglomerated[i] = true;
        }
      }
    }
  }
}

SCENARIO("Test dual graph and neighborhood computing", "[Dual graph & Neighborhood]") {
  GIVEN("We have a 7x7 Cartesian 2D matrix") {
    DualGPy_quad_7 Data = DualGPy_quad_7();
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 2,
        Data.s_anisotropic_compliant_fc);
    CoMMAIndexT seed = 24;
    CoMMAIntT neigh_order = 3;
    unordered_set<CoMMAIndexT> s_seeds = {seed};
    CoMMAIntT card = 4;
    WHEN("We compute neighborhood of cell 24 (no cell is agglomerated)") {
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
    WHEN("We compute neighborhood of cell 24 (cell 10,16, 28-to-34 agglomerated)") {
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
    WHEN("We compute the neighborhood of a coarse cell") {
      vector<bool> agglomerated = vector<bool>(Data.nb_fc, false);
      unordered_set<CoMMAIndexT> cc = {16, 17, 18, 23, 24};
      THEN("The whole neighborhood is returned if no cell is agglomerated") {
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
      THEN("If some cells are agglomerated, then they do not appear in the neighborhood") {
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
  };
  GIVEN("We have a 7x7 Cartesian 2D matrix and set up a standard First Order Neighborhood for 24") {
 #define check_(fun, op, cont, obj) fun(cont.begin(), cont.end(), obj) op cont.end()
 #define found_(cont, obj) check_(find, !=, cont, obj)
 #define not_found_(cont, obj) check_(find, ==, cont, obj)
 #define found_1stEl_(cont, obj) check_(find_if, !=, (*cont), CoMMAPairFindFirstBasedT(obj))
 #define not_found_1stEl_(cont, obj) check_(find_if, ==, (*cont), CoMMAPairFindFirstBasedT(obj))
    DualGPy_quad_7 Data = DualGPy_quad_7();
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 2,
        Data.s_anisotropic_compliant_fc);
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
    First_Order_Neighbourhood_Extended<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> f_o_n(
        s_neighbours_of_seed, Data.weights);
    auto fon = f_o_n.update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first FON") {
      THEN("Only direct neighbors are in the FON") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
        REQUIRE(found_(fon, 31));
      }
    }
    fon = f_o_n.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      THEN("Cell 31 is no more in the FON") {
        REQUIRE(not_found_(fon, 31));
      }
      THEN("Old neighbors are still in the FON") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
      }
      THEN("Direct neighbors of 31 are in the FON") {
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
        REQUIRE(found_(fon, 38));
      }
    }
    fon = f_o_n.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      THEN("Cell 38 is no more in the FON") {
        REQUIRE(not_found_(fon, 38));
      }
      THEN("Old neighbors are still in the FON") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
      }
      THEN("Direct neighbors of 31 are NOT in the FON (max order neighborhood)") {
        REQUIRE(not_found_(fon, 37));
        REQUIRE(not_found_(fon, 39));
        REQUIRE(not_found_(fon, 45));
      }
    }
  };
  GIVEN("We have a 7x7 Cartesian 2D matrix and set up a Pure Front First Order Neighborhood for 24") {
    DualGPy_quad_7 Data = DualGPy_quad_7();
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 2,
        Data.s_anisotropic_compliant_fc);
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
    First_Order_Neighbourhood_Pure_Front<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> f_o_n(
        s_neighbours_of_seed, Data.weights, 2);
    auto fon = f_o_n.update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first FON") {
      THEN("Only direct neighbors are in the FON") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
        REQUIRE(found_(fon, 31));
      }
    }
    fon = f_o_n.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      THEN("Direct neighbors of 31 are in the current FON") {
        REQUIRE(found_(fon, 30));
        REQUIRE(found_(fon, 32));
        REQUIRE(found_(fon, 38));
      }
      auto prev_fon = f_o_n._q_fon.begin() + 1;
      THEN("Cell 31 is no more in the previous FON") {
        REQUIRE(not_found_1stEl_(prev_fon, 31));
      }
      THEN("Old neighbors are still in the previous FON") {
        REQUIRE(found_1stEl_(prev_fon, 17));
        REQUIRE(found_1stEl_(prev_fon, 23));
        REQUIRE(found_1stEl_(prev_fon, 25));
      }
    }
    fon = f_o_n.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      THEN("Direct neighbors of 31 are NOT in the FON (max order neighborhood)") {
        REQUIRE(not_found_(fon, 37));
        REQUIRE(not_found_(fon, 39));
        REQUIRE(not_found_(fon, 45));
      }
      THEN("First ever computed FON is returned since no direct neighbors were added") {
        REQUIRE(found_(fon, 17));
        REQUIRE(found_(fon, 23));
        REQUIRE(found_(fon, 25));
      }
      auto prev_fon = f_o_n._q_fon.begin() + 1;
      THEN("Cell 38 is no more in the previous FON") {
        REQUIRE(not_found_1stEl_(prev_fon, 38));
      }
      THEN("Old neighbors are still in the previous FON") {
        REQUIRE(found_1stEl_(prev_fon, 30));
        REQUIRE(found_1stEl_(prev_fon, 32));
      }
      prev_fon++;
      THEN("Older neighbors are still in the second-to-last FON (which happens to be the first ever, hence the current)") {
        REQUIRE(found_1stEl_(prev_fon, 17));
        REQUIRE(found_1stEl_(prev_fon, 23));
        REQUIRE(found_1stEl_(prev_fon, 25));
      }
    }
#undef check_
#undef found_
#undef not_found_
#undef found_1stEl_
#undef not_found_1stEl_
  };
}

SCENARIO("Test of the in-house Bimap", "[Bimap]") {
  GIVEN(
      "We have a 4x4 square 2D matrix of 16 elements that we consider divided "
      "in 4 Subgraph the structure of the subgraph is the same, changes "
      "fundamentally the mapping") {
    vector<CoMMAIndexT> adjMatrix_row_ptr = {0, 2, 4, 6, 8};
    vector<CoMMAIndexT> adjMatrix_col_ind = {0, 1, 0, 2, 1, 3, 0, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues = {1, 1, 1, 1, 1, 1, 1, 1};
    vector<CoMMAWeightT> volumes = {1, 1, 1, 1};
    vector<CoMMAIndexT> mapping_l_to_g_0 = {0, 1, 5, 4};
    vector<CoMMAIndexT> mapping_l_to_g_1 = {2, 3, 7, 6};
    vector<CoMMAIndexT> mapping_l_to_g_2 = {8, 3, 13, 12};
    vector<CoMMAIndexT> mapping_l_to_g_3 = {10, 11, 15, 14};
    auto cc0 = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        4, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        mapping_l_to_g_0, true);
    auto cc1 = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        4, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        mapping_l_to_g_1, true);
    auto cc2 = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        4, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        mapping_l_to_g_2, true);
    auto cc3 = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        4, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        mapping_l_to_g_3, true);
    WHEN("We Collect the cells in the Bimap") {

      Bimap<CoMMAIndexT, shared_ptr<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>>
          Collection;
      Collection.insert(0, cc0);
      Collection.insert(1, cc1);
      Collection.insert(2, cc2);
      Collection.insert(3, cc3);
      THEN("We eliminate an element and we check the dimension changed") {
        auto lung = Collection.lung();
        REQUIRE(lung == 4);
        decltype(lung) elim = 2;
        Collection.erase_B(elim);
        lung = Collection.lung();
        REQUIRE(lung == 3);
        for (auto i = elim + 1; i != lung + 1; i++) {
          Collection.update_nodeB(i, i - 1);
        }
        //Collection.print();
      }
    }
  };
}

SCENARIO("Test the insertion of a coarse cell and deletion",
         "[Insertion Deletion]") {
  GIVEN("We have a Bimap of CoMMAIndexT and shared_ptr") {
    vector<CoMMAIndexT> adjMatrix_row_ptr = {0, 2, 4, 7, 9, 10};
    vector<CoMMAIndexT> adjMatrix_col_ind = {1, 3, 0, 2, 1, 3, 4, 0, 2, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    vector<CoMMAWeightT> volumes = {1, 1, 1, 1, 1};
    vector<CoMMAIndexT> _mapping_l_to_g = {20, 30, 40, 50, 60};
    auto Marion = make_shared<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        5, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        _mapping_l_to_g, true);
    Bimap<CoMMAIndexT, shared_ptr<Subgraph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>> Collection;
    WHEN("We insert an element and we delete it") {
      CoMMAIndexT ins = 0;
      Collection.insert(ins, Marion);
      //Collection.print();
      auto prova = Collection.get_B(ins);
      Collection.erase_B(ins);
      THEN("Bimap is empty") { REQUIRE(Collection.empty() == true); }
    }
  };
}

SCENARIO("Test the Isotropic agglomeration for small 3D cases",
         "[Isotropic]") {
  GIVEN("We load the Isotropic mesh structure") {
    DualGPy_cube_4 Data = DualGPy_cube_4();
    Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 3,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_graph(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_graph, seeds_pool, 3);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(8, 8, 8, Data.weights, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        const auto &fccc = cc_graph._fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }

      }
    }
    WHEN("We agglomerate the mesh with a biconnected agglomerator and we try to correct") {
      agg->agglomerate_one_level(8, 8, 8, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_graph._fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }
      }
    }

    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_PF_graph(fc_graph);
    auto agg_PF =
      make_unique<Agglomerator_Pure_Front<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_PF_graph, seeds_pool, 3);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(8, 8, 8, Data.weights, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        const auto &fccc = cc_PF_graph._fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }
      }
    }
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct") {
      agg_PF->agglomerate_one_level(8, 8, 8, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_PF_graph._fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }
      }
    }

    WHEN("We compute the aspect-ratio of a coarse cell on the boundary") {
      const CoMMAWeightT eps = 1e-10;
      // In
      unordered_set<CoMMAIndexT> cc = {0,1,4,5};
      CoMMAWeightT tmp_diam{-1.}, tmp_vol{-1.};
      CoMMAWeightT cc_diam = sqrt(2.),
                   cc_vol  = 4.;
      // Out
      CoMMAIntT shared_faces;
      CoMMAWeightT ar;
      CoMMAWeightT ref_diam = sqrt(3.),
                   ref_vol  = 5.;
      CoMMAWeightT ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("New coarse cell has 1 shared face") {
        agg->compute_next_cc_features(17, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      cc.insert(17);
      cc_diam = ref_diam, cc_vol = ref_vol;
      ref_vol = 6.; // ref_diam does not change
      ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("New coarse cell has 2 shared face") {
        agg->compute_next_cc_features(21, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 2);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      cc.insert(21);
      cc.insert(20);
      cc_vol = 7.; // cc_diam does not change
      ref_vol = 8.; // ref_diam does not change
      ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("New coarse cell has 3 shared face") {
        agg->compute_next_cc_features(16, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 3);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
    } // Aspect ratio

  };
}

SCENARIO("Test the Isotropic agglomeration for small 2D cases",
         "[Isotropic]") {
  GIVEN("We load the Isotropic mesh structure") {
    DualGPy_quad_4 Data = DualGPy_quad_4();
    Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 2,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_graph(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_graph, seeds_pool, 2);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(4, 4, 4, Data.weights, false);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        const auto &fccc = cc_graph._fc_2_cc;
        REQUIRE(fccc[0]== 0);
        REQUIRE(fccc[1]== 0);
        REQUIRE(fccc[2]== 1);
        REQUIRE(fccc[3]== 1);
        REQUIRE(fccc[4]== 0);
        REQUIRE(fccc[5]== 0);
        REQUIRE(fccc[6]== 1);
        REQUIRE(fccc[7]== 1);
        REQUIRE(fccc[8]== 2);
        REQUIRE(fccc[9]== 2);
        REQUIRE(fccc[10]== 3);
        REQUIRE(fccc[11]== 3);
        REQUIRE(fccc[12]== 2);
        REQUIRE(fccc[13]== 2);
        REQUIRE(fccc[14]== 3);
        REQUIRE(fccc[15]== 3);
      }
    }
    WHEN("We agglomerate the mesh with a biconnected agglomerator and we try to correct") {
      agg->agglomerate_one_level(4, 4, 4, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_graph._fc_2_cc;
        REQUIRE(fccc[0]== 0);
        REQUIRE(fccc[1]== 0);
        REQUIRE(fccc[2]== 1);
        REQUIRE(fccc[3]== 1);
        REQUIRE(fccc[4]== 0);
        REQUIRE(fccc[5]== 0);
        REQUIRE(fccc[6]== 1);
        REQUIRE(fccc[7]== 1);
        REQUIRE(fccc[8]== 2);
        REQUIRE(fccc[9]== 2);
        REQUIRE(fccc[10]== 3);
        REQUIRE(fccc[11]== 3);
        REQUIRE(fccc[12]== 2);
        REQUIRE(fccc[13]== 2);
        REQUIRE(fccc[14]== 3);
        REQUIRE(fccc[15]== 3);
      }
    }

    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_PF_graph(fc_graph);
    auto agg_PF =
      make_unique<Agglomerator_Pure_Front<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_PF_graph, seeds_pool, 2);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(4, 4, 4, Data.weights, false);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        // Nothing changes with respect to the case of the standard Biconnected
        // We have a trick that make it work as it should
        const auto &fccc = cc_PF_graph._fc_2_cc;
        REQUIRE(fccc[0]== 0);
        REQUIRE(fccc[1]== 0);
        REQUIRE(fccc[2]== 1);
        REQUIRE(fccc[3]== 1);
        REQUIRE(fccc[4]== 0);
        REQUIRE(fccc[5]== 0);
        REQUIRE(fccc[6]== 1);
        REQUIRE(fccc[7]== 1);
        REQUIRE(fccc[8]== 2);
        REQUIRE(fccc[9]== 2);
        REQUIRE(fccc[10]== 3);
        REQUIRE(fccc[11]== 3);
        REQUIRE(fccc[12]== 2);
        REQUIRE(fccc[13]== 2);
        REQUIRE(fccc[14]== 3);
        REQUIRE(fccc[15]== 3);
      }
    }
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct") {
      agg_PF->agglomerate_one_level(4, 4, 4, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_PF_graph._fc_2_cc;
        REQUIRE(fccc[0]== 0);
        REQUIRE(fccc[1]== 0);
        REQUIRE(fccc[2]== 1);
        REQUIRE(fccc[3]== 1);
        REQUIRE(fccc[4]== 0);
        REQUIRE(fccc[5]== 0);
        REQUIRE(fccc[6]== 1);
        REQUIRE(fccc[7]== 1);
        REQUIRE(fccc[8]== 2);
        REQUIRE(fccc[9]== 2);
        REQUIRE(fccc[10]== 3);
        REQUIRE(fccc[11]== 3);
        REQUIRE(fccc[12]== 2);
        REQUIRE(fccc[13]== 2);
        REQUIRE(fccc[14]== 3);
        REQUIRE(fccc[15]== 3);
      }
    }

    WHEN("We compute the aspect-ratio of a coarse cell") {
      const CoMMAWeightT eps = 1e-10;
      // In
      unordered_set<CoMMAIndexT> cc = {0,1};
      CoMMAWeightT cc_diam = 1.,
                   cc_vol  = 2.;
      // Out
      CoMMAWeightT tmp_diam{-1.}, tmp_vol{-1.};
      CoMMAIntT shared_faces;
      CoMMAWeightT ar;
      CoMMAWeightT ref_diam = sqrt(2.),
                   ref_vol  = 3.;
      CoMMAWeightT ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("L-shaped coarse cell, 1 shared face") {
        agg->compute_next_cc_features(5, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      ref_diam = 2.;
      ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("I-shaped coarse cell, 1 shared face") {
        agg->compute_next_cc_features(2, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      cc.insert(5);
      cc_diam  = sqrt(2.), cc_vol  = 3.;
      ref_diam = cc_diam,  ref_vol = 4.;
      ref_ar = agg->_compute_AR(ref_diam, ref_vol);
      THEN("Squared coarse cell, 2 shared faces") {
        agg->compute_next_cc_features(4, cc_diam, cc_vol, cc, shared_faces, ar, tmp_diam, tmp_vol);
        REQUIRE(shared_faces == 2);
        REQUIRE(equal_up_to(ref_diam, tmp_diam, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }

    }

  };
}

SCENARIO("Test the anisotropic agglomeration for small cases",
         "[Anisotropic]") {
  GIVEN("We load the anisotropic mesh structure") {
    DualGPy_aniso Data = DualGPy_aniso();
    Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 3,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_graph(fc_graph);
    const CoMMAWeightT aniso_thresh{2.};
    const bool isFirstAgglomeration = true;
    vector<CoMMAIndexT> agglomerationLines_Idx{};
    vector<CoMMAIndexT> agglomerationLines{};
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, isFirstAgglomeration,
                  3);

    WHEN("We proceed with the agglomeration of the anisotropic lines (we gather them and later we agglomerate") {
         aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
      THEN("We have a number of agglomeration lines != 0") {REQUIRE(aniso_agg._nb_lines[0]!=0);}
    }
  };
  GIVEN("We load a 4by6 quad 2D mesh which has 4 anisotropic lines each of length 5 cells") {
    DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    const bool isFirstAgglomeration = true;
    vector<CoMMAIndexT> agglomerationLines_Idx{};
    vector<CoMMAIndexT> agglomerationLines{};
    Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 2,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_graph(fc_graph);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, isFirstAgglomeration,
                  Data.dim);
    aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
    WHEN("We agglomerate the mesh") {
      const auto f2c = cc_graph._fc_2_cc;
      THEN("There is only one isotropic coarse cell") {
        REQUIRE((f2c[5] == f2c[6] && f2c[6] == f2c[17] && f2c[17] == f2c[18]));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2") {
        REQUIRE(f2c[0]  == f2c[1]);
        REQUIRE(f2c[11] == f2c[10]);
        REQUIRE(f2c[16] == f2c[14]);
        REQUIRE(f2c[20] == f2c[22]);
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
#define check3cells(a,b,c) (f2c[a] == f2c[b] && f2c[b] == f2c[c])
        REQUIRE(check3cells(2,3,4));
        REQUIRE(check3cells(9,8,7));
        REQUIRE(check3cells(12,13,15));
        REQUIRE(check3cells(23,21,19));
#undef check3cells
      }
    }

  };
}

SCENARIO("Test the correction in 2D", "[Isotropic Correction]") {
  GIVEN("We load the Minimal Isotropic mesh structure") {
    DualGPy_minimal Data = DualGPy_minimal();
    Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 2,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> cc_graph(fc_graph);
    auto agg = make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        fc_graph, cc_graph, seeds_pool, 2);
    // COMPLETE THE TEST
    WHEN("We proceed with the Isotropic agglomeration") {
      agg->agglomerate_one_level(2, 2, 2, Data.weights, true);

      THEN("No cells are left with cardinality 1") {
        for (auto i = cc_graph._cc_vec.begin(); i != cc_graph._cc_vec.end();
             i++) {
          REQUIRE(i->second->_cardinality != 1);
        }
      }
    }
  };
  GIVEN("A simple 8-cell Cartesian grid") {
#define fc_in_cc(graph, fc, cc) graph._fc_2_cc[fc].value() == cc
    DualGPy_correction Data = DualGPy_correction();
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, 2,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> cc_graph(fc_graph);
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1") {
      cc_graph.cc_create_a_cc({0,4,5});
      cc_graph.cc_create_a_cc({1});
      cc_graph.cc_create_a_cc({2,3,6,7});
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
      cc_graph.correct();
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
#undef fc_in_cc
  }
}
#undef equal_up_to
