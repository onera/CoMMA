#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "CoMMA.h"

#include "CoMMATypes.h"

namespace py = pybind11;
PYBIND11_MODULE(CoMMA, module_handle) {
  module_handle.doc() = "CoMMA is an agglomeration library";
  module_handle.def("agglomerate_one_level",
                    [](const vector<CoMMAIndexT> adjMatrix_row_ptr,
                       const vector<CoMMAIndexT> adjMatrix_col_ind,
                       const vector<CoMMAWeightT> adjMatrix_areaValues,
                       const vector<CoMMAWeightT> volumes,

                       // Indices of compliant cc
                       vector<CoMMAIndexT> arrayOfFineAnisotropicCompliantCells,

                       // boundaries
                       const vector<CoMMAIndexT> isOnFineBnd,

                       // Agglomeration argument
                       bool isFirstAgglomeration, bool is_anisotropic,

                       // Outputs
                       vector<CoMMAIndexT> fc_to_cc,                // Out
                       vector<CoMMAIndexT> agglomerationLines_Idx,  // In & out
                       vector<CoMMAIndexT> agglomerationLines,      // In & out

                       // Args with default value
                       bool correction, short dimension, short goal_card,
                       short min_card, short max_card) {
    agglomerate_one_level<CoMMAIndexT, CoMMAWeightT>(
        adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,
        arrayOfFineAnisotropicCompliantCells, isOnFineBnd, isFirstAgglomeration,
        is_anisotropic, fc_to_cc, agglomerationLines_Idx, agglomerationLines,
        correction, dimension, goal_card, min_card, max_card);
    return std::make_tuple(fc_to_cc, agglomerationLines_Idx,
                           agglomerationLines);
  });
}
