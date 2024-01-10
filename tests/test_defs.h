/*
 * CoMMA
 *
 * Copyright © 2023 ONERA
 *
 * Authors: Nicolas Lantos, Alberto Remigi, and Riccardo Milani
 * Contributors: Karim Anemiche
 *
 * Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef COMMA_PROJECT_TESTDEFS_H
#define COMMA_PROJECT_TESTDEFS_H

#include "CoMMATypes.h"
#include "Coarse_Cell_Container.h"
#include "Dual_Graph.h"
#include "Seeds_Pool.h"

constexpr comma::CoMMAIntT SING_CARD_THRESH = 1;

constexpr comma::CoMMAIntT FC_ITER = 1;

constexpr bool ODD_LINE_LENGTH = true;

#define check_(fun, op, cont, obj) \
  fun(cont.begin(), cont.end(), obj) op cont.end()
#define found_(cont, obj) check_(find, !=, cont, obj)
#define not_found_(cont, obj) check_(find, ==, cont, obj)
#define equal_up_to(a, b, eps) (fabs(a - b) < eps)

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
