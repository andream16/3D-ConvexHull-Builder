#ifndef TETRAHEDRONBUILDER_H
#define TETRAHEDRONBUILDER_H

#include "lib/dcel/drawable_dcel.h"
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>

class TetrahedronBuilder{

public:
    TetrahedronBuilder(DrawableDcel* dcel, std::vector<Dcel::Vertex*> allVertices);
    ~TetrahedronBuilder(); //Destructor Declaration
    void buildTetrahedron();

private:
    DrawableDcel *dcel;
    std::vector<Dcel::Vertex*> allVertices;

    std::vector<Dcel::Vertex*> verticesShuffler();
    std::vector <Pointd> getFirstFourVertices(std::vector<Dcel::Vertex*>);
    void buildTetrahedron(std::vector<Dcel::Vertex*>);
    int coplanarityChecker(std::vector <Pointd>);
    void tetrahedronMaker(std::vector <Pointd>, int);
    void addFaceTotetrahedron(Dcel::Vertex*, Dcel::HalfEdge*);

};

#endif // TETRAHEDRONBUILDER_H
