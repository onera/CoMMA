#include "../CoMMA_lib/Triconnected_graph.h"
#include "Triconnected_graph_A.h"
#include "Triconnected_graph_H.h"
#include "Triconnected_graph_MGridGen.h"
#include "gtest/gtest.h"


TEST(Triconnected_graph_TestSuite, Constructor) {

    vector<long> row_ptr = {0, 1, 2};
    vector<long> col_ind = {1, 0};
    vector<double> values = {4.0, 4.0};

    Triconnected_graph graph = Triconnected_graph(row_ptr, col_ind, values);

    ASSERT_EQ(row_ptr, graph.row_ptr);
    ASSERT_EQ(col_ind, graph.col_ind);
    ASSERT_EQ(values, graph.values);
    ASSERT_EQ(2, graph.nb_of_nodes);
    ASSERT_EQ(1, graph.nb_of_edges);

    vector<unsigned int> ref_m_degree = {1, 1};
    ASSERT_EQ(ref_m_degree, graph.m_degree);

    // edgeadj, 0
    ASSERT_EQ(0, graph.adj_edges[0].front().source);
    ASSERT_EQ(1, graph.adj_edges[0].front().target);
    ASSERT_EQ(false, graph.adj_edges[0].front().start);
    ASSERT_EQ(0, graph.adj_edges[0].front().index);

    // edge_adj 1
    ASSERT_EQ(0, graph.adj_edges[1].front().source);
    ASSERT_EQ(1, graph.adj_edges[1].front().target);
    ASSERT_EQ(false, graph.adj_edges[1].front().start);
    ASSERT_EQ(0, graph.adj_edges[1].front().index);

}

TEST_F(Triconnected_graph_A, is_connected) {
    ASSERT_EQ(15, (*g).nb_of_nodes);
    ASSERT_EQ(22, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());

    vector<long> v_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    ASSERT_TRUE((*g).is_connected(v_fc));

    v_fc = {0, 1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13, 14};
    ASSERT_TRUE((*g).is_connected(v_fc));

    v_fc = {0, 1, 3, 4, 7, 8, 9, 10, 11, 12, 13, 14};
    ASSERT_FALSE((*g).is_connected(v_fc));

    v_fc = {0, 1, 2, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14};
    ASSERT_FALSE((*g).is_connected(v_fc));

    v_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 14};
    ASSERT_FALSE((*g).is_connected(v_fc));
}

TEST_F(Triconnected_graph_MGridGen, is_connected) {
    ASSERT_EQ(15, (*g).nb_of_nodes);
    ASSERT_EQ(18, (*g).nb_of_edges);

    ASSERT_TRUE((*g).is_connected());

    vector<long> v_fc = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    ASSERT_TRUE((*g).is_connected(v_fc));

    v_fc = {0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14};
    ASSERT_TRUE((*g).is_connected(v_fc));

    v_fc = {0, 1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13, 14};
    ASSERT_FALSE((*g).is_connected(v_fc));

    v_fc = {0, 1, 2, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14};
    ASSERT_FALSE((*g).is_connected(v_fc));

    v_fc = {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14};
    ASSERT_FALSE((*g).is_connected(v_fc));
}

TEST_F(Triconnected_graph_A, build_dfs_tree) {

    vector<int> number_dfs(nb_nodes, -1.0);  // discovery times
    vector<long> parent(nb_nodes, -1.0);  // parents in the dfs tree
    vector<long> childNr(nb_nodes, 0.0);
    list<long> revS;  // nodes of the dfs tree in reverse order

    long root = 0;
    long numberOfVisitedNodes = (*g).build_dfs_tree(root, number_dfs, parent, childNr, revS);
    vector<int> ref_number_dfs = {1, 12, 11, 10, 9, 2, 3, 4, 13, 8, 15, 14, 5, 6, 7};
    vector<long> ref_parent = {-1, 2, 3, 4, 9, 0, 5, 6, 3, 14, 11, 12, 7, 12, 13};
    vector<long> ref_childNr = {1, 0, 1, 2, 1, 1, 1, 1, 0, 1, 0, 1, 2, 1, 1};
    list<long> ref_revS = {0, 5, 6, 7, 12, 13, 14, 9, 4, 3, 2, 1, 8, 11, 10};

    ASSERT_EQ(ref_number_dfs, number_dfs);
    ASSERT_EQ(ref_parent, parent);
    ASSERT_EQ(ref_childNr, childNr);
    ASSERT_EQ(ref_revS, revS);
    ASSERT_EQ(15, numberOfVisitedNodes);
}

TEST_F(Triconnected_graph_A, is_Biconnected_And_CutVertices) {

    list<long> cut_vertices;
    bool is_biconnected = (*g).is_Biconnected_And_CutVertices(cut_vertices);
    ASSERT_TRUE(is_biconnected);
    ASSERT_TRUE(cut_vertices.empty());
}

TEST_F(Triconnected_graph_MGridGen, build_dfs_tree) {

    vector<int> number_dfs(nb_nodes, -1.0);  // discovery times
    vector<long> parent(nb_nodes, -1.0);  // parents in the dfs tree
    vector<long> childNr(nb_nodes, 0.0);
    list<long> revS;  // nodes of the dfs tree in reverse order

    long root = 0;
    long numberOfVisitedNodes = (*g).build_dfs_tree(root, number_dfs, parent, childNr, revS);
    vector<int> ref_number_dfs = {1, 5, 2, 4, 11, 12, 3, 10, 13, 9, 8, 6, 14, 7, 15};
    vector<long> ref_parent = {-1, 3, 0, 6, 7, 4, 2, 9, 5, 10, 13, 6, 9, 11, 11};
    vector<long> ref_childNr = {1, 0, 1, 1, 1, 1, 2, 1, 0, 2, 1, 2, 0, 1, 0};
    list<long> ref_revS = {0, 2, 6, 3, 1, 11, 13, 10, 9, 7, 4, 5, 8, 12, 14};

    ASSERT_EQ(ref_number_dfs, number_dfs);
    ASSERT_EQ(ref_parent, parent);
    ASSERT_EQ(ref_childNr, childNr);
    ASSERT_EQ(ref_revS, revS);
    ASSERT_EQ(15, numberOfVisitedNodes);
}

TEST_F(Triconnected_graph_MGridGen, is_Biconnected_And_CutVertices) {

    list<long> cut_vertices;
    bool is_biconnected = (*g).is_Biconnected_And_CutVertices(cut_vertices);
    ASSERT_FALSE(is_biconnected);
    list<long> ref_cut_vertices = {9, 11};
    ASSERT_EQ(ref_cut_vertices, cut_vertices);
}

TEST_F(Triconnected_graph_H, computeTriconnectivity_partial_test_1) {

    ASSERT_EQ(8, (*g).nb_of_nodes);
    ASSERT_EQ(12, (*g).nb_of_edges);

    (*g).computeTriconnectivity(1);
    vector<int> ref_m_number_dfs = {0, 1, 2, 3, 5, 6, 7, 4};
    ASSERT_EQ(ref_m_number_dfs, (*g).m_number_dfs);

    vector<long> ref_m_low_pt_1 = {0, 0, 0, 0, 0, 1, 2, 0};
    ASSERT_EQ(ref_m_low_pt_1, (*g).m_low_pt_1);

    vector<long> ref_m_low_pt_2 = {0, 1, 1, 1, 1, 2, 4, 1};
    ASSERT_EQ(ref_m_low_pt_2, (*g).m_low_pt_2);

    vector<long> ref_m_FATHER = {-1, 0, 1, 2, 7, 4, 5, 3};
    ASSERT_EQ(ref_m_FATHER, (*g).m_FATHER);

    vector<int> ref_m_NumberOfDescendants = {8, 7, 6, 5, 3, 2, 1, 4};
    ASSERT_EQ(ref_m_NumberOfDescendants, (*g).m_NumberOfDescendants);

    Edge ref_e_1(0, 1, 0, 3);
    ASSERT_EQ(ref_e_1, (*g).m_TREE_ARC[1]);
    ASSERT_EQ(ref_e_1.type, (*g).m_TREE_ARC[1].type);
    ASSERT_EQ(ref_e_1.index, (*g).m_TREE_ARC[1].index);

    Edge ref_e_2(1, 2, 3, 3);
    ASSERT_EQ(ref_e_2, (*g).m_TREE_ARC[2]);
    ASSERT_EQ(ref_e_2.type, (*g).m_TREE_ARC[2].type);
    ASSERT_EQ(ref_e_2.index, (*g).m_TREE_ARC[2].index);


    Edge ref_e_3(2, 3, 5, 3);
    ASSERT_EQ(ref_e_3, (*g).m_TREE_ARC[3]);
    ASSERT_EQ(ref_e_3.type, (*g).m_TREE_ARC[3].type);
    ASSERT_EQ(ref_e_3.index, (*g).m_TREE_ARC[3].index);

    Edge ref_e_4(4, 7, 9, 3);
    ASSERT_EQ(ref_e_4, (*g).m_TREE_ARC[4]);
    ASSERT_EQ(ref_e_4.type, (*g).m_TREE_ARC[4].type);
    ASSERT_EQ(ref_e_4.index, (*g).m_TREE_ARC[4].index);

    Edge ref_e_5(4, 5, 8, 3);
    ASSERT_EQ(ref_e_5, (*g).m_TREE_ARC[5]);
    ASSERT_EQ(ref_e_5.type, (*g).m_TREE_ARC[5].type);
    ASSERT_EQ(ref_e_5.index, (*g).m_TREE_ARC[5].index);

    Edge ref_e_6(5, 6, 10, 3);
    ASSERT_EQ(ref_e_6, (*g).m_TREE_ARC[6]);
    ASSERT_EQ(ref_e_6.type, (*g).m_TREE_ARC[6].type);
    ASSERT_EQ(ref_e_6.index, (*g).m_TREE_ARC[6].index);

    Edge ref_e_7(3, 7, 7, 3);
    ASSERT_EQ(ref_e_7, (*g).m_TREE_ARC[7]);
    ASSERT_EQ(ref_e_7.type, (*g).m_TREE_ARC[7].type);
    ASSERT_EQ(ref_e_7.index, (*g).m_TREE_ARC[7].index);
}
