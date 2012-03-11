#include "diagramaview.h"

DiagramaView::DiagramaView(QWidget *parent) :
    QGraphicsView(parent), antigoZoom(100.0)
{
}

DiagramaView::DiagramaView(QGraphicsScene * scene, QWidget *parent) : QGraphicsView(scene, parent), antigoZoom(100.0)
{
}

void DiagramaView::setarZoom(int zoomLevel)
{
    //scale((100 + (zoomLevel - antigoZoom))/100.0,
    //      (100 + (zoomLevel - antigoZoom))/100.0);
    scale(((qreal)zoomLevel)/antigoZoom,
          ((qreal)zoomLevel)/antigoZoom);
    antigoZoom = zoomLevel;
}

void DiagramaView::wheelEvent(QWheelEvent *event)
{
    if( event->modifiers().testFlag(Qt::ControlModifier))
    {
        mScrool->setValue(mScrool->value()+(event->delta() / 10));
    }
}

void DiagramaView::setScrool(QSpinBox *scrool)
{
    mScrool = scrool;
}
