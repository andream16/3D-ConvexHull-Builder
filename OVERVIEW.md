# 3D Convex Hull Builder — Repository Overview

## Purpose

This is a **C++11 / Qt 5 desktop application** developed as a university project for an
*Algorithms and Data Structures 2* course. It computes the **3D convex hull** of a triangle
mesh — the smallest convex polyhedron enclosing all input vertices — and renders the result
in an interactive OpenGL viewer.

**Author:** Andrea Medda (student ID 65034)

## Algorithm

The application implements the classic **incremental convex hull with conflict graph**:

1. **Load** a 3D mesh (`.obj` format) and extract all vertex positions.
2. **Build an initial tetrahedron** from four non-coplanar points (selected via Eigen
   4×4 determinant test).
3. **Initialize a conflict graph** — a bidirectional mapping between remaining vertices
   and the tetrahedron faces visible from each vertex.
4. **Incrementally insert** each remaining vertex:
   - Identify all faces visible from the vertex (via the conflict graph).
   - Compute the **horizon** — the boundary edges between visible and non-visible faces.
   - Delete visible faces from the DCEL.
   - Create new triangular faces connecting the horizon to the new vertex.
   - Update the conflict graph for the newly created faces.
5. **Output** the convex hull as a fully connected DCEL mesh, rendered in cyan.

An optional "Show Phases" mode re-draws the viewer after each vertex insertion to
visualize the hull growing step by step.

## Technology Stack

| Component | Technology |
|---|---|
| Language | C++11 |
| GUI framework | Qt 5 (core, gui, widgets, opengl, xml) |
| 3D viewer | libQGLViewer (QGLViewer) |
| Linear algebra | Eigen 3 |
| Containers / parsing | Boost (bimap, tokenizer) |
| Extended mesh ops | CGAL (optional, behind `CGAL_DEFINED`) |
| Arbitrary precision | GMP (required by CGAL) |
| Build system | qmake (`.pro` file) |

## Project Structure

```
├── README.md                         # User-facing documentation
├── models/                           # Sample .obj meshes (bunny, cow, homer, …)
└── ConvexHull/
    ├── 65034_medda_andrea.pro        # qmake project file (build entry point)
    ├── main.cpp                      # Application entry point
    ├── convexhullbuilder.*           # Algorithm orchestrator
    ├── convexhullbuilderhelper.*     # Vertex extraction, horizon computation
    ├── tetrahedronbuilder.*          # Initial tetrahedron construction
    ├── conflictgraph.*               # Conflict graph (vertex ↔ face visibility)
    ├── facebuilderhelper.*           # New triangular face creation on DCEL
    ├── GUI/
    │   ├── mainwindow.*              # Main window with toolbox manager stack
    │   ├── glcanvas.*                # QGLViewer-based 3D canvas
    │   └── managers/
    │       ├── dcelmanager.*         # Mesh load/save/render
    │       ├── convexhullmanager.*   # Run the convex hull algorithm
    │       └── windowmanager.*       # View settings (background, snapshots)
    └── lib/
        ├── common/                   # Shared utilities (Point, BoundingBox, timers)
        └── dcel/                     # DCEL mesh library (Muntoni / Scalas, GPL)
            ├── dcel_struct.*         # Core DCEL (load/save OBJ/PLY/DCEL)
            ├── drawable_dcel.*       # OpenGL-renderable DCEL
            └── …                     # Vertex, HalfEdge, Face implementations
```

## Key Classes

| Class | Responsibility |
|---|---|
| `ConvexHullBuilder` | Orchestrates the full incremental hull algorithm |
| `TetrahedronBuilder` | Selects 4 non-coplanar points and builds the seed tetrahedron |
| `ConflictGraph` | Maintains bidirectional vertex ↔ face visibility mappings |
| `ConvexHullBuilderHelper` | Extracts vertices; computes the horizon boundary |
| `FaceBuilderHelper` | Wires new DCEL faces (twins, next/prev) from a horizon edge + vertex |
| `DrawableDcel` | DCEL mesh with OpenGL rendering (wireframe, flat/smooth shading) |

## Building

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

System dependencies: `gcc/g++ ≥ 6`, Qt 5, `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`,
`libqglviewer-dev`, Eigen 3. Library paths in the `.pro` file may require manual adjustment
for your system.

## Usage

1. Launch the application.
2. In the **Dcel** tab, click **Load Dcel** and select an `.obj` file from `models/`.
3. Switch to the **Convex Hull Solver** tab.
4. Optionally check **Show Phases** for step-by-step visualization.
5. Click **Convex Hull!** to compute. The result appears as a cyan mesh overlay.
