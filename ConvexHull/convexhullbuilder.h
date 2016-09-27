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

#include "conflictgraph.h"

class ConvexHullBuilder{

public:
    ConvexHullBuilder(DrawableDcel* dcel); //Constructor Declaration
    ~ConvexHullBuilder(); //Destructor Declaration
    void computeConvexHull();

private:
    DrawableDcel *dcel;
    ConflictGraph *conflictGraph;

    typedef std::vector<Dcel::Vertex*> VertexPointersList;
    typedef std::vector <Pointd> PointsVector;
    typedef Dcel::Vertex* Vertex;
    typedef Dcel::HalfEdge* HalfEdge;
    typedef Dcel::Face* Face;

    VERTEX_POINTERS_ARRAY(addVertices);
    VERTEX_POINTERS_ARRAY(verticesShuffler);
    PointsVector getFirstFourVertices(VertexPointersList);
    void buildTetrahedron(VertexPointersList);
    int coplanarityChecker(PointsVector);
    void tetrahedronMaker(PointsVector, int);
    void addFaceTotetrahedron(Vertex, HalfEdge);
    VertexPointersList getVertices(VertexPointersList);
    void finalizeConvexHull(VertexPointersList);
    std::vector<HalfEdge> bringMeTheHorizon(std::set<Dcel::Face*>*);

};

#undef VERTEX_POINTERS_ARRAY
#endif // CONVEXHULL_H
