//
// Created by Nicolas Lantos on 31/05/2021.
//

#include "CompStruct.h"

CompStruct::CompStruct(short compType) {
    m_type = compType;
}

void CompStruct::add(Edge *e) {
    m_edges.push_back(e);

}

void CompStruct::finishTricOrPoly(Edge *edge) {

    m_edges.push_back(edge);
    if (m_edges.size() >= 4) {
        m_type = 2;
    } else {
        m_type = 1;
    }
}