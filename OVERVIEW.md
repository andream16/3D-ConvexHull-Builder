# Repository Overview

## Purpose

This repository implements a **3D Convex Hull Builder** — a desktop application that loads a 3D mesh and computes the minimum convex polyhedron (convex hull) enclosing all of its vertices. The result is rendered interactively in an OpenGL viewer. It was developed as a project for the "Algorithms and Data Structures 2" university course.

## Technology Stack

- **Language:** C++11
- **GUI Framework:** Qt 5
- **3D Rendering:** libQGLViewer / OpenGL
- **Math:** Eigen3
- **Geometry:** CGAL
- **Utilities:** Boost

## Algorithm

The convex hull is computed using an **incremental construction algorithm with a conflict graph**:

1. Load a 3D mesh and extract its vertex set.
2. Find four non-coplanar points and build an initial tetrahedron.
3. Initialize a conflict graph linking each remaining vertex to the tetrahedron faces it can "see."
4. For each unprocessed vertex (that still has visible faces):
   - Determine the horizon edges of the visible region.
   - Remove all visible faces from the hull.
   - Create new triangular faces connecting the vertex to each horizon edge.
   - Update the conflict graph for the newly created faces.
5. The final DCEL represents the convex hull, rendered in cyan over the original mesh.

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*        # Main algorithm orchestrator
├── conflictgraph.*            # Conflict graph (face ↔ vertex visibility)
├── tetrahedronbuilder.*       # Initial tetrahedron construction
├── facebuilderhelper.*        # New triangular face creation
├── convexhullbuilderhelper.*  # Horizon computation, vertex extraction
├── common.*                   # Shared utilities
├── main.cpp                   # Application entry point
├── GUI/
│   ├── mainwindow.*           # Main application window
│   ├── glcanvas.*             # OpenGL rendering canvas
│   ├── managers/              # UI panel managers (DCEL, convex hull, window)
│   └── objects/               # Geometric primitives (sphere, cylinder)
├── lib/
│   ├── dcel/                  # Doubly-Connected Edge List mesh library
│   └── common/                # Shared types (point, bounding box, timers)
└── 65034_medda_andrea.pro     # Qt project file

models/                        # Sample OBJ meshes (bunny, cow, homer, etc.)
```

## Building and Running

### Prerequisites

- GCC/G++ ~6.2.1
- Qt ~5.7
- libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev

### Build

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

### Usage

1. Launch the application.
2. Click **Load Dcel** and select a model from the `models/` directory.
3. Click **Convex Hull Solver**.
4. Choose **Show Phases** (step-by-step) or **Convex Hull!** (run to completion).

## Author

Andrea Medda (student ID 65034). DCEL library by Muntoni/Scalas; GUI shell by Marco Livesu.
