//
// Created by Nicolas Lantos on 31/05/2021.
//

#ifndef COMMA_PROJECT_COMPSTRUCT_H
#define COMMA_PROJECT_COMPSTRUCT_H

#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include "Edge.h"

using namespace std;


class CompStruct {

public:
    unordered_map<string, unsigned short> ComponentType2Int = {{"bond",         0},
                                                               {"polygon",      1},
                                                               {"triconnected", 2}};
    vector<string> Int2ComponentType = {"bond", "polygon", "triconnected"};

    list<Edge> m_edges;
    short m_type;

public:

    CompStruct(short compType = -1);

    void add(Edge);
};


#endif //COMMA_PROJECT_COMPSTRUCT_H
