//
// Created by Nicolas Lantos on 06/04/2021.
//

#ifndef COMMA_PROJECT_COMMA_H
#define COMMA_PROJECT_COMMA_H
#include "Agglomerator.h"
#include <climits>

// We define the pointer type but not the structure
void agglomerate_one_level(long (&sizes)[10],
                          const vector<long> &adjMatrix_row_ptr,
                          const vector<long> &adjMatrix_col_ind,
                          const vector<double> &adjMatrix_areaValues,
                          const vector<double> &volumes,

                           vector<long> &arrayOfFineAnisotropicCompliantCells,

                           const vector<long> &isOnFineBnd,
                           vector<long> &array_is_on_valley,
                           vector<long> &array_is_on_ridge,
                           vector<long> &array_is_on_corner,

                           long isFirstAgglomeration,
                           long is_anisotropic_long,

                           vector<long> &fc_to_cc,

                           vector<long> &agglomerationLines_Idx,
                           vector<long> &agglomerationLines,

                           long is_basic_or_triconnected = 0,
                           long dimension = 3,
                           long goal_card = -1,
                           long min_card = -1,
                           long max_card = -3,
                           long checks = 0,
                           long verbose = 0);

#endif //COMMA_PROJECT_COMMA_H
