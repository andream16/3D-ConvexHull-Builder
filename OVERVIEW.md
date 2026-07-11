# 3D Convex Hull Builder — Repository Overview

## Purpose

This is a **C++11 / Qt5 desktop application** that computes and visualizes the **3D convex hull** of a polygonal mesh. It was developed as coursework for an *Algorithms and Data Structures 2* university course.

Given a 3D model (OBJ, PLY, or DCEL format), the program calculates the smallest convex polyhedron enclosing all input vertices and renders the result interactively in an OpenGL viewer.

## Algorithm

The core algorithm is an **incremental convex hull construction with a conflict graph**, a standard computational-geometry approach:

1. **Extract vertices** from the loaded mesh.
2. **Build an initial tetrahedron** from four non-coplanar points (coplanarity checked via Eigen determinants).
3. **Initialize a conflict graph** mapping each remaining vertex to the faces it can "see" and vice versa.
4. **Iterate** over each remaining vertex:
   - Identify visible faces from the conflict graph.
   - Compute the **horizon** — the boundary edges of the visible face region.
   - Delete visible faces and create new triangular faces connecting the horizon to the current vertex.
   - Update the conflict graph for newly created faces.
5. The resulting DCEL mesh is the convex hull, rendered in cyan with elapsed time displayed in the UI.

## Architecture

```
ConvexHull/
├── main.cpp                      # Application entry point
├── 65034_medda_andrea.pro        # qmake project file
├── convexhullbuilder.*           # Algorithm orchestrator
├── conflictgraph.*               # Face ↔ vertex conflict tracking
├── tetrahedronbuilder.*          # Initial tetrahedron construction
├── facebuilderhelper.*           # Triangular face / half-edge creation
├── convexhullbuilderhelper.*     # Vertex extraction, horizon computation
├── common.*                      # Global manager ID constants
├── GUI/
│   ├── mainwindow.*              # Central Qt window with toolbox panels
│   ├── glcanvas.*                # QGLViewer-based OpenGL canvas
│   ├── managers/
│   │   ├── dcelmanager.*         # Load / save / display meshes
│   │   ├── convexhullmanager.*   # Run the hull algorithm
│   │   └── windowmanager.*       # View settings (background, axes, etc.)
│   └── objects/                  # Debug primitives (spheres, cylinders)
└── lib/
    ├── dcel/                     # Full DCEL mesh library (vertices, half-edges, faces)
    └── common/                   # Utilities: Point3D, bounding box, timers, serialization
```

### Key components

| Component | Responsibility |
|---|---|
| `ConvexHullBuilder` | Orchestrates the full pipeline from mesh to hull |
| `TetrahedronBuilder` | Selects four non-coplanar seed points and builds the initial tetrahedron |
| `ConflictGraph` | Tracks which vertices are visible from which faces (and vice versa) |
| `ConvexHullBuilderHelper` | Extracts vertices from the DCEL, computes and orders horizon edges |
| `FaceBuilderHelper` | Creates new triangular faces with correct half-edge topology |
| `DrawableDcel` | Extends the DCEL data structure with OpenGL rendering |
| GUI managers | Handle mesh I/O, algorithm invocation, and viewer settings |

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, OpenGL integration) |
| QGLViewer | Interactive 3D OpenGL canvas |
| Eigen3 | Linear algebra (determinants, orientation tests) |
| Boost | `boost::bimap` used in the main window |
| CGAL + GMP | Optional mesh utilities |
| qmake | Build system |

## Building

### Prerequisites

- GCC/G++ with C++11 support
- Qt 5 (modules: core, gui, opengl, xml, widgets)
- libQGLViewer, Eigen3, Boost, CGAL, GMP

### Steps

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

> Library paths in the `.pro` file are hard-coded and may need adjustment for your system (Boost version, QGLViewer location, etc.).

## Usage

1. Launch the built binary.
2. In the **Dcel** tab, click **Load Dcel** and select a model from the `models/` directory (bunny, cow, sphere, homer, etc.).
3. Switch to the **Convex Hull Solver** tab.
4. Optionally check **Show Phases** to visualize each algorithmic step.
5. Click **Convex Hull!** — the result appears as a cyan mesh with timing information.

## Credits

- **Author:** Andrea Medda
- **DCEL library:** Alessandro Muntoni and Stefano Nuvoli (with Doxygen documentation)
- **GUI scaffolding:** Marco Livesu
