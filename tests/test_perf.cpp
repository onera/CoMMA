#include "input/DualGPy.h"

int main(int argv, char** argc) {

    DualGPy Data = DualGPy();
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

    // To test protected variables I use a child class. This is a trick to access. 
    class test : public Agglomerator_Anisotropic{
	    public:
		 test(Dual_Graph &graph,
			Coarse_Cell_Graph &cc_graph,
                        int verbose,
                        bool is_visu_data_stored,
                        int dimension) : Agglomerator_Anisotropic(graph,cc_graph,verbose,is_visu_data_stored,dimension){};

		    bool test_variable(){
                       return(_is_visu_data_stored); 
		    };
                   void test_fill(long &nb_aniso_agglo_lines,forward_list<deque<long> *> &anisotropic_lines){
                       set_agglo_lines(nb_aniso_agglo_lines,anisotropic_lines); 
		    };

    };

    test* agg = new test(fc_graph,
		         cc_graph,
                                    0,
                                    Data.is_visu_data_stored,
                                    2);
 
 
    bool testing = agg->test_variable();
     vector<long> gg = agg->get_fc_2_cc(); 
     long nb_agglomeration_lines = 0;
    forward_list<deque<long> *> agglomeration_lines;
//    // TODO: add exception if it is not the first agglomeration (so if we are at a further level)
    agg->test_fill(nb_agglomeration_lines,agglomeration_lines);
    agg->agglomerate_one_level(2,2,2,-1);
     Agglomerator* test = new Agglomerator_Biconnected(fc_graph,cc_graph,0,Data.is_visu_data_stored,2);
    test->agglomerate_one_level(2,2,2,-1); 
    // Check if the structure is correct: Have I really changed the testing variable that is in the Father class Agglomerator by setting it true (by default is false) in the child class??
}
