#include "Tests_Triconnected_graph.h"

void check_edge(const Edge *e, const Edge ref) {
    ASSERT_EQ(ref.source, (*e).source);
    ASSERT_EQ(ref.target, (*e).target);
    ASSERT_EQ(ref.type, (*e).type);
    ASSERT_EQ(ref.index, (*e).index);

}

void check_m_TREE_ARC(const Triconnected_graph g, vector<Edge> v_ref) {
    ASSERT_EQ(g.nb_of_nodes, v_ref.size());
    for (int i = 1; i < g.nb_of_nodes; i++) {
        check_edge(g.m_TREE_ARC[i], v_ref[i]);
    }
}

void check_edges(const Triconnected_graph g, vector<Edge> v_ref) {
    ASSERT_EQ(g.nb_of_edges, v_ref.size());
    for (int i = 0; i < g.nb_of_edges; i++) {
        cout << "i=" << i << "\t" << (*g.edges[i]).source << " " << (*g.edges[i]).target << " " << v_ref[i].source << " " << v_ref[i].target << endl;
        check_edge(g.edges[i], v_ref[i]);
    }
}

void check_m_A(const Triconnected_graph g, vector<list<Edge>> v_ref) {
    bool verbose = true;
    ASSERT_EQ(g.nb_of_nodes, v_ref.size());
    for (int i = 0; i < g.nb_of_nodes; i++) {
        if (verbose) {
            cout << "i=" << i << endl;
        }

        ASSERT_EQ(g.m_A[i].size(), v_ref[i].size());
        list<Edge>::iterator it_ref = v_ref[i].begin();
        int count = 0;
        for (list<Edge *>::const_iterator it = g.m_A[i].begin(); it != g.m_A[i].end(); ++it) {
            if (verbose) {
                cout << "\tj=" << count;
            }
            check_edge(*it, *it_ref);
            it_ref++;
            count++;
        }
        if (verbose) {
            cout << endl;
        }
    }
}


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

    Edge *e_0 = graph.adj_edges[0].front();
    Edge *e_1 = graph.adj_edges[1].front();

    ASSERT_EQ(0, (*e_0).source);
    ASSERT_EQ(1, (*e_0).target);
    ASSERT_EQ(false, (*e_0).start);
    ASSERT_EQ(0, (*e_0).index);

    // edge_adj 1
    ASSERT_EQ(0, (*e_1).source);
    ASSERT_EQ(1, (*e_1).target);
    ASSERT_EQ(false, (*e_1).start);
    ASSERT_EQ(0, (*e_1).index);

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

TEST_F(Triconnected_graph_OGDF_original_connectedGraph_13_16, is_Biconnected_And_CutVertices) {
//    original_connectedGraph_13_16
    list<long> cut_vertices;
    bool is_biconnected = (*g).is_Biconnected_And_CutVertices(cut_vertices);
    ASSERT_FALSE(is_biconnected);
    list<long> ref_cut_vertices = {4, 2, 2, 6, 6};
    ASSERT_EQ(ref_cut_vertices, cut_vertices);
}

TEST_F(Triconnected_graph_OGDF_original_connectedGraph_13_16, build_dfs_tree) {

    vector<int> number_dfs(nb_nodes, -1.0);  // discovery times
    vector<long> parent(nb_nodes, -1.0);  // parents in the dfs tree
    vector<long> childNr(nb_nodes, 0.0);
    list<long> revS;  // nodes of the dfs tree in reverse order

    long root = 0;
    long numberOfVisitedNodes = (*g).build_dfs_tree(root, number_dfs, parent, childNr, revS);
    vector<int> ref_number_dfs = {1, 8, 10, 13, 11, 9, 5, 2, 12, 3, 4, 7, 6};
    vector<long> ref_parent = {-1, 6, 5, 2, 2, 7, 10, 0, 4, 7, 9, 6, 6};
    vector<long> ref_childNr = {1, 0, 2, 0, 1, 1, 3, 2, 0, 1, 1, 0, 0};
    list<long> ref_revS = {0, 7, 9, 10, 6, 12, 11, 1, 5, 2, 4, 8, 3};

    ASSERT_EQ(ref_number_dfs, number_dfs);
    ASSERT_EQ(ref_parent, parent);
    ASSERT_EQ(ref_childNr, childNr);
    ASSERT_EQ(ref_revS, revS);
    ASSERT_EQ(13, numberOfVisitedNodes);
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

    list<unordered_set<long>> triconnectedComponents = {};
    (*g).computeTriconnectivity(triconnectedComponents, 1);

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

    vector<Edge> ref_m_TREE_ARC(nb_nodes);
    ref_m_TREE_ARC[1] = Edge(0, 1, 0, 3);
    ref_m_TREE_ARC[2] = Edge(1, 2, 3, 3);
    ref_m_TREE_ARC[3] = Edge(2, 3, 5, 3);
    ref_m_TREE_ARC[4] = Edge(4, 7, 9, 3);
    ref_m_TREE_ARC[5] = Edge(4, 5, 8, 3);
    ref_m_TREE_ARC[6] = Edge(5, 6, 10, 3);
    ref_m_TREE_ARC[7] = Edge(3, 7, 7, 3);

    check_m_TREE_ARC((*g), ref_m_TREE_ARC);


    vector<Edge> ref_edges(nb_edges);
    ref_edges[0] = Edge(0, 1, 0, 3);
    ref_edges[1] = Edge(0, 3, 1, 1);
    ref_edges[2] = Edge(0, 4, 2, 1);
    ref_edges[3] = Edge(1, 2, 3, 3);
    ref_edges[4] = Edge(1, 5, 4, 1);
    ref_edges[5] = Edge(2, 3, 5, 3);
    ref_edges[6] = Edge(2, 6, 6, 1);
    ref_edges[7] = Edge(3, 7, 7, 3);
    ref_edges[8] = Edge(4, 5, 8, 3);
    ref_edges[9] = Edge(4, 7, 9, 3);
    ref_edges[10] = Edge(5, 6, 10, 3);
    ref_edges[11] = Edge(6, 7, 11, 1);

    check_edges((*g), ref_edges);

}

TEST_F(Triconnected_graph_H, computeTriconnectivity_partial_test_2) {

    ASSERT_EQ(8, (*g).nb_of_nodes);
    ASSERT_EQ(12, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents = {};
    (*g).computeTriconnectivity(triconnectedComponents, 2);

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

    Edge *e = NULL;

    vector<Edge> ref_m_TREE_ARC(nb_nodes);
    ref_m_TREE_ARC[1] = Edge(0, 1, 0, 3);
    ref_m_TREE_ARC[2] = Edge(1, 2, 3, 3);
    ref_m_TREE_ARC[3] = Edge(2, 3, 5, 3);
    ref_m_TREE_ARC[4] = Edge(7, 4, 9, 3);
    ref_m_TREE_ARC[5] = Edge(4, 5, 8, 3);
    ref_m_TREE_ARC[6] = Edge(5, 6, 10, 3);
    ref_m_TREE_ARC[7] = Edge(3, 7, 7, 3);

    check_m_TREE_ARC((*g), ref_m_TREE_ARC);

    vector<list<Edge>> ref_m_A(nb_nodes);
    ref_m_A[0] = {Edge(0, 1, 0, 3)};
    ref_m_A[1] = {Edge(1, 2, 3, 3)};
    ref_m_A[2] = {Edge(2, 3, 5, 3)};
    ref_m_A[3] = {Edge(3, 7, 7, 3), Edge(3, 0, 1, 1)};
    ref_m_A[4] = {Edge(4, 0, 2, 1), Edge(4, 5, 8, 3)};
    ref_m_A[5] = {Edge(5, 1, 4, 1), Edge(5, 6, 10, 3)};
    ref_m_A[6] = {Edge(6, 2, 6, 1), Edge(6, 7, 11, 1)};
    ref_m_A[7] = {Edge(7, 4, 9, 3)};
    check_m_A(*g, ref_m_A);

//    for (Edge* e: (*g).edges) {
//        (*e).print();
//        cout << endl;
//    }
    vector<Edge> ref_edges(nb_edges);
    ref_edges[0] = Edge(0, 1, 0, 3);
    ref_edges[1] = Edge(3, 0, 1, 1);
    ref_edges[2] = Edge(4, 0, 2, 1);
    ref_edges[3] = Edge(1, 2, 3, 3);
    ref_edges[4] = Edge(5, 1, 4, 1);
    ref_edges[5] = Edge(2, 3, 5, 3);
    ref_edges[6] = Edge(6, 2, 6, 1);
    ref_edges[7] = Edge(3, 7, 7, 3);
    ref_edges[8] = Edge(4, 5, 8, 3);
    ref_edges[9] = Edge(7, 4, 9, 3);
    ref_edges[10] = Edge(5, 6, 10, 3);
    ref_edges[11] = Edge(6, 7, 11, 1);

    check_edges((*g), ref_edges);

}

TEST_F(Triconnected_graph_H, computeTriconnectivity_partial_test_3) {

    ASSERT_EQ(8, (*g).nb_of_nodes);
    ASSERT_EQ(12, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents = {};
    (*g).computeTriconnectivity(triconnectedComponents, 3);

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

    Edge *e = NULL;

    vector<Edge> ref_m_TREE_ARC(nb_nodes);
    ref_m_TREE_ARC[1] = Edge(0, 1, 0, 3);
    ref_m_TREE_ARC[2] = Edge(1, 2, 3, 3);
    ref_m_TREE_ARC[3] = Edge(2, 3, 5, 3);
    ref_m_TREE_ARC[4] = Edge(7, 4, 9, 3);
    ref_m_TREE_ARC[5] = Edge(4, 5, 8, 3);
    ref_m_TREE_ARC[6] = Edge(5, 6, 10, 3);
    ref_m_TREE_ARC[7] = Edge(3, 7, 7, 3);

    check_m_TREE_ARC((*g), ref_m_TREE_ARC);

    vector<list<Edge>> ref_m_A(nb_nodes);
    ref_m_A[0] = {Edge(0, 1, 0, 3)};
    ref_m_A[1] = {Edge(1, 2, 3, 3)};
    ref_m_A[2] = {Edge(2, 3, 5, 3)};
    ref_m_A[3] = {Edge(3, 7, 7, 3), Edge(3, 0, 1, 1)};
    ref_m_A[4] = {Edge(4, 0, 2, 1), Edge(4, 5, 8, 3)};
    ref_m_A[5] = {Edge(5, 1, 4, 1), Edge(5, 6, 10, 3)};
    ref_m_A[6] = {Edge(6, 2, 6, 1), Edge(6, 7, 11, 1)};
    ref_m_A[7] = {Edge(7, 4, 9, 3)};
    check_m_A(*g, ref_m_A);

//    for (Edge* e: (*g).edges) {
//        (*e).print();
//        cout << endl;
//    }
    vector<Edge> ref_edges(nb_edges);
    ref_edges[0] = Edge(0, 1, 0, 3);
    ref_edges[1] = Edge(3, 0, 1, 1);
    ref_edges[2] = Edge(4, 0, 2, 1);
    ref_edges[3] = Edge(1, 2, 3, 3);
    ref_edges[4] = Edge(5, 1, 4, 1);
    ref_edges[5] = Edge(2, 3, 5, 3);
    ref_edges[6] = Edge(6, 2, 6, 1);
    ref_edges[7] = Edge(3, 7, 7, 3);
    ref_edges[8] = Edge(4, 5, 8, 3);
    ref_edges[9] = Edge(7, 4, 9, 3);
    ref_edges[10] = Edge(5, 6, 10, 3);
    ref_edges[11] = Edge(6, 7, 11, 1);

    check_edges((*g), ref_edges);

    vector<long> ref_m_NEWNUM = {0, 1, 2, 3, 5, 6, 7, 4};
    ASSERT_EQ(ref_m_NEWNUM, (*g).m_NEWNUM);

    vector<long> ref_m_ORIGINAL = {0, 1, 2, 3, 7, 4, 5, 6};
    ASSERT_EQ(ref_m_ORIGINAL, (*g).m_ORIGINAL);

    vector<list<long>> ref_HIGHTPT = {list<long>({5, 3}),
                                      list<long>({6}),
                                      list<long>({7}),
                                      list<long>({}),
                                      list<long>({}),
                                      list<long>({}),
                                      list<long>({}),
                                      list<long>({7})};
    ASSERT_EQ(ref_HIGHTPT, (*g).m_HIGHPT);
    vector<list<pair<long, long>>> ref_m_IN_HIGH = {list<pair<long, long>>({}),
                                                    list<pair<long, long>>({pair<long, long>(0, 1)}),
                                                    list<pair<long, long>>({{pair<long, long>(0, 0)}}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({{pair<long, long>(1, 0)}}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({{pair<long, long>(2, 0)}}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({{pair<long, long>(7, 0)}})
    };
//    ref_m_IN_HIGH = [[], [(0, 1)], [(0, 0)], [], [(1, 0)], [], [(2, 0)], [], [], [], [], [(7, 0)]]
//    self.assertEqual(ref_m_IN_HIGH, graph.m_HIGHPT)
    ASSERT_EQ(ref_m_IN_HIGH, (*g).m_IN_HIGH);
    ASSERT_EQ(1, (*g).m_numCount);


}

TEST_F(Triconnected_graph_H, computeTriconnectivity_partial_test_4) {

    ASSERT_EQ(8, (*g).nb_of_nodes);
    ASSERT_EQ(12, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents = {};
    (*g).computeTriconnectivity(triconnectedComponents, 4);

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

    Edge *e = NULL;

    vector<Edge> ref_m_TREE_ARC(nb_nodes);
    ref_m_TREE_ARC[1] = Edge(0, 1, 0, 3);
    ref_m_TREE_ARC[2] = Edge(1, 2, 3, 3);
    ref_m_TREE_ARC[3] = Edge(2, 3, 5, 3);
    ref_m_TREE_ARC[4] = Edge(7, 4, 9, 3);
    ref_m_TREE_ARC[5] = Edge(4, 5, 8, 3);
    ref_m_TREE_ARC[6] = Edge(5, 6, 10, 3);
    ref_m_TREE_ARC[7] = Edge(3, 7, 7, 3);

    check_m_TREE_ARC((*g), ref_m_TREE_ARC);

    vector<list<Edge>> ref_m_A(nb_nodes);
    ref_m_A[0] = {Edge(0, 1, 0, 3)};
    ref_m_A[1] = {Edge(1, 2, 3, 3)};
    ref_m_A[2] = {Edge(2, 3, 5, 3)};
    ref_m_A[3] = {Edge(3, 7, 7, 3), Edge(3, 0, 1, 1)};
    ref_m_A[4] = {Edge(4, 0, 2, 1), Edge(4, 5, 8, 3)};
    ref_m_A[5] = {Edge(5, 1, 4, 1), Edge(5, 6, 10, 3)};
    ref_m_A[6] = {Edge(6, 2, 6, 1), Edge(6, 7, 11, 1)};
    ref_m_A[7] = {Edge(7, 4, 9, 3)};
    check_m_A(*g, ref_m_A);

//    for (Edge* e: (*g).edges) {
//        (*e).print();
//        cout << endl;
//    }
    vector<Edge> ref_edges(nb_edges);
    ref_edges[0] = Edge(0, 1, 0, 3);
    ref_edges[1] = Edge(3, 0, 1, 1);
    ref_edges[2] = Edge(4, 0, 2, 1);
    ref_edges[3] = Edge(1, 2, 3, 3);
    ref_edges[4] = Edge(5, 1, 4, 1);
    ref_edges[5] = Edge(2, 3, 5, 3);
    ref_edges[6] = Edge(6, 2, 6, 1);
    ref_edges[7] = Edge(3, 7, 7, 3);
    ref_edges[8] = Edge(4, 5, 8, 3);
    ref_edges[9] = Edge(7, 4, 9, 3);
    ref_edges[10] = Edge(5, 6, 10, 3);
    ref_edges[11] = Edge(6, 7, 11, 1);

    check_edges((*g), ref_edges);

    vector<long> ref_m_NEWNUM = {0, 1, 2, 3, 5, 6, 7, 4};
    ASSERT_EQ(ref_m_NEWNUM, (*g).m_NEWNUM);

    vector<long> ref_m_ORIGINAL = {0, 1, 2, 3, 7, 4, 5, 6};
    ASSERT_EQ(ref_m_ORIGINAL, (*g).m_ORIGINAL);

    vector<list<long>> ref_HIGHTPT = {list<long>({5, 3}),
                                      list<long>({6}),
                                      list<long>({7}),
                                      list<long>({}),
                                      list<long>({}),
                                      list<long>({}),
                                      list<long>({}),
                                      list<long>({7})};
    ASSERT_EQ(ref_HIGHTPT, (*g).m_HIGHPT);
    vector<list<pair<long, long>>> ref_m_IN_HIGH = {list<pair<long, long>>({}),
                                                    list<pair<long, long>>({pair<long, long>(0, 1)}),
                                                    list<pair<long, long>>({{pair<long, long>(0, 0)}}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({{pair<long, long>(1, 0)}}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({{pair<long, long>(2, 0)}}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({}),
                                                    list<pair<long, long>>({{pair<long, long>(7, 0)}})
    };
//    ref_m_IN_HIGH = [[], [(0, 1)], [(0, 0)], [], [(1, 0)], [], [(2, 0)], [], [], [], [], [(7, 0)]]
//    self.assertEqual(ref_m_IN_HIGH, graph.m_HIGHPT)
    ASSERT_EQ(ref_m_IN_HIGH, (*g).m_IN_HIGH);
    ASSERT_EQ(1, (*g).m_numCount);

    // Partial test 4:
    ASSERT_EQ(1, (*g).m_top);

    vector<long> ref_m_TSTACK_h = {0, 7, 0, 7, 0, 7, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(ref_m_TSTACK_h, (*g).m_TSTACK_h);

    vector<long> ref_m_TSTACK_a = {-1, 0, -1, 0, -1, 2, -1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(ref_m_TSTACK_a, (*g).m_TSTACK_a);

    vector<long> ref_m_TSTACK_b = {0, 0, 0, 5, 0, 6, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(ref_m_TSTACK_b, (*g).m_TSTACK_b);

    vector<Edge> ref_m_estack(nb_edges);
    ref_m_estack[0] = Edge(4, 0, 2, 1);
    ref_m_estack[1] = Edge(5, 1, 4, 1);
    ref_m_estack[2] = Edge(6, 2, 6, 1);
    ref_m_estack[3] = Edge(6, 7, 11, 1);
    ref_m_estack[4] = Edge(5, 6, 10, 3);
    ref_m_estack[5] = Edge(4, 5, 8, 3);
    ref_m_estack[6] = Edge(7, 4, 9, 3);
    ref_m_estack[7] = Edge(3, 7, 7, 3);
    ref_m_estack[8] = Edge(3, 0, 1, 1);
    ref_m_estack[9] = Edge(2, 3, 5, 3);
    ref_m_estack[10] = Edge(1, 2, 3, 3);
    ref_m_estack[11] = Edge(0, 1, 0, 3);

    check_edges((*g), ref_edges);
}

TEST_F(Triconnected_graph_H, computeTriconnectivity) {

    ASSERT_EQ(8, (*g).nb_of_nodes);
    ASSERT_EQ(12, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);

    list<unordered_set<long>> ref_triconnectedComponents = {unordered_set<long>({0, 1, 2, 3, 4, 5, 6, 7})};
    cout << "triconnectedComponents= {";
    for (long i_v:triconnectedComponents.front()) {
        cout << i_v << ", ";
    }
    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}

TEST_F(Triconnected_graph_Biconnected_Hexahedron, computeTriconnectivity_partial_test_1) {

    ASSERT_EQ(17, (*g).nb_of_nodes);
    ASSERT_EQ(27, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents, 1);

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

    vector<Edge> ref_m_TREE_ARC(nb_nodes);
    ref_m_TREE_ARC[1] = Edge(0, 1, 0, 3);
    ref_m_TREE_ARC[2] = Edge(1, 2, 3, 3);
    ref_m_TREE_ARC[3] = Edge(2, 3, 5, 3);
    ref_m_TREE_ARC[4] = Edge(4, 7, 9, 3);
    ref_m_TREE_ARC[5] = Edge(4, 5, 8, 3);
    ref_m_TREE_ARC[6] = Edge(5, 6, 10, 3);
    ref_m_TREE_ARC[7] = Edge(3, 7, 7, 3);

    check_m_TREE_ARC((*g), ref_m_TREE_ARC);


    vector<Edge> ref_edges(nb_edges);
    ref_edges[0] = Edge(0, 1, 0, 3);
    ref_edges[1] = Edge(0, 3, 1, 1);
    ref_edges[2] = Edge(0, 4, 2, 1);
    ref_edges[3] = Edge(1, 2, 3, 3);
    ref_edges[4] = Edge(1, 5, 4, 1);
    ref_edges[5] = Edge(2, 3, 5, 3);
    ref_edges[6] = Edge(2, 6, 6, 1);
    ref_edges[7] = Edge(3, 7, 7, 3);
    ref_edges[8] = Edge(4, 5, 8, 3);
    ref_edges[9] = Edge(4, 7, 9, 3);
    ref_edges[10] = Edge(5, 6, 10, 3);
    ref_edges[11] = Edge(6, 7, 11, 1);

    check_edges((*g), ref_edges);

}

TEST_F(Triconnected_graph_Biconnected_Hexahedron, computeTriconnectivity) {

    ASSERT_EQ(17, (*g).nb_of_nodes);
    ASSERT_EQ(27, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);
    list<unordered_set<long>> ref_triconnectedComponents = {unordered_set<long>({0, 1, 3, 4, 8, 9, 11, 12})};
//    cout << "triconnectedComponents= {";
//    for (long i_v:triconnectedComponents.front()) {
//        cout << i_v << ", ";
//    }
//    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}


TEST_F(Triconnected_graph_Biconnected_n_13_m_16, computeTriconnectivity) {

    ASSERT_EQ(13, (*g).nb_of_nodes);
    ASSERT_EQ(16, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);
    list<unordered_set<long>> ref_triconnectedComponents;

//    cout << "triconnectedComponents= {";
//    for (long i_v:triconnectedComponents.front()) {
//        cout << i_v << ", ";
//    }
//    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}

TEST_F(Triconnected_graph_Biconnected_n_13_m_16, computeTriconnectivity_partial_2) {

    ASSERT_EQ(13, (*g).nb_of_nodes);
    ASSERT_EQ(16, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents = {};
    (*g).computeTriconnectivity(triconnectedComponents, 2);

    vector<int> ref_m_number_dfs = {0, 4, 5, 1, 8, 3, 9, 11, 7, 6, 10, 2, 12};
    ASSERT_EQ(ref_m_number_dfs, (*g).m_number_dfs);

    vector<long> ref_m_low_pt_1 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3};
    ASSERT_EQ(ref_m_low_pt_1, (*g).m_low_pt_1);

    vector<long> ref_m_low_pt_2 = {0, 3, 5, 1, 8, 3, 9, 3, 7, 6, 10, 2, 12};
    ASSERT_EQ(ref_m_low_pt_2, (*g).m_low_pt_2);

    vector<long> ref_m_FATHER = {-1, 5, 1, 0, 8, 11, 4, 1, 9, 2, 2, 3, 7};
    ASSERT_EQ(ref_m_FATHER, (*g).m_FATHER);

    vector<int> ref_m_NumberOfDescendants = {13, 9, 6, 12, 2, 10, 1, 2, 3, 4, 1, 11, 1};
    ASSERT_EQ(ref_m_NumberOfDescendants, (*g).m_NumberOfDescendants);

    cout << "1" << endl;
    vector<Edge> ref_m_TREE_ARC(nb_nodes);
    ref_m_TREE_ARC[1] = Edge(5, 1, 5, 3);
    ref_m_TREE_ARC[2] = Edge(1, 2, 4, 3);
    ref_m_TREE_ARC[3] = Edge(0, 3, 0, 3);
    ref_m_TREE_ARC[4] = Edge(8, 4, 11, 3);
    ref_m_TREE_ARC[5] = Edge(11, 5, 12, 3);
    ref_m_TREE_ARC[6] = Edge(4, 6, 10, 3);
    ref_m_TREE_ARC[7] = Edge(1, 7, 6, 3);
    ref_m_TREE_ARC[8] = Edge(9, 8, 15, 3);
    ref_m_TREE_ARC[9] = Edge(2, 9, 7, 3);
    ref_m_TREE_ARC[10] = Edge(2, 10, 8, 3);
    ref_m_TREE_ARC[11] = Edge(3, 11, 9, 3);
    ref_m_TREE_ARC[12] = Edge(7, 12, 14, 3);


    check_m_TREE_ARC((*g), ref_m_TREE_ARC);
    cout << "2" << endl;

    vector<list<Edge>> ref_m_A(nb_nodes);
    ref_m_A[0] = {Edge(0, 3, 0, 3)};
    ref_m_A[1] = {Edge(1, 7, 6, 3), Edge(1, 2, 4, 3)};
    ref_m_A[2] = {Edge(2, 9, 7, 3), Edge(2, 10, 8, 3)};
    ref_m_A[3] = {Edge(3, 11, 9, 3)};
    ref_m_A[4] = {Edge(4, 6, 10, 3)};
    ref_m_A[5] = {Edge(5, 1, 5, 3)};
    ref_m_A[6] = {Edge(6, 0, 1, 1)};
    ref_m_A[7] = {Edge(7, 0, 2, 1), Edge(7, 12, 14, 3)};
    ref_m_A[8] = {Edge(8, 4, 11, 3)};
    ref_m_A[9] = {Edge(9, 8, 15, 3)};
    ref_m_A[10] = {Edge(10, 0, 3, 1)};
    ref_m_A[11] = {Edge(11, 5, 12, 3)};
    ref_m_A[12] = {Edge(12, 5, 13, 1)};
    check_m_A(*g, ref_m_A);
    cout << "3" << endl;
//    for (Edge* e: (*g).edges) {
//        (*e).print();
//        cout << endl;
//    }
    vector<Edge> ref_edges(nb_edges);
    ref_edges[0] = Edge(0, 3, 0, 3);
    ref_edges[1] = Edge(6, 0, 1, 1);
    ref_edges[2] = Edge(7, 0, 2, 1);
    ref_edges[3] = Edge(10, 0, 3, 1);
    ref_edges[4] = Edge(1, 2, 4, 3);
    ref_edges[5] = Edge(5, 1, 5, 3);
    ref_edges[6] = Edge(1, 7, 6, 3);
    ref_edges[7] = Edge(2, 9, 7, 3);
    ref_edges[8] = Edge(2, 10, 8, 3);
    ref_edges[9] = Edge(3, 11, 9, 3);
    ref_edges[10] = Edge(4, 6, 10, 3);
    ref_edges[11] = Edge(8, 4, 11, 3);
    ref_edges[12] = Edge(11, 5, 12, 3);
    ref_edges[13] = Edge(12, 5, 13, 1);
    ref_edges[14] = Edge(7, 12, 14, 3);
    ref_edges[15] = Edge(9, 8, 15, 3);

    check_edges((*g), ref_edges);
}


TEST_F(Triconnected_graph_Biconnected_OGDF_n_10_m_19, computeTriconnectivity) {

    ASSERT_EQ(10, (*g).nb_of_nodes);
    ASSERT_EQ(19, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);
    list<unordered_set<long>> ref_triconnectedComponents = {unordered_set<long>({0, 1, 2, 3, 4, 5, 8})};

//    cout << "triconnectedComponents= {";
//    for (long i_v:triconnectedComponents.front()) {
//        cout << i_v << ", ";
//    }
//    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}

TEST_F(Triconnected_graph_Biconnected_OGDF_n_15_m_30, computeTriconnectivity) {

    ASSERT_EQ(15, (*g).nb_of_nodes);
    ASSERT_EQ(30, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);
    list<unordered_set<long>> ref_triconnectedComponents = {unordered_set<long>({0, 1, 2, 3, 4, 5, 8, 10, 12, 13})};

//    cout << "triconnectedComponents= {";
//    for (long i_v:triconnectedComponents.front()) {
//        cout << i_v << ", ";
//    }
//    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}

TEST_F(Triconnected_graph_Biconnected_OGDF_v2_n_10_m_19, computeTriconnectivity) {

    ASSERT_EQ(10, (*g).nb_of_nodes);
    ASSERT_EQ(19, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);
    list<unordered_set<long>> ref_triconnectedComponents = {unordered_set<long>({0, 1, 2, 3, 4, 5, 8})};

//    cout << "triconnectedComponents= {";
//    for (long i_v:triconnectedComponents.front()) {
//        cout << i_v << ", ";
//    }
//    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}

TEST_F(Triconnected_graph_Biconnected_OGDF_v3_n_10_m_19, computeTriconnectivity) {

    ASSERT_EQ(10, (*g).nb_of_nodes);
    ASSERT_EQ(19, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);
    list<unordered_set<long>> ref_triconnectedComponents = {unordered_set<long>({0, 1, 2, 3, 4, 5, 8})};

//    cout << "triconnectedComponents= {";
//    for (long i_v:triconnectedComponents.front()) {
//        cout << i_v << ", ";
//    }
//    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}

TEST_F(Triconnected_graph_MGridGen_biconnected, computeTriconnectivity) {

    ASSERT_EQ(13, (*g).nb_of_nodes);
    ASSERT_EQ(16, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);
    list<unordered_set<long>> ref_triconnectedComponents = {};

//    cout << "triconnectedComponents= {";
//    for (long i_v:triconnectedComponents.front()) {
//        cout << i_v << ", ";
//    }
//    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}

TEST_F(Triconnected_graph_2_Graphs_Biconnected_n_8_m_14, computeTriconnectivity) {

    ASSERT_EQ(8, (*g).nb_of_nodes);
    ASSERT_EQ(14, (*g).nb_of_edges);

    list<unordered_set<long>> triconnectedComponents;
    (*g).computeTriconnectivity(triconnectedComponents);
    list<unordered_set<long>> ref_triconnectedComponents = {unordered_set<long>({2, 3, 6, 7}),
                                                            unordered_set<long>({0, 1, 4, 5})
    };

//    cout << "triconnectedComponents= {";
//    for (long i_v:triconnectedComponents.front()) {
//        cout << i_v << ", ";
//    }
//    cout << "}" << endl;
    ASSERT_EQ(ref_triconnectedComponents, triconnectedComponents);
}
