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
 * @brief FaceBuilderHelper::buildFaces(Dcel::Vertex* const &passedVertex, std::vector<Dcel::HalfEdge*> const &halfEdges) const
 *        Builds a new face per each passed HalfEdge
 * @param Dcel::Vertex* const &passedVertex, std::vector<Dcel::HalfEdge*> const &halfEdges
 **/
std::vector<Dcel::Face*> FaceBuilderHelper::buildFaces(Dcel::Vertex* const &passedVertex, std::vector<Dcel::HalfEdge*> const &halfEdges) const{

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

    /**
      * In order to set the twins, we take advantage of the fact that, since we have
      * an array of ordered faces with their Outer Halfedges set on Horizon's HalfEdges
      * The ordered faces are next to each other, and the edges of those faces who are
      * one next the other can be set as twins.
      *  - i-face's twin is always i+1-face's outer halfedge's next
      *  - i+1 face's twin is always i-face's outerhalfedge's prev
      * Exception made for the last face. Since we have to close the loop, we need to use
      * the very first face of the array to set their respective twins
     **/

    //Get Faces Number
    int facesSize = faces.size();

    //Set Twins for each face except Last One
    for( int i = 0; i < facesSize; i++ ){
        //Get current OuterHalfEdge from current Face [i]
        Dcel::HalfEdge* currOuterHalfEdge = faces[i]->getOuterHalfEdge();
        //Get Next Face OuterHalfEdge [i+1]
        Dcel::HalfEdge* nextOuterHalfEdge;

        //If currOuterHalfEdge exists
        if( currOuterHalfEdge != nullptr ){

            //If last Face
            if( i == facesSize-1 ){
                //Get first face's outer halfedge
                nextOuterHalfEdge = faces.front()->getOuterHalfEdge();
            } else {
                //Get Next Face OuterHalfEdge [i+1]
                nextOuterHalfEdge = faces[i+1]->getOuterHalfEdge();
            }

            //Get currOuterHalfEdge prev and nextOuterHalfEdge next
            Dcel::HalfEdge* currOHEPrev = (*currOuterHalfEdge).getPrev();
            Dcel::HalfEdge* nextOHENext = (*nextOuterHalfEdge).getNext();
            //currOHEPrev twin's is exactly nextOHENext and viceversa
            currOHEPrev->setTwin(nextOHENext);
            nextOHENext->setTwin(currOHEPrev);

        }
    }

    //Return Face Array in any case, only ConvexHullBuilder will use the Array of Faces
    return faces;
}
