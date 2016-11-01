#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include "lib/dcel/drawable_dcel.h"
#include <string>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>


class ConflictGraph{

public:
    ConflictGraph(DrawableDcel* dcel, const std::vector<Dcel::Vertex*> &remainingVertices); // Class Constructor
    ~ConflictGraph(); //Destructor Declaration
    void initializeConflictGraph();

    std::set<Dcel::Face*>* getFacesVisibleByVertex (Dcel::Vertex*);
    std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> joinVertices(std::vector<Dcel::HalfEdge*>);
    void deleteFaces(std::set<Dcel::Face*>*);

    std::map<Dcel::Face*,   std::set<Dcel::Vertex*>*> vertexConflictMap;
    std::map<Dcel::Vertex*, std::set<Dcel::Face*>*>   faceConflictMap;

private:
    DrawableDcel *dcel;
    std::vector<Dcel::Vertex*> remainingVertices;

    void fillCrossProductMatrix(Dcel::Vertex*);
    bool crossProduct(Eigen::Matrix4d);
    void addToFaceConflictMap(Dcel::Face*, Dcel::Vertex*);
    void addToVertexConflictMap(Dcel::Face*, Dcel::Vertex*);
    std::set<Dcel::Vertex*>* getVerticesVisibleByFace(Dcel::Face*);

};

#endif // CONFLICTGRAPH_H
