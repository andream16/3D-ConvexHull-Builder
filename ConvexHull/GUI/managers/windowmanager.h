#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QFrame>
#include <QColor>
#include <iostream>
#include <QColorDialog>
#include "GUI/mainwindow.h"

namespace Ui {
    class WindowManager;
}

class WindowManager : public QFrame {
        Q_OBJECT

    public:
        explicit WindowManager(QWidget *parent = 0);
        ~WindowManager();

    private slots:
        void on_backgroundColorButton_clicked();

        void on_saveSnapshotButton_clicked();

        void on_fullScreenCheckBox_stateChanged(int state);

        void on_showAxisCheckBox_stateChanged(int state);

    private:
        Ui::WindowManager* ui;
        MainWindow* mainwindow;
};

#endif // WINDOWMANAGER_H
