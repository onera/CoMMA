#include "../CoMMA_lib/Triconnected_graph.h"

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

