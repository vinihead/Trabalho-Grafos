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
#define PRETO 1
#define BRANCO 0

///Estrutura auxiliar usada como paramentro na ordenação da lista de candidatos (por custo) usando std::sort
struct {
    bool operator()(pair<int, double> cand1, pair<int, double> cand2) const
    {
        return cand1.second < cand2.second;
    }
} ordenaCusto;


///Estrutura para guardar solução Parcial do PCVPB





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
    int corPB;
    string line;
    stringstream copiaDados;
    getline(*inFile, line);
    copiaDados.str(line);
    copiaDados >> numVertices >> numPretos >> maxVertBranco >> maxCusto;
    ordem = 0;

    cout << "--------------------------------------" << endl;
    cout << "Instanciando o grafo do PCVPB. . . . ." << endl;
    cout << "--------------------------------------" << endl;
    //cout <<  "linha de fora: "<<line << endl;
    getline(*inFile, line); ///Ja pega proxima linha depois da ordem do grago para verificar se eh ponderado
    ///Detectar se eh ponderado, pra isso precisa fazer um getline e "entender o line para saber se tem 3 numeros ou 2
    //ponderado = verificaPonderado(line); Será sempre ponderado.
    //cout <<  "linha de fora: "<<line << endl;
    //Primeiro vou adicionar os vertices
    //if(inFile->good())
    //{
        do
       {
           //cout <<  "linha: "<<line << endl;
           copiaDados.clear(); //limpar o que tiver na stream, afim de não ter nenhum erro
           copiaDados.str(line);
           copiaDados >> vert >> x >> y >> corPB;
           this->adicionaVertice(vert, x, y, corPB);
           //cout << "vert add: " << vert << endl;
           //this->adicionaAresta(v1, v2, peso, 0);
           ///Se nao for ponderado adiciona aresta com peso 1.0, para facilitar na hora de verificar caminhos minimos
       } while(getline(*inFile, line) && !line.empty());
    //}
    ///No caso de termos um arquivo de entrada informando quantidade errada de vertices, o programa eh finalizado
    //cout << numVertices << " " << ordem << endl;
    //cout << numPretos << " " << maxVertBranco << " " << maxCusto << endl;
    if(this->ordem != numVertices)
    {
       cout << "ERRO! Arquivo de entrada com quantidade de vertices errada." << endl;
       cout << "Numero de vertices no arquivo de entrada eh " << ((ordem<numVertices)?"menor":"maior");
        cout << " que a quantidade real.\nAlgoritmo FINALIZADO." << endl;
       getchar();
       exit(-1);
    }

    ///Criar matriz de pesos
    this->criaMatrizPeso();
    ///Criar Arestas
    //cout <<"erro1" <<endl;
    this->criaTodasArestas();
    //cout <<"erro2" <<endl;

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
            if (adj->getIdVertice() != vert->getIdVertice())
                vert->adicionaAresta(adj, matrizDistancia[i][j], digrafo);
        }
        j=0;
    }
}

list<Vertice>::iterator Grafo::adicionaVertice(int id, int x, int y, int corPB)
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

void Grafo::adicionaAresta(int idOrigem, int idDestino, float peso, int corPB)
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
                //auto aux=itVert;
                //itVert--;
                itVert = vertices.erase(itVert);
                ordem--;
            }/*
            else if(digrafo) //se for digrafo, tem que remover todas arestas  adjacentes ao vertice removido
            {
                for(auto itAdj : itVert->getAdjacencia())
                    if(itAdj.getIdAdj() == idVert)
                        removeAresta(itVert->getIdVertice(), idVert);
            }*/
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
    auto *grafoInstancia = new Grafo(this->digrafo, this->ponderado, this->maxCusto, this->maxVertBranco);
    grafoInstancia->ordem = this->ordem;
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
    //cout << "Digite o nome do arquivo de saida desejado (sem a extensao do arquivo): ";
    //cin >> fileName;
    //fileName += ".dot";
    outFile.open("graphViz.dot");
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


string Grafo::arvoreBuscaProfundidade(int idVert)
{
    string textoSalvar = "Arvore de Busca em Profundidade (em pre-ordem):";
    for(auto itVert = vertices.begin(); itVert != vertices.end(); itVert++)
        itVert->setVisita(false);

    textoSalvar += arvoreBuscaProfundidadeAux(idVert);
    return textoSalvar;
}

string Grafo::arvoreBuscaProfundidadeAux(int idVert)
{
    auto itVert = getVertice(idVert);
    string textoSalvar;
    if(!itVert->getVisitado())
    {
        textoSalvar = " -> " + to_string(itVert->getIdVertice());
        itVert->setVisita(true);
        for(auto itAdj : itVert->getAdjacencia())
        {

            textoSalvar += arvoreBuscaProfundidadeAux(itAdj.getIdAdj());
        }
    }
    return textoSalvar;
}


bool Grafo::verificaConexo()
{
    for(auto itVert: vertices)
        itVert.setVisita(false);

    arvoreBuscaProfundidade(vertices.begin()->getIdVertice());

    for(auto itVert : vertices)
    {
        if (!itVert.getVisitado())
        {
            return false;
        }
    }
    return true;
}

bool Grafo::verificaEuleriano()
{
    ///Verifica se todos os vertices tem grau par
    ///Se tiverem, verifica se o grafo eh conexo
    bool euleriano = true;
    for(auto itVert : vertices)
        if(itVert.getGrau()%2!=0)
        {
            euleriano=false;
            break;
        }

    if(euleriano)
        euleriano = verificaConexo();
    return euleriano;
}


///Heuristica da Insercao Mais Barata
void Grafo::algConstrutGuloso()
{
    if((ordem-numPretos) > (maxCusto*numPretos))
    {
        cout << "A restricao de cardinalidade nao pode ser satisfeita" << endl;
        return;
    }

    vector<pair<int,double>> candidatos;
    vector<int> solucaoInicial;
    int indiceInsercao=0;
    int adj;
    double peso;
    bool existeSolucao;
    double custoSolucaoInicial=0;
    pair<int,int> aresta;


    ///Necessário para a solucao inicial de vertices pretos ser gerada mais rápida
    Grafo grafoPreto(digrafo, ponderado, maxCusto, maxVertBranco);
    for(auto it : vertices)
        if(it.getCorPB() == PRETO)
            grafoPreto.adicionaVertice(it.getIdVertice(), it.getX(), it.getY(), PRETO);
    grafoPreto.criaMatrizPeso(); //é melhor gerar o peso ou clonar o grafo e retirar os vertices brancos?
    grafoPreto.criaTodasArestas();

    ///loop para escolha do primeira aresta da solucao inicial: a de menor custo
    peso = INFINITO;
    for(auto itVert : grafoPreto.vertices)
        for(auto itAdj : itVert.getAdjacencia())
            if(itAdj.getPeso() < peso)
            {
                peso = itAdj.getPeso();
                aresta.first = itVert.getIdVertice();
                aresta.second = itAdj.getIdAdj();
                cout << aresta.first << " " << aresta.second << endl;
            }

    solucaoInicial.emplace_back(aresta.first);
    solucaoInicial.emplace_back(aresta.second);
    //grafoPreto.removeVertice(aresta.first);
    //grafoPreto.removeVertice(aresta.second);
    cout << "Primeira aresta: ("<< solucaoInicial.front() << ", " << solucaoInicial.back() << ")" << endl;

    grafoPreto.imprime();
    ///Gerando lista de candidatos
    for(auto itVert : grafoPreto.vertices)
        if(itVert.getIdVertice()!=solucaoInicial[0] &&itVert.getIdVertice()!=solucaoInicial[1])
            candidatos.emplace_back(make_pair(itVert.getIdVertice(),0));
    ///Loop para gerar a solucao inicial de pretos Pela Heuristica de Inserção Mais Barata do PCV
    ///Sem a necessidade de se preocupar com as restrições de cardinalidade e comprimento do PB
    while(solucaoInicial.size() < numPretos)
    {



        cout << "Aresta escolhida: " << aresta.first << "," << aresta.second << " Peso: " << peso<< endl;

        /// Gerando custo de adicao dos candidatos as arestas
        for(auto & itCand : candidatos)
        {
            ///Escolhe a aresta de menor custo que já esta na solucao inicial
            ///É feita essa escolha para inserir o vertice da lista de candidatos
            ///entre esse dois vertices dessa aresta de menor custo escolhida
            peso = INFINITO;
            double distAresta1;
            double distAresta2;
            for(int i=0; i<solucaoInicial.size(); i++)
            {


                pair<int,int> arestaAux;
                arestaAux.first = solucaoInicial[i%solucaoInicial.size()];
                arestaAux.second = solucaoInicial[(i+1)%solucaoInicial.size()];

                distAresta1 = matrizDistancia[itCand.first-1][arestaAux.first-1];
                distAresta2 = matrizDistancia[itCand.first-1][arestaAux.second-1];
                if(((distAresta1+distAresta2)-matrizDistancia[arestaAux.first-1][arestaAux.second-1]) < peso && (distAresta1+distAresta2) < maxCusto)
                {
                    aresta.first = arestaAux.first;
                    aresta.second = arestaAux.second;
                    peso = ((distAresta1+distAresta2)-matrizDistancia[arestaAux.first-1][arestaAux.second-1]) ;
                    indiceInsercao = (i+1)%solucaoInicial.size();
                }
            }


            itCand.second = ((distAresta1+distAresta2)-matrizDistancia[aresta.first-1][aresta.second-1]) ;
        }
        /// Proximo passo -> ordenar os candidatos de acordo com o custo e inserir o melhor na solucao inicial
        cout << "Desordenado: " << endl;
        for(auto & itCand : candidatos)
            cout << itCand.first << " " << itCand.second << endl;
        cout <<endl;

        /// Ordena candidatos por custo de insercao
        sort(candidatos.begin(),candidatos.end(), ordenaCusto);

        cout << "Ordenado: " << endl;
        for(auto & itCand : candidatos)
            cout << itCand.first << " " << itCand.second << endl;
        cout <<endl;

        /// Inserindo o melhor na solucao inicial entre os vertices da melhor aresta
        /// E excluindo da lista de candidatos

        solucaoInicial.insert(solucaoInicial.begin()+indiceInsercao, candidatos.begin()->first);
        candidatos.erase(candidatos.begin());
        ///??????? FAZER A ordenacao em ordem ao contrario e remover sempre com pop_back, no final do vetor
        /// ????? INTERESSANTE PARA GASTAR MENOS RECURSOS COMPUTACIONAIS??

        cout << "Solucao inicial Parcial: ";
        for(int i=0; i<solucaoInicial.size(); i++)
        {
            cout << solucaoInicial[i] << " ";
        }
        cout << endl << endl;
    }

    grafoPreto.imprimeMatrizDistancia();

    cout << "Solucao Inicial: ";
    for(int i=0; i<solucaoInicial.size(); i++)
    {
        cout << solucaoInicial[i] << " ";
        //custoSolucaoInicial += matrizDistancia[(i%solucaoInicial.size())-1][((i+1)%solucaoInicial.size())-1];
        custoSolucaoInicial += matrizDistancia[solucaoInicial[i]-1][solucaoInicial[(i+1)%solucaoInicial.size()]-1];
    }
    cout << endl << "Custo Solucao inicial: " << custoSolucaoInicial;


    ///Segunda parte, solucao PCVPB
/*
    bool viavel = false;

    for(auto it : vertices)
        if(it.getCorPB() == BRANCO)
            candidatos.emplace_back(make_pair(it.getIdVertice(),0));

    ///Gerando cadeias da solução
    Solucao solucao;
    solucao.solucao = solucaoInicial;
    for(int i=0; i < solucaoInicial.size(); i++)
    {
        Cadeia cadeia(0,matrizDistancia[solucaoInicial[i]-1][solucaoInicial[(i+1)%solucaoInicial.size()]-1]);
        //cadeia.cardinalidadeQ=0;
        //cadeia.comprimetoL=matrizDistancia[solucaoInicial[i]-1][solucaoInicial[(i+1)%solucaoInicial.size()]-1];
        //cadeia.vert.emplace_back(solucaoInicial[i]);
        //cadeia.vert.emplace_back(solucaoInicial[(i+1)%solucaoInicial.size()]);
        solucao.cadeias.emplace_back(cadeia);
    }

    cout << endl << "Cadeias iniciais: " << endl;
    for(auto itSolucao : solucao.cadeias)
    {
        cout << endl << "Cadeia: " << itSolucao.vert[0] << " " << itSolucao.vert[1];
    }

    /// Variaveis teste

    int cardinalidadeOficial = 2;
    double comprimentoOficial = 15;
    double comprimentoBacaninha = 0;
    vector<Vertice> candidatosBrancos;

    /// IEB
    while (candidatos.size() != 0)
    {
        //candidatosBrancos.emplace_back(solucaoBrancos);

        for(auto itSolucao : solucao.cadeias)
        {
            for(auto itBranco : candidatosBrancos)
            {
                comprimentoBacaninha = itBranco.getPeso(itSolucao.vert[0]) + itBranco.getPeso(itSolucao.vert[1]);

                if(comprimentoBacaninha <= comprimentoOficial && itSolucao.cardinalidadeQ < cardinalidadeOficial &&
                        itBranco.getPeso(itSolucao.vert[0]) < itSolucao.comprimetoL  &&
                            itBranco.getPeso(itSolucao.vert[1]) < itSolucao.comprimetoL)
                {
                    //itSolucao.vert.
                    // Acho que precisa criar mais um indice pra alocar entre os dois vertices pretos da cadeia
                    //indiceInsercao = (i+1)%solucaoInicial.size(); Nao sei fazer isso
                    viavel = true;
                }
            }
        }
    }
    */
}


/*
///Função auxiliar usada como paramentro na ordenação da lista de candidatos pelo std::sort
bool Grafo::ordenaCandidatos(pair<int, double> cand1, pair<int, double> cand2)
{
    return cand1.second < cand2.second;
}





bool pesoMinimo(const double& p1, const double& p2)
{
    return

    return p1.age < p2.age;
}*/

void Grafo::algConstrutGulosoAntigo()
{
    if((ordem-numPretos) > (maxCusto*numPretos))
    {
        cout << "A restricao de cardinalidade nao pode ser satisfeita" << endl;
        return;
    }



    Grafo *grafoAux = retornaInstanciaGrafo();

    //grafoAux->imprime();

    vector<int> candidatos;
    //vector<int,double> listPretos;
    //vector<int> solucaoInicial; //pode ser feito com vector<Vertice> se for melhor
    Grafo grafoSolucao(digrafo, ponderado, maxCusto, maxVertBranco);
    vector<int> solucaoInicial;
    int adj;
    double peso;
    bool existeSolucao;
    double custoSolucaoInicial=0;

    ///Necessário para a solucao inicial de vertices pretos ser gerada mais rápida
    Grafo grafoPreto(digrafo, ponderado, maxCusto, maxVertBranco);
    for(auto it : vertices)
        if(it.getCorPB() == PRETO)
            grafoPreto.adicionaVertice(it.getIdVertice(), it.getX(), it.getY(), PRETO);
    grafoPreto.criaMatrizPeso(); //é melhor gerar o peso ou clonar o grafo e retirar os vertices brancos?
    grafoPreto.criaTodasArestas();
    grafoPreto.imprime();
    ///Aqui, seria interessante ordenar os vertices do grafoPreto pela soma dos pesos de suas arestas

    ///Escolher primeiro vertice Preto da solucao
    //for(auto it : vertices)
        //if(it.getCorPB() == PRETO)
        //{
            solucaoInicial.push_back(grafoPreto.vertices.front().getIdVertice());//(vertices.front().getIdVertice());
            //break;
        //}

    ///Pensar se vai precisar mesmo do grafoAux <- Dependerá de como será construida a lista de candidatos
    ///Escolha da solucao inicial Gulosa de menor custo com Vértices pretos
    for(auto itVert : grafoPreto.vertices)//for(int i=0; i<numPretos;i++)//
    {
        existeSolucao = false;
        peso=INFINITO;
        ///Solucao caminho pelos vertices
        /*for(auto itAdj : grafoPreto.getVertice(solucaoInicial[i])->getAdjacencia())
        {
            //if(getCorPB(itAdj.getIdAdj()) == PRETO) /// Primeiro vertice eh preto e os proximos também
            //{
                auto it = find (solucaoInicial.begin(), solucaoInicial.end(), itAdj.getIdAdj());
                if(itAdj.getPeso() < peso && *it!=itAdj.getIdAdj())
                {
                    peso = itAdj.getPeso();
                    adj=itAdj.getIdAdj();
                    existeSolucao = true;
                }
            //}
        }*/
        ///Solucao melhor aresta de cada vertice
        for(auto itAdj : itVert.getAdjacencia())
        {
            //if(itVert.getCorPB()==PRETO && getCorPB(itAdj.getIdAdj()) == PRETO)
            //{
                auto it = find (solucaoInicial.begin(), solucaoInicial.end(), itAdj.getIdAdj());
                if(*it!=itAdj.getIdAdj() && itAdj.getPeso() < peso)
                {
                    peso = itAdj.getPeso();
                    //if(peso > maxCusto)
                        //cout << "JA ERA!!!!!!!!!!!!!!!! " << "maxCusto: " << maxCusto << " - pesoAresta: " << peso << endl;
                    adj=itAdj.getIdAdj();
                    existeSolucao = true;
                }
            //}
        }
        if(existeSolucao)
        {
            solucaoInicial.push_back(adj);
        }
        if(solucaoInicial.size() == this->numPretos)
            break;

    }
    //grafoAux->imprime();


    cout << endl << "Solucao inicial: ";
    for(auto it : solucaoInicial)
    {
        cout << it << " ";
    }

    /// Antes da lista de candidatos, remover os pretos ou remover as arestas incidentes a pretos?
    ///Fazer lista de candidadtos: tirar os pretos? tirar as arestas adjacentes aos pretos? colocar ja visitado?
    for(auto it :  vertices)
    {
        if(it.getCorPB() == BRANCO)
            candidatos.emplace_back(it.getIdVertice());
    }

    grafoPreto.imprimeMatrizDistancia();

    cout << "Solucao Inicial: ";
    for(int i=0; i<solucaoInicial.size(); i++)
    {
        cout << solucaoInicial[i] << " ";
        //custoSolucaoInicial += matrizDistancia[(i%solucaoInicial.size())-1][((i+1)%solucaoInicial.size())-1];
        custoSolucaoInicial += matrizDistancia[solucaoInicial[i]-1][solucaoInicial[(i+1)%solucaoInicial.size()]-1];
    }
    cout << endl << "Custo Solucao inicial: " << custoSolucaoInicial;


    double menorCusto;
    //escolhe primeiro vertice aleatoriamente ou com alguma regra especifica

    /*
    double mediaPeso=0;
    double mediaPesoP=0;
    double mediaPesoB=0;
    //for(int i=0; i<ordem; i++)
        //for(int j=0; j<ordem; j++)
            //mediaPeso += matrizDistancia[i][j];
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
    //grafoAux->geraLinguagemDot();
    cout << grafoAux->vertices.begin()->retornaListAdjacencia(ponderado) << endl;
*/

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
    cout << endl;
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



void Grafo::caminhoMinimoDijkstra(int _idVerticeOrigem, int _idVerticeDestino) {
    vector<pair<int,int>> abertos;
    vector<int> fechados;
    list<Vertice>::iterator vtcAtual;
    double distanciaIntermediaria;

    getVertice(_idVerticeOrigem)->setDistancia(0);
    getVertice(_idVerticeOrigem)->setAntecessor(_idVerticeOrigem);
    abertos.emplace_back(make_pair(0,_idVerticeOrigem));

    //inicializaçao
    for(auto it : vertices){
        if(it.getIdVertice() != _idVerticeOrigem) {
            it.setDistancia(INFINITO);
            it.setAntecessor(0);
            abertos.emplace_back(make_pair(INFINITO,it.getIdVertice()));    //insere os vertices a lista de vertices abertos
        }
    }

    while(fechados.size() != ordem){

        sort(abertos.begin(), abertos.end());       //ordena por distancia
        vtcAtual = getVertice(abertos[0].second);   //recebe o de menor
        abertos.erase(abertos.begin());
        for(auto itVert = vertices.begin(); itVert!=vertices.end(); itVert++)
        {
            for(auto itAdj = vtcAtual->getAdjacencia().begin(); itAdj!= vtcAtual->getAdjacencia().end(); itAdj++)
            {
                //se distancia conhecida for maior que a distancia ate o vertice, passando pelo atual, atualizar.
                distanciaIntermediaria = vtcAtual->getDistancia() + itAdj->getPeso();
                if(getVertice(itAdj->getIdAdj())->getDistancia() > distanciaIntermediaria)
                {
                    getVertice(itAdj->getIdAdj())->setDistancia(distanciaIntermediaria);
                }
            }
        }
        fechados.push_back(vtcAtual->getIdVertice());
    }

    cout << "Caminho feito: ";
    for(auto it : fechados)
        cout << it << " ";
    cout << "\nCaminho minimo: " << fixed << getVertice(_idVerticeDestino)->getDistancia() << endl;
}


/*
bool Grafo::viavel(Cadeia cadeia, double dist)
{
    return (cadeia.comprimetoL + dist <= maxCusto) && (cadeia.comprimetoL+1 <=maxVertBranco);
}
 */