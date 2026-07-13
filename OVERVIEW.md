# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a Qt5 desktop application that computes the 3D convex hull of a loaded mesh model and visualizes the result in an interactive OpenGL viewer. It was developed as a university project for an Algorithms and Data Structures 2 course.

Given a 3D model (`.obj`, `.ply`, or `.dcel` format), the program extracts its vertices, computes the minimum convex polyhedron enclosing all points, and renders it alongside the original mesh.

## Core Algorithm

The implementation uses an **incremental convex hull** approach with a **conflict graph**:

1. **Extract vertices** from the input mesh and reset the DCEL structure.
2. **Build an initial tetrahedron** from four non-coplanar points (verified via Eigen3 determinant checks).
3. **Initialize a conflict graph** — a bidirectional mapping between hull faces and vertices visible from each face.
4. **Iterate over remaining vertices**: for each point visible to at least one hull face, compute the horizon (boundary between visible and non-visible faces), delete visible faces, create new triangular faces along the horizon, and update the conflict graph.
5. **Render the result** in cyan over the original mesh in the 3D viewer.

## Technology Stack

| Component | Technology |
|-----------|-----------|
| Language | C++11 |
| GUI framework | Qt 5 |
| Linear algebra | Eigen3 |
| Utilities | Boost (`bimap`, `tokenizer`) |
| Mesh operations | CGAL (optional triangulation) |
| 3D rendering | QGLViewer / OpenGL |
| Build system | qmake (`.pro` file) |

## Project Structure

```
ConvexHull/
├── main.cpp                        # Application entry point
├── convexhullbuilder.*             # Algorithm orchestrator
├── conflictgraph.*                 # Face-vertex visibility tracking
├── tetrahedronbuilder.*            # Initial tetrahedron construction
├── facebuilderhelper.*             # New face creation along horizon
├── convexhullbuilderhelper.*       # Vertex extraction, horizon computation
├── GUI/
│   ├── mainwindow.*                # Main window with dock panels
│   ├── glcanvas.*                  # QGLViewer-based 3D canvas
│   └── managers/
│       ├── dcelmanager.*           # Mesh loading and display controls
│       ├── convexhullmanager.*     # Algorithm execution and timing
│       └── windowmanager.*         # Display settings (background, axes)
└── lib/
    ├── dcel/                       # Doubly-Connected Edge List library
    │   ├── dcel_struct.*           # Core DCEL container
    │   ├── dcel_vertex/half_edge/face.*  # Topological elements
    │   └── drawable_dcel.*         # DCEL with OpenGL rendering
    └── common/                     # Shared utilities (Point, BoundingBox, Timer)

models/                             # Sample OBJ meshes (bunny, cow, homer, etc.)
```

## How to Use

1. Build with Qt Creator or `qmake` using the provided `65034_medda_andrea.pro` file.
2. Launch the application and click **Load Dcel** to open a model from the `models/` directory.
3. Switch to the **Convex Hull Solver** panel.
4. Optionally enable **Show Phases** to step through the algorithm visually.
5. Click **Convex Hull!** to compute and display the convex hull.

## Dependencies

- GCC/G++ ~6.2.1, Qt ~5.7
- `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`
- Library paths in the `.pro` file may need adjustment for your system.

## Authorship

Algorithm implementation by Andrea Medda. DCEL library by Alessandro Muntoni and Andreas Scalas (GPL, 2016). GUI framework shell by Marco Livesu.
