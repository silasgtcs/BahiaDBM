/*Classe utilizada para criar cardinalidade. O objeto é chamado no momento em que a ligação é feita entre os poligonos.*/

#ifndef CARDINALIDADE_H
#define CARDINALIDADE_H

#include <QtGui>
#include <poligono.h>
#include <ligacao.h>
#include <dialog.h>

class Cardinalidade : public QGraphicsTextItem
{
    Q_OBJECT
public:
    Cardinalidade(Poligono *item1, Poligono *item2, Ligacao *linha, QGraphicsItem *parent = 0);
    void setItemA ( Poligono *item ) { this->itemA = item; }
    void setItemB ( Poligono *item ) { this->itemB = item; }

    void setLinhaAtual ( Ligacao *linha ) { this->linha_atual = linha; }
    QString getCardinalidadeAtual() { return this->atualCardinalidade; }
    void setCardinalidadeAtual( QString c ) { this->atualCardinalidade = c; }

    void removerPoligonoAssociado(Poligono * poligono) {
        poligonosAssociados[poligono->getTipo()].remove(poligono);
    }

    template<Poligono::Tipo T>
    void limparPoligonosOfType() {
        poligonosAssociados[T].clear();
    }

    void addPoligonoAssociado(Poligono * pol) {
        poligonosAssociados[pol->getTipo()].insert(pol);
    }

    template<Poligono::Tipo T>
    QList<Poligono *> getPoligonosAssociadoOfType() {
       return poligonosAssociados[T].toList();
    }

    enum { Type = UserType + 4 };
    int type() const {
        return Type;
    }


protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Poligono *itemA;
    Poligono *itemB;
    Ligacao *linha_atual;
    Dialog chamarTelaCardinalidade;
    QString atualCardinalidade;

    QMap<Poligono::Tipo, QSet<Poligono *> > poligonosAssociados;
public slots:
    void atualizaPos();
};

#endif // CARDINALIDADE_H
