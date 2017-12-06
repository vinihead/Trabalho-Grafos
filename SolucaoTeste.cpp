//
// Created by viniman on 12/5/17.
//

#include "SolucaoTeste.h"



/**
 *
 * @param cadeia
 * @param custoInsercao
 * @param dist1
 * @param dist2
 * @return
 */
bool SolucaoTeste::viabilidade(int indiceCadeia, double custoInsercao) const
{
    return cadeias[indiceCadeia].getComprimento()+custoInsercao<=maxCusto && cadeias[indiceCadeia].getCardinalidade()<maxVertBranco;///dist1+dist2<maxCusto && nao precisa disso
}

/**
 *
 * @param dist1
 * @param dist2
 * @return
 */
bool SolucaoTeste::viabilidade(double dist1, double dist2) const
{
    return dist1<maxCusto && dist2<maxCusto;
}