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

#ifndef TRACE_CATEGORIES_H
#define TRACE_CATEGORIES_H

#include <perfetto.h>

// The set of track event categories that the example is using.
PERFETTO_DEFINE_CATEGORIES(
  perfetto::Category("setup").SetDescription("Rendering and graphics events"),
  perfetto::Category("agglomerator").SetDescription("Agglomerator Events"));
#endif  // TRACE_CATEGORIES_H
