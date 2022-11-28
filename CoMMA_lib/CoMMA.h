#ifndef COMMA_PROJECT_COMMA_H
#define COMMA_PROJECT_COMMA_H

/*
 *  * CoMMA
    * Copyright © 2022 ONERA
    *
    * Authors and Architects : Alberto Remigi and Nicolas Lantos
    *
    * Contributors : Riccardo Milani
    *
    * This program is free software: you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation, either version 3 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
    * You should have received a copy of the GNU General Public License
    * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Agglomerator.h"
#include "templateHelpers.h"
#include "CoMMATypes.h"

template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
using IsotropicPtr = std::unique_ptr<Agglomerator_Isotropic<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>;

/** @brief Main function of the agglomerator, it is used as an interface
 * to build up all the agglomeration process. The result will be the definition
 * of the agglomerated cells fc2cc.
 * */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
void agglomerate_one_level(  // Dual graph:
    const vector<CoMMAIndexType> adjMatrix_row_ptr,
    const vector<CoMMAIndexType> adjMatrix_col_ind,
    const vector<CoMMAWeightType> adjMatrix_areaValues,
    const vector<CoMMAWeightType> volumes,

    // Indices of compliant cc
    vector<CoMMAIndexType> &arrayOfFineAnisotropicCompliantCells,

    // boundaries
    const vector<CoMMAIndexType> &isOnFineBnd,

    // Agglomeration argument
    bool isFirstAgglomeration, bool is_anisotropic,
    CoMMAWeightType threshold_anisotropy,
    const CoMMAIntType type_of_isotropic_agglomeration,

    // Outputs
    vector<CoMMAIndexType> &fc_to_cc,                // Out
    vector<CoMMAIndexType> &agglomerationLines_Idx,  // In & out
    vector<CoMMAIndexType> &agglomerationLines,      // In & out

    // Args with default value
    bool correction, CoMMAIntType dimension, CoMMAIntType goal_card,
    CoMMAIntType min_card, CoMMAIntType max_card) {

  // SIZES CAST
  //======================================
  // number of faces
  const CoMMAIndexType nb_fc =
      static_cast<CoMMAIndexType>(adjMatrix_row_ptr.size() - 1);

  // BOUNDARIES
  //======================================
  // initialization of map d_is_on_bnd.
  // We create a dictionary of the faces on boundary
  // In particular starting from the vector we pass we store in a map
  // the key relative to the cell analysed and the relative NUMBER OF FACES on
  // the boundary
  unordered_map<CoMMAIndexType, CoMMAIntType> d_is_on_bnd;
  for (CoMMAIndexType i = 0; i < nb_fc; i++) {
    if (isOnFineBnd[i] > CoMMACellT::INTERIOR) {
      fill_value<true, CoMMAIntType, const CoMMAIndexType>(d_is_on_bnd[i],
                                                           isOnFineBnd[i]);
    }
  }
  // ANISOTROPIC COMPLIANT FC
  //======================================
  // Elements that is checked if they are anisotropic.
  // e.g : in case of CODA software are passed all the children, and hence all
  // the source elements of the
  // previous agglomeration process.
  unordered_set<CoMMAIndexType> s_anisotropic_compliant_fc(
      arrayOfFineAnisotropicCompliantCells.begin(),
      arrayOfFineAnisotropicCompliantCells.end());

  // DUAL GRAPH
  //======================================
  // fc = Fine Cells
  assert(dimension == 2 || dimension == 3);
  Seeds_Pool<CoMMAIndexType, CoMMAIntType> seeds_pool(nb_fc, d_is_on_bnd);
  Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> fc_graph(
      nb_fc, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues,
      volumes, seeds_pool, dimension, s_anisotropic_compliant_fc);
  Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> cc_graph(
      fc_graph);
  // AGGLOMERATION ANISOTROPIC FOLLOWED BY ISOTROPIC AGGLOMERATION
  // @todo maybe re-refactor the class agglomerator to allow the implicit upcast
  // like the biconnected case
  // The agglomerator anisotropic is not called with the  implicit upcasting
  // pointing because of the initialization of
  // the anisotropic lines.
  // for more information look at:
  // https://stackoverflow.com/questions/19682402/initialize-child-object-on-a-pointer-to-parent
  // About constructors when upcasting :
  // https://www.reddit.com/r/learnprogramming/comments/1wopf6/java_which_constructor_is_called_when_upcasting/
  if (is_anisotropic) {

    shared_ptr<Agglomerator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>
        agg1 = make_shared<Agglomerator_Anisotropic<
            CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
            fc_graph, cc_graph, threshold_anisotropy, dimension);
    CoMMAIndexType nb_agglomeration_lines = 0;
    vector<deque<CoMMAIndexType> *> agglomeration_lines;
    // case in which we have already agglomerated one level and hence we have
    // already agglomeration lines available; no need to recreate them.
    if (!isFirstAgglomeration) {
      correction = false;
      auto fineAgglomerationLines_array_Idx_size =
          agglomerationLines_Idx.size();
      for (CoMMAIndexType i = fineAgglomerationLines_array_Idx_size - 2; i > -1;
           i--) {
        CoMMAIndexType ind = agglomerationLines_Idx[i];
        CoMMAIndexType indPOne = agglomerationLines_Idx[i + 1];
        deque<CoMMAIndexType> *dQue =
            new deque<CoMMAIndexType>(agglomerationLines.begin() + ind,
                                      agglomerationLines.begin() + indPOne);
        agglomeration_lines.push_back(dQue);
        nb_agglomeration_lines++;
      }
    }
    shared_ptr<Agglomerator_Anisotropic<CoMMAIndexType, CoMMAWeightType,
                                        CoMMAIntType>> agg_dyn =
        dynamic_pointer_cast<Agglomerator_Anisotropic<
            CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(agg1);
    agg_dyn->_v_lines[0] = agglomeration_lines;
    agg_dyn->_v_nb_lines[0] = nb_agglomeration_lines;
    agg_dyn->agglomerate_one_level(min_card, goal_card, max_card, false);
    // level of the line: WARNING! here 1 it means that we give it back lines in
    // the new global
    // index, 0 the old
    CoMMAIntType i_level = 1;
    agg_dyn->get_agglo_lines(i_level, agglomerationLines_Idx,
                             agglomerationLines);
  }
  // We define here the type of Agglomerator
  IsotropicPtr<CoMMAIndexType, CoMMAWeightType,CoMMAIntType> agg = nullptr;
  // TODO: maybe pass to a switch when another agglomerator will be implemented
  if (type_of_isotropic_agglomeration==CoMMAAgglT::BICONNECTED){
    agg = make_unique<
        Agglomerator_Biconnected<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
        fc_graph, cc_graph, dimension);
  }
  else {
    agg = make_unique<
        Agglomerator_Pure_Front<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
        fc_graph, cc_graph, dimension);
  }
  agg->agglomerate_one_level(min_card, goal_card, max_card, correction);
  // Agglomerate
  // FILLING FC TO CC (it is a property of the cc_graph but retrieved through an
  // helper of the agglomerator)
  auto fccc = cc_graph._fc_2_cc;
  for (CoMMAIndexType i_fc = 0; i_fc < nb_fc; i_fc++) {
    fc_to_cc[i_fc] = fccc[i_fc];
  }
}
#endif
