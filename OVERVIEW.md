# Repository Overview

## Purpose

**3D Convex Hull Builder** is a C++11 / Qt 5 desktop application that loads a 3D mesh and computes the minimum convex polyhedron (convex hull) enclosing all of its vertices. The result is rendered interactively in an OpenGL viewer. It was developed as a university project for an "Algorithms and Data Structures 2" course.

## Core Algorithm

The application implements an **incremental convex hull** construction with a conflict graph:

1. Load a 3D mesh (OBJ, PLY, or DCEL format) and extract its vertex set.
2. Select four non-coplanar points and build an initial tetrahedron.
3. Initialize a conflict graph linking remaining vertices to the tetrahedron faces they can "see."
4. For each unprocessed vertex visible to at least one face:
   - Compute the horizon (boundary edges between visible and non-visible faces).
   - Remove all visible faces from the hull.
   - Construct new triangular faces connecting the vertex to the horizon edges.
   - Update the conflict graph for newly created faces.
5. The final DCEL represents the convex hull and is rendered in the viewer.

## Technology Stack

| Component | Technology |
|-----------|------------|
| Language | C++11 |
| GUI / Windowing | Qt 5 |
| 3D Rendering | OpenGL via libQGLViewer |
| Linear Algebra | Eigen 3 |
| Geometry Utilities | CGAL, Boost |
| Mesh Data Structure | DCEL (Doubly-Connected Edge List) |

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Face–vertex conflict tracking
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New triangular face creation
├── convexhullbuilderhelper.*    # Vertex extraction, horizon computation
├── common.*                     # Shared constants and utilities
├── main.cpp                     # Application entry point
├── GUI/
│   ├── mainwindow.*             # Main application window
│   ├── glcanvas.*               # OpenGL rendering canvas
│   ├── managers/
│   │   ├── convexhullmanager.*  # Convex hull computation UI
│   │   ├── dcelmanager.*        # Mesh loading/display UI
│   │   └── windowmanager.*      # Camera and window controls
│   └── objects/                 # Geometric primitives (sphere, cylinder)
├── lib/
│   ├── dcel/                    # Full DCEL mesh library (Muntoni/Scalas)
│   └── common/                  # Utility types (Point, BoundingBox, timers)
└── 65034_medda_andrea.pro       # qmake project file

models/                          # Sample OBJ meshes (bunny, cow, homer, etc.)
```

## Building and Running

### Prerequisites

- GCC / G++ ≥ 6.2
- Qt ≥ 5.7
- `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`

### Build

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

> Library paths in the `.pro` file may need adjusting for your system.

### Usage

1. Launch the application.
2. Click **Load Dcel** and select a model from the `models/` directory.
3. Click **Convex Hull Solver**.
4. Choose **Show Phases** to step through the algorithm or **Convex Hull!** to run it in one shot.

## Credits

- **Author:** Andrea Medda (student ID 65034)
- **DCEL library:** Alessandro Muntoni & Stefano Scalas
- **GUI shell:** Marco Livesu
