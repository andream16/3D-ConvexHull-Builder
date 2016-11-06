#include "facebuilderhelper.h"

/** @brief Class used to build new Faces starting froma  given HalfEdge vector and a given vertex
 *  @param DrawableDcel* dcel, Pointd vertex, std::vector<Dcel::HalfEdge*> halfEdges
**/
FaceBuilderHelper::FaceBuilderHelper(DrawableDcel* dcel){
    this->dcel = dcel;
}

/**
 * @brief FaceBuilderHelper Class Destructor
 **/
FaceBuilderHelper::~FaceBuilderHelper(){}

/**
 * @brief FaceBuilderHelper::buildFaces
 *        Builds a new face per each passed HalfEdge
 * @param Pointd vertex, std::vector<Dcel::HalfEdge*> halfEdges
 **/
std::vector<Dcel::Face*> FaceBuilderHelper::buildFaces(Dcel::Vertex* passedVertex, std::vector<Dcel::HalfEdge*> halfEdges){

    //Initialize Possible arra Of faces
    std::vector<Dcel::Face*> faces;

    //Add Current vertex to the Dcel
    Dcel::Vertex* vertex = this->dcel->addVertex(passedVertex->getCoordinate());

    //For each passed halfEdge
    for( auto halfEdgeIterator = halfEdges.begin(); halfEdgeIterator != halfEdges.end(); halfEdgeIterator++){
        //Get Current HalfEdge
        Dcel::HalfEdge* passedEdge = *halfEdgeIterator;

        //Initialize Vertices for passedEdge
        Dcel::Vertex* fromVertex = passedEdge->getFromVertex();
        Dcel::Vertex* toVertex   = passedEdge->getToVertex();

        //Initialize new half edges
        Dcel::HalfEdge* he1 = this->dcel->addHalfEdge();
        he1->setFromVertex(toVertex);
        he1->setToVertex(fromVertex);

        Dcel::HalfEdge* he2 = this->dcel->addHalfEdge();
        he2->setFromVertex(fromVertex);
        he2->setToVertex(vertex);

        Dcel::HalfEdge* he3 = this->dcel->addHalfEdge();
        he3->setFromVertex(vertex);
        he3->setToVertex(toVertex);

        //Set Next and prevs
        he1->setNext(he2);
        he1->setPrev(he3);

        he2->setNext(he3);
        he2->setPrev(he1);

        he3->setNext(he1);
        he3->setPrev(he2);

        //Incident Half Edges, when a new edge 'starts' from them
        toVertex->setIncidentHalfEdge(he1);
        fromVertex->setIncidentHalfEdge(he2);
        vertex->setIncidentHalfEdge(he3);

        //Set Twins
        he1->setTwin(passedEdge);
        passedEdge->setTwin(he1);

        //Empty at the first steps but filled in next steps
        /*if(passedEdge->getPrev()->getTwin() != nullptr){
           Dcel::HalfEdge* he2_twin = passedEdge->getPrev()->getTwin()->getPrev();
           he2->setTwin(he2_twin);
           he2_twin->setTwin(he2);
        }

        //Empty at the first steps but filled in next steps
        if(passedEdge->getNext()->getTwin() != nullptr){
                Dcel::HalfEdge* he3_twin = passedEdge->getNext()->getTwin()->getNext();
                he3->setTwin(he3_twin);
                he3_twin->setTwin(he3);
        }*/

        //Cardinality, increments each time a new half edge 'starts' or 'ends' in a certain vertex
        fromVertex->incrementCardinality();
        fromVertex->incrementCardinality();

        toVertex->incrementCardinality();
        toVertex->incrementCardinality();

        vertex->incrementCardinality();
        vertex->incrementCardinality();

        //Create a new face from newly created edges
        Dcel::Face* newFace = this->dcel->addFace();
        //NewFace will access external faces by he1
        newFace->setOuterHalfEdge(he1);
        //Set each edge to newFace
        he1->setFace(newFace);
        he2->setFace(newFace);
        he3->setFace(newFace);

        //Add the new face to face Array
        faces.push_back(newFace);

    }

    //Set Twins for each Face
    for(unsigned int i=1;i<=faces.size();i++){

        Dcel::HalfEdge* nextOfOuter=faces[i% faces.size()]->getOuterHalfEdge()->getNext();
        Dcel::HalfEdge* prevOfOuterPrevFace=faces[i-1]->getOuterHalfEdge()->getPrev();

        nextOfOuter->setTwin(prevOfOuterPrevFace);
        prevOfOuterPrevFace->setTwin(nextOfOuter);
    }

    //Return Face Array in any case, only ConvexHullBuilder will use the Array of Faces
    return faces;
}
