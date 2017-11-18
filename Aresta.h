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
    double peso;
    int idVerticeAdj;

public:
    Aresta(int idAdj, double peso);
    int getIdAdj();
    double getPeso();
};


#endif //TRABALHOGRAFOSNOVO_ARESTA_H
