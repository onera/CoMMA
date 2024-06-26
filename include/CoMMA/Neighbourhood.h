#ifndef COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
#define COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H

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

#include <algorithm>
#include <cassert>
#include <deque>
#include <memory>
#include <queue>
#include <set>
#include <unordered_set>
#include <vector>

#include "CoMMA/Util.h"

namespace comma {

/** @brief Class representing the neighbourhood of a given cell in the graph.
 * Mind that no information about the element being already agglomerated or not
 * is known here.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Neighbourhood {
#if 0
Some remarks about the implementation. All the work is done in the function "update",
hence that is where we have to focus on. There are only two constraints for the
returned object: it should be an iterable, and the content (the indices of the neighbours)
must be ordered using their weights. An ordered set of pair of (index, weight) with
CustomPairGreaterFunctor as comparator would not work as because we want unique indices
and that would have been quite hard to achieve even if defining a custom operator==,
have a look here, https://stackoverflow.com/a/1114862
We still use sets, but we have an extra check before inserting whether there already is
a pair with the same index.
#endif

public:
  /** @brief Type of pair */
  using CoMMAPairType = std::pair<CoMMAIndexType, CoMMAWeightType>;
  /** @brief Type of set of pairs */
  using CoMMASetOfPairType =
    std::set<CoMMAPairType, CustomPairGreaterFunctor<CoMMAPairType>>;
  /** @brief Functor used if find-like function relying only on first element of
   * the pair */
  using CoMMAPairFindFirstBasedType = PairFindFirstBasedFunctor<CoMMAPairType>;
  /** @brief Type for container of candidates */
  using CandidatesContainerType = std::deque<CoMMAIndexType>;
  /** @brief Type for containers of indices */
  using ContainerIndexType = std::vector<CoMMAIndexType>;
  /** @brief Type for constant iterators of containers of indices */
  using ContainerIndexConstIt = typename ContainerIndexType::const_iterator;

  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed Set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] weights Weights used to set up the order of the neighbours to
   *visit
   **/
  Neighbourhood(
    const std::unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
    const std::vector<CoMMAWeightType> &weights
  ) :
    _s_neighbours_of_seed(std::move(s_neighbours_of_seed)),
    _weights(weights),
    _s_fc(),
    _candidates() {}

  /** @brief Copy constructor */
  Neighbourhood(
    const Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> &other
  ) = default;

  /** @brief Destructor */
  virtual ~Neighbourhood() = default;

  /** @brief Method that updates the neighbourhood. Given the \p new_fc, if is
   * in the neighbours, it is deleted. Then, the new neighbours are added as
   * candidates
   * @param[in] new_fc Index of the new fine cell to be added to the set of fine
   * cells
   * @param[in] new_neighbours Vector of the new neighbours to be analysed
   */
  inline void update(
    const CoMMAIndexType new_fc, const ContainerIndexType &new_neighbours
  ) {
    update_it(new_fc, new_neighbours.cbegin(), new_neighbours.cend());
  }

  /** @brief Method that updates the neighbourhood. Given the \p new_fc, if is
   * in the neighbours, it is deleted. Then, the new neighbours are added as
   * candidates
   * @param[in] new_fc Index of the new fine cell to be added to the set of fine
   * cells
   * @param[in] neighs_begin Constant iterator giving the beginning of the
   * neighbours
   * @param[in] neighs_end Constant iterator giving the end of the neighbours
   */
  virtual void update_it(
    const CoMMAIndexType new_fc,
    ContainerIndexConstIt neighs_begin,
    ContainerIndexConstIt neighs_end
  ) = 0;

  /** @brief Get candidates that should be consider in the next step of the
   * agglomeration
   *  @return The candidates
   */
  inline const CandidatesContainerType &get_candidates() const {
    return this->_candidates;
  }

protected:
  /** @brief Set of the neighbours of seed given as an input in the constructor.
   * Here, we can find all the neighbours of order up to a user-defined value
   * of the initial seed. Hence, it holds the cells allowed to be agglomerated
   */
  const std::unordered_set<CoMMAIndexType> _s_neighbours_of_seed;

  /** @brief Priority weights */
  const std::vector<CoMMAWeightType> &_weights;

  /** @brief Set of the fine cells composing the coarse cell */
  std::unordered_set<CoMMAIndexType> _s_fc;

  /** @brief Candidates that should be considered when choosing the next fine
   * cell to add to the coarse one
   */
  CandidatesContainerType _candidates;

  /** @brief Extract the indices from a list of index-weight pairs and add them
   * at the back of the candidates list
   */
  inline void extract_and_update_candidates(
    const CoMMASetOfPairType &candidates_w_weights
  ) {
    for (const auto &[idx, w] : candidates_w_weights)
      this->_candidates.push_back(idx);
  }
};

/** @brief Class representing the neighbourhood of a given cell in the graph.
 * In this derived class the neighbourhood is extended, meaning that all the
 * neighbours seen so far are candidates.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Neighbourhood_Extended :
  public Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief Base class */
  using NeighbourhoodBaseType =
    Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Type of pair */
  using typename NeighbourhoodBaseType::CoMMAPairType;
  /** @brief Type of set of pairs */
  using typename NeighbourhoodBaseType::CoMMASetOfPairType;
  /** @brief Functor used if find-like function relying only on first element of
   * the pair */
  using typename NeighbourhoodBaseType::CoMMAPairFindFirstBasedType;
  /** @brief Type for containers of indices */
  using typename NeighbourhoodBaseType::ContainerIndexType;
  /** @brief Type for constant iterators of containers of indices */
  using typename NeighbourhoodBaseType::ContainerIndexConstIt;

  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed Set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] weights Weights used to set up the order of the neighbours to
   *visit
   **/
  Neighbourhood_Extended(
    const std::unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
    const std::vector<CoMMAWeightType> &weights
  ) :
    Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
      s_neighbours_of_seed, weights
    ) {}

  /** @brief Copy constructor */
  Neighbourhood_Extended(
    const Neighbourhood_Extended<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>
      &other
  ) = default;

  /** @brief Destructor */
  ~Neighbourhood_Extended() override = default;

  /** @brief Method that updates the neighbourhood. Given the new_fc, if is in
   * the neighbours, it is deleted. Then, the new neighbours are added as
   * candidates
   * @param[in] new_fc Index of the new fine cell to be added to the set of fine
   * cells
   * @param[in] neighs_begin Constant iterator giving the beginning of the
   * neighbours
   * @param[in] neighs_end Constant iterator giving the end of the neighbours
   */
  void update_it(
    const CoMMAIndexType new_fc,
    ContainerIndexConstIt neighs_begin,
    ContainerIndexConstIt neighs_end
  ) override {
    // Add new_fc to current CC and remove it from previous neighbourhoods
    this->_s_fc.insert(new_fc);
    auto new_fc_ptr =
      std::find(this->_candidates.begin(), this->_candidates.end(), new_fc);
    if (new_fc_ptr != this->_candidates.end())
      this->_candidates.erase(new_fc_ptr);

    // Compute the set of direct neighbours allowed by original
    // neighbourhood-order
    CoMMASetOfPairType neighs;
    for (auto it_fc = neighs_begin; it_fc != neighs_end; ++it_fc) {
      if ((std::find(this->_candidates.begin(), this->_candidates.end(), *it_fc)
           == this->_candidates.end())
          && (this->_s_fc.count(*it_fc) == 0)
          && (this->_s_neighbours_of_seed.count(*it_fc) > 0)) {
        // If not yet in the FON, not yet in the coarse cell and among the
        // allowed neighbours, insert
        neighs.emplace(*it_fc, this->_weights[*it_fc]);
      }
    }
    // Just add new candidates at the back. This will leave candidates closer to
    // the original seed at the top, hence giving them a slightly higher
    // priority
    this->extract_and_update_candidates(neighs);
  }
};

/** @brief Class representing the neighbourhood of a given cell in the graph.
 * In this derived class, the neighbourhood is 'pure front-advancing',
 * meaning that the next candidates are only the direct neighbours of the last
 * added cell.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class Neighbourhood_Pure_Front :
  public Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief Base class */
  using NeighbourhoodBaseType =
    Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Type of pair */
  using typename NeighbourhoodBaseType::CoMMAPairType;
  /** @brief Type of set of pairs */
  using typename NeighbourhoodBaseType::CoMMASetOfPairType;
  /** @brief Functor used if find-like function relying only on first element of
   * the pair */
  using typename NeighbourhoodBaseType::CoMMAPairFindFirstBasedType;
  /** @brief Type for containers of indices */
  using typename NeighbourhoodBaseType::ContainerIndexType;
  /** @brief Type for constant iterators of containers of indices */
  using typename NeighbourhoodBaseType::ContainerIndexConstIt;

  /** @brief Constructor
   *  @param[in] s_neighbours_of_seed Set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] weights Weights used to set up the order of the neighbours to
   *visit
   *  @param[in] dimension Dimension of the problem
   **/
  Neighbourhood_Pure_Front(
    const std::unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
    const std::vector<CoMMAWeightType> &weights,
    CoMMAIntType dimension
  ) :
    Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>(
      s_neighbours_of_seed, weights
    ),
    _q_neighs_w_weights(),
    _dimension(dimension) {}

  /** @brief Copy constructor */
  Neighbourhood_Pure_Front(const Neighbourhood_Pure_Front<
                           CoMMAIndexType,
                           CoMMAWeightType,
                           CoMMAIntType> &other) = default;

  /** @brief Destructor */
  ~Neighbourhood_Pure_Front() override = default;

  /** @brief Method that updates the neighbourhood. Given the new_fc, if is in
   * the neighbours, it is deleted. Then, the new neighbours are considered as
   * candidates
   * @param[in] new_fc Index of the new fine cell to be added to the set of fine
   * cells
   * @param[in] neighs_begin Constant iterator giving the beginning of the
   * neighbours
   * @param[in] neighs_end Constant iterator giving the end of the neighbours
   */
  void update_it(
    const CoMMAIndexType new_fc,
    ContainerIndexConstIt neighs_begin,
    ContainerIndexConstIt neighs_end
  ) override {
    this->_candidates.clear();
    // Add new_fc to current CC and remove it from previous neighbourhoods
    this->_s_fc.insert(new_fc);
    for (auto &queue : this->_q_neighs_w_weights) {
      // There is erase_if for sets in C++20
      // erase_if(queue, [&new_fc](const CoMMAPairType &p){return p.first ==
      // new_fc;});
      auto it = std::find_if(
        queue.begin(), queue.end(), CoMMAPairFindFirstBasedType(new_fc)
      );
      //[&new_fc](const CoMMAPairType &p){return p.first == new_fc;});
      if (it != queue.end()) queue.erase(it);
    }

    // Compute the set of direct neighbours allowed by original
    // neighbourhood-order
    CoMMASetOfPairType curr_set = CoMMASetOfPairType();
    for (auto it_fc = neighs_begin; it_fc != neighs_end; ++it_fc) {
      if ((this->_s_fc.count(*it_fc) == 0)
          && (this->_s_neighbours_of_seed.count(*it_fc) > 0)) {
        // If not yet in coarse cell and among the allowed neighbours, insert
        curr_set.emplace(*it_fc, this->_weights[*it_fc]);
      }
    }

    this->_q_neighs_w_weights.push_front(curr_set);

    // Now, see which neighbours to return. Here is the strategy:
    // If most recent list is not empty, return it. If not, check the oldest
    // list: if not empty return it, otherwise check the previous list. If
    // empty, check the second oldest, and so on... We grant ourselves one
    // exception...
    if (this->_q_neighs_w_weights.size()
        <= static_cast<decltype(this->_q_neighs_w_weights.size())>(
          this->_dimension
        )) {
      // If at the (very) beginning of the agglomeration, still consider every
      // possible neighbour. This will allow to obtain nice quads from quads
      // TODO[RM]: I think this workaround is needed because we are not able to
      // compute exactly the AR; if we ever we will be able we should try to
      // remove it
      for (auto prev_q = this->_q_neighs_w_weights.begin() + 1;
           prev_q != this->_q_neighs_w_weights.end();
           ++prev_q)
        curr_set.insert(prev_q->begin(), prev_q->end());
      this->extract_and_update_candidates(curr_set);
    } else {
      auto cur_front = decltype(this->_q_neighs_w_weights.size()){0};
      auto cur_back = decltype(this->_q_neighs_w_weights.size()
      ){this->_q_neighs_w_weights.size() - 1};
      while (cur_front <= cur_back) {
        // typename decltype(this->_q_neighs_w_weights)::iterator it =
        auto it = this->_q_neighs_w_weights.begin() + (cur_front++);
        if (!it->empty()) {
          this->extract_and_update_candidates(*it);
          break;
        }
        it = this->_q_neighs_w_weights.begin() + (cur_back--);
        if (!it->empty()) {
          this->extract_and_update_candidates(*it);
          break;
        }
      }
    }
  }

  /** @brief Get the neighbours from a previous stage
   *  @param[in] lvl Stage identifier. 0 = current neighbours, 1 = previous
   * neighbours, 2 = second-to-last neighbours, etc...
   *  @return A constant reference to a set of pairs of neighbours and weights
   */
  inline const CoMMASetOfPairType &get_neighbours_by_level(
    const CoMMAIntType lvl
  ) const {
    assert(
      lvl >= 0
      && lvl < static_cast<CoMMAIntType>(this->_q_neighs_w_weights.size())
    );
    return this->_q_neighs_w_weights[lvl];
  }

protected:
  /** @brief History of the first-order-neighbourhoods of the fine cells
   * recently agglomerated */
  std::deque<CoMMASetOfPairType> _q_neighs_w_weights;

  /** @brief Dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3
   * -> 3D)*/
  CoMMAIntType _dimension;
};

/** @brief Pure abstract class for a creator of Neighbourhood objects. It can
 * create from scratch or by copy.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class NeighbourhoodCreator {
public:
  /** @brief Shortcut for the Neighborhood object type */
  using NeighbourhoodBaseType =
    Neighbourhood<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;

  /** @brief Constructor */
  NeighbourhoodCreator() = default;

  /** @brief Destructor */
  virtual ~NeighbourhoodCreator() = default;

  /** @brief Create a new Neighbourhood object from scratch using the given
   * arguments
   *  @param[in] s_neighbours_of_seed Set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] priority_weights Weights used to set up the order of the
   * neighbours to visit
   *  @param[in] dimension Dimension of the problem
   *  @return a shared pointer to a new Neighborhood object
   */
  virtual std::shared_ptr<NeighbourhoodBaseType> create(
    const std::unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
    const std::vector<CoMMAWeightType> &priority_weights,
    const CoMMAIntType dimension
  ) const = 0;

  /** @brief Create a new Neighbourhood object by copy
   *  @param[in] other Shared pointer to an existing Neighbourhood object
   *  @return a shared pointer to a new Neighborhood object
   */
  virtual std::shared_ptr<NeighbourhoodBaseType> clone(
    std::shared_ptr<NeighbourhoodBaseType> other
  ) const = 0;
};

/** @brief Creator of Neighbourhood_Extended objects. It can create
 * from scratch or by copy.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class NeighbourhoodExtendedCreator :
  public NeighbourhoodCreator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief Base class */
  using CreatorBaseType =
    NeighbourhoodCreator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Shortcut for the Neighborhood object base type */
  using typename CreatorBaseType::NeighbourhoodBaseType;
  /** @brief Shortcut for the Neighborhood object derived type (Extended, in
   * this case)
   */
  using NeighbourhoodDerivedType =
    Neighbourhood_Extended<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;

  /** @brief Constructor */
  NeighbourhoodExtendedCreator() :
    CreatorBaseType() {}

  /** @brief Destructor */
  ~NeighbourhoodExtendedCreator() override = default;

  /** @brief Create a new Neighbourhood object from scratch using the given
   * arguments
   *  @param[in] s_neighbours_of_seed Set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] priority_weights Weights used to set up the order of the
   * neighbours to visit
   *  @param[in] dimension Dimension of the problem
   *  @return a shared pointer (base type) to a new Neighborhood (derived type)
   *  object
   */
  inline std::shared_ptr<NeighbourhoodBaseType> create(
    const std::unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
    const std::vector<CoMMAWeightType> &priority_weights,
    const CoMMAIntType dimension
  ) const override {
    CoMMAUnused(dimension);
    return std::make_shared<NeighbourhoodDerivedType>(
      s_neighbours_of_seed, priority_weights
    );
  }

  /** @brief Create a new Neighbourhood object by copy
   *  @param[in] other Shared pointer to an existing Neighbourhood object
   *  @return a shared pointer (base type) to a new Neighborhood (derived type)
   *  object
   */
  inline std::shared_ptr<NeighbourhoodBaseType> clone(
    std::shared_ptr<NeighbourhoodBaseType> other
  ) const override {
    // Using copy-constructor
    return std::make_shared<NeighbourhoodDerivedType>(
      *std::dynamic_pointer_cast<NeighbourhoodDerivedType>(other)
    );
  }
};

/** @brief Creator of Neighbourhood_Extended objects. It can create
 * from scratch or by copy.
 * @tparam CoMMAIndexType the CoMMA index type for the global index of the mesh
 * @tparam CoMMAWeightType the CoMMA weight type for the weights (volume or
 * area) of the nodes or edges of the Mesh
 * @tparam CoMMAIntType the CoMMA type for integers
 */
template<
  typename CoMMAIndexType,
  typename CoMMAWeightType,
  typename CoMMAIntType>
class NeighbourhoodPureFrontCreator :
  public NeighbourhoodCreator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType> {
public:
  /** @brief Base class */
  using CreatorBaseType =
    NeighbourhoodCreator<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;
  /** @brief Shortcut for the Neighborhood object base type */
  using typename CreatorBaseType::NeighbourhoodBaseType;
  /** @brief Shortcut for the Neighborhood object derived type (Pure_Front, in
   * this case)
   */
  using NeighbourhoodDerivedType =
    Neighbourhood_Pure_Front<CoMMAIndexType, CoMMAWeightType, CoMMAIntType>;

  /** @brief Constructor */
  NeighbourhoodPureFrontCreator() :
    CreatorBaseType() {}

  /** @brief Destructor */
  ~NeighbourhoodPureFrontCreator() override = default;

  /** @brief Create a new Neighbourhood object from scratch using the given
   * arguments
   *  @param[in] s_neighbours_of_seed Set of the neighbours of the given cell
   * chosen as seed
   *  @param[in] priority_weights Weights used to set up the order of the
   * neighbours to visit
   *  @param[in] dimension Dimension of the problem
   *  @return a shared pointer (base type) to a new Neighborhood (derived type)
   *  object
   */
  inline std::shared_ptr<NeighbourhoodBaseType> create(
    const std::unordered_set<CoMMAIndexType> &s_neighbours_of_seed,
    const std::vector<CoMMAWeightType> &priority_weights,
    const CoMMAIntType dimension
  ) const override {
    return std::make_shared<NeighbourhoodDerivedType>(
      s_neighbours_of_seed, priority_weights, dimension
    );
  }

  /** @brief Create a new Neighbourhood object by copy
   *  @param[in] other Shared pointer to an existing Neighbourhood object
   *  @return a shared pointer (base type) to a new Neighborhood (derived type)
   *  object
   */
  inline std::shared_ptr<NeighbourhoodBaseType> clone(
    std::shared_ptr<NeighbourhoodBaseType> other
  ) const override {
    // Using copy-constructor
    return std::make_shared<NeighbourhoodDerivedType>(
      *std::dynamic_pointer_cast<NeighbourhoodDerivedType>(other)
    );
  }
};

}  // end namespace comma

#endif  // COMMA_PROJECT_FIRST_ORDER_NEIGHBOURHOOD_H
