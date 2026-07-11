# Repository Overview: 3D Convex Hull Builder

## Purpose

This is a **C++11 / Qt5 desktop application** created for the *Algorithms and Data Structures 2* university course. It loads a 3D mesh, computes its **3D convex hull** (the smallest convex polyhedron that contains all input vertices), and visualizes both the original mesh and the result in an interactive OpenGL viewer.

## Core Algorithm

The project implements an **incremental convex hull construction with a conflict graph**, following these high-level steps:

1. **Extract vertices** from the input mesh (DCEL representation).
2. **Build an initial tetrahedron** from four non-coplanar points, using an Eigen 4×4 determinant for coplanarity testing.
3. **Initialize a conflict graph** — bidirectional maps tracking which vertices are visible from which faces.
4. **Iteratively expand the hull**: for each remaining vertex, compute the horizon (boundary between visible and non-visible faces), delete visible faces, build new triangular faces connecting the horizon to the vertex, and update the conflict graph.
5. **Output**: a valid convex hull mesh stored in a DCEL, rendered in the GUI.

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 (core, gui, widgets, opengl, xml) | GUI framework, file dialogs, UI forms |
| QGLViewer | 3D OpenGL canvas and camera controls |
| Eigen3 | Linear algebra (determinant-based coplanarity checks) |
| Boost (bimap, unordered containers) | Data structures |
| CGAL (optional) | Mesh triangulation, advanced DCEL operations |
| GMP | Required by CGAL |

## Project Structure

```
ConvexHull/
├── main.cpp                     # Application entry point
├── 65034_medda_andrea.pro       # Qt qmake build configuration
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Face–vertex conflict graph
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New triangular face creation
├── convexhullbuilderhelper.*    # Horizon detection, vertex extraction
├── GUI/
│   ├── mainwindow.*             # Main window with QGLViewer canvas
│   ├── glcanvas.*               # OpenGL rendering
│   └── managers/
│       ├── dcelmanager.*        # Load/save/visualize meshes
│       ├── convexhullmanager.*  # Run the convex hull solver
│       └── windowmanager.*      # Display settings
└── lib/
    ├── dcel/                    # Doubly-Connected Edge List library
    └── common/                  # Utilities (point, bounding box, timers)

models/                          # Sample OBJ meshes (bunny, cow, sphere, etc.)
```

## How to Build

```bash
# Install dependencies (Debian/Ubuntu)
sudo apt install gcc g++ qt5-default libboost-all-dev libcgal-dev libgmp-dev libqglviewer-dev

# Build
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

> **Note:** Library paths in the `.pro` file are hardcoded for specific library versions and may need adjustment on modern systems.

## How to Use

1. Launch the application.
2. Click **Load Dcel** and select a model from the `models/` directory.
3. Click the **Convex Hull Solver** tab.
4. Choose **Show Phases** to step through the algorithm visually, or **Convex Hull!** to run it in one pass.

## Authors

- **Andrea Medda** (student ID 65034) — convex hull algorithm and application integration
- **Alessandro Muntoni / Stefano Nuvoli** — DCEL library
- **Marco Livesu** — GUI shell
