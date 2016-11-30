#include "conflictgraph.h"
#include <stdlib.h>

/**
 * @brief ConflictGraph::ConflictGraph()
 * @params DrawableDcel* dcel, const std::vector<Dcel::Vertex*> &remainingVertices
 *         Both std::tr1::unordered_set and map are used because they are faster that normal std::map and std::set.
 *         Also, we don't need to keep the order inside our maps and sets, so, we can use unordered sets/maps.
 */
ConflictGraph::ConflictGraph(DrawableDcel* dcel, const std::vector<Dcel::Vertex*> &remainingVertices){
    this->dcel = dcel;
    this->remainingVertices = remainingVertices;
}

/**
 * @brief ConflictGraph Class Destructor
 **/
ConflictGraph::~ConflictGraph(){
    faceConflictMap.clear();
    vertexConflictMap.clear();
}

/**
 * @brief ConflictGraph::initializeConflictGraph() initializes the conflict graph
 */
void ConflictGraph::initializeConflictGraph(){

    //Number of Remaining vertices
    int verticesNumber = remainingVertices.size();

    //For each face in the dcel
    for(auto faceIterator = dcel->faceBegin(); faceIterator != dcel->faceEnd(); ++faceIterator){

        //For Each Remaining Vertex
        for(int i = 0; i < verticesNumber; i++){
            Dcel::Vertex* currVertex = remainingVertices[i];
            //Check if they lie on the same half-space
            halfSpaceChecker(*faceIterator, currVertex);
        }

    }
}


/**
 * @brief ConflictGraph::halfSpaceChecker(Dcel::Face* face, Dcel::Vertex* vertex)
 *        Since all the vertices of a face lie on the same plain, we take one of them, for instance, the first
 *        and calculate the difference between it and the passed vertex to get checked.
 *        Then, we calculate all the arguments of the normal to the face and set them as coordinates to the normal itselfs.
 *        Then we calculate the dot product between the latter vertex and the normal.
 *        - If the dot product is > epsilon (in order to avoid floating point errors) then the vertex and the face
 *          lie on the very same half-space, implying that the vertex is in front of the face
 *        - Else we don't care because it means that they are not on the same half-space.
 * @param Dcel::Face* const &face, Dcel::Vertex* const &vertex
 */
void ConflictGraph::halfSpaceChecker(Dcel::Face* face, Dcel::Vertex* vertex){
    //Array to contain all the vertices of the face
    std::vector<Dcel::Vertex*> faceVertices;
    //Initialize faceNormal
    Dcel::Vertex* faceNormal = new Dcel::Vertex;
    //For each Vertex in the face
    for( auto vertexIterator = face->incidentVertexBegin(); vertexIterator != face->incidentVertexEnd(); vertexIterator++ ){
        //Add it to the array
        faceVertices.push_back(*vertexIterator);
    }

    //Getting Temp vector
    auto tmpVector = vertex->getCoordinate() - faceVertices[0]->getCoordinate();

    //Getting Face Arguments per each coordinate
    auto a = (
                ((faceVertices[1]->getCoordinate().y() - faceVertices[0]->getCoordinate().y())*(faceVertices[2]->getCoordinate().z()-faceVertices[0]->getCoordinate().z()))-
            ((faceVertices[2]->getCoordinate().y() - faceVertices[0]->getCoordinate().y())*(faceVertices[1]->getCoordinate().z()-faceVertices[0]->getCoordinate().z()))
            );

    auto b = (
                ((faceVertices[1]->getCoordinate().z() - faceVertices[0]->getCoordinate().z())*(faceVertices[2]->getCoordinate().x()-faceVertices[0]->getCoordinate().x()))-
            ((faceVertices[2]->getCoordinate().z() - faceVertices[0]->getCoordinate().z())*(faceVertices[1]->getCoordinate().x()-faceVertices[0]->getCoordinate().x()))
            );

    auto c = (
                ((faceVertices[1]->getCoordinate().x() - faceVertices[0]->getCoordinate().x())*(faceVertices[2]->getCoordinate().y()-faceVertices[0]->getCoordinate().y()))-
            ((faceVertices[2]->getCoordinate().x() - faceVertices[0]->getCoordinate().x())*(faceVertices[1]->getCoordinate().y()-faceVertices[0]->getCoordinate().y()))
            );

    //Setting faceNormal coordinates
    faceNormal->setCoordinate(Pointd(a, b, c));

    //Calculate Dot Product
    auto dotProduct = tmpVector.dot(faceNormal->getCoordinate());

    //If dotProduct > epsilon
    if( dotProduct > std::numeric_limits<double>::epsilon() ){
        /* Current vertex and current face lie on the same half-space, implying
         * that the vertex is in front of the face, so they are in conflict */
        addToVertexConflictMap(face, vertex);
        addToFaceConflictMap  (face, vertex);
    }
}

/**
 * @brief addToVertexConflictMap(Dcel::Face* face, Dcel::Vertex* vertex) *
 *        checks if, for that face, the map exists for that key entry,
 *        if not, a new set is created then, in any case, passed vertex is inserted into it
 *        at face's position
 * @param Dcel::Face* const &face, Dcel::Vertex* const &vertex
 */
void ConflictGraph::addToVertexConflictMap(Dcel::Face* face, Dcel::Vertex* vertex) {
    //If at current face's position there's nothing
    if( this->vertexConflictMap[face] == nullptr ){
        //Initialize a new unordered_set of vertices and assign it to face's position
        this->vertexConflictMap[face] = new std::tr1::unordered_set<Dcel::Vertex*>;
    }
    //In any case, Insert the current vertex in it
    this->vertexConflictMap[face]->insert(vertex);
}

/**
 * @brief addToFaceConflictMap(Dcel::Face* face, Dcel::Vertex* vertex)
 *        checks if, for that vertex, the map exists for that key entry,
 *        if not, a new set is created then, in any case, passed face is inserted into it
 *        at vertex's position
 * @param Dcel::Face* const &face, Dcel::Vertex* const &vertex
 */
void ConflictGraph::addToFaceConflictMap(Dcel::Face* face, Dcel::Vertex* vertex) {
    //If at current vertex's position there's nothing
    if( this->faceConflictMap[vertex] == nullptr ){
        //Initialize a new unordered_set of faces and assign it to vertex's position
        this->faceConflictMap[vertex] = new std::tr1::unordered_set<Dcel::Face*>;
    }
    //In any case, Insert the current face in it
    this->faceConflictMap[vertex]->insert(face);
}

/**
 * @brief  getFacesVisibleByVertex(Dcel::Vertex* &currentVertex)
 *         Checks if passed vertex is in conflict with dcel's faces and if it indeed is in conflict,
 *         return set of faces visible by it
 * @param  Dcel::Vertex* const &currentVertex
 * @return std::tr1::unordered_set<Dcel::Face*> set of faces visible by the vertex
 */
std::tr1::unordered_set<Dcel::Face*>* ConflictGraph::getFacesVisibleByVertex(Dcel::Vertex* &currentVertex){

    //Check if the vertex is in conflict. If it is indeed, visible faces will not be nullptr
    std::tr1::unordered_set<Dcel::Face*>* visibleFaces = this->faceConflictMap[currentVertex];

    //If the Vertex is not in conflict
    if( visibleFaces == nullptr ){
        //Build a new set and put it at vertex's position
        visibleFaces = new std::tr1::unordered_set<Dcel::Face*>;
        this->faceConflictMap[currentVertex] = visibleFaces;
    }

    //Return a set of visible faces by the vertex
    return new std::tr1::unordered_set<Dcel::Face*> (*visibleFaces);
}

/**
* @brief  joinVertices(std::vector<Dcel::HalfEdge*>* std::vector<Dcel::HalfEdge*> horizon)
*         Merges the vertices that are in conflict with a given horizon halfedge face and twin's face
*         in order to speed up the process of understanding which vertices are in conflict with the new faces
*         to be added
* @param  const std::vector<Dcel::HalfEdge*> &horizon) const
* @return std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>> map that for each HalfEdge has the latter merged vertices
*/
std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>*> ConflictGraph::joinVertices( std::vector<Dcel::HalfEdge*> horizon){

    //Initialize joined vertices map
    std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>*> joinedVerticesMap;

    //For each HalfEdge in the horizon
    for(auto halfEdgeIterator = horizon.begin(); halfEdgeIterator != horizon.end(); halfEdgeIterator++){
        //Get current HalfEdge's twin
        Dcel::HalfEdge *currTwin=  (*halfEdgeIterator)->getTwin();
        //Get its face
        Dcel::Face *currentFace = (*halfEdgeIterator)->getFace();
        //Get twin's face
        Dcel::Face *twinsFace   = currTwin->getFace();

        //Get Vertices visible by the latter faces
        std::tr1::unordered_set<Dcel::Vertex*> *verticesInConflictWithCurrFace  = getVerticesVisibleByFace(currentFace);
        std::tr1::unordered_set<Dcel::Vertex*> *verticesInConflictWithTwinsFace = getVerticesVisibleByFace(twinsFace);

        //Join the two sets of vertices
        verticesInConflictWithCurrFace->insert(verticesInConflictWithTwinsFace->begin(), verticesInConflictWithTwinsFace->end());

        //Build the map with new vertices
        joinedVerticesMap[*halfEdgeIterator] = verticesInConflictWithCurrFace;
    }

    //Return populated Map
    return joinedVerticesMap;

}

/**
 * @brief  getVerticesVisibleByFace(Dcel::Face* currentFace)
 *         Check if passed face is in conflict with dcel's vertices and if it is, then,
 *         return set of vertices visible by it
 * @param  const Dcel::Face* &currentFace
 * @return std::tr1::unordered_set<Dcel::Vertex*>* set of vertices visible by the face
 */
std::tr1::unordered_set<Dcel::Vertex*>* ConflictGraph::getVerticesVisibleByFace(Dcel::Face* currentFace){
    //Check if the vertex is in conflict. If it is indeed, visible faces will not be nullptr
    std::tr1::unordered_set<Dcel::Vertex*>* visibleVertices = this->vertexConflictMap[currentFace];

    //If the Vertex is not in conflict
    if( visibleVertices == nullptr ){
        //Build a new set and associate put it at face's position
        visibleVertices = new std::tr1::unordered_set<Dcel::Vertex*>;
        this->vertexConflictMap[currentFace] = visibleVertices;
    }

    //Return a set of visible vertices by the face
    return new std::tr1::unordered_set<Dcel::Vertex*> (*visibleVertices);

}

/**
 * @brief  deleteFaces(std::tr1::unordered_set<Dcel::Face*>* visibleFaces)
 *         Deletes all the visible faces by the current vertex from the Conflict Graph,
 *         Removes their Halfedges from the dcel and their vertices if there are no more
 *         Connecting Halfedges between a From and a To Vertex
 * @param  std::tr1::unordered_set<Dcel::Face*>* visibleFaces set of visible faces by current vertex
 */
void ConflictGraph::deleteFaces(std::tr1::unordered_set<Dcel::Face*>* visibleFaces){
    //Loop through all the faces
    for( auto faceIterator = visibleFaces->begin(); faceIterator != visibleFaces->end(); faceIterator++ ){

        //Get Current Face
        Dcel::Face* currFace = *faceIterator;

        /** Delete from Conflict Graph Begin **/
        //Get All the Vertices visible by the current face
        std::tr1::unordered_set<Dcel::Vertex*> *verticesVisibleByFace = getVerticesVisibleByFace(currFace);
        //If there are visible vertices indeed
        if( verticesVisibleByFace != nullptr ){
            //Delete current Face From the vertexConflictMap
            this->vertexConflictMap.erase(currFace);
            //For each visible vertex
            for( auto currVisibleVertex = verticesVisibleByFace->begin(); currVisibleVertex != verticesVisibleByFace->end(); currVisibleVertex++ ){
                //Get Current Visible Vertex
                Dcel::Vertex* currVertex = *currVisibleVertex;
                //Delete current face at current vertex's position in faceConflictMap
                this->faceConflictMap[currVertex]->erase(currFace);
            }
        }
        /** Delete from Conflict Graph End **/

        /** Delete from Dcel Begin **/
        //For each halfedge in the face
        for( auto halfEdgeIterator = (*faceIterator)->incidentHalfEdgeBegin(); halfEdgeIterator != (*faceIterator)->incidentHalfEdgeEnd(); halfEdgeIterator++ ){

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

            //When Cardinality is 0, it means that the Vertex Has no connections, so, we can safely remove it
            if( fromVertex->getCardinality() == 0 ){
                this->dcel->deleteVertex(fromVertex);
            }
            if( toVertex->getCardinality() == 0 ){
                this->dcel->deleteVertex(toVertex);
            }
        }

        //Delete the current face to close the loop
        this->dcel->deleteFace(currFace);

        /** Delete from Dcel End   **/
    }
}

/**
 * @brief  ConflictGraph::checkConflict(std::vector<Dcel::Face*> faces, std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>*> oldVertices, std::vector<Dcel::HalfEdge*> horizon)
 *         For each new Face, check if they are in conflict with the old vertices from the latter destroyed face
 * @param  std::vector<Dcel::Face*> faces, std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>*> oldVertices, std::vector<Dcel::HalfEdge*> horizon
 */
void ConflictGraph::checkConflict(std::vector<Dcel::Face*> faces, std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>*> oldVertices, std::vector<Dcel::HalfEdge*> horizon){
    //For each face in faces
    for( auto faceIterator = faces.begin(); faceIterator != faces.end(); faceIterator++){
        //For each halfedge in the horizon
        for( auto halfEdgeIterator = horizon.begin(); halfEdgeIterator != horizon.end(); halfEdgeIterator++){
            //Get Set Of vertices associated to the current horizon halfEdge
            std::tr1::unordered_set<Dcel::Vertex*> *currVertexSet = oldVertices[*halfEdgeIterator];
            //For each vertex in the possible conflict ones
            for( auto vertexIterator = currVertexSet->begin(); vertexIterator != currVertexSet->end(); vertexIterator++ ){
                //Check if they lie on the same half-space, if so, update the Conflict Graph
                halfSpaceChecker(*faceIterator, *vertexIterator);
            }
        }
    }
}

/**
 * @brief  ConflictGraph::eraseVertex(Dcel::Vertex* vertex)
 *         Delete Current Vertex from the conflict Graph
 * @param  Dcel::Vertex* vertex
 */
void ConflictGraph::eraseVertex(Dcel::Vertex* vertex){
    //Get All Visible Faces by the passed Vertex
    std::tr1::unordered_set<Dcel::Face*> *visibleFaces = getFacesVisibleByVertex(vertex);

    //Remove it from the faceConflictMap
    this->faceConflictMap.erase(vertex);

    //For each visible face
    for( auto faceIterator = visibleFaces->begin(); faceIterator != visibleFaces->end(); faceIterator++ ){
        //Delete the vertex from vertexConflictMap for each face which was in conflict with it
        this->vertexConflictMap[(*faceIterator)]->erase(vertex);
    }

}
