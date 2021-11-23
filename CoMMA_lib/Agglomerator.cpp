//
// Created by Nicolas Lantos on 02/04/2021.
//

#include "Agglomerator.h"
#include <stdexcept>

 //"""
    // Main Class of the CoMMA library, containing the methods to operate the agglomeration
    // process. The constructor require the following parameters:
    //:param Dual_Graph: the struct Dual Graph 
    //:param verbose: [boolean]
    //:param is_visu_data_stored: parameter to store visualization data
    //:param dimension: dimension of the problem
    //"""

// How to pass parameters from base class 
// https://stackoverflow.com/questions/9692675/c-constructor-where-parameters-are-used-by-base-class-constructor
// https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-base-class-constructor
// Error handling from https://weseetips.wordpress.com/tag/exception-from-constructor-initializer-list/
namespace CoMMA{


Agglomerator::Agglomerator(Dual_Graph &graph,
                 unsigned short int verbose,
                 bool is_visu_data_stored,
                 int dimension):
                 _fc_graphs(graph), _verbose(verbose), _dimension(dimension), _is_visu_data_stored(is_visu_data_stored), _cc_graphs(new Coarse_Cell_Graph(graph))
{
  if((_dimension != 2) && (_dimension != 3)) {
    throw range_error("dimension can only be 2 or 3");
  }
  if (_dimension == 2) {
        _min_neighbourhood = 2;
  } else {
        _min_neighbourhood = 3;
  }
}


// Anisotropic constructor
// =======================
Agglomerator_Anisotropic::Agglomerator_Anisotropic(Dual_Graph &graph,
                 unsigned short int verbose,
                 bool is_visu_data_stored,
                 int dimension) : Agglomerator(graph,verbose,is_visu_data_stored,dimension)
{
// for every defined level (1 by default), contains the number of cells
// e.g. __l_nb_of_cells[0]= number of cells on finest level
//      __l_nb_of_cells[1]= number of cells on the first coarse level
    _l_nb_of_cells.push_back(graph.number_of_cells);
// For every level, we have a set containing the admissible cells for anisotropy cell number:
// For level 0, it is the cell number of prism or hexahedron ...
    _v_of_s_anisotropic_compliant_fc = {};
    _v_nb_lines = {};
    _v_lines = {};
}

void Agglomerator_Anisotropic::set_agglomeration_parameter(
        string kind_of_agglomerator,
        short goal_card,
        short min_card,
        short max_card) {

     _v_of_s_anisotropic_compliant_fc = vector<unordered_set<long>>(2);
     _v_of_s_anisotropic_compliant_fc[0] = _fc_graphs.s_anisotropic_compliant_cells;
     _v_nb_lines = vector<unsigned long>(2);
     _v_lines = vector<forward_list<deque<long> *> >(2);

    }

    // Definition of (*this).__goal_card
    if (goal_card == -1) {
        __goal_card = d_default_goal_card[__dimension];
    } else {
        __goal_card = goal_card;
    }

    // Definition of (*this).__threshold_card
    __threshold_card = d_default_threshold_card[__dimension];
}





// Isotropic Constructor
// ======================
Agglomerator_Isotropic::Agglomerator_Isotropic(Dual_Graph &graph,
                 unsigned short int verbose,
                 bool is_visu_data_stored,
                 int dimension) : Agglomerator(graph,verbose,is_visu_data_stored,dimension)


void Agglomerator_Isotropic::set_agglomeration_parameter(
        string kind_of_agglomerator,
        short goal_card,
        short min_card,
        short max_card
	){
    _kind_of_agglomerator = kind_of_agglomerator;
    // print("Call of agglomerator {}".format((*this).__kind_of_agglomerator))
    unordered_map<unsigned short int, unsigned short int> d_default_min_card = {{2, 3},
                                                                                {3, 6}};
    unordered_map<unsigned short int, unsigned short int> d_default_max_card = {{2, 5},
                                                                                {3, 10}};
    unordered_map<unsigned short int, unsigned short int> d_default_goal_card = {{2, 4},
                                                                                 {3, 8}};
    unordered_map<unsigned short int, unsigned short int> d_default_threshold_card = {{2, 2},
                                                                                      {3, 3}};

    // Definition of __min_card
    if (min_card == -1) {
        _min_card = d_default_min_card[_dimension];
    } else {
        _min_card = min_card;
    }

    // Definition of (*this).__max_card
    if (max_card == -1) {
        _max_card = d_default_max_card[__dimension];
    } else {
        _max_card = max_card;
    }
}

} //namespace CoMMA
