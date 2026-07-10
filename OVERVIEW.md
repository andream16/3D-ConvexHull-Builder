# 3D Convex Hull Builder

A C++11 / Qt5 desktop application that loads a 3D mesh, computes its **3D convex hull** (the smallest convex polyhedron enclosing all vertices), and renders the result interactively with OpenGL. Built as a course project for *Algorithms and Data Structures 2*.

## How It Works

1. **Load a mesh** — supports Wavefront OBJ, PLY, and native DCEL formats. Sample models (bunny, cow, homer, sphere, etc.) are provided in `models/`.
2. **Compute the convex hull** — uses an incremental conflict-graph algorithm over the mesh vertices.
3. **Visualize** — the hull is drawn in cyan alongside the original mesh in a QGLViewer-based 3D viewport. An optional step-by-step mode animates each vertex insertion.

## Core Algorithm

The convex hull is built incrementally:

| Step | Description |
|------|-------------|
| **Seed tetrahedron** | Pick 4 non-coplanar points (coplanarity tested via a 4×4 determinant with Eigen) and construct an initial tetrahedron. |
| **Conflict graph** | Build a bipartite mapping between remaining vertices and hull faces, recording which vertices are visible from which faces. |
| **Incremental insertion** | For each remaining vertex that sees at least one face: find the horizon edges (boundary between visible and hidden faces), delete visible faces, create new triangular faces connecting the horizon to the vertex, and update the conflict graph. |
| **Finalize** | Recompute normals, color the hull, refresh the viewport. |

## Project Structure

```
ConvexHull/
├── main.cpp                    # Application entry point
├── convexhullbuilder.*         # Algorithm orchestration
├── conflictgraph.*             # Face ↔ vertex visibility tracking
├── tetrahedronbuilder.*        # Initial tetrahedron construction
├── facebuilderhelper.*         # Half-edge face creation
├── convexhullbuilderhelper.*   # Vertex extraction & horizon computation
├── GUI/
│   ├── mainwindow.*            # Qt main window
│   ├── glcanvas.*              # QGLViewer OpenGL canvas
│   └── managers/               # DcelManager, ConvexHullManager, WindowManager
└── lib/
    ├── dcel/                   # Doubly-Connected Edge List mesh library
    └── common/                 # Point, bounding box, timer utilities
models/                         # Sample OBJ meshes for testing
```

## Dependencies

| Library | Purpose |
|---------|---------|
| Qt 5 (core, gui, widgets, opengl, xml) | GUI framework |
| QGLViewer | Interactive 3D navigation |
| Eigen 3 | Matrix math (determinants) |
| Boost | `bimap` in the UI layer |
| CGAL + GMP | Optional mesh triangulation and I/O |
| OpenGL / GLU | Rendering |

## Building

```bash
# Install dependencies (Debian/Ubuntu)
sudo apt install qt5-default libqglviewer-dev-qt5 libeigen3-dev \
                 libboost-all-dev libcgal-dev libgmp-dev

# Build
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

> Library paths in the `.pro` file may need adjustment for your system.

## Credits

- **Convex hull algorithm & integration**: Andrea Medda
- **DCEL library**: Alessandro Muntoni, Andreas Scalas
- **GUI shell**: Marco Livesu
