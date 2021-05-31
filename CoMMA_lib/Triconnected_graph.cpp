//
// Created by Nicolas Lantos on 31/05/2021.
//

#include "Triconnected_graph.h"

Triconnected_graph::Triconnected_graph(vector<long> row_ptr,
                                       vector<long> col_ind,
                                       vector<double> values,
                                       short verbose
) {
    this->verbose = verbose;
    edge_type_2_short = {{"unseen",  0},
                         {"frond",   1},
                         {"removed", 2},
                         {"tree",    3}};
    short_2_edge_type = {"unseen", "frond", "removed", "tree"};

    nb_of_nodes = row_ptr.size() - 1;  // number of nodes
    this->row_ptr = row_ptr;
    this->col_ind = col_ind;
    this->values = values;

    // Computation of the number of edges:
    int nb_edges = 0;
    m_degree = vector<unsigned int>(nb_of_nodes, 0); // < degree of v

    for (long i_v = 0; i_v < nb_of_nodes; i_v++) {

        long ind = row_ptr[i_v];  // Usefull to find neighbours of i_v
        long ind_p_one = row_ptr[i_v + 1];  // Usefull to find neighbours of i_v
        for (long i = ind; i < ind_p_one; i++) {
            long i_w = col_ind[i];
            if (i_v != i_w) {
                nb_edges++;
                if (i_v < i_w) {
                    m_degree[i_v]++;
                    m_degree[i_w]++;
                }
            }
        }
    }
    assert(nb_edges % 2 == 0);
    nb_of_edges = int(nb_edges / 2);

    adj_edges = vector<list<Edge>>(nb_of_nodes);

    __creation_of_edges();

}

void Triconnected_graph::__creation_of_edges() {
    /**
     * Creates the edges data structure for triconnectivity components computation
     * edges are "oriented" according to the spanning tree
     */
    unordered_map<long, unordered_set<long>> d_edges;
    long i_count = 0;
    long tmp_v = 0;
    long tmp_w = 0;

    for (long i_v = 0; i_v < nb_of_nodes; i_v++) {
        long ind = row_ptr[i_v];  // Usefull to find neighbours of i_v
        long ind_p_one = row_ptr[i_v + 1];  // Usefull to find neighbours of i_v
        for (long i_neighbour = ind; i_neighbour < ind_p_one; i_neighbour++) {
            long i_w = col_ind[i_neighbour];
            if (i_v == i_w) {
                continue;
            }
            if (i_v < i_w) {
                tmp_v = i_v;
                tmp_w = i_w;
            } else {
                tmp_v = i_w;
                tmp_w = i_v;
            }

            if (d_edges.count(tmp_v) == 0) {
                d_edges[tmp_v] = {tmp_w};

            } else if ((d_edges.count(tmp_v) && d_edges[tmp_v].count(tmp_w) == 0)) {
                d_edges[tmp_v].insert(tmp_w);
            } else {
                continue;
            }
            Edge e = Edge(i_v, i_w, i_count);
            adj_edges[i_v].push_back(e);
            adj_edges[i_w].push_back(e);
            i_count++;
        }
    }
}