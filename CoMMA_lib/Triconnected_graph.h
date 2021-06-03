//
// Created by Nicolas Lantos on 31/05/2021.
//

#ifndef COMMA_PROJECT_TRICONNECTED_GRAPH_H
#define COMMA_PROJECT_TRICONNECTED_GRAPH_H

#include <vector>
#include <string>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include "Edge.h"
#include "CompStruct.h"
#include <iostream>

using namespace std;

class Triconnected_graph {
public:
    short verbose;
    unordered_map<string, short> edge_type_2_int;
    vector<string> int_2_edge_type;

    int nb_of_nodes;
    int nb_of_edges;
    vector<long> row_ptr;
    vector<long> col_ind;
    vector<double> values;
    vector<unsigned int> m_degree;

    // for biconnected algorithm
    long time = 0;
    long count = 0;

    //triconnectivity datas:
    vector<Edge> edges;
    vector<list<Edge>> adj_edges;

//    m_estack
    vector<int> m_number_dfs;      // NodeArray < int > ! < (first) dfs-number of v
    list<pair<long, long>> m_estack;  // ArrayBuffer<edge> : stack of currently active  edges : size m

    vector<long> m_low_pt_1;  // NodeArray < int >
    vector<long> m_low_pt_2;  // NodeArray < int >
    vector<int> m_NumberOfDescendants;  // NodeArray < int >  // ! < number of descendants in palm tree
    int m_numCount = 0;  // int:  counter for dfs - traversal
    vector<long> m_FATHER;  // NodeArray < node >  // < father of v in palm tree
    vector<Edge> m_TREE_ARC;  // NodeArray < edge > : tree arc entering v

    // m_NODEAT = None  // Array < node >  ! < node with number i
    vector<list<Edge>> m_A;  // NodeArray < List < edge > >//  adjacency list of v
    vector<long> m_NEWNUM; // NodeArray < int > // ! < (second)dfs-number of v
    vector<long> m_ORIGINAL;  // NodeArray < int > // ! < (second)dfs-number of v

    //self.m_START = None  # EdgeArray < bool > edge starts a path
    vector<list<long>> m_HIGHPT;  // NodeArray < List < int > > : list of fronds entering v in the order they are visited
//    m_IN_ADJ = None  // EdgeArray < ListIterator<edge>>:  pointer to element in adjacency list containing e
//    m_IN_HIGH = None  // EdgeArray < ListIterator < int > > : pointer to element in HIGHPT list containing e

    long m_start = -1;  // Node: start node of dfs traversal
//    m_newPath = None  // Boolean:  true iff we start a new path

    vector<long> m_TSTACK_h;  // stack of triples
    vector<long> m_TSTACK_a;  // stack of triples
    vector<long> m_TSTACK_b;  // stack of triples
    long m_top = -1;  // length of triples stacks.

    vector<CompStruct> m_component; // array of components

public:
    Triconnected_graph(vector<long> row_ptr,
                       vector<long> col_ind,
                       vector<double> values,
                       short verbose = 0
    );

    void __creation_of_edges();

    bool is_connected(vector<long> v_fc = {});

    bool is_empty();

    bool is_Biconnected_And_CutVertices(list<long> &cut_vertices);

    long build_dfs_tree(const long root, vector<int> &number_dfs, vector<long> &parent, vector<long> &childNr, list<long> &revS, long first_nr = 1);

    bool find_cut_vertices(vector<int> &number_dfs, vector<long> &parent, list<long> &revS, list<long> &cutVertices, list<pair<long, long>> &addEdges, bool only_one);

    void __DFS1(const long nodeV, const long nodeU);

    void __splitMultiEdges();

    inline long first_node() {
        return 0;
    }

    void __buildAcceptableAdjStruct();

    short computeTriconnectivity(short partialTest = 0);
};


#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_H
