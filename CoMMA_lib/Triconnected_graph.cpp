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
    edge_type_2_int = {{"unseen",  0},
                       {"frond",   1},
                       {"removed", 2},
                       {"tree",    3}};
    int_2_edge_type = {"unseen", "frond", "removed", "tree"};

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

    adj_edges = vector<list<Edge *>>(nb_of_nodes);

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
            Edge *e = new Edge(i_v, i_w, i_count);

            adj_edges[i_v].push_back(e);
            adj_edges[i_w].push_back(e);

            edges.push_back(e);

            i_count++;
        }
    }
}

bool Triconnected_graph::is_connected(vector<long> v_fc) {

    /**
     * Checks connectivity of a graph
     */
    // TODO optimisation a prevoir dans le cas d'un test de connectivite pour un maillage complet.
    // TODO remplacer le dictionnaire par un numpy array

    if (v_fc.empty()) {
        v_fc = vector<long>(nb_of_nodes);
        for (long i = 0; i < nb_of_nodes; i++) {
            v_fc[i] = i;
        }
    }
    long size = v_fc.size();
    if (size <= 1) {
        return true;
    }
    vector<bool> is_already_connected = vector<bool>(size, false);

    is_already_connected[0] = true;
    unordered_set<long> s_next = {v_fc[0]};

    long nbConnectedCells = 1;
    unordered_map<long, long> dict_GlobalToLocal;
    for (long indFineCell = 0; indFineCell < size; indFineCell++) {
        dict_GlobalToLocal[v_fc[indFineCell]] = indFineCell;
    }


    while (nbConnectedCells < size) {
        if (!s_next.empty()) {

            long i_fc = *s_next.begin();  // s_next.pop();
            s_next.erase(s_next.begin());     // s_next.pop( );

            long ind = row_ptr[i_fc];
            long ind_p_one = row_ptr[i_fc + 1];  // Usefull to find neighbours of seed
            for (long ind_fc = ind; ind_fc < ind_p_one; ind_fc++) {
                long iFCellNeighbour = col_ind[ind_fc];
                if (dict_GlobalToLocal.count(iFCellNeighbour)) {
                    if (iFCellNeighbour != i_fc && !is_already_connected[dict_GlobalToLocal[iFCellNeighbour]]) {
                        s_next.insert(iFCellNeighbour);
                        nbConnectedCells++;
                        is_already_connected[dict_GlobalToLocal[iFCellNeighbour]] = true;
                    }
                }
            }
        } else {
            break;
        }
    }

    if (verbose && nbConnectedCells != size) {
        cout << "v_fc: {";
        for (long iFC: v_fc) {
            cout << iFC << ", ";
        }
        cout << "}\t";
        cout << "is_already_connected: {";
        for (bool iFC: is_already_connected) {
            cout << iFC << ", ";
        }
        cout << "}" << endl;
        cout << "nbConnectedCells " << nbConnectedCells << "\tsize " << size << endl;
    }
    return nbConnectedCells == size;

}

bool Triconnected_graph::is_empty() {
    if (nb_of_nodes == 0 && nb_of_edges == 0) {
        return true;
    } else {
        return false;
    }
}

bool Triconnected_graph::is_Biconnected_And_CutVertices(list<long> &cut_vertices) {

    if (is_empty()) {
        return true;
    }


    vector<int> number_dfs(nb_of_nodes, -1.0);  // discovery times
    vector<long> parent(nb_of_nodes, -1.0);  // parents in the dfs tree
    vector<long> childNr(nb_of_nodes, 0.0);
    list<long> revS;  // nodes of the dfs tree in reverse order

    // Build the dfs - tree and get the number of visited nodes.
    long num_count = build_dfs_tree(first_node(), number_dfs, parent, childNr, revS);

    // If the graph is not connected, return false.
    if (num_count != nb_of_nodes) {
        return false;
    }

    // If there are cut vertices in the graph, return false, else true.
    list<pair<long, long>> add_edges;

    if (find_cut_vertices(number_dfs, parent, revS, cut_vertices, add_edges, false)) {
        return false;
    } else {
        return true;
    }

}

bool Triconnected_graph::find_cut_vertices(
        vector<int> &number_dfs,
        vector<long> &parent,
        list<long> &revS,
        list<long> &cutVertices,
        list<pair<long, long>> &addEdges,
        bool only_one) {
/**
 *  Find cut vertices and potential edges that could be added to turn the cut vertices into non-cut vertices.
The algorithm is applied to the graph whose nodes were pushed to the ArrayBuffer revS.number, parent and revS can be obtained with build_dfs_tree.

:param number_dfs: number contains the number(discovery time) for each node. The number of root is 1, the number of unvisited nodes is 0.
:param parent: parent contains the parent in the dfs-tree for each node.
:param revS: revS contains the nodes of a graph such that the node that was visited last during the dfs-traversal is its top element.
:param cutVertices: cutVertices is assigned the cut vertices of the graph.

:param addEdges: addEdges is assigned the tuples of nodes which have to be connected in order to turn each cut vertex into a non-cut vertex.

:param only_one: only_one should be set to true if the search should stop after finding one cut vertex, to false if all cut vertices should be
        found.
:return: true if the graph contains at least one cut vertex, false otherwise.

 */

    vector<int> low_pt(number_dfs);

    // Go backwards through the dfs - tree:
    // Calculate the lowpoint for each node and test for cut vertices.
    while (!revS.empty()) {
        long v = revS.back();
        revS.pop_back();
        long first_child = -1;

        // For all adjacent nodes w of v:
        long ind = row_ptr[v];  // Usefull to find neighbours of seed
        long ind_p_one = row_ptr[v + 1];  // Usefull to find neighbours of seed
        for (long i_fc = ind; i_fc < ind_p_one; i_fc++) {
            long w = col_ind[i_fc];

            // Ignore self-loops and the parent of v.
            if (v == w || parent[v] == w) {
                continue;
            }

            // if v->w is a backedge in the dfs - tree, update v's lowpoint.
            if (number_dfs[v] > number_dfs[w]) {
                if (low_pt[v] > number_dfs[w]) {
                    low_pt[v] = number_dfs[w];
                }
            } else {

                // If w is v's child in the dfs-tree, update v's lowpoint.
                if (parent[w] == v) {

                    if (low_pt[v] > low_pt[w]) {
                        low_pt[v] = low_pt[w];
                    }
                    // See whether w is v's first son.
                    if (first_child == -1) {
                        first_child = w;
                    }

                    // Non-root v is a cut vertex if low_pt[w] >= number_dfs[v].
                    if (parent[v] != -1 and low_pt[w] >= number_dfs[v]) {

                        // Suggest to add an e between w and v's parent.
                        cutVertices.push_back(v);

                        pair<long, long> e(w, parent[v]);
                        addEdges.push_back(e);

                        if (only_one) {
                            return true;
                        }
                    }
                    // Root v is a cut vertex if v has two or more children.
                    if (parent[v] == -1 && w != first_child) {

                        // Suggest to add an e between those children.
                        cutVertices.push_back(v);
                        pair<long, long> e(w, first_child);
                        addEdges.push_back(e);
                        if (only_one) {
                            return true;
                        }
                    }
                }
            }

        }
    }

    return cutVertices.size() > 0;
}

long Triconnected_graph::build_dfs_tree(const long root, vector<int> &number_dfs, vector<long> &parent, vector<long> &childNr, list<long> &revS, long first_nr) {


/**

Testing and establishing biconnectivity
        Build up a dfs-tree starting from the node root by assigning each reachable
node in the graph a discovery time (number) and a parent.

:param self:
:param root: root is the node which should be the root of the dfs-tree.
:param number_dfs:number is assigned the number (discovery time) for each node.
The number of root is firstNr, the number of unvisited nodes is 0.
:param parent: parent is assigned the parent in the dfs-tree for each node.
:param childNr: childNr is assigned the number of children for each node.
:param revS: revS is assigned all visited nodes such that the top element of revS
        is the node that was visited last.
:param first_nr:firstNr is the index > 0 at which the numbering of the nodes starts.
:return: the number of visited nodes, i.e., nodes in the dfs-tree.
*
 */

    assert(first_nr > 0);
    list<long> stack_list = {root};
    long num_count = first_nr;

    // Build up search tree and note discovery time and parent for each node.
    while (!stack_list.empty()) {

        long node_v = stack_list.back();
        stack_list.pop_back();
        if (verbose) {
            cout << "node_v " << node_v << endl;
        }
        // Ignore nodes that were already visited.
        if (number_dfs[node_v] != -1) {
            continue;
        }

        revS.push_back(node_v);

        // Set correct discovery time for node_v
        number_dfs[node_v] = num_count;
        num_count += 1;
        if (verbose) {
            cout << "-> [";
        }
        // For all adjacent nodes w of v:
        for (long i_neighbour = row_ptr[node_v]; i_neighbour < row_ptr[node_v + 1]; i_neighbour++) {
            long node_w = col_ind[i_neighbour];
            if (verbose) {
                cout << node_w << ", ";
            }
            if (node_w != node_v) {
                // If w has not been visited yet:
                // Push it on the stack, remember its parent and number of children.
                if (number_dfs[node_w] == -1) {

                    stack_list.push_back(node_w);
                    // If a parent was determined previously, revert that.
                    if (parent[node_w] != -1) {
                        childNr[parent[node_w]] -= 1;
                    }
                    parent[node_w] = node_v;
                    childNr[node_v] += 1;
                }
            }
        }
        if (verbose) {
            cout << "]" << endl;
        }
    }
    return num_count - first_nr;
}

void Triconnected_graph::__DFS1(const long nodeV, const long nodeU) {

    /**
     * first dfs traversal
     */

    m_number_dfs[nodeV] = m_numCount;
    m_numCount += 1;

    m_FATHER[nodeV] = nodeU;

    m_low_pt_1[nodeV] = m_number_dfs[nodeV];
    m_low_pt_2[nodeV] = m_number_dfs[nodeV];

    m_NumberOfDescendants[nodeV] = 1;
    long nodeW;

    for (Edge *adj_edge : adj_edges[nodeV]) {

        if (nodeV == (*adj_edge).source) {
            nodeW = (*adj_edge).target;
        } else {
            nodeW = (*adj_edge).source;
        }

        if ((*adj_edge).type != edge_type_2_int["unseen"]) {
            // if self.m_TYPE[(*adj_edge).index] != self.edge_type_2_int["unseen"]:
            continue;
        }
        if (m_number_dfs[nodeW] == -1) {
            (*adj_edge).type = edge_type_2_int["tree"];
            // self.m_TYPE[(*adj_edge).index] = self.edge_type_2_int["tree"];

            m_TREE_ARC[nodeW] = adj_edge;

            __DFS1(nodeW, nodeV);

            if (m_low_pt_1[nodeW] < m_low_pt_1[nodeV]) {

                m_low_pt_2[nodeV] = min(m_low_pt_1[nodeV], m_low_pt_2[nodeW]);
                m_low_pt_1[nodeV] = m_low_pt_1[nodeW];
            } else if (m_low_pt_1[nodeW] == m_low_pt_1[nodeV]) {

                m_low_pt_2[nodeV] = min(m_low_pt_2[nodeV], m_low_pt_2[nodeW]);
            } else {


                m_low_pt_2[nodeV] = min(m_low_pt_2[nodeV], m_low_pt_1[nodeW]);
            }

            m_NumberOfDescendants[nodeV] += m_NumberOfDescendants[nodeW];
        } else {

            (*adj_edge).type = edge_type_2_int["frond"];
            // m_TYPE[(*adj_edge).index] = edge_type_2_int["frond"]

            if (m_number_dfs[nodeW] < m_low_pt_1[nodeV]) {

                m_low_pt_2[nodeV] = m_low_pt_1[nodeV];
                m_low_pt_1[nodeV] = m_number_dfs[nodeW];
            } else if (m_number_dfs[nodeW] > m_low_pt_1[nodeV]) {
                m_low_pt_2[nodeV] = min(m_low_pt_2[nodeV], (long) m_number_dfs[nodeW]);
            }
        }
    }
}

void Triconnected_graph::__DFS2() {

/**
 * A different numbering of the vertices are needed.
* During a second execution of DFS, the vertices of P are numbered from |V| to 1 in order they are last examined, that is,
* they are numbered according to the inverse post-order numbering.

 */
    m_NEWNUM = vector<long>(nb_of_nodes, 0);
    m_ORIGINAL = vector<long>(nb_of_nodes, 0);
    m_HIGHPT = vector<list<long>>(nb_of_nodes);

    m_IN_HIGH = vector<list<pair<long, long>>>(nb_of_edges);

    // self.m_START = np.zeros(self.number_of_edges, dtype=bool)  // .init(G, false);

    m_numCount = nb_of_nodes;
    m_newPath = true;

    __pathFinder(m_start);

    vector<long> old2new = vector<long>(nb_of_nodes, 0);

    for (long iV = 0; iV < nb_of_nodes; iV++) {
        old2new[m_number_dfs[iV]] = m_NEWNUM[iV];
        m_ORIGINAL[m_NEWNUM[iV]] = iV;
    }

    for (long iV = 0; iV < nb_of_nodes; iV++) {

        // self.m_NODEAT[self.m_NEWNUM[i_v]] = i_v
        m_low_pt_1[iV] = old2new[m_low_pt_1[iV]];
        m_low_pt_2[iV] = old2new[m_low_pt_2[iV]];
    }
}


short Triconnected_graph::computeTriconnectivity(list<unordered_set<long>> &triconnectedComponents, short partialTest) {

    // m_pGC = new GraphCopySimple(G);
    // GraphCopySimple & GC = *m_pGC;
    verbose = false;
    if (verbose) {
        cout << "Call of Triconnectivity::Triconnectivity nb Edges= " << nb_of_edges << " nb nodes " << nb_of_nodes << endl;
    }

    if (verbose) {
        cout << "Dividing G into triconnected components.\n" << endl;
        cout << "n = " << nb_of_nodes << " m= " << nb_of_edges << endl;
    }

    // special cases
    assert (nb_of_nodes >= 2);

    list<long> cut_vertices;
    if (!is_Biconnected_And_CutVertices(cut_vertices)) {
        cerr << "computeTriconnectivity: is_Biconnected_And_CutVertices() is false" << endl;
        // print( "self.edges", self.edges
//        raise ValueError
        return false;
    }

    if (nb_of_nodes <= 2) {

        assert (nb_of_edges >= 3);
        CompStruct *component = new CompStruct();

        // Process of edges
        for (Edge *edge: edges) {
            (*component).add(edge);
        }

        (*component).m_type = (*component).ComponentType2Int["bond"];
        m_component.push_back(component);
        return 0;
    }
    __splitMultiEdges();  // TODO does nothing !!!

    // initialize arrays for triconnectivity
    m_number_dfs = vector<int>(nb_of_nodes, -1);
    m_low_pt_1 = vector<long>(nb_of_nodes, -1);
    m_low_pt_2 = vector<long>(nb_of_nodes, -1);
    m_FATHER = vector<long>(nb_of_nodes, -1);
    m_NumberOfDescendants = vector<int>(nb_of_nodes, 0);

    m_TREE_ARC = vector<Edge *>(nb_of_nodes);// Node index to Edge

    // self.m_NODEAT = Array < node > (1, n);
    m_start = first_node();
    __DFS1(m_start, -1);

    // Debug use only
    if (partialTest == 1) {
        return 1;
    }

    for (Edge *edge : edges) {

        long i_v = (*edge).source;
        long i_w = (*edge).target;
        bool up = (m_number_dfs[i_w] - m_number_dfs[i_v]) > 0;

        if ((up && ((*edge).type == edge_type_2_int["frond"])) || (!up && ((*edge).type == edge_type_2_int["tree"]))) {
            // if (up and self.m_TYPE[i_e] == self.edge_type_2_int["frond"]) or (not up and self.m_TYPE[i_e] == self.edge_type_2_int["tree"]):
            (*edge).reverse();
        }
    }
    if (verbose) {
        cout << "\nnode\tNUMBER\tFATHER\tLOWPT1\tLOWPT2\tND" << endl;


        for (long i_v = 0; i_v < nb_of_nodes; i_v++) {
            cout << i_v << ":  \t" << m_number_dfs[i_v] << "   \t";


            if (m_FATHER[i_v] == -1) {
                cout << "nil \t";
            } else {
                cout << m_FATHER[i_v] << "   \t";
            }


            cout << m_low_pt_1[i_v] << "   \t" << m_low_pt_2[i_v] << "   \t" << m_NumberOfDescendants[i_v] << endl;
        }
    }
    m_A = vector<list<Edge *>>(nb_of_nodes); // adjacency list of v
//    m_IN_ADJ = [[];  //self.number_of_edges)]  // dict()  // .init(GC, nullptr); // pointer to element in adjacency list containing e
    __buildAcceptableAdjStruct();

    if (verbose) {
        cout << "\nadjacency lists:" << endl;

        for (long i_v = 0; i_v < nb_of_nodes; i_v++) {
            cout << i_v << "\t";
            for (Edge *ei :m_A[i_v]) {
                (*ei).print();
                cout << ", ";
            }
            cout << endl;
        }
    }
    if (partialTest == 2) {
        return 2;
    }


    __DFS2();

    if (verbose) {
        cout << "\nnode\tNEWNUM\tLOWPT1\tLOWPT2\tHIGHPT" << endl;
        for (int i_v = 0; i_v < nb_of_nodes; i_v++) {
            cout << i_v << ":  \t" << m_NEWNUM[i_v] << "   \t";
            cout << m_low_pt_1[i_v] << "   \t" << m_low_pt_2[i_v] << "   \t";
            for (long i:m_HIGHPT[i_v]) {
                cout << i << ", ";
            }
            cout << endl;
        }
        cout << "\nedges starting a path:" << endl;
        for (long i_e = 0; i_e < nb_of_edges; i_e++) {
            if (edges[i_e]->start) {
                (*edges[i_e]).print();
                cout << ", ";
            }
        }
        cout << endl;

    }

    if (partialTest == 3) {
        return 3;
    }


    if (verbose) {
        cout << endl;
    }
    m_TSTACK_h = vector<long>(2 * nb_of_edges + 1, 0);
    m_TSTACK_a = vector<long>(2 * nb_of_edges + 1, 0);
    m_TSTACK_b = vector<long>(2 * nb_of_edges + 1, 0);
    m_top = 0;
    m_TSTACK_a[m_top] = -1; // start with EOS

    __pathSearch(m_start);
    if (partialTest == 4) {
        return 4;
    }

    short cType;
//    ComponentType2Int = {"bond": 0, "polygon": 1, "triconnected": 2}
    // last split component
    if (m_estack.size() > 4) {
        cType = 2; //CompStruct.ComponentType2Int["triconnected"];
    } else {
        cType = 1;  //CompStruct.ComponentType2Int["polygon"]
    }
    CompStruct *C = __newComp(cType);
    while (!m_estack.empty()) {
        (*C).add(m_estack.back()); //pop()
        m_estack.pop_back();
    }


    if (verbose) {
        if ((*C).m_type == 1) {
            cout << "Creation Polygon " << m_component.size() << ": \t";
        } else {
            cout << "Creation triconnected " << m_component.size() << ": \t";
        }


        for (Edge *ed: (*C).m_edges) {
            (*ed).print();
        }
        cout << endl;
    }

//    if (verbose){
//        __printStacks();
//    }


    m_TSTACK_h = {};
    m_TSTACK_a = {};
    m_TSTACK_b = {};

    __assembleTriconnectedComponents();

    // Caution: checkComp() assumes that the graph is simple!
    //    assert (__check_comp());

    if (verbose) {
        cout << "\n\nTriconnected components:\n" << endl;

        for (int iC = 0; iC < m_component.size(); iC++) {
            CompStruct *component = m_component[iC];

            if ((*component).m_edges.size() == 0) {
                continue;
            }
            cout << "[" << iC << "] ";

            cout << (*component).Int2ComponentType[(*component).m_type];
            for (Edge *edge: (*component).m_edges) {
                (*edge).print();
                cout << ", ";
            }
            cout << endl;
        }
    }

    // Compute a simpler way to express triconnectivity:
//    list<unordered_set<long>> triconnectedComponents = {};

    //    for iC, component in enumerate(self.m_component):
//    cout<<"toto"<<endl;
    for (int iC = 0; iC < m_component.size(); iC++) {
        CompStruct *component = m_component[iC];

        if ((*component).m_edges.size() == 0) {
            continue;
        }

        if ((*component).m_type == 2) {

            unordered_set<long> vertices;
            unordered_map<long, long> degree;
            for (Edge *edge: (*component).m_edges) {

                if ((*edge).index != -1 || is_found_v_edges(edge, edges)) {// virtual edges or real edge
                    if (degree.count((*edge).source)) {
                        degree[(*edge).source] += 1;
                    } else {
                        degree[(*edge).source] = 1;
                    }

                    if (degree.count((*edge).target)) {
                        degree[(*edge).target] += 1;
                    } else {
                        degree[(*edge).target] = 1;
                    }
                    vertices.insert((*edge).source);
                    vertices.insert((*edge).target);
//                    if(verbose){
//                        (*edge).print();
//                        cout<<endl;
//                        cout<<"insert "<<(*edge).source <<" and "<<(*edge).target<<" and vertices={";
//                        for (long tmp: vertices)
//                        {
//                            cout<<tmp<<", ";
//                        }
//                        cout<<endl;
//                    }

                }
            }
            if (verbose) {
                cout << endl;
                cout << "vertices={";
                for (long tmp: vertices) {
                    cout << tmp << ", ";
                }
                cout << "}" << endl;
                cout << "degree={";
                for (auto i_k_v: degree) {
                    cout << "{" << i_k_v.first << ", " << i_k_v.second << "}, ";
                }
                cout << "}" << endl;

            }
            // print( "vertices", vertices, degree
            bool isVertexRemoved = false;
            for (const auto i_k_v : degree) {
                if (i_k_v.second < 3) {
                    long i_v = i_k_v.first;
                    vertices.erase(i_v);
                    isVertexRemoved = true;
                    for (Edge *tmp_edge: (*component).m_edges) {
                        if ((*tmp_edge).index != -1 || is_found_v_edges(tmp_edge, edges)) {// virtual edges or real edge
                            if (i_v == (*tmp_edge).source || i_v == (*tmp_edge).target) {
                                long opposite = -1;
                                if (i_v == (*tmp_edge).source) {
                                    opposite = (*tmp_edge).target;
                                } else {
                                    opposite = (*tmp_edge).source;
                                }
                                if (vertices.count(opposite)) {
                                    degree[opposite] -= 1;
                                }
                            }
                        }

                    }
                }
            }
            // print( "vertices", vertices,  degree
            while (isVertexRemoved) {

                isVertexRemoved = false;
                unordered_set<long> cpy_vertices(vertices);
                for (long i_v :cpy_vertices) {
                    if (degree[i_v] < 3) {
                        vertices.erase(i_v);
                        isVertexRemoved = true;
                        for (Edge *tmp_edge: (*component).m_edges) {
                            if ((*tmp_edge).index != -1 || is_found_v_edges(tmp_edge, edges)) {// virtual edges or real edge
                                if (i_v == (*tmp_edge).source || i_v == (*tmp_edge).target) {
                                    long opposite = -1;
                                    if (i_v == (*tmp_edge).source) {
                                        opposite = (*tmp_edge).target;
                                    } else {
                                        opposite = (*tmp_edge).source;
                                    }
                                    if (vertices.count(opposite)) {
                                        degree[opposite] -= 1;
                                    }
                                }
                            }
                        }

                    }
                }
            }

            if (!vertices.empty()) {
                triconnectedComponents.push_back(vertices);
            }
        }
    }
    return 5;


}

void Triconnected_graph::__splitMultiEdges() {
/**
 * splits bundles of multi-edges into bonds and creates a new virtual edge in GC.
 */

}


void Triconnected_graph::__buildAcceptableAdjStruct() {
/**
 *  Construction of ordered adjacency lists
 */

    if (verbose) {
        cout << "\nCall of __buildAcceptableAdjStruct " << edges.size() << endl;
    }


    long maximum = 3 * nb_of_nodes + 2;
    unordered_map<long, list<Edge *>> BUCKET; // (1, maximum)

    long phi = -1;

    for (Edge *edge :edges) {

        long nodeV = (*edge).source;
        long nodeW = (*edge).target;

        unsigned short t = (*edge).type;  // t = self.m_TYPE[iEdge]

        if (t == edge_type_2_int["removed"]) {
            if (verbose) {
                cout << "edge removed: (" << (*edge).source << ", " << (*edge).target << endl;
            }
            continue;
        }
        if (t == edge_type_2_int["frond"]) {
            phi = 3 * m_number_dfs[nodeW] + 1;
        } else if (m_low_pt_2[nodeW] < m_number_dfs[nodeV]) {
            phi = 3 * m_low_pt_1[nodeW];
        } else {
            phi = 3 * m_low_pt_1[nodeW] + 2;
        }
        BUCKET[phi].push_back(edge);
    }


    for (long i = 0; i < maximum + 1; i++) {
        if (BUCKET.count(i)) {
            for (Edge *e :BUCKET[i]) {
                m_A[(*e).source].push_back(e);
                // self.m_IN_ADJ[e.index].append((e.source, len(self.m_A[e.source]) - 1))
            }
        }
    }
    // print( "__buildAcceptableAdjStruct self.m_A", self.m_A
}

void Triconnected_graph::__pathFinder(long iV) {


    /**
      * The second dfs-search
      */

    m_NEWNUM[iV] = m_numCount - m_NumberOfDescendants[iV];

    for (Edge *edge : m_A[iV]) {

        long iEdge = (*edge).index;
        long iW;
        // TODO Definir fonction opposite(i_v) pour une arete
        if ((*edge).source == iV) {
            iW = (*edge).target;
        } else {
            iW = (*edge).source;
        }

        if (m_newPath) {

            m_newPath = false;
            (*edge).start = true;
            // self.m_START[iEdge] = True
        }
        if ((*edge).type == edge_type_2_int["tree"]) {

            // if self.m_TYPE[iEdge] == self.edge_type_2_int["tree"]:
            __pathFinder(iW);
            m_numCount -= 1;
        } else {

            long pos = m_HIGHPT[iW].size();
            m_HIGHPT[iW].push_back(m_NEWNUM[iV]);
            pair<long, long> p(iW, pos);
            m_IN_HIGH[iEdge].push_back(p); // self.m_HIGHPT[i_w].pushBack(self.m_NEWNUM[i_v]);
            m_newPath = true;
        }
    }
}

void Triconnected_graph::__pathSearch(long iV) {

    /**
     * recognition of split components
     */
//     verbose=true;
    if (verbose) {
        cout << "\n\tCall of __pathSearch " << iV << " m_top " << m_top << endl;
        cout << "\t\tm_HIGHPT ";
        print_m_HIGHPT();
        cout << endl;

        cout << "\t\tm_estack [";
        for (Edge *e: m_estack) {
            (*e).print();
            cout << ", ";
        }
        cout << "]" << endl;

        cout << "\t\tm_TSTACK_a [";
        for (long e: m_TSTACK_a) {
            cout << e << ", ";
        }
        cout << "]" << endl;
    }

    long v_num = m_NEWNUM[iV];

    list<Edge *> adj = m_A[iV];
    // print( "adj", adj
    long out_v = adj.size();

    for (Edge *pit : adj) {
        if (verbose) {
            cout << "\t\t\tpit ";
            (*pit).print();
            cout << "\ti_v: " << iV;
            cout << "\t m_A[i_v] ";
            print_m_A(iV);
//            cout << "\tm_A[i_v]\t" << m_A[iV] << endl;
            cout << endl;
        }


        Edge *edge = pit;

        long i_w = (*edge).target;
        long w_num = m_NEWNUM[i_w];

        if ((*edge).type == edge_type_2_int["tree"]) {

            if (verbose) {
                cout << "\t\t\tIn\t" << iV << ", " << i_w << " Tree " << "m_top " << m_top << endl;
            }
            if ((*edge).start) {

                long y = 0;
                if (m_TSTACK_a[m_top] > m_low_pt_1[i_w]) {
                    long b;
                    while (m_TSTACK_a[m_top] > m_low_pt_1[i_w]) {
                        y = max(y, m_TSTACK_h[m_top]);  // New
                        b = m_TSTACK_b[m_top];
                        m_top -= 1;
                    }
                    // do {
                    //     y = max(y,m_TSTACK_h[m_top])
                    //     b = m_TSTACK_b[m_top]
                    // m_top -= 1
                    // } while (m_TSTACK_a[m_top] > m_low_pt_1[i_w])
                    TSTACK_push(y, m_low_pt_1[i_w], b);  // New
                } else {
                    TSTACK_push(w_num + m_NumberOfDescendants[i_w] - 1, m_low_pt_1[i_w], v_num); // New
                }
                TSTACK_pushEOS();
            }
            __pathSearch(i_w);

            m_estack.push_back(m_TREE_ARC[i_w]);  // add (v,w) to ESTACK (can differ from e!)

            // node x
            if (verbose) {
                cout << "\t\tOut\t" << iV << ", " << i_w << " Tree " << "m_top " << m_top << endl;
            }

            // print( "m_HIGHPT", m_HIGHPT
            // print( "m_estack", m_estack
            // print( "m_TSTACK_a", m_TSTACK_h
            // print( "m_TSTACK_a", m_TSTACK_a
            // print( "m_TSTACK_a", m_TSTACK_b
            // print( "m_NEWNUM", m_NEWNUM
            // print( "i_v", i_v, "v_num", v_num, "i_w", i_w, "w_num", w_num, "m_top", m_top, "m_A[i_w][0].target", m_A[i_w][0].target
            // print( "\nv_num != 0", v_num != 0
            // print( "(m_TSTACK_a[m_top] == v_num)", (m_TSTACK_a[m_top] == v_num)
            // print( "m_degree[i_w] == 2", m_degree[i_w] == 2
            // print( "i_w", i_w, "m_A[i_w]", m_A[i_w]
            // print( "m_A[i_w][0].target", m_A[i_w][0].target
            // print( "m_NEWNUM[m_A[i_w][0].target] > w_num", m_NEWNUM[m_A[i_w][0].target] > w_num

            while (v_num != 0 && ((m_TSTACK_a[m_top] == v_num) ||
                                  (m_degree[i_w] == 2 && m_NEWNUM[(*m_A[i_w].front()).target] > w_num))) {


                long i_v_a = m_TSTACK_a[m_top];  // New
                long i_v_b = m_TSTACK_b[m_top];  // New
                if (verbose) {
                    cout << "\n\t\ta " << i_v_a << " b " << i_v_b << " wnum " << w_num << " m_FATHER[m_NODEAT[b]] " << m_NEWNUM[m_FATHER[m_ORIGINAL[i_v_b]]] << " m_ORIGINAL[i_v_b] "
                         << m_ORIGINAL[i_v_b] << " m_FATHER[m_ORIGINAL[i_v_b]] " << m_FATHER[m_ORIGINAL[i_v_b]] << endl;
                }

                // edge e_virt

                if (i_v_a == v_num and m_NEWNUM[m_FATHER[m_ORIGINAL[i_v_b]]] == i_v_a) {// New
                    // if a == v_num and m_FATHER[m_NODEAT[b]] == m_NODEAT[a] :
                    m_top -= 1;
                } else {
                    long i_x = -1;
                    Edge *e_ab = NULL;  // edge
                    Edge *e_virt = NULL;
                    if (m_degree[i_w] == 2 && m_NEWNUM[(*m_A[i_w].front()).target] > w_num) {

                        e_virt = __pathSearch_sub_type_2_1(iV, i_w, i_x);

                    } else {

                        if (verbose) {
                            // print( "\nfound type-2 separation pair ", m_pGC->original(m_NODEAT[a]), ", ", m_pGC->original(m_NODEAT[b])
                            cout << endl;
                            cout << "\nfound type-2 separation pair [else] " << m_ORIGINAL[i_v_a] << ", " << m_ORIGINAL[i_v_b] << endl;
                        }

                        // Storage of separation pair
                        long i_v_0 = m_ORIGINAL[i_v_a];
                        long i_v_1 = m_ORIGINAL[i_v_b];

                        if (i_v_0 > i_v_1) {
                            separation_pair.push_back(pair<long, long>(i_v_1, i_v_0));
                        } else {
                            separation_pair.push_back(pair<long, long>(i_v_0, i_v_1));
                        }

                        long h = m_TSTACK_h[m_top];
                        m_top -= 1;

                        CompStruct *C = __newComp(-1);
                        while (true) {
                            Edge *xy = m_estack.back(); // .top()  //return edge...
                            i_x = (*xy).source;
                            long xyTarget = (*xy).target;

                            if (!(i_v_a <= m_NEWNUM[i_x] <= h && (i_v_a <= m_NEWNUM[xyTarget] && m_NEWNUM[xyTarget] <= h))) {
                                break;
                            }

                            if ((m_NEWNUM[i_x] == i_v_a && m_NEWNUM[xyTarget] == i_v_b) || \
                                        (m_NEWNUM[xyTarget] == i_v_a && m_NEWNUM[i_x] == i_v_b)) {

                                e_ab = m_estack.back();  //pop()
                                m_estack.pop_back();
                                // print( "m_A[e_ab.source]",  m_A[e_ab.source], e_ab
                                m_A[(*e_ab).source].remove(e_ab);
                                if (verbose) {
                                    cout << "\tAFTER m_A[e_ab.source].remove(e_ab) ";
//                                    for (list<Edge *>::const_iterator it = m_A[(*e_ab).source].begin(); it != m_A[(*e_ab).source].end(); ++it) {
//                                        cout << "\t";
//                                        (**it).print();
//                                        cout << endl;
//                                    }
//                                    cout << "]" << endl;
                                    print_m_A((*e_ab).source);
                                    cout << endl;
                                }
                                // print( "__delHigh(e_ab) 2)"
                                __delHigh(e_ab);
                            } else {

                                Edge *eh = m_estack.back();  //pop()
                                m_estack.pop_back();

                                if ((*pit) != (*eh)) {  // m_IN_ADJ[eh.index]:
                                    //TODO is it correct: it was: if (pit != eh){ in python...

                                    m_A[(*eh).source].remove(eh);
                                    if (verbose) {
                                        cout << "\tAFTER m_A[eh.source].remove(eh): ";
//                                        for (list<Edge *>::const_iterator it = m_A[(*eh).source].begin(); it != m_A[(*eh).source].end(); ++it) {
//                                            cout << "\t";
//                                            (**it).print();
//                                            cout << endl;
//                                        }
//                                        cout << "]" << endl;
                                        print_m_A((*eh).source);
                                        // print( "__delHigh(eh)"
                                        cout << endl;
                                    }
                                    __delHigh(eh);
                                }
                                (*C).add(eh);
                                m_degree[i_x] -= 1;
                                m_degree[xyTarget] -= 1;
                            }
                        }
                        // TODO delete pointer on edge!
                        e_virt = new Edge(m_ORIGINAL[i_v_a], m_ORIGINAL[i_v_b]);

                        virt_edges.push_back(e_virt);

                        (*C).finishTricOrPoly(e_virt);
                        if (verbose) {
                            print_verbose_creation_poly_or_triconnected(C);

//                            if ((*C).m_type == 1) {
//                                cout << "Creation Polygon " << m_component.size() << ": ";
//                            } else {
//                                cout << "Creation triconnected " << m_component.size() << ": ";
//                            }
//
//                            for (Edge *ed : (*C).m_edges) {
//                                (*ed).print();
//                                cout << ", ";
//                            }
//                            cout << endl;
                        }
                        i_x = m_ORIGINAL[i_v_b];
                        if (verbose) {
                            cout << "i_x " << i_x << " i_v_b " << i_v_b << endl;
                        }
                    }
                    if (e_ab != NULL) {

                        CompStruct *C = __newComp(0); // "bond"
                        (*C).add(e_ab);
                        (*C).add(e_virt);
                        if (verbose) {
                            cout << "Creation Bond " << m_component.size() << ": ";
                            (*e_ab).print();
                            cout << "\t";
                            (*e_virt).print();
                        }
                        e_virt = new Edge(iV, i_x);
                        virt_edges.push_back(e_virt);
                        (*C).add(e_virt);
                        if (verbose) {
                            (*e_virt).print();
                            cout << endl;
                        }
                        m_degree[i_x] -= 1;
                        m_degree[iV] -= 1;
                    }
                    m_estack.push_back(e_virt);

                    // edge = e_virt
                    // m_IN_ADJ[e_virt] = it
                    // Remplacement de edge par e_virt
                    if (verbose) {
                        cout << "pit";
                        (*pit).print();
                        cout << "\te_virt\t";
                        (*e_virt).print();
                        cout << "\tm_A[pit.source]\t";
//                        for (list<Edge *>::const_iterator it = m_A[(*pit).source].begin(); it != m_A[(*pit).source].end(); ++it) {
//                            cout << "\t";
//                            (**it).print();
//                            cout << endl;
//                        }
//                        cout << "]" << endl;
                        print_m_A((*pit).source);
                        cout << endl;
                    }
                    //if pit in m_A[(*pit).source]:
                    bool is_found = false;

//                        for (Edge *e :m_A[(*pit).source]) {
//                        if ((*pit) == (*e)) {
//                            is_found = true;
//                            e = e_virt;  //verifier cela, je ne sais pas si ca va marcher...
//                            break;
//                        }
//                    }

                    long size_list = m_A[(*pit).source].size();
                    list<Edge *>::iterator it;
                    for (it = m_A[(*pit).source].begin(); it != m_A[(*pit).source].end(); ++it) {
                        if ((*pit) == (**it)) {
                            is_found = true;
                            break;
                        }
                    }
                    if (is_found) {
                        it = m_A[(*pit).source].erase(it);
                        m_A[(*pit).source].insert(it, e_virt);


                        assert(size_list == m_A[(*pit).source].size());
//                        pos = m_A[(*pit).source]).index(pit);
//                        m_A[(*pit).source][pos] = e_virt;
                    } else {
                        m_A[(*pit).source].push_back(e_virt);
                    }
                    if (verbose) {
                        cout << "\telse: AFTER m_A[pit.source][pos] = e_virt ";
//                        for (list<Edge *>::const_iterator it = m_A[(*pit).source].begin(); it != m_A[(*pit).source].end(); ++it) {
//                            cout << "\t";
//                            (**it).print();
//                            cout << endl;
//                        }
//
//                        cout << "]" << endl;
                        print_m_A((*pit).source);
                        cout << endl;
                    }
                    pit = e_virt;
                    m_degree[i_x] += 1;
                    m_degree[iV] += 1;
                    m_FATHER[i_x] = iV;
                    if (verbose) {
                        cout << "\t\t\t__pathSearch(self, iV): iV " << iV << " i_x " << i_x << " m_FATHER [";

                        for (long i :m_FATHER) {
                            cout << i << ", ";
                        }
                        cout << "]" << endl;
                    }
                    m_TREE_ARC[i_x] = e_virt;
                    (*e_virt).type = edge_type_2_int["tree"];
                    // m_TYPE[e_virt] = edge_type_2_int("tree")

                    i_w = i_x;
                    w_num = m_NEWNUM[i_w];
                }
                // print( "output"
                // print( "v_num != 0", v_num != 0
                // print( "(m_TSTACK_a[m_top] == v_num)", (m_TSTACK_a[m_top] == v_num)
                // print( "m_degree[i_w] == 2", m_degree[i_w] == 2
                // print( "m_NEWNUM[m_A[i_w][0].target] > w_num", m_NEWNUM[m_A[i_w][0].target] > w_num

            }
            // print( ""
            // print( "i_v", i_v, "v_num", v_num, "out_v",out_v
            // print( "i_w", i_w, "m_low_pt_1[i_w]", m_low_pt_1[i_w], "original", m_ORIGINAL[m_low_pt_1[i_w]]
            // print( "m_low_pt_1", m_low_pt_1
            // print( "m_low_pt_2", m_low_pt_2
            // print( "m_FATHER", m_FATHER
            if ((m_low_pt_1[i_w] < v_num && v_num <= m_low_pt_2[i_w]) &&
                (m_FATHER[iV] != m_start || out_v >= 2)) {

                if (verbose) {
                    cout << "\nfound type-1 separation pair " << m_ORIGINAL[m_low_pt_1[i_w]] << ", " << iV << " m_top " << m_top << endl;
                    // print( "\nfound type-1 separation pair ", m_pGC->original(m_NODEAT[m_low_pt_1[i_w]]), ", ", m_pGC->original(v)
                }

                // Storage of separation pair
                long i_v_0 = m_ORIGINAL[m_low_pt_1[i_w]];
                long i_v_1 = iV;
                if (i_v_0 > i_v_1) {
                    separation_pair.push_back(pair<long, long>(i_v_1, i_v_0));
                } else {
                    separation_pair.push_back(pair<long, long>(i_v_0, i_v_1));
                }
                CompStruct *C = __newComp(-1);
                long xx;
                long y;
                assert (!m_estack.empty());  // otherwise undefined behavior since i_x is not initialized
                while (!m_estack.empty()) {

                    Edge *xy = m_estack.back();  // .top()
                    xx = m_NEWNUM[(*xy).source];
                    y = m_NEWNUM[(*xy).target];

                    // if (!  ((wnum <= xx & & xx < wnum+m_ND[w]) | | (wnum <= y & & y < wnum + m_ND[w])))
                    if (!((w_num <= xx && xx < w_num + m_NumberOfDescendants[i_w]) || (
                            w_num <= y && y < w_num + m_NumberOfDescendants[i_w]))) {

                        if (verbose) {
                            cout << "BREAK!" << endl;
                        }
                        break;
                    }
                    (*C).add(m_estack.back());
                    m_estack.pop_back();

                    if (verbose) {
                        cout << "\t\t__delHigh(xy)";
                        (*xy).print();
                        cout << " (" << xx << ", " << y << ") " << "m_top " << m_top << endl;
                    }
                    __delHigh(xy);
                    m_degree[(*xy).source] -= 1;
                    m_degree[(*xy).target] -= 1;
                }

                // print( "m_HIGHPT", m_HIGHPT
                Edge *e_virt = new Edge(iV, m_ORIGINAL[m_low_pt_1[i_w]]);
                virt_edges.push_back(e_virt);
                (*C).finishTricOrPoly(e_virt);
                if (verbose) {
                    print_verbose_creation_poly_or_triconnected(C);

                    cout << "\t\te_virt ";
                    (*e_virt).print();
                    cout << "xx: " << xx << " v_num: " << v_num << " y " << y << " i_w " << i_w << " m_low_pt_1[i_w] " << m_low_pt_1[i_w] << " m_FATHER[i_v] " << m_NEWNUM[m_FATHER[iV]] << endl;
                }

                if ((xx == v_num && y == m_low_pt_1[i_w]) || (y == v_num and xx == m_low_pt_1[i_w])) {

                    if (verbose) {
                        cout << "\t\tcompBond eh " << "m_top " << m_top << endl;
                    }
                    CompStruct *compBond = __newComp(0);  // "bond"

                    Edge *eh = m_estack.back();
                    m_estack.pop_back();

                    // if m_IN_ADJ[eh] != edge :
                    if (verbose) {
                        cout << "\t\teh ";
                        (*eh).print();
                        cout << " pit ";
                        (*pit).print();
                        cout << " eh.source " << (*eh).source << "m_A[eh.source]";
                        print_m_A((*eh).source);
                    }
                    if ((*eh) != (*pit)) {
                        m_A[(*eh).source].remove(eh);
                        if (verbose) {
                            cout << "\tAFTER m_A[eh.source].remove(eh)";
                            print_m_A((*eh).source);
                        }
                    }

                    (*compBond).add(eh);
                    (*compBond).add(e_virt);
                    if (verbose) {
                        cout << "Creation Bond " << m_component.size() << " : ";
                        (*eh).print();
                        cout << "\t";
                        (*e_virt).print();
                        cout << endl;
                    }
                    e_virt = new Edge(iV, m_ORIGINAL[m_low_pt_1[i_w]]);
                    virt_edges.push_back(e_virt);
                    if (verbose) {
                        (*e_virt).print();
                        cout << endl;
                    }

                    (*compBond).add(e_virt);

                    // m_IN_HIGH[e_virt] = m_IN_HIGH[eh]
                    m_degree[iV] -= 1;
                    m_degree[m_ORIGINAL[m_low_pt_1[i_w]]] -= 1;
                }
                if (m_low_pt_1[i_w] != m_NEWNUM[m_FATHER[iV]]) {

                    if (verbose) {
                        cout << "\n\t\tm_low_pt_1[i_w] != m_FATHER[i_v]" << endl;
                        cout << "\t\t\tedge";
                        (*edge).print();
                        cout << " pit ";
                        (*pit).print();
                        cout << " e_virt ";
                        (*e_virt).print();
                        cout << " v_num " << v_num << " m_low_pt_1[i_w] " << m_low_pt_1[i_w] << " m_top " << m_top << endl;
                    }

                    m_estack.push_back(e_virt);

                    if (verbose) {
                        cout << "\t\t\tm_A:" << (*e_virt).source;
                        print_m_A((*e_virt).source);
                    }

                    // Remplacement de edge par e_virt
//                    if pit in m_A[pit.source]:  // Call of Edge.__eq__
//                    for (Edge *e :m_A[(*pit).source]) {
//                        if ((*pit) == (*e)) {
//                            cout << "TODO Check this!" << endl;
//                            e = e_virt;  //TODO verifier cela, je ne sais pas si ca va marcher...
//                            break;
//                        }
//                    }

                    bool is_found = false;
                    long size_list = m_A[(*pit).source].size();

                    list<Edge *>::iterator it;
                    for (it = m_A[(*pit).source].begin(); it != m_A[(*pit).source].end(); ++it) {
                        if ((*pit) == (**it)) {
                            is_found = true;
                            break;
                        }
                    }
                    if (is_found) {

                        it = m_A[(*pit).source].erase(it);
                        m_A[(*pit).source].insert(it, e_virt);

                        assert(size_list == m_A[(*pit).source].size());
//                        pos = m_A[(*pit).source]).index(pit);
//                        m_A[(*pit).source][pos] = e_virt;
                    }
                    // End Remplacement de edge par e_virt


//                    pos = m_A[pit.source].index(pit)
//                    m_A[pit.source][pos] = e_virt
                    if (verbose) {
                        cout << "\tAFTER m_A[pit.source][pos] = e_virt";
                        print_m_A((*pit).source);
                    }
                    // *it = e_virt
                    pit = e_virt;

                    // m_IN_ADJ[e_virt] = it
                    is_found = false;
                    for (long i : m_HIGHPT[m_ORIGINAL[m_low_pt_1[i_w]]]) {
                        if (i == m_NEWNUM[iV]) {
                            is_found = true;
                            break;
                        }
                    }
//                    if m_NEWNUM[iV]
//                    not in
//                    m_HIGHPT[
//                            m_ORIGINAL[m_low_pt_1[i_w]]] and __high(
//                            m_ORIGINAL[
//                                    m_low_pt_1[i_w]]) < v_num:
                    if ((!is_found) && (__high(m_ORIGINAL[m_low_pt_1[i_w]]) < v_num)) {

                        // cout<<'\tiV not in m_HIGHPT[m_low_pt_1[i_w]] and __high(m_low_pt_1[i_w]) < v_num:'
                        // if not m_IN_HIGH[e_virt] and __high(m_low_pt_1[i_w]) < v_num:
                        m_HIGHPT[m_ORIGINAL[m_low_pt_1[i_w]]].push_front(v_num);  // pushFront(v_num)
                        // m_IN_HIGH[e_virt] = m_HIGHPT[m_low_pt_1[i_w]].appendleft(v_num)  // pushFront(v_num)
                        // cout<< "m_HIGHPT", m_HIGHPT
                    }
                    if (verbose) {
                        cout << "\t\t\tm_A:" << (*e_virt).source;
                        print_m_A((*e_virt).source);
                    }
                    m_degree[iV] += 1;
                    // m_degree[v]+=1m_degree[v]+=1m_degree[v]+=1
                    m_degree[m_ORIGINAL[m_low_pt_1[i_w]]] += 1;
                } else {

                    if (verbose) {
                        cout << "\t\tm_low_pt_1[i_w] == m_FATHER[i_v]" << "m_top" << m_top << endl;
                        cout << "\t\t\te_virt";
                        (*e_virt).print();
                    }
                    m_A[iV].remove(pit);
                    if (verbose) {
                        cout << "\tAFTER m_A[i_v].remove(pit)";
                        print_m_A(iV);
                    }
                    // adj.remove(pit)
                    CompStruct *compBond = __newComp(0);  // "bond"
                    (*compBond).add(e_virt);
                    if (verbose) {
                        cout << "Creation Bond " << m_component.size() << ": ";
                        (*e_virt).print();
                    }
                    e_virt = new Edge(m_ORIGINAL[m_low_pt_1[i_w]], iV);
                    virt_edges.push_back(e_virt);
                    (*compBond).add(e_virt);
                    if (verbose) {
                        cout << " ";
                        (*e_virt).print();
                    }
                    Edge *eh = m_TREE_ARC[iV];

                    (*compBond).add(m_TREE_ARC[iV]);
                    if (verbose) {
                        (*m_TREE_ARC[iV]).print();
                    }
                    m_TREE_ARC[iV] = e_virt;

                    (*e_virt).type = edge_type_2_int["tree"];
                    // m_TYPE[e_virt.index] = edge_type_2_int("tree")

                    // m_IN_ADJ[e_virt] = m_IN_ADJ[eh]
                    // m_IN_ADJ[eh] = e_virt
                    if (verbose) {
                        cout << "\t\t\tRemplacement dans m_A[" << (*eh).source << "] de ";
                        (*eh).print();
                        cout << " par ";
                        (*e_virt).print();
                    }
//                    pos = m_A[eh.source].index(eh)
//                        m_A[eh.source][pos] = e_virt
//                        if (verbose) {
//                            cout << "\tAFTER m_A[eh.source][pos] = e_virt", m_A[eh.source]);
//                        }
//                    }
                    //remplacement de eh par e_virt
//                    for (Edge *e :m_A[(*eh).source]) {
//                        if ((*pit) == (*eh)) {
//                            cout << "TODO Check this!" << endl;
//                            eh = e_virt;  //TODO verifier cela, je ne sais pas si ca va marcher...
//                            break;
//                        }
//                    }
                    bool is_found = false;
                    long size_list = m_A[(*eh).source].size();

                    list<Edge *>::iterator it;
                    for (it = m_A[(*eh).source].begin(); it != m_A[(*eh).source].end(); ++it) {
                        if ((*eh) == (**it)) {
                            is_found = true;
                            break;
                        }
                    }
                    if (is_found) {
                        it = m_A[(*eh).source].erase(it);
                        m_A[(*eh).source].insert(it, e_virt);
                        assert(size_list == m_A[(*eh).source].size());
                    }
                    // End Remplacement de edge par e_virt
                }
            }
            // cout<< "\t\tedge",edge
            // cout<< "\t\tbefore edge.start", "m_top", m_top
            if ((*edge).start) {

                // cout<< "\t\tInside edge.start", "m_top", m_top
                // if m_START[edge]:
                while (TSTACK_notEOS()) {
                    m_top -= 1;
                }
                // cout<< "\t\tInside edge.start", "m_top", m_top
                m_top -= 1;
            }
            // cout<< "\t\tduring edge.start", "m_top", m_top
            // cout<< "\t\tTSTACK_notEOS()", TSTACK_notEOS(), "m_TSTACK_b[m_top]", m_TSTACK_b[m_top], "v_num", v_num, "i_v", i_v, \
            //     "__high(i_v)",     __high(i_v), "m_TSTACK_h[m_top]", m_TSTACK_h[m_top]
            while ((TSTACK_notEOS()) && (m_TSTACK_b[m_top] != v_num) && (__high(iV) > m_TSTACK_h[m_top])) {
                m_top -= 1;
            }
            out_v -= 1;
            // cout<< "\t\tafter edge.start", "m_top", m_top, "out_v", out_v
        } else {

            // frond arc
            if (verbose) {
                cout << "\t\t" << iV << " " << i_w << " " << "Frond" << " " << "m_top" << " " << m_top << endl;
            }

            if ((*edge).start) {
                // if m_START[edge]:
                long y = 0;
                if (m_TSTACK_a[m_top] > w_num) {
                    long b;
                    while (m_TSTACK_a[m_top] > w_num) {

                        y = max(y, m_TSTACK_h[m_top]);
                        b = m_TSTACK_b[m_top];
                        m_top -= 1;
                    }
                    TSTACK_push(y, w_num, b);
                } else {
                    TSTACK_push(v_num, w_num, v_num);
                }
            }
            m_estack.push_back(edge);  // add (v,i_w) to ESTACK
        }
    }
}

void Triconnected_graph::TSTACK_push(long h, long a, long b) {
    m_top += 1;
    m_TSTACK_h[m_top] = h;
    m_TSTACK_a[m_top] = a;
    m_TSTACK_b[m_top] = b;
}

void Triconnected_graph::TSTACK_pushEOS() {
    m_top += 1;
    m_TSTACK_a[m_top] = -1;
}

bool Triconnected_graph::TSTACK_notEOS() {
    return m_TSTACK_a[m_top] != -1;
}


CompStruct *Triconnected_graph::__newComp(short compType) {

    m_component.push_back(new CompStruct(compType));
    return m_component.back();
}

void Triconnected_graph::__delHigh(Edge *edge) {

    // ListIterator < int > it = m_IN_HIGH[edge];
    // if (it.valid()) {
    // (i_w, pos) = self.m_IN_HIGH[edge.index]
    // i_v = edge.target
    bool is_found = false;
    for (long i :m_HIGHPT[(*edge).target]) {
        if (i == m_NEWNUM[(*edge).source]) {
            is_found = true;
            break;
        }
    }
    if (is_found) {

        if (verbose) {
            cout << "\t\t\tCall of __delHigh m_HIGHPT ";
            print_m_HIGHPT();
            (*edge).print();
            cout << " m_top= " << m_top << endl;
        }

        m_HIGHPT[(*edge).target].remove(m_NEWNUM[(*edge).source]);
        if (verbose) {
            cout << "\t\t\tAfter of __delHigh m_HIGHPT ";
            print_m_HIGHPT();
            cout << "\t" << m_NEWNUM[(*edge).source] << "\t" << m_NEWNUM[(*edge).target] << "\t" << (*edge).index << endl;
        }
    }
}

void Triconnected_graph::print_m_HIGHPT() {
    cout << "[ ";
    for (long i = 0; i < nb_of_nodes; i++) {
        cout << "[";
        for (list<long>::iterator it = m_HIGHPT[i].begin(); it != m_HIGHPT[i].end(); ++it) {
            cout << *it << ", ";
        }
        cout << "], ";
    }
    cout << "]";
}


Edge *Triconnected_graph::__pathSearch_sub_type_2_1(long iV, long i_w, long &i_x) {

    Edge *e_ab = NULL;  // edge
    if (verbose) {
        cout << endl;
        cout << "\nfound type-2 separation pair [if] " << iV << ", " << (*m_A[i_w].front()).target << endl;
    }

// Storage of separation pair
    long i_v_0 = iV;
    long i_v_1 = (*m_A[i_w].front()).target;
    if (i_v_0 > i_v_1) {
        separation_pair.push_back(pair<long, long>(i_v_1, i_v_0));
    } else {
        separation_pair.push_back(pair<long, long>(i_v_0, i_v_1));
    }


    Edge *e1 = m_estack.back();  //.pop()  // edge popRet()
    m_estack.pop_back();

    Edge *e2 = m_estack.back();//m_estack.pop()  // edge
    m_estack.pop_back();

    m_A[i_w].remove(e2);  // on veut supprimer l'arete e2 de la liste!

    if (verbose) {
        cout << "\tAFTER m_A[i_w].remove(e2) : ";
        print_m_A(i_w);
        cout << endl;
    }
    i_x = (*e2).target;
    if (verbose) {
        cout << "__pathSearch_sub_type_2_1: (" << iV << ", " << i_w << ") i_x " << i_x << endl;
    }
    Edge *e_virt = new Edge(iV, i_x);  // m_pGC->newEdge(v,i_x)
    virt_edges.push_back(e_virt);
    m_degree[i_x] -= 1;
    m_degree[iV] -= 1;

    assert ((*e2).source == i_w);
    CompStruct *component = __newComp(1);  // Polygon
    (*component).add(e1);
    (*component).add(e2);
    (*component).add(e_virt);
    if (verbose) {
        cout << "Creation Polygon " << m_component.size() << ": ";
        (*e1).print();
        cout << ", ";
        (*e2).print();
        cout << ", ";
        (*e_virt).print();
        cout << endl;
    }
    if (!m_estack.empty()) {  // !m_estack.empty():
        e1 = m_estack.back();  // .top()
        if ((*e1).source == i_x && (*e1).target == iV) {
            e_ab = m_estack.back();  //pop()
            m_estack.pop_back();
            m_A[i_x].remove(e_ab);  // TODO: remove n'est pas forcement le mieux car on parcourt j'imagine toute la liste
            if (verbose) {
                cout << "\tAFTER m_A[i_x].remove(e_ab): ";
//                for (list<Edge *>::const_iterator it = m_A[i_x].begin(); it != m_A[i_x].end(); ++it) {
//                    cout << "\t";
//                    (**it).print();
//                    cout << endl;
//                }
//                cout << "]" << endl;
                print_m_A(i_x);
            }
// pour supprimer l'element
// cout<< "__delHigh(e_ab) 1)"
            __delHigh(e_ab);
        }
    }
    return e_virt;
}

void Triconnected_graph::print_verbose_creation_poly_or_triconnected(CompStruct *C) {
    if ((*C).m_type == 1) {
        cout << "Creation Polygon " << m_component.size() << ": ";
    } else {
        cout << "Creation triconnected " << m_component.size() << ": ";
    }

    for (Edge *ed : (*C).m_edges) {
        (*ed).print();
        cout << ", ";
    }
    cout << endl;
}

void Triconnected_graph::print_l_edges(list<Edge *> l_e) {
    cout << "[";
    for (list<Edge *>::const_iterator it = l_e.begin(); it != l_e.end(); ++it) {
        cout << "\t";
        (**it).print();
        cout << ",";
    }
    cout << "]";
}

void Triconnected_graph::print_s_edges(unordered_set<Edge *> s_e) {
    cout << "{";
    for (Edge *e : s_e) {
        (*e).print();
        cout << ", ";
    }
    cout << "}";
}

//void Triconnected_graph::print_map_map(list<Edge*> l_e) {
//    cout << "{";
//
//    for (list<Edge *>::const_iterator it = l_e.begin(); it != l_e.end(); ++it) {
//        cout << "\t";
//        (**it).print();
//        cout << ",";
//    }
//    cout << "]";
//}

void Triconnected_graph::print_m_A(long i_v) {
    cout << "[";
    for (list<Edge *>::const_iterator it = m_A[i_v].begin(); it != m_A[i_v].end(); ++it) {
        cout << "";
        (**it).print();
        cout << ",";
    }
    cout << "]";
}

long Triconnected_graph::__high(long iV) {

    if (!m_HIGHPT[iV].empty()) {
        return m_HIGHPT[iV].front();  // .front()
    } else {
        return 0;
    }
}


void Triconnected_graph::__assembleTriconnectedComponents() {

    /**
     * joins bonds and polygons with common virtual edge in
     * order to build the triconnected components.
     */
    // GraphCopySimple & GC = *m_pGC;
    unordered_map<long, unordered_map<long, long> > comp1;
    unordered_map<long, unordered_map<long, long> > comp2;
    // EdgeArray < int > comp1(GC), comp2(GC);
    // EdgeArray < ListIterator < edge > > item1(GC, ListIterator < edge > ());
    // EdgeArray < ListIterator < edge > > item2(GC);
    unordered_map<long, unordered_map<long, Edge * >> item1;
    unordered_map<long, unordered_map<long, Edge * >> item2;

    // bool * visited = new bool[m_numComp];
    vector<bool> visited(m_component.size(), 0);// = np.zeros((len(self.m_component),), dtype=bool)
    // int i;
    // for (i = 0; i < m_numComp; i++) {
    //     visited[i] = false;

    // List < edge > & L = m_component[i].m_edges;

//    for iComp, comp in enumerate(m_component):
    for (int iComp = 0; iComp < m_component.size(); iComp++) {
        CompStruct *comp = m_component[iComp];


        if (verbose) {
            cout << "iComp " << iComp << "\tcomp " << (*comp).m_type << ":";
            print_l_edges((*comp).m_edges);
            cout << endl;
        }

        // L = comp.m_edges
        // ListIterator < edge > it;
        // for (it = L.begin(); it.valid(); ++it) :
        for (Edge *edge:(*comp).m_edges) {

            // if not valid? .valid() Returns true iff the iterator points to an element. On mettra dans item1 la
            // premiere fois. La deuxieme, on ira dans else:
            if (comp1.count((*edge).source) == 0) {

                comp1[(*edge).source] = unordered_map<long, long>();
                comp1[(*edge).source][(*edge).target] = iComp;

                item1[(*edge).source] = unordered_map<long, Edge *>();
                item1[(*edge).source][(*edge).target] = edge;
            } else {
                // edge.source in comp1:

                if (comp1[(*edge).source].count((*edge).target) == 0) {
                    comp1[(*edge).source][(*edge).target] = iComp;
                    item1[(*edge).source][(*edge).target] = edge;

                } else {  // les doublons?
                    // not in comp2
                    if (comp2.count((*edge).source) == 0) {

                        comp2[(*edge).source] = unordered_map<long, long>();
                        comp2[(*edge).source][(*edge).target] = iComp;

                        item2[(*edge).source] = unordered_map<long, Edge *>();
                        item2[(*edge).source][(*edge).target] = edge;
                    } else {
                        comp2[(*edge).source][(*edge).target] = iComp;
                        item2[(*edge).source][(*edge).target] = edge;
                    }
                }
            }

        }
    }
    if (verbose) {
        cout << "TODO outputs" << endl;
//        cout<<"\ncomp1", comp1
//        cout<<"item1", item1)
//        cout<<"comp2", comp2)
//        cout<<"item2", item2)
    }

    for (int iComp = 0; iComp < m_component.size(); iComp++) {

        //        for iComp, comp in enumerate(self.m_component):
        CompStruct *comp = m_component[iComp];
        // for (i = 0; i < m_numComp; i++) {

        if (verbose) {
            cout << "\n\tICOMP" << iComp;
        }
        // for edge in comp.m_edges:
        // CompStruct & C1 = m_component[i];
        // List < edge > & L1 = C1.m_edges;
        visited[iComp] = true;
        // cout<< "visited", visited
        if ((*comp).m_edges.size() == 0) {
            continue;  // si L1 est vide on passe
        }
        if ((*comp).m_type == 1 or (*comp).m_type == 0) {


            // ListIterator < edge > it, itNext;
            // for (it = L1.begin(); it.valid(); it = itNext) {
            //     itNext = it.succ();
            //     edge e  = * it;

            unordered_set<Edge *> setOfEdgesToAdd;
            unordered_set<Edge *> setOfEdgesToDelete;
            bool isFirst = true;
            vector<long> componentsToDelete;
            while ((!setOfEdgesToAdd.empty()) || isFirst) {

                if (verbose) {
                    cout << "\n\n\t\tisFirst " << isFirst << " setOfEdgesToAdd ";
                    print_s_edges(setOfEdgesToAdd);
                    cout << endl;
                }
                list<Edge *> L1;
                if (isFirst) {
                    L1 = list<Edge *>((*comp).m_edges);
                    isFirst = false;
                } else {
//                    L1 = list<Edge *>(setOfEdgesToAdd);
                    for (Edge *tmp_e: setOfEdgesToAdd) {
                        L1.push_back(tmp_e);
                    }
                }

                if (verbose) {
                    cout << "\t\t\tL1";
                    print_l_edges(L1);
                    cout << endl;
                }
                setOfEdgesToAdd = {};
                setOfEdgesToDelete = {};

                for (Edge *edge : L1) {

                    if (verbose) {
                        cout << "\t\t\tedge ";
                        (*edge).print_endl();
                    }
                    if (((*edge).index != -1) || (setOfEdgesToDelete.count(edge))) {
                        continue;  // on ne  travaille ici que sur les aretes virtuelles
                    }
                    // if (GC.original(e) != nullptr)
                    //     continue;
                    // cout<< "edge", edge
                    long jComp = comp1[(*edge).source][(*edge).target];
                    if (verbose) {
                        cout << "\t\t\tBEFORE jComp " << jComp << " " << visited[jComp] << endl;
                    }
                    Edge *it2;
                    // ListIterator < edge > it2;
                    if (visited[jComp]) {

                        jComp = comp2[(*edge).source][(*edge).target];
                        if (visited[jComp]) {
                            continue;
                        }
                        it2 = item2[(*edge).source][(*edge).target];
                    } else {
                        it2 = item1[(*edge).source][(*edge).target];
                    }

                    if (verbose) {
                        cout << "\t\t\tAFTER  jComp" << jComp << " " << visited[jComp] << endl;
                    }

                    CompStruct *C2 = m_component[jComp];
                    if ((*C2).m_type != (*comp).m_type) {
                        continue;
                    }

                    visited[jComp] = true;

                    if (verbose) {
                        cout << "\t\t\t" << jComp << " C2.m_edges ";
                        print_l_edges((*C2).m_edges);
                        cout << " it2 ";
                        (*it2).print_endl();
                    }
                    (*C2).m_edges.remove(it2);  // on efface le doublon (arete virtuelle) de L2

                    if (verbose) {
                        cout << "\t\t\t" << jComp << " C2.m_edges ";
                        print_l_edges((*C2).m_edges);
                    }

                    // (*comp).m_edges.extend(C2.m_edges)
                    for (Edge *edge : (*C2).m_edges) {
                        (*comp).m_edges.push_back(edge);
                    }

                    setOfEdgesToDelete.insert(edge);

                    for (Edge *e2 : (*C2).m_edges) {
                        setOfEdgesToAdd.insert(e2);
                    }

                    // C2.m_edges = []
                    componentsToDelete.push_back(jComp);
                }
                if (verbose) {
                    cout << "\t\t\t(*comp).m_edges init ";
                    print_l_edges((*comp).m_edges);
                    cout << endl;
                    cout << "\t\t\tsetOfEdgesToDelete ";
                    print_s_edges(setOfEdgesToDelete);
                    cout << endl;
                }

                for (Edge *e : setOfEdgesToDelete) {
                    (*comp).m_edges.remove(e);
//                    if (Edgee :(*comp).m_edges){ //TODO:strange: we remove the same edge from the same set???
//                        (*comp).m_edges.remove(e);
//                    }
                }

                if (verbose) {
                    cout << "\t\t\t(*comp).m_edges after remove ";
                    print_l_edges((*comp).m_edges);
                    cout << endl;
                    cout << "\t\t\tsetOfEdgesToAdd ";
                    print_s_edges(setOfEdgesToAdd);
                    cout << endl;
                }
                for (Edge *e : setOfEdgesToAdd) {
                    bool is_found = false;
                    for (Edge *tmp_e :(*comp).m_edges) {
                        if ((*tmp_e) == (*e)) {
                            is_found = true;
                            break;
                        }
                    }
                    if (!is_found) {
                        (*comp).m_edges.push_back(e);
                    }
                }
                if (verbose) {
                    cout << "\t\t\t(*comp).m_edges after remove and add ";
                    print_l_edges((*comp).m_edges);
                    cout << endl;
                }
                // L1.del (it); // on efface le doublon (arete virtuelle) de L2

                // GC.delEdge(e);
            }
            for (long iC : componentsToDelete) {
                (*m_component[iC]).m_edges = {};
            }
        }
    }
}

bool Triconnected_graph::is_found_v_edges(Edge *e, vector<Edge *> v_e) {
    for (Edge *tmp_e :v_e) {
        if ((*tmp_e) == (*e)) {
            return true;
        }
    }
    return false;
}