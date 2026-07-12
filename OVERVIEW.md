# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a desktop application that computes the three-dimensional convex hull of an input mesh. Given a 3D model (`.obj`, `.ply`, or `.dcel` format), it calculates the minimum convex polyhedron enclosing all vertices and renders the result in an interactive OpenGL viewer. It was developed as a university project for an *Algorithms and Data Structures 2* course.

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, OpenGL module) |
| QGLViewer | Interactive 3D camera and rendering |
| Eigen3 | Linear algebra (determinants for coplanarity tests) |
| Boost | `bimap` for UI object mapping, `tokenizer` for file parsing |
| CGAL / GMP | Optional mesh utilities on Unix/macOS |
| qmake | Build system (`ConvexHull/65034_medda_andrea.pro`) |

## Architecture

The application is a single-process desktop program with three layers:

1. **GUI** (`ConvexHull/GUI/`) — Qt `MainWindow` with a `QToolBox` of manager panels and a `GLcanvas` powered by QGLViewer. Three panels exist: `WindowManager` (display settings), `DcelManager` (mesh I/O), and `ConvexHullManager` (algorithm control and timing).

2. **Algorithm** (`ConvexHull/`) — An incremental 3D convex hull algorithm using a conflict graph, implemented across several classes:
   - `ConvexHullBuilder` — orchestrates the full pipeline.
   - `TetrahedronBuilder` — selects four non-coplanar seed points and constructs the initial tetrahedron.
   - `ConflictGraph` — maintains bidirectional mappings between faces and the vertices visible from them.
   - `ConvexHullBuilderHelper` — extracts vertices from the mesh and computes the horizon (boundary between visible and hidden faces).
   - `FaceBuilderHelper` — constructs new triangular faces from horizon edges and the current vertex.

3. **Mesh Library** (`ConvexHull/lib/`) — A custom Doubly-Connected Edge List (DCEL) implementation by Muntoni and Scalas. Supports vertices, half-edges, and faces with full adjacency traversal, plus OBJ/PLY/DCEL serialization. `DrawableDcel` extends the DCEL with OpenGL rendering (wireframe, flat/smooth shading).

## Algorithm Summary

1. Load input mesh and extract all vertices.
2. Reset the DCEL and build an initial tetrahedron from four non-coplanar points.
3. Initialize the conflict graph: for each remaining vertex, determine which tetrahedron faces it can "see" (positive half-space test via dot product with the face normal).
4. For each remaining vertex with at least one visible face:
   - Compute the horizon — the boundary loop of half-edges separating visible faces from hidden ones.
   - Delete all visible faces from the DCEL.
   - Build new triangular faces connecting the horizon edges to the current vertex.
   - Update the conflict graph for the newly created faces.
5. The resulting DCEL represents the convex hull, rendered in cyan in the viewer.

An optional "Show Phases" mode refreshes the display after each vertex insertion for step-by-step visualization.

## Directory Layout

```
/
├── README.md                         # Project description and usage
├── models/                           # Sample meshes (bunny, cow, homer, sphere, etc.)
└── ConvexHull/
    ├── 65034_medda_andrea.pro        # qmake project file
    ├── main.cpp                      # Application entry point
    ├── convexhullbuilder.*           # Main algorithm orchestrator
    ├── conflictgraph.*               # Face-vertex conflict tracking
    ├── tetrahedronbuilder.*          # Initial tetrahedron construction
    ├── facebuilderhelper.*           # New face creation from horizon edges
    ├── convexhullbuilderhelper.*     # Vertex extraction, horizon computation
    ├── GUI/
    │   ├── mainwindow.*              # Main window shell
    │   ├── glcanvas.*                # QGLViewer wrapper
    │   ├── managers/                 # Panel controllers
    │   └── objects/                  # Debug draw helpers
    └── lib/
        ├── dcel/                     # DCEL mesh data structure library
        └── common/                   # Point, bounding box, timer utilities
```

## Authorship

- **DCEL library**: Alessandro Muntoni and Andreas Scalas (GPL v3)
- **GUI framework shell**: Marco Livesu
- **Convex hull algorithm**: Andrea Medda (student ID 65034)

## Building

Open `ConvexHull/65034_medda_andrea.pro` in Qt Creator, or run:

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

Library paths in the `.pro` file may need adjustment for your system. See `README.md` for required system packages.
