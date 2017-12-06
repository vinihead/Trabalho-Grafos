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
 <.\executavel> <arqEntrada>
 obs: sem utilizar ".txt", apenas o nome do arquivo
 */

/**
Consideramos um grafo nao direcionado e completo
 *
 */

#include <iostream>
#include <iomanip>
#include "Grafo.h"

using namespace std;

int menuShow();
void cabecalho();
void informacoesInstancia(Grafo *grafo, const string &in, const string &out);
void escreveCabecalhoArquivo(ofstream *outFile, Grafo *grafo, const string &str);
void pausarTela(bool continuar);
void finalizaPrograma(ifstream *inFile, ofstream *outFile);

int main(int argc, char **argv)
{
    cabecalho();
    ofstream outFile;
    ifstream inFile;
    string inFileName;
    string outFileName;

    if(argc != 2)
    {
        cerr << "Erro na chamada do programa. Informar corretamente o nome da instancia (arquivo de entrada)." << endl;
        cerr << "Formato a inserir na linha de comando para execucao do algoritmo:" << endl;
        cerr << "<./executavel> <arqEntrada>" << endl;
        finalizaPrograma(&inFile, &outFile);
        return -1;
    }

    inFileName = argv[1];
    if (inFileName.compare(inFileName.size() - 4, 4, ".txt") != 0)
    {
        outFileName = inFileName + "_resultados.txt";
        inFileName += ".txt";
    }
    else
    {
        outFileName = inFileName;
        outFileName.insert(inFileName.size()-4,"_resultados");
    }
    inFile.open(inFileName);
    ///Testar se os arquivos foram inseridos corretamente
    if(!inFile) {
        cerr << "ERRO! Tente novamente!" << endl;
        cerr << "Desculpe, mas aconteceu algo inesperado." << endl;
        cerr << "A instancia digitada foi: \"" << inFileName << "\"." << endl;
        cerr << "VERIFIQUE se digitou o nome da instancia corretamente." << endl;
        cerr << "Pode-se digitar o nome da instancia com \".txt\" ou sem." << endl;
        cerr << "Obs: a instancia precisa estar na mesma pasta de execucao do programa" << endl;
        finalizaPrograma(&inFile, &outFile);
        return -1;
    }
    outFile.open(outFileName);
    if(!outFile)
    {
        cerr << "ERRO! Nao foi possivel criar/editar o arquivo de SAIDA \"" << outFileName << "\"!" << endl;
        cerr << "Erro na abertura/criação do arquivo de saida" << endl;
        cerr << "Verifique se a instancia foi digitada corretamente\n";
        cerr << "Ou se voce possui permissao para criar/editar o arquivo de saida\n\n" << endl;
        finalizaPrograma(&inFile, &outFile);
        return -1;
    }




    Grafo grafo(&inFile);
    escreveCabecalhoArquivo(&outFile, &grafo, inFileName);
    int opcao;
    do {
        opcao = menuShow();
        switch (opcao) {
            case 0:
            {
                finalizaPrograma(&inFile, &outFile);
                break;
            }
            case 1:
            {
                cout << "Algoritmo Guloso." << endl;
                grafo.algConstrutGuloso(&outFile);
                break;
            }
            case 2:
            {
                cout << "Algoritmo Guloso Randomizado." << endl;
                int opc=1;
                do {
                    cout << "|--------------- MENU ----------------|" << endl;
                    cout << "|------ ESCOLHA O ALFA DESEJADO ------|" << endl;
                    cout << "|-------------------------------------|" << endl;
                    cout << "||     01 - Alfa 0.1                 ||" << endl;
                    cout << "||     02 - Alfa 0.2                 ||" << endl;
                    cout << "||     03 - Alfa 0.3                 ||" << endl;
                    cout << "|-------------------------------------|" << endl;
                    if(opc==1||opc==2||opc==3)
                        cout << "Opcao: ";
                    else
                        cout <<"Digite a opcao adequadamente\nOpcao: ";
                    cin >> opc;
                    switch (opc){
                        case 1:
                            grafo.algConstrutGulRandomizado(0.1, &outFile);
                            break;
                        case 2:
                            grafo.algConstrutGulRandomizado(0.2, &outFile);
                            break;
                        case 3:
                            grafo.algConstrutGulRandomizado(0.3, &outFile);
                            break;
                    }
                }while(opc!=1&&opc!=2&&opc!=3);
                break;
            }
            case 3:
            {
                cout << "Algoritmo Guloso Randomizado Reativo." << endl;
                grafo.algConstrutGulRandReativo(&outFile);
                break;
            }
            case 4:
            {
                informacoesInstancia(&grafo, inFileName, outFileName);
                break;
            }
            /*case 5:
            {
                cout << "Matriz de Distancias:" << endl;
                grafo.imprimeMatrizDistancia();
                break;
            }
            case 6:
            {
                grafo.imprime();
                break;
            }*/
            default:
                break;
        }
    } while (opcao != 0);
    ///Fechando streams de entrada e saida
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
        cout << "|----------------------------------------------------------|" << endl;
        cout << "|------ PROBLEMA DO CAIXEIRA VIAJANTE PRETO E BRANCO ------|" << endl;
        cout << "|-------------------------- MENU --------------------------|" << endl;
        cout << "||     00 - Sair                                          ||" << endl;
        cout << "||     01 - Algoritmo do PCVPB Guloso                     ||" << endl;
        cout << "||     02 - Algoritmo do PCVPB Guloso Randomizado         ||" << endl;
        cout << "||     03 - Algoritmo do PCVPB Guloso Randomizado Reativo ||" << endl;
        cout << "||     04 - Informacoes sobre a instancia                 ||" << endl;
        //cout << "||     05 - Imprime Matriz de distancias                  ||" << endl;
        //cout << "||     06 - Imprimir grafo                                ||" << endl;
        cout << "|----------------------------------------------------------|" << endl << endl;
        if(passouMenu)
            cout << "Opcao Invalida.\nDigite uma opcao de 0 a 6: ";
        else
        {
            passouMenu = true;
            cout << "Digite uma opcao: ";
        }
        cin >> opc;
        cout << endl;
    }while(opc < 0 || opc > 4); //6
    return opc;
}

void cabecalho()
{
    cout << "   --------------------------------------------" << endl;
    cout << "   -------  -   Trabalho de Grafos   -  -------" << endl;
    cout << "   -------   ---      Fase 2      ---   -------" << endl;
    cout << "   --------------------------------------------" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "---------       ---   GRUPO 5   ---      ---------" << endl;
    cout << "---------          -> AUTORES <-         ---------" << endl;
    cout << "-------- Ruan Lucas de Oliveira Rodrigues --------" << endl;
    cout << "-------------- Matricula: 201635005 --------------" << endl;
    cout << "----------  Vinicius Carlos de Oliveira  ---------" << endl;
    cout << "-------------- Matricula: 201635025 --------------" << endl;
    cout << "--------------------------------------------------" << endl << endl;
    //cout << "Tecle <Enter> para continuar o algoritmo...." << endl;
    //getchar();
    //pausarTela(true);
}

void escreveCabecalhoArquivo(ofstream *outFile, Grafo *grafo, const string &str)
{
    *outFile << "Trabalho de Teoria dos Grafos" << endl;
    *outFile << "Autores:" << endl;
    *outFile << "Vinicius Carlos de Oliveira - Matricula: 201635025" << endl;
    *outFile << "Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005" << endl << endl;
    *outFile  << endl << "Problema do Caixeiro Viajante Preto e Branco" << endl;
    *outFile << "-> ANALISE DO GRAFO PCVPB" << endl << endl;

    *outFile  << "Nome da instância: " << str << endl;
    *outFile  << "Grafo completo, ponderado e com vértices classificados em pretos e brancos." << endl;
    *outFile  << "Ordem do Grafo: " << grafo->getOrdem() << endl;
    *outFile  << "Número de vértices pretos (P): " << grafo->getNumPretos() << endl;
    *outFile  << "Número de vértices brancos (B): " << grafo->getOrdem()-grafo->getNumPretos() << endl;
    *outFile  << "Restrições (entre dois vértices pretos):\n";
    *outFile  << "Máxima Cardinalidade (Q): " << grafo->getMaxVertBranco() << endl;
    *outFile  << "Máximo Comprimento (L): " << grafo->getMaxCusto() << endl;
}

void pausarTela(bool continuar)
{

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    cout << endl << "Pressione <Enter> para " << (continuar ? "continuar" : "fechar");
    cout << " o algoritmo...." << endl << endl;
    clearerr(stdin);
    getchar();

}

void finalizaPrograma(ifstream *inFile, ofstream *outFile)
{

    cout << "\n\n         --             FIM DO PROGRAMA            --" << endl;
    cout << "           Os resultados dos testes foram salvos no" << endl;
    cout << "           arquivo \"nomeDaInstancia_resultados.txt\"" << endl;
    cout << "\n-------------------- ALGORITMO FINALIZADO -------------------" << endl;
    inFile->close();
    outFile->close();
    pausarTela(false);
    exit(-1);
}

void informacoesInstancia(Grafo *grafo, const string &in, const string &out)
{
    cout << "GRAFO -> INSTANCIA PCVPB" << endl;
    cout << "Nome: " << in << endl;
    cout << "Grafo completo, ponderado, com vértices classificados em pretos e brancos." << endl;
    cout << "Ordem do Grafo: " << grafo->getOrdem() << endl;
    cout << "Número de vértices pretos (P): " << grafo->getNumPretos() << endl;
    cout << "Número de vértices brancos (B): " << grafo->getOrdem()-grafo->getNumPretos() << endl;
    cout << "Restrições (entre dois vértices pretos):\n";
    cout << "Máxima Cardinalidade (Q): " << grafo->getMaxVertBranco() << endl;
    cout << "Máximo Comprimento (L): " << grafo->getMaxCusto() << endl;
    cout << "Arquivo de entrada: " << in << endl;
    cout << "Arquivo de saida: " << out << endl << endl;
}
