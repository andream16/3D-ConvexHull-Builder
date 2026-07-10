# Repository Overview: 3D-ConvexHull-Builder

## Purpose

This repository contains a C++/Qt5 desktop application that loads 3D triangle meshes, computes their **3D convex hull** (the smallest convex polyhedron enclosing all vertices), and renders both the original mesh and the result in an interactive OpenGL viewer. It was developed as a university project for an Algorithms and Data Structures 2 course.

## Tech Stack

- **C++11** with **Qt 5** for the GUI and OpenGL integration
- **QGLViewer** for interactive 3D camera controls
- **Eigen3** for linear algebra (determinants, coplanarity checks)
- **Boost** for data structures (`boost::bimap`)
- **CGAL** for optional mesh utilities
- **qmake** build system

## Architecture

### Core Algorithm (`ConvexHull/`)

The convex hull is computed using an **incremental algorithm with a conflict graph**, a well-known approach with expected O(n log n) complexity:

1. **TetrahedronBuilder** constructs an initial tetrahedron from four non-coplanar input vertices.
2. **ConflictGraph** maintains bidirectional mappings between faces and vertices visible from those faces, using half-space dot-product tests.
3. For each remaining vertex, the algorithm finds visible faces, computes the **horizon** (boundary edges separating visible from non-visible regions), removes visible faces, builds new triangular faces connecting the horizon to the vertex, and updates the conflict graph.
4. **ConvexHullBuilderHelper** handles vertex extraction and horizon computation.
5. **FaceBuilderHelper** creates the new triangular faces from horizon edges.

### Mesh Library (`ConvexHull/lib/dcel/`)

A full **Doubly-Connected Edge List (DCEL)** implementation supporting vertices, half-edges, faces, iterators, and file I/O for OBJ, PLY, and DCEL formats. The convex hull is built and stored directly in this data structure.

### GUI (`ConvexHull/GUI/`)

A tabbed interface with three manager panels:

| Manager | Functionality |
|---------|---------------|
| **WindowManager** | Background color, snapshots, fullscreen, axis display |
| **DcelManager** | Load/save meshes, shading modes, wireframe toggle |
| **ConvexHullManager** | Run the algorithm, optional step-by-step visualization, timing |

The 3D viewer (`GLCanvas`) uses QGLViewer for interactive rotation, panning, and zooming. The computed hull is rendered in cyan with flat shading.

## Directory Structure

```
├── README.md                         # Original project readme
├── OVERVIEW.md                       # This file
├── models/                           # Sample OBJ meshes (bunny, cow, homer, sphere, ...)
└── ConvexHull/
    ├── main.cpp                      # Application entry point
    ├── 65034_medda_andrea.pro        # qmake project file
    ├── convexhullbuilder.*           # Algorithm orchestrator
    ├── conflictgraph.*               # Conflict graph (face-vertex visibility)
    ├── tetrahedronbuilder.*          # Initial tetrahedron construction
    ├── facebuilderhelper.*           # New face creation from horizon edges
    ├── convexhullbuilderhelper.*     # Vertex extraction, horizon computation
    ├── GUI/
    │   ├── mainwindow.*              # Main window with QGLViewer
    │   ├── glcanvas.*                # OpenGL rendering
    │   └── managers/                 # UI manager panels
    └── lib/
        ├── dcel/                     # DCEL mesh data structure library
        └── common/                   # Points, bounding boxes, timers, serialization
```

## Building and Running

**Dependencies:** `gcc`/`g++`, Qt 5.7+, `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`

Build with qmake from the `ConvexHull/` directory. Library paths in the `.pro` file may need adjustment for your system.

**Usage:**
1. Launch the application and click **Load Dcel** to load a mesh from `models/`
2. Switch to the **Convex Hull Solver** tab
3. Click **Convex Hull!** to compute, or enable **Show Phases** for step-by-step visualization

## Authorship

- **Convex hull algorithm:** Andrea Medda (student ID 65034)
- **DCEL library:** Alessandro Muntoni and Andreas Scalas
- **GUI framework:** Marco Livesu
