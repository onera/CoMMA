//
// Created by Nicolas Lantos on 06/04/2021.
//
#include "Util.h"

void convert_agglomeration_lines_arrays_to_agglomeration_lines(
        const unsigned long fineAgglomerationLines_array_Idx_size,
        const unsigned long fineAgglomerationLines_array_size,
        vector<long> &fineAgglomerationLines_array_Idx,
        vector<long> &fineAgglomerationLines_array,
        long &nb_agglomeration_lines,
        forward_list<deque<long> *> &agglomeration_lines
) {

    nb_agglomeration_lines = 0;
    if (fineAgglomerationLines_array_Idx_size != 0 and fineAgglomerationLines_array_Idx_size != 0) {

        for (long i = fineAgglomerationLines_array_Idx_size - 2; i > -1; i--) {
            long ind = fineAgglomerationLines_array_Idx[i];
            long indPOne = fineAgglomerationLines_array_Idx[i + 1];

            deque<long> *dQue = new deque<long>();
            for (long j = ind; j < indPOne; j++) {
                (*dQue).push_back(fineAgglomerationLines_array[j]);
            }
            agglomeration_lines.push_front(dQue);
            nb_agglomeration_lines++;
        }
    }
}

void convert_agglo_lines_to_agglomeration_lines_arrays(const long nb_lines, // because forward_list does not have a size() method.
                                                       forward_list<deque<long> *> const &agglo_lines,
                                                       long *sizes,  // out
                                                       vector<long> &agglo_lines_array_idx,// out
                                                       vector<long> &agglo_lines_array) {  // out
    // arrays agglo_lines_array_idx and agglo_lines_array are already allocated.
    // As we agglomerate cells, the number of agglo lines does not vary a priori, but they are getting smaller and smaller.

    if (nb_lines == 0) {

        sizes[0] = 1 + 1;
        sizes[1] = 0;
    }

    long number_of_fc_in_agglomeration_lines = 0;
    agglo_lines_array_idx[0] = 0;

    long i_l = 0;
    for (auto &line :(agglo_lines)) {
        long size_of_line = (*line).size();
        agglo_lines_array_idx[i_l + 1] = size_of_line + number_of_fc_in_agglomeration_lines;

        for (long i = 0; i < size_of_line; i++) {
            agglo_lines_array[i + number_of_fc_in_agglomeration_lines] = (*line)[i];
        }
        number_of_fc_in_agglomeration_lines += size_of_line;
        i_l++;
    }
    sizes[0] = nb_lines + 1;
    sizes[1] = number_of_fc_in_agglomeration_lines;
}


list<unordered_set<long>> parts_list(vector<long> seq, int length) {
    // generates all subparts of a list:
    list<unordered_set<long>> p;
    int i = 1;
    int iMax = pow(2, seq.size()) - 1;

    while (i <= iMax) {
        unordered_set<long> s;
        int j = 0, jmax = seq.size() - 1;
        while (j <= jmax) {
            if (((i >> j) & 1) == 1) {
                s.insert(seq[j]);
            }
            j += 1;
        }
        if (length > 0) {
            if (s.size() == length) {
                p.push_back(s);
            }
        } else {
            p.push_back(s);
        }
        i += 1;
    }
    return p;
}

void print_queue(queue<long> q) {
    cout << "[";
    while (!q.empty()) {
        cout << q.front() << ", ";
        q.pop();
    }
    cout << "]" << endl;
}

void print_agglomeration_lines(forward_list<deque<long> *> lines) {

    int count = 0;
    forward_list<deque<long> *>::iterator fLIt;
    for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {

        cout << "<" << count;
        cout << ": {";
        for (auto i :(*(*fLIt))) {
            cout << i << ", ";
        }
        cout << "} >" << endl;
        count++;
    }

}

void clean_agglomeration_lines(forward_list<deque<long> *> lines) {

    forward_list<deque<long> *>::iterator fLIt;
    // Deletion of pointer to deque:
    for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {
        delete (*fLIt);
    }

}

forward_list<deque<long> *> copy_agglomeration_lines(forward_list<deque<long> *> agglo_lines) {

    forward_list<deque<long> *> cp_agglo_lines;
    forward_list<deque<long> *> cp_agglo_lines_tmp;

    forward_list<deque<long> *>::iterator fLIt;

    int count = 0;

    for (fLIt = agglo_lines.begin(); fLIt != agglo_lines.end(); fLIt++) {
        cp_agglo_lines_tmp.push_front(new deque<long>(**fLIt));
//        cout << "count " << count;
//        cout << " {";
//        for (auto i :(*(*fLIt))) {
//            cout << i << ", ";
//        }
//        cout << "}" << endl;
        count++;
    }
    for (fLIt = cp_agglo_lines_tmp.begin(); fLIt != cp_agglo_lines_tmp.end(); fLIt++) {
        cp_agglo_lines.push_front(new deque<long>(**fLIt));
//        cout << "count " << count << endl;
//        cout << "{";
//        for (auto i :(*(*fLIt))) {
//            cout << i << ", ";
//        }
//        cout << "}" << endl;
//        count++;
    }
//    for (fLIt = agglo_lines.begin(); fLIt != agglo_lines.end(); fLIt++) {
//
//                cout << "count " << count << endl;
//        cout << "{";
//        for (auto i :(*(*fLIt))) {
//            cout << i << ", ";
//        }
//        cout << "}" << endl;
//        count++;
//    }
    clean_agglomeration_lines(cp_agglo_lines_tmp);
    return cp_agglo_lines;
}

bool compare_maps_keys(const unordered_map<long, unsigned short> &m_1, const unordered_map<long, unsigned short> &m_2) {

    if (m_1.size() != m_2.size()) {
        return false;
    }
    for (const auto i_k_v : m_1) {
        if (m_2.count(i_k_v.first) == 0) {
            return false;
        }
    }
    return true;
}

list<unordered_set<long>> partsList(vector<long> seq, int length) {
    // generates all subparts of a list:
    list<unordered_set<long>> p;
    int i = 1;
    int iMax = pow(2, seq.size()) - 1;

    while (i <= iMax) {
        unordered_set<long> s;
        int j = 0, jmax = seq.size() - 1;
        while (j <= jmax) {
            if (((i >> j) & 1) == 1) {
                s.insert(seq[j]);
            }
            j += 1;
        }
        if (length > 0) {
            if (s.size() == length) {
                p.push_back(s);
            }
        } else {
            p.push_back(s);
        }
        i += 1;
    }
    return p;
}

bool checkConnectivity_w_set(unordered_set<long> setFineCells, long *matrixAdj_CRS_row_ptr, long *matrixAdj_CRS_col_ind, int verbose) {

//"""
//Checks connectivity of the coarse cell
//        :param listFineCells: List of fine cells defining the coarse element.
//:param matrixAdj_CRS_row_ptr: dual graph information see MgridGen data structure
//        :param matrixAdj_CRS_col_ind: dual graph information see MgridGen data structure
//        :return: True or False
//"""
    unordered_map<long, bool> map_isAlreadyConnected;
    int size = setFineCells.size();
    if (size <= 1) {
        return true;
    }
    for (long iFC :setFineCells) {
        map_isAlreadyConnected[iFC] = false;
    }
    long front = *setFineCells.begin();
    map_isAlreadyConnected[front] = true;

    unordered_set<long> setNext({front});
    int nbConnectedCells = 1;

    while (nbConnectedCells < size) {

        if (!setNext.empty()) {
            long iFineCell = *setNext.begin();
            setNext.erase(setNext.begin());
            long ind = matrixAdj_CRS_row_ptr[iFineCell];  // Usefull to find neighbours of seed
            long ind_p_one = matrixAdj_CRS_row_ptr[iFineCell + 1]; // Usefull to find neighbours of seed
            for (long iFC = ind; iFC < ind_p_one; iFC++) {
                long iFCellNeighbour = matrixAdj_CRS_col_ind[iFC];
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
        cout << "\ncheckConnectivity_w_set " << endl;
        cout << "{";
        for (auto i : setFineCells) {
            cout << i << ", ";
        }
        cout << "}";
        cout << "setFineCells " << endl;
        int iCount = 0;
        for (auto i : setFineCells) {
            cout << i << " : " << map_isAlreadyConnected[i] << endl;
            iCount++;
        }

        cout << "nbConnectedCells " << nbConnectedCells << " size " << size << endl;
    }

    return nbConnectedCells == size;

}


int computeNumberOfCommonFaces(long iFine, long iCoarse,
                               long *matrixAdj_CRS_row_ptr,
                               long *matrixAdj_CRS_col_ind,
                               long *fine_Cell_indices_To_Coarse_Cell_Indices) {
    int nbCommonFaces = 0;

    long ind = matrixAdj_CRS_row_ptr[iFine];
    long ind_p_one = matrixAdj_CRS_row_ptr[iFine + 1];
    long indOfFineNeighbor = 0;
    for (int iN = ind; iN < ind_p_one; iN++) { //We process every neighbour of iFine
        indOfFineNeighbor = matrixAdj_CRS_col_ind[iN];
        if ((indOfFineNeighbor != iFine) && (fine_Cell_indices_To_Coarse_Cell_Indices[indOfFineNeighbor] == iCoarse)) {
            nbCommonFaces += 1;
        }
    }
    return nbCommonFaces;
}


void store_agglomeration_datas(long *sizes,
        // Dual graph:
                           vector<long> &adjMatrix_row_ptr,
                           vector<long> &adjMatrix_col_ind,
                           vector<double> &adjMatrix_areaValues,
                           vector<double> &volumes,


                           vector<long> &arrayOfFineAnisotropicCompliantCells,

                           vector<long> &isOnFineBnd,
                           vector<long> &array_is_on_valley,
                           vector<long> &array_is_on_ridge,
                           vector<long> &array_is_on_corner,

        // Agglomeration argument
                           long isFirstAgglomeration_long,

                           long is_anisotropic_long,


                           vector<long> fineCellToCoarseCell,
                       
                           vector<long> &agglomerationLines_Idx,  // In & out
                           vector<long> &agglomerationLines,  // In & out



                               long dimension,
                               long goalCard,
                               long minCard,
                               long maxCard,
                               long checks_long,
                               long verbose_long) {

    cout << "Call of store_agglomeration_datas" << endl;
    long numberOfFineCells = sizes[0];
    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
    long adjMatrix_col_ind_size = sizes[1];
    long adjMatrix_areaValues_size = sizes[1];

    // Rmk: sizes[2] ==indCoarseCell
    long numberOfFineAgglomeratedCells = sizes[3];
    long isOnValley_size = sizes[4];
    long isOnRidge_size = sizes[5];
    long isOnCorner_size = sizes[6];
    long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
    long agglomerationLines_Idx_size = sizes[8];
    long agglomerationLines_size = sizes[9];

    bool isFirstAgglomeration = isFirstAgglomeration_long == 1;
    bool isAnisotropic = is_anisotropic_long == 1;
    //string filename = "/Users/lantos/CLionProjects/CoMMA/0_Outputs/Datas_Agglomeration_";
    string filename = "/stck/aremigi/aggl_data/";
    ofstream myfile;
    myfile.open(filename + to_string(numberOfFineCells) + "_" + to_string(adjMatrix_col_ind_size) + ".txt");

    myfile << "adjMatrix_row_ptr; ";
    myfile << to_string(numberOfFineCells + 1) + "; ";
    for (int i = 0; i < numberOfFineCells + 1; i++) {
        myfile << to_string(adjMatrix_row_ptr[i]) << "; ";
    }
    myfile << endl;

    myfile << "adjMatrix_col_ind; ";
    myfile << to_string(adjMatrix_col_ind_size) + "; ";
    for (int i = 0; i < adjMatrix_col_ind_size; i++) {
        myfile << to_string(adjMatrix_col_ind[i]) << "; ";
    }
    myfile << endl;

    myfile << "adjMatrix_areaValues; ";
    myfile << to_string(adjMatrix_areaValues_size) + "; ";
    for (int i = 0; i < adjMatrix_areaValues_size; i++) {
        myfile << std::scientific << adjMatrix_areaValues[i] << "; ";
    }
    myfile << endl;

    myfile << "volumes; ";
    myfile << to_string(numberOfFineCells) + "; ";
    for (int i = 0; i < numberOfFineCells; i++) {
        myfile << std::scientific << volumes[i] << "; ";
    }
    myfile << endl;
    myfile << "arrayOfFineAnisotropicCompliantCells; ";
    myfile << to_string(numberOfFineCells) + "; ";
    for (int i = 0; i < numberOfFineCells; i++) {
        myfile << to_string(arrayOfFineAnisotropicCompliantCells[i]) << "; ";
    }
    myfile << endl;


    myfile << "isOnFineBnd; ";
    myfile << to_string(numberOfFineCells) + "; ";
    for (int i = 0; i < numberOfFineCells; i++) {
        myfile << to_string(isOnFineBnd[i]) << "; ";
    }
    myfile << endl;

    myfile << "array_is_on_valley; ";
    myfile << to_string(isOnValley_size) + "; ";
    for (int i = 0; i < isOnValley_size; i++) {
        myfile << to_string(array_is_on_valley[i]) << "; ";
    }
    myfile << endl;

    myfile << "array_is_on_ridge; ";
    myfile << to_string(isOnRidge_size) + "; ";
    for (int i = 0; i < isOnRidge_size; i++) {
        myfile << to_string(array_is_on_ridge[i]) << "; ";
    }
    myfile << endl;

    myfile << "array_is_on_corner; ";
    myfile << to_string(isOnCorner_size) + "; ";
    for (int i = 0; i < isOnCorner_size; i++) {
        myfile << to_string(array_is_on_corner[i]) << "; ";
    }
    myfile << endl;

    myfile << "isFirstAgglomeration_long; ";
    myfile << "1; ";
    myfile << to_string(isFirstAgglomeration_long) + "; ";
    myfile << endl;

    myfile << "isAnisotropic_long; ";
    myfile << "1; ";
    myfile << to_string(is_anisotropic_long) + "; ";
    myfile << endl;

    myfile << "agglomerationLines_Idx; ";
    myfile << to_string(agglomerationLines_Idx_size) + "; ";
    for (int i = 0; i < agglomerationLines_Idx_size; i++) {
        myfile << to_string(agglomerationLines_Idx[i]) << "; ";
    }
    myfile << endl;

    myfile << "agglomerationLines; ";
    myfile << to_string(agglomerationLines_size) + "; ";
    for (int i = 0; i < agglomerationLines_size; i++) {
        myfile << to_string(agglomerationLines[i]) << "; ";
    }
    myfile << endl;

    myfile << "dimension; ";
    myfile << "1; ";
    myfile << to_string(dimension) + "; ";
    myfile << endl;

    myfile << "goalCard; ";
    myfile << "1; ";
    myfile << to_string(goalCard) + "; ";
    myfile << endl;

    myfile << "minCard; ";
    myfile << "1; ";
    myfile << to_string(minCard) + "; ";
    myfile << endl;

    myfile << "maxCard; ";
    myfile << "1; ";
    myfile << to_string(maxCard) + "; ";
    myfile << endl;

    myfile.close();
}

void read_agglomeration_datas_from_file(string filename,
                                        long *sizes,
                                        long *&adjMatrix_row_ptr,
                                        long *&adjMatrix_col_ind,
                                        double *&adjMatrix_areaValues,
                                        double *&volumes,

                                        long *&arrayOfFineAnisotropicCompliantCells,

                                        long *&isOnFineBnd_l,
                                        long *&array_isOnValley,
                                        long *&array_isOnRidge,
                                        long *&array_isOnCorner,
                                        long *isFirstAgglomeration_long,
                                        long *isAnisotropic_long,

                                        long *&agglomerationLines_Idx,
                                        long *&agglomerationLines,

                                        long *dimension,
                                        long *goalCard,
                                        long *minCard,
                                        long *maxCard
) {

//    cout << "\nCall of read_agglomeration_datas" << endl;
    fstream my_file;
    my_file.open(filename, ios::in);
    if (my_file.is_open()) {   //checking whether the file is open
        string tp;

        sizes[0] = read_long_array(my_file, adjMatrix_row_ptr) - 1;
//        cout << "\nadjMatrix_row_ptr= " << sizes[0] << endl;
//        for (int i = 0; i < sizes[0] + 1; i++) {
//            cout << adjMatrix_row_ptr[i] << " ";
//        }
//        cout<<endl;
        sizes[1] = read_long_array(my_file, adjMatrix_col_ind);
//                cout << "\nadjMatrix_col_ind= " << sizes[1] << endl;
//        for (int i = 0; i < sizes[1]; i++) {
//            cout << adjMatrix_col_ind[i] << " ";
//        }
//        cout << endl;

        read_double_array(my_file, adjMatrix_areaValues);
//        for (int i = 0; i < sizes[1]; i++) {
//            cout << adjMatrix_areaValues[i] << " ";
//        }
//        cout << endl;

//                cout << "sizes[1]= " << sizes[1] << endl;
//        cout << "adjMatrix_areaValues"<<endl;
//        for (int i = 0; i < sizes[1]; i++) {
//            cout << adjMatrix_areaValues[i] << " ";
//        }
//        cout << endl;

        read_double_array(my_file, volumes);
//        cout << "volumes"<<endl;
//        for (int i = 0; i < sizes[0]; i++) {
//            cout << volumes[i] << " ";
//        }
//        cout << endl;

        sizes[7] = read_long_array(my_file, arrayOfFineAnisotropicCompliantCells);

        read_long_array(my_file, isOnFineBnd_l);
//        for (int i = 0; i < sizes[0]; i++) {
//            cout << isOnFineBnd_l[i] << " ";
//        }
//        cout<<endl;

        sizes[4] = read_long_array(my_file, array_isOnValley);
//        cout << "array_isOnValley "<<sizes[7]<<":";
//        for (int i = 0; i < sizes[7]; i++) {
//            cout << array_isOnValley[i] << " ";
//        }
        cout << endl;
        sizes[5] = read_long_array(my_file, array_isOnRidge);
        sizes[6] = read_long_array(my_file, array_isOnCorner);
        isFirstAgglomeration_long[0] = read_long(my_file);
        isAnisotropic_long[0] = read_long(my_file);

        sizes[8] = read_long_array(my_file, agglomerationLines_Idx);
        sizes[9] = read_long_array(my_file, agglomerationLines);

        dimension[0] = read_long(my_file);
//        cout<<"dimension[0]"<<dimension[0]<<endl;
        goalCard[0] = read_long(my_file);
//        cout<<"goalCard[0]"<<goalCard[0]<<endl;
        minCard[0] = read_long(my_file);
//        cout<<"minCard[0]"<<minCard[0]<<endl;
        maxCard[0] = read_long(my_file);
//        cout<<"maxCard[0]"<<maxCard[0]<<endl;


        my_file.close();   //close the file object.
    } else {
        cout << "Fail to read file " << filename << endl;
    }
//    long numberOfFineCells = sizes[0];
//    long adjMatrix_row_ptr_size = numberOfFineCells + 1;
//    long adjMatrix_col_ind_size = sizes[1];
//    long adjMatrix_areaValues_size = sizes[1];
//
//    // Rmk: sizes[2] ==indCoarseCell
//    long numberOfFineAgglomeratedCells = sizes[3];
//    long isOnValley_size = sizes[4];
//    long isOnRidge_size = sizes[5];
//    long isOnCorner_size = sizes[6];
//    long arrayOfFineAnisotropicCompliantCells_size = sizes[7];
//    long agglomerationLines_Idx_size = sizes[8];
//    long agglomerationLines_size = sizes[9];
//
//    bool isFirstAgglomeration = isFirstAgglomeration_long == 1;
//    bool isAnisotropic = isAnisotropic_long == 1;

//    myfile << "adjMatrix_row_ptr; ";
//    myfile << to_string(numberOfFineCells) + "; ";
//    for (int i = 0; i < numberOfFineCells; i++) {
//        myfile << to_string(adjMatrix_row_ptr[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "adjMatrix_col_ind; ";
//    myfile << to_string(adjMatrix_col_ind_size) + "; ";
//    for (int i = 0; i < adjMatrix_col_ind_size; i++) {
//        myfile << to_string(adjMatrix_col_ind[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "adjMatrix_areaValues; ";
//    myfile << to_string(adjMatrix_areaValues_size) + "; ";
//    for (int i = 0; i < adjMatrix_areaValues_size; i++) {
//        myfile << to_string(adjMatrix_areaValues[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "volumes; ";
//    myfile << to_string(numberOfFineCells) + "; ";
//    for (int i = 0; i < numberOfFineCells; i++) {
//        myfile << to_string(volumes[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "isOnFineBnd_l; ";
//    myfile << to_string(numberOfFineCells) + "; ";
//    for (int i = 0; i < numberOfFineCells; i++) {
//        myfile << to_string(isOnFineBnd_l[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "array_isOnValley; ";
//    myfile << to_string(isOnValley_size) + "; ";
//    for (int i = 0; i < isOnValley_size; i++) {
//        myfile << to_string(array_isOnValley[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "array_isOnRidge; ";
//    myfile << to_string(isOnRidge_size) + "; ";
//    for (int i = 0; i < isOnRidge_size; i++) {
//        myfile << to_string(array_isOnRidge[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "array_isOnCorner; ";
//    myfile << to_string(isOnCorner_size) + "; ";
//    for (int i = 0; i < isOnCorner_size; i++) {
//        myfile << to_string(array_isOnCorner[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "isFirstAgglomeration_long; ";
//    myfile << to_string(isFirstAgglomeration_long) + "; ";
//    myfile << endl;
//
//    myfile << "isAnisotropic_long; ";
//    myfile << to_string(isAnisotropic_long) + "; ";
//    myfile << endl;
//
//    myfile << "agglomerationLines_Idx; ";
//    myfile << to_string(agglomerationLines_Idx_size) + "; ";
//    for (int i = 0; i < agglomerationLines_Idx_size; i++) {
//        myfile << to_string(agglomerationLines_Idx[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "agglomerationLines; ";
//    myfile << to_string(agglomerationLines_size) + "; ";
//    for (int i = 0; i < agglomerationLines_size; i++) {
//        myfile << to_string(agglomerationLines[i]) << "; ";
//    }
//    myfile << endl;
//
//    myfile << "dimension; ";
//    myfile << to_string(dimension) + "; ";
//    myfile << endl;
//
//    myfile << "goalCard; ";
//    myfile << to_string(goalCard) + "; ";
//    myfile << endl;
//
//    myfile << "minCard; ";
//    myfile << to_string(minCard) + "; ";
//    myfile << endl;
//
//    myfile << "maxCard; ";
//    myfile << to_string(maxCard) + "; ";
//    myfile << endl;
//
//    myfile.close();
}

long read_long_array(std::fstream &stream,
                     long *&array) {

    string tp;

    // 16 entries
    char delim = ';';
    getline(stream, tp, delim);
    tp.erase(std::remove(tp.begin(), tp.end(), '\n'), tp.end());
//    cout <<"\nName="<< tp << "\t";
    getline(stream, tp, delim);
    int size = std::stoi(tp);
    array = new long[size];
    for (int j = 0; j < size; j++) {
        getline(stream, tp, delim);
        array[j] = std::stoi(tp);
    }
    return size;
}

long read_double_array(std::fstream &stream,
                       double *&array) {

    string tp;

    char delim = ';';
    getline(stream, tp, delim);
    tp.erase(std::remove(tp.begin(), tp.end(), '\n'), tp.end());
    getline(stream, tp, delim);
    int size = std::stoi(tp);
    array = new double[size];
    for (int j = 0; j < size; j++) {
        getline(stream, tp, delim);
        array[j] = std::stod(tp);
    }

    return size;
}

long read_long(std::fstream &stream) {

    string tp;
    char delim = ';';
    getline(stream, tp, delim);
    tp.erase(std::remove(tp.begin(), tp.end(), '\n'), tp.end());

    getline(stream, tp, delim);
    getline(stream, tp, delim);

    return std::stoi(tp);
}
