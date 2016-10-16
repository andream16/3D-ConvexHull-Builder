#ifndef CONVEXHULL_H
#define CONVEXHULL_H
#define VERTEX_POINTERS_ARRAY(name) \
    std::vector<Dcel::Vertex*> name(std::vector<Dcel::Vertex*>)

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
    bool checkIfHalfEdgeExist(Dcel::HalfEdge*);

};

#undef VERTEX_POINTERS_ARRAY
#endif // CONVEXHULL_H
