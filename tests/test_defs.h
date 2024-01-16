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

#include "CoMMA/CoMMADefs.h"
#include "CoMMA/Coarse_Cell_Container.h"
#include "CoMMA/Dual_Graph.h"
#include "CoMMA/Seeds_Pool.h"

constexpr comma::CoMMAIntT SING_CARD_THRESH = 1;

constexpr comma::CoMMAIntT FC_ITER = 1;

constexpr bool ODD_LINE_LENGTH = true;

// Some containers have contains, but only in C++20; others has find, others
// neither So we set up one function for all
#define CONTAINS_(cont, obj) \
  (find((cont).begin(), (cont).end(), (obj)) != (cont).end())
#define EQUAL_UP_TO(a, b, eps) (fabs((a) - (b)) < (eps))

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

#endif  // COMMA_PROJECT_TESTDEFS_H
