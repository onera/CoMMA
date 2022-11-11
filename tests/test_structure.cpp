#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "input/DualGPy.h"
#include "CoMMATypes.h"
#include "Priority_Pair.h"
#include <iterator>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>
#include <utility>
// I input with DualGPy configuration the configuration you can
// find in the README of the library

using namespace std;

#define equal_up_to(a,b,eps) (fabs(a - b) < eps)

SCENARIO("Test of a structure", "[structure]") {
  GIVEN("A simple graph, and we build the Dual Graph") {
    DualGPy Data = DualGPy();
    // Construction of the Dual Graph element
    Seeds_Pool<CoMMAIndexT, CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> cc_graph(fc_graph);
    // Check the effective length
    WHEN("We try to access to the member variables") {
      class test : public Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> {
       public:
        test(Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &graph,
             Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &cc_graph,
             CoMMAIntT dimension)
            : Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(graph, cc_graph,
                                                                dimension) {};

        CoMMAIntT test_variable() {
          return (this->_threshold_card);
        };
      };
      THEN("We see that the agglomeration is not set, hence set to -1") {
        test *agg = new test(fc_graph, cc_graph, 2);
        CoMMAIntT testing = agg->test_variable();
        REQUIRE(testing == -1);
      }
    }
    WHEN("We try to access to Define the cardinality") {
      class test : public Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> {
       public:
        test(Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &graph,
             Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> &cc_graph,
             CoMMAIntT dimension)
            : Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>(graph, cc_graph,
                                                                dimension) {};

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
      THEN("We see that the cardinality passes from -1 to 2") {
        test *agg = new test(fc_graph, cc_graph, 2);

        REQUIRE(agg->thres() == -1);
        REQUIRE(agg->max() == -1);
        REQUIRE(agg->min() == -1);
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
    Queue<CoMMAIndexT> st_long;
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
    Priority_Pair<CoMMAIntT, CoMMAIntT> p1{0,1}, p1bis{0,1},
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
    set<Priority_Pair<CoMMAIntT, CoMMAIntT>> s = 
      {{0,1}, {0,1}, {3,0}, {0,0}, {2,-1}};
    WHEN("We iterate over the set") {
      vector<CoMMAIntT> fe1 = {3,  2, 0, 0};
      vector<CoMMAIntT> se1 = {0, -1, 0, 1};
      THEN("The expected order is obtined") {
        for (auto it = s.begin(); it != s.end(); ++it) {
          const auto idx = distance(s.begin(), it);
          REQUIRE(it->first()  == fe1[idx]);
          REQUIRE(it->second() == se1[idx]);
        }
      }
    }
    WHEN("We add an item and iterate on the new set") {
      s.emplace(1,4);
      vector<CoMMAIntT> fe2 = {3,  2, 1, 0, 0};
      vector<CoMMAIntT> se2 = {0, -1, 4, 0, 1};
      THEN("The expected order is obtined") {
        for (auto it = s.begin(); it != s.end(); ++it) {
          const auto idx = distance(s.begin(), it);
          REQUIRE(it->first()  == fe2[idx]);
          REQUIRE(it->second() == se2[idx]);
        }
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

SCENARIO("Test dual graph and neighborhood computing", "[Dual graph & Neighborhood]") {
  GIVEN("We have a 7x7 Cartesian 2D matrix") {
    DualGPy_quad_7 Data = DualGPy_quad_7();
    Seeds_Pool<CoMMAIndexT,CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
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
      THEN("Check order sizes and composition") {
        // Size
        REQUIRE(neighs[0].size() == 4);
        REQUIRE(neighs[1].size() == 8);
        REQUIRE(neighs[2].size() == 12);
        // First order
        REQUIRE(neighs[0].count(17) > 0);
        REQUIRE(neighs[0].count(23) > 0);
        REQUIRE(neighs[0].count(25) > 0);
        REQUIRE(neighs[0].count(31) > 0);
        // Second order
        REQUIRE(neighs[1].count(10) > 0);
        REQUIRE(neighs[1].count(16) > 0);
        REQUIRE(neighs[1].count(18) > 0);
        REQUIRE(neighs[1].count(22) > 0);
        REQUIRE(neighs[1].count(26) > 0);
        REQUIRE(neighs[1].count(30) > 0);
        REQUIRE(neighs[1].count(32) > 0);
        REQUIRE(neighs[1].count(38) > 0);
        // Third order
        REQUIRE(neighs[2].count(3) > 0);
        REQUIRE(neighs[2].count(9) > 0);
        REQUIRE(neighs[2].count(11) > 0);
        REQUIRE(neighs[2].count(15) > 0);
        REQUIRE(neighs[2].count(19) > 0);
        REQUIRE(neighs[2].count(21) > 0);
        REQUIRE(neighs[2].count(27) > 0);
        REQUIRE(neighs[2].count(29) > 0);
        REQUIRE(neighs[2].count(33) > 0);
        REQUIRE(neighs[2].count(37) > 0);
        REQUIRE(neighs[2].count(39) > 0);
        REQUIRE(neighs[2].count(45) > 0);
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
      THEN("Check order sizes and composition") {
        // Size
        REQUIRE(neighs[0].size() == 3);
        REQUIRE(neighs[1].size() == 3);
        REQUIRE(neighs[2].size() == 5);
        // First order
        REQUIRE(neighs[0].count(17) > 0);
        REQUIRE(neighs[0].count(23) > 0);
        REQUIRE(neighs[0].count(25) > 0);
        // Second order
        REQUIRE(neighs[1].count(18) > 0);
        REQUIRE(neighs[1].count(22) > 0);
        REQUIRE(neighs[1].count(26) > 0);
        // Third order
        REQUIRE(neighs[2].count(11) > 0);
        REQUIRE(neighs[2].count(15) > 0);
        REQUIRE(neighs[2].count(19) > 0);
        REQUIRE(neighs[2].count(21) > 0);
        REQUIRE(neighs[2].count(27) > 0);
      }
    } // WHEN PREVIOUS AGGLOMERATION
  };
  GIVEN("We have a 7x7 Cartesian 2D matrix and set up a standard First Order Neighborhood for 24") {
    DualGPy_quad_7 Data = DualGPy_quad_7();
    Seeds_Pool<CoMMAIndexT,CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
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
    First_Order_Neighbourhood<CoMMAIndexT, CoMMAIntT> f_o_n =
        First_Order_Neighbourhood<CoMMAIndexT, CoMMAIntT>(s_neighbours_of_seed, 2, false);
    unordered_set<CoMMAIndexT> fon = f_o_n.update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first FON") {
      THEN("Only direct neighbors are in the FON") {
        REQUIRE(fon.count(17) > 0);
        REQUIRE(fon.count(23) > 0);
        REQUIRE(fon.count(25) > 0);
        REQUIRE(fon.count(31) > 0);
      }
    }
    fon = f_o_n.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      THEN("Cell 31 is no more in the FON") {
        REQUIRE(fon.count(31) == 0);
      }
      THEN("Old neighbors are still in the FON") {
        REQUIRE(fon.count(17) > 0);
        REQUIRE(fon.count(23) > 0);
        REQUIRE(fon.count(25) > 0);
      }
      THEN("Direct neighbors of 31 are in the FON") {
        REQUIRE(fon.count(30) > 0);
        REQUIRE(fon.count(32) > 0);
        REQUIRE(fon.count(38) > 0);
      }
    }
    fon = f_o_n.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      THEN("Cell 38 is no more in the FON") {
        REQUIRE(fon.count(38) == 0);
      }
      THEN("Old neighbors are still in the FON") {
        REQUIRE(fon.count(17) > 0);
        REQUIRE(fon.count(23) > 0);
        REQUIRE(fon.count(25) > 0);
        REQUIRE(fon.count(30) > 0);
        REQUIRE(fon.count(32) > 0);
      }
      THEN("Direct neighbors of 31 are NOT in the FON (max order neighborhood)") {
        REQUIRE(fon.count(37) == 0);
        REQUIRE(fon.count(39) == 0);
        REQUIRE(fon.count(45) == 0);
      }
    }
  };
  GIVEN("We have a 7x7 Cartesian 2D matrix and set up a Pure Front First Order Neighborhood for 24") {
    DualGPy_quad_7 Data = DualGPy_quad_7();
    Seeds_Pool<CoMMAIndexT,CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
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
    First_Order_Neighbourhood<CoMMAIndexT, CoMMAIntT> f_o_n =
        First_Order_Neighbourhood<CoMMAIndexT, CoMMAIntT>(s_neighbours_of_seed, 2, true);
    unordered_set<CoMMAIndexT> fon = f_o_n.update(seed, fc_graph.get_neighbours(seed));
    WHEN("We check the first FON") {
      THEN("Only direct neighbors are in the FON") {
        REQUIRE(fon.count(17) > 0);
        REQUIRE(fon.count(23) > 0);
        REQUIRE(fon.count(25) > 0);
        REQUIRE(fon.count(31) > 0);
      }
    }
    fon = f_o_n.update(31, fc_graph.get_neighbours(31));
    WHEN("We add cell 31") {
      THEN("Direct neighbors of 31 are in the current FON") {
        REQUIRE(fon.count(30) > 0);
        REQUIRE(fon.count(32) > 0);
        REQUIRE(fon.count(38) > 0);
      }
      auto prev_fon = f_o_n._q_fon.begin() + 1;
      THEN("Cell 31 is no more in the previous FON") {
        REQUIRE(prev_fon->count(31) == 0);
      }
      THEN("Old neighbors are still in the previous FON") {
        REQUIRE(prev_fon->count(17) > 0);
        REQUIRE(prev_fon->count(23) > 0);
        REQUIRE(prev_fon->count(25) > 0);
      }
    }
    fon = f_o_n.update(38, fc_graph.get_neighbours(38));
    WHEN("We add cell 38") {
      THEN("Direct neighbors of 31 are NOT in the NOT (max order neighborhood)") {
        REQUIRE(fon.count(37) == 0);
        REQUIRE(fon.count(39) == 0);
        REQUIRE(fon.count(45) == 0);
      }
      THEN("First ever computed FON is returned since no direct neighbors were added") {
        REQUIRE(fon.count(17) > 0);
        REQUIRE(fon.count(23) > 0);
        REQUIRE(fon.count(25) > 0);
      }
      auto prev_fon = f_o_n._q_fon.begin() + 1;
      THEN("Cell 38 is no more in the previous FON") {
        REQUIRE(prev_fon->count(38) == 0);
      }
      THEN("Old neighbors are still in the previous FON") {
        REQUIRE(prev_fon->count(30) > 0);
        REQUIRE(prev_fon->count(32) > 0);
      }
      prev_fon++;
      THEN("Older neighbors are still in the second-to-last FON (which happens to be the first ever, hence the current)") {
        REQUIRE(prev_fon->count(17) > 0);
        REQUIRE(prev_fon->count(23) > 0);
        REQUIRE(prev_fon->count(25) > 0);
      }
    }
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
    Seeds_Pool<CoMMAIndexT,CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_graph(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_graph, 3);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(8, 8, 8, false);
      THEN("We obtain the 64 fine cells divided in 8 coarse cells") {
        auto fccc = cc_graph._fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4};
        for (auto i = 0; i != Data.nb_fc; i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }

      }
    }
    WHEN("We agglomerate the mesh with a biconnected agglomerator and we try to correct") {
      agg->agglomerate_one_level(8, 8, 8, true);
      THEN("Corrections are applied") {
        auto fccc = cc_graph._fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4};
        for (auto i = 0; i != Data.nb_fc; i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }
      }
    }

    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_PF_graph(fc_graph);
    auto agg_PF =
      make_unique<Agglomerator_Pure_Front<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_PF_graph, 3);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(8, 8, 8, false);
      THEN("We obtain the 64 fine cells divided in 9 coarse cells") {
        auto fccc = cc_PF_graph._fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {1, 1, 3, 3, 1, 8, 8, 3, 4, 8, 8, 0, 4, 4, 0, 0, 1, 1, 3, 3, 6, 6, 8, 5, 4, 6, 8, 0, 4, 4, 8, 0, 1, 1, 3, 3, 6, 6, 5, 5, 4, 6, 6, 0, 4, 2, 2, 0, 1, 7, 5, 3, 6, 5, 5, 5, 2, 2, 2, 5, 2, 2, 2, 0};
        for (auto i = 0; i != Data.nb_fc; i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }
      }
    }
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct") {
      agg_PF->agglomerate_one_level(8, 8, 8, true);
      THEN("Corrections are applied") {
        auto fccc = cc_PF_graph._fc_2_cc;
        vector<CoMMAIndexT> fc2cc_req = {1, 1, 3, 3, 1, 7, 7, 3, 4, 7, 7, 0, 4, 4, 0, 0, 1, 1, 3, 3, 6, 6, 7, 5, 4, 6, 7, 0, 4, 4, 7, 0, 1, 1, 3, 3, 6, 6, 5, 5, 4, 6, 6, 0, 4, 2, 2, 0, 1, 1, 5, 3, 6, 5, 5, 5, 2, 2, 2, 5, 2, 2, 2, 0};
        for (auto i = 0; i != Data.nb_fc; i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }
      }
    }

    WHEN("We compute the aspect-ratio of a coarse cell on the boundary") {
      const CoMMAWeightT eps = 1e-10;
      // In
      unordered_set<CoMMAIndexT> cc = {0,1,4,5};
      CoMMAWeightT tmp_surf{-1.}, tmp_vol{-1.};
      CoMMAWeightT cc_surf = 12.,
                   cc_vol  = 4.;
      // Out
      CoMMAIntT shared_faces;
      CoMMAWeightT ar, ar2, ar3;
      CoMMAWeightT ref_surf = 16.,
                   ref_vol  = 5.;
      CoMMAWeightT ref_ar = agg->_compute_AR(ref_surf, ref_vol);
      THEN("Boundary faces are approximated [0 boundary faces]") {
        agg->compute_next_cc_features(21, cc_surf, cc_vol, cc, shared_faces, ar, tmp_surf, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_surf, tmp_surf, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      THEN("Boundary faces are approximated [1 boundary face]") {
        agg->compute_next_cc_features(6, cc_surf, cc_vol, cc, shared_faces, ar2, tmp_surf, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_surf, tmp_surf, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar2, eps));
      }
      // If the two above pass, then the following one should pass too, still, better
      // be safe than sorry
      THEN("With or without boundary faces, the result is the same") {
        REQUIRE(equal_up_to(ar, ar2, eps));
      }
      THEN("Boundary faces are approximated [2 boundary faces]") {
        agg->compute_next_cc_features(2, cc_surf, cc_vol, cc, shared_faces, ar3, tmp_surf, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_surf, tmp_surf, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar3, eps));
      }
      // If the two above pass, then the following one should pass too, still, better
      // be safe than sorry
      THEN("With or without boundary faces, the result is the same") {
        REQUIRE(equal_up_to(ar2, ar3, eps));
      }
      cc.erase(0);
      cc_surf  = 14., cc_vol  = 3.;
      ref_surf = 16., ref_vol = 4.;
      ref_ar = agg->_compute_AR(ref_surf, ref_vol);
      THEN("Boundary faces are approximated [3 boundary faces, 2 shared faces]") {
        agg->compute_next_cc_features(0, cc_surf, cc_vol, cc, shared_faces, ar, tmp_surf, tmp_vol);
        REQUIRE(shared_faces == 2);
        REQUIRE(equal_up_to(ref_surf, tmp_surf, eps));
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
    Seeds_Pool<CoMMAIndexT,CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_graph(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_graph, 2);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a biconnected agglomerator") {
      agg->agglomerate_one_level(4, 4, 4, false);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        auto fccc = cc_graph._fc_2_cc;
        REQUIRE(fccc[0]== 0);
        REQUIRE(fccc[1]== 0);
        REQUIRE(fccc[2]== 2);
        REQUIRE(fccc[3]== 2);
        REQUIRE(fccc[4]== 0);
        REQUIRE(fccc[5]== 0);
        REQUIRE(fccc[6]== 2);
        REQUIRE(fccc[7]== 2);
        REQUIRE(fccc[8]== 3);
        REQUIRE(fccc[9]== 3);
        REQUIRE(fccc[10]== 1);
        REQUIRE(fccc[11]== 1);
        REQUIRE(fccc[12]== 3);
        REQUIRE(fccc[13]== 3);
        REQUIRE(fccc[14]== 1);
        REQUIRE(fccc[15]== 1);
      }
    }
    WHEN("We agglomerate the mesh with a biconnected agglomerator and we try to correct") {
      agg->agglomerate_one_level(4, 4, 4, true);
      THEN("Nothing changes with respect to the case without correction") {
        auto fccc = cc_graph._fc_2_cc;
        REQUIRE(fccc[0]== 0);
        REQUIRE(fccc[1]== 0);
        REQUIRE(fccc[2]== 2);
        REQUIRE(fccc[3]== 2);
        REQUIRE(fccc[4]== 0);
        REQUIRE(fccc[5]== 0);
        REQUIRE(fccc[6]== 2);
        REQUIRE(fccc[7]== 2);
        REQUIRE(fccc[8]== 3);
        REQUIRE(fccc[9]== 3);
        REQUIRE(fccc[10]== 1);
        REQUIRE(fccc[11]== 1);
        REQUIRE(fccc[12]== 3);
        REQUIRE(fccc[13]== 3);
        REQUIRE(fccc[14]== 1);
        REQUIRE(fccc[15]== 1);
      }
    }
    
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_PF_graph(fc_graph);
    auto agg_PF =
      make_unique<Agglomerator_Pure_Front<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
          fc_graph, cc_PF_graph, 2);
    // COMPLETE THE TEST
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator") {
      agg_PF->agglomerate_one_level(4, 4, 4, false);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
        // Nothing changes with respect to the case of the standard Biconnected 
        // We have a trick that make it work as it should
        auto fccc = cc_PF_graph._fc_2_cc;
        REQUIRE(fccc[0]== 0);
        REQUIRE(fccc[1]== 0);
        REQUIRE(fccc[2]== 2);
        REQUIRE(fccc[3]== 2);
        REQUIRE(fccc[4]== 0);
        REQUIRE(fccc[5]== 0);
        REQUIRE(fccc[6]== 2);
        REQUIRE(fccc[7]== 2);
        REQUIRE(fccc[8]== 3);
        REQUIRE(fccc[9]== 3);
        REQUIRE(fccc[10]== 1);
        REQUIRE(fccc[11]== 1);
        REQUIRE(fccc[12]== 3);
        REQUIRE(fccc[13]== 3);
        REQUIRE(fccc[14]== 1);
        REQUIRE(fccc[15]== 1);
      }
    }
    WHEN("We agglomerate the mesh with a pure front-advancing agglomerator and we try to correct") {
      agg_PF->agglomerate_one_level(4, 4, 4, true);
      THEN("Nothing changes with respect to the case without correction") {
        auto fccc = cc_PF_graph._fc_2_cc;
        REQUIRE(fccc[0]== 0);
        REQUIRE(fccc[1]== 0);
        REQUIRE(fccc[2]== 2);
        REQUIRE(fccc[3]== 2);
        REQUIRE(fccc[4]== 0);
        REQUIRE(fccc[5]== 0);
        REQUIRE(fccc[6]== 2);
        REQUIRE(fccc[7]== 2);
        REQUIRE(fccc[8]== 3);
        REQUIRE(fccc[9]== 3);
        REQUIRE(fccc[10]== 1);
        REQUIRE(fccc[11]== 1);
        REQUIRE(fccc[12]== 3);
        REQUIRE(fccc[13]== 3);
        REQUIRE(fccc[14]== 1);
        REQUIRE(fccc[15]== 1);
      }
    }

    WHEN("We compute the aspect-ratio of a coarse cell on the boundary") {
      const CoMMAWeightT eps = 1e-10;
      // In
      unordered_set<CoMMAIndexT> cc = {0,1};
      CoMMAWeightT cc_surf = 6.,
                   cc_vol  = 2.;
      // Out
      CoMMAWeightT tmp_surf{-1.}, tmp_vol{-1.};
      CoMMAIntT shared_faces;
      CoMMAWeightT ar, ar2;
      CoMMAWeightT ref_surf = 8.,
                   ref_vol  = 3.;
      CoMMAWeightT ref_ar = agg->_compute_AR(ref_surf, ref_vol);
      THEN("Boundary faces are approximated [0 boundary faces]") {
        agg->compute_next_cc_features(5, cc_surf, cc_vol, cc, shared_faces, ar, tmp_surf, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_surf, tmp_surf, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      THEN("Boundary faces are approximated [1 boundary face]") {
        agg->compute_next_cc_features(2, cc_surf, cc_vol, cc, shared_faces, ar2, tmp_surf, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_surf, tmp_surf, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar2, eps));
      }
      // If the two above pass, then the following one should pass too, still, better
      // be safe than sorry
      THEN("With or without boundary faces, the result is the same") {
        REQUIRE(equal_up_to(ar, ar2, eps));
      }
      cc.erase(0);
      cc.insert(2);
      THEN("Boundary faces are approximated [2 boundary faces]") {
        agg->compute_next_cc_features(0, cc_surf, cc_vol, cc, shared_faces, ar, tmp_surf, tmp_vol);
        REQUIRE(shared_faces == 1);
        REQUIRE(equal_up_to(ref_surf, tmp_surf, eps));
        REQUIRE(equal_up_to(ref_vol, tmp_vol, eps));
        REQUIRE(equal_up_to(ref_ar, ar, eps));
      }
      cc.erase(2);
      cc.insert(0);
      cc.insert(4);
      cc_surf  = 8., cc_vol  = 3.;
      ref_surf = 8., ref_vol = 4.;
      ref_ar = agg->_compute_AR(ref_surf, ref_vol);
      THEN("Boundary faces are approximated [2 shared faces]") {
        agg->compute_next_cc_features(5, cc_surf, cc_vol, cc, shared_faces, ar, tmp_surf, tmp_vol);
        REQUIRE(shared_faces == 2);
        REQUIRE(equal_up_to(ref_surf, tmp_surf, eps));
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
    Seeds_Pool<CoMMAIndexT, CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_graph(fc_graph);
    shared_ptr<Agglomerator<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>> agg1 =
        make_shared<Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
            fc_graph, cc_graph, 4, 3);
    // I progress with the downcasting to get the anisotropic lines
    shared_ptr<Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>
        agg_dyn = dynamic_pointer_cast<
            Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(agg1);
    // We setup the structures to gather the agglomeration lines
    CoMMAIndexT nb_agglomeration_lines = 0;
    vector<deque<CoMMAIndexT> *> agglomeration_lines;
    // We pass the structures to the level 0
    agg_dyn->_v_lines[0] = agglomeration_lines;
    agg_dyn->_v_nb_lines[0] = nb_agglomeration_lines;
    WHEN("We proceed with the agglomeration of the anisotropic lines (we gather them and later we agglomerate") {
         agg_dyn->agglomerate_one_level(2, 2, 2, false);
      THEN("We have a number of agglomeration lines != 0") { REQUIRE(agg_dyn->_v_nb_lines[0]!=0);}
    }
  };
  GIVEN("We load a 4by6 quad 2D mesh which has 4 anisotropic lines each of length 5 cells") {
    DualGPy_aniso_3cell Data = DualGPy_aniso_3cell();
    const CoMMAWeightT aniso_thresh{4.};
    Seeds_Pool<CoMMAIndexT, CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT,CoMMAIntT> cc_graph(fc_graph);
    auto agg = make_unique<Agglomerator_Anisotropic<CoMMAIndexT, CoMMAWeightT,CoMMAIntT>>(
            fc_graph, cc_graph, aniso_thresh, Data.dim);
    agg->agglomerate_one_level(4, 4, 4, false);
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
    Seeds_Pool<CoMMAIndexT, CoMMAIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc);
    Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT> cc_graph(fc_graph);
    auto agg = make_unique<Agglomerator_Biconnected<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>>(
        fc_graph, cc_graph, 2);
    // COMPLETE THE TEST
    WHEN("We proceed with the Isotropic agglomeration") {
      agg->agglomerate_one_level(2, 2, 2, true);

      THEN("No cells are left with cardinality 1") {
        for (auto i = cc_graph._cc_vec.begin(); i != cc_graph._cc_vec.end();
             i++) {
          REQUIRE(i->second->_cardinality != 1);
        }
      }
    }
  };
}
#undef equal_up_to
