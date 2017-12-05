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
#include <climits>
#include <cmath>
#include <iomanip>
#include <random>

#define INFINITO LONG_MAX

///Constantes usadas no problema do caixeiro viajante preto e branco
#define PRETO 1
#define BRANCO 0

///Estrutura auxiliar usada como paramentro na ordenação da lista de candidatos (por custo) usando std::sort
struct {
    bool operator()(pair<pair<int, double>,int> cand1, pair<pair<int, double>,int> cand2) const
    {
        return cand1.first.second < cand2.first.second;
    }
    bool operator()(pair<pair<int, double>,pair<int,int>> cand1, pair<pair<int, double>,pair<int,int>> cand2) const
    {
        return cand1.first.second < cand2.first.second;
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

    //if(inFile->good())
    //{
    ///Adiciona os vertices primeiramente
    while(getline(*inFile, line) && !line.empty())
       {
           copiaDados.clear(); ///limpar o que tiver na stream, afim de não ter nenhum erro
           copiaDados.str(line);
           copiaDados >> vert >> x >> y >> corPB;
           /**Ao adicionar o vertice subtraimos 1, pois como as instancias sao comportadas de 1 a n,
            podemos faze-la de 0 a n-1, facilitando na solucao, no uso de vetores e na busca na matriz distancia*/
           this->adicionaVertice((vert-2+1), x, y, corPB); }
    //}
    ///No caso de termos um arquivo de entrada informando quantidade errada de vertices, o programa eh finalizado
    if(this->ordem != numVertices)
    {
        cout << ordem << " " << numVertices << endl;
        cout << "ERRO! Arquivo de entrada com quantidade de vertices errada." << endl;
        cout << "Numero de vertices no arquivo de entrada eh " << ((ordem<numVertices)?"menor":"maior");
        cout << " que a quantidade real.\nAlgo de errado aconteceu.\nAlgoritmo FINALIZADO." << endl;
        getchar();
        exit(-1);
    }

    ///Cria matriz de pesos
    this->criaMatrizPeso();
    ///Cria Arestas
    this->criaTodasArestas();

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
    matrizDistancia = new double*[this->ordem];

    for(int i=0; i<this->ordem; i++)
    {
        matrizDistancia[i] = new double[this->ordem];
        for(int j=0; j<this->ordem; j++)
        {
            if(i!=j)
                matrizDistancia[i][j] = calculaPesoAresta(i, j);
            else
                matrizDistancia[i][j] = 0;
        }
    }
}

double Grafo::calculaPesoAresta(int id1, int id2)
{
    auto vert1 = getVertice(id1);
    auto vert2 = getVertice(id2);
    return sqrt((pow(vert2->getX()-vert1->getX(),2) + pow(vert2->getY()-vert1->getY(),2)));
}

void Grafo::criaTodasArestas()
{
    int i,j;
    i=j=0;
    for(auto vert = vertices.begin(); vert != vertices.end(); vert++, i++,j=0) {
        for (auto adj = vertices.begin(); adj != vertices.end(); adj++, j++) {
            if (adj->getIdVertice() != vert->getIdVertice())
                vert->adicionaAresta(adj, matrizDistancia[i][j], digrafo);
        }
    }
}

list<Vertice>::iterator Grafo::adicionaVertice(int id, int x, int y, int corPB)
{
    auto itVertice = getVertice(id);
    if(itVertice->getIdVertice() != id || vertices.size()==0) //Se nao encontrar tem que adicionar
    {
        //Vertice v(id, x, y, corPB);
        vertices.emplace_back(id, x, y, corPB);//push_back(v);
        ordem++;
        return --vertices.end(); //retorna o ultimo vertice, o que foi adicionado
    }
    else
    {
        cout << "ENCONTREI!";
        return itVertice; //retorna o vertice encontrado, que ja foi adicionado

    }
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
        cout << "[" << it.getIdVertice()+1 << "]  ->   ";
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

double Grafo::getMaxCusto() const {
    return maxCusto;
}

int Grafo::getMaxVertBranco() const {
    return maxVertBranco;
}

int Grafo::getNumPretos() const {
    return numPretos;
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

void Grafo::imprimeMatrizDistancia() {
    cout << "Vert|";
    cout << fixed;
    cout.precision(2);
    cout.setf(ios::fixed);
    for (int i = -1; i < this->ordem || i == -1; i++) {
        for (int j = 0; j < this->ordem; j++) {
            if (i == -1)
                cout << setw(10) << j + 1;
            else {
                if (j == 0)
                    cout << i + 1 << "   |";
                cout << setw(10) << matrizDistancia[i][j];
            }
        }
        cout << endl;
    }
}

///Heuristica PCVPB - Insercao Mais Barata
Grafo::Solucao Grafo::heuristicaGulosoRandomizado(float alfa)
{
    vector<pair<pair<int, double>, int>> candidatosPretos;
    vector<pair<pair<int, double>, pair<int, int>>> candidatosBrancos;
    vector<int> solucaoInicial;
    unsigned int indiceInsercao;
    unsigned int indiceInsercaoCadeia;
    double melhorCusto;
    bool viavel;
    bool existeSolucao;
    pair<int, int> aresta;
    Solucao solucao;
    solucao.custo = 0;
    solucao.alfa = alfa;

    ///Gera as listas de candidatos pretos e brancos
    for (auto itVert : vertices) {
        if (itVert.getCorPB() == PRETO) {
            candidatosPretos.emplace_back(pair<pair<int, double>, int>(make_pair(itVert.getIdVertice(), 0), 0));
        } else if (itVert.getCorPB() == BRANCO) {
            candidatosBrancos.emplace_back(pair<pair<int, double>, pair<int, int>>(make_pair(itVert.getIdVertice(), 0), make_pair(0, 0)));
        }
    }

    ///loop para escolha da primeira aresta da solucao inicial: a de menor peso/distancia
    melhorCusto = INFINITO;
    for (int i = 0; i < candidatosPretos.size() - 1; i++) {
        for (int j = i + 1; j < candidatosPretos.size(); j++) {
            if (matrizDistancia[candidatosPretos[i].first.first][candidatosPretos[j].first.first] < melhorCusto) {
                melhorCusto = matrizDistancia[candidatosPretos[i].first.first][candidatosPretos[j].first.first];
                aresta.first = candidatosPretos[i].first.first;
                aresta.second = candidatosPretos[j].first.first;
            }
        }
    }
    solucaoInicial.emplace_back(aresta.first);
    solucaoInicial.emplace_back(aresta.second);
    solucao.custo += matrizDistancia[aresta.first][aresta.second] * 2;

         ///Remove da lista de candidatos os dois vertices da aresta inicial
    for (auto it = candidatosPretos.begin(); it != candidatosPretos.end();)
        if (it->first.first == aresta.first || it->first.first == aresta.second)
            it = candidatosPretos.erase(it);
        else
            it++;



    ///Loop para gerar a solucao inicial de pretos Pela Heuristica de Inserção Mais Barata do PCV
    ///Com a necessidade apenas de se preocupar com a restrição comprimento do PCVPB
    while (!candidatosPretos.empty()) {
        /// Gerando custo de adicao dos candidatos
        ///Para cada vertice candidato, eh escolhido a aresta da solucao que tem o menor custo
        viavel = false;
        for (auto &itCand : candidatosPretos) {
            melhorCusto = INFINITO;
            existeSolucao = false;
            double distAresta1;
            double distAresta2;
            double custoInsercao;
            for (int i = 0; i < solucaoInicial.size(); i++) {
                aresta.first = solucaoInicial[i];
                aresta.second = solucaoInicial[(i + 1) % solucaoInicial.size()];
                distAresta1 = matrizDistancia[itCand.first.first][aresta.first];
                distAresta2 = matrizDistancia[itCand.first.first][aresta.second];
                custoInsercao = distAresta1 + distAresta2 - matrizDistancia[aresta.first][aresta.second];
                if (custoInsercao < melhorCusto && viabilidade(distAresta1, distAresta2)) {
                    melhorCusto = custoInsercao;
                    indiceInsercao = (i + 1) % solucaoInicial.size();
                    viavel = true;
                    existeSolucao = true;
                }
            }
            if (existeSolucao) {
                itCand.second = indiceInsercao;
                itCand.first.second = melhorCusto;
            } else
                itCand.first.second = INFINITO;
        }

        if (!viavel) {
            cout << "NAO EH VIAVEL!!!" << endl;
            solucao.custo=-1;
            return solucao;
        }

        /// Ordena candidatos por custo de insercao
        sort(candidatosPretos.begin(), candidatosPretos.end(), ordenaCusto);

        if(alfa == 0)
        {
            /// Insere o melhor vertice, com o menor custo de insercao na solucao inicial
            solucaoInicial.insert(solucaoInicial.begin()+candidatosPretos.begin()->second, candidatosPretos.begin()->first.first);
            solucao.custo+=candidatosPretos.begin()->first.second;
            /// Exclui o vertice da lista de candidatos que foi inserido na solução
            candidatosPretos.erase(candidatosPretos.begin());
        }
        else
        {
            ///Pegar ate qual indice quero inserir e sortear nisso
            vector<pair<pair<int, double>, int>>::iterator itInsercao;
            auto escopoInsercaoMax = static_cast<int>(ceil(candidatosPretos.size() * alfa));
            int indiceEscolhido=rand()%(escopoInsercaoMax+1);
            itInsercao = candidatosPretos.begin()+indiceEscolhido;

            /// Insere o vertice sorteado com base no alfa
            solucaoInicial.insert(solucaoInicial.begin()+itInsercao->second, itInsercao->first.first);
            solucao.custo += itInsercao->first.second;
            /// Exclui o vertice da lista de candidatos que foi inserido na solução
            candidatosPretos.erase(itInsercao);
        }

    }

    ///Segunda parte, solucao PCVPB
    ///Gerando cadeias da solução
    for (int i = 0; i < solucaoInicial.size(); i++) {
        double peso = matrizDistancia[solucaoInicial[i]][solucaoInicial[(i + 1) % solucaoInicial.size()]];
        Cadeia cadeia(solucaoInicial[i], solucaoInicial[(i + 1) % solucaoInicial.size()], peso);
        solucao.cadeias.emplace_back(cadeia);
    }

    ///Inserção especifica de brancos (IEB)
    viavel = false;
    while (!candidatosBrancos.empty()) {
        viavel = false;
        for (auto &itCand : candidatosBrancos) {
            double distAresta1;
            double distAresta2;
            double custoInsercao;
            melhorCusto = INFINITO;
            existeSolucao = false;
            for (int i = 0; i < solucao.cadeias.size(); i++) {
                vector<int> cadeia = solucao.cadeias[i].getCadeia();
                for (int j = 0; j < cadeia.size() - 1; j++) {
                    aresta.first = cadeia[j];
                    aresta.second = cadeia[j + 1];
                    distAresta1 = matrizDistancia[itCand.first.first][aresta.first];
                    distAresta2 = matrizDistancia[itCand.first.first][aresta.second];
                    custoInsercao = distAresta1 + distAresta2 - matrizDistancia[aresta.first][aresta.second];
                    if (custoInsercao < melhorCusto &&
                        viabilidade(&solucao.cadeias[i], custoInsercao, distAresta1, distAresta2)) {
                        melhorCusto = custoInsercao;
                        indiceInsercaoCadeia = i;
                        indiceInsercao = j + 1;
                        viavel = true;
                        existeSolucao = true;
                    }
                }
            }
            if (existeSolucao) {
                itCand.second.first = indiceInsercaoCadeia;
                itCand.second.second = indiceInsercao;
                itCand.first.second = melhorCusto;
            } else
                itCand.first.second = INFINITO;
        }

        if (!viavel) {
            solucao.custo=-1;
            return solucao;
        }

        /// Ordena candidatos por custo de insercao
        sort(candidatosBrancos.begin(), candidatosBrancos.end(), ordenaCusto);

        if(alfa == 0)
        {
            /// Insere o melhor vertice, com o menor custo de insercao na solucao inicial
            /// E o exclui da lista de candidatos
            solucao.cadeias[candidatosBrancos.begin()->second.first].insereVertice(candidatosBrancos.begin()->second.second,candidatosBrancos.begin()->first.first,candidatosBrancos.begin()->first.second);
            solucao.custo+=candidatosBrancos.begin()->first.second;
            candidatosBrancos.erase(candidatosBrancos.begin());
        }
        else
        {
            ///Pegar ate qual indice quero inserir e sortear nisso
            vector<pair<pair<int, double>, pair<int, int>>>::iterator itInsercao;
            auto escopoInsercaoMax = static_cast<int>(ceil(candidatosBrancos.size() * alfa));
            int indiceEscolhido=rand()%(escopoInsercaoMax+1);
            itInsercao = candidatosBrancos.begin()+indiceEscolhido;

            /// Insere o vertice sorteado com base no alfa
            solucao.cadeias[itInsercao->second.first].insereVertice(itInsercao->second.second,itInsercao->first.first,itInsercao->first.second);
            solucao.custo += itInsercao->first.second;
            /// Exclui o vertice da lista de candidatos que foi inserido na solução
            candidatosBrancos.erase(itInsercao);
        }
    }

    ///Ao finalizar a construção da solução, é gerado na estrutura solução, o ciclo hamiltoniano.
    ///Afim da solução ficar disponível e de fácil acesso.
    for (const auto &it : solucao.cadeias) {
        vector<int> cadeia = it.getCadeia();
        for (int j = 0; j < cadeia.size(); j++) {
            if (j != cadeia.size() - 1) {
                solucao.listVertices.emplace_back(cadeia[j]);
            }
        }
    }
    return solucao;
}


void Grafo::algConstrutGuloso()
{
    Solucao solucao = heuristicaGulosoRandomizado(0);
    ofstream outFile;
    imprimeSolucao(2, &solucao, &outFile);
}

/**
 *
 * @param alfa
 */
void Grafo::algConstrutGulRandomizado(float alfa)
{
    ///Já roda o guloso para pegar a melhor solução para comparar com as soluções do randomizado
    Solucao melhorSolucao = heuristicaGulosoRandomizado(0);
    int numIteracoes = 100;
    for(int i=0; i<numIteracoes; i++)
    {
        Solucao solucaoAtual = heuristicaGulosoRandomizado(alfa);
        if(solucaoAtual.custo < melhorSolucao.custo && solucaoAtual.custo!=-1)
                melhorSolucao = solucaoAtual;
    }
    ofstream outFile;
    imprimeSolucao(2, &melhorSolucao, &outFile);
}

void Grafo::algConstrutGulRandReativo()
{
    ///Já roda o guloso para pegar a melhor solução para comparar com as soluções do randomizado reativo
    cout.setf(ios::fixed);
    Solucao melhorSolucao = heuristicaGulosoRandomizado(0);
    const int numAlfas = 10;
    int indiceAlfaAtual;//,op;
    const float vetAlfas[numAlfas] = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50};
    const int numIteracoes = 1000;
    const int iteracoesAtualizaProb = 50;
    long somaAlfas[numAlfas] = {0};    //soma dos resultados obtidos com cada alpha
    long qtdEscolhido[numAlfas] ={0};   //quantidade de vezes que cada alfa foi escolhido e executado
    vector<float> vetProbabilidades = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};//(p_i.begin(), p_i.end());   //cria variavel de escolher indice aleatorio
    //int melhorSemente;

    discrete_distribution<> distribuicao(vetProbabilidades.begin(),vetProbabilidades.end());// = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};//(p_i.begin(), p_i.end());   //cria variavel de escolher indice aleatorio
    default_random_engine gerador((unsigned int)time(nullptr));  //gerador num aleatorios
    for(int i=0; i<numIteracoes; i++)
    {
        cout << "it " << i << endl;
        //escolhe alfa aleatoriamente atraves da distribuiçao de probabilidades
        indiceAlfaAtual = distribuicao(gerador);
        cout << "geradorHAHA:" << gerador << endl;
        cout << "Indice alfa Atual: " << indiceAlfaAtual <<endl;
        Solucao solucaoAtual = heuristicaGulosoRandomizado(vetAlfas[indiceAlfaAtual]);//valorDaSolucaoAtual = auxCVGR(alfas[indiceAlfaAtual],false);
        somaAlfas[indiceAlfaAtual] += solucaoAtual.custo;//valorDaSolucaoAtual;   //soma dos resultados obtidos com esse alfa
        qtdEscolhido[indiceAlfaAtual] += 1;  //num execuçao este alfa

        //atualiza valores da melhor soluçao
        if(solucaoAtual.custo < melhorSolucao.custo){
            cout << "TROCA DE MELHOR SOLUCAO\n";
            cout << "Melhor Solucao: " <<melhorSolucao.custo << endl;
            cout << "Alfa melhor solucao: " << melhorSolucao.alfa << endl;
            cout << "Solucao para troca: " << solucaoAtual.custo << endl;
            cout << "Alfa solucao para troca: " << solucaoAtual.alfa << endl;
            //solucaoAtual.alfa = vetAlfas[indiceAlfaAtual];
            melhorSolucao = solucaoAtual;//valorDaSolucaoAtual;

            //melhorSemente = i;
        }

        cout << "solucao atual: " << solucaoAtual.custo << endl;
        cout << "alfa atual: " << solucaoAtual.alfa << endl;
        cout << "Custo melhor solucao: " << melhorSolucao.custo << endl;
        cout << "melhor alfa: " << melhorSolucao.alfa << endl;


        //Reativo
        if((i+1) % iteracoesAtualizaProb == 0){
            double q_i[numAlfas] = {0}; ///Utilizado para gerar as novas probabilidades
            cout << "Atualizacao de probabilidade\n";
            float soma_qi = 0;
            float mediaResultados = 0;//media soma_i/qtdeUso_i
            float delta = 10.0;

            for(int j=0; j<numAlfas; j++){
                //caso o alfa tenha sido utilizado
                if(qtdEscolhido[j] > 0){
                    mediaResultados = somaAlfas[j] / qtdEscolhido[j]; //media dos resultados para alfa
                    q_i[j] = pow(melhorSolucao.custo / mediaResultados, delta); // quanto o melhor resultado influencia  a novas probabilidades
                    soma_qi += q_i[j];
                }
            }

            cout << "Probabilidades da distribuicao: " << endl;
            for(auto it : distribuicao.probabilities())
                cout << it << " ";
            cout << endl;


            //vector<double> auxNovaDistribuicao;
            vetProbabilidades.clear();
            for (auto itQi : q_i) {
                vetProbabilidades.push_back(itQi / soma_qi); ///Atualiza a probabilidade do alfa i;
            }



            //atualiza distribuiçao
            discrete_distribution<> novaDistribuicao(vetProbabilidades.begin(), vetProbabilidades.end());
            distribuicao = novaDistribuicao;
            cout << "Nova distribuicao\n";
            cout << "Probabilidades da nova distribuicao: " << endl;
            for(auto it : distribuicao.probabilities())
                cout << it << " ";
            cout << endl;

        }
    }
    ofstream outFile;
    imprimeSolucao(3, &melhorSolucao, &outFile);
}

/**
 *
 * @param cadeia
 * @param custoInsercao
 * @param dist1
 * @param dist2
 * @return
 */
bool Grafo::viabilidade(Cadeia *cadeia, double custoInsercao, double dist1, double dist2) const
{
    return viabilidade(dist1, dist2) && cadeia->getComprimento()+custoInsercao<=maxCusto && cadeia->getCardinalidade()<maxVertBranco;
}

/**
 *
 * @param dist1
 * @param dist2
 * @return
 */
bool Grafo::viabilidade(double dist1, double dist2) const
{
    return dist1<maxCusto && dist2<maxCusto && dist1+dist2<maxCusto;
}

///Funcao imprime informacoes sobre a solucao na tela e no arquivo de saida
/**
 *
 * @param algoritmo (1-Guloso, 2-Randomizado, 3-Reativo)
 * @param melhorSolucao
 * @param outFile
 */
void Grafo::imprimeSolucao(int algoritmo, Solucao *melhorSolucao, ofstream *outFile)
{
    cout << "Algoritmo Finalizado." << endl;
    if (melhorSolucao->custo == -1)
    {
        cout << "Não encontrada solução viavel para a instancia." << endl;
    }
    else
    {
        cout << "Solucao construida." << endl;
        cout << "Deseja visualizar informacoes sobre a solucao? (tecle \'1\' para SIM): ";
        int opcao;
        cin >> opcao;
        if(opcao==1){
            cout << endl << "-------------------------------------------------------------" << endl;
            if(algoritmo == 1)
                cout << "Algoritmo Construtivo Guloso." << endl;
            else if(algoritmo == 2)
            {
                cout << "Algoritmo Contrutivo Guloso Randomizado." << endl;
                cout << "Alfa utilizado: " << setprecision(2) << melhorSolucao->alfa << endl;
            }
            else if(algoritmo == 3)
            {
                cout << "Algoritmo Contrutivo Guloso Randomizado Reativo." << endl;
                cout << "Melhor Alfa: " << fixed << setprecision(2) << melhorSolucao->alfa << endl;
            }
            cout << "Solucao final de melhor custo: " << fixed << setprecision(10) << melhorSolucao->custo << endl;
            cout << "Ciclo Hamiltoniano de menor custo, obedecendo as restricoes do PCVPB:" << endl;
            for (const auto &it : melhorSolucao->listVertices)
                cout << (it + 1) << " ";
            cout << endl << "-------------------------------------------------------------" << endl;
        }
    }
}
