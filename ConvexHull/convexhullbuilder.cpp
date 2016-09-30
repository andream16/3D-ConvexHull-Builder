#include "convexhullbuilder.h"
#include <iostream>
#include <stdlib.h>

/**
 * @brief ConvexHullBuilder::ConvexHullBuilder() Conmstructor
 * @params takes dcel as input
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
 *        Adds Vertices, Builds a Tetrahedron . . .
 */
void ConvexHullBuilder::computeConvexHull(){

    //gets all the vertices from the dcel
    std::vector<Dcel::Vertex*> allVertices = getAllVertices();

    //Reset Dcel
    dcel->reset();

    //Builds the initial tetrahedron
    tetrahedronBuilder = new TetrahedronBuilder(dcel, allVertices);
    tetrahedronBuilder->buildTetrahedron();
    bool teeeeee = true;
    //Initializes Conflict Graph with Dcel And First 4 Vertices
    //conflictGraph = new ConflictGraph(dcel, dcelVertices);
    //conflictGraph->initializeConflictGraph();
    //bool ddd = true;
    //Loop through remaining vertices finishing the convex hull
    //finalizeConvexHull(allVertices);

    //Loop through remaining vertices
    //for(unsigned int i=4; i<dcelVertices.size(); i++){
        //bool kappa = true;
      //Check Which faces sees i-Vertex and assigning them to a set
      /**std::set<Dcel::Face*>* facesVisible = conflictGraph->lookForVisibleFaces(allVertices[i]);

      //Initializing Horizon, vector of half edges
      std::vector<Dcel::HalfEdge*> horizon;

      if( facesVisible->size() > 0 ){

          Dcel::Vertex* currVertx = allVertices[i];

          //add current vertex to dcel
          dcel->addVertex(currVertx->getCoordinate());

          horizon = bringMeTheHorizon(facesVisible); //proper name

          std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> candidateVertexMap = getCandidateVerticesMap(horizon);

            conflictGraph->deleteFaces(facesVisible);
            removeVisibleFaces(facesVisible);

            std::vector<Dcel::Face*> newFaces;


            bool kappa = true;

            /**
            for(std::vector<HalfEdge>::iterator horizonIterator = horizon.begin(); horizonIterator != horizon.end(); horizonIterator++){
               HalfEdge halfEdge = *horizonIterator;

               //create a new face and push it in the vector of new faces
               Dcel::Face* newFace = addFace(currVertx, halfEdge);
               newFaces.push_back(newFace);

               conflictGraph->updateConflictGraph(newFace, candidateVertexMap[halfEdge]);
            }

            setTwins(newFaces);**/

      //}

      //conflictGraph->deletePoint(allVertices[i]);
    //}
}


Dcel::Face* ConvexHullBuilder::addFace(Dcel::Vertex* otherVertex, Dcel::HalfEdge* existingHe){
    Dcel::HalfEdge* he1 = this->dcel->addHalfEdge();
    Dcel::HalfEdge* he2 = this->dcel->addHalfEdge();
    Dcel::HalfEdge* he3 = this->dcel->addHalfEdge();

    Dcel::Vertex* startVertex = existingHe->getFromVertex();
    Dcel::Vertex* endVertex = existingHe->getToVertex();

    he1->setFromVertex(endVertex);
    endVertex->setIncidentHalfEdge(he1);
    he1->setToVertex(startVertex);
    he1->setNext(he2);
    he1->setPrev(he3);
    he1->setTwin(existingHe);
    existingHe->setTwin(he1);
    endVertex->incrementCardinality();
    startVertex->incrementCardinality();

    he2->setFromVertex(startVertex);
    startVertex->setIncidentHalfEdge(he2);
    he2->setToVertex(otherVertex);
    he2->setNext(he3);
    he2->setPrev(he1);
    startVertex->incrementCardinality();
    otherVertex->incrementCardinality();

    he3->setFromVertex(otherVertex);
    otherVertex->setIncidentHalfEdge(he3);
    he3->setToVertex(endVertex);
    he3->setNext(he1);
    he3->setPrev(he2);
    endVertex->incrementCardinality();
    otherVertex->incrementCardinality();

    Dcel::Face* face = this->dcel->addFace();
    face->setOuterHalfEdge(he1);
    he1->setFace(face);
    he2->setFace(face);
    he3->setFace(face);

    return face;
}

void ConvexHullBuilder::removeVisibleFaces(std::set<Dcel::Face*> *faceList){

    for(auto it = faceList->begin(); it != faceList->end(); it++){

        Dcel::Face* face = *it;

        for(Dcel::Face::IncidentHalfEdgeIterator vit = face->incidentHalfEdgeBegin(); vit != face->incidentHalfEdgeEnd(); ++vit){

            Dcel::HalfEdge* he = *vit;

            Dcel::Vertex* start = he->getFromVertex();
            Dcel::Vertex* end = he->getToVertex();

            this->dcel->deleteHalfEdge(he);

            //decrement the cardinality of each vertex every time we remove an half edge
            start->decrementCardinality();
            end->decrementCardinality();

            //if the cardinality is 0, the vertex is disconnected and must be removed
            if(start->getCardinality() == 0){
                this->dcel->deleteVertex(start);
            }
            if(end->getCardinality() == 0){
                this->dcel->deleteVertex(end);
            }
        }


        this->dcel->deleteFace(face);
    }
}


void ConvexHullBuilder::setTwins(std::vector<Dcel::Face*> faceList){

    std::vector<Dcel::HalfEdge*> he1Vector(faceList.size());
    std::vector<Dcel::HalfEdge*> he2Vector(faceList.size());
    std::vector<Dcel::HalfEdge*> he3Vector(faceList.size());

    for(unsigned int i=0; i<faceList.size(); i++){
        he1Vector[i] = faceList[i]->getOuterHalfEdge();
        he2Vector[i] = faceList[i]->getOuterHalfEdge()->getNext();
        he3Vector[i] = faceList[i]->getOuterHalfEdge()->getNext()->getNext();
    }

    for(unsigned int i=1; i<=faceList.size(); i++){
        he2Vector[i%faceList.size()]->setTwin(he3Vector[i-1]);
        he3Vector[i-1]->setTwin(he2Vector[i%faceList.size()]);
    }
}



std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> ConvexHullBuilder::getCandidateVerticesMap(std::vector<Dcel::HalfEdge*> horizon){

    std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> result;

    //for each half edge in the horizon
    for(auto it = horizon.begin(); it != horizon.end(); ++it){
        Dcel::HalfEdge *halfEdge = *it;

        //get its and its twin incident faces
        Dcel::Face *face1 = halfEdge->getFace();
        Dcel::Face *face2 = halfEdge->getTwin()->getFace();

        //get the set of the vertices visible from the former faces
        std::set<Dcel::Vertex*> *conflict1, *conflict2;
        conflict1 = conflictGraph->getVisibleVertices(face1);
        conflict2 = conflictGraph->getVisibleVertices(face2);

        //merge the sets
        conflict1->insert(conflict2->begin(), conflict2->end());

        //associate the merged set to the horizon half edge
        result[halfEdge] = conflict1;
    }

    return result;

}


/**
 * @brief ConvexHullBuilder::bringMeTheHorizon adds visible faces correct half edge that lies
 *        on the horizon to a vector of half edges
 *        Loops through visible faces, finds an half edges whose twin lies in an unseen face,
 *        adds the latter to the horizon, then, iterates until we get back to it, then, if
 *        the incident face of the twin is visible, then its edge belongs to the horizon
 *
 * @param std::set<FACE>* visibleFaces
 */
std::vector<Dcel::HalfEdge*> ConvexHullBuilder::bringMeTheHorizon(std::set<Dcel::Face*>* visibleFaces){

    bool found = false;

    //Initializing Horizon, vector of half edges
    std::vector<Dcel::HalfEdge*> horizon;

    Dcel::HalfEdge* first;

    //For each Visible Face
    for(auto face = visibleFaces->begin(); face != visibleFaces->end() && !found; face++){

        Dcel::Face* visibleFace = *face;

        //iterate trough visible faces and stop when finding first horizon edge
        for(auto currentEdge = visibleFace->incidentHalfEdgeBegin(); currentEdge != visibleFace->incidentHalfEdgeEnd() && !found; currentEdge++){

           Dcel::HalfEdge* visibleHE = *currentEdge;

           if(visibleHE->getTwin() != nullptr){
             if(visibleFaces->count(visibleHE->getTwin()->getFace()) == 0){
                 first = visibleHE->getTwin();
                 found = true;
             }
           }
        }
    }

    if(found){
        Dcel::HalfEdge* current;
        Dcel::HalfEdge* next;
        Dcel::HalfEdge*  twinOfNext;

        Dcel::Face* incidentFace;

        current = first;
        horizon.push_back(first);

        do{
           next = current->getNext();
           twinOfNext = next->getTwin();
           incidentFace = twinOfNext->getFace();

           //if the incident face of the twin is visible, we have an horizon half edge
          if(visibleFaces->count(incidentFace) == 1){
             horizon.push_back(next);
             current = next;
          } else {
            current = twinOfNext;
          }
        } while (first != current && first != current->getNext());
    }

    return horizon;
}

/**
 * @brief  std::vector<Dcel::Vertex*> ConvexHullBuilder::getAllVertices takes dcel vertices
 * @return returns array of all vertices
 */
std::vector<Dcel::Vertex*>  ConvexHullBuilder::getAllVertices(){
    //Initialize container
    std::vector<Dcel::Vertex*> allVertices;
    //Adds all vertices to an array
    for (auto vertex = dcel->vertexBegin(); vertex != dcel->vertexEnd(); vertex++){
        Dcel::Vertex* v = new Dcel::Vertex((*vertex)->getCoordinate());
        allVertices.push_back(v);
    }
    //returns array of pointers to vertices
    return allVertices;
}


