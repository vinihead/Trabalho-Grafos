/*
 *******************************************************************
 *         -----------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */

/**
 Para COMPILAR, uitlize:
 g++ *.cpp -o TrabalhoGrafos
 Para EXECUTAR, utilize:
 <.\executavel> <arqEntrada> <arqSaida>
 obs: sem utilizar ".txt", apenas o nome do arquivo
 */

/**
Consideramos um grafo nao direcionado e completo
 *
 */

#include <iostream>
#include <algorithm>
#include "Grafo.h"

using namespace std;

int menuShow();
void cabecalho();
void escreveCabecalhoArquivo();
void pausarTela(bool continuar);
void finalizaPrograma();

int main(int argc, char **argv)
{
    cabecalho();
    int opcao;
    bool digrafo;
    ofstream outFile;
    ifstream inFile;
    string inFileName;
    string outFileName;
    if(argc == 3)
    {
        inFileName = argv[1];
        outFileName = argv[2];
        outFileName = "outfile";
        outFileName += ".txt";
        inFileName += ".txt";
        inFile.open(inFileName);
        outFile.open(outFileName);
        ///Testar se os arquivos foram inseridos corretamente
        if(!inFile || !outFile)
        {
            cerr << "ERRO! Tente novamente!" << endl;
            cerr << "NAO eh necessario informat o \".txt\", informe apenas o nome do arquivo" << endl;
            if(!inFile)
                cerr << "ERRO! O arquivo de ENTRADA \"" << inFileName << "\" NAO foi encontrado!" << endl;
            if(!outFile)
                cerr << "ERRO! Nao foi possivel criar o arquivo de SAIDA \"" << outFileName << "\"!" << endl;
            pausarTela(false);
            exit(30);
        }
    }
    else
    {
        cerr << "Erro na chamada do programa. Informar corretamente o arquivo de entrada e de saida." << endl;
        cerr << "Obs: nao eh necessario informar a extensao \".txt\" do arquivo." << endl;
        cerr << "Formato a inserir na linha de comando para execucao do algoritmo:" << endl;
        cerr << "<.\\executavel> <arqEntrada> <arqSaida>" << endl;
        pausarTela(false);
        return -1;
    }
    escreveCabecalhoArquivo();
    cout << "Arquivo de entrada: " << inFileName << endl;
    cout << "Arquivo de saida: " << outFileName << endl << endl;

    Grafo grafo(&inFile);
    do {
        opcao = menuShow();
        switch (opcao) {
            case 0:
            {
                outFile << "\n\n---------------- GRAFO FINAL RESULTANTE ----------------" << endl;
                grafo.saveGrafo(&outFile);
                outFile << "\n------------------ ALGORITMO FINALIZADO ------------------" << endl;
                finalizaPrograma();
                break;
            }
            case 1:
            {
                grafo.saveGrafo(&outFile);
                break;
            }
            case 2:
            {
                grafo.saveGrafoAdjacencia(&outFile);
                break;
            }
            case 3:
            {
                cout << "Matriz de Distancias:" << endl;
                grafo.imprimeMatrizDistancia();
                break;
            }
            case 4:
            {
                cout << "Chamando Algoritmo GULOSO." << endl;
                grafo.algConstrutGuloso();
                break;
            }
            case 5:
            {
                cout << "Chamando Algoritmo GULOSO RANDOMIZADO." << endl;
                grafo.algConstrutGulRandomizado();
                break;
            }
            case 6:
            {
                cout << "Chamando Algoritmo GULOSO RANDOMIZADO REATIVO." << endl;
                grafo.algConstrutGulRandReativo();
                break;
            }
            case 7:
            {
                cout << "Caixeiro viajante Solucao normal" << endl;
                grafo.caixeiroViajante();
                break;
            }
            case 8:
            {
                grafo.geraLinguagemDot();
                break;
            }
            case 9:
            {
                grafo.imprime();
                break;
            }
            case 10:
                grafo.caminhoMinimoDijkstra(4,2);
                break;
            default:
                break;
        }
    } while (opcao != 0);
    ///Fechando streams de entrada e saida
    inFile.close();
    outFile.close();
    return 0;
}



int menuShow()
{
    int opc;
    bool passouMenu = false;
    do
    {
        if(!passouMenu)
            pausarTela(true);
        cout << "------ PROBLEMA DO CAIXEIRA VIAJANTE PRETO E BRANCO ------" << endl;
        cout << "-------------------------- MENU --------------------------" << endl;
        cout << "|     00 - Sair                                          |" << endl;
        cout << "|     01 - Salvar grafo em arquivo                       |" << endl;
        cout << "|     02 - Salvar grafo em modo lista de adjacencia      |" << endl;
        cout << "|     03 - Imprime Matriz de distancias                  |" << endl;
        cout << "|     04 - Algoritmo do PCVPB Guloso                     |" << endl;
        cout << "|     05 - Algoritmo do PCVPB Guloso Randomizado         |" << endl;
        cout << "|     06 - Algoritmo do PCVPB Guloso Randomizado Reativo |" << endl;
        cout << "|     07 - Algoritmo PCV                                 |" << endl;
        cout << "|     08 - Gerar Grafo em linguagem DOT                  |" << endl;
        cout << "|     09 - Imprimir grafo                                |" << endl;
        cout << "|     10 -                                               |" << endl;
        cout << "----------------------------------------------------------" << endl << endl;
        if(passouMenu)
            cout << "Opcao Invalida.\nDigite uma opcao de 0 a 39: ";
        else
        {
            passouMenu = true;
            cout << "Digite uma opcao: ";
        }
        cin >> opc;
        cout << endl;
    }while(opc < 0 || opc > 10);
    return opc;
}

void cabecalho()
{
    cout << "   --------------------------------------------" << endl;
    cout << "   -------  -   Trabalho de Grafos   -  -------" << endl;
    cout << "   -------   ---      Fase 1      ---   -------" << endl;
    cout << "   --------------------------------------------" << endl << endl;

    cout << "--------------------------------------------------" << endl;
    cout << "---------       ---   GRUPO 5   ---      ---------" << endl;
    cout << "---------          -> AUTORES <-         ---------" << endl;
    cout << "-------- Ruan Lucas de Oliveira Rodrigues --------" << endl;
    cout << "-------------- Matricula: 201635005 --------------" << endl;
    cout << "----------  Vinicius Carlos de Oliveira  ---------" << endl;
    cout << "-------------- Matricula: 201635025 --------------" << endl;
    cout << "--------------------------------------------------" << endl << endl;

    cout << "Tecle <Enter> para continuar o algoritmo...." << endl;
    getchar();

}

void escreveCabecalhoArquivo()
{
    outFile << "Trabalho de Introducao a Teoria dos Grafos" << endl;
    outFile << "Autores:" << endl;
    outFile << "Vinicius Carlos de Oliveira - Matricula: 201635025" << endl;
    outFile << "Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005" << endl;
    outFile << endl << "-> ANALISE DO GRAFO" << endl << endl;
}

void pausarTela(bool continuar)
{

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    cout << endl << "Pressione <Enter> para " << (continuar ? "continuar" : "finalizar");
    cout << " o algoritmo...." << endl << endl;
    clearerr(stdin);
    getchar();

}

void finalizaPrograma()
{
    cout << "Algoritmo finalizado." << endl;
    cout << "Os dados foram armazenados no arquivo de saida." << endl;
    pausarTela(false);
}