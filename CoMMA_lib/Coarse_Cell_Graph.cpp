//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Coarse_Cell_Graph.h"
Coarse_Cell_Graph::Coarse_Cell_Graph(Dual_Graph& fc_graph,
                                     int verbose,
                                     vector<long> debug_only_fc_to_cc):_fc_graph(fc_graph), _verbose(verbose){
    vector<bool> tmp(fc_graph.number_of_cells, false);
    (*this)._a_is_fc_agglomerated = tmp;

    // TODO mutualized Anisotropic with isotropic.
//    self._d_isotropic_cc: Dict[int, Coarse_Cell] = dict()
//
//# Contains the CC index: list of FC, that can be modified (not anisotropic CC, and not "boundary" CC!
//
//    self._d_anisotropic_cc: Dict[int, Set[int]] = dict()  # Dict of indices of anisotropic CC
//
//# Pointers from caracteristics to cc index:
//    self._d_card_2_cc: Dict[int, Set[int]] = dict()  # Contains the CC that can be modified, indexed by their
//# cardinal i.e. {card: set of index of coarse cells of cardinal card}
//    self._d_compactness_2_cc: Dict[int, Set[int]] = dict()  # Contains for every degree of compactness the list of
//# CC of this type i.e. {deg_Of_compactness: set of CC of compactness deg_Of_compactness}
//
//    #############################
//# Output datas
//    #############################
//
//# TODO np.int32!
//# TODO assert that the number of fc is smaller and max np.int32
//    self._fc_2_cc: np.ndarray = -1 * np.ones((self._fc_graph.number_of_cells,), dtype=np.int)
//
//    if debug_only_fc_to_cc is not None:
//    assert debug_only_fc_to_cc.size == self._fc_2_cc.size, str(debug_only_fc_to_cc.size) + "\t" + str(
//            self._fc_2_cc.size)
//    d_cc = dict()
//    for i_fc, i_cc in enumerate(debug_only_fc_to_cc):
//    if i_cc in d_cc:
//    d_cc[i_cc].add(i_fc)
//    else:
//    d_cc[i_cc] = {i_fc}
//    s_anisotropic_cc = set()
//    if debug_a_anisotropic_lines:
//        for i_l in debug_a_anisotropic_lines:
//    for i_cc in i_l:
//    s_anisotropic_cc.add(i_cc)
//
//    nb_cc = np.max(debug_only_fc_to_cc) + 1
//    for i_cc in range(nb_cc):
//    self.cc_create_a_cc(s_fc=d_cc[i_cc], is_anisotropic=i_cc in s_anisotropic_cc)
//
//    np.copyto(self._fc_2_cc, debug_only_fc_to_cc)
//    self.fill_cc_neighbouring()
}
