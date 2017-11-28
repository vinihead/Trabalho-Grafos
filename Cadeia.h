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
    vector<int> vert;
    int cardinalidadeQ;
    double comprimetoL;

public:
    void setCardinalidade(int c);
    void setComprimento(double c);
    void setVertice(int v);
    int getCardinalidade();
    double getComprimento();
    int getQuantVertice();
};

#endif //TRABALHOGRAFOS_CADEIA_H
