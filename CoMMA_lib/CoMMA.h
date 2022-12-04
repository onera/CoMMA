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

#include <algorithm>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "CoMMATypes.h"

#include "Agglomerator.h"
#include "Coarse_Cell_Container.h"
#include "Dual_Graph.h"
#include "Seeds_Pool.h"

#define check_signed_int_type(intT, label) \
  static_assert(is_signed<intT>::value && numeric_limits<intT>::is_integer, \
      "CoMMA works with signed integer types, but " #intT " (" label ") is not")

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
 * @param[in] centers Cell centers
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
 * @throw `invalid_argument` if dimension is not 2 nor 3, or if cardinalities are
 * smaller than 1 or not in order
 * */
template <typename CoMMAIndexType, typename CoMMAWeightType,
          typename CoMMAIntType>
void agglomerate_one_level(
    // Dual graph:
    const vector<CoMMAIndexType> &adjMatrix_row_ptr,
    const vector<CoMMAIndexType> &adjMatrix_col_ind,
    const vector<CoMMAWeightType> &adjMatrix_areaValues,
    const vector<CoMMAWeightType> &volumes,
    const vector<vector<CoMMAWeightType>> &centers,

    // Order related parameter:
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
  // NOTATION
  //======================================
  // fc = Fine Cell
  // cc = Coarse Cell

  // SANITY CHECKS
  //======================================
  // We sometimes rely on -1 as default parameters (@TODO: could it be changed?)
  check_signed_int_type(CoMMAIndexType, "first template argument");
  check_signed_int_type(CoMMAIntT, "third template argument");
  if ( !(dimension == 2 || dimension == 3) )
    throw invalid_argument( "CoMMA - Error: dimension must be 2 or 3" );
  if ( min_card <= 1 || goal_card <= 1 || max_card <= 1 )
    throw invalid_argument( "CoMMA - Error: Cardinalities must be greater than 1" );
  if ( !( min_card <= goal_card && goal_card <= max_card ) )
    throw invalid_argument( "CoMMA - Error: Cardinalities must be in order (min <= goal <= max)" );

  // SIZES CAST
  //======================================
  const CoMMAIndexType nb_fc =
      static_cast<CoMMAIndexType>(adjMatrix_row_ptr.size() - 1);

  // BOUNDARY FACES
  //======================================
  // Sometimes partitioners give a number of boundary faces higher than the physical
  // one. We fix this
  const CoMMAIntType expected_max_n_bnd = dimension;
  vector<CoMMAIntType> fixed_n_bnd_faces(n_bnd_faces.size());
  replace_copy_if(n_bnd_faces.begin(), n_bnd_faces.end(), fixed_n_bnd_faces.begin(),
      [expected_max_n_bnd](auto n){return n > expected_max_n_bnd;}, expected_max_n_bnd);

  // ANISOTROPIC COMPLIANT FC
  //======================================
  // Elements that are checked if they are anisotropic. If an element satisfies
  // the condition for being anisotropic (typically, AR > threshold) but it not
  // in this set, it will not considered as anisotropic.
  // We use a set to ensure uniqueness
  unordered_set<CoMMAIndexType> s_anisotropic_compliant_fc(
      arrayOfFineAnisotropicCompliantCells.begin(),
      arrayOfFineAnisotropicCompliantCells.end());

  // SEED POOL
  //======================================
  // Object providing the order of agglomeration
  Seeds_Pool<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> seeds_pool(
      fixed_n_bnd_faces, priority_weights);

  // DUAL GRAPH
  //======================================
  // Object containing the graph representation and related info in a convenient structure
  Dual_Graph<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> fc_graph(
      nb_fc, adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues,
      volumes, centers, fixed_n_bnd_faces, dimension, s_anisotropic_compliant_fc);

  // COARSE CELL CONTAINER
  //======================================
  // Preparing the object that will contain all the coarse cells
  Coarse_Cell_Container<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> cc_graph(
      fc_graph);

  // AGGLOMERATION OF ANISOTROPIC CELLS
  //======================================
  // @todo maybe re-refactor the class agglomerator to allow the implicit upcast
  // like the biconnected case
  // The agglomerator anisotropic is not called with the implicit upcasting
  // pointing because of the initialization of
  // the anisotropic lines.
  // for more information look at:
  // https://stackoverflow.com/questions/19682402/initialize-child-object-on-a-pointer-to-parent
  // About constructors when upcasting:
  // https://www.reddit.com/r/learnprogramming/comments/1wopf6/java_which_constructor_is_called_when_upcasting/
  if (is_anisotropic) {
    // Build anisotropic agglomerator
    Agglomerator_Anisotropic<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>
        aniso_agg(fc_graph, cc_graph, seeds_pool, threshold_anisotropy,
                  agglomerationLines_Idx, agglomerationLines, isFirstAgglomeration,
                  dimension);

    // Agglomerate anisotropic cells only
    aniso_agg.agglomerate_one_level(goal_card, min_card, max_card, priority_weights, false);

    // Put anisotropic lines computed just above into the out parameters
    // (Info about level of the line: WARNING! here 1 it means that we give it back
    // lines in the new global index, 0 the old)
    const CoMMAIntType i_level{1};
    aniso_agg.get_agglo_lines(i_level, agglomerationLines_Idx, agglomerationLines);
  }

  // AGGLOMERATION OF ISOTROPIC CELLS
  //======================================
  // We define here the type of Agglomerator
  IsotropicPtr<CoMMAIndexType, CoMMAWeightType,CoMMAIntType> agg = nullptr;
  // TODO: maybe pass to a switch when another agglomerator will be implemented
  if (type_of_isotropic_agglomeration==CoMMAAgglT::BICONNECTED){
    agg = make_unique<
        Agglomerator_Biconnected<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
        fc_graph, cc_graph, seeds_pool, dimension);
  }
  else {
    agg = make_unique<
        Agglomerator_Pure_Front<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>>(
        fc_graph, cc_graph, seeds_pool, dimension);
  }
  agg->agglomerate_one_level(goal_card, min_card, max_card, priority_weights, correction);
  // Agglomerate
  // FILLING FC TO CC (it is a property of the cc_graph but retrieved through an
  // helper of the agglomerator)
  auto fccc = cc_graph._fc_2_cc;
  for (CoMMAIndexType i_fc = 0; i_fc < nb_fc; i_fc++) {
    fc_to_cc[i_fc] = fccc[i_fc];
  }
}
#undef check_signed_int_type
#endif
