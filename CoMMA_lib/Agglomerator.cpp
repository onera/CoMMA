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


Agglomerator::Agglomerator(Dual_Graph &graph,
                 unsigned short int verbose,
                 bool is_visu_data_stored,
                 int dimension):
                 _fc_graph(graph), _verbose(verbose), _dimension(dimension), _is_visu_data_stored(is_visu_data_stored), _cc_graph(new Coarse_Cell_Graph(graph))
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
// e.g. _l_nb_of_cells[0]= number of cells on finest level
//      _l_nb_of_cells[1]= number of cells on the first coarse level
    _l_nb_of_cells.push_back(graph.number_of_cells);
// For every level, we have a set containing the admissible cells for anisotropy cell number:
// For level 0, it is the cell number of prism or hexahedron ...
    _v_of_s_anisotropic_compliant_fc = {};
    _v_nb_lines = {};
    _v_lines = {};
};

void Agglomerator_Anisotropic::set_agglomeration_parameter(
        string kind_of_agglomerator,
        short goal_card,
        short min_card,
        short max_card) {
     _v_of_s_anisotropic_compliant_fc = vector<unordered_set<long>>(2);
     _v_of_s_anisotropic_compliant_fc[0] = _fc_graph.s_anisotropic_compliant_cells;
     _v_nb_lines = vector<unsigned long>(2);
     _v_lines = vector<forward_list<deque<long> *> >(2);
};

/** @todo maybe delete the aggl_lines_sizes here. Not so sure that is useful.
 * Maybe only for statistics.  */
void Agglomerator_Anisotropic::get_agglo_lines(int level,  
                                               long *aggl_lines_sizes,
                                	       vector<long> &agglo_lines_array_idx,
					       vector<long> &agglo_lines_array){
// If at the level of agglomeration "level" the vector containing the number of
// lines is empty, hence it means no line has been found at the current level.
// we define the sizes.
    if (_v_nb_lines[level] == 0) {
        aggl_lines_sizes[0] = 1 + 1;
        aggl_lines_sizes[1] = 0;
    }
// variable cumulating the number of fine cells in the agglomeration lines
// of the current level
    long number_of_fc_in_agglomeration_lines = 0;
    agglo_lines_array_idx[0] = 0;

    long i_l = 0;
//We cycle over the line (in _v_lines)
    for (auto &line :(_v_lines[level])) {
        long size_of_line = (*line).size();
// This vector store the end of a line and the start of a new anisotropic line
// WARNING! We are storing the anisotropic lines in a vector so we need a way to point
// to the end of a line and the starting of a new one.
        agglo_lines_array_idx[i_l + 1] = size_of_line + number_of_fc_in_agglomeration_lines;
// Here we store the index of the cell.
        for (long i = 0; i < size_of_line; i++) {
            agglo_lines_array[i + number_of_fc_in_agglomeration_lines] = (*line)[i];
        }
        number_of_fc_in_agglomeration_lines += size_of_line;
// Next element in the pointer array idx.
       	i_l++;
    }
    aggl_lines_sizes[0] = _v_nb_lines[level] + 1;
    aggl_lines_sizes[1] = number_of_fc_in_agglomeration_lines;
};


void Agglomerator_Anisotropic::agglomerate_one_level(){
// if the finest agglomeration line is not computed, hence compute it (REMEMBER! We compute the agglomeration lines 
// only on the finest level, the other one are stored only for visualization purpose
    if (_v_lines[0].empty()) {
        // The anisotropic lines are only computed on the original (finest) mesh.
        long nb_agglomeration_lines(0);
        _v_lines[0] = _fc_graph.compute_anisotropic_line(nb_agglomeration_lines);  // finest level!!!
        _v_nb_lines[0] = nb_agglomeration_lines;
    }
// In case the if is not realized, this is not the first generation of a coarse level.
// The anisotropic lines are given as input.

    // Copy of the current agglomeration_lines as a backup for visualization purpose.
    _v_lines[1] = copy_agglomeration_lines(_v_lines[0]);
    _v_nb_lines[1] = _v_nb_lines[0];

    // Asserts that __v_lines[1] exists:
    if (!_v_lines[1].empty()) {
	// We creathe the anisotropic coarse cells.
        create_all_anisotropic_cc_wrt_agglomeration_lines();
    }
}

void Agglomerator_Anisotropic::create_all_anisotropic_cc_wrt_agglomeration_lines() {
    // list of set of hexaedric or prismatic cell number (for level 0)
    _v_of_s_anisotropic_compliant_fc[1] = {};
    // Process of every agglomeration lines:
    forward_list<deque<long> *>::iterator fLIt;
    for (fLIt = _v_lines[1].begin(); fLIt != _v_lines[1].end(); fLIt++) {
    // We iterate on the anisotropic lines of a particular level (the level 1, where they were copied from
    // level 0.
    // We create a pointer to an empty deque for the line + 1, and hence for the next level of agglomeration 
        deque<long> *line_lvl_p_one = new deque<long>();
        // We check the line size for the pointed line by the iterator
        long line_size = (**fLIt).size();
        if (line_size <= 1) {
            // the agglomeration_line is empty and hence the iterator points again to the
            // empty deque, updating what is pointed by it and hence __v_lines[1]
            // (each time we iterate on the line, a new deque line_lvl_p_one is defined)
            *fLIt = line_lvl_p_one;
            // we pass to the next line
            continue;
        }

        long i_count = 0;
        bool is_anisotropic = true; // TODO here is necessary for the cc_create_a_cc but maybe we
	// need in some way to change that.
        long i_cc;
        // we agglomerate cells along the agglomeration line, hence we have to
        // go through the faces and agglomerate two faces together, getting to cardinal 2
        // ANISOTROPIC: we agglomerate 2 by 2, hence the anisotropic agglomeration will 
	// provoke agglomerated coarse cells of cardinality 2.
	while (i_count + 2 <= line_size) {
            const long i_fc = (**fLIt)[i_count];
            const long i_fc_p_one = (**fLIt)[i_count + 1];
            unordered_set<long> s_fc = {i_fc, i_fc_p_one};
            // We create the coarse cell
            i_cc = (*_cc_graph).cc_create_a_cc(s_fc, is_anisotropic);
            line_lvl_p_one->push_back(i_cc);
            _v_of_s_anisotropic_compliant_fc[1].insert(i_cc);
            i_count += 2;
        }

        // if i_count < len(line): there is a fc left!
        // i.e. the agglomeration line was of odd size.
        // 2 situations: si la cellule abandonnee est contigue a la zone Euler, c'est OK.
        //               sinon, c'est la merde!
        if (i_count < line_size) {
            // We add this fc to the last coarse element (thus of cardinal 3)
            (*_cc_graph).cc_update_cc({(**fLIt)[i_count]}, i_cc);
        }
        // We free the pointer and we reassign it in order to progress
	delete *fLIt;
        *fLIt = line_lvl_p_one;
    };
}





// Isotropic Constructor
// ======================
Agglomerator_Isotropic::Agglomerator_Isotropic (Dual_Graph &graph,
                 unsigned short int verbose,
                 bool is_visu_data_stored,
		 int dimension) : Agglomerator(graph,verbose,is_visu_data_stored,dimension){
	//no particular constructor
};

void Agglomerator_Isotropic::set_agglomeration_parameter(
        string kind_of_agglomerator,
        short goal_card,
        short min_card,
        short max_card
	){
    _kind_of_agglomerator = kind_of_agglomerator;
    // print("Call of agglomerator {}".format((*this)._kind_of_agglomerator))
    unordered_map<unsigned short int, unsigned short int> d_default_min_card = {{2, 3},
                                                                                {3, 6}};
    unordered_map<unsigned short int, unsigned short int> d_default_max_card = {{2, 5},
                                                                                {3, 10}};
    unordered_map<unsigned short int, unsigned short int> d_default_goal_card = {{2, 4},
                                                                                 {3, 8}};
    unordered_map<unsigned short int, unsigned short int> d_default_threshold_card = {{2, 2},
                                                                                      {3, 3}};

    // Definition of _min_card
    if (min_card == -1) {
        _min_card = d_default_min_card[_dimension];
    } else {
        _min_card = min_card;
    }

    // Definition of (*this)._max_card
    if (max_card == -1) {
        _max_card = d_default_max_card[_dimension];
    } else {
        _max_card = max_card;
    }
    // Definition of (*this)._goal_card
    if (goal_card == -1) {
        _goal_card = d_default_goal_card[_dimension];
    } else {
        _goal_card = goal_card;
    }

    // Definition of (*this)._threshold_card
    _threshold_card = d_default_threshold_card[_dimension];
}

Agglomerator_Biconnected::Agglomerator_Biconnected (Dual_Graph &graph,
                 unsigned short int verbose,
                 bool is_visu_data_stored,
		 int dimension) : Agglomerator_Isotropic(graph,verbose,is_visu_data_stored,dimension){
	//no particular constructor
};
Agglomerator_Triconnected::Agglomerator_Triconnected (Dual_Graph &graph,
                 unsigned short int verbose,
                 bool is_visu_data_stored,
		 int dimension) : Agglomerator_Isotropic(graph,verbose,is_visu_data_stored,dimension){
	//no particular constructor
};

void Agglomerator_Isotropic::agglomerate_one_level(){
    // We define a while for which we control the number of agglomerated cells
    unsigned short compactness = 0;
    long nb_of_fc = _l_nb_of_cells[0];
    while ((*_cc_graph).get_number_of_fc_agglomerated() < nb_of_fc) {
	// 1) Choose a new seed
        long seed = (*_cc_graph).choose_new_seed();
        // 2) Choose the set of Coarse Cells with the specification of the algorithm
	// in the children class (triconnected or biconnected)
	unordered_set<long> set_current_cc = choose_optimal_cc_and_update_seed_pool(seed,compactness);
        // 3)  Creation of cc:
         bool is_anistropic = false; // needed the create_cc of the coarse cell class
         // Check if delay the agglomeration based on compactness written during
	 // the optimal cc choice process
    	 bool is_creation_delayed = compactness < _dimension;
         (*_cc_graph).cc_create_a_cc(set_current_cc, is_anistropic, is_creation_delayed);
    }
    // When we exit from this process all the cell are agglomerated
}

unordered_set<long> Agglomerator_Biconnected::choose_optimal_cc_and_update_seed_pool(const long seed,
		unsigned short &compactness){

  bool is_order_primary = false;
  bool increase_neighbouring = true; }




unordered_set<long> Agglomerator_Triconnected::choose_optimal_cc_and_update_seed_pool(const long seed,
                                                                   unsigned short &compactness){ 
                                                                   bool is_order_primary = false; 
                                                                   bool increase_neighbouring = true; }




