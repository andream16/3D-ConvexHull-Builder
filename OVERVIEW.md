# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++ desktop application that loads a 3D mesh and computes its **convex hull** — the smallest convex polyhedron enclosing all input vertices. The result is rendered interactively in an OpenGL viewer alongside the original model.

This project was developed for an **Algorithms and Data Structures 2** university course (student: Andrea Medda, ID 65034).

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework |
| QGLViewer | Interactive 3D OpenGL viewer |
| Eigen3 | Linear algebra (determinants, coplanarity) |
| Boost | `bimap` (object management), `tokenizer` (file parsing) |
| CGAL | Extended DCEL mesh operations |
| libgmp | Required by CGAL |

**Build system:** Qt `qmake` via `ConvexHull/65034_medda_andrea.pro`.

## Algorithm

The application implements an **incremental 3D convex hull** algorithm:

1. Extract all vertices from the input mesh (stored as a DCEL — Doubly-Connected Edge List).
2. Shuffle vertices and select 4 non-coplanar points to build an initial tetrahedron.
3. Construct a conflict graph mapping remaining vertices to faces they can "see."
4. For each remaining vertex: identify visible faces, compute the horizon boundary, remove visible faces, create new triangular faces from horizon edges to the new vertex, and update the conflict graph.
5. Optionally display each insertion step in real time when "Show Phases" is enabled.

## Directory Structure

```
ConvexHull/
├── main.cpp                        # Application entry point
├── 65034_medda_andrea.pro          # Qt qmake project file
├── convexhullbuilder.{h,cpp}       # Main algorithm orchestrator
├── convexhullbuilderhelper.{h,cpp}  # Vertex extraction, horizon computation
├── tetrahedronbuilder.{h,cpp}      # Initial tetrahedron seeding
├── conflictgraph.{h,cpp}           # Vertex–face conflict tracking
├── facebuilderhelper.{h,cpp}       # New face construction
├── GUI/
│   ├── mainwindow.{h,cpp,ui}      # Main window
│   ├── glcanvas.{h,cpp}           # OpenGL canvas (QGLViewer)
│   └── managers/
│       ├── dcelmanager.*           # Mesh load/visualization controls
│       ├── convexhullmanager.*     # Convex hull execution UI
│       └── windowmanager.*         # Window/display settings
└── lib/
    ├── dcel/                       # DCEL data structure library
    │   ├── dcel_struct.{h,cpp}     # Core DCEL implementation
    │   ├── dcel_vertex.* / dcel_half_edge.* / dcel_face.*
    │   └── drawable_dcel.*         # Renderable DCEL
    └── common/                     # Utilities (point, bounding box, timers)
models/                             # Sample .obj test meshes (bunny, cow, homer, etc.)
```

## Usage

1. Click **Load Dcel** and select a model from the `models/` directory.
2. Open the **Convex Hull Solver** tab.
3. Optionally enable **Show Phases** for step-by-step visualization.
4. Click **Convex Hull!** to compute and display the result.

## Build Requirements

- `gcc` / `g++` (~6.2.1)
- Qt (~5.7)
- `libboost-all-dev`
- `libcgal-dev`
- `libgmp-dev`
- `libqglviewer-dev`
- Eigen3

Optional: `doxygen` for generating DCEL library documentation.
