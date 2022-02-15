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
SCENARIO("Test of the seed pool", "[Seed Pool]") {
  GIVEN("The DualGPy configuration used for the example") {
    DualGPy Data = DualGPy();
    WHEN("We build the seed pool") {
      Seeds_Pool *sp =
          new Seeds_Pool(Data.nb_fc, Data.d_is_on_bnd, Data.s_is_on_corner,
                         Data.s_is_on_ridge, Data.s_is_on_valley, 2);
      THEN(
          "We see that the sequence of seed choice is the one of Diskin "
          "Mavriplis") {
        long seed = sp->choose_new_seed(Data.a_is_fc_agglomerated);
        REQUIRE(seed == 20);
        seed = sp->choose_new_seed(Data.a_is_fc_agglomerated);
        REQUIRE(seed == 17);
        int b_v = sp->boundary_value(20);
        REQUIRE(b_v == 2);
      }
      WHEN("We build the seed pool to access the list of possible seed") {
        class test : public Seeds_Pool {
         public:
          test(int number_of_cells, unordered_map<long, int> &d_is_on_bnd,
               unordered_set<long> is_on_corner,
               unordered_set<long> is_on_ridge,
               unordered_set<long> is_on_valley, int init_bnd_level)
              : Seeds_Pool(number_of_cells, d_is_on_bnd, is_on_corner,
                           is_on_ridge, is_on_valley, init_bnd_level) {};

          vector<Stack<long>> list() {
            return (_l_of_seeds);
          };
        };

        test *sp = new test(Data.nb_fc, Data.d_is_on_bnd, Data.s_is_on_corner,
                            Data.s_is_on_ridge, Data.s_is_on_valley, 2);
        THEN(
            "We see that the sequence of seed choice is the one of Diskin "
            "Mavriplis") {
          long seed = sp->choose_new_seed(Data.a_is_fc_agglomerated);
          REQUIRE(seed == 20);
          list<long> new_seeds = {5};
          sp->update(new_seeds, false);
          vector<Stack<long>> new_s_list = sp->list();
          REQUIRE(new_s_list[0].top() == 5);
        }
      };
    }
  }
}
