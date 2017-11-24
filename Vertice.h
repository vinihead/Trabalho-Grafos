/*
 *******************************************************************
 *         -----------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */

#ifndef TRABALHOGRAFOSNOVO_VERTICE_H
#define TRABALHOGRAFOSNOVO_VERTICE_H

#include <iostream>
#include <list>
#include "Aresta.h"

using namespace std;

class Aresta;

class Vertice
{
private:
    list<Aresta>::iterator k;
    list<Aresta> adj;
    int id;
    int x;
    int y;
    int corPB; ///Variavel que contem a cor do vertice (preto ou branco)
    unsigned int grauEntrada;
    unsigned int grauSaida;
    bool visitado;
    double distancia;
    int antecessor;

public:
    Vertice(int id, int y, int x, int corPB);
    int getIdVertice();
    bool procuraAdjacencia(int vert);
    unsigned int getGrau();
    void imprimeListAdjacencia(bool ponderado);
    unsigned int contaAdjacencia(int vert);
    list<Aresta> getAdjacencia();
    void adicionaAresta(list<Vertice>::iterator vert, double peso, bool digrafo);
    bool removeAresta(list<Vertice>::iterator vert, bool digrafo);
    void diminuiGrauEntrada();
    void aumentaGrauEntrada();
    unsigned int getGrauEntrada();
    unsigned int getGrauSaida();
    bool getVisitado();
    void setVisita(bool visitado);
    string retornaListAdjacencia(bool ponderado);

    int getY();

    int getX();

    int getCorPB();

    bool operator<(const Vertice &rhs) const;

    bool operator>(const Vertice &rhs) const;

    bool operator<=(const Vertice &rhs) const;

    bool operator>=(const Vertice &rhs) const;


    double getDistancia() const;

    void setDistancia(double _distancia);

    int getAntecessor() const;

    void setAntecessor(int _antecessor);

    list<Aresta>::iterator getAresta(int id);

    double getPeso(int id);
};


#endif //TRABALHOGRAFOSNOVO_VERTICE_H
