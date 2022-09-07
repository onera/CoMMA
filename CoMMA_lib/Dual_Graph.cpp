//
// Created by Nicolas Lantos on 31/03/2021.
//
#include "Dual_Graph.h"

// Constructor : initialization of variables by :. The variables are recorded in
// member functions
Graph::Graph(const long &nb_c, const vector<long> &m_crs_row_ptr,
             const vector<long> &m_crs_col_ind,
             const vector<double> &m_crs_values, const vector<double> &volumes)
    : _m_CRS_Row_Ptr(m_crs_row_ptr),
      _m_CRS_Col_Ind(m_crs_col_ind),
      _m_CRS_Values(m_crs_values),
      _volumes(volumes),
      _number_of_cells(nb_c) {
  _visited.resize(_number_of_cells);
  std::fill(_visited.begin(), _visited.end(), false);
  //   long start = 0;
  //   DFS(start);
  //   BFS(start);
  //   cout<<"connected?"<<check_connectivity_g()<<endl;
}

vector<long> Graph::get_neighbours(const long &i_c) const {
  // given the index of a cell refurn the neighborhoods of this cell
  long ind = _m_CRS_Row_Ptr[i_c];
  long ind_p_one = _m_CRS_Row_Ptr[i_c + 1];
  // insert the values of the CRS_vaue from begin+ind (pointed to the face) till
  // the next pointed one, so related to all the
  // connected areai (and hence to the faces)
  vector<long> result(_m_CRS_Col_Ind.begin() + ind,
                      _m_CRS_Col_Ind.begin() + ind_p_one);
  return result;
}

vector<double> Graph::get_weights(const long &i_c) const {
  // Given the index of a cell, return the value of the faces connected
  long ind = _m_CRS_Row_Ptr[i_c];
  long ind_p_one = _m_CRS_Row_Ptr[i_c + 1];
  // insert the values of the CRS_value from begin+ind (pointed to the face)
  // till the next pointed one, so related to all the
  // connected areas (and hence to the faces)
  vector<double> result(_m_CRS_Values.begin() + ind,
                        _m_CRS_Values.begin() + ind_p_one);
  return result;
}

// https://www.youtube.com/watch?v=oDqjPvD54Ss
void Graph::BFS(const long &root) {
  queue<long> coda;
  vector<long> v_neigh;
  vector<long> path;
  coda.push(root);
  vector<bool> visited(_number_of_cells, false);
  visited[root] = true;
  vector<long> prev(_number_of_cells, -1);
  while (!coda.empty()) {
    long node = coda.front();
    coda.pop();
    v_neigh = get_neighbours(node);
    for (auto &it : v_neigh) {
      if (!visited[it]) {
        coda.push(it);
        visited[it] = true;
        prev[it] = node;
      }
    }
  }
  // to print the inverse path
  long retro = prev[_number_of_cells - 1];
  while (retro != -1) {
    path.push_back(retro);
    retro = prev[retro];
  }
  reverse(path.begin(), path.end());
  //     for (int i = 0; i < _number_of_cells; i++) {
  //            cout<<"BFS"<<path[i]<<endl;
  //     }
}

void Graph::DFS(const long &i_fc) {
  _visited[i_fc] = true;
  vector<long> v_neigh;
  v_neigh = get_neighbours(i_fc);
  for (auto &it : v_neigh) {
    if (!_visited[it]) {
      DFS(it);
    }
  }
}

bool Graph::check_connectivity() {
  for (int i = 0; i < _number_of_cells; i++) {
    _visited.push_back(false);
  }
  if (_number_of_cells == 1) {
    return (true);
  }
  DFS(_m_CRS_Col_Ind[0]);
  for (int i = 0; i < _number_of_cells; i++) {
    if (_visited[i] == false) {
      return (false);
    }
  }
  return (true);
}

Subgraph::Subgraph(const long &nb_c, const vector<long> &m_crs_row_ptr,
                   const vector<long> &m_crs_col_ind,
                   const vector<double> &m_crs_values,
                   const vector<double> &volumes,
                   const vector<long> &mapping_l_to_g, const bool &is_isotropic)
    : Graph(nb_c, m_crs_row_ptr, m_crs_col_ind, m_crs_values, volumes),
      _mapping_l_to_g(mapping_l_to_g),
      _is_isotropic(is_isotropic) {
  // Definition degree
  long pos_old = 0;
  long degree = 0;
  for (const long &elem : m_crs_row_ptr) {
    degree = elem - pos_old;
    if (degree > _compactness) {
      _compactness = degree;
    }
    pos_old = elem;
  }
  // Definition cardinality
  _cardinality = nb_c;
}

void Subgraph::insert_node(vector<long> &v_neigh, const long &i_fc,
                           const double &volume, const vector<double> &weight) {
  // Use the mapping
  // local vector of neighborhood
  vector<long> v_l_neigh;
  vector<long>::iterator low1;
  // @todo this solution clearly help in the connection of the subnode BUT can
  // bring to instability
  // and errors.
  for (const auto &elem : v_neigh) {
    low1 = find(_mapping_l_to_g.begin(), _mapping_l_to_g.end(), elem);
    if (low1 != _mapping_l_to_g.end()) {
      v_l_neigh.push_back(low1 - _mapping_l_to_g.begin());
    }
  }
  // variable to add weight for each face
  int iter_weight = 0;
  long local_index = _m_CRS_Row_Ptr.size() - 1;
  // initialization pointers for insertion, pointing to the first element of
  // each
  auto pos_col = _m_CRS_Col_Ind.begin();
  auto pos_Values = _m_CRS_Values.begin();
  auto row_end = _m_CRS_Row_Ptr.end() - 1;

  // cycle on the set of neighbours
  for (const auto &elem : v_l_neigh) {
    // insert the node and the weight (we have an iterator for this and remember
    // that at edge is associated one weight)
    // look at here
    // https://stackoverflow.com/questions/71299247/inserting-an-element-in-given-positions-more-than-one-of-a-vector/71299304#71299304
    // to understand why we re-initialize.
    _m_CRS_Col_Ind.insert(_m_CRS_Col_Ind.begin() + _m_CRS_Row_Ptr[elem],
                          local_index);
    _m_CRS_Values.insert(_m_CRS_Values.begin() + _m_CRS_Row_Ptr[elem],
                         weight[iter_weight]);
    // We modify the row pointer as far it is related with what we have done
    // before
    for (auto it = _m_CRS_Row_Ptr.begin() + elem; it != _m_CRS_Row_Ptr.end();
         ++it) {
      ++(*it);
    }
    // we do the same.
    _m_CRS_Col_Ind.insert(_m_CRS_Col_Ind.end(), elem);
    _m_CRS_Values.insert(_m_CRS_Values.end(), weight[iter_weight]);
    // We increment the weight flag iterator
    ++iter_weight;
  }
  _m_CRS_Row_Ptr.push_back(*row_end + v_neigh.size());
  _volumes.push_back(volume);
  _mapping_l_to_g.push_back(i_fc);
}

void Subgraph::remove_node(const long &elemento) {
  // Pass to the local
  auto low = find(_mapping_l_to_g.begin(), _mapping_l_to_g.end(), elemento);
  long i_fc = low - _mapping_l_to_g.begin();
  // initialize starting indices
  long ind;
  long ind_p_one;
  // getting neighbours
  vector<long> v_neigh = get_neighbours(i_fc);

  // weight iterator for erasing in the weight vector
  vector<double>::iterator weight_it;
  auto pos_col = _m_CRS_Col_Ind.begin();
  auto pos_Values = _m_CRS_Values.begin();
  long k;
  // mapping for the renumbering of the nodes
  vector<long> internal_mapping;
  for (const auto &elem : v_neigh) {
    ind = _m_CRS_Row_Ptr[elem];
    ind_p_one = _m_CRS_Row_Ptr[elem + 1];
    // Constant to keep track and erase the weight
    for (auto it = pos_col + ind; it != pos_col + ind_p_one; ++it) {
      if (*it == i_fc) {
        _m_CRS_Col_Ind.erase(it);
        // define the exact position of the element for the processing of the
        // weight
        // later.
        k = it - pos_col;
        weight_it = pos_Values + k;
        _m_CRS_Values.erase(weight_it);
        // for each found i decrease the successive of 1 for the offset
        for (auto it_bis = _m_CRS_Row_Ptr.begin() + elem + 1;
             it_bis != _m_CRS_Row_Ptr.end(); ++it_bis) {
          *it_bis = *it_bis - 1;
        }
      }
    }
  }
  // reduce the row ptr value of the deleted value
  // do the same with i_fc
  ind = _m_CRS_Row_Ptr[i_fc];
  ind_p_one = _m_CRS_Row_Ptr[i_fc + 1];
  for (auto it = pos_col + ind; it != pos_col + ind_p_one; ++it) {
    _m_CRS_Col_Ind.erase(it);
    // define the exact position of the element for the processing of the weight
    // later.
    k = it - pos_col;
    weight_it = pos_Values + k;
    _m_CRS_Values.erase(weight_it);
    // for each found i decrease the successive of 1 for the offset
    for (auto it_bis = _m_CRS_Row_Ptr.begin() + i_fc + 1;
         it_bis != _m_CRS_Row_Ptr.end(); ++it_bis) {
      *it_bis = *it_bis - 1;
    }
  }
  // Get rid of the col element
  auto Col_pointer = _m_CRS_Row_Ptr.begin() + i_fc;
  // Modify the mapping
  auto mapping_pointer = _mapping_l_to_g.begin() + i_fc;
  auto volumes_pointer = _volumes.begin() + i_fc;
  _volumes.erase(volumes_pointer);
  _m_CRS_Row_Ptr.erase(Col_pointer);
  _mapping_l_to_g.erase(mapping_pointer);
  // now we do not have nomore our node, but we must create a mapping between
  // the
  // before and now, and translate it in the col_ind and update the mapping with
  // the
  // global graph
  long indice = 0;
  // to cycle on the main vector
  long ix = 0;
  while (ix != _m_CRS_Row_Ptr.size() + 1) {
    if (ix != i_fc) {
      internal_mapping.push_back(indice);
      indice++;
    } else {
      internal_mapping.push_back(-1);
    }
    ++ix;
  }
  for (auto &actual : _m_CRS_Col_Ind) {
    actual = internal_mapping[actual];
  }
}

Dual_Graph::Dual_Graph(const long &nb_c, const vector<long> &m_crs_row_ptr,
                       const vector<long> &m_crs_col_ind,
                       const vector<double> &m_crs_values,
                       const vector<double> &volumes,
                       const Seeds_Pool<long> &seeds_pool,
                       const unordered_set<long> &s_anisotropic_compliant_fc,
                       int verbose, short dim)
    : Graph(nb_c, m_crs_row_ptr, m_crs_col_ind, m_crs_values, volumes),
      _seeds_pool(seeds_pool),
      _verbose(verbose),
      _dimension(dim) {
  // We check that effectively in the dictionary we have recorded cells with
  // boundaries and we define the seed pool depending on the dimension of the
  // problem. (2D or 3D)
  if (s_anisotropic_compliant_fc.size() > 0) {
    _s_anisotropic_compliant_cells = s_anisotropic_compliant_fc;
  } else {
    // Default initialization of this->s_anisotropic_compliant_cells
    for (int i = 0; i < _number_of_cells; i++) {
      _s_anisotropic_compliant_cells.insert(i);
    }
  }
}

unsigned short Graph::get_nb_of_neighbours(long i_c) {
  // Return the number of neightbohurs of the ith cell
  return this->_m_CRS_Row_Ptr[i_c + 1] - this->_m_CRS_Row_Ptr[i_c];
}

// Return the dictionary of anisotropic faces. The values are the ratio of the
// area by definition
// while the keys are the global index of the faces
void Dual_Graph::compute_d_anisotropic_fc(
    vector<double> &maxArray, unordered_map<long, double> &d_anisotropic_fc,
    unordered_map<long, double> &d_isotropic_fc, const long preserving) {
  // values are the ratio Max to average (ratioArray[iCell]) and keys
  // the (global) index of the cell. d_anisotropic_fc[ifc]=
  // max_weight/min_weight
  double min_weight, max_weight, averageWeight, weight;

  // Process of every compliant fine cells (it is a member variable, so it is
  // not passed to the function):
  for (const long i_fc : _s_anisotropic_compliant_cells) {
    min_weight = numeric_limits<double>::max();
    max_weight = 0.0;
    averageWeight = 0.0;

    // computation of min_weight, max_weight and averageWeight for the current
    // cell i_loc_fc
    // Process of every faces/Neighboursi and compute for the current cell the
    // neighborhood and the
    // area associated with the neighborhood cells
    vector<long> v_neighbours = get_neighbours(i_fc);
    vector<double> v_weights = get_weights(i_fc);

    assert(v_neighbours.size() == v_weights.size());
    int nb_neighbours = v_neighbours.size();

    for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {
      long i_fc_n = v_neighbours[i_n];
      double i_w_fc_n = v_weights[i_n];
      if (i_fc_n !=
          i_fc) {  // to avoid special case where the boundary value are stored
        if (max_weight < i_w_fc_n) {
          max_weight = i_w_fc_n;
        }
        if (min_weight > i_w_fc_n) {
          min_weight = i_w_fc_n;
        }
      } else {
        nb_neighbours--;
      }

      averageWeight += i_w_fc_n / (double)(nb_neighbours);
    }
    maxArray[i_fc] = max_weight;
    if (preserving == 0) {
      // Anisotropy criteria for the line Admissibility
      if (max_weight / min_weight >= 4.0) {
        // If the ratio is more than 4 of the biggest zith the smallest cell ,
        // add it to the dictionary
        // where I store the ratio between the max and the average
        d_anisotropic_fc[i_fc] = max_weight / averageWeight;
      } else {
        d_isotropic_fc[i_fc] = max_weight / averageWeight;
      }
    } else if (preserving == 2) {
      if (max_weight / min_weight >= 4.0) {
        if (_seeds_pool._d_is_on_bnd.count(i_fc) && nb_neighbours == 3) {
          d_anisotropic_fc[i_fc] = max_weight / averageWeight;
        } else if (nb_neighbours == 4) {
          d_anisotropic_fc[i_fc] = max_weight / averageWeight;
        } else {
          d_isotropic_fc[i_fc] = max_weight / averageWeight;
        }
      } else {
        d_isotropic_fc[i_fc] = max_weight / averageWeight;
      }
    } else if (preserving == 3) {
      if (max_weight / min_weight >= 4.0) {
        if (_seeds_pool._d_is_on_bnd.count(i_fc) && nb_neighbours == 5) {
          d_anisotropic_fc[i_fc] = max_weight / averageWeight;
        } else if (nb_neighbours == 6) {
          d_anisotropic_fc[i_fc] = max_weight / averageWeight;
        } else {
          d_isotropic_fc[i_fc] = max_weight / averageWeight;
        }
      } else {
        d_isotropic_fc[i_fc] = max_weight / averageWeight;
      }
    }
  }
}

vector<deque<long> *> Dual_Graph::compute_anisotropic_line(
    long &nb_agglomeration_lines) {
  /**
   * The goal of this function is :
   * - firstly to look for anisotropic cells through the use of d_anisotropic_fc
   * - secondly build anisotropic lines
   */

  long nb_fc = _number_of_cells;  // Number of cells is a member variable
                                  // initialized through nb_fc in the
  // it is computed in d_anisotropic_fc as the max_weight, hence the maximum
  // area among the faces composing the cell.
  vector<double> maxArray(nb_fc, 0.0);
  unordered_map<long, double> d_anisotropic_fc;
  unordered_map<long, double> d_isotropic_fc;
  // Map to address if the cell has been added to a line
  unordered_map<long, bool> has_been_treated;
  // Computation of the anisotropic cell , alias of the cells for which the
  // ratio between the face with maximum area and the face with minimum area
  // is more than 4. It is a method of the class.
  compute_d_anisotropic_fc(maxArray, d_anisotropic_fc, d_isotropic_fc, 0);
  // Initialization of the map: for each anisotropic cell
  // we check if has been analyzed or not
  for (auto &it : d_anisotropic_fc) {
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
  // vector of deques containing the lines
  vector<deque<long> *> lines;
  // vector of neighbours to the seed
  vector<long> v_neighbours;
  // vector of weight (face area) that connects to the seed neighborhood
  vector<double> v_w_neighbours;
  // we cycle on all the anisotropic cells identified before
  for (auto &it : d_anisotropic_fc) {
    // seed from where we start the deck
    if (has_been_treated[it.first]) {
      // If the cell has been already treated, continue to the next anisotropic
      // cell in the unordered map
      continue;
    }
    // we save the primal seed for the opposite direction check that will happen
    // later
    auto primal_seed = it.first;
    // we initialize the seed at the beginning of each line
    seed = primal_seed;
    // Create the new line
    auto dQue = new deque<long>();
    // we add the first seed
    (*dQue).push_back(seed);
    has_been_treated[seed] = true;
    // Start the check from the seed
    // while the line is not ended
    while (!end) {
      // for the seed (that is updated each time end!= true) we fill the
      // neighbours
      // and the weights
      v_neighbours = get_neighbours(seed);
      v_w_neighbours = get_weights(seed);
      for (auto i = 0; i < v_neighbours.size(); i++) {
        // we check if in the neighbours there is the seed (it should not
        // happen,
        // but we prevent like this mistakes)
        if (v_neighbours[i] == seed) {
          continue;
        }
        if (d_anisotropic_fc.count(v_neighbours[i]) !=
            0  // if anisotropic cell...
                and v_w_neighbours[i] >
                0.75 * maxArray[seed]  // ...and if along the max interface...
                    and !has_been_treated[v_neighbours[i]]) {  // ...and if not
                                                               // treated
          candidates.push_back(v_neighbours[i]);
        }
      }  // end for loop
      // case we have only 1 candidate to continue the line
      if (candidates.size() == 1) {
        // we can add to the actual deque
        if (!opposite_direction_check) {
          (*dQue).push_back(candidates[0]);
        } else {
          (*dQue).push_front(candidates[0]);
        }
        // update the seed to the actual candidate
        seed = candidates[0];
        // the candidate (new seed) has been treated
        has_been_treated[seed] = true;
      }
      // case we have more than one candidate
      else if (candidates.size() > 1) {
        // we cycle on candidates
        /** @todo Not properly efficient. We risk to do twice the operations (we
         * overwrite the seed. This is not proper **/
        for (auto &element : candidates) {
          // if has been treated ==> we check the next candidate
          if (has_been_treated[element]) {
            continue;
          } else {
            // if has not been treated, the opposite direction flag
            // is not active? ==> push back
            if (!opposite_direction_check) {
              (*dQue).push_back(element);
              seed = element;
              has_been_treated[element] = true;
              // It break otherwise we risk to add 2 (problematic with primal
              // seed)
              // It is what is done in Mavriplis
              // https://scicomp.stackexchange.com/questions/41830/anisotropic-lines-identification-algorithm
              break;
            } else {  // if it is active push front
              (*dQue).push_front(element);
              seed = element;
              has_been_treated[element] = true;
              break;
            }
            // we update the seed and the has been treated
          }
        }
      }  // end elseif
      // 0 candidate, we are at the end of the line or at the end
      // of one direction
      else if (candidates.size() == 0) {
        if (opposite_direction_check) {
          end = true;
        } else {
          seed = primal_seed;
          opposite_direction_check = true;
        }
      }
      // we clear the candidates and the neighbours value for the
      // next seed
      candidates.clear();
      v_w_neighbours.clear();
      v_neighbours.clear();
    }
    // we initialize the flags
    end = false;
    opposite_direction_check = false;
    // we push the deque to the list if are bigger than 1
    if ((*dQue).size() > 1) {
      lines.push_back(dQue);
      lines_size++;
    }
  }
  nb_agglomeration_lines = (long)lines_size;
  return lines;
}

unsigned short int Dual_Graph::compute_min_fc_compactness_inside_a_cc(
    unordered_set<long> &s_fc) {
  // Compute Compactness of a cc
  // Be careful: connectivity is assumed
  if (s_fc.size() > 1) {
    unordered_map<long, unsigned short int> dict_fc_compactness =
        compute_fc_compactness_inside_a_cc(s_fc);
    if (dict_fc_compactness.empty()) {
      return 0;
    }
    unsigned short int min_comp = USHRT_MAX;
    for (auto &i_k_v : dict_fc_compactness) {
      if (i_k_v.second < min_comp) {
        min_comp = i_k_v.second;
      }
    }
    return min_comp;
  } else {
    return 0;
  }
}

unordered_map<long, unsigned short int>
Dual_Graph::compute_fc_compactness_inside_a_cc(unordered_set<long> &s_fc) {
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

// To chose if the metric of cardinality is pertinent
void Dual_Graph::compute_neighbourhood_of_cc(
    const unordered_set<long> s_seeds, short &nb_of_order_of_neighbourhood,
    unordered_map<long, short> &d_n_of_seed,  // defined also as
                                              // d_of_neighborhood, output
    const short max_card, vector<bool> &is_fc_agglomerated_tmp) {
  // Basic checks
  assert(max_card != -1);
  unordered_map<long, int> d_n_of_order_o_m_one;  // dict of FC with the order
                                                  // of neighbouring from seed
  // we initialize for seeds where order is 0
  for (const long &i_fc : s_seeds) {
    d_n_of_order_o_m_one[i_fc] = 0;
  }

  int i_order = 1;

  while ((i_order < nb_of_order_of_neighbourhood + 1) ||
         (d_n_of_seed.size() + d_n_of_order_o_m_one.size()) < max_card) {

    unordered_map<long, int> d_n_of_order_o;

    for (auto id_M_one : d_n_of_order_o_m_one) {
      d_n_of_seed[id_M_one.first] = id_M_one.second;
    }

    for (const auto &i_k_v : d_n_of_order_o_m_one) {

      long seed_tmp = i_k_v.first;
      for (const long &i_fc_n : get_neighbours(seed_tmp)) {
        if ((d_n_of_seed.count(i_fc_n) == 0) &&
            ((!is_fc_agglomerated_tmp[i_fc_n]))) {
          if (d_n_of_order_o.count(i_fc_n) == 0) {
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

    // Exit condition
    if (d_n_of_order_o.empty()) {
      // No more neighbours available:
      break;
    }

    d_n_of_order_o_m_one.clear();
    for (auto id : d_n_of_order_o) {
      d_n_of_order_o_m_one[id.first] = id.second;
    }
    i_order++;
  }
  // Update of d_n_of_seed
  // d_n_of_seed.update(d_n_of_order_o_m_one)
  for (auto id_M_one : d_n_of_order_o_m_one) {
    d_n_of_seed[id_M_one.first] = id_M_one.second;
  }

  // We remove the seed from the neighbours of seed
  for (const long &i_fc : s_seeds) {
    d_n_of_seed.erase(i_fc);
  }

  nb_of_order_of_neighbourhood = i_order;
}
