# Repository Overview

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application that computes the **3D convex hull** of a given mesh — the smallest convex polyhedron enclosing all of the mesh's vertices. It was developed as a university project for an Algorithms and Data Structures 2 course.

## What It Does

1. **Loads a 3D model** in OBJ, PLY, or DCEL format (sample models such as the Stanford bunny, cow, and homer are included in `models/`).
2. **Computes the convex hull** using an incremental algorithm backed by a conflict graph for efficient visibility tracking.
3. **Visualizes the result** in an interactive OpenGL viewer, displaying both the original mesh and the computed hull side by side.

## Core Algorithm

The convex hull is built incrementally:

- **Seed tetrahedron** — Four non-coplanar vertices are selected (with coplanarity checked via a 4×4 determinant using Eigen) and used to construct an initial tetrahedron.
- **Conflict graph** — A bidirectional mapping tracks which remaining vertices can "see" which faces, enabling efficient updates without full recomputation.
- **Incremental insertion** — Each remaining vertex is processed in turn: visible faces are identified, the horizon boundary is computed, visible faces are removed, new triangular faces are built from the horizon edges to the new vertex, and the conflict graph is updated.

The mesh is represented internally with a **Doubly-Connected Edge List (DCEL)**, a half-edge data structure supporting vertices, edges, and faces with full adjacency traversal.

## Technology Stack

| Component | Purpose |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, OpenGL integration) |
| QGLViewer | Interactive 3D camera and rendering |
| Eigen3 | Matrix determinant for coplanarity tests |
| Boost | Bimap, tokenizer utilities |
| CGAL (optional) | Mesh triangulation |

## Project Structure

```
ConvexHull/
├── main.cpp                    # Application entry point
├── convexhullbuilder.*         # Main hull algorithm orchestrator
├── tetrahedronbuilder.*        # Initial tetrahedron construction
├── conflictgraph.*             # Conflict graph for visibility tracking
├── facebuilderhelper.*         # New face construction from horizon edges
├── convexhullbuilderhelper.*   # Horizon computation and utilities
├── GUI/
│   ├── mainwindow.*            # Main window with manager tabs
│   ├── glcanvas.*              # QGLViewer OpenGL canvas
│   └── managers/               # UI panels for mesh I/O and hull solving
└── lib/
    ├── dcel/                   # DCEL mesh library (Muntoni & Scalas)
    └── common/                 # Shared utilities (Point, timers, etc.)
models/                         # Sample OBJ meshes (bunny, cow, sphere, …)
```

## Building and Running

The project uses **qmake** via the `65034_medda_andrea.pro` project file. Required system packages: `gcc`/`g++` (~6.2), Qt 5, `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`. Library paths in the `.pro` file may need adjustment for your system.

To run: load a mesh via the **Dcel** manager tab, switch to the **Convex Hull Solver** tab, and click **Convex Hull!**. Enable **Show Phases** to watch the algorithm step by step.
