#ifndef CONVEXHULLBUILDERHELPER_H
#define CONVEXHULLBUILDERHELPER_H

#include "lib/dcel/drawable_dcel.h"

class ConvexHullBuilderHelper
{
public:
    ConvexHullBuilderHelper(DrawableDcel* dcel);
    ~ConvexHullBuilderHelper(); //Class Destructor

    std::vector<Dcel::Vertex*>   getAllVertices();
    std::vector<Dcel::HalfEdge*> bringMeTheHorizon(std::set<Dcel::Face*>*);
    std::vector<Dcel::HalfEdge*> orderHorizon(std::vector<Dcel::HalfEdge*>, std::map<Dcel::Vertex*, Dcel::Vertex*>&);

private:
    DrawableDcel *dcel;
};

#endif // CONVEXHULLBUILDERHELPER_H
