# Repository Purpose & Architecture

## What This Repo Does

**3D-ConvexHull-Builder** is a C++11 / Qt 5 desktop application that loads a 3D mesh and computes its **convex hull** — the smallest convex polyhedron enclosing all input vertices. The result is visualized interactively in an OpenGL viewer.

Built as a project for the "Algorithms and Data Structures 2" university course (student ID 65034, Andrea Medda).

## Core Algorithm

The implementation uses the **incremental convex hull** construction with a **conflict graph**:

1. Load a mesh (OBJ/PLY/DCEL) and extract its vertices.
2. Build an initial tetrahedron from 4 non-coplanar points.
3. Initialize a conflict graph mapping each remaining vertex to the faces it can "see."
4. For each unprocessed vertex (picked from the conflict graph):
   - Compute the horizon edges of the visible region.
   - Delete all visible faces.
   - Create new triangular faces connecting the horizon to the vertex.
   - Update the conflict graph for the new faces.
5. The final DCEL represents the convex hull, rendered in cyan over the original mesh.

## Tech Stack

| Component | Technology |
|-----------|-----------|
| Language | C++11 |
| GUI framework | Qt 5 |
| Linear algebra | Eigen 3 |
| Geometry/utilities | Boost, CGAL |
| 3D viewer | libQGLViewer (OpenGL) |
| Mesh data structure | DCEL (Doubly-Connected Edge List) |

## Directory Layout

```
ConvexHull/
├── convexhullbuilder.*        # Main algorithm orchestrator
├── conflictgraph.*            # Face-vertex conflict tracking
├── tetrahedronbuilder.*       # Initial tetrahedron construction
├── facebuilderhelper.*        # New triangular face creation
├── convexhullbuilderhelper.*  # Vertex extraction, horizon computation
├── common.*                   # Shared constants and helpers
├── main.cpp                   # Application entry point
├── GUI/
│   ├── mainwindow.*           # Top-level window
│   ├── glcanvas.*             # OpenGL rendering canvas
│   └── managers/              # UI panels (DCEL, ConvexHull, Window)
└── lib/
    ├── dcel/                  # Full DCEL mesh library (Muntoni/Scalas)
    └── common/                # Utilities (Point, BoundingBox, timers)

models/                        # Sample OBJ meshes (bunny, cow, homer, etc.)
65034_medda_andrea.pro         # qmake project file
```

## Building & Running

**Prerequisites:** gcc/g++ ≥ 6.2, Qt ≥ 5.7, libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev.

```bash
qmake 65034_medda_andrea.pro
make
./ConvexHull
```

**Usage:**
1. Click *Load Dcel* and select a model from the `models/` directory.
2. Click *Convex Hull Solver*.
3. Choose *Show Phases* for a step-by-step view, or *Convex Hull!* to compute in one shot.
