#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include "lib/dcel/drawable_dcel.h"
#include <string>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>


class ConflictGraph{

public:
    ConflictGraph(DrawableDcel* dcel, std::vector<Dcel::Vertex*> tetrahedronVertices); // Class COnstructor
    ~ConflictGraph(); //Destructor Declaration
    void initializeConflictGraph();
    std::set<Dcel::Face*>* lookForVisibleFaces(Dcel::Vertex*);

    std::map<Dcel::Face*, std::set<Dcel::Vertex*>*> vertexConflictList;
    std::map<Dcel::Vertex*, std::set<Dcel::Face*>*> faceConflictList;

private:
    DrawableDcel *dcel;
    std::vector<Dcel::Vertex*> remainingVertices;

    typedef std::vector <Pointd> PointsVector;
    typedef std::vector <Dcel::Face*> FaceArray;
    typedef Dcel::Vertex* Vertex;
    typedef Dcel::Face* Face;
    typedef std::vector<Dcel::Vertex*> VertexPointersList;
    typedef Dcel::HalfEdge* HalfEdge;


    void checkVisibility();
    void fillCrossProductMatrix(Vertex);
    void crossProduct(Eigen::Matrix4d, Face, Vertex);
    void addFace(Face, Vertex);
    void addVertex(Face, Vertex);


};

#endif // CONFLICTGRAPH_H
