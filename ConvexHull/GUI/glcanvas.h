/**
 @author    Marco Livesu (marco.livesu@gmail.com)
 @copyright Marco Livesu 2014.
*/

#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedCameraFrame.h>
#include <vector>

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include "lib/common/bounding_box.h"
#include "lib/common/drawable_object.h"

//using namespace std;

class GLcanvas : public QGLViewer {
    public:

        GLcanvas(QWidget * parent = nullptr);

        void init();
        void draw();
        void clear();
        void fitScene();
        void setClearColor(const QColor & color);
        BoundingBox getFullBoundingBox();
        int getNumberVisibleObjects();

        int  pushObj(const DrawableObject * obj);
        void deleteObj(const DrawableObject* obj);

    private:

        QColor clearColor;
        std::vector<const DrawableObject *> drawlist;
};

#endif // GLCANVAS_H
