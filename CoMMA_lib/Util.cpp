//
// Created by Nicolas Lantos on 06/04/2021.
//
#include "Util.h"

void convert_agglomeration_lines_arrays_to_agglomeration_lines(
        const unsigned long fineAgglomerationLines_array_Idx_size,
        const unsigned long fineAgglomerationLines_array_size,
        long *fineAgglomerationLines_array_Idx,
        long *fineAgglomerationLines_array,
        long &nb_agglomeration_lines,
        forward_list<deque<long> *> *agglomeration_lines
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
            (*agglomeration_lines).push_front(dQue);
            nb_agglomeration_lines++;
        }
    }
}

void convert_agglo_lines_to_agglomeration_lines_arrays(const unsigned long nb_lines, // because forward_list does not have a size() method.
                                                       forward_list<deque<long> *> *agglo_lines,
                                                       long *sizes,  // out
                                                       long *agglo_lines_array_idx,// out
                                                       long *agglo_lines_array) {  // out
    // arrays agglo_lines_array_idx and agglo_lines_array are already allocated.
    // As we agglomerate cells, the number of agglo lines does not vary a priori, but they are getting smaller and smaller.

    if (nb_lines == 0) {

        sizes[0] = 1 + 1;
        sizes[1] = 0;
    }

    long number_of_fc_in_agglomeration_lines = 0;
    agglo_lines_array_idx[0] = 0;

    long i_l = 0;
    for (auto &line :(*agglo_lines)) {
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

void clean_agglomeration_lines(forward_list<deque<long> *> lines) {

    forward_list<deque<long> *>::iterator fLIt;
    // Deletion of pointer to deque:
    for (fLIt = lines.begin(); fLIt != lines.end(); fLIt++) {
        delete (*fLIt);
    }

}