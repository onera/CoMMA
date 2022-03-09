//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_COARSE_CELL_H
#define COMMA_PROJECT_COARSE_CELL_H

#include "Dual_Graph.h"
#include <memory>

/** @brief Class implementing a Coarse_Cell object.
 * @param[in] fc_graph Dual_Graph object from where are taken the set of fine cells to create the 
 * coarse cell.
 * @param[in] global index of the coarse cell
 * @param[in] s_fc unordered set of fine cells constituting the coarse cell
 * @param[in] is_isotropic boolean describing if the cell is coming from an isotropic agglomeration process
 * or an anisotropic agglomeration process. The default value is set to true.
 * @param[in] is_delayed boolean determing if the agglomeration process is delayed or not. The default value is set
 * to true */

class Coarse_Cell {
public:
    Coarse_Cell(Dual_Graph &fc_graph,
                long i_cc,
                const unordered_set<long> &s_fc,
                bool is_isotropic = true,
                bool is_delayed = false
    );

    ~Coarse_Cell() {
    };
/** @brief Method that return a boolean determing if the Coarse Cell is defined by a connected sub-graph or not.
 *  @return true if the subgraph is connected, false if the subgraph is not connected*/
    bool is_connected();

/** @brief mapping vector. The position of the index is the local node, the value is the global*/
    vector<long> _mapping_g_to_l;
/** @brief The row pointer of the CSR representation of the subgraph*/
    vector<long> _adjMatrix_row_ptr;
/** @brief The column index representation of the CSR representation*/
    vector<long> _adjMatrix_col_ind;
/** @brief The area value of the internal fine cells*/
    vector<double> _adjMatrix_areaValues;
/** @brief The volumes of the internal fine cells*/
    vector<double> _volumes;
/** @brief shared pointer of the subgraph structure (CSR representation)*/
    shared_ptr<Subgraph> _cc_graph;
/** @brief function to build the local CSR subgraph representation
 * @return a vector representing the local to global mapping.*/
    vector<long> build_CRS();
/** @brief The global dual graph*/
    Dual_Graph *__fc_graph;
/** @brief the dimension of the problem (2D or 3D)*/
    short int __dim;
/** @brief Is the cell isotropic or anisotropic*/
    bool __is_isotropic;
/** @brief is delayed the building of the cell or not*/
    bool __is_delayed;
/** @brief is the cell connected*/
    bool __is_connected;
/** @brief the connectivity has been checked*/
    bool __is_connectivity_up_to_date = false;//  # TODO useful for is_connected?
/** @brief Set of fine cells composing the  Coarse cell*/
    unordered_set<long> __s_fc; 
 /** @brief Total volume of the coarse cell */
    double volume = 0.0;  
};


#endif //COMMA_PROJECT_COARSE_CELL_H
