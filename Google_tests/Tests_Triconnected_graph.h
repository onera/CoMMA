#include "../CoMMA_lib/Triconnected_graph.h"
#include "Triconnected_graph_A.h"
#include "Triconnected_graph_H.h"
#include "Triconnected_graph_MGridGen.h"
#include "gtest/gtest.h"

void check_edge(const Edge *e, const Edge ref);
void check_m_TREE_ARC(const Triconnected_graph g, vector<Edge> v_ref);
void check_edges(const Triconnected_graph g, vector<Edge> v_ref);
void check_m_A(const Triconnected_graph g, vector<list<Edge>> v_ref);