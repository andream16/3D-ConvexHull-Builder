# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application that computes the 3D convex hull of a loaded 3D mesh. Given any 3D model (OBJ, PLY, or DCEL format), it calculates the minimum convex polyhedron that contains all input vertices and displays the result in an interactive OpenGL viewer.

Built as an academic project for an Algorithms and Data Structures 2 course.

## Core Algorithm

The application implements an **incremental convex hull algorithm** backed by a **conflict graph**:

1. Load a 3D mesh and extract its vertices.
2. Construct an initial tetrahedron from four non-coplanar points.
3. Initialize a conflict graph mapping vertices to visible faces.
4. For each remaining vertex visible from at least one face:
   - Compute the horizon (boundary between visible and hidden faces).
   - Remove visible faces from the hull.
   - Create new triangular faces connecting the horizon edges to the vertex.
   - Update the conflict graph.
5. Output the resulting convex hull as a DCEL mesh.

## Tech Stack

| Component | Technology |
|-----------|------------|
| Language | C++11 |
| GUI framework | Qt 5 (widgets, OpenGL, XML) |
| 3D rendering | libQGLViewer + OpenGL |
| Linear algebra | Eigen3 |
| Utilities | Boost (tokenizer, bimap) |
| Geometry (optional) | CGAL |
| Build system | qmake (`.pro` file) |

## Project Structure

```
ConvexHull/
├── main.cpp                      # Entry point
├── convexhullbuilder.*           # Algorithm orchestrator
├── conflictgraph.*               # Face-vertex conflict tracking
├── tetrahedronbuilder.*          # Initial tetrahedron construction
├── facebuilderhelper.*           # New face creation during expansion
├── convexhullbuilderhelper.*     # Horizon computation, vertex extraction
├── GUI/
│   ├── mainwindow.*              # Main window with manager toolbox
│   ├── glcanvas.*                # QGLViewer OpenGL canvas
│   └── managers/
│       ├── dcelmanager.*         # Load/save/visualize meshes
│       ├── convexhullmanager.*   # Run convex hull, display results
│       └── windowmanager.*       # View settings (background, axis, etc.)
└── lib/
    ├── dcel/                     # DCEL mesh data structure library
    └── common/                   # Point, bounding box, serialization
models/                           # Sample OBJ meshes (bunny, cow, etc.)
```

## Key Components

- **DCEL Library** (`lib/dcel/`): A full Doubly-Connected Edge List implementation for 3D polygonal meshes, supporting vertices, half-edges, and faces with O(1) adjacency navigation. Includes `DrawableDcel` for OpenGL rendering.

- **ConvexHullBuilder**: Orchestrates the incremental algorithm, coordinating the conflict graph, tetrahedron construction, horizon computation, and face building.

- **ConflictGraph**: Maintains bidirectional mappings between vertices and the faces they can "see" (via half-space tests), enabling efficient updates as the hull expands.

- **GUI Managers**: A toolbox-based UI where `DcelManager` handles mesh I/O, `ConvexHullManager` runs the algorithm and displays timing, and `WindowManager` controls rendering options. An optional "Show Phases" mode refreshes the 3D view after each vertex insertion for step-by-step visualization.

## Usage

1. Click **Load Dcel** and select a model from the `models/` directory.
2. Switch to the **Convex Hull Solver** tab.
3. Optionally enable **Show Phases** to watch the algorithm step by step.
4. Click **Convex Hull!** to compute and display the result.
