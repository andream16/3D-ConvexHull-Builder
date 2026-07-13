# Repository Overview

## What This Project Does

**3D-ConvexHull-Builder** is a desktop application that computes the **3D convex hull** of a given mesh — the smallest convex polyhedron that encloses all of its vertices. It loads a 3D model, runs an incremental convex hull algorithm, and renders the result in an interactive OpenGL viewer.

This is a university project for an *Algorithms and Data Structures 2* course, authored by Andrea Medda.

## Core Algorithm

The project implements the **incremental convex hull** algorithm with a **conflict graph**, a well-known approach from computational geometry:

1. **Extract vertices** from the input mesh (only coordinates matter; topology is discarded).
2. **Build an initial tetrahedron** from four non-coplanar points, using a 4x4 determinant (Eigen) to verify non-coplanarity.
3. **Initialize a conflict graph** — a bidirectional mapping between faces and the vertices that can "see" them (lie on the positive side of the face plane).
4. **Incrementally insert** each remaining vertex:
   - Find all faces visible from the vertex via the conflict graph.
   - Compute the **horizon** — the boundary edges between visible and non-visible faces.
   - Delete visible faces and create new triangular faces connecting the horizon edges to the new vertex.
   - Update the conflict graph for the newly created faces.
5. The result is a convex hull mesh stored in a **DCEL (Doubly-Connected Edge List)** and rendered as a cyan overlay.

## Technology Stack

| Component | Technology |
|---|---|
| Language | C++11 |
| GUI Framework | Qt 5 (Widgets, OpenGL) |
| 3D Viewer | libQGLViewer / OpenGL / GLU |
| Linear Algebra | Eigen3 (determinant for coplanarity) |
| Parsing / Utilities | Boost (tokenizer, bimap) |
| Triangulation | CGAL (optional, for non-triangle mesh loading) |
| Build System | qmake |

## Project Structure

```
ConvexHull/
├── main.cpp                        # Application entry point
├── 65034_medda_andrea.pro          # qmake project file
├── convexhullbuilder.*             # Main hull algorithm orchestrator
├── conflictgraph.*                 # Face-vertex conflict graph
├── tetrahedronbuilder.*            # Initial tetrahedron construction
├── facebuilderhelper.*             # New face creation from horizon edges
├── convexhullbuilderhelper.*       # Vertex extraction, horizon computation
├── GUI/
│   ├── mainwindow.*                # Main window with toolbox panels
│   ├── glcanvas.*                  # QGLViewer-based 3D canvas
│   └── managers/
│       ├── dcelmanager.*           # Mesh load/save/display
│       ├── convexhullmanager.*     # Run algorithm, show timing
│       └── windowmanager.*         # Scene settings (background, axes)
└── lib/
    ├── dcel/                       # DCEL mesh library (by Muntoni & Scalas)
    └── common/                     # Shared utilities (Point, BoundingBox, timers)

models/                             # 11 sample OBJ meshes (bunny, cow, sphere, etc.)
```

## How to Build and Run

**Prerequisites:** GCC ~6.2.1, Qt ~5.7, libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev, Eigen3.

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

Library paths in the `.pro` file may need adjustment for your system.

**Usage:**
1. Launch the application.
2. In the **Dcel** panel, click **Load Dcel** and select a model from `models/`.
3. Switch to **Convex Hull Solver**.
4. Optionally enable **Show Phases** to watch the hull build incrementally.
5. Click **Convex Hull!** to compute and display the result.

## Credits

- **Convex hull algorithm:** Andrea Medda (student project)
- **DCEL library:** Alessandro Muntoni & Andreas Scalas (2016, GPL)
- **GUI framework:** Marco Livesu (2014)
