#ifndef COMMA_PROJECT_UTILS_AGGLOMERATOR_ANISOTROPIC_H
#define COMMA_PROJECT_UTILS_AGGLOMERATOR_ANISOTROPIC_H


#include "Util.h"
#include "Agglomerator.h"
void get_agglo_lines(
		Agglomerator &agg, 
		int level,
                long *sizes,
		vector<long> &agglo_lines_array_idx,
                vector<long> &agglo_lines_array);

#endif
