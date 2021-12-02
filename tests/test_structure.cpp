#define CATCH_CONFIG_MAIN 
#include "catch2/catch.hpp"
#include "input/DualGPy.h"
// I input with DualGPy configuration the configuration you can
// find in the README of the library


TEST_CASE("Structure test","[structure]")
{
    // DualGPy class constructor loads the configuration
    // with the graph and all the variables required
    // TODO: maybe change to pointer for omogenity
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
    // Check the effective length
    REQUIRE (Data.adj_matrix_row_ptr_size == (Data.nb_fc + 1));
    REQUIRE(Data.nb_fc == ((Data.adjMatrix_row_ptr).size()-1));

    // To test protected variables I use a child class. This is a trick to access. 
    class test : public Agglomerator_Anisotropic{
	    public:
		 test(Dual_Graph &graph,
                              int verbose,
                              bool is_visu_data_stored,
                              int dimension) : Agglomerator_Anisotropic(graph,verbose,is_visu_data_stored,dimension){};

		    bool test_variable(){
                       return(_is_visu_data_stored); 
		    };
    };
    test* agg = new test(fc_graph,
                                    0,
                                    Data.is_visu_data_stored,
                                    2);
    bool testing = agg->test_variable();
    vector<long> gg = agg->get_fc_2_cc();
    // Check if the structure is correct: Have I really changed the testing variable that is in the Father class Agglomerator by setting it true (by default is false) in the child class??
    REQUIRE(testing == true);
}


TEST_CASE("Agglomerator Isotropic","[Isotropic]")
{
    // DualGPy class constructor loads the configuration
    // with the graph and all the variables required
    // TODO: maybe change to pointer for omogenity
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
    // Check the effective length
    REQUIRE (Data.adj_matrix_row_ptr_size == (Data.nb_fc + 1));
    REQUIRE(Data.nb_fc == ((Data.adjMatrix_row_ptr).size()-1));
    Agglomerator* test = new Agglomerator_Biconnected(fc_graph,0,Data.is_visu_data_stored,2);
    test->agglomerate_one_level(2,2,2);
}
