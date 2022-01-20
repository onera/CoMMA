#ifndef COMMA_PROJECT_COMMA_H
#define COMMA_PROJECT_COMMA_H

#include "Agglomerator.h"
#include "templateHelpers.h"
/** @brief Main function of the agglomerator, it is used as an interface
 * to build up all the agglomeration process. The result will be the definition
 * of the agglomerated cells fc2cc.  
 * */



template<typename CoMMAIndexType, typename CoMMAWeightType>
void agglomerate_one_level( // Dual graph:
                           const vector<CoMMAIndexType> adjMatrix_row_ptr,
                           const vector<CoMMAIndexType> adjMatrix_col_ind,
                           const vector<CoMMAWeightType> adjMatrix_areaValues,
                           const vector<CoMMAWeightType> volumes,

        // Indices of compliant cc
                           vector<CoMMAIndexType> &arrayOfFineAnisotropicCompliantCells,

        // boundaries
                           const vector<CoMMAIndexType> &isOnFineBnd,
                           vector<CoMMAIndexType> &array_is_on_valley,
                           vector<CoMMAIndexType> &array_is_on_ridge,
                           vector<CoMMAIndexType> &array_is_on_corner,

        // Agglomeration argument
                           long isFirstAgglomeration_long,
                           long is_anisotropic_long,


        // Outputs
                           vector<CoMMAIndexType> &fc_to_cc,  // Out

                           vector<CoMMAIndexType> &agglomerationLines_Idx,  // In & out
                           vector<CoMMAIndexType> &agglomerationLines,  // In & out

        //Args with default value
                           long is_basic_or_triconnected,
                           long dimension,
                           long goal_card,
                           long min_card,
                           long max_card,
                           long verbose_long)

{

    // GENERAL DESCRIPTION PARAMETERS
    //====================================== 
    // number of faces
    long nb_fc = static_cast<long>(adjMatrix_row_ptr.size()-1);
    // Length of the offset vector of the CSR representation. it should be long as the number of faces
    //  augmented of 1
    long adj_matrix_row_ptr_size = nb_fc + 1;
    // Length of the esges vector of the CSR representation, representing the adjacency
    long adj_matrix_col_ind_size = static_cast<long>(adjMatrix_col_ind.size());
    // Length of the weigth of the CSR representation. In this kind of representation it is the same
    long adj_matrix_areas_size = static_cast<long>(adjMatrix_col_ind.size());
    // Initialization of sets: s_is_on_valley, s_is_on_ridge, s_is_on_corner;
    long is_on_valley_size = static_cast<long>(array_is_on_valley.size());
    long is_on_ridge_size = static_cast<long>(array_is_on_ridge.size());
    long is_on_corner_size = static_cast<long>(array_is_on_corner.size());
    // Initialize anisotropic compliant
    long arrayOfFineAnisotropicCompliantCells_size = static_cast<long>(arrayOfFineAnisotropicCompliantCells.size());

    // BOUNDARIES
    //======================================
    //initialization of map d_is_on_bnd.
    // We create a dictionary of the faces on boundary
    // In particular starting from the vector we pass we store in a map
    // the key relative to the cell analysed and the relative NUMBER OF FACES on the boundary
    unordered_map<CoMMAIndexType, int> d_is_on_bnd;
    for (int i = 0; i < nb_fc; i++) {
	    if (isOnFineBnd[i] > 0){
	    	fill_value<true,int,const long>(d_is_on_bnd[i],isOnFineBnd[i]);		
    }
    }
    // initialization set
    unordered_set<CoMMAIndexType> s_is_on_valley, s_is_on_ridge, s_is_on_corner;
    // filling
    for (long i_o_v = 0; i_o_v < is_on_valley_size; i_o_v++) {
        s_is_on_valley.insert(array_is_on_valley[i_o_v]);
    }

    for (long i_o_r = 0; i_o_r < is_on_ridge_size; i_o_r++) {
        s_is_on_ridge.insert(array_is_on_ridge[i_o_r]);
    }

    for (long iOC = 0; iOC < is_on_corner_size; iOC++) {
        s_is_on_corner.insert(array_is_on_corner[iOC]);
    }

    // ANISOTROPIC COMPLIANT FC
    //======================================
    // Elements that is checked if they are anisotropic. 
    // e.g : in case of CODA software are passed all the children, and hence all the source elements of the 
    // previous agglomeration process.
    unordered_set<CoMMAIndexType> s_anisotropic_compliant_fc;
    for (long i_a_c_fc = 0; i_a_c_fc < arrayOfFineAnisotropicCompliantCells_size; i_a_c_fc++) {
        s_anisotropic_compliant_fc.insert(arrayOfFineAnisotropicCompliantCells[i_a_c_fc]);
    }

    // We add the following lines because the following sets can be rebuilt in the constructor of Dual_Graph class.
    // it introduce some randomness compare to the direct agglomerator call see Test_Agglomerator_basic.cpp
//    s_is_on_corner.clear();
    //    s_is_on_ridge.clear();
//    s_is_on_valley.clear();

    // DUAL GRAPH
    //======================================
    //It is built the dual graph class through the constructor. To see it look at DualGraph.hpp and DualGraph.cpp
    // fc = Fine Cells
    assert(verbose_long < USHRT_MAX);
    assert(dimension < USHRT_MAX);

    Dual_Graph fc_graph = Dual_Graph(nb_fc,
                                     adjMatrix_row_ptr,
                                     adjMatrix_col_ind,
                                     adjMatrix_areaValues,
                                     volumes,
                                     d_is_on_bnd,
                                     s_is_on_corner,
                                     s_is_on_ridge,
                                     s_is_on_valley,
                                     s_anisotropic_compliant_fc,
                                     verbose_long,
                                     dimension);
    
    Coarse_Cell_Graph cc_graph = Coarse_Cell_Graph(fc_graph); 
    bool is_visu_data_stored = true;  //TODO get this via argument:
 
   // AGGLOMERATION ANISOTROPIC FOLLOWED BY ISOTROPIC AGGLOMERATION
    // @todo maybe re-refactor the class agglomerator to allow the implicit upcast like the biconnected case
    // The agglomerator anisotropic is not called with the  implicit upcasting pointing because of the initialization of
    // the anisotropic lines.
    // for more information look at: https://stackoverflow.com/questions/19682402/initialize-child-object-on-a-pointer-to-parent
    // About constructors when upcasting : https://www.reddit.com/r/learnprogramming/comments/1wopf6/java_which_constructor_is_called_when_upcasting/
    if (is_anisotropic_long){
    Agglomerator* agg1 = new Agglomerator_Anisotropic(fc_graph,
                                    cc_graph,
        	                    verbose_long,
                                    is_visu_data_stored,
                                    dimension = dimension);
   long nb_agglomeration_lines = 0;
   forward_list<deque<long> *> agglomeration_lines;
    // @todo add exception if it is not the first agglomeration (so if we are at a further level)
    // Initialization of nb_agglo_lines in dependency if we are at the first agglomeration or not
    // in this case we are. in the todo we have to treat the case in which we are not
   Agglomerator_Anisotropic* agg_dyn = dynamic_cast<Agglomerator_Anisotropic*>(agg1);
   // cout << "Agg aniso";  
    agg_dyn->_v_nb_lines[0]= nb_agglomeration_lines;
    agg_dyn->_v_lines[0]= agglomeration_lines;
    agg_dyn->agglomerate_one_level(min_card,goal_card,max_card,-1);  
    int i_level = 1;
    agg_dyn->get_agglo_lines(i_level,
                            agglomerationLines_Idx,
                            agglomerationLines);;  
    // Free the pointer

    delete(agg1); 
    }
  unique_ptr<Agglomerator> agg(new Agglomerator_Biconnected(fc_graph,
		                    cc_graph,
                                    verbose_long,
                                    is_visu_data_stored,
                                    dimension = dimension)); 

 
    agg->agglomerate_one_level(min_card,goal_card,max_card,-1);
    // FILLING FC TO CC (it is a property of the cc_graph but retrived through an helper of the agglomerator)
    for (long i_fc = 0; i_fc < nb_fc; i_fc++) {
        fc_to_cc[i_fc] = agg->get_fc_2_cc()[i_fc];
    } 
}
#endif
