# Repository Overview

## Purpose

This repository implements a **3D Convex Hull Builder** — a desktop application that computes the minimum convex polyhedron (convex hull) enclosing all vertices of a given 3D mesh. It was developed as a university project for an "Algorithms and Data Structures 2" course.

## Technology Stack

- **Language:** C++11
- **GUI Framework:** Qt 5 with OpenGL (libQGLViewer)
- **Math Libraries:** Eigen3, CGAL, Boost
- **Build System:** qmake (`.pro` file)

## Core Algorithm

The application uses an **incremental convex hull construction** algorithm with a conflict graph:

1. **Load mesh** — Reads a 3D model file (OBJ, PLY, or DCEL format) and extracts all vertices.
2. **Build initial tetrahedron** — Selects 4 non-coplanar points from the vertex set to form a starting convex hull.
3. **Initialize conflict graph** — Records which vertices are visible from which faces (i.e., lie "in front of" them).
4. **Iterative insertion** — For each remaining vertex:
   - If the vertex sees one or more faces (is in conflict), compute the **horizon** (boundary edges between visible and non-visible faces).
   - Delete the visible faces from both the DCEL and the conflict graph.
   - Build new triangular faces connecting the horizon edges to the current vertex.
   - Update the conflict graph with the newly created faces.
5. **Result** — The final DCEL represents the convex hull, rendered interactively in the OpenGL viewer.

## Project Structure

| Directory / File | Description |
|---|---|
| `ConvexHull/convexhullbuilder.*` | Main algorithm orchestrator |
| `ConvexHull/conflictgraph.*` | Face–vertex conflict graph tracking |
| `ConvexHull/tetrahedronbuilder.*` | Initial tetrahedron construction |
| `ConvexHull/facebuilderhelper.*` | New triangular face creation during insertion |
| `ConvexHull/convexhullbuilderhelper.*` | Vertex extraction, horizon computation |
| `ConvexHull/GUI/` | Qt GUI: main window, OpenGL canvas, manager panels |
| `ConvexHull/lib/dcel/` | DCEL (Doubly-Connected Edge List) mesh library |
| `ConvexHull/lib/common/` | Utility types (point, bounding box, timers, serialization) |
| `models/` | Sample 3D models (bunny, cow, homer, sphere, etc.) |
| `ConvexHull/65034_medda_andrea.pro` | qmake project file |

## Data Structure

The mesh is stored using a **DCEL (Doubly-Connected Edge List)**, which provides efficient traversal of vertices, edges, and faces — essential for the horizon computation and face deletion/creation steps.

## How to Run

1. Install dependencies: Qt 5.7+, GCC 6+, libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev.
2. Open the `.pro` file in Qt Creator or run `qmake` followed by `make`.
3. Launch the application, load a model via "Load Dcel", then click "Convex Hull Solver" to compute the hull.
