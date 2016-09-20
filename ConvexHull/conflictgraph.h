#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include "lib/dcel/drawable_dcel.h"
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <GUI/managers/dcelmanager.h>
#include <lib/dcel/dcel_vertex_iterators.h>
#include <vector>
#include <string>

class ConflictGraph{

public:
    ConflictGraph(DrawableDcel* dcel, std::vector<Pointd> tetrahedronVertices);
    void initializeConflictGraph();

private:
    DrawableDcel *dcel;
    std::vector<Pointd> tetrahedronVertices;
};

#endif // CONFLICTGRAPH_H
