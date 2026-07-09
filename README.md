# 3D-ConvexHull-Builder

A **C++11 / Qt5 desktop application** that computes the **3D convex hull** of a mesh — the smallest convex polyhedron enclosing all input vertices — and visualizes the result in an interactive OpenGL viewer.

Built as a project for the **Algorithms and Data Structures 2** course.

## Overview

The application loads a 3D model (OBJ, PLY, or DCEL format), runs an **incremental convex hull algorithm** backed by a **conflict graph**, and renders the resulting hull in a 3D viewport powered by [libQGLViewer](http://libqglviewer.com/).

### Algorithm

The core implementation follows the classic incremental 3D convex hull construction:

1. **Extract vertices** from the input mesh.
2. **Build an initial tetrahedron** from four non-coplanar points (coplanarity checked via Eigen 4×4 determinant).
3. **Initialize a conflict graph** mapping faces to the vertices visible from them (half-space test).
4. **Insert remaining vertices** one at a time:
   - Identify faces visible from the current vertex.
   - Compute the **horizon** — the boundary between visible and non-visible faces.
   - Remove visible faces and build new triangular faces connecting the horizon to the vertex.
   - Update the conflict graph for the newly created faces.
5. **Finalize** — recompute normals, color the hull, and display.

### Key Components

| Component | Purpose |
|---|---|
| `ConvexHullBuilder` | Orchestrates the full algorithm pipeline |
| `TetrahedronBuilder` | Constructs the seed tetrahedron from 4 non-coplanar points |
| `ConflictGraph` | Bidirectional face ↔ vertex visibility tracking |
| `ConvexHullBuilderHelper` | Horizon detection, vertex extraction |
| `FaceBuilderHelper` | Creates new triangular faces with correct half-edge topology |
| `lib/dcel/` | Full **DCEL** (Doubly-Connected Edge List) mesh library with file I/O and OpenGL rendering |
| `GUI/` | Qt main window, QGLViewer canvas, and manager panels |

### Technologies

- **C++11** — core language
- **Qt 5** — GUI framework (widgets, OpenGL integration)
- **Eigen3** — determinant computation for coplanarity tests
- **Boost** — `bimap` for UI object mapping
- **CGAL** — optional mesh triangulation
- **libQGLViewer** — interactive 3D camera and rendering

## Requirements

- `gcc` / `g++` ~6.2.1
- `Qt` ~5.7
- `libboost-all-dev`
- `libcgal-dev`
- `libgmp-dev`
- `libqglviewer-dev`

## Building

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

> **Note:** The `.pro` file contains some hard-coded library paths. You may need to edit them to match your system's installation directories.

## Documentation

To generate the DCEL library documentation:

```bash
# Install doxygen first, then:
cd ConvexHull/lib
doxygen dcel_doxygen_config_it    # Italian
doxygen dcel_doxygen_config_eng   # English
# Output goes to dcel_doc/
```

## Usage

1. Click **Load Dcel** and select a model from the `models/` directory (bunny, cow, sphere, etc.).
2. Open the **Convex Hull Solver** tab.
3. Optionally enable **Show Phases** to step through the algorithm visually.
4. Click **Convex Hull!** to compute the hull. The result appears as a cyan-colored mesh, and execution time is displayed.

## Sample Models

The `models/` directory contains OBJ meshes for testing, including Stanford Bunny (~28K vertices), cow, sphere, homer, and others.

## Screenshots

### Input
![1.png](https://s22.postimg.org/4o2fu3kbl/1.png)
### Output
![2.png](https://s22.postimg.org/c5bn9b9up/2.png)

## Project Structure

```
ConvexHull/
├── main.cpp                        # Entry point
├── convexhullbuilder.*             # Algorithm orchestrator
├── tetrahedronbuilder.*            # Initial tetrahedron construction
├── conflictgraph.*                 # Face–vertex conflict graph
├── convexhullbuilderhelper.*       # Horizon computation, vertex extraction
├── facebuilderhelper.*             # New face construction
├── GUI/
│   ├── mainwindow.*                # Main window + manager toolbox
│   ├── glcanvas.*                  # QGLViewer OpenGL canvas
│   └── managers/                   # DCEL, Convex Hull, Window managers
└── lib/
    ├── dcel/                       # DCEL mesh library
    └── common/                     # Point, bounding box, timers, serialization
```
