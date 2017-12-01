//
// Created by rrodri on 26/11/17.
//

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

int Cadeia::getCardinalidade()
{
    return cardinalidadeQ;
}

double Cadeia::getComprimento()
{
    return comprimetoL;
}

void Cadeia::insereVertice(int indiceInsercao, int idVertice, double custo)
{
    this->cardinalidadeQ++;
    this->comprimetoL+=custo;
    vertices.insert(vertices.begin()+indiceInsercao, idVertice);
}