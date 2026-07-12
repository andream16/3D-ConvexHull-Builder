# 3D Convex Hull Builder

## Purpose

This is a **C++11 desktop application** that computes and visualizes the **3D convex hull** of a set of mesh vertices. Given a 3D model (loaded from OBJ, PLY, or DCEL format), it calculates the minimum convex polyhedron enclosing all input vertices and renders the result in an interactive OpenGL viewer.

The project was developed for the **Algorithms and Data Structures 2** university course.

## Algorithm

The core algorithm is an **incremental 3D convex hull** construction with a **conflict graph**:

1. **Load mesh** and extract all vertices.
2. **Build an initial tetrahedron** from four non-coplanar points (verified via a 4x4 Eigen determinant).
3. **Initialize a conflict graph** — a bidirectional mapping between faces and vertices, tracking which vertices are visible from each face and vice versa.
4. **Incrementally insert** each remaining vertex:
   - Identify all hull faces visible from the vertex (via the conflict graph).
   - Compute the **horizon** — the boundary edges of the visible face region.
   - Delete visible faces and create new triangular faces connecting horizon edges to the inserted vertex.
   - Update the conflict graph for the newly created faces.
5. The result is the convex hull mesh, rendered in cyan.

## Technologies

| Technology | Role |
|---|---|
| **C++11** | Primary language |
| **Qt 5** | GUI framework (widgets, file dialogs, UI forms) |
| **QGLViewer** | Interactive 3D OpenGL canvas and camera control |
| **Eigen3** | Linear algebra (determinant-based coplanarity checks) |
| **Boost** | Tokenizer for mesh file parsing, bimap for GUI object mapping |
| **CGAL** (optional) | Advanced polygon mesh operations in the DCEL library |

## Project Structure

```
ConvexHull/
├── main.cpp                        # Application entry point
├── convexhullbuilder.*             # Main algorithm orchestrator
├── convexhullbuilderhelper.*       # Vertex extraction, horizon computation
├── tetrahedronbuilder.*            # Initial tetrahedron construction
├── conflictgraph.*                 # Conflict graph data structure
├── facebuilderhelper.*             # New face creation during hull expansion
├── GUI/
│   ├── mainwindow.*                # Main window with manager toolbox
│   ├── glcanvas.*                  # QGLViewer-based OpenGL rendering
│   └── managers/
│       ├── dcelmanager.*           # Load/save/display input mesh
│       ├── convexhullmanager.*     # Trigger and display convex hull
│       └── windowmanager.*         # Scene settings (background, axis, etc.)
├── lib/
│   ├── common/                     # Shared utilities (point, bounding box, timers)
│   └── dcel/                       # Doubly-Connected Edge List mesh library
models/                             # Sample 3D meshes (bunny, cow, homer, sphere, etc.)
```

## Key Components

- **ConvexHullBuilder** — Orchestrates the full convex hull pipeline.
- **TetrahedronBuilder** — Selects four non-coplanar seed points and constructs the initial tetrahedron.
- **ConflictGraph** — Maintains bidirectional face-vertex visibility mappings; uses half-space tests to determine visibility.
- **FaceBuilderHelper** — Creates new triangular DCEL faces from horizon edges and the inserting vertex.
- **ConvexHullBuilderHelper** — Extracts vertices from the input mesh and computes horizon edges around visible face regions.
- **DCEL Library** — A full half-edge mesh data structure supporting vertices, half-edges, and faces with iterator-based adjacency navigation. Serves as both the input and output representation.

## Building

The project uses Qt's `qmake` build system:

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

Requires gcc/g++ (~6.2.1), Qt 5, libboost, libcgal, libgmp, and libqglviewer.

## Usage

1. Open the application.
2. Use the **Dcel** panel to load a mesh from `models/`.
3. Switch to the **Convex Hull Solver** panel.
4. Optionally enable **Show Phases** to step through algorithm iterations.
5. Click **Convex Hull!** to compute and display the result.

## Credits

- **Author:** Andrea Medda
- **DCEL Library:** Alessandro Muntoni, Andreas Scalas (2016)
- **GUI Shell:** Marco Livesu (2014)
