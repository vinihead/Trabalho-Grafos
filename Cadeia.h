//
// Created by rrodri on 26/11/17.
//

#ifndef TRABALHOGRAFOS_CADEIA_H
#define TRABALHOGRAFOS_CADEIA_H

#include <iostream>
#include <vector>
#include "Vertice.h"
#include <sstream> // Fluxo de strings
#include <fstream> // Leitura e escrita de arquivo
#include <cstdlib> //funcao atoi e exit
#include <set>

using namespace std;

class Cadeia
{
private:



    vector<int> vertices;
    int cardinalidadeQ;
    double comprimetoL;

public:
    Cadeia(int vert1, int vert2, double comprimento);
    void setCardinalidade(int c);
    void setComprimento(double c);
    void setVertice(int v);
    void insereVertice(int indiceInsercao, int idVertice, double custo);
    int getCardinalidade();
    double getComprimento();
    int getQuantVertice();

    vector<int> getCadeia()const;
};

#endif //TRABALHOGRAFOS_CADEIA_H
