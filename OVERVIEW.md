# Repository Overview: 3D-ConvexHull-Builder

## Purpose

This repository is a **C++/Qt5 desktop application** developed as a course project for **Algorithms and Data Structures 2**. It loads a 3D mesh, computes its **3D convex hull** (the smallest convex polyhedron enclosing all input vertices), and visualizes the result interactively using OpenGL.

## Tech Stack

| Component | Technology |
|-----------|------------|
| Language | C++11 |
| GUI Framework | Qt 5 (core, gui, widgets, OpenGL, XML) |
| 3D Viewer | QGLViewer |
| Linear Algebra | Eigen3 |
| Utilities | Boost (bimap) |
| Computational Geometry | CGAL (optional) |
| Math | GMP, GLU |

## Project Structure

```
ConvexHull/
├── main.cpp                    # Application entry point
├── 65034_medda_andrea.pro      # Qt qmake build file
├── convexhullbuilder.*         # Main algorithm orchestrator
├── tetrahedronbuilder.*        # Initial tetrahedron construction
├── conflictgraph.*             # Conflict graph data structure
├── facebuilderhelper.*         # New face construction
├── convexhullbuilderhelper.*   # Horizon & vertex utilities
├── GUI/
│   ├── mainwindow.*            # Main Qt window
│   ├── glcanvas.*              # QGLViewer OpenGL canvas
│   └── managers/
│       ├── dcelmanager.*       # Mesh load/save/render
│       ├── convexhullmanager.* # Convex hull solver UI
│       └── windowmanager.*     # Background, snapshot, axis controls
└── lib/
    ├── common/                 # Point, bounding box, drawable abstractions
    └── dcel/                   # Doubly-Connected Edge List mesh library
models/                         # Sample .obj meshes (bunny, cow, homer, etc.)
```

## Algorithm

The application implements an **incremental 3D convex hull algorithm** with a **conflict graph**:

1. **Extract vertices** from the loaded mesh (stored as a DCEL).
2. **Build an initial tetrahedron** from four non-coplanar vertices using Eigen for determinant and coplanarity checks.
3. **Initialize the conflict graph** — a bidirectional mapping between faces and vertices where the vertex lies on the visible side of the face.
4. **Incrementally insert remaining vertices**: for each vertex, find visible faces, compute the horizon (boundary edges of the visible region), delete visible faces, create new triangular faces connecting horizon edges to the vertex, and update the conflict graph.
5. **Finalize** — update normals, color the hull, and report elapsed time.

## Key Features

- **Mesh I/O**: Load and save meshes in OBJ, PLY, and DCEL formats.
- **Multiple rendering modes**: Wireframe, points, flat shading, smooth shading.
- **Step-by-step visualization**: Enable "Show Phases" to watch each incremental step of the convex hull algorithm.
- **Interactive 3D navigation**: Rotate, zoom, and pan the scene via QGLViewer.
- **Sample models included**: bunny, cow, homer, sphere, cylinder, and more.

## Building

Requires GCC/G++ ~6.2.1, Qt ~5.7, and the following libraries: `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`. Build using `qmake` with the provided `.pro` file.

## Credits

- **Author**: Andrea Medda (student project)
- **GUI framework**: Marco Livesu
- **DCEL library**: Alessandro Muntoni, Andreas Scalas
