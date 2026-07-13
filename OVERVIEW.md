# Repository Overview

## Purpose

This repository implements a **3D Convex Hull Builder** — a desktop application that loads a 3D mesh, computes the minimum convex polyhedron (convex hull) enclosing all input vertices, and visualizes the result in an interactive OpenGL viewer.

It was developed as a university project for the "Algorithms and Data Structures 2" course.

## Technology Stack

- **Language:** C++11
- **UI Framework:** Qt 5 with libQGLViewer for 3D rendering
- **Math Libraries:** Eigen3, CGAL, Boost
- **Data Structure:** DCEL (Doubly-Connected Edge List) for mesh representation
- **Build System:** qmake (`.pro` file)

## Algorithm

The convex hull is computed using an **incremental construction algorithm with a conflict graph**:

1. Load mesh and extract vertices, resetting the DCEL structure
2. Construct an initial tetrahedron from 4 non-coplanar points
3. Initialize a conflict graph mapping unprocessed vertices to visible faces
4. For each remaining vertex in the conflict graph:
   - Compute the horizon (boundary edges between visible and non-visible faces)
   - Delete all faces visible from the current vertex
   - Build new triangular faces connecting the vertex to horizon edges
   - Update the conflict graph for the new faces
5. The final DCEL contains the convex hull, rendered in cyan in the viewer

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*         # Main algorithm orchestrator
├── conflictgraph.*             # Conflict graph (face↔vertex visibility tracking)
├── tetrahedronbuilder.*        # Initial tetrahedron construction
├── facebuilderhelper.*         # New triangular face creation during expansion
├── convexhullbuilderhelper.*   # Vertex extraction, horizon computation
├── GUI/                        # Qt UI (main window, OpenGL canvas, managers)
├── lib/dcel/                   # Full DCEL mesh library (by Muntoni/Scalas)
└── lib/common/                 # Utilities (point, bounding box, timers)

models/                         # Sample OBJ files (bunny, cow, homer, sphere, etc.)
```

## Usage

1. Launch the application
2. Click **Load Dcel** and select a model from the `models/` directory
3. Click **Convex Hull Solver**
4. Choose **Show Phases** to step through the algorithm, or **Convex Hull!** to run it directly
