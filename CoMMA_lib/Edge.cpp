//
// Created by Nicolas Lantos on 31/05/2021.
//

#include "Edge.h"

Edge::Edge() {
    source = -1;
    target = -1;
}

Edge::Edge(long i_v, long i_w, long ind, unsigned short type) {
    source = i_v;
    target = i_w;
    s_e = {i_v, i_w};
    this->type = type;
    this->start = false;
    index = ind;
}

void Edge::reverse() {
    long tmp = source;
    source = target;
    target = tmp;

}

void Edge::initialize(long i_v, long i_w, long ind, unsigned short type) {
    source = i_v;
    target = i_w;
    s_e = {i_v, i_w};
    this->type = type;
    this->start = false;
    index = ind;
}

// TODO avoid copy???
Edge &Edge::operator=(Edge const &e) {

    if (&e == this) {
        return *this;
    }
    source = e.source;
    target = e.target;
    s_e = {e.source, e.target};
    type = e.type;
    start = e.start;
    index = e.index;
    return *this;
};

void Edge::print() const {
    cout << "(" << source << ", " << target << ", " << index << ", " << type << ") ";
}

void Edge::print_endl() const {
    cout << "<" << source << ", " << target << "> " << "[" << index << "]" << endl;
}