//
// Created by Nicolas Lantos on 31/03/2021.
//

#include "Dual_Graph.h"

Dual_Graph::Dual_Graph(long nb_c,
                       const vector<long>& m_crs_row_ptr,
                       const vector<long>& m_crs_col_ind,
                       const vector<double>& m_crs_values,
                       double* volumes,
                       unordered_map<long, int> &d_is_on_bnd,
                       unordered_set<long> is_on_corner,
                       unordered_set<long> is_on_ridge,
                       unordered_set<long> is_on_valley,
                       unordered_set<long> s_anisotropic_compliant_fc,
                       int verbose,
                       int dim
) : _m_CRS_Row_Ptr(m_crs_row_ptr), _m_CRS_Col_Ind(m_crs_col_ind), _m_CRS_Values(m_crs_values), _volumes(volumes) {
    this->number_of_cells = nb_c;
    this->_dimension = dim;

    if (d_is_on_bnd.size() != 0) {
        int init_bnd_level = -1;
        if (this->_dimension == 2) {
            init_bnd_level = 2;
        } else if (this->_dimension == 3) {
            init_bnd_level = 3;
        } else {
            assert(false);
        }

        this->seeds_pool = new Seeds_Pool(this->number_of_cells,
                                          d_is_on_bnd,
                                          is_on_corner,
                                          is_on_ridge,
                                          is_on_valley,
                                          init_bnd_level = init_bnd_level);
    }

    if (s_anisotropic_compliant_fc.size() > 0) {
        this->s_anisotropic_compliant_cells = s_anisotropic_compliant_fc;
    } else {
        // Default initialization of this->s_anisotropic_compliant_cells
        for (int i = 0; i < this->number_of_cells; i++) {
            this->s_anisotropic_compliant_cells.insert(i);
        }
    }
    this->_verbose = verbose;
}

vector<long> Dual_Graph::get_neighbours(long i_c) {
    long ind = this->_m_CRS_Row_Ptr[i_c];
    long ind_p_one = this->_m_CRS_Row_Ptr[i_c + 1];
    vector<long> result(this->_m_CRS_Col_Ind.begin() + ind, this->_m_CRS_Col_Ind.begin() + ind_p_one);
    return result;

}

vector<double> Dual_Graph::get_weights(long i_c) {
    long ind = this->_m_CRS_Row_Ptr[i_c];
    long ind_p_one = this->_m_CRS_Row_Ptr[i_c + 1];
    vector<double> result(this->_m_CRS_Values.begin() + ind, this->_m_CRS_Values.begin() + ind_p_one);
    return result;

}
