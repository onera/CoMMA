#include "../CoMMA_lib/Agglomerator_Isotropic.h"
#include "../CoMMA_lib/Dual_Graph.h"
#include "../CoMMA_lib/Coarse_Cell_Graph.h"

#include "MGridGen_Dual_Graph.h"
#include "MGridGen_ext_v2_Dual_Graph.h"

#include "gtest/gtest.h"

class Test_CCG_box_5x5x5 : public ::testing::Test {
    // filename_ini = "box_1_rect_5_d_F.hdf"
    // input_directory = os.path.join(self.cgns_path, os.path.join("0_Inputs", "0_Box"))

protected:

    Dual_Graph *g;
    int box_5x5x5_number_of_cells;
    int box_5x5x5_col_ind_size;
    vector<long> v_box_5x5x5_row_ptr;
    vector<long> v_box_5x5x5_col_ind;
    vector<double> v_box_5x5x5_values;
    vector<double> v_box_5x5x5_volumes;

    virtual void SetUp() {
        v_box_5x5x5_row_ptr = {0, 4, 9, 14, 18, 23, 29, 35, 40, 45, 51, 57, 62,
                               66, 71, 76, 80, 85, 91, 97, 102, 108, 114, 120, 126, 132,
                               138, 144, 150, 155, 161, 167, 172, 177, 183, 189, 194, 200, 206,
                               212, 218, 224, 230, 236, 242, 247, 253, 259, 264, 268, 273, 278,
                               282, 287, 293, 299, 304, 309, 315, 321, 326, 330, 335, 340, 344};

        v_box_5x5x5_col_ind = {0, 1, 4, 16, 0, 1, 2, 5, 17, 1, 2, 3, 6, 18, 2, 3, 7,
                               19, 0, 4, 5, 8, 20, 1, 4, 5, 6, 9, 21, 2, 5, 6, 7, 10,
                               22, 3, 6, 7, 11, 23, 4, 8, 9, 12, 24, 5, 8, 9, 10, 13, 25,
                               6, 9, 10, 11, 14, 26, 7, 10, 11, 15, 27, 8, 12, 13, 28, 9, 12,
                               13, 14, 29, 10, 13, 14, 15, 30, 11, 14, 15, 31, 0, 16, 17, 20, 32,
                               1, 16, 17, 18, 21, 33, 2, 17, 18, 19, 22, 34, 3, 18, 19, 23, 35,
                               4, 16, 20, 21, 24, 36, 5, 17, 20, 22, 25, 37, 6, 18, 21, 23, 26,
                               38, 7, 19, 22, 23, 27, 39, 8, 20, 24, 25, 28, 40, 9, 21, 24, 26,
                               29, 41, 10, 22, 25, 27, 30, 42, 11, 23, 26, 27, 31, 43, 12, 24, 28,
                               29, 44, 13, 25, 28, 29, 30, 45, 14, 26, 29, 30, 31, 46, 15, 27, 30,
                               31, 47, 16, 32, 33, 36, 48, 17, 32, 33, 34, 37, 49, 18, 33, 34, 35,
                               38, 50, 19, 34, 35, 39, 51, 20, 32, 36, 37, 40, 52, 21, 33, 36, 38,
                               41, 53, 22, 34, 37, 39, 42, 54, 23, 35, 38, 39, 43, 55, 24, 36, 40,
                               41, 44, 56, 25, 37, 40, 42, 45, 57, 26, 38, 41, 43, 46, 58, 27, 39,
                               42, 43, 47, 59, 28, 40, 44, 45, 60, 29, 41, 44, 45, 46, 61, 30, 42,
                               45, 46, 47, 62, 31, 43, 46, 47, 63, 32, 48, 49, 52, 33, 48, 49, 50,
                               53, 34, 49, 50, 51, 54, 35, 50, 51, 55, 36, 48, 52, 53, 56, 37, 49,
                               52, 53, 54, 57, 38, 50, 53, 54, 55, 58, 39, 51, 54, 55, 59, 40, 52,
                               56, 57, 60, 41, 53, 56, 57, 58, 61, 42, 54, 57, 58, 59, 62, 43, 55,
                               58, 59, 63, 44, 56, 60, 61, 45, 57, 60, 61, 62, 46, 58, 61, 62, 63,
                               47, 59, 62, 63};

        v_box_5x5x5_values = {75., 25., 25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25.,
                              25., 25., 75., 25., 25., 25., 50., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25.,
                              25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25., 75., 25.,
                              25., 25., 25., 50., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                              75., 25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 50., 25., 25., 50., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 50., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25.,
                              25., 25., 50., 25., 25., 25., 25., 75., 25., 25., 25., 50., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25.,
                              25., 25., 25., 50., 25., 25., 25., 50., 25., 25., 25., 25., 25.,
                              25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 25., 50.,
                              25., 25., 25., 75., 25., 25., 25., 25., 50., 25., 25., 25., 25.,
                              50., 25., 25., 25., 25., 75.};

        v_box_5x5x5_volumes = {125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                               125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                               125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                               125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                               125., 125., 125., 125., 125., 125., 125., 125., 125., 125., 125.,
                               125., 125., 125., 125., 125., 125., 125., 125., 125.};

        vector<int> box_5x5x5_is_on_bnd = {3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3, 2, 1, 1, 2, 1, 0,
                                           0, 1, 1, 0, 0, 1, 2, 1, 1, 2, 2, 1, 1, 2, 1, 0, 0, 1, 1, 0, 0, 1,
                                           2, 1, 1, 2, 3, 2, 2, 3, 2, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 3};

        box_5x5x5_number_of_cells = 64;
        box_5x5x5_col_ind_size = 344;

        unordered_map<long, int> box_5x5x5_d_is_on_bnd;
        int i(0);
        for (int value : box_5x5x5_is_on_bnd)
            box_5x5x5_d_is_on_bnd[i++] = value;

        g = new Dual_Graph(box_5x5x5_number_of_cells,
                           v_box_5x5x5_row_ptr,
                           v_box_5x5x5_col_ind,
                           v_box_5x5x5_values,
                           v_box_5x5x5_volumes,
                           box_5x5x5_d_is_on_bnd);

    }

    virtual void TearDown() {
        delete g;
    }
};

TEST_F(Test_CCG_box_5x5x5, constructor_box_5x5x5) {
    Coarse_Cell_Graph ccg((*g));
    ASSERT_EQ(64, ccg._fc_graph.number_of_cells);
    ASSERT_FALSE(ccg._a_is_fc_agglomerated[0]);
}

TEST_F(MGridGen_Dual_Graph, create_a_cc_Case_1_MGridGen) {

    Coarse_Cell_Graph ccg((*g));
    ASSERT_EQ(ccg._cc_counter, 0);

    //========================
    // Create coarse cell 0
    //========================
    unordered_set<long> s_cc_0 = {3};
    ccg.cc_create_a_cc(s_cc_0);

    ASSERT_TRUE(ccg.is_isotropic_cc(0));
    ASSERT_FALSE(ccg.is_fc_agglomerated_in_isotropic_cc(0));
    ASSERT_TRUE(ccg.is_fc_agglomerated_in_isotropic_cc(3));
    ASSERT_FALSE(ccg.is_anisotropic_cc(0));

    ASSERT_EQ(1, ccg._cc_counter);
    ASSERT_EQ(1, ccg._nb_of_agglomerated_fc);

    unordered_map<long, unordered_set<long>> ref_d_cc_all = {{0, unordered_set<long>({3})}};
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    unordered_map<unsigned short int, long> ref_d_dist_of_card = {{1, 1}};
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_card_2_cc = {{1, {0}}};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_2_cc = {{0, {0}}};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);
    ASSERT_EQ(0, ccg.get_cc_compactness(0));

    ASSERT_TRUE(ccg._delayed_cc.empty());
    vector<long> ref_fc_2_cc = {-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 1
    //========================
    unordered_set<long> s_cc_1 = {1, 2, 0};
    ccg.cc_create_a_cc(s_cc_1);

    ASSERT_TRUE(ccg.is_isotropic_cc(1));
    ASSERT_FALSE(ccg.is_anisotropic_cc(1));
    ASSERT_TRUE(ccg.is_fc_agglomerated_in_isotropic_cc(0));
    ASSERT_TRUE(ccg.is_fc_agglomerated_in_isotropic_cc(3));

    ASSERT_EQ(ccg._cc_counter, 2);
    ASSERT_EQ(4, ccg._nb_of_agglomerated_fc);
    ref_d_cc_all[1] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    ref_d_dist_of_card[3] = 1;
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ref_d_card_2_cc[3] = {1};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    ref_d_compactness_2_cc[1] = {1};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));

    ASSERT_TRUE(ccg._delayed_cc.empty());

    ref_fc_2_cc = {1, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 2
    //========================
    unordered_set<long> s_cc_2 = {4, 5, 8, 7};
    ccg.cc_create_a_cc(s_cc_2);

    ASSERT_TRUE(ccg.is_isotropic_cc(2));
    ASSERT_FALSE(ccg.is_anisotropic_cc(2));

    ASSERT_EQ(ccg._cc_counter, 3);
    ASSERT_EQ(8, ccg._nb_of_agglomerated_fc);
    ref_d_cc_all[2] = unordered_set<long>({4, 5, 8, 7});
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    ref_d_dist_of_card[4] = 1;
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ref_d_card_2_cc[4] = {2};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

//    ASSERT_EQ({ 0: { 0 }, 1: { 1 }}, ccg._d_compactness_2_cc, 1);
    ref_d_compactness_2_cc[2] = {2};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));
    ASSERT_EQ(2, ccg.get_cc_compactness(2));

    ASSERT_TRUE(ccg._delayed_cc.empty());

    ref_fc_2_cc = {1, 1, 1, 0, 2, 2, -1, 2, 2, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 3 and 4
    //========================
    unordered_set<long> s_cc_3 = {6, 11, 13, 14};
    ccg.cc_create_a_cc(s_cc_3);

    ASSERT_TRUE(ccg.is_isotropic_cc(3));
    ASSERT_FALSE(ccg.is_anisotropic_cc(3));

    unordered_set<long> s_cc_4 = {10, 9, 12};
    ccg.cc_create_a_cc(s_cc_4);

    ASSERT_TRUE(ccg.is_isotropic_cc(4));
    ASSERT_FALSE(ccg.is_anisotropic_cc(4));

    ASSERT_EQ(ccg._cc_counter, 5);
    ASSERT_EQ(15, ccg._nb_of_agglomerated_fc);
    ref_d_cc_all[3] = unordered_set<long>(s_cc_3);
    ref_d_cc_all[4] = unordered_set<long>(s_cc_4);
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    ref_d_dist_of_card[3] = 2;
    ref_d_dist_of_card[4] = 2;
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

//    ASSERT_EQ({1: 1, 3: 2, 4: 2}, ccg.d_distribution_of_cardinal_of_isotropic_cc)

    ref_d_card_2_cc[4].insert(3);
    ref_d_card_2_cc[3].insert(4);
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

//    ASSERT_EQ({1: {0}, 3: {1, 4}, 4: {2, 3}}, ccg._d_card_2_cc)
    ref_d_compactness_2_cc[1].insert(3);
    ref_d_compactness_2_cc[1].insert(4);
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

//    ASSERT_EQ({0: {0}, 1: {1, 3, 4}, 2: {2}}, ccg._d_compactness_2_cc, 1)
    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));
    ASSERT_EQ(2, ccg.get_cc_compactness(2));
    ASSERT_EQ(1, ccg.get_cc_compactness(3));
    ASSERT_EQ(1, ccg.get_cc_compactness(4));

// print(ccg._fc_2_cc.__repr__())
    ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

//    ref_fc_2_cc = np.array([1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3])
//    np.testing.assert_equal(ref_fc_2_cc, ccg.fc_2_cc)
    ASSERT_EQ(15, ccg._fc_graph.number_of_cells);
    ASSERT_TRUE(ccg._a_is_fc_agglomerated[0]);
}

TEST_F(MGridGen_Dual_Graph, create_a_cc_Case_2_MGridGen_is_anisotropic) {

    unordered_set<long> empty_set = unordered_set<long>({});
    unordered_map<long, unordered_set<long>> empty_dict_l_sl;
    unordered_map<unsigned short int, unordered_set<long>> empty_dict_s_sl;
    unordered_map<unsigned short int, long> empty_dict_s_l;

    Coarse_Cell_Graph ccg((*g));
    ASSERT_EQ(ccg._cc_counter, 0);

    const bool is_anisotropic = true;
    //========================
    // Create coarse cell 0
    //========================
    unordered_set<long> s_cc_0 = {3};
    ccg.cc_create_a_cc(s_cc_0, is_anisotropic);

    ASSERT_FALSE(ccg.is_isotropic_cc(0));
    ASSERT_TRUE(ccg.is_anisotropic_cc(0));
    ASSERT_FALSE(ccg.is_fc_agglomerated_in_isotropic_cc(0));


    ASSERT_EQ(1, ccg._cc_counter);

    unordered_map<long, unordered_set<long>> ref_d_cc_all = {{0, unordered_set<long>({3})}};
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());
    ASSERT_EQ(empty_dict_l_sl, ccg.get_d_cc_iso());

    unordered_map<unsigned short int, long> ref_d_dist_of_card = {{1, 1}};
    ASSERT_EQ(empty_dict_s_l, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_card_2_cc = {{1, {0}}};
    ASSERT_EQ(empty_dict_s_sl, ccg._d_card_2_cc);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_2_cc = {{0, {0}}};
    ASSERT_EQ(empty_dict_s_sl, ccg._d_compactness_2_cc);
    ASSERT_EQ(0, ccg.get_cc_compactness(0));
//
    ASSERT_TRUE(ccg._delayed_cc.empty());
    vector<long> ref_fc_2_cc = {-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 1
    //========================
    unordered_set<long> s_cc_1 = {1, 2, 0};
    ccg.cc_create_a_cc(s_cc_1, is_anisotropic);

    ASSERT_FALSE(ccg.is_isotropic_cc(1));
    ASSERT_TRUE(ccg.is_anisotropic_cc(1));

    ASSERT_EQ(ccg._cc_counter, 2);
    ref_d_cc_all[1] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());
    ASSERT_EQ(empty_dict_l_sl, ccg.get_d_cc_iso());

    ASSERT_EQ(empty_dict_s_l, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());
    ASSERT_EQ(empty_dict_s_sl, ccg._d_card_2_cc);

    ref_d_compactness_2_cc[1] = {1};
    ASSERT_EQ(empty_dict_s_sl, ccg._d_compactness_2_cc);

    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));

    ASSERT_TRUE(ccg._delayed_cc.empty());

    ref_fc_2_cc = {1, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 2
    //========================
    unordered_set<long> s_cc_2 = {4, 5, 8, 7};
    ccg.cc_create_a_cc(s_cc_2, is_anisotropic);

    ASSERT_FALSE(ccg.is_isotropic_cc(2));
    ASSERT_TRUE(ccg.is_anisotropic_cc(2));


    ASSERT_EQ(ccg._cc_counter, 3);
    ref_d_cc_all[2] = unordered_set<long>({4, 5, 8, 7});
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());
    ASSERT_EQ(empty_dict_l_sl, ccg.get_d_cc_iso());

    ref_d_dist_of_card[4] = 1;
    ASSERT_EQ(empty_dict_s_l, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ref_d_card_2_cc[4] = {2};
    ASSERT_EQ(empty_dict_s_sl, ccg._d_card_2_cc);

//    ASSERT_EQ({ 0: { 0 }, 1: { 1 }}, ccg._d_compactness_2_cc, 1);
    ref_d_compactness_2_cc[2] = {2};
    ASSERT_EQ(empty_dict_s_sl, ccg._d_compactness_2_cc);

    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));
    ASSERT_EQ(1, ccg.get_cc_compactness(2));

    ASSERT_TRUE(ccg._delayed_cc.empty());

    ref_fc_2_cc = {1, 1, 1, 0, 2, 2, -1, 2, 2, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 3 and 4
    //========================
    unordered_set<long> s_cc_3 = {6, 11, 13, 14};
    ccg.cc_create_a_cc(s_cc_3, is_anisotropic);

    ASSERT_FALSE(ccg.is_isotropic_cc(3));
    ASSERT_TRUE(ccg.is_anisotropic_cc(3));

    unordered_set<long> s_cc_4 = {10, 9, 12};
    ccg.cc_create_a_cc(s_cc_4, is_anisotropic);

    ASSERT_FALSE(ccg.is_isotropic_cc(4));
    ASSERT_TRUE(ccg.is_anisotropic_cc(4));

    ASSERT_EQ(ccg._cc_counter, 5);
    ref_d_cc_all[3] = unordered_set<long>(s_cc_3);
    ref_d_cc_all[4] = unordered_set<long>(s_cc_4);
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    ref_d_dist_of_card[3] = 2;
    ref_d_dist_of_card[4] = 2;
    ASSERT_EQ(empty_dict_s_l, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ref_d_card_2_cc[4].insert(3);
    ref_d_card_2_cc[3].insert(4);
    ASSERT_EQ(empty_dict_s_sl, ccg._d_card_2_cc);

//    ASSERT_EQ({1: {0}, 3: {1, 4}, 4: {2, 3}}, ccg._d_card_2_cc)
    ref_d_compactness_2_cc[1].insert(3);
    ref_d_compactness_2_cc[1].insert(4);
    ASSERT_EQ(empty_dict_s_sl, ccg._d_compactness_2_cc);

//    ASSERT_EQ({0: {0}, 1: {1, 3, 4}, 2: {2}}, ccg._d_compactness_2_cc, 1)
    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));
    ASSERT_EQ(1, ccg.get_cc_compactness(2));
    ASSERT_EQ(1, ccg.get_cc_compactness(3));
    ASSERT_EQ(1, ccg.get_cc_compactness(4));

// print(ccg._fc_2_cc.__repr__())
    ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

//    ref_fc_2_cc = np.array([1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3])
//    np.testing.assert_equal(ref_fc_2_cc, ccg.fc_2_cc)
    ASSERT_EQ(15, ccg._fc_graph.number_of_cells);
    ASSERT_TRUE(ccg._a_is_fc_agglomerated[0]);
}

TEST_F(MGridGen_Dual_Graph, create_a_cc_delayed_Case_1_MGridGen) {

    bool is_aniso = false;
    bool is_creation_delayed = true;

    unordered_set<long> empty_set = unordered_set<long>({});
    unordered_map<long, unordered_set<long>> empty_dict_l_sl;
    unordered_map<unsigned short int, unordered_set<long>> empty_dict_s_sl;
    unordered_map<unsigned short int, long> empty_dict_s_l;

    Coarse_Cell_Graph ccg((*g));
    ASSERT_EQ(ccg._cc_counter, 0);

    //========================
    // Create coarse cell 0
    //========================
    unordered_set<long> s_cc_0 = {3};
    ccg.cc_create_a_cc(s_cc_0, is_aniso, is_creation_delayed);
    ASSERT_EQ(0, ccg._cc_counter);

    ASSERT_EQ(empty_dict_l_sl, ccg.get_d_cc_all());

    ASSERT_EQ(empty_dict_s_l, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ASSERT_EQ(empty_dict_s_sl, ccg._d_card_2_cc);

    ASSERT_EQ(empty_dict_s_sl, ccg._d_compactness_2_cc);
    ASSERT_EQ(0, ccg.get_cc_compactness(0));

    ASSERT_EQ(1, ccg._delayed_cc.size());
    ASSERT_EQ(s_cc_0, ccg._delayed_cc[0]);
    vector<long> ref_fc_2_cc = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 1
    //========================
    unordered_set<long> s_cc_1 = {1, 2, 0};
    ccg.cc_create_a_cc(s_cc_1, is_aniso, is_creation_delayed);

    ASSERT_EQ(ccg._cc_counter, 0);
    ASSERT_EQ(empty_dict_l_sl, ccg.get_d_cc_all());
    ASSERT_EQ(empty_dict_s_l, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());
    ASSERT_EQ(empty_dict_s_sl, ccg._d_card_2_cc);
    ASSERT_EQ(empty_dict_s_sl, ccg._d_compactness_2_cc);

    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));

    ASSERT_EQ(2, ccg._delayed_cc.size());
    ASSERT_EQ(s_cc_0, ccg._delayed_cc[0]);
    ASSERT_EQ(s_cc_1, ccg._delayed_cc[1]);

    ref_fc_2_cc = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 2
    //========================
    unordered_set<long> s_cc_2 = {4, 5, 8, 7};
    ccg.cc_create_a_cc(s_cc_2, is_aniso, is_creation_delayed);

    ASSERT_EQ(ccg._cc_counter, 0);
    ASSERT_EQ(empty_dict_l_sl, ccg.get_d_cc_all());
    ASSERT_EQ(empty_dict_s_l, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());
    ASSERT_EQ(empty_dict_s_sl, ccg._d_card_2_cc);
    ASSERT_EQ(empty_dict_s_sl, ccg._d_compactness_2_cc);

    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));
    ASSERT_EQ(2, ccg.get_cc_compactness(2));

    ASSERT_EQ(3, ccg._delayed_cc.size());
    ASSERT_EQ(s_cc_0, ccg._delayed_cc[0]);
    ASSERT_EQ(s_cc_1, ccg._delayed_cc[1]);
    ASSERT_EQ(s_cc_2, ccg._delayed_cc[2]);


    ref_fc_2_cc = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //========================
    // Create coarse cell 3 and 4
    //========================
    unordered_set<long> s_cc_3 = {6, 11, 13, 14};
    ccg.cc_create_a_cc(s_cc_3, is_aniso, false);

    unordered_set<long> s_cc_4 = {10, 9, 12};
    ccg.cc_create_a_cc(s_cc_4, is_aniso, is_creation_delayed);

    ASSERT_EQ(ccg._cc_counter, 1);

    unordered_map<long, unordered_set<long>> ref_d_cc_all = {{0, unordered_set<long>({6, 11, 13, 14})}};
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    unordered_map<unsigned short int, long> ref_d_dist_of_card = {{4, 1}};
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_card_2_cc = {{4, {0}}};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_2_cc = {{1, {0}}};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    ASSERT_EQ(1, ccg.get_cc_compactness(0));

    ref_fc_2_cc = {-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, 0, 0};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    ASSERT_EQ(15, ccg._fc_graph.number_of_cells);
    ASSERT_TRUE(ccg._a_is_fc_agglomerated[0]);
    ccg.cc_create_all_delayed_cc();

    ASSERT_EQ(ccg._cc_counter, 5);
    //{ (0, { 11, 6, 13, 14 }), (1, { 3 }), (2, { 0, 2, 1 }), (3, { 7, 8, 5, 4 }), (4, { 12, 9, 10 }) }
    ref_d_cc_all[1] = unordered_set<long>({3});
    ref_d_cc_all[2] = unordered_set<long>({0, 2, 1});
    ref_d_cc_all[3] = unordered_set<long>({7, 8, 5, 4});
    ref_d_cc_all[4] = unordered_set<long>({12, 9, 10});
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    ref_d_dist_of_card[1] = 1;
    ref_d_dist_of_card[3] = 2;
    ref_d_dist_of_card[4] = 2;
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ref_d_card_2_cc[1] = unordered_set<long>({1});
    ref_d_card_2_cc[3] = unordered_set<long>({4, 2});
    ref_d_card_2_cc[4] = unordered_set<long>({3, 0});
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    ref_d_compactness_2_cc[0] = unordered_set<long>({1});
    ref_d_compactness_2_cc[1] = unordered_set<long>({4, 2, 0});
    ref_d_compactness_2_cc[2] = unordered_set<long>({3});
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    ASSERT_EQ(1, ccg.get_cc_compactness(0));
    ASSERT_EQ(0, ccg.get_cc_compactness(1));
    ASSERT_EQ(1, ccg.get_cc_compactness(2));
    ASSERT_EQ(2, ccg.get_cc_compactness(3));
    ASSERT_EQ(1, ccg.get_cc_compactness(4));
    ref_fc_2_cc = {2, 2, 2, 1, 3, 3, 0, 3, 3, 4, 4, 0, 4, 0, 0};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

}

TEST_F(MGridGen_ext_v2_Dual_Graph, _cc_update_neighbours_MGridGen_ext_v2) {

    Coarse_Cell_Graph ccg((*g));

    vector<long> ref_fc_2_cc = {0, 0, 0, 0, 0,
                                0, 0, 1, 3, 3,
                                2, 2, 2, 2, 5,
                                3, 3, 3, 4, 4,
                                4, 4, 5, 4, 2,
                                0};

    //========================
    // Create coarse cell 0
    //========================
    unordered_set<long> s_fc_0 = {0, 1, 2, 3, 4, 5, 6, 25};
    ccg.cc_create_a_cc(s_fc_0);

    //========================
    // Create coarse cell 1
    //========================
    unordered_set<long> s_fc_1 = {7};
    ccg.cc_create_a_cc(s_fc_1);

    //========================
    // Create coarse cell 2
    //========================
    unordered_set<long> s_fc_2 = {10, 11, 12, 13, 24};
    ccg.cc_create_a_cc(s_fc_2);

    //========================
    // Create coarse cell 3
    //========================
    unordered_set<long> s_fc_3 = {8, 9, 15, 16, 17};
    ccg.cc_create_a_cc(s_fc_3);

    //========================
    // Create coarse cell 4
    //========================
    unordered_set<long> s_fc_4 = {18, 19, 20, 21, 23};
    ccg.cc_create_a_cc(s_fc_4);

    //========================
    // Create coarse cell 5
    //========================
    unordered_set<long> s_fc_5 = {14, 22};
    ccg.cc_create_a_cc(s_fc_5);

    ccg.fill_cc_neighbouring();

    // cc = ccg._d_isotropic_cc[0]
    // fc 9: 3->2
    long i_origin_cc = 3;
    long i_destination_cc = 2;

    unordered_set<long> s_fc = {9};

    Coarse_Cell *origin_cc = ccg._d_isotropic_cc[i_origin_cc];
    Coarse_Cell *destination_cc = ccg._d_isotropic_cc[i_destination_cc];

    (*destination_cc).add_fc(s_fc, ref_fc_2_cc);

    unordered_set<long> union_s_fc = (*destination_cc).get_s_fc();
    for (const long &i_fc :(*origin_cc).get_s_fc()) {
        union_s_fc.insert(i_fc);
    }

    unordered_set<long> s_fc_n;

    long i_fc = 9;
    vector<long> v_neighbours = (*g).get_neighbours(i_fc);
    for (const long &i_fc_n :  v_neighbours) {
        s_fc_n.insert(i_fc_n);
    }

    for (const long &i_fc_n :  union_s_fc) {
        if (s_fc_n.count(i_fc_n) > 0) {
            s_fc_n.erase(i_fc_n);
        }
    }
    unordered_set<long> ref_s_fc_n = {18, 23};
    ASSERT_EQ(ref_s_fc_n, s_fc_n);
    // s_fc_n = set(ccg._fc_graph.get_neighbours(i_fc)) - union_s_fc

//    for (const long &i_fc_n: s_fc_n) {
//        const long &i_cc_n = ref_fc_2_cc[i_fc_n];
//        (*ccg._d_isotropic_cc[i_cc_n]).update_cc_neighbour(i_fc_n, i_fc, i_origin_cc, i_destination_cc);
//    }
    ccg._cc_update_neighbours(i_fc,
                              s_fc_n,
                              i_origin_cc,
                              i_destination_cc);
    ccg._fc_2_cc[i_fc] = i_destination_cc;

//    ref_fc_2_cc[i_fc] = i_destination_cc;
    (*origin_cc).remove_fc(s_fc, ref_fc_2_cc);

    ASSERT_TRUE((*destination_cc).check_consistency(ref_fc_2_cc));
    ASSERT_TRUE((*origin_cc).check_consistency(ref_fc_2_cc));
}


TEST_F(MGridGen_Dual_Graph, swap_fc_case_1_MGridGen) {

    Coarse_Cell_Graph ccg((*g));

    ccg.cc_create_a_cc({0, 2});
    ccg.cc_create_a_cc({1, 3});

    unordered_map<long, unordered_set<long>> ref_d_cc_all = {{0, unordered_set<long>({0, 2})},
                                                             {1, unordered_set<long>({1, 3})}};
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_card_2_cc = {{2, {0, 1}}};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    unordered_map<unsigned short int, long> ref_d_dist_of_card = {{2, 2}};
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ASSERT_EQ(1, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_2_cc = {{1, {0, 1}}};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    ccg.fill_cc_neighbouring();

    long i_fc = 0;
    long i_origin_cc = 0;
    long i_target_cc = 1;
    unordered_set<long> s_fc = {i_fc};
    unordered_set<long> set_removed_cc = ccg.cc_swap_fc(s_fc, i_origin_cc, i_target_cc);

    unordered_set<long> empty_set = unordered_set<long>({});
    ASSERT_EQ(empty_set, set_removed_cc);

    ref_d_cc_all = {{0, unordered_set<long>({2})},
                    {1, unordered_set<long>({1, 3, 0})}};
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());
    ref_d_card_2_cc = {{1, {0}},
                       {3, {1}}};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    ref_d_dist_of_card = {{1, 1},
                          {3, 1}};
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ASSERT_EQ(0, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));

    ref_d_compactness_2_cc = {{0, {0}},
                              {1, {1}}};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    vector<long> ref_fc_2_cc = {1, 1, 0, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

}

TEST_F(MGridGen_Dual_Graph, swap_fc_case_2_MGridGen) {

    Coarse_Cell_Graph ccg((*g));

    ccg.cc_create_a_cc({0, 2});
    ccg.cc_create_a_cc({1, 3});

    unordered_map<long, unordered_set<long>> ref_d_cc_all = {{0, unordered_set<long>({0, 2})},
                                                             {1, unordered_set<long>({1, 3})}};
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_card_2_cc = {{2, {0, 1}}};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    unordered_map<unsigned short int, long> ref_d_dist_of_card = {{2, 2}};
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ASSERT_EQ(1, ccg.get_cc_compactness(0));
    ASSERT_EQ(1, ccg.get_cc_compactness(1));

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_2_cc = {{1, {0, 1}}};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    ccg.fill_cc_neighbouring();

    unordered_set<long> s_fc = {0, 2};
    long i_origin_cc = 0;
    long i_target_cc = 1;
    unordered_set<long> set_removed_cc = ccg.cc_swap_fc(s_fc, i_origin_cc, i_target_cc);
    ccg._s_cc_to_remove = set_removed_cc;
    unordered_set<long> ref_s_removed = unordered_set<long>({0});
    ASSERT_EQ(ref_s_removed, ccg._s_cc_to_remove);

    ref_d_cc_all = {{1, unordered_set<long>({1, 3, 0, 2})}};
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    ref_d_card_2_cc = {{4, {1}}};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    ref_d_dist_of_card = {{4, 1}};
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ASSERT_EQ(1, ccg.get_cc_compactness(1));

    ref_d_compactness_2_cc = {{1, {1}}};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    vector<long> ref_fc_2_cc = {1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    ccg.cc_renumber();
    ref_s_removed = unordered_set<long>({});
    ASSERT_EQ(ref_s_removed, ccg._s_cc_to_remove);

    ref_d_cc_all = {{0, unordered_set<long>({1, 3, 0, 2})}};
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    ref_d_card_2_cc = {{4, {0}}};
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    ref_d_dist_of_card = {{4, 1}};
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    ASSERT_EQ(1, ccg.get_cc_compactness(0));

    ref_d_compactness_2_cc = {{1, {0}}};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    ref_fc_2_cc = {0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);
}

TEST_F(MGridGen_ext_v2_Dual_Graph, swap_fc_case_MGridGen_ext_v2) {

    Coarse_Cell_Graph ccg((*g));

    ccg.cc_create_a_cc({0, 1, 2, 3, 4, 25});
    ccg.cc_create_a_cc({6, 7, 5});
    ccg.cc_create_a_cc({10, 11, 12, 13, 24});
    ccg.cc_create_a_cc({8, 9, 15, 16, 17});
    ccg.cc_create_a_cc({18, 19, 20, 21, 23});
    ccg.cc_create_a_cc({14, 22});

    ccg.fill_cc_neighbouring();

    vector<long> ref_fc_2_cc = {0, 0, 0, 0, 0, 1, 1, 1, 3, 3, 2, 2, 2, 2, 5, 3, 3, 3, 4, 4, 4, 4, 5, 4, 2, 0};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    //============================
    // First swap
    //============================
    unordered_set<long> s_fc = {5};
    long i_origin_cc = 1;
    long i_target_cc = 0;
    unordered_set<long> set_removed_cc = ccg.cc_swap_fc(s_fc, i_origin_cc, i_target_cc);
    ccg._s_cc_to_remove = set_removed_cc;

    Coarse_Cell *cc = ccg._d_isotropic_cc[0];

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{2,  {{2, {{10, 2.0}}}}},
                                                                                                                       {4,  {{2, {{11, 1.0}}}}},
                                                                                                                       {5,  {{1, {{6,  1.0}}},
                                                                                                                                    {2, {{11, 1.0},
                                                                                                                                                {12, 1.0},
                                                                                                                                                {13, 1.0}}}}},
                                                                                                                       {25, {{1, {{6,  3.16227766}}}}},

    };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>> ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{2, {2}},
                                                                                                                                                      {4, {2}},
                                                                                                                                                      {5, {1}},
                                                                                                                                                      {25, {1}},
                                                                                                                                              }},
                                                                                                                                          {3, {{5, {2}}}}
    };
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    cc = ccg._d_isotropic_cc[1];

    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{6, {{2, {{13, 2.0}}},
                                                             {0, {{5, 1.0}, {25, 3.16227766}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{2, {{6, {0}}}},
                                                              {1, {{6, {2}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    cc = ccg._d_isotropic_cc[2];

    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{10, {{0, {{2, 2.0}}}, {3, {{9,  1.0}}}, {4, {{23, 1.0}}}}},
                                                 {11, {{0, {{5, 1.0},        {4, 1.0}}},
                                                                        {4, {{23, 1}}}}},
                                                 {12, {{0, {{5, 1}}},
                                                                        {5, {{14, 1}}},
                                                                                          {4, {{19, 2}, {23, 1}}}}},
                                                 {24, {{3, {{8, 2.23606798}, {9, 1}, {17, 1}}}}},
                                                 {13, {{1, {{6, 2}}},
                                                                        {0, {{5,  1}}},
                                                                                          {5, {{14, 1}, {22, 2.23606798}}}}}
    };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{13, {0, 1}}, {10, {4, 3, 0}}, {11, {4, 0}}, {12, {0, 5}}}},
                                                              {2, {{11, {0}},    {12, {4}},       {13, {5}}}},
                                                              {3, {{24, {3}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);


    cc = ccg._d_isotropic_cc[4];

    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{18, {{3, {{9,  1.0}, {17, 2.23606798}}}}},
                                                 {19, {{2, {{12, 2.0}}}, {5, {{14, 1.0}}}}},
                                                 {20, {{5, {{14, 1},   {22, 1}}}}},
                                                 {23, {{3, {{9,  1}}},   {2, {{10, 1}, {11, 1}, {12, 1}}}}}
    };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{3, {{23, {2}}}},
                                                              {1, {{23, {3}}, {19, {5, 2}}}},
                                                              {2, {{18, {3}}, {20, {5}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //============================
    // Second swap
    //============================
    s_fc = {6};
    ccg.cc_swap_fc(s_fc, 1, 0);

    cc = ccg._d_isotropic_cc[0];
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{6, {{2, {{13, 2}}}, {1, {{7, 3.16227766}}}}},
                                                 {4, {{2, {{11, 1}}}}},
                                                 {5, {{2, {{13, 1}, {12, 1}, {11, 1}}}}},
                                                 {2, {{2, {{10, 2}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{3, {{5, {2}}}},
                                                              {1, {{2, {2}}, {4, {2}}, {6, {1, 2}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //============================
    // Third swap
    //============================
    s_fc = {9};
    ccg.cc_swap_fc(s_fc, 3, 2);

    cc = ccg._d_isotropic_cc[4];
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{20, {{5, {{14, 1}, {22, 1}}}}},
                                                 {19, {{2, {{12, 2}}}, {5, {{14, 1}}}}},
                                                 {23, {{2, {{9,  1}, {10, 1}, {11, 1}, {12, 1}}}}},
                                                 {18, {{2, {{9,  1}}}, {3, {{17, 2.23606798}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{4, {{23, {2}}}},
                                                              {3, {{23, {2}}}},
                                                              {1, {{18, {2, 3}}, {19, {5, 2}}}},
                                                              {2, {{20, {5}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //============================
    // Fourth swap
    //============================
    s_fc = {13};
    ccg.cc_swap_fc(s_fc, 2, 0);
    ccg.check_cc_consistency();
    cc = ccg._d_isotropic_cc[0];
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{13, {{5, {{22, 2.23606798}, {14, 1}}}, {2, {{12, 2.23606798}}}}},
                                                 {6,  {{1, {{7,  3.16227766}}}}},
                                                 {4,  {{2, {{11, 1}}}}},
                                                 {5,  {{2, {{12, 1},          {11, 1}}}}},
                                                 {2,  {{2, {{10, 2}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
//
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{1, {{2, {2}}, {4,  {2}}, {6, {1}}, {13, {2}}}},
                                                              {2, {{5, {2}}, {13, {5}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //============================
    // Fifth swap
    //============================
    s_fc = {14};
    ccg.cc_swap_fc(s_fc, 5, 4);
    ccg.check_cc_consistency();
    cc = ccg._d_isotropic_cc[4];
    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = {{20, {{5, {{22, 1}}}}},
                                                 {14, {{5, {{22, 1}}}, {0, {{13, 1}}}, {2, {{12, 1}}}}},
                                                 {19, {{2, {{12, 2}}}}},
                                                 {23, {{2, {{9,  1}, {10, 1}, {11, 1}, {12, 1}}}}},
                                                 {18, {{2, {{9,  1}}}, {3, {{17, 2.23606798}}}}}};
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);
//
    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = {{4, {{23, {2}}}},
                                                              {1, {{18, {3, 2}}, {19, {2}}, {14, {2, 0, 5}}, {20, {5}}}}};
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

}

TEST_F(MGridGen_Dual_Graph, swap_fc_deletion_of_cc) {

    Coarse_Cell_Graph ccg((*g));

    ccg.cc_create_a_cc({2});
    ccg.cc_create_a_cc({0, 1, 3});
    ccg.cc_create_a_cc({6});
    ccg.cc_create_a_cc({11, 13, 14});
    ccg.cc_create_a_cc({5,4,7, 8, 9, 10, 12});

    unordered_map<long, unordered_set<long>> ref_d_cc_all = {{0, unordered_set<long>({2})},
                                                             {1, unordered_set<long>({0, 1, 3})},
                                                             {2, unordered_set<long>({6})},
                                                             {3, unordered_set<long>({11, 13, 14})},
                                                             {4, unordered_set<long>({5, 10, 9, 4, 8, 7, 12})},
                                                             };
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_card_2_cc = {{1, {2, 0}},
                                                                              {3, {1, 3}},
                                                                              {7, {4}}
                                                                              };
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    unordered_map<unsigned short int, long> ref_d_dist_of_card = {{1, 2},
                                                                  {3, 2},
                                                                  {7, 1}};
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());

    unordered_map<unsigned short int, unordered_set<long>> ref_d_compactness_2_cc = {{0, {0, 2}},
                                                                                     {1, {1, 3, 4}}};
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    ccg.fill_cc_neighbouring();

    vector<long> ref_fc_2_cc = {1, 1, 0, 1, 4, 4, 2, 4, 4, 4, 4, 3, 4, 3, 3};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    Coarse_Cell *cc = ccg._d_isotropic_cc[2];

    unordered_map<long, unordered_map<long, unordered_map<long, double>>> ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = { {6, { {0, { {2, 2.23606798} }}, {1, { {3, 2.23606798} }}, {3, { {11, 2} }} }} };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    unordered_map<unsigned short int, unordered_map<long, unordered_set<long>>> ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour =  { {1, { {6, { 3, 1, 0 }} }} };
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    cc = ccg._d_isotropic_cc[4];

     ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc ={ {10, { {3, { {13, 3.16227766} }} }}, {5, { {0, { {2, 2.23606798} }} }} };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = { {1, { {5, { 0 }}, {10, { 3 }} }} };
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    //============================
    // First swap
    //============================
    unordered_set<long> s_fc = {2};
    long i_origin_cc = 0;
    long i_target_cc = 1;
    unordered_set<long> set_removed_cc = ccg.cc_swap_fc(s_fc, i_origin_cc, i_target_cc);
    ccg._s_cc_to_remove = set_removed_cc;

    cc = ccg._d_isotropic_cc[2];

     ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = { {6, { {1, { {2, 2.23606798}, {3, 2.23606798} }}, {3, { {11, 2} }} }} };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

     ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour =  { {2, { {6, { 1}} }},
                                                                 {1, { {6, { 3, 1}} }} };
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    cc = ccg._d_isotropic_cc[4];

    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc ={ {10, { {3, { {13, 3.16227766} }} }}, {5, { {1, { {2, 2.23606798} }} }} };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = { {1, { {5, { 1 }}, {10, { 3 }} }} };
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    ccg.cc_renumber();

    ref_d_cc_all = {{0, unordered_set<long>({0, 1,2,  3})},
                    {1, unordered_set<long>({6})},
                    {2, unordered_set<long>({11, 13, 14})},
                    {3, unordered_set<long>({5, 10, 9, 4, 8, 7, 12})},
                    };
    ASSERT_EQ(ref_d_cc_all, ccg.get_d_cc_all());

    ref_d_card_2_cc = {{4, {0}},
                       {7, {3}},
                       {3, {2}},
                       {1, {1}},
    };
    ASSERT_EQ(ref_d_card_2_cc, ccg._d_card_2_cc);

    ref_d_dist_of_card = {{4, 1},
                          {1, 1},
                          {3, 1},
                          {7, 1},
                          };
    ASSERT_EQ(ref_d_dist_of_card, ccg.get_d_distribution_of_cardinal_of_isotropic_cc());
    ref_d_compactness_2_cc = {{1, {0, 2, 3}},
                              {0, {1}},
                              };
    ASSERT_EQ(ref_d_compactness_2_cc, ccg._d_compactness_2_cc);

    cc = ccg._d_isotropic_cc[1];

    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc = { {6, { {0, { {2, 2.23606798}, {3, 2.23606798} }},
                                                        {2, { {11, 2} }} }} };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour =  { {2, { {6, { 0}} }},
                                                                {1, { {6, { 2, 0}} }} };
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);

    cc = ccg._d_isotropic_cc[3];

    ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc ={ {10, { {2, { {13, 3.16227766} }} }},
                                                 {5, { {0, { {2, 2.23606798} }} }} };
    ASSERT_EQ(ref__d_i_fc_to_j_cc_neighbourhood_to_j_fc, (*cc).__d_i_fc_to_j_cc_neighbourhood_to_j_fc);

    ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour = { {1, { {5, { 0 }}, {10, { 2 }} }} };
    ASSERT_EQ(ref_d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour, (*cc).d_outer_fine_degree_wrt_cc_to_fc_to_s_cc_neighbour);


}

TEST_F(Test_CCG_box_5x5x5, check_consistency_Case_1_box_5x5x5) {

    Coarse_Cell_Graph ccg((*g));

    unordered_set<long> s_fc = {0, 1, 4, 5, 16, 17, 20, 21};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {10, 11, 14, 15, 26, 27, 30, 31};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {2, 3, 6, 7, 18, 19, 22, 23};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {8, 9, 12, 13, 28, 29, 24, 25};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {32, 33, 36, 37, 52, 53, 48, 49};
    ccg.cc_create_a_cc(s_fc);

    s_fc ={34, 35, 38, 39, 50, 51, 54, 55};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {40, 41, 44, 45, 56, 57, 60, 61};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {42, 43, 46, 47, 58, 59, 62, 63};
    ccg.cc_create_a_cc(s_fc);

    ASSERT_TRUE(ccg._check_consistency());
    ASSERT_TRUE(ccg.check_data_consistency_and_connectivity());
}

TEST_F(MGridGen_Dual_Graph, compute_cc_root_Case_1_MGridGen) {

    Coarse_Cell_Graph ccg((*g));
    unordered_set<long> s_fc ={2, 3, 6, 11};
    ccg.cc_create_a_cc(s_fc);

    ASSERT_EQ(6, (*g)._compute_subgraph_root((*ccg._d_isotropic_cc[0]).get_s_fc()));

    s_fc ={4, 5, 7, 8, 9};
    ccg.cc_create_a_cc(s_fc);

    ASSERT_EQ(7, (*g)._compute_subgraph_root((*ccg._d_isotropic_cc[1]).get_s_fc()));
}

TEST_F(MGridGen_Dual_Graph, compute_nb_faces_in_common_faces_with_cc_neighbourhood_MGridGen) {

    Coarse_Cell_Graph ccg((*g));

    unordered_set<long> s_fc = {3};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {1, 2, 0};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {4, 5, 8, 7};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {6, 11, 13, 14};
    ccg.cc_create_a_cc(s_fc);

    s_fc = {10, 9, 12};
    ccg.cc_create_a_cc(s_fc);

    vector<long> ref_fc_2_cc = {1, 1, 1, 0, 2, 2, 3, 2, 2, 4, 4, 3, 4, 3, 3};
    ASSERT_EQ(ref_fc_2_cc, ccg._fc_2_cc);

    unordered_map<long, unsigned short> ref_d_nb_faces ={{1, 1}, {0, 1}, {3, 1}};
    ASSERT_EQ(ref_d_nb_faces, ccg.compute_nb_faces_in_common_faces_with_cc_neighbourhood(6));
    unordered_map<long, unordered_set<long>> ref_d_faces ={{0, {3}},
                                                           {1, {2}},
                                                           {3, {11}}};
    ASSERT_EQ(ref_d_faces, ccg.compute_dict_faces_in_common_faces_with_cc_neighbourhood(6))   ;

    unordered_set<long> set_argmax_number_common_faces;
    unordered_map<long, unsigned short> dict_adjacent_cc;

    unordered_set<long> empty_set = unordered_set<long>({});
    ccg.compute_nb_faces_in_common_faces_with_cc_neighbourhood_w_unwanted_fc(6,
                                                                             empty_set,
                                                                             set_argmax_number_common_faces,
                                                                             dict_adjacent_cc);

    unordered_set<long> ref_set_argmax_number_common_faces = {0,1,3};
    ASSERT_EQ(ref_set_argmax_number_common_faces,set_argmax_number_common_faces);
    ref_d_nb_faces ={{1, 1}, {0, 1}, {3, 1}};
    ASSERT_EQ(ref_d_nb_faces,dict_adjacent_cc);

    ref_d_nb_faces ={{2, 2}, {4, 1}};
    ASSERT_EQ(ref_d_nb_faces, ccg.compute_nb_faces_in_common_faces_with_cc_neighbourhood(8));

    ref_d_faces ={{2, {5, 7}},
                  {4, {10}}};
    ASSERT_EQ(ref_d_faces, ccg.compute_dict_faces_in_common_faces_with_cc_neighbourhood(8));

    set_argmax_number_common_faces.clear();
     dict_adjacent_cc.clear();

    ccg.compute_nb_faces_in_common_faces_with_cc_neighbourhood_w_unwanted_fc(8, empty_set,
                                                                             set_argmax_number_common_faces,
                                                                             dict_adjacent_cc);
    ref_set_argmax_number_common_faces = {2};
    ASSERT_EQ(ref_set_argmax_number_common_faces,set_argmax_number_common_faces);
    ref_d_nb_faces ={{2, 2}, {4, 1}};
    ASSERT_EQ(ref_d_nb_faces,dict_adjacent_cc);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef unordered_map<long, unordered_set<long>> unorderedMap;

TEST(CoarseCellGraph_TestSuite, splitNonConnectedCoarseCell_9_Squares_1) {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    unordered_set<long> listInitialCoarseCell({0, 1, 2, 3, 4, 5, 6, 7, 8});
    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 3, 1, 3, 3, 3, 2, 3, 1};

    long indCoarseElement = 4;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0});
    dict_Coarse_Cells[1] = unordered_set<long>({2, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({6});
    dict_Coarse_Cells[3] = unordered_set<long>({1, 3, 4, 5, 7});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 2});
    dict_Card_Coarse_Cells[2] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 2;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    long iCoarseCell = 1;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);

    ASSERT_EQ(5, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({2});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({6});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({1, 3, 4, 5, 7});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 1, 2, 4});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({3});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 4;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 3, 1, 3, 3, 3, 2, 3, 4};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, splitNonConnectedCoarseCell_9_Squares_2) {
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    unordered_set<long> listInitialCoarseCell({0, 1, 2, 3, 4, 5, 6, 7, 8});
    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 2, 2, 2, 1, 2, 1};

    long indCoarseElement = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 2});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 3, 4, 5, 7});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 2;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    long iCoarseCell = 1;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);


    ASSERT_EQ(4, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 2});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 3, 4, 5, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({1, 3});
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);


    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 2, 2, 2, 1, 2, 3};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);

    }

    iCoarseCell = 0;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);
    ASSERT_EQ(5, indCoarseElement);

    ref_dict_Coarse_Cells[0] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 3, 4, 5, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    ref_dict_Card_Coarse_Cells.erase(2);
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 1, 3, 4});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    ref_dict_DistributionOfCardinalOfCoarseElements.erase(2);
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 4;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp_2[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[9] = {0, 2, 4, 2, 2, 2, 1, 2, 3};
    for (int i = 0; i < 9; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp_2[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, splitNonConnectedCoarseCell_9_Squares_3) {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};

    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 1, 2, 0, 1, 2, 1};

    long indCoarseElement = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 2, 5});
    dict_Coarse_Cells[1] = unordered_set<long>({3, 6, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 3;

    long iCoarseCell = 1;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);


    ASSERT_EQ(4, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 2, 5});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({3, 6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({3});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 2});
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 1, 2, 0, 1, 2, 3};
    for (int i = 0; i < 9; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, splitNonConnectedCoarseCell_9_Squares_5) {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};

    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 2, 0, 1, 2, 1, 0, 0, 1};

    long indCoarseElement = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 2, 6, 7});
    dict_Coarse_Cells[1] = unordered_set<long>({3, 5, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[2] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    long iCoarseCell = 0;

    splitNonConnectedCoarseCell(indCoarseElement, numberOfFineAgglomeratedCells,
                                iCoarseCell,
                                dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                dict_DistributionOfCardinalOfCoarseElements,
                                row_ptr, col_ind,
                                isFineCellAgglomerated,
                                fine_Cell_indices_To_Coarse_Cell_Indices);


    ASSERT_EQ(5, indCoarseElement);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({6, 7});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({3, 5, 8});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 4});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[4] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({3, 4});
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 2});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {3, 2, 4, 1, 2, 1, 0, 0, 1};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);

    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_3_isotropicCell_0_anisotropic) {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    unordered_set<long> listInitialCoarseCell({0, 1, 2, 3, 4, 5, 6, 7, 8});
    bool isFineCellAgglomerated[9] = {true, true, true, true, true, true, true, true, true};
    long fineCellIndicesToCoarseCellIndices[9] = {1, 0, 3, 1, 2, 3, 1, 3, 3};


    int thresholdCard = 2;  // wrong value in 2D but for test!
    long indCoarseCell = 4;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[2] = unordered_set<long>({4});
    dict_Coarse_Cells[3] = unordered_set<long>({2, 5, 7, 8});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 3, 6});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 7, 8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[9] = {true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 0, 2, 1, 2, 2, 1, 2, 2};
    for (int i = 0; i < 9; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_3_isotropicCell_4) {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    long fineCellIndicesToCoarseCellIndices[9] = {1, 2, 4, 1, 0, 4, 1, 3, 4};


    int thresholdCard = 4;  // wrong valu in 2D but for test!
    long indCoarseCell = 4;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({4});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    ASSERT_EQ(3, indCoarseCell);

    ASSERT_TRUE(dict_Coarse_Cells.empty());
    ASSERT_TRUE(dict_Card_Coarse_Cells.empty());
    ASSERT_TRUE(dict_DistributionOfCardinalOfCoarseElements.empty());

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 2, 4, 1, 3, 4, 1, 3, 4};
    for (int i = 0; i < 9; i++) {
//        cout << i << " " << fineCellIndicesToCoarseCellIndices[i] << endl;
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_Case_3) {

//    cout<<"call of test_splitNonConnectedCoarseCell_9_Squares_3_isotropicCell_0_anisotropic"<<endl;
    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    long fineCellIndicesToCoarseCellIndices[9] = {1, 0, 2, 1, 0, 2, 1, 2, 2};


    int thresholdCard = 2;  // wrong valu in 2D but for test!
    long indCoarseCell = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({1, 4});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({1, 4});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 0, 2, 1, 0, 2, 1, 2, 2};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_Too_small_Cell_size_2) {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    long fineCellIndicesToCoarseCellIndices[9] = {1, 0, 2, 1, 0, 2, 1, 2, 2};


    int thresholdCard = 2;  // wrong valu in 2D but for test!
    long indCoarseCell = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({1, 4});
    dict_Coarse_Cells[1] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 5, 7, 8});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 4, 5, 7, 8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[6] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[6] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);


    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {0, 1, 1, 0, 1, 1, 0, 1, 1};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_9_Squares_Too_small_Cell_size_2_case_2) {

    int nbCells = 9;

    long row_ptr[10] = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
    long col_ind[33] = {0, 1, 3, 0, 1,
                        2, 4, 1, 2, 5,
                        0, 3, 4, 6, 1,
                        3, 4, 5, 7, 2,
                        4, 5, 8, 3, 6,
                        7, 4, 6, 7, 8, 5, 7, 8};
    double values[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    long fineCellIndicesToCoarseCellIndices[9] = {2, 1, 0, 2, 1, 0, 2, 0, 0};


    int thresholdCard = 2;  // wrong valu in 2D but for test!
    long indCoarseCell = 3;
    long numberOfFineAgglomeratedCells = 9; // TODO usefull ???

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({1, 4});
    dict_Coarse_Cells[2] = unordered_set<long>({0, 3, 6});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    remove_Too_Small_Cells_v2(
            thresholdCard,
            fineCellIndicesToCoarseCellIndices,
            indCoarseCell,
            row_ptr, col_ind,
            values,
            dict_Coarse_Cells,
            dict_Card_Coarse_Cells,
            dict_DistributionOfCardinalOfCoarseElements);

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 3, 4, 6});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[9] = {1, 1, 0, 1, 1, 0, 1, 0, 0};
    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_12_Squares_8_isotropicCell_1_anisotropic) {
    //"""
    //9 squares (3*3)
    //4 is anisotropic
    //The biggest face area is between 1 and 4
    //isotropic fine cell 4 is added to anisotropic cell 2.
    //Coarse cell 6 is remove.
    //:return:
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 4, 0, 6, 4, 1, 3, 5, 1, 3, 5};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;// wrong value in 2D but for test!
    long indCoarseCell = 7;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[6] = unordered_set<long>({4});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[1] = unordered_set<long>({6});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;

    remove_Too_Small_Cells_v2(thresholdCard,
                              fineCellIndicesToCoarseCellIndices,
                              indCoarseCell,
                              row_ptr, col_ind,
                              values,
                              dict_Coarse_Cells,
                              dict_Card_Coarse_Cells,
                              dict_DistributionOfCardinalOfCoarseElements);
    ASSERT_EQ(6, indCoarseCell);
    ASSERT_TRUE(dict_Coarse_Cells.empty());
    ASSERT_TRUE(dict_Card_Coarse_Cells.empty());
    ASSERT_TRUE(dict_DistributionOfCardinalOfCoarseElements.empty());

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 4, 0, 2, 4, 1, 3, 5, 1, 3, 5};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_12_Squares_8_isotropicCell_1_anisotropic_case_2) {

    //"""
    //9 squares (3*3)
    //4 is anisotropic
    //The biggest face area is between 1 and 4
    //isotropic fine cell 4 is added to anisotropic cell 2.
    //Coarse cell 6 is remove.
    //:return:
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1.2, 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1.2, 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 4, 0, 6, 4, 1, 3, 5, 1, 3, 5};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;// wrong value in 2D but for test!
    long indCoarseCell = 7;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[6] = unordered_set<long>({4});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[1] = unordered_set<long>({6});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;

    remove_Too_Small_Cells_v2(thresholdCard,
                              fineCellIndicesToCoarseCellIndices,
                              indCoarseCell,
                              row_ptr, col_ind,
                              values,
                              dict_Coarse_Cells,
                              dict_Card_Coarse_Cells,
                              dict_DistributionOfCardinalOfCoarseElements);
    ASSERT_EQ(6, indCoarseCell);
    ASSERT_TRUE(dict_Coarse_Cells.empty());
    ASSERT_TRUE(dict_Card_Coarse_Cells.empty());
    ASSERT_TRUE(dict_DistributionOfCardinalOfCoarseElements.empty());

    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 4, 0, 3, 4, 1, 3, 5, 1, 3, 5};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, remove_Too_Small_Cells_12_Squares_9_isotropicCell) {
    //"""
    //9 squares (3*3)
    //4 is anisotropic
    //The biggest face area is between 1 and 4
    //isotropic fine cell 4 is added to anisotropic cell 2.
    //Coarse cell 6 is remove.
    //:return:
    //"""
    cout << "\n\n\n" << endl;
    cout << "============================>" << endl;
    cout << "\n\n\n" << endl;
    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 4,
                                                   0, 5, 4,
                                                   1, 3, 2,
                                                   1, 3, 2};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;// wrong value in 2D but for test!
    long indCoarseCell = 6;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 9});
    dict_Coarse_Cells[2] = unordered_set<long>({1, 8, 11});
    dict_Coarse_Cells[3] = unordered_set<long>({7, 10});
    dict_Coarse_Cells[4] = unordered_set<long>({2, 5});
    dict_Coarse_Cells[5] = unordered_set<long>({4});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[1] = unordered_set<long>({5});
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 1, 3, 4});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[2] = 4;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    remove_Too_Small_Cells_v2(thresholdCard,
                              fineCellIndicesToCoarseCellIndices,
                              indCoarseCell,
                              row_ptr, col_ind,
                              values,
                              dict_Coarse_Cells,
                              dict_Card_Coarse_Cells,
                              dict_DistributionOfCardinalOfCoarseElements);
    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2, 5, 8, 11});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({3, 4, 6, 7, 9, 10});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[6] = unordered_set<long>({0, 1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[6] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells) {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2, 0, 2, 2, 1, 1, 2, 1, 1, 2};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 3;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 7, 9, 10], 2: [2, 4, 5, 8, 11]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}


    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 8, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);
    ASSERT_EQ(3, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3, 4});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2, 5, 8, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 3;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 2, 0, 0, 2, 1, 1, 2, 1, 1, 2};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_2) {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2,
                                                   0, 2, 2,
                                                   1, 3, 3,
                                                   1, 1, 3};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 4;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 7, 9, 10], 2: [2, 4, 5, 8, 11]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}


    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 9, 10});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5});
    dict_Coarse_Cells[3] = unordered_set<long>({7, 8, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2, 3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 4;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);

    ASSERT_EQ(4, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 4, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({7, 9, 10});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 2, 5});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({3});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1, 2});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 0, 2, 0, 1, 3, 1, 1, 3};
    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_3) {

    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2,
                                                   0, 2, 2,
                                                   2, 2, 1,
                                                   1, 1, 1};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 3;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [8, 9, 10, 11], 2: [2, 4, 5, 6, 7]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({8, 9, 10, 11});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 6, 7});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);
//    cout<<"dict_Coarse_Cells[0]: [";
//    for(auto i :dict_Coarse_Cells[0]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"dict_Coarse_Cells[1]: [";
//    for(auto i :dict_Coarse_Cells[1]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;
//    cout<<"dict_Coarse_Cells[2]: [";
//    for(auto i :dict_Coarse_Cells[2]){
//        cout<<i<<", ";
//    }
//    cout<<"]"<<endl;

    ASSERT_EQ(3, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2, 3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({8, 9, 10, 11});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({4, 5, 6, 7});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 3;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1};
    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_4) {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 1,
                                                   0, 2, 1,
                                                   0, 2, 1,
                                                   3, 3, 3};
    // The coarse cell 0 is considered to be anisotropic.
    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 4;

    long numberOfFineAgglomeratedCells = 12;

//    dict_Coarse_Cells = {2: [1, 4, 7], 3: [9, 10, 11]}
//    dict_Card_Coarse_Cells = {3: {2, 3}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 2}


    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({2, 3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 2;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);

    ASSERT_EQ(4, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({2, 3});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 1, 0, 2, 1, 0, 2, 1, 3, 3, 3};
    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, makeSmallCellBigger_12_Squares_12_isotropic_Cells_case_5) {

    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 1, 1,
                                                   0, 2, 1,
                                                   0, 2, 1,
                                                   3, 3, 3};

    int thresholdCard = 2;  // wrong value in 2D but for test!
    int minCard = 4;  // wrong value in 2D but for test!
    int goalCard = 4;  // wrong value in 2D but for test!
    long indCoarseCell = 4;

    long numberOfFineAgglomeratedCells = 12;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 5, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({4, 7});
    dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[2] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 3});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});


    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    makeSmallCellBigger(dict_Coarse_Cells,
                        dict_Card_Coarse_Cells,
                        row_ptr,
                        col_ind,
                        dict_DistributionOfCardinalOfCoarseElements,
                        indCoarseCell,
                        numberOfFineAgglomeratedCells,
                        isFineCellAgglomerated,
                        fineCellIndicesToCoarseCellIndices,
                        minCard,
                        goalCard, thresholdCard,
                        false);

    ASSERT_EQ(3, indCoarseCell);
    ASSERT_EQ(12, numberOfFineAgglomeratedCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 4, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 5, 8});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({7, 9, 10, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 3;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 1, 1, 0, 0, 1, 0, 2, 1, 2, 2, 2};
    bool ref_isFineCellAgglomerated_tmp[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_isFineCellAgglomerated_tmp[i], isFineCellAgglomerated[i]);
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}


TEST(CoarseCellGraph_TestSuite, removeDeletedCoarseCells_Case_1) {

    long numberOfCoarseCells = 2;
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({3});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});

    long fineCellIndicesToCoarseCellIndices[4] = {1, 1, 1, 0};
    unordered_set<long> set_removedCoarseCells;

    removeDeletedCoarseCells_v3(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                fineCellIndicesToCoarseCellIndices, set_removedCoarseCells, numberOfCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, removeDeletedCoarseCells_Deletion_of_Coarse_Element) {


    long numberOfCoarseCells = 2;
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 0});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});

    long fineCellIndicesToCoarseCellIndices[3] = {1, 1, 1};
    unordered_set<long> set_removedCoarseCells({0});

    removeDeletedCoarseCells_v3(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                fineCellIndicesToCoarseCellIndices, set_removedCoarseCells, numberOfCoarseCells);
    ASSERT_EQ(1, numberOfCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, removeDeletedCoarseCells_Deletion_of_Coarse_Element_Case_2) {

    long numberOfCoarseCells = 8;
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({0});
    dict_Coarse_Cells[3] = unordered_set<long>({1});
    dict_Coarse_Cells[4] = unordered_set<long>({2});
    dict_Coarse_Cells[6] = unordered_set<long>({3});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({1, 3, 4, 6});

    long fineCellIndicesToCoarseCellIndices[4] = {1, 3, 4, 6};
    unordered_set<long> set_removedCoarseCells({0, 2, 5, 7});

    removeDeletedCoarseCells_v3(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                fineCellIndicesToCoarseCellIndices, set_removedCoarseCells, numberOfCoarseCells);
    ASSERT_EQ(4, numberOfCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({1});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({3});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0, 1, 2, 3});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, removeDeletedCoarseCells_Deletion_of_Coarse_Element_Case_3) {

    long numberOfCoarseCells = 7;
    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[1] = unordered_set<long>({0, 4, 9});
    dict_Coarse_Cells[3] = unordered_set<long>({3, 5, 7});
    dict_Coarse_Cells[4] = unordered_set<long>({2, 6, 8});
    dict_Coarse_Cells[6] = unordered_set<long>({1, 10});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({6});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1, 3, 4});

    long fineCellIndicesToCoarseCellIndices[11] = {1, 6, 4, 3, 1, 3, 4, 3, 4, 1, 6};
    unordered_set<long> set_removedCoarseCells({0, 2, 5});

    removeDeletedCoarseCells_v3(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                fineCellIndicesToCoarseCellIndices, set_removedCoarseCells, numberOfCoarseCells);

    ASSERT_EQ(4, numberOfCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 4, 9});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({3, 5, 7});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2, 6, 8});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({1, 10});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({3});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[11] = {0, 3, 2, 1, 0, 1, 2, 1, 2, 0, 3};
    for (int i = 0; i < 11; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}


TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells) {

    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2, 0, 2, 2, 1, 1, 2, 1, 1, 2};

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 7, 9, 10], 2: [2, 4, 5, 8, 11]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}

    long indCoarseCell = 3;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 8, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 2, 4, 5, 8, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[6] = unordered_set<long>({2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[6] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 2, 2, 1, 1, 2, 1, 1, 2};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells_Case_2) {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2,
                                                   0, 2, 2,
                                                   1, 3, 3,
                                                   1, 1, 3};



//            dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 9, 10], 2: [2, 4, 5], 3: [7, 8, 11]}
//            dict_Card_Coarse_Cells = {3: {0, 1, 2, 3}}
//            dict_DistributionOfCardinalOfCoarseElements = {3: 4}

//    dict_Coarse_Cells = {0: [0, 1, 3], 1: [6, 7, 9, 10], 2: [2, 4, 5, 8, 11]}
//    dict_Card_Coarse_Cells = {3: {0}, 4: {1}, 5: {2}}
//    dict_DistributionOfCardinalOfCoarseElements = {3: 1, 4: 1, 5: 1}

    long indCoarseCell = 4;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({6, 9, 10});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5});
    dict_Coarse_Cells[3] = unordered_set<long>({7, 8, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 1, 2, 3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 4;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);

    ASSERT_EQ(4, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 7, 9, 10});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 2, 4, 5});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({8, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 3});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 2, 0, 2, 2, 1, 1, 3, 1, 1, 3};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells_Case_3) {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 2,
                                                   0, 2, 2,
                                                   2, 2, 1,
                                                   1, 1, 1};

    long indCoarseCell = 3;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3});
    dict_Coarse_Cells[1] = unordered_set<long>({8, 9, 10, 11});
    dict_Coarse_Cells[2] = unordered_set<long>({2, 4, 5, 6, 7});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    dict_Card_Coarse_Cells[5] = unordered_set<long>({2});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    dict_DistributionOfCardinalOfCoarseElements[5] = 1;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({9, 10, 11});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({0, 1, 2, 3, 4, 5, 6, 7, 8});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[9] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[9] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells_Case_4) {

    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 2, 1,
                                                   0, 2, 1,
                                                   0, 2, 1,
                                                   3, 3, 3};

    long indCoarseCell = 4;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});


    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[3] = unordered_set<long>({2, 3});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[3] = 2;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);

    ASSERT_EQ(4, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[2] = unordered_set<long>({1, 4, 7});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({2, 3});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 2, 1, 0, 2, 1, 0, 2, 1, 3, 3, 3};
    for (int i = 0; i < 12; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Swap_12_Squares_12_isotropic_Cells_Case_5) {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 1, 1,
                                                   0, 2, 1,
                                                   0, 2, 1,
                                                   3, 3, 3};


    long indCoarseCell = 4;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 6});
    dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 5, 8});
    dict_Coarse_Cells[2] = unordered_set<long>({4, 7});
    dict_Coarse_Cells[3] = unordered_set<long>({9, 10, 11});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;

    dict_Card_Coarse_Cells[2] = unordered_set<long>({2});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 3});
    dict_Card_Coarse_Cells[4] = unordered_set<long>({1});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    dict_DistributionOfCardinalOfCoarseElements[4] = 1;

    agglomerate_Isotropic_Correction_Swap(dict_Coarse_Cells,
                                          dict_Card_Coarse_Cells,
                                          row_ptr,
                                          col_ind,
                                          dict_DistributionOfCardinalOfCoarseElements,
                                          indCoarseCell,
                                          nbCells,
                                          fineCellIndicesToCoarseCellIndices, true);

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 3, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({1, 2, 4, 5, 7, 8});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({9, 10, 11});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({0, 2});
    ref_dict_Card_Coarse_Cells[6] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[6] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 1, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, Correction_Too_Big_Cells_12_Squares_12_isotropic_Cells) {

    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 0,
                                                   0, 0, 2,
                                                   3, 1, 1,
                                                   1, 1, 1};

    int goalCard = 4;
    long indCoarseCell = 4;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_0;
    ref_dict_Card_Coarse_Cells_0[1] = unordered_set<long>({2, 3});
    ref_dict_Card_Coarse_Cells_0[5] = unordered_set<long>({0, 1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_0, dict_Card_Coarse_Cells);

    agglomerate_Isotropic_Correction_Too_Big_Cells(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                                   row_ptr, col_ind,
                                                   dict_DistributionOfCardinalOfCoarseElements,
                                                   fineCellIndicesToCoarseCellIndices,
                                                   indCoarseCell,
                                                   goalCard, false);

    ASSERT_EQ(4, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3, 4});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({7, 8, 10, 11});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({2, 5});
    ref_dict_Coarse_Cells[3] = unordered_set<long>({6, 9});
    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[2] = unordered_set<long>({2, 3});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0, 1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[2] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 2;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 2, 0, 0, 2, 3, 1, 1, 3, 1, 1};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
//        assert(fineCellIndicesToCoarseCellIndices[i] == ref_fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_Too_Big_Cells_12_Squares_12_isotropic_Cells_Case_2) {
    //"""
    //12 squares (4*3)
    //"""

    int nbCells = 12;

    long row_ptr[13] = {0, 3, 7, 10, 14, 19, 23, 27, 32, 36, 39, 43, 46};
    long col_ind[46] = {0, 1, 3, 0, 1, 2, 4, 1, 2, 5, 0, 3, 4, 6, 1, 3, 4, 5, 7, 2, 4, 5, 8, 3, 6,
                        7, 9, 4, 6, 7, 8, 10, 5, 7, 8, 11, 6, 9, 10, 7, 9, 10, 11, 8, 10, 11};
    double values[46] = {1., 0.1, 1., 0.1, 1., 0.1, 1.1, 0.1, 1., 1., 1., 1., 0.1, 1., 1.1,
                         0.1, 1., 0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 1., 0.1, 1.,
                         0.1, 1., 1., 0.1, 1., 1., 1., 1., 0.1, 1., 0.1, 1., 0.1, 1., 0.1,
                         1.,};


    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }
    long fineCellIndicesToCoarseCellIndices[12] = {0, 0, 1,
                                                   0, 1, 1,
                                                   1, 1, 1,
                                                   2, 2, 1};

    int goalCard = 4;
    long indCoarseCell = 3;

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

    agglomerate_Isotropic_Correction_Too_Big_Cells(dict_Coarse_Cells, dict_Card_Coarse_Cells,
                                                   row_ptr, col_ind,
                                                   dict_DistributionOfCardinalOfCoarseElements,
                                                   fineCellIndicesToCoarseCellIndices,
                                                   indCoarseCell,
                                                   goalCard, false);

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 3, 6});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({2, 4, 5, 7, 8});
    ref_dict_Coarse_Cells[2] = unordered_set<long>({9, 10, 11});

    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({2});
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[5] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[12] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 2, 2, 2};
    for (int i = 0; i < 12; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CoarseCellGraph_TestSuite, Correction_SplitTooBigCoarseCellInTwo_1_cell) {

    // MGridGen Test case
    long matrixAdj_CRS_row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long matrixAdj_CRS_col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                                      7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                                      14};
    double matrixAdj_CRS_values[51] = {6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
                                       6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
                                       2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
                                       3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
                                       3.16227766, 7.};

    int nbCells = 15;
    double volumes[15] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[15] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2};
    long fineCellIndicesToCoarseCellIndices[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>();
    listOfSeeds[3] = queue<long>();

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

    long indCoarseCell = 1;
    int minCard = 3;
    int maxCard = 5;
    bool checks = true;
    bool verbose = true;
    long numberOfFineAgglomeratedCells = nbCells;
    int nbSizes = 2;
    long sizes[2] = {16, 51};
    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 11, 14});

    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[12] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[12] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[15] = {0, 0, 0,
                                                             0, 0, 0,
                                                             1, 0, 0,
                                                             0, 0, 1,
                                                             0, 0, 1};
    for (int i = 0; i < 15; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }
    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_2;
    ref_dict_Coarse_Cells_2[0] = unordered_set<long>({0, 1, 2, 3, 4, 5, 8, 10, 13});
    ref_dict_Coarse_Cells_2[1] = unordered_set<long>({6, 11, 14});
    ref_dict_Coarse_Cells_2[2] = unordered_set<long>({7, 9, 12});
    ASSERT_EQ(ref_dict_Coarse_Cells_2, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_2;
    ref_dict_Card_Coarse_Cells_2[3] = unordered_set<long>({1, 2});
    ref_dict_Card_Coarse_Cells_2[9] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_2, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[3] = 2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[9] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_2, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[15] = {0, 0, 0,
                                                               0, 0, 0,
                                                               1, 2, 0,
                                                               2, 0, 1,
                                                               2, 0, 1};
    for (int i = 0; i < 15; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fineCellIndicesToCoarseCellIndices[i]);
    }

    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    ASSERT_EQ(4, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_3;
    ref_dict_Coarse_Cells_3[0] = unordered_set<long>({2, 4, 5, 8, 10, 13});
    ref_dict_Coarse_Cells_3[1] = unordered_set<long>({6, 11, 14});
    ref_dict_Coarse_Cells_3[2] = unordered_set<long>({7, 9, 12});
    ref_dict_Coarse_Cells_3[3] = unordered_set<long>({0, 1, 3});
    ASSERT_EQ(ref_dict_Coarse_Cells_3, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_3;
    ref_dict_Card_Coarse_Cells_3[3] = unordered_set<long>({1, 2, 3});
    ref_dict_Card_Coarse_Cells_3[6] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_3, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_3;
    ref_dict_DistributionOfCardinalOfCoarseElements_3[3] = 3;
    ref_dict_DistributionOfCardinalOfCoarseElements_3[6] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_3, dict_DistributionOfCardinalOfCoarseElements);

//    assert(indCoarseCell == 4);
//    assert(!dict_Coarse_Cells.empty());
//    assert(dict_Coarse_Cells.size() == 4);
//    assert(dict_Coarse_Cells[0].count(2) == 1);
//    assert(dict_Coarse_Cells[0].count(4) == 1);
//    assert(dict_Coarse_Cells[0].count(5) == 1);
//    assert(dict_Coarse_Cells[0].count(8) == 1);
//    assert(dict_Coarse_Cells[0].count(10) == 1);
//    assert(dict_Coarse_Cells[0].count(13) == 1);
//
//    assert(dict_Coarse_Cells[1].count(6) == 1);
//    assert(dict_Coarse_Cells[1].count(11) == 1);
//    assert(dict_Coarse_Cells[1].count(14) == 1);
//
//    assert(dict_Coarse_Cells[2].count(7) == 1);
//    assert(dict_Coarse_Cells[2].count(9) == 1);
//    assert(dict_Coarse_Cells[2].count(12) == 1);
//
//    assert(dict_Coarse_Cells[3].count(0) == 1);
//    assert(dict_Coarse_Cells[3].count(1) == 1);
//    assert(dict_Coarse_Cells[3].count(3) == 1);
//
//    assert(!dict_Card_Coarse_Cells.empty());
//    assert(dict_Card_Coarse_Cells.size() == 2);
//    assert(dict_Card_Coarse_Cells[3].count(1) == 1);
//    assert(dict_Card_Coarse_Cells[3].count(2) == 1);
//    assert(dict_Card_Coarse_Cells[3].count(3) == 1);
//    assert(dict_Card_Coarse_Cells[6].count(0) == 1);
//
//    assert(!dict_DistributionOfCardinalOfCoarseElements.empty());
//    assert(dict_DistributionOfCardinalOfCoarseElements[3] == 3);
//    assert(dict_DistributionOfCardinalOfCoarseElements[6] == 1);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_3[15] = {3, 3, 0,
                                                               3, 0, 0,
                                                               1, 2, 0,
                                                               2, 0, 1,
                                                               2, 0, 1};
    for (int i = 0; i < 15; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_3[i], fineCellIndicesToCoarseCellIndices[i]);
    }

}

TEST(CoarseCellGraph_TestSuite, Correction_SplitTooBigCoarseCellInTwo_1_cell_v2) {

    // MGridGen Test case
    long matrixAdj_CRS_row_ptr[16] = {0, 3, 6, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 46, 49, 51};
    long matrixAdj_CRS_col_ind[51] = {0, 1, 2, 0, 1, 3, 0, 2, 5, 6, 1, 3, 6, 4, 5, 7, 2, 4, 5, 8, 2, 3, 6, 11, 4,
                                      7, 8, 9, 5, 7, 8, 10, 7, 9, 10, 12, 8, 9, 10, 13, 6, 11, 13, 14, 9, 12, 10, 11, 13, 11,
                                      14};
    double matrixAdj_CRS_values[51] = {6.82842712, 2., 2., 2., 4.23606798, 1., 2., 4., 2.23606798, 2.23606798, 1., 4., 2.23606798,
                                       6.82842712, 2., 2., 2.23606798, 2., 2., 1., 2.23606798, 2.23606798, 4., 2., 2., 2., 1.,
                                       2.23606798, 1., 1., 1., 1.41421356, 2.23606798, 4., 2., 3.60555128, 1.41421356, 2., 2.,
                                       3.16227766, 2., 6., 3.16227766, 3.16227766, 3.60555128, 11., 3.16227766, 3.16227766, 8.,
                                       3.16227766, 7.};

    int nbCells = 15;
    double volumes[15] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    int isOnBnd[15] = {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2};
    long fineCellIndicesToCoarseCellIndices[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool *isFineCellAgglomerated = new bool[nbCells];
    for (int i = 0; i < nbCells; i++) {
        isFineCellAgglomerated[i] = true;
    }

    vector<queue<long>> listOfSeeds(4);
    listOfSeeds[0] = queue<long>();
    listOfSeeds[1] = queue<long>();
    listOfSeeds[2] = queue<long>();
    listOfSeeds[3] = queue<long>();

    unordered_map<long, unordered_set<long>> dict_Coarse_Cells;
    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;

    compute_Dicts_From_FineCellIndicesToCoarseCellIndices(nbCells, fineCellIndicesToCoarseCellIndices, dict_Coarse_Cells, dict_Card_Coarse_Cells, dict_DistributionOfCardinalOfCoarseElements);

    long indCoarseCell = 1;
    int minCard = 5;
    int maxCard = 5;
    bool checks = true;
    bool verbose = false;
    long numberOfFineAgglomeratedCells = nbCells;
    int nbSizes = 2;
    long sizes[2] = {16, 51};
    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    ASSERT_EQ(2, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells;
    ref_dict_Coarse_Cells[0] = unordered_set<long>({0, 1, 2, 3, 4, 5, 7, 8, 9, 12});
    ref_dict_Coarse_Cells[1] = unordered_set<long>({6, 10, 11, 13, 14});

    ASSERT_EQ(ref_dict_Coarse_Cells, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[5] = unordered_set<long>({1});
    ref_dict_Card_Coarse_Cells[10] = unordered_set<long>({0});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements = {{5,  1},
                                                                                {10, 1}};
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices[15] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1};

    for (int i = 0; i < 15; i++) {
        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices[i], fineCellIndicesToCoarseCellIndices[i]);
    }

    agglomerate_Isotropic_Correction_SplitTooBigCoarseCellInTwo(nbSizes,
                                                                sizes,
                                                                listOfSeeds,
                                                                dict_Coarse_Cells,
                                                                dict_Card_Coarse_Cells,
                                                                matrixAdj_CRS_row_ptr,
                                                                matrixAdj_CRS_col_ind,
                                                                matrixAdj_CRS_values, volumes,
                                                                dict_DistributionOfCardinalOfCoarseElements,
                                                                indCoarseCell,
                                                                fineCellIndicesToCoarseCellIndices,
                                                                numberOfFineAgglomeratedCells,
                                                                isFineCellAgglomerated,
                                                                isOnBnd,
                                                                minCard,
                                                                maxCard,
                                                                checks,
                                                                verbose);

    ASSERT_EQ(3, indCoarseCell);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Cells_2;
    ref_dict_Coarse_Cells_2[0] = unordered_set<long>({4, 7, 8, 9, 12});
    ref_dict_Coarse_Cells_2[1] = unordered_set<long>({6, 10, 11, 13, 14});
    ref_dict_Coarse_Cells_2[2] = unordered_set<long>({0, 1, 2, 3, 5});

    ASSERT_EQ(ref_dict_Coarse_Cells_2, dict_Coarse_Cells);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells_2;
    ref_dict_Card_Coarse_Cells_2[5] = unordered_set<long>({0, 1, 2});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells_2, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements_2;
    ref_dict_DistributionOfCardinalOfCoarseElements_2[5] = 3;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements_2, dict_DistributionOfCardinalOfCoarseElements);

    long ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[15] = {2, 2, 2, 2, 0, 2, 1, 0, 0, 0, 1, 1, 0, 1, 1};

    for (int i = 0; i < 15; i++) {

        ASSERT_EQ(ref_fine_Cell_indices_To_Coarse_Cell_Indices_2[i], fineCellIndicesToCoarseCellIndices[i]);
    }
}

TEST(CC_TestSuite, SwapFineCell_Case_1) {

    int iLevel = 0;
    long iFineCell = 0;
    long iOrigineCoarseCell = 0;
    long iDestinationCoarseCell = 1;

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({0, 3});
    dict_Coarse_Elem[1] = unordered_set<long>({1, 2});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[2] = unordered_set<long>({0, 1});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[2] = 2;

    long fine_Cell_indices_To_Coarse_Cell_Indices[4] = {0, 1, 1, 0};

    unordered_set<long> set_removedCoarseCells = swapFineCell(iFineCell, iOrigineCoarseCell, iDestinationCoarseCell,
                                                              dict_Coarse_Elem,
                                                              dict_Card_Coarse_Cells,
                                                              dict_DistributionOfCardinalOfCoarseElements,
                                                              fine_Cell_indices_To_Coarse_Cell_Indices);
//    assert(set_removedCoarseCells.size() == 0);
    ASSERT_EQ(0, set_removedCoarseCells.size());

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[0] = unordered_set<long>({3});
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    ref_dict_Card_Coarse_Cells[3] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    ref_dict_DistributionOfCardinalOfCoarseElements[3] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);

    long ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};
    for (int i = 0; i < 4; i++) {
//        assert(ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[i] == fine_Cell_indices_To_Coarse_Cell_Indices[i]);
        ASSERT_EQ(ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}

TEST(CC_TestSuite, SwapFineCell_Deletion_of_Coarse_Element) {

    int iLevel = 0;
    long iFineCell = 3;
    long iOrigineCoarseCell = 0;
    long iDestinationCoarseCell = 1;

    unordered_map<long, unordered_set<long>> dict_Coarse_Elem;
    dict_Coarse_Elem[0] = unordered_set<long>({3});
    dict_Coarse_Elem[1] = unordered_set<long>({0, 1, 2});

    unordered_map<int, unordered_set<long>> dict_Card_Coarse_Cells;
    dict_Card_Coarse_Cells[1] = unordered_set<long>({0});
    dict_Card_Coarse_Cells[3] = unordered_set<long>({1});

    unordered_map<int, long> dict_DistributionOfCardinalOfCoarseElements;
    dict_DistributionOfCardinalOfCoarseElements[1] = 1;
    dict_DistributionOfCardinalOfCoarseElements[3] = 1;

    long fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 0};

    unordered_set<long> set_removedCoarseCells = swapFineCell(iFineCell, iOrigineCoarseCell, iDestinationCoarseCell,
                                                              dict_Coarse_Elem,
                                                              dict_Card_Coarse_Cells,
                                                              dict_DistributionOfCardinalOfCoarseElements,
                                                              fine_Cell_indices_To_Coarse_Cell_Indices);
//    assert(set_removedCoarseCells.size() == 1);
//    assert(set_removedCoarseCells.count(0) == 1);
    unordered_set<long> ref_set_removedCoarseCells = unordered_set<long>({0});
    ASSERT_EQ(ref_set_removedCoarseCells, set_removedCoarseCells);

    unordered_map<long, unordered_set<long>> ref_dict_Coarse_Elem;
    ref_dict_Coarse_Elem[1] = unordered_set<long>({1, 2, 0, 3});
    ASSERT_EQ(ref_dict_Coarse_Elem, dict_Coarse_Elem);

    unordered_map<int, unordered_set<long>> ref_dict_Card_Coarse_Cells;
    ref_dict_Card_Coarse_Cells[4] = unordered_set<long>({1});
    ASSERT_EQ(ref_dict_Card_Coarse_Cells, dict_Card_Coarse_Cells);

    unordered_map<int, long> ref_dict_DistributionOfCardinalOfCoarseElements;
    ref_dict_DistributionOfCardinalOfCoarseElements[4] = 1;
    ASSERT_EQ(ref_dict_DistributionOfCardinalOfCoarseElements, dict_DistributionOfCardinalOfCoarseElements);


    long ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[4] = {1, 1, 1, 1};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(ref_results_fine_Cell_indices_To_Coarse_Cell_Indices[i], fine_Cell_indices_To_Coarse_Cell_Indices[i]);
    }
}
