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
    //vector<vector<flot>> matrizPesos;
    //bool verificaPonderado(string s);
    Grafo * retornaInstanciaGrafo();
    //void auxFechoTransitivoDireto(int _idVertice, set<int> *percorridos);
    //void numComponentesConexasAux(list<Vertice>::iterator);
    //bool verificaBipartidoAux(list<Vertice>::iterator itVert, int c);

    list<Vertice>::iterator adicionaVertice(int id, int x, int y, char corPB);
    bool removeVertice(int idVert);
    void adicionaAresta(int idOrigem, int idDestino, float peso, char corPB);
    bool removeAresta(int v1, int v2);


    void criaMatrizPeso();
    void criaTodasArestas();
    double calculaPesoAresta(int id1, int id2);


public:
    Grafo(ifstream *inFile);
    Grafo(bool digrafo, bool ponderado, double maxCusto, int maxVertBranco);

    bool verificaTrivial();
    bool kRegularidade(int k);
    int grauVertice(int idVert, bool mostraGrau, ofstream *outFile, bool imprimir);
    unsigned int getGrau();
    bool verificaNulo();
    Grafo * complementar();
    void imprime();
    void saveGrafo(ofstream *outFile);
    string vizinhacaAberta(int vert);
    string vizinhacaFechada(int vert);
    bool verificaMultigrafo();
    bool verificaCompleto();
    list<Vertice>::iterator getVertice(int idVert);
    bool procuraVertice(int idVert);
    void apresentaSequenciaGraus(ofstream *outFile, bool imprime);
    string arvoreBuscaLargura(int idVert);
    string arvoreBuscaProfundidade(int idVert);
    bool verificaConexo();
    bool verificaEuleriano();
    //bool verificaOrdenado();
    string verificaVerticesArticulacao();
    //string fechoTransitivoDireto(int v);
    //string fechoTransitivoIndireto(int v);
    string arvoreBuscaProfundidadeAux(int idVert);
    //int caminhoMinimoDijkstra(int orig, int dest);
    //bool verificaBipartido();
    Grafo *subgrafoInduzido(vector<int> vetorVertices);
    void verificaArestasPonte(ofstream *outFile);
    //long caminhoMinimoFloyd(long _idVerticeOrigem, long _idVerticeDestino);
    int getQntdArestas();
    unsigned int getOrdem();
    //bool getPonderado();
    //bool adicionaVerticeNaMain(int id);
    void saveGrafoAdjacencia(ofstream *outFile);
    //int numComponentesConexas();

    //void auxFechoTransitivoIndireto(Grafo *grafoAux, int id, set<int> *percorridos);

    void geraLinguagemDot();

    void algConstrutGuloso();
    void algConstrutGulRandReativo();
    void algConstrutGulRandomizado();


    void caixeiroViajante();

    //void caixeiroViajanteGulosoAux();

    //void caixeiroViajanteGulosoAux(int i);

    void caixeiroViajanteGulosoAux(int i, double &valorSolucaoAtual, double &valorMelhorSolucao, int *tempSolucao,
                                   int *melhorSolucao, bool *visitados);

    void imprimeMatrizDistancia();

    void
    caixeiroViajanteAux(int i, double &valorSolucaoAtual, double &valorMelhorSolucao, int *tempSolucao, int *melhorSolucao,
                        bool *visitados);

    int getCorPB();

    int getCorPB(int idVert);
};


#endif //TRABALHOGRAFOSNOVO_GRAFO_H
