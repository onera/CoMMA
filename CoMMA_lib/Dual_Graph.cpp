//
// Created by Nicolas Lantos on 31/03/2021.
//

#include "Dual_Graph.h"

template<typename A, typename B> std::pair<B,A> flip_pair(const pair<A,B> &p)
{
    return pair<B,A>(p.second, p.first);
};

template<typename A, typename B, template<class, class, class...> class M, class... Args> multimap<B, A> flip_map(const M<A,B,Args...> &src)
{
    multimap<B,A> dst;
    transform(src.begin(), src.end(), inserter(dst, dst.begin()), flip_pair<A,B>);
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

            averageWeight += this->_m_CRS_Values[i_n] / (double)(ind_p_one - ind);
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