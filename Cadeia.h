//
// Created by rrodri on 26/11/17.
//

#ifndef TRABALHOGRAFOS_CADEIA_H
#define TRABALHOGRAFOS_CADEIA_H

#include <vector>

using namespace std;

class Cadeia
{
private:
    vector<int> vertices;
    int cardinalidadeQ;
    double comprimetoL;
public:
    Cadeia(int vert1, int vert2, double comprimento);
    void insereVertice(int indiceInsercao, int idVertice, double custo);
    int getCardinalidade();
    double getComprimento();
    vector<int> getCadeia()const;
};

#endif //TRABALHOGRAFOS_CADEIA_H
