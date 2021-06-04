//
// Created by Nicolas Lantos on 01/06/2021.
//

#include "Util_graph.h"


bool BCC_NR(const Triconnected_graph &graph, list<unordered_set<long>> &component) {
    /**
     * The function to do DFS traversal.
     * It uses recursive BCCUtil()
     *
     *
     * Computes the biconnected components
     *   A recursive function that finds and prints strongly connected
        components using DFS traversal
        u --> The vertex to be visited next
        disc[] --> Stores discovery times of visited vertices
        low[] -- >> earliest visited vertex (the vertex with minimum
               discovery time) that can be reached from subtree
               rooted with current vertex
     *
     * stack to store visited edges
     */

    long numberOfNodes = graph.nb_of_nodes;

    // Initialize disc and low, and parent arrays
    vector<short> dfs_Number = vector<short>(numberOfNodes, -1);
    vector<short> low = vector<short>(numberOfNodes, -1);
    vector<short> parent = vector<short>(numberOfNodes, -1);

    list<pair<long, long>> stack_Of_BCC;

    unsigned short nb_Of_Component = 0;

    vector<unordered_set<long>> biconnectedComponents;
    bool isFirst = true;
    unordered_set<long> set_Of_Articulation_Point = {};

    long iNeighbour = -1;

    // Call the recursive helper function to
    // find articulation points
    // in DFS tree rooted with vertex 'iRoot'
    for (long iRoot = 0; iRoot < numberOfNodes; iRoot++) {

        // We test if the current vertex has already been visited.
        if (dfs_Number[iRoot] == -1) {

            list<vector<long>> stack;
            long iU = -1;  // father of i_v
            long iV = iRoot;

            // initialisation of count useful for dfs_Number
            long count = 0;

            while ((!stack.empty()) || isFirst) {

                isFirst = false;  // isFirst is usefull to get into the loop the first time
                if (iNeighbour == -1) {

                    // Count of children in current node
                    short number_Of_Children = 0;

                    // Initialize discovery time and low value
                    dfs_Number[iV] = count;
                    low[iV] = count;
                    count += 1;
                    long iW = -1;
                    // Loop on neighbours/son of i_v
                    for (iNeighbour = 0; iNeighbour < (graph.row_ptr[iV + 1] - graph.row_ptr[iV]); iNeighbour++) {

                        iW = graph.col_ind[iNeighbour + graph.row_ptr[iV]];  // Get real number of the vertex
                        // If i_w is not visited yet, then make it a child of i_v in DFS tree and recur for it
                        if (dfs_Number[iW] == -1) {

                            parent[iW] = iV;
                            number_Of_Children += 1;

                            pair<long, long> e(iV, iW);
                            stack_Of_BCC.push_back(e);  // store the e in stack

                            vector<long> backup_point = {number_Of_Children, iNeighbour, iU, iV};
                            stack.push_back(backup_point);  // Backup point

                            iNeighbour = -1;
                            iU = iV;
                            iV = iW;
                            break;
                        } else if (iW != iU and low[iV] > dfs_Number[iW]) {
                            // update low number - ignore reverse of e leading to i_v
                            low[iV] = min(low[iV], dfs_Number[iW]);
                            pair<long, long> e(iV, iW);
                            stack_Of_BCC.push_back(e);  // store the e in stack

                        }
                    }

                    // Root of DFS is an articulation point if it has more than 1 child
                    if (parent[iV] == -1 && number_Of_Children > 1) {

                        set_Of_Articulation_Point.insert(iV);
                        nb_Of_Component += 1;
                        pair<long, long> w;
                        unordered_set<long> s;

                        pair<long, long> e(iV, iW);
                        while (w != e) {

                            w = stack_Of_BCC.back();  //2 steps pop(...)
                            stack_Of_BCC.pop_back();

                            s.insert(w.first);
                            s.insert(w.second);
                        }
                        biconnectedComponents.push_back(s);
                    }
                } else {  // continue the DFS : we get back on vertex with potentially multiple sons

                    vector<long> backup = stack.back();
                    stack.pop_back();
                    short number_Of_Children = backup[0];
                    iNeighbour = backup[1];
                    iU = backup[2];
                    iV = backup[3];

                    long iW = graph.col_ind[iNeighbour + graph.row_ptr[iV]];

                    // update low number
                    low[iV] = min(low[iV], low[iW]);

                    // non-root of DFS is an articulation point if low[i_w]>= dfs_number[i_v]
                    if ((low[iW] >= dfs_Number[iV] && parent[iV] != -1) || (parent[iV] == -1 && number_Of_Children > 1)) {

                        set_Of_Articulation_Point.insert(iV);
                        nb_Of_Component += 1;

                        pair<long, long> w;
                        unordered_set<long> s;

                        pair<long, long> e(iV, iW);
                        while (w != e) {
                            w = stack_Of_BCC.back();  //2 steps pop(...)
                            stack_Of_BCC.pop_back();

                            s.insert(w.first);
                            s.insert(w.second);
                        }

                        biconnectedComponents.push_back(s);
                    }

                    iNeighbour += 1;
                    while (iNeighbour < graph.row_ptr[iV + 1] - graph.row_ptr[iV]) {

                        iW = graph.col_ind[iNeighbour + graph.row_ptr[iV]];
                        if (dfs_Number[iW] == -1) {
                            parent[iW] = iV;
                            number_Of_Children += 1;

                            pair<long, long> e(iV, iW);
                            stack_Of_BCC.push_back(e); // store the edge in stack

                            vector<long> backup_point = {number_Of_Children, iNeighbour, iU, iV};
                            stack.push_back(backup_point);  // Backup point   // DFS (G, v, w)???

                            iNeighbour = -1;
                            iU = iV;
                            iV = iW;
                            break;
                        } else if (iW != iU && low[iV] > dfs_Number[iW]) {

                            // update low number - ignore reverse of edge leading to i_v
                            low[iV] = min(low[iV], dfs_Number[iW]);
                            pair<long, long> e(iV, iW);
                            stack_Of_BCC.push_back(e);  // store the edge in stack
                        }
                        iNeighbour += 1;
                    }
                }
            }

        }
        // If stack is not empty, pop all edges from stack
        if (!stack_Of_BCC.empty()) {

            nb_Of_Component += 1;
            unordered_set<long> s;
            pair<long, long> w;
            while (!stack_Of_BCC.empty()) {
                w = stack_Of_BCC.back();  //2 steps pop(...)
                stack_Of_BCC.pop_back();

                s.insert(w.first);
                s.insert(w.second);
            }

            biconnectedComponents.push_back(s);
        }
    }
    // print( "\nparent", parent
    // print( "low", low
    // print( "dfs_Number", dfs_Number
    // print( "set_Of_Articulation_Point", set_Of_Articulation_Point
//    list<unordered_set<long>> component;
    for (unordered_set<long> &s :biconnectedComponents) {
        removeLeaf(s, graph);
        if (!s.empty()) {
            component.push_back(s);
        }
    }
    bool result = component.size() == 1 && component.front().size() == numberOfNodes;
    return result;
}

void removeLeaf(unordered_set<long> &set_Of_FC, const Triconnected_graph &graph) {
/**
 * This function removes the vertices of degree 1 in set_Of_FC according to the Triconnected_graph "graph"
 */

    bool is_V_Removed = true;
    while (is_V_Removed) {
        is_V_Removed = false;
        unordered_map<long, short> d = compute_Degree(set_Of_FC, graph.row_ptr, graph.col_ind);
        if (!d.empty()) {
            for (auto &ikv: d) {
                if (ikv.second == 1) {
                    is_V_Removed = true;
                    set_Of_FC.erase(ikv.first);
                }
            }
        }
    }
}

unordered_map<long, short> compute_Degree(const unordered_set<long> &set_Of_FC, const vector<long> &matrixAdj_CRS_row_ptr, const vector<long> &matrixAdj_CRS_col_ind) {
    // TODO on pourrait stocker l'info selon les degres... ca serait util pour removeLeaf!!!
    /**
     * O(n + m)
     */
    if (set_Of_FC.size() > 1) {
        unordered_map<long, short> dict_FineCell_compactness;
        for (long iFC:set_Of_FC) {
            for (long i = matrixAdj_CRS_row_ptr[iFC]; i < matrixAdj_CRS_row_ptr[iFC + 1]; i++) {
                long indCellN = matrixAdj_CRS_col_ind[i];

                if (set_Of_FC.count(indCellN) && indCellN != iFC) {
                    if (dict_FineCell_compactness.count(iFC)) {
                        dict_FineCell_compactness[iFC] += 1;
                    } else {
                        dict_FineCell_compactness[iFC] = 1;
                    }

                }
            }
        }
        return dict_FineCell_compactness;
    } else {
        unordered_map<long, short> empty_d;
        return empty_d;
    }
}