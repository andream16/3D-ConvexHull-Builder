# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application that computes and visualizes the 3D convex hull of a polygonal mesh. Given a 3D model as input, it calculates the smallest convex polyhedron that contains all of the model's vertices and renders the result in an interactive OpenGL viewer. The project was developed for the "Algorithms and Data Structures 2" university course.

## Technology Stack

| Layer | Technology |
|-------|------------|
| Language | C++11 |
| GUI framework | Qt 5 (core, gui, opengl, xml, widgets) |
| 3D viewer | libQGLViewer (Qt5 build) |
| Linear algebra | Eigen3 |
| Utilities | Boost (tokenizer, bimap) |
| Geometry (optional) | CGAL, libgmp |
| Build system | qmake |

## Project Structure

```
/
├── README.md                    # Original project readme
├── OVERVIEW.md                  # This file
├── models/                      # Sample OBJ meshes (bunny, cow, sphere, etc.)
└── ConvexHull/                  # Application source code
    ├── main.cpp                 # Entry point — boots Qt event loop
    ├── 65034_medda_andrea.pro   # qmake build configuration
    ├── convexhullbuilder.*      # Core algorithm orchestrator
    ├── conflictgraph.*          # Vertex–face conflict tracking
    ├── tetrahedronbuilder.*     # Initial tetrahedron construction
    ├── facebuilderhelper.*      # New face creation from horizon edges
    ├── convexhullbuilderhelper.*# Horizon computation and vertex utilities
    ├── common.*                 # Global manager IDs
    ├── GUI/
    │   ├── mainwindow.*         # Main application window
    │   ├── glcanvas.*           # OpenGL rendering canvas (QGLViewer)
    │   ├── managers/            # Dcel, ConvexHull, and Window managers
    │   └── objects/             # Debug primitives (sphere, cylinder)
    └── lib/
        ├── dcel/                # Doubly-Connected Edge List mesh library
        └── common/              # Shared utilities (point, bounding box, timers)
```

## Algorithm

The convex hull is computed using an **incremental algorithm with a conflict graph**:

1. **Initialization** — Four non-coplanar vertices are selected (after random shuffling) to form an initial tetrahedron.
2. **Conflict graph construction** — A bipartite mapping is built between remaining vertices and tetrahedron faces, recording which vertices are "visible" from (in the positive half-space of) each face.
3. **Incremental insertion** — For each remaining vertex that has visible faces:
   - The **horizon** (boundary edge loop of visible faces) is computed.
   - Visible faces are deleted from the DCEL.
   - New triangular faces are created connecting the horizon edges to the current vertex.
   - The conflict graph is updated for the new faces.
4. **Cleanup** — Processed vertices are removed and the resulting convex hull mesh is returned.

## Key Components

- **ConvexHullBuilder** — Orchestrates the full pipeline from vertex extraction through incremental construction.
- **TetrahedronBuilder** — Constructs the seed tetrahedron using Eigen determinant checks for non-coplanarity.
- **ConflictGraph** — Maintains the vertex-to-face and face-to-vertex visibility mappings that drive the incremental step.
- **DCEL library** (`lib/dcel/`) — A full Doubly-Connected Edge List implementation supporting OBJ/PLY/DCEL file I/O, constant-time insert/delete, and adjacency iterators.
- **GUI managers** — `DcelManager` handles mesh loading and display settings; `ConvexHullManager` runs the algorithm and displays timing; `WindowManager` controls view settings.

## Building and Running

### Prerequisites

- GCC/G++ with C++11 support
- Qt 5 (~5.7+)
- libboost-all-dev
- libcgal-dev, libgmp-dev
- libqglviewer-dev (Qt5)
- Eigen3 headers

### Build

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

> Library paths in the `.pro` file may need adjustment for your system.

### Usage

1. Launch the compiled binary.
2. In the **Dcel** panel, click **Load Dcel** and select a model from `models/` (e.g., `bunny.obj`).
3. Switch to the **Convex Hull Solver** panel.
4. Optionally enable **Show Phases** to visualize each incremental step.
5. Click **Convex Hull!** to compute. The result appears as a cyan-colored mesh with elapsed time displayed.

## Credits

- **Convex hull algorithm**: Andrea Medda
- **DCEL library**: Alessandro Muntoni and Andreas Scalas
- **GUI framework**: Marco Livesu
