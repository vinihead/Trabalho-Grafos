/*
 *******************************************************************
 *         -----------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */

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
    int getCardinalidade()const;
    double getComprimento()const;
    vector<int> getCadeia()const;
};

#endif //TRABALHOGRAFOS_CADEIA_H
