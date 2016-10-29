#include "conflictgraph.h"
#include <stdlib.h>

/**
 * @brief ConflictGraph::ConflictGraph() Constructor
 * @params takes dcel and tetrahedron vertices as input
 */
ConflictGraph::ConflictGraph(DrawableDcel* dcel, const std::vector<Dcel::Vertex*> &remainingVertices){
    this->dcel = dcel;
    this->remainingVertices = remainingVertices;
}

/**
 * @brief ConflictGraph Class Destructor
 **/
ConflictGraph::~ConflictGraph(){}

/**
 * @brief ConflictGraph::initializeConflictGraph() initializes the conflict graph
 */
void ConflictGraph::initializeConflictGraph(){
    //Checks which faces can be seen by a vertex
    checkVisibility();
}

/**
 * @brief ConflictGraph::checkVisibility loops through all the faces in the dcel, picks
 *        the outer half edge of each face, gets its from vertex, builds a matrix with
 *        the coordinates of each halfedge in a face and current vertex from dcel coordinates,
 *        checks if a vertex sees a face. If it does, both face and vertex are inserted into 2 maps.
 * @param takes the dcel
 */
void ConflictGraph::checkVisibility(){

    //Initialize 4*4 Matrix
    Eigen::Matrix4d matrix;

    //Number of Remaining vertices
    int verticesNumber = remainingVertices.size();

    //for each face in the dcel
    for(auto faceIterator = dcel->faceBegin(); faceIterator != dcel->faceEnd(); ++faceIterator){

        //used to fill the matrix
        int externalIterator = 0;

        //Pick current face
        Dcel::Face* currFace = *faceIterator;

        //Get current Outer Half Edge
        Dcel::HalfEdge* he = currFace->getOuterHalfEdge();

        //For each Vertex of the face
        for(unsigned int i = 0; i<3; i++){

          //Get Current From Vertex Coordinates
          Pointd currVertexCoordinates = he->getFromVertex()->getCoordinate();

          //Fill first 3 rows of the Matrix with currVertexCoordinates
          matrix(externalIterator, 0) = currVertexCoordinates.x();
          matrix(externalIterator, 1) = currVertexCoordinates.y();
          matrix(externalIterator, 2) = currVertexCoordinates.z();
          matrix(externalIterator, 3) = 1; //last column made of ones

          //Take Next Edge
          he = he->getNext();
          //Increment externalIterator
          externalIterator++;

        }

        //For each vertex in the dcel, check if vertex[j] sees current passed face
        for(unsigned int j=4; j<verticesNumber; j++){

          //Add current vertex coordinates to last row of the matrix
          matrix(3,0) = remainingVertices[j]->getCoordinate().x();
          matrix(3,1) = remainingVertices[j]->getCoordinate().y();
          matrix(3,2) = remainingVertices[j]->getCoordinate().z();
          matrix(3,3) = 1;

          //Compute Cross Product, if returns true -> addVertex and addFace
          if(crossProduct(matrix)){
            //if true, add passed vertex and face to the conflict maps
            addToVertexConflictMap(currFace, remainingVertices[j]);
            addToFaceConflictMap(currFace, remainingVertices[j]);
          }

        }


     }

}

/**
 * @brief crossProduct (Eigen::Matrix4d matrix)
 *        calculates determinant of the passed matrix and if it's <0, then the vertex sees the face, else if >0 it does not
 * @param Eigen::Matrix4d matrix current matrix
 */
bool ConflictGraph::crossProduct(Eigen::Matrix4d matrix){
    //if true -> <0, else false
    return (matrix.determinant() < -std::numeric_limits<double>::epsilon());
}

/**
 * @brief addToVertexConflictMap(Dcel::Face* face, Dcel::Vertex* vertex)
 *        checks if the map already exists for the current face, if not, it creates it,
 *        then, in any case, passed vertex is inserted into the set of vertices associated
 *        to it
 * @param Dcel::Face* face, Dcel::Vertex* vertex
 */
void ConflictGraph::addToVertexConflictMap(Dcel::Face* face, Dcel::Vertex* vertex){
        //If the map for the current face does not exist
        if(this->vertexConflictMap[face] == nullptr){
            //Create a new one for the passed face
            this->vertexConflictMap[face] = new std::set<Dcel::Vertex*>;
        }
          //Insert the current vertex in it
          this->vertexConflictMap[face]->insert(vertex);
}

/**
 * @brief addToFaceConflictMap(Dcel::Face* face, Dcel::Vertex* vertex)
 *        checks if the map already exists for the current vertex, if not, it creates it,
 *        then, in any case, passed face is inserted into the set of faces associated
 *        to it
 * @param Dcel::Face* face, Dcel::Vertex* vertex
 */
void ConflictGraph::addToFaceConflictMap(Dcel::Face* face, Dcel::Vertex* vertex){
    //If the map for the current vertex does not exist
    if(this->faceConflictMap[vertex] == nullptr){
        //create a new one for the passed vertex
        this->faceConflictMap[vertex] = new std::set<Dcel::Face*>;
    }
      //insert the current face in it
      this->faceConflictMap[vertex]->insert(face);
}

/**
 * @brief  getFacesVisibleByVertex(Dcel::Vertex* currentVertex)
 *         Check if passed vertex is in conflict with dcel's faces and if it is
 *         return set of faces visible by it
 * @param  Dcel::Vertex* currentVertex
 * @return std::set<Dcel::Face*> set of faces visible by the vertex
 */
std::set<Dcel::Face*>* ConflictGraph::getFacesVisibleByVertex(Dcel::Vertex* currentVertex){
    //Initialize set of Faces
    std::set<Dcel::Face*>* visibleFaces;
    //Check if the vertex is in conflict. If it is indeed, visible faces will not be empty
    visibleFaces = this->faceConflictMap[currentVertex];

    //If the Vertex is not in conflict
    if(visibleFaces->size() == 0){
        //Build and Empty set and associate it to the vertex in the map
        this->faceConflictMap[currentVertex] = new std::set<Dcel::Face*>;
    }

    //Return a set of visible faces by a vertex
    return visibleFaces;
}

/**
 * @brief  getVerticesVisibleByFace(Dcel::Face* currentFace)
 *         Check if passed face is in conflict with dcel's vertices and if it is, then,
 *         return set of vertices visible by it
 * @param  Dcel::Face* currentFace
 * @return std::set<Dcel::Vertex*>* set of vertices visible by the face
 */
std::set<Dcel::Vertex*>* ConflictGraph::getVerticesVisibleByFace(Dcel::Face* currentFace){
    //Initialize set of Faces
    std::set<Dcel::Vertex*>* visibleVertices;
    //Check if the vertex is in conflict. If it is indeed, visible faces will not be empty
    visibleVertices = this->vertexConflictMap[currentFace];

    //If the Vertex is not in conflict
    if(visibleVertices->size() == 0){
        //Build and Empty set and associate it to the vertex in the map
        this->vertexConflictMap[currentFace] = new std::set<Dcel::Vertex*>;
    }

    //Return a set of visible faces by a vertex
    return visibleVertices;

}

/**
 * @brief  joinVertices(std::vector<Dcel::HalfEdge*>* horizon)
 *         Merges the vertices that are in conflict with a given horizon halfedge face and twin's face
 *         in order to speed up the process of understanding which vertices are in conflict with the new faces
 *         to be added
 * @param  std::vector<Dcel::HalfEdge*>* horizon
 * @return std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>> map that for each HalfEdge has the latter merged vertices
 */
std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> ConflictGraph::joinVertices(std::vector<Dcel::HalfEdge*> horizon){

    //Initialize joined vertices map
    std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> joinedVerticesMap;

        //For each HalfEdge in the horizon
        for(auto halfEdgeIterator = horizon.begin(); halfEdgeIterator != horizon.end(); halfEdgeIterator++){
            //Get current HalfEdge
            Dcel::HalfEdge *currHorizonHalfEdge = *halfEdgeIterator;
            //Get current HalfEdge's twin
            Dcel::HalfEdge *currTwin            =  currHorizonHalfEdge->getTwin();

            //Get its face
            Dcel::Face *currentFace = (*halfEdgeIterator)->getFace();
            //Get twin's face
            Dcel::Face *twinsFace   = currTwin->getFace();

            //Get Vertices visible by the latter faces
            std::set<Dcel::Vertex*> *verticesInConflictWithCurrFace  = getVerticesVisibleByFace(currentFace);
            std::set<Dcel::Vertex*> *verticesInConflictWithTwinsFace = getVerticesVisibleByFace(twinsFace);

            //Join the two sets of vertices
            verticesInConflictWithCurrFace->insert(verticesInConflictWithTwinsFace->begin(), verticesInConflictWithTwinsFace->end());

            //Build the map with new vertices
            joinedVerticesMap[currHorizonHalfEdge] = verticesInConflictWithCurrFace;
        }

    //Return populated Map
    return joinedVerticesMap;

}

/**
 * @brief  deleteFaces(std::set<Dcel::Face*>* visibleFaces)
 *         Deletes all the visible faces by the current vertex from the Conflict Graph,
 *         Removes their Halfedges from the dcel and their vertices if there are no more
 *         Connecting Halfedges between a From and a To Vertex
 * @param  std::set<Dcel::Face*>* visibleFaces set of visible faces by current vertex
 */
void ConflictGraph::deleteFaces(std::set<Dcel::Face*>* visibleFaces){
    //Loop through all the faces
    for( auto faceIterator = visibleFaces->begin(); faceIterator != visibleFaces->end(); faceIterator++ ){
        //Get Current Face
        Dcel::Face* currentFace = *faceIterator;
        /** Delete from Conflict Graph Begin **/
        //Get All the Vertices visible by the current face
        std::set<Dcel::Vertex*> *verticesVisibleByFace = this->vertexConflictMap[currentFace];
        //If there are visible vertices indeed
        if( verticesVisibleByFace->size() > 0 ){
            //Delete current Face From the Conflict Graph
            this->vertexConflictMap.erase(currentFace);
            //For each visible vertex
            for( auto currVisibleVertex = verticesVisibleByFace->begin(); currVisibleVertex != verticesVisibleByFace->end(); currVisibleVertex++ ){
                //Get current Visible Vertex
                Dcel::Vertex* cVisibleVertex = *currVisibleVertex;
                //Delete Current Face from all the Maps of the current Vertex
                auto currentSet = this->faceConflictMap[cVisibleVertex];
                //If it is not empty
                if( currentSet->size() > 0 ){
                    //Delete it
                    currentSet->erase(currentFace);
                }
            }
        }
        /** Delete from Conflict Graph End **/

        /** Delete from Dcel Begin **/
        //For each halfedge in the face
        for( auto halfEdgeIterator = currentFace->incidentHalfEdgeBegin(); halfEdgeIterator != currentFace->incidentHalfEdgeEnd(); halfEdgeIterator++ ){
            //Get Current HalfEdge
            Dcel::HalfEdge* currHalfEdge = *halfEdgeIterator;
            //Get its From and To Vertex
            Dcel::Vertex* fromVertex = currHalfEdge->getFromVertex();
            Dcel::Vertex* toVertex   = currHalfEdge->getToVertex();
            //Remove The HalfEdge from The Dcel
            this->dcel->deleteHalfEdge(currHalfEdge);
            //Decrement its From and To Vertices Cardinalities since we have removed an HalfEdge
            fromVertex->decrementCardinality();
            toVertex  ->decrementCardinality();
            //When Cardinality is 0, it means that the Vertex Has no connections, so, we can remove it
            if( fromVertex->getCardinality() == 0 ){
                this->dcel->deleteVertex(fromVertex);
            }
            if( toVertex->getCardinality() == 0 ){
                this->dcel->deleteVertex(toVertex);
            }
        }
        /** Delete from Dcel End   **/
    }
}
