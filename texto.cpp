#include "texto.h"

Texto::Texto(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsTextItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setTipoGenEsp("xt");
    setGenEspAtiva(false);
    setTabelaLogicoAtiva(false);
}

void Texto::setGenEspAtiva( bool gea )
{
    this->genEspAtiva = gea;
//    setFlag(QGraphicsItem::ItemIsMovable, false);
//    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
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

void Texto::focusInEvent(QFocusEvent *event)
{
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    cursor.setPosition(Texto::toPlainText().length(), QTextCursor::MoveAnchor);
    setTextCursor(cursor);

    //Texto::textCursor().setPosition(Texto::toPlainText().length(), QTextCursor::MoveAnchor);
    // pq apenas isso não funciona?

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

    if (( !getGenEspAtiva() ) && ( !getTabelaLogicoAtiva() ))
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        this->setFocus(); // faz já começar a editar
    }

    QGraphicsTextItem::mouseReleaseEvent(event);
}

void Texto::setTextoTabelaLogico(QString txt, int pos, bool alterarRestricao, bool nulo)
{
    textoTabelaLogico.nome = txt;
    textoTabelaLogico.pos = pos;
    textoTabelaLogico.alterarRestricao = alterarRestricao;
    textoTabelaLogico.nulo = nulo;
}

void Texto::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //this->setFocus();

    if ( getGenEspAtiva() )
    {
        QString temp = chamarTelaTipoGenEsp.mostrarTipoGenEsp();
        if ( temp != NULL )
        {
            setTipoGenEsp(temp);
            this->setPlainText(getTipoGenEsp());
        }
    }

    if ( getTabelaLogicoAtiva() )
    {
        QPair<QString,bool> temp = chamarTelaLogico.alterarNomeLogico(getTextoTabelaLogico().nome, getTextoTabelaLogico().alterarRestricao, getTextoTabelaLogico().nulo);
        if ( temp.first != NULL )
        {
            setTextoTabelaLogico(temp.first, getTextoTabelaLogico().pos, getTextoTabelaLogico().alterarRestricao, temp.second );
            emit textoTabelaLogicoAlterado(getTextoTabelaLogico().pos);
            //this->setPlainText(getTextoTabelaLogico());
        }
    }

    QGraphicsTextItem::mousePressEvent(event);
}
