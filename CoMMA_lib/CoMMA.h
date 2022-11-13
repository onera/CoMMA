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
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 * @param[in] adjMatrix_row_ptr the row pointer of the CRS representation
 * @param[in] adjMatrix_col_ind the column index of the CRS representation
 * @param[in] adjMatrix_areaValues the weight of the CRS representation (in CoMMA case
 * will be the area of the faces that in the graph representation are the edges
 * between two nodes represented by the cell centers.
 * @param[in] volumes The volumes of the cells
 * @param[in] priority_weights Weights used to set the order telling where to start
 * agglomerating. The higher the weight, the higher the priority
 *  @param[in] arrayOfFineAnisotropicCompliantCells List of cells which have to be looked
 * for anisotropy
 * @param[in] n_bnd_faces Vector telling how many boundary faces each cell has
 * @param[in] isFirstAgglomeration Whether if it's the first level of agglomeration
 * (it that's the case, the agglomeration lines will be built)
 * @param[in] is_anisotropic Whether to consider an anisotropic agglomeration
 * @param[in] threshold_anisotropy Value of the aspect-ratio above which a cell is
 * considered as anisotropic
 * @param[in] type_of_isotropic_agglomeration Type of algorithm to consider when
 * agglomerating isotropic cells. Two alternatives: Biconnected: requested with 0,
 * standard algorithm where we consider every neighbour of the coarse cell as candidate;
 * Pure Front Advancing: requested with 1, only direct neighbours of the last added
 * cell are candidates
 * @param[out] fc_to_cc Vector telling the ID of the coarse cell to which a fine cell
 * belongs to after agglomeration
 * @param[in,out] agglomerationLines_Idx Connectivity for the agglomeration lines: each
 * element points to a particular element in the vector agglomerationLines
 * @param[in,out] agglomerationLines Vector storing all the elements of the
 * anisotropic lines
 * @param[in] correction Whether to apply correction step (avoid isolated cells) after
 * agglomeration
 * @param[in] dimension Dimensionality of the problem, 2- or 3D
 * @param[in] goal_card Expected cardinality of the coarse cells (might not be ensured)
 * @param[in] min_card Minimum cardinality accepted for the coarse cells
 * @param[in] max_card Maximum cardinality accepted for the coarse cells
 * */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
void agglomerate_one_level(  // Dual graph:
    const vector<CoMMAIndexType> &adjMatrix_row_ptr,
    const vector<CoMMAIndexType> &adjMatrix_col_ind,
    const vector<CoMMAWeightType> &adjMatrix_areaValues,
    const vector<CoMMAWeightType> &volumes,
    const vector<CoMMAWeightType> &priority_weights,

    // Indices of compliant cc
    const vector<CoMMAIndexType> &arrayOfFineAnisotropicCompliantCells,

    // boundaries
    const vector<CoMMAIntType> &n_bnd_faces,

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
  // It is built the dual graph class through the constructor. To see it look at
  // DualGraph.hpp and DualGraph.cpp
  // fc = Fine Cells
  assert(dimension < USHRT_MAX);
  Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> seeds_pool(
    n_bnd_faces, priority_weights);
  Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> fc_graph(
      nb_fc, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues,
      volumes, seeds_pool, s_anisotropic_compliant_fc);
  // Debug
  //    vector<double> maxArray(nb_fc, 0.0);
  //    unordered_map<long, double> d_anisotropic_fc;
  //    unordered_map<long, double> d_isotropic_fc;
  //    //             // ration between the face with maximum area and the face
  // with minimum area
  //    //                 // is more than 4.
  //    fc_graph.compute_d_anisotropic_fc(maxArray,d_anisotropic_fc,d_isotropic_fc);
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
            fc_graph, cc_graph, threshold_anisotropy, dimension = dimension);
    //    Agglomerator* agg1 = new Agglomerator_Anisotropic(fc_graph,
    //                                    cc_graph,
    //                                    dimension = dimension);
    CoMMAIndexType nb_agglomeration_lines = 0;
    vector<deque<CoMMAIndexType> *> agglomeration_lines;
    // case in which we have already agglomerated one level and hence we have
    // already agglomeration
    // lines available; no need to recreate them.
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
        // for (long j = ind; j < indPOne; j++) {
        //     (*dQue).push_back(agglomerationLines[j]);
        // }
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
    agg_dyn->agglomerate_one_level(min_card, goal_card, max_card, priority_weights, false);
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
        fc_graph, cc_graph, dimension = dimension);
  }
  else {
    agg = make_unique<
        Agglomerator_Pure_Front<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
        fc_graph, cc_graph, dimension = dimension);
  }
  agg->agglomerate_one_level(min_card, goal_card, max_card, priority_weights, correction);
  // Agglomerate
  // FILLING FC TO CC (it is a property of the cc_graph but retrieved through an
  // helper of the agglomerator)
  auto fccc = cc_graph._fc_2_cc;
  for (CoMMAIndexType i_fc = 0; i_fc < nb_fc; i_fc++) {
    fc_to_cc[i_fc] = fccc[i_fc];
  }
}
#endif
