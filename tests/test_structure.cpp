#define CATCH_CONFIG_MAIN 
#include "catch2/catch.hpp"
#include "input/DualGPy.h"
// I input with DualGPy configuration the configuration you can
// find in the README of the library


//TEST_CASE("Structure test","[structure]")
SCENARIO("Test of a structure", "[structure]") {
//{
 GIVEN("A simple graph, and we build the Dual Graph") {
    DualGPy Data = DualGPy();
    // Construction of the Dual Graph element
    Dual_Graph fc_graph = Dual_Graph(Data.nb_fc,
                                     Data.adjMatrix_row_ptr,
                                     Data.adjMatrix_col_ind,
                                     Data.adjMatrix_areaValues,
                                     Data.volumes,
                                     Data.d_is_on_bnd,
                                     Data.s_is_on_corner,
                                     Data.s_is_on_ridge,
                                     Data.s_is_on_valley,
                                     Data.s_anisotropic_compliant_fc,
                                     0,
                                     2
    );   
    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph(fc_graph);
    // Check the effective length
    WHEN("We try to access to the member variables") {
    class test : public Agglomerator_Biconnected{
	    public:
		 test(Dual_Graph &graph,
			Coarse_Cell_Graph &cc_graph,
                        int dimension) : Agglomerator_Biconnected(graph,cc_graph,dimension){};

		    short test_variable(){
                       return(_threshold_card); 
		    };
    
    };
    THEN(
          "We see that the agglomeration is not set, hence set to -1") {
    test* agg = new test(fc_graph,
		         cc_graph,
                          2);
    short testing = agg->test_variable();
    REQUIRE(testing == -1);
    }
    }
   WHEN("We try to access to Define the cardinality") {
    class test : public Agglomerator_Biconnected{
	    public:
		 test(Dual_Graph &graph,
			Coarse_Cell_Graph &cc_graph,
                        int dimension) : Agglomerator_Biconnected(graph,cc_graph,dimension){};

		    short thres(){
                       return(_threshold_card); 
		    };
        	    short max(){
                       return(_max_card); 
		    };
        	    short min(){
                       return(_min_card); 
		    };
    };
    THEN(
          "We see that the cardinality passes from -1 to 2") {
    test* agg = new test(fc_graph,
		         cc_graph,
                          2);
   
    REQUIRE(agg->thres() == -1);
    REQUIRE(agg->max() == -1);
    REQUIRE(agg->min() == -1);
    agg->set_agglomeration_parameter(2,2,2); 
    REQUIRE(agg->thres() == 2);
    REQUIRE(agg->max() == 2);
    REQUIRE(agg->min() == 2);
    }
}
}
}

