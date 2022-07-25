#include "scene.h"
#include <QToolTip>
#include <QtMath>





Scenez::Scenez(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    itemToDraw = 0;
    secItemToDraw = 0;
    secSeg = 0;
}

void Scenez::setMode(Mode mode){
    sceneMode = mode;
    QGraphicsView::DragMode vMode =
            QGraphicsView::NoDrag;
    if(mode == DrawLine){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if(mode == SelectObject){
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;
    }
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);

}
void Scenez::setView(Scenez::viewName vname){
    Scenez::currentView =vname;
}
void Scenez::setSpacing(double PixelSpacing, double SliceThickness){
    Scenez::Slice_Thickness = SliceThickness;
    Scenez::Pixel_Spacing = PixelSpacing;
}
void Scenez::makeItemsControllable(bool areControllable){
    foreach(QGraphicsItem* item, items()){
        item->setFlag(QGraphicsItem::ItemIsSelectable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemIsMovable,
                      areControllable);
    }
}

 // length measurements
void Scenez::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Scenez::sceneHeight = this->height();
    Scenez::sceneWidth =this->width();
    if(Scenez::currentView ==axial){
        Scenez::imageHeight = 600;
        Scenez::imageWidth= 450;
        Scenez::space = Scenez::Pixel_Spacing;
      }
     if(Scenez::currentView ==notAxial){
         Scenez::imageHeight = 450;
         Scenez::imageWidth= 450;
        Scenez::space = Scenez::Slice_Thickness;
    }


    if(sceneMode == DrawLine)
        origPoint = event->scenePos();
    QGraphicsScene::mousePressEvent(event);


}

void Scenez::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine){
        if(!itemToDraw){
            itemToDraw = new QGraphicsLineItem;

            this->addItem(itemToDraw);
            itemToDraw->setPen(QPen(Qt::red, 3, Qt::SolidLine)); 
            itemToDraw->setPos(origPoint);
        }
        itemToDraw->setLine(0,0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y());

        float distance =  Scenez::space*qSqrt(qPow((event->scenePos().x() - origPoint.x())* Scenez::imageWidth/ Scenez::sceneWidth,2)
                               + qPow((event->scenePos().y() - origPoint.y())* Scenez::imageHeight/ Scenez::sceneHeight,2));
        dist = QString::number(distance, 'f', 2);
        itemToDraw->setToolTip(dist);
        //itemToDraw->setToolTipDuration(500);

    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void Scenez::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
	
	
        //QMessageBox kk;
        //kk.setText("The Length is " + dist);
        //kk.exec();
	
    itemToDraw = 0;
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scenez::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            removeItem(item);
            delete item;
        }
    else
        QGraphicsScene::keyPressEvent(event);
}


