# Repository Overview

## Purpose

This repository implements a **3D Convex Hull Builder** — a desktop application that computes the minimum convex polyhedron (convex hull) enclosing all vertices of a given 3D mesh. It was developed as a project for the "Algorithms and Data Structures 2" university course.

## Technology Stack

- **Language:** C++11
- **GUI Framework:** Qt 5 with libQGLViewer for 3D rendering
- **Math Libraries:** Eigen3, CGAL, Boost
- **Build System:** qmake (`.pro` file)

## Core Algorithm

The application uses an **incremental convex hull** algorithm with a conflict graph:

1. **Load mesh** — Reads a 3D model (OBJ/PLY/DCEL format) and extracts its vertices.
2. **Build initial tetrahedron** — Selects four non-coplanar points to form the starting polyhedron.
3. **Initialize conflict graph** — Maps each remaining vertex to the faces it can "see" (lies in front of).
4. **Incremental insertion** — For each unprocessed vertex visible to a face:
   - Compute the horizon (boundary edges between visible and non-visible faces).
   - Remove visible faces from the hull.
   - Construct new triangular faces connecting the vertex to the horizon edges.
   - Update the conflict graph for newly created faces.
5. **Output** — The resulting convex hull is stored as a DCEL mesh and rendered in the viewer.

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Face-vertex conflict graph
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New face creation utilities
├── convexhullbuilderhelper.*    # Horizon computation, vertex extraction
├── common.*                     # Shared constants/helpers
├── main.cpp                     # Application entry point
├── GUI/
│   ├── mainwindow.*             # Main application window
│   ├── glcanvas.*               # OpenGL rendering canvas
│   └── managers/                # UI panel controllers (DCEL, convex hull, window)
└── lib/
    ├── dcel/                    # Doubly-Connected Edge List mesh library
    └── common/                  # Utility classes (point, bounding box, timers)

models/                          # Sample 3D models (bunny, cow, homer, sphere, etc.)
```

## Data Structure

The mesh is represented using a **DCEL (Doubly-Connected Edge List)**, which stores vertices, half-edges, and faces with full adjacency information, allowing efficient traversal and modification during hull construction.

## Usage

1. Launch the application and click **Load Dcel** to select a 3D model from the `models/` directory.
2. Click **Convex Hull Solver** to open the solver panel.
3. Choose **Show Phases** for step-by-step visualization, or **Convex Hull!** to compute the result directly.

## Author

Andrea Medda (Student ID: 65034). The DCEL library was authored by Muntoni/Scalas, and the GUI shell by Marco Livesu.
