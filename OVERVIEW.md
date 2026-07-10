# Repository Purpose & Overview

## What This Repository Does

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application that computes and visualizes the **3D convex hull** of a given mesh — the smallest convex polyhedron enclosing all input vertices. It was built as a university project for an *Algorithms and Data Structures 2* course.

### Core Workflow

1. **Load** a 3D mesh (OBJ, PLY, or DCEL format) from the bundled `models/` directory
2. **Compute** the convex hull using an incremental conflict-graph algorithm
3. **Visualize** the result in an interactive OpenGL viewer, with optional step-by-step animation

---

## Tech Stack

| Component | Technology |
|-----------|------------|
| Language | C++11 |
| Build system | Qt `qmake` (`.pro` file) |
| GUI | Qt5 Widgets + OpenGL |
| 3D viewer | libQGLViewer |
| Linear algebra | Eigen3 |
| Containers/Tokenization | Boost (bimap, tokenizer) |
| Geometry (optional) | CGAL + GMP |
| Mesh representation | Custom DCEL (Doubly-Connected Edge List) |

---

## Algorithm

The convex hull solver implements an **incremental construction with a conflict graph**, a standard computational geometry algorithm:

1. **Extract vertices** from the loaded mesh
2. **Build initial tetrahedron** — pick 4 non-coplanar points (verified via a 4×4 determinant)
3. **Initialize conflict graph** — bidirectional mapping between faces and vertices visible to them
4. **Incremental insertion** — for each remaining vertex:
   - Find all hull faces visible to the vertex
   - Compute the **horizon** (boundary edges between visible and non-visible faces)
   - Delete visible faces
   - Create new triangular faces connecting the horizon to the vertex
   - Update the conflict graph

---

## Project Structure

```
/
├── README.md                         # Original project readme
├── OVERVIEW.md                       # This file
├── models/                           # Sample 3D meshes (OBJ format)
└── ConvexHull/                       # Application source
    ├── main.cpp                      # Entry point
    ├── 65034_medda_andrea.pro        # Qt build configuration
    ├── convexhullbuilder.*           # Algorithm orchestrator
    ├── conflictgraph.*               # Conflict graph data structure
    ├── tetrahedronbuilder.*          # Initial tetrahedron construction
    ├── facebuilderhelper.*           # New face creation
    ├── convexhullbuilderhelper.*     # Horizon computation & helpers
    ├── GUI/
    │   ├── mainwindow.*              # Main window with tool panels
    │   ├── glcanvas.*                # OpenGL canvas (QGLViewer)
    │   └── managers/
    │       ├── dcelmanager.*         # Mesh load/save/render
    │       ├── convexhullmanager.*   # Hull solver UI controls
    │       └── windowmanager.*       # Viewport settings
    └── lib/
        ├── dcel/                     # Full DCEL mesh library
        └── common/                   # Utilities (Point, BoundingBox, Timer)
```

---

## Key Features

- **Interactive 3D viewer** — rotate, zoom, pan; wireframe/flat/smooth shading modes
- **Step-by-step visualization** — watch each phase of the algorithm execute
- **Multiple mesh formats** — OBJ, PLY, and native DCEL I/O
- **Bundled sample models** — bunny, sphere, cow, homer, cylinder, and more
- **Execution timer** — measures algorithm performance
- **Reusable DCEL library** — full mesh data structure with topological navigation and iterators

---

## Building & Running

**Prerequisites:** gcc/g++ ~6.2.1, Qt ~5.7, libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev, Eigen3

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
./65034_medda_andrea
```

Then: *Load Dcel* → select a model → *Convex Hull Solver* → *Convex Hull!* (or enable *Show Phases* first).

---

## Credits

- **Algorithm & Application:** Andrea Medda
- **DCEL Library:** Alessandro Muntoni, Andreas Scalas
- **GUI Framework Shell:** Marco Livesu
