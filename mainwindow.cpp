#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pilhaDeAcoes = AcoesPilha::instanciaUnica();

    scene = new Diagrama(this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserido()),
            this, SLOT(itemInserido()));
    scene->setTipoER(Diagrama::TipoER(7));
    view = new DiagramaView(scene);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);

    createActions();
    createMenu();
    createToolBar();

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle("BahiaDBM");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::itemInserido()
{
    botoesER->button(int(scene->getTipoER()))->setChecked(false);
    scene->setTipoER(Diagrama::TipoER(7)); //Tipo(7) = nenhum
    botoesML->button(0)->setChecked(true);
    botoesML->button(1)->setChecked(false);
}

void MainWindow::botoesERClicked(int id)
{
    QList<QAbstractButton *> botoes = botoesER->buttons();
    foreach ( QAbstractButton *botao, botoes )
        if ( botoesER->button(id) != botao )
            botao->setChecked(false);

    scene->setTipoER(Diagrama::TipoER(id));

    /*Inicialmente verifica se botão clicado é o "EF". Caso afirmativo, verifica se existe alguma linha selecionada, e então aumenta
      a sua espessura.*/
    if ( id == 6 )
    {
        scene->setTipoER(Diagrama::TipoER(7));

        QList<QGraphicsItem *> entidade = scene->selectedItems();

        if ((entidade.length() > 0) && ( entidade[0]->type() == Ligacao::Type ))
        {
            castLinha = qgraphicsitem_cast<Ligacao *>(entidade[0]);
            castLinha->setEntidadeFracaAtiva(true);
        }
    }
}

void MainWindow::botoesManipClicked(int id)
{
    botoesManip->button(id)->setChecked(false);

    if(id == 0 || id == 1)
    {
        QList<QGraphicsItem *> itens = scene->selectedItems();
        for ( int i=0; i<itens.size(); i++ )
        {
            if(itens[i]->type() == Poligono::Type || itens[i]->type() == Atributo::Type || itens[i]->type() == Texto::Type)
            {
                bool parentInList = false;
                QGraphicsItem * parentItem = itens[i]->parentItem();
                while(parentItem != NULL)
                {
                    if(itens.contains(parentItem))
                    {
                        parentInList = true;
                        break;
                    }
                    parentItem = parentItem->parentItem();
                }
                if(!parentInList)
                    itens[i]->setScale(itens[i]->scale() + ((id == 0) ? 0.1 : -0.1));

                //Move item para atualizar linha, caso exista
                itens[i]->setPos(itens[i]->x()+0.1, itens[i]->y()+0.1);
                itens[i]->setPos(itens[i]->x()-0.1, itens[i]->y()-0.1);
            }

        }
    }
    else if(id == 2 || id == 3)
    {
        organizarEntidades(id - 2);
    }
}

void MainWindow::organizarEntidades(int direcao) // 0 = Hor, 1 = Vert
{
    QList<QGraphicsItem*> itemsSelecionados = scene->selectedItems();
    QGraphicsItem* item;
    QGraphicsItem* lastItem = NULL;
    QGraphicsItem* firstItem = NULL;

    for(int i = 0; i < itemsSelecionados.size(); i++)
    {
        if(itemsSelecionados.at(i)->type() != Poligono::Type)
        {
            itemsSelecionados.removeAt(i);
            i--; // reflete mudanca de tamanho do vetor
        }
    }

    if(!itemsSelecionados.empty())
    {
        lastItem = firstItem = itemsSelecionados.first();
        if(firstItem != NULL)
        {
            foreach(item, itemsSelecionados)
            {
                if(firstItem == item)
                    continue;

                if(lastItem != NULL)
                {
                    if(direcao == 0)
                        item->setPos(item->pos() + item->mapFromScene(QPointF(lastItem->scenePos().x(), lastItem->scenePos().y()  + lastItem->boundingRect().height() + 100)));
                    else
                        item->setPos(item->pos() + item->mapFromScene(QPointF(lastItem->scenePos().x() + lastItem->boundingRect().width() + 150, lastItem->scenePos().y())));
                }
                lastItem = item;
            }
        }
    }
}

void MainWindow::desfazer()
{
    pilhaDeAcoes->desfazer();
}

void MainWindow::refazer()
{
    pilhaDeAcoes->refazer();
}

void MainWindow::botoesMLClicked(int id)
{
    QList<QAbstractButton *> botoes = botoesML->buttons();
    foreach ( QAbstractButton *botao, botoes )
        if ( botoesML->button(id) != botao )
            botao->setChecked(false);

    if ( id == 0 ) //Mouse
        scene->setTipoER(Diagrama::TipoER(7));
    else if ( id == 1 ) //Linha
        scene->setTipoER(Diagrama::TipoER(6));
    else //Remover
    {
        QList<QGraphicsItem *> selecionados = scene->selectedItems();
        for ( int i=0; i<selecionados.length(); i++)
        {
            if ( selecionados[i] != NULL )
            {
                if ( selecionados[i]->type() == Poligono::Type )
                {
                    //apaga linhas associadas
                    cast_poligono = qgraphicsitem_cast<Poligono *>(selecionados[i]);
                    for ( int j=0; j<cast_poligono->getLinhas_Associadas().size(); j++ )
                    {
                        if ( cast_poligono->getLinhas_Associadas().at(j)->isSelected() )
                        {
                            int temp = selecionados.indexOf(cast_poligono->getLinhas_Associadas().at(j));
                            if ( temp != -1 )
                            {
                                selecionados[temp] = NULL;
                            }
                        }

                        scene->removeItem(cast_poligono->getLinhas_Associadas().at(j));
                    }
                    cast_poligono->RemoveTodasLinhasAssociadas();

                    //apaga cardinalidades associadas
                    for ( int j=0; j<cast_poligono->getCardinalidades_Associadas().size(); j++ )
                    {
                        if ( cast_poligono->getCardinalidades_Associadas().at(j)->isSelected() )
                        {
                            int temp = selecionados.indexOf(cast_poligono->getCardinalidades_Associadas().at(j));
                            if ( temp != -1 )
                            {
                                selecionados[temp] = NULL;
                            }
                        }
                        scene->removeItem(cast_poligono->getCardinalidades_Associadas().at(j));
                    }
                    cast_poligono->RemoveTodasCardinalidadesAssociadas();

                    //Retira associações dos objetos que não foram removidos mas estavam associados aos que foram removidos.
                    //Inicia percorrendo as entidades.
                    for ( int j=0; j<cast_poligono->getEntidades_Associadas().size(); j++ )
                    {
                        for ( int w = 0; w<cast_poligono->getEntidades_Associadas().at(j)->getRelacionamentos_Associados().size(); w++ )
                        {
                            if ( cast_poligono->getEntidades_Associadas().at(j)->getRelacionamentos_Associados().at(w) == selecionados[i] )
                            {
                                cast_poligono->getEntidades_Associadas().at(j)->Remove_Relacionamento_Associado(w);
                            }
                        }

                        for ( int w = 0; w<cast_poligono->getEntidades_Associadas().at(j)->getLinhas_Associadas().size(); w++ )
                        {
                            bool control = false;
                            if ((cast_poligono->getEntidades_Associadas().at(j)->getLinhas_Associadas().at(w)->getRelacionamentos_Associados().size() > 0)
                               && ( cast_poligono->getEntidades_Associadas().at(j)->getLinhas_Associadas().at(w)->getRelacionamentos_Associados().at(0)
                                    == selecionados[i] ))
                            {
                                cast_poligono->getEntidades_Associadas().at(j)->Remove_Linha_Associada(w);
                                control = true;
                            }

                            if ((!control) && ((cast_poligono->getEntidades_Associadas().at(j)->getLinhas_Associadas().at(w)->getEntidades_Associativas_Associadas().size() > 0)
                               && ( cast_poligono->getEntidades_Associadas().at(j)->getLinhas_Associadas().at(w)->getEntidades_Associativas_Associadas().at(0)
                                    == selecionados[i] )))
                            {
                                cast_poligono->getEntidades_Associadas().at(j)->Remove_Linha_Associada(w);
                            }
                        }

                        for ( int w= 0; w<cast_poligono->getEntidades_Associadas().at(j)->getCardinalidades_Associadas().size(); w++ )
                        {
                            bool control = false;
                            if (( cast_poligono->getEntidades_Associadas().at(j)->getCardinalidades_Associadas().at(w)->getRelacionamentos_Associados().size() > 0 )
                               && ( cast_poligono->getEntidades_Associadas().at(j)->getCardinalidades_Associadas().at(w)->getRelacionamentos_Associados().at(0)
                                    == selecionados[i] ))
                            {
                                cast_poligono->getEntidades_Associadas().at(j)->Remove_Cardinalidade_Associada(w);
                                control = true;
                            }

                            if ((!control) && (( cast_poligono->getEntidades_Associadas().at(j)->getCardinalidades_Associadas().at(w)->getEntidades_Associativas_Associadas().size() > 0 )
                               && ( cast_poligono->getEntidades_Associadas().at(j)->getCardinalidades_Associadas().at(w)->getEntidades_Associativas_Associadas().at(0)
                                    == selecionados[i] )))
                            {
                                cast_poligono->getEntidades_Associadas().at(j)->Remove_Cardinalidade_Associada(w);
                            }
                        }

                        for ( int w=0; w<cast_poligono->getEntidades_Associadas().at(j)->getEntidades_Associativas_Associadas().size(); w++ )
                        {
                            if ( cast_poligono->getEntidades_Associadas().at(j)->getEntidades_Associativas_Associadas().at(w)
                                    == selecionados[i] )
                            {
                                cast_poligono->getEntidades_Associadas().at(j)->Remove_Entidade_Associativa_Associada(w);
                            }
                        }

                        if ( cast_poligono->getEntidades_Associadas().at(j)->getRelacionamentos_Associados().size() == 0 )
                            cast_poligono->getEntidades_Associadas().at(j)->setConectado(false);
                    }

                    //Percorre agora os relacionamentos verificando se existe alguma associação que foi removida.
                    for ( int j=0; j<cast_poligono->getRelacionamentos_Associados().size(); j++ )
                    {
                        for ( int w = 0; w<cast_poligono->getRelacionamentos_Associados().at(j)->getEntidades_Associadas().size(); w++ )
                        {
                            if ( cast_poligono->getRelacionamentos_Associados().at(j)->getEntidades_Associadas().at(w) == selecionados[i] )
                            {
                                cast_poligono->getRelacionamentos_Associados().at(j)->Remove_Entidade_Associada(w);
                            }
                        }

                        for ( int w = 0; w<cast_poligono->getRelacionamentos_Associados().at(j)->getLinhas_Associadas().size(); w++ )
                        {
                            if ( cast_poligono->getRelacionamentos_Associados().at(j)->getLinhas_Associadas().at(w)->getEntidades_Associadas().at(0)
                                    == selecionados[i] )
                            {
                                cast_poligono->getRelacionamentos_Associados().at(j)->Remove_Linha_Associada(w);
                            }
                        }

                        for ( int w= 0; w<cast_poligono->getRelacionamentos_Associados().at(j)->getCardinalidades_Associadas().size(); w++ )
                        {
                            if ( cast_poligono->getRelacionamentos_Associados().at(j)->getCardinalidades_Associadas().at(w)->getEntidades_Associadas().at(0)
                                    == selecionados[i] )
                            {
                                cast_poligono->getRelacionamentos_Associados().at(j)->Remove_Cardinalidade_Associada(w);
                            }
                        }

                        if ( cast_poligono->getRelacionamentos_Associados().at(j)->getEntidades_Associadas().size() == 0 )
                            cast_poligono->getRelacionamentos_Associados().at(j)->setConectado(false);
                    }

                    //Percorre agora as entidades associativas verificando se existe alguma associação que foi removida.
                    for ( int j=0; j<cast_poligono->getEntidades_Associativas_Associadas().size(); j++ )
                    {
                        for ( int w = 0; w<cast_poligono->getEntidades_Associativas_Associadas().at(j)->getEntidades_Associadas().size(); w++ )
                        {
                            if ( cast_poligono->getEntidades_Associativas_Associadas().at(j)->getEntidades_Associadas().at(w) == selecionados[i] )
                            {
                                cast_poligono->getEntidades_Associativas_Associadas().at(j)->Remove_Entidade_Associada(w);
                            }
                        }

                        for ( int w = 0; w<cast_poligono->getEntidades_Associativas_Associadas().at(j)->getLinhas_Associadas().size(); w++ )
                        {
                            if ( cast_poligono->getEntidades_Associativas_Associadas().at(j)->getLinhas_Associadas().at(w)->getEntidades_Associadas().at(0)
                                    == selecionados[i] )
                            {
                                cast_poligono->getEntidades_Associativas_Associadas().at(j)->Remove_Linha_Associada(w);
                            }
                        }

                        for ( int w= 0; w<cast_poligono->getEntidades_Associativas_Associadas().at(j)->getCardinalidades_Associadas().size(); w++ )
                        {
                            if ( cast_poligono->getEntidades_Associativas_Associadas().at(j)->getCardinalidades_Associadas().at(w)->getEntidades_Associadas().at(0)
                                    == selecionados[i] )
                            {
                                cast_poligono->getEntidades_Associativas_Associadas().at(j)->Remove_Cardinalidade_Associada(w);
                            }
                        }
                    }

                    scene->removeItem(selecionados[i]);
                    selecionados[i] = NULL;
                    lixo.push_back(selecionados[i]);
                    cast_poligono->itemRemovido();
                }

                else if ( selecionados[i]->type() == Atributo::Type )
                {
                    //apaga linhas associadas
                    castAtributo = qgraphicsitem_cast<Atributo *>(selecionados[i]);
                    for ( int j=0; j<castAtributo->getLinhaAssociada().size(); j++ )
                    {
                        scene->removeItem(castAtributo->getLinhaAssociada().at(j));
                    }
                    castAtributo->RemoveTodasLinhasAssociadas();

                    scene->removeItem(selecionados[i]);
                    selecionados[i] = NULL;
                    lixo.push_back(selecionados[i]);
                }

                else if ( selecionados[i]->type() == Ligacao::Type )
                {
                    castLinha = qgraphicsitem_cast<Ligacao *>(selecionados[i]);
                    //apaga cardinalidades associadas
                    for ( int j=0; j<castLinha->getCardinalidades_Associadas().size(); j++ )
                    {
                        if ( castLinha->getCardinalidades_Associadas().at(j)->isSelected() )
                        {
                            int temp = selecionados.indexOf(castLinha->getCardinalidades_Associadas().at(j));
                            if ( temp != -1 )
                            {
                                selecionados[temp] = NULL;
                            }
                        }

                        scene->removeItem(castLinha->getCardinalidades_Associadas().at(j));
                    }

                    for ( int j=0; j<castLinha->getEntidades_Associadas().size(); j++ )
                    {
                        for ( int w=0; w<castLinha->getEntidades_Associadas().at(j)->getLinhas_Associadas().size(); w++ )
                        {
                            if ( castLinha->getEntidades_Associadas().at(j)->getLinhas_Associadas().at(w) == selecionados[i] )
                            {
                                castLinha->getEntidades_Associadas().at(j)->Remove_Linha_Associada(w);
                            }
                        }

                        for ( int w=0; w<castLinha->getRelacionamentos_Associados().at(j)->getLinhas_Associadas().size(); w++ )
                        {
                            if ( castLinha->getRelacionamentos_Associados().at(j)->getLinhas_Associadas().at(w) == selecionados[i] )
                            {
                                castLinha->getRelacionamentos_Associados().at(j)->Remove_Linha_Associada(w);
                            }
                        }

                        for ( int w=0; w<castLinha->getEntidades_Associadas().at(j)->getCardinalidades_Associadas().size(); w++ )
                        {
                            if ( castLinha->getEntidades_Associadas().at(j)->getCardinalidades_Associadas().at(w)
                                    == castLinha->getCardinalidades_Associadas().at(0) )
                            {
                                castLinha->getEntidades_Associadas().at(j)->Remove_Cardinalidade_Associada(w);
                            }
                        }

                        for ( int w=0; w<castLinha->getRelacionamentos_Associados().at(j)->getCardinalidades_Associadas().size(); w++ )
                        {
                            if ( castLinha->getRelacionamentos_Associados().at(j)->getCardinalidades_Associadas().at(w)
                                    == castLinha->getCardinalidades_Associadas().at(0) )
                            {
                                castLinha->getRelacionamentos_Associados().at(j)->Remove_Cardinalidade_Associada(w);
                            }
                        }

                        if ( castLinha->getEntidades_Associadas().size() == 0 )
                            castLinha->getEntidades_Associadas().at(j)->setConectado(false);

                        if ( castLinha->getRelacionamentos_Associados().size() == 0 )
                            castLinha->getRelacionamentos_Associados().at(j)->setConectado(false);
                    }

                    castLinha->RemoveTodasCardinalidadesAssociadas();

                    scene->removeItem(selecionados[i]);
                    selecionados[i] = NULL;
                    lixo.push_back(selecionados[i]);
                }
            }
        }

        lixo.erase(lixo.begin(), lixo.end());
    }
}

void MainWindow::sobre()
{
    QMessageBox::about(this, tr("Sobre BahiaDBM"), tr("Bahia Database Modeler"));
}

void MainWindow::createActions()
{
    sairAction = new QAction(tr("&Sair"), this);
    sairAction->setShortcut(QKeySequence::Quit);
    sairAction->setStatusTip(tr("Sair do BahiaDBM"));
    connect(sairAction, SIGNAL(triggered()), this, SLOT(close()));

    desfazerAction = new QAction(tr("&Desfazer"), this);
    desfazerAction->setShortcut(tr("Ctrl+Z"));
    desfazerAction->setStatusTip(tr("Desfazer"));
    connect(desfazerAction, SIGNAL(triggered()), this, SLOT(desfazer()));

    refazerAction = new QAction(tr("&Refazer"), this);
    refazerAction->setShortcut(tr("Ctrl+R"));
    refazerAction->setStatusTip(tr("Refazer"));
    connect(refazerAction, SIGNAL(triggered()), this, SLOT(refazer()));

    ajudaAction = new QAction(tr("&Sobre"), this);
    ajudaAction->setShortcut(tr("Ctrl+H"));
    ajudaAction->setStatusTip(tr("Sobre BahiaDBM"));
    connect(ajudaAction, SIGNAL(triggered()), this, SLOT(sobre()));
}

void MainWindow::createMenu()
{
    arquivoMenu = menuBar()->addMenu(tr("&Arquivo"));
    arquivoMenu->addAction(sairAction);

    editarMenu = menuBar()->addMenu(tr("&Editar"));
    editarMenu->addAction(desfazerAction);
    editarMenu->addAction(refazerAction);

    ajudaMenu = menuBar()->addMenu(tr("&Ajuda"));
    ajudaMenu->addAction(ajudaAction);
}

void MainWindow::createToolBar()
{
    //Cria botões que selecionam as figuras geométricas
    entidade = new QToolButton();
    relacionamento = new QToolButton();
    gen_esp = new QToolButton();
    entid_associativa = new QToolButton();
    atributo = new QToolButton();
    atributo_identif = new QToolButton();
    entidadeFraca = new QToolButton();
    entidade->setCheckable(true);
    relacionamento->setCheckable(true);
    gen_esp->setCheckable(true);
    entid_associativa->setCheckable(true);
    atributo->setCheckable(true);
    atributo_identif->setCheckable(true);
    entidade->setIcon(QIcon(":/imagens/entidade.png"));
    entidade->setStatusTip(tr("Entidade"));
    relacionamento->setIcon(QIcon(":/imagens/relacionamento.png"));
    relacionamento->setStatusTip(tr("Relacionamento"));
    gen_esp->setIcon(QIcon(":/imagens/gen_especializacao.png"));
    gen_esp->setStatusTip(tr("Generalizacao/Especializacao"));
    entid_associativa->setIcon(QIcon(":/imagens/ent_associativa.png"));
    entid_associativa->setStatusTip(tr("Entidade Associativa"));
    atributo->setIcon(QIcon(":/imagens/atributo.png"));
    atributo->setStatusTip(tr("Atributo"));
    atributo_identif->setIcon(QIcon(":/imagens/atributo_identif.png"));
    atributo_identif->setStatusTip(tr("Atributo Identificador"));
    entidadeFraca->setText("EF");
    entidadeFraca->setStatusTip(tr("Entidade Fraca"));


    botoesER = new QButtonGroup(this);
    botoesER->setExclusive(false);
    botoesER->addButton(entidade,0);
    botoesER->addButton(relacionamento,1);
    botoesER->addButton(gen_esp,2);
    botoesER->addButton(entid_associativa,3);
    botoesER->addButton(atributo,4);
    botoesER->addButton(atributo_identif,5);
    botoesER->addButton(entidadeFraca, 6);
    connect(botoesER, SIGNAL(buttonClicked(int)),
            this, SLOT(botoesERClicked(int)));

    formas = addToolBar(tr("&Formas"));
    formas->addWidget(entidade);
    formas->addWidget(relacionamento);
    formas->addWidget(gen_esp);
    formas->addWidget(entid_associativa);
    formas->addWidget(atributo);
    formas->addWidget(atributo_identif);
    formas->addWidget(entidadeFraca);

    //Cria botões de manipulação das figuras geométricas
    ampliar = new QToolButton();
    ampliar->setCheckable(true);
    ampliar->setStatusTip(tr("Ampliar"));
    ampliar->setIcon(QIcon(":/imagens/ampliar.png"));

    reduzir = new QToolButton();
    reduzir->setCheckable(true);
    reduzir->setStatusTip(tr("Reduzir"));
    reduzir->setIcon(QIcon(":/imagens/reduzir.png"));

//    organizarHor = new QToolButton();
//    organizarHor->setCheckable(true);
//    organizarHor->setStatusTip(tr("Organizar Horizontalmente"));
//    organizarHor->setText(tr("Org. Horiz."));
//    //organizarHor->setIcon(QIcon(":/imagens/reduzir.png"));

//    organizarVer = new QToolButton();
//    organizarVer->setCheckable(true);
//    organizarVer->setStatusTip(tr("Organizar Verticalmente"));
//    organizarVer->setText(tr("Org. Vert."));
//    //organizarVer->setIcon(QIcon(":/imagens/reduzir.png"));

    botoesManip = new QButtonGroup(this);
    botoesManip->setExclusive(false);
    botoesManip->addButton(ampliar,0);
    botoesManip->addButton(reduzir,1);
//    botoesManip->addButton(organizarHor,3);
//    botoesManip->addButton(organizarVer,2);
    connect(botoesManip, SIGNAL(buttonClicked(int)),
            this, SLOT(botoesManipClicked(int)));

    manipulacoes = addToolBar(tr("&Manipulacoes"));
    manipulacoes->addWidget(ampliar);
    manipulacoes->addWidget(reduzir);
//    manipulacoes->addWidget(organizarHor);
//    manipulacoes->addWidget(organizarVer);

    //Cria botões(linha ou mouse) que permite escolher entre ligar ou manipular figuras
    mouse = new QToolButton();
    linhas = new QToolButton();
    deletar = new QToolButton();
    mouse->setCheckable(true);
    mouse->setChecked(true);
    linhas->setCheckable(true);
    deletar->setCheckable(false);
    mouse->setStatusTip(tr("Mouse"));
    linhas->setStatusTip(tr("Linha"));
    deletar->setStatusTip(tr("Deletar"));
    mouse->setIcon(QIcon(":/imagens/mouse.png"));
    linhas->setIcon(QIcon(":/imagens/linha.png"));
    deletar->setIcon(QIcon(":/imagens/deletar.png"));

    botoesML = new QButtonGroup(this);
    botoesML->setExclusive(false);
    botoesML->addButton(mouse,0);
    botoesML->addButton(linhas,1);
    botoesML->addButton(deletar,2);
    connect(botoesML, SIGNAL(buttonClicked(int)),
            this, SLOT(botoesMLClicked(int)));

    mouseLinha = addToolBar(tr("&MouseLinha"));
    mouseLinha->addWidget(mouse);
    mouseLinha->addWidget(linhas);
    mouseLinha->addWidget(deletar);

    //Criando toolbar de exibicao(zoom, fit-to-all)
    exibicaoToolBar = addToolBar(tr("&Exibicao"));

    scroolZoom = new QSpinBox(exibicaoToolBar);
    scroolZoom->setFixedWidth(50);
    scroolZoom->setMaximum(500);
    scroolZoom->setMinimum(10);
    scroolZoom->setValue(100);
    connect(scroolZoom, SIGNAL(valueChanged(int)), view, SLOT(setarZoom(int)));

    //Associa scrool ao graphicView(assim wheel do mouse pode ser redirecionado para o scroll)
    view->setScrool(scroolZoom);

    exibicaoToolBar->addWidget(scroolZoom);
}
