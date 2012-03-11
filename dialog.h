/*Utilizada para criar a janela de opções para que seja modificado os tipos de generalização/especialização
ou os tipos de cardinalidade.*/

#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    QString mostrarCardinalidade();
    QString mostrarTipoGenEsp();
};

#endif // DIALOG_H
