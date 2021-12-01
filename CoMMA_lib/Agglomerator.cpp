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
                bool increase_neighbouring = true; 
    //  The goal of this function is to choose from a pool of neighbour the better one to build a compact coarse cell
    assert(_goal_card != -1);  // _goal_card has been initialized
    // Definition of the current cc
    // remark: does contain seed
    unordered_set<long> s_current_cc = {seed};
    unordered_map<long, unsigned short> d_n_of_seed;
   // Number of fine cells constituting the current coarse cell in construction.
    unsigned short size_current_cc = 1; // CC contains only one cell: the seed

    // set of fc for current cc:

    unordered_set<long> s_seeds = {seed};

    // Compute _min_neighbourhood order neighbourhood:
    //=====================================================
    unsigned short max_order_of_neighbourhood = _min_neighbourhood;
    _fc_graph.compute_neighbourhood_of_cc(s_seeds,
                                            max_order_of_neighbourhood,   //in and out
                                            d_n_of_seed, //output
                                            _max_card,
                                            (*_cc_graph)._a_is_fc_agglomerated);

    unsigned short nb_neighbours = _fc_graph.get_nb_of_neighbours(seed);
    vector<double> a_neighbours_weights = _fc_graph.get_weights(seed);

    // If no neighbour is found for seed: this case happened only when isotropic cell is surrounded
    // by anisotropic cells.
    if (d_n_of_seed.empty()) {
        // d_n_of_seed is empty, i.e: the cc is not complete
        // and no neighbour are available!
        compactness = 0;
    }
    // The available neighborhood cells are not enought to reach the goal cardinality

    else if ((d_n_of_seed.size() + s_current_cc.size()) < _goal_card) {
        // Not enough available neighbour, the dictionary
	// of the available cells size summed with the current
	// cell size is not enough to reach the goal cardinality
	// : creation of a (too small) coarse cell.
        // We add the cells of the dictionary to the set of current coarse
	// cell.    
       for (auto &i_k_v : d_n_of_seed) {
            s_current_cc.insert(i_k_v.first);
       }
       // We check as a consequence the threshold cardinality that is a minimum limit
       bool is_creation_delayed = (s_current_cc.size() <= _threshold_card);
       if (is_creation_delayed) {
             compactness = 0;
       } else {
             compactness = _dimension; //minimum number of nrighborhood of a connected cell TODO: CHECK THAT, it is not
	     // better to be sustituted with number of neighborhood?
       }
    }
    else{
     // If  we passed the two previous checks, the minimum size is the goal cardinality required TODO : CHECK THAT, if the goal is 2, the minimum size would be 3?
     unsigned short min_size = _goal_card;
    // Computation of the initial aspect ratio: we need cc_surf: i.e. the external area (perimeter in 2D and sum of external faces in 3D) and volume
    double cc_surf = 0.0;
   // the weights are the area of the neightborhood faces of a cell
    for (const double &w:a_neighbours_weights) {
        cc_surf += w;
    }
    // volume of cc is at first the volume of the seed.
    double vol_cc = _fc_graph._volumes[seed];
    // This dictionary is used to store the eligible cc: i.e. its size is inside the permitted range.
    // This is useful to track back our step if needed.
    // [size of the current, [cell set, [arg_min_ar, n.neigh of seed]]
    unordered_map<unsigned short, pair<unordered_set<long>, unordered_map<long, int>>> dict_cc_in_creation;
    double min_external_faces = numeric_limits<double>::max();
    unsigned short arg_min_external_faces = min_size;
    // Here we define the exact dimension of the coarse cell as the min between the max cardinality and the dictionary of the boundary cells
    unsigned short max_ind = min(_max_card, (short) (d_n_of_seed.size() + 1));
    int number_of_external_faces_current_cc = nb_neighbours + (*_fc_graph.seeds_pool).boundary_value(seed) - 1;
    // d_keys_to_set from Util.h
    unordered_set<long> s_neighbours_of_seed = d_keys_to_set(d_n_of_seed);

    First_Order_Neighbourhood f_o_neighbourhood = First_Order_Neighbourhood(s_neighbours_of_seed);

    vector<long> v = _fc_graph.get_neighbours(seed);
    unordered_set<long> s_up(v.begin(), v.end());

    unordered_set<long> fon = f_o_neighbourhood.update(seed, s_up);

    // Choice of the fine cells to agglomerate
    while (size_current_cc < max_ind) {

        long argmin_ar = -1;
        double min_ar_surf = numeric_limits<double>::max();
        double min_ar_vol = numeric_limits<double>::max();
        unsigned short max_faces_in_common = 0;

        compute_best_fc_to_add(_fc_graph,
                               fon,
                               d_n_of_seed,
                               is_order_primary,
                               cc_surf,
                               vol_cc,
                               s_current_cc,
                               argmin_ar, //output
                               max_faces_in_common,
                               min_ar_surf,
                               min_ar_vol);

        number_of_external_faces_current_cc += _fc_graph.get_nb_of_neighbours(argmin_ar)
                                               + (*_fc_graph.seeds_pool).boundary_value(argmin_ar) - 1 - 2 * max_faces_in_common;

        size_current_cc++;
        s_current_cc.insert(argmin_ar);

        // if the constructed cc is eligible, i.e. its size is inside the permitted range
        // we store it inside dict_cc_in_creation
        // This choice is based on the idea that the smallest cc (w.r.t. card) is may be not the one that minimized
        // the number of external faces.
        // if this if is satisfied it means we have an eligible cell 
        if ((min_size <= size_current_cc) || size_current_cc == max_ind) {

            if (number_of_external_faces_current_cc <= min_external_faces) {

                min_external_faces = number_of_external_faces_current_cc;
                arg_min_external_faces = size_current_cc;
            }

            unordered_map<long, int> new_dict;
            new_dict[argmin_ar] = d_n_of_seed[argmin_ar];

            pair<unordered_set<long>, unordered_map<long, int>> p = make_pair(s_current_cc, new_dict);
            dict_cc_in_creation[size_current_cc] = p;

        }

        // Update of cc_surf and vol_cc with the new fc added
        cc_surf = min_ar_surf;
        vol_cc = min_ar_vol;

        // Remove added fc from the available neighbours
        d_n_of_seed.erase(argmin_ar);

        vector<long> v = _fc_graph.get_neighbours(argmin_ar);
        unordered_set<long> s_up(v.begin(), v.end());
        fon = f_o_neighbourhood.update(argmin_ar, s_up);
    }
    s_current_cc = dict_cc_in_creation[arg_min_external_faces].first;

    // If we do not chose the biggest cc, we put the useless fc back to the pool
    for (long i_s = arg_min_external_faces + 1; i_s < max_ind + 1; i_s++) {
        // for all size of Cell from arg_min_external_faces+1 to  min(max_card, len(d_n_of_seed) + 1) + 1
        //d_n_of_seed.
        //            update(dict_cc_in_creation[i_s][1])
        // Merge/update:
        for (auto iKV:dict_cc_in_creation[i_s].second) {
            d_n_of_seed[iKV.first] = iKV.second;
        }


        // print("arg_min_external_faces + 1", arg_min_external_faces + 1, "max_ind + 1", max_ind + 1)
        // raise ValueError
    }
    assert(arg_min_external_faces == s_current_cc.size());
    // Computes the actual compactness of the coarse cell
    compactness = _fc_graph.compute_min_fc_compactness_inside_a_cc(s_current_cc);
    } //end else
    // Create of l_of_new_seeds:
//    list<long> l_of_new_seeds //= create_list_of_seeds(*_cc_graphs, seed, d_n_of_seed, s_current_cc);
    // Update of list_of_seeds:
//    (*_fc_graph.seeds_pool).update(l_of_new_seeds);
    return s_current_cc;
}

void Agglomerator_Biconnected::compute_best_fc_to_add(Dual_Graph &graph,
                                            unordered_set<long> fon,  //First order neighborhood
                                            const unordered_map<long, unsigned short> &d_n_of_seed,
                                            const bool &is_order_primary,
                                            const double &cc_surf,
                                            const double &vol_cc,
                                            const unordered_set<long> &s_of_fc_for_current_cc,
                                            long &argmin_ar,
                                            unsigned short &max_faces_in_common,
                                            double &min_ar_surf,
					    double &min_ar_vol){
//	//Nothing
}



unordered_set<long> Agglomerator_Triconnected::choose_optimal_cc_and_update_seed_pool(const long seed,
                                                                   unsigned short &compactness){ 
                                                                   bool is_order_primary = false; 
                                                                   bool increase_neighbouring = true; }

