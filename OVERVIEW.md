# Repository Overview: 3D-ConvexHull-Builder

A **C++11 / Qt5 desktop application** that computes the 3D convex hull of a loaded mesh and visualizes the result in an interactive OpenGL viewer. Built as a project for the *Algorithms and Data Structures 2* university course.

## Purpose

Given a 3D model, the program computes the **minimum convex polyhedron** (convex hull) that encloses all of its vertices. Users can load a mesh, run the solver, optionally step through each algorithm phase, and inspect the resulting hull in a separate 3D view.

## Tech Stack

| Layer | Technology |
|---|---|
| Language | C++11 |
| Build system | Qt qmake (`.pro` file) |
| GUI | Qt5 (widgets, OpenGL, XML) |
| 3D rendering | libQGLViewer + OpenGL/GLU |
| Linear algebra | Eigen3 |
| Utilities | Boost (bimap, tokenizer, system) |
| Optional mesh ops | CGAL + GMP (via `CGAL_DEFINED` flag) |
| Documentation | Doxygen (for the DCEL library) |

## Project Structure

```
/
├── README.md                         # Usage instructions
├── models/                           # Sample 3D meshes (.obj)
│   ├── bunny.obj, cow.obj, homer.obj, sphere.obj, ...
└── ConvexHull/                       # Main application source
    ├── 65034_medda_andrea.pro        # Qt qmake project file
    ├── main.cpp                      # Entry point
    ├── convexhullbuilder.*           # Algorithm orchestrator
    ├── conflictgraph.*               # Face–vertex conflict graph
    ├── tetrahedronbuilder.*          # Initial tetrahedron construction
    ├── facebuilderhelper.*           # New face creation during expansion
    ├── convexhullbuilderhelper.*     # Horizon computation & vertex extraction
    ├── GUI/
    │   ├── mainwindow.*              # Main window + object stack
    │   ├── glcanvas.*                # QGLViewer OpenGL canvas
    │   └── managers/
    │       ├── dcelmanager.*         # Load/save/display meshes
    │       ├── convexhullmanager.*   # Run hull algorithm
    │       └── windowmanager.*       # View settings
    └── lib/
        ├── dcel/                     # Doubly-Connected Edge List library
        │   ├── dcel_struct.*         # Core DCEL (load OBJ/PLY/DCEL)
        │   ├── dcel_vertex/face/half_edge.*
        │   └── drawable_dcel.*       # Renderable DCEL for OpenGL
        └── common/                   # Shared utilities (point, bounding box, timers)
```

## Algorithm

The implementation uses the **incremental 3D convex hull** algorithm with a **conflict graph**:

1. **Extract vertices** from the loaded mesh and reset the DCEL topology.
2. **Build an initial tetrahedron** from four non-coplanar points (`TetrahedronBuilder`).
3. **Initialize the conflict graph** — a bidirectional mapping between faces and vertices that can "see" them (`ConflictGraph`).
4. **For each remaining vertex**, if it lies outside the current hull:
   - Compute the **horizon** — the boundary edges of the faces visible from the vertex.
   - Delete the visible faces.
   - Create new triangular faces connecting the horizon edges to the new vertex (`FaceBuilderHelper`).
   - Update the conflict graph for the newly created faces.
5. **Finalize** normals and produce the output mesh.

Visibility is determined by a half-space test using the dot product of a face's normal with the offset from the face to the vertex.

## Key Components

- **DCEL Library** (`lib/dcel/`): A full Doubly-Connected Edge List implementation supporting vertices, half-edges, and faces with pointer-based topology. Handles OBJ, PLY, and custom DCEL file formats. Originally by Alessandro Muntoni and Andreas Scalas (GPL).
- **Convex Hull Solver** (`convexhullbuilder.*`): Orchestrates the incremental algorithm and manages the conflict graph lifecycle.
- **GUI Layer** (`GUI/`): Built on Marco Livesu's Qt/QGLViewer framework. Provides mesh loading, algorithm execution controls, and interactive 3D visualization. The original mesh and computed hull are displayed side by side.
- **Sample Models** (`models/`): Bundled `.obj` test meshes including bunny, cow, homer, kitten, sphere, cylinder, and others.

## Build and Run

**Prerequisites:** GCC/G++ ~6.2.1, Qt ~5.7, `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`.

1. Open `ConvexHull/65034_medda_andrea.pro` in Qt Creator, or run `qmake && make`.
2. Adjust library paths in the `.pro` file if needed for your system.
3. Launch the application, load a model via **Load Dcel**, then click **Convex Hull!** to compute the hull.
