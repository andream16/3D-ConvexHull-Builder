# Repository Overview

## Purpose

This is a **3D Convex Hull Builder** — a C++11/Qt5 desktop application developed for an "Algorithms and Data Structures 2" university course. It loads a 3D mesh (OBJ, PLY, or DCEL format), computes the minimum convex polyhedron (convex hull) that contains all input vertices, and visualizes the result in an interactive OpenGL viewer.

## Tech Stack

- **Language:** C++11
- **GUI Framework:** Qt 5 with libQGLViewer for 3D rendering
- **Math Libraries:** Eigen3, Boost, CGAL
- **Data Structure:** DCEL (Doubly-Connected Edge List) for half-edge mesh representation
- **Build System:** Qt qmake (`.pro` file)

## Algorithm

The core algorithm is an **incremental convex hull construction with a conflict graph**:

1. Load the 3D mesh and extract all vertices
2. Construct an initial tetrahedron from 4 non-coplanar points
3. Initialize a conflict graph linking remaining vertices to visible faces
4. For each unprocessed vertex (picked from the conflict graph):
   - Determine the set of faces visible from that vertex
   - Compute the horizon (boundary edges of the visible region)
   - Delete visible faces and build new triangular faces connecting the vertex to horizon edges
   - Update the conflict graph for newly created faces
5. Output the resulting convex hull mesh, rendered in cyan over the original model

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Face-vertex conflict graph tracking
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New triangular face creation
├── convexhullbuilderhelper.*    # Vertex extraction, horizon computation
├── common.*                     # Shared constants and utilities
├── main.cpp                     # Application entry point
├── GUI/
│   ├── mainwindow.*             # Main application window
│   ├── glcanvas.*               # OpenGL rendering canvas
│   ├── managers/
│   │   ├── windowmanager.*      # Window layout manager
│   │   ├── dcelmanager.*        # DCEL load/save UI
│   │   └── convexhullmanager.*  # Convex hull controls UI
│   └── objects/                 # Geometric primitives for rendering
└── lib/
    ├── dcel/                    # Full DCEL mesh library (vertices, edges, faces, iterators)
    └── common/                  # Utilities (point, bounding box, timers, serialization)

models/                          # Sample OBJ meshes (bunny, cow, homer, sphere, etc.)
```

## How to Build and Run

### Prerequisites

- GCC/G++ ~6.2.1
- Qt ~5.7
- `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`

### Running

1. Open the project in Qt Creator or run `qmake` on the `.pro` file
2. Build and launch the application
3. Click **Load Dcel** and select a model from the `models/` directory
4. Click **Convex Hull Solver**
5. Choose **Show Phases** for step-by-step visualization, or **Convex Hull!** for immediate computation

## Author

Andrea Medda (student ID 65034). DCEL library by Muntoni/Scalas; GUI shell by Marco Livesu.
