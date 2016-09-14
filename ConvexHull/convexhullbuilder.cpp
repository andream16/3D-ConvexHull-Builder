#include "convexhullbuilder.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define VERTEX_POINTERS_LIST std::vector<Dcel::Vertex*>

/**
 * @brief ConvexHullBuilder::ConvexHullBuilder()
 * @params takes dcel as input
 */
ConvexHullBuilder::ConvexHullBuilder(DrawableDcel* dcel){
   this->dcel = dcel;
}

/**
 * @brief ConvexHullBuilder::computeConvexHull() takes dcel as input. Starts the algorithm calling all the different functions needed.
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

    //shuffles all vertices the very first time
    shuffledVertices = verticesShuffler(allVertices);
    //checks if the first 4 vertices are coplanar
    coplanarity = coplanarityChecker(shuffledVertices);

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
 * @return true if coplanar, false else
 */
bool ConvexHullBuilder::coplanarityChecker(VERTEX_POINTERS_LIST vertices){

    //Initializes fourPoints Array
    std::vector <Pointd> fourPoints;
    //Initializing Coplanarity
    bool coplanarity = true;

    //gets first 4 points fro all vertices remaining
    fourPoints = getFirstFourVertices(vertices);

    //builds matrix using eigen to check the coplanarity
    Eigen::Matrix4d matrix;
            for(int i=0; i<4; i++){
                matrix(i, 0) = fourPoints[i].x();
                matrix(i, 1) = fourPoints[i].y();
                matrix(i, 2) = fourPoints[i].z();
                matrix(i, 3) = 1;
    }

            double det = matrix.determinant();

            //check if the determinant is 0 +- epsilon
            coplanarity = det > -std::numeric_limits<double>::epsilon() && det < std::numeric_limits<double>::epsilon();

            std::cout << coplanarity << endl;

}

/**
 * @brief  ConvexHullBuilder::getFirstFourVertices gets first 4 vertices
 * @param  VERTEX_POINTERS_LIST allEVrtices contains all remaining vertices
 * @return array of vertices first 4 vertices pointers
 */
std::vector <Pointd> ConvexHullBuilder::getFirstFourVertices(VERTEX_POINTERS_LIST allVertices){
    //Initializing firstFourVertices Array
    std::vector <Pointd> firstFourVertices;

    firstFourVertices.push_back(allVertices[0].getCoordinate());
    firstFourVertices.push_back(allVertices[1].getCoordinate());
    firstFourVertices.push_back(allVertices[2].getCoordinate());
    firstFourVertices.push_back(allVertices[3].getCoordinate());

    return firstFourVertices;
}

#undef VERTEX_POINTERS_LIST
