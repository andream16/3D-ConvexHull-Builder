# 3D Convex Hull Builder — Repository Overview

## Purpose

This repository contains a **C++/Qt5 desktop application** that computes the **3D convex hull** of a triangulated mesh. Given any 3D model, it finds the smallest convex polyhedron that encloses all of the model's vertices and renders the result in an interactive OpenGL viewer.

The project was developed for the **Algorithms and Data Structures 2** university course.

## How It Works

The application follows an **incremental convex hull algorithm** backed by a **conflict graph** for efficient visibility tracking:

1. **Load a mesh** — The user imports a 3D model (OBJ, PLY, or DCEL format) through the GUI.
2. **Extract vertices** — Only vertex positions are used; original faces are discarded.
3. **Build an initial tetrahedron** — Four non-coplanar points are selected to seed the hull.
4. **Initialize the conflict graph** — A bidirectional mapping tracks which vertices are visible from which hull faces, and vice versa.
5. **Incremental insertion** — Each remaining vertex is processed in turn:
   - If visible from any current hull face, the **horizon** (boundary between visible and hidden faces) is computed.
   - Visible faces are removed and new triangular faces are created from the horizon edges to the new vertex.
   - The conflict graph is updated for the new faces.
6. **Render the result** — The convex hull is displayed as a cyan mesh in the 3D viewer.

An optional **"Show Phases"** mode lets the user watch the hull grow step by step.

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core implementation language |
| Qt 5 | GUI framework (widgets, dialogs, OpenGL integration) |
| QGLViewer | Interactive 3D rendering and camera control |
| Eigen3 | Linear algebra (determinants for coplanarity checks) |
| Boost | `bimap` for UI object management |
| CGAL | Optional mesh triangulation |
| GMP | Arbitrary-precision arithmetic (CGAL dependency) |

## Project Structure

```
ConvexHull/
├── main.cpp                    # Application entry point
├── convexhullbuilder.*         # Algorithm orchestrator
├── conflictgraph.*             # Face–vertex visibility tracking
├── tetrahedronbuilder.*        # Initial tetrahedron construction
├── facebuilderhelper.*         # New face creation in the DCEL
├── convexhullbuilderhelper.*   # Vertex extraction, horizon computation
├── GUI/
│   ├── mainwindow.*            # Main window and manager toolbox
│   ├── glcanvas.*              # OpenGL 3D viewer
│   └── managers/               # UI panels (Dcel, ConvexHull, Window)
└── lib/
    ├── dcel/                   # Doubly-Connected Edge List mesh library
    └── common/                 # Utilities (points, bounding boxes, timers)

models/                         # Sample 3D models (bunny, cow, sphere, homer, etc.)
```

## Key Data Structure: DCEL

The project uses a **Doubly-Connected Edge List** (DCEL) to represent 3D meshes. This structure stores vertices, half-edges, and faces with full adjacency information (`next`, `prev`, `twin` pointers), enabling efficient traversal and modification during hull construction. The DCEL library supports loading and saving OBJ, PLY, and native DCEL formats.

## Building

The project uses **qmake** via the provided `.pro` file. Required dependencies:

- GCC/G++ (6.2+)
- Qt 5.7+
- `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`

Refer to the [README](README.md) for platform-specific build instructions.

## Sample Models

The `models/` directory contains OBJ files for testing: Stanford Bunny, cow, sphere, Homer Simpson, and others.
