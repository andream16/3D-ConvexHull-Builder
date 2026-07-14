# Repository Overview

## Purpose

**3D-ConvexHull-Builder** is a C++/Qt5 desktop application that computes the 3D convex hull of a loaded mesh. Given any 3D model, it calculates the minimum convex polyhedron containing all input vertices and renders the result in an interactive OpenGL viewer. The project was built for an Algorithms and Data Structures 2 university course.

## Core Algorithm

The application implements an **incremental convex hull algorithm with a conflict graph**, a well-known computational geometry approach:

1. **Tetrahedron seeding** — Four non-coplanar vertices are selected to form an initial tetrahedron (the simplest 3D convex hull).
2. **Conflict graph initialization** — A bidirectional mapping tracks which remaining vertices are visible from which hull faces.
3. **Incremental insertion** — Each remaining vertex is processed in turn:
   - Visible faces are identified via the conflict graph.
   - The **horizon** (boundary between visible and non-visible faces) is computed.
   - Visible faces are removed and new triangular faces are built from each horizon edge to the new vertex.
   - The conflict graph is updated for the new faces.
4. **Result** — The final DCEL mesh represents the convex hull, rendered in cyan in the viewer.

An optional **step-by-step visualization** mode refreshes the 3D view after each vertex insertion, showing the hull growing incrementally.

## Architecture

### Source Layout

| Directory / File | Role |
|---|---|
| `ConvexHull/main.cpp` | Application entry point |
| `ConvexHull/convexhullbuilder.*` | Algorithm orchestrator — drives the full pipeline |
| `ConvexHull/conflictgraph.*` | Conflict graph using Boost bimaps and hash maps |
| `ConvexHull/tetrahedronbuilder.*` | Initial tetrahedron construction with coplanarity checks (Eigen) |
| `ConvexHull/convexhullbuilderhelper.*` | Vertex extraction and horizon computation |
| `ConvexHull/facebuilderhelper.*` | New triangular face construction with proper DCEL wiring |
| `ConvexHull/GUI/` | Qt UI: main window, OpenGL canvas, manager panels |
| `ConvexHull/lib/dcel/` | Doubly-Connected Edge List mesh library (by Muntoni & Scalas) |
| `ConvexHull/lib/common/` | Utilities: 3D points, bounding boxes, timers, serialization |
| `models/` | Sample OBJ meshes for testing (bunny, cow, homer, sphere, etc.) |

### GUI Managers

The interface uses docked tool panels:

- **WindowManager** — Background color, fullscreen, axis display, screenshots.
- **DcelManager** — Load/save/clean meshes, wireframe and shading controls.
- **ConvexHullManager** — Run the hull algorithm, toggle phase visualization, display timing.

### Data Structures

- **DCEL (Doubly-Connected Edge List)** — The primary mesh representation, supporting vertices, half-edges, and faces with full adjacency navigation. Includes a `DrawableDcel` subclass for OpenGL rendering with wireframe, flat shading, and smooth shading modes.
- **Conflict Graph** — Two Boost `bimap`/`unordered_map` structures mapping faces to visible vertices and vertices to visible faces, enabling efficient visibility queries during hull construction.

## Technology Stack

| Technology | Purpose |
|---|---|
| C++11 | Core language |
| Qt 5 (core, gui, widgets, opengl, xml) | GUI framework |
| Eigen3 | Linear algebra (determinants, coplanarity checks) |
| Boost (bimap, unordered containers) | Data structures for conflict graph |
| CGAL | Optional mesh triangulation |
| libQGLViewer | Interactive 3D camera and viewer |
| OpenGL / GLU | 3D rendering |

## Build & Run

**Build system:** Qt `qmake` via `65034_medda_andrea.pro`.

**Prerequisites:** GCC/G++ ~6.2.1, Qt ~5.7, plus `libboost-all-dev`, `libcgal-dev`, `libgmp-dev`, `libqglviewer-dev`.

**Usage:**
1. Load a mesh via **Load Dcel** (select from `models/`).
2. Switch to the **Convex Hull Solver** tab.
3. Optionally enable **Show Phases** for step-by-step animation.
4. Click **Convex Hull!** to compute and display the result.
