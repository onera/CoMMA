//
// Created by Nicolas Lantos on 02/04/2021.
//

#ifndef COMMA_PROJECT_AGGLOMERATOR_H
#define COMMA_PROJECT_AGGLOMERATOR_H

#include "Dual_Graph.h"
#include "Coarse_Cell_Graph.h"
#include "Util.h"
#include "First_Order_Neighbourhood.h"
#include "Triconnected_graph.h"
#include "Util_graph.h"
    //"""
    // Main Class of the CoMMA library, containing the methods to operate the agglomeration
    // process. It is constructed and the methods are prevalently called from the function agglomerate_one_level in
    // CoMMA.cpp. The constructor require the following parameters:
    //:param Dual_Graph: the struct Dual Graph 
    //:param verbose: [boolean]
    //:param is_visu_data_stored: parameter to store visualization data
    //:param dimension: dimension of the problem
    //"""
class Agglomerator {

   public:
   // Constructor 
   Agglomerator(Dual_Graph &graph,
                 unsigned short int verbose = 0,
                 bool is_visu_data_stored = false,
                 int dimension = 3);
    // Destructor
    ~Agglomerator() {
        if (_cc_graphs != NULL) {
            delete _cc_graphs->_fc_graph.seeds_pool;
            delete _cc_graphs;
        }
    };
    // Set the agglomerator parameters
    virtual void _set_agglomeration_parameter(
            bool is_anisotropic,
            string kind_of_agglomerator = "basic",
            short goal_card = -1,
            short min_card = -1,
            short max_card = -1) = 0;


    protected :
    // Protected member variables, accessible to the derived
    // classes
    // dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3 -> 3D)
    int _dimension;    
    // boolean to define if it is anisotropic or not TODO: check if we can get rid of it
    bool _is_anisotropic = false;
    // string to define the kindo of algorithm used for the agglomerator (basic or triconnected)
    string _kind_of_agglomerator;
    // minimum number of neighborhood
    short _min_neighbourhood = 3;
    // max number of neighborhood
    short int _max_neighbourhood = 6;
    // minimum number of neighborhood for the correction step
    short _min_neighbourhood_correction_step = 1;
    // maximum number of neighborhood for the correction
    short _max_neighbourhood_correction_step = 3;
    // minimum cardinality
    short _min_card = -1;
    // maximum cardinality
    short _max_card = -1;
    // goal cardinality
    short _goal_card = -1;  
    // Threshold cardinality
    short _threshold_card = -1;
    // list of number of cells
    vector<long> _l_nb_of_cells;
    // Fine cells graph (constructed from fc_graph)
    Dual_Graph _fc_graph;
    // Coarse Cell Graph pointer
    Coarse_Cell_Graph *_cc_graph; 
    // Visualization section
    // flag to determine if visualization data are stored
    bool _is_visu_data_stored;
    // flag for the verbose mode
    int _verbose;
}

class Agglomerator_Anisotropic : public Agglomerator{
   // Constructor 
   public:
   Agglomerator_Anisotropic(Dual_Graph &graph,
                 unsigned short int verbose = 0,
                 bool is_visu_data_stored = false,
                 int dimension = 3);
    // Destructor
    ~Agglomerator_Anisotropic() {
        clean_agglomeration_lines(_v_lines[0]);
        clean_agglomeration_lines(_v_lines[1]);
        }
    };
    // Specialization of the pure virtual method in the base class to fill
    // the parameter space

    void _set_agglomeration_parameter(
         bool is_anisotropic,
         string kind_of_agglomerator = "basic",
         short goal_card = -1,
         short min_card = -1,
         short max_card = -1);

    protected:
    // Vector of set of the anisotropic compliant of fine cells
    vector<unordered_set<long>> _v_of_s_anisotropic_compliant_fc;
    // Vector of number of Anisotropic agglomeration lines
    vector<unsigned long> _v_nb_lines;
    // _v_lines : Agglomeration lines structure:
    // vector : level
    // forward list : identifier of the line
    // deque : line cells
    // e.g _v_lines[0] --> agglomeration lines at the finest level
    vector<forward_list<deque<long> *>> _v_lines;  
}

class Agglomerator_Isotropic : Agglomerator{
   // Constructor 
   public:
   Agglomerator_Anisotropic (Dual_Graph &graph,
                 unsigned short int verbose = 0,
                 bool is_visu_data_stored = false,
                 int dimension = 3);
    // Destructor
    ~Agglomerator_Anisotropic() {
        clean_agglomeration_lines(_v_lines[0]);
        clean_agglomeration_lines(_v_lines[1]);
        }
    };
}

#endif //COMMA_PROJECT_AGGLOMERATOR_H
