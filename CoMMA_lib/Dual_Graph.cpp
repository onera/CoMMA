//
// Created by Nicolas Lantos on 31/03/2021.
//
#include "Dual_Graph.h"

// Constructor : initialization of variables by :. The variables are recorded in member functions

Dual_Graph::Dual_Graph(long nb_c,
                       const vector<long> &m_crs_row_ptr,
                       const vector<long> &m_crs_col_ind,
                       const vector<double> &m_crs_values, 
                       const vector<double> &volumes,
                       const Seeds_Pool &seeds_pool,
                       const unordered_set<long> &s_anisotropic_compliant_fc,
                       int verbose,
                       int dim) : _m_CRS_Row_Ptr(m_crs_row_ptr), _m_CRS_Col_Ind(m_crs_col_ind), _m_CRS_Values(m_crs_values), _volumes(volumes),_seeds_pool(seeds_pool),_number_of_cells(nb_c),_verbose(verbose),_dimension(dim){
    // We check that effectively in the dictionary we have recorded cells with boundaries and we define the seed pool depending on the dimension of the problem. (2D or 3D)
        if (s_anisotropic_compliant_fc.size() > 0) {
        _s_anisotropic_compliant_cells = s_anisotropic_compliant_fc;
    } else {
        // Default initialization of this->s_anisotropic_compliant_cells
        for (int i = 0; i < _number_of_cells; i++) {
            _s_anisotropic_compliant_cells.insert(i);
        }
    }
}

vector<long> Dual_Graph::get_neighbours(const long &i_c) const {
    // given the index of a cell refurn the neighborhoods of this cell
    long ind = this->_m_CRS_Row_Ptr[i_c];
    long ind_p_one = this->_m_CRS_Row_Ptr[i_c + 1];
    // insert the values of the CRS_vaue from begin+ind (pointed to the face) till the next pointed one, so related to all the 
    // connected areai (and hence to the faces)
    vector<long> result(this->_m_CRS_Col_Ind.begin() + ind, this->_m_CRS_Col_Ind.begin() + ind_p_one);
    return result;

}

vector<double> Dual_Graph::get_weights(const long &i_c) const {
    // Given the index of a cell, return the value of the faces connected
    long ind = this->_m_CRS_Row_Ptr[i_c];
    long ind_p_one = this->_m_CRS_Row_Ptr[i_c + 1];
    // insert the values of the CRS_vaue from begin+ind (pointed to the face) till the next pointed one, so related to all the 
    // connected areai (and hence to the faces)
    vector<double> result(this->_m_CRS_Values.begin() + ind, this->_m_CRS_Values.begin() + ind_p_one);
    return result;

}

bool Dual_Graph::check_connectivity(unordered_set<long> s_fc,
                                    int verbose) {
//Checks connectivity of the coarse cell
    unordered_map<long, bool> map_isAlreadyConnected;
    int size = s_fc.size();
    if (size <= 1) {
        return true;
    }
    for (long iFC :s_fc) {
        map_isAlreadyConnected[iFC] = false;
    }
    long front = *s_fc.begin();
    map_isAlreadyConnected[front] = true;

    unordered_set<long> setNext({front});
    int nbConnectedCells = 1;

    while (nbConnectedCells < size) {

        if (!setNext.empty()) {
            long iFineCell = *setNext.begin();
            setNext.erase(setNext.begin());
            long ind = this->_m_CRS_Row_Ptr[iFineCell];  // Usefull to find neighbours of seed
            long ind_p_one = this->_m_CRS_Row_Ptr[iFineCell + 1]; // Usefull to find neighbours of seed
            for (long iFC = ind; iFC < ind_p_one; iFC++) {
                long iFCellNeighbour = this->_m_CRS_Col_Ind[iFC];
                if (map_isAlreadyConnected.count(iFCellNeighbour) == 1) {
                    if ((iFCellNeighbour != iFineCell) && (!map_isAlreadyConnected[iFCellNeighbour])) {
                        setNext.insert(iFCellNeighbour);
                        nbConnectedCells += 1;
                        map_isAlreadyConnected[iFCellNeighbour] = true;
                    }
                }
            }
        } else {
            break;
        }
    }
    if ((verbose) && (nbConnectedCells != size)) {
        cout << "s_fc " << endl;
        int iCount = 0;
        for (auto i : s_fc) {
            cout << i << " : " << map_isAlreadyConnected[i] << endl;
            iCount++;
        }

        cout << "nbConnectedCells " << nbConnectedCells << " size " << size << endl;
    }

    return nbConnectedCells == size;

}

unsigned short Dual_Graph::get_nb_of_neighbours(long i_c) {
// Return the number of neightbohurs of the ith cell
    	return this->_m_CRS_Row_Ptr[i_c + 1] - this->_m_CRS_Row_Ptr[i_c];
}


// Return the dictionary of anisotropic faces. The values are the ratio of the area by definition
// while the keys are the global index of the faces
void Dual_Graph::compute_d_anisotropic_fc(vector<double> &maxArray,unordered_map<long, double> &d_anisotropic_fc, unordered_map<long, double> &d_isotropic_fc) {
    // values are the ratio Max to average (ratioArray[iCell]) and keys
    // the (global) index of the cell. d_anisotropic_fc[ifc]= max_weight/min_weight
    double min_weight, max_weight, averageWeight, weight;

    // Process of every compliant fine cells (it is a member variable, so it is not passed to the function):
    for (const long i_fc : _s_anisotropic_compliant_cells) {
        min_weight = numeric_limits<double>::max();
        max_weight = 0.0;
        averageWeight = 0.0;

        // computation of min_weight, max_weight and averageWeight for the current cell i_loc_fc
        // Process of every faces/Neighboursi and compute for the current cell the neighborhood and the
	// area associated with the neighborhood cells
        vector<long> v_neighbours = get_neighbours(i_fc);
        vector<double> v_weights = get_weights(i_fc);

        assert(v_neighbours.size() == v_weights.size());
        int nb_neighbours = v_neighbours.size();

	for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {
  
            long i_fc_n = v_neighbours[i_n]; 
            double i_w_fc_n = v_weights[i_n];
            if (i_fc_n != i_fc) {  // to avoid special case where the boundary value are stored
                if (max_weight < i_w_fc_n) {
                    max_weight = i_w_fc_n;
                }
                if (min_weight > i_w_fc_n) {
                    min_weight = i_w_fc_n;
                }
            } else {
                nb_neighbours--;
            }

            averageWeight += i_w_fc_n / (double) (nb_neighbours);
        }
        maxArray[i_fc] = max_weight;

        // Anisotropy criteria for the line Admissibility
        if (max_weight / min_weight >= 4.0) {
        // If the ratio is more than 4 of the biggest zith the smallest cell , add it to the dictionary
        // where I store the ratio between the max and the average
	    d_anisotropic_fc[i_fc] = max_weight / averageWeight;
        } else {
            d_isotropic_fc[i_fc] = max_weight / averageWeight;
        }
    }
}


forward_list<deque<long> *> Dual_Graph::compute_anisotropic_line(long &nb_agglomeration_lines) {
    /**
     * The goal of this function is :
     * - firstly to look for anisotropic cells through the use of d_anisotropic_fc
     * - secondly build anisotropic lines
     * Rmk: costly function: sort of a dictionary!
     */

    long nb_fc = _number_of_cells; // Number of cells is a member variable initialized through nb_fc in the 
    // it is computed in d_anisotropic_fc as rhe max_weight
    vector<double> maxArray(nb_fc, 0.0);
    unordered_map<long, double> d_anisotropic_fc;
    unordered_map<long, double> d_isotropic_fc; 
    // Map to address if the cell has been added to a line
    unordered_map<long, bool> has_been_treated;
    // Computation of the anisotropic cell , alias of the cells for which the
    // ration between the face with maximum area and the face with minimum area
    // is more than 4.
    compute_d_anisotropic_fc(maxArray,d_anisotropic_fc,d_isotropic_fc);
    // Initialization of the map: for each anisotropic cell
    // we check if has been analyzed or not 
    for (auto& it: d_anisotropic_fc){
        has_been_treated[it.first] = false;
    }
    // seed to be considered to add or not a new cell to the line
    long seed = 0;
    // to determine end of line
    bool end = false;
    // to determine if we arrived at the end of an extreme of a line
    bool opposite_direction_check = false;
    // size of the line
    int lines_size = 0;
    // vector of the candidates to continue the line
    vector<long> candidates;
    // forward list containing the lines
    forward_list<deque<long> *> lines;
    // vector of neighbours to the seed
    vector<long> v_neighbours;
    // vector of weight (face area) that connects to the seed neighborhood
    vector<double> v_w_neighbours;
    // we cycle on all the anisotropic cells identified before
    for (auto& it: d_anisotropic_fc){
        // seed from where we start the deck
        if (has_been_treated[it.first]==true){
           // If the cell has been already treated, continue to the next anisotropic
           // cell in the unordered map
           continue;
        } 
        // we save the primal seed for the opposite direction check that will happen later
        auto primal_seed = it.first;
        // we initialize the seed at the beginning of each line
        seed = primal_seed; 
        // Create the new line
        deque<long> *dQue;
        dQue = new deque<long>();
        // we add the first seed
        (*dQue).push_back(seed);
        has_been_treated[seed]=true;
        // Start the check from the seed
        // while the line is not enced
        while (end!=true){
            // for the seed (that is updated each time end!= true) we fill the neighbours
            // and the weights
            v_neighbours = get_neighbours(seed);
            v_w_neighbours = get_weights(seed);
            for(auto i = 0; i < v_neighbours.size(); i++){
               // we check if in the neighbours there is the seed (it should not happen,
               // but we prevent like this mistakes)
               if (v_neighbours[i]==seed){continue;}
               // we check if it is in the dictionary of anisotropic cells
               // the boundary and if it is along the maximum interface
               if(d_anisotropic_fc.count(v_neighbours[i]) != 0 and v_w_neighbours[i] > 0.75 * maxArray[seed]){
               // it it is in the dictionary of anisotropic we check if the neigh has been already treated
                 if(has_been_treated[v_neighbours[i]]==false){
               // if has not been treated we add it to the vectors of candidates to continue the line
                   candidates.push_back(v_neighbours[i]);
                 }                
               }               
             } // end for loop
             // case we have only 1 candidate to continue the line
             if (candidates.size() == 1){
                // we can add to the actual deque
                if (opposite_direction_check == false){
                   (*dQue).push_back(candidates[0]);
                } else {(*dQue).push_front(candidates[0]);}
                // update the seed to the actual candidate
                seed = candidates[0];
                // the candidate (new seed) has been treated
                has_been_treated[seed]=true;
             }
             // case we have more than one candidate
             else if (candidates.size()>1){
                // we cycle on candidates
                for (auto & element : candidates) {
                        // if has been treated ==> we check the next candidate
                        if(has_been_treated[element]==true){
                          continue;
                        }
                        else{
                        // if has not been treated, the opposite direction flag
                        // is not active? ==> push back
                          if (opposite_direction_check == false){
                           (*dQue).push_back(element);
                          }
                          else { // if it is active push front
                           (*dQue).push_front(element);
                          }
                          // we update the seed and the has been treated
                          seed = element;
                          has_been_treated[element]=true;
                        }
                    }                      
             }// end elseif
             // 0 candidate, we are at the end of the line or at the end
             // of one direction
             else if (candidates.size() == 0){
                  if (opposite_direction_check==true){
                      end = true;
                  }
                  else{
                  seed = primal_seed;
                  opposite_direction_check = true;}
             }
            // we clear the cansidates and the neighbours value for the 
            // next seed
            candidates.clear(); 
            v_w_neighbours.clear();
            v_neighbours.clear(); 
       }
     // we initialize the flags
     end = false;
     opposite_direction_check = false;
     // we push the deque to the list if are bigger than 1
     if ((*dQue).size()>1){
       lines.push_front(dQue);
       lines_size++;
     }
  }
  nb_agglomeration_lines = (long) lines_size; 
  return lines;
}

unsigned short int Dual_Graph::compute_degree_of_node_in_subgraph(int i_fc, unordered_set<long> s_of_fc) {

    unsigned short int deg(0);
    for (const long &i_fc_n : get_neighbours(i_fc)) {
        // the count method computes the number of occurence of the neighborhood and
        // hence it guarantees that is in the subset
        if (i_fc_n != i_fc && s_of_fc.count(i_fc_n) > 0) {
            deg++;
        }
    }
    return deg;
}


long Dual_Graph::_compute_subgraph_root(unordered_set<long> s_fc) {
    long max_number_common_faces(-1), arg_max_number_common_faces(-1);
    for (auto i_fc : s_fc) {
        // Computation of the degree of the node in the CC subgraph.
        unsigned short int nb_common_faces_i_fc = compute_degree_of_node_in_subgraph(i_fc, s_fc);

        if (nb_common_faces_i_fc > max_number_common_faces) {
            max_number_common_faces = nb_common_faces_i_fc;
            arg_max_number_common_faces = i_fc;
        }
    }
    return arg_max_number_common_faces;
}


void Dual_Graph::clean_d_neighbours_of_seed(unordered_set<long> s_fc,
                                            unordered_map<long, unsigned short> &d_neighbours_of_seed) {

    // Cleaning of d_neighbours_of_seed:
    //   we remove fc which belongs to the selected cc: s_fc_for_cc
    for (const long i_fc : s_fc) {
        if (d_neighbours_of_seed.count(i_fc) > 0) {
            d_neighbours_of_seed.erase(i_fc);
        }
    }

    // To do improve this if possible?
    //   we remove from d_neighbours_of_seed fc that are not in a first order neighbouhood of the selected set of fc

    vector<long> l_neighbours_of_seed;
    for (auto dnos_k_v : d_neighbours_of_seed) {
        l_neighbours_of_seed.push_back(dnos_k_v.first);
    }

    for (long i_fc : l_neighbours_of_seed) {
        vector<long> neighbours = get_neighbours(i_fc);

        bool is_intersection = false;
        for (long i : neighbours) {
            if (s_fc.count(i) > 0) {
                is_intersection = true;
            }
        }

        if (!is_intersection) {
            d_neighbours_of_seed.erase(i_fc);
        }
    }
}


unsigned short int Dual_Graph::compute_min_fc_compactness_inside_a_cc(unordered_set<long> &s_fc) {
    // Compute Compactness of a cc
    // Be careful: connectivity is assumed
    if (s_fc.size() > 1) {
        unordered_map<long, unsigned short int> dict_fc_compactness = compute_fc_compactness_inside_a_cc(s_fc);
        if (dict_fc_compactness.empty()) {
            return 0;
        }
        unsigned short int min_comp = USHRT_MAX;
        for (auto &i_k_v :dict_fc_compactness) {
            if (i_k_v.second < min_comp) {
                min_comp = i_k_v.second;
            }
        }
        return min_comp;
    } else {
        return 0;
    }
}

unordered_map<long, unsigned short int> Dual_Graph::compute_fc_compactness_inside_a_cc(unordered_set<long> &s_fc) {
    unordered_map<long, unsigned short int> dict_fc_compactness;
    if (s_fc.size() > 1) {

        // for every fc constituting a cc
        for (const long &i_fc : s_fc) {

            vector<long> v_neighbours = get_neighbours(i_fc);
            for (const long &i_fc_n : v_neighbours) {
                if ((s_fc.count(i_fc_n) > 0) && (i_fc != i_fc_n)) {
                    if (dict_fc_compactness.count(i_fc) > 0) {
                        dict_fc_compactness[i_fc]++;
                    } else {
                        dict_fc_compactness[i_fc] = 1;
                    }
                }
            }
            if (dict_fc_compactness.count(i_fc) == 0) {
                dict_fc_compactness[i_fc] = 0;
            }
        }
    }
    return dict_fc_compactness;
}

vector<unordered_set<long>> Dual_Graph::compute_connected_components(const unordered_set<long> &s_initial_fc) {


/**
 * With a set of non connected fine cells (s_initial_cc), we compute connected components of the graph.
 * :param s_initial_cc: set of fine cells composing the initial coarse cell (not connected).
 * :return: a set of connected component (stored as frozenset)
 */

    vector<unordered_set<long>> v_of_connected_set;
    vector<long> v_initial_fc(s_initial_fc.size());
    int i = 0;
    for (const long &i_fc: s_initial_fc) {
        v_initial_fc[i++] = i_fc;
    }
    unsigned short int size_cc = s_initial_fc.size();
    if (size_cc <= 1) {
        unordered_set<long> s = {v_initial_fc[0]};
        v_of_connected_set.push_back(s);
        return v_of_connected_set;
    }
    vector<bool> is_already_connected(size_cc, false);
    is_already_connected[0] = true;

    unordered_set<long> s_next({v_initial_fc[0]});
    unsigned short int nb_connected_fc = 1;
    unordered_map<long, unsigned short int> dict_global_to_local;
    for (unsigned short int i_fc_loc = 0; i_fc_loc < size_cc; i_fc_loc++) {
        dict_global_to_local[v_initial_fc[i_fc_loc]] = i_fc_loc;
    }

    unordered_set<long> s_fc({v_initial_fc[0]});
    while (nb_connected_fc < size_cc) {
        if (!s_next.empty()) {

            const long i_fc = *s_next.begin(); // equiv. i_fc = s_next.pop()
            s_next.erase(s_next.begin()); // equiv. i_fc = s_next.pop()
            for (const long &i_fc_n : get_neighbours(i_fc)) {
                if (dict_global_to_local.count(i_fc_n) > 0) {
                    if ((i_fc_n != i_fc) && (!is_already_connected[dict_global_to_local[i_fc_n]])) {
                        s_next.insert(i_fc_n);
                        nb_connected_fc++;
                        is_already_connected[dict_global_to_local[i_fc_n]] = true;
                        s_fc.insert(i_fc_n);
                    }
                }
            }
        } else {
            // End of a connected component
            v_of_connected_set.push_back(unordered_set<long>(s_fc));
            short int arg_new_seed = -1;  // This is the new seed of the new connected component
            for (unsigned short int i = 0; i < size_cc; i++) {
                if (!is_already_connected[i]) {
                    arg_new_seed = i;
                    is_already_connected[i] = true;
                    nb_connected_fc++;
                    break;
                }
            }

            // Creation of a new connected component
            s_fc = {v_initial_fc[arg_new_seed]};
            s_next = {v_initial_fc[arg_new_seed]};
        }
    }


// We add the last connected component
    v_of_connected_set.push_back(s_fc);

    return v_of_connected_set;

}



unsigned short int Dual_Graph::compute_degree_of_node(int i_fc, bool (*test_function)(int)) {

    if (test_function == nullptr) {
        unsigned short int deg(0);
        for (auto i_fc_n : get_neighbours(i_fc))
            if (i_fc_n != i_fc)
                deg += 1;
        return deg;
    } else {
        unsigned short int deg(0);
        for (auto i_fc_n : get_neighbours(i_fc))
            if (i_fc_n != i_fc && (*test_function)(i_fc_n))
                deg += 1;
        return deg;
    }
}

short Dual_Graph::compute_degree_of_node_not_a(const long &i_fc, vector<bool> a) {
    /**
     * we compute the degree of nodes such that a[i_fc_n] is false
     */
    unsigned short int deg(0);
    for (auto i_fc_n : get_neighbours(i_fc))
        if (i_fc_n != i_fc && !a[i_fc_n])
            deg += 1;
    return deg;

}


void Dual_Graph::compute_local_crs_subgraph_from_global_crs(unordered_set<long> set_of_node,
                                                            vector<long> &row_ptr_l,
                                                            vector<long> &col_ind_l,
                                                            vector<double> &values_l,
                                                            vector<long> &g_to_l,
                                                            vector<long> &l_to_g) const {
// Given a set of nodes, it compute the csr subgraph starting from the global. We need to consider the 
// vector passing from global to local and from local to global.
    unsigned short int number_Of_Nodes_L = set_of_node.size();
    unsigned short int number_of_nodes_g = _number_of_cells;
    assert(_number_of_cells == _m_CRS_Row_Ptr.size() - 1);

    vector<unordered_map<long, double>> adj_Matrix(number_of_nodes_g);

    int count(0), i_c(0);

    g_to_l.resize(number_of_nodes_g);
    l_to_g.resize(number_Of_Nodes_L);
    for (int i = 0; i < number_of_nodes_g; i++)
        g_to_l[i] = -1;

    for (auto iV : set_of_node) {
        g_to_l[iV] = i_c;
        l_to_g[i_c] = iV;
        i_c++;
    }

    for (auto iV : set_of_node) {
        for (int i = _m_CRS_Row_Ptr[iV]; i < _m_CRS_Row_Ptr[iV + 1]; i++) {
            long i_w = _m_CRS_Col_Ind[i];
            if (set_of_node.count(i_w) > 0) {
                adj_Matrix[g_to_l[iV]][g_to_l[i_w]] = _m_CRS_Values[i];
                count++;
            }
        }
    }

    row_ptr_l.resize(number_Of_Nodes_L + 1);
    for (int i = 0; i < number_Of_Nodes_L + 1; i++)
        row_ptr_l[i] = 1;

    col_ind_l.resize(count);
    values_l.resize(count);
    for (int i = 0; i < count; i++) {
        col_ind_l[i] = 0;
        values_l[i] = 0.0;
    }

    row_ptr_l[0] = 0;
    long index_col_and_values = 0;

    for (int i = 0; i < number_Of_Nodes_L; i++) {
        vector<long> sortedList_i;
        for (auto adj_M_i_k_v : adj_Matrix[i])
            sortedList_i.push_back(adj_M_i_k_v.first);
        for (auto j : sortedList_i) {
            values_l[index_col_and_values] = adj_Matrix[i][j];
            col_ind_l[index_col_and_values] = j;

            index_col_and_values++;
            row_ptr_l[i + 1] = index_col_and_values;
        }
    }
}


// To chose if the metric of cardinality is pertinent
void Dual_Graph::compute_neighbourhood_of_cc(const unordered_set<long> s_seeds,
                                             short &nb_of_order_of_neighbourhood,
                                             unordered_map<long, short> &d_n_of_seed, // defined also as d_of_neighborhood, output
                                             const short max_card,
                                             vector<bool> &is_fc_agglomerated_tmp,
                                             unordered_set<long> s_of_constrained_fc) {
    // Basic checks
    assert(max_card != -1);
    // TODO : If the fine cells foud are not sufficient to build a course cell of given cardinality, maybe define a wile to reach this 
    // goal and go the furthest possible.    
    // This function computes the neighbourhood of a seed passed as argument.
    // It looks in the neighbourhood of order at least nb_of_order_of_neighbourhood, but if the size of the set of neighbour
    // is too small (<max_card), we look in higher order neighbourhood.
    //
//    unordered_map<long, int> d_n_of_seed;  // dict of fc with the order of neighbouring from seed
    unordered_map<long, int> d_n_of_order_o_m_one;  // dict of FC with the order of neighbouring from seed
    // we initialize for seeds where order is 0
    for (const long &i_fc : s_seeds) {
        d_n_of_order_o_m_one[i_fc] = 0;
    }


    int i_order = 1;

    while ((i_order < nb_of_order_of_neighbourhood + 1) ||
           (d_n_of_seed.size() + d_n_of_order_o_m_one.size()) < max_card) {

        unordered_map<long, int> d_n_of_order_o;

        for (auto id_M_one:d_n_of_order_o_m_one) {
            d_n_of_seed[id_M_one.first] = id_M_one.second;
        }

        for (const auto &i_k_v : d_n_of_order_o_m_one) {

            long seed_tmp = i_k_v.first;
            for (const long &i_fc_n : get_neighbours(seed_tmp)) {

                if ((d_n_of_seed.count(i_fc_n) == 0) &&
                    ((!is_fc_agglomerated_tmp[i_fc_n] || !(s_of_constrained_fc).empty()))) {
                    if (d_n_of_order_o.count(i_fc_n) == 0) {
                        if (!(s_of_constrained_fc).empty()) {
                            if ((s_of_constrained_fc).count(i_fc_n)) {
                                if (d_n_of_order_o_m_one.count(i_fc_n)) {
                                    if (i_order < d_n_of_order_o_m_one[i_fc_n]) {
                                        d_n_of_order_o[i_fc_n] = i_order;
                                    }
                                } else {
                                    d_n_of_order_o[i_fc_n] = i_order;
                                }
                            }
                        } else {
                            // a fc can be access via multiple ways. We look for the quickest
                            if (d_n_of_order_o_m_one.count(i_fc_n)) {
                                if (i_order < d_n_of_order_o_m_one[i_fc_n]) {
                                    d_n_of_order_o[i_fc_n] = i_order;
                                }
                            } else {
                                d_n_of_order_o[i_fc_n] = i_order;
                            }
                        }
                    }
                }
            }
        }

        // Exit condition
        if (d_n_of_order_o.empty()) {
            // No more neighbours available:
            break;
        }

        // Copy: d_n_of_order_o_m_one = d_n_of_order_o;
        d_n_of_order_o_m_one.clear();
        for (auto id:d_n_of_order_o) {
            d_n_of_order_o_m_one[id.first] = id.second;
        }
        i_order++;
    }
    // Update of d_n_of_seed
    //d_n_of_seed.update(d_n_of_order_o_m_one)
    for (auto id_M_one:d_n_of_order_o_m_one) {
        d_n_of_seed[id_M_one.first] = id_M_one.second;
    }

    // We remove the seed from the neighbours of seed
    for (const long &i_fc:s_seeds) {
        d_n_of_seed.erase(i_fc);
    }

    nb_of_order_of_neighbourhood = i_order;
//    return d_n_of_seed;
}
