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
#include <iomanip>

Vertice::Vertice(int id, int y, int x, int corPB)
{
    grauEntrada = 0;
    grauSaida = 0;
    this->id = id;
    this->x = x;
    this->y = y;
    this->corPB = corPB;
}

void Vertice::setVisita(bool visitado)
{
    this->visitado = visitado;
}

bool Vertice::getVisitado()
{
    return visitado;

}

int Vertice::getX()
{
    return x;
}

int Vertice::getY()
{
    return y;
}

void Vertice::adicionaAresta(list<Vertice>::iterator vert, double peso, bool digrafo) //metodo para adicionar aresta em grafo orientado
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
    cout.precision(2);
    cout.setf(ios::fixed);
    for(k = adj.begin(); k != adj.end(); k++)
    {
        ponderado ? cout << right << setw(2) << k->getIdAdj() << "|" << k->getPeso() << fixed : cout << setw(2) << k->getIdAdj();
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

int Vertice::getCorPB()
{
    return corPB;
}

bool Vertice::operator<(const Vertice &rhs) const {
    return id < rhs.id;
}

bool Vertice::operator>(const Vertice &rhs) const {
    return rhs < *this;
}

bool Vertice::operator<=(const Vertice &rhs) const {
    return !(rhs < *this);
}

bool Vertice::operator>=(const Vertice &rhs) const {
    return !(*this < rhs);
}


double Vertice::getDistancia() const {
    return distancia;
}

void Vertice::setDistancia(double _distancia) {
    distancia = _distancia;
}

int Vertice::getAntecessor() const {
    return antecessor;
}

void Vertice::setAntecessor(int _antecessor) {
    antecessor = _antecessor;
}

list<Aresta>::iterator Vertice::getAresta(int id)
{
    for(k = adj.begin(); k != adj.end(); k++)
        if(k->getIdAdj() == id)
            break;
    return k;
}