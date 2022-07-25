#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include<qlist.h>

class Scenez : public QGraphicsScene
{
public:
    enum Mode {NoMode, SelectObject, DrawLine};
    enum viewName {axial, notAxial};
    Scenez(QObject* parent = 0);
    void setSpacing(double PixelSpacing, double SliceThickness);
    void setView(viewName vname);
    void setMode(Mode mode);

     QString dist;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    int sceneHeight;
    int sceneWidth;
    int imageHeight;
    int imageWidth;
    double Slice_Thickness;
    double Pixel_Spacing;
    double space;
    Mode sceneMode;
    viewName currentView;
    QPointF origPoint;
    QPointF secOrigPoint;
    QGraphicsLineItem* itemToDraw;
    QGraphicsLineItem* secItemToDraw;
    int secSeg;
    void makeItemsControllable(bool areControllable);
};

#endif // SCENE_H
