/*Utilizada para criar a linha que liga os poligonos e atributos.*/

#ifndef LIGACOES_H
#define LIGACOES_H

#include <QtGui>
#include <atributo.h>
#include <texto.h>
#include <poligono.h>
#include <objetoremovivel.h>
#include <tabela.h>
class Cardinalidade;


class Ligacao : public QObject, public QGraphicsLineItem, public ObjetoRemovivel
{
    Q_OBJECT
public:
    Ligacao( QGraphicsItem *item1, QGraphicsItem *item2, QGraphicsItem *parent = 0 );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void setEntidadeFracaAtiva( bool set ) { this->entidadeFracaAtiva = set; }
    bool getEntidadeFracaAtiva () { return this->entidadeFracaAtiva; }

    QList< Cardinalidade *> getCardinalidades_Associadas() { return cardinalidades_associadas.toList(); }
    QList< Atributo *> getAtributoAssociado() { return atributo_associado.toList(); }

    void addCardinalidadeAssociada( Cardinalidade *ca ) { this->cardinalidades_associadas.insert(ca); }
    void addAtributoAssociado( Atributo *aa ) { this->atributo_associado.insert(aa); }

    void limparCardinalidadesAssociadas() { this->cardinalidades_associadas.clear(); }
    void limparAtributosAssociados() { this->atributo_associado.clear(); }

    void removerCardinalidadeAssociada(Cardinalidade * it) { cardinalidades_associadas.remove(it); }
    void removerAtributoAssociado(Atributo * it) { atributo_associado.remove(it); }

    Poligono * getCastItem1P() { return this->castItem1P; }
    Poligono * getCastItem2P() { return this->castItem2P; }
    Atributo * getCastItemA() { return this->castItemA; }
    Cardinalidade * getCardItem() { return this->cardItem; }
    Tabela *getCastItem1T() { return this->castItem1T; }
    Tabela *getCastItem2T() { return this->castItem2T; }

    QPair<Tabela *, Tabela *> getTabelasAssociadas() { return this->tabelasAssociadas; }

    // Garante tipo unico para objeto desse tipo
    enum { Type = UserType + 2 };
    int type() const
    {
        return Type;
    }

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

    QPainterPath shape() const;

    void conectarObjetos();
    void desconectarObjetos();
    virtual void doRemove(bool value);
    virtual QList<QGraphicsItem *> getToDelete();
private:
    Poligono *castItem1P;
    Poligono *castItem2P;

    Atributo *castItemA;

    Tabela *castItem1T;
    Tabela *castItem2T;

    Cardinalidade *cardItem;

    Texto *nomeLigacao1, *nomeLigacao2;

    QPointF temp;

    bool entidadeFracaAtiva;

    QPair<Tabela *, Tabela *> tabelasAssociadas;
    QSet< Cardinalidade * > cardinalidades_associadas;
    QSet< Atributo * > atributo_associado;
    QMap<Poligono::Tipo, QSet<Poligono *> > poligonosAssociados;
signals:
    void linhaAlterada( QLineF nova_linha );

public slots:
    void atualizaPos();
    void atualizaPosAutoRelacionamento();
};

#endif // LIGACOES_H
