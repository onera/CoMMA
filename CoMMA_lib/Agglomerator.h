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


// forward definition to keep the agglomerators in one file


class Agglomerator_Anisotropic;
class Agglomerator_Isotropic;

/** @brief A class responsible to do the interface between the different kinds of
 * agglomerator
 *  @author Alberto Remigi and Nicolas Lantos
 */
class Agglomerator {
   public:
   /** @brief The constructor of the interface
    *  @param[in] graph    *Dual Graph* object that determines the connectivity of the matrix 
    *  @param[in] verbose  a boolean to determine if to store the visualization data
    *  @param[in] dimension the dimension of the problem*/
   Agglomerator(Dual_Graph &graph,
                 unsigned short int verbose = 0,
                 bool is_visu_data_stored = false,
                 int dimension = 3);
    /** @brief The destructor of the class */ 
    ~Agglomerator() {
        if (_cc_graph != NULL) {
            delete _cc_graph->_fc_graph.seeds_pool;
            delete _cc_graph;
        }
    };
    /** @brief Pure virtual function where the implementation is specified in the 
     * child classes. The task of the pure virtual function is to set the parameters of
     * dermine the cardinality limits with respect to the parameters passed
     * @param[in] kind_of_agglomeration Parameter that determine the type of agglomeration algorithm used
     * for the *Isotropic* agglomeration algorithm. The choices are between: - basic; -triconnected. Set
     * as default to basic.
     * @param[in] goal_card goal cardinality of the coarse cell (set as default to -1 indicating in our case
     * the maximum value)
     * @param[in] min_card minimum cardinality of the coarse cell(set as default to -1 indicating in our case
     * the maximum value)
     * @param[in] max_card maximum cardinality of the coarse cell(set as default to -1 indicating in our case
     * the maximum value)*/    
    virtual void set_agglomeration_parameter(
            string kind_of_agglomerator = "basic",
            short goal_card = -1,
            short min_card = -1,
            short max_card = -1) = 0;


    protected :
    
    /** @brief dimensionality of the problem (_dimension = 2 -> 2D, _dimension = 3 -> 3D)*/
    int _dimension;    
    /** @brief boolean to define if it is anisotropic or not. It is set as default to false. 
     *  @todo: check if we can get rid of _is_anisotropic variable*/
    bool _is_anisotropic = false;
    /** @brief string to define the kindo of algorithm used for the agglomerator (basic or triconnected)*/
    string _kind_of_agglomerator;
    /** @brief minimum number of neighborhood. Set as default to 3.*/
    short _min_neighbourhood = 3;
    /** @brief max number of neighborhood. Set as default to 6.*/
    short int _max_neighbourhood = 6;
    /** @brief minimum number of neighborhood for the correction step. Set as default to 1.*/
    short _min_neighbourhood_correction_step = 1;
    /** @brief maximum number of neighborhood for the correction. Set as default to 3.*/ 
    short _max_neighbourhood_correction_step = 3;
    /** @brief minimum cardinality. Set as default to -1 (meaning the maximum possible).*/
    short _min_card = -1;
    /** @brief maximum cardinality. Set as default to -1 (meaning the maximum possible)*/
    short _max_card = -1;
    /** @brief Goal cardinality. Set as default to -1 (meaning the maximum possible)*/
    short _goal_card = -1;  
    /** @brief Threshold cardinality. Set as default to -1 (meaning the maximum possible)*/
    short _threshold_card = -1;
    /** @brief List of number of cells per coarse cell created.*/
    vector<long> _l_nb_of_cells;
    /** @brief Dual_Graph object detemining Fine cells graph and hence the connectivity.*/
    Dual_Graph _fc_graph;
    /** @brief pointer to Coarse Cell Graph element */
    Coarse_Cell_Graph *_cc_graph; 
    /** @brief boolean flag to determine if visualization data are stored*/
    bool _is_visu_data_stored;
    /** @brief integer flag for the verbose mode.*/
    int _verbose;
};

/** @brief Agglomerator_Anisotropic class is a child class of the Agglomerator class
 * that specializes the implementation to the case of Anisotropic agglomeration.*/
class Agglomerator_Anisotropic : public Agglomerator{
   // Constructor 
   public:
   /** @brief Constructor. The constructor takes as arguments the same arguments of the father and
    * in this way activates also the constructor of the base class.*/
   Agglomerator_Anisotropic(Dual_Graph &graph,unsigned short int verbose = 0,bool is_visu_data_stored = false,int dimension = 3);
   /** @brief Destructor*/
    ~Agglomerator_Anisotropic();
   /** @brief Specialization of the pure virtual function to the class Agglomerator_Anisotropic. 
    * We add the override key as a guard to possible mistakes:
    * https://stackoverflow.com/questions/46446652/is-there-any-point-in-using-override-when-overriding-a-pure-virtual-function*/
    void set_agglomeration_parameter(
         string kind_of_agglomerator = "basic",
         short goal_card = -1,
         short min_card = -1,
         short max_card = -1) override;

    protected:
    /** @brief Vector of set of the anisotropic compliant of fine cells*/
    vector<unordered_set<long>> _v_of_s_anisotropic_compliant_fc;
    /** @brief Vector of number of Anisotropic agglomeration lines*/
    vector<unsigned long> _v_nb_lines;
    /** @brief _v_lines : Agglomeration lines structure:
    * vector : level
    * forward list : identifier of the line
    * deque : line cells
    * e.g _v_lines[0] --> agglomeration lines at the finest level*/
    vector<forward_list<deque<long> *>> _v_lines;  
    /** @brief Function that returns the vector of agglomeration lines
     *  @param[in] level of the agglomeration process into the Multigrid algorithm
     *  @param[in] *aggl_line_sizes pointer to the vector of sizes for the agglomeration
     *  lines. 
     *  aggl_line_sizes[0] = number of anisotropic lines for the current level
     *  aggl_line_sizes[1] = total number of fine cells in the anisotropic lines found for
     *  the current level.
     *  @param[out] agglo_lines_array_idx Each element points to a particular element in the 
     *  vector agglo_lines_array. This is due to the storing structure.
     *  @param[out] agglo_lines_array Array storing all the element of the anisotropic
     *  lines.
     *  */
    void get_agglo_lines(int level,
		         long *aggl_line_sizes,
                         vector<long> &agglo_lines_array_idx,
                         vector<long> &agglo_lines_array);

};

/** @brief Agglomerator_Isotropic class is a child class of the Agglomerator class
 * that specializes the implementation to the case of Isotropic agglomeration.*/
class Agglomerator_Isotropic : Agglomerator{
   public:
    /** @brief Constructor. The constructor takes as arguments the same arguments of the father and
    * in this way activates also the constructor of the base class.*/
    Agglomerator_Isotropic (Dual_Graph &graph,
                 unsigned short int verbose = 0,
                 bool is_visu_data_stored = false,
                 int dimension = 3); 
    /** @brief Destructor*/
    ~Agglomerator_Isotropic();
   /** @brief Specialization of the pure virtual function to the class Agglomerator_Anisotropic. 
    * We add the override key as a guard to possible mistakes:
    * https://stackoverflow.com/questions/46446652/is-there-any-point-in-using-override-when-overriding-a-pure-virtual-function*/
    void set_agglomeration_parameter(
         string kind_of_agglomerator = "basic",
         short goal_card = -1,
         short min_card = -1,
         short max_card = -1) override;
};

#endif //COMMA_PROJECT_AGGLOMERATOR_H
