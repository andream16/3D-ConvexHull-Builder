# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application that computes the **3D convex hull** of a loaded 3D mesh and visualizes the result interactively via an OpenGL viewer. It was developed as an academic project for an *Algorithms and Data Structures 2* university course by Andrea Medda.

Given an arbitrary 3D model (e.g., a bunny, cow, or sculpture), the application extracts all vertex positions, discards the original face connectivity, and constructs the **smallest convex polyhedron** enclosing every vertex. The result is displayed alongside the original mesh in a 3D viewport.

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, signals/slots, file dialogs) |
| QGLViewer | Interactive OpenGL 3D camera and rendering |
| Eigen3 | Linear algebra (4×4 determinant for coplanarity tests) |
| Boost | `bimap`, tokenizer, system utilities |
| CGAL / GMP | Optional mesh triangulation and arbitrary-precision arithmetic |

Build system: Qt **qmake** via `ConvexHull/65034_medda_andrea.pro`.

## Core Algorithm

The implementation uses the classic **incremental convex hull with a conflict graph**, an expected O(n log n) approach:

1. **Extract vertices** from the loaded mesh and reset the DCEL (Doubly-Connected Edge List).
2. **Build an initial tetrahedron** from 4 non-coplanar points (verified via a 4×4 determinant). Points are shuffled randomly until a valid set is found.
3. **Initialize a conflict graph** — bidirectional maps tracking which vertices are visible from which hull faces, determined by half-space dot-product tests.
4. **Incrementally insert each remaining vertex:**
   - Query the conflict graph for faces visible from the vertex.
   - If none are visible, the vertex is already inside the hull — skip it.
   - Compute the **horizon**: the boundary loop of edges between visible and non-visible faces.
   - Delete all visible faces from the DCEL and conflict graph.
   - Stitch new triangular faces connecting each horizon edge to the new vertex.
   - Update the conflict graph for newly created faces.
5. **Finalize** — assign cyan coloring, recompute normals, and render the convex hull mesh.

## Project Structure

```
├── README.md                         # Original project readme
├── OVERVIEW.md                       # This file
├── models/                           # 12 sample OBJ meshes (bunny, cow, sphere, etc.)
└── ConvexHull/                       # Application source
    ├── 65034_medda_andrea.pro        # qmake build file
    ├── main.cpp                      # Entry point
    ├── convexhullbuilder.*           # Algorithm orchestrator
    ├── tetrahedronbuilder.*          # Initial tetrahedron construction
    ├── conflictgraph.*               # Conflict graph (face ↔ vertex visibility)
    ├── facebuilderhelper.*           # Triangle face/half-edge construction
    ├── convexhullbuilderhelper.*     # Vertex extraction, horizon detection
    ├── GUI/
    │   ├── mainwindow.*              # Main Qt window with toolbox panels
    │   ├── glcanvas.*                # QGLViewer OpenGL canvas
    │   └── managers/                 # UI panels (Dcel, ConvexHull, Window)
    └── lib/
        ├── dcel/                     # DCEL mesh library (vertices, half-edges, faces)
        └── common/                   # Utilities (Point, bounding box, serialization)
```

## Key Components

- **`ConvexHullBuilder`** — orchestrates the full algorithm: extracts vertices, delegates tetrahedron construction, runs the incremental loop, and finalizes the hull.
- **`TetrahedronBuilder`** — shuffles vertices and finds 4 non-coplanar points to form the seed tetrahedron, using Eigen for the determinant test.
- **`ConflictGraph`** — maintains two hash maps (`vertex → visible faces` and `face → visible vertices`) for efficient visibility queries and updates.
- **`ConvexHullBuilderHelper`** — detects horizon edges by walking half-edge twins across visible/non-visible face boundaries, then orders them into a closed loop.
- **`FaceBuilderHelper`** — creates new triangular DCEL faces from horizon edges and the inserted vertex, wiring up half-edge next/prev/twin pointers.
- **DCEL library** (`lib/dcel/`) — a full doubly-connected edge list implementation supporting OBJ/PLY/DCEL file I/O, iterators, and OpenGL rendering with wireframe and smooth shading.

## User Workflow

1. Launch the application and click **Load Dcel** to open a 3D model from the `models/` directory.
2. Switch to the **Convex Hull Solver** panel.
3. Optionally enable **Show Phases** to step through the algorithm visually.
4. Click **Convex Hull!** to compute and display the result.
5. The convex hull appears as a cyan mesh in the 3D viewer alongside the original model.
