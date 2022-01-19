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
                 Coarse_Cell_Graph &cc_graph,
        	 int verbose,
                 bool is_visu_data_stored,
                 int dimension):
                 _fc_graph(graph), _verbose(verbose), _dimension(dimension), _is_visu_data_stored(is_visu_data_stored), _cc_graph(&cc_graph)
{
  if((_dimension != 2) && (_dimension != 3)) {
    throw range_error("dimension can only be 2 or 3");
  }
  if (_dimension == 2) {
        _min_neighbourhood = 2;
  } else {
        _min_neighbourhood = 3;
  }
  _l_nb_of_cells.push_back(graph.number_of_cells);
}


// Anisotropic constructor
// =======================
Agglomerator_Anisotropic::Agglomerator_Anisotropic(Dual_Graph &graph,
		 Coarse_Cell_Graph &cc_graph,
                 int verbose,
                 bool is_visu_data_stored,
                 int dimension) : Agglomerator(graph,cc_graph,verbose,is_visu_data_stored,dimension)
{
// for every defined level (1 by default), contains the number of cells
// e.g. _l_nb_of_cells[0]= number of cells on finest level
//      _l_nb_of_cells[1]= number of cells on the first coarse level
    _l_nb_of_cells.push_back(graph.number_of_cells);
// For every level, we have a set containing the admissible cells for anisotropy cell number:
// For level 0, it is the cell number of prism or hexahedron ...
     _v_of_s_anisotropic_compliant_fc = vector<unordered_set<long>>(2);
     _v_of_s_anisotropic_compliant_fc[0] = _fc_graph.s_anisotropic_compliant_cells;
     _v_nb_lines = vector<long>(2);
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

void Agglomerator_Anisotropic::agglomerate_one_level(const short goal_card,
                                         const short min_card,
                                         const short max_card,
					 int correction_steps){
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
	// We create the anisotropic coarse cells.
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
		 Coarse_Cell_Graph &cc_graph,
                 int verbose,
                 bool is_visu_data_stored,
		 int dimension) : Agglomerator(graph,cc_graph,verbose,is_visu_data_stored,dimension){
	//no particular constructor
};

void Agglomerator_Isotropic::set_agglomeration_parameter(
        short goal_card,
        short min_card,
        short max_card
	){
    unordered_map<int, int> d_default_min_card = {{2, 3},
                                                                                {3, 6}};
    unordered_map<int, int> d_default_max_card = {{2, 5},
                                                                                {3, 10}};
    unordered_map<int, int> d_default_goal_card = {{2, 4},
                                                                                 {3, 8}};
    unordered_map<int, int> d_default_threshold_card = {{2, 2},
                                                                                      {3, 3}};

    // Definition of _min_card
    if (min_card == -1) {
        _min_card = d_default_min_card[_dimension];
    } else {
        _min_card = min_card;
    }

    // Definition of _max_card
    if (max_card == -1) {
        _max_card = d_default_max_card[_dimension];
    } else {
        _max_card = max_card;
    }
    // Definition of _goal_card
    if (goal_card == -1) {
        _goal_card = d_default_goal_card[_dimension];
    } else {
        _goal_card = goal_card;
    }

    // Definition of _threshold_card
    _threshold_card = d_default_threshold_card[_dimension];
}

Agglomerator_Biconnected::Agglomerator_Biconnected (Dual_Graph &graph,
		 Coarse_Cell_Graph &cc_graph,
                 int verbose,
                 bool is_visu_data_stored,
		 int dimension) : Agglomerator_Isotropic(graph,cc_graph,verbose,is_visu_data_stored,dimension){
	//no particular constructor
};
Agglomerator_Triconnected::Agglomerator_Triconnected (Dual_Graph &graph,
		Coarse_Cell_Graph &cc_graph,
                  int verbose,
                 bool is_visu_data_stored,
		 int dimension) : Agglomerator_Isotropic(graph,cc_graph,verbose,is_visu_data_stored,dimension){
	//no particular constructor
};

void Agglomerator_Isotropic::agglomerate_one_level(const short goal_card,
                                         const short min_card,
                                         const short max_card,
					 int correction_steps){
    set_agglomeration_parameter(goal_card,min_card,max_card);
    // We define a while for which we control the number of agglomerated cells
    short compactness = 0; 
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
    // When we exit from this process all the cell are agglomerated, apart the delayed one
    // We proceed in creating the delayed one
    (*_cc_graph).cc_create_all_delayed_cc();
    (*_cc_graph).fill_cc_neighbouring();
    if ((*_cc_graph).is_cc_grid_not_structured(_goal_card)){
      correction(correction_steps,4);
    }
    (*_cc_graph).cc_renumber();
    _l_nb_of_cells.push_back((*_cc_graph)._cc_counter);
}

unordered_set<long> Agglomerator_Biconnected::choose_optimal_cc_and_update_seed_pool(const long seed,
		short &compactness){
    bool is_order_primary = false;
    bool increase_neighbouring = true; 
    //  The goal of this function is to choose from a pool of neighbour the better one to build a compact coarse cell
    assert(_goal_card != -1);  // _goal_card has been initialized
    // OUTPUT: Definition of the current cc, IT WILL BE GIVEN AS AN OUTPUT
    unordered_set<long> s_current_cc = {seed};
    // Dictionary of the neighborhood of the seed, the key is the global index of the cell and the value
    // the order of distance from the seed (1 order direct neighborhood, 2 order etc.)
    unordered_map<long, short> d_n_of_seed;
   // Number of fine cells constituting the current coarse cell in construction.
    short size_current_cc = 1; // CC contains only one cell: the seed
    short max_order_of_neighbourhood = _min_neighbourhood; // set to 3 as default we set to this value the maximum order to which we search to compose the coarse cell
    // We fill the d_n_of_seeds considerng the initial seed passed
    _fc_graph.compute_neighbourhood_of_cc(s_current_cc,
                                            max_order_of_neighbourhood,   //in and out
                                            d_n_of_seed, //output, the function fills the dictionary
                                            _max_card,
                                            (*_cc_graph)._a_is_fc_agglomerated); // anisotropic cells agglomerated (not to take into account in the process)
    // We get the number of neighborhoods
    short nb_neighbours = _fc_graph.get_nb_of_neighbours(seed);
    // return the area of the face connected to the seed
    vector<double> neighbours_weights = _fc_graph.get_weights(seed);

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
             compactness = 0; //Return
       } else {
             compactness = _dimension; //minimum number of nrighborhood of a connected cell TODO: CHECK THAT, it is not
	     // better to be sustituted with number of neighborhood?
	     //
       }
    }
    else{
     // If  we passed the two previous checks, the minimum size is the goal cardinality required TODO : CHECK THAT, if the goal is 2, the minimum size would be 3? 
    // ARGUABLE! Let's think to 3
    short min_size = _goal_card;
    // Computation of the initial aspect ratio: we need cc_surf: i.e. the external area (perimeter in 2D and sum of external faces in 3D) and volume
    double cc_surf = 0.0;
   // the weights are the area of the neightborhood faces of a cell
    for (const double &w:neighbours_weights) {
        cc_surf += w;
    }
    // volume of cc is at first the volume of the seed.
    double vol_cc = _fc_graph._volumes[seed];
    // This dictionary is used to store the eligible cc: i.e. its size is inside the permitted range.
    // This is useful to track back our step if needed.
    // [size of the current, [cell set, d_n_of seed]]
    unordered_map<short, pair<unordered_set<long>, unordered_map<long, int>>> dict_cc_in_creation;
    double min_external_faces = numeric_limits<double>::max();
    short arg_min_external_faces = min_size;
    // Here we define the exact dimension of the coarse cell as the min between the max cardinality given as an input
    // (remember the constructor choice in case of -1) and the dictionary of the boundary cells, it means the total number of 
    // neighborhood cells until the order we have given (as default 3, so until the third order)
    short max_ind = min(_max_card, (short) (d_n_of_seed.size() + 1));
    // We add the faces that are on boundary calling the method of seed pool.
    int number_of_external_faces_current_cc = nb_neighbours + (*_fc_graph.seeds_pool).boundary_value(seed) - 1;
    // d_keys_to_set from Util.h, it takes the keys of the unordered map and create an unordered set. The unordered
    // set is representing hence all the neighborood of seed until a given order.
    unordered_set<long> s_neighbours_of_seed = d_keys_to_set(d_n_of_seed);
    // Build the class first order neighborhood
    First_Order_Neighbourhood f_o_neighbourhood = First_Order_Neighbourhood(s_neighbours_of_seed);
    // Retrives the neighborhood of the seed
   vector<long> v = _fc_graph.get_neighbours(seed);
    unordered_set<long> s_up(v.begin(), v.end());
    // Generate the set of the first order neighborhood to the given seed
    unordered_set<long> fon = f_o_neighbourhood.update(seed, s_up);

    // Choice of the fine cells to agglomerate
    // we enter in a while, we store anyways all the possible coarse cells 
    // (not only the max dimension one)
    while (size_current_cc < max_ind) {
        // argmin_ar is the best fine cell to add
        long argmin_ar = -1;
        double min_ar_surf = numeric_limits<double>::max();
        double min_ar_vol = numeric_limits<double>::max();
        short max_faces_in_common = 0;
    // We compute the best fine cell to add, based on the aspect
    // ratio and is given back in argmin_ar. It takes account also
    // the fine cells that has been added until now.
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
        // we increase the cc 
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

            // We update the dictionary of eligible coarse cells
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
    }
    assert(arg_min_external_faces == s_current_cc.size());
    // Computes the actual compactness of the coarse cell
    compactness = _fc_graph.compute_min_fc_compactness_inside_a_cc(s_current_cc);
    } //end else
    // Update seeds
    // Create of l_of_new_seed:
    list<long> l_of_new_seed;
    if (!d_n_of_seed.empty()) {

        short size = d_n_of_seed.size();
        // l_of_new_seed.resize(size);
        // if d_n_of_seed is not empty
        // Reminder: d_n_of_seed is here the pool of cell neighbouring the previous seed!
        for (auto &i_k_v : d_n_of_seed) {
            if (i_k_v.second <= 2) {
                l_of_new_seed.push_back(i_k_v.first);
            }
        }
        // if list of new seeds is still empty we go to order 3
        short i_k = 3;
        while (l_of_new_seed.empty()) {
            // We put FC in the l_of_new_seed according to its order of neighbouring of previous seed.
            // order 1 or 2 then 3, then 4 and so on.
            for (auto &i_k_v : d_n_of_seed) {
                if (i_k_v.second <= i_k) {
                    l_of_new_seed.push_back(i_k_v.first);
                }
            }
            i_k++;
        }
    } else {
        // else d_n_of_seed is empty: we used every neighbour!
        // if list_of_seeds is empty, we look for new seeds to add to the list_of_seeds.
        if ((*_fc_graph.seeds_pool).is_empty()) {
            // if list_of_seeds is empty
            // we look if there is some neighbour to the current fc:
            // s_fc_neighbours_of_cc = set()
            // we remove seed because we already update its first neighbours.
            unordered_set<long> tmp_set(s_current_cc);  // copy needed because the set is used inside ccg
            tmp_set.erase(seed);

            // We add to s_fc_neighbours_of_cc all the neighbours of FC included in s_fc_for_current_cc without seed
            for (auto &i_fc: tmp_set) {
                vector<long> a_neighbours = _fc_graph.get_neighbours(i_fc);
                for (const long &i_fc_n: a_neighbours) {
                    if ((*_cc_graph).is_fc_not_already_agglomerated(i_fc_n)) {
                        // _a_is_fc_agglomerated is up-to-date.
                        // remark: we don't care i_fc_n == i_fc.
                        l_of_new_seed.push_back(i_fc_n);
                    }
                }
            }
        }
    }
    // Update of list_of_seeds:
    (*_fc_graph.seeds_pool).update(l_of_new_seed);
    return s_current_cc;
}

void Agglomerator_Biconnected::compute_best_fc_to_add(Dual_Graph &graph,
                                            unordered_set<long> fon,  //First order neighborhood
                                            const unordered_map<long, short> &d_n_of_seed,
                                            const bool &is_order_primary,
                                            const double &cc_surf,
                                            const double &vol_cc,
                                            const unordered_set<long> &s_of_fc_for_current_cc,
                                            long &argmin_ar,
                                            short &max_faces_in_common,
                                            double &min_ar_surf,
					    double &min_ar_vol){
    //  this function defines the best fine cells to add to create the coarse
    // cell for the current coarse cell considered
    double min_ar = numeric_limits<double>::max();
    long arg_max_faces_in_common = -1;

    // For every fc in the neighbourhood:
    // we update the new aspect ratio
    // we verify that fon is a sub member of the dict of seeds
    for (const long &i_fc: fon) {  // we test every possible new cells to chose the one that locally
        // minimizes the Aspect Ratio at the first fine cell of the fon.
        if (arg_max_faces_in_common == -1) {
            arg_max_faces_in_common = i_fc;
        }

        // update of the vol
        double new_ar_vol = vol_cc + graph._volumes[i_fc];

        short number_faces_in_common = 0;
        bool is_fc_adjacent_to_any_cell_of_the_cc = false;
        double new_ar_surf = cc_surf;
        // New Aspect Ratio of the tested neighborhood
        vector<long> v_neighbours = graph.get_neighbours(i_fc);
        vector<double> v_weights = graph.get_weights(i_fc);
        assert(v_neighbours.size() == v_weights.size());

    for (int i_n = 0; i_n < v_neighbours.size(); i_n++) {

        long i_fc_n = v_neighbours[i_n];
        double i_w_fc_n = v_weights[i_n];

        if (i_fc_n == i_fc) {  // Boundary surface
            new_ar_surf += i_w_fc_n;
        } else if (s_of_fc_for_current_cc.count(i_fc_n) == 0) {
            new_ar_surf += i_w_fc_n;
        } else {
            is_fc_adjacent_to_any_cell_of_the_cc = true;
            new_ar_surf -= i_w_fc_n;
            number_faces_in_common++;
        }
    }
       // end new AR

        double new_ar = pow(new_ar_surf, 1.5) / new_ar_vol;

        const short &order = d_n_of_seed.at(i_fc); // [i_fc] is not const the method at returns the reference to the value of the key i_fc.

// TODO This version seems good but refactorisation to do: perhaps it is not needed to update every new possible coarse cell aspect ratio?
// TODO also need to remove the list of min_ar, argmin_ar, etc.
        if (number_faces_in_common >= max_faces_in_common or is_order_primary) {  // if is_order_primary is True the order of
// neighbourhood is primary
            if (number_faces_in_common == max_faces_in_common or is_order_primary) {

                if (order <= d_n_of_seed.at(arg_max_faces_in_common)) {  // [arg_max_faces_in_common] is not const.
                    if (order == d_n_of_seed.at(arg_max_faces_in_common)) {
                        if (new_ar < min_ar and is_fc_adjacent_to_any_cell_of_the_cc) {
// The second condition asserts the connectivity of the coarse element.
                            min_ar = new_ar;
                            argmin_ar = i_fc;
                            min_ar_surf = new_ar_surf;
                            min_ar_vol = new_ar_vol;

                            arg_max_faces_in_common = i_fc;
// The number of face in common is the same no need to touch it
                        }
                    } else {
// Case :number_faces_in_common == max_faces_in_common and order < dict_neighbours_of_seed[arg_max_faces_in_common]:
                        arg_max_faces_in_common = i_fc;
                        min_ar = new_ar;
                        argmin_ar = i_fc;
                        min_ar_surf = new_ar_surf;
                        min_ar_vol = new_ar_vol;
// The number of face in common is the same no need to touch it
                    }
                }
            } else {
// Case :number_faces_in_common > max_faces_in_common:
                max_faces_in_common = number_faces_in_common;
                arg_max_faces_in_common = i_fc;
                min_ar = new_ar;
                argmin_ar = i_fc;
                min_ar_surf = new_ar_surf;
                min_ar_vol = new_ar_vol;
            }
        }
    }


}

void Agglomerator_Triconnected::correction(int correction_steps, int num_iterations){
  // We iterate the corrections 
}

template<>
void Agglomerator_Biconnected::operation_correction<1>(int num_iterations){
	for (int i = 0; i<num_iterations; i++){
			(*_cc_graph).correction_remove_too_small_cc(_threshold_card);
	}
}
template<>
void Agglomerator_Biconnected::operation_correction<2>(int num_iterations){
	for (int i = 0; i<num_iterations; i++){
			(*_cc_graph).correction_remove_too_small_cc(_threshold_card);
			(*_cc_graph).correction_make_small_cc_bigger(_threshold_card,_min_card,_goal_card,_verbose);
	}
}
template<>
void Agglomerator_Biconnected::operation_correction<3>(int num_iterations){
	for (int i = 0; i<num_iterations; i++){
			(*_cc_graph).correction_remove_too_small_cc(_threshold_card);
			(*_cc_graph).correction_make_small_cc_bigger(_threshold_card,_min_card,_goal_card,_verbose);
			(*_cc_graph).correction_swap_leaf_fc_v2();
	}
}
template<>
void Agglomerator_Biconnected::operation_correction<4>(int num_iterations){
	for (int i = 0; i<num_iterations; i++){
			(*_cc_graph).correction_remove_too_small_cc(_threshold_card);
			(*_cc_graph).correction_make_small_cc_bigger(_threshold_card,_min_card,_goal_card,_verbose);
			(*_cc_graph).correction_swap_leaf_fc_v2();
			(*_cc_graph).correction_reduce_too_big_cc(_goal_card, _verbose);
	}
}
template<>
void Agglomerator_Biconnected::operation_correction<5>(int num_iterations){
	for (int i = 0; i<num_iterations; i++){
			(*_cc_graph).correction_remove_too_small_cc(_threshold_card);
			(*_cc_graph).correction_make_small_cc_bigger(_threshold_card,_min_card,_goal_card,_verbose);
			(*_cc_graph).correction_swap_leaf_fc_v2();
			(*_cc_graph).correction_reduce_too_big_cc(_goal_card, _verbose);
			(*_cc_graph).correction_remove_too_small_cc(_threshold_card);
	}
}





void Agglomerator_Biconnected::correction(int correction_steps, int num_iterations){
	if (correction_steps == 0){
               // if correction steps = 0 no correction is happening
	} else if (correction_steps==1){
		operation_correction<1>(num_iterations);
	} else if (correction_steps==2){
		operation_correction<2>(num_iterations);
	} else if (correction_steps==3){
		operation_correction<3>(num_iterations);
	} else if (correction_steps==4){
		operation_correction<4>(num_iterations);
	} else if (correction_steps==5 || correction_steps==-1){
		operation_correction<5>(num_iterations);
	}
	else{cout<<"Value out of range\n";}
	// TODO reimplement  _correction_split_too_big_cc_in_two();
}



unordered_set<long> Agglomerator_Triconnected::choose_optimal_cc_and_update_seed_pool(const long seed,
                                                                   short &compactness){ 
                                                                   bool is_order_primary = false; 
                                                                   bool increase_neighbouring = true; }

