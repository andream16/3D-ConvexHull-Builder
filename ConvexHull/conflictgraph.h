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

    std::set<Dcel::Face*>* lookForVisibleFaces(Dcel::Vertex*);
    std::set<Dcel::Vertex*> getVisibleFaces(Dcel::Face*);
    std::set<Dcel::Vertex*>* getVisibleVertices(Dcel::Face*);

    void updateConflictGraph(Dcel::Face*, std::set<Dcel::Vertex*>*);
    void deleteFaces(std::set<Dcel::Face*>*);

    void deletePoint(Dcel::Vertex*);


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

    bool checkV(Face, Vertex);
    Pointd getFaceNormalDirection(Dcel::Face*);

};

#endif // CONFLICTGRAPH_H
