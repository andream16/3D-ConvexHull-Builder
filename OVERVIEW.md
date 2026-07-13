# 3D Convex Hull Builder — Project Overview

A **C++11 / Qt 5** desktop application that loads 3D meshes, computes their
[convex hull](https://en.wikipedia.org/wiki/Convex_hull) using an incremental
algorithm with a conflict graph, and renders the result in an interactive
OpenGL viewer. Built as a student project for an *Algorithms and Data
Structures 2* university course.

## What It Does

| Step | Description |
|------|-------------|
| **Load** | Import a 3D model (`.obj`, `.ply`, or `.dcel` format) via the GUI |
| **Compute** | Run the incremental convex hull algorithm on the mesh vertices |
| **Visualize** | Display the resulting convex polyhedron in a 3D OpenGL canvas |

An optional **Show Phases** mode lets you watch the algorithm build the hull
one vertex at a time.

## Core Algorithm

The convex hull is constructed incrementally:

1. Extract all vertices from the input mesh and reset the DCEL.
2. Find four non-coplanar points (verified with a 4×4 determinant via Eigen)
   and build an initial **tetrahedron**.
3. Initialize a **conflict graph** that tracks which remaining vertices are
   visible from which hull faces.
4. For each remaining vertex that conflicts with at least one face:
   - Compute the **horizon** — the boundary edges between visible and
     non-visible faces.
   - Remove visible faces from the hull.
   - Create new triangular faces connecting the horizon edges to the new
     vertex.
   - Update the conflict graph for the new faces.
5. Output the final convex hull with correct normals, colored cyan.

## Data Structure

The mesh is represented with a **DCEL (Doubly-Connected Edge List)** —
vertices, half-edges, and faces linked by pointers — supporting general
polygons (not just triangles), iterators for traversal, and file I/O for
OBJ/PLY/DCEL formats.

## Technology Stack

| Component | Technology |
|-----------|-----------|
| Language | C++11 |
| GUI | Qt 5, QGLViewer |
| Rendering | OpenGL / GLU |
| Linear algebra | Eigen 3 |
| Utilities | Boost (bimap, tokenizer) |
| Geometry (optional) | CGAL, GMP |
| Build system | qmake (`.pro` file) |

## Project Layout

```
ConvexHull/
├── main.cpp                       # Application entry point
├── 65034_medda_andrea.pro         # qmake build file
├── convexhullbuilder.*            # Algorithm orchestrator
├── conflictgraph.*                # Face ↔ vertex conflict tracking
├── tetrahedronbuilder.*           # Seed tetrahedron construction
├── facebuilderhelper.*            # New face creation from horizon edges
├── convexhullbuilderhelper.*      # Vertex extraction, horizon computation
├── GUI/
│   ├── mainwindow.*               # Main application window
│   ├── glcanvas.*                 # QGLViewer-based 3D canvas
│   └── managers/
│       ├── dcelmanager.*          # Load / save / display meshes
│       ├── convexhullmanager.*    # Run the convex hull solver
│       └── windowmanager.*        # Background, snapshot, axis controls
└── lib/
    ├── dcel/                      # Full DCEL mesh library
    └── common/                    # Shared utilities (Point, BoundingBox, …)

models/                            # 11 sample OBJ meshes (bunny, cow, …)
```

## Building and Running

1. Install dependencies: GCC/G++, Qt 5, Boost, CGAL, GMP, libQGLViewer.
2. Build with qmake:
   ```bash
   cd ConvexHull
   qmake 65034_medda_andrea.pro
   make
   ```
3. Run the binary, click **Load Dcel** to open a model, then switch to the
   **Convex Hull Solver** panel and click **Convex Hull!**.
