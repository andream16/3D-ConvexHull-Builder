# 3D-ConvexHull-Builder
## C++ and QT5 Project for Algorithms and Data Structures 2 Course

This project takes a 3D model as input and build the minimun convex polygon that contains it (Convex Hull).

It was made using:

 - C++
 - QT5
 - Eigen3
 - LibBoost

# What you need to run the project:

 - `gcc` ~6.2.1
 - `g++` ~6.2.1
 - `Qt`  ~5.7

Also install:

 - `libboost-all-dev`
 - `libcgal-dev`
 - `libgmp-dev`
 - `libqglviewer-dev`

If you want to take a look at the documentation:

 - Install `doxygen`
 - Then, on `lib` folder: `doxygen dcel_doxygen_config_it`
 - Check `dcel_doc` folder to see the documentation

 There may be errors while running the project. If so, take care of correctly editing the `.pro`.

# How to launch the program

  - Click on `Load Dcel` and select a model from the model directory
  - Click on `Convex Hull Solver`
  - Then, you can choose if you want to see each process phase by clicking on `Show Phases` or just run it normally by clicking on `Convex Hull!`

## Input
![1.png](https://s22.postimg.org/4o2fu3kbl/1.png)
## Output
![2.png](https://s22.postimg.org/c5bn9b9up/2.png)
