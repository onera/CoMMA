#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "input/DualGPy.h"
#include "test_types.h"
// I input with DualGPy configuration the configuration you can
// find in the README of the library

SCENARIO("Test of a structure", "[structure]") {
  GIVEN("A simple graph, and we build the Dual Graph") {
    DualGPy Data = DualGPy();
    // Construction of the Dual Graph element
    Seeds_Pool<TestIndexT, TestIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<TestIndexT, TestWeightT, TestIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc, 2);
    Coarse_Cell_Container<TestIndexT, TestWeightT, TestIntT> cc_graph(fc_graph);
    // Check the effective length
    WHEN("We try to access to the member variables") {
      class test : public Agglomerator_Biconnected<TestIndexT, TestWeightT, TestIntT> {
       public:
        test(Dual_Graph<TestIndexT, TestWeightT, TestIntT> &graph,
             Coarse_Cell_Container<TestIndexT, TestWeightT, TestIntT> &cc_graph,
             TestIntT dimension)
            : Agglomerator_Biconnected<TestIndexT, TestWeightT, TestIntT>(graph, cc_graph,
                                                                dimension) {};

        TestIntT test_variable() {
          return (this->_threshold_card);
        };
      };
      THEN("We see that the agglomeration is not set, hence set to -1") {
        test *agg = new test(fc_graph, cc_graph, 2);
        TestIntT testing = agg->test_variable();
        REQUIRE(testing == -1);
      }
    }
    WHEN("We try to access to Define the cardinality") {
      class test : public Agglomerator_Biconnected<TestIndexT, TestWeightT, TestIntT> {
       public:
        test(Dual_Graph<TestIndexT, TestWeightT, TestIntT> &graph,
             Coarse_Cell_Container<TestIndexT, TestWeightT, TestIntT> &cc_graph,
             TestIntT dimension)
            : Agglomerator_Biconnected<TestIndexT, TestWeightT, TestIntT>(graph, cc_graph,
                                                                dimension) {};

        TestIntT thres() {
          return (_threshold_card);
        };
        TestIntT max() {
          return (_max_card);
        };
        TestIntT min() {
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
  GIVEN("A simple Queue of TestIndexT type") {
    Queue<TestIndexT> st_long;
    WHEN("We push a certain number of elements") {
      st_long.push(0);
      st_long.push(1);
      st_long.push(2);
      THEN(
          "We can check that the operation on the stack are correctly "
          "realized") {

        REQUIRE(st_long.top() == 0);
        TestIndexT top = st_long.pop();
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
    shared_ptr<node> A(new node(2, 1));
    shared_ptr<Tree> Albero(new Tree(A));
    WHEN("We add something to the tree") {
      Albero->insertSon(2, 1, 1, 1);
      THEN("The tree has a child") {
        Albero->insertSon(2, 3, 1, 1);
        Albero->insertSon(2, 4, 1, 1);
        Albero->insertSon(4, 2, 1, 0);
        Albero->print();
        REQUIRE(Albero->_root->_sonc == 3);
        Albero->deleteNode(4);
        Albero->print();
        REQUIRE(Albero->_root->_sonc == 2);
      }
    };
  }
}
SCENARIO("Subgraph", "[Subgraph]") {
  GIVEN("We have the CSR representation") {
    vector<CoMMAIndexT> adjMatrix_row_ptr = {0, 2, 4, 7, 9, 10};
    vector<CoMMAIndexT> adjMatrix_col_ind = {1, 3, 0, 2, 1, 3, 4, 0, 2, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    vector<CoMMAWeightT> volumes = {1, 1, 1, 1, 1};
    WHEN("We build the graph") {
      vector<TestIndexT> _mapping_l_to_g = {20, 30, 40, 50, 60};
      auto Marion = make_shared<Subgraph<TestIndexT, TestWeightT, TestIntT>>(
          5, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues,
          volumes, _mapping_l_to_g, true);
      THEN("We remove a node") { Marion->remove_node(50); }
      THEN("We add a node") {
        vector<TestIndexT> v_neigh = {30, 20};
        TestIndexT i_fc = 10;
        vector<TestWeightT> weight = {1, 1};
        Marion->insert_node(v_neigh, i_fc, 1, weight);
        REQUIRE(Marion->check_connectivity() == true);
        REQUIRE(Marion->_mapping_l_to_g[5] == 10);
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
    vector<TestIndexT> mapping_l_to_g_0 = {0, 1, 5, 4};
    vector<TestIndexT> mapping_l_to_g_1 = {2, 3, 7, 6};
    vector<TestIndexT> mapping_l_to_g_2 = {8, 3, 13, 12};
    vector<TestIndexT> mapping_l_to_g_3 = {10, 11, 15, 14};
    auto cc0 = make_shared<Subgraph<TestIndexT, TestWeightT, TestIntT>>(
        4, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        mapping_l_to_g_0, true);
    auto cc1 = make_shared<Subgraph<TestIndexT, TestWeightT, TestIntT>>(
        4, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        mapping_l_to_g_1, true);
    auto cc2 = make_shared<Subgraph<TestIndexT, TestWeightT, TestIntT>>(
        4, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        mapping_l_to_g_2, true);
    auto cc3 = make_shared<Subgraph<TestIndexT, TestWeightT, TestIntT>>(
        4, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        mapping_l_to_g_3, true);
    WHEN("We Collect the cells in the Bimap") {

      Bimap<TestIndexT, shared_ptr<Subgraph<TestIndexT, TestWeightT, TestIntT>>>
          Collection;
      Collection.insert(0, cc0);
      Collection.insert(1, cc1);
      Collection.insert(2, cc2);
      Collection.insert(3, cc3);
      THEN("We eliminate an element and we check the dimension changed") {
        auto lung = Collection.lung();
        REQUIRE(lung == 4);
        TestIndexT elim = 2;
        Collection.erase_B(elim);
        lung = Collection.lung();
        REQUIRE(lung == 3);
        for (auto i = elim + 1; i != lung + 1; i++) {
          Collection.update_nodeB(i, i - 1);
        }
        Collection.print();
      }
    }
  };
}

SCENARIO("Test the insertion of a coarse cell and deletion",
         "[Insertion Deletion]") {
  GIVEN("We have a Bimap of TestIndexT and shared_ptr") {
    vector<CoMMAIndexT> adjMatrix_row_ptr = {0, 2, 4, 7, 9, 10};
    vector<CoMMAIndexT> adjMatrix_col_ind = {1, 3, 0, 2, 1, 3, 4, 0, 2, 2};
    vector<CoMMAWeightT> adjMatrix_areaValues = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    vector<CoMMAWeightT> volumes = {1, 1, 1, 1, 1};
    vector<TestIndexT> _mapping_l_to_g = {20, 30, 40, 50, 60};
    auto Marion = make_shared<Subgraph<TestIndexT, TestWeightT, TestIntT>>(
        5, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        _mapping_l_to_g, true);
    Bimap<TestIndexT, shared_ptr<Subgraph<TestIndexT, TestWeightT, TestIntT>>> Collection;
    WHEN("We insert an element and we delete it") {
      TestIndexT ins = 0;
      Collection.insert(ins, Marion);
      Collection.print();
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
    Seeds_Pool<TestIndexT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<TestIndexT, TestWeightT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc, 3);
    Coarse_Cell_Container<TestIndexT, TestWeightT> cc_graph(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<TestIndexT, TestWeightT>>(
          fc_graph, cc_graph, 3);
    // COMPLETE THE TEST
    WHEN("We Agglomerate the mesh") {  
      agg->agglomerate_one_level(8, 8, 8, 0);
      THEN("We obtain the 16 fine cells divided in 4 coarse cells") {
         auto fccc = cc_graph._fc_2_cc;
         vector<TestIndexT> fc2cc_req = {1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4};
        for (auto i = 0; i != Data.nb_fc;
             i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }

      }
    }
    WHEN("We Agglomerate the mesh and we try to correct") {  
      agg->agglomerate_one_level(8, 8, 8, 1);
      THEN("Nothing changes with respect to the case without correction") {
         auto fccc = cc_graph._fc_2_cc;
         vector<TestIndexT> fc2cc_req = {1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 1, 1, 3, 3, 1, 1, 3, 3, 6, 6, 0, 0, 6, 6, 0, 0, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4, 5, 5, 7, 7, 5, 5, 7, 7, 2, 2, 4, 4, 2, 2, 4, 4};
        for (auto i = 0; i != Data.nb_fc;
             i++) {
          REQUIRE(fccc[i]==fc2cc_req[i]);
        }
      }
    }

  };
}

SCENARIO("Test the Isotropic agglomeration for small 2D cases",
         "[Isotropic]") {
  GIVEN("We load the Isotropic mesh structure") {
    DualGPy_quad_4 Data = DualGPy_quad_4();
    Seeds_Pool<TestIndexT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<TestIndexT, TestWeightT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc, 2);
    Coarse_Cell_Container<TestIndexT, TestWeightT> cc_graph(fc_graph);
    auto agg =
      make_unique<Agglomerator_Biconnected<TestIndexT, TestWeightT>>(
          fc_graph, cc_graph, 2);
    // COMPLETE THE TEST
    WHEN("We Agglomerate the mesh") {  
      agg->agglomerate_one_level(4, 4, 4, 0);
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
    WHEN("We Agglomerate the mesh and we try to correct") {  
      agg->agglomerate_one_level(4, 4, 4, 1);
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

  };
}



SCENARIO("Test the anisotropic agglomeration for small cases",
         "[Anisotropic]") {
  GIVEN("We load the anisotropic mesh structure") {
    DualGPy_aniso Data = DualGPy_aniso();
    Seeds_Pool<TestIndexT, TestIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<TestIndexT, TestWeightT, TestIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc, 3);
    Coarse_Cell_Container<TestIndexT, TestWeightT> cc_graph(fc_graph);
    shared_ptr<Agglomerator<TestIndexT, TestWeightT>> agg1 =
        make_shared<Agglomerator_Anisotropic<TestIndexT, TestWeightT>>(
            fc_graph, cc_graph, 4, 3);
    // I progress with the downcasting to get the anisotropic lines
    shared_ptr<Agglomerator_Anisotropic<TestIndexT, TestWeightT>>
        agg_dyn = dynamic_pointer_cast<
            Agglomerator_Anisotropic<TestIndexT, TestWeightT>>(agg1);
    // We setup the structures to gather the agglomeration lines
    TestIndexT nb_agglomeration_lines = 0;
    vector<deque<TestIndexT> *> agglomeration_lines;
    // We pass the structures to the level 0
    agg_dyn->_v_lines[0] = agglomeration_lines;
    agg_dyn->_v_nb_lines[0] = nb_agglomeration_lines;
    WHEN("We proceed with the agglomeration of the anisotropic lines (we gatherthem and later we agglomerate") {      
         agg_dyn->agglomerate_one_level(2, 2, 2, false);
      THEN("We have a number of agglomeration lines != 0") { REQUIRE(agg_dyn->_v_nb_lines[0]!=0);}
    }
  };
}


SCENARIO("Test the correction in 2D", "[Isotropic Correction]") {
  GIVEN("We load the Minimal Isotropic mesh structure") {
    DualGPy_minimal Data = DualGPy_minimal();
    Seeds_Pool<TestIndexT, TestIntT> seeds_pool(Data.nb_fc, Data.d_is_on_bnd);
    Dual_Graph<TestIndexT, TestWeightT, TestIntT> fc_graph(
        Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
        Data.adjMatrix_areaValues, Data.volumes, seeds_pool,
        Data.s_anisotropic_compliant_fc, 2);
    Coarse_Cell_Container<TestIndexT, TestWeightT, TestIntT> cc_graph(fc_graph);
    auto agg = make_unique<Agglomerator_Biconnected<TestIndexT, TestWeightT, TestIntT>>(
        fc_graph, cc_graph, 2);
    // COMPLETE THE TEST
    WHEN("We proceed with the Isotropic agglomeration") {
      agg->agglomerate_one_level(2, 2, 2, 1);

      THEN("No cells are left with cardinality 1") {
        for (auto i = cc_graph._cc_vec.begin(); i != cc_graph._cc_vec.end();
             i++) {
          REQUIRE(i->second->_cardinality != 1);
        }
      }
    }
  };
}
