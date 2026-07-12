# Repository Overview

## Purpose

This is a **3D Convex Hull Builder** — a C++11/Qt5 desktop application developed as coursework for an Algorithms and Data Structures 2 university course. Given a 3D mesh as input, it computes the smallest convex polyhedron (convex hull) enclosing all input vertices and renders the result in an interactive OpenGL viewer.

## Core Algorithm

The application implements an **incremental 3D convex hull** algorithm with a **conflict graph**:

1. **Load mesh** — extract vertices from a DCEL (Doubly-Connected Edge List) structure.
2. **Build initial tetrahedron** — select 4 non-coplanar points to seed the hull.
3. **Initialize conflict graph** — track which unprocessed vertices are visible from which hull faces.
4. **Iteratively insert vertices** — for each remaining vertex:
   - Identify all hull faces visible from the vertex.
   - Compute the horizon (boundary between visible and non-visible faces).
   - Remove visible faces and create new triangular faces connecting the horizon to the vertex.
   - Update the conflict graph.
5. **Output** — the final convex hull mesh, rendered in cyan over the original model.

## Technology Stack

| Technology | Role |
|------------|------|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, forms, signals/slots) |
| Eigen3 | Linear algebra (determinants, coplanarity tests) |
| Boost | Bidirectional maps (`boost::bimap`) |
| CGAL + GMP | Optional mesh triangulation, arbitrary precision |
| libQGLViewer | 3D OpenGL camera and interaction |
| qmake | Build system |

## Project Structure

```
├── README.md                         # Usage instructions
├── models/                           # Sample 3D meshes (.obj)
└── ConvexHull/                       # Application source
    ├── main.cpp                      # Entry point
    ├── 65034_medda_andrea.pro        # qmake project file
    ├── convexhullbuilder.*           # Algorithm orchestrator
    ├── conflictgraph.*               # Face–vertex conflict tracking
    ├── tetrahedronbuilder.*          # Initial tetrahedron construction
    ├── facebuilderhelper.*           # New face creation along horizon
    ├── convexhullbuilderhelper.*     # Vertex extraction, horizon computation
    ├── GUI/
    │   ├── mainwindow.*              # Main application window
    │   ├── glcanvas.*                # OpenGL rendering canvas
    │   └── managers/
    │       ├── dcelmanager.*         # Mesh I/O and display controls
    │       ├── convexhullmanager.*   # Algorithm execution and timing
    │       └── windowmanager.*       # View/scene controls
    └── lib/
        ├── dcel/                     # DCEL mesh library (vertices, half-edges, faces)
        └── common/                   # Utilities (point, bounding box, timer)
```

## Key Data Structure: DCEL

The mesh is stored as a **Doubly-Connected Edge List** supporting:
- Efficient adjacency traversal via half-edge pointers (twin, next, prev).
- Loading/saving in OBJ, PLY, and a custom DCEL format.
- In-place mutation during convex hull construction.

## How to Build and Run

**Prerequisites:** GCC ~6.2.1, Qt ~5.7, libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev, Eigen3.

1. Open/build the project using qmake with `ConvexHull/65034_medda_andrea.pro`.
2. Load a model via the GUI (`Load Dcel` → select from `models/`).
3. Run `Convex Hull Solver` → `Convex Hull!` (or step through phases with `Show Phases`).

## Author

Andrea Medda (student project), with the DCEL library by Alessandro Muntoni and Andreas Scalas (2016).
