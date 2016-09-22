#include "convexhullbuilder.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define VERTEX_POINTERS_LIST std::vector<Dcel::Vertex*>
#define POINTS_VECTOR std::vector <Pointd>
#define VERTEX Dcel::Vertex*
#define HALF_EDGE Dcel::HalfEdge*
#define FACE Dcel::Face*


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
    VERTEX_POINTERS_LIST allVertices;
    //Vertices To Get passed to Conflict Graph
    VERTEX_POINTERS_LIST verticesForCG;
    //Four non coplanar vertices
    POINTS_VECTOR fourVertices;

    //Adds all Dcel vertices pointers to allVertices
    allVertices = addVertices(allVertices);

    //Reset Dcel
    dcel->reset();

    /** Checks Coplanarity of 4 vertices,
     *  if coplanar -> pick other 4 vertices, if not -> build a tetrahedron with them**/
    buildTetrahedron(allVertices);

    //Array of Pointers to Vertices needed for Conflict Graph Initialization
    verticesForCG = getVertices(allVertices);

    //Initializes Conflict Graph with Dcel And First 4 Vertices
    ConflictGraph conflictGraph = ConflictGraph(this->dcel, verticesForCG);
    conflictGraph.initializeConflictGraph();

}

/**
 * @brief ConvexHullBuilder::buildTetrahedron builds a tetrahedron with different steps
 * @param VERTEX_POINTERS_LIST allVertices array of all vertices
 */
void ConvexHullBuilder::buildTetrahedron(VERTEX_POINTERS_LIST allVertices){
    //Initialize Array of Shuffled Vertices
    VERTEX_POINTERS_LIST shuffledVertices;
    //Initialize vector that will contain picked 4 points to get checked
    POINTS_VECTOR fourPoints;
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
void ConvexHullBuilder::tetrahedronMaker(POINTS_VECTOR vertices, int determinant){

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
    FACE initialFace = this->dcel->addFace();

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
void ConvexHullBuilder::addFaceTotetrahedron(VERTEX lastVertex, HALF_EDGE passedEdge){

    //Initializing Vertices for passedEdge
    VERTEX fromVertex = passedEdge->getFromVertex();
    VERTEX toVertex   = passedEdge->getToVertex();

    //Initializing new half edges
    HALF_EDGE he1 = this->dcel->addHalfEdge();
    he1->setFromVertex(toVertex);
    he1->setToVertex(fromVertex);

    HALF_EDGE he2 = this->dcel->addHalfEdge();
    he2->setFromVertex(fromVertex);
    he2->setToVertex(lastVertex);

    HALF_EDGE he3 = this->dcel->addHalfEdge();
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
       HALF_EDGE he2_twin = passedEdge->getPrev()->getTwin()->getPrev();
       he2->setTwin(he2_twin);
       he2_twin->setTwin(he2);
    }

    //Empty at the first steps but filled in next steps
    if(passedEdge->getNext()->getTwin() != nullptr){
            HALF_EDGE he3_twin = passedEdge->getNext()->getTwin()->getNext();
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
    FACE newFace = this->dcel->addFace();
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
VERTEX_POINTERS_LIST ConvexHullBuilder::addVertices(VERTEX_POINTERS_LIST allVertices){
    //Adds all vertices to an array
    for (Dcel::VertexIterator vertex = dcel->vertexBegin(); vertex != dcel->vertexEnd(); vertex++){
        Dcel::Vertex* v = *vertex;
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
VERTEX_POINTERS_LIST ConvexHullBuilder::verticesShuffler(VERTEX_POINTERS_LIST allVertx){

    //From http://en.cppreference.com/w/cpp/algorithm/random_shuffle
    std::random_device rd;
    std::mt19937 g(rd());

    //compute a random permutation of the vertices vector
    std::shuffle(allVertx.begin(), allVertx.end(), g);

    //returns shuffled array
    return allVertx;

}

/**
 * @brief  ConvexHullBuilder::coplanarityChecker Checks if passed vertices are coplanar
 * @param  VERTEX_POINTERS_LIST vertices contains passed vertices to get checked
 * @return 1 if coplanar, 0 else
 */
int ConvexHullBuilder::coplanarityChecker(POINTS_VECTOR fourPoints){

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
POINTS_VECTOR ConvexHullBuilder::getFirstFourVertices(VERTEX_POINTERS_LIST allVertices){
    //Initializing firstFourVertices Array
    POINTS_VECTOR firstFourVertices;

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
VERTEX_POINTERS_LIST ConvexHullBuilder::getVertices(VERTEX_POINTERS_LIST allVertices){
    //Init returning array
    VERTEX_POINTERS_LIST sendVertices;

    sendVertices.push_back(allVertices[0]);
    sendVertices.push_back(allVertices[1]);
    sendVertices.push_back(allVertices[2]);
    sendVertices.push_back(allVertices[3]);

    return sendVertices;
}

#undef FACE
#undef VERTEX
#undef HALF_EDGE
#undef VERTEX_POINTERS_LIST
#undef POINTS_VECTOR
