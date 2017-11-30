/*
 *******************************************************************
 *         -----------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */

#ifndef TRABALHOGRAFOSNOVO_GRAFO_H
#define TRABALHOGRAFOSNOVO_GRAFO_H

#include <iostream>
#include <vector>
#include "Vertice.h"
#include "Cadeia.h"

#include <sstream> // Fluxo de strings
#include <fstream> // Leitura e escrita de arquivo
#include <cstdlib> //funcao atoi e exit
#include <set>

using namespace std;

class Grafo
{
private:
    list<Vertice> vertices;
    unsigned int ordem;
    bool digrafo;
    bool ponderado;
    double maxCusto;
    int maxVertBranco;
    double **matrizDistancia;
    int numPretos;
    Grafo * retornaInstanciaGrafo();
    list<Vertice>::iterator adicionaVertice(int id, int x, int y, int corPB);

    void adicionaAresta(int idOrigem, int idDestino, float peso, int corPB);
    bool removeAresta(int v1, int v2);
    void criaMatrizPeso();
    void criaTodasArestas();
    double calculaPesoAresta(int id1, int id2);


public:


    typedef struct {
        vector<int> vert;
        int cardinalidadeQ;
        double comprimetoL;
    } CadeiaStrct;

    typedef struct {
        vector<Cadeia> cadeias;
        vector<int> solucao;
        //vector<int> solucao;
    } Solucao;








    bool removeVertice(int idVert);


    explicit Grafo(ifstream *inFile);
    Grafo(bool digrafo, bool ponderado, double maxCusto, int maxVertBranco);

    void imprime();
    void saveGrafo(ofstream *outFile);
    list<Vertice>::iterator getVertice(int idVert);
    bool procuraVertice(int idVert);
    unsigned int getOrdem();
    void saveGrafoAdjacencia(ofstream *outFile);
    void geraLinguagemDot();
    void algConstrutGuloso();
    void algConstrutGulRandReativo();
    void algConstrutGulRandomizado();
    void caixeiroViajante();
    void imprimeMatrizDistancia();
    void caixeiroViajanteAux(int i, double &valorSolucaoAtual, double &valorMelhorSolucao, int *tempSolucao, int *melhorSolucao, bool *visitados);
    int getCorPB(int idVert);



    //bool viavel(double dist);
    //bool viavel(CadeiaTeste cadeia, double dist);
    //bool viavel(Cadeia cadeia, double dist);






};



#endif //TRABALHOGRAFOSNOVO_GRAFO_H
