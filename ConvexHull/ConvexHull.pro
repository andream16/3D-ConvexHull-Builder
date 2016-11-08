QT += core gui opengl xml widgets

CONFIG(debug, debug|release){
    message(debug)
    DEFINES += DEBUG
}

unix:!macx{
    LIBS += /usr/lib/x86_64-linux-gnu/libGLU.so
    LIBS += -lboost_system -frounding-math -lgmp -lQGLViewer -lCGAL
    LIBS += \
       -lboost_system\


    DEFINES += CGAL_DEFINED
    QMAKE_CXXFLAGS += -std=c++11 -g
}

macx{
    CONFIG += c++11
    DEFINES += CGAL_DEFINED
    INCLUDEPATH += -I /libs/include/boost/
    INCLUDEPATH += -I /libs/include/CGAL/
    INCLUDEPATH += /libs/frameworks/QGLViewer/QGLViewer.framework/Headers
    LIBS += -frounding-math
    LIBS += -L/libs/lib/gmp -lgmp
    LIBS += -F/libs/frameworks/QGLViewer -framework QGLViewer
    LIBS += -L/libs/lib/CGAL -lCGAL
}

HEADERS += \
    GUI/managers/dcelmanager.h \
    GUI/managers/convexhullmanager.h \
    GUI/managers/windowmanager.h \
    GUI/objects/cylinder.h \
    GUI/objects/sphere.h \
    GUI/glcanvas.h \
    GUI/mainwindow.h \
    lib/common/bounding_box.h \
    lib/common/comparators.h \
    lib/common/drawable_object.h \
    lib/common/point.h \
    lib/common/timer.h \
    lib/dcel/dcel.h \
    lib/dcel/dcel_face.h \
    lib/dcel/dcel_face_iterators.h \
    lib/dcel/dcel_half_edge.h \
    lib/dcel/dcel_iterators.h \
    lib/dcel/dcel_struct.h \
    lib/dcel/dcel_vertex.h \
    lib/dcel/dcel_vertex_iterators.h \
    lib/dcel/drawable_dcel.h \
    lib/common/common.h \
    common.h \
    lib/common/serialize.h \
    convexhullbuilder.h \
    conflictgraph.h \
    tetrahedronbuilder.h \
    facebuilderhelper.h \
    convexhullbuilderhelper.h

SOURCES += \
    GUI/managers/dcelmanager.cpp \
    GUI/managers/convexhullmanager.cpp \
    GUI/managers/windowmanager.cpp \
    GUI/glcanvas.cpp \
    GUI/mainwindow.cpp \
    lib/dcel/dcel_face.cpp \
    lib/dcel/dcel_half_edge.cpp \
    lib/dcel/dcel_vertex.cpp \
    lib/dcel/dcel_struct.cpp \
    lib/dcel/drawable_dcel.cpp \
    common.cpp \
    main.cpp \
    convexhullbuilder.cpp \
    conflictgraph.cpp \
    tetrahedronbuilder.cpp \
    facebuilderhelper.cpp \
    convexhullbuilderhelper.cpp

FORMS += \
    GUI/managers/dcelmanager.ui \
    GUI/managers/convexhullmanager.ui \
    GUI/managers/windowmanager.ui \
    GUI/mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

DISTFILES += \
    README.txt
