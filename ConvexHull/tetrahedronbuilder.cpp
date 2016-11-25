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
std::vector<Dcel::Vertex*> TetrahedronBuilder::buildTetrahedron(){

    //Instantiate FaceBuilderHelper
    faceBuilderHelper = new FaceBuilderHelper(dcel);

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
     *  build first tetrahedron face, orientation based on determinant value (coplanarity)
     * *Returns new halfedges*/
    std::vector<Dcel::HalfEdge*> newHalfEdges = tetrahedronMaker(fourPoints, coplanarity);

    //Build 3 new faces using the passed halfEdges and Vector
    faceBuilderHelper->buildFaces(shuffledVertices[3], newHalfEdges);

    return shuffledVertices;
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

    //If Coplanar
    if(!coplanarity){
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
 *         to build the tetrahedron itself basing orientation on determinant result.
 *         It builds the very first face of the tetrahedron, then, calls an helper function to build
 *         the other 3 faces
 * @param  std::vector<Pointd> vertices 4 non-coplanar vertices
 */
std::vector<Dcel::HalfEdge*> TetrahedronBuilder::tetrahedronMaker(std::vector<Pointd> vertices, int determinant){

    //Initialize Vector of Halfedges to contain new HalfEdges and build new faces on them
    std::vector<Dcel::HalfEdge*> halfEdges;

    //Adding passed vertices to the Dcel and assigning each one of them to a Dcel::Vertex* v
    Dcel::Vertex* v1 = this->dcel->addVertex(vertices[0]);
    Dcel::Vertex* v2 = this->dcel->addVertex(vertices[1]);
    Dcel::Vertex* v3 = this->dcel->addVertex(vertices[2]);

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

       v1->setIncidentHalfEdge(h1);
       v2->setIncidentHalfEdge(h2);
       v3->setIncidentHalfEdge(h3);


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

       v1->setIncidentHalfEdge(h3);
       v2->setIncidentHalfEdge(h1);
       v3->setIncidentHalfEdge(h2);

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

     //Add new halfedges to the vector
     halfEdges.push_back(h1);
     halfEdges.push_back(h2);
     halfEdges.push_back(h3);

     return halfEdges;

}
