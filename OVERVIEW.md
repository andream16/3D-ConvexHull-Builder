# Repository Overview

## Purpose

This is a **3D Convex Hull Builder** — a C++/Qt5 desktop application that loads a 3D mesh and computes the minimum convex polyhedron (convex hull) enclosing all input vertices, then visualizes the result in an interactive OpenGL viewer. It was developed as a university project for an "Algorithms and Data Structures 2" course.

## Technology Stack

- **Language:** C++11
- **UI Framework:** Qt 5
- **Libraries:** Eigen3, Boost, CGAL, libQGLViewer
- **Build System:** qmake (`.pro` file)
- **Compiler:** GCC 6.2+

## Algorithm

The core algorithm is an **incremental convex hull construction** with a conflict graph:

1. Load a 3D mesh (OBJ format) and extract its vertices
2. Build an initial tetrahedron from 4 non-coplanar points
3. Initialize a conflict graph linking remaining vertices to visible faces
4. For each unprocessed vertex (chosen from the conflict graph):
   - Compute the horizon edges of the visible region
   - Delete all faces visible from the vertex
   - Construct new triangular faces connecting the vertex to the horizon
   - Update the conflict graph for the new faces
5. Output the convex hull as a DCEL mesh rendered in the OpenGL viewer

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*        # Main algorithm orchestrator
├── conflictgraph.*            # Face-vertex conflict graph tracking
├── tetrahedronbuilder.*       # Initial tetrahedron construction
├── facebuilderhelper.*        # New triangular face creation
├── convexhullbuilderhelper.*  # Vertex extraction, horizon computation
├── main.cpp                   # Application entry point
├── common.*                   # Shared utilities
├── GUI/
│   ├── mainwindow.*           # Application main window
│   ├── glcanvas.*             # OpenGL rendering canvas
│   └── managers/              # UI panel managers (DCEL, convex hull, window)
└── lib/
    ├── dcel/                  # Doubly-Connected Edge List mesh library
    └── common/                # Shared types (point, bounding box, timers)

models/                        # Sample OBJ meshes (bunny, cow, homer, sphere, etc.)
```

## How to Use

1. Build the project with `qmake` and `make`
2. Launch the application
3. Click **Load Dcel** and select a model from the `models/` directory
4. Click **Convex Hull Solver**
5. Choose **Show Phases** for step-by-step visualization or **Convex Hull!** for immediate computation

## Credits

- **Author:** Andrea Medda (student ID 65034)
- **DCEL Library:** Muntoni/Scalas
- **GUI Shell:** Marco Livesu
