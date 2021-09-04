
#include "CoMMA.h"

void agglomerate_one_level(long *sizes,
        // Dual graph:
                           long *adjMatrix_row_ptr,
                           long *adjMatrix_col_ind,
                           double *adjMatrix_areaValues,
                           double *volumes,

        // Indices of compliant cc
                           long *arrayOfFineAnisotropicCompliantCells,

        // boundaries
                           long *isOnFineBnd_l,
                           long *array_is_on_valley,
                           long *array_is_on_ridge,
                           long *array_is_on_corner,

        // Agglomeration argument
                           long isFirstAgglomeration_long,
                           long is_anisotropic_long,


        // Outputs
                           long *fc_to_cc,  // Out

                           long *agglomerationLines_Idx,  // In & out
                           long *agglomerationLines,  // In & out

        //Args with default value
                           long is_basic_or_triconnected,
                           long dimension,
                           long goal_card,
                           long min_card,
                           long max_card,
                           long checks_long,
                           long verbose_long) {


    //"""
    // Main function of the agglomerator.
    // It agglomerates one MG level.
    //:param is_anisotropic:  [boolean], do we want an anisotropic agglomeration (True) or an isotropic (False)?
    //:param goal_card: [int] goal cardinal of the coarse cells
    //:param min_card: [int] minimum cardinal of the coarse cells
    //:param max_card: [int] maximum cardinal of the coarse cells
    //:param checks: [boolean]
    //:param verbose: [boolean]
    //"""

    // Reminder:
    //    long sizes[10] = {nb_fc, adj_matrix_col_ind_size,
    //                      indCoarseCell,  // OUT
    //                      numberOfFineAgglomeratedCells, // OUT
    //                      is_on_valley_size, is_on_ridge_size, is_on_corner_size,
    //                      arrayOfFineAnisotropicCompliantCells_size,
    //                      agglomerationLines_Idx_size, agglomerationLines_size};

    cout << "\n\nCall of agglomerate_one_level" << endl;

    // DUAL GRAPH
    //======================================
    // number of faces
    long nb_fc = sizes[0];
    // Length of the offset vector of the CSR representation. it should be long as the number of faces
    //  augmented of 1
    long adj_matrix_row_ptr_size = nb_fc + 1;
    // Length of the esges vector of the CSR representation, representing the adjacency
    long adj_matrix_col_ind_size = sizes[1];
    // Length of the weigth of the CSR representation. In this kind of representation it is the same
    long adj_matrix_areas_size = sizes[1];
    // Initialization vector v for the dual graph structure, it is an initialization in which we copy exactly the vector passed in input to the function.
    vector<long> v_row_ptr(adjMatrix_row_ptr, adjMatrix_row_ptr + adj_matrix_row_ptr_size);
    vector<long> v_col_ind(adjMatrix_col_ind, adjMatrix_col_ind + adj_matrix_col_ind_size);
    vector<double> v_values(adjMatrix_areaValues, adjMatrix_areaValues + adj_matrix_areas_size);
    vector<double> v_volumes(volumes, volumes + nb_fc);


    // BOUNDARIES
    //======================================

    //initialization of map d_is_on_bnd.
    // We create the list of the faces on boundary
    unordered_map<long, int> d_is_on_bnd;
    for (int i = 0; i < nb_fc; i++) {
        if (isOnFineBnd_l[i] > 0) {
            d_is_on_bnd[i] = isOnFineBnd_l[i];
        }
    }

    // Initialization of sets: s_is_on_valley, s_is_on_ridge, s_is_on_corner;
    long is_on_valley_size = sizes[4];
    long is_on_ridge_size = sizes[5];
    long is_on_corner_size = sizes[6];

    unordered_set<long> s_is_on_valley, s_is_on_ridge, s_is_on_corner;
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
    long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
    unordered_set<long> s_anisotropic_compliant_fc;
    for (long i_a_c_fc = 0; i_a_c_fc < arrayOfFineAnisotropicCompliantCells_size; i_a_c_fc++) {
        s_anisotropic_compliant_fc.insert(arrayOfFineAnisotropicCompliantCells[i_a_c_fc]);
    }
    // We add the following lines because the following sets can be rebuilt in the constructor.
    // it introduce some randomness compare to the direct agglomerator call see Test_Agglomerator_basic.cpp
    s_is_on_corner.clear();
    s_is_on_ridge.clear();
    s_is_on_valley.clear();

    // DUAL GRAPH
    //======================================
    //It is built the dual graph class through the constructor. To see it look at DualGraph.hpp and DualGraph.cpp
    assert(verbose_long < USHRT_MAX);
    assert(dimension < USHRT_MAX);
    Dual_Graph fc_graph = Dual_Graph(nb_fc,
                                     v_row_ptr,
                                     v_col_ind,
                                     v_values,
                                     v_volumes,
                                     d_is_on_bnd,
                                     s_is_on_corner,
                                     s_is_on_ridge,
                                     s_is_on_valley,
                                     s_anisotropic_compliant_fc,
                                     verbose_long,
                                     dimension
    );

    bool checks = checks_long == 1;

    bool is_visu_data_stored = true;  //TODO get this via argument:
    string kind_of_agglomerator;
    if (is_basic_or_triconnected == 0) {
        kind_of_agglomerator = "basic";
    } else if (is_basic_or_triconnected == 1) {
        kind_of_agglomerator = "triconnected";
    }

    // Initialization of Agglomerator class from Agglomerator.hpp
    Agglomerator agg = Agglomerator(fc_graph,
                                    verbose_long,
                                    is_visu_data_stored,
                                    dimension = dimension,
                                    checks);

    bool is_anisotropic = is_anisotropic_long == 1;
    // To be changed if we want the visu data stored. The routine to do it is
    // in Util. 
    // In the PostProcessing folder it is possible to find the python tool to
    // parse and to draw the graph from the data saved
    if (false) {
        cout << "Storing"<< endl;
        store_agglomeration_datas(sizes,
                                  adjMatrix_row_ptr,
                                  adjMatrix_col_ind,
                                  adjMatrix_areaValues,
                                  volumes,

                                  arrayOfFineAnisotropicCompliantCells,

                                  isOnFineBnd_l,
                                  array_is_on_valley,
                                  array_is_on_ridge,
                                  array_is_on_corner,
                                  isFirstAgglomeration_long, is_anisotropic_long, fc_to_cc,
                                  agglomerationLines_Idx,
                                  agglomerationLines, dimension, goal_card, min_card, max_card, checks_long, verbose_long);
    }

    assert(goal_card < USHRT_MAX);
    assert(min_card < USHRT_MAX);
    assert(max_card < USHRT_MAX);

    // ANISOTROPIC LINES
    //======================================
    long nb_agglomeration_lines = 0;
    // We use forward_list, more efficient in memory management than a list and deque that allows an easier 
    // manipulation with respect to vectors
    forward_list<deque<long> *> agglomeration_lines;

    if (is_anisotropic && !isFirstAgglomeration_long) {
        long agglomerationLines_Idx_size = sizes[8];
        long agglomerationLines_size = sizes[9];
        // Function from Util
        convert_agglomeration_lines_arrays_to_agglomeration_lines(agglomerationLines_Idx_size,
                                                                  agglomerationLines_size,
                                                                  agglomerationLines_Idx,
                                                                  agglomerationLines,
                                                                  nb_agglomeration_lines,
                                                                  agglomeration_lines);
    }


    short goal_card_s = short(goal_card);
    short min_card_s = short(min_card);
    short max_card_s = short(max_card);


    // Compute the agglomeration:
    //======================================
    agg.agglomerate_one_level(is_anisotropic,
                              nb_agglomeration_lines, agglomeration_lines,
                              kind_of_agglomerator,
                              goal_card_s,
                              min_card_s,
                              max_card_s
    );

    sizes[2] = agg.get_nb_cc();  //indCoarseCell
    for (long i_fc = 0; i_fc < nb_fc; i_fc++) {
        fc_to_cc[i_fc] = agg.get_fc_2_cc()[i_fc];
    }

    // check if there are single parents
    // =============================================
    // Parents counter
    // To keep track
    vector<long> course(nb_fc,1);
    int counter = 0;
    // Flag to understand if there are double elements in fc_to_cc
    int flag = 0;
    // We loop over faces of the children
    for (long i_fc = 0; i_fc < nb_fc; i_fc++) {
        flag=0;
        for (long j_fc = 0; j_fc < nb_fc; j_fc++) {
             // If we have a double the flag = 1
             if (fc_to_cc[i_fc]==fc_to_cc[j_fc] && i_fc!=j_fc) {
                flag = 1;}
        }
        // If flag is still 0 we can print value of parent and child
        if (flag == 0){
           course[i_fc]=0;
           long k_fc=i_fc;
//           while (course[k_fc]==0){
//                cout << "index = " << k_fc  << endl;
//        	fc_to_cc[i_fc]=fc_to_cc[k_fc-1];
//                k_fc--;
//		}
           counter++;
           cout << "parent = " << i_fc << "child =" << fc_to_cc[i_fc]  << endl;
        }	
    }
    cout << "m. single parents = " << counter << endl;


    // get agglomeration lines:
    //======================================
    if (is_anisotropic && agg.is_agglomeration_anisotropic()) {

        int i_level = 1;
        long Agg_lines_sizes[2] = {0, 0};

        agg.get_agglo_lines(i_level,
                            Agg_lines_sizes,
                            agglomerationLines_Idx,
                            agglomerationLines);

        sizes[8] = Agg_lines_sizes[0];
        sizes[9] = Agg_lines_sizes[1];
    }
}


//PYBIND11_MODULE(example, m) {
//    m.doc() = "pybind11 example plugin"; // optional module docstring
//
//    m.def("add", &add, "A function which adds two numbers");
//}
