#include "tetrahedronbuilder.h"
#include <stdlib.h>

/** @brief Class used to build the starting Tetrahedron, inserts first items in the dcel
 *  @param Dcel dcel**/
TetrahedronBuilder::TetrahedronBuilder(DrawableDcel* dcel, std::vector<Dcel::Vertex*> allVertices){
    this->dcel = dcel;
    this->allVertices = allVertices;
}

/**
 * @brief TetrahedronBuilder Class Destructor
 **/
TetrahedronBuilder::~TetrahedronBuilder(){}

/**
 * @brief TetrahedronBuilder::buildTetrahedron builds a tetrahedron with different steps
 *        - Until getting 4 non-coplanar vertices
 *          - Shuffles all vertices
 *          - Gets first four vertices
 *          - Checks Coplanarity
 *        - Builds Tetrahedron using the latter 4 non-coplanar vertices
 *
 */
void TetrahedronBuilder::buildTetrahedron(){
    //Initialize Array of Shuffled Vertices
    std::vector<Dcel::Vertex*> shuffledVertices;
    //Initialize vector that will contain picked 4 points to get checked
    std::vector<Pointd> fourPoints;

    /** Initialize int var coplanarity, tells if the given vertices are coplanar (0) or not (1 or -1)
     *  if matrix's determinant < 0 coplanarity will return -1 else, it'll return 1 */
    int coplanarity = 0;

    //while we get 4 not coplanar vertices
    while(coplanarity == 0){
     //shuffles all vertices
     shuffledVertices = verticesShuffler();
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
 * @brief  std::vector<Dcel::Vertex*> TetrahedronBuilder::verticesShuffler takes allVertices
 * @return returns shuffled array of vertices
 */
std::vector<Dcel::Vertex*> TetrahedronBuilder::verticesShuffler(){

    //From http://en.cppreference.com/w/cpp/algorithm/random_shuffle
    std::random_device rd;
    std::mt19937 g(rd());

    //compute a random permutation of the vertices vector
    std::shuffle(allVertices.begin(), allVertices.end(), g);

    //returns shuffled array
    return allVertices;

}

/**
 * @brief  std::vector <Pointd> TetrahedronBuilder::getFirstFourVertices(std::vector<Dcel::Vertex*> allVertices)
 *         gets first four vertices from all vertices passed
 * @param  std::vector<Dcel::Vertex*> allVertices contains all shuffled vertices
 * @return array of first 4 pointers to vertices
 */
std::vector <Pointd> TetrahedronBuilder::getFirstFourVertices(std::vector<Dcel::Vertex*> allVertices){
    //Initializing firstFourVertices Array
    std::vector<Pointd> firstFourVertices;

    firstFourVertices.push_back(allVertices[0]->getCoordinate());
    firstFourVertices.push_back(allVertices[1]->getCoordinate());
    firstFourVertices.push_back(allVertices[2]->getCoordinate());
    firstFourVertices.push_back(allVertices[3]->getCoordinate());

    return firstFourVertices;
}

/**
 * @brief  int TetrahedronBuilder::coplanarityChecker(std::vector<Pointd> fourPoints)
 * @param  std::vector<Pointd> fourPoints
 * @return 0 if coplanar, 1 or -1 else
 */
int TetrahedronBuilder::coplanarityChecker(std::vector<Pointd> fourPoints){

    //Initialize Coplanarity
    bool coplanarity = true;
    //Tells it the determinant is greater (1) or lesser than 0 (0) for next steps
    int determinantRes = 0;

    //builds 4*4 matrix using eigen from @https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html
    Eigen::Matrix4d m;

    //Add all the points to the Matrix by their x, y and z coordinates
    for(int i=0; i<4; i++){
      m(i, 0) = fourPoints[i].x();
      m(i, 1) = fourPoints[i].y();
      m(i, 2) = fourPoints[i].z();
      m(i, 3) = 1; //last column made of ones
    }

    //Check Matrix determinant
    double det = m.determinant();

    //Check if the determinant is 0 +- epsilon
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
 * @brief  void TetrahedronBuilder::tetrahedronMaker(std::vector<Pointd> vertices, int determinant)
 *         takes the four non-coplanar vertices and inserts their data in the dcel
 *         to build the tetrahedron itself basing orientation on determinant result
 * @param  std::vector<Pointd> vertices 4 non-coplanar vertices
 */
void TetrahedronBuilder::tetrahedronMaker(std::vector<Pointd> vertices, int determinant){

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
    Dcel::Face* initialFace = this->dcel->addFace();

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
 * @brief  void TetrahedronBuilder::addFaceTotetrahedron(Dcel::Vertex* lastVertex, Dcel::HalfEdge* passedEdge)
 *         adds 3 new faces from the last vertex and a given half edge
 * @param  VERTEX lastVertex, HALF_EDGE lastVertex given half edge
 */
void TetrahedronBuilder::addFaceTotetrahedron(Dcel::Vertex* lastVertex, Dcel::HalfEdge* passedEdge){

    //Initialize Vertices for passedEdge
    Dcel::Vertex* fromVertex = passedEdge->getFromVertex();
    Dcel::Vertex* toVertex   = passedEdge->getToVertex();

    //Initialize new half edges
    Dcel::HalfEdge* he1 = this->dcel->addHalfEdge();
    he1->setFromVertex(toVertex);
    he1->setToVertex(fromVertex);

    Dcel::HalfEdge* he2 = this->dcel->addHalfEdge();
    he2->setFromVertex(fromVertex);
    he2->setToVertex(lastVertex);

    Dcel::HalfEdge* he3 = this->dcel->addHalfEdge();
    he3->setFromVertex(lastVertex);
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
    lastVertex->setIncidentHalfEdge(he3);

    //Set Twins
    he1->setTwin(passedEdge);
    passedEdge->setTwin(he1);

    //Empty at the first steps but filled in next steps
    if(passedEdge->getPrev()->getTwin() != nullptr){
       Dcel::HalfEdge* he2_twin = passedEdge->getPrev()->getTwin()->getPrev();
       he2->setTwin(he2_twin);
       he2_twin->setTwin(he2);
    }

    //Empty at the first steps but filled in next steps
    if(passedEdge->getNext()->getTwin() != nullptr){
            Dcel::HalfEdge* he3_twin = passedEdge->getNext()->getTwin()->getNext();
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

    //Create a new face from newly created edges
    Dcel::Face* newFace = this->dcel->addFace();
    //NewFace will access external faces by he1
    newFace->setOuterHalfEdge(he1);
    //Set each edge to newFace
    he1->setFace(newFace);
    he2->setFace(newFace);
    he3->setFace(newFace);

}
