//
// Created by Nicolas Lantos on 31/05/2021.
//

#include "Edge.h"

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
