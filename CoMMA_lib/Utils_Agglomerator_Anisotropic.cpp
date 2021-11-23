#include "Utils_Agglomerator_Anisotropic.h"
void get_agglo_lines(int level,  //TODO Change to short
		                   Agglomerator &agg, 
                                   long *sizes,
                                   vector<long> &agglo_lines_array_idx,
                                   vector<long> &agglo_lines_array) {
    //remark: sizes[2]={nb_agglomeration_lines +1, nb_fc_in_agglomeration_lines}
    assert(level == 0 || level == 1);
    convert_agglo_lines_to_agglomeration_lines_arrays(agg.__v_nb_lines[level],
                                                      agg.__v_lines[level],
                                                      sizes,
                                                      agglo_lines_array_idx,
                                                      agglo_lines_array);
}


