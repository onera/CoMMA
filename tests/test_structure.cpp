#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
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

#define FC_ITER 1

#define equal_up_to(a,b,eps) (fabs(a - b) < eps)

using SeedsPoolT = Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using DualGraphT = Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using CCContainerT = Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using CoMMAPairT = pair<CoMMAIndexT, CoMMAWeightT>;
using CoMMAPairFindFirstBasedT = PairFindFirstBasedFunctor<CoMMAPairT>;
using PairValueTestT = int; // Leave this since we might try something than what usually found in CoMMA

SCENARIO("Test of a structure", "[structure]") {
  GIVEN("A simple graph, and we build the Dual Graph") {
    const DualGPy Data = DualGPy();
    // Construction of the Dual Graph element
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    // Check the effective length
    WHEN("We try to access to the member variables") {
      class test : public Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> {
       public:
        test(shared_ptr<DualGraphT> &graph,
             shared_ptr<CCContainerT> &cc_graph,
             shared_ptr<SeedsPoolT> &seeds_pool, CoMMAIntT dimension)
            : Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(graph, cc_graph,
                                                                seeds_pool, CoMMAAgglT::BICONNECTED, FC_ITER, dimension) {};

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
        test(shared_ptr<DualGraphT> &graph,
             shared_ptr<CCContainerT> &cc_graph,
             shared_ptr<SeedsPoolT> &seeds_pool, CoMMAIntT dimension)
            : Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(graph, cc_graph,
                                                                seeds_pool, CoMMAAgglT::BICONNECTED, FC_ITER, dimension) {};

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
    const DualGPy_cube_4 Data = DualGPy_cube_4();
    Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, Data.weights, false);
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
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : ridges) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : valleys) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : interior) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
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
        auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(63 == opt_seed.value());
        agglomerated[63] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(44 == opt_seed.value());
        agglomerated[44] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(30 == opt_seed.value());
        agglomerated[30] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(21 == opt_seed.value());
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
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
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
        auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(63 == opt_seed.value());
        agglomerated[63] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(44 == opt_seed.value());
        agglomerated[44] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(30 == opt_seed.value());
        agglomerated[30] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(21 == opt_seed.value());
        agglomerated[21] = true;
      }
      new_seeds = {10, 5, 6, 9};
      fill(agglomerated.begin(), agglomerated.end(), true);
      for (auto &i : new_seeds) {
        agglomerated[i] = false;
      }
      seeds_pool.order_new_seeds_and_update(new_seeds);
      THEN("The order respects the priority all in the same queue") {
        auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(5 == opt_seed.value());
        agglomerated[5] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(6 == opt_seed.value());
        agglomerated[6] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(9 == opt_seed.value());
        agglomerated[9] = true;
        opt_seed = seeds_pool.choose_new_seed(agglomerated);
        REQUIRE(opt_seed.has_value());
        REQUIRE(10 == opt_seed.value());
        agglomerated[10] = true;
      }
    }
  }

  GIVEN("A 4x4x4 cube and a Seed Pool which should force an order reversed wrt the cell numbering") {
    const DualGPy_cube_4 Data = DualGPy_cube_4();
    vector<CoMMAWeightT> w(Data.nb_fc);
    iota(w.begin(), w.end(), 0);
    Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> seeds_pool(Data.n_bnd_faces, w, false);
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
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : ridges) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : valleys) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
        for (auto i : interior) {
          const auto opt_seed = seeds_pool.choose_new_seed(agglomerated);
          REQUIRE(opt_seed.has_value());
          REQUIRE(i == opt_seed.value());
          agglomerated[i] = true;
        }
      }
    }
  }
  GIVEN("A mesh with a hole and two corners") {
    const DualGPy_hole_w_corners Data = DualGPy_hole_w_corners();
    const Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    WHEN("We use a seeds pool with boundary priority") {
      Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, false);
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 7, 5, 6};   // ...finally, interior cells in an order given by neighbourhood
      assert(expected_order.size() == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        seeds_pool.update(neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
    WHEN("We use a seeds pool with boundary priority and with only one start point") {
      Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, true);
#if 0
The one-point initialization would not any impact w.r.t. the standard one on this mesh
#endif
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 7, 5, 6};   // ...finally, interior cells in an order given by neighbourhood
      assert(expected_order.size() == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        seeds_pool.update(neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
    WHEN("We use a seeds pool with neighbourhood priority") {
      Seeds_Pool_Neighbourhood_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, false);
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        4, 7, 5, 6,    // ...then, interior cells in an order given by neighbourhood
        8, 11, 9, 10}; // ...finally, inner boundary cells in an order given by neighbourhood
      assert(expected_order.size() == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        seeds_pool.update(neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
    WHEN("We use a seeds pool with neighbourhood priority and with only one start point") {
      Seeds_Pool_Neighbourhood_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, true);
      const vector<CoMMAIndexT> expected_order = {
        12,             // Starting point
        0, 3,           // Direct neighbours
        13,             // Another corner
        2, 1,           // Outer boundary cells
        4, 7, 6, 5,     // Interior cells
        8, 11, 10, 9};  // Inner boundary cells
      assert(expected_order.size() == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        seeds_pool.update(neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
  }
  GIVEN("A mesh with a hole and no corners") {
    const DualGPy_hole_no_corners Data = DualGPy_hole_no_corners();
    const Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    WHEN("We use a seeds pool with boundary priority") {
      Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, false);
      const vector<CoMMAIndexT> expected_order = {
        0, 1, 2, 3, 8, 9, 10, 11, // First, outer boundary cells in an order given by the weights
        4, 5, 6, 7};              // ...finally, interior cells in an order given by neighbourhood
      assert(expected_order.size() == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        seeds_pool.update(neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
    WHEN("We use a seeds pool with boundary priority") {
      Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        seeds_pool(Data.n_bnd_faces, Data.weights, true);
      const vector<CoMMAIndexT> expected_order = {
        0,             // Starting point,
        1, 3, 2,       // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 5, 7, 6};   // ...finally, interior cells in an order given by neighbourhood
      assert(expected_order.size() == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        seeds_pool.update(neighs);
      }
      THEN("The expected order is found") {
        for (auto i = decltype(res_seeds.size()){0}; i < res_seeds.size(); ++i) {
          REQUIRE(expected_order[i] == res_seeds[i]);
        }
      }
    }
  }
}

SCENARIO("Test dual graph and neighbourhood computing", "[Dual graph & Neighbourhood]") {
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
  };
  GIVEN("We have a 7x7 Cartesian 2D matrix and set up a standard Neighbourhood for 24") {
 #define check_(fun, op, cont, obj) fun(cont.begin(), cont.end(), obj) op cont.end()
 #define found_(cont, obj) check_(find, !=, cont, obj)
 #define not_found_(cont, obj) check_(find, ==, cont, obj)
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
  };
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
    const DualGPy_cube_4 Data = DualGPy_cube_4();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_graph, seeds_pool, CoMMAAgglT::BICONNECTED, FC_ITER, Data.dim);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(8, 8, 8, Data.weights, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        const auto &fccc = cc_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value()==fc2cc_req[i]);
        }

      }
    }
    WHEN("We agglomerate the mesh with a biconnected agglomerator and we try to correct") {
      agg->agglomerate_one_level(8, 8, 8, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value()==fc2cc_req[i]);
        }
      }
    }

    shared_ptr<CCContainerT> cc_PF_graph = make_shared<CCContainerT>(fc_graph);
    auto agg_PF =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_PF_graph, seeds_pool, CoMMAAgglT::PURE_FRONT, FC_ITER, Data.dim);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(8, 8, 8, Data.weights, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        const auto &fccc = cc_PF_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value()==fc2cc_req[i]);
        }
      }
    }
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct") {
      agg_PF->agglomerate_one_level(8, 8, 8, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_PF_graph->_fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6, 7, 7, 6, 6, 7, 7};
        for (auto i = decltype(Data.nb_fc){0}; i < Data.nb_fc; i++) {
          REQUIRE(fccc[i].value()==fc2cc_req[i]);
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
    const DualGPy_quad_4 Data = DualGPy_quad_4();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_graph, seeds_pool, CoMMAAgglT::BICONNECTED, FC_ITER, Data.dim);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(4, 4, 4, Data.weights, false);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        const auto &fccc = cc_graph->_fc_2_cc;
        REQUIRE(fccc[0].value()== 0);
        REQUIRE(fccc[1].value()== 0);
        REQUIRE(fccc[2].value()== 1);
        REQUIRE(fccc[3].value()== 1);
        REQUIRE(fccc[4].value()== 0);
        REQUIRE(fccc[5].value()== 0);
        REQUIRE(fccc[6].value()== 1);
        REQUIRE(fccc[7].value()== 1);
        REQUIRE(fccc[8].value()== 2);
        REQUIRE(fccc[9].value()== 2);
        REQUIRE(fccc[10].value()== 3);
        REQUIRE(fccc[11].value()== 3);
        REQUIRE(fccc[12].value()== 2);
        REQUIRE(fccc[13].value()== 2);
        REQUIRE(fccc[14].value()== 3);
        REQUIRE(fccc[15].value()== 3);
      }
    }
    WHEN("We agglomerate the mesh with a biconnected agglomerator and we try to correct") {
      agg->agglomerate_one_level(4, 4, 4, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto &fccc = cc_graph->_fc_2_cc;
        REQUIRE(fccc[0].value()== 0);
        REQUIRE(fccc[1].value()== 0);
        REQUIRE(fccc[2].value()== 1);
        REQUIRE(fccc[3].value()== 1);
        REQUIRE(fccc[4].value()== 0);
        REQUIRE(fccc[5].value()== 0);
        REQUIRE(fccc[6].value()== 1);
        REQUIRE(fccc[7].value()== 1);
        REQUIRE(fccc[8].value()== 2);
        REQUIRE(fccc[9].value()== 2);
        REQUIRE(fccc[10].value()== 3);
        REQUIRE(fccc[11].value()== 3);
        REQUIRE(fccc[12].value()== 2);
        REQUIRE(fccc[13].value()== 2);
        REQUIRE(fccc[14].value()== 3);
        REQUIRE(fccc[15].value()== 3);
      }
    }

    shared_ptr<CCContainerT> cc_PF_graph = make_shared<CCContainerT>(fc_graph);
    auto agg_PF =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_PF_graph, seeds_pool, CoMMAAgglT::PURE_FRONT, FC_ITER, Data.dim);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(4, 4, 4, Data.weights, false);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        // Nothing changes with respect to the case of the standard Biconnected
        // We have a trick that make it work as it should
        const auto &fccc = cc_PF_graph->_fc_2_cc;
        REQUIRE(fccc[0].value()== 0);
        REQUIRE(fccc[1].value()== 0);
        REQUIRE(fccc[2].value()== 1);
        REQUIRE(fccc[3].value()== 1);
        REQUIRE(fccc[4].value()== 0);
        REQUIRE(fccc[5].value()== 0);
        REQUIRE(fccc[6].value()== 1);
        REQUIRE(fccc[7].value()== 1);
        REQUIRE(fccc[8].value()== 2);
        REQUIRE(fccc[9].value()== 2);
        REQUIRE(fccc[10].value()== 3);
        REQUIRE(fccc[11].value()== 3);
        REQUIRE(fccc[12].value()== 2);
        REQUIRE(fccc[13].value()== 2);
        REQUIRE(fccc[14].value()== 3);
        REQUIRE(fccc[15].value()== 3);
      }
    }
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct") {
      agg_PF->agglomerate_one_level(4, 4, 4, Data.weights, true);
      THEN("Nothing changes with respect to the case without correction") {
        const auto fccc = cc_PF_graph->_fc_2_cc;
        REQUIRE(fccc[0].value()== 0);
        REQUIRE(fccc[1].value()== 0);
        REQUIRE(fccc[2].value()== 1);
        REQUIRE(fccc[3].value()== 1);
        REQUIRE(fccc[4].value()== 0);
        REQUIRE(fccc[5].value()== 0);
        REQUIRE(fccc[6].value()== 1);
        REQUIRE(fccc[7].value()== 1);
        REQUIRE(fccc[8].value()== 2);
        REQUIRE(fccc[9].value()== 2);
        REQUIRE(fccc[10].value()== 3);
        REQUIRE(fccc[11].value()== 3);
        REQUIRE(fccc[12].value()== 2);
        REQUIRE(fccc[13].value()== 2);
        REQUIRE(fccc[14].value()== 3);
        REQUIRE(fccc[15].value()== 3);
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
    const DualGPy_aniso Data = DualGPy_aniso();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    const CoMMAWeightT aniso_thresh{2.};
    const bool isFirstAgglomeration = true;
    vector<CoMMAIndexT> agglomerationLines_Idx{};
    vector<CoMMAIndexT> agglomerationLines{};
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, isFirstAgglomeration, Data.dim);

    WHEN("We proceed with the agglomeration of the anisotropic lines (we gather them and later we agglomerate") {
         aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
      THEN("We have a number of agglomeration lines != 0") {REQUIRE(aniso_agg._nb_lines[0]!=0);}
    }
  };
  GIVEN("We load a 4by6 quad 2D mesh which has 4 anisotropic lines each of length 5 cells") {
    const DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    const bool isFirstAgglomeration = true;
    vector<CoMMAIndexT> agglomerationLines_Idx{};
    vector<CoMMAIndexT> agglomerationLines{};
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, isFirstAgglomeration,
                  Data.dim);
    aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        iso_agg(fc_graph, cc_graph, seeds_pool, CoMMAAgglT::BICONNECTED, FC_ITER, Data.dim);
    iso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
    WHEN("We agglomerate the mesh") {
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("There is only one isotropic coarse cell") {
        REQUIRE((f2c[5].value() == f2c[6].value() && f2c[6].value() == f2c[17].value()
                 && f2c[17].value() == f2c[18].value()));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2") {
        REQUIRE(f2c[0].value()  == f2c[1].value());
        REQUIRE(f2c[11].value() == f2c[10].value());
        REQUIRE(f2c[16].value() == f2c[14].value());
        REQUIRE(f2c[20].value() == f2c[22].value());
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
#define check3cells(a,b,c) (f2c[a].value() == f2c[b].value() && f2c[b].value() == f2c[c].value())
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
    const DualGPy_minimal Data = DualGPy_minimal();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    auto agg = make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        fc_graph, cc_graph, seeds_pool, CoMMAAgglT::BICONNECTED, FC_ITER, Data.dim);
    // COMPLETE THE TEST
    WHEN("We proceed with the Isotropic agglomeration") {
      agg->agglomerate_one_level(2, 2, 2, Data.weights, true);

      THEN("No cells are left with cardinality 1") {
        for (auto i = cc_graph->_cc_vec.begin(); i != cc_graph->_cc_vec.end();
             i++) {
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
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1") {
      cc_graph->cc_create_a_cc({0,4,5});
      cc_graph->cc_create_a_cc({1});
      cc_graph->cc_create_a_cc({2,3,6,7});
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
  }
  GIVEN("A simple 3x3 Cartesian grid") {
    const DualGPy_quad_3 Data = DualGPy_quad_3();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    WHEN("We agglomerate (manually) leaving one coarse cell with cardinality 1 "
         "and two coarse cells with cardinality 5 and 3") {
      cc_graph->cc_create_a_cc({0,1,2,5,8});
      cc_graph->cc_create_a_cc({4});
      cc_graph->cc_create_a_cc({3,6,7});
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
      cc_graph->cc_create_a_cc({0,3});
      cc_graph->cc_create_a_cc({6,7});
      cc_graph->cc_create_a_cc({4});
      cc_graph->cc_create_a_cc({1,2,5,8});
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
      cc_graph->cc_create_a_cc({0,1,3,4});
      cc_graph->cc_create_a_cc({7,8});
      cc_graph->cc_create_a_cc({6});
      cc_graph->cc_create_a_cc({2,5});
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
      cc_graph->cc_create_a_cc({0,3});
      cc_graph->cc_create_a_cc({6,7});
      cc_graph->cc_create_a_cc({4});
      cc_graph->cc_create_a_cc({5,8});
      cc_graph->cc_create_a_cc({1,2});
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
  }
#undef fc_in_cc
}
#undef equal_up_to
