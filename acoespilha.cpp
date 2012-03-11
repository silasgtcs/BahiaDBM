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

void AcoesPilha::addAcao(Acao *acao)
{
        if(!acao->podeDesfazer())
                pilhaDeAcoes.clear();
        else
        {
                posAtual++;
                while(posAtual < pilhaDeAcoes.size())
                        pilhaDeAcoes.removeAt(posAtual);
                pilhaDeAcoes.append(acao);
        }
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
