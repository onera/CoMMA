//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Coarse_Cell.h"

Coarse_Cell::Coarse_Cell(Dual_Graph<long, double> &fc_graph, long i_cc,
                         const unordered_set<long> &s_fc, bool is_isotropic)
    : _dim(fc_graph._dimension), _is_isotropic(is_isotropic) {
  // compactness, degrees are defined in the Subgraph
  // Other quantities are defined in the cc_graph map (e.h the i_cc)
  _fc_graph = &fc_graph;

  for (const long &i_fc : s_fc) {
    _s_fc.insert(i_fc);
  }
  for (const long &i_fc : s_fc) {
    volume += fc_graph._volumes[i_fc];
  }

  _mapping_g_to_l = build_CRS();
  _cc_graph = make_shared<Subgraph<long, double> >(
      s_fc.size(), _adjMatrix_row_ptr, _adjMatrix_col_ind,
      _adjMatrix_areaValues, _volumes, _mapping_g_to_l, is_isotropic);
}

vector<long> Coarse_Cell::build_CRS() {
  // initialization vectors
  long position = 0;
  long index_weight;
  vector<long> neigh;
  vector<double> weight;
  vector<long> row_ptr = {0};
  vector<long> col_ind;
  vector<long> mapping;
  vector<double> area;
  for (const long &i_fc : _s_fc) {
    // we add to the mapping the i_fc
    mapping.push_back(i_fc);
    // get neighbours and the weights associated
    neigh = _fc_graph->get_neighbours(i_fc);
    area = _fc_graph->get_weights(i_fc);
    for (auto it = neigh.begin(); it != neigh.end(); ++it) {
      index_weight = it - neigh.begin();
      if (_s_fc.count(*it)) {
        ++position;
        col_ind.push_back(*it);
        weight.push_back(area[index_weight]);
      }
    }
    row_ptr.push_back(position);
    _volumes.push_back(_fc_graph->_volumes[i_fc]);
  }
  _adjMatrix_row_ptr = row_ptr;
  // Map in the local subgraph
  for (auto it = col_ind.begin(); it != col_ind.end(); ++it) {
    auto indx = find(mapping.begin(), mapping.end(), *it);
    index_weight = indx - mapping.begin();
    _adjMatrix_col_ind.push_back(index_weight);
  }
  _adjMatrix_areaValues = weight;

  return (mapping);
}

bool Coarse_Cell::is_connected() {

  if (!_is_connectivity_up_to_date) {
    _is_connected = _cc_graph->check_connectivity();
    _is_connectivity_up_to_date = true;
  }
  return _is_connected;
}
