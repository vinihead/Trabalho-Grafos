/*
 *******************************************************************
 *         -----------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */

#include "Cadeia.h"

Cadeia::Cadeia(int vert1, int vert2, double comprimento)
{
    this->vertices.emplace_back(vert1);
    this->vertices.emplace_back(vert2);
    this->cardinalidadeQ = 2;
    this->comprimetoL = comprimento;
}

vector<int> Cadeia::getCadeia() const {
    return vertices;
}

int Cadeia::getCardinalidade()const
{
    return cardinalidadeQ;
}

double Cadeia::getComprimento()const {
    return comprimetoL;
}

void Cadeia::insereVertice(int indiceInsercao, int idVertice, double custo)
{
    this->cardinalidadeQ++;
    this->comprimetoL+=custo;
    vertices.insert(vertices.begin()+indiceInsercao, idVertice);
}
