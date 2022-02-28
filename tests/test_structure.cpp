#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "input/DualGPy.h"
// I input with DualGPy configuration the configuration you can
// find in the README of the library

// TEST_CASE("Structure test","[structure]")
SCENARIO("Test of a structure", "[structure]") {
  //{
  GIVEN("A simple graph, and we build the Dual Graph") {
    DualGPy Data = DualGPy();
    // Construction of the Dual Graph element
    Seeds_Pool seeds_pool= Seeds_Pool(Data.nb_fc,Data.d_is_on_bnd,
                   Data.s_is_on_corner, Data.s_is_on_ridge, Data.s_is_on_valley,2);
    Dual_Graph fc_graph =
        Dual_Graph(Data.nb_fc, Data.adjMatrix_row_ptr, Data.adjMatrix_col_ind,
                   Data.adjMatrix_areaValues, Data.volumes,seeds_pool,Data.s_anisotropic_compliant_fc, 0, 2);
    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph(fc_graph);
    // Check the effective length
    WHEN("We try to access to the member variables") {
      class test : public Agglomerator_Biconnected {
       public:
        test(Dual_Graph &graph, Coarse_Cell_Graph &cc_graph, int dimension)
            : Agglomerator_Biconnected(graph, cc_graph, dimension) {};

        short test_variable() {
          return (_threshold_card);
        };
      };
      THEN("We see that the agglomeration is not set, hence set to -1") {
        test *agg = new test(fc_graph, cc_graph, 2);
        short testing = agg->test_variable();
        REQUIRE(testing == -1);
      }
    }
    WHEN("We try to access to Define the cardinality") {
      class test : public Agglomerator_Biconnected {
       public:
        test(Dual_Graph &graph, Coarse_Cell_Graph &cc_graph, int dimension)
            : Agglomerator_Biconnected(graph, cc_graph, dimension) {};

        short thres() {
          return (_threshold_card);
        };
        short max() {
          return (_max_card);
        };
        short min() {
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
SCENARIO("Test of the stack", "[stack]") {
  GIVEN("A simple stack of long type") {
    Stack<long> st_long;
    WHEN("We push a certain number of elements") {
      st_long.push(0);
      st_long.push(1);
      st_long.push(2);
      THEN(
          "We can check that the operation on the stack are correctly "
          "realized") {

        REQUIRE(st_long.top() == 2);
        long top = st_long.pop();
        REQUIRE(top == 2);
        REQUIRE(st_long.top() == 1);
        top = st_long.pop();
        REQUIRE(top == 1);
        REQUIRE(st_long.top() == 0);
        top = st_long.pop();
        REQUIRE(top == 0);
        REQUIRE(st_long.top() == -1);
      }
    };
  }
}
SCENARIO("Test of the tree", "[Tree]") {
  GIVEN("The node") {
    shared_ptr<node> A(new node(2,1)); 
    shared_ptr<Tree> Albero(new Tree(A));
    WHEN("We add something to the tree") {
      Albero->insertSon(2,1,1,1);
      THEN(
          "The tree has a child") {
           Albero->insertSon(2,3,1,1);
           Albero->insertSon(2,4,1,1);
           Albero->insertSon(4,2,1,0);
           Albero->print();
           REQUIRE(Albero->_root->_sonc==3);
           Albero->deleteNode(4);
           cout<< "deletion 4 performed" << endl;
           Albero->print();
           REQUIRE(Albero->_root->_sonc==2);
      }
     };
    }
}
SCENARIO("Subgraph", "[Subgraph]") {
  GIVEN("We have the CSR representation") {
    vector<CoMMAIndexT> adjMatrix_row_ptr ={0,2,4,7,9,10};
    vector<CoMMAIndexT> adjMatrix_col_ind ={1,3,0,2,1,3,4,0,2,2}; 
    vector<CoMMAWeightT> adjMatrix_areaValues ={1,1,1,1,1,1,1,1,1,1};
    vector<CoMMAWeightT> volumes ={1,1,1,1,1};
    WHEN("We build the graph") {
       shared_ptr<Subgraph> Marion(new Subgraph(5, adjMatrix_row_ptr, adjMatrix_col_ind,
                   adjMatrix_areaValues, volumes));
       Marion->_mapping_l_to_g = {20,30,40,50,60};
      THEN(
          "We remove a node") {
	      Marion->remove_node(3);
	      for (auto &elem:Marion->_m_CRS_Row_Ptr){
		      cout<<"row_ptr"<<elem<<endl;
	      }
	      for (auto &elem:Marion->_m_CRS_Col_Ind){
		      cout<<"col_ind"<<elem<<endl;
	      }
	      Marion->remove_node(0);
	      for (auto &elem:Marion->_m_CRS_Row_Ptr){
		      cout<<"row_ptr"<<elem<<endl;
	      }
	      for (auto &elem:Marion->_m_CRS_Col_Ind){
		      cout<<"col_ind"<<elem<<endl;
	      }
	      }
       THEN(
          "We add a node") {
              vector<long> v_neigh = {30,20};
              long i_fc = 10;
              vector<double> weight = {1,1};
	      Marion->insert_node(v_neigh,i_fc,1,weight);
	      for (auto &elem:Marion->_m_CRS_Row_Ptr){
		      cout<<"row_ptr"<<elem<<endl;
	      }
	      for (auto &elem:Marion->_m_CRS_Col_Ind){
		      cout<<"col_ind"<<elem<<endl;
	      }
	      }
      }
     };
}
