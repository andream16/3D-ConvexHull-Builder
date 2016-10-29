#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <vector>
#include <string>
#include <GUI/managers/dcelmanager.h>
#include <lib/dcel/dcel_vertex_iterators.h>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>

#include "tetrahedronbuilder.h"
#include "conflictgraph.h"

class ConvexHullBuilder{

public:
    ConvexHullBuilder(DrawableDcel* dcel); //Constructor Declaration
    ~ConvexHullBuilder(); //Destructor Declaration
    void computeConvexHull();

private:
    DrawableDcel *dcel;
    ConflictGraph *conflictGraph;
    TetrahedronBuilder *tetrahedronBuilder;

    std::vector<Dcel::Vertex*> getAllVertices();
    std::vector<Dcel::HalfEdge*> bringMeTheHorizon(std::set<Dcel::Face*>*);
    std::vector<Dcel::HalfEdge*> orderHorizon(std::vector<Dcel::HalfEdge*>, std::map<Dcel::Vertex*, Dcel::Vertex*>);
    void buildNewFace(std::vector<Dcel::HalfEdge*>, Dcel::Vertex*, std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*>);
    Dcel::Face* craftNewFace(Dcel::HalfEdge*, Dcel::Vertex*);
    void setTwins(std::vector<Dcel::Face*> &faceArray);
};

#endif // CONVEXHULL_H
