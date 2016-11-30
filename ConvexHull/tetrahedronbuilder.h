#ifndef TETRAHEDRONBUILDER_H
#define TETRAHEDRONBUILDER_H

#include "lib/dcel/drawable_dcel.h"
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <random>

#include "facebuilderhelper.h"

class TetrahedronBuilder{

public:
    TetrahedronBuilder(DrawableDcel* dcel, const std::vector<Dcel::Vertex*> &allVertices);
    ~TetrahedronBuilder(); //Destructor Declaration
    std::vector<Dcel::Vertex*> buildTetrahedron();

private:
    DrawableDcel *dcel;
    std::vector<Dcel::Vertex*> allVertices;
    FaceBuilderHelper *faceBuilderHelper;

    std::vector<Dcel::Vertex*> verticesShuffler();
    std::vector <Pointd> getFirstFourVertices(std::vector<Dcel::Vertex*> const &allVertices) const;
    void buildTetrahedron(std::vector<Dcel::Vertex*>);
    int  coplanarityChecker(const std::vector<Pointd> &fourPoints) const;
    std::vector<Dcel::HalfEdge*> tetrahedronMaker(std::vector<Pointd> const &vertices, int const &determinant) const;

};

#endif // TETRAHEDRONBUILDER_H
