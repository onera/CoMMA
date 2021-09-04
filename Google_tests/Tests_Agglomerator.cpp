#include "gtest/gtest.h"

<<<<<<< HEAD
//#include "../CoMMA_lib/Agglomerator_Isotropic.h"
=======
#define protected public
>>>>>>> Alberto_fix
#include "../CoMMA_v1/Agglomerator_anisotropic.h"
#include "../CoMMA_lib/Agglomerator.h"

#include "MGridGen_Dual_Graph.h"
#include "MGridGen_ext_Dual_Graph.h"
#include "MGridGen_ext_v2_Dual_Graph.h"
#include "Nine_squares_3x3_Dual_Graph.h"
#include "Box_1_prism_Dual_Graph.h"
#include "Box_5x5x5_Aniso_Dual_Graph.h"
#include "Box_5x5x5_Aniso_lvl_2_Dual_Graph.h"
#include "Box_5x5x5_iso_and_Aniso_Dual_Graph.h"


TEST_F(MGridGen_Dual_Graph, Agglomerator_Constructor) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = true;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);
    ASSERT_TRUE(agg.__is_visu_data_stored);
    ASSERT_EQ(0, agg.__verbose);
    ASSERT_EQ(2, agg.__dimension);
    ASSERT_TRUE(agg.__checks);
    ASSERT_FALSE(agg.__is_anisotropic);
}

TEST_F(Box_1_prism_Dual_Graph, correction_split_too_big_cc_in_two_Box_1_Prism) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = false;
    int dimension = 3;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short goal_card = -1;
    short min_card = -1;
    short max_card = -1;
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> aniso_agglo_lines = {};
    vector<long> debug_only_fc_to_cc = {53, 53, 53, 24, 24, 24, 43, 47, 47, 4, 4, 4, 19, 19, 19, 9, 12,
                                        12, 6, 9, 9, 2, 2, 2, 26, 26, 33, 13, 13, 20, 16, 44, 44, 1,
                                        16, 16, 25, 25, 25, 12, 12, 48, 9, 9, 45, 2, 2, 2, 19, 26, 26,
                                        13, 13, 13, 6, 6, 6, 1, 1, 1, 33, 33, 33, 45, 48, 48, 44, 44,
                                        45, 16, 11, 11, 42, 61, 61, 68, 68, 42, 38, 38, 38, 10, 10, 10, 32,
                                        32, 32, 41, 41, 36, 15, 41, 41, 5, 5, 15, 32, 32, 32, 36, 36, 36,
                                        38, 38, 38, 10, 15, 15, 29, 29, 35, 49, 49, 49, 22, 22, 49, 5, 5,
                                        5, 58, 58, 59, 40, 73, 73, 34, 34, 40, 30, 27, 34, 73, 57, 57, 67,
                                        67, 73, 64, 64, 67, 31, 31, 31, 35, 35, 35, 68, 68, 68, 51, 51, 51,
                                        8, 8, 8, 73, 58, 58, 67, 73, 73, 66, 66, 67, 27, 27, 27, 60, 60,
                                        60, 69, 69, 69, 50, 37, 37, 14, 14, 50, 59, 59, 59, 40, 40, 59, 37,
                                        37, 40, 30, 30, 30, 54, 54, 55, 71, 52, 52, 46, 47, 71, 17, 17, 46,
                                        56, 56, 60, 74, 74, 56, 50, 66, 66, 21, 21, 50, 55, 55, 57, 72, 72,
                                        72, 64, 64, 64, 17, 31, 31, 53, 53, 57, 24, 24, 72, 28, 28, 28, 4,
                                        4, 28, 52, 54, 54, 18, 18, 52, 43, 43, 47, 3, 3, 43, 63, 63, 55,
                                        71, 65, 74, 46, 70, 71, 17, 23, 46, 25, 25, 25, 7, 18, 18, 7, 7,
                                        7, 3, 3, 3, 62, 56, 56, 65, 74, 74, 70, 70, 71, 23, 21, 21, 65,
                                        63, 63, 48, 48, 65, 51, 45, 70, 11, 11, 23, 62, 62, 62, 68, 65, 74,
                                        51, 70, 70, 8, 23, 23, 29, 29, 29, 20, 20, 20, 0, 22, 22, 0, 0,
                                        0, 61, 61, 61, 69, 42, 42, 39, 39, 39, 14, 14, 14};
    //    np.array([0, 0, 0, 0, 0, 0])]
    agg.agglomerate_one_level(is_anisotropic,
                              nb_aniso_agglo_lines,
                              aniso_agglo_lines,
                              kind_of_agglomerator,
                              goal_card,
                              min_card,
                              max_card,
                              debug_only_fc_to_cc);
    ASSERT_EQ(debug_only_fc_to_cc, agg.__cc_graphs->_fc_2_cc);
    agg._correction_split_too_big_cc_in_two();
//    cout << endl;
//    for (auto i:agg.get_fc_2_cc()) {
//        cout << i << ", ";
//    }
//    cout << endl;
    vector<long> ref_fc_2_cc = {53, 53, 53, 24, 24, 24, 43, 47, 47, 4, 4, 4, 19, 19, 19, 9, 12, 12, 6, 9, 9, 2, 2, 2, 26, 26, 33, 13, 13, 20, 16, 44, 44, 1, 16, 16, 25, 25, 25, 12, 12, 48, 9, 9, 45,
                                2, 2, 2, 19, 26, 26, 13, 13, 13, 6, 6, 6, 1, 1, 1, 33, 33, 33, 45, 48, 48, 44, 44, 45, 16, 11, 11, 42, 61, 61, 68, 68, 42, 38, 38, 38, 10, 10, 10, 32, 32, 32, 41, 41,
                                36, 15, 41, 41, 5, 5, 15, 32, 32, 32, 36, 36, 36, 38, 38, 38, 10, 15, 15, 29, 29, 35, 49, 49, 49, 22, 22, 49, 5, 5, 5, 58, 58, 59, 40, 73, 73, 34, 34, 40, 30, 27, 34,
                                73, 57, 57, 67, 67, 73, 64, 64, 67, 31, 31, 31, 35, 35, 35, 68, 68, 68, 51, 51, 51, 8, 8, 8, 73, 58, 58, 67, 73, 73, 66, 66, 67, 27, 27, 27, 60, 60, 60, 69, 69, 69, 50,
                                37, 37, 14, 14, 50, 59, 59, 59, 40, 40, 59, 37, 37, 40, 30, 30, 30, 54, 54, 55, 71, 52, 52, 46, 47, 71, 17, 17, 46, 56, 56, 60, 74, 74, 56, 50, 66, 66, 21, 21, 50, 55,
                                55, 57, 72, 72, 72, 64, 64, 64, 17, 31, 31, 53, 53, 57, 24, 24, 72, 28, 28, 28, 4, 4, 28, 52, 54, 54, 18, 18, 52, 43, 43, 47, 3, 3, 43, 63, 63, 55, 71, 65, 74, 46, 70,
                                71, 17, 23, 46, 25, 25, 25, 7, 18, 18, 7, 7, 7, 3, 3, 3, 62, 56, 56, 65, 74, 74, 70, 70, 71, 23, 21, 21, 65, 63, 63, 48, 48, 65, 51, 45, 70, 11, 11, 23, 62, 62, 62, 68,
                                65, 74, 51, 70, 70, 8, 23, 23, 29, 29, 29, 20, 20, 20, 0, 22, 22, 0, 0, 0, 61, 61, 61, 69, 42, 42, 39, 39, 39, 14, 14, 14};
    ASSERT_EQ(ref_fc_2_cc, agg.__cc_graphs->_fc_2_cc);
}

TEST_F(MGridGen_ext_v2_Dual_Graph, correction_split_too_big_cc_in_two_MGridGen_ext_v2_MG_1_level) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = false;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short goal_card = -1;
    short min_card = -1;
    short max_card = -1;
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> aniso_agglo_lines = {};
    vector<long> debug_only_fc_to_cc = {0, 0, 0, 0, 0, 4, 1, 1, 3, 5, 5, 4, 4, 4, 2, 3, 3, 5, 5, 2, 2, 2, 2, 5, 5, 1};
    //    np.array([0, 0, 0, 0, 0, 0])]
    agg.agglomerate_one_level(is_anisotropic,
                              nb_aniso_agglo_lines,
                              aniso_agglo_lines,
                              kind_of_agglomerator,
                              goal_card,
                              min_card,
                              max_card,
                              debug_only_fc_to_cc);
    ASSERT_EQ(debug_only_fc_to_cc, agg.__cc_graphs->_fc_2_cc);
    ASSERT_TRUE((*agg.__cc_graphs).check_data_consistency_and_connectivity());

    unordered_set<long> s_fc = (*(*agg.__cc_graphs)._d_isotropic_cc[5]).get_s_fc();
    vector<long> v_fc;
    for (const long i_fc :s_fc) {
        v_fc.push_back(i_fc);
    }
    long seed = -1;
    unordered_map<long, queue<long> *> dict_ConnectivityTree = (*g).find_seed_via_frontal_method(seed, v_fc);

    EXPECT_EQ(18, seed);   // correct answer 1, 3, 6
    unordered_map<long, queue<long> *> ref_max_dict;
    ref_max_dict[9] = new queue<long>({10, 17, 18, 23, 24});
    ref_max_dict[10] = new queue<long>({9, 23, 24});
    ref_max_dict[17] = new queue<long>({9, 18, 24});
    ref_max_dict[18] = new queue<long>({9, 17, 23});
    ref_max_dict[23] = new queue<long>({9, 10, 18});
    ref_max_dict[24] = new queue<long>({9, 10, 17});

    for (auto i : dict_ConnectivityTree) {

//        cout<<i.first<<"\t"<< i.second<<"\t"<< (*i.second).size()<<"\t"<<(*dict_ConnectivityTree[i.first]).size()<<endl;
//        print_queue(*dict_ConnectivityTree[i.first]);
        ASSERT_EQ(*ref_max_dict[i.first], *dict_ConnectivityTree[i.first]);
    }
    agg._correction_split_too_big_cc_in_two();
    vector<long> ref_fc_2_cc = {0, 0, 0, 0, 0, 4, 1, 1, 3, 6, 5, 4, 4, 4, 2, 3, 3, 6, 6, 2, 2, 2, 2, 5, 5, 1};
    ASSERT_EQ(ref_fc_2_cc, agg.__cc_graphs->_fc_2_cc);
    ASSERT_TRUE((*agg.__cc_graphs).check_data_consistency_and_connectivity());
}

TEST_F(MGridGen_ext_v2_Dual_Graph, correction_split_too_big_cc_in_two_MGridGen_ext_v2_MG_1_level_bis) {


    unsigned short int verbose = 0;
    bool is_visu_data_stored = false;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    bool is_anisotropic = false;
    unsigned short min_card = -1;
    unsigned short goal_card = -1;
    unsigned short max_card = -1;
    string kind_of_agglomerator = "basic";

    agg._set_agglomeration_parameter(is_anisotropic, kind_of_agglomerator, goal_card, min_card, max_card);
    agg.__cc_graphs = new Coarse_Cell_Graph(agg.__fc_graphs);

    //    creer la meme agglo avec la creation des cc a la main.
//    Je pense que c est un truc pas bien initialise.
    vector<long> debug_only_fc_to_cc = {0, 0, 0, 0, 0,
                                        4, 1, 1, 3, 5,
                                        5, 4, 4, 4, 2,
                                        3, 3, 5, 5, 2, 2, 2, 2, 5, 5, 1};

    unordered_set<long> s_fc = {0, 1, 2, 3, 4};
    (*agg.__cc_graphs).cc_create_a_cc(s_fc);

    s_fc = {6, 7, 25};
    (*agg.__cc_graphs).cc_create_a_cc(s_fc);

    s_fc = {14, 19, 20, 21, 22};
    (*agg.__cc_graphs).cc_create_a_cc(s_fc);

    s_fc = {8, 15, 16};
    (*agg.__cc_graphs).cc_create_a_cc(s_fc);

    s_fc = {5, 11, 12, 13};
    (*agg.__cc_graphs).cc_create_a_cc(s_fc);

    s_fc = {9, 10, 17, 18, 23, 24};
    (*agg.__cc_graphs).cc_create_a_cc(s_fc);

    (*agg.__cc_graphs).fill_cc_neighbouring();

    ASSERT_EQ(debug_only_fc_to_cc, agg.__cc_graphs->_fc_2_cc);
    agg._correction_split_too_big_cc_in_two();
    vector<long> ref_fc_2_cc = {0, 0, 0, 0, 0, 4, 1, 1, 3, 6, 5, 4, 4, 4, 2, 3, 3, 6, 6, 2, 2, 2, 2, 5, 5, 1};
    ASSERT_EQ(ref_fc_2_cc, agg.__cc_graphs->_fc_2_cc);
}

TEST_F(MGridGen_ext_v2_Dual_Graph, correction_split_too_big_cc_in_two_MGridGen_ext_v2_MG_1_level_Agglo_2) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = false;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short goal_card = -1;
    short min_card = -1;
    short max_card = -1;
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> aniso_agglo_lines = {};
    vector<long> debug_only_fc_to_cc = {0, 0, 0, 0, 0,
                                        0, 0, 0, 1, 1,
                                        1, 2, 2, 2, 2,
                                        1, 1, 1, 1, 2,
                                        2, 2, 2, 1, 1,
                                        0};
    //    np.array([0, 0, 0, 0, 0, 0])]
    agg.agglomerate_one_level(is_anisotropic,
                              nb_aniso_agglo_lines,
                              aniso_agglo_lines,
                              kind_of_agglomerator,
                              goal_card,
                              min_card,
                              max_card,
                              debug_only_fc_to_cc);
    ASSERT_EQ(debug_only_fc_to_cc, agg.__cc_graphs->_fc_2_cc);
    ASSERT_TRUE((*agg.__cc_graphs).check_data_consistency_and_connectivity());
    agg._correction_split_too_big_cc_in_two();
    vector<long> ref_fc_2_cc = {3, 3, 3, 0, 0, 0, 0, 0, 1, 1, 1, 5, 5, 5, 2, 4, 4, 4, 1, 2, 2, 2, 2, 1, 1, 0};
    ASSERT_EQ(ref_fc_2_cc, agg.__cc_graphs->_fc_2_cc);
    ASSERT_TRUE((*agg.__cc_graphs).check_data_consistency_and_connectivity());
}

TEST_F(MGridGen_ext_v2_Dual_Graph, correction_split_too_big_cc_in_two_MGridGen_ext_v2_MG_1_level_Agglo_3) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = false;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short goal_card = -1;
    short min_card = -1;
    short max_card = -1;
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> aniso_agglo_lines = {};
    vector<long> debug_only_fc_to_cc = {0, 0, 0, 0, 0,
                                        0, 1, 1, 3, 2,
                                        3, 1, 1, 1, 2,
                                        3, 3, 3, 3, 2,
                                        2, 2, 1, 2, 3,
                                        0};
    //    np.array([0, 0, 0, 0, 0, 0])]
    agg.agglomerate_one_level(is_anisotropic,
                              nb_aniso_agglo_lines,
                              aniso_agglo_lines,
                              kind_of_agglomerator,
                              goal_card,
                              min_card,
                              max_card,
                              debug_only_fc_to_cc);
    ASSERT_EQ(debug_only_fc_to_cc, agg.__cc_graphs->_fc_2_cc);

    agg._correction_split_too_big_cc_in_two();
//    {4, 4, 4, 0, 0, 0, 5, 5, 3, 2, 3, 1, 1, 1, 6, 7, 7, 7, 7, 2, 6, 6, 1, 2, 3, 0}; is also correct
//    {4, 4, 4, 0, 0, 0, 1, 1, 3, 2, 3, 6, 6, 1, 7, 5, 5, 5, 5, 2, 7, 7, 1, 2, 3, 0}; is also correct
    vector<long> ref_fc_2_cc = {4, 4, 4, 0, 0, 0, 1, 1, 5, 7, 5, 6, 6, 1, 2, 3, 3, 3, 3, 7, 2, 2, 1, 7, 5, 0};

    ASSERT_EQ(ref_fc_2_cc, agg.__cc_graphs->_fc_2_cc);
}

TEST_F(MGridGen_ext_v2_Dual_Graph, correction_split_too_big_cc_in_two_MGridGen_ext_v2_MG_1_level_Agglo_4) {

    unsigned short int verbose = 0;
    bool is_visu_data_stored = false;
    int dimension = 2;
    bool checks = true;

    Agglomerator agg = Agglomerator((*g),
                                    verbose,
                                    is_visu_data_stored,
                                    dimension,
                                    checks);

    bool is_anisotropic = false;
    string kind_of_agglomerator = "basic";
    short goal_card = -1;
    short min_card = -1;
    short max_card = -1;
    unsigned long nb_aniso_agglo_lines = 0;
    forward_list<deque<long> *> aniso_agglo_lines = {};
    vector<long> debug_only_fc_to_cc = {3, 3, 3, 0, 0,
                                        0, 0, 0, 1, 1,
                                        1, 2, 2, 2, 2,
                                        4, 4, 4, 1, 2,
                                        2, 2, 2, 1, 1, 0};

    agg.agglomerate_one_level(is_anisotropic,
                              nb_aniso_agglo_lines,
                              aniso_agglo_lines,
                              kind_of_agglomerator,
                              goal_card,
                              min_card,
                              max_card,
                              debug_only_fc_to_cc);
    ASSERT_EQ(debug_only_fc_to_cc, agg.__cc_graphs->_fc_2_cc);

    agg._correction_split_too_big_cc_in_two();
    vector<long> ref_fc_2_cc = {3, 3, 3, 0, 0, 6, 6, 6, 7, 1, 7, 5, 5, 5, 2, 4, 4, 4, 1, 2, 2, 2, 2, 1, 7, 0};

    ASSERT_EQ(ref_fc_2_cc, agg.__cc_graphs->_fc_2_cc);
}
