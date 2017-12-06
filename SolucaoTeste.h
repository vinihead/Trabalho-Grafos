//
// Created by viniman on 12/5/17.
//

#ifndef TRABALHOGRAFOS_SOLUCAOTESTE_H
#define TRABALHOGRAFOS_SOLUCAOTESTE_H


#include "Cadeia.h"

class SolucaoTeste {
private:
    vector<Cadeia> cadeias;
    vector<int> listVertices;
    double custo;
    float alfa;
    double maxCusto;
    int maxVertBranco;
public:

    bool viabilidade(double dist1, double dist2) const;

    bool viabilidade(int indiceCadeia, double custoInsercao) const;
};


#endif //TRABALHOGRAFOS_SOLUCAOTESTE_H
