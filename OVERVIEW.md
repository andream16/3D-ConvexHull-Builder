# Repository Overview

**3D-ConvexHull-Builder** is a C++11 / Qt5 desktop application built for an *Algorithms and Data Structures 2* university course. It loads a 3D mesh, extracts its vertices, computes the **3D convex hull** (the smallest convex polyhedron enclosing all points), and visualizes the result interactively using OpenGL.

## Technologies

| Category | Details |
|----------|---------|
| Language | C++11 |
| Build | qmake (`65034_medda_andrea.pro`) |
| GUI | Qt 5 (widgets, OpenGL, XML) |
| 3D Viewer | libQGLViewer |
| Linear Algebra | Eigen3 |
| Utilities | Boost (bimap, tokenizer) |
| Geometry (optional) | CGAL, GMP |
| Rendering | OpenGL / GLU |

## How It Works

1. **Load a mesh** — supports OBJ, PLY, and a custom DCEL serialization format.
2. **Extract vertices** — the input face topology is discarded; only vertex positions are used.
3. **Build an initial tetrahedron** — four non-coplanar points are selected and used to construct the starting convex hull.
4. **Initialize a conflict graph** — a bidirectional mapping tracks which vertices are visible from which faces.
5. **Incrementally insert remaining vertices** — for each vertex outside the current hull, visible faces are removed, the horizon (boundary between visible and non-visible faces) is computed, and new triangular faces are created connecting the horizon edges to the new vertex. The conflict graph is updated after each insertion.
6. **Display the result** — the convex hull mesh is rendered in cyan alongside the original input model.

## Project Structure

```
ConvexHull/
├── main.cpp                     # Application entry point
├── convexhullbuilder.*          # Algorithm orchestrator
├── conflictgraph.*              # Conflict graph (face ↔ vertex visibility)
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New face creation during hull growth
├── convexhullbuilderhelper.*    # Horizon computation, vertex extraction
├── GUI/
│   ├── mainwindow.*             # Main window with manager toolbox
│   ├── glcanvas.*               # QGLViewer-based OpenGL canvas
│   ├── managers/                # UI panels (Dcel, ConvexHull, Window)
│   └── objects/                 # Debug rendering helpers
└── lib/
    ├── dcel/                    # Doubly-Connected Edge List mesh library
    └── common/                  # Point, bounding box, timer utilities

models/                          # Sample 3D meshes (bunny, cow, homer, etc.)
```

## Key Components

- **ConvexHullBuilder** — orchestrates the full convex hull pipeline from vertex extraction through final mesh construction.
- **TetrahedronBuilder** — finds four non-coplanar points and constructs the initial tetrahedron, using Eigen 4x4 determinants for coplanarity checks.
- **ConflictGraph** — maintains bidirectional maps between faces and their visible vertices; supports incremental updates as faces are added and removed.
- **ConvexHullBuilderHelper** — extracts vertices from the input DCEL and computes the ordered horizon half-edges separating visible from non-visible faces.
- **FaceBuilderHelper** — creates new triangular faces with correct twin/next/prev half-edge links.
- **DCEL Library** — a full Doubly-Connected Edge List implementation (vertices, half-edges, faces) with OBJ/PLY I/O and OpenGL rendering support.

## Features

- **Interactive 3D visualization** with rotate, zoom, and pan controls via QGLViewer.
- **Step-by-step mode** — enabling "Show Phases" refreshes the canvas after each vertex insertion, making the incremental hull growth visible.
- **Performance timing** — wall-clock computation time is measured and displayed in the UI.
- **Multiple file formats** — OBJ (primary), PLY, and native DCEL serialization.
- **Sample models** — includes Stanford bunny (~28K vertices), cow, homer, sphere, cylinder, and others.
