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

        // Ignore nodes that were already visited.
        if (number_dfs[node_v] != -1) {
            continue;
        }

        revS.push_back(node_v);

        // Set correct discovery time for node_v
        number_dfs[node_v] = num_count;
        num_count += 1;

        // For all adjacent nodes w of v:
        for (long i_neighbour = row_ptr[node_v]; i_neighbour < row_ptr[node_v + 1]; i_neighbour++) {
            long node_w = col_ind[i_neighbour];
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


short Triconnected_graph::computeTriconnectivity(short partialTest) {

    // m_pGC = new GraphCopySimple(G);
    // GraphCopySimple & GC = *m_pGC;
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
        CompStruct component = CompStruct();

        // Process of edges
        for (Edge *edge: edges) {
            component.add(edge);
        }

        component.m_type = component.ComponentType2Int["bond"];
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
            }
        }
    }
    if (partialTest == 2) {
        return 2;
    }


    __DFS2();
//
//    if self.verbose:
//    print("\nnode\tNEWNUM\tLOWPT1\tLOWPT2\tHIGHPT")
//    for
//    i_v
//            in
//    range(self.number_of_nodes):
//    print(i_v, ":  \t", self.m_NEWNUM[i_v], "   \t", end = "")
//    print(self.m_low_pt_1[i_v], "   \t", self.m_low_pt_2[i_v], "   \t", end = "")
//    for
//    i
//            in
//    self.m_HIGHPT[i_v]:
//    print(i, " ", end = "")
//    print("")
//
//    print("\nedges starting a path:")
//    for
//    i_e
//            in
//    range(self.number_of_edges):
//    if self.edges[i_e].start:
//    // if self.m_START[i_e]:
//    print(self.edges[i_e], end = "")
//
    if (partialTest == 3) {
        return 3;
    }

//
//    if self.verbose:
//    print("")
//
//    self.m_TSTACK_h = np.zeros((2 * self.number_of_edges + 1,), dtype = int)
//    self.m_TSTACK_a = np.zeros((2 * self.number_of_edges + 1,), dtype = int)
//    self.m_TSTACK_b = np.zeros((2 * self.number_of_edges + 1,), dtype = int)
//    self.m_top = 0
//    self.m_TSTACK_a[self.m_top] = -1  // start with EOS
//
//    self.__pathSearch(self.m_start)
//
//    // last split component
//    if len(self.m_estack) > 4:
//    cType = CompStruct.ComponentType2Int["triconnected"]
//    else:
//    cType = CompStruct.ComponentType2Int["polygon"]
//
//    C = self.__newComp(compType = cType)
//    while self.m_estack:
//    C.add(self.m_estack.pop())
//
//    if self.verbose:
//    if C.m_type == 1:
//    print("Creation Polygon", len(self.m_component), ": ", end = "\t")
//    else:
//    print("Creation triconnected", len(self.m_component), ": ", end = "\t")
//    for
//    ed
//            in
//    C.m_edges:
//    print(ed, end = "")
//    print("")
//
//    if self.verbose:
//    self.__printStacks()
//
//    self.m_TSTACK_h = None
//    self.m_TSTACK_a = None
//    self.m_TSTACK_b = None
//
//    self.__assembleTriconnectedComponents()
//
//            // Caution: checkComp() assumes that the graph is simple!
//            assert (self.__check_comp())
//
//    if self.verbose:
//    print("\n\nTriconnected components:\n")
//
//    for
//    iC, component
//    in
//    enumerate(self.m_component):
//
//    if len(component.m_edges) == 0:
//    continue
//
//    print("[", iC, "] ", end = "")
//    print(CompStruct.Int2ComponentType[component.m_type], end = "")
//    for
//    edge
//            in
//    component.m_edges:
//    print(edge, end = "")
//    print("")
//
//    // Compute a simpler way to express triconnectivity:
//    triconnectedComponents = []
//    for
//    iC, component
//    in
//    enumerate(self.m_component):
//
//    if len(component.m_edges) == 0:
//    continue
//
//    if component.m_type == 2:
//
//    vertices = set()
//    degree = dict()
//    for
//    edge
//            in
//    component.m_edges:
//    if edge.index != -1 or edge
//    in
//    self.edges:  // virtual edges or real edge
//    if edge.source
//    in
//    degree:
//
//    degree[edge.source] += 1
//    else:
//    degree[edge.source] = 1
//    if edge.target
//    in
//    degree:
//
//    degree[edge.target] += 1
//    else:
//    degree[edge.target] = 1
//    vertices.add(edge.source)
//    vertices.add(edge.target)
//    // print( "vertices", vertices, degree
//    isVertexRemoved = False
//    for
//    i_v
//            in
//    degree:
//    if degree[i_v] < 3:
//    // print( "i_v", i_v
//    vertices.remove(i_v)
//    isVertexRemoved = True
//    for
//    edge_tmp
//            in
//    component.m_edges:
//    if edge_tmp.index != -1 or edge_tmp
//    in
//    self.edges:  // virtual edges or real edge
//    if i_v == edge_tmp.source or i_v == edge_tmp.target:
//
//    if edge_tmp.source == i_v:
//    opposite = edge_tmp.target
//    else:
//    opposite = edge_tmp.source
//    if opposite
//                in
//        vertices:
//        degree[opposite] -= 1
//
//    // print( "vertices", vertices,  degree
//    while
//        isVertexRemoved:
//        isVertexRemoved = False
//    for
//    i_v
//            in
//    set(vertices):  // copy
//    if degree[i_v] < 3:
//    vertices.remove(i_v)
//    isVertexRemoved = True
//    for
//    edge_tmp
//            in
//    component.m_edges:
//    if edge_tmp.index != -1 or edge_tmp
//    in
//    self.edges:  // virtual edges or real edge
//    if i_v == edge_tmp.source or i_v == edge_tmp.target:
//
//    if edge_tmp.source == i_v:
//    opposite = edge_tmp.target
//    else:
//    opposite = edge_tmp.source
//    if opposite
//                in
//        vertices:
//        degree[opposite] -= 1
//    if (vertices) {
//        triconnectedComponents.append(vertices);
//    }
//    return triconnectedComponents


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