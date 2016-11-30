#include "convexhullbuilder.h"
#include <iostream>
#include <stdlib.h>

/**
 * @brief ConvexHullBuilder::ConvexHullBuilder()
 * @params DrawableDcel *dcel, MainWindow* mainWindow, bool showPhases
 */
ConvexHullBuilder::ConvexHullBuilder(DrawableDcel *dcel, MainWindow* mainWindow, bool showPhases){
    this->dcel       = dcel;
    this->mainWindow = mainWindow;
    this->showPhases = showPhases;
}

/**
 * @brief ConvexHullBuilder Class Destructor
 **/
ConvexHullBuilder::~ConvexHullBuilder(){
    delete convexHullBuilderHelper;
    delete conflictGraph;
    delete faceBuilderHelper;
}

/**
 * @brief ConvexHullBuilder::computeConvexHull() takes dcel as input.
 *        Starts the algorithm calling all the different functions needed.
 *         - Takes all the vertices from the Dcel and saves them into a vector of vertices
 *         - Resets the dcel
 *         - Calls tetrahedron's buildTetrahedron method in order to build the latter with the passed vertices
 *         - Initializes Conflict Graph with, respectively, Vertices visible from a Face and Faces visible from a Vertex
 *         - For each remaining vertex, checks if the vertex is in conflict with a certain number of faces, if so:
 *           - Gets the Horizon for the faces visible by the latter vertex
 *           - Gets Candidate Vertex Map for the passed horizon (vector of halfedges)
 *           - Deletes visible faces
 *           - For each halfedge in the Horizon
 *             - Builds a new face using the current horizon and the passed vertex
 *             - Updates the Conflict Graph with new informations
 *           - Deletes the passed Vertex
 *         If GUI checkbox is checked, it shows the different phases of the process
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

    /** Build Tetrahedron with using the first 4 vertices in dcelVertices (after that they have been shuffled up)
     *  and return shuffled dcelVertices **/
    dcelVertices = tetrahedronBuilder->buildTetrahedron();

    //Initialize FaceBuilderHelper
    faceBuilderHelper = new FaceBuilderHelper(dcel);

    //If showPhases is checked, show the Tetrahedron
    if(showPhases){
        dcel->update();
        this->mainWindow->updateGlCanvas();
    }

    //Instantiate ConflictGraph
    conflictGraph = new ConflictGraph(dcel, dcelVertices);

    //Initialize Conflict Graph with Dcel and dcelVertices
    conflictGraph->initializeConflictGraph();

    //Get all vertices size
    int verticesSize = dcelVertices.size();

    //Loop through remaining vertices
    for(int i=4; i<verticesSize; i++){

        //Initialize vertex to which will be assigned, each time, dcelVertices[i] vertex
        Dcel::Vertex* currentVertex = new Dcel::Vertex;
        currentVertex = dcelVertices[i];

        //Check if current vertex is in conflict with dcel's faces
        std::tr1::unordered_set<Dcel::Face*>* facesVisibleByVertex = conflictGraph->getFacesVisibleByVertex(currentVertex);

        //If the vertex is in front of some faces, so, facesVisibleByVertex is not empty
        if( !facesVisibleByVertex->empty() ){

            //Get the Horizon for the current Visible Faces
            std::vector<Dcel::HalfEdge*> horizon = convexHullBuilderHelper->bringMeTheHorizon(facesVisibleByVertex);

            //Get the probable visible vertices for each face of each horizon's halfedge
            std::tr1::unordered_map<Dcel::HalfEdge*, std::tr1::unordered_set<Dcel::Vertex*>*> oldVertices = conflictGraph->joinVertices(horizon);

            //Delete Visible Faces from the Conflict Graph and Dcel
            conflictGraph->deleteFaces(facesVisibleByVertex);

            //Build a Face for each halfedge in the horizon and save them into an array of faces
            std::vector<Dcel::Face*> faces = faceBuilderHelper->buildFaces(currentVertex, horizon);

            /* Update Conflict Graph for each new face and possible vertices in conflict with them
             * since they were in conflict with the destroyed face */
            conflictGraph->checkConflict(faces, oldVertices, horizon);

            //If showPhases is clicked, show the progress
            if( showPhases ){
                dcel->update();
                this->mainWindow->updateGlCanvas();
            }

        }

        //Erase The Current Vertex From the Conflict Graph
        conflictGraph->eraseVertex(currentVertex);

    }

    //Delete each vertex from the dcelVertices array
    for (auto vertexIterator = dcelVertices.begin(); vertexIterator != dcelVertices.end(); vertexIterator++){
        delete (*vertexIterator);
    }
    //Clear dcelVertices array
    dcelVertices.clear();

}

