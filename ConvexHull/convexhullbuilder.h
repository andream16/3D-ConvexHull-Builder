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

    VERTEX_POINTERS_ARRAY(addVertices);
    VERTEX_POINTERS_ARRAY(verticesShuffler);
    PointsVector getFirstFourVertices(VertexPointersList);
    void buildTetrahedron(VertexPointersList);
    bool coplanarityChecker(PointsVector);
};

#undef MY_PREFIX_DECL_FN
#endif // CONVEXHULL_H
