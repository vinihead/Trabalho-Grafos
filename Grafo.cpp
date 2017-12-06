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

    ///Adiciona os vertices primeiramente
    while(getline(*inFile, line) && !line.empty())
       {
           copiaDados.clear(); ///limpar o que tiver na stream, afim de não ter nenhum erro
           copiaDados.str(line);
           copiaDados >> vert >> x >> y >> corPB;
           /**Ao adicionar o vertice subtraimos 1, pois como as instancias sao comportadas de 1 a n,
            podemos faze-la de 0 a n-1, facilitando na solucao, no uso de vetores e na busca na matriz distancia*/
           this->adicionaVertice((vert-2+1), x, y, corPB); }
    ///No caso de termos um arquivo de entrada informando quantidade errada de vertices, o programa eh finalizado
    if(this->ordem != numVertices)
    {
        cerr << "ERRO! Arquivo de entrada com quantidade de vertices errada." << endl;
        cerr << "Numero de vertices no arquivo de entrada eh " << ((ordem<numVertices)?"menor":"maior");
        cerr << " que a quantidade real.\nAlgo de errado aconteceu.\nAlgoritmo FINALIZADO." << endl;
        getchar();
        exit(-1);
    }

    ///Cria matriz de pesos
    this->criaMatrizPeso();
    ///Cria Arestas
    this->criaTodasArestas();

    //cout << "----------------------------------------------" << endl;
    //cout << "Grafo do PCVPB Criado e instanciado com sucesso!" << endl;
    //cout << "------------------------------------------------" << endl;
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
    if(itVertice->getIdVertice() != id || vertices.empty()) //Se nao encontrar tem que adicionar
    {
        vertices.emplace_back(id, x, y, corPB);//push_back(v);
        ordem++;
        return --vertices.end(); //retorna o ultimo vertice, o que foi adicionado
    }
    else
    {
        return itVertice; //retorna o vertice encontrado, que ja foi adicionado

    }
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

unsigned int Grafo::getOrdem()
{
    return ordem;
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

/**
 * Heuristica PCVPB - Insercao Mais Barata
 * @param alfa
 * @return
 */
Grafo::Solucao Grafo::heuristicaGulosoRandomizado(float alfa)
{
    random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator


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

    /** Formada uma subrota inicial com dois vértices, aplicamos o metodo da
      insercao mais barata para decidir qual cidade inserir entre cada
      par de cidades i e j. A cidade k escolhida sera aquela que minimizar
      custo(k) = d(i,k) + d(k,j) - d(i,j). No entanto, como temos vertices
      pretos e brancos, primeiro geramos a solução inicial com pretos, e
      depois usamos a mesma heuríticas para inserir os vértices brancos
      entre os vertices pretos.*/

    ///Loop para gerar a solucao inicial de pretos Pela Heuristica de Inserção Mais Barata do PCV
    ///Com a necessidade apenas de se preocupar com a restrição comprimento do PCVPB
    while (!candidatosPretos.empty()) {

        auto escopoInsercaoMax = static_cast<int>(ceil(candidatosPretos.size() * alfa));
        //int indiceEscolhido=rand()%escopoInsercaoMax;
        std::uniform_int_distribution<> generator(0, escopoInsercaoMax);

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
            //auto escopoInsercaoMax = static_cast<int>(ceil(candidatosPretos.size() * alfa));
            int indiceEscolhido=generator(eng);
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
        auto escopoInsercaoMax = static_cast<int>(ceil(candidatosBrancos.size() * alfa));
        //int indiceEscolhido=rand()%escopoInsercaoMax;
        std::uniform_int_distribution<> generator(0, escopoInsercaoMax);
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
                            viabilidade(&solucao.cadeias[i], custoInsercao)) {
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
            //auto escopoInsercaoMax = static_cast<int>(ceil(candidatosPretos.size() * alfa));
            int indiceEscolhido=generator(eng);
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

    /**
     * Impressao de informacoes da solucao, que foi comentaod, no qual
     * auxiliou a conferir se esta tudo certo com a solução, ou tem algum erro
     * Deixado comentado, caso seja necessário utiliza-la futuramente
     */
/*
    ///Impressão das cadeias
    cout << endl << "Cadeias:" << endl;
    for(int i=0; i<solucao.cadeias.size(); i++)
    {
        vector<int> cadeia = solucao.cadeias[i].getCadeia();
        cout << "Cadeia " << i << ": ";
        for(int j=0; j <cadeia.size(); j++)
        {
            cout << cadeia[j] << " ";
        }
        cout << endl<<"Quantidade de vertices na cadeia: " << solucao.cadeias[i].getCardinalidade() << endl;
        cout << "Custo cadeia: " << solucao.cadeias[i].getComprimento() << endl << endl;

    }
    cout << endl << endl;

    ///Impressao solucao final, conferindo o custo na estrutura Solucao com o custo gerado a partir do cilco hamiltoniano
    double solFinalReal=0;
    cout.precision(10);
    cout << endl << "-------------------------------------------------------" << endl;
    cout << "Solucao Final:";
    for(const auto& it : solucao.listVertices)
        cout << " "<< (it+1);
    for(int i=0; i<ordem; i++)
    solFinalReal+=matrizDistancia[solucao.listVertices[i]][solucao.listVertices[(i+1)%ordem]];
    cout << endl << "Custo Solucao Final: " << solucao.custo << endl;
    cout << "Custo solucao final REAL: " << solFinalReal << endl;
*/
    //cout << "alfa: " << solucao.alfa << endl;

    return solucao;
}

/**
 *
 */
void Grafo::algConstrutGuloso(ofstream *outFile)
{
    Solucao solucao = heuristicaGulosoRandomizado(0);
    imprimeSolucao(1, &solucao, outFile);
}

/**
 *
 * @param alfa
 */
void Grafo::algConstrutGulRandomizado(float alfa, ofstream *outFile)
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
    imprimeSolucao(2, &melhorSolucao, outFile);
}

/**
 *
 */
void Grafo::algConstrutGulRandReativo(ofstream *outFile)
{
    cout.setf(ios::fixed); ///Resultados sempre em decimal
    ///Já roda o guloso para pegar a melhor solução para comparar com as soluções do randomizado reativo
    Solucao melhorSolucao = heuristicaGulosoRandomizado(0);
    const int numAlfas = 10;
    int indiceAlfaAtual;
    const float vetAlfas[numAlfas] = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50}; ///Vetor de alfas
    const int numIteracoes = 1000; ///Ireracoes que ira rodar o heuristicaGulosoRandomizado
    const int iteracoesAtualizaProb = 50; ///A cada 50 iteracoes atualiza a probabilidade de cada alfa ser escolhido
    long somaResultsPorAlfas[numAlfas] = {0};    ///soma dos resultados obtidos com cada alpha
    long qtdEscolhido[numAlfas] ={0};   ///quantidade de vezes que cada alfa foi escolhido e executado
    vector<double> vetProbabilidades = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};///vetor de probabilidades iniciado com a mesma prob pra todos

    discrete_distribution<> distribuicao(vetProbabilidades.begin(),vetProbabilidades.end()); ///Distribuicao discreta, soma de todas é igual a 1
    default_random_engine gerador((unsigned int)time(nullptr));  ///gerador de numeros aleatorios, usado na distribuicao
    for(int i=0; i<numIteracoes; i++)
    {
        //cout << flush;
        //cout << i/10 << "%"<< endl;

        //cout << "it " << i << endl;
        ///escolhe alfaatraves da distribuiçao de probabilidades aleatorio pelo gerador
        indiceAlfaAtual = distribuicao(gerador); /// A distribuicao discreta, retorna um indice de 0 a 9 aleatorio de acordo com as prob
        //cout << "Indice alfa Atual: " << indiceAlfaAtual <<endl;
        Solucao solucaoAtual = heuristicaGulosoRandomizado(vetAlfas[indiceAlfaAtual]);
        somaResultsPorAlfas[indiceAlfaAtual] += solucaoAtual.custo;///soma dos resultados obtidos com esse alfa
        qtdEscolhido[indiceAlfaAtual] += 1;  //numero de execuçoes deste alfa

        ///atualiza valores da melhor soluçao
        if(solucaoAtual.custo < melhorSolucao.custo){
            //cout << "TROCA DE MELHOR SOLUCAO\n";
            //cout << "Melhor Solucao: " <<melhorSolucao.custo << endl;
           // cout << "Alfa melhor solucao: " << melhorSolucao.alfa << endl;
           // cout << "Solucao para troca: " << solucaoAtual.custo << endl;
           // cout << "Alfa solucao para troca: " << solucaoAtual.alfa << endl;
            melhorSolucao = solucaoAtual;
        }

       // cout << "solucao atual: " << solucaoAtual.custo << endl;
      //  cout << "alfa atual: " << solucaoAtual.alfa << endl;
       // cout << "Custo melhor solucao: " << melhorSolucao.custo << endl;
      //  cout << "melhor alfa: " << melhorSolucao.alfa << endl;


        ///Parte do Reativo: a cada 50 iteracoes atualiza a distribuicao de probabilidades
        if((i+1) % iteracoesAtualizaProb == 0){
            double qi[numAlfas] = {0}; ///Utilizado para gerar as novas probabilidades
          //  cout << "Atualizacao de probabilidade\n";
            float somaQi = 0;
            float mediaResultados = 0;
            float delta = 10.0; ///Valor de delta testado, que mais nos agradou

            for(int j=0; j<numAlfas; j++){
                if(qtdEscolhido[j] > 0){
                    mediaResultados = somaResultsPorAlfas[j] / qtdEscolhido[j]; //media dos resultados para alfa
                    qi[j] = pow(melhorSolucao.custo / mediaResultados, delta); // quanto o melhor resultado influencia a novas probabilidades
                    somaQi += qi[j];
                }
            }

           // cout << "Probabilidades da distribuicao: " << endl;
          //  for(auto it : distribuicao.probabilities())
          //      cout << it << " ";
          //  cout << endl;


            ///Atualiza o vetor de probabilidades para cada alfa
            vetProbabilidades.clear();
            for (auto itQi : qi) {
                vetProbabilidades.push_back(itQi / somaQi);
            }

            ///atualiza distribuiçao de probabilidades
            discrete_distribution<> novaDistribuicao(vetProbabilidades.begin(), vetProbabilidades.end());
            distribuicao = novaDistribuicao;
           // cout << "Nova distribuicao\n";
          //  cout << "Probabilidades da nova distribuicao: " << endl;
            //for(auto it : distribuicao.probabilities())
            //    cout << it << " ";
            ///cout << endl;

        }
    }
    //imprimeSolucao(3, &melhorSolucao, outFile);
    cout << melhorSolucao.custo << " " << melhorSolucao.alfa << endl;
}

/**
 *
 * @param cadeia
 * @param custoInsercao
 * @param dist1
 * @param dist2
 * @return
 */
bool Grafo::viabilidade(Cadeia *cadeia, double custoInsercao) const
{
    return cadeia->getComprimento()+custoInsercao<=maxCusto && cadeia->getCardinalidade()<maxVertBranco;///dist1+dist2<maxCusto && nao precisa disso
}

/**
 *
 * @param dist1
 * @param dist2
 * @return
 */
bool Grafo::viabilidade(double dist1, double dist2) const
{
    return dist1<maxCusto && dist2<maxCusto;
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

    if(melhorSolucao->custo != -1)
    {
        if(algoritmo == 1)
        {
            *outFile << endl << endl << "------------------------------------------------------" << endl;
            *outFile << "Algoritmo Construtivo Guloso." << endl;
        }
        else if(algoritmo == 2)
        {
            *outFile << endl << endl << "-------------------------------------------------------" << endl;
            *outFile << "Algoritmo Construtivo Guloso Randomizado." << endl;
            *outFile << "Alfa utilizado: " << setprecision(2) << melhorSolucao->alfa << endl;
        }
        else if(algoritmo == 3)
        {
            *outFile << endl << endl << "-------------------------------------------------------" << endl;
            *outFile << "Algoritmo Construtivo Guloso Randomizado Reativo." << endl;
            *outFile << "Melhor Alfa: " << fixed << setprecision(2) << melhorSolucao->alfa << endl;
        }
        *outFile << "Solucao final de melhor custo: " << fixed << setprecision(10) << melhorSolucao->custo << endl;
        *outFile << "Ciclo Hamiltoniano de menor custo, obedecendo as restricoes do PCVPB:" << endl;
        for (const auto &it : melhorSolucao->listVertices)
            *outFile << (it + 1) << " ";
        *outFile << endl << "-------------------------------------------------------" << endl;

    }

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
            {
                cout << "Algoritmo Construtivo Guloso." << endl;
            }
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
