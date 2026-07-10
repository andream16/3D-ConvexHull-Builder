# 3D Convex Hull Builder — Repository Overview

This repository contains a **3D Convex Hull Builder**, a C++11/Qt5 desktop application developed as a course project for *Algorithms and Data Structures 2*. It loads 3D mesh models, computes the smallest convex polyhedron (convex hull) enclosing all input vertices, and visualizes the result in an interactive OpenGL viewer.

## Purpose

Given a 3D mesh (`.obj`, `.ply`, or `.dcel` format), the application:

1. Extracts the mesh vertices.
2. Computes the **3D convex hull** using an incremental algorithm with a conflict graph.
3. Renders both the original mesh and the resulting hull in an interactive 3D viewport.

Users can optionally enable **step-by-step visualization** to watch the hull grow as each vertex is processed.

## Core Algorithm

The convex hull is built incrementally:

1. **Initial tetrahedron** — four non-coplanar points form the starting hull (`TetrahedronBuilder`).
2. **Conflict graph initialization** — each remaining vertex is mapped to the hull faces it can "see" (`ConflictGraph`).
3. **Incremental insertion** — for each remaining vertex that lies outside the current hull:
   - Compute the **horizon** (boundary edges of visible faces).
   - Remove visible faces.
   - Create new triangular faces connecting the vertex to each horizon edge (`FaceBuilderHelper`).
   - Update the conflict graph.
4. The result is stored as a **DCEL** (Doubly-Connected Edge List) mesh.

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, forms, OpenGL integration) |
| QGLViewer | Interactive 3D camera and navigation |
| OpenGL / GLU | Mesh rendering |
| Eigen3 | Linear algebra (determinant computations) |
| Boost | `bimap` for object mapping, `tokenizer` for file parsing |
| CGAL + GMP | Optional advanced mesh operations |

**Build system:** Qt `qmake` (`.pro` file).

## Project Structure

```
ConvexHull/
├── main.cpp                        Entry point
├── 65034_medda_andrea.pro          Qt project file
├── convexhullbuilder.*             Main algorithm orchestrator
├── conflictgraph.*                 Vertex ↔ face visibility tracking
├── tetrahedronbuilder.*            Initial tetrahedron construction
├── facebuilderhelper.*             New face creation on horizon edges
├── convexhullbuilderhelper.*       Horizon computation, vertex extraction
├── GUI/
│   ├── mainwindow.*                Main window and toolbox
│   ├── glcanvas.*                  QGLViewer-based OpenGL canvas
│   └── managers/
│       ├── dcelmanager.*           Load/save/render meshes
│       ├── convexhullmanager.*     Run hull algorithm, display timing
│       └── windowmanager.*         Viewport settings
└── lib/
    ├── common/                     Shared utilities (point, bounding box, timer)
    └── dcel/                       DCEL mesh library (vertices, half-edges, faces, iterators)

models/                             Sample .obj meshes (bunny, cow, homer, sphere, etc.)
```

## How to Use

1. Build the project with `qmake` and `make` (requires Qt 5, Boost, Eigen3, QGLViewer, and optionally CGAL).
2. Launch the application.
3. Click **Load Dcel** and select a model from `models/`.
4. Open the **Convex Hull Solver** panel.
5. Optionally enable **Show Phases** for step-by-step visualization.
6. Click **Convex Hull!** to compute and display the result.

## Authorship

- **Convex hull algorithm:** Andrea Medda (student ID 65034)
- **GUI framework:** Marco Livesu
- **DCEL library:** Alessandro Muntoni and Andreas Scalas
