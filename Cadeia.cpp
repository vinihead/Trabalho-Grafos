//
// Created by rrodri on 26/11/17.
//

#include "Cadeia.h"

void Cadeia::setCardinalidade(int c)
{
    cardinalidadeQ = c;
}

void Cadeia::setComprimento(double c)
{
    comprimetoL = c;
}

void Cadeia::setVertice(int v)
{
    vert.emplace_back(v);
}

int Cadeia::getCardinalidade()
{
    return cardinalidadeQ;
}

double Cadeia::getComprimento()
{
    return comprimetoL;
}

int Cadeia::getQuantVertice()
{
    return vert.size();
}