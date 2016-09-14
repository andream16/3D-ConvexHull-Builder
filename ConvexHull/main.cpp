#include "GUI/mainwindow.h"
#include "GUI/managers/dcelmanager.h"
#include "GUI/managers/windowmanager.h"
#include "GUI/managers/convexhullmanager.h"
#include "common.h"
#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MainWindow gui;  // main window, contains the canvas of QGLViewer

    // Creating a new WindowManager that will be added to the main window
    WindowManager wm(&gui);
    WINDOW_MANAGER_ID = gui.addManager(&wm, "Window");

    // Creating a new DcelManager that will be added to the main window
    DcelManager d(&gui);
    DCEL_MANAGER_ID = gui.addManager(&d, "Dcel");
    gui.setCurrentIndexToolBox(DCEL_MANAGER_ID); // DcelManager will be focused on start

    // Creating a new ConvexHullManager that will be added to the main window
    ConvexHullManager ch(&gui);
    CONVEX_HULL_MANAGER_ID = gui.addManager(&ch, "Convex Hull Solver");

    gui.updateAndFit();
    gui.show();


    return app.exec();
}
