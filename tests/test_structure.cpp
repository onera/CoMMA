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
#include "CoMMA.h"
#include "CoMMATypes.h"

#include "deprecated/Bimap.h"
#include "deprecated/Priority_Pair.h"
#include "deprecated/Queue.h"
#include "deprecated/Tree.h"

using namespace std;

constexpr CoMMAIntT FC_ITER  = 1;

constexpr bool ODD_LINE_LENGTH = true;

#define check_(fun, op, cont, obj) fun(cont.begin(), cont.end(), obj) op cont.end()
#define found_(cont, obj) check_(find, !=, cont, obj)
#define not_found_(cont, obj) check_(find, ==, cont, obj)
#define equal_up_to(a,b,eps) (fabs(a - b) < eps)

using SeedsPoolT = Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using DualGraphT = Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using CCContainerT = Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using CoMMAPairT = pair<CoMMAIndexT, CoMMAWeightT>;
using CoMMAPairFindFirstBasedT = PairFindFirstBasedFunctor<CoMMAPairT>;
using PairValueTestT = int; // Leave this since we might try something than what usually found in CoMMA
                            //
using NodeType = Node<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using TreeType = Tree<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;

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
                                                                seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, dimension) {};

        CoMMAIntT test_variable() {
          return (this->_threshold_card);
        };
      };
      THEN("We see that the agglomeration is not set, hence set to 0") {
        test agg = test(fc_graph, cc_graph, seeds_pool, 2);
        CoMMAIntT testing = agg.test_variable();
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
                                                                seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, dimension) {};

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

#if 0
SCENARIO("Test of the tree", "[Tree]") {
  GIVEN("The node") {
    shared_ptr<NodeType> node = make_shared<NodeType>(2,1);
    shared_ptr<TreeType> tree = make_shared<TreeType>(node);
    WHEN("We add something to the tree") {
      tree->insertSon(2, 1, 1, 1);
      THEN("The tree has a child") {
        tree->insertSon(2, 3, 1, 1);
        tree->insertSon(2, 4, 1, 1);
        tree->insertSon(4, 2, 1, 0);
        REQUIRE(tree->_root->_sonc == 3);
        tree->deleteNode(4);
        REQUIRE(tree->_root->_sonc == 2);
      }
    };
  }
}
#endif

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

SCENARIO("Test of the seeds pool", "[Seeds_Pool]") {
  GIVEN("A 4x4x4 cube and a Seeds Pool which should ensure that the order respects the cell numbering") {
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
    WHEN("We create the seeds pool") {
      THEN("It is not initialized") {
        REQUIRE(seeds_pool.is_empty(0));
        REQUIRE(seeds_pool.is_empty(1));
        REQUIRE(seeds_pool.is_empty(2));
        REQUIRE(seeds_pool.is_empty(3));
        REQUIRE(seeds_pool.is_empty());
        REQUIRE(seeds_pool.need_initialization(agglomerated));
      }
    }
    seeds_pool.initialize();
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
    WHEN("We simply update the seeds pool") {
      // Spoil all the corners so that the seed is completely void
      for (auto i : corners) {
        seeds_pool.choose_new_seed(agglomerated);
        agglomerated[i] = true;
      }
      // In order: Internal, ridge, corner, valley
      deque<CoMMAIndexT> new_seeds = {21, 44, 63, 30};
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
    WHEN("We update the seeds pool by asking a reorder") {
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

  GIVEN("A 4x4x4 cube and a Seeds Pool which should force an order reversed wrt the cell numbering") {
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
    WHEN("We create the seeds pool") {
      THEN("It is not initialized") {
        REQUIRE(seeds_pool.need_initialization(agglomerated));
      }
    }
    seeds_pool.initialize();
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
      seeds_pool.initialize();
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 7, 5, 6};   // ...finally, interior cells in an order given by neighbourhood
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
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
      seeds_pool.initialize();
#if 0
The one-point initialization would not any impact w.r.t. the standard one on this mesh
#endif
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 7, 5, 6};   // ...finally, interior cells in an order given by neighbourhood
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
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
      seeds_pool.initialize();
      const vector<CoMMAIndexT> expected_order = {
        12, 13,        // First, corners
        0, 3, 1, 2,    // ...then, outer boundary cells in an order given by neighbourhood
        4, 7, 5, 6,    // ...then, interior cells in an order given by neighbourhood
        8, 11, 9, 10}; // ...finally, inner boundary cells in an order given by neighbourhood
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
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
      seeds_pool.initialize();
      const vector<CoMMAIndexT> expected_order = {
        12,             // Starting point
        0, 3,           // Direct neighbours
        13,             // Another corner
        2, 1,           // Outer boundary cells
        4, 7, 6, 5,     // Interior cells
        8, 11, 10, 9};  // Inner boundary cells
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
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
      seeds_pool.initialize();
      const vector<CoMMAIndexT> expected_order = {
        0, 1, 2, 3, 8, 9, 10, 11, // First, outer boundary cells in an order given by the weights
        4, 5, 6, 7};              // ...finally, interior cells in an order given by neighbourhood
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
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
      seeds_pool.initialize();
      const vector<CoMMAIndexT> expected_order = {
        0,             // Starting point,
        1, 3, 2,       // ...then, outer boundary cells in an order given by neighbourhood
        8, 9, 10, 11,  // ...then, inner boundary cells in an order given by the weights (new queue)
        4, 5, 7, 6};   // ...finally, interior cells in an order given by neighbourhood
      assert(static_cast<CoMMAIndexT>(expected_order.size()) == Data.nb_fc);
      vector<bool> agglomerated(Data.nb_fc, false);
      vector<CoMMAIndexT> res_seeds(Data.nb_fc);
      for (auto & s : res_seeds) {
        const auto opt_s = seeds_pool.choose_new_seed(agglomerated);
        assert(opt_s.has_value());
        s = opt_s.value();
        agglomerated[s] = true;
        auto neighs = fc_graph.get_neighbours(s);
        sort(neighs.begin(), neighs.end()); // Simulates order by weights
        deque<CoMMAIndexT> d_neighs(neighs.begin(), neighs.end());
        seeds_pool.update(d_neighs);
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
  };
}

SCENARIO("Test neighbourhood-based wall-distance", "[Wall-distance]") {
  GIVEN("A 7x7 Cartesian 2D matrix") {
    const DualGPy_quad_7 Data = DualGPy_quad_7();
    const vector<CoMMAIndexT> wall = {0,1,2,3,4,5,6,7,14,21,28,35,42};
    WHEN("We compute the neighbourhood-based wall-distance") {
      vector<CoMMAIntT> dist{};
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAIntT>(
              Data.adjMatrix_row_ptr,
              Data.adjMatrix_col_ind,
              wall,
              dist);
      THEN("Wall") {
        for (const auto & cell : wall) {
          REQUIRE(dist[cell] == 0);
        }
      }
      THEN("First set of neighbours") {
        const vector<CoMMAIndexT> cells = {8,9,10,11,12,13,15,22,29,36,43};
        for (const auto & cell : cells) {
          REQUIRE(dist[cell] == 1);
        }
      }
      THEN("Second set of neighbours") {
        const vector<CoMMAIndexT> cells = {16,17,18,19,20,23,30,37,44};
        for (const auto & cell : cells) {
          REQUIRE(dist[cell] == 2);
        }
      }
      THEN("Third set of neighbours") {
        const vector<CoMMAIndexT> cells = {24,25,26,27,31,38,45};
        for (const auto & cell : cells) {
          REQUIRE(dist[cell] == 3);
        }
      }
      THEN("Fourth set of neighbours") {
        const vector<CoMMAIndexT> cells = {32,33,34,39,46};
        for (const auto & cell : cells) {
          REQUIRE(dist[cell] == 4);
        }
      }
      THEN("Fifth set of neighbours") {
        const vector<CoMMAIndexT> cells = {40,41,47};
        for (const auto & cell : cells) {
          REQUIRE(dist[cell] == 5);
        }
      }
      THEN("Sixth set of neighbours") {
        REQUIRE(dist[48] == 6);
      }
    }

    WHEN("We compute the neighbourhood-based wall-distance (double type)") {
      vector<CoMMAWeightT> dist{};
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAWeightT>(
              Data.adjMatrix_row_ptr,
              Data.adjMatrix_col_ind,
              wall,
              dist);
      THEN("Wall") {
        for (const auto & cell : wall) {
          REQUIRE(equal_up_to(dist[cell], 0, 1e-10));
        }
      }
      THEN("First set of neighbours") {
        const vector<CoMMAIndexT> cells = {8,9,10,11,12,13,15,22,29,36,43};
        for (const auto & cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 1, 1e-10));
        }
      }
      THEN("Second set of neighbours") {
        const vector<CoMMAIndexT> cells = {16,17,18,19,20,23,30,37,44};
        for (const auto & cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 2, 1e-10));
        }
      }
      THEN("Third set of neighbours") {
        const vector<CoMMAIndexT> cells = {24,25,26,27,31,38,45};
        for (const auto & cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 3, 1e-10));
        }
      }
      THEN("Fourth set of neighbours") {
        const vector<CoMMAIndexT> cells = {32,33,34,39,46};
        for (const auto & cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 4, 1e-10));
        }
      }
      THEN("Fifth set of neighbours") {
        const vector<CoMMAIndexT> cells = {40,41,47};
        for (const auto & cell : cells) {
          REQUIRE(equal_up_to(dist[cell], 5, 1e-10));
        }
      }
      THEN("Sixth set of neighbours") {
        REQUIRE(equal_up_to(dist[48], 6, 1e-10));
      }
    }
    WHEN("We compute the neighbourhood-based wall-distance wrt to an empty wall") {
      const vector<CoMMAIndexT> empty_wall{};
      vector<CoMMAWeightT> dist{};
      compute_neighbourhood_based_wall_distance<CoMMAIndexT, CoMMAWeightT>(
              Data.adjMatrix_row_ptr,
              Data.adjMatrix_col_ind,
              empty_wall,
              dist);
      THEN("All the distances are negative") {
        for (const auto & d : dist) {
          REQUIRE(d < 0);
        }
      }
    }
  }
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

SCENARIO("Test compactness computation", "[Compactness]") {
  GIVEN("A simple 3x3 Cartesian grid") {
    const DualGPy_quad_3 Data = DualGPy_quad_3();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    WHEN("We consider an empty coarse cell") {
      const unordered_set<CoMMAIndexT> cc = {};
      THEN("The single compactness value are right") {
        REQUIRE(fc_graph->compute_fc_compactness_inside_a_cc(cc).empty());
      }
      THEN("The minimum is well identified") {
        REQUIRE(fc_graph->compute_min_fc_compactness_inside_a_cc(cc) == 0);
      }
    }
    WHEN("We consider a singular coarse cell") {
      const unordered_set<CoMMAIndexT> cc = {1};
      THEN("The single compactness value are right") {
        const auto cpt = fc_graph->compute_fc_compactness_inside_a_cc(cc);
        REQUIRE(cpt.size() == 1);
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
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
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
          fc_graph, cc_PF_graph, seeds_pool, CoMMANeighbourhoodT::PURE_FRONT, FC_ITER, Data.dim);
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
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
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
          fc_graph, cc_PF_graph, seeds_pool, CoMMANeighbourhoodT::PURE_FRONT, FC_ITER, Data.dim);
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
  GIVEN("A 3x2 mesh of slightly stretched (x1.75) rectangles") {
    const DualGPy_T_shaped Data = DualGPy_T_shaped();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
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
    WHEN("We agglomerate from the bottom central cell with a standard agglomerator, we get a T-shaped coarse cell") {
      const CoMMAIntT fc_iter = 1;
      auto agg = Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
            fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, fc_iter, Data.dim);
      agg.set_agglomeration_parameter(card, card, card);
      auto cc = agg.choose_optimal_cc_and_update_seeds_pool(seed, Data.weights, comp);
      THEN("Compactness is well computed") {
        REQUIRE(comp == 1);
      }
      THEN("The cell is T-shaped") {
        REQUIRE(found_(cc, seed));
        REQUIRE(found_(cc, 0));
        REQUIRE(found_(cc, 2));
        REQUIRE(found_(cc, 4));
      }
    }
    WHEN("We agglomerate from the bottom central cell with a 2-steps iterative agglomerator, we get a square coarse cell") {
      const CoMMAIntT fc_iter = 2;
      auto agg = Agglomerator_Iterative<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
            fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, fc_iter, Data.dim);
      agg.set_agglomeration_parameter(card, card, card);
      auto cc = agg.choose_optimal_cc_and_update_seeds_pool(seed, Data.weights, comp);
      THEN("Compactness is well computed") {
        REQUIRE(comp == 2);
      }
      THEN("The cell is a square") {
        REQUIRE(found_(cc, seed));
        REQUIRE(found_(cc, 0));
        REQUIRE(found_(cc, 3));
        REQUIRE(found_(cc, 4));
      }
    }
  }
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
    const bool build_lines = true;
    vector<CoMMAIndexT> agglomerationLines_Idx{};
    vector<CoMMAIndexT> agglomerationLines{};
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, Data.weights,
                  build_lines, ODD_LINE_LENGTH, Data.dim);

    WHEN("We proceed with the agglomeration of the anisotropic lines (we gather them and later we agglomerate)") {
         aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
      THEN("We have a number of agglomeration lines != 0") {
        REQUIRE(aniso_agg._nb_lines[0]!=0);
      }
    }
  }
  GIVEN("We load the anisotropic mesh structure, but there is no anisotropic cell") {
    /* ATTENTION: This test produces (prints) a warning, twice */
    const DualGPy_aniso Data = DualGPy_aniso();
    WHEN("We consider no anisotropic compliant cell") {
      shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
      vector<CoMMAIndexT> v_aniso = {};
      shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
          Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
          Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
          v_aniso);
      shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
      const CoMMAWeightT aniso_thresh{-2.};
      const bool build_lines = true;
      vector<CoMMAIndexT> agglomerationLines_Idx{};
      vector<CoMMAIndexT> agglomerationLines{};
      Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
          aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                    agglomerationLines_Idx, agglomerationLines, Data.weights,
                    build_lines, ODD_LINE_LENGTH, Data.dim);

      THEN("There is no need to agglomerate anisotropically since no line can be built") {
        REQUIRE(!aniso_agg._should_agglomerate);
        REQUIRE(aniso_agg._nb_lines[0]==0);
        THEN("Even if one tries to agglomerate anisotropically, nothing happens") {
          aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
          REQUIRE(cc_graph->get_number_of_fc_agglomerated() == 0);
        }
      }
    }
    WHEN("We consider a too high threshold") {
      shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
      shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
          Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
          Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
          Data.arrayOfFineAnisotropicCompliantCells);
      shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
      const CoMMAWeightT aniso_thresh{10000.};
      const bool build_lines = true;
      vector<CoMMAIndexT> agglomerationLines_Idx{};
      vector<CoMMAIndexT> agglomerationLines{};
      Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
          aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                    agglomerationLines_Idx, agglomerationLines, Data.weights,
                    build_lines, ODD_LINE_LENGTH, Data.dim);

      THEN("There is no need to agglomerate anisotropically since no line can be built") {
        REQUIRE(!aniso_agg._should_agglomerate);
        REQUIRE(aniso_agg._nb_lines[0]==0);
        THEN("Even if one tries to agglomerate anisotropically, nothing happens") {
          aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
          REQUIRE(cc_graph->get_number_of_fc_agglomerated() == 0);
        }
      }
    }
    WHEN("We consider only anisotropic lines composed of only one cell") {
      shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
      shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
          Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
          Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
          Data.arrayOfFineAnisotropicCompliantCells);
      shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
      const CoMMAWeightT aniso_thresh{10000.};
      const bool build_lines = false;
      vector<CoMMAIndexT> agglomerationLines_Idx{0,1,2};
      vector<CoMMAIndexT> agglomerationLines{1,2};
      Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
          aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                    agglomerationLines_Idx, agglomerationLines, Data.weights,
                    build_lines, ODD_LINE_LENGTH, Data.dim);

      THEN("There is no need to agglomerate anisotropically since no line can be built") {
        REQUIRE(!aniso_agg._should_agglomerate);
        REQUIRE(aniso_agg._nb_lines[0]==0);
        THEN("Even if one tries to agglomerate anisotropically, nothing happens") {
          aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
          REQUIRE(cc_graph->get_number_of_fc_agglomerated() == 0);
        }
        THEN("Even if one tries to get the output lines, they come back empty") {
          aniso_agg.export_anisotropic_lines(1, agglomerationLines_Idx, agglomerationLines);
          REQUIRE(agglomerationLines_Idx.empty());
          REQUIRE(agglomerationLines.empty());
        }
      }
    }
  }
  GIVEN("We load the anisotropic mesh structure, but only a cell is anisotropic") {
    /* ATTENTION: This test produces (prints) a warning */
    const DualGPy_aniso Data = DualGPy_aniso();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    vector<CoMMAIndexT> v_aniso = {0};
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        v_aniso);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    const CoMMAWeightT aniso_thresh{-2.};
    const bool build_lines = true;
    vector<CoMMAIndexT> agglomerationLines_Idx{};
    vector<CoMMAIndexT> agglomerationLines{};
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, Data.weights,
                  build_lines, ODD_LINE_LENGTH, Data.dim);

    THEN("There is no need to agglomerate anisotropically since no line can be built") {
      REQUIRE(!aniso_agg._should_agglomerate);
      REQUIRE(aniso_agg._nb_lines[0]==0);
      THEN("Even if one tries to agglomerate anisotropically, nothing happens") {
        aniso_agg.agglomerate_one_level(2, 2, 2, Data.weights, false);
        REQUIRE(cc_graph->get_number_of_fc_agglomerated() == 0);
      }
    }
  }
  GIVEN("We load a 4by7 quad 2D mesh, we provide 4 lines but one is just 1-cell long") {
    /* ATTENTION: This test produces (prints) a warning */
    const DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    vector<CoMMAIndexT> v_aniso = {0};
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        v_aniso);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    const CoMMAWeightT aniso_thresh{-2.};
    const bool build_lines = false;
    vector<CoMMAIndexT> agglomerationLines_Idx{0,1,6,11,16};
    vector<CoMMAIndexT> agglomerationLines{0,
                                           11,10, 9, 8, 7,
                                           16,14,12,13,15,
                                           20,22,23,21,19};
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, Data.weights,
                  build_lines, ODD_LINE_LENGTH, Data.dim);

    THEN("Only 3 lines are built") {
      REQUIRE(aniso_agg._nb_lines[0]==3);
      REQUIRE(aniso_agg._should_agglomerate);
    }
  }

#define check2cells(a,b)     (f2c[a].value() == f2c[b].value())
#define check3cells(a,b,c)   (f2c[a].value() == f2c[b].value() && f2c[b].value() == f2c[c].value())
#define check4cells(a,b,c,d) (f2c[a].value() == f2c[b].value() && f2c[b].value() == f2c[c].value() \
                              && f2c[c].value() == f2c[d].value())
  GIVEN("We load a 4by7 quad 2D mesh which has 4 anisotropic lines each of length 5 cells and"
        " a seeds pool with boundary priority with full initialization") {
    const DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    const bool build_lines = true;
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
                  agglomerationLines_Idx, agglomerationLines, Data.weights,
                  build_lines, ODD_LINE_LENGTH, Data.dim);
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        iso_agg(fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      aniso_agg.update_seeds_pool();
      THEN("After the anisotropic agglomeration, the seeds pool is not empty and does not need an initialization") {
        REQUIRE(!seeds_pool->is_empty());
        REQUIRE(!seeds_pool->need_initialization(cc_graph->_is_fc_agglomerated));
      }
      THEN("Having chosen a priority by boundary, the seed is a corner who is not a neighbour") {
        REQUIRE(seeds_pool->choose_new_seed(cc_graph->_is_fc_agglomerated).value() == 24);
      }
      iso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("There are two isotropic coarse cells") {
        REQUIRE(check4cells(5,6,24,25));
        REQUIRE(check4cells(17,18,27,26));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2") {
        REQUIRE(check2cells(0,1));
        REQUIRE(check2cells(11,10));
        REQUIRE(check2cells(16,14));
        REQUIRE(check2cells(20,22));
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
        REQUIRE(check3cells(2,3,4));
        REQUIRE(check3cells(9,8,7));
        REQUIRE(check3cells(12,13,15));
        REQUIRE(check3cells(23,21,19));
      }
      THEN("The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
           " and the priority weights") {
        REQUIRE(f2c[ 0].value() == 0);
        REQUIRE(f2c[ 2].value() == 1);
        REQUIRE(f2c[ 9].value() == 3);
        REQUIRE(f2c[11].value() == 2);
        REQUIRE(f2c[13].value() == 5);
        REQUIRE(f2c[16].value() == 4);
        REQUIRE(f2c[20].value() == 6);
        REQUIRE(f2c[23].value() == 7);
      }
    }
  }

  GIVEN("We load a 4by7 quad 2D mesh which has 4 anisotropic lines each of length 5 cells and"
        " a seeds pool with boundary priority with full initialization but we forbid odd lines") {
    const DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    const bool build_lines = true;
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
                  agglomerationLines_Idx, agglomerationLines, Data.weights,
                  build_lines, false, Data.dim);
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        iso_agg(fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      aniso_agg.update_seeds_pool();
      THEN("After the anisotropic agglomeration, the seeds pool is not empty and does not need an initialization") {
        REQUIRE(!seeds_pool->is_empty());
        REQUIRE(!seeds_pool->need_initialization(cc_graph->_is_fc_agglomerated));
      }
      THEN("Having chosen a priority by boundary, the seed is a corner who is not a neighbour") {
        REQUIRE(seeds_pool->choose_new_seed(cc_graph->_is_fc_agglomerated).value() == 24);
      }
      iso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("There are three isotropic coarse cells") {
        REQUIRE(check4cells(5,6,24,25));
        REQUIRE(check4cells(17,18,27,26));
        REQUIRE(check4cells(4,7,15,19));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2") {
        REQUIRE(check2cells(0,1));
        REQUIRE(check2cells(2,3));
        REQUIRE(check2cells(9,8));
        REQUIRE(check2cells(11,10));
        REQUIRE(check2cells(12,13));
        REQUIRE(check2cells(16,14));
        REQUIRE(check2cells(20,22));
        REQUIRE(check2cells(23,21));
      }
      THEN("The coarse-cell numbering the anisotropy and the priority weights") {
        REQUIRE(f2c[ 0].value() == 0);
        REQUIRE(f2c[ 2].value() == 1);
        REQUIRE(f2c[ 4].value() == 10);
        REQUIRE(f2c[ 9].value() == 3);
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

  GIVEN("We load a 4by7 quad 2D mesh [stretched wrt other similar cases] and consider all cells as anisotropic") {
#if 0
We stretch a little the top two layer of cells so that they are a rectangle (instead of a square) with the height
greater than the width. This implies that the max connection would be horizontal, whereas the anisotropic lines grow
vertically: in this case, we test that the construction take into account the direction.
We also have to modify the weights so that the seeds of the lines are at the bottom of the mesh which will ensure that
the line grows vertically
#endif
    DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    const vector<CoMMAIndexT> bottom_layer = {0,11,16,20};
    const vector<CoMMAIndexT> first_stretched_layer = {5,6,17,18};
    const vector<CoMMAIndexT> second_stretched_layer = {24,25,26,27};
    for (const auto &idx : first_stretched_layer) {
      // Stretching center
      Data.centers[idx][1] += 0.5;
      for (CoMMAIndexT i = Data.adjMatrix_row_ptr[idx];
           i < Data.adjMatrix_row_ptr[idx+1]; ++i) {
        const auto neigh = Data.adjMatrix_col_ind[i];
        if (find(first_stretched_layer.begin(),
                 first_stretched_layer.end(), neigh)
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
           i < Data.adjMatrix_row_ptr[idx+1]; ++i) {
        const auto neigh = Data.adjMatrix_col_ind[i];
        if (find(second_stretched_layer.begin(),
                 second_stretched_layer.end(), neigh)
              != second_stretched_layer.end()) {
          // Stretch the face
          Data.adjMatrix_areaValues[i] += 0.5;
        }
      }
    }
    const auto max_w = *max_element(Data.weights.begin(), Data.weights.end());
    for (const auto idx : bottom_layer)
      Data.weights[idx] = max_w + 1.;
    const CoMMAWeightT aniso_thresh{-4.}; // Negative so that every cell is considered
    const bool build_lines = true;
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
                  agglomerationLines_Idx, agglomerationLines, Data.weights,
                  build_lines, ODD_LINE_LENGTH, Data.dim);
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        iso_agg(fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, Data.weights, false);
      THEN("All cells have been agglomerated") {
        REQUIRE(Data.nb_fc ==
            static_cast<CoMMAIndexT>(
              count(cc_graph->_is_fc_agglomerated.begin(),
                    cc_graph->_is_fc_agglomerated.end(),
                    true)));
      }
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("The anisotropic coarse cells at the bottom are of cardinality 2") {
        REQUIRE(check2cells(0,1));
        REQUIRE(check2cells(2,3));
        REQUIRE(check2cells(9,8));
        REQUIRE(check2cells(11,10));
        REQUIRE(check2cells(12,13));
        REQUIRE(check2cells(16,14));
        REQUIRE(check2cells(20,22));
        REQUIRE(check2cells(23,21));
      }
      THEN("The anisotropic coarse cells at the top are of cardinality 3") {
        REQUIRE(check3cells(4,5,24));
        REQUIRE(check3cells(7,6,25));
        REQUIRE(check3cells(15,17,26));
        REQUIRE(check3cells(19,18,27));
      }
      THEN("The coarse-cell numbering the anisotropy and the priority weights") {
        REQUIRE(f2c[ 0].value() == 0);
        REQUIRE(f2c[ 2].value() == 1);
        REQUIRE(f2c[ 4].value() == 2);
        REQUIRE(f2c[11].value() == 3);
        REQUIRE(f2c[ 9].value() == 4);
        REQUIRE(f2c[ 7].value() == 5);
        REQUIRE(f2c[16].value() == 6);
        REQUIRE(f2c[12].value() == 7);
        REQUIRE(f2c[15].value() == 8);
        REQUIRE(f2c[20].value() == 9);
        REQUIRE(f2c[21].value() == 10);
        REQUIRE(f2c[19].value() == 11);
      }
    }
  }

  GIVEN("We load a 4by7 quad 2D mesh which has 4 anisotropic lines each of length 5 cells and"
        " a seeds pool with neighbourhood priority and priority weights for inverse numbering") {
    const DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    const bool build_lines = true;
    vector<CoMMAIndexT> agglomerationLines_Idx{};
    vector<CoMMAIndexT> agglomerationLines{};
    vector<CoMMAWeightT> w(Data.nb_fc);
    iota(w.begin(), w.end(), 0);
    shared_ptr<Seeds_Pool<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>> seeds_pool = make_shared<
      Seeds_Pool_Neighbourhood_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(Data.n_bnd_faces, w, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, w,
                  build_lines, Data.dim);
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        iso_agg(fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
    WHEN("We agglomerate the mesh") {
      aniso_agg.agglomerate_one_level(4, 4, 4, w, false);
      aniso_agg.update_seeds_pool();
      THEN("After the anisotropic agglomeration, the seeds pool is not empty and does not need an initialization") {
        REQUIRE(!seeds_pool->is_empty());
        REQUIRE(!seeds_pool->need_initialization(cc_graph->_is_fc_agglomerated));
      }
      THEN("Having chosen a neighbourhood by boundary, the seed is a neighbour of the first line") {
        REQUIRE(seeds_pool->choose_new_seed(cc_graph->_is_fc_agglomerated).value() == 18);
      }
      iso_agg.agglomerate_one_level(4, 4, 4, w, false);
      const auto f2c = cc_graph->_fc_2_cc;
      THEN("There are two isotropic coarse cells") {
        REQUIRE(check4cells(5,6,24,25));
        REQUIRE(check4cells(17,18,27,26));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2") {
        REQUIRE(check2cells(0,1));
        REQUIRE(check2cells(11,10));
        REQUIRE(check2cells(16,14));
        REQUIRE(check2cells(20,22));
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
        REQUIRE(check3cells(2,3,4));
        REQUIRE(check3cells(9,8,7));
        REQUIRE(check3cells(12,13,15));
        REQUIRE(check3cells(23,21,19));
      }
      THEN("The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
           " and the priority weights") {
        REQUIRE(f2c[ 0].value() == 6);
        REQUIRE(f2c[ 2].value() == 7);
        REQUIRE(f2c[ 9].value() == 5);
        REQUIRE(f2c[11].value() == 4);
        REQUIRE(f2c[13].value() == 3);
        REQUIRE(f2c[16].value() == 2);
        REQUIRE(f2c[20].value() == 0);
        REQUIRE(f2c[23].value() == 1);
      }
    }
  };

  GIVEN("We load a 4by7 quad 2D mesh which has 4 anisotropic lines each of length 5 cells and"
        " we simulate a restart (not first agglomeration, we already have the liens)") {
    const DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    const CoMMAWeightT aniso_thresh{2.};
    const bool build_lines = false;
    vector<CoMMAIndexT> agglomerationLines_Idx = {0,5,10,15,20};
    vector<CoMMAIndexT> agglomerationLines = {0,1,2,3,4,
                                              11,10,9,8,7,
                                              16,14,12,13,15,
                                              20,22,23,21,19};
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
    Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        aniso_agg(fc_graph, cc_graph, seeds_pool, aniso_thresh,
                  agglomerationLines_Idx, agglomerationLines, Data.weights,
                  build_lines, ODD_LINE_LENGTH, Data.dim);
    Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>
        iso_agg(fc_graph, cc_graph, seeds_pool, CoMMANeighbourhoodT::EXTENDED, FC_ITER, Data.dim);
    WHEN("We initialize the agglomerator") {
      THEN("The anisotropic lines have been read correctly and in order") {
        const auto n_lines = agglomerationLines_Idx.size() - 1;
        REQUIRE(static_cast<CoMMAIndexT>(n_lines) == aniso_agg._nb_lines[0]);
        auto read_line = aniso_agg._v_lines[0].cbegin();
        for (auto ref_line = agglomerationLines_Idx.cbegin() + 1;
             ref_line != agglomerationLines_Idx.cend();
             ++ref_line, ++read_line) {
          for (auto ref_fc = agglomerationLines.cbegin() + (*(ref_line - 1));
               ref_fc != agglomerationLines.cbegin() + (*ref_line);
               ++ref_fc) {
            REQUIRE(find((*read_line)->cbegin(), (*read_line)->cend(), *ref_fc)
                    != (*read_line)->cend());
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
        REQUIRE(check4cells(5,6,24,25));
        REQUIRE(check4cells(17,18,27,26));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2") {
        REQUIRE(check2cells(0,1));
        REQUIRE(check2cells(11,10));
        REQUIRE(check2cells(16,14));
        REQUIRE(check2cells(20,22));
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
        REQUIRE(check3cells(2,3,4));
        REQUIRE(check3cells(9,8,7));
        REQUIRE(check3cells(12,13,15));
        REQUIRE(check3cells(23,21,19));
      }
      THEN("The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
           " and the priority weights") {
        REQUIRE(f2c[ 0].value() == 0);
        REQUIRE(f2c[ 2].value() == 1);
        REQUIRE(f2c[ 9].value() == 3);
        REQUIRE(f2c[11].value() == 2);
        REQUIRE(f2c[13].value() == 5);
        REQUIRE(f2c[16].value() == 4);
        REQUIRE(f2c[20].value() == 6);
        REQUIRE(f2c[23].value() == 7);
      }
    }
  }

#undef check2cells
#undef check3cells
#undef check4cells
}

SCENARIO("Test the correction in 2D", "[Isotropic Correction]") {
  GIVEN("We load the Minimal Isotropic mesh structure") {
    const DualGPy_minimal Data = DualGPy_minimal();
    shared_ptr<SeedsPoolT> seeds_pool = make_shared<SeedsPoolT>(Data.n_bnd_faces, Data.weights, false);
    seeds_pool->initialize();
    shared_ptr<DualGraphT> fc_graph = make_shared<DualGraphT>(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.n_bnd_faces, Data.dim,
        Data.arrayOfFineAnisotropicCompliantCells);
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
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
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
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
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
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
    shared_ptr<CCContainerT> cc_graph = make_shared<CCContainerT>(fc_graph);
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

SCENARIO("Test of main function", "[structure]") {
  // The following are basically a copy of previous tests but with the main function
  GIVEN("A simple 4x4 Cartesian mesh") {
    const DualGPy_quad_4 Data = DualGPy_quad_4();
    WHEN("We agglomerate with neighbourhood priority") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc),
                          alines_idx{},
                          alines{};
      const bool aniso = false,
                 build_lines = true,
                 odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4,
                      min_card = 4,
                      max_card = 4;
      const CoMMAWeightT aniso_thr = -4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
          Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
          Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
          build_lines, aniso, odd_length, aniso_thr,
          seed,
          fc2cc, alines_idx, alines,
          correction, Data.dim, goal_card, min_card, max_card);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        REQUIRE(fc2cc[0]== 0);
        REQUIRE(fc2cc[1]== 0);
        REQUIRE(fc2cc[2]== 1);
        REQUIRE(fc2cc[3]== 1);
        REQUIRE(fc2cc[4]== 0);
        REQUIRE(fc2cc[5]== 0);
        REQUIRE(fc2cc[6]== 1);
        REQUIRE(fc2cc[7]== 1);
        REQUIRE(fc2cc[8]== 2);
        REQUIRE(fc2cc[9]== 2);
        REQUIRE(fc2cc[10]== 3);
        REQUIRE(fc2cc[11]== 3);
        REQUIRE(fc2cc[12]== 2);
        REQUIRE(fc2cc[13]== 2);
        REQUIRE(fc2cc[14]== 3);
        REQUIRE(fc2cc[15]== 3);
      }
    }
    WHEN("We agglomerate with boundary priority") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc),
                          alines_idx{},
                          alines{};
      const bool aniso = false,
                 build_lines = true,
                 odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4,
                      min_card = 4,
                      max_card = 4;
      const CoMMAWeightT aniso_thr = -4.;
      const auto seed = CoMMASeedsPoolT::BOUNDARY_PRIORITY;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
          Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
          Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
          build_lines, aniso, odd_length, aniso_thr,
          seed,
          fc2cc, alines_idx, alines,
          correction, Data.dim, goal_card, min_card, max_card);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        REQUIRE(fc2cc[0]== 0);
        REQUIRE(fc2cc[1]== 0);
        REQUIRE(fc2cc[2]== 1);
        REQUIRE(fc2cc[3]== 1);
        REQUIRE(fc2cc[4]== 0);
        REQUIRE(fc2cc[5]== 0);
        REQUIRE(fc2cc[6]== 1);
        REQUIRE(fc2cc[7]== 1);
        REQUIRE(fc2cc[8]== 2);
        REQUIRE(fc2cc[9]== 2);
        REQUIRE(fc2cc[10]== 3);
        REQUIRE(fc2cc[11]== 3);
        REQUIRE(fc2cc[12]== 2);
        REQUIRE(fc2cc[13]== 2);
        REQUIRE(fc2cc[14]== 3);
        REQUIRE(fc2cc[15]== 3);
      }
    }
    WHEN("We agglomerate with neighbourhood priority one point initialization") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc),
                          alines_idx{},
                          alines{};
      const bool aniso = false,
                 build_lines = true,
                 odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4,
                      min_card = 4,
                      max_card = 4;
      const CoMMAWeightT aniso_thr = -4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY_ONE_POINT_INIT;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
          Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
          Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
          build_lines, aniso, odd_length, aniso_thr,
          seed,
          fc2cc, alines_idx, alines,
          correction, Data.dim, goal_card, min_card, max_card);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        REQUIRE(fc2cc[0]== 0);
        REQUIRE(fc2cc[1]== 0);
        REQUIRE(fc2cc[2]== 1);
        REQUIRE(fc2cc[3]== 1);
        REQUIRE(fc2cc[4]== 0);
        REQUIRE(fc2cc[5]== 0);
        REQUIRE(fc2cc[6]== 1);
        REQUIRE(fc2cc[7]== 1);
        REQUIRE(fc2cc[8]== 2);
        REQUIRE(fc2cc[9]== 2);
        REQUIRE(fc2cc[10]== 3);
        REQUIRE(fc2cc[11]== 3);
        REQUIRE(fc2cc[12]== 2);
        REQUIRE(fc2cc[13]== 2);
        REQUIRE(fc2cc[14]== 3);
        REQUIRE(fc2cc[15]== 3);
      }
    }
    WHEN("We agglomerate with boundary priority one point initialization") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc),
                          alines_idx{},
                          alines{};
      const bool aniso = false,
                 build_lines = true,
                 odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4,
                      min_card = 4,
                      max_card = 4;
      const CoMMAWeightT aniso_thr = -4.;
      const auto seed = CoMMASeedsPoolT::BOUNDARY_PRIORITY_ONE_POINT_INIT;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
          Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
          Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
          build_lines, aniso, odd_length, aniso_thr,
          seed,
          fc2cc, alines_idx, alines,
          correction, Data.dim, goal_card, min_card, max_card);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        REQUIRE(fc2cc[0]== 0);
        REQUIRE(fc2cc[1]== 0);
        REQUIRE(fc2cc[2]== 1);
        REQUIRE(fc2cc[3]== 1);
        REQUIRE(fc2cc[4]== 0);
        REQUIRE(fc2cc[5]== 0);
        REQUIRE(fc2cc[6]== 1);
        REQUIRE(fc2cc[7]== 1);
        REQUIRE(fc2cc[8]== 2);
        REQUIRE(fc2cc[9]== 2);
        REQUIRE(fc2cc[10]== 3);
        REQUIRE(fc2cc[11]== 3);
        REQUIRE(fc2cc[12]== 2);
        REQUIRE(fc2cc[13]== 2);
        REQUIRE(fc2cc[14]== 3);
        REQUIRE(fc2cc[15]== 3);
      }
    }
  }
#define outcheck2cells(a,b)     (fc2cc[a] == fc2cc[b])
#define outcheck3cells(a,b,c)   (fc2cc[a] == fc2cc[b] && fc2cc[b] == fc2cc[c])
#define outcheck4cells(a,b,c,d) (fc2cc[a] == fc2cc[b] && fc2cc[b] == fc2cc[c] && fc2cc[c] == fc2cc[d])
  GIVEN("a 4by7 quad 2D mesh which has 4 anisotropic lines") {
    const DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    WHEN("We agglomerate with bad argument") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc),
                          alines_idx{},
                          alines{};
      const bool aniso = true,
                 build_lines = true,
                 odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4,
                      min_card = 4,
                      max_card = 4;
      const CoMMAWeightT aniso_thr = 4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY;
      THEN("CoMMA throws if invalid arguments") {
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              5,
              Data.dim, goal_card, min_card, max_card)
        );
        vector<CoMMAIndexT> tmp = {};
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              tmp,
              Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, min_card, goal_card, min_card, max_card)
        );
        tmp.push_back(2);
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              tmp,
              Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes, Data.centers,
              Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces, build_lines,
              aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction, Data.dim,
              min_card, goal_card, min_card, max_card)
        );
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr,
              tmp,
              Data.adjMatrix_areaValues, Data.volumes, Data.centers, Data.weights,
              Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces, build_lines, aniso,
              odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction, Data.dim, goal_card,
              min_card, max_card)
        );
        vector<CoMMAWeightT> tmp_w = {};
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
              tmp_w,
              Data.volumes, Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells,
              Data.n_bnd_faces, build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx,
              alines, correction, Data.dim, goal_card, min_card, max_card)
        );
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, goal_card,
              8,
              max_card)
        );
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim,
              0,
              min_card, max_card)
        );
        vector<CoMMAIndexT> tmp_idx = {0, 0},
                            tmp_lines = {4};
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
              false,
              aniso, odd_length, aniso_thr, seed, fc2cc,
              tmp_idx, tmp_lines,
              correction, Data.dim, goal_card, min_card, max_card)
        );
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, goal_card, min_card, max_card,
              0)
        );
        REQUIRE_THROWS(
          agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
              Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
              Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
              build_lines, aniso, odd_length, aniso_thr, seed, fc2cc, alines_idx, alines, correction,
              Data.dim, goal_card, min_card, max_card, 100)
        );
      }
    }
    WHEN("We agglomerate with neighbourhood priority") {
      vector<CoMMAIndexT> fc2cc(Data.nb_fc),
                          alines_idx{},
                          alines{};
      const bool aniso = true,
                 build_lines = true,
                 odd_length = true,
                 correction = true;
      const CoMMAIntT goal_card = 4,
                      min_card = 4,
                      max_card = 4;
      const CoMMAWeightT aniso_thr = 4.;
      const auto seed = CoMMASeedsPoolT::NEIGHBOURHOOD_PRIORITY;
      agglomerate_one_level<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(
          Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind, Data.adjMatrix_areaValues, Data.volumes,
          Data.centers, Data.weights, Data.arrayOfFineAnisotropicCompliantCells, Data.n_bnd_faces,
          build_lines, aniso, odd_length, aniso_thr,
          seed,
          fc2cc, alines_idx, alines,
          correction, Data.dim, goal_card, min_card, max_card);
      THEN("There are two isotropic coarse cells") {
        REQUIRE(outcheck4cells(5,6,24,25));
        REQUIRE(outcheck4cells(17,18,27,26));
      }
      THEN("The anisotropic coarse cells at the boundary are of cardinality 2") {
        REQUIRE(outcheck2cells(0,1));
        REQUIRE(outcheck2cells(11,10));
        REQUIRE(outcheck2cells(16,14));
        REQUIRE(outcheck2cells(20,22));
      }
      THEN("The interior anisotropic coarse cells are of cardinality 3") {
        REQUIRE(outcheck3cells(2,3,4));
        REQUIRE(outcheck3cells(9,8,7));
        REQUIRE(outcheck3cells(12,13,15));
        REQUIRE(outcheck3cells(23,21,19));
      }
      THEN("The coarse-cell numbering reflects the boundary trick (leave 3-cells cluster inside)"
           " and the priority weights") {
        REQUIRE(fc2cc[ 0] == 0);
        REQUIRE(fc2cc[ 2] == 1);
        REQUIRE(fc2cc[ 9] == 3);
        REQUIRE(fc2cc[11] == 2);
        REQUIRE(fc2cc[13] == 5);
        REQUIRE(fc2cc[16] == 4);
        REQUIRE(fc2cc[20] == 6);
        REQUIRE(fc2cc[23] == 7);
      }
    }
  }
#undef outcheck2cells
#undef outcheck3cells
#undef outcheck4cells
}
#undef equal_up_to
#undef check_
#undef found_
#undef not_found_
