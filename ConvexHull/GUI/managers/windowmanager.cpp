#include "windowmanager.h"
#include "ui_windowmanager.h"

#include "GUI/mainwindow.h"

WindowManager::WindowManager(QWidget *parent) : QFrame(parent), ui(new Ui::WindowManager), mainwindow((MainWindow*)parent) {
    ui->setupUi(this);
}

WindowManager::~WindowManager() {
    delete ui;
}

void WindowManager::on_backgroundColorButton_clicked() {
    QColor color = QColorDialog::getColor(Qt::white, this);

    mainwindow->setBackgroundColor(color);
    mainwindow->updateGlCanvas();
}

void WindowManager::on_saveSnapshotButton_clicked() {
    mainwindow->saveSnapshot();
}

void WindowManager::on_fullScreenCheckBox_stateChanged(int state) {
    mainwindow->setFullScreen(state== Qt::Checked);
}

void WindowManager::on_showAxisCheckBox_stateChanged(int state) {
    if (state == Qt::Checked) mainwindow->drawAxis(true);
    else mainwindow->drawAxis(false);
    mainwindow->updateGlCanvas();
}
