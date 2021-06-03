//
// Created by Nicolas Lantos on 31/05/2021.
//

#ifndef COMMA_PROJECT_EDGE_H
#define COMMA_PROJECT_EDGE_H

#include <unordered_set>
#include <string>

using namespace std;

class Edge {
public :
    long source;
    long target;
    unordered_set<long> s_e;
    unsigned short type;
    bool start = false;
    long index;

public:
    Edge();

    Edge(long i_v, long i_w, long ind = -1, unsigned short type = 0);

    void reverse();

    void initialize(long i_v, long i_w, long ind, unsigned short type);

    bool operator==(Edge e) {
        if (s_e == e.s_e)
            return true;
        else
            return false;
    }

    bool operator==(const Edge e) const {
        if (s_e == e.s_e)
            return true;
        else
            return false;
    }

    bool operator!=(Edge e) {
        if (s_e != e.s_e)
            return true;
        else
            return false;
    }

    Edge &operator=(Edge const &e);
};

//struct edge_hash {
//    inline std::size_t operator()(const Edge &e) const {
//        return e.source*e.target * 31 + e.target+ e.source;
//    }
//};

#endif //COMMA_PROJECT_EDGE_H
