# Repository Overview

## What This Repo Does

**3D-ConvexHull-Builder** is a C++11/Qt5 desktop application built for an *Algorithms and Data Structures 2* university course. It loads a 3D mesh, computes the **3D convex hull** (the smallest convex polyhedron enclosing all input vertices), and visualizes both the original mesh and the result in an interactive OpenGL viewer.

## Core Algorithm

The application implements the **incremental convex hull algorithm with a conflict graph**:

1. Extract all vertices from the loaded mesh.
2. Build an initial tetrahedron from four non-coplanar points.
3. Initialize a conflict graph mapping vertices to the faces visible from them.
4. For each remaining vertex:
   - Identify all faces visible from the vertex.
   - Compute the **horizon** — the boundary edge loop of the visible region.
   - Remove visible faces from the DCEL.
   - Construct new triangular faces connecting the horizon to the vertex.
   - Update the conflict graph for newly created faces.
5. The result is a convex hull mesh rendered in the viewer.

## Technologies

| Category | Technology |
|---|---|
| Language | C++11 |
| Build System | Qt `qmake` (`.pro` file) |
| GUI | Qt5 (widgets, OpenGL, XML) |
| 3D Rendering | libQGLViewer (OpenGL wrapper) |
| Linear Algebra | Eigen3 |
| Data Structures | Boost (`bimap`, `boost_system`) |
| Geometry (optional) | CGAL + GMP (via `CGAL_DEFINED` flag) |

## Project Structure

```
/
├── README.md                        # Build instructions and usage
├── models/                          # Sample .obj meshes (bunny, cow, homer, etc.)
└── ConvexHull/                      # Application source
    ├── 65034_medda_andrea.pro       # qmake project file
    ├── main.cpp                     # Entry point
    ├── convexhullbuilder.*          # Algorithm orchestrator
    ├── convexhullbuilderhelper.*    # Horizon computation & vertex utilities
    ├── tetrahedronbuilder.*         # Initial tetrahedron construction
    ├── conflictgraph.*              # Vertex–face conflict graph
    ├── facebuilderhelper.*          # New face creation from horizon edges
    ├── GUI/
    │   ├── mainwindow.*             # Main Qt window
    │   ├── glcanvas.*               # QGLViewer-based OpenGL canvas
    │   └── managers/
    │       ├── windowmanager.*      # View/scene controls
    │       ├── dcelmanager.*        # Mesh I/O and rendering options
    │       └── convexhullmanager.*  # Convex hull solver controls
    └── lib/
        ├── dcel/                    # DCEL mesh library (~15 files)
        └── common/                  # Point, BoundingBox, Timer, etc.
```

## Key Components

- **ConvexHullBuilder** — Orchestrates the full convex hull computation pipeline.
- **ConflictGraph** — Maintains bidirectional mappings between vertices and the faces visible from them, enabling efficient face deletion and creation.
- **TetrahedronBuilder** — Selects four non-coplanar seed points and constructs the initial tetrahedron in the DCEL.
- **DCEL Library** (`lib/dcel/`) — A full Doubly-Connected Edge List implementation supporting vertices, half-edges, and faces with topological navigation, plus OBJ/PLY/DCEL file I/O.
- **DrawableDcel** — Extends the DCEL with OpenGL rendering (wireframe, flat/smooth shading, per-vertex and per-face colors).

## How to Build

1. Install dependencies: Qt5, Eigen3, Boost, libQGLViewer (and optionally CGAL + GMP).
2. Open `ConvexHull/65034_medda_andrea.pro` in Qt Creator, or run:
   ```bash
   cd ConvexHull && qmake && make
   ```
3. Library paths in the `.pro` file may need adjustment for your system.

## Authors

- **Andrea Medda** (student ID 65034) — convex hull algorithm and helpers
- **Alessandro Muntoni, Stefano Nuvoli, Marco Livesu** — DCEL library and GUI scaffolding
