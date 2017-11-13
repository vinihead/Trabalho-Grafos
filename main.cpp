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
 ./gafosGrupo1 instanciaArquivoEntrada TrabalhoGrafos
 Para EXECUTAR, utilize:
 <.\executavel> <arqEntrada> <arqSaida>
 obs: sem utilizar ".txt", apenas o nome do arquivo
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
void excluiAresta(Grafo *grafo);
void excluiVertice(Grafo *grafo);
void adicionaAresta(Grafo *grafo);
void adicionaVertice(Grafo *grafo);
void verificaGrauVertice(Grafo *grafo);
void verificaGrauGrafo(Grafo *grafo);
void informaOrdem(Grafo *grafo);
void verificaTrivial(Grafo *grafo);
void verificaNulo(Grafo *grafo);
void verificaKRegularidade(Grafo *grafo);
void verificaMultigrafo(Grafo *grafo);
void verificaGrafoCompleto(Grafo *grafo);
void verificaBipartido(Grafo *grafo);
void verificaEuleriano(Grafo *grafo);
void mostraVizinhancaAberta(Grafo *grafo);
void mostraVizinhancaFechada(Grafo *grafo);
void apresentaSequenciaGraus(Grafo *grafo);
void fechoTransitivoDireto(Grafo *grafo);
void fechoTransitivoIndireto(Grafo *grafo);
void subgrafoInduzido(Grafo *grafo);
void grafoComplementar(Grafo *grafo);
void verificaVerticesArticulacao(Grafo *grafo);
void verificaArestasPonte(Grafo *grafo);
void componentesFortementeConexas(Grafo *grafo);
void arvoreBuscaProfundidade(Grafo *grafo);
void arvoreBuscaLargura(Grafo *grafo);
void verificaConexo(Grafo *grafo);
void caminhoMinimoDijkstra(Grafo *grafo);
void caminhoMinimoFloyd(Grafo *grafo);

///Variaveis globais criadas para facilitar a escrita de arquivos
ofstream outFile;
ifstream inFile;

int main(int argc, char **argv)
{
    cabecalho();
    int opcao;
    bool digrafo;
    string inFileName;
    string outFileName;
    if(argc == 3)
    {
        inFileName = argv[1];
        outFileName = argv[2];
        //inFileName =  "bipartido";//"infile_1";////"instance1534";//
        //outFileName = "outfile";
        outFileName += ".txt";
        inFileName += ".txt";
        inFile.open(inFileName);
        outFile.open(outFileName);
        ///Testar se os arquivos foram inseridos corretamente
        if(!inFile || !outFile)
        {
            cout << "ERRO! Tente novamente!" << endl;
            cout << "NAO eh necessario informat o \".txt\", informe apenas o nome do arquivo" << endl;
            if(!inFile)
                cerr << "ERRO! O arquivo de ENTRADA \"" << inFileName << "\" NAO foi encontrado!" << endl;
            if(!outFile)
                cout << "ERRO! Nao foi possivel criar o arquivo de SAIDA \"" << outFileName << "\"!" << endl;
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

    do {
        cout << "O Grafo eh Direcionado? Se for, tecle '1', se nao tecle '0': ";
        cin >> opcao;
    }
    while(opcao!=0 && opcao!=1);
    digrafo = (opcao==1);
    Grafo grafo(&inFile, digrafo);

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
                excluiAresta(&grafo);
                break;
            }
            case 4:
            {
                excluiVertice(&grafo);
                break;
            }
            case 5:
            {
                adicionaAresta(&grafo);
                break;
            }
            case 6:
            {
                adicionaVertice(&grafo);
                break;
            }
            case 7:
            {
                verificaGrauVertice(&grafo);
                break;
            }
            case 8:
            {
                verificaGrauGrafo(&grafo);

                break;
            }
            case 9:
            {
                informaOrdem(&grafo);
                break;
            }
            case 10:
            {
                verificaTrivial(&grafo);
                break;
            }
            case 11:
            {
                verificaNulo(&grafo);
                break;
            }
            case 12:
            {
                verificaKRegularidade(&grafo);
                break;
            }
            case 13:
            {
                verificaMultigrafo(&grafo);
                break;
            }
            case 14:
            {
                verificaGrafoCompleto(&grafo);
                break;
            }
            case 15:
            {
                verificaBipartido(&grafo);
                break;
            }
            case 16:
            {
                verificaConexo(&grafo);
                break;
            }
            case 17:
            {
                verificaEuleriano(&grafo);
                break;
            }
            case 18:
            {
                mostraVizinhancaAberta(&grafo);
                break;
            }
            case 19:
            {
                mostraVizinhancaFechada(&grafo);
                break;
            }
            case 20:
            {
                //caminhoMinimoDijkstra(&grafo);
                break;
            }
            case 21:
            {
                //caminhoMinimoFloyd(&grafo);
                break;
            }
            case 22:
            {
                fechoTransitivoDireto(&grafo);
                break;
            }
            case 23:
            {
                fechoTransitivoIndireto(&grafo);
                break;
            }
            case 24:
            {
                apresentaSequenciaGraus(&grafo);
                break;
            }
            case 25:
            {
                subgrafoInduzido(&grafo);
                break;
            }
            case 26:
            {
                grafoComplementar(&grafo);
                break;
            }
            case 27:
            {
                //verificaVerticesArticulacao(&grafo);
                break;
            }
            case 28:
            {
                verificaArestasPonte(&grafo);
                break;
            }
            case 29:
            {
                //raio
                break;
            }
            case 30:
            {
                //diamentro
                break;
            }
            case 31:
            {
                //centro
                break;
            }
            case 32:
            {
                //perifeira
                break;
            }
            case 33:
            {
                componentesFortementeConexas(&grafo);
                break;
            }
            case 34:
            {
                //AGM PRIM
                break;
            }
            case 35:
            {
                //Florestas de custo minimo
                break;
            }
            case 36:
            {
                arvoreBuscaProfundidade(&grafo);
                break;
            }
            case 37:
            {
                arvoreBuscaLargura(&grafo);
                break;
            }
            case 38:
            {
                grafo.geraLinguagemDot();
                break;
            }
            case 39:
            {
                grafo.imprime();
                break;
            }
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
        cout << "---------------------- MENU -------------------------" << endl;
        cout << "|     00 - Sair                                     |" << endl;
        cout << "|     01 - Salvar grafo em arquivo                  |" << endl;
        cout << "|     02 - Salvar grafo em modo lista de adjacencia |" << endl;
        cout << "|     03 - Exclusao de Aresta                       |" << endl;
        cout << "|     04 - Exclusao de Vertice                      |" << endl;
        cout << "|     05 - Adicionar Aresta                         |" << endl;
        cout << "|     06 - Adicionar Vertice                        |" << endl;
        cout << "|     07 - Verificar Grau de um dado Vertice        |" << endl;
        cout << "|     08 - Verificar Grau do Grafo                  |" << endl;
        cout << "|     09 - Verificar Ordem do Grafo                 |" << endl;
        cout << "|     10 - Verificar se eh Grafo Trivial            |" << endl;
        cout << "|     11 - Verificar se eh Grafo Nulo               |" << endl;
        cout << "|     12 - Verificar se o grafo eh k-regular        |" << endl;
        cout << "|     13 - Verificar se o grafo eh Multigrafo       |" << endl;
        cout << "|     14 - Verificar se o grafo eh Completo         |" << endl;
        cout << "|     15 - Verificar se o grafo eh bipartido        |" << endl;
        cout << "|     16 - Verificar se o grafo eh conexo           |" << endl;
        cout << "|     17 - Verificar se eh grafo euleriano          |" << endl;
        cout << "|     18 - Mostraz vizinhanca aberta de um vertice  |" << endl;
        cout << "|     19 - Mostraz vizinhanca fechada de um vertice |" << endl;
        cout << "|     20 - Mostrar Caminho Minimo (Dijkstra)        |" << endl;
        cout << "|     21 - Mostrar Caminho Minimo (Floyd)           |" << endl;
        cout << "|     22 - Fecho transitivo direto de um Vertice    |" << endl;
        cout << "|     23 - Fecho transitivo indireto de um Vertice  |" << endl;
        cout << "|     24 - Apresentar sequencia de graus            |" << endl;
        cout << "|     25 - Apresentar um subgrafo induzido          |" << endl;
        cout << "|     26 - Apresentar o complementar do grafo       |" << endl;
        cout << "|     27 - Apresentar os Vertices de articulacao    |" << endl;
        cout << "|     28 - Apresentar as Arestas ponte              |" << endl;
        cout << "|     29 - Raio do grafo                            |" << endl;
        cout << "|     30 - Diamentro de grafo                       |" << endl;
        cout << "|     31 - Centro do grafo                          |" << endl;
        cout << "|     32 - Periferia do grafo                       |" << endl;
        cout << "|     33 - Componentes fortemente conexas (digrafo) |" << endl;
        cout << "|     34 - Apresentar a AGM do grafo                |" << endl;
        cout << "|     35 - Apresentar as florestas de custo minimo  |" << endl;
        cout << "|     36 - Arvores de busca em profundidade         |" << endl;
        cout << "|     37 - Arvores de busca em largura              |" << endl;
        cout << "|     38 - Gerar Grafo em linguagem DOT             |" << endl;
        cout << "|     39 - Imprimir grafo                           |" << endl;
        cout << "-----------------------------------------------------" << endl << endl;

        if(passouMenu)
            cout << "Opcao Invalida.\nDigite uma opcao de 0 a 39: ";
        else
        {
            passouMenu = true;
            cout << "Digite uma opcao: ";
        }
        cin >> opc;
        cout << endl;
    }while(opc < 0 || opc > 39);
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

void excluiAresta(Grafo *grafo)
{
    int id1, id2;
    cout << "----------------------------" << endl;
    cout << "QUAL ARESTA DESEJA REMOVER?" << endl;
    cout << "Digite o id de origem: ";
    cin >> id1;
    cout << "Digite o id de destino: ";
    cin >> id2;
    if(grafo->removeAresta(id1, id2))
    {
        cout << "Aresta removida com sucesso!" << endl;
        outFile << endl << "Aresta removida: (" << id1 << ", " << id2 << ")" << endl;
    }
    else
        cout << "ERRO! Aresta nao encontrada." << endl;
    cout << "----------------------------" << endl;
}

void excluiVertice(Grafo *grafo)
{
    int id;
    cout << "--------------------------------" << endl;
    cout << "Informe o vertice a remover: ";
    cin >> id;
    if(grafo->removeVertice(id))
    {
        cout << "Vertice Removido." << endl;
        outFile << endl << "Vertice removido: [" << id << "]" << endl;
    }
    else
        cout << "Vertice NAO Existente." << endl;
    cout << "--------------------------------" << endl;
}

void adicionaAresta(Grafo *grafo)
{
    int idDest, idOrig;
    float peso=1.0;
    cout << "-------------------------------------------" << endl;
    cout << "Informa a aresta a adicionar:" << endl;
    cout << "Id de origem: ";
    cin >> idOrig;
    cout << "Id de destino: ";
    cin >> idDest;
    if(grafo->getPonderado())
    {
        cout << "Peso da aresta: ";
        cin >> peso;
    }
    grafo->adicionaAresta(idOrig, idDest, peso);
    cout << "Aresta (" << idOrig << ", " << idDest << ") adicionada."<< endl;
    outFile << endl << "Aresta (" << idOrig << ", " << idDest << ") adicionada."<< endl;
    cout << "-------------------------------------------" << endl;
}

void adicionaVertice(Grafo *grafo)
{
    int idVert;
    float peso=1.0;
    cout << "-------------------------------------------" << endl;
    cout << "Obs: caso o vertice ja exista, ele nao sera readicionado." << endl;
    cout << "Informa o id do vertice a adicionar: ";
    cin >> idVert;
    if(grafo->adicionaVerticeNaMain(idVert))
    {
        cout << "Vertice [" << idVert << "] adicionado."<< endl;
        outFile << endl << "Vertice [" << idVert << "] adicionado."<< endl;
    }
    else
    {
        cout << "Vertice [" << idVert << "] ja exista."<< endl;
    }
    cout << "-------------------------------------------" << endl;
}

void verificaGrauVertice(Grafo *grafo)
{
    int idVertice;
    cout << "-------- Verificar grau de determinado vertice ---------" << endl;
    cout << "Digite o vertice: ";
    cin >> idVertice;
    cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
    int opcao;
    cin >> opcao;
    ///Passa como parametro "true", pois caso precise, habilita para imprimir informacoes do graude de saida e entrada de um digrafo:
    cout << "-------------------- Vertice " << idVertice << " ---------------------------" << endl;
    int grauVertice = grafo->grauVertice(idVertice, true, &outFile, (opcao==1));
    if(grauVertice != -1)
    {
        outFile << endl << "GrauVertice[" << idVertice << "]: " << grauVertice << endl;
        if(opcao==1)
            cout << "Grau do vertice " << idVertice << ": " << grauVertice << endl;
        cout << "Os dados foram salvos no arquivo de saida." << endl;
    }
    else
        cout << "Vertice "<< idVertice << " NAO encontrado." << endl;
    cout << "----------------------------------------------------------------" << endl;
}

void informaOrdem(Grafo *grafo)
{
    unsigned int ordemGrafo = grafo->getOrdem();
    cout << "------------------ ORDEM DO GRAFO ------------------" << endl;
    outFile << endl << "Ordem do Grafo: " << ordemGrafo << endl;
    cout << "Deseja visualizar a saida? (tecle '1' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Ordem do grafo: " << ordemGrafo << endl;
    cout << "Verificado e salvo." << endl;
    cout << "----------------------------------------------------" << endl;
}

void verificaTrivial(Grafo *grafo)
{
    bool trivial = grafo->verificaTrivial();
    cout << "----------------------------------------------------" << endl;
    outFile << endl << "Grafo Trivial: " << (trivial?"SIM":"NAO") << endl;
    cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Grafo Trivial: " << (trivial?"SIM":"NAO") << endl;
    cout << "Verificado e salvo." << endl;
    cout << "----------------------------------------------------" << endl;
}

void verificaNulo(Grafo *grafo)
{
    bool nulo = grafo->verificaNulo();
    cout << "------------------- VERIFICA NULO -------------------" << endl;
    outFile << endl << "Grafo Nulo: " << (nulo?"SIM":"NAO") << endl;
    cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Grafo Nulo: " << (nulo?"SIM":"NAO") << endl;
    cout << "Verificado e salvo." << endl;
    cout << "----------------------------------------------------" << endl;
}

void verificaKRegularidade(Grafo *grafo)
{

    int k;
    cout << "----------------- VERIFICA K-REGULARIDADE -------------------" << endl;
    cout << "Verifica K-Regularidade de um grafo:" << endl;
    cout << "Digite o K: ";
    cin >> k;
    bool kRegular = grafo->kRegularidade(k);
    outFile << endl << "Grafo " << k << "-Regular: " << (kRegular?"SIM":"NAO") << endl;
    cout << "Deseja visualizar a saida? (tecle '1' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Grafo " << k << "-Regular: " << (kRegular?"SIM":"NAO") << endl;
    cout << "Verificado e salvo." << endl;
    cout << "-------------------------------------------------------------" << endl;
}

void verificaGrauGrafo(Grafo *grafo)
{
    unsigned int grauGrafo = grafo->getGrau();
    cout << "--------------- VERIFICA GRAU DO GRAFO ----------------" << endl;
    outFile << endl <<"Grau do grafo: " << grauGrafo << endl;
    cout << "Deseja visualizar a saida? (tecle '1' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Grau do grafo: " << grauGrafo << endl;
    cout << "Verificado e salvo no arquivo de saida." << endl;
    cout << "----------------------------------------------------" << endl;
}

void mostraVizinhancaAberta(Grafo *grafo)
{
    string dados;
    int idVert;
    cout << "--------------- MOSTRAR VIZINHANCA ABERTA ----------------" << endl;
    cout << "Digite o vertice desejado: ";
    cin >> idVert;
    dados = grafo->vizinhacaAberta(idVert);
    if(dados.empty())
        cout << "Vertice [" << idVert << "] nao existe." << endl;
    else
    {
        outFile << endl << dados << endl;
        cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
        int opcao;
        cin >> opcao;
        if(opcao == 1) cout << endl << dados << endl;
        cout << "Dados salvos no arquivo de saida." << endl;
    }
    cout << "----------------------------------------------------------" << endl;
}

void mostraVizinhancaFechada(Grafo *grafo)
{
    string dados;
    int idVert;
    cout << "--------------- MOSTRAR VIZINHANCA FECHADA ----------------" << endl;
    cout << "Digite o vertice desejado: ";
    cin >> idVert;
    dados = grafo->vizinhacaFechada(idVert);
    if(dados.empty())
        cout << "Vertice [" << idVert << "] nao existe." << endl;
    else
    {
        outFile << endl << dados << endl;
        cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
        int opcao;
        cin >> opcao;
        if(opcao == 1) cout << endl << dados << endl;
        cout << "Dados salvos no arquivo de saida." << endl;
    }
    cout << "----------------------------------------------------------" << endl;
}

void apresentaSequenciaGraus(Grafo *grafo)
{
    cout << "------------ APRESENTA SEQUENCIA DE GRAUS --------------------" << endl;
    cout << "Sequencia de graus dos vertices:" << endl;
    cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
    int opcao;
    cin >> opcao;
    grafo->apresentaSequenciaGraus(&outFile, (opcao == 1));
    cout << "Salvo no arquivo de saida." << endl;
    cout << "--------------------------------------------------------------" << endl;
}

void fechoTransitivoDireto(Grafo *grafo)
{
    string fechoTransDireto;
    int idVert;
    cout << "--------------- FECHO TRANSITIVO DIRETO -----------------" << endl;
    cout << "Digite o vertice que ira partir: ";
    cin >> idVert;

    fechoTransDireto = grafo->fechoTransitivoDireto(idVert);
    outFile << "Fecho transitivo direto, apartir do vertice " << idVert << ":" << endl;
    outFile << endl <<fechoTransDireto << endl;
    cout << "Os dados foram salvos no arquivo de saida" << endl;
    cout << "---------------------------------------------------------" << endl;
}

void fechoTransitivoIndireto(Grafo *grafo)
{
    string fechoTransIndireto;
    int idVert;
    cout << "--------------- FECHO TRANSITIVO INDIRETO -----------------" << endl;
    cout << "Digite o vertice que ira partir: ";
    cin >> idVert;
    //fechoTransIndireto = grafo->fechoTransitivoIndireto(idVert);
    cout << endl <<fechoTransIndireto << endl;
    cout << "-----------------------------------------------------------" << endl;
}

void verificaMultigrafo(Grafo *grafo)
{
    bool multigrafo = grafo->verificaMultigrafo();
    outFile << endl << "Multigrafo: " << (multigrafo?"SIM":"NAO") << endl;
    cout << "\n\n-------- VERIFICA MULTIGRAFO -----------\n";
    cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Multigrafo: " << (multigrafo?"SIM":"NAO") << endl;
    cout << "Verificado e salvo." << endl;
    cout << "----------------------------------------\n";
}

void verificaGrafoCompleto(Grafo *grafo)
{
    bool completo = grafo->verificaCompleto();
    outFile << endl << "Grafo Completo: " << (completo?"SIM":"NAO") << endl;
    cout << "\n----------- VERIFICA GRAFO COMPLETO -----------------\n";
    cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Grafo Completo: " << (completo?"SIM":"NAO") << endl;
    cout << "Verificado e salvo." << endl;
    cout << "---------------------------------------------------------\n";


}

void verificaBipartido(Grafo *grafo)
{
    bool bipartido = grafo->verificaBipartido();
    outFile << endl << "Grafo Bipartido: " << (bipartido?"SIM":"NAO") << endl;
    cout << "\n----------- VERIFICA GRAFO BIPARTIDO -----------------\n";
    cout << "Deseja visualizar a saida? (tecle '1' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Grafo Bipartido: " << (bipartido?"SIM":"NAO") << endl;
    cout << "Verificado e salvo." << endl;
    cout << "----------------------------------------------------------\n";
}

void verificaEuleriano(Grafo *grafo)
{
    bool euleriano = grafo->verificaEuleriano();
    outFile << endl << "Grafo Euleriano: " << (euleriano?"SIM":"NAO") << endl;
    cout << "\n----------- VERIFICA GRAFO EULERIANO -----------------\n";
    cout << "Deseja visualizar a saida? (tecle '1' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Grafo Euleriano: " << (euleriano?"SIM":"NAO") << endl;
    cout << "Verificado e salvo." << endl;
    cout << "----------------------------------------------------------\n";
}

void subgrafoInduzido(Grafo *grafo) {
    Grafo *subgrafo;
    int opcao, idVertice;
    unsigned int qntdVertices;
    cout << "----------------------- SUBGRAFO INDUZIDO -----------------------" << endl;
    outFile << endl << "------------- SUBGRAFO INDUZIDO ----------------" << endl;
    cout << "SUBGRAFO INDUZIDO POR UM SUBCONJUNTO DE VERTICES" << endl;
    cout << "Digite a quantidade de vertices: ";
    cin >> qntdVertices;
    vector<int> subconjuntoVertices(qntdVertices);
    cout << "Digite os vertices: " << endl;
    ///Leitura dos vertices com cuidado, verificando se o vertice existe e se ja esta na sequencia
    for (int i = 0; i < qntdVertices; i++)
    {
        cout << "Vertice " << i << ": ";
        cin >> idVertice;
        ///verifica se o vertice existe
        if (grafo->procuraVertice(idVertice))
        {
            ///verifica se o vertice ja esta na sequencia
            auto it = find (subconjuntoVertices.begin(), subconjuntoVertices.end(), idVertice);
            if (it == subconjuntoVertices.end())
            {
                subconjuntoVertices[i] = idVertice;
            }
            else
            {
                cout << "EROO: Vertice [" << i << "]=" << idVertice << " ja esta na sequencia! Digite outro." << endl;
                i--;
            }
         }
        else
        {
            cout << "EROO: Vertice [" << i << "]=" << idVertice << " inexistente! Digite outro." << endl;
            i--;
        }
    }
    sort(subconjuntoVertices.begin(), subconjuntoVertices.end()); ///Ordena o subconjunto de vertices
    outFile << "Subgrafo Induzido pelo subconjunto de Vertices: {";
    for(auto it = subconjuntoVertices.begin(); it != subconjuntoVertices.end(); it++)
    {
        if(++it==subconjuntoVertices.end())
            outFile << *(--it) << "}" << endl;
        else
            outFile << *(--it) << ", ";
    }
    subgrafo = grafo->subgrafoInduzido(subconjuntoVertices);
    subgrafo->saveGrafo(&outFile);
    cout << endl << "Deseja visualizar o grafo? (tecle \'1\' para SIM): ";
    cin >> opcao;
    if(opcao == 1) subgrafo->imprime();
    cout << "Os dados forams salvos no arquivo de saida." << endl;
    outFile << "--------------------------------------------------------" << endl;
    cout << "------------------------------------------------------" << endl;
    delete subgrafo;
}

void grafoComplementar(Grafo *grafo)
{
    int opcao;
    Grafo * grafoComplementar;
    grafoComplementar = grafo->complementar();
    cout << "------------- GRAFO COMPLEMENTAR -------------" << endl;
    outFile << endl << "------------- GRAFO COMPLEMENTAR -------------" << endl;
    grafoComplementar->saveGrafo(&outFile);
    cout << "Grafo salvo no arquivo de saida." << endl;
    cout << "Deseja visualizar o grafo? (tecle \'1\' para SIM): ";
    cin >> opcao;
    if(opcao == 1)
        grafoComplementar->imprime();
    outFile << "----------------------------------------------" << endl;
    cout << "----------------------------------------------" << endl;
    delete grafoComplementar;

}

void componentesFortementeConexas(Grafo *grafo)
{
//    grafo->componentesFortementeConexas();
}

void arvoreBuscaProfundidade(Grafo *grafo)
{
    int idVert;
    string msg;
    cout << "------------- ARVORE DE BUSCA EM PROFUNDIDADE -------------" << endl;
    cout << "Obs1: Esse algoritmo so consegue fazer a busca em um grafo conexo" << endl;
    cout << "Obs2: Ou apenas na componente conexa do Vertice raiz." << endl;
    cout << "-> Id do vertice raiz: ";
    cin >> idVert;
    if(grafo->procuraVertice(idVert))
    {
        string textoSalvar = grafo->arvoreBuscaProfundidade(idVert);
        outFile << endl << textoSalvar << endl;
        cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
        int opcao;
        cin >> opcao;
        if(opcao == 1)
            cout << textoSalvar << endl;
        cout << "Salvo no arquivo de saida." << endl;
    }
    else
    {
        cout << "Vertice NAO Encontrado!" << endl;
    }
    cout << "-------------------------------------------" << endl;

}

void arvoreBuscaLargura(Grafo *grafo)
{
    int idVert;
    string msg;
    cout << "----- ARVORE DE BUSCA EM LARGURA -----" << endl;
    //cout << "Obs1: Esse algoritmo so consegue fazer a busca em um grafo conexo" << endl;
    //cout << "Obs2: Ou apenas na componente conexa do Vertice raiz." << endl;
    cout << "-> Id do vertice raiz: ";
    cin >> idVert;
    if(grafo->procuraVertice(idVert))
    {
        string textoSalvar = grafo->arvoreBuscaLargura(idVert);
        outFile << endl << textoSalvar << endl;
        cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
        int opcao;
        cin >> opcao;
        if(opcao == 1)
            cout << textoSalvar << endl;
        cout << "Salvo no arquivo de saida." << endl;
    }
    else
    {
        cout << "Vertice NAO Encontrado!" << endl;
    }
    cout << "-------------------------------------------" << endl;
}

void verificaConexo(Grafo *grafo)
{
    bool conexo = grafo->verificaConexo();
    cout << "------------- VERIFICA GRAFO CONEXO -------------" << endl;
    outFile << endl << "Grafo Conexo: " << (conexo?"SIM":"NAO") << endl;
    cout << "Deseja visualizar a saida? (tecle \'1\' para SIM): ";
    int opcao;
    cin >> opcao;
    if(opcao == 1) cout << endl << "Grafo Conexo: " << (conexo?"SIM":"NAO") << endl;
    cout << "Verificado e salvo." << endl;
    cout << "-------------------------------------------------" << endl;
}

void verificaVerticesArticulacao(Grafo *grafo)
{
    cout << "---------- VERIFICA VERTICES DE ARTICULACAO -----------" << endl;
    outFile << "---------- VERIFICA VERTICES DE ARTICULACAO -----------" << endl;
    string vetorArticulacoes = grafo->verificaVerticesArticulacao();
    cout << vetorArticulacoes << endl;
    cout << "-------------------------------------------------------" << endl;
    outFile << "-------------------------------------------------------" << endl;
}

void verificaArestasPonte(Grafo *grafo)
{
    cout << "---------- VERIFICA ARESTAS PONTE -----------" << endl;
    outFile << "---------- VERIFICA ARESTAS PONTE -----------" << endl;
    grafo->verificaArestasPonte(&outFile);
    cout << "---------------------------------------------" << endl;
    outFile << "---------------------------------------------" << endl;
}
/*
void caminhoMinimoDijkstra(Grafo *grafo)
{
    cout << "Dijkstra: " << grafo->caminhoMinimoDijkstra(1,7) << endl;
    cout << "Verificado e salvo." << endl;
}
*/
void caminhoMinimoFloyd(Grafo *grafo)
{
    cout << "Floyd: " << grafo->caminhoMinimoFloyd(1,7) << endl;
    cout << "Verificado e salvo." << endl;
}
