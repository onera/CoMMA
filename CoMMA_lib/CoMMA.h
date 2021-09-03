//
// Created by Nicolas Lantos on 06/04/2021.
//

#ifndef COMMA_PROJECT_COMMA_H
#define COMMA_PROJECT_COMMA_H

#include "Agglomerator.h"
#include <climits>

// We define the pointer type but not the structure
void agglomerate_one_level(long *sizes,
                           long *adjMatrix_row_ptr,
                           long *adjMatrix_col_ind,
                           double *adjMatrix_areaValues,
                           double *volumes,

                           long *arrayOfFineAnisotropicCompliantCells,

                           long *isOnFineBnd,
                           long *array_is_on_valley,
                           long *array_is_on_ridge,
                           long *array_is_on_corner,

                           long isFirstAgglomeration,
                           long is_anisotropic_long,

                           long *fc_to_cc,

                           long *agglomerationLines_Idx,
                           long *agglomerationLines,

                           long is_basic_or_triconnected = 0,
                           long dimension = 3,
                           long goal_card = -1,
                           long min_card = -1,
                           long max_card = -3,
                           long checks = 0,
                           long verbose = 0);

#endif //COMMA_PROJECT_COMMA_H
