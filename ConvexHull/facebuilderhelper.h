#ifndef FACEBUILDERHELPER_H
#define FACEBUILDERHELPER_H

#include "lib/dcel/drawable_dcel.h"

class FaceBuilderHelper{

public:
    FaceBuilderHelper(DrawableDcel* dcel);
    ~FaceBuilderHelper(); //Destructor Declaration
    void buildFaces(Dcel::Vertex*, std::vector<Dcel::HalfEdge*>);

private:
    DrawableDcel *dcel;
};

#endif // FACEBUILDERHELPER_H
