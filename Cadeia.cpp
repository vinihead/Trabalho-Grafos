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
    vertices.emplace_back(v);
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
    return vertices.size();
}

void Cadeia::insereVertice(int indiceInsercao, int idVertice, double custo)
{
    this->cardinalidadeQ++;
    this->comprimetoL+=custo;
    vertices.insert(vertices.begin()+indiceInsercao, idVertice);
}

vector<int> Cadeia::getCadeia() const {
    return vertices;
}

Cadeia::Cadeia(int vert1, int vert2, double comprimento)
{
    this->vertices.emplace_back(vert1);
    this->vertices.emplace_back(vert2);
    this->cardinalidadeQ = 2;
    this->comprimetoL = comprimento;
}
