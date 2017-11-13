/*
 *******************************************************************
 *         -----------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */

#ifndef TRABALHOGRAFOSNOVO_ARESTA_H
#define TRABALHOGRAFOSNOVO_ARESTA_H


#include "Vertice.h"

class Vertice;

class Aresta {
private:
    float peso;
    int idVerticeAdj;

public:
    Aresta(int idAdj, float peso);
    int getIdAdj();
    float getPeso();
};


#endif //TRABALHOGRAFOSNOVO_ARESTA_H
