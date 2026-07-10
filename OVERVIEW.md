# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++11 / Qt5 desktop application that loads a 3D mesh, computes its **3D convex hull** (the smallest convex polyhedron enclosing all input vertices), and renders the result in an interactive OpenGL viewer. It was developed as a project for the *Algorithms and Data Structures 2* university course.

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, dialogs, OpenGL integration) |
| libQGLViewer | Interactive 3D canvas (extends `QGLViewer`) |
| Eigen3 | Linear algebra (determinants for orientation tests) |
| Boost | `bimap` in the GUI; `tokenizer` in mesh I/O |
| CGAL (optional) | Constrained Delaunay triangulation for non-triangle meshes |
| OpenGL / GLU | Rendering |

**Build system:** qmake via `ConvexHull/65034_medda_andrea.pro`.

## Algorithm

The convex hull is built using an **incremental algorithm with a conflict graph**:

1. Load a mesh and extract its vertices.
2. Find 4 non-coplanar points and build an initial **tetrahedron**.
3. Initialize a **conflict graph** that maps each remaining vertex to the tetrahedron faces visible from it.
4. For each remaining vertex (chosen from a face's conflict list):
   - Identify all faces of the current hull visible from that vertex.
   - Compute the **horizon** — the boundary edges between visible and non-visible faces.
   - Delete the visible faces.
   - Create new triangular faces connecting each horizon edge to the inserted vertex.
   - Update the conflict graph for the new faces.
5. The result is a convex hull stored as a DCEL mesh and rendered in cyan.

## Directory Structure

```
/
├── README.md                        # Build instructions, usage, screenshots
├── OVERVIEW.md                      # This file
├── models/                          # Sample .obj meshes (bunny, cow, sphere, etc.)
└── ConvexHull/                      # Qt/C++ application
    ├── main.cpp                     # Entry point
    ├── 65034_medda_andrea.pro       # qmake project file
    ├── convexhullbuilder.*          # Algorithm orchestrator
    ├── tetrahedronbuilder.*         # Initial tetrahedron construction
    ├── conflictgraph.*              # Vertex–face conflict graph
    ├── convexhullbuilderhelper.*    # Horizon computation, vertex extraction
    ├── facebuilderhelper.*          # New face construction
    ├── GUI/
    │   ├── mainwindow.*             # Main window, manager toolbox
    │   ├── glcanvas.*               # OpenGL rendering canvas
    │   └── managers/
    │       ├── dcelmanager.*        # Load/save/display mesh
    │       ├── convexhullmanager.*  # Run convex hull solver
    │       └── windowmanager.*      # Axes, background, fullscreen
    └── lib/
        ├── common/                  # Point, bounding box, comparators
        └── dcel/                    # DCEL mesh library + rendering
```

## Architecture

The codebase has three layers:

- **GUI layer** (`GUI/`) — Qt panels and an OpenGL viewer. Managers plug into a toolbox in `MainWindow`. `DcelManager` handles mesh I/O; `ConvexHullManager` triggers the algorithm.
- **Algorithm layer** (root `ConvexHull/` files) — Implements the incremental convex hull with a conflict graph. `ConvexHullBuilder` orchestrates the algorithm; helper classes handle the tetrahedron, conflict graph, horizon computation, and face construction.
- **Data layer** (`lib/`) — A reusable DCEL (Doubly-Connected Edge List) mesh library supporting OBJ/PLY/DCEL formats, iterators, and optional CGAL triangulation. `DrawableDcel` extends the core `Dcel` with OpenGL rendering.

## Usage

1. **Load Dcel** — select a model from the `models/` directory.
2. **Convex Hull Solver** — open the solver panel.
3. **Convex Hull!** — run the algorithm. Optionally enable **Show Phases** for step-by-step visualization.

## Supported Mesh Formats

- Wavefront OBJ (`.obj`)
- Stanford PLY (`.ply`)
- Custom DCEL (`.dcel`)
