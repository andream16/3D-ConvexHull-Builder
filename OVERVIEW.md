# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application developed for the *Algorithms and Data Structures 2* university course. It loads a 3D mesh (OBJ, PLY, or DCEL format), extracts its vertices, computes the **3D convex hull** (the smallest convex polyhedron enclosing all vertices), and renders the result in an interactive OpenGL viewer.

## Core Algorithm

The project implements the **incremental convex hull algorithm with a conflict graph**, a well-known approach from computational geometry:

1. **Seed tetrahedron** — Four non-coplanar points are selected (coplanarity checked via a 4x4 determinant using Eigen) and used to build an initial tetrahedron.
2. **Conflict graph initialization** — A bipartite structure mapping faces to the vertices visible from them (and vice versa) is built using half-space visibility tests.
3. **Incremental insertion** — For each remaining vertex:
   - Retrieve visible faces from the conflict graph.
   - Compute the **horizon**: the boundary edges between visible and non-visible faces.
   - Delete visible faces from the DCEL and conflict graph.
   - Build new triangular faces connecting the horizon edges to the inserted vertex.
   - Update the conflict graph for the newly created faces.
4. **Result** — A triangle mesh stored in a DCEL, rendered in cyan alongside the original model.

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, forms, OpenGL module) |
| QGLViewer | 3D camera navigation and OpenGL canvas |
| Eigen3 | Matrix math (determinant-based coplanarity check) |
| Boost | `bimap` for UI mappings, tokenizer for I/O |
| CGAL | Optional mesh triangulation on load |
| OpenGL / GLU | 3D rendering |

## Project Structure

```
ConvexHull/
├── main.cpp                      # Entry point
├── convexhullbuilder.*           # Algorithm orchestrator (main loop)
├── conflictgraph.*               # Face-vertex conflict graph
├── tetrahedronbuilder.*          # Initial tetrahedron construction
├── convexhullbuilderhelper.*     # Horizon computation, vertex extraction
├── facebuilderhelper.*           # New triangular face creation
├── GUI/
│   ├── mainwindow.*              # Main window with manager toolbox
│   ├── glcanvas.*                # QGLViewer-based render canvas
│   └── managers/
│       ├── dcelmanager.*         # Load/save/display input mesh
│       ├── convexhullmanager.*   # Run convex hull, show results
│       └── windowmanager.*       # Background, snapshot, axis controls
└── lib/
    ├── dcel/                     # Full DCEL mesh library (~2000+ lines)
    │   ├── dcel_struct.*         # Core DCEL (load/save, iterators)
    │   ├── dcel_vertex.*, dcel_half_edge.*, dcel_face.*
    │   └── drawable_dcel.*       # DCEL with OpenGL rendering
    └── common/                   # Point, bounding box, timer utilities

models/                           # Sample OBJ meshes (bunny, cow, sphere, etc.)
```

## Key Data Structure: DCEL

The mesh is represented as a **Doubly-Connected Edge List (DCEL)** — a half-edge data structure with vertices, half-edges (each storing next/prev/twin pointers), and faces. This allows efficient traversal of mesh topology, which is critical for horizon computation and face insertion during the convex hull algorithm.

## Building and Running

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

**Dependencies:** `gcc`/`g++` (C++11), Qt 5, `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`, `libeigen3-dev`.

**Usage:**
1. Launch the application.
2. In the **Dcel** panel, click **Load Dcel** and select a model from `models/`.
3. Switch to **Convex Hull Solver**.
4. Optionally enable **Show Phases** for step-by-step visualization.
5. Click **Convex Hull!** to compute and display the result.

## Author

Andrea Medda (student ID 65034). The DCEL library was authored by Muntoni/Scalas; the GUI shell was provided by Marco Livesu.
