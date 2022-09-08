#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "CoMMA.h"

#include "CoMMATypes.h"

namespace py = pybind11;
PYBIND11_MODULE(CoMMA, module_handle) {
  module_handle.doc() = "CoMMA is an agglomeration library";
  module_handle.def("agglomerate_one_level",
                    [](const vector<long> adjMatrix_row_ptr,
                       const vector<long> adjMatrix_col_ind,
                       const vector<double> adjMatrix_areaValues,
                       const vector<double> volumes,

                       // Indices of compliant cc
                       vector<long> arrayOfFineAnisotropicCompliantCells,

                       // boundaries
                       const vector<long> isOnFineBnd,

                       // Agglomeration argument
                       bool isFirstAgglomeration, bool is_anisotropic,
                       double threshold_anisotropy,

                       // Outputs
                       vector<long> fc_to_cc,                // Out
                       vector<long> agglomerationLines_Idx,  // In & out
                       vector<long> agglomerationLines,      // In & out

                       // Args with default value
                       bool correction, short dimension, short goal_card,
                       short min_card, short max_card) {
    agglomerate_one_level<CoMMAIndexT, CoMMAWeightT>(
        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        arrayOfFineAnisotropicCompliantCells, isOnFineBnd, isFirstAgglomeration,
        is_anisotropic, threshold_anisotropy, fc_to_cc, agglomerationLines_Idx,
        agglomerationLines, correction, dimension, goal_card, min_card,
        max_card);
    return std::make_tuple(fc_to_cc, agglomerationLines_Idx,
                           agglomerationLines);
  });
}
