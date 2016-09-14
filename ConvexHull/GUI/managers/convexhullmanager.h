#ifndef CONVEXHULLMANAGER_H
#define CONVEXHULLMANAGER_H

#include <QFrame>
#include <iomanip>
#include "lib/dcel/drawable_dcel.h"
#include "GUI/mainwindow.h"
#include "common.h"
#include "dcelmanager.h"
#include "lib/common/timer.h"

// Add your includes here:

#include "convexhullbuilder.h"

// End includes

namespace Ui {
    class ConvexHullManager;
}

class ConvexHullManager : public QFrame
{
        Q_OBJECT

    public:
        explicit ConvexHullManager(QWidget *parent = 0);
        ~ConvexHullManager();

        void enableCHButton();
        void disableCHButton();

    private slots:
        void on_convexHullPushButton_clicked();

    private:
        Ui::ConvexHullManager *ui;
        MainWindow* mainWindow; //pointer to the mainWindow, which manages the canvas
        DrawableDcel* drawableDcel; //pointer to the DrawableDcel managed by this manager

        QFrame* dcelCHManager;
        int meshStackIndex; // id of this manager
};

#endif // CONVEXHULLMANAGER_H
