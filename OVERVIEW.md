# 3D-ConvexHull-Builder — Repository Overview

## Purpose

This is a **C++ desktop application** developed for an Algorithms and Data Structures 2 university course. It loads a 3D mesh from file (OBJ, PLY, or DCEL format), computes its **3D convex hull** — the smallest convex polyhedron enclosing all input vertices — and renders the result in an interactive OpenGL viewer.

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 (core, gui, widgets, opengl, xml) | GUI framework |
| QGLViewer | Interactive 3D OpenGL canvas |
| Eigen3 | Linear algebra (coplanarity checks) |
| Boost | Tokenizer for mesh parsing, bimap for UI |
| CGAL / GMP | Optional robust geometry operations |
| qmake | Build system |

## Project Structure

```
ConvexHull/
├── main.cpp                     # Application entry point
├── convexhullbuilder.*          # Core convex hull orchestration
├── conflictgraph.*              # Conflict graph (face ↔ vertex visibility)
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # Triangular face / half-edge creation
├── convexhullbuilderhelper.*    # Horizon computation, vertex extraction
├── common.*                     # Manager ID constants
├── GUI/
│   ├── mainwindow.*             # Main window with manager tabs
│   ├── glcanvas.*               # OpenGL rendering widget
│   └── managers/                # UI panels (Dcel, ConvexHull, Window)
└── lib/
    ├── dcel/                    # DCEL mesh library (~2000+ lines)
    └── common/                  # Point, bounding box, timer utilities

models/                          # 11 sample OBJ meshes (bunny, cow, sphere, etc.)
```

## Algorithm

The project implements an **incremental convex hull algorithm** backed by a conflict graph:

1. **Extract vertices** from the loaded mesh and reset the DCEL.
2. **Build an initial tetrahedron** from four non-coplanar points (verified via a 4×4 determinant using Eigen).
3. **Initialize the conflict graph** — a bidirectional mapping tracking which remaining vertices are visible from which hull faces.
4. **Iterate** over each remaining vertex:
   - Identify all hull faces visible to the vertex.
   - Compute the **horizon** (boundary edges of the visible region).
   - Delete visible faces and create new triangular faces connecting the horizon to the vertex.
   - Update the conflict graph for the new faces.
5. **Output** the convex hull as a valid DCEL mesh rendered in the viewer.

## Key Features

- **Interactive 3D visualization** with wireframe, flat-shading, smooth-shading, and point-cloud modes.
- **Step-by-step mode** ("Show Phases") to watch the hull grow incrementally.
- **Multiple mesh formats** supported (OBJ, PLY, custom DCEL).
- **Sample models** included — from simple primitives (sphere, cylinder) to complex scans (Stanford bunny with ~28K vertices).
- **Timer** reporting algorithm runtime in the UI.

## Building

The project uses a qmake `.pro` file (`ConvexHull/65034_medda_andrea.pro`). Library paths for QGLViewer, Eigen, Boost, and CGAL may need adjustment for your system. See the [README](README.md) for detailed setup instructions.

## Authors

- **Andrea Medda** — convex hull algorithm and application
- **Alessandro Muntoni & Andreas Scalas** — DCEL mesh library
- **Marco Livesu** — GUI framework shell
