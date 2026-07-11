# Repository Overview

## Purpose

This repository implements a **3D Convex Hull Builder** — a C++11/Qt5 desktop application that computes the minimum convex polyhedron enclosing all vertices of a given 3D mesh and visualizes the result in an interactive OpenGL viewer. It was created as a course project for **Algorithms and Data Structures 2** by Andrea Medda.

## Technology Stack

| Category | Technology |
|---|---|
| Language | C++11 |
| Build System | Qt qmake (`65034_medda_andrea.pro`) |
| GUI Framework | Qt5 (Widgets, OpenGL, XML) |
| 3D Rendering | QGLViewer + OpenGL |
| Linear Algebra | Eigen3 (determinant-based coplanarity checks) |
| Utilities | Boost (`bimap` for UI object tracking) |
| Optional | CGAL + GMP (enabled via `CGAL_DEFINED`) |
| Documentation | Doxygen |

## Project Structure

```
/workspace/
├── README.md
├── OVERVIEW.md            # This file
├── models/                # Sample .obj meshes (bunny, cow, homer, sphere, etc.)
└── ConvexHull/
    ├── main.cpp                    # Application entry point
    ├── 65034_medda_andrea.pro      # Qt project file / build configuration
    ├── convexhullbuilder.*         # Main algorithm orchestrator
    ├── tetrahedronbuilder.*        # Initial tetrahedron construction
    ├── conflictgraph.*             # Conflict graph for incremental hull
    ├── facebuilderhelper.*         # Triangular face creation along horizon
    ├── convexhullbuilderhelper.*   # Vertex extraction & horizon computation
    ├── common.*                    # Global manager IDs
    ├── GUI/
    │   ├── mainwindow.*            # Main window + visibility management
    │   ├── glcanvas.*              # OpenGL canvas (QGLViewer-based)
    │   ├── managers/               # UI panels (Dcel, ConvexHull, Window)
    │   └── objects/                # Primitive drawable shapes
    └── lib/
        ├── dcel/                   # Doubly-Connected Edge List mesh library
        └── common/                 # Point, bounding box, serialization, timers
```

## Core Algorithm

The application implements the **incremental 3D convex hull algorithm with a conflict graph**, a well-known computational geometry technique. The process is:

1. **Load Input** — A 3D mesh (`.obj` file) is loaded into a DCEL (Doubly-Connected Edge List) data structure and all vertex coordinates are extracted.

2. **Build Initial Tetrahedron** — Four non-coplanar vertices are identified (using Eigen 4×4 determinants) and used to construct a seed tetrahedron as the starting hull.

3. **Initialize Conflict Graph** — For each hull face and each remaining vertex, a half-space test determines visibility. Two bidirectional maps are maintained: faces → visible vertices and vertices → visible faces.

4. **Incremental Insertion** — Each remaining vertex is processed in turn:
   - If the vertex is visible from any hull face, the **horizon** (boundary between visible and non-visible faces) is computed.
   - Visible faces are deleted.
   - New triangular faces are created connecting each horizon edge to the current vertex.
   - The conflict graph is updated for the new faces.

5. **Output** — The resulting convex hull is rendered as a cyan-colored mesh in the OpenGL viewer with properly computed normals.

## Key Modules

| Module | Responsibility |
|---|---|
| `ConvexHullBuilder` | Orchestrates the full convex hull pipeline |
| `TetrahedronBuilder` | Finds 4 non-coplanar points and builds the seed tetrahedron |
| `ConflictGraph` | Tracks vertex-face visibility relationships |
| `ConvexHullBuilderHelper` | Extracts vertices from the mesh and computes horizon edges |
| `FaceBuilderHelper` | Creates new triangular faces along the horizon |
| `DCEL Library` | Full doubly-connected edge list implementation for 3D meshes (authored by Muntoni/Scalas) |
| `GUI Layer` | Qt5 main window, OpenGL canvas, and manager panels for user interaction |

## Building and Running

### Prerequisites

- GCC/G++ ~6.2+, Qt ~5.7
- `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`

### Build

Open `ConvexHull/65034_medda_andrea.pro` in Qt Creator, or run:

```bash
cd ConvexHull
qmake
make
```

### Usage

1. Launch the application.
2. Click **Load Dcel** and select a model from the `models/` directory.
3. Open the **Convex Hull Solver** panel.
4. Optionally enable **Show Phases** for step-by-step visualization, then click **Convex Hull!**.
