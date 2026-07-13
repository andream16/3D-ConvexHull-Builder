# Repository Overview

## Purpose

**3D Convex Hull Builder** is a C++/Qt5 desktop application that computes and visualizes the 3D convex hull of a mesh's vertex set. Given a 3D model (OBJ, PLY, or DCEL format), it calculates the smallest convex polyhedron enclosing all input vertices and renders the result in an interactive OpenGL viewer alongside the original mesh.

This project was developed as coursework for an **Algorithms and Data Structures 2** university course.

## Core Algorithm

The application implements the **incremental 3D convex hull** algorithm with a **conflict graph** optimization:

1. **Extract vertices** from the input mesh; discard original face topology.
2. **Build an initial tetrahedron** from 4 non-coplanar points (coplanarity checked via 4x4 determinant using Eigen3).
3. **Initialize a conflict graph** mapping faces to vertices visible from them and vice versa.
4. **Incrementally insert** each remaining vertex:
   - Identify all faces visible from the vertex (via the conflict graph).
   - Compute the **horizon** — the boundary between visible and non-visible faces.
   - Delete visible faces from the DCEL and conflict graph.
   - Create new triangular faces connecting the horizon edges to the inserted vertex.
   - Update the conflict graph by re-testing only candidate vertices from destroyed faces against new faces.
5. **Output** a triangulated convex hull stored in a DCEL (Doubly-Connected Edge List).

## Technology Stack

| Component | Technology |
|-----------|------------|
| Language | C++11 |
| GUI Framework | Qt 5 (widgets, forms, dialogs) |
| 3D Rendering | QGLViewer + OpenGL/GLU |
| Linear Algebra | Eigen3 |
| Mesh Utilities | CGAL (optional triangulation), GMP |
| Containers | Boost (`bimap`, tokenizer) |
| Build System | qmake (`.pro` file) |

## Project Structure

```
ConvexHull/
├── main.cpp                     Entry point
├── convexhullbuilder.*          Algorithm orchestrator
├── tetrahedronbuilder.*         Initial tetrahedron from 4 non-coplanar points
├── conflictgraph.*              Face-vertex visibility conflict graph
├── facebuilderhelper.*          Triangle face construction from horizon edges
├── convexhullbuilderhelper.*    Vertex extraction and horizon computation
├── GUI/
│   ├── mainwindow.*             Main window with object stack
│   ├── glcanvas.*               QGLViewer-based OpenGL canvas
│   └── managers/
│       ├── dcelmanager.*        Load/save/display meshes
│       ├── convexhullmanager.*  Run algorithm, show timing
│       └── windowmanager.*      Background, axis, snapshots
└── lib/
    ├── dcel/                    DCEL mesh library (by Muntoni/Scalas)
    └── common/                  Point, bounding box, timer utilities

models/                          Sample OBJ test meshes (bunny, cow, homer, sphere, etc.)
```

## Usage

1. Build with `qmake` using `ConvexHull/65034_medda_andrea.pro`.
2. Launch the application.
3. Click **Load Dcel** and select a mesh from the `models/` directory.
4. Open the **Convex Hull Solver** tab.
5. Optionally enable **Show Phases** to visualize each incremental step.
6. Click **Convex Hull!** to compute. The result renders in cyan alongside the original mesh.

## Key Design Decisions

- **Conflict graph optimization** avoids re-testing all vertices against all faces at each step; only vertices from destroyed faces are re-checked against new faces.
- **DCEL data structure** provides efficient mesh traversal (vertex/edge/face adjacency) and supports incremental modification during hull construction.
- **Phase visualization** enables step-by-step debugging by rendering the DCEL state after each vertex insertion.
- **Vertices-only approach** — the input mesh's face connectivity is discarded; only vertex positions are used, and the output is an independently constructed triangulated convex hull.
