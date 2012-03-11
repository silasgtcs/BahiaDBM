#include "acoespilha.h"

AcoesPilha::AcoesPilha() : posAtual(-1)
{
}

Acao * AcoesPilha::getTopCommand()
{
    if(posAtual < 0 || posAtual >= pilhaDeAcoes.size())
        return NULL;
    return pilhaDeAcoes.at(posAtual);
}

void AcoesPilha::addAcao(Acao *command) {
    addAcao(command, false);
}

void AcoesPilha::addAcao(Acao *acao, bool run)
{
    if(!acao->podeDesfazer())
            pilhaDeAcoes.clear();
    else
    {
        posAtual++;
        int i = pilhaDeAcoes.size() - 1;
        while(i >= posAtual) {
            pilhaDeAcoes.at(i)->dispose();
            pilhaDeAcoes.at(i)->deleteLater();
            pilhaDeAcoes.removeAt(i);
            i--;
        }
        pilhaDeAcoes.append(acao);
    }
    if(run)
        acao->fazerAcao();
}

bool AcoesPilha::desfazer()
{
    if(posAtual == -1)
            return false;

    pilhaDeAcoes.at(posAtual)->desfazerAcao();
    posAtual--;
    return true;
}

bool AcoesPilha::refazer()
{
    if(posAtual == pilhaDeAcoes.size()-1)
            return false;

    pilhaDeAcoes.at(++posAtual)->fazerAcao();
    return true;
}

void AcoesPilha::limpar()
{
    for(int i = pilhaDeAcoes.size() - 1; i >= 0; i--) {
        //pilhaDeAcoes.at(i)->dispose();
        pilhaDeAcoes.at(i)->deleteLater();
    }
    pilhaDeAcoes.clear();
}
