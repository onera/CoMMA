#include "../CoMMA_lib/Util_graph.h"
#include "Triconnected_graph_C.h"
#include "Triconnected_graph_C_2_BCC.h"
#include "Triconnected_graph_C_3_BCC.h"
#include "Triconnected_graph_D.h"
#include "Triconnected_graph_E.h"
#include "Triconnected_graph_F.h"
#include "Triconnected_graph_G.h"
#include "Triconnected_graph_OGDF_original_connectedGraph_13_16.h"
#include "Triconnected_graph_OGDF_100_200.h"
#include "Triconnected_graph_connected_hexahedra.h"
#include "Triconnected_graph_MGridGen_biconnected.h"
#include "Triconnected_graph_bug_cube.h"
#include "Box_5x5x5_Dual_Graph.h"
#include "gtest/gtest.h"

TEST_F(Triconnected_graph_C, BCC_NR_Case_Graph_C) {

    ASSERT_EQ(14, (*g).nb_of_nodes);
    ASSERT_EQ(14, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {2, 3, 4, 7, 8, 9};
    ASSERT_EQ(1, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());

}

TEST_F(Triconnected_graph_C_2_BCC, BCC_NR_Case_Graph_C_2_BCC) {

    ASSERT_EQ(6, (*g).nb_of_nodes);
    ASSERT_EQ(7, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {3, 4, 5};
    unordered_set<long> ref_s_component_1 = {0, 1, 2};
    ASSERT_EQ(2, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());
    ASSERT_EQ(ref_s_component_1, component.back());


}

TEST_F(Triconnected_graph_C_3_BCC, BCC_NR_Case_Graph_C_3_BCC) {

    ASSERT_EQ(9, (*g).nb_of_nodes);
    ASSERT_EQ(11, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {6, 7, 8};
    unordered_set<long> ref_s_component_1 = {3, 4, 5};
    unordered_set<long> ref_s_component_2 = {0, 1, 2};
    ASSERT_EQ(3, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());
    component.pop_front();
    ASSERT_EQ(ref_s_component_1, component.front());
    ASSERT_EQ(ref_s_component_2, component.back());
}

TEST_F(Triconnected_graph_D, BCC_NR_Case_Graph_D) {

    ASSERT_EQ(5, (*g).nb_of_nodes);
    ASSERT_EQ(5, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component = {1, 2, 3, 4};
    ASSERT_EQ(1, component.size());
    ASSERT_EQ(ref_s_component, component.front());

}

TEST_F(Triconnected_graph_E, BCC_NR_Case_Graph_E) {

    ASSERT_EQ(5, (*g).nb_of_nodes);
    ASSERT_EQ(6, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {0, 3, 4};
    unordered_set<long> ref_s_component_1 = {0, 1, 2};
    ASSERT_EQ(2, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());
    ASSERT_EQ(ref_s_component_1, component.back());

}

TEST_F(Triconnected_graph_F, BCC_NR_Case_Graph_F) {

    ASSERT_EQ(6, (*g).nb_of_nodes);
    ASSERT_EQ(7, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {1, 2, 3};
    unordered_set<long> ref_s_component_1 = {1, 4, 5};
    ASSERT_EQ(2, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());
    ASSERT_EQ(ref_s_component_1, component.back());

}

TEST_F(Triconnected_graph_G, BCC_NR) {

    ASSERT_EQ(8, (*g).nb_of_nodes);
    ASSERT_EQ(12, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {0, 2, 3, 4, 5, 6, 7};
    ASSERT_EQ(1, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());

}

TEST_F(Triconnected_graph_OGDF_original_connectedGraph_13_16, BCC_NR) {

    ASSERT_EQ(13, (*g).nb_of_nodes);
    ASSERT_EQ(16, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {0, 1, 2, 5, 6, 7, 9, 10};
    ASSERT_EQ(1, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());
}

TEST_F(Triconnected_graph_OGDF_100_200, BCC_NR) {

    ASSERT_EQ(100, (*g).nb_of_nodes);
    ASSERT_EQ(200, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                                             26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49,
                                             50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 66, 67, 68, 69, 70, 71, 72, 73, 74,
                                             76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 93, 94, 95, 96, 97, 99};
    ASSERT_EQ(1, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());
}

TEST_F(Triconnected_graph_connected_hexahedra, BCC_NR) {
    /**
     * Box_5x5x5
     */
    ASSERT_EQ(64, (*g).nb_of_nodes);
    ASSERT_EQ(144, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_TRUE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                             41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};
    ASSERT_EQ(1, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());
}

TEST_F(Box_5x5x5_Dual_Graph, BCC_NR_on_subgraph) {

    // We build a subgraph to have the 3-neighbouring cells of seed 0.
    unordered_set<long> s_of_node = {0, 1, 2, 3, 4, 5, 6, 8, 9, 12,
                                     16, 17, 18, 20, 21, 24, 32, 33, 36, 48};

    vector<long> row_ptr_l;
    vector<long> col_ind_l;
    vector<double> values_l;
    vector<long> g_to_l;
    vector<long> l_to_g;

    (*g).compute_local_crs_subgraph_from_global_crs(s_of_node,
                                                    row_ptr_l,
                                                    col_ind_l,
                                                    values_l,
                                                    g_to_l,
                                                    l_to_g);

    Triconnected_graph tri_g(row_ptr_l,
                             col_ind_l,
                             values_l);

    ASSERT_EQ(20, tri_g.nb_of_nodes);
    ASSERT_EQ(30, tri_g.nb_of_edges);

    ASSERT_TRUE(tri_g.is_connected());

    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR(tri_g, component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_g_0 = {0, 1, 2, 4, 5, 6, 8, 9, 16, 17, 18, 20, 21, 24, 32, 33, 36};
    unordered_set<long> ref_s_component_l_0 = {};
    for (const int i: ref_s_component_g_0) {
        ref_s_component_l_0.insert(g_to_l[i]);
    }

    ASSERT_EQ(1, component.size());
    ASSERT_EQ(ref_s_component_l_0, component.front());
}

TEST_F(Triconnected_graph_MGridGen_biconnected, BCC_NR) {

    ASSERT_EQ(13, (*g).nb_of_nodes);
    ASSERT_EQ(16, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());
    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR((*g), component);

    ASSERT_TRUE(is_biconnected);
    unordered_set<long> ref_s_component_0 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    ASSERT_EQ(1, component.size());
    ASSERT_EQ(ref_s_component_0, component.front());
}

TEST_F(Triconnected_graph_bug_cube, BCC_NR_on_subgraph) {

    vector<double> v_volumes(nb_nodes, 1.0);
    unordered_map<long, int> d_is_on_bnd;
    Dual_Graph dg(nb_nodes,
                  v_row_ptr,
                  v_col_ind,
                  v_values,
                  v_volumes,
                  d_is_on_bnd);

//    // We build a subgraph to have the 3-neighbouring cells of seed 0.
//    unordered_set<long> s_of_node;
//    for (int i=0; i<nb_nodes; i++)
//    {
//        s_of_node.insert(i);
//    }
//
//    vector<long> row_ptr_l;
//    vector<long> col_ind_l;
//    vector<double> values_l;
//    vector<long> g_to_l;
//
//    dg.compute_local_crs_subgraph_from_global_crs(s_of_node,
//                                                  row_ptr_l,
//                                                  col_ind_l,
//                                                  values_l,
//                                                  g_to_l);
//
//    Triconnected_graph tri_g(row_ptr_l,
//                             col_ind_l,
//                             values_l);

    Triconnected_graph tri_g(v_row_ptr,
                             v_col_ind,
                             v_values);

    ASSERT_EQ(39, tri_g.nb_of_nodes);
    ASSERT_EQ(60, tri_g.nb_of_edges);

    ASSERT_TRUE(tri_g.is_connected());

    list<unordered_set<long>> component;
    bool is_biconnected = BCC_NR(tri_g, component);

    ASSERT_FALSE(is_biconnected);
    unordered_set<long> ref_s_component_g_0 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20,
                                               21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,};
//    unordered_set<long> ref_s_component_l_0 = {};
//    for (const int i: ref_s_component_g_0) {
//        ref_s_component_l_0.insert(g_to_l[i]);
//    }

    ASSERT_EQ(1, component.size());
    cout << "{";
    for (const int i: component.front()) {
        cout << i << ", ";
    }
    cout << "}" << endl;
    cout << "size " << component.front().size() << endl;
    ASSERT_EQ(ref_s_component_g_0, component.front());
}

