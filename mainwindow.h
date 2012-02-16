#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ligacao.h"
#include "diagrama.h"
#include "diagramaview.h"
#include "acoespilha.h"

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

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenu();
    void createToolBar();
    void organizarEntidades(int direcao);

    AcoesPilha * pilhaDeAcoes;

    Diagrama *scene;
    DiagramaView *view;
    Poligono *cast_poligono;

    QAction *sairAction;
    QAction *ajudaAction;
    QAction *desfazerAction;
    QAction *refazerAction;

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

    QList<QGraphicsItem *> lixo;
};

#endif // MAINWINDOW_H
