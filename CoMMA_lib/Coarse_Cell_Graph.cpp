//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Coarse_Cell_Graph.h"

Coarse_Cell_Graph::Coarse_Cell_Graph(const Dual_Graph<long, double> &fc_graph,
                                     int verbose)
    : _fc_graph(fc_graph), _verbose(verbose) {
  //==================
  // Input datas
  //==================
  // Initialization of the vector of the agglomerated cells
  // as false at the beginning, are all not agglomerated.
  vector<bool> tmp(fc_graph._number_of_cells, false);
  _a_is_fc_agglomerated = tmp;
  //==================
  // temporary fields
  //==================
  // field that identify the index of the coarse cells in the
  // fine cell (and retrived as the output)
  _fc_2_cc = vector<long>(fc_graph._number_of_cells, -1);
}

Coarse_Cell_Graph::~Coarse_Cell_Graph() {}

long Coarse_Cell_Graph::cc_create_a_cc(const unordered_set<long> &s_fc,
                                       bool is_anisotropic,
                                       bool is_creation_delayed) {

  // Create a course cell from the fine cells and update the fc_2_cc tree.
  assert((!is_anisotropic) || (!is_creation_delayed));
  // error handling
  double vol_cc = 0;
  for (const long &i_fc : s_fc) {
    if (!_a_is_fc_agglomerated[i_fc]) {
      // Rq: initialise to False pour chaque niveau dans agglomerate(...)
      _a_is_fc_agglomerated[i_fc] = true;
      _nb_of_agglomerated_fc++;
    }
  }
  for (const long &i_fc : s_fc) {
    vol_cc = vol_cc + _fc_graph._volumes[i_fc];
    assert(_fc_2_cc[i_fc] == -1);
  }
  // Anisotropic case
  bool is_mutable = true;
  if (is_anisotropic) {
    assert(!is_creation_delayed);
    _d_anisotropic_cc[_cc_counter] = unordered_set<long>(s_fc);
    auto new_cc = make_shared<Coarse_Cell<long, double>>(_fc_graph, _cc_counter,
                                                         s_fc, false);
    // we collect the various cc_graph, where the index in the vector is the
    // i_cc
    _cc_vec.insert(pair<long, shared_ptr<Subgraph<long, double>>>(
        _cc_counter, new_cc->_cc_graph));
    is_mutable = false;
  }
  if (!is_creation_delayed) {
    // We create the cc right now.
    // Everything is updated:
    if (is_mutable) {
      // the cell can be modified afterwards and is thus defined in dict_cc and
      // dict_card_cc, dict_compactness_2_cc, dict_cc_to_compactness
      // Update of dict_cc:
      //==================
      auto new_cc =
          make_shared<Coarse_Cell<long, double>>(_fc_graph, _cc_counter, s_fc);
      // we collect the various cc_graph, where the index in the vector is the
      // i_cc
      _cc_vec.insert(pair<long, shared_ptr<Subgraph<long, double>>>(
          _cc_counter, new_cc->_cc_graph));
      _d_isotropic_cc[_cc_counter] = new_cc;

      // Update of compactness informations:
      //####################################
      assert((*new_cc).is_connected());
    }
    // Update of _associatedCoarseCellNumber the output of the current function
    // agglomerate
    // _fc_2_cc is filled with _cc_counter
    for (const long &i_fc : s_fc) {
      // Only if not isCreationDelayed:
      assert(_fc_2_cc[i_fc] == -1);
      _fc_2_cc[i_fc] = _cc_counter;
    }
    // Update of the number of CC
    //####################################
    _cc_counter++;  // Only if not isCreationDelayed
  } else {
    // We do not create the coarse cell yet.
    // As this coarse cell will be soon deleted, we want its coarse index to be
    // the greater possible.
    // Only isFineCellAgglomerated_tmp, number_of_fine_agglomerated_cells_tmp
    // and
    // dict_DistributionOfCardinalOfCoarseElements are modified!
    _delayed_cc.push_back(s_fc);
  }
  return (_cc_counter - 1);
}

unordered_map<long, unordered_set<long>> Coarse_Cell_Graph::get_d_cc_iso() {

  // Watch out that delayed_cc may not be empty , i.e. not yet created and
  // numbered!
  unordered_map<long, unordered_set<long>> d;

  for (auto &i_k_v : _d_isotropic_cc) {
    long i_cc = i_k_v.first;
    unordered_set<long> tmp(i_k_v.second->_s_fc);
    d[i_cc] = tmp;
  }
  return d;
}

unordered_map<long, unordered_set<long>> Coarse_Cell_Graph::get_d_cc_aniso() {

  // Watch out that delayed_cc may not be empty , i.e. not yet created and
  // numbered!
  unordered_map<long, unordered_set<long>> d;

  for (auto &i_k_v : _d_anisotropic_cc) {
    long i_cc = i_k_v.first;
    d[i_cc] = i_k_v.second;
  }
  return d;
}

unordered_map<long, unordered_set<long>> Coarse_Cell_Graph::get_d_cc_all() {
  // Watch out that delayed_cc may not be empty , i.e. not yet created and
  // numbered!
  unordered_map<long, unordered_set<long>> d;

  for (auto &i_k_v : _d_isotropic_cc) {
    long i_cc = i_k_v.first;
    unordered_set<long> tmp(i_k_v.second->_s_fc);
    d[i_cc] = tmp;
  }
  for (auto &i_k_v : _d_anisotropic_cc) {
    long i_cc = i_k_v.first;
    d[i_cc] = i_k_v.second;
  }
  return d;
}

vector<long> Coarse_Cell_Graph::get_neigh_cc(const long &i_fc,
                                             const long &i_cc) {
  vector<long> neigh = _fc_graph.get_neighbours(i_fc);
  vector<long> result;
  for (const long &elem : neigh) {
    if (_fc_2_cc[elem] != i_cc) {
      result.push_back(_fc_2_cc[elem]);
    }
  }
  return (result);
}

// update the member variable based on mapping
void Coarse_Cell_Graph::update_fc_2_cc(const vector<long> &mapping) {
  for (auto &elem : _fc_2_cc) {
    // we substitute in elem the mapped element
    elem = mapping[elem];
  }
}

map<long, shared_ptr<Subgraph<long, double>>>::iterator
Coarse_Cell_Graph::remove_cc(
    map<long, shared_ptr<Subgraph<long, double>>>::iterator elim) {
  // we delete the element and we obtainer the pointer to the next element in
  // memory
  map<long, shared_ptr<Subgraph<long, double>>>::iterator it =
      _cc_vec.erase(elim);
  // We get the length
  auto lung = _cc_vec.size();
  // update value of the other nodes
  for (auto i = it; i != _cc_vec.end(); i++) {
    for (auto const &i_fc : i->second->_mapping_l_to_g) {
      _fc_2_cc[i_fc] = (i->first) - 1;
    }
    auto node = _cc_vec.extract(i);
    if (!node.empty()) {
      node.key() = (i->first) - 1;
      _cc_vec.insert(move(node));
    }
  }
  // return pointer to the next element
  return (it);
}

void Coarse_Cell_Graph::correct(const long &max_card) {
  // initializing vector neigh_cc
  vector<long> neigh;
  // We cycle on the subgraphs of the bimap structure
  auto it = _cc_vec.begin();
  // We use itold to understand if we have succeded in the correction
  auto it_old = _cc_vec.begin();
  auto end = _cc_vec.end();
  while (it != end) {
    // We enter in the property of the subgraph of being 1
    // and we consider what hapepns. Remember that second because
    // we are checking the subgraph
    auto current_cc = it->second;
    auto i_cc = it->first;
    // check the isotropic cells with cardinality 1
    if (current_cc->_cardinality == 1 && current_cc->_is_isotropic == true) {
      // Get the cc neigh of the given fine cell
      auto i_fc = current_cc->_mapping_l_to_g[0];
      neigh = get_neigh_cc(i_fc, i_cc);
      // now we have the neighborhood cc cell, we can access to them and
      // control the characteristics
      for (auto const &elem : neigh) {
        auto neig_cc = _cc_vec[elem];
        if (neig_cc->_compactness > 0 && neig_cc->_cardinality >= 2 &&
            neig_cc->_is_isotropic == true) {
          // If the condition is verified we add the cell to the identified cc
          // and
          // we remove it from the current cc
          // first we assign to the fc_2_cc the new cc (later it will be
          // renumbered considering the deleted
          // cc)
          _fc_2_cc[i_fc] = elem;
          vector<long> fine_neigh = _fc_graph.get_neighbours(i_fc);
          vector<double> fine_weights = _fc_graph.get_weights(i_fc);
          neig_cc->insert_node(fine_neigh, i_fc, _fc_graph._volumes[i_fc],
                               fine_weights);
          current_cc->remove_node(i_fc);
          // the new it point directly to the next element in the map
          it = remove_cc(it);
          break;
        }
      }
      // if we failed we go on, it is life, so we agglomerate to the nearest
      // cell (the first one of the vector
      if (it == it_old) {
        auto const elem = neigh[0];
        auto neig_cc = _cc_vec[elem];
        _fc_2_cc[i_fc] = elem;
        vector<long> fine_neigh = _fc_graph.get_neighbours(i_fc);
        vector<double> fine_weights = _fc_graph.get_weights(i_fc);
        neig_cc->insert_node(fine_neigh, i_fc, _fc_graph._volumes[i_fc],
                             fine_weights);
        current_cc->remove_node(i_fc);
        // the new it point directly to the next element in the map
        it = remove_cc(it);
      }
      neigh.clear();
      it_old = it;
    } else {
      ++it;
    }
  }
}

void Coarse_Cell_Graph::cc_create_all_delayed_cc() {
  for (const unordered_set<long> &s_fc : _delayed_cc) {
    cc_create_a_cc(s_fc);
  }
  _delayed_cc.clear();
}

bool Coarse_Cell_Graph::is_cc_grid_not_structured(short goal_card) {

  // All fc are agglomerated
  assert(_nb_of_agglomerated_fc == _fc_graph._number_of_cells);

  if (!_d_anisotropic_cc.empty()) {
    return true;
  } else {
    short goal_compactness = _fc_graph._dimension;

    for (const auto &i_k_v : _d_isotropic_cc) {
      if ((*i_k_v.second)._cc_graph->_compactness != goal_compactness) {
        return true;
      }
    }
    if (!_delayed_cc.empty()) {
      return true;
    }
    return _fc_graph._s_anisotropic_compliant_cells.empty();
  }
}

unordered_map<unsigned short, long>
Coarse_Cell_Graph::compute_d_distribution_of_cardinal_of_isotropic_cc() {
  unordered_map<unsigned short, long> d_distr_card_cc;
  for (const auto &i_k_v : _d_isotropic_cc) {
    unsigned short card =
        (*_d_isotropic_cc[i_k_v.first])._cc_graph->_cardinality;
    if (d_distr_card_cc.count(card)) {
    } else {
      d_distr_card_cc[card] = 1;
    }
  }
  return d_distr_card_cc;
}

void Coarse_Cell_Graph::print_d_distribution_of_cardinal_of_isotropic_cc() {

  unordered_map<unsigned short, long> d_dist =
      compute_d_distribution_of_cardinal_of_isotropic_cc();
  cout << "{ ";
  for (auto &i_k_v : d_dist) {
    cout << i_k_v.first << ": " << i_k_v.second << ", ";
  }
  cout << "}" << endl;
}
