//
// Created by Nicolas Lantos on 01/06/2021.
//

#include "Util_graph.h"

unordered_map<long, short> compute_Degree(const unordered_set<long> &set_Of_FC, const vector<long> &matrixAdj_CRS_row_ptr, const vector<long> &matrixAdj_CRS_col_ind) {
    // TODO on pourrait stocker l'info selon les degres... ca serait util pour removeLeaf!!!
    /**
     * O(n + m)
     */
    if (set_Of_FC.size() > 1) {
        unordered_map<long, short> dict_FineCell_compactness;
        for (long iFC:set_Of_FC) {
            for (long i = matrixAdj_CRS_row_ptr[iFC]; i < matrixAdj_CRS_row_ptr[iFC + 1]; i++) {
                long indCellN = matrixAdj_CRS_col_ind[i];

                if (set_Of_FC.count(indCellN) && indCellN != iFC) {
                    if (dict_FineCell_compactness.count(iFC)) {
                        dict_FineCell_compactness[iFC] += 1;
                    } else {
                        dict_FineCell_compactness[iFC] = 1;
                    }

                }
            }
        }
        return dict_FineCell_compactness;
    } else {
        unordered_map<long, short> empty_d;
        return empty_d;
    }
}
