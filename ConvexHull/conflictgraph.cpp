#include "conflictgraph.h"
#include <stdlib.h>

#define VERTEX_POINTERS_LIST std::vector<Dcel::Vertex*>
#define POINTS_VECTOR std::vector <Pointd>
#define VERTEX Dcel::Vertex*
#define FACE Dcel::Face*

/**
 * @brief ConflictGraph::ConflictGraph()
 * @params takes dcel and tetrahedron vertices as input
 */
ConflictGraph::ConflictGraph(DrawableDcel* dcel, std::vector <Pointd> tetrahedronVertices){
    this->dcel = dcel;
    this->tetrahedronVertices = tetrahedronVertices;
}

/**
 * @brief ConflictGraph::initializeConflictGraph() initializes the conflict graph
 */
void ConflictGraph::initializeConflictGraph(){

}

#undef FACE
#undef VERTEX
#undef VERTEX_POINTERS_LIST
#undef POINTS_VECTOR
