#include "convexhullbuilder.h"
#include <iostream>
#include <stdlib.h>

/**
 * @brief ConvexHullBuilder::ConvexHullBuilder() Constructor
 * @params takes passed dcel
 */
ConvexHullBuilder::ConvexHullBuilder(DrawableDcel* dcel){
   this->dcel = dcel;
}

/**
 * @brief ConvexHullBuilder Class Destructor
 **/
ConvexHullBuilder::~ConvexHullBuilder(){}

/**
 * @brief ConvexHullBuilder::computeConvexHull() takes dcel as input.
 *        Starts the algorithm calling all the different functions needed.
 *         - Takes all the vertices from the Dcel and saves them into a vector of vertices
 *         - Resets the dcel
 *         - Calls tetrahedron's buildTetrahedron method in order to build the latter with the passed vertices
 *         - Initializes Conflict Graph with, respectively, Vertices visible froma Face and Faces visible froma Vertex
 *         - For each remaining vertex, checks if the vertex is in conflict with a certain number of faces, if it is in conflict then:
 *           - Gets the Horizon for the faces visible by the latter vertex
 *           - Gets Candidate Vertex Map for the passed horizon (vector of halfedges)
 *           - Deletes visible faces
 *           - For each halfedge in the Horizon
 *             - Builds a new face using the current halfedge and the passed vertex
 *             - Updates the Conflict Graph with new informations
 *           - Sets Twins for the new faces created
 *           - Deletes the passed Vertex
 *
 */
void ConvexHullBuilder::computeConvexHull(){

    //Get all the vertices from the dcel
    std::vector<Dcel::Vertex*> dcelVertices = getAllVertices();

    //Reset Dcel
    dcel->reset();

    //Build Tetrahedron with the dcel and all vertices
    tetrahedronBuilder = new TetrahedronBuilder(dcel, dcelVertices);
    tetrahedronBuilder->buildTetrahedron();

    //Initialize Conflict Graph with Dcel And First 4 Vertices
    conflictGraph = new ConflictGraph(dcel, dcelVertices);
    conflictGraph->initializeConflictGraph();

    //Get all vertices size
    int verticesSize = dcelVertices.size();

    //Loop through remaining vertices
    for(unsigned int i=4; i<verticesSize; i++){

      //Get Current Vertex
      Dcel::Vertex* currVert = dcelVertices[i];

      //Check if current vertex is in conflict with dcel's faces
      std::set<Dcel::Face*>* facesVisibleByVertex = conflictGraph->getFacesVisibleByVertex(currVert);

      //If the vertex is in conflict with dcel's faces
      if(facesVisibleByVertex->size() > 0){

          //Add it to the Dcel
         dcel->addVertex(currVert->getCoordinate());

          //Get the Horizon for the current Visible Faces
          std::vector<Dcel::HalfEdge*> horizon = bringMeTheHorizon(facesVisibleByVertex);

          //Get the probable visible vertices for each face of each horizon's halfedge
          std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> probableVertices = conflictGraph->joinVertices(horizon);

          //Delete Visible Faces Faces from the Conflict Graph and Dcel
          conflictGraph->deleteFaces(facesVisibleByVertex);

          //Build a new Face for each HalfEdge in the Horizon and for the Current vertex
          buildNewFace(horizon, currVert, probableVertices);
      }

      bool fasulllooooo = true;
      conflictGraph->deletePoint(currVert);

    }
}


/**
 * @brief  std::vector<Dcel::Vertex*> ConvexHullBuilder::getAllVertices
 *         Takes all vertices from the Dcel and saves them into an arra of pointers to vertices
 * @return returns array of pointers to all vertices
 */
std::vector<Dcel::Vertex*>  ConvexHullBuilder::getAllVertices(){
    //Initiale vertices array
    std::vector<Dcel::Vertex*> allVertices;
    //Add all vertices to the latter
    for (auto vertex = dcel->vertexBegin(); vertex != dcel->vertexEnd(); vertex++){
        Dcel::Vertex* v = new Dcel::Vertex((*vertex)->getCoordinate());
        allVertices.push_back(v);
        //If last element then free the memory
        if(vertex == dcel->vertexEnd()){
          delete v;
        }
    }
    //Return array of pointers to vertices
    return allVertices;
}

/**
 * @brief  std::vector<Dcel::HalfEdge*> ConvexHullBuilder::bringMeTheHorizon
 *         Takes current set of visible faces and returns a vector of ordered halfedges which form the
 *         horizon.
 *         - Loop through faces
 *           - For each HalfEdge in the face
 *             - If current given halfedge's twin is not null
 *               - Check if twin's face is visible by the current vertex
 *               - If it is not visible then the twin belongs to the horizon
 *               - Add it to the horizon and set its to and from vertex in a map
 *         - Order the horizon using the map and the horizon itself
 *
 * @param  std::set<Dcel::Face*>facesVisibleByVertex
 * @return returns array of pointers to halfedges belonging to the horizon
 */
std::vector<Dcel::HalfEdge*> ConvexHullBuilder::bringMeTheHorizon(std::set<Dcel::Face*>* facesVisibleByVertex){

    //Initialize Horizon
    std::vector<Dcel::HalfEdge*> horizon;

    //Initialize a map which maps each important from vertex to its to vertex (only on horizon edges)
    std::map<Dcel::Vertex*, Dcel::Vertex*> mapOfVertices;

     //For Each Face in Visible Faces
     for(auto faceIterator = facesVisibleByVertex->begin(); faceIterator != facesVisibleByVertex->end(); faceIterator++){
       //Initialize current face
       Dcel::Face* currentFace = *faceIterator;
       Dcel::HalfEdge* outerHalfEdge     = (*faceIterator)->getOuterHalfEdge();
       Dcel::HalfEdge* outerHalfEdgeTwin = outerHalfEdge->getTwin();

       for(int i = 0; i<3; i++){
         //Get its face
         Dcel::Face* twinsFace = outerHalfEdgeTwin->getFace();
        //If the face is not visible by the Vertex
        if(facesVisibleByVertex->count(twinsFace) == 0){
           //Add current halfedge from and to vertex to a map
           mapOfVertices[outerHalfEdgeTwin->getFromVertex()] = outerHalfEdgeTwin->getToVertex();
           //Add current twin to unordered horizon
           horizon.push_back(outerHalfEdgeTwin);
         }
         outerHalfEdge     = outerHalfEdge->getNext();
         outerHalfEdgeTwin = outerHalfEdge->getTwin();
       }
    }

    //Get the ordered Horizon
    horizon = orderHorizon(horizon, mapOfVertices);

    //Return populated Horizon
    return horizon;
}

/**
 * @brief std::vector<Dcel::HalfEdge*> ConvexHullBuilder::orderHorizon
 *        Reorders the Horizon for future utilizations (setting twins) using from and to vertex from the map
 * @param std::vector<Dcel::HalfEdge*> unHorizon, std::map<Dcel::Vertex*, Dcel::Vertex*>
 * @return ordered Horizon
 */
std::vector<Dcel::HalfEdge*> ConvexHullBuilder::orderHorizon(std::vector<Dcel::HalfEdge*> unHorizon, std::map<Dcel::Vertex*, Dcel::Vertex*> map){
    //Initialize Ordered Horizon
    std::vector<Dcel::HalfEdge*> orderedHorizon;
    //Initialize unordered horizon size
    int unHorizonSize = unHorizon.size();
    //Initialize from and to Vertex
    Dcel::Vertex* fromVertex;
    Dcel::Vertex* toVertex;

    //While all the halfedges haven't been ordered
    while(unHorizonSize != orderedHorizon.size()){
      //The very first time
      if(orderedHorizon.size() == 0){
        //Take from vertex of first's halfedge in the horizon
        fromVertex = unHorizon.front()->getFromVertex();
      } else {
        //The next fromVertex is the old toVertex
        fromVertex = toVertex;
      }
      //Get to vertex using the map. For [fromVertex] will contain only the next toVertex
      toVertex = map[fromVertex];

      //For All the Halfedges
      for(auto unHorizonIterator = unHorizon.begin(); unHorizonIterator != unHorizon.end(); unHorizonIterator ++ ){
          //Get Current HalfEdge
          Dcel::HalfEdge* currHalfEdge = *unHorizonIterator;
          //If that halfedge has exact same from and to vertex of the ordered ones
          if( currHalfEdge->getFromVertex() == fromVertex && currHalfEdge->getToVertex() == toVertex ){
              //Add it to the ordered horizon
              orderedHorizon.push_back(currHalfEdge);
          }
      }
    }

    return orderedHorizon;
}

/**
 * @brief buildNewFace(std::vector<Dcel::HalfEdge*> horizon, Dcel::Vertex* vertex, std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> probableVertices)
 *        Builds a New face frp, Given vertex and Each HalfEdge in the Horizon, updates the conflict Graph with new Face and possible vertices visible by it
 * @param std::vector<Dcel::HalfEdge*> horizon, Dcel::Vertex* vertex, std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> probableVertices
 */
void ConvexHullBuilder::buildNewFace(std::vector<Dcel::HalfEdge*> horizon, Dcel::Vertex* vertex, std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> probableVertices){
    //Array Of New Faces That will be used to set twins
    std::vector<Dcel::Face*> faceArray;
    //For each HalfEdge in the Horizon
    for( auto halfEdgeIterator = horizon.begin(); halfEdgeIterator != horizon.end(); halfEdgeIterator++ ){
        Dcel::HalfEdge *currHalfEdge = *halfEdgeIterator;
        //Setting Up a New Faced using current HalfEdge and Given Vertex
        Dcel::Face* newFace = craftNewFace(currHalfEdge, vertex);
        //Add new Created Face to Face Array
        faceArray.push_back(newFace);
        std::set<Dcel::Vertex*>* verticesForGivenHalfEdge = probableVertices[currHalfEdge];
        //Update The Conflict Graph with current New Face and probable vertices for Given HalfEdge
        conflictGraph->updateConflictGraph(newFace, verticesForGivenHalfEdge);
        setTwins(faceArray);
    }
}

void ConvexHullBuilder::setTwins(std::vector<Dcel::Face*> &faceArray){

    std::vector<Dcel::HalfEdge*> he1Vector(faceArray.size());
    std::vector<Dcel::HalfEdge*> he2Vector(faceArray.size());
    std::vector<Dcel::HalfEdge*> he3Vector(faceArray.size());

    for(unsigned int i=0; i<faceArray.size(); i++){
        he1Vector[i] = faceArray[i]->getOuterHalfEdge();
        he2Vector[i] = faceArray[i]->getOuterHalfEdge()->getNext();
        he3Vector[i] = faceArray[i]->getOuterHalfEdge()->getNext()->getNext();
    }

    for(unsigned int i=1; i<=faceArray.size(); i++){
        he2Vector[i%faceArray.size()]->setTwin(he3Vector[i-1]);
        he3Vector[i-1]->setTwin(he2Vector[i%faceArray.size()]);
    }
}

/**
 * @brief craftNewFace(Dcel::HalfEdge* halfedge, Dcel::Vertex* vertex)
 *        Builds a New face connecting counter-clockwise given vertices (like for the tetrahedron)
 * @param Dcel::HalfEdge* halfedge, Dcel::Vertex* vertex
 * @return new built face
 */
Dcel::Face* ConvexHullBuilder::craftNewFace(Dcel::HalfEdge* halfedge, Dcel::Vertex* vertex){
    //Add 3 New HalfEdges to the Dcel
    //Initializing and Adding first 3 Half Edges
    Dcel::HalfEdge* h1 = this->dcel->addHalfEdge();
    Dcel::HalfEdge* h2 = this->dcel->addHalfEdge();
    Dcel::HalfEdge* h3 = this->dcel->addHalfEdge();

    //Initializing new Vertices
    Dcel::Vertex* v1 = halfedge->getFromVertex();
    Dcel::Vertex* v2 = halfedge->getToVertex();
    Dcel::Vertex* v3 = vertex;

    //Connecting the halfedges counter-clockwise
    h1->setFromVertex(v2);
    h1->setToVertex(v1);
    h2->setFromVertex(v1);
    h2->setToVertex(v3);
    h3->setFromVertex(v3);
    h3->setToVertex(v2);

    //Set Next and Prev
    h1->setNext(h2);
    h2->setNext(h3);
    h3->setNext(h1);

    h1->setPrev(h3);
    h2->setPrev(h1);
    h3->setPrev(h2);

    //Set current HalfEdge Twin
    h1->setTwin(halfedge);

    //Set Incidental HalfEdges
    v1->setIncidentHalfEdge(h2);
    v2->setIncidentHalfEdge(h1);
    v3->setIncidentHalfEdge(h3);

    //Increment Cardinality Each time a new edge enters/exits from a Vertex
    v1->incrementCardinality();
    v1->incrementCardinality();
    v2->incrementCardinality();
    v2->incrementCardinality();
    v3->incrementCardinality();
    v3->incrementCardinality();

    //Set a New Face
    Dcel::Face* newFace = this->dcel->addFace();

    //Set new Face Half Edges
    h1->setFace(newFace);
    h2->setFace(newFace);
    h3->setFace(newFace);

    //Set new Face Outer HalfEdge
    newFace->setOuterHalfEdge(h1);

    //Return new Face
    return newFace;
}
