/*Tanto diagrama.h quanto diagramaview.h são utilizados para fornecer uma interface para que as figuras geometricas
utilizadas nessa ferramenta sejam criadas.*/

#ifndef DIAGRAMAVIEW_H
#define DIAGRAMAVIEW_H

#include <QtGui>

class DiagramaView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DiagramaView(QGraphicsScene * scene = 0, QWidget *parent = 0);
    explicit DiagramaView(QWidget *parent = 0);

    void setScrool(QSpinBox * scrool);
private:
    qreal antigoZoom;
protected:
    virtual void wheelEvent(QWheelEvent *event);
    QSpinBox * mScrool;

signals:
    void mudandoZoom(int zoomLevel);

public slots:
    void setarZoom(int zoomLevel);

};

#endif // DIAGRAMAVIEW_H
