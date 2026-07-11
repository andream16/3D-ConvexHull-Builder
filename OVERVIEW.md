# Repository Overview: 3D Convex Hull Builder

## Purpose

This repository is a **C++/Qt5 desktop application** that computes the **3D convex hull** of a loaded mesh. Built for an *Algorithms and Data Structures 2* university course, it implements the **incremental convex hull algorithm** with a **conflict graph** data structure and visualizes results in an interactive OpenGL viewer.

Given a 3D model as input, the program calculates the minimum convex polyhedron that contains all of its vertices — the convex hull.

## How It Works

### Algorithm

The application follows the **incremental convex hull** approach:

1. **Load mesh** — Import a 3D model (OBJ, PLY, or DCEL format) and extract its vertices
2. **Build initial tetrahedron** — Select 4 non-coplanar points (verified via Eigen determinant) to form the starting hull
3. **Initialize conflict graph** — Track which vertices are visible from which faces using half-space tests
4. **Incremental insertion** — For each remaining vertex:
   - Find all faces visible from the vertex
   - Compute the horizon (boundary between visible and non-visible faces)
   - Delete visible faces from the DCEL and conflict graph
   - Create new triangular faces connecting the horizon edges to the new vertex
   - Update the conflict graph for the new faces
5. **Finalize** — Color the hull cyan, recompute normals, and display alongside the original mesh

### User Workflow

1. Click **Load Dcel** and select a model from the `models/` directory
2. Open the **Convex Hull Solver** panel
3. Optionally enable **Show Phases** for step-by-step visualization
4. Click **Convex Hull!** to run the algorithm

## Project Structure

```
ConvexHull/
├── main.cpp                      # Application entry point
├── convexhullbuilder.*           # Main algorithm orchestrator
├── conflictgraph.*               # Conflict graph (face ↔ vertex visibility)
├── tetrahedronbuilder.*          # Initial tetrahedron construction
├── facebuilderhelper.*           # New face creation from horizon edges
├── convexhullbuilderhelper.*     # Vertex extraction & horizon computation
├── GUI/
│   ├── mainwindow.*              # Main window with toolbox panels
│   ├── glcanvas.*                # OpenGL 3D canvas (QGLViewer)
│   └── managers/
│       ├── dcelmanager.*         # Load/save/display meshes
│       ├── convexhullmanager.*   # Run convex hull solver
│       └── windowmanager.*       # View controls
└── lib/
    ├── dcel/                     # Doubly-Connected Edge List library
    └── common/                   # Utilities (Point, BoundingBox, Timer)

models/                           # Sample 3D meshes (bunny, cow, sphere, etc.)
```

## Technologies

| Technology | Role |
|---|---|
| **C++11** | Core language |
| **Qt 5** | GUI framework (widgets, OpenGL, XML) |
| **Eigen3** | Linear algebra (determinants, coplanarity checks) |
| **Boost** | `bimap` for UI mapping; `unordered_map/set` for conflict graph |
| **CGAL** | Optional mesh triangulation for non-triangle inputs |
| **libQGLViewer** | Interactive 3D OpenGL viewer |
| **GMP** | Required by CGAL on Linux |

## Key Data Structure: DCEL

The project uses a custom **Doubly-Connected Edge List** library to represent 3D meshes with vertices, directed half-edges (`twin`/`next`/`prev` pointers), and polygonal faces. This structure supports efficient local mesh navigation and incremental insertion/deletion — essential for the incremental convex hull algorithm.

## Building

The project uses **qmake** via `ConvexHull/65034_medda_andrea.pro`. Prerequisites:

- GCC/G++ ~6.2.1, Qt ~5.7
- System packages: `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`

Open the `.pro` file in Qt Creator or run `qmake` followed by `make`.

## Credits

- **Algorithm implementation**: Andrea Medda (student ID 65034)
- **DCEL library**: Alessandro Muntoni and Stefano Nuvoli
- **GUI framework**: Marco Livesu
