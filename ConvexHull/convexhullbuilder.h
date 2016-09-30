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
    /**std::vector<Dcel::Vertex*> verticesShuffler(std::vector<Dcel::Vertex*>);

    std::vector <Pointd> getFirstFourVertices(std::vector<Dcel::Vertex*>);
    void buildTetrahedron(std::vector<Dcel::Vertex*>);
    int coplanarityChecker(std::vector <Pointd>);
    void tetrahedronMaker(std::vector <Pointd>, int);
    void addFaceTotetrahedron(Dcel::Vertex*, Dcel::HalfEdge*);**/
    void finalizeConvexHull(std::vector<Dcel::Vertex*>);
    std::vector<Dcel::HalfEdge*> bringMeTheHorizon(std::set<Dcel::Face*>*);

    std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> getCandidateVerticesMap(std::vector<Dcel::HalfEdge*>);
    void setTwins(std::vector<Dcel::Face*>);
    void removeVisibleFaces(std::set<Dcel::Face*>*);
    Dcel::Face* addFace(Dcel::Vertex* otherVertex, Dcel::HalfEdge* existingHe);

};

#undef VERTEX_POINTERS_ARRAY
#endif // CONVEXHULL_H
