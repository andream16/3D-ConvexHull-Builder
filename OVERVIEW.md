# 3D Convex Hull Builder

An interactive C++/Qt5 desktop application that computes and visualizes the **3D convex hull** of a loaded mesh. Built as an academic project for an *Algorithms and Data Structures 2* university course.

## What It Does

Given a 3D model (`.obj` file), the application:

1. **Loads** the mesh into a DCEL (Doubly-Connected Edge List) data structure
2. **Computes** the smallest convex polyhedron that encloses every vertex of the input mesh
3. **Visualizes** the result in an interactive OpenGL viewer, with optional step-by-step animation of the algorithm

## Algorithm

The project implements an **incremental 3D convex hull** algorithm with a **conflict graph**:

1. **Seed tetrahedron** — Select 4 non-coplanar points and build an initial tetrahedron (coplanarity checked via Eigen 4×4 determinant).
2. **Conflict graph initialization** — For every remaining vertex, record which faces of the current hull it can "see" (half-space test using face normals).
3. **Incremental insertion** — For each remaining vertex that lies outside the hull:
   - Compute the **horizon**: the boundary edges between visible and non-visible faces.
   - Delete all visible faces.
   - Create new triangular faces connecting the horizon edges to the new vertex.
   - Update the conflict graph for the newly created faces.
4. Vertices already inside the hull are skipped.

## Project Structure

```
ConvexHull/
├── main.cpp                      # Entry point
├── convexhullbuilder.*           # Algorithm orchestrator
├── tetrahedronbuilder.*          # Initial tetrahedron construction
├── conflictgraph.*               # Face–vertex conflict tracking
├── facebuilderhelper.*           # New face creation from horizon edges
├── convexhullbuilderhelper.*     # Vertex extraction and horizon computation
├── GUI/
│   ├── mainwindow.*              # Main window with docked panels
│   ├── glcanvas.*                # QGLViewer-based OpenGL canvas
│   └── managers/
│       ├── dcelmanager.*         # Load/save/display the input mesh
│       ├── convexhullmanager.*   # Run the hull algorithm and display results
│       └── windowmanager.*       # Window and scene controls
├── lib/
│   ├── dcel/                     # Doubly-Connected Edge List mesh library
│   └── common/                   # Shared utilities (Point, BoundingBox, etc.)
models/                           # Sample .obj meshes (bunny, cow, sphere, etc.)
```

## Technologies

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 | GUI framework (widgets, signals/slots) |
| QGLViewer | Interactive 3D OpenGL viewer |
| Eigen3 | Matrix determinant for coplanarity checks |
| CGAL | Optional mesh utilities |
| Boost | `bimap` for UI object tracking |
| GMP | Arbitrary-precision arithmetic (CGAL dependency) |

## Building

Requires GCC/G++, Qt 5, and the libraries listed above. Build with qmake:

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

Library paths in the `.pro` file may need adjustment for your system.

## Credits

- **Author:** Andrea Medda (student project)
- **DCEL library:** Alessandro Muntoni and Stefano Nuvoli
- **GUI framework:** Marco Livesu
