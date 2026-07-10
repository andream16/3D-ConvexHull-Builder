# Repository Overview

## Purpose

This repository implements a **3D Convex Hull Builder** — a desktop application that loads a 3D mesh, computes the minimum convex polyhedron (convex hull) enclosing all input vertices, and renders the result in an interactive OpenGL viewer.

It was developed as a project for the *Algorithms and Data Structures 2* university course (student: Andrea Medda, ID 65034).

## Technology Stack

- **Language:** C++11
- **GUI Framework:** Qt 5
- **Math Libraries:** Eigen3, Boost, CGAL
- **3D Rendering:** libQGLViewer (OpenGL-based)
- **Build System:** qmake (`.pro` file)

## Core Algorithm

The convex hull is computed using an **incremental construction algorithm with a conflict graph**:

1. **Load mesh** — Parse an OBJ/PLY/DCEL file and extract the vertex set.
2. **Build initial tetrahedron** — Select 4 non-coplanar points to form the starting polyhedron.
3. **Initialize conflict graph** — Track which unprocessed vertices can "see" which faces.
4. **Incremental insertion** — For each remaining vertex visible to at least one face:
   - Compute the horizon (boundary edges between visible and non-visible faces).
   - Delete all visible faces.
   - Build new triangular faces connecting the vertex to each horizon edge.
   - Update the conflict graph for the new faces.
5. **Output** — The final DCEL represents the convex hull, rendered in cyan in the viewer.

## Project Structure

```
ConvexHull/
├── convexhullbuilder.*          # Main algorithm orchestrator
├── conflictgraph.*              # Face–vertex conflict graph
├── tetrahedronbuilder.*         # Initial tetrahedron construction
├── facebuilderhelper.*          # New triangular face creation
├── convexhullbuilderhelper.*    # Vertex extraction, horizon computation
├── common.*                     # Shared utilities
├── main.cpp                     # Application entry point
├── GUI/
│   ├── mainwindow.*             # Main application window
│   ├── glcanvas.*               # OpenGL rendering canvas
│   ├── managers/                # UI panel managers (DCEL, ConvexHull, Window)
│   └── objects/                 # Primitive shapes (sphere, cylinder)
└── lib/
    ├── dcel/                    # Doubly-Connected Edge List mesh library
    └── common/                  # Utility types (point, bounding box, timers)

models/                          # Sample OBJ meshes (bunny, cow, homer, sphere, etc.)
```

## How to Use

1. Build using Qt Creator or `qmake` + `make`.
2. Launch the application.
3. Click **Load Dcel** and select a model from the `models/` directory.
4. Click **Convex Hull Solver**, then either **Show Phases** (step-by-step) or **Convex Hull!** (run all at once).

## Data Structure

The mesh is stored in a **DCEL (Doubly-Connected Edge List)**, which provides efficient traversal of vertices, half-edges, and faces. The DCEL library (by Muntoni/Scalas) supports serialization, drawing, and iterators for adjacency queries.
