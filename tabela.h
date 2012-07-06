#ifndef TABELA_H
#define TABELA_H

#include <QtGui>
#include "texto.h"

class Tabela : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Tabela( QString nomeTitulo, QGraphicsItem *parent = 0, QGraphicsScene *sceneConceitual = 0, QGraphicsScene *sceneLogico = 0 );

    void setTitulo(QString);
    QString getTitulo();
    void addAtributo(QString nome, bool chavePrimaria, bool chaveEstrangeira, bool nulo);
    void setMaiorLargura(int ml) { this->maiorLargura = ml; }
    int getMaiorLargura() { return this->maiorLargura; }
    void atualizarLargura();
    void atualizarNomeAtributo(QString, QString, int);
    void setQtdAtributo( int qtd ) { this->qtdAtributo = qtd; }
    int getQtdAtributo() { return this->qtdAtributo; }
    QList<Texto*> getListaAtributo() { return this->listaAtributo; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void alterarNomeConceitualELogico( QString antigo, QString novo );
    QString formataAtributo(QString nome);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void posicaoAlterada();

private:
    QRectF *tab;

    QGraphicsLineItem *linha;

    QGraphicsScene *sceneL;
    QGraphicsScene *sceneC;

    Texto *titulo;
    Texto *atributo;

    QString nomeTitulo;

    int maiorLargura;
    int qtdAtributo;

    QList<Texto *> listaAtributo;

public slots:
    void atualizarTitulo();
    void atualizarAtributo(int);
};

#endif // TABELA_H
