#include "cardinalidade.h"

Cardinalidade::Cardinalidade(Poligono *item1, Poligono *item2, Ligacao *linha, QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    atualCardinalidade = "1 : 1";

    setLinha_Atual(linha);
    setItemA(item1);
    setItemB(item2);

    connect(item1, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
    connect(item2, SIGNAL(posicaoAlterada()), this, SLOT(atualizaPos()) );
}

void Cardinalidade::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QString temp = chamarTelaCardinalidade.mostrarCardinalidade();
    if ( temp != NULL )
    {
        setAtualCardinalidade(temp);
        atualizaPos();
    }
    event->accept();
}

void Cardinalidade::atualizaPos()
{
    setPos((linha_atual->line().x1()+((linha_atual->line().dx())/2)), ((linha_atual->line().y1())+(((linha_atual->line().dy()))/2)));
    setPlainText(QString("( "+ getAtualCardinalidade() +" )"));
}
