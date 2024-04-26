#ifndef COMMA_PROJECT_TESTDEFS_H
#define COMMA_PROJECT_TESTDEFS_H

/*
 * CoMMA
 *
 * Copyright © 2024 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "CoMMA/CoMMADefs.h"
#include "CoMMA/Coarse_Cell_Container.h"
#include "CoMMA/Dual_Graph.h"
#include "CoMMA/Seeds_Pool.h"
#include "CoMMA/Util.h"

constexpr comma::CoMMAIntT SING_CARD_THRESH = 1;

constexpr comma::CoMMAIntT FC_ITER = 1;

constexpr bool ODD_LINE_LENGTH = true;

constexpr comma::CoMMAAspectRatioT DEFAULT_AR =
  comma::CoMMAAspectRatioT::DIAMETER_OVER_RADIUS;

constexpr comma::CoMMAWeightT eps = 1e-10;

using SeedsPoolT = comma::Seeds_Pool_Boundary_Priority<
  comma::CoMMAIndexT,
  comma::CoMMAWeightT,
  comma::CoMMAIntT>;
using DualGraphT =
  comma::Dual_Graph<comma::CoMMAIndexT, comma::CoMMAWeightT, comma::CoMMAIntT>;
using CCContainerT = comma::Coarse_Cell_Container<
  comma::CoMMAIndexT,
  comma::CoMMAWeightT,
  comma::CoMMAIntT>;

// Coarse Cell
using CCT = std::vector<comma::CoMMAIndexT>;
// Coarse Cell, with optional
using OptIndT = std::optional<comma::CoMMAIndexT>;
using CCOptT = std::vector<OptIndT>;

// See
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#writing-custom-matchers-old-style
class BelongsToCCMatcher : public Catch::Matchers::MatcherBase<OptIndT> {
  typename OptIndT::value_type _cc;

public:
  explicit BelongsToCCMatcher(const typename OptIndT::value_type &cc) :
    _cc(cc) {}

  bool match(OptIndT const &fc) const override {
    return fc.has_value() && fc.value() == _cc;
  }

  std::string describe() const override {
    std::ostringstream ss;
    ss << "belongs to Coarse Cell " << _cc;
    return ss.str();
  }
};

inline BelongsToCCMatcher BelongsToCC(const typename OptIndT::value_type &cc) {
  return BelongsToCCMatcher(cc);
}

struct Contains1stElemMatcher : Catch::Matchers::MatcherGenericBase {
  using PairT = std::pair<comma::CoMMAIndexT, comma::CoMMAWeightT>;
  using PairFindFirstBasedT = comma::PairFindFirstBasedFunctor<PairT>;

  explicit Contains1stElemMatcher(comma::CoMMAIndexT const &target) :
    target{target} {}

  template<typename Range>
  bool match(Range const &range) const {
    using std::begin;
    using std::end;

    return std::find_if(begin(range), end(range), PairFindFirstBasedT(target))
      != end(range);
  }

  std::string describe() const override {
    std::ostringstream ss;
    ss << "contains " << target << " as first element of one of its pairs";
    return ss.str();
  }

private:
  comma::CoMMAIndexT target;
};

inline Contains1stElemMatcher Contains1stElem(comma::CoMMAIndexT const &target
) {
  return Contains1stElemMatcher(target);
}

#endif  // COMMA_PROJECT_TESTDEFS_H
