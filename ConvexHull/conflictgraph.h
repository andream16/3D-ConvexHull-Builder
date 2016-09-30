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


    std::map<Dcel::Face*, std::set<Dcel::Vertex*>*> vertexConflictMap;
    std::map<Dcel::Vertex*, std::set<Dcel::Face*>*> faceConflictMap;


private:
    DrawableDcel *dcel;
    const std::vector<Dcel::Vertex*> remainingVertices;

    void checkVisibility();
    void fillCrossProductMatrix(Dcel::Vertex*);
    bool crossProduct(Eigen::Matrix4d);
    void addToFaceConflictMap(Dcel::Face*, Dcel::Vertex*);
    void addToVertexConflictMap(Dcel::Face*, Dcel::Vertex*);

    bool checkV(Dcel::Face*, Dcel::Vertex*);
    Pointd getFaceNormalDirection(Dcel::Face*);

};

#endif // CONFLICTGRAPH_H
