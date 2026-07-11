# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++11/Qt5 desktop application that computes the 3D convex hull of a loaded triangle mesh. Given any 3D model (OBJ, PLY, or DCEL format), it calculates the minimum convex polyhedron enclosing all input vertices and renders the result in an interactive OpenGL viewer.

This project was developed as a coursework deliverable for an **Algorithms and Data Structures 2** university course.

## Core Algorithm

The application implements an **incremental convex hull algorithm** backed by a **conflict graph**, following the classic approach from computational geometry (Clarkson–Shor style):

1. **Extract vertices** from the input mesh and reset the internal DCEL structure.
2. **Build an initial tetrahedron** from four non-coplanar points (verified via Eigen determinant checks).
3. **Initialize a conflict graph** that tracks bidirectional visibility between remaining vertices and hull faces.
4. **Iteratively insert** each remaining vertex:
   - Identify faces visible from the vertex via the conflict graph.
   - Compute the **horizon** — the boundary loop between visible and non-visible faces.
   - Delete visible faces and construct new triangular faces connecting the horizon to the vertex.
   - Update the conflict graph for the newly created faces.
5. **Render** the final convex hull (colored cyan) alongside the original mesh.

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Primary language |
| Qt 5 | GUI framework (widgets, OpenGL integration) |
| Eigen3 | Linear algebra (determinants, coplanarity checks) |
| Boost | Bimap, tokenizer, unordered containers |
| CGAL | Optional mesh triangulation and advanced DCEL operations |
| libQGLViewer | Interactive 3D OpenGL canvas |
| OpenGL / GLU | Mesh rendering |

## Project Structure

```
ConvexHull/
├── main.cpp                        # Application entry point
├── 65034_medda_andrea.pro          # Qt qmake build file
│
├── convexhullbuilder.*             # Algorithm orchestrator
├── convexhullbuilderhelper.*       # Vertex extraction, horizon computation
├── tetrahedronbuilder.*            # Initial tetrahedron construction
├── conflictgraph.*                 # Vertex-face conflict graph
├── facebuilderhelper.*             # New face construction from horizon edges
│
├── GUI/
│   ├── mainwindow.*                # Main window with toolbox panels
│   ├── glcanvas.*                  # QGLViewer-based 3D canvas
│   └── managers/
│       ├── dcelmanager.*           # Load/save/display meshes
│       ├── convexhullmanager.*     # Run solver, show phases
│       └── windowmanager.*         # Scene settings (background, axis, etc.)
│
└── lib/
    ├── dcel/                       # Full DCEL mesh library (~2,400 LOC)
    └── common/                     # Point, bounding box, serialization, timers

models/                             # 11 sample OBJ meshes (bunny, cow, homer, etc.)
```

## Key Components

- **ConvexHullBuilder** — orchestrates the full pipeline from mesh loading through hull computation.
- **ConflictGraph** — maintains two bidirectional maps (`face → visible vertices` and `vertex → visible faces`) using `boost::bimap`, updated incrementally as faces are added and removed.
- **TetrahedronBuilder** — randomly shuffles vertices and selects four non-coplanar points to seed the hull.
- **DCEL Library** (`lib/dcel/`) — a reusable Doubly-Connected Edge List implementation supporting vertex/half-edge/face manipulation, file I/O (OBJ, PLY, native binary), adjacency iterators, and OpenGL rendering.
- **ConvexHullManager** — Qt panel that triggers the solver, optionally enables step-by-step visualization, and times execution.

## Building and Running

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

Library paths in the `.pro` file may need adjustment for your system (Boost version paths are hardcoded to 1.62.0).

**Dependencies:** `gcc/g++` ~6.2.1, Qt ~5.7, `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`.

## Authorship

| Component | Author |
|---|---|
| Convex hull algorithm | Andrea Medda (student project) |
| DCEL library | Alessandro Muntoni, Andreas Scalas (2016, GPL) |
| GUI framework shell | Marco Livesu (2014) |
