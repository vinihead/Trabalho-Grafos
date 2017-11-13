/*
 *******************************************************************
 *         -----------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */

#include "Vertice.h"
#include <algorithm> // função find

Vertice::Vertice()
{

}

Vertice::Vertice(int id)
{
    grauEntrada = 0;
    grauSaida = 0;
    this->id = id;
}

void Vertice::setVisita(bool visitado)
{
    this->visitado = visitado;
}

bool Vertice::getVisitado()
{
    return visitado;

}

void Vertice::adicionaAresta(list<Vertice>::iterator vert, float peso, bool digrafo) //metodo para adicionar aresta em grafo orientado
{
    adj.emplace_back(vert->getIdVertice(), peso);

    vert->aumentaGrauEntrada();
    if(digrafo)
        grauSaida++;
}

bool Vertice::removeAresta(list<Vertice>::iterator vert, bool digrafo)
{
    int id = vert->getIdVertice();
    for(k = adj.begin(); k != adj.end(); k++) //auto k é a mesma coisa que list<Aresta>::iterator k
        if(k->getIdAdj() == id)
        {
            adj.erase(k);
            vert->diminuiGrauEntrada();
            if(digrafo)
                grauSaida--;
            return true;
        }
    return false;
}

unsigned int Vertice::getGrau()
{
    return grauEntrada+grauSaida;
}

unsigned int Vertice::getGrauEntrada()
{
    return  grauEntrada;
}

unsigned int Vertice::getGrauSaida()
{
    return grauSaida;
}

bool Vertice::procuraAdjacencia(int idVert) //Retorna true caso ache uma adjacencia com o vertice de id "idVert"
{
    for(k = adj.begin(); k != adj.end(); k++)
        if(k->getIdAdj() == idVert)
            return true;
    return false;
}

unsigned int Vertice::contaAdjacencia(int vert)
{
    unsigned int cont = 0;
    for(k = adj.begin(); k != adj.end(); k++)
        if(k->getIdAdj() == vert)
            cont++;
    return cont;
}

void Vertice::imprimeListAdjacencia(bool ponderado)
{
    for(k = adj.begin(); k != adj.end(); k++)
    {
        ponderado ? cout << k->getIdAdj() << "|" << k->getPeso() : cout << k->getIdAdj();
        if(++k != adj.end())
        {
            cout << " ";
        }
        k--;
    }
}

string Vertice::retornaListAdjacencia(bool ponderado)
{
    string salvaDados;
    for(k = adj.begin(); k != adj.end(); k++)
    {
        if(ponderado)
            salvaDados += to_string(k->getIdAdj()) + "|" + to_string(k->getPeso());
        else
            salvaDados += to_string(k->getIdAdj());
        if(++k != adj.end())
            salvaDados += " ";
        k--;
    }
    return salvaDados;
}


list<Aresta> Vertice::getAdjacencia()
{
    return adj;
}

int Vertice::getIdVertice() {
    return id;
}

void Vertice::diminuiGrauEntrada()
{
    grauEntrada--;
}

void Vertice::aumentaGrauEntrada()
{
    grauEntrada++;
}

void Vertice::adicionaListaAdj(list<Aresta> adj)
{
    this->adj = adj;
}

void Vertice::setCor(int cor)
{
    this->cor = cor;
}

int Vertice::getCor()
{
    return cor;
}

bool Vertice::getDisponivel() {
    return disp;
}

void Vertice::setDisponivel(bool _d) {
    disp = _d;
}

long Vertice::getDistancia() const {
    return dist;
}

void Vertice::setDistancia(int dist) {
    this->dist = dist;
}

long Vertice::getAntecessor() const {
    return ant;
}

void Vertice::setAntecessor(int ant) {
    this->ant = ant;
}