# Repository Overview

## Purpose

This is a **3D Convex Hull Builder** — a C++11/Qt5 desktop application developed for an "Algorithms and Data Structures 2" university course. It loads a 3D mesh (in OBJ, PLY, or DCEL format), computes the minimum convex polyhedron (convex hull) that encloses all input vertices, and renders the result in an interactive OpenGL viewer.

## Technology Stack

- **Language:** C++11
- **GUI Framework:** Qt 5
- **Linear Algebra:** Eigen3
- **Geometry:** CGAL, Boost
- **3D Rendering:** libQGLViewer / OpenGL
- **Build System:** qmake (`.pro` file)

## Core Algorithm

The application implements an **incremental convex hull** algorithm using a **conflict graph** data structure:

1. **Load mesh** — parse an OBJ/PLY/DCEL file and extract vertices
2. **Build initial tetrahedron** — select 4 non-coplanar points to form the starting polyhedron
3. **Initialize conflict graph** — track which unprocessed vertices can "see" which faces
4. **Iterate** — for each remaining vertex visible to the hull:
   - Compute the horizon (boundary edges of visible faces)
   - Delete visible faces from the hull
   - Build new triangular faces connecting the vertex to the horizon
   - Update the conflict graph
5. **Result** — the final convex hull mesh, stored in a DCEL and rendered in cyan

## Data Structure

The mesh is represented as a **DCEL (Doubly-Connected Edge List)**, which provides efficient traversal of vertices, half-edges, and faces and supports the dynamic insertions/deletions needed during hull construction.

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Face–vertex conflict tracking
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New triangular face creation
├── convexhullbuilderhelper.*    # Vertex extraction, horizon computation
├── common.*                     # Shared utilities
├── main.cpp                     # Application entry point
├── GUI/
│   ├── mainwindow.*             # Top-level Qt window
│   ├── glcanvas.*               # OpenGL rendering canvas
│   ├── managers/                # UI panels (window, DCEL, convex hull)
│   └── objects/                 # Geometric primitives (sphere, cylinder)
└── lib/
    ├── dcel/                    # Full DCEL mesh library
    └── common/                  # Utilities (point, bounding box, timers)

models/                          # Sample 3D models (bunny, cow, homer, etc.)
```

## How to Use

1. Build with qmake: `qmake 65034_medda_andrea.pro && make`
2. Launch the application
3. Click **Load Dcel** and select a model from the `models/` directory
4. Click **Convex Hull Solver**
5. Choose **Show Phases** to step through the algorithm, or **Convex Hull!** to run it in one shot

## Author

Andrea Medda (student ID 65034). DCEL library by Muntoni/Scalas; GUI shell by Marco Livesu.
