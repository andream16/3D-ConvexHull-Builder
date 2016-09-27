#include "conflictgraph.h"
#include <stdlib.h>

/**
 * @brief ConflictGraph::ConflictGraph() Constructor
 * @params takes dcel and tetrahedron vertices as input
 */
ConflictGraph::ConflictGraph(DrawableDcel* dcel, std::vector<Dcel::Vertex*> tetrahedronVertices){
    this->dcel = dcel;
    this->tetrahedronVertices = tetrahedronVertices;
}

/**
 * @brief ConflictGraph Class Destructor
 **/
ConflictGraph::~ConflictGraph(){}

/**
 * @brief ConflictGraph::initializeConflictGraph() initializes the conflict graph
 */
void ConflictGraph::initializeConflictGraph(){
    //Check which faces see which vertices and viceversa
    checkVisibility();
}

/**
 * @brief checkVisibility(POINTS_VECTOR tetrahedronVertices, FACE_ARRAY tetrahedronFaces)
 *        checks which faces can be seen by a vector and viceversa
 * @param POINTS_VECTOR tetrahedronVertices, FACE_ARRAY tetrahedronFaces
 */
void ConflictGraph::checkVisibility(){
    //for each vertex
    for(unsigned int i=0; i<tetrahedronVertices.size(); i ++){
        //Compute the cross product for i-vertex coordinates
        fillCrossProductMatrix(tetrahedronVertices[i]);
    }
}

/**
 * @brief fillCrossProductMatrix(VERTEX currentVertex) fills Eigen Matrix with current coordinates
 *        and calls crossProduct to check whether ord not a vertex sees a face
 * @param VERTEX currVertex
 */
void ConflictGraph::fillCrossProductMatrix(Vertex currVertex){

    //Initialize 4*4 Matrix
    Eigen::Matrix4d matrix;

    //For each face present in the dcel
    for(Dcel::FaceIterator face = dcel->faceBegin(); face != dcel->faceEnd(); face++){

        //Pointing to current Face
        Dcel::Face* currentFace= *face;

        //Get current Outer Half Edge
        Dcel::HalfEdge* he = currentFace->getOuterHalfEdge();

        //For each Vertex of the face
        for(unsigned int k=0; k<3; k++){
          //Get Current Vertex Coordinates
          Pointd currVertexCoordinates = he->getFromVertex()->getCoordinate();

          //Fill Matrix with currVertexCoordinates
          matrix(k, 0) = currVertexCoordinates.x();
          matrix(k, 1) = currVertexCoordinates.y();
          matrix(k, 2) = currVertexCoordinates.z();
          matrix(k, 3) = 1; //last column made of ones

          //Take Next Edge
          he= he->getNext();

        }

        //Add current vertex coordinates to last row of the matrix
        matrix(3,0) = currVertex->getCoordinate().x();
        matrix(3,1) = currVertex->getCoordinate().y();
        matrix(3,2) = currVertex->getCoordinate().z();
        matrix(3,3) = 1;

        //compute cross product
        crossProduct(matrix, currentFace, currVertex);
    }
}

/**
 * @brief crossProduct(Eigen::Matrix4d matrix) calculates determinant od the passed matrix and
 *        if it's <0, then the vertex sees the face, else if >0 it does not, if = 0 they are coplanar
 * @param Eigen::Matrix4d matrix current matrix
 */
void ConflictGraph::crossProduct(Eigen::Matrix4d matrix, Face face, Vertex vertex){
    //if true -> 0, if false -> 1
    if(matrix.determinant() < -std::numeric_limits<double>::epsilon()){
        //if true, add passed vertex and face to the dcel
        addVertex(face, vertex);
        addFace(face, vertex);
    }
}

/**
 * @brief addVertex(FACE face, VERTEX vertex) adds a vertex to conflict list
 * @param FACE face, VERTEX vertex
 */
void ConflictGraph::addVertex(Face face, Vertex vertex){
    //dinamically typed iterator
    auto vertexIterator = this->VertexConflictList.find(face);
        //If the list already exists
        if(vertexIterator != VertexConflictList.end()){
            //Insert current face into it
            std::set<Vertex>* vertexList = VertexConflictList[face];
            vertexList->insert(vertex);

        } else {
            //else a new one gets created and face gets inserted
            std::set<Vertex>* vertexList = new std::set<Vertex>();
            vertexList->insert(vertex);
            VertexConflictList[face]=vertexList;
    }
}

/**
 * @brief addFace(FACE face, VERTEX vertex) adds a face to conflict list
 * @param FACE face, VERTEX vertex
 */
void ConflictGraph::addFace(Face face, Vertex vertex){
    //dynamically typed iterator, looks for face that contains vertex
    auto faceIterator =this->FaceConflictList.find(vertex);
        //If the list already exists
        if(faceIterator!=FaceConflictList.end()){
            //Insert current Vertex into it
            std::set<Face>* faceList = FaceConflictList[vertex];
            faceList->insert(face);
        } else {
            //else a new one gets created and face gets inserted
            std::set<Face>* faceList = new std::set<Face>();
            faceList->insert(face);
            FaceConflictList[vertex]=faceList;
        }
}

/**
 * @brief lookForVisibleFaces(VERTEX vertex) finds which faces are visible from a given vertex
 * @param VERTEX vertex given vertex
 * @retuns map of visible faces and passed Vertex
 */
std::set<Dcel::Face*>* ConflictGraph::lookForVisibleFaces(Vertex vertex){
    //dynamically typed iterator, looks for face that contains vertex
    auto faceFinderIterator = this->FaceConflictList.find(vertex);
    //If it is not present in FaceConflictList
    if(faceFinderIterator != FaceConflictList.end()){
       //Add that face
       return new std::set<Face>(*FaceConflictList.at(vertex));
    } else {
       //return void
       return new std::set<Face>();
    }

}
