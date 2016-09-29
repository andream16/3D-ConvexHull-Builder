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
    //Check which faces see which vertices and viceversa
    checkVisibility();
}

/**
 * @brief checkVisibility(POINTS_VECTOR tetrahedronVertices, FACE_ARRAY tetrahedronFaces)
 *        checks which faces can be seen by a vector and viceversa
 * @param POINTS_VECTOR tetrahedronVertices, FACE_ARRAY tetrahedronFaces
 */
void ConflictGraph::checkVisibility(){

    //Initialize 4*4 Matrix
    Eigen::Matrix4d matrix;

    //for each face
    for(auto faceIterator = dcel->faceBegin(); faceIterator != dcel->faceEnd(); ++faceIterator){

        Face currentFace = *faceIterator;

        int k=0;

        //Get current Outer Half Edge
        Dcel::HalfEdge* he = currentFace->getOuterHalfEdge();

        //For each Vertex of the face
        for( int i=0; i<3; i++, k++ ){

          //Get Current Vertex Coordinates
          Pointd currVertexCoordinates = he->getFromVertex()->getCoordinate();

          //Fill Matrix with currVertexCoordinates
          matrix(k, 0) = currVertexCoordinates.x();
          matrix(k, 1) = currVertexCoordinates.y();
          matrix(k, 2) = currVertexCoordinates.z();
          matrix(k, 3) = 1; //last column made of ones

          //Take Next Edge
          he = he->getNext();

        }

        for(unsigned int j=4; j<remainingVertices.size(); j++){

          //Add current vertex coordinates to last row of the matrix
          matrix(3,0) = remainingVertices[j]->getCoordinate().x();
          matrix(3,1) = remainingVertices[j]->getCoordinate().y();
          matrix(3,2) = remainingVertices[j]->getCoordinate().z();
          matrix(3,3) = 1;

          //compute cross product
          crossProduct(matrix, currentFace, remainingVertices[j]);
        }


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

        //If the list already exists
        if(this->vertexConflictList[face] == nullptr){
            //Insert current face into it
            this->vertexConflictList[face] = new std::set<Dcel::Vertex*>;
        }
          //else a new one gets created and face gets inserted
          this->vertexConflictList.at(face)->insert(vertex);
}

/**
 * @brief addFace(FACE face, VERTEX vertex) adds a face to conflict list
 * @param FACE face, VERTEX vertex
 */
void ConflictGraph::addFace(Face face, Vertex vertex){

    //If the set already exists
    if(this->faceConflictList[vertex] == nullptr){
        //Insert current face into it
        this->faceConflictList[vertex] = new std::set<Dcel::Face*>;
    }
      //else a new one gets created and face gets inserted
      this->faceConflictList.at(vertex)->insert(face);
}

/**
 * @brief lookForVisibleFaces(VERTEX vertex) finds which faces are visible from a given vertex
 * @param VERTEX vertex given vertex
 * @retuns set of Faces seen by the vertex
 */
std::set<Dcel::Face*>* ConflictGraph::lookForVisibleFaces(Dcel::Vertex* vertex){
    //dynamically typed iterator, looks for face that contains vertex
    auto facesSet = this->faceConflictList[vertex];
    //If it is not present in faceConflictList
    if(facesSet == nullptr){
        facesSet = new std::set<Dcel::Face*>;
       //Add that face
       this->faceConflictList[vertex] = facesSet;
    }
       //return void
       return new std::set<Dcel::Face*>(*facesSet);

}

std::set<Dcel::Vertex*>* ConflictGraph::getVisibleVertices(Dcel::Face* face){
    std::set<Vertex> *vertices = this->vertexConflictList[face];

    //if the vertices set associated to the vertex is null, construct an empty and add it to the map
    if(vertices == nullptr){
        vertices = new std::set<Vertex>;
        this->vertexConflictList[face] = vertices;
    }

    //return a clone of the set for further manipulations
    return new std::set<Vertex>;
}


void ConflictGraph::updateConflictGraph(Dcel::Face* face, std::set<Dcel::Vertex*>* candidateVertices)
{
    for(auto pit = candidateVertices->begin(); pit != candidateVertices->end(); ++pit){
        if(checkV(face, *pit)){
            addFace(face, *pit);
            addVertex(face, *pit);
        }
    }
}

bool ConflictGraph::checkV(Face face, Vertex vert){

    //get a vertex from the face
    Dcel::Vertex *v = *(face->incidentVertexBegin());

    //(vertex - v) is a vector joining the face and the vertex to check
    //if the dot product betweet it and the face normal is positive,
    //the vector lies in the same semi-space of the normal, implying that the vertex sees the face
    //I use this form instead the determinant of the 4x4 matrix for performance
    return ((vert->getCoordinate() - v->getCoordinate()).dot(getFaceNormalDirection(face)) > std::numeric_limits<double>::epsilon());
}

Pointd ConflictGraph::getFaceNormalDirection(Dcel::Face* face){
    Pointd vertices[3], vec1, vec2, dir;

    //get the vertices of the face
    int i=0;
    for(Dcel::Face::IncidentVertexIterator vit = face->incidentVertexBegin(); vit != face->incidentVertexEnd(); ++vit){
        vertices[i] = (*vit)->getCoordinate();
        i++;
    }

    //the normal vector is the cross product between two edge vectors of the face
    //I don't normalize because I only need the direction
    vec1 = vertices[1] - vertices[0];
    vec2 = vertices[2] - vertices[0];
    dir = vec1.cross(vec2);

    return dir;

}


void ConflictGraph::deleteFaces(std::set<Face>* faces)
{
    //iterate over the faces
    for(auto fit = faces->begin(); fit != faces->end(); ++fit){

        Dcel::Face* currface = *fit;

        std::set<Dcel::Vertex*>* vertices = getVisibleVertices(currface);
        for(auto it = vertices->begin(); it != vertices->end(); it++){
            Dcel::Vertex* newVertex = * it;
            faceConflictList.at(newVertex)->erase(currface);
        }
        vertexConflictList.erase(currface);
    }
}

void ConflictGraph::deletePoint(Vertex vertex){

    std::set<Dcel::Face*>* faces = lookForVisibleFaces(vertex);

    for(auto fit = faces->begin(); fit != faces->end(); ++fit){

        Dcel::Face* currface = *fit;

        vertexConflictList.at(currface)->erase(vertex);
    }

    faceConflictList.erase(vertex);
}
