#include "conflictgraph.h"
#include <stdlib.h>

/**
 * @brief ConflictGraph::ConflictGraph() Constructor
 * @params takes dcel and tetrahedron vertices as input
 */
ConflictGraph::ConflictGraph(DrawableDcel* dcel, const std::vector<Dcel::Vertex*> &remainingVertices){
    this->dcel = dcel;
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
        for(unsigned int j=4; j<remainingVertices.size(); j++){

          //Add current vertex coordinates to last row of the matrix
          matrix(3,0) = remainingVertices[j]->getCoordinate().x();
          matrix(3,1) = remainingVertices[j]->getCoordinate().y();
          matrix(3,2) = remainingVertices[j]->getCoordinate().z();
          matrix(3,3) = 1;

          //Compute Cross Product, if returns true -> addVertex and addFace
          if(crossProduct(matrix)){
            //if true, add passed vertex and face to the dcel
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
            //create a new one for the passed face
            this->vertexConflictMap[face] = new std::set<Dcel::Vertex*>;
        }
          //insert the current vertex in it
          this->vertexConflictMap.at(face)->insert(vertex);
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
      this->faceConflictMap.at(vertex)->insert(face);
}

/**
 * @brief lookForVisibleFaces(VERTEX vertex) finds which faces are visible from a given vertex
 * @param VERTEX vertex given vertex
 * @retuns set of Faces seen by the vertex
 */
std::set<Dcel::Face*>* ConflictGraph::lookForVisibleFaces(Dcel::Vertex* vertex){
    //dynamically typed iterator, looks for face that contains vertex
    auto facesSet = this->faceConflictMap[vertex];
    //If it is not present in faceConflictMap
    if(facesSet == nullptr){
        facesSet = new std::set<Dcel::Face*>;
       //Add that face
       this->faceConflictMap[vertex] = facesSet;
    }
       //return void
       return new std::set<Dcel::Face*>(*facesSet);

}

std::set<Dcel::Vertex*>* ConflictGraph::getVisibleVertices(Dcel::Face* face){
    std::set<Dcel::Vertex*> *vertices = this->vertexConflictMap[face];

    //if the vertices set associated to the vertex is null, construct an empty and add it to the map
    if(vertices == nullptr){
        vertices = new std::set<Dcel::Vertex*>;
        this->vertexConflictMap[face] = vertices;
    }

    //return a clone of the set for further manipulations
    return new std::set<Dcel::Vertex*>;
}


void ConflictGraph::updateConflictGraph(Dcel::Face* face, std::set<Dcel::Vertex*>* candidateVertices)
{
    for(auto pit = candidateVertices->begin(); pit != candidateVertices->end(); ++pit){
        if(checkV(face, *pit)){
            addToFaceConflictMap(face, *pit);
            addToVertexConflictMap(face, *pit);
        }
    }
}

bool ConflictGraph::checkV(Dcel::Face* face, Dcel::Vertex* vert){

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


void ConflictGraph::deleteFaces(std::set<Dcel::Face*>* faces)
{
    //iterate over the faces
    for(auto fit = faces->begin(); fit != faces->end(); ++fit){

        Dcel::Face* currface = *fit;

        std::set<Dcel::Vertex*>* vertices = getVisibleVertices(currface);
        for(auto it = vertices->begin(); it != vertices->end(); it++){
            Dcel::Vertex* newVertex = * it;
            faceConflictMap.at(newVertex)->erase(currface);
        }
        vertexConflictMap.erase(currface);
    }
}

void ConflictGraph::deletePoint(Dcel::Vertex* vertex){

    std::set<Dcel::Face*>* faces = lookForVisibleFaces(vertex);

    for(auto fit = faces->begin(); fit != faces->end(); ++fit){

        Dcel::Face* currface = *fit;

        vertexConflictMap.at(currface)->erase(vertex);
    }

    faceConflictMap.erase(vertex);
}
