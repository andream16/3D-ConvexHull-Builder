# 3D Convex Hull Builder — Repository Overview

## Purpose

This is a **C++11 / Qt 5 desktop application** that computes and visualizes the **3D convex hull** of a loaded mesh. Given a 3D model (OBJ, PLY, or DCEL format), it finds the minimum convex polyhedron enclosing all input vertices and renders the result in an interactive OpenGL viewer.

The project was developed for an *Algorithms and Data Structures 2* university course (author: Andrea Medda).

## How It Works

1. **Load a mesh** via the *Dcel* panel (supports `.obj`, `.ply`, `.dcel`).
2. **Run the solver** from the *Convex Hull Solver* panel.
3. **View the result** — the convex hull renders in cyan in a separate viewer pane.

### Algorithm

The core algorithm is an **incremental convex hull with conflict graph**:

1. Extract all vertices from the input mesh and reset the DCEL.
2. Build an initial **tetrahedron** from four non-coplanar points (checked via an Eigen 4×4 determinant).
3. Initialize a **conflict graph** mapping faces to the vertices visible from each face and vice-versa.
4. For each remaining vertex:
   - Identify faces visible to the vertex.
   - Compute the **horizon** (boundary edges of the visible face set).
   - Delete visible faces and build new triangular faces connecting the horizon to the current vertex.
   - Update the conflict graph for the newly created faces.
5. Post-process: assign colors, recompute normals, render.

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 (core, gui, widgets, opengl, xml) | GUI framework |
| QGLViewer / OpenGL | 3D rendering and camera interaction |
| Eigen3 | Linear algebra (coplanarity determinants) |
| Boost | Tokenizer (file parsing), bimap (UI binding) |
| CGAL (optional) | Polygon triangulation for non-triangle meshes |
| GMP | Exact arithmetic (required by CGAL) |

**Build system:** Qt `.pro` file (`ConvexHull/65034_medda_andrea.pro`).

## Project Structure

```
├── README.md                        # Setup and usage instructions
├── models/                          # Sample .obj meshes (bunny, cow, sphere, etc.)
└── ConvexHull/                      # All application source code
    ├── 65034_medda_andrea.pro       # Qt project file
    ├── main.cpp                     # Entry point
    ├── convexhullbuilder.*          # Algorithm orchestrator
    ├── convexhullbuilderhelper.*    # Vertex extraction, horizon computation
    ├── tetrahedronbuilder.*         # Initial tetrahedron construction
    ├── conflictgraph.*              # Face ↔ vertex visibility tracking
    ├── facebuilderhelper.*          # New face / half-edge creation
    ├── GUI/
    │   ├── mainwindow.*             # Main window with toolbox panels
    │   ├── glcanvas.*               # QGLViewer OpenGL canvas
    │   └── managers/
    │       ├── dcelmanager.*        # Load/save/display input mesh
    │       ├── convexhullmanager.*  # Trigger algorithm, show timing
    │       └── windowmanager.*      # Display settings (background, axis, etc.)
    └── lib/
        ├── common/                  # Utilities (Point, BoundingBox, Timer)
        └── dcel/                    # DCEL mesh library + drawable wrapper
```

## Key Components

- **ConvexHullBuilder** — orchestrates the full algorithm, calling into helper classes.
- **ConflictGraph** — maintains bidirectional visibility maps between faces and vertices; performs half-space tests.
- **TetrahedronBuilder** — shuffles vertices to find four non-coplanar points and creates the seed tetrahedron.
- **ConvexHullBuilderHelper** — extracts vertex coordinates and computes the ordered horizon.
- **FaceBuilderHelper** — constructs new DCEL faces from horizon edges and the current vertex.
- **DCEL library** (`lib/dcel/`) — half-edge mesh representation with vertices, half-edges, and faces; includes an OpenGL-drawable subclass.
- **GUI managers** — separate panels for mesh I/O, algorithm control, and display settings.

## Running

Prerequisites: `gcc`/`g++` ≥ 6.2, Qt ≥ 5.7, `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`.

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
./65034_medda_andrea
```

Load a mesh from `models/` and click **Convex Hull!** to compute.
