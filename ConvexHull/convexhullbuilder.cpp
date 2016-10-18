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

    //Loop through remaining vertices
    for(unsigned int i=4; i<dcelVertices.size(); i++){

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
bool fasullo = true;
      }

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

       //For each edge in the face
       for(auto halfEdgeIterator = currentFace->incidentHalfEdgeBegin(); halfEdgeIterator != currentFace->incidentHalfEdgeEnd(); halfEdgeIterator++){
          //Initialize current HalfEdge
          Dcel::HalfEdge* currHalfEdge = *halfEdgeIterator;
          //Initialize current HalfEdge Twin
          Dcel::HalfEdge* twin = currHalfEdge->getTwin();
          //If the twin exists
          if( checkIfHalfEdgeExist(twin) ){
             //Get its face
             Dcel::Face* twinsFace = twin->getFace();
             //If the face is not visible by the Vertex
             if(facesVisibleByVertex->count(twinsFace) == 0){
               //Add current halfedge from and to vertex to a map
               mapOfVertices[twin->getFromVertex()] = twin->getToVertex();
               //Add current twin to unordered horizon
               horizon.push_back(twin);
             }
          }
       }
    }

    //Get the ordered Horizon
    horizon = orderHorizon(horizon, mapOfVertices);

    //Return populated Horizon
    return horizon;
}

/**
 * @brief Checks if a given halfedge exists. It gets its from vertex's x coordinate and divides it for itselfs
 *        if the halfedge exists then the reult would be 1 and return true, esle false.
 * @param Dcel::HalfEdge *he
 * @return true if exists, false else
 */
bool ConvexHullBuilder::checkIfHalfEdgeExist(Dcel::HalfEdge *he){
    int checker = he->getFromVertex()->getCoordinate().x() / he->getFromVertex()->getCoordinate().x();
    if(checker == 1){
        return true;
    } else {
        return false;
    }
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
      //Initializing a new halfedge
      Dcel::HalfEdge* currentHalfEdge = new Dcel::HalfEdge;
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
      //Set the new halfedge
      currentHalfEdge->setFromVertex(fromVertex);
      currentHalfEdge->setToVertex(toVertex);
      //Add it to the ordered horizon
      orderedHorizon.push_back(currentHalfEdge);

    }

    return orderedHorizon;
}
