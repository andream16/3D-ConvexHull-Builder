# 3D-ConvexHull-Builder — Repository Overview

A **C++11 / Qt 5** desktop application that loads a 3D mesh, computes its
**minimum convex hull**, and renders the result in an interactive OpenGL viewer.
Built as a course project for *Algorithms and Data Structures 2*.

## What It Does

Given a 3D point cloud or polygonal mesh the program finds the smallest convex
polyhedron that contains every vertex and displays it in cyan over the original
model.

**Workflow:**

1. Load a 3D model (`.obj`, `.ply`, or `.dcel` format).
2. Open the **Convex Hull Solver** panel.
3. Run the algorithm (optionally stepping through phases).
4. Inspect the resulting convex hull in the 3D viewer.

Sample models (bunny, cow, homer, sphere, cylinder, etc.) are included in
`models/`.

## Algorithm

The core implements an **incremental convex hull** construction accelerated by a
conflict graph (similar in spirit to the randomized incremental / QuickHull
family):

1. **Extract vertices** from the loaded mesh and reset the DCEL.
2. **Build an initial tetrahedron** from four non-coplanar points
   (`TetrahedronBuilder`).
3. **Initialize a conflict graph** mapping each remaining vertex to the hull
   faces visible from it, and vice-versa (`ConflictGraph`).
4. **Incrementally insert** each remaining vertex:
   - Compute the *horizon* (boundary of the visible region) via
     `ConvexHullBuilderHelper`.
   - Remove visible faces.
   - Create new triangular faces connecting the horizon to the new vertex
     (`FaceBuilderHelper`).
   - Update the conflict graph.
5. **Finalize** — recompute normals, update the bounding box, colour faces cyan.

A *Show Phases* option refreshes the 3D view after every insertion so the hull
construction can be observed step by step.

## Project Structure

```
ConvexHull/
├── main.cpp                     # Application entry point
├── convexhullbuilder.*          # Algorithm orchestrator
├── conflictgraph.*              # Face ↔ vertex conflict maps
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New face creation during expansion
├── convexhullbuilderhelper.*    # Horizon computation & utilities
├── common.*                     # Global manager IDs
├── GUI/
│   ├── mainwindow.*             # Main window & object stack
│   ├── glcanvas.*               # QGLViewer-based 3D canvas
│   ├── managers/                # Toolbox panels (Dcel, Convex Hull, Window)
│   └── objects/                 # Primitive drawable shapes
└── lib/
    ├── dcel/                    # Doubly-Connected Edge List mesh library
    └── common/                  # Point, bounding box, serialization, timers
models/                          # Sample OBJ meshes
```

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, OpenGL, XML) |
| Eigen 3 | Linear algebra (determinants, coplanarity) |
| CGAL | Optional mesh operations (triangulation) |
| Boost | `bimap` for UI object tracking |
| libQGLViewer | 3D navigation and rendering |
| GMP | Required by CGAL |

## Building

```bash
# Install dependencies (Debian/Ubuntu)
sudo apt install qt5-default libboost-all-dev libcgal-dev \
                 libgmp-dev libqglviewer-dev-qt5 libeigen3-dev

cd ConvexHull
qmake 65034_medda_andrea.pro
make
./ConvexHull
```

Library paths in the `.pro` file may need adjusting for your system's package
versions.
