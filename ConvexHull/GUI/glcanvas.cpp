/**
 @author    Marco Livesu (marco.livesu@gmail.com)
 @copyright Marco Livesu 2014.
*/

#include "glcanvas.h"

GLcanvas::GLcanvas(QWidget * parent) : clearColor(Qt::white) {
    setParent(parent);
}

void GLcanvas::init() {
    setFPSIsDisplayed(true);
    camera()->frame()->setSpinningSensitivity(100.0);
}

void GLcanvas::clear() {
    drawlist.clear();
}

void GLcanvas::draw() {
    setBackgroundColor(clearColor);

    for(int i=0; i<(int)drawlist.size(); ++i)
    {
        drawlist[i]->draw();
    }
}

int GLcanvas::pushObj(const DrawableObject * obj) {
    drawlist.push_back(obj);
    updateGL();

    return drawlist.size();
}

void GLcanvas::deleteObj(const DrawableObject* obj) {
    std::vector<const DrawableObject *>::iterator it = std::find(drawlist.begin(), drawlist.end(), obj);
    if (it != drawlist.end()) drawlist.erase(it);

}

void GLcanvas::fitScene() {
    Pointd center(0,0,0);
    double radius = 0.0;
    int   count  = 0;

    if (drawlist.size() == 0) radius = 1.0;
    else {
        for(int i=0; i<(int)drawlist.size(); ++i) {
            const DrawableObject * obj = drawlist[i];
            if (obj->isVisible()) {
                center += obj->sceneCenter();
                radius = std::max(radius, obj->sceneRadius());
                ++count;
            }
        }
        if (count != 0)
            center /= (double)count;
        else radius = 1.0;
    }

    setSceneCenter(qglviewer::Vec(center.x(), center.y(), center.z()));
    setSceneRadius(radius/2);

    showEntireScene();       
}

void GLcanvas::setClearColor(const QColor &color) {
    clearColor = color;
    updateGL();
}

BoundingBox GLcanvas::getFullBoundingBox() {
    BoundingBox bb;
    for(int i=0; i<(int)drawlist.size(); ++i) {
        const DrawableObject * obj = drawlist[i];
        if (obj->isVisible()) {
            Pointd center = obj->sceneCenter();
            bb.setMin(bb.getMin().min(Pointd(center.x() - obj->sceneRadius(), center.y() - obj->sceneRadius(), center.z() - obj->sceneRadius())));
            bb.setMax(bb.getMax().max(Pointd(center.x() + obj->sceneRadius(), center.y() + obj->sceneRadius(), center.z() + obj->sceneRadius())));
        }
    }
    return bb;
}

int GLcanvas::getNumberVisibleObjects() {
    int count = 0;
    for(int i=0; i<(int)drawlist.size(); ++i) {
        const DrawableObject * obj = drawlist[i];
        if (obj->isVisible()) count++;
    }
    return count;
}
