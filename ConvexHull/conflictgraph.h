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
    ConflictGraph(DrawableDcel* dcel, std::vector<Dcel::Vertex*> tetrahedronVertices);
    void initializeConflictGraph();

    std::map<Dcel::Face*, std::set<Dcel::Vertex*>*> VertexConflictList;
    std::map<Dcel::Vertex*, std::set<Dcel::Face*>*> FaceConflictList;

private:
    DrawableDcel *dcel;
    std::vector<Dcel::Vertex*> tetrahedronVertices;

    typedef std::vector <Pointd> PointsVector;
    typedef std::vector <Dcel::Face*> FaceArray;
    typedef Dcel::Vertex* Vertex;
    typedef Dcel::Face* Face;

    void checkVisibility();
    void fillCrossProductMatrix(Vertex);
    void crossProduct(Eigen::Matrix4d, Face, Vertex);
    void addFace(Face, Vertex);
    void addVertex(Face, Vertex);

};

#endif // CONFLICTGRAPH_H
