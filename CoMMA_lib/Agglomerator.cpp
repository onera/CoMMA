//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Agglomerator.h"

Agglomerator::Agglomerator(Dual_Graph &graph,
                           int verbose,
                           bool is_visu_data_stored,
                           int dimension,
                           bool checks
): __verbose(verbose), __checks(checks), __dimension(dimension)  {

    if (((*this).__dimension != 2)&&((*this).__dimension != 3))
    {
        cerr<<"Wrong definition of dimension !=2 and !=3"<<endl;
        assert(((*this).__dimension == 2)||((*this).__dimension == 3));
    }

    // for every defined level (1 by default), contains the number of cells
    // e.g. self.__l_nb_of_cells[0]= number of cells on finest level
    // self.__l_nb_of_cells[1]= number of cells on the first coarse level
    (*this).__l_nb_of_cells.push_back(graph.number_of_cells);

    // Anisotropic agglomeration datas:
    // For every level, we have a set containing the admissible cells for anisotropy cell number:
    // For level 0, it is the cell number of prism or hexahedron ...
    (*this).__l_of_s_anisotropic_compliant_fc.push_back(graph.s_anisotropic_compliant_cells);

    // for every defined level (1 by default), contains the associated fine/coarse graph
    // e.g. self.__l_fc_graphs[0]= finest dual graph
    //      self.__l_fc_graphs[1]= first level coarse graph
//    (*this).__l_fc_graphs.push_back(graph);

//    self.__l_cc_graphs: Optional[List[ccg.Coarse_Cell_Graph]] = None
//
//    self.__l_d_cc_to_finest = None
//    self.__finest_c_to_cc: Optional[List[np.ndarray]] = None
//
//    self.__l_primal_meshes: List[Primal_Mesh] = [primal_mesh]
//
//# Visualization:
//    ###########################
//    self.__is_visu_data_stored = is_visu_data_stored
//
//    self.__d_datas_to_visualize_to_delete = None

}