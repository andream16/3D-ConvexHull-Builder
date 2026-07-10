# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++ desktop application that loads a 3D mesh, computes the minimum convex polyhedron (convex hull) enclosing all of its vertices, and renders both the original model and the hull side-by-side in an interactive OpenGL viewer. It was developed as a university project for an Algorithms and Data Structures 2 course.

## Technology Stack

| Layer | Technologies |
|-------|-------------|
| Language | C++11 |
| Build | Qt qmake (`.pro` file) |
| GUI / Rendering | Qt 5 Widgets, libQGLViewer, OpenGL |
| Linear Algebra | Eigen3 |
| Parsing / Utilities | Boost (tokenizer, bimap) |
| Geometry | CGAL (polygon triangulation), GMP |

## Algorithm

The project implements a **conflict-graph-based incremental 3D convex hull** algorithm:

1. Extract all vertices from the input mesh.
2. Build an initial tetrahedron from four non-coplanar points (validated with an Eigen determinant check).
3. Initialize a conflict graph that maps each face to the vertices visible from it and vice-versa.
4. For each remaining vertex that lies outside the current hull:
   - Identify visible faces via the conflict graph.
   - Compute the horizon (boundary edges between visible and non-visible faces).
   - Remove visible faces and construct new triangular faces connecting the horizon to the vertex.
   - Update the conflict graph for the new faces.
5. Finalize normals, assign a distinct color to the hull, and display elapsed time.

An optional "Show Phases" mode refreshes the 3D view after each vertex insertion for educational/debugging purposes.

## Project Structure

```
├── README.md                        # Usage instructions
├── models/                          # Sample OBJ meshes (bunny, cow, homer, sphere, …)
└── ConvexHull/                      # Application source
    ├── 65034_medda_andrea.pro       # Qt project file
    ├── main.cpp                     # Entry point
    ├── convexhullbuilder.*          # Algorithm orchestrator
    ├── conflictgraph.*              # Face–vertex visibility tracking
    ├── tetrahedronbuilder.*         # Seed tetrahedron construction
    ├── facebuilderhelper.*          # New face creation from horizon edges
    ├── convexhullbuilderhelper.*    # Horizon computation, vertex extraction
    ├── GUI/
    │   ├── mainwindow.*             # Main window with manager toolbox
    │   ├── glcanvas.*               # QGLViewer-based OpenGL canvas
    │   └── managers/                # DcelManager, ConvexHullManager, WindowManager
    └── lib/
        ├── dcel/                    # Doubly-Connected Edge List mesh library
        └── common/                  # Point, bounding box, timers, comparators
```

## Data Structure

The mesh is represented with a full **DCEL (Doubly-Connected Edge List)** implementation supporting:

- Vertices, half-edges, and faces with full adjacency navigation.
- Import/export in OBJ, PLY, and a custom `.dcel` binary format.
- Optional CGAL-powered constrained Delaunay triangulation for non-triangle polygons.
- OpenGL rendering with flat/smooth shading, wireframe, and per-vertex coloring.

## Building and Running

Prerequisites: GCC/G++ (~6.2), Qt (~5.7), plus `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`.

1. Open the `.pro` file in Qt Creator or run `qmake && make`.
2. Launch the application.
3. Click **Load Dcel** and select a model from the `models/` directory.
4. Switch to **Convex Hull Solver** and click **Convex Hull!** (optionally enable **Show Phases**).
