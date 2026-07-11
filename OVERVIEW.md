# Repository Overview

## Purpose

This repository is a **3D Convex Hull Builder** — a C++11 / Qt5 desktop application created for an *Algorithms and Data Structures 2* university course. It loads a 3D mesh, computes the smallest convex polyhedron (convex hull) enclosing all its vertices, and renders the result in an interactive OpenGL viewer.

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 (core, gui, widgets, opengl, xml) | GUI framework and file I/O |
| libQGLViewer | Interactive 3D OpenGL canvas |
| Eigen3 | Matrix determinant for coplanarity checks |
| Boost (bimap, tokenizer) | Conflict graph mapping, file parsing |
| CGAL (optional) | Mesh triangulation for non-triangle polygons |
| GMP | Arbitrary-precision arithmetic (CGAL dependency) |

## Project Structure

```
├── README.md
├── OVERVIEW.md
├── models/                          # Sample OBJ meshes (bunny, cow, homer, etc.)
└── ConvexHull/
    ├── main.cpp                     # Application entry point
    ├── 65034_medda_andrea.pro       # Qt qmake build configuration
    ├── convexhullbuilder.*          # Main hull algorithm orchestrator
    ├── conflictgraph.*              # Face–vertex conflict tracking
    ├── tetrahedronbuilder.*         # Initial tetrahedron construction
    ├── facebuilderhelper.*          # New triangular face creation
    ├── convexhullbuilderhelper.*    # Vertex extraction and horizon computation
    ├── common.*                     # Global manager IDs
    ├── GUI/
    │   ├── mainwindow.*             # Main window with manager toolbox
    │   ├── glcanvas.*               # QGLViewer-based 3D canvas
    │   └── managers/
    │       ├── dcelmanager.*        # Load/save/visualize meshes
    │       ├── convexhullmanager.*  # Run hull algorithm and display results
    │       └── windowmanager.*      # Window and display settings
    └── lib/
        ├── dcel/                    # Doubly-Connected Edge List mesh library
        └── common/                  # Utilities (Point, BoundingBox, timers)
```

## Algorithm

The core algorithm is an **incremental 3D convex hull** construction using a **conflict graph**, a standard technique from computational geometry:

1. **Load mesh** — read vertices from an OBJ, PLY, or DCEL file into a Doubly-Connected Edge List.
2. **Build initial tetrahedron** — select 4 non-coplanar points (verified via a 4×4 determinant) and create the first 4 triangular faces.
3. **Initialize conflict graph** — for every hull face and every remaining vertex, record which vertices are visible from which faces (half-space dot-product test).
4. **Incremental insertion** — for each remaining vertex:
   - Find all hull faces visible from the vertex.
   - Compute the **horizon** — the ring of boundary edges between visible and non-visible faces.
   - Delete visible faces and create new triangular faces connecting the horizon edges to the new vertex.
   - Update the conflict graph with the new faces.
5. **Render** — display the resulting convex hull (colored cyan) alongside the original mesh.

The conflict graph avoids redundant visibility recomputation on each insertion. The algorithm runs in O(n²) worst-case time for n input vertices.

## Key Components

- **`ConvexHullBuilder`** — orchestrates the full pipeline: vertex extraction, tetrahedron seeding, conflict-graph-driven incremental insertion, and optional step-by-step animation.
- **`ConflictGraph`** — maintains bidirectional maps (face → visible vertices, vertex → visible faces) using `boost::bimap`.
- **`TetrahedronBuilder`** — shuffles input vertices, checks coplanarity via Eigen determinant, and builds the correctly oriented initial tetrahedron.
- **`ConvexHullBuilderHelper`** — extracts vertices from the DCEL and computes/orders the horizon edge ring from visible faces.
- **`FaceBuilderHelper`** — creates new triangular faces from horizon half-edges and the inserted vertex, wiring all DCEL topology (twin, next, prev).
- **DCEL library** (`lib/dcel/`) — a full Doubly-Connected Edge List implementation with vertices, half-edges, faces, iterators, OBJ/PLY I/O, and OpenGL rendering (wireframe, flat/smooth shading).

## Building and Running

**Prerequisites:** gcc/g++ ~6.2, Qt ~5.7, libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev.

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
./65034_medda_andrea
```

**Usage:**
1. Click **Load Dcel** and select a model from the `models/` directory.
2. Open the **Convex Hull Solver** tab.
3. Click **Convex Hull!** to compute the hull, or enable **Show Phases** to watch the algorithm step by step.
