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
    Vertice();

    list<Aresta>::iterator k;
    list<Aresta> adj;
    int id;
    unsigned int grauEntrada;
    unsigned int grauSaida;
    bool visitado;


    ///RUAN
    bool disp;
    long dist;
    long ant;
    int cor; ///SEMCOR, AZUL, VERMELHO

public:
    Vertice(int id);
    int getIdVertice();
    bool procuraAdjacencia(int vert);
    unsigned int getGrau();
    void imprimeListAdjacencia(bool ponderado);
    unsigned int contaAdjacencia(int vert);
    list<Aresta> getAdjacencia();
    void adicionaAresta(list<Vertice>::iterator vert, float peso, bool digrafo);
    bool removeAresta(list<Vertice>::iterator vert, bool digrafo);
    void diminuiGrauEntrada();
    void aumentaGrauEntrada();
    unsigned int getGrauEntrada();
    unsigned int getGrauSaida();
    bool getVisitado();
    bool getDisponivel();
    void setDisponivel(bool d);
    long getDistancia() const;
    void setDistancia(int dist);
    long getAntecessor() const;
    void setAntecessor(int ant);
    void adicionaListaAdj(list<Aresta> adj);
    void setCor(int cor);
    int getCor();
    void setVisita(bool visitado);
    string retornaListAdjacencia(bool ponderado);
};


#endif //TRABALHOGRAFOSNOVO_VERTICE_H
