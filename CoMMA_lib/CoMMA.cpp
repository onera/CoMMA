#include "CoMMATypes.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "CoMMA.h"
namespace py = pybind11;
PYBIND11_MODULE(CoMMA, module_handle) {
  module_handle.doc() = "CoMMA is an agglomeration library";
  module_handle.def("agglomerate_one_level",
  [](const vector<long> adjMatrix_row_ptr,
                           const vector<long> adjMatrix_col_ind,
                           const vector<double> adjMatrix_areaValues,
                           const vector<double> &volumes,

        // Indices of compliant cc
                           vector<long> arrayOfFineAnisotropicCompliantCells,

        // boundaries
                           const vector<long> isOnFineBnd,
                           vector<long> array_is_on_valley,
                           vector<long> array_is_on_ridge,
                           vector<long> array_is_on_corner,

        // Agglomeration argument
                           long isFirstAgglomeration_long,
                           long is_anisotropic_long,


        // Outputs
                           vector<long> fc_to_cc,  // Out

                           vector<long> agglomerationLines_Idx,  // In & out
                           vector<long> agglomerationLines,  // In & out

        //Args with default value
                           long is_basic_or_triconnected,
                           long dimension,
                           long goal_card,
                           long min_card,
                           long max_card,
                           long verbose_long)
      {
          agglomerate_one_level<CoMMAIndexT,CoMMAWeightT>(adjMatrix_row_ptr, adjMatrix_col_ind, adjMatrix_areaValues, volumes,arrayOfFineAnisotropicCompliantCells,isOnFineBnd,array_is_on_valley,array_is_on_ridge,array_is_on_corner,isFirstAgglomeration_long,is_anisotropic_long,fc_to_cc, agglomerationLines_Idx, agglomerationLines,is_basic_or_triconnected,dimension,goal_card,min_card,max_card,verbose_long);
      return std::make_tuple(fc_to_cc, agglomerationLines_Idx, agglomerationLines); 
      });
}

