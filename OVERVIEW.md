# Repository Overview

## Purpose

**3D Convex Hull Builder** is a C++11 / Qt 5 desktop application developed for the *Algorithms and Data Structures 2* university course. It loads a 3D mesh (OBJ, PLY, or DCEL format), computes the minimum convex polyhedron (convex hull) enclosing all input vertices using an incremental algorithm with a conflict graph, and visualizes the result interactively via an OpenGL viewer.

## Technology Stack

| Layer | Technology |
|-------|-----------|
| Language | C++11 |
| GUI / Windowing | Qt 5, libQGLViewer |
| Linear Algebra | Eigen 3 |
| Geometry | CGAL, Boost |
| Build System | qmake (`.pro` file) |

## Algorithm

The convex hull is built incrementally:

1. **Load mesh** – parse an OBJ/PLY/DCEL file, extract vertices, reset the internal DCEL.
2. **Initial tetrahedron** – select four non-coplanar points and build the starting polyhedron.
3. **Conflict graph initialization** – for each remaining vertex, record which faces of the current hull it can "see."
4. **Iterative insertion** – for each unprocessed vertex:
   - Compute the horizon (boundary edges of the set of visible faces).
   - Delete visible faces.
   - Build new triangular faces connecting the vertex to the horizon.
   - Update the conflict graph.
5. **Result** – the final DCEL represents the convex hull and is rendered in cyan over the original mesh.

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Face-vertex conflict tracking
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New triangular face creation
├── convexhullbuilderhelper.*    # Vertex extraction, horizon computation
├── common.*                     # Shared constants and utilities
├── main.cpp                     # Application entry point
├── GUI/
│   ├── mainwindow.*             # Main application window
│   ├── glcanvas.*               # OpenGL rendering canvas
│   ├── managers/
│   │   ├── convexhullmanager.*  # Convex hull controls panel
│   │   ├── dcelmanager.*        # DCEL load/display panel
│   │   └── windowmanager.*      # Window layout management
│   └── objects/                 # Primitive geometry helpers
└── lib/
    ├── dcel/                    # Full DCEL mesh library (Muntoni/Scalas)
    └── common/                  # Utilities (point, bounding box, timers)

models/                          # Sample OBJ meshes (bunny, cow, homer, sphere, etc.)
```

## Building & Running

### Prerequisites

- GCC/G++ ≥ 6.2, Qt ≥ 5.7
- System packages: `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`

### Steps

```bash
qmake ConvexHull/65034_medda_andrea.pro
make
./65034_medda_andrea        # or whatever the output binary is named
```

> Library paths in the `.pro` file may need adjustment for your system.

### Usage

1. Click **Load Dcel** and select a model from `models/`.
2. Click **Convex Hull Solver**.
3. Choose **Show Phases** to step through the algorithm, or **Convex Hull!** to run it in one shot.

## Author

Andrea Medda (student ID 65034). DCEL library by Muntoni/Scalas; GUI shell by Marco Livesu.
