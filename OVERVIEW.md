# 3D-ConvexHull-Builder

A C++11/Qt5 desktop application that computes and visualizes **3D convex hulls** — the smallest convex polyhedron enclosing a set of 3D points. Built as a university project for an Algorithms and Data Structures 2 course.

## What It Does

1. **Loads a 3D mesh** (OBJ, PLY, or DCEL format) from the `models/` directory or user-selected files
2. **Computes the 3D convex hull** using an incremental algorithm with a conflict graph
3. **Visualizes the result** in an interactive OpenGL viewer, with optional step-by-step animation of the hull construction

## Core Algorithm

The implementation follows the classic **incremental convex hull** approach:

- **Initial tetrahedron**: Finds 4 non-coplanar points and builds a seed tetrahedron, using a 4×4 determinant (Eigen) for coplanarity checks
- **Conflict graph**: Tracks face–vertex visibility using bidirectional maps (`std::unordered_map`/`unordered_set`) for O(1) lookups
- **Incremental insertion**: For each remaining vertex, computes the horizon (boundary between visible and non-visible faces), deletes visible faces, builds new triangular faces from horizon edges to the new vertex, and updates the conflict graph
- **Finalization**: Recomputes normals and renders the hull in cyan

## Data Structures

The mesh is represented by a **DCEL (Doubly-Connected Edge List)** library (`lib/dcel/`), which supports vertices, half-edges, faces with twin/prev/next pointers, iterators, and OpenGL rendering (wireframe, flat/smooth shading).

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, file dialogs, dock panels) |
| Eigen3 | Determinant computation for orientation tests |
| Boost | `bimap` for visibility maps, tokenizer for parsing |
| CGAL | Optional mesh triangulation |
| libQGLViewer | Interactive 3D camera and rendering |
| OpenGL / GLU | Mesh rendering |
| qmake | Build system (`.pro` file) |

## Project Structure

```
ConvexHull/
├── main.cpp                         # Entry point
├── convexhullbuilder.{h,cpp}        # Algorithm orchestrator
├── convexhullbuilderhelper.{h,cpp}  # Vertex extraction, horizon computation
├── tetrahedronbuilder.{h,cpp}       # Initial tetrahedron seed
├── conflictgraph.{h,cpp}            # Face–vertex conflict tracking
├── facebuilderhelper.{h,cpp}        # New face construction
├── lib/
│   ├── dcel/                        # DCEL mesh library
│   └── common/                      # Utilities (point, bounding box, timer)
└── GUI/
    ├── mainwindow.*                 # Main window
    ├── glcanvas.*                   # OpenGL canvas
    └── managers/                    # Dcel, ConvexHull, Window manager panels
models/                              # Sample OBJ meshes (bunny, cow, homer, etc.)
```

## Building

Requires GCC with C++11, Qt 5, Eigen3, Boost, CGAL, GMP, and libQGLViewer. Open `ConvexHull/65034_medda_andrea.pro` in Qt Creator or run:

```bash
cd ConvexHull
qmake
make
```

Library paths in the `.pro` file may need adjustment for your system.

## Authors

- **Andrea Medda** — convex hull algorithm implementation
- **Alessandro Muntoni & Andreas Scalas** — DCEL library
- **Marco Livesu** — GUI shell/framework
