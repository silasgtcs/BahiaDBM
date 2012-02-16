#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    QString mostrarCardinalidade();

};

#endif // DIALOG_H
