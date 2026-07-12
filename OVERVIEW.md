# Repository Overview

## Purpose

This repository implements an **interactive 3D Convex Hull Builder** — a Qt5 desktop application that loads a 3D mesh, computes the minimum convex polyhedron enclosing all its vertices, and visualizes the result in an OpenGL viewer. It was developed as a university project for an Algorithms and Data Structures 2 course.

## Tech Stack

| Layer | Technology |
|-------|------------|
| Language | C++11 |
| Build system | Qt qmake (`.pro` file) |
| GUI | Qt 5 (widgets, OpenGL) |
| 3D rendering | QGLViewer + OpenGL |
| Linear algebra | Eigen3 |
| Containers | Boost (`bimap`, `system`) |
| Computational geometry | CGAL (optional) |
| Arbitrary precision | GMP |

## Core Algorithm

The application uses an **incremental convex hull** algorithm with a **conflict graph**:

1. **Load mesh** — read vertices from an OBJ, PLY, or DCEL file.
2. **Build initial tetrahedron** — find 4 non-coplanar points and construct the seed polyhedron.
3. **Initialize conflict graph** — track which vertices are visible from which faces.
4. **Incremental insertion** — for each remaining vertex, compute the horizon of visible faces, remove them, build new triangular faces along the horizon, and update the conflict graph.
5. **Result** — the final convex hull mesh is rendered in cyan in the 3D viewer.

## Project Structure

```
ConvexHull/
├── main.cpp                      # Application entry point
├── 65034_medda_andrea.pro        # Qt qmake project file
├── convexhullbuilder.*           # Main algorithm orchestrator
├── tetrahedronbuilder.*          # Initial tetrahedron from 4 non-coplanar points
├── conflictgraph.*               # Vertex↔face visibility tracking
├── facebuilderhelper.*           # New triangular face construction
├── convexhullbuilderhelper.*     # Vertex extraction, horizon computation
├── GUI/
│   ├── mainwindow.*              # Main window with toolbox managers
│   ├── glcanvas.*                # QGLViewer-based OpenGL canvas
│   └── managers/
│       ├── dcelmanager.*         # Load/save/display meshes
│       ├── convexhullmanager.*   # Run convex hull + timing
│       └── windowmanager.*       # Viewport settings
└── lib/
    ├── dcel/                     # DCEL (Doubly-Connected Edge List) library
    │   ├── dcel_struct.*         # Core topology and I/O
    │   ├── drawable_dcel.*       # OpenGL rendering bridge
    │   └── ...                   # Vertices, half-edges, faces, iterators
    └── common/                   # Utilities (Point, BoundingBox, Timer)

models/                           # Sample OBJ meshes (bunny, cow, homer, etc.)
```

## Key Components

- **ConvexHullBuilder** — orchestrates the full incremental hull computation.
- **TetrahedronBuilder** — shuffles vertices, finds 4 non-coplanar points using Eigen determinants, and constructs the initial tetrahedron.
- **ConflictGraph** — maintains bidirectional maps between faces and their visible vertices; uses half-space (dot product) tests.
- **ConvexHullBuilderHelper** — extracts vertices from the DCEL and computes the horizon edge loop of visible faces.
- **FaceBuilderHelper** — creates new triangular faces along the horizon for each newly inserted vertex.
- **DCEL Library** (by Muntoni & Scalas) — a full Doubly-Connected Edge List implementation supporting general polygonal meshes, OBJ/PLY/DCEL I/O, adjacency iterators, and OpenGL rendering.

## How to Build and Run

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

> Library paths in the `.pro` file are hardcoded and may need editing for your system.

### Usage

1. Click **Load Dcel** and select a model from the `models/` directory.
2. Open the **Convex Hull Solver** tab.
3. Optionally enable **Show Phases** for step-by-step visualization.
4. Click **Convex Hull!** to compute and display the result.
