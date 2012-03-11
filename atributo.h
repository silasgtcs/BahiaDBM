/*Classe genérica utilizada para criar atributos e atributos identificadores.*/

#ifndef VERTICES_H
#define VERTICES_H

#include <QtGui>
class Poligono;

class Atributo : public QObject, public QGraphicsEllipseItem
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
    QList< QGraphicsLineItem *> getLinhaAssociada() { return linhaAssociada; }
    void setLinhaAssociada( QGraphicsLineItem *la ) { this->linhaAssociada.push_back(la); }
    void RemoveTodasLinhasAssociadas() { this->linhaAssociada.erase(this->linhaAssociada.begin(), this->linhaAssociada.end()); }
    QList< Poligono *> getEntidades_Associadas() { return entidades_associadas; }
    void setEntidades_Associadas( Poligono *ea ) { this->entidades_associadas.push_back(ea); }
    void RemoveTodasEntidadesAssociadas() { this->entidades_associadas.erase(this->entidades_associadas.begin(), this->entidades_associadas.end()); }
    QList< Poligono *> getRelacionamentos_Associados() { return relacionamentos_associados; }
    void setRelacionamentos_Associados( Poligono *ra ) { this->relacionamentos_associados.push_back(ra); }
    void RemoveTodosRelacionamentosAssociados() { this->relacionamentos_associados.erase(this->relacionamentos_associados.begin(), this->relacionamentos_associados.end()); }
    QList< Poligono *> getEntidades_Associativas_Associadas() { return entidades_associativas_associadas; }
    void setEntidades_Associativas_Associadas( Poligono *eaa ) { this->entidades_associativas_associadas.push_back(eaa); }
    void RemoveTodasEntidadesAssociativasAssociadas() { this->entidades_associativas_associadas.erase(this->entidades_associativas_associadas.begin(), this->entidades_associativas_associadas.end()); }

    // Garante tipo unico para objeto desse tipo
    enum { Type = UserType + 3 };
    int type() const
    {
        return Type;
    }
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void posicaoAlterada();

private:
    QRectF *circ;

    Tipo tipo;

    bool conectado;

    QList< QGraphicsLineItem * > linhaAssociada;
    QList< Poligono * > entidades_associadas;
    QList< Poligono * > relacionamentos_associados;
    QList < Poligono *> entidades_associativas_associadas;
};

#endif // VERTICES_H
