# Repository Overview

## Purpose

This repository implements a **3D Convex Hull Builder** — a C++11/Qt5 desktop application created for a university "Algorithms and Data Structures 2" course. Given a 3D mesh as input, it computes the minimum convex polyhedron (convex hull) that contains all vertices of the mesh and renders the result in an interactive OpenGL viewer.

## Technology Stack

- **Language:** C++11
- **GUI Framework:** Qt 5, libQGLViewer (OpenGL-based 3D viewer)
- **Libraries:** Eigen3, Boost, CGAL, GMP
- **Build System:** qmake (`.pro` file)
- **Data Structure:** DCEL (Doubly-Connected Edge List) for mesh representation

## Core Algorithm

The convex hull is computed using an **incremental construction algorithm** with a **conflict graph**:

1. Load a 3D mesh and extract its vertices
2. Build an initial tetrahedron from 4 non-coplanar points
3. Initialize a conflict graph linking unprocessed vertices to visible faces
4. For each remaining vertex:
   - Identify the horizon (boundary edges of visible faces)
   - Delete visible faces from the hull
   - Construct new triangular faces connecting the vertex to the horizon
   - Update the conflict graph
5. Output the final convex hull as a DCEL mesh, rendered in cyan

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Face-vertex conflict graph tracking
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New triangular face creation
├── convexhullbuilderhelper.*    # Horizon computation, vertex extraction
├── common.*                     # Shared utilities
├── main.cpp                     # Application entry point
├── 65034_medda_andrea.pro       # qmake project file
├── GUI/
│   ├── mainwindow.*             # Main application window
│   ├── glcanvas.*               # OpenGL rendering canvas
│   ├── managers/                # UI panel managers (DCEL, convex hull, window)
│   └── objects/                 # Geometric primitives (sphere, cylinder)
└── lib/
    ├── dcel/                    # Full DCEL mesh library (vertices, edges, faces, iterators)
    └── common/                  # Utilities (point, bounding box, timers, comparators)

models/                          # Sample 3D models (OBJ format): bunny, cow, homer, sphere, etc.
```

## How to Use

1. Build with Qt 5 and the required dependencies (Boost, CGAL, Eigen3, libQGLViewer)
2. Launch the application
3. Click **Load Dcel** and select a model from the `models/` directory
4. Click **Convex Hull Solver**
5. Choose **Show Phases** (step-by-step) or **Convex Hull!** (immediate result)

## Author

Andrea Medda (student ID 65034). DCEL library by Muntoni/Scalas; GUI shell by Marco Livesu.
