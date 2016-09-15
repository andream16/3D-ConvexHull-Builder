#ifndef CONVEXHULL_H
#define CONVEXHULL_H
#define VERTEX_POINTERS_ARRAY(name) \
    std::vector<Dcel::Vertex*> name(std::vector<Dcel::Vertex*>)


/**Importing Eigen for matrix operations**/
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <GUI/managers/dcelmanager.h>
#include <lib/dcel/dcel_vertex_iterators.h>
#include <vector>
#include <string>
class ConvexHullBuilder{

public:
    ConvexHullBuilder(DrawableDcel* dcel);
    void computeConvexHull();

private:
    DrawableDcel *dcel;
    typedef std::vector<Dcel::Vertex*> VertexPointersList;
    typedef std::vector <Pointd> PointsVector;
    typedef Dcel::Vertex* Vertex;
    typedef Dcel::HalfEdge* HalfEdge;

    VERTEX_POINTERS_ARRAY(addVertices);
    VERTEX_POINTERS_ARRAY(verticesShuffler);
    PointsVector getFirstFourVertices(VertexPointersList);
    void buildTetrahedron(VertexPointersList);
    int coplanarityChecker(PointsVector);
    void tetrahedronMaker(PointsVector, int);
    void addFaceTotetrahedron(Vertex, HalfEdge);
};

#undef VERTEX_POINTERS_ARRAY
#endif // CONVEXHULL_H
