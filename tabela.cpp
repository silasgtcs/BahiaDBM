#include "tabela.h"

Tabela::Tabela( QString nomeTitulo, QGraphicsItem *parent, QGraphicsScene *scene )
    : QGraphicsRectItem( parent, scene )
{
    setFlag(QGraphicsRectItem::ItemIsMovable, true);
    setFlag(QGraphicsRectItem::ItemIsSelectable, true);
    setFlag(QGraphicsRectItem::ItemSendsGeometryChanges, true);

    setQtdAtributo(0);
    this->scene = scene;
    tab = new QRectF(0,0,216,25);
    setRect(*tab);

    linha = this->scene->addLine(tab->x(),tab->y()+25,tab->x()+tab->width(),tab->y()+25);
    linha->setParentItem(this);

    titulo = new Texto();
    titulo->setParentItem(this);
    titulo->setTabelaLogicoAtiva(true);
    setTitulo(nomeTitulo);

    connect(titulo, SIGNAL(textoTabelaLogicoAlterado(int)), this, SLOT(atualizarTitulo()));
}

void Tabela::atualizarLargura()
{
    int maior=11;

    if ( getTitulo().size() > maior )
        maior = getTitulo().size();

    for ( int i=0; i<listaAtributo.size(); i++ )
        if ( listaAtributo[i]->toPlainText().size() > maior )
            maior = listaAtributo[i]->toPlainText().size();

    tab->setWidth(maior*8.5);
    setRect(*tab);

    linha->setLine(tab->x(),tab->y()+25,tab->x()+tab->width(),tab->y()+25);
    this->titulo->setPos(tab->x()+(tab->width()/2)-(this->nomeTitulo.size()*3.5),tab->y());
    setMaiorLargura(maior);
}

void Tabela::setTitulo(QString titulo)
{
    this->nomeTitulo = titulo;
    this->titulo->setPlainText(this->nomeTitulo);
    atualizarLargura();
    this->titulo->setPos(tab->x()+(tab->width()/2)-(this->nomeTitulo.size()*3.5),tab->y());
    this->titulo->setTextoTabelaLogico(titulo,-1);
    this->titulo->setCursor(Qt::PointingHandCursor);
}

void Tabela::atualizarNomeAtributo(QString novoNome, int pos)
{
    bool pk,fk,nulo;
    QString temp = listaAtributo[pos]->toPlainText();

    pk = (temp.contains("[PK]", Qt::CaseSensitive)) ? true : false;
    fk = (temp.contains("[FK]", Qt::CaseSensitive)) ? true : false;
    nulo = (temp.contains("NOT NULL", Qt::CaseSensitive)) ? false : true;

    if ( fk ) novoNome.insert(0, "[FK] ");
    if ( pk ) novoNome.insert(0, "[PK] ");
    if ( nulo ) novoNome.insert(novoNome.size(), " \"NULL\"");
    else novoNome.insert(novoNome.size(), " \"NOT NULL\"");

    listaAtributo[pos]->setPlainText(novoNome);
    atualizarLargura();
}

QString Tabela::getTitulo()
{
    return this->nomeTitulo;
}

void Tabela::addAtributo(QString nome, bool chavePrimaria, bool chaveEstrangeira, bool nulo)
{
    atributo = new Texto();
    atributo->setTextoTabelaLogico(nome,listaAtributo.size());

    if ( chaveEstrangeira )
    {
        nome.insert(0, "[FK] ");
    }

    if ( chavePrimaria )
    {
        nome.insert(0, "[PK] ");
    }

    if ( nulo )
    {
        nome.insert(nome.size(), " \"NULL\"");
    }
    else
    {
        nome.insert(nome.size(), " \"NOT NULL\"");
    }

    atributo->setPlainText(nome);
    atributo->setPos(tab->x(),tab->y()+tab->height());
    atributo->setParentItem(this);
    atributo->setTabelaLogicoAtiva(true);
    atributo->setCursor(Qt::PointingHandCursor);
    listaAtributo.push_back(atributo);
    connect(listaAtributo[listaAtributo.size()-1], SIGNAL(textoTabelaLogicoAlterado(int)), this, SLOT(atualizarAtributo(int)));
    atualizarLargura();

    tab->setHeight(tab->height()+25);
    setRect(*tab);
    setQtdAtributo(getQtdAtributo()+1);
}

void Tabela::atualizarTitulo()
{
    setTitulo(this->titulo->getTextoTabelaLogico().first);
}

void Tabela::atualizarAtributo(int pos)
{
    atualizarNomeAtributo(listaAtributo[pos]->getTextoTabelaLogico().first, pos);
}

QVariant Tabela::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ((change == QGraphicsItem::ItemPositionChange) || ( change == QGraphicsItem::ItemPositionHasChanged ))
    {
        emit posicaoAlterada();
    }

    return value;
}

void Tabela::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::white);
    if ( isSelected() )
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));
    painter->drawRect(*tab);
}
