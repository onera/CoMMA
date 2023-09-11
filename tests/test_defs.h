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
#include "Seeds_Pool.h"
#include "Dual_Graph.h"
#include "Coarse_Cell_Container.h"

constexpr CoMMAIntT SING_CARD_THRESH = 1;

constexpr CoMMAIntT FC_ITER = 1;

constexpr bool ODD_LINE_LENGTH = true;

#define check_(fun, op, cont, obj) fun(cont.begin(), cont.end(), obj) op cont.end()
#define found_(cont, obj) check_(find, !=, cont, obj)
#define not_found_(cont, obj) check_(find, ==, cont, obj)
#define equal_up_to(a,b,eps) (fabs(a - b) < eps)

using SeedsPoolT = Seeds_Pool_Boundary_Priority<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using DualGraphT = Dual_Graph<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;
using CCContainerT = Coarse_Cell_Container<CoMMAIndexT, CoMMAWeightT, CoMMAIntT>;

#endif // COMMA_PROJECT_TESTDEFS_H
