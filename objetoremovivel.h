#ifndef OBJETOREMOVIVEL_H
#define OBJETOREMOVIVEL_H

#include <QtGui>

class ObjetoRemovivel
{
public:
    virtual void doRemove(bool value) = 0;
    virtual QList<QGraphicsItem *> getToDelete() = 0;
};

#endif // OBJETOREMOVIVEL_H
