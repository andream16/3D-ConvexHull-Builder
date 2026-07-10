# 3D Convex Hull Builder — Repository Overview

This repository is a **C++11 / Qt5 desktop application** that computes the **3D convex hull** of a loaded mesh and visualizes the result in an interactive OpenGL viewer. It was developed as a project for an *Algorithms and Data Structures 2* university course by Andrea Medda.

## What It Does

1. **Loads a 3D mesh** from disk (`.obj`, `.ply`, or `.dcel` format)
2. **Extracts vertex positions** from the mesh as a point set
3. **Runs an incremental 3D convex hull algorithm** using a conflict graph
4. **Displays the convex hull** alongside the original mesh in a 3D OpenGL viewer

The convex hull is the smallest convex polyhedron that contains every vertex in the input mesh. The algorithm constructs this shape and stores it as a DCEL (Doubly-Connected Edge List) mesh.

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, forms, OpenGL integration) |
| Eigen3 | Linear algebra (determinants, orientation tests) |
| Boost | `bimap` for object/checkbox mapping in the UI |
| CGAL | Optional DCEL and triangulation features |
| libQGLViewer | 3D OpenGL canvas |
| GMP | Arbitrary-precision arithmetic (required by CGAL) |

## Project Structure

```
├── README.md                          # Original project readme
├── models/                            # Sample 3D meshes (.obj)
│   ├── bunny.obj, cow.obj, homer.obj, sphere.obj, ...
└── ConvexHull/                        # All source code
    ├── main.cpp                       # Application entry point
    ├── 65034_medda_andrea.pro         # Qt qmake build file
    ├── convexhullbuilder.*            # Main algorithm orchestrator
    ├── tetrahedronbuilder.*           # Initial tetrahedron construction
    ├── conflictgraph.*                # Vertex–face conflict tracking
    ├── facebuilderhelper.*            # New face creation from horizon edges
    ├── convexhullbuilderhelper.*      # Horizon computation, vertex extraction
    ├── GUI/
    │   ├── mainwindow.*               # Main window and manager toolbox
    │   ├── glcanvas.*                 # QGLViewer-based 3D canvas
    │   └── managers/
    │       ├── dcelmanager.*          # Load/save/render meshes
    │       ├── convexhullmanager.*    # Run convex hull algorithm
    │       └── windowmanager.*        # Window and view controls
    └── lib/
        ├── dcel/                      # DCEL mesh data structure library
        └── common/                    # Utilities (point, bounding box, timers)
```

## Algorithm

The implementation follows an **incremental convex hull** approach with a **conflict graph**:

1. **Initialize** — Select 4 non-coplanar vertices and construct a starting tetrahedron.
2. **Build conflict graph** — For each remaining vertex, record which tetrahedron faces it can "see" (i.e., it lies in the outer half-space of the face).
3. **Iterate** — For each remaining vertex that sees at least one face:
   - Compute the **horizon**: the boundary edges between visible and non-visible faces.
   - Delete all visible faces from the DCEL.
   - Create new triangular faces connecting the horizon edges to the inserted vertex.
   - Update the conflict graph for the newly created faces.
4. **Result** — The DCEL now represents the convex hull of all input vertices.

An optional **Show Phases** mode refreshes the 3D view after each vertex insertion for step-by-step visualization.

### Key Classes

| Class | Responsibility |
|---|---|
| `ConvexHullBuilder` | Orchestrates the full algorithm loop |
| `TetrahedronBuilder` | Shuffles vertices and builds the initial tetrahedron |
| `ConflictGraph` | Maintains the face ↔ vertex visibility mapping |
| `ConvexHullBuilderHelper` | Extracts vertices from the mesh and computes horizon edges |
| `FaceBuilderHelper` | Creates new triangular DCEL faces from horizon edges and the new vertex |

## Building and Running

### Prerequisites

- GCC/G++ with C++11 support
- Qt 5 (core, gui, widgets, opengl, xml modules)
- System libraries: `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`, Eigen3 headers

### Build

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

> **Note:** The `.pro` file contains hardcoded library paths that may need editing for your system.

### Usage

1. Launch the built executable.
2. In the **Dcel** panel, click **Load Dcel** and select a model (e.g., `models/bunny.obj`).
3. Switch to the **Convex Hull Solver** panel.
4. Optionally check **Show Phases** for step-by-step visualization.
5. Click **Convex Hull!** to compute and display the result.

## Credits

- **Convex hull algorithm**: Andrea Medda
- **DCEL library**: Alessandro Muntoni and Stefano Nuvoli
- **GUI framework**: Marco Livesu (MainWindow / GLcanvas)
