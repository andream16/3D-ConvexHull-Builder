# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application that computes and visualizes the **3D convex hull** of a loaded mesh. Built as a university project for an Algorithms and Data Structures 2 course, it lets users load a 3D model, run an incremental convex hull algorithm on its vertices, and interactively view the result in an OpenGL-based viewer.

## How It Works

1. **Load a mesh** — Supports `.obj`, `.ply`, and `.dcel` file formats (sample models included in `models/`).
2. **Compute the convex hull** — Uses a classic incremental algorithm with a conflict graph:
   - Extract vertices from the input mesh.
   - Construct an initial tetrahedron from four non-coplanar points.
   - Initialize a conflict graph mapping vertices to visible faces.
   - Iteratively add each remaining vertex: compute the horizon of visible faces, remove them, create new triangular faces connecting the horizon to the vertex, and update the conflict graph.
3. **Visualize the result** — The convex hull is rendered as a cyan mesh overlay in an interactive 3D viewer with camera controls.

Users can optionally enable **Show Phases** to step through the algorithm visually.

## Technology Stack

| Component | Technology |
|---|---|
| Language | C++11 |
| GUI | Qt 5 (widgets, forms) |
| 3D Rendering | QGLViewer + OpenGL |
| Linear Algebra | Eigen3 |
| Parsing / Utilities | Boost (tokenizer, bimap) |
| Geometry (optional) | CGAL + GMP |
| Build System | qmake |

## Project Structure

```
ConvexHull/
├── main.cpp                        # Application entry point
├── convexhullbuilder.*             # Core convex hull algorithm orchestrator
├── conflictgraph.*                 # Vertex-face conflict graph
├── tetrahedronbuilder.*            # Initial tetrahedron construction
├── facebuilderhelper.*             # New face creation at the horizon
├── convexhullbuilderhelper.*       # Vertex extraction and horizon computation
├── GUI/
│   ├── mainwindow.*                # Main application window
│   ├── glcanvas.*                  # OpenGL 3D canvas (QGLViewer)
│   └── managers/
│       ├── dcelmanager.*           # Mesh I/O and display controls
│       ├── convexhullmanager.*     # Convex hull execution and timing
│       └── windowmanager.*         # Window settings (background, axes, etc.)
└── lib/
    ├── dcel/                       # Doubly-Connected Edge List mesh library
    └── common/                     # Shared utilities (Point, BoundingBox, timers)

models/                             # Sample OBJ meshes (bunny, cow, sphere, etc.)
```

## Key Data Structure

The project uses a **DCEL (Doubly-Connected Edge List)** — a half-edge mesh data structure that represents vertices, half-edges, and faces with full adjacency information. This enables efficient traversal for the convex hull algorithm (e.g., finding horizon edges and neighboring faces). The DCEL library was authored by Alessandro Muntoni and Andreas Scalas under the GNU GPL.

## Building and Running

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt install qt5-default libboost-all-dev libcgal-dev libgmp-dev libqglviewer-dev

# Build
cd ConvexHull
qmake 65034_medda_andrea.pro
make

# Run the application, then:
# 1. Click "Load Dcel" and select a model from models/
# 2. Click "Convex Hull Solver"
# 3. Click "Convex Hull!" (or enable "Show Phases" for step-by-step)
```

## Credits

- **Convex hull implementation**: Andrea Medda (student project)
- **GUI framework**: Marco Livesu
- **DCEL library**: Alessandro Muntoni & Andreas Scalas (GNU GPL)
