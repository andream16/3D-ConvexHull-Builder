# Repository Overview

## Purpose

This repository contains a **3D Convex Hull Builder** — a C++11 / Qt5 desktop application developed as a university project for an *Algorithms and Data Structures 2* course. Given a 3D mesh as input (OBJ, PLY, or DCEL format), it computes the **minimum convex polyhedron** (convex hull) enclosing all vertices and renders the result in an interactive OpenGL viewer.

## Algorithm

The implementation follows the **incremental convex hull** construction with a **conflict graph**:

1. **Extract vertices** from the loaded mesh and reset the DCEL structure.
2. **Build a seed tetrahedron** from four non-coplanar points (coplanarity checked via Eigen 4x4 determinants).
3. **Initialize a conflict graph** mapping each remaining vertex to the faces it can "see" (half-space test).
4. **Incrementally insert** each remaining vertex: identify visible faces, compute the horizon (boundary of the visible region), delete visible faces, create new triangular faces from the horizon to the inserted vertex, and update the conflict graph.
5. **Finalize** by coloring the hull, recomputing normals, and refreshing the viewer.

## Technology Stack

| Technology | Role |
|---|---|
| C++11 | Core language |
| Qt 5 (core, gui, widgets, opengl, xml) | Application framework and UI |
| QGLViewer (libQGLViewer-qt5) | Interactive 3D OpenGL canvas |
| Eigen3 | Matrix math for coplanarity / determinant checks |
| Boost | Tokenizer for OBJ/PLY parsing; bimap for UI state |
| CGAL (optional) | Advanced polygon triangulation when enabled |
| GMP | Exact arithmetic support for CGAL |

**Build system:** Qt `qmake` via `ConvexHull/65034_medda_andrea.pro`.

## Project Structure

```
ConvexHull/
├── main.cpp                        Application entry point
├── convexhullbuilder.*             Core algorithm orchestrator
├── convexhullbuilderhelper.*       Vertex extraction, horizon computation
├── conflictgraph.*                 Face-vertex visibility (conflict graph)
├── tetrahedronbuilder.*            Seed tetrahedron construction
├── facebuilderhelper.*             New triangular face creation
├── GUI/
│   ├── mainwindow.*                Main window with QToolBox of managers
│   ├── glcanvas.*                  QGLViewer-based OpenGL rendering
│   └── managers/
│       ├── dcelmanager.*           Load/save/display mesh
│       ├── convexhullmanager.*     Run convex hull, display timing
│       └── windowmanager.*         Viewer settings (background, axis, etc.)
├── lib/
│   ├── common/                     Shared utilities (Point, BoundingBox, Timer)
│   └── dcel/                       Doubly-Connected Edge List mesh library
│       ├── dcel_struct.*           Core DCEL: vertices, half-edges, faces
│       ├── drawable_dcel.*         DCEL + OpenGL rendering
│       └── *_iterators.h          Mesh traversal iterators
models/                             Sample 3D meshes (bunny, cow, homer, sphere, etc.)
```

## Key Design Decisions

- **DCEL as sole mesh representation** — the hull is built by directly manipulating half-edges, twins, and prev/next pointers, ensuring topologically valid output.
- **Separation of concerns** — course-provided infrastructure (DCEL library, GUI shell) lives in `lib/` and `GUI/`; the student-implemented convex hull logic occupies the top-level `ConvexHull/*.cpp` files.
- **Phase visualization** — enabling "Show Phases" refreshes the viewer after the seed tetrahedron and after each vertex insertion, making the incremental process visible step by step.

## Author

Andrea Medda (student ID 65034), with the DCEL library by Alessandro Muntoni and Andreas Scalas, and the GUI framework by Marco Livesu.
