# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application built for a university *Algorithms and Data Structures 2* course. It loads a 3D polygonal mesh, computes its **3D convex hull** (the smallest convex polyhedron enclosing all vertices), and renders both the original model and the result in an interactive OpenGL viewer.

## Algorithm

The core implements an **incremental convex hull** algorithm backed by a **conflict graph**:

1. **Extract vertices** from the loaded mesh.
2. **Build an initial tetrahedron** from four non-coplanar points (using Eigen determinants for orientation tests).
3. **Initialize a conflict graph** — a bidirectional mapping between remaining vertices and the tetrahedron faces they can "see."
4. **Iterate** over remaining vertices: for each visible vertex, compute the horizon (boundary edges of visible faces), remove visible faces, construct new triangular faces connecting the horizon to the vertex, and update the conflict graph.
5. **Output** the convex hull as a DCEL mesh, rendered in cyan in the viewer.

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, XML, OpenGL integration) |
| QGLViewer | Interactive 3D OpenGL canvas with camera controls |
| Eigen3 | Linear algebra for orientation and determinant checks |
| Boost | `bimap` for UI object/checkbox mapping |
| CGAL | Optional mesh utilities (when `CGAL_DEFINED` is set) |
| libgmp | Arbitrary-precision arithmetic required by CGAL |

The project builds with **qmake** via the `.pro` file and targets GCC/G++ on Linux (with partial macOS support).

## Project Structure

```
├── README.md                        # Original project readme
├── OVERVIEW.md                      # This file
├── models/                          # Sample .obj meshes (bunny, cow, homer, etc.)
└── ConvexHull/                      # All application source code
    ├── 65034_medda_andrea.pro       # Qt project / build file
    ├── main.cpp                     # Application entry point
    ├── common.h / common.cpp        # Shared globals
    ├── convexhullbuilder.*          # Algorithm orchestrator
    ├── tetrahedronbuilder.*         # Initial tetrahedron construction
    ├── conflictgraph.*              # Face–vertex conflict graph
    ├── facebuilderhelper.*          # New face creation from horizon edges
    ├── convexhullbuilderhelper.*    # Vertex extraction and horizon computation
    ├── GUI/
    │   ├── mainwindow.*             # Main window with toolbox managers
    │   ├── glcanvas.*               # QGLViewer-based OpenGL renderer
    │   ├── managers/
    │   │   ├── dcelmanager.*        # Mesh loading/saving/display
    │   │   ├── convexhullmanager.*  # Convex hull solver panel
    │   │   └── windowmanager.*      # Viewer settings (background, axes, etc.)
    │   └── objects/
    │       └── sphere.h, cylinder.h # Debug visualization primitives
    └── lib/
        ├── dcel/                    # Doubly-Connected Edge List mesh library
        │   ├── dcel.h, dcel_struct.*, dcel_vertex.*, dcel_half_edge.*, dcel_face.*
        │   └── drawable_dcel.*      # Renderable DCEL wrapper for OpenGL
        └── common/                  # Utilities (point, bounding box, timer, serialization)
```

## Key Components

- **ConvexHullBuilder** — orchestrates the full algorithm: vertex extraction, tetrahedron seeding, conflict graph construction, and incremental vertex insertion.
- **ConflictGraph** — maintains fast bidirectional lookups (via `unordered_map` / `unordered_set`) between faces and the vertices that can see them.
- **TetrahedronBuilder** — shuffles the vertex list, selects four non-coplanar points, and constructs the seed tetrahedron in the DCEL.
- **FaceBuilderHelper** — creates new triangular faces along the horizon boundary during each insertion step.
- **DCEL library** (`lib/dcel/`) — a full doubly-connected edge list implementation supporting vertices, half-edges, faces, polygon holes, and OBJ/PLY/DCEL I/O. Includes a `DrawableDcel` class bridging the data structure to OpenGL rendering.
- **GUI layer** — `MainWindow` hosts a `QToolBox` with three manager panels; `GLcanvas` renders all `DrawableObject` instances via QGLViewer.

## Usage

1. Open the application and click **Load Dcel** to select a model from the `models/` directory.
2. Switch to the **Convex Hull Solver** panel.
3. Click **Convex Hull!** to compute the hull, or enable **Show Phases** to step through each insertion visually.

## Authorship

- **Convex hull algorithm and integration:** Andrea Medda (student ID 65034)
- **DCEL library:** Alessandro Muntoni / Andreas Scalas
- **GUI shell:** Marco Livesu
