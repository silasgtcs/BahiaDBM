#ifndef ACOES_H
#define ACOES_H

class Acao // Interface para as acoes
{
public:
    virtual ~Acao() {}

    virtual void fazerAcao() = 0;
    virtual void desfazerAcao() = 0;
    virtual bool podeDesfazer() = 0;
};

#endif // ACOES_H
