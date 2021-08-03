#include "../CoMMA_lib/Util_graph.h"
#include "../CoMMA_lib/Triconnected_graph.h"
#include "Triconnected_graph_A.h"
#include "Triconnected_graph_H.h"
#include "Triconnected_graph_MGridGen.h"
#include "Triconnected_graph_MGridGen_biconnected.h"
#include "Triconnected_graph_biconnected_hexahedra.h"
#include "Triconnected_graph_OGDF_original_connectedGraph_13_16.h"
#include "Triconnected_graph_biconnected_n_13_m_16.h"
#include "Triconnected_graph_biconnected_OGDF_n_10_m_19.h"
#include "Triconnected_graph_biconnected_OGDF_v2_n_10_m_19.h"
#include "Triconnected_graph_biconnected_OGDF_v3_n_10_m_19.h"
#include "Triconnected_graph_biconnected_OGDF_n_15_m_30.h"
#include "Triconnected_graph_2_graphs_biconnected_n_8_m_14.h"
#include "Triconnected_graph_bug_cube.h"
#include "Triconnected_graph_bug_cube_v3_bis.h"
#include "Triconnected_graph_box_5x5x5.h"
#include "gtest/gtest.h"

void check_edge(const Edge *e, const Edge ref);

void check_m_TREE_ARC(const Triconnected_graph &g, vector<Edge> v_ref);

void check_edges(const Triconnected_graph &g, vector<Edge> v_ref);

void check_m_A(const Triconnected_graph &g, vector<list<Edge>> v_ref);
