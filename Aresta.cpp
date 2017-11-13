/*
 *******************************************************************
 *         -----------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */

#include "Aresta.h"

Aresta::Aresta(int idAdj, float peso)
{
    this->idVerticeAdj = idAdj;
    this->peso = peso;
}

int Aresta::getIdAdj()
{
    return idVerticeAdj;
}

float Aresta::getPeso()
{
    return peso;
}