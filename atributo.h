/*Classe genérica utilizada para criar atributos e atributos identificadores.*/

#ifndef VERTICES_H
#define VERTICES_H

#include <QtGui>
#include <poligono.h>
#include <objetoremovivel.h>

class Atributo : public QObject, public QGraphicsEllipseItem, public ObjetoRemovivel
{
    Q_OBJECT

public:
    enum Tipo { atributo, atributo_identif };
    Atributo(Tipo tipo, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void setTipo(Tipo tipo) { this->tipo = tipo; }
    Tipo getTipo() { return tipo; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool getConectado() { return conectado; }
    void setConectado( bool c ) { this->conectado = c; }

    QList< Ligacao *> getLinhasAssociadas() { return linhaAssociada.toList(); }
    void addLinhaAssociada( Ligacao *la ) { this->linhaAssociada.insert(la); }
    void limparLinhasAssociadas() { this->linhaAssociada.clear(); }
    void removerLinhaAssociada(Ligacao * it) { linhaAssociada.remove(it); }

    // Garante tipo unico para objeto desse tipo
    enum { Type = UserType + 3 };
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

    virtual void doRemove(bool value);
    virtual QList<QGraphicsItem *> getToDelete();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void posicaoAlterada();

private:
    QRectF *circ;

    Tipo tipo;

    bool conectado;

    QSet< Ligacao * > linhaAssociada;
    QMap<Poligono::Tipo, QSet< Poligono * > > poligonosAssociados;
};

#endif // VERTICES_H
