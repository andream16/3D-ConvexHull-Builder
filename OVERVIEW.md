# Repository Purpose & Architecture Overview

## What This Repository Does

**3D-ConvexHull-Builder** is a desktop application that loads a 3D mesh model, computes its [convex hull](https://en.wikipedia.org/wiki/Convex_hull) (the smallest convex polyhedron enclosing all vertices), and visualizes both the original model and the resulting hull in an interactive 3D viewer.

It was developed as a university project for an **Algorithms and Data Structures 2** course (author: Andrea Medda, student ID 65034).

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, OpenGL integration) |
| Eigen3 | Linear algebra (determinants, coplanarity checks) |
| Boost | Bidirectional maps (`boost::bimap`) |
| CGAL / GMP | Optional computational geometry and arbitrary-precision math |
| libQGLViewer | Interactive 3D OpenGL viewer widget |
| qmake | Build system (`.pro` project file) |

## Algorithm

The project implements an **incremental 3D convex hull** algorithm backed by a **conflict graph**:

1. **Load mesh** — Parse vertices from an OBJ, PLY, or DCEL file.
2. **Build initial tetrahedron** — Select 4 non-coplanar points and construct a seed polyhedron (`TetrahedronBuilder`).
3. **Initialize conflict graph** — For every remaining vertex, record which tetrahedron faces it can "see" (half-space test via dot product with face normal) (`ConflictGraph`).
4. **Incremental insertion** — For each remaining vertex that sees at least one face:
   - Compute the **horizon** (boundary between visible and non-visible faces).
   - Delete all visible faces from the hull.
   - Stitch new triangular faces along the horizon to the new vertex.
   - Update the conflict graph for the newly created faces.
5. **Output** — The final convex hull is stored as a DCEL mesh and rendered in the viewer.

## Project Structure

```
/
├── README.md                        # Original project readme
├── OVERVIEW.md                      # This file
├── models/                          # Sample 3D meshes (bunny, cow, homer, sphere, …)
└── ConvexHull/                      # Application source
    ├── 65034_medda_andrea.pro       # qmake project file
    ├── main.cpp                     # Entry point — sets up Qt app and managers
    ├── convexhullbuilder.*          # Main algorithm loop
    ├── tetrahedronbuilder.*         # Initial tetrahedron construction
    ├── conflictgraph.*              # Conflict graph (vertex ↔ face visibility)
    ├── convexhullbuilderhelper.*    # Horizon computation, vertex extraction
    ├── facebuilderhelper.*          # New face creation along the horizon
    ├── common.*                     # Shared manager IDs
    ├── GUI/
    │   ├── mainwindow.*             # Main window with manager toolbox
    │   ├── glcanvas.*               # QGLViewer-based OpenGL canvas
    │   ├── managers/
    │   │   ├── windowmanager.*      # Background color, fullscreen, screenshots
    │   │   ├── dcelmanager.*        # Load/save/display mesh controls
    │   │   └── convexhullmanager.*  # Run algorithm, step-by-step mode, timing
    │   └── objects/                 # Debug primitives (sphere, cylinder)
    └── lib/
        ├── dcel/                    # Doubly-Connected Edge List mesh library
        └── common/                  # Point, bounding box, serialization utilities
```

## Key Components

- **`ConvexHullBuilder`** — Orchestrates the full algorithm: extracts vertices, delegates to `TetrahedronBuilder`, then iterates over remaining vertices using the conflict graph.
- **`ConflictGraph`** — Maintains bidirectional maps between vertices and faces to efficiently determine visibility during each insertion step.
- **`TetrahedronBuilder`** — Shuffles the vertex list and finds 4 non-coplanar points (using an Eigen determinant check) to seed the hull.
- **`ConvexHullBuilderHelper`** — Computes and orders the horizon half-edges that separate visible from non-visible faces.
- **`FaceBuilderHelper`** — Creates new DCEL triangular faces from a vertex and the ordered horizon edges.
- **DCEL library (`lib/dcel/`)** — A full mesh data structure (vertices, half-edges, faces) with OBJ/PLY I/O and OpenGL rendering support. Authors: Alessandro Muntoni & Andreas Scalas (2016).

## How to Use

1. Build with qmake and make (requires Qt 5, Eigen3, Boost, CGAL, GMP, libQGLViewer).
2. Launch the application.
3. Click **Load Dcel** and select a model from the `models/` directory.
4. Open the **Convex Hull Solver** tab.
5. Optionally enable **Show Phases** to watch the algorithm step by step.
6. Click **Convex Hull!** to compute and display the result.
