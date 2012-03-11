#include <QtGui>
#include "dialog.h"

Dialog::Dialog(QWidget *parent) : QWidget(parent)
{
}

QString Dialog::mostrarCardinalidade()
{
    QStringList tipos;
    tipos << tr("1 : 1") << tr("1 : N") << tr("0 : 1") << ("0 : N");

    bool ok;
    QString tipo = QInputDialog::getItem(this, tr("Cardinalidade"),
                                         tr("Tipos:"), tipos, 0, false, &ok);

    if (ok && !tipo.isEmpty())
        return tipo;
    else
        return NULL;
}

QString Dialog::mostrarTipoGenEsp()
{
    QStringList tipos;
    tipos << tr("xt") << tr("ct") << tr("xp") << tr("cp");

    bool ok;
    QString tipo = QInputDialog::getItem(this, tr("Tipo Generalização/Especialização"),
                                         tr("Tipos:"), tipos, 0, false, &ok);

    if (ok && !tipo.isEmpty())
        return tipo;
    else
        return NULL;
}
