#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ligacao.h"
#include "diagrama.h"
#include "diagramaview.h"
#include "acoespilha.h"
#include "cardinalidade.h"
#include "poligono.h"
#include "texto.h"

#include <QMainWindow>
#include <QtGui>

class Diagrama;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

signals:

private slots:
    void sobre();
    void botoesERClicked(int id);
    void botoesManipClicked(int id);
    void botoesMLClicked(int id);
    void itemInserido();
    void desfazer();
    void refazer();
    void novaJanela();
    void abrir();
    void salvarComo();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenu();
    void createToolBar();
    void organizarEntidades(int direcao);
    void salvarArquivo(const QString nomeArquivo);
    void abrirArquivo(const QString nomeArquivo);
    void deletarSelecionado();

    AcoesPilha * pilhaDeAcoes;

    Diagrama *scene;
    DiagramaView *view;
    Poligono *cast_poligono;

    QAction *sairAction;
    QAction *ajudaAction;
    QAction *desfazerAction;
    QAction *refazerAction;
    QAction *abrirAction;
    QAction *salvarAction;
    QAction *novaAction;

    QMenu *arquivoMenu;
    QMenu *editarMenu;
    QMenu *ajudaMenu;

    QToolBar *formas;
    QToolBar *manipulacoes;
    QToolBar *mouseLinha;

    QToolButton *entidade;
    QToolButton *relacionamento;
    QToolButton *gen_esp;
    QToolButton *entid_associativa;
    QToolButton *atributo;
    QToolButton *atributo_identif;
    QToolButton *ampliar;
    QToolButton *reduzir;
    QToolButton *mouse;
    QToolButton *linhas;

    QToolButton *deletar;
    QToolButton *entidadeFraca;

    QToolButton *organizarHor;
    QToolButton *organizarVer;

    QButtonGroup *botoesER;
    QButtonGroup *botoesManip;
    QButtonGroup *botoesML;    

    QToolBar * exibicaoToolBar;
    QSpinBox * scroolZoom;

    Ligacao *lig;
    Ligacao *castLinha;
    Poligono *castEntidade;
    Atributo *castAtributo;
    Cardinalidade *castCardinalidade;

    QList<QGraphicsItem *> lixo;

    Poligono *castPoligono;
    Poligono *castPoligono2;
    Poligono *abrirPoligono;
    Poligono *abrirPoligono2;
    Atributo *castAtributo2;
    Atributo *abrirAtributo;
    Ligacao *castLigacao;
    Texto *abrirTexto;
    Texto *abrirAutoRelacionamento1;
    Texto *abrirAutoRelacionamento2;
    Cardinalidade *abrirCardinalidade;
};

#endif // MAINWINDOW_H
