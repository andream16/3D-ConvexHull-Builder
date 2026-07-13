# 3D Convex Hull Builder

## Purpose

This is a C++11/Qt5 desktop application built as a university course project (Algorithms and Data Structures 2). It loads a 3D mesh, computes its **3D convex hull** — the smallest convex polyhedron enclosing all vertices — and visualizes the result in an interactive OpenGL viewer.

## How It Works

1. **Load** a 3D model (`.obj`, `.ply`, or `.dcel` format)
2. **Build an initial tetrahedron** from four non-coplanar vertices
3. **Initialize a conflict graph** mapping vertices to the faces they can "see"
4. **Incrementally insert** remaining vertices: find visible faces, compute the horizon boundary, replace visible faces with new triangles, and update the conflict graph
5. **Visualize** the resulting convex hull interactively

The algorithm follows the classic **incremental convex hull with conflict graph** approach.

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, OpenGL integration) |
| libQGLViewer | 3D OpenGL canvas with camera controls |
| Eigen3 | Linear algebra (determinants, orientation tests) |
| Boost | Utilities (bimap, system) |
| CGAL (optional) | Extended mesh operations |

Build system: Qt `.pro` file (`ConvexHull/65034_medda_andrea.pro`).

## Project Structure

```
ConvexHull/
├── main.cpp                     # Application entry point
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Conflict graph data structure
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # Face/half-edge topology construction
├── convexhullbuilderhelper.*    # Horizon finding, vertex extraction
├── GUI/
│   ├── mainwindow.*             # Main window with manager toolbox
│   ├── glcanvas.*               # QGLViewer-based 3D canvas
│   └── managers/                # UI panels (Dcel, ConvexHull, Window)
└── lib/
    ├── dcel/                    # DCEL mesh library (vertices, half-edges, faces)
    └── common/                  # Points, bounding boxes, timers, serialization

models/                          # Sample 3D meshes (bunny, cow, homer, sphere, etc.)
```

## Key Components

- **ConvexHullBuilder** — orchestrates the incremental algorithm
- **ConflictGraph** — tracks which vertices are visible from which faces using hash maps
- **TetrahedronBuilder** — constructs the initial seed tetrahedron from non-coplanar points
- **FaceBuilderHelper** — creates new triangular faces and sets half-edge topology
- **DCEL Library** — full Doubly-Connected Edge List implementation for 3D mesh representation
- **GUI Layer** — Qt5 interface with interactive 3D rendering, wireframe/shading modes, and step-by-step algorithm visualization

## Author

Andrea Medda (student project), with DCEL library by Alessandro Muntoni and Andreas Scalas, and GUI framework by Marco Livesu.
