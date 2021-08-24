
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

    long nb_fc = sizes[0];
    long adj_matrix_row_ptr_size = nb_fc + 1;
    long adj_matrix_col_ind_size = sizes[1];
    long adj_matrix_areas_size = sizes[1];

    vector<long> v_row_ptr(adjMatrix_row_ptr, adjMatrix_row_ptr + adj_matrix_row_ptr_size);
    vector<long> v_col_ind(adjMatrix_col_ind, adjMatrix_col_ind + adj_matrix_col_ind_size);
    vector<double> v_values(adjMatrix_areaValues, adjMatrix_areaValues + adj_matrix_areas_size);
    vector<double> v_volumes(volumes, volumes + nb_fc);


    // BOUNDARIES
    //======================================

    //initialization of map d_is_on_bnd
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

    Agglomerator agg = Agglomerator(fc_graph,
                                    verbose_long,
                                    is_visu_data_stored,
                                    dimension = dimension,
                                    checks);

    bool is_anisotropic = is_anisotropic_long == 1;
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
    forward_list<deque<long> *> agglomeration_lines;

    if (is_anisotropic && !isFirstAgglomeration_long) {
        long agglomerationLines_Idx_size = sizes[8];
        long agglomerationLines_size = sizes[9];

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

//
//    long numberOfFineAnisotropicCompliantCells = arrayOfFineAnisotropicCompliantCells_size;
//
//    // Keep track of agglomerated fine cell
//    long indCoarseCell = 0;
//    // Rmk: sizes[2] ==indCoarseCell
//
//    long numberOfFineAgglomeratedCells = sizes[3];  //out
//    numberOfFineAgglomeratedCells = 0;  // number of fine (already) agglomerated cells
//
//    bool *isFineCellAgglomerated = new bool[nb_fc];
//    for (long i = 0; i < nb_fc; i++) {
//        isFineCellAgglomerated[i] = false;
//    }
//// TODO On pourrait ne l'allouer qu'une seule fois!
//    // fineAgglomerationLines = None
//    // fineAgglomerationLines_for_visu = None
//
//    //fineAgglomerationLines_array_Idx = None
//    //fineAgglomerationLines_array = None
//
//    long *fineAgglomerationLines_for_visu_array_Idx = NULL;
//    long *fineAgglomerationLines_for_visu_array = NULL;
//
////    long* arrayOfCoarseAnisotropicCompliantCells = NULL;
////    long numberOfAnisotropicLinesPOne_size = 0;
////    long agglomerationLines_size = 0;
////    bool isAnisotropicLines = false;
//    bool isAnisotropicLines = is_anisotropic && !isFirstAgglomeration;
//    if (is_anisotropic) {
//        if (isFirstAgglomeration) {
////            numberOfAnisotropicLinesPOne_size = agglomerationLines_Idx_size;
//            agglomerationLines_size = agglomerationLines_size;
//            isAnisotropicLines = computeAnisotropicLine(sizes,
//                                                        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues,
//                                                        arrayOfFineAnisotropicCompliantCells,
//                                                        agglomerationLines_Idx,
//                                                        agglomerationLines,
//                                                        verbose_long > 0);
//
////            numberOfAnisotropicLinesPOne_size = sizes[3];  // number of agglomeration lines +1
////            agglomerationLines_size = sizes[4];
////            numberOfAnisotropicLinesPOne_size = sizes[8];
//            agglomerationLines_size = sizes[9];
////            cout << "numberOfAnisotropicLinesPOne_size " << numberOfAnisotropicLinesPOne_size << endl;
////            cout << "agglomerationLines_size " << agglomerationLines_size << endl;
////          Pas de resize, c'est pas possible avec un tableau
////            agglomerationLines_Idx.resize((numberOfAnisotropicLinesPOne_size,), refcheck = False)
////            agglomerationLines.resize((agglomerationLines_size,), refcheck = False)
//
//            // For Visu only:
////            fineAgglomerationLines_for_visu_array_Idx = np.copy(agglomerationLines_Idx);
////            fineAgglomerationLines_for_visu_array = np.copy(agglomerationLines);
//        }
//        if (isAnisotropicLines) {
//
////            cout << "agglomerationLines_size " << agglomerationLines_size << endl;
////            long arrayOfCoarseAnisotropicCompliantCells_size = agglomerationLines_size; //np.shape(arrayOfFineAnisotropicCompliantCells)[0]
////            arrayOfCoarseAnisotropicCompliantCells = np.zeros((arrayOfCoarseAnisotropicCompliantCells_size,), dtype = int)
//            agglomerationLines_Idx_size = sizes[8];
////            cout << "sizes[0]= " << sizes[0] << endl;
////            cout << "sizes[1]= " << sizes[1] << endl;
////            cout << "sizes[2]= " << sizes[2] << endl;
////            cout << "sizes[3]= " << sizes[3] << endl;
////            cout << "sizes[4]= " << sizes[4] << endl;
////            cout << "sizes[5]= " << sizes[5] << endl;
////            cout << "sizes[6]= " << sizes[6] << endl;
////            cout << "sizes[7]= " << sizes[7] << endl;
////            cout << "sizes[8]= " << sizes[8] << endl;
////            cout << "sizes[9]= " << sizes[9] << endl;
//
//            long sizes_aniso[5] = {agglomerationLines_Idx_size, nb_fc, numberOfFineAgglomeratedCells, indCoarseCell, -1};
//
//            agglomerate_Anisotropic_One_Level_without_list_lines(sizes_aniso,
//                                                                 agglomerationLines_Idx,
//                                                                 agglomerationLines,
//                                                                 fc_to_cc, isFineCellAgglomerated, arrayOfFineAnisotropicCompliantCells);//arrayOfCoarseAnisotropicCompliantCells);
//
////            agglomerationLines_Idx_size = sizes_aniso[0];
////            nb_fc = sizes_aniso[1];
////            numberOfFineAgglomeratedCells = sizes_aniso[2];
////            indCoarseCell = sizes_aniso[3];
////            long arrayOfCoarseAnisotropicCompliantCells_size = sizes_aniso[4];
////            long nb_fc = sizes[0];
////            long adj_matrix_row_ptr_size = nb_fc + 1;
////            long adj_matrix_col_ind_size = sizes[1];
////            long adj_matrix_areas_size = sizes[1];
////
////            // Rmk: sizes[2] ==indCoarseCell
////            long numberOfFineAgglomeratedCells = sizes[3];
////            long is_on_valley_size = sizes[4];
////            long is_on_ridge_size = sizes[5];
////            long is_on_corner_size = sizes[6];
////            long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
////            long agglomerationLines_Idx_size = sizes[8];
////            long agglomerationLines_size = sizes[9];
//            sizes[2] = sizes_aniso[3];
//            sizes[3] = sizes_aniso[2];
//            sizes[7] = sizes_aniso[4];
//            sizes[8] = sizes_aniso[0];
////            sizes[9] = sizes[8] - 1;
//            sizes[9] = agglomerationLines_Idx[sizes[8] - 1];
//
////            if (agglomerationLines_Idx_size>0){
////
////            agglomerationLines_Idx.resize((agglomerationLines_Idx_size,), refcheck = False)
////            agglomerationLines.resize((agglomerationLines_Idx[agglomerationLines_Idx_size - 1],), refcheck = False)
//
////            if
////                arrayOfCoarseAnisotropicCompliantCells
////                        is
////                not None:
////            arrayOfCoarseAnisotropicCompliantCells.resize((arrayOfCoarseAnisotropicCompliantCells_size,), refcheck = False)
//        }
//    }
//
//    // Rmk: sizes[2] ==indCoarseCell
////    long numberOfFineAgglomeratedCells = sizes[3];
////    long is_on_valley_size = sizes[4];
////    long is_on_ridge_size = sizes[5];
////    long is_on_corner_size = sizes[6];
////    long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
////    long agglomerationLines_Idx_size = s    izes[8];
////    long agglomerationLines_size = sizes[9];
//
////    sizes[2] = sizes_iso[2];  //indCoarseCell
////    sizes[3] = sizes_iso[3];  //numberOfFineAgglomeratedCells
////    sizes[7] = arrayOfFineAnisotropicCompliantCells_size;
////    sizes[8] = arrayOfFineAnisotropicCompliantCells_size;
////    sizes[9] = arrayOfFineAnisotropicCompliantCells_size;
////    return indCoarseCell, fineAgglomerationLines_for_visu_array_Idx, fineAgglomerationLines_for_visu_array, agglomerationLines_Idx, \
////           agglomerationLines,  \
////           arrayOfCoarseAnisotropicCompliantCells
//    delete[] isFineCellAgglomerated;
    sizes[2] = agg.get_nb_cc();  //indCoarseCell
//    sizes[3] = agg.;  //numberOfFineAgglomeratedCells
////    sizes[7] = arrayOfFineAnisotropicCompliantCells_size;
////    sizes[8] = arrayOfFineAnisotropicCompliantCells_size;
////    sizes[9] = arrayOfFineAnisotropicCompliantCells_size;
    for (long i_fc = 0; i_fc < nb_fc; i_fc++) {
        fc_to_cc[i_fc] = agg.get_fc_2_cc()[i_fc];
    }

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
//    delete agg;
}
