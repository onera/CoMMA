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

using namespace std;

class Triconnected_graph {
public:
    short verbose;
    unordered_map<string, short> edge_type_2_short;
    vector<string> short_2_edge_type;

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
    vector<list<Edge>> adj_edges;

//    m_estack
public:
    Triconnected_graph(vector<long> row_ptr,
                       vector<long> col_ind,
                       vector<double> values,
                       short verbose = 0
    );

    void __creation_of_edges();
};


#endif //COMMA_PROJECT_TRICONNECTED_GRAPH_H
