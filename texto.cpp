#include "texto.h"

Texto::Texto(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsTextItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setTipoGenEsp("xt");
    setGenEspAtiva(false);
}

void Texto::focusOutEvent(QFocusEvent *event)
{
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

QVariant Texto::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}

void Texto::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //setTextInteractionFlags(Qt::TextEditorInteraction);
    painter->setBrush(Qt::blue);
    if ( isSelected() )
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));

    //painter->drawText(0,0, QString("R%1").arg(getCount()));
    QGraphicsTextItem::paint(painter, option, widget);
}

void Texto::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if ( !getGenEspAtiva() )
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
    }
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void Texto::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if ( getGenEspAtiva() )
    {
        QString temp = chamarTelaTipoGenEsp.mostrarTipoGenEsp();
        if ( temp != NULL )
        {
            setTipoGenEsp(temp);
            this->setPlainText(getTipoGenEsp());
        }
    }

    QGraphicsTextItem::mousePressEvent(event);
}
