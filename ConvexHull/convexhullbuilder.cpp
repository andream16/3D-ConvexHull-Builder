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
          //dcel->addVertex(currVert);

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
    }
    //Return array of pointers to vertices
    return allVertices;
}

/**
 * @brief  std::vector<Dcel::HalfEdge*> ConvexHullBuilder::bringMeTheHorizon
 *         Takes current set of visible faces and returns a vector of halfedges which form the
 *         horizon.
 *         - Loop through faces
 *           - For each HalfEdge in the face
 *             - If given halfedge's twin is not null
 *               - Check if twin's face is visible by the current vertex
 *               - If it is not visible then the twin belongs to the horizon and exit from the loop,
 *                 else skip
 *         - Starting from the first horizon's halfedge
 *           While we reach again the latter halfedge
 *            - Pick its next and, if it is visible from the vertex, add it to the horizon
 *              else skip
 *
 * @param  std::set<Dcel::Face*>facesVisibleByVertex
 * @return returns array of pointers to halfedges belonging to the horizon
 */
std::vector<Dcel::HalfEdge*> ConvexHullBuilder::bringMeTheHorizon(std::set<Dcel::Face*>* facesVisibleByVertex){

    //Initialize Horizon
    std::vector<Dcel::HalfEdge*> horizon;

    //Initialize first element of the horizon
    Dcel::HalfEdge* firstHorizonHalfEdge;

    bool firstHalfEdgeFound = false;

    //While an edge belonging to the horizon is not found
    while(firstHalfEdgeFound == false){
      //For Each Face in Visible Face
      for(auto faceIterator = facesVisibleByVertex->begin(); faceIterator != facesVisibleByVertex->end(); faceIterator++){
          //Initialize current face
          Dcel::Face* currentFace = *faceIterator;
          //For each edge in the face
          for(auto halfEdgeIterator = currentFace->incidentHalfEdgeBegin(); halfEdgeIterator != currentFace->incidentHalfEdgeEnd(); halfEdgeIterator++){
              //Initialize current HalfEdge
              Dcel::HalfEdge* currHalfEdge = * halfEdgeIterator;
              //Initialize current HalfEdge Twin
              Dcel::HalfEdge* twin = currHalfEdge->getTwin();

              //If the twin exists
              if(twin != nullptr){
                  //Get its face
                  Dcel::Face* twinsFace = twin->getFace();
                  //If the face is not visible by the Vertex
                  if(facesVisibleByVertex->count(twinsFace) == 0){
                      //First HalfEdge is the twin, break
                      firstHorizonHalfEdge = twin;
                      //Add the very first horizon's halfedge to it
                      horizon.push_back(firstHorizonHalfEdge);
                      firstHalfEdgeFound = true;
                  }
              }
          }
      }
    }

    //If firstHalfEdge is found
    if(firstHorizonHalfEdge != nullptr){
      //While we reach again the same halfedge (e.g. circular tail)
      while(firstHorizonHalfEdge->getNext() == firstHorizonHalfEdge){
        //Initialize current halfedge's next's twin
        Dcel::HalfEdge* nextTwin;
        nextTwin = firstHorizonHalfEdge->getTwin()->getTwin();
        //Check if its face is visible
        if(facesVisibleByVertex->count(nextTwin->getFace()) == 0){
          //If it is not visible then it belongs to the horizon
          horizon.push_back(nextTwin);
        }
      }
    }

    //Return populated Horizon
    return horizon;

}

