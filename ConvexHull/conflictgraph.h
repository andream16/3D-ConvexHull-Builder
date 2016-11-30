#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include <tr1/unordered_set>
#include <tr1/unordered_map>
#include "lib/dcel/drawable_dcel.h"

class ConflictGraph{

public:
    ConflictGraph(DrawableDcel* dcel, const std::vector<Dcel::Vertex*> &remainingVertices); // Class Constructor
    ~ConflictGraph(); //Destructor Declaration
    void initializeConflictGraph();

    std::tr1::unordered_set<Dcel::Face*>* getFacesVisibleByVertex (Dcel::Vertex* &currentVertex);
    std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>*> joinVertices(std::vector<Dcel::HalfEdge*> horizon);
    void deleteFaces(std::tr1::unordered_set<Dcel::Face*>*);
    void checkConflict(std::vector<Dcel::Face*>, std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>*>, std::vector<Dcel::HalfEdge*>);
    void eraseVertex(Dcel::Vertex*);

    std::tr1::unordered_map<Dcel::Face*,   std::tr1::unordered_set<Dcel::Vertex*>*> vertexConflictMap;
    std::tr1::unordered_map<Dcel::Vertex*, std::tr1::unordered_set<Dcel::Face*>*>   faceConflictMap;

private:
    DrawableDcel *dcel;
    std::vector<Dcel::Vertex*> remainingVertices;
    void halfSpaceChecker(Dcel::Face* face, Dcel::Vertex* vertex);
    void addToFaceConflictMap(Dcel::Face* face, Dcel::Vertex* vertex);
    void addToVertexConflictMap(Dcel::Face* face, Dcel::Vertex* vertex);
    std::tr1::unordered_set<Dcel::Vertex*>* getVerticesVisibleByFace(Dcel::Face* currentFace);

};

#endif // CONFLICTGRAPH_H
