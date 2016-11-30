#ifndef CONVEXHULLBUILDERHELPER_H
#define CONVEXHULLBUILDERHELPER_H

#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include "lib/dcel/drawable_dcel.h"

class ConvexHullBuilderHelper
{
public:
    ConvexHullBuilderHelper(DrawableDcel* dcel);
    ~ConvexHullBuilderHelper(); //Class Destructor

    std::vector<Dcel::Vertex*>   getAllVertices();
    std::vector<Dcel::HalfEdge*> bringMeTheHorizon(std::tr1::unordered_set<Dcel::Face*>* const &facesVisibleByVertex) const;
    std::vector<Dcel::HalfEdge*> orderHorizon(std::vector<Dcel::HalfEdge*> const &unHorizon, std::tr1::unordered_map<Dcel::Vertex*, Dcel::Vertex*>* const &map) const;

private:
    DrawableDcel *dcel;
};

#endif // CONVEXHULLBUILDERHELPER_H
