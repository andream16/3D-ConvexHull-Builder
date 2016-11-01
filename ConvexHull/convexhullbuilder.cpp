#include "convexhullbuilder.h"
#include <iostream>
#include <stdlib.h>

/**
 * @brief ConvexHullBuilder::ConvexHullBuilder() Constructor
 * @params takes passed dcel
 */
ConvexHullBuilder::ConvexHullBuilder(DrawableDcel *dcel){
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

    //Instantiate ConvexHullBuilderHelper
    convexHullBuilderHelper = new ConvexHullBuilderHelper(dcel);

    //Get all the vertices from the dcel
    std::vector<Dcel::Vertex*> dcelVertices = convexHullBuilderHelper->getAllVertices();

    //Reset Dcel
    dcel->reset();

    //Instantiate TetrahedronBuilder
    tetrahedronBuilder = new TetrahedronBuilder(dcel, dcelVertices);
    //Build Tetrahedron with the dcel and all vertices
    tetrahedronBuilder->buildTetrahedron();

    //Instantiate ConflictGraph
    conflictGraph = new ConflictGraph(dcel, dcelVertices);
    //Initialize Conflict Graph with Dcel And First 4 Vertices
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
      if( !facesVisibleByVertex->empty() ){

         //Add it to the Dcel
         dcel->addVertex(currVert->getCoordinate());

         //Get the Horizon for the current Visible Faces
         std::vector<Dcel::HalfEdge*> horizon = convexHullBuilderHelper->bringMeTheHorizon(facesVisibleByVertex);

         //Get the probable visible vertices for each face of each horizon's halfedge
         conflictGraph->joinVertices(horizon);

         //Delete Visible Faces from the Conflict Graph and Dcel
         conflictGraph->deleteFaces(facesVisibleByVertex);

       }

    }
}

