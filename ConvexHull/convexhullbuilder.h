#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <vector>
#include <GUI/managers/dcelmanager.h>
#include <lib/dcel/dcel_vertex_iterators.h>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>

#include "convexhullbuilderhelper.h"
#include "tetrahedronbuilder.h"
#include "conflictgraph.h"
#include "facebuilderhelper.h"

class ConvexHullBuilder{

public:
    ConvexHullBuilder(DrawableDcel* dcel, MainWindow* mainWindow, bool const &showPhases); //Constructor Declaration
    ~ConvexHullBuilder(); //Destructor Declaration
    void computeConvexHull();

private:
    DrawableDcel            *dcel;
    MainWindow*             mainWindow;
    bool                    showPhases;
    ConvexHullBuilderHelper *convexHullBuilderHelper;
    TetrahedronBuilder      *tetrahedronBuilder;
    ConflictGraph           *conflictGraph;
    FaceBuilderHelper       *faceBuilderHelper;
};

#endif // CONVEXHULL_H
