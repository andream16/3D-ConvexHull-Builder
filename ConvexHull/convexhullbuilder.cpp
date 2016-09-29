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

    //Array of vertices
    std::vector<Dcel::Vertex*> allVertices;
    //Vertices To Get passed to Conflict Graph
    std::vector<Dcel::Vertex*> verticesForCG;
    //Four non coplanar vertices
    PointsVector fourVertices;

    //Adds all Dcel vertices pointers to allVertices
    allVertices = addVertices(allVertices);

    //Reset Dcel
    dcel->reset();

    /** Checks Coplanarity of 4 vertices,
     *  if coplanar -> pick other 4 vertices, if not -> build a tetrahedron with them**/
    buildTetrahedron(&allVertices);

    //Array of Pointers to Vertices needed for Conflict Graph Initialization
    verticesForCG = getVertices(allVertices);

    //Initializes Conflict Graph with Dcel And First 4 Vertices
    conflictGraph = new ConflictGraph(dcel, allVertices);
    conflictGraph->initializeConflictGraph();

    //Loop through remaining vertices finishing the convex hull
    //finalizeConvexHull(allVertices);

    //Loop through remaining vertices
    for(unsigned int i=4; i<allVertices.size(); i++){

      //Check Which faces sees i-Vertex and assigning them to a set
      std::set<Face>* facesVisible = conflictGraph->lookForVisibleFaces(allVertices[i]);

      //Initializing Horizon, vector of half edges
      std::vector<HalfEdge> horizon;

      if( facesVisible->size() > 0 ){

          Dcel::Vertex* currVertx = allVertices[i];

          //add current vertex to dcel
          dcel->addVertex(currVertx->getCoordinate());

          horizon = bringMeTheHorizon(facesVisible); //proper name

          std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> candidateVertexMap = getCandidateVerticesMap(horizon);

            conflictGraph->deleteFaces(facesVisible);
            removeVisibleFaces(facesVisible);

            std::vector<Dcel::Face*> newFaces;

            for(std::vector<HalfEdge>::iterator horizonIterator = horizon.begin(); horizonIterator != horizon.end(); horizonIterator++){
               HalfEdge halfEdge = *horizonIterator;

               //create a new face and push it in the vector of new faces
               Dcel::Face* newFace = addFace(currVertx, halfEdge);
               newFaces.push_back(newFace);

               conflictGraph->updateConflictGraph(newFace, candidateVertexMap[halfEdge]);
            }

            setTwins(newFaces);

      }

      conflictGraph->deletePoint(allVertices[i]);
    }
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

    for(std::set<Face>::iterator it = faceList->begin(); it != faceList->end(); it++){

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



std::map<Dcel::HalfEdge*, std::set<Dcel::Vertex*>*> ConvexHullBuilder::getCandidateVerticesMap(std::vector<HalfEdge> horizon){

    std::map<HalfEdge, std::set<Vertex>*> result;

    //for each half edge in the horizon
    for(std::vector<Dcel::HalfEdge*>::iterator it = horizon.begin(); it != horizon.end(); ++it){
        Dcel::HalfEdge *halfEdge = *it;

        //get its and its twin incident faces
        Dcel::Face *face1 = halfEdge->getFace();
        Dcel::Face *face2 = halfEdge->getTwin()->getFace();

        //get the set of the vertices visible from the former faces
        std::set<Vertex> *conflict1, *conflict2;
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
std::vector<Dcel::HalfEdge*> ConvexHullBuilder::bringMeTheHorizon(std::set<Face>* visibleFaces){

    bool found = false;

    //Initializing Horizon, vector of half edges
    std::vector<Dcel::HalfEdge*> horizon;

    Dcel::HalfEdge* first;

    //For each Visible Face
    for(auto face = visibleFaces->begin(); face != visibleFaces->end() && !found; face++){

        Face visibleFace = *face;
        //Initialize a new Incident Half Edge Iterator
        Dcel::Face::IncidentHalfEdgeIterator currentEdge;

        //iterate trough visible faces and stop when finding first horizon edge
        for(currentEdge = visibleFace->incidentHalfEdgeBegin(); currentEdge != visibleFace->incidentHalfEdgeEnd() && !found; currentEdge++){

           HalfEdge visibleHE = *currentEdge;

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
 * @brief ConvexHullBuilder::buildTetrahedron builds a tetrahedron with different steps
 * @param VERTEX_POINTERS_LIST allVertices array of all vertices
 */
void ConvexHullBuilder::buildTetrahedron(std::vector<Dcel::Vertex*>* allVertices){
    //Initialize Array of Shuffled Vertices
    VertexPointersList shuffledVertices;
    //Initialize vector that will contain picked 4 points to get checked
    PointsVector fourPoints;
    /** Initialize int var coplanarity, tells if the given vertices are coplanar (0) or not (1 or -1)
     * if matrix's determinat < 0 coplanarity will contain -1 else if determinant > 0 will contain 1 */
    int coplanarity = 0;

    //while we get 4 not coplanar vertices
    while(coplanarity == 0){
     //shuffles all vertices
     shuffledVertices = verticesShuffler(allVertices);
     //gets first 4 points (0-3) from all vertices remaining
     fourPoints = getFirstFourVertices(shuffledVertices);
     //check if the first 4 vertices are coplanar (0 not coplanar, 1 coplanar)
     coplanarity = coplanarityChecker(fourPoints);
    }

    /** Takes the four non complanar points and inserts them into the dcel to
     *  build the tetrahedron, orientation based on determinant value (coplanarity)*/
    tetrahedronMaker(fourPoints, coplanarity);

}

/**
 * @brief  ConvexHullBuilder::tetrahedronMaker takes the four non-coplanar vertices and inserts their data in the dcel
 *         to build the tetrahedron itself basing orientation on determinant result
 * @param  POINTS_VECTOR vertices 4 non-coplanar vertices
 */
void ConvexHullBuilder::tetrahedronMaker(PointsVector vertices, int determinant){

    //Adding passed vertices to the Dcel and assigning each one of them to a Dcel::Vertex* v
    Dcel::Vertex* v1 = this->dcel->addVertex(vertices[0]);
    Dcel::Vertex* v2 = this->dcel->addVertex(vertices[1]);
    Dcel::Vertex* v3 = this->dcel->addVertex(vertices[2]);
    Dcel::Vertex* v4 = this->dcel->addVertex(vertices[3]);

    //Initializing first 3 Half Edges
    Dcel::HalfEdge* h1 = this->dcel->addHalfEdge();
    Dcel::HalfEdge* h2 = this->dcel->addHalfEdge();
    Dcel::HalfEdge* h3 = this->dcel->addHalfEdge();

    /** In order to ensure that we are always working in counter-clockwise way
     *  we need to change settings based on the determinant calculated before **/
    //If determinant is > 0
    if(determinant == 1){
        h1->setFromVertex(v1);
        h1->setToVertex(v2);
        h1->setNext(h2);
        h1->setPrev(h3);

        h2->setFromVertex(v2);
        h2->setToVertex(v3);
        h2->setNext(h3);
        h2->setPrev(h1);

        h3->setFromVertex(v3);
        h3->setToVertex(v1);
        h3->setNext(h1);
        h3->setPrev(h2);

     //if determinant is < 0
     } else {
        h1->setFromVertex(v2);
        h1->setToVertex(v1);
        h1->setNext(h3);
        h1->setPrev(h2);

        h2->setFromVertex(v3);
        h2->setToVertex(v2);
        h2->setNext(h1);
        h2->setPrev(h3);

        h3->setFromVertex(v1);
        h3->setToVertex(v3);
        h3->setNext(h2);
        h3->setPrev(h1);
     }

    //Setting the cardinality of vertices
    v1->setCardinality(2);
    v2->setCardinality(2);
    v3->setCardinality(2);

    //Create and adding a new face based on previous edges
    Face initialFace = this->dcel->addFace();

    //Setting Outher Edge to access external face
    initialFace->setOuterHalfEdge(h1);
    h1->setFace(initialFace);
    h2->setFace(initialFace);
    h3->setFace(initialFace);

    //Adding faces to tetrahedron
    addFaceTotetrahedron(v4, h1);
    addFaceTotetrahedron(v4, h2);
    addFaceTotetrahedron(v4, h3);

}

/**
 * @brief  ConvexHullBuilder::addFaceTotetrahedron adds 3 new faces from the last vertix and a given half edge
 * @param  VERTEX lastVertex, HALF_EDGE lastVertex given half edge
 */
void ConvexHullBuilder::addFaceTotetrahedron(Vertex lastVertex, HalfEdge passedEdge){

    //Initializing Vertices for passedEdge
    Vertex fromVertex = passedEdge->getFromVertex();
    Vertex toVertex   = passedEdge->getToVertex();

    //Initializing new half edges
    HalfEdge he1 = this->dcel->addHalfEdge();
    he1->setFromVertex(toVertex);
    he1->setToVertex(fromVertex);

    HalfEdge he2 = this->dcel->addHalfEdge();
    he2->setFromVertex(fromVertex);
    he2->setToVertex(lastVertex);

    HalfEdge he3 = this->dcel->addHalfEdge();
    he3->setFromVertex(lastVertex);
    he3->setToVertex(toVertex);

    //Next and prevs
    he1->setNext(he2);
    he1->setPrev(he3);

    he2->setNext(he3);
    he2->setPrev(he1);

    he3->setNext(he1);
    he3->setPrev(he2);

    //Incident Half Edges, when a new edge 'starts' from them
    toVertex->setIncidentHalfEdge(he1);
    fromVertex->setIncidentHalfEdge(he2);
    lastVertex->setIncidentHalfEdge(he3);

    //Setting Twins
    he1->setTwin(passedEdge);
    passedEdge->setTwin(he1);

    //Empty at the first steps but filled in next steps
    if(passedEdge->getPrev()->getTwin() != nullptr){
       HalfEdge he2_twin = passedEdge->getPrev()->getTwin()->getPrev();
       he2->setTwin(he2_twin);
       he2_twin->setTwin(he2);
    }

    //Empty at the first steps but filled in next steps
    if(passedEdge->getNext()->getTwin() != nullptr){
            HalfEdge he3_twin = passedEdge->getNext()->getTwin()->getNext();
            he3->setTwin(he3_twin);
            he3_twin->setTwin(he3);
    }

    //Cardinality, increments each time a new half edge 'starts' or 'ends' in a certain vertix
    fromVertex->incrementCardinality();
    fromVertex->incrementCardinality();

    toVertex->incrementCardinality();
    toVertex->incrementCardinality();

    lastVertex->incrementCardinality();
    lastVertex->incrementCardinality();

    //Creating a new face from newly created edges
    Face newFace = this->dcel->addFace();
    //NewFace will access external faces by he1
    newFace->setOuterHalfEdge(he1);
    //setting each edge to newFace
    he1->setFace(newFace);
    he2->setFace(newFace);
    he3->setFace(newFace);

}

/**
 * @brief  VERTEX_POINTERS_LIST ConvexHullBuilder::addVertices takes dcel
 * @return returns array of all vertices
 */
std::vector<Dcel::Vertex*>  ConvexHullBuilder::addVertices(VertexPointersList allVertices){
    //Adds all vertices to an array
    for (Dcel::VertexIterator vertex = dcel->vertexBegin(); vertex != dcel->vertexEnd(); vertex++){
        Dcel::Vertex* v = new Dcel::Vertex();
        v = *vertex;
        allVertices.push_back(v);
    }
    //returns array of pointers to vertices
    return allVertices;
}

/**
 * @brief  VERTEX_POINTERS_LIST ConvexHullBuilder::randomPointsGetter takes allVertices and returns an array of pointers to 4 random vertices
 * @param  VERTEX_POINTERS_LIST allVertx contains all the vertices
 * @return returns shuffled array of vertices
 */
std::vector<Dcel::Vertex*> ConvexHullBuilder::verticesShuffler(std::vector<Dcel::Vertex*> *allVertx){

    //From http://en.cppreference.com/w/cpp/algorithm/random_shuffle
    std::random_device rd;
    std::mt19937 g(rd());

    //compute a random permutation of the vertices vector
    std::shuffle(allVertx->begin(), allVertx->end(), g);

    //returns shuffled array
    return *allVertx;

}

/**
 * @brief  ConvexHullBuilder::coplanarityChecker Checks if passed vertices are coplanar
 * @param  VERTEX_POINTERS_LIST vertices contains passed vertices to get checked
 * @return 1 if coplanar, 0 else
 */
int ConvexHullBuilder::coplanarityChecker(PointsVector fourPoints){

    //Initializing Coplanarity
    bool coplanarity = true;
    //Tells it the determinant is greater (1) or lesser than 0 (0) for next steps
    int determinantRes = 0;

    //builds 4*4 matrix using eigen from @https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html
    Eigen::Matrix4d m;

    for(int i=0; i<4; i++){
      m(i, 0) = fourPoints[i].x();
      m(i, 1) = fourPoints[i].y();
      m(i, 2) = fourPoints[i].z();
      m(i, 3) = 1; //last column made of ones
    }

    //checks matrix determinant from @https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
    double det = m.determinant();

    //check if the determinant is 0 +- epsilon
    coplanarity = det > -std::numeric_limits<double>::epsilon() && det < std::numeric_limits<double>::epsilon();

    /** If it is not coplanar (==0) then, if det < 0 we return -1 else 1
      * if it is coplanar we return 0 **/
    if(coplanarity == 0){
        if(det < -std::numeric_limits<double>::epsilon()){
          determinantRes = -1;
        } else {
          determinantRes = 1;
        }
    } else {
        determinantRes = 0;
    }

    //0 coplanar, 1 or -1 if not coplanar
    return determinantRes;
}

/**
 * @brief  ConvexHullBuilder::getFirstFourVertices gets first 4 vertices
 * @param  VERTEX_POINTERS_LIST allVertices contains all remaining vertices
 * @return array of vertices first 4 vertices pointers
 */
std::vector <Pointd> ConvexHullBuilder::getFirstFourVertices(VertexPointersList allVertices){
    //Initializing firstFourVertices Array
    PointsVector firstFourVertices;

    firstFourVertices.push_back(allVertices[0]->getCoordinate());
    firstFourVertices.push_back(allVertices[1]->getCoordinate());
    firstFourVertices.push_back(allVertices[2]->getCoordinate());
    firstFourVertices.push_back(allVertices[3]->getCoordinate());

    return firstFourVertices;
}

/**
 * @brief  ConvexHullBuilder::getVertices gets first 4 vertices*
 * @param  VERTEX_POINTERS_LIST sendVertices
 * @return array of vertices first 4 vertices pointers
 */
 std::vector<Dcel::Vertex*> ConvexHullBuilder::getVertices(VertexPointersList allVertices){
    //Init returning array
    VertexPointersList sendVertices;

    sendVertices.push_back(allVertices[0]);
    sendVertices.push_back(allVertices[1]);
    sendVertices.push_back(allVertices[2]);
    sendVertices.push_back(allVertices[3]);

    return sendVertices;
}
