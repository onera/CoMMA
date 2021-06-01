#include "../CoMMA_lib/Util_graph.h"
#include "Triconnected_graph_C.h"
#include "Triconnected_graph_C_2_BCC.h"
#include "Triconnected_graph_C_3_BCC.h"
#include "Triconnected_graph_D.h"
#include "Triconnected_graph_E.h"
#include "Triconnected_graph_F.h"
#include "Triconnected_graph_OGDF_13_16.h"
#include "Triconnected_graph_OGDF_100_200.h"
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

TEST_F(Triconnected_graph_OGDF_13_16, BCC_NR) {

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

