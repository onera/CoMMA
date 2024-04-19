#ifndef COMMA_PROJECT_ARCOMPUTER_H
#define COMMA_PROJECT_ARCOMPUTER_H

/*
 * CoMMA
 *
 * Copyright © 2024 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <cmath>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CoMMA/Dual_Graph.h"
#include "CoMMA/Util.h"

namespace comma {

/** @brief Convenient class containing salient features of a cell. According to
 * to the chosen AR computation (see ARComputer), some features may be outdated
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 */
template<typename IndexT, typename RealT, typename IntT>
class CellFeatures {
public:
  /** Measure of the cell: area in 2D or volume in 3D */
  RealT _measure;

  /** Sum of all external weights (i.e., the area of external faces) */
  RealT _external_weights;

  /** Sum of all internal weights (i.e., the area of internal faces) */
  RealT _internal_weights;

  /** Number of internal faces */
  IntT _n_internal_faces;

  /** Square of the approximation of the minimum edge of the cell, that is, the
   * minimum distance between two cell centers.
   */
  RealT _sq_min_edge;

  /** Square of the approximation of the diameter of the cell, that is, the
   * maximum distance between two cell centers.
   */
  RealT _sq_diam;

  /** Sum of the coordinates of the centers weighted by the measures, useful to
   * compute barycenter */
  std::vector<RealT> _sum_centers;

  /** Number of external facets (not shared with other cells) by cell */
  std::unordered_map<IndexT, IntT> _external_facets;

  /** @brief Constructor.
   * @param[in] measure Measure of the cell
   * @param[in] external_weights External weights, i.e., the area of external
   * faces
   * @param[in] internal_weights Internal weights, i.e., the area of internal
   * faces
   * @param[in] n_internal_faces Number of internal faces
   * @param[in] min_edge (Approximation of) The minimum edge of the cell
   * @param[in] diam (Approximation of) The diameter of the cell
   * @param[in] sum_centers Sum of the coordinates of the centers weighted by
   * the mesures, useful to compute barycenter
   * @param[in] external_facets Number of external facets (not shared with other
   * cells) by cell
   * @note All the arguments are optional and defaults are available.
   */
  explicit CellFeatures(
    const RealT measure = std::numeric_limits<RealT>::min(),
    const RealT external_weights = 0.0,
    const RealT internal_weights = 0.0,
    const IntT n_internal_faces = 0,
    const RealT min_edge = std::numeric_limits<RealT>::max(),
    const RealT diam = std::numeric_limits<RealT>::min(),
    const std::vector<RealT> sum_centers = std::vector<RealT>{},
    const std::unordered_map<IndexT, IntT> external_facets =
      std::unordered_map<IndexT, IntT>{}
  ) :
    _measure(measure),
    _external_weights(external_weights),
    _internal_weights(internal_weights),
    _n_internal_faces(n_internal_faces),
    _sq_min_edge(min_edge * min_edge),
    _sq_diam(diam * diam),
    _sum_centers(sum_centers),
    _external_facets(external_facets){};

  /** @brief Constructor
   */
  CellFeatures(
    const IndexT index, std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph
  ) :
    _measure(graph->_volumes[index]),
    _external_weights(graph->estimated_total_weight(index)),
    _internal_weights(0.0),
    _n_internal_faces(0),
    _sq_min_edge(std::numeric_limits<RealT>::max()),
    _sq_diam(std::numeric_limits<RealT>::min()),
    _external_facets({{index, graph->get_total_n_faces(index)}}) {
    _sum_centers = graph->_centers[index];
    for (auto &xyz : _sum_centers) {
      xyz *= graph->_volumes[index];
    }
  }

  /** @brief Copy constructor
   * @param[in] other Features to move
   */
  CellFeatures(const CellFeatures<IndexT, RealT, IntT> &other) = default;

  /** @brief Move constructor
   * @param[in] other Features to move
   */
  CellFeatures(CellFeatures<IndexT, RealT, IntT> &&other) noexcept = default;

  /** @brief Move assignment
   * @param[in] other Features to move
   */
  CellFeatures &operator=(CellFeatures<IndexT, RealT, IntT> &&other
  ) noexcept = default;

  /** @brief Copy assignment
   * @param[in] other Features to copy
   */
  CellFeatures &operator=(const CellFeatures<IndexT, RealT, IntT> &other
  ) = default;

  /** @brief Compute the radius of a cell.
   * @tparam dim dimension of the problem (e.g., 1-, 2-, 3D,...)
   */
  template<unsigned int dim>
  inline RealT constexpr get_radius() const {
    if constexpr (dim < 2) {
      return _measure;
    } else if constexpr (dim == 2) {
      return sqrt(_measure);
    } else if constexpr (dim == 3) {
      return cbrt(_measure);
    } else {
      return pow(_measure, 1.0 / dim);
    }
  }
};

/** @brief Similar to a functor, the key point is the method that computes the
 * AR and update the features of a cell. Here, AR is used lightly, and might be
 * any quantity which should be minimized in the coarsening process. This is an
 * abstract class.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 */
template<typename IndexT, typename RealT, typename IntT>
class ARComputer {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   */
  explicit ARComputer(std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph) :
    _graph(graph){};

  /** @brief The destructor of the class */
  virtual ~ARComputer() = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  virtual void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const = 0;

protected:
  /** @brief Dual_Graph object determining Fine cells graph and hence the
   * connectivity
   */
  std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> _graph;

  /** @brief Computes number of shared faces and features of the CC obtained by
   * adding a given fine cell. The features are Aspect-Ratio and number of face
   * shared with other cells already agglomerated (Current coarse cell means
   * without \p i_fc)
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_weights Sum of the weights of the shared faces
   * @param[out] shared_faces Faces shared by the fine cell with the coarse one
   */
  inline void compute_shared_faces(
    const IndexT i_fc,
    const std::unordered_set<IndexT> &fc_of_cc,
    RealT &shared_weights,
    std::unordered_set<IndexT> &shared_faces
  ) const {
    shared_weights = 0.;
    auto n_it = this->_graph->neighbours_cbegin(i_fc);
    auto w_it = this->_graph->weights_cbegin(i_fc);
    for (; n_it != this->_graph->neighbours_cend(i_fc); ++n_it, ++w_it) {
      if (*n_it != i_fc && (fc_of_cc.count(*n_it) != 0)) {
        shared_faces.insert(*n_it);
        shared_weights += *w_it;
      }
    }
  }

  /** @brief Compute approximated geometric features (e.g., diameter)
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  inline void update_approximated_geometric_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const {
    const std::vector<RealT> &cen_fc = this->_graph->_centers[i_fc];
    RealT max_diam = cc_feats._sq_diam, min_edge = cc_feats._sq_min_edge;
    for (const auto i_fc_cc : fc_of_cc) {
      const auto dist = squared_euclidean_distance<RealT>(
        cen_fc, this->_graph->_centers[i_fc_cc]
      );
      if (dist > max_diam) max_diam = dist;
      if (dist < min_edge) min_edge = dist;
    }  // for i_fc_cc
    new_feats._sq_diam = max_diam;
    new_feats._sq_min_edge = min_edge;
  }

  /** @brief Compute the barycenter and update the features accordingly
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   * @return the barycenter
   */
  inline std::vector<RealT> compute_and_update_barycenter(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const {
    new_feats._sum_centers = cc_feats._sum_centers;
    const auto &cur_c = this->_graph->_centers[i_fc];
    std::vector<RealT> bary(cur_c.size());
    const RealT ov_N = 1. / (fc_of_cc.size() + 1);
    for (auto i = decltype(cur_c.size()){0}; i < cur_c.size(); ++i) {
      new_feats._sum_centers[i] += this->_graph->_volumes[i_fc] * cur_c[i];
      bary[i] = new_feats._sum_centers[i] * ov_N;
    }
    return bary;
  }

  /** @brief Compute approximated geometric features (e.g., diameter)
   * @tparam compute_weights Whether algebraic features should be computed
   * (e.g., weights)
   * @tparam update_facets Whether to update external facets
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] n_shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  template<bool compute_weights = false, bool update_facets = false>
  inline void update_basic_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    IntT &n_shared_faces,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const {
    new_feats._measure = cc_feats._measure + this->_graph->_volumes[i_fc];
    RealT shared_weights{};
    std::unordered_set<IndexT> shared_faces{};
    this->compute_shared_faces(i_fc, fc_of_cc, shared_weights, shared_faces);
    n_shared_faces = static_cast<IntT>(shared_faces.size());
    new_feats._n_internal_faces = cc_feats._n_internal_faces + n_shared_faces;
    if constexpr (compute_weights) {
      new_feats._external_weights = cc_feats._external_weights
        + this->_graph->estimated_total_weight(i_fc) - 2 * shared_weights;
      new_feats._internal_weights = cc_feats._internal_weights + shared_weights;
    }
    if constexpr (update_facets) {
      new_feats._external_facets = cc_feats._external_facets;  // OK copy
      for (const auto &face : shared_faces) {
        new_feats._external_facets.at(face)--;
      }
      new_feats._external_facets[i_fc] =
        this->_graph->get_total_n_faces(i_fc) - n_shared_faces;
    }
  }

  /** @brief Given a squared quantity and cell features, compute the ratio
   * between the quantity and the radius of the cell (that is
   * \f$ \sqrt[dim](vol(CC)) \f$), or an approximation of it where the
   * numerator and denominator are raised to certain powers (instead of taking
   * roots) to ensure non-dimensionality.
   * @param[in] sqx Square of the quantity used as numerator
   * @param[in] feats Features of the cell
   * @return \f$ sqx^{dim} / vol(CC)^2 \f$
   */
  template<unsigned int dim>
  inline RealT constexpr x_over_radius(
    const RealT sqx, const CellFeatures<IndexT, RealT, IntT> &feats
  ) const {
    if constexpr (dim < 2) {
      return sqrt(sqx) / feats.template get_radius<dim>();
    } else if constexpr (dim == 2) {
      return sqx / feats._measure;
    } else if constexpr (dim == 3) {
      return _cb(sqx) / _sq(feats._measure);
    } else {
      return int_power<dim>(sqx) / _sq(feats._measure);
    }
  }
};

/** @brief ARComputer. Here, AR is the ratio of the diameter over the
 * estimated one (typically, in 2D, the squared root of the surface).
 * It is the definition used in CoMMA from 1.1 up to 1.3.2.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 * @tparam dim dimension of the problem (e.g., 1-, 2-, 3D,...)
 */
template<typename IndexT, typename RealT, typename IntT, unsigned int dim>
class ARDiamOverRadius : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   */
  explicit ARDiamOverRadius(
    std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph
  ) :
    ARComputer<IndexT, RealT, IntT>(graph){};

  /** @brief Destructor*/
  ~ARDiamOverRadius() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    this->update_approximated_geometric_features(
      i_fc, cc_feats, fc_of_cc, new_feats
    );
    // Compute AR
    aspect_ratio =
      this->template x_over_radius<dim>(new_feats._sq_diam, new_feats);
  }
};

/** @brief ARComputer. Here, AR is the reciprocal of the measure, hence the
 * optimal solution should be the one with the max measure.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 */
template<typename IndexT, typename RealT, typename IntT>
class AROverMeasure : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   */
  explicit AROverMeasure(std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph
  ) :
    ARComputer<IndexT, RealT, IntT>(graph){};

  /** @brief Destructor*/
  ~AROverMeasure() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    // Compute AR
    aspect_ratio = 1. / new_feats._measure;
  }
};

/** @brief ARComputer. Here, AR is the ratio of the diameter over the smallest
 * edge.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 */
template<typename IndexT, typename RealT, typename IntT>
class ARDiamOverMinEdge : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   */
  explicit ARDiamOverMinEdge(
    std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph
  ) :
    ARComputer<IndexT, RealT, IntT>(graph){};

  /** @brief Destructor*/
  ~ARDiamOverMinEdge() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    this->update_approximated_geometric_features(
      i_fc, cc_feats, fc_of_cc, new_feats
    );
    // Compute AR
    // If only 2 cells (hence the reference coarse cell is only one cell), max
    // and min are the same, hence use only max
    aspect_ratio = fc_of_cc.size() == 1
      ? new_feats._sq_diam
      : new_feats._sq_diam / new_feats._sq_min_edge;
  }
};

/** @brief ARComputer. Here, AR is the ratio of the external weights over the
 * measure. With \p dim equal to 2, it is roughly equivalent to the ration of
 * the perimeter and the surface of the cell. Moreover, with \p dim = 2, it is
 * equivalent to the definition used by CoMMA up to version 1.0. For an
 * algebraic version (where no concern is given to adimensionalize the AR), use
 * \p dim equal to 1.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 * @tparam dim dimension of the problem (e.g., 1-, 2-, 3D,...)
 */
template<typename IndexT, typename RealT, typename IntT, unsigned int dim>
class ARExternalWeightOverRadius : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   */
  explicit ARExternalWeightOverRadius(
    std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph
  ) :
    ARComputer<IndexT, RealT, IntT>(graph){};

  /** @brief Destructor*/
  ~ARExternalWeightOverRadius() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<true>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    // Compute AR
    aspect_ratio = this->compute_AR(new_feats);
  }

protected:
  /** @brief Compute the aspect-ratio.
   * @param[in] feats Features of the current coarse cell
   * @return \f$ \frac{ExtWeights_{CC}}{\sqrt[dim]{vol_{CC}}} \f$ or a similar
   * ratio where numerator and denominator are raised to certain powers
   * (instead of their roots) as to ensure non-dimensionality
   */
  inline RealT constexpr compute_AR(
    const CellFeatures<IndexT, RealT, IntT> &feats
  ) const {
    if constexpr (dim < 2) {
      return feats._external_weights / feats.template get_radius<dim>();
    } else if constexpr (dim == 2) {
      // return feats._external_weights / feats.template get_radius<dim>();
      return _sq(feats._external_weights) / feats._measure;
    } else if constexpr (dim == 3) {
      // return sqrt(feats._external_weights) / feats.template
      // get_radius<dim>();
      return _cb(feats._external_weights) / _sq(feats._measure);
    } else {
      // return pow(feats._external_weights, 1.0 / (dim - 1))
      // / feats.template get_radius<dim>();
      return int_power<dim>(feats._external_weights)
        / int_power<dim - 1>(feats._measure);
    }
  }
};

/** @brief ARComputer. Here, AR is the approximated diameter.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 */
template<typename IndexT, typename RealT, typename IntT>
class ARDiameter : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   */
  explicit ARDiameter(std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph) :
    ARComputer<IndexT, RealT, IntT>(graph){};

  /** @brief Destructor*/
  ~ARDiameter() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    this->update_approximated_geometric_features(
      i_fc, cc_feats, fc_of_cc, new_feats
    );
    // Compute AR
    aspect_ratio = new_feats._sq_diam;
  }
};

/** @brief ARComputer. Here, AR is the total external weights (that is, from a
 * geometric point of view, the perimeter). Looking for the minimum leads in
 * graph terms to a min-cut.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 */
template<typename IndexT, typename RealT, typename IntT>
class ARExternalWeights : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   */
  explicit ARExternalWeights(
    std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph
  ) :
    ARComputer<IndexT, RealT, IntT>(graph){};

  /** @brief Destructor*/
  ~ARExternalWeights() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<true>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    // Compute AR
    aspect_ratio = new_feats._external_weights;
  }
};

/** @brief ARComputer. Here, AR is one over the internal weights (looking for
 * the minimum leads to the maximization of the internal weights).
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 */
template<typename IndexT, typename RealT, typename IntT>
class AROverInternalWeights : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   */
  explicit AROverInternalWeights(
    std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph
  ) :
    ARComputer<IndexT, RealT, IntT>(graph){};

  /** @brief Destructor*/
  ~AROverInternalWeights() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<true>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    // Compute AR
    aspect_ratio = 1. / new_feats._internal_weights;
  }
};

/** @brief ARComputer. Here, AR is the ratio of the maximum over minimum
 * distance of the cell centers from the barycenter (computed as weighted
 * average of centers). If a cell is totally internal, it won't be included in
 * the computations.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 * @tparam dim dimension of the problem (e.g., 1-, 2-, 3D,...)
 */
template<typename IndexT, typename RealT, typename IntT, unsigned int dim>
class ARMaxBaryDistanceOverRadius : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   * null
   */
  explicit ARMaxBaryDistanceOverRadius(
    std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph
  ) :
    ARComputer<IndexT, RealT, IntT>(graph){};

  /** @brief Destructor*/
  ~ARMaxBaryDistanceOverRadius() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   * @note The returned AR might not exactly the above-mentioned ration but a
   * similar one where numerator and denominator are raised to certain powers
   * (instead of their roots) as to ensure non-dimensionality
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    const auto bary =
      this->compute_and_update_barycenter(i_fc, cc_feats, fc_of_cc, new_feats);
    // Compute AR
    const auto dist_fc =
      squared_euclidean_distance<RealT>(bary, this->_graph->_centers[i_fc]);
    RealT max_dist = dist_fc;
    for (const auto i_fc_cc : fc_of_cc) {
      const auto dist = squared_euclidean_distance<RealT>(
        bary, this->_graph->_centers[i_fc_cc]
      );
      if (dist > max_dist) max_dist = dist;
    }  // for i_fc_cc
    aspect_ratio = this->template x_over_radius<dim>(max_dist, new_feats);
  }
};

/** @brief ARComputer. Here, AR is the ratio of the maximum over minimum
 * distance of the cell centers from the barycenter (computed as weighted
 * average of centers). If a cell is totally internal, it won't be included in
 * the computations.
 * @tparam IndexT type used for indices
 * @tparam RealT type used for features
 * @tparam IntT type used for integers (e.g., number of cells, faces,...)
 */
template<typename IndexT, typename RealT, typename IntT>
class ARMaxOverMinBaryDistance : public ARComputer<IndexT, RealT, IntT> {
public:
  /** @brief Constructor
   * @param[in] graph Dual_Graph object that determines the connectivity
   * @param[in] tolerance Threshold below which the distances are considered
   * null
   */
  explicit ARMaxOverMinBaryDistance(
    std::shared_ptr<Dual_Graph<IndexT, RealT, IntT>> graph,
    const RealT tolerance
  ) :
    ARComputer<IndexT, RealT, IntT>(graph), _tolerance(tolerance){};

  /** @brief Destructor*/
  ~ARMaxOverMinBaryDistance() override = default;

  /** @brief Computes features of the CC obtained by adding a given fine cell.
   * The features are Aspect-Ratio, number of face shared with other cells
   * already agglomerated (Current coarse cell means without \p i_fc) and those
   * of CellFeatures.
   * @param[in] i_fc Index of the fine cell to add to the coarse cell
   * @param[in] cc_feats Features of the current coarse cell
   * @param[in] fc_of_cc Index of the fine cells already agglomerated in the
   * coarse cell
   * @param[out] shared_faces Number of faces shared by the fine cell with the
   * current coarse cell
   * @param[out] aspect_ratio Aspect-Ratio of the (final) coarse cell
   * @param[out] new_feats Features of the (final) coarse cell
   */
  void compute_and_update_features(
    const IndexT i_fc,
    const CellFeatures<IndexT, RealT, IntT> &cc_feats,
    const std::unordered_set<IndexT> &fc_of_cc,
    // out
    IntT &shared_faces,
    RealT &aspect_ratio,
    CellFeatures<IndexT, RealT, IntT> &new_feats
  ) const override {
    // Update
    this->template update_basic_features<false, true>(
      i_fc, cc_feats, fc_of_cc, shared_faces, new_feats
    );
    const auto bary =
      this->compute_and_update_barycenter(i_fc, cc_feats, fc_of_cc, new_feats);
    // Compute AR
    RealT min_dist = std::numeric_limits<RealT>::max(),
          max_dist = std::numeric_limits<RealT>::min();
    // Skipping if totally internal cell
    if (new_feats._external_facets[i_fc] > 0) {
      // Since we are comparing to a threshold, keep sqrt here
      const auto dist = sqrt(
        squared_euclidean_distance<RealT>(bary, this->_graph->_centers[i_fc])
      );
      // Skipping if barycenter is the center of the cell
      if (dist > this->_tolerance) {
        min_dist = max_dist = dist;
      }
    }
    for (const auto i_fc_cc : fc_of_cc) {
      if (new_feats._external_facets[i_fc_cc] > 0) {
        const auto dist = sqrt(squared_euclidean_distance<RealT>(
          bary, this->_graph->_centers[i_fc_cc]
        ));
        if (dist > this->_tolerance) {
          if (dist > max_dist) max_dist = dist;
          if (dist < min_dist) min_dist = dist;
        }
      }
    }  // for i_fc_cc
    aspect_ratio = max_dist / min_dist;
  }

protected:
  /** Threshold below which the distances are considered null */
  RealT _tolerance;
};

}  // end namespace comma

#endif  // COMMA_PROJECT_ARCOMPUTER_H
