# Repository Overview

## Purpose

**3D Convex Hull Builder** is a C++11/Qt5 desktop application that computes and visualizes the 3D convex hull of arbitrary meshes. Given any 3D model (OBJ, PLY, or DCEL format), it calculates the minimum convex polyhedron enclosing all input vertices and renders the result interactively via OpenGL.

This project was developed as coursework for "Algorithms and Data Structures 2" (student ID 65034, Andrea Medda).

## Core Algorithm

The application implements **incremental convex hull construction with a conflict graph**:

1. Load the mesh and extract all vertices; reset the output DCEL.
2. Select four non-coplanar points to build an initial tetrahedron.
3. Initialize a conflict graph that tracks which unprocessed vertices can "see" which faces.
4. For each remaining vertex (in arbitrary order):
   - Identify visible faces from the conflict graph.
   - Compute the horizon (boundary edges between visible and non-visible faces).
   - Delete visible faces from the hull.
   - Build new triangular faces connecting the vertex to horizon edges.
   - Update the conflict graph for the new faces.
5. The result is a convex hull mesh stored as a DCEL and rendered in cyan.

**Expected complexity:** O(n log n) average case for uniformly distributed points.

## Architecture

```
ConvexHull/
├── main.cpp                        Entry point
├── convexhullbuilder.h/cpp         Main algorithm orchestrator
├── convexhullbuilderhelper.h/cpp   Vertex extraction, horizon computation
├── conflictgraph.h/cpp             Face-vertex conflict tracking
├── tetrahedronbuilder.h/cpp        Initial tetrahedron construction
├── facebuilderhelper.h/cpp         New triangular face creation
├── common.h/cpp                    Shared utilities
├── GUI/
│   ├── mainwindow.h/cpp/ui         Application main window
│   ├── glcanvas.h/cpp              OpenGL rendering canvas (libQGLViewer)
│   ├── managers/
│   │   ├── windowmanager.*         Window layout management
│   │   ├── dcelmanager.*           DCEL load/save/display controls
│   │   └── convexhullmanager.*     Convex hull execution controls
│   └── objects/
│       ├── sphere.h                Sphere drawable
│       └── cylinder.h              Cylinder drawable
├── lib/
│   ├── dcel/                       Full DCEL mesh library (vertices, half-edges, faces, iterators)
│   └── common/                     Utilities (point, bounding box, timers, comparators)
└── 65034_medda_andrea.pro          Qt project file

models/                             Sample OBJ meshes (bunny, cow, homer, sphere, etc.)
```

## Technology Stack

| Component | Technology |
|-----------|-----------|
| Language | C++11 |
| GUI Framework | Qt 5 |
| 3D Viewer | libQGLViewer / OpenGL |
| Linear Algebra | Eigen3 |
| Geometry | CGAL |
| Utilities | Boost |
| Build System | qmake (.pro file) |

## Data Structures

- **DCEL (Doubly-Connected Edge List):** Represents the mesh topology with vertices, half-edges, and faces. Supports efficient adjacency queries and iterators needed by the incremental algorithm.
- **Conflict Graph:** A bipartite structure mapping unprocessed vertices to the hull faces they can see, enabling efficient identification of visible faces during each insertion step.

## How to Run

1. Install dependencies: `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`, Qt 5.
2. Open `ConvexHull/65034_medda_andrea.pro` in Qt Creator or run `qmake && make`.
3. Launch the application, click **Load Dcel** to load a model from `models/`.
4. Click **Convex Hull Solver**, then either **Show Phases** (step-by-step) or **Convex Hull!** (full computation).
