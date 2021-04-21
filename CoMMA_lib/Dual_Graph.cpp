//
// Created by Nicolas Lantos on 31/03/2021.
//

#include "Dual_Graph.h"

template<typename A, typename B>
std::pair<B, A> flip_pair(const pair<A, B> &p) {
    return pair<B, A>(p.second, p.first);
};

template<typename A, typename B, template<class, class, class...> class M, class... Args>
multimap<B, A> flip_map(const M<A, B, Args...> &src) {
    multimap<B, A> dst;
    transform(src.begin(), src.end(), inserter(dst, dst.begin()), flip_pair<A, B>);
    return dst;
};

Dual_Graph::Dual_Graph(long nb_c,
                       const vector<long> &m_crs_row_ptr,
                       const vector<long> &m_crs_col_ind,
                       const vector<double> &m_crs_values,
                       const vector<double> &volumes,
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

bool Dual_Graph::check_connectivity(unordered_set<long> s_fc,
                                    int verbose) {

//"""
//Checks connectivity of the coarse cell
//        :param listFineCells: List of fine cells defining the coarse element.
//:param matrixAdj_CRS_row_ptr: dual graph information see MgridGen data structure
//        :param matrixAdj_CRS_col_ind: dual graph information see MgridGen data structure
//        :return: True or False
//"""
    unordered_map<long, bool> map_isAlreadyConnected;
    int size = s_fc.size();
    if (size <= 1) {
        return true;
    }
    for (long iFC :s_fc) {
        map_isAlreadyConnected[iFC] = false;
    }
    long front = *s_fc.begin();
    map_isAlreadyConnected[front] = true;

    unordered_set<long> setNext({front});
    int nbConnectedCells = 1;

    while (nbConnectedCells < size) {

        if (!setNext.empty()) {
            long iFineCell = *setNext.begin();
            setNext.erase(setNext.begin());
            long ind = this->_m_CRS_Row_Ptr[iFineCell];  // Usefull to find neighbours of seed
            long ind_p_one = this->_m_CRS_Row_Ptr[iFineCell + 1]; // Usefull to find neighbours of seed
            for (long iFC = ind; iFC < ind_p_one; iFC++) {
                long iFCellNeighbour = this->_m_CRS_Col_Ind[iFC];
                if (map_isAlreadyConnected.count(iFCellNeighbour) == 1) {
                    if ((iFCellNeighbour != iFineCell) && (!map_isAlreadyConnected[iFCellNeighbour])) {
                        setNext.insert(iFCellNeighbour);
                        nbConnectedCells += 1;
                        map_isAlreadyConnected[iFCellNeighbour] = true;
                    }
                }
            }
        } else {
            break;
        }
    }
    if ((verbose) && (nbConnectedCells != size)) {
        cout << "s_fc " << endl;
        int iCount = 0;
        for (auto i : s_fc) {
            cout << i << " : " << map_isAlreadyConnected[i] << endl;
            iCount++;
        }

        cout << "nbConnectedCells " << nbConnectedCells << " size " << size << endl;
    }

    return nbConnectedCells == size;

}

int Dual_Graph::get_nb_of_neighbours(long i_c) {
    return this->_m_CRS_Row_Ptr[i_c + 1] - this->_m_CRS_Row_Ptr[i_c];
}


bool Dual_Graph::compute_anisotropic_line(long *sizes,
                                          long *array_of_anisotropic_compliant_fc,
                                          long *agglomerationLines_Idx,
                                          long *agglomerationLines,
                                          bool verbose) {

    // Rmk: costly function: sort of a dictionary!
    long nb_fc = this->number_of_cells;

    long nb_anisotropic_compliant_fc = sizes[7];
    long numberOfAnisotropicLinesPOne_size = sizes[8];  // nb_anisotropic_compliant_fc at the  beginning
    long agglomerationLines_size = sizes[9];            // nb_anisotropic_compliant_fc at the  beginning

    bool isAnisotropicLines = false;
    // self._listOfSetAnisotropicCompliant[iLevel] contains the set of anisotropic compliant cells (i.e. Hexa and
    //                                                                                                prism)

    // TODO Au lieu de faire un tableau avec toutes les cellules, les Hexa et les prismes sont suffisants!
    double *maxArray = new double[nb_fc];
    unordered_map<long, double> d_anisotropic_fc;// keys are the ratio Max to average (ratioArray[iCell]) and value
    //                                                 the (global) index of the cell.

    // Process of every compliant fine cells:
    long i_fc;
    long ind, ind_p_one, i_neighbour_fc;
    double min_weight, max_weight, averageWeight, weight;

    for (int i_loc_fc = 0; i_loc_fc < nb_anisotropic_compliant_fc; ++i_loc_fc) {

        i_fc = array_of_anisotropic_compliant_fc[i_loc_fc];
        ind = this->_m_CRS_Row_Ptr[i_fc];
        ind_p_one = this->_m_CRS_Row_Ptr[i_fc + 1];

        min_weight = numeric_limits<double>::max();
        max_weight = 0.0;
        averageWeight = 0.0;

        // computation of min_weight, max_weight and averageWeight for the current cell i_loc_fc
        // Process of every faces/Neighbours
        for (long i_n = ind; i_n < ind_p_one; ++i_n) {
            i_neighbour_fc = this->_m_CRS_Col_Ind[i_n];
            if (i_neighbour_fc != i_fc) {  // to avoid special case where the boundary value are stored
                weight = this->_m_CRS_Values[i_n];
                if (max_weight < weight) {
                    max_weight = weight;
                }
                if (min_weight > weight) {
                    min_weight = weight;
                }
            }

            averageWeight += this->_m_CRS_Values[i_n] / (double) (ind_p_one - ind);
        }
        maxArray[i_fc] = max_weight;

        // TODO comprendre la raison d'etre des 2 criteres: present chez Mavriplis et DLR
        // Anisotropy criteria for the initial sort
        // if ratioArray[i_loc_fc] >= 2.0:
        //     d_anisotropic_fc[ratioArray[i_loc_fc]] = i_loc_fc
        //
        //     count += 1

        // Anisotropy criteria for the line Admissibility
        if (max_weight / min_weight >= 4.0) {
            // if i_loc_fc in setOfAnisotropicCompliantFineCells:
            // we check if the cell is for lvl 0 hexa or prism
            // or for lvl >0 if is build from hexa or prism.
            // isAnisotropic[i_loc_fc] = True
            d_anisotropic_fc[i_fc] = max_weight / averageWeight;
        }
    }

    if (!d_anisotropic_fc.empty()) {

        // There are anisotropic cells
        // We sort the dict w.r.t. the ratio  ????
        // From the biggest "anisotropy" to the smallest
        // orderedDictAnisotropyCell = OrderedDict(sorted(d_anisotropic_fc.items(), key=lambda t: t[1],
        //                                                reverse=True))
        //sortedList_anisotropicCells = sorted(d_anisotropic_fc, key = d_anisotropic_fc.get, reverse = True)

        multimap<double, long> sorted_anisotropicCells = flip_map(d_anisotropic_fc);  // Sorting!
        multimap<double, long>::reverse_iterator rIt;

        long nb_anisotropic_fc = d_anisotropic_fc.size();
        long *index_l_to_g = new long[nb_anisotropic_fc];
        unordered_map<long, long> dictGlobalToLocal;

        // Initialisation of index_l_to_g
        // To work only on anisotropic cells.
        long i = 0, index;
        for (rIt = sorted_anisotropicCells.rbegin(); rIt != sorted_anisotropicCells.rend(); rIt++) {
            index = rIt->second;
            index_l_to_g[i] = index;
            dictGlobalToLocal[index] = i;
            ++i;
        }

        // is_anisotropic_local: to check if a cell has already been treated
        bool *is_anisotropic_local = new bool[nb_anisotropic_fc];
        for (long iLoc = 0; iLoc < nb_anisotropic_fc; iLoc++) {
            is_anisotropic_local[iLoc] = true;
        }

        // TODO Think of a more pertinent seed???
        long i_loc_seed = 0;

        long i_seed = index_l_to_g[i_loc_seed];

        int lines_size = 0;
        forward_list<deque<long> *> lines;

        long i_current_fc;

        bool searchOppositeDirection = false;
        long cellAtTheLimitIsoAniso;
        long previousCurrentCell;

        // We try every possible cell as seed:
        while (i_loc_seed < nb_anisotropic_fc) {

            i_current_fc = i_seed;
            deque<long> *dQue;
            dQue = new deque<long>();
            (*dQue).push_back(i_seed);
            deque<long>::iterator iter;

            // Line contains the global numbering of cells
            searchOppositeDirection = false;

            is_anisotropic_local[i_loc_seed] = false;
            cellAtTheLimitIsoAniso = -1;

            while (i_current_fc != -1) {
                // Ca c'est pour verifier si on part ou non d'une extremite d'une ligne.
                // voir if i_current_fc == previousCurrentCell:
                previousCurrentCell = i_current_fc;
//                cout<<"CurrentCell "<<i_current_fc <<endl;
                ind = this->_m_CRS_Row_Ptr[i_current_fc];
                ind_p_one = this->_m_CRS_Row_Ptr[i_current_fc + 1];

                // Process neighbours of current cell
                for (long iN = ind; iN < ind_p_one; ++iN) {
                    i_neighbour_fc = this->_m_CRS_Col_Ind[iN];
//                    cout<<"i_current_fc "<< i_current_fc<< " i_neighbour_fc "<<i_neighbour_fc<<endl;
                    if (i_neighbour_fc != i_current_fc) {
                        // Reminder: if i_neighbour_fc == i_current_fc, we are at the boundary of the domain
                        if (this->_m_CRS_Values[iN] > 0.75 * maxArray[i_current_fc]) {
                            // Search faces with biggest areas
//                            cout<<"(adjMatrix_areaValues[iN] > 0.75 * maxArray[i_current_fc])"<<endl;
                            if (dictGlobalToLocal.count(i_neighbour_fc) == 1 and
                                is_anisotropic_local[dictGlobalToLocal[i_neighbour_fc]]) {

                                // We find an anisotropic neighbour

                                // On veut que la ligne soit definie de maniere contigue:
                                // c'est a  dire qu'on parte d'un bout vers l'autre et
                                // pas du milieu, jusqu'a un bout et puis on repart dans l'autre sens.
                                // TODO le cas test en ne triant pas initialement devra me permettre de tester ca!
                                if (!searchOppositeDirection) {
                                    // General case:
                                    // Correct direction from wall to farfield
                                    (*dQue).push_back(i_neighbour_fc);
                                    i_current_fc = i_neighbour_fc;
                                } else {
                                    (*dQue).push_front(i_neighbour_fc);
                                    i_current_fc = i_neighbour_fc;
                                }
                                // TODO Est-ce le bon moyen de marquer les cellules comme visitees????
                                // isAnisotropic[i_neighbour_fc] = False
                                is_anisotropic_local[dictGlobalToLocal[i_neighbour_fc]] = false;
                                break;
                            } else {
                                // We find an isotropic neighbour! (and we are not at the boundary

                                // assert cellAtTheLimitIsoAniso ==-1, "Problem cellAtTheLimitIsoAniso
                                // is overwritten"
                                bool isInDeque = false;
                                for (long index: (*dQue)) {
                                    if (i_neighbour_fc == index) {
                                        isInDeque = true;
                                        break;
                                    }
                                }
                                if (!isInDeque) {
                                    // As isAnisotropic[i_neighbour_fc] is used to mark/color cells already treated
                                    // we check is the neighbour cell has been treated in the current line
                                    // TODO what happend if the neighbour is in an another agglomerationLine???
                                    cellAtTheLimitIsoAniso = i_current_fc;
                                }
                            }
                        }
                    }
                }


                // TODO Could it be 3 anisotropic neighbour with area bigger than the criteria?
                if (i_current_fc == previousCurrentCell) {
                    // in this case, the seed was not at one end of the line.
                    // We have done a part of the line and we now try the other direction
                    if (!searchOppositeDirection) {
                        //assert(dQue.size() > 1); // "Trouble"
                        searchOppositeDirection = true;
                        i_current_fc = i_seed;
                    } else {
                        i_current_fc = -1;
                    }
                }
            }
            //cout<<"Line found: ";
            //for (iter = (*dQue).begin();iter!=(*dQue).end(); iter++) {
            //    cout<<*iter<<" ";
            //}
            //cout<<endl;

            // The current line contains more cells than the seed.
            // We keep it
            // otherwise we rewrite on it.
            if ((*dQue).size() > 1) {
//                cout<<"(*dQue).size() > 1"<<endl;
                //Si on recontre une cellulle isotrope, on inverse l'ordre!
                if ((*dQue).front() == cellAtTheLimitIsoAniso) {
                    //lines[lineNumber] = deque(reversed(lines[lineNumber]));
                    deque<long> *dQ2;
                    dQ2 = new deque<long>();
                    for (auto iFC :(*dQue)) {
//                        cout<<"(*dQue) "<<i<<endl;
                        (*dQ2).push_front(iFC);
                    }
                    assert((*dQ2).front() != cellAtTheLimitIsoAniso);// "Problem both ends are at the limit"
                    // deletion of dQue which is useless now
                    delete dQue;

                    // Creation of a new line
                    lines.push_front(dQ2);
                    lines_size++;
                } else {
                    // Creation of a new line
                    lines.push_front(dQue);
                    lines_size++;
                }
            } else {
                delete dQue;
            }

            // On pourrait prendre directement le suivant i.e. iSeed+=1
            // Sauf que potentiellement ce suivant pourrait deja appartenir a une ligne.
            while ((i_loc_seed < nb_anisotropic_fc) && (!is_anisotropic_local[i_loc_seed])) {
                // while i_loc_seed < nb_anisotropic_fc and not isAnisotropic[index_l_to_g[i_loc_seed]]:
                i_loc_seed += 1;
            }
            if (i_loc_seed == nb_anisotropic_fc) {
                continue;
            }
            i_seed = index_l_to_g[i_loc_seed];
        }
//        cout<<"End of While Loop"<<endl;
//        forward_list<deque<long>*>::iterator fLIter;
//        for (fLIter = lines.begin(); fLIter!=lines.end(); fLIter++)
//        {
//            cout<<(*(*fLIter)).size()<<endl;
//        }
//        cout<<"lines.front().size() "<< (*lines.front()).size()<<endl;

        // The last line may be of size 1 and will not be overwritten.
        if (!lines.empty()) {
            if ((*lines.front()).size() == 1) {

                if (verbose) {
                    cout << "lines.pop() " << (*lines.front()).front() << endl;
                }
                lines.pop_front();
            }
        }
//        cout<<"Conversion of deque to array"<<endl;
        long numberOfAgglomerationLines = lines_size;

        if (numberOfAgglomerationLines == 0) {
//            numberOfAgglomerationLines = 1;
            agglomerationLines_Idx[0] = 0;
            agglomerationLines_Idx[1] = 0;
            agglomerationLines_size = 0;
            numberOfAnisotropicLinesPOne_size = 2;
        } else {

            long numberOfFCellsInAgglomerationLines = 0;

            agglomerationLines_Idx[0] = 0;
            forward_list<deque<long> *>::iterator fLIt;
            long iLines = 1;
            for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {

                agglomerationLines_Idx[iLines] = (*(*fLIt)).size() + numberOfFCellsInAgglomerationLines;
                long jCount = 0;

                for (auto i :(*(*fLIt))) {
                    agglomerationLines[jCount + numberOfFCellsInAgglomerationLines] = i;
                    jCount++;
                }
                numberOfFCellsInAgglomerationLines += (*(*fLIt)).size();
                iLines++;
            }

            // Deletion of pointer to deque:
            for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {
                delete (*fLIt);
            }

            numberOfAnisotropicLinesPOne_size = iLines;
            agglomerationLines_size = numberOfFCellsInAgglomerationLines;
            isAnisotropicLines = true;
        }


        delete[] index_l_to_g;
        delete[] is_anisotropic_local;
    } else {
        agglomerationLines_Idx[0] = 0;
        agglomerationLines_Idx[1] = 0;
        agglomerationLines_size = 0;
        numberOfAnisotropicLinesPOne_size = 2;

    }

    sizes[8] = numberOfAnisotropicLinesPOne_size;
    sizes[9] = agglomerationLines_size;

    delete[] maxArray;
    return isAnisotropicLines;
}

unordered_map<long, unordered_set<pair<long, long>, pair_hash>> Dual_Graph::_compute_d_cut_edges(unordered_map<long, unordered_set<long>> d_cc) {

    unordered_map<long, unordered_set<pair<long, long>, pair_hash>> d_cut_edges;

    for (auto &d_cc_k_v : d_cc) {
        long i_cc = d_cc_k_v.first;
        for (auto &i_fc : d_cc[i_cc]) {
            auto i_fc_n_ptr = get_neighbours(i_fc).begin();
            auto i_w_n_fc_ptr = get_weights(i_fc).begin();
            for (; i_fc_n_ptr != get_neighbours(i_fc).end() && i_w_n_fc_ptr != get_weights(i_fc).end(); i_w_n_fc_ptr++, i_fc_n_ptr++) {
                long i_fc_n = *i_fc_n_ptr;
                long i_w_n_fc = *i_w_n_fc_ptr;
                if (d_cc[i_cc].count(i_fc_n) == 0 || i_fc == i_fc_n) {
                    if (d_cut_edges.count(i_cc) > 0) {
                        pair<long, long> tmp(i_fc, i_fc_n);
                        d_cut_edges[i_cc].insert(tmp);
                    } else {
                        d_cut_edges[i_cc];
                        d_cut_edges[i_cc].insert(make_pair(i_fc, i_fc_n));
                    }
                }
            }
        }
    }
    return d_cut_edges;
}

unsigned short int Dual_Graph::compute_degree_of_node_in_subgraph(int i_fc, unordered_set<long> s_of_fc) {

    unsigned short int deg(0);
    for (const long &i_fc_n : get_neighbours(i_fc)) {
        if (i_fc_n != i_fc && s_of_fc.count(i_fc_n) > 0) {
            deg++;
        }
    }
    return deg;
}


long Dual_Graph::_compute_subgraph_root(unordered_set<long> s_fc) {
    /*
      search of the fine cell at the "root" of the coarse cell, i.e. the fine cell with the most faces in common with
      its coarse cell.
      :param i_cc:
      :return:
    */
    long max_number_common_faces(-1), arg_max_number_common_faces(-1);
    for (auto i_fc : s_fc) {
        // Computation of the degree of the node in the CC subgraph.
        unsigned short int nb_common_faces_i_fc = compute_degree_of_node_in_subgraph(i_fc, s_fc);

        if (nb_common_faces_i_fc > max_number_common_faces) {
            max_number_common_faces = nb_common_faces_i_fc;
            arg_max_number_common_faces = i_fc;
        }
    }
    return arg_max_number_common_faces;
}


void Dual_Graph::clean_d_neighbours_of_seed(unordered_set<long> s_fc, unordered_map<long, long> &d_neighbours_of_seed) {
    /* Cleaning of d_neighbours_of_seed:
       We remove fc which belongs to the selected cc s_fc_for_cc*/
    for (auto i_fc : s_fc) {
        if (d_neighbours_of_seed.count(i_fc) > 0) {
            d_neighbours_of_seed.erase(i_fc);
        }
    }
    /* To do improve this if possible?
       we remove from d_neighbours_of_seed fc that are not in a first order neighbouhood of the selected set of fc*/

    vector<long> l_neighbours_of_seed;
    for (auto dnos_k_v : d_neighbours_of_seed)
        l_neighbours_of_seed.push_back(dnos_k_v.first);

    for (auto i_fc : l_neighbours_of_seed) {
        vector<long> neighbours = get_neighbours(i_fc);
        unordered_set<long> s_n(neighbours.begin(), neighbours.end());
        vector<long> result;
        set_intersection(s_n.begin(), s_n.end(), s_fc.begin(), s_fc.end(), result.begin());
        if (result.size() > 0)
            d_neighbours_of_seed.erase(i_fc);
        else
            d_neighbours_of_seed[i_fc] = 2;

    }
}


vector<double> Dual_Graph::compute_aspect_ratio() {
    /*
      With the storage in the CRS format of the dual mesh we can easily compute the aspect ratio of the cells.
      Once the aspect ratio computed for every cells, we compute the mean, the min, the max, the standard deviation, the variance
    */
    vector<double> aspect_ratio;
    for (int i_fc = 0; i_fc < number_of_cells; i_fc++) {
        double surface = 0.;
        for (auto i_w_n : get_weights(i_fc)) {
            /*we directly use j_cell because we don't need the real index of the neighbouring cell.
          only the index for matrixAdj_CRS_surface_values_lvl_l*/
            surface += i_w_n;
        }
        double tmp = pow(surface, 1.5) / _volumes[i_fc];
        aspect_ratio.push_back(tmp);
    }
    return aspect_ratio;
}


void compute_characteristics_utility(vector<double> aspect_ratio, double &min, double &max, double &mean, double &sd, double &median) {
    min = *min_element(aspect_ratio.begin(), aspect_ratio.end());
    max = *max_element(aspect_ratio.begin(), aspect_ratio.end());
    mean = accumulate(aspect_ratio.begin(), aspect_ratio.end(), 0);
    mean /= aspect_ratio.size();

    sd = 0.;
    for (auto ar : aspect_ratio) {
        sd += (ar - mean) * (ar - mean);
    }

    sd = sqrt(sd / aspect_ratio.size());


    // TODO
    median = 0.;
}


vector<double> Dual_Graph::compute_aspect_ratio_and_characteristics(double &min, double &max, double &mean, double &sd, double &median) {
    /*
      With the storage in the CRS format of the dual mesh we can easily compute the aspect ratio of the cells.
      Once the aspect ratio computed for every cells, we compute the mean, the min, the max, the standard deviation, the variance
      :param matrixAdj_CRS_row_ptr_lvl_l:
      :param matrixAdj_CRS_surface_values_lvl_l:
      :param volume_lvl_l:
      :return:
    */

    vector<double> aspect_ratio;
    for (int i_fc = 0; i_fc < number_of_cells; i_fc++) {
        double surface = 0.;
        for (auto i_w_n : get_weights(i_fc)) {
            /* we directly use j_cell because we don't need the real index of the neighbouring cell.
           only the index for matrixAdj_CRS_surface_values_lvl_l */
            surface += i_w_n;
        }

        double tmp = pow(surface, 1.5) / _volumes[i_fc];
        aspect_ratio.push_back(tmp);
    }

    compute_characteristics_utility(aspect_ratio, min, max, mean, sd, median);

    return aspect_ratio;
    //return aspect_ratio, aspect_ratio.min(), aspect_ratio.max(), np.mean(aspect_ratio), np.std(aspect_ratio), np.median(aspect_ratio)
}


void Dual_Graph::compute_aspect_ratio_characteristics(double &min, double &max, double &mean, double &sd, double &median, double &min_aniso, double &max_aniso, double &mean_aniso, double &sd_aniso,
                                                      double &median_aniso) {
    bool separate_iso_and_aniso(false);
    if (separate_iso_and_aniso) {
        vector<long> v_iso;
        unordered_set<long> s_cells;
        for (int i = 0; i < number_of_cells; i++)
            s_cells.insert(i);
        set_difference(s_cells.begin(), s_cells.end(), s_anisotropic_compliant_cells.begin(), s_anisotropic_compliant_cells.end(), back_inserter(v_iso));

        unsigned short int nb_aniso = s_anisotropic_compliant_cells.size();
        unsigned short int nb_iso = v_iso.size();;

        vector<double> a_aspect_ratio = compute_aspect_ratio();
        vector<double> a_aspect_ratio_iso;
        vector<double> a_aspect_ratio_aniso;

        for (auto i_cc : v_iso)
            a_aspect_ratio_iso.push_back(a_aspect_ratio[i_cc]);

        for (auto i_cc : s_anisotropic_compliant_cells)
            a_aspect_ratio_aniso.push_back(a_aspect_ratio[i_cc]);

        if (nb_iso) {
            compute_characteristics_utility(a_aspect_ratio_iso, min, max, mean, sd, median);
        }

        if (nb_aniso) {
            compute_characteristics_utility(a_aspect_ratio_aniso, min_aniso, max_aniso, mean_aniso, sd_aniso, median_aniso);
        }
    } else {
        vector<double> a_aspect_ratio = compute_aspect_ratio();
        compute_characteristics_utility(a_aspect_ratio, min, max, mean, sd, median);
    }
}


double Dual_Graph::compute_average_fc_compactness_inside_a_cc(unordered_set<long> set_of_fc) {
    // TODO Delete unused???
    if (set_of_fc.size() > 1) {
        unordered_map<long, unsigned short int> dict_fc_compactness = compute_fc_compactness_inside_a_cc(set_of_fc);
        vector<double> values;
        for (auto i_k_v : dict_fc_compactness)
            values.push_back(i_k_v.second);
        double sum = accumulate(values.begin(), values.end(), 0);
        return sum / dict_fc_compactness.size();
    } else
        return 0;
}


void Dual_Graph::compute_breadth_first_search(unordered_set<long> set_of_fc, long current_seed, unordered_map<long, long> dict_inv_list_of_fc, vector<long> color, long &max_color,
                                              unordered_set<long> &s_fc_max_color, unordered_map<long, vector<long>> &d_spanning_tree) {
    /*
      :param set_of_fc:
      :param current_seed:
      :param dict_inv_list_of_fc:
      :param color:
      :return:
    */
    //TODO trouble, d_spanning_tree is not a Tree
    //unordered_map<long,vector<long> > d_spanning_tree;
    // Reinitialization of color at every iteration
    for (int i = 0; i < color.size(); i++)
        color[i] = -1;
    long arg_seed = dict_inv_list_of_fc[current_seed];
    color[arg_seed] = 0;

    //long max_color = 0;

    deque<long> deque_of_new_seed;
    deque_of_new_seed.push_back(current_seed);

    //unordered_set<long> s_fc_max_color;

    while (!deque_of_new_seed.empty()) {
        // we want the first:
        current_seed = deque_of_new_seed.front();
        arg_seed = dict_inv_list_of_fc[current_seed];
        deque_of_new_seed.pop_front();
        /* TODO For reproduction testing: to remove hazard in set iteration (from != python version and much more!)
           print("remove this!!!!!! Useless when stabilized")
           print("As I had the sorting of initial Dictionary for dynamic graph, this should be useless!!!!")
           Add to insure independence w.r.t. data structure*/
        //l_sorted_neighbours = list(self.get_neighbours(current_seed))
        //print("l_sorted_neighbours", l_sorted_neighbours)
        //l_sorted_neighbours.sort()
        vector<long> l_sorted_neighbours = get_neighbours(current_seed);
        //print(l_sorted_neighbours)
        for (auto i_fc_neighbour : l_sorted_neighbours) {
            // for i_fc_neighbour in self.get_neighbours(current_seed):
            if (i_fc_neighbour != current_seed && set_of_fc.count(i_fc_neighbour) > 0) {
                long arg = dict_inv_list_of_fc[i_fc_neighbour];
                if (color[arg] == -1) {
                    color[arg] = color[arg_seed] + 1;
                    deque_of_new_seed.push_back(i_fc_neighbour);
                    if (max_color < color[arg_seed] + 1) {
                        max_color = color[arg_seed] + 1;
                        /* on veut seulement les indices de couleur max.
                           on le remet donc a zero quand on trouve une nouvelle coleur max.*/
                        s_fc_max_color.clear();
                    }
                    s_fc_max_color.insert(i_fc_neighbour);
                }
                /* if color[arg] >= color[arg_seed]:
                   building of spanning tree:*/
                if (d_spanning_tree.count(i_fc_neighbour) > 0) {
                    if (d_spanning_tree.count(current_seed) > 0)
                        d_spanning_tree[current_seed].push_back(i_fc_neighbour);
                    else {
                        d_spanning_tree[current_seed];
                        d_spanning_tree[current_seed].push_back(i_fc_neighbour);
                    }
                }
            }
        }
    }
}

void Dual_Graph::compute_breadth_first_search_v2(unordered_set<long> set_of_fc, long current_seed, vector<long> &predecessor, long &i_depth, unordered_map<long, vector<long> > &d_spanning_tree) {
    assert(set_of_fc.size() == number_of_cells);
    //unordered_map<long, vector<long> > d_spanning_tree;

    unordered_set<long> s_fc_visited;
    unordered_map<long, vector<long> > d_fringe;
    d_fringe[0];
    d_fringe[0].push_back(current_seed);

    i_depth = 0;
    s_fc_visited.insert(current_seed);
    if (!predecessor.empty()) {
        for (int i = 0; i < predecessor.size(); i++)
            predecessor[i] = -1;
    }

    while (i_depth < d_fringe.size()) {
        for (auto i_current_seed : d_fringe[i_depth]) {
            /* TODO For reproduction testing: to remove hazard in set iteration (from != python version and much more!)
           print("BFS_v2 remove this!!!!!! Useless when stabilized")
           print("As I had the sorting of initial Dictionary for dynamic graph, this should be useless!!!!")

           Add to insure independence w.r.t. data structure
           l_sorted_neighbours = list(self.get_neighbours(i_current_seed))
           l_sorted_neighbours.sort()*/

            for (auto i_fc_neighbour : get_neighbours(i_current_seed)) {
                // for i_fc_neighbour in self.get_neighbours(i_current_seed):
                if (i_fc_neighbour != i_current_seed && set_of_fc.count(i_fc_neighbour) > 0) {
                    if (s_fc_visited.count(i_fc_neighbour) == 0) {
                        if (d_fringe.count(i_depth + 1) > 0)
                            d_fringe[i_depth + 1].push_back(i_fc_neighbour);
                        else {
                            d_fringe[i_depth + 1];
                            d_fringe[i_depth + 1].push_back(i_fc_neighbour);
                        }
                        if (!predecessor.empty())
                            predecessor[i_fc_neighbour] = i_current_seed;
                        if (d_spanning_tree.count(i_current_seed) > 0)
                            d_spanning_tree[i_current_seed].push_back(i_fc_neighbour);
                        else {
                            d_spanning_tree[i_current_seed];
                            d_spanning_tree[i_current_seed].push_back(i_fc_neighbour);
                        }
                        s_fc_visited.insert(i_fc_neighbour);
                    }
                }
            }
        }
        i_depth++;
    }
    i_depth--;
}

/*
    def compute_breadth_first_search_v2(self,
                                        set_of_fc,
                                        current_seed: int,
                                        predecessor: np.ndarray = None):

        while i_depth < len(d_fringe):

            for i_current_seed in d_fringe[i_depth]:

                # TODO For reproduction testing: to remove hazard in set iteration (from != python version and much more!)
                # print("BFS_v2 remove this!!!!!! Useless when stabilized")
                # print("As I had the sorting of initial Dictionary for dynamic graph, this should be useless!!!!")

                # Add to insure independence w.r.t. data structure
                # l_sorted_neighbours = list(self.get_neighbours(i_current_seed))
                # l_sorted_neighbours.sort()
                for i_fc_neighbour in self.get_neighbours(i_current_seed):
                    # for i_fc_neighbour in self.get_neighbours(i_current_seed):

                    if i_fc_neighbour != i_current_seed and i_fc_neighbour in set_of_fc:
                        if i_fc_neighbour not in s_fc_visited:

                            if i_depth + 1 in d_fringe:
                                d_fringe[i_depth + 1].append(i_fc_neighbour)
                            else:
                                d_fringe[i_depth + 1] = [i_fc_neighbour]
                            if predecessor is not None:
                                predecessor[i_fc_neighbour] = i_current_seed
                            if i_current_seed in d_spanning_tree:
                                d_spanning_tree[i_current_seed].append(i_fc_neighbour)
                            else:
                                d_spanning_tree[i_current_seed] = [i_fc_neighbour]
                            s_fc_visited.add(i_fc_neighbour)
            i_depth += 1
        # print(d_fringe)
        if predecessor is not None:
            return i_depth - 1, d_fringe, d_spanning_tree, predecessor
        else:
            return i_depth - 1, d_fringe, d_spanning_tree
*/



unsigned short int Dual_Graph::compute_min_fc_compactness_inside_a_cc(unordered_set<long> &s_fc) {
    // Compute Compactness of a cc
    // Be careful: connectivity is assumed
    if (s_fc.size() > 1) {
        unordered_map<long, unsigned short int> dict_fc_compactness = compute_fc_compactness_inside_a_cc(s_fc);
        if (dict_fc_compactness.empty()) {
            return 0;
        }
        unsigned short int min_comp = USHRT_MAX;
        for (auto &i_k_v :dict_fc_compactness) {
            if (i_k_v.second < min_comp) {
                min_comp = i_k_v.second;
            }
        }
        return min_comp;
    } else {
        return 0;
    }
}

unordered_map<long, unsigned short int> Dual_Graph::compute_fc_compactness_inside_a_cc(unordered_set<long> &s_fc) {
    unordered_map<long, unsigned short int> dict_fc_compactness;
    if (s_fc.size() > 1) {

        // for every fc constituting a cc
        for (const long &i_fc : s_fc) {

            vector<long> v_neighbours = get_neighbours(i_fc);
            for (const long &i_fc_n : v_neighbours) {
                if ((s_fc.count(i_fc_n)) && (i_fc != i_fc_n)) {
                    if (dict_fc_compactness.count(i_fc)) {
                        dict_fc_compactness[i_fc]++;
                    } else {
                        dict_fc_compactness[i_fc] = 1;
                    }
                }
            }
            if (dict_fc_compactness.count(i_fc) == 0) {
                dict_fc_compactness[i_fc] = 0;
            }
        }
    }
    return dict_fc_compactness;
}

vector<unordered_set<long>> Dual_Graph::compute_connected_components(const unordered_set<long> &s_initial_fc) {


/**
 * With a set of non connected fine cells (s_initial_cc), we compute connected components of the graph.
 * :param s_initial_cc: set of fine cells composing the initial coarse cell (not connected).
 * :return: a set of connected component (stored as frozenset)
 */

    vector<unordered_set<long>> v_of_connected_set;
    vector<long> v_initial_fc(s_initial_fc.size());
    int i = 0;
    for (const long &i_fc: s_initial_fc) {
        v_initial_fc[i++] = i_fc;
    }
    unsigned short int size_cc = s_initial_fc.size();
    if (size_cc <= 1) {
        unordered_set<long> s = {v_initial_fc[0]};
        v_of_connected_set.push_back(s);
        return v_of_connected_set;
    }
    vector<bool> is_already_connected(size_cc, false);
    is_already_connected[0] = true;

    unordered_set<long> s_next({v_initial_fc[0]});
    unsigned short int nb_connected_fc = 1;
    unordered_map<long, unsigned short int> dict_global_to_local;
    for (unsigned short int i_fc_loc = 0; i_fc_loc < size_cc; i_fc_loc++) {
        dict_global_to_local[v_initial_fc[i_fc_loc]] = i_fc_loc;
    }

    unordered_set<long> s_fc({v_initial_fc[0]});
    while (nb_connected_fc < size_cc) {
        if (!s_next.empty()) {

            const long i_fc = *s_next.begin(); // equiv. i_fc = s_next.pop()
            s_next.erase(s_next.begin()); // equiv. i_fc = s_next.pop()
            for (const long &i_fc_n : get_neighbours(i_fc)) {
                if (dict_global_to_local.count(i_fc_n)) {
                    if ((i_fc_n != i_fc) && (!is_already_connected[dict_global_to_local[i_fc_n]])) {
                        s_next.insert(i_fc_n);
                        nb_connected_fc++;
                        is_already_connected[dict_global_to_local[i_fc_n]] = true;
                        s_fc.insert(i_fc_n);
                    }
                }
            }
        } else {
            // End of a connected component
            v_of_connected_set.push_back(unordered_set<long>(s_fc));
            short int arg_new_seed = -1;  // This is the new seed of the new connected component
            for (unsigned short int i = 0; i < size_cc; i++) {
                if (!is_already_connected[i]) {
                    arg_new_seed = i;
                    is_already_connected[i] = true;
                    nb_connected_fc++;
                    break;
                }
            }

            // Creation of a new connected component
            s_fc = {v_initial_fc[arg_new_seed]};
            s_next = {v_initial_fc[arg_new_seed]};
        }
    }


// We add the last connected component
    v_of_connected_set.push_back(s_fc);

    return v_of_connected_set;

}

vector<double> Dual_Graph::compute_cells_color(vector<double> a_cells_color) {
    /*
      We first compute the maximum degree of the graph to approx the maximum number of color
      This is not an optimal coloring.
      :param a_cells_color: an np.ndarray initialized to -1
      :return: an  of colors
    */

    int max_number_of_neighbours(0);

    // Process cells/vertices

    for (int i_cell = 0; i_cell < number_of_cells; i_cell++) {
        unordered_set<long> s;
        vector<long> a_neighbours = get_neighbours(i_cell);
        int i(0);
        for (auto ind_neighbor_cell : a_neighbours) {
            i++;
            if (ind_neighbor_cell != i_cell) // if the cell is on boundary, it is defined in the row_ptr table
                s.insert(ind_neighbor_cell);
        }
        if (s.size() > max_number_of_neighbours)
            max_number_of_neighbours = s.size();
    }

    for (int i_cell = 0; i_cell < number_of_cells; i_cell++) {
        if (a_cells_color[i_cell] == -1) {
            unordered_set<long> available_colour;
            for (int i = 0; i < max_number_of_neighbours + 2; i++) // plus 2 est au pif!!!
                available_colour.insert(i);

            vector<long> a_neighbours = get_neighbours(i_cell);
            int i(0);
            for (auto ind_neighbor_cell : a_neighbours) {
                i++;
                if (ind_neighbor_cell != i_cell) { //if the cell is on boundary, it is defined in the row=ptr table
                    long c = a_cells_color[ind_neighbor_cell];
                    if (c != -1 && available_colour.count(c) > 0)
                        available_colour.erase(c);
                }
            }
            a_cells_color[i_cell] = *available_colour.begin();
            available_colour.erase(*available_colour.begin());
        }
    }

    return a_cells_color;

}

vector<double> Dual_Graph::compute_a_visualization_data(Available_Datas data_type) {
    if (data_type == Available_Datas::COMPACTNESS)
        return vector<double>(number_of_cells, 0.);
    else if (data_type == Available_Datas::CARD)
        return vector<double>(number_of_cells, 1.);
    else if (data_type == Available_Datas::ASPECT_RATIO) {
        double min(0), max(0), mean(0), sd(0), median(0);
        return compute_aspect_ratio_and_characteristics(min, max, mean, sd, median);
    } else if (data_type == Available_Datas::CELL_INDEX) {
        vector<double> tmp;
        for (int i = 0; i < number_of_cells; i++)
            tmp.push_back(i);
    } else if (data_type == Available_Datas::CELL_COLOR)
        return compute_cells_color(vector<double>(number_of_cells, -1.));
    else if (data_type == Available_Datas::CELLS_ON_BND) {
        assert(!seeds_pool->is_empty());
        vector<double> a_is_on_bnd(number_of_cells, 0.);
        for (auto i_k_v : seeds_pool->d_is_on_bnd) {
            long i = i_k_v.first;
            a_is_on_bnd[i] = i_k_v.second;
        }
        return a_is_on_bnd;
    } else {
        // throw exception;
    }
}

void Dual_Graph::compute_mesh_characteristics(int &nb_cells, int &nb_iso, int &nb_aniso) {
    nb_cells = number_of_cells;
    nb_aniso = s_anisotropic_compliant_cells.size();
    nb_iso = number_of_cells - nb_aniso;
}

void Dual_Graph::compute_characteristics(Available_Characteristic characteristic, double &min, double &max, double &mean, double &sd, double &median, double &min_aniso, double &max_aniso,
                                         double &mean_aniso, double &sd_aniso, double &median_aniso, int &nb_cells, int &nb_iso, int &nb_aniso) {
    Available_Characteristic c = Available_Characteristic::COMPACTNESS_DIST | Available_Characteristic::COMPACTNESS;
    c = c | Available_Characteristic::CARD;
    c = c | Available_Characteristic::CARD_DIST;

    if ((c & characteristic) != Available_Characteristic::NONE)
        return;
    else if (characteristic == Available_Characteristic::MESH)
        compute_mesh_characteristics(nb_cells, nb_iso, nb_aniso);
    else if (characteristic == Available_Characteristic::ASPECT_RATIO)
        compute_aspect_ratio_characteristics(min, max, mean, sd, median, min_aniso, max_aniso, mean_aniso, sd_aniso, median_aniso);
}

vector<long> Dual_Graph::get_vertices_iterable() {
    vector<long> tmp;
    for (int i = 0; i < number_of_cells; i++)
        tmp.push_back(i);
    return tmp;
}

unordered_map<long, vector<long> > Dual_Graph::compute_d_cc(vector<long> fc_2_cc) {
    assert(fc_2_cc.size() == number_of_cells);
    unordered_map<long, vector<long>> d_cc;
    for (auto i_fc : get_vertices_iterable()) {
        long i_cc = fc_2_cc[i_fc];
        if (d_cc.count(i_cc) > 0)
            d_cc[i_cc].push_back(i_fc);
        else {
            d_cc[i_cc];
            d_cc[i_cc].push_back(i_fc);
        }
    }
    return d_cc;
}

unordered_map<long, unordered_set<pair<long, long>, pair_hash>> Dual_Graph::compute_d_cc_edges(vector<long> fc_2_cc) {
    unordered_map<long, unordered_set<pair<long, long>, pair_hash>> d_cc_edges;

    for (int i_v = 0; i_v < number_of_cells; i_v++) {
        for (int i = _m_CRS_Row_Ptr[i_v]; i < _m_CRS_Row_Ptr[i_v + 1]; i++) {
            long i_w = _m_CRS_Col_Ind[i];
            long i_cc = fc_2_cc[i_v];
            pair<long, long> edge;
            if (i_v < i_w) {
                edge.first = i_v;
                edge.second = i_w;
            } else {
                edge.first = i_w;
                edge.second = i_v;
            }

            if (i_v != i_w) {
                if (i_cc == fc_2_cc[i_w]) {
                    if (d_cc_edges.count(i_cc) == 0)
                        d_cc_edges[i_cc];
                    d_cc_edges.insert(edge);
                } else {
                    i_cc = -1;
                    if (d_cc_edges.count(i_cc) == 0)
                        d_cc_edges[i_cc];
                    d_cc_edges.insert(edge);
                }
            }
        }
    }
    return d_cc_edges;
}

unsigned short int Dual_Graph::compute_degree_of_node(int i_fc, bool (*test_function)(int)) {
    // TODO Cythonize This
    if (test_function == nullptr) {
        unsigned short int deg(0);
        for (auto i_fc_n : get_neighbours(i_fc))
            if (i_fc_n != i_fc)
                deg += 1;
        return deg;
    } else {
        unsigned short int deg(0);
        for (auto i_fc_n : get_neighbours(i_fc))
            if (i_fc_n != i_fc && (*test_function)(i_fc_n))
                deg += 1;
        return deg;
    }
}

void Dual_Graph::compute_local_crs_subgraph_from_global_crs(unordered_set<long> set_of_node,
                                                            vector<long> &row_ptr_l,
                                                            vector<long> &col_ind_l,
                                                            vector<double> &values_l,
                                                            vector<long> &g_to_l) {
    unsigned short int number_Of_Nodes_L = set_of_node.size();
    unsigned short int number_of_nodes_g = number_of_cells;
    assert(number_of_cells == _m_CRS_Row_Ptr.size() - 1);

    vector<unordered_map<long, double>> adj_Matrix(number_of_nodes_g);

    int count(0), i_c(0);

    g_to_l.resize(number_of_nodes_g);
    for (int i = 0; i < number_of_nodes_g; i++)
        g_to_l[i] = -1;

    for (auto iV : set_of_node) {
        g_to_l[iV] = i_c;
        i_c++;
    }

    for (auto iV : set_of_node) {
        for (int i = _m_CRS_Row_Ptr[iV]; i < _m_CRS_Row_Ptr[iV + 1]; i++) {
            long i_w = _m_CRS_Col_Ind[i];
            if (set_of_node.count(i_w) > 0) {
                adj_Matrix[g_to_l[iV]][g_to_l[i_w]] = _m_CRS_Values[i];
                count++;
            }
        }
    }

    row_ptr_l.resize(number_Of_Nodes_L + 1);
    for (int i = 0; i < number_Of_Nodes_L + 1; i++)
        row_ptr_l[i] = 1;

    col_ind_l.resize(count);
    values_l.resize(count);
    for (int i = 0; i < count; i++) {
        col_ind_l[i] = 0;
        values_l[i] = 0.0;
    }

    row_ptr_l[0] = 0;
    long index_col_and_values = 0;

    for (int i = 0; i < number_Of_Nodes_L; i++) {
        vector<long> sortedList_i;
        for (auto adj_M_i_k_v : adj_Matrix[i])
            sortedList_i.push_back(adj_M_i_k_v.first);
        for (auto j : sortedList_i) {
            values_l[index_col_and_values] = adj_Matrix[i][j];
            col_ind_l[index_col_and_values] = j;

            index_col_and_values++;
            row_ptr_l[i + 1] = index_col_and_values;
        }
    }
}

unordered_set<long> Dual_Graph::compute_s_leaves(unordered_set<long> s_fc) {
    unordered_set<long> s_leaves;

    if (s_fc.size() == 0) {
        for (int i = 0; i < number_of_cells; i++)
            s_fc.insert(i);
    }

    for (auto i_fc : s_fc) {
        unsigned short int nb_common_faces_i_fc = compute_degree_of_node_in_subgraph(i_fc, s_fc);
        if (nb_common_faces_i_fc < 2)
            s_leaves.insert(i_fc);
    }
    return s_leaves;
}

/*
create_coarser_dual_graph
generate_coarser_dual_graph
 */


int Dual_Graph::compute_distance_from_seed_to_first_vertex_of_degree_2(long seed,
                                                                       unordered_map<long, queue<long> *> dict_Covering_Tree) {
    // TODO Cette fonction ne me parait pas ausi utile que pourrait le laisser penser son nom.
    // TODO En gros vu les arbres passes en arg, le retour est toujours 0 ou 1.
    // TODO et le nom n'est pas top, car le degre peut etre de plus de 2.
    long iter_seed = seed;
    int dist = 0;
    while (dict_Covering_Tree.count(iter_seed) == 1) {

        if ((*dict_Covering_Tree[iter_seed]).size() == 1) {
            iter_seed = (*dict_Covering_Tree[iter_seed]).front();
            dist += 1;
        } else {
            return dist;
        }
    }
    return dist;
}

unordered_map<long, queue<long> *> Dual_Graph::find_seed_via_frontal_method(long &max_seed,
                                                                            vector<long> listOfFineCells) {


//"""
//This function finds the fine cells in the list listOfFineCells that defined the biggest distance of the covering tree.
//:param listOfFineCells:
//:param matrixAdj_CRS_row_ptr:
//:param matrixAdj_CRS_col_ind:
//:return:
//"""

    long nbIteration = 5;

    long seed = listOfFineCells[0];
    int argSeed = 0;  //TODO Check int/Long
    unordered_set<long> possibleSeed;
    int iteration = 0;

//    long max_seed = -1;
    int max_dist = -1;
    int max_length = -1;
    unordered_map<long, queue<long> *> max_dict;

    int nbOfCells = listOfFineCells.size();
    unordered_map<long, int> dict_inv_listOfFineCells;
    unordered_set<long> setOfFineCells(nbOfCells);
    for (int i = 0; i < nbOfCells; i++) {
        dict_inv_listOfFineCells[listOfFineCells[i]] = i;
        setOfFineCells.insert(listOfFineCells[i]);
    }


    vector<int> colour = vector<int>(nbOfCells);//-1 * np.ones((nbOfCells,), dtype=int)

    // Loop to find a good seed.
    // Many iteration are needed.
    while (iteration < nbIteration) {

        // print "\niter", iteration
        unordered_map<long, queue<long> *> dict_ConnectivityTree;

        // parcours en profondeur
        //Initialisation of the vector to -1;
        for (int i = 0; i < nbOfCells; i++) {
            colour[i] = -1;
        }

        colour[argSeed] = 0;
        int maxColour = 0;

        queue<long> queueOfNewSeed = queue<long>({seed});

        long old_seed = seed;

        while (!queueOfNewSeed.empty()) {

            seed = queueOfNewSeed.front();
//            cout<<iteration<<" seed "<<seed<<endl;
            queueOfNewSeed.pop();
            argSeed = dict_inv_listOfFineCells[seed];

            long ind = _m_CRS_Row_Ptr[seed];
            long ind_p_one = _m_CRS_Row_Ptr[seed + 1];
            for (long iNCell = ind; iNCell < ind_p_one; ++iNCell) // Process of Neighbours
            {
                long indNeighborCell = _m_CRS_Col_Ind[iNCell];
//                cout<<"\t"<<indNeighborCell<<endl;
                if ((indNeighborCell != seed) && (setOfFineCells.count(indNeighborCell) == 1)) {

                    int arg = dict_inv_listOfFineCells[indNeighborCell];

                    if (colour[arg] == -1) {
                        colour[arg] = colour[argSeed] + 1;
                        queueOfNewSeed.push(indNeighborCell);
                        //cout<<"queueOfNewSeed.front() "<<queueOfNewSeed.front()<<endl;
                        if (maxColour < colour[argSeed] + 1) {
                            maxColour = colour[argSeed] + 1;
                        }
                    }
                    // building of connectivity tree:
                    if (dict_ConnectivityTree.count(seed) == 1) {
                        (*dict_ConnectivityTree[seed]).push(indNeighborCell);
                    } else {
                        dict_ConnectivityTree[seed] = new queue<long>({indNeighborCell});
                    }

                }
            }
        }
        if (max_length <= maxColour) {  // max length c'est la longueur max pour toutes les iterations

            int dist = compute_distance_from_seed_to_first_vertex_of_degree_2(old_seed, dict_ConnectivityTree);
            if (max_length < maxColour) {
                possibleSeed.clear();
            }
            max_length = maxColour;  // max length c'est la longueur max pour toutes les iterations

            if (max_dist <= dist) {
                max_seed = old_seed;
                max_dist = dist;

                // Deep copy:
                //unordered_map<long, queue<long>*> max_dict;
                //max_dict.erase();
                for (auto iPairMD:max_dict) {
                    delete iPairMD.second;
                }
                for (auto iPairDict:dict_ConnectivityTree) {
                    max_dict[iPairDict.first] = iPairDict.second;
//                    while(!(*iPair.second).empty())
//                    {
//                        (* max_dict[iPair.first]).push((*iPair.second).front());
//                        (*iPair.second).pop();
//                    }
                }
                //max_dict = copy.deepcopy(dict_ConnectivityTree);
            } else {
                // Destruction of the pointer to queue
                for (auto iPairDict:dict_ConnectivityTree) {
                    delete iPairDict.second;
                }
            }
        }
        unordered_set<long> set_max_New_Seed;
        for (int i = 0; i < nbOfCells; ++i) {
            if (colour[i] == maxColour) {
                set_max_New_Seed.insert(listOfFineCells[i]);
            }
        }

        if (set_max_New_Seed.size() == 1) {

            for (auto s : set_max_New_Seed) {
                seed = s;  //set_max_New_Seed.pop();
            }

            if (possibleSeed.count(seed) == 0) {
                argSeed = dict_inv_listOfFineCells[seed];
                possibleSeed.insert(old_seed);
            } else {
//                sizes[2] = max_seed;
                return max_dict;
            }
        } else {
            bool isAllSeedTested = true;

            for (auto newSeed: set_max_New_Seed) {

                if (possibleSeed.count(newSeed) == 0) {
                    isAllSeedTested = false;
                    seed = newSeed;
                    argSeed = dict_inv_listOfFineCells[seed];
                    possibleSeed.insert(old_seed);
                }
            }
            if (isAllSeedTested) {
                //seed = max_seed;
//                sizes[2] = max_seed;
                return max_dict;
            }

        }
        iteration += 1;

        // print "maxColour", maxColour
        // print "maxDist", max_dist, "seed", max_seed, "dict", max_dict
//        return max_seed, max_dict;
    }
//    sizes[2] = max_seed;
    return max_dict;
}

int Dual_Graph::remove_separating_vertex(long seed,
                                         unordered_map<long, queue<long> *> d_spanning_tree,
                                         unordered_set<long> &s_fc,
                                         long verbose) {
    // TODO Check pertinence of the algorithm. Can we do better???
    //The bipartition of the too big cell is a complex problem.
    //This is a stupid version of a solution.
    //The goal is to remove from s_fc the vertices that may split the coarse in more than two parts.
    //
    //:param seed:
    //:param d_spanning_tree:
    //:param s_fc:
    //:param matrixAdj_CRS_row_ptr:
    //:param matrixAdj_CRS_col_ind:
    //:return:
    //"""

    // We remove from the s_fc the cells of degree greater than (or equal to) 3.

    long iter_seed = seed;
    unordered_set<long> first_cc;  // it was list we try set
    unordered_set<long> second_cc(s_fc);  // it was list we try set = list(s_fc)
    unordered_set<long> setRemovedCells;

    // If seed is not on a cycle, we move as much as possible until we face a vertex of third degree (Cycle).
    while (d_spanning_tree.count(iter_seed)) {
        if ((*d_spanning_tree[iter_seed]).size() == 1) {
            first_cc.insert(iter_seed);
            second_cc.erase(iter_seed);
            iter_seed = (*d_spanning_tree[iter_seed]).front();
        } else {
            break;
        }

    }
    if (verbose) {
        cout << "iter_seed " << iter_seed << " d_spanning_tree.count(iter_seed) "
             << d_spanning_tree.count(iter_seed) << endl;
        cout << "[";
        for (auto i : first_cc) {
            cout << i << ", ";
        }
        cout << "]";
        cout << "[";
        for (auto i : second_cc) {
            cout << i << ", ";
        }
        cout << "]" << endl;
    }
    if (d_spanning_tree.count(iter_seed) == 1) {

        unordered_set<long> s_l({iter_seed});  // = set([iter_seed])
        unordered_set<long> s_l_plus_one;

        while (!s_l.empty()) {

//            print "\ns_l", s_l
            if (verbose) {
                cout << "\ts_l = [";

                for (auto iFineCell: s_l) {
                    cout << iFineCell << ", ";
                }
                cout << "]" << endl;
            }
            unordered_set<long> set_l_cells_to_remove;
            unordered_set<long> tmp_set_l(s_l);  // copy: we will remove some element during the computation
            if (verbose) {
                cout << "\ttmp_set_l = [";

                for (auto iFineCell: tmp_set_l) {
                    cout << iFineCell << ", ";
                }
                cout << "]" << endl;
            }
            for (int i_length = 1; i_length < s_l.size() + 1; i_length++) {
                if (verbose) {
                    cout << "\t\ti_length " << i_length << endl;
                }
                vector<long> vector_tmp_SetL(tmp_set_l.size());
                int i = 0;
                for (auto iFC : tmp_set_l) {
                    vector_tmp_SetL[i++] = iFC;
                }
                list<unordered_set<long>> list_of_list_l = parts_list(vector_tmp_SetL, i_length);
                if (verbose) {
                    cout << "\t\tlist_of_list_l" << endl;
                }

                for (auto iSubList : list_of_list_l) {
                    if (verbose) {
                        cout << "\t\t\tiSubList = [";

                        for (auto iFcell : iSubList) {
                            cout << iFcell << ", ";
                        }
                        cout << "]" << endl;
                    }

                    unordered_set<long> tmp_set(second_cc);
                    for (auto i_cell:iSubList) {
                        tmp_set.erase(i_cell);
                    }
                    if (verbose) {
                        cout << "\t\t\ttmp_set = [";

                        for (auto iFcell : tmp_set) {
                            cout << iFcell << ", ";
                        }
                        cout << "]" << endl;
                    }
//                    vector<long> v_tmp_list(tmp_set.size());
//                    int i = 0;
//                    for (auto iCell :tmp_set) {
//                        v_tmp_list[i] = iCell;
//                        ++i;
//                    }
                    if (verbose) {
                        cout << "\t\t\t=== 4" << endl;
                    }
                    if (!check_connectivity(tmp_set)) {
                        if (verbose) {
                            cout << "\t\t\t!check_connectivity" << endl;
                        }
                        for (auto iCell : iSubList) {

                            if (tmp_set_l.count(iCell)) {
                                tmp_set_l.erase(iCell);
                            }
                            if (s_fc.count(iCell)) {
                                set_l_cells_to_remove.insert(iCell);
                            }
                        }
                    }
                    if (verbose) {
                        cout << "\t\t\t=== 5" << endl;
                    }
                }
            }
            if (verbose) {
                cout << "\t=== 6" << endl;
            }


// cout<< "set_l_cells_to_remove"<<endl;
// set_l_cells_to_remove
            unordered_set<long> setL_Minus_setLCellsToRemove(s_l);
            for (long iCell : set_l_cells_to_remove) {
                setL_Minus_setLCellsToRemove.erase(iCell);
            }
            if (verbose) {
                cout << "=== 7" << endl;
            }
// Construction of the next level of the tree
            for (long iCell : setL_Minus_setLCellsToRemove) {
                if (verbose) { cout << "iCell " << iCell << endl; }
                if (d_spanning_tree.count(iCell) == 1) {
                    if (verbose) { cout << "=== 8" << endl; }
                    long iN = (*d_spanning_tree[iCell]).front();
                    if (verbose) { cout << "iN " << iN << endl; }
                    while (!(*d_spanning_tree[iCell]).empty()) {
                        if (verbose) { cout << "iN " << iN << endl; }
                        (*d_spanning_tree[iCell]).pop();
                        if ((s_l.count(iN) == 0) && (first_cc.count(iN) == 0) &&
                            (setRemovedCells.count(iN) == 0)) {
                            if (verbose) { cout << "s_l_plus_one.insert(" << iN << ")" << endl; }
                            s_l_plus_one.insert(iN);
                        }
                        iN = (*d_spanning_tree[iCell]).front();

                    }
                }
            }
            if (verbose) { cout << "Update" << endl; }
            for (auto id_Cell:set_l_cells_to_remove) {
                setRemovedCells.insert(id_Cell);  //setRemovedCells.merge(set_l_cells_to_remove);
            }

//s_fc -= set_l_cells_to_remove
            for (long iCell : set_l_cells_to_remove) {
                if (verbose) { cout << "s_fc.erase(" << iCell << ")" << endl; }
                s_fc.erase(iCell);
            }

            setL_Minus_setLCellsToRemove = s_l;
            for (long iCell : set_l_cells_to_remove) {
                setL_Minus_setLCellsToRemove.erase(iCell);
            }

            for (auto iC :setL_Minus_setLCellsToRemove) {
                first_cc.insert(iC);
                second_cc.erase(iC);

            }
            if (verbose) { cout << "=== 9" << endl; }
// print first_cc, second_cc
// print s_l, s_l_plus_one
            if (verbose) {
                cout << "s_l_plus_one" << endl;

                cout << "[";
                for (auto id_Ce:s_l_plus_one) {
                    cout << id_Ce << ", ";
                }
                cout << "]" << endl;
            }
            s_l = s_l_plus_one;
// listOfListL = partsList(list(s_l))
            s_l_plus_one.clear(); // = set([])
            if (verbose) {
                cout << "s_l = [";

                for (auto id_Ce:s_l) {
                    cout << id_Ce << ", ";
                }
                cout << "]" << endl;
                cout << "s_l_plus_one = [";
                for (auto id_Ce:s_l_plus_one) {
                    cout << id_Ce << ", ";
                }
                cout << "]" << endl;
                cout << "=== 10\n\n\n" << endl;
            }
        }
    }
}

// TODO Ici on utilise le card...
// TODO Est-ce qu'utiliser une autre metrique serait pertinent?
unordered_map<long, int> Dual_Graph::compute_neighbourhood_of_cc(const unordered_set<long> &s_seeds,
                                                                 unsigned short &nb_of_order_of_neighbourhood,
                                                                 unsigned short max_card,
                                                                 vector<bool> &isFineCellAgglomerated_tmp,
                                                                 unordered_set<long> s_of_constrained_fc) {
    // cout<<"Call of compute_neighbourhood_of_cc"<<endl;

    // Basic checks
    assert(max_card != -1);
    // TODO resoudre le probleme suivant: si l'ordre n'est pas assez grand,
    // c'est a dire que le nombre de voisin trouve n'est pas suffisant pour constituer une cellule grossiere entiere,
    // il faut peut-etre faire une boucle while pour aller le plus loin possible!
    if (max_card == 0) {
        max_card = USHRT_MAX;
    }

    // This function computes the neighbourhood of a seed passed as argument.
    // It looks in the neighbourhood of order at least nb_of_order_of_neighbourhood, but if the size of the set of neighbour
    // is too small (<max_card), we look in higher order neighbourhood.
    //
    unordered_map<long, int> d_n_of_seed;  // dict of fc with the order of neighbouring from seed
    unordered_map<long, int> d_n_of_order_o_m_one;  // dict of FC with the order of neighbouring from seed
    for (const long &i_fc : s_seeds) {
        d_n_of_order_o_m_one[i_fc] = 0;
    }


    int i_order = 1;

    while ((i_order < nb_of_order_of_neighbourhood + 1) ||
           (d_n_of_seed.size() + d_n_of_order_o_m_one.size()) < max_card) {

        unordered_map<long, int> d_n_of_order_o;

        //d_n_of_seed.update(d_n_of_order_o_m_one)
        for (auto id_M_one:d_n_of_order_o_m_one) {
            d_n_of_seed[id_M_one.first] = id_M_one.second;
        }

        for (const auto &i_k_v : d_n_of_order_o_m_one) {

            long seed_tmp = i_k_v.first;
            for (const long &i_fc_n : get_neighbours(seed_tmp)) {

                if ((d_n_of_seed.count(i_fc_n) == 0) &&
                    ((!isFineCellAgglomerated_tmp[i_fc_n] || !(s_of_constrained_fc).empty()))) {
                    if (d_n_of_order_o.count(i_fc_n) == 0) {
                        if (!(s_of_constrained_fc).empty()) {
                            if ((s_of_constrained_fc).count(i_fc_n)) {
                                if (d_n_of_order_o_m_one.count(i_fc_n)) {
                                    if (i_order < d_n_of_order_o_m_one[i_fc_n]) {
                                        d_n_of_order_o[i_fc_n] = i_order;
                                    }
                                } else {
                                    d_n_of_order_o[i_fc_n] = i_order;
                                }
                            }
                        } else {
                            // a fc can be access via multiple ways. We look for the quickest
                            if (d_n_of_order_o_m_one.count(i_fc_n)) {
                                if (i_order < d_n_of_order_o_m_one[i_fc_n]) {
                                    d_n_of_order_o[i_fc_n] = i_order;
                                }
                            } else {
                                d_n_of_order_o[i_fc_n] = i_order;
                            }
                        }
                    }
                }
            }
        }

        // Exit condition
        if (d_n_of_order_o.empty()) {
            // No more neighbours available:
            break;
        }

        // Copy: d_n_of_order_o_m_one = d_n_of_order_o;
        d_n_of_order_o_m_one.clear();
        for (auto id:d_n_of_order_o) {
            d_n_of_order_o_m_one[id.first] = id.second;
        }
        i_order++;
    }
    // Update of d_n_of_seed
    //d_n_of_seed.update(d_n_of_order_o_m_one)
    for (auto id_M_one:d_n_of_order_o_m_one) {
        d_n_of_seed[id_M_one.first] = id_M_one.second;
    }

    // We remove the seed from the neighbours of seed
    for (const long &i_fc:s_seeds) {
        d_n_of_seed.erase(i_fc);
    }

    nb_of_order_of_neighbourhood = i_order;
    return d_n_of_seed;
}