/*
 *******************************************************************
 *       -------------    TRABALHO GRAFOS    -------------         *
 *               --------     AUTORES     --------                 *
 *     Ruan Lucas de Oliveira Rodrigues - Matricula: 201635005     *
 *        Vinicius Carlos de Oliveira - Matricula: 201635025       *
 *******************************************************************
 */


#include <algorithm>
#include "Grafo.h"
//#include <set>
#include <queue>
#include <climits>
#include <cmath>
#include <iomanip>

#define INFINITO LONG_MAX

///Constantes usadas no problema do caixeiro viajante preto e branco
#define PRETO 0
#define BRANCO 1

//Construtor padrão, para no caso de precisar criar um grafo vazio.
Grafo::Grafo(bool digrafo, bool ponderado, double maxCusto, int maxVertBranco)
        : maxCusto(maxCusto), maxVertBranco(maxVertBranco){
    this->digrafo = digrafo;
    this->ponderado = ponderado;
    this->ordem = 0;
}


//Já cria o grafo lendo do arquivo de entrada
Grafo::Grafo(ifstream *inFile)
{
    this->digrafo = false;
    this->ponderado = true;
    int vert, x, y;
    int numVertices;
    //float peso = 1.0;
    char corPB;
    string line;
    stringstream copiaDados;
    getline(*inFile, line);
    copiaDados.str(line);
    copiaDados >> numVertices >> numPretos >> maxVertBranco >> maxCusto;
    ordem = 0;

    cout << "--------------------------------------" << endl;
    cout << "Instanciando o grafo do PCVPB. . . . ." << endl;
    cout << "--------------------------------------" << endl;
    cout <<  "linha de fora: "<<line << endl;
    getline(*inFile, line); ///Ja pega proxima linha depois da ordem do grago para verificar se eh ponderado
    ///Detectar se eh ponderado, pra isso precisa fazer um getline e "entender o line para saber se tem 3 numeros ou 2
    //ponderado = verificaPonderado(line); Será sempre ponderado.
    cout <<  "linha de fora: "<<line << endl;
    //Primeiro vou adicionar os vertices
    //if(inFile->good())
    //{
        do
       {
           cout <<  "linha: "<<line << endl;
           copiaDados.clear(); //limpar o que tiver na stream, afim de não ter nenhum erro
           copiaDados.str(line);
           copiaDados >> vert >> x >> y >> corPB;
           this->adicionaVertice(vert, x, y, corPB);
           cout << "vert add: " << vert << endl;
           //this->adicionaAresta(v1, v2, peso, 0);
           ///Se nao for ponderado adiciona aresta com peso 1.0, para facilitar na hora de verificar caminhos minimos
       } while(getline(*inFile, line) && !line.empty());
    //}
    ///No caso de termos um arquivo de entrada informando quantidade errada de vertices, o programa eh finalizado
    cout << numVertices << " " << ordem << endl;
    cout << numPretos << " " << maxVertBranco << " " << maxCusto << endl;
    if(this->ordem != numVertices)
    {
       cout << "ERRO! Arquivo de entrada com quantidade de vertices errada." << endl;
       cout << "Numero de vertices no arquivo de entrada eh menor que a quantidade real.\nAlgoritmo FINALIZADO." << endl;
       getchar();
       exit(-1);
    }

    ///Criar matriz de pesos
    this->criaMatrizPeso();
    ///Criar Arestas
    cout <<"erro1" <<endl;
    this->criaTodasArestas();
    cout <<"erro2" <<endl;

    cout << "\n---------------------------------------" << endl;
    cout << "Grafo Criado e instanciado com sucesso!" << endl;
    cout << "Quantidade de Vertices: " << getOrdem() << endl;
    cout << "Quantidade de Vertices Pretos: " << this->numPretos << endl;
    cout << "Cardinalidade Maxima de Vertices Brancos (Q): " << this->maxVertBranco << endl;
    cout << "Comprimento/Custo Maximo (L): " << this->maxCusto << endl;
    cout << "---------------------------------------" << endl;
    vertices.sort();
}

void Grafo::criaMatrizPeso()
{
    //vector<vector<float>> matriz(this->ordem, std::vector<float>(this->ordem));
    //matrizPesos = matriz;
    matrizDistancia = new double*[this->ordem];

    for(int i=0; i<this->ordem; i++)
    {
        matrizDistancia[i] = new double[this->ordem];
        for(int j=0; j<this->ordem; j++)
        {
            if(i!=j)
                matrizDistancia[i][j] = calculaPesoAresta(i+1, j+1);
            else
                matrizDistancia[i][j] = 0;
        }
    }
}

double Grafo::calculaPesoAresta(int id1, int id2)
{
    auto vert1 = getVertice(id1);
    auto vert2 = getVertice(id2);
    return sqrt(pow(vert2->getX()-vert1->getX(),2) + pow(vert2->getY()-vert1->getY(),2));
}

void Grafo::criaTodasArestas()
{
    int i,j;
    i=j=0;
    for(auto vert = vertices.begin(); vert != vertices.end(); vert++, i++) {
        for (auto adj = vertices.begin(); adj != vertices.end(); adj++, j++) {
            cout <<"matriz " << i+1 << " " << j+1 << endl;
            if (adj->getIdVertice() != vert->getIdVertice())
                vert->adicionaAresta(adj, matrizDistancia[i][j], digrafo);
        }
        j=0;
    }
}

list<Vertice>::iterator Grafo::adicionaVertice(int id, int x, int y, char corPB)
{
    auto itVertice = getVertice(id);
    if(itVertice->getIdVertice() != id) //Se nao encontrar tem que adicionar
    {
        //Vertice v(id, x, y, corPB);
        vertices.emplace_back(id, x, y, corPB);//push_back(v);
        ordem++;
        return --vertices.end(); //retorna o ultimo vertice, o que foi adicionado
    }
    else
        return itVertice; //retorna o vertice encontrado, que ja foi adicionado
}

void Grafo::adicionaAresta(int idOrigem, int idDestino, float peso, char corPB)
{
    auto itOrigem = adicionaVertice(idOrigem, 0, 0, corPB);
    auto itDestino = adicionaVertice(idDestino, 0, 0, 0);
    itOrigem->adicionaAresta(itDestino, peso, digrafo);
    if(!digrafo)
    {
        itDestino->adicionaAresta(itOrigem, peso, digrafo);
        //itDestino->diminuiGrauEntrada();
    }
}

bool Grafo::removeAresta(int idOrigem, int idDestino)
{
    auto itOrigem = getVertice(idOrigem);
    auto itDestino = getVertice(idDestino);
    bool arestaEncontrada=false;
    if(itOrigem->getIdVertice()==idOrigem && itDestino->getIdVertice()==idDestino)
    {
        arestaEncontrada = itOrigem->removeAresta(itDestino, digrafo);
        if(!digrafo)
        {
            itDestino->removeAresta(itOrigem, digrafo);
        }
    }
    return arestaEncontrada;
}

bool Grafo::removeVertice(int idVert)
{

    if(procuraVertice(idVert))
    {
        for(auto itVert = vertices.begin(); itVert != vertices.end(); itVert++)
        {
            if(itVert->getIdVertice() == idVert)
            {
                for(auto itAdj : itVert->getAdjacencia())
                    removeAresta(itVert->getIdVertice(), itAdj.getIdAdj());
                auto aux=itVert;
                itVert--;
                vertices.erase(aux);
                ordem--;
            }
            else if(digrafo) //se for digrafo, tem que remover todas arestas  adjacentes ao vertice removido
            {
                for(auto itAdj : itVert->getAdjacencia())
                    if(itAdj.getIdAdj() == idVert)
                        removeAresta(itVert->getIdVertice(), idVert);
            }
        }
        return true;
    }
    return false;
}

void Grafo::imprime()
{
    cout << endl << "-----------------------------------" << endl;
    cout << "Imprimindo o grafo:" << endl;
    cout << "Estilo [Vertice] -> Adjacencia|Peso(se existir) ..." << endl;
    cout << "Vertices - Adjacencias\n";
    for(auto it : vertices)
    {
        cout << "[" << it.getIdVertice() << "]  ->   ";
        it.imprimeListAdjacencia(ponderado);
        cout << endl;
    }
    cout << "-----------------------------------" << endl << endl;
}

void Grafo::saveGrafoAdjacencia(ofstream *outFile)
{
    cout << endl << "-------------------------------------------------------------" << endl;
    cout << "Salvando grafo em modo de lista de adjacencias. . . . . ." << endl;
    if(outFile->good()) {
        *outFile << endl << "-----------------------------------" << endl;
        *outFile << "Grafo em Estrutura de Adjacencias:" << endl;
        *outFile << "Estilo [Vertice] -> Adjacencia|Peso(se existir) ..." << endl;
        for (auto itVert : vertices) {
            *outFile << "[" << itVert.getIdVertice() << "] ->  ";
            *outFile << itVert.retornaListAdjacencia(ponderado) << endl;
        }
        *outFile << "-----------------------------------" << endl << endl;
        cout << endl << "Grafo salvo com sucesso!" << endl;
    } else
        cout << "NAO FOI POSSIVEL SALVAR O GRAFO!" << endl;
    cout << "-------------------------------------------------------------" << endl;
}


unsigned int Grafo::getOrdem()
{
    return ordem;
}

Grafo *Grafo::retornaInstanciaGrafo() {
    auto *grafoInstancia = new Grafo(false, false, 0, 0);
    grafoInstancia->ponderado = this->ponderado;
    grafoInstancia->ordem = this->ordem;
    grafoInstancia->digrafo = this->digrafo;
    grafoInstancia->vertices = this->vertices;
    return grafoInstancia;
}

void Grafo::saveGrafo(ofstream *outFile)
{
    cout << endl << "--------------------------------------------------" << endl;
    cout << "Salvando grafo. . . . . . . . . . . ." << endl;
    if(outFile->good()) {
        *outFile << endl <<"GRAFO, 1 LINHA QUANTIDADE DE VERTICES, AS OUTRAS SAO AS ARESTAS:" << endl << endl;
        *outFile << this->ordem << endl;
        if (digrafo) //Se for digrafo, eh mais facil salvar o grafo, pois nao precisa se preocupar com arestas duplicadas
        {
            for (auto itVert : vertices)
                for (auto itAdj : itVert.getAdjacencia()) {
                    if (ponderado)
                        *outFile << itVert.getIdVertice() << " " << itAdj.getIdAdj() << " " << itAdj.getPeso() << endl;
                    else
                        *outFile << itVert.getIdVertice() << " " << itAdj.getIdAdj() << endl;
                }
            cout << endl;
        } else {
            Grafo *grafoCopy = retornaInstanciaGrafo();
            for (auto itVertice = grafoCopy->vertices.begin(); !grafoCopy->vertices.empty(); itVertice++) {
                for (auto itAdj : itVertice->getAdjacencia())//for(auto it = adj.begin(); it != adj.end(); it++) - iterator da lista ajacencia de um vertice "itVertice"
                {
                    if (ponderado)
                        *outFile << itVertice->getIdVertice() << " " << itAdj.getIdAdj() << " " << itAdj.getPeso()
                                 << endl;
                    else
                        *outFile << itVertice->getIdVertice() << " " << itAdj.getIdAdj() << endl;
                }
                int id = itVertice->getIdVertice();
                itVertice--;
                grafoCopy->removeVertice(id); //Remove o vertice, pois ele nao sera usado mais, ja foi salvo
            }
            delete grafoCopy;
        }
        cout << endl << "Grafo salvo com sucesso!" << endl;
    }
    else
    {
        cout << "NAO FOI POSSIVEL SALVAR O GRAFO!" << endl;
    }
    cout << "--------------------------------------------------" << endl;
}

void Grafo::geraLinguagemDot()
{
    string fileName;
    ofstream outFile;
    cout << endl << "---------------- SALVAR GRAFO EM LINGUAGEM DOT ----------------" << endl;
    cout << "----------------   PODE SER USADO NO GRAPHVIZ  ----------------" << endl;
    cout << "Digite o nome do arquivo de saida desejado (sem a extensao do arquivo): ";
    cin >> fileName;
    fileName += ".dot";
    outFile.open(fileName);
    if(!outFile)
        cout << endl << "ERRO! Nao foi possivel criar o arquivo de SAIDA \"" << fileName << "\"!" << endl;
    cout << "Gerando em linguagem DOT. . . . . . . . . . . ." << endl;



    if(outFile.good()) {
        outFile << (digrafo ? "digraph G" : "graph G") << endl << "{" << endl;
        if (digrafo) //Se for digrafo, eh mais facil salvar o grafo, pois nao precisa se preocupar com arestas duplicadas
        {
            for (auto itVert : vertices)
                for (auto itAdj : itVert.getAdjacencia()) {
                    if (ponderado)
                        outFile << "\t" << itVert.getIdVertice() << " -> " << itAdj.getIdAdj() << " [label=\"" << itAdj.getPeso() << "\"];" << endl;
                    else
                        outFile << "\t" << itVert.getIdVertice() << " -> " << itAdj.getIdAdj() << ";" << endl;
                }
        } else
        {
            Grafo *grafoCopy = retornaInstanciaGrafo();
            for (auto itVertice = grafoCopy->vertices.begin(); !grafoCopy->vertices.empty(); itVertice++) {
                for (auto itAdj : itVertice->getAdjacencia())
                {
                    if (ponderado)
                        outFile << "\t" << itVertice->getIdVertice() << " -- " << itAdj.getIdAdj() << " [label=\"" << itAdj.getPeso() << "\"];" << endl;
                    else
                        outFile << "\t" << itVertice->getIdVertice() << " -- " << itAdj.getIdAdj() << ";" << endl;
                }
                int id = itVertice->getIdVertice();
                itVertice--;
                grafoCopy->removeVertice(id); //Remove o vertice, pois ele nao sera usado mais, ja foi salvo
            }
            delete grafoCopy;
        }
        outFile << "}";
        cout << endl << "Grafo gerado em Linguagem DOT com sucesso!" << endl;
        cout << "Arquivo de Saida: " << fileName << endl;
        cout << "Agora voce podera usar o arquivo na ferramenta GraphViz ou outro de preferencia." << endl;
        cout << "Pelo terminal usando GraphViz, use o comando: dot -Kneato -Tpng demo2.dot -o demo2_dot.png" << endl; //Usar -Kneato ou -Kdot
    }
    else
    {
        cout << "NAO FOI POSSIVEL GERAR O GRAFO EM LINGUAGEM DOT!" << endl;
    }
    cout << "--------------------------------------------------" << endl;
    outFile.close();
}


list<Vertice>::iterator Grafo::getVertice(int idVert)
{
    list<Vertice>::iterator it;
    for(it = vertices.begin(); it != vertices.end(); it++)
        if(it->getIdVertice() == idVert)
            break;
    if(it == vertices.end())
        return --it;
    return it;
}

bool Grafo::procuraVertice(int idVert) //Retorna true caso ache uma adjacencia com o vertice de id "idVert"
{
    for(auto it = vertices.begin(); it != vertices.end(); it++)
        if(it->getIdVertice() == idVert)
            return true;
    return false;
}

void Grafo::algConstrutGuloso()
{
    Grafo *grafoAux = retornaInstanciaGrafo();
    grafoAux->imprime();
    double mediaPeso=0;
    double mediaPesoP=0;
    double mediaPesoB=0;
    /*for(int i=0; i<ordem; i++)
        for(int j=0; j<ordem; j++)
            mediaPeso += matrizDistancia[i][j];*/
    for (auto & itVert : grafoAux->vertices)
        for (auto & itAdj : itVert.getAdjacencia())
        {
            mediaPeso += itAdj.getPeso();
            if(itVert.getCorPB()==BRANCO && grafoAux->getCorPB(itAdj.getIdAdj()) == BRANCO)
                mediaPesoB+=itAdj.getPeso();
            else if(itVert.getCorPB()==PRETO && grafoAux->getCorPB(itAdj.getIdAdj()) == PRETO)
                mediaPesoP+=itAdj.getPeso();
        }

    mediaPeso/=(ordem*(ordem-1))/2.0;
    mediaPesoP = (numPretos*(numPretos-1))/2.0;
    mediaPesoB = ((ordem-numPretos)*(ordem-numPretos-1))/2.0;

    cout << "Media Peso: " <<mediaPeso << endl;
    cout << "Media Peso Brancos: " <<mediaPesoB << endl;
    cout << "Media Peso Pretos: " <<mediaPesoP << endl;

    for (auto & itVert : grafoAux->vertices)//(auto itVert = grafoAux->vertices.begin(); itVert != grafoAux->vertices.end(); itVert++)//
    {
        for (auto & itAdj : itVert.getAdjacencia())//(auto itAdj = itVert->getAdjacencia().begin(); itAdj != itVert->getAdjacencia().end(); itAdj++)//
        {
            if(itAdj.getPeso()>maxCusto)
            {
                cout << "mais uma" << endl;
                grafoAux->removeAresta(itVert.getIdVertice(), itAdj.getIdAdj());
            }
        }
    }
    grafoAux->imprime();
    //grafoAux->geraLinguagemDot();
    vector<int> listCandidatos; //pode ser feito com vector<Vertice> se for melhor
    vector<int> solucaoInicial; //pode ser feito com vector<Vertice> se for melhor
    double menorCusto;
    Grafo grafoSolucao(digrafo, ponderado, maxCusto, maxVertBranco);
    //escolhe primeiro vertice aleatoriamente ou com alguma regra especifica

    delete grafoAux;
}

void Grafo::algConstrutGulRandomizado()
{

}

void Grafo::algConstrutGulRandReativo()
{

}

void Grafo::caixeiroViajante()
{
    int inicial = 1;
    auto *tempSolucao = new int[ordem];
    auto *melhorSolucao = new int[ordem];
    double valorMelhorSolucao = INFINITO;
    double valorSolucaoAtual = 0;
    auto *visitados = new bool[ordem];
    visitados[inicial] = true;
    tempSolucao[0] = inicial;
    caixeiroViajanteAux(1, valorSolucaoAtual, valorMelhorSolucao, tempSolucao, melhorSolucao, visitados);
    if (valorMelhorSolucao < INFINITO) cout << "Melhor Solucao: "<< valorMelhorSolucao << endl;
    else cout << "Solucao: Infinito!" << endl;
    for(int i=0; i<ordem; i++)
        cout << melhorSolucao[i]+1 << " ";
}

void Grafo::caixeiroViajanteAux(int i, double &valorSolucaoAtual, double &valorMelhorSolucao, int tempSolucao[], int melhorSolucao[], bool visitados[]) {
    if (valorSolucaoAtual > valorMelhorSolucao) return;
    if (i == this->ordem) {
        double dist = matrizDistancia[tempSolucao[i - 1]][tempSolucao[0]];
        if (dist < INFINITO && valorSolucaoAtual + dist < valorMelhorSolucao) {
            cout << "\t" << valorMelhorSolucao << " -> " << (valorSolucaoAtual + dist) << endl;
            valorMelhorSolucao = valorSolucaoAtual + dist;
            for(int k=0;k<ordem;k++) melhorSolucao[k] = tempSolucao[k];
        }
        return;
    }
    int ultimo = tempSolucao[i - 1];
    for (int t = 0; t < this->ordem; t++) {
        if (!visitados[t] && matrizDistancia[ultimo][t] < INFINITO) {
            visitados[t] = true;
            tempSolucao[i] = t;
            valorSolucaoAtual += matrizDistancia[ultimo][t];
            caixeiroViajanteAux(i + 1, valorSolucaoAtual, valorMelhorSolucao, tempSolucao, melhorSolucao, visitados);
            valorSolucaoAtual -= matrizDistancia[ultimo][t];
            visitados[t] = false;
        }
    }
}

void Grafo::imprimeMatrizDistancia()
{
    cout << "Vert|";
    cout << fixed;
    cout.precision(2);
    cout.setf(ios::fixed);
    for(int i=0; i<=this->ordem; i++)
    {
        for(int j=0; j<this->ordem; j++)
        {
            if(i==0)
                cout << setw(10) << j+1;
            else
            {
                if(j==0)
                    cout <<  i << "   |";
                cout << setw(10) <<matrizDistancia[i-1][j];
            }
        }
        cout << endl;
    }
}


int Grafo::getCorPB(int idVert)
{
    for(auto itVert : vertices)
        if(itVert.getIdVertice() == idVert)
            return itVert.getCorPB();
    return -1;
}