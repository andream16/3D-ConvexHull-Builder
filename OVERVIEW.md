# Repository Overview: 3D-ConvexHull-Builder

## Purpose

This is a **C++/Qt5 desktop application** that loads 3D mesh models and computes their **3D convex hull** — the smallest convex polyhedron enclosing all input vertices. It was built as coursework for an Algorithms and Data Structures 2 university course by Andrea Medda.

Users can load a mesh (OBJ, PLY, or DCEL format), visualize it in an interactive OpenGL viewer, compute its convex hull, and optionally step through each phase of the algorithm.

## Core Algorithm

The application implements an **incremental 3D convex hull** algorithm:

1. Extract all vertices from the input mesh (stored as a DCEL — Doubly-Connected Edge List)
2. Build an initial tetrahedron from four non-coplanar vertices
3. Initialize a **conflict graph** mapping unprocessed vertices to visible faces
4. For each remaining vertex: identify visible faces, compute the horizon boundary, remove visible faces, create new triangular faces connecting the horizon to the vertex, and update the conflict graph
5. Render the resulting convex hull in the 3D viewer

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 (core, gui, widgets, opengl, xml) | Application framework and UI |
| libQGLViewer | Interactive 3D OpenGL rendering |
| Eigen3 | Linear algebra (determinant computations) |
| Boost | Bimap for UI mappings, tokenizer for file parsing |
| CGAL (optional) | Mesh triangulation and polygon operations |
| GMP | Arbitrary-precision arithmetic (CGAL dependency) |

**Build system:** Qt `qmake` via `65034_medda_andrea.pro`.

## Project Structure

```
ConvexHull/
├── main.cpp                        # Application entry point
├── convexhullbuilder.*             # Main algorithm orchestrator
├── conflictgraph.*                 # Conflict graph (vertex ↔ face visibility)
├── tetrahedronbuilder.*            # Initial tetrahedron construction
├── convexhullbuilderhelper.*       # Vertex extraction, horizon computation
├── facebuilderhelper.*             # New face creation during hull expansion
├── GUI/
│   ├── mainwindow.*                # Main window and manager toolbox
│   ├── glcanvas.*                  # QGLViewer-based OpenGL canvas
│   └── managers/                   # UI managers (mesh, hull, window settings)
└── lib/
    ├── dcel/                       # DCEL mesh library (vertices, edges, faces)
    └── common/                     # Utilities (points, bounding boxes, timers)

models/                             # Sample OBJ meshes (bunny, cow, homer, etc.)
```

## How to Build and Run

**Prerequisites:** GCC/G++ ~6.2.1, Qt ~5.7, libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev, Eigen3.

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

**Usage:** Load a mesh via "Load Dcel", switch to the "Convex Hull Solver" tab, optionally enable "Show Phases", and click "Convex Hull!" to compute and visualize the result.
