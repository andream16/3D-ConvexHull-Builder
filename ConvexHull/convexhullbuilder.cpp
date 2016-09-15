#include "convexhullbuilder.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define VERTEX_POINTERS_LIST std::vector<Dcel::Vertex*>
#define POINTS_VECTOR std::vector <Pointd>

/**
 * @brief ConvexHullBuilder::ConvexHullBuilder()
 * @params takes dcel as input
 */
ConvexHullBuilder::ConvexHullBuilder(DrawableDcel* dcel){
   this->dcel = dcel;
}

/**
 * @brief ConvexHullBuilder::computeConvexHull() takes dcel as input.
 *        Starts the algorithm calling all the different functions needed.
 *        Adds Vertices, Builds a Tetrahedron . . .
 */
void ConvexHullBuilder::computeConvexHull(){

    //Array of vertices
    VERTEX_POINTERS_LIST allVertices;

    //Adds all Dcel vertices pointers to allVertices
    allVertices = addVertices(allVertices);

    /** Checks Coplanarity of 4 vertices,
     *  if coplanar -> pick other 4 vertices, if not -> build a tetrahedron with them**/
    buildTetrahedron(allVertices);

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
    //Initialize bool var coplanarity, tells if the given vertices are coplanar (true) or not (false)
    bool coplanarity = true;

    //while we get 4 not coplanar vertices
    while(coplanarity){
     //shuffles all vertices
     shuffledVertices = verticesShuffler(allVertices);
     //gets first 4 points (0-3) from all vertices remaining
     fourPoints = getFirstFourVertices(shuffledVertices);
     //check if the first 4 vertices are coplanar (0 not coplanar, 1 coplanar)
     coplanarity = coplanarityChecker(fourPoints);
    }

    std::cout << coplanarity;
}

/**
 * @brief  VERTEX_POINTERS_LIST ConvexHullBuilder::addVertices takes dcel
 * @return returns array of all vertices
 */
VERTEX_POINTERS_LIST ConvexHullBuilder::addVertices(VERTEX_POINTERS_LIST allVertices){
    //Adds all vertices to an array
    for (Dcel::VertexIterator vit = dcel->vertexBegin(); vit != dcel->vertexEnd(); ++vit){
        Dcel::Vertex* v = *vit;
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
bool ConvexHullBuilder::coplanarityChecker(POINTS_VECTOR fourPoints){

    //Initializing Coplanarity
    bool coplanarity = true;

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

    //0 if not coplanar, 1 else
    return coplanarity;
}

/**
 * @brief  ConvexHullBuilder::getFirstFourVertices gets first 4 vertices
 * @param  VERTEX_POINTERS_LIST allEVrtices contains all remaining vertices
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

#undef VERTEX_POINTERS_LIST
#undef POINTS_VECTOR
