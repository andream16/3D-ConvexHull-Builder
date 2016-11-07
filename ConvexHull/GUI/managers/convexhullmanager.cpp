#include "convexhullmanager.h"
#include "ui_convexhullmanager.h"

ConvexHullManager::ConvexHullManager(QWidget *parent) : QFrame(parent), ui(new Ui::ConvexHullManager), mainWindow((MainWindow*)parent), drawableDcel(nullptr), dcelCHManager(nullptr) {
    ui->setupUi(this);
}

ConvexHullManager::~ConvexHullManager() {
    if (dcelCHManager != nullptr) delete dcelCHManager;
    delete ui;
}

/**
 * @brief ConvexHullManager::enableCHButton
 * Method called by the Dcel Manager when a dcel is loaded,
 * and an input for the Convex Hull is available.
 * Please do not call this method.
 */
void ConvexHullManager::enableCHButton() {
    ui->convexHullPushButton->setEnabled(true);
    ui->showPhasesCheckBox->setEnabled(true);
}

/**
 * @brief ConvexHullManager::enableCHButton
 * Method called by the Dcel Manager when a dcel is erased.
 * Please do not call this method.
 */
void ConvexHullManager::disableCHButton() {
    ui->convexHullPushButton->setEnabled(false);
    ui->showPhasesCheckBox->setEnabled(false);
}

/**
 * @brief ConvexHullManager::on_convexHullPushButton_clicked
 * This method is executed when the "Convex Hull" button is pushed.
 * You can edit only the portion of code dedicated to the execution of the convex hull algorithm.
 * The algorithm should modify the variable "dcel", which contains the input mesh, in a convex hull.
 * For the bonus point, you can check if the checkbox for the convex hull phase is checked by checking
 * "ui->showPhasesCheckBox->isChecked()". For this point, you will also need to use the variable mainwindow.
 */
void ConvexHullManager::on_convexHullPushButton_clicked() {
    DcelManager * manager =(DcelManager*) mainWindow->getManager(DCEL_MANAGER_ID);
    DrawableDcel * drawableDcel = manager->getDcel();
    if (drawableDcel != nullptr){
        if (DCEL_CH_MANAGER_ID == -1){
            // Creating a new Manager for the Convex Hull
            dcelCHManager = new DcelManager(mainWindow);
            DCEL_CH_MANAGER_ID = mainWindow->addManager(dcelCHManager, "Dcel Convex Hull");
            ((DcelManager*)dcelCHManager)->setDcel(*drawableDcel, "Convex Hull");
            // dcel contains the input mesh, and it will be our convex hull
            DrawableDcel * dcel = ((DcelManager*)dcelCHManager)->getDcel();
            dcel->setFlatShading();
            dcel->setEnableTriangleColor();

            Timer t("Convex Hull"); // timer

            /********************************
             * Begin Convex Hull Algorithm  *
             ********************************/
            ConvexHullBuilder chb(dcel, mainWindow);
            chb.computeConvexHull();

            /********************************
             * End Convex Hull Algorithm    *
             ********************************/

            t.stop_and_print();
            std::stringstream ss;
            ss << std::setprecision(std::numeric_limits<double>::digits10+1);
            ss << t.delay();
            ui->timeLabel->setText(ss.str().c_str());

            // Coloring Convex hull with cyano color
            for (Dcel::FaceIterator fit = dcel->faceBegin(); fit != dcel->faceEnd(); ++fit)
                (*fit)->setColor(QColor(0,255,255));
            dcel->updateBoundingBox();
            /***
             * Warning: updateFaceNormals() crashes if there is at least one half edge which doesn't have
             * prev or next fields setted properly.
             * These fields are necessary for the mesh rendering, therefore if this method crashes,
             * you cannot view the result until all next and prev are setted properly.
             *****/
            dcel->updateFaceNormals();
            /***
             * Warning: updateVertexNormals() crashes if there is at least one half edge which doesn't have
             * twin fields setted properly.
             * These fields are not strictly necessary for the mesh rendering, therefore
             * you can comment this instruction for debugging (you will see a mesh without lighting).
             * However, when you are submitting the project, this line must be decommented.
             * In other words: your convex hull must have all the twin fields setted properly.
             *****/
             dcel->updateVertexNormals();

            //Final operations to render the final dcel

            dcel->update();
            mainWindow->updateGlCanvas();
        }

    }
}
