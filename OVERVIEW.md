# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++ desktop application that computes the 3D convex hull of a loaded polygonal mesh and visualizes the result in an interactive OpenGL viewer. It was developed as coursework for an Algorithms and Data Structures 2 university course.

Given a 3D model (e.g. a bunny, cow, or arbitrary shape), the program calculates the smallest convex polyhedron that fully encloses every vertex of the input mesh.

## Tech Stack

| Component | Technology |
|-----------|------------|
| Language | C++11 |
| GUI framework | Qt 5 (widgets, OpenGL, XML) |
| 3D viewer | libQGLViewer (QGLViewer) |
| Linear algebra | Eigen3 |
| Containers | Boost (bimap) |
| Geometry (optional) | CGAL + GMP |
| Build system | qmake |
| Documentation | Doxygen |

## Algorithm

The convex hull is computed using an **incremental insertion algorithm with a conflict graph**:

1. **Extract vertices** from the input mesh.
2. **Build an initial tetrahedron** from four non-coplanar vertices (coplanarity checked via Eigen determinants).
3. **Initialize a conflict graph** — a bidirectional mapping between remaining vertices and the tetrahedron faces visible from each vertex.
4. **Insert remaining vertices one at a time**: for each vertex, find its visible (conflicting) faces, compute the horizon (the boundary edge loop between visible and hidden faces), delete visible faces, create new triangular faces from the horizon edges to the inserted vertex, and update the conflict graph.
5. **Finalize** — recompute normals, color the hull, and refresh the viewer.

## Project Structure

```
ConvexHull/
├── main.cpp                      # Application entry point
├── 65034_medda_andrea.pro        # qmake build configuration
├── convexhullbuilder.*           # Orchestrates the hull algorithm
├── tetrahedronbuilder.*          # Constructs the initial tetrahedron
├── conflictgraph.*               # Face/vertex conflict graph
├── facebuilderhelper.*           # Creates new hull faces
├── convexhullbuilderhelper.*     # Horizon computation, vertex extraction
├── common.*                      # Global manager IDs
├── GUI/
│   ├── mainwindow.*              # Main Qt window
│   ├── glcanvas.*                # OpenGL rendering canvas
│   ├── managers/
│   │   ├── dcelmanager.*         # Load/save/display meshes
│   │   ├── convexhullmanager.*   # Run the convex hull solver
│   │   └── windowmanager.*       # Display settings
│   └── objects/                  # Primitive shapes (sphere, cylinder)
└── lib/
    ├── dcel/                     # Doubly-Connected Edge List library
    └── common/                   # Point, bounding box, serialization

models/                           # Sample OBJ meshes (bunny, cow, sphere, etc.)
```

## Key Components

### DCEL Mesh Library (`lib/dcel/`)

A full Doubly-Connected Edge List implementation supporting vertices, half-edges, and faces with pointer-based navigation. Supports OBJ, PLY, and custom `.dcel` file formats. The `DrawableDcel` subclass adds OpenGL rendering with wireframe, flat/smooth shading, and color modes. Originally authored by Alessandro Muntoni and Andreas Scalas.

### Interactive GUI

A manager-based Qt5 interface where each panel controls a specific aspect of the application:

- **DcelManager** — file I/O and rendering options for the loaded mesh.
- **ConvexHullManager** — runs the algorithm with optional step-by-step visualization and elapsed-time display.
- **GLcanvas** — QGLViewer-based 3D canvas with camera controls and object visibility toggles.

### Convex Hull Solver

The algorithm implementation spans several classes: `ConvexHullBuilder` orchestrates the process, `TetrahedronBuilder` sets up the initial simplex, `ConflictGraph` tracks vertex-face visibility, `ConvexHullBuilderHelper` handles horizon computation, and `FaceBuilderHelper` constructs new triangular faces.

## Usage

1. Build using qmake against `ConvexHull/65034_medda_andrea.pro`.
2. Launch the application.
3. Click **Load Dcel** and select a model from the `models/` directory.
4. Open the **Convex Hull Solver** panel.
5. Optionally enable **Show Phases** for step-by-step visualization, then click **Convex Hull!**.

## Build Requirements

- gcc/g++ with C++11 support
- Qt 5
- System packages: `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`
- Eigen3 headers (expected at `/usr/include/eigen3/`)

Library paths in the `.pro` file may need manual adjustment for different system configurations.
