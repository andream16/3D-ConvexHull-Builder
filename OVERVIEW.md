# 3D Convex Hull Builder

## Purpose

This repository implements a **3D incremental convex hull algorithm** as a desktop application for an "Algorithms and Data Structures 2" university course. It loads a 3D mesh (OBJ, PLY, or DCEL format), computes the smallest convex polyhedron enclosing all input vertices, and visualizes both the original mesh and resulting hull in an interactive OpenGL viewer.

**Author:** Andrea Medda (student ID 65034)  
**Libraries by:** Alessandro Muntoni, Andreas Scalas (DCEL), Marco Livesu (GUI framework)

## Technologies

| Category | Technology |
|----------|------------|
| Language | C++11 |
| Build system | Qt qmake |
| GUI | Qt 5 (widgets, opengl, xml) |
| 3D rendering | OpenGL via libQGLViewer |
| Linear algebra | Eigen3 |
| Computational geometry | CGAL |
| Math precision | GMP |
| Containers | Boost (bimap) |

## Algorithm

The convex hull is computed using an **incremental construction with a conflict graph**:

1. **Load mesh** — extract vertices from the input DCEL
2. **Build initial tetrahedron** — find 4 non-coplanar points using Eigen determinants
3. **Initialize conflict graph** — track which unprocessed vertices can "see" which faces
4. **Incremental insertion** — for each remaining vertex:
   - Find all faces visible from the vertex (via conflict graph)
   - Compute the horizon (boundary between visible and non-visible faces)
   - Remove visible faces and build new triangular faces from horizon edges to the vertex
   - Update the conflict graph for newly created faces
5. **Result** — the final DCEL represents the convex hull, rendered in cyan

## Repository Structure

```
/
├── README.md                        # Build/run instructions
├── OVERVIEW.md                      # This file
├── models/                          # Sample OBJ meshes (bunny, cow, sphere, etc.)
└── ConvexHull/                      # Application source
    ├── 65034_medda_andrea.pro       # Qt project file
    ├── main.cpp                     # Entry point
    │
    ├── convexhullbuilder.*          # Algorithm orchestrator
    ├── conflictgraph.*              # Face-vertex conflict tracking
    ├── tetrahedronbuilder.*         # Initial tetrahedron construction
    ├── facebuilderhelper.*          # New face creation from horizon edges
    ├── convexhullbuilderhelper.*    # Horizon computation, vertex extraction
    │
    ├── GUI/
    │   ├── mainwindow.*             # Main application window
    │   ├── glcanvas.*               # QGLViewer OpenGL canvas
    │   └── managers/                # UI panels (dcel, convexhull, window)
    │
    └── lib/
        ├── dcel/                    # Full DCEL mesh library
        └── common/                  # Shared utilities (Point, BoundingBox, etc.)
```

## Building

Requires: GCC/G++ ~6.2+, Qt ~5.7, libboost-all-dev, libcgal-dev, libgmp-dev, libqglviewer-dev, Eigen3.

```bash
cd ConvexHull
qmake 65034_medda_andrea.pro
make
```

Library paths in the `.pro` file may need adjustment for your system.

## Usage

1. Launch the application
2. Use **Load Dcel** to import a mesh from `models/`
3. Open the **Convex Hull Solver** panel
4. Optionally enable **Show Phases** for step-by-step visualization
5. Click **Convex Hull!** to compute and display the result
