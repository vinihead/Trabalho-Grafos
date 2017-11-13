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
#include <set>
#include <queue>
#include <climits>

#define INFINITO LONG_MAX
#define SEMCOR -1
#define AZUL 0
#define VERMELHO 1

//Construtor padrão, para no caso de precisar criar um grafo vazio.
Grafo::Grafo(bool digrafo, bool ponderado) {
    this->digrafo = digrafo;
    this->ponderado = ponderado;
    this->ordem = 0;
}

//Já cria o grafo lendo do arquivo de entrada
Grafo::Grafo(ifstream *inFile, bool digrafo)
{
    this->digrafo = digrafo;
    int v1, v2, peso;
    string line;
    stringstream copiaDados;
    getline(*inFile, line);
    int numVertices = atoi(line.c_str());
    ordem = 0;

    cout << "------------------------------" << endl;
    cout << "Instanciando o grafo . . . . ." << endl;
    cout << "------------------------------" << endl;

    getline(*inFile, line); ///Ja pega proxima linha depois da ordem do grago para verificar se eh ponderado
    ///Detectar se eh ponderado, pra isso precisa fazer um getline e "entender o line para saber se tem 3 numeros ou 2
    ponderado = verificaPonderado(line);

    if(inFile->good())
    {
       do
       {
           copiaDados.clear(); //limpar o que tiver na stream, afim de não ter nenhum erro
           copiaDados.str(line);
           ponderado ? copiaDados >> v1 >> v2 >> peso : copiaDados >> v1 >> v2;
           ponderado ? this->adicionaAresta(v1, v2, peso) : this->adicionaAresta(v1, v2, 1.0);
           ///Se nao for ponderado adiciona aresta com peso 1.0, para facilitar na hora de verificar caminhos minimos
       } while(getline(*inFile, line) && !line.empty());
    }
    ///No caso de termos um arquivo de entrada informando quantidade errada de vertices, o programa eh finalizado
    if(this->ordem > numVertices)
    {
       cout << "ERRO! Arquivo de entrada com quantidade de vertices errada." << endl;
       cout << "Numero de vertices no arquivo de entrada eh menor que a quantidade real.\nAlgoritmo FINALIZADO." << endl;
       getchar();
       exit(-1);
    }
    /**
    Neste caso adicionamos os vertices isolados
    Caso o numero de vertices instanciados for menor, existe vertices isolados
    Portando, o algoritmo adiciona os vertices faltantes
    */
    while(this->ordem < numVertices)
    {
       while(true)
       {
           if(!procuraVertice(++v1))
           {
               this->adicionaVertice(v1);
               break;
           }

       }
    }
    cout << "\n---------------------------------------" << endl;
    cout << "Grafo Criado e instanciado com sucesso!" << endl;
    cout << "Quantidade de Vertices: " << getOrdem() << endl;
    cout << "Quantidade de Arestas: " << getQntdArestas() << endl;
    cout << "---------------------------------------" << endl;
}


bool Grafo::verificaPonderado(string s)
{
    int palavras = 1;
    for(int i=0; s[i]!= '\0' && palavras!=3; i++) // "palavras!=3 usada para o caso de ter espaço depois do peso, no arquivo de entrada
    {
        if(s[i] == ' ' )
            palavras++;
    }
    if(palavras == 2)
        return false;
    else if(palavras == 3)
        return true;
    else
    {
        cout << "Erro! Quantidade de \"palavras\" esperadas eh 2 ou 3." << endl;
        cout << "Conclusao: arquivo de entrada nao esta no formato esperado.\n\n";
        exit(-1);
    }

}

list<Vertice>::iterator Grafo::adicionaVertice(int id)
{
    auto itVertice = getVertice(id);
    if(itVertice->getIdVertice() != id) //Se nao encontrar tem que adicionar
    {
        Vertice v(id);
        vertices.push_back(v);
        ordem++;
        return --vertices.end(); //retorna o ultimo vertice, o que foi adicionado
    }
    else
        return itVertice; //retorna o vertice encontrado, que ja foi adicionado

}

bool Grafo::adicionaVerticeNaMain(int id)
{
    auto itVertice = getVertice(id);
    if(itVertice->getIdVertice() != id) //Se nao encontrar tem que adicionar
    {
        Vertice v(id);
        vertices.push_back(v);
        ordem++;
        return true; //retorna o ultimo vertice, o que foi adicionado
    }
    else
        return false; //retorna o vertice encontrado, que ja foi adicionado

}

void Grafo::adicionaAresta(int idOrigem, int idDestino, float peso)
{
    auto itOrigem = adicionaVertice(idOrigem);
    auto itDestino = adicionaVertice(idDestino);
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

bool Grafo::verificaTrivial()
{
    return (ordem == 1);
}

bool Grafo::getPonderado()
{
    return ponderado;
}

bool Grafo::kRegularidade(int k)
{
    for(auto i = vertices.begin(); i != vertices.end(); i++)
        if(grauVertice(i->getIdVertice(), false, nullptr, false) != k)
            return false;
    return true;
}

string Grafo::vizinhacaAberta(int vert)
{
    string salvaDados;
    for(auto it : vertices)
    {
        if(it.getIdVertice() == vert)
        {

            salvaDados += "Vizinhaca Aberta do vertice " ;
            salvaDados += to_string(it.getIdVertice());
            salvaDados += ":\n{";
            salvaDados += it.retornaListAdjacencia(false);
            salvaDados += "}\n" ;
            break;
        }
    }
    return salvaDados;
}
string Grafo::vizinhacaFechada(int vert)
{
    string salvaDados;
    for(auto it : vertices)
    {
        if(it.getIdVertice() == vert)
        {

            salvaDados += "Vizinhaca Fechada do vertice ";
            salvaDados += to_string(it.getIdVertice());
            salvaDados += ":\n{";
            salvaDados += to_string(it.getIdVertice());
            if((it.getGrau()!=0 && !digrafo) || (digrafo && it.getGrauSaida()!=0)) salvaDados += " ";
            salvaDados += it.retornaListAdjacencia(false);
            salvaDados += "}\n" ;
            break;
        }
    }
    return salvaDados;
}
bool Grafo::verificaMultigrafo()
{
    int cont = 0;
    for(auto vertPrincipal = vertices.begin(); vertPrincipal != vertices.end(); vertPrincipal++)
    {
        for(auto vertDinamico = vertices.begin(); vertDinamico != vertices.end(); vertDinamico++)
        {
            if(vertDinamico->contaAdjacencia(vertPrincipal->getIdVertice()) >= 2)
                cont++;
        }
    }
    return (cont >= 2);
}

bool Grafo::verificaCompleto()
{
    int cont = 0;
    for(auto vertPrincipal = vertices.begin(); vertPrincipal != vertices.end(); vertPrincipal++)
    {
        for(auto vertDinamico = vertices.begin(); vertDinamico != vertices.end(); vertDinamico++)
        {
                if(vertPrincipal->procuraAdjacencia(vertDinamico->getIdVertice()))
                {
                    cont++;
                }
        }
    }
    if(((ordem * (ordem - 1)) / 2) == (cont / 2))
    {
        cout << ((ordem * (ordem - 1)) / 2) << endl;
        cout << cont<< endl;
        return true;
    }

    return false;
}

int Grafo::grauVertice(int vert, bool mostraGrau, ofstream *outFile, bool imprimir) //bool mostraGrau é usado para quando precisar imprimir as informaçoes do grau e quando é só retornar o grau.
{
    auto itVert = getVertice(vert);
    if(itVert->getIdVertice() != vert)
    {
        return -1;
    }
    if(digrafo && mostraGrau)
    {
        *outFile << "Grau de entrada: " << itVert->getGrauEntrada()<< endl;
        *outFile << "Grau de saida: " << itVert->getGrauSaida() << endl;
        if(imprimir)
        {
            cout << "Este eh um digrafo, portanto tem grau de entrada e saida" << endl;
            cout << "Grau de entrada: " << itVert->getGrauEntrada() << endl;
            cout << "Grau de saida: " << itVert->getGrauSaida() << endl;
        }
    }
    return itVert->getGrau();
}

unsigned int Grafo::getGrau()
{
    unsigned int grau = vertices.begin()->getGrau();
    for(auto it = vertices.begin()++; it != vertices.end(); it++)
    {
        unsigned int auxGrau = it->getGrau();
        if(auxGrau > grau)
            grau = auxGrau;
    }
    return grau;
}

bool Grafo::verificaNulo()
{
    bool grafoNulo = true; //eh grafo nulo, se existe |V|>0 & |E|=0
    if(ordem > 0) //Um grafo precisa ter pelo menos um vertice
    {
        for(auto itVert : vertices)
        {
            if(grauVertice(itVert.getIdVertice(), false, nullptr, false) > 0)
            {
                grafoNulo = false;
                break;
            }
        }
    }
    return grafoNulo;
}

void Grafo::imprime()
{
    cout << endl << "-----------------------------------" << endl;
    cout << "Imprimindo o grafo:" << endl;
    cout << "Estilo [Vertice] -> Adjacencia|Peso(se existir) ..." << endl;
    cout << "Vertices - Adjacencias\n";
    for(auto it = vertices.begin(); it != vertices.end(); it++)
    {
        cout << "[" << it->getIdVertice() << "]  ->   ";
        it->imprimeListAdjacencia(ponderado);
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
    Grafo *grafoInstancia = new Grafo(false, false);
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


Grafo * Grafo::subgrafoInduzido(vector<int> subconjuntoVertices)
{
    Grafo *subgrafo = new Grafo(digrafo, ponderado);
    Grafo *grafoCopy = retornaInstanciaGrafo();
    for(auto itSubconjVert : subconjuntoVertices)
    {
        subgrafo->adicionaVertice(itSubconjVert);
        list<Aresta> listAdj = grafoCopy->getVertice(itSubconjVert)->getAdjacencia();

        for(auto itAdj : listAdj){
            for (auto it : subconjuntoVertices) {
                if (itAdj.getIdAdj() == it)
                {
                    subgrafo->adicionaAresta(itSubconjVert, itAdj.getIdAdj(), itAdj.getPeso());
                    grafoCopy->removeAresta(itSubconjVert, itAdj.getIdAdj());
                }
            }
        }
    }
    delete grafoCopy;
    return subgrafo;
}

Grafo * Grafo::complementar()
{
    list<int> idVertices;
    Grafo *complementar = new Grafo(digrafo, false);

    for(auto it : vertices)
    {
        idVertices.push_back(it.getIdVertice());
    }

    if(digrafo)
    {
        for(auto it : vertices)
        {
            for (auto k : idVertices)
            {
                if(k!=it.getIdVertice() &&!it.procuraAdjacencia(k)) //so procura a adjacencia se o indice for diferente
                    complementar->adicionaAresta(it.getIdVertice(), k, 1.0);//cout << it->getIdVertice() << " " << k << endl;
            }
        }
    }
    else
    {
        Grafo *grafoCopy = retornaInstanciaGrafo();
        for(auto itVertice = grafoCopy->vertices.begin(); itVertice != grafoCopy->vertices.end(); itVertice++)
        {
            for(auto k : idVertices)
            {
                if(k!=itVertice->getIdVertice() &&!itVertice->procuraAdjacencia(k)) //so procura a adjacencia se o indice for diferente
                {
                    grafoCopy->adicionaAresta(itVertice->getIdVertice(), k, 1.0);
                    complementar->adicionaAresta(itVertice->getIdVertice(), k, 1.0);
                }
            }
        }
    }
    return complementar;
}

/*

void Grafo::componentesFortementeConexas()
{
    if(digrafo)
    {

    }
    else
    {
        cout << "Este eh um grafo nao direcionado.\n";
        cout << "Portanto, todas componentes conexas do grafo, sao fortemente conexos.\n";
        ///Colocar saida no arquivo
    }
}


int Grafo::componentesConexas()
{
    int componenteConexa = 0;
    No *no = primeiroNo;
    while (no != nullptr)
    {
        if(!no->isVisitado())
        {
            componenteConexa++;
            auxComponentesConexas(no);
        }
        no = no->getProx();
    }
    no = primeiroNo;
    while (no != nullptr)
    {
        no->setVisitado(false);
        no = no->getProx();
    }
    return componenteConexa;
}


void Grafo::auxComponentesConexas(No *no)
{
    if(!no->isVisitado())
    {
        no->setVisitado(true);
        Aresta *aresta = no->getPrimAresta();
        while (aresta != nullptr)
        {
            auxComponentesConexas(aresta->getNoDestino());
            aresta = aresta->getProx();
        }
    }
}
*/


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

void Grafo::apresentaSequenciaGraus(ofstream *outFile, bool imprime)
{
    *outFile << endl <<"-> SEQUENCIA DE GRAU DOS VERTICES <-" << endl;
    *outFile << "ID VERTICE: GRAU VERTICE" << endl;
    if(imprime) cout << "ID VERTICE: GRAU VERTICE" << endl;
    for(auto it : vertices)
    {
        *outFile << it.getIdVertice() << ": " << it.getGrau() << endl;
        if(imprime) cout << it.getIdVertice() << ": " << it.getGrau() << endl;
    }
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

string Grafo::arvoreBuscaLargura(int idVert)
{
    queue<int> visitados;
    int idVerticeAtual;
    list<Aresta> adjacentes;
    list<Vertice>::iterator itAdjacenteAtual;
    string textoSalvar = "Arvore de Busca em Largura: ";
    for(auto itVert : vertices)
        itVert.setVisita(false);

    auto origem = getVertice(idVert);
    origem->setVisita(true);
    visitados.push(idVert);

    while(!visitados.empty())
    {
        idVerticeAtual = visitados.front();
        textoSalvar += " -> " + to_string(idVerticeAtual);
        adjacentes = getVertice(idVerticeAtual)->getAdjacencia();
        for(auto itAdj : adjacentes)
        {
            itAdjacenteAtual = getVertice(itAdj.getIdAdj());
            if(!itAdjacenteAtual->getVisitado()) ///Caso nao foi visitado, visita
            {
                itAdjacenteAtual->setVisita(true);
                visitados.push(itAdjacenteAtual->getIdVertice());
            }
        }
        visitados.pop();
    }
    return textoSalvar;
}

bool Grafo::verificaOrdenado()
{
    return digrafo;
}



/*
int Grafo::caminhoMinimoDijkstra(int orig, int dest)
{
    pair<int,int> aux;
    vector<pair<int,int>> abertos;
    vector<int> fechados;
    list<Vertice>::iterator vtcAtual;
    int distanciaIntermediaria;

    getVertice(orig)->setDistancia(0);
    getVertice(orig)->setAntecessor(orig);
    aux = make_pair(0,orig);
    abertos.push_back(aux);

    // Inicializaçao

    for(auto it=vertices.begin(); it != vertices.end(); it++)
    {
        if(it->getIdVertice() != orig)
        {
            it->setDistancia(INFINITO);
            it->setAntecessor(0);
            aux = make_pair(INFINITO,it->getIdVertice());
            abertos.push_back(aux);    // Insere os vertices a lista de vertices abertos
        }
    }

    while(fechados.size() != ordem)
    {
        sort(abertos.begin(), abertos.end());       //ordena por distancia
        vtcAtual = getVertice(abertos[0].second);   //recebe o de menor
        abertos.erase(abertos.begin());

        for(auto it = vtcAtual->getAdjacencia().begin(); it!= vtcAtual->getAdjacencia().end(); it++)
        {
            // Se distancia conhecida for maior que a distancia ate o vertice, passando pelo atual, atualizar.
            distanciaIntermediaria = vtcAtual->getDistancia() + it->getPeso();
            if(getVertice(it->getIdAdj())->getDistancia() > distanciaIntermediaria) {
                getVertice(it->getIdAdj())->setDistancia(distanciaIntermediaria);
            }
        }

        fechados.push_back(vtcAtual->getIdVertice());
    }
    cout << "Distancia: " << getVertice(dest)->getDistancia();
    return getVertice(dest)->getDistancia();
}
*/
/**
 * Algoritmo de caminho minimo: Floyd
 */
long Grafo::caminhoMinimoFloyd(long _idVerticeOrigem, long _idVerticeDestino) {

    long distancias[ordem][ordem];
    list<Vertice>::iterator vtcAtual;

    //inicializaçao
    for(int i=0; i < this->ordem; i++){
        for(int j=0; j < this->ordem; j++){
            if(i==j){
                distancias[i][j]=0;
            }
            else{
                distancias[i][j]=INFINITO;
            }
        }
    }

    //Preenche as adjacencias

    for(auto it = this->vertices.begin(); it != this->vertices.end(); it++)
    {
        vtcAtual = getVertice(it->getIdVertice());

        for(auto itAdj = vtcAtual->getAdjacencia().begin(); itAdj != vtcAtual->getAdjacencia().end(); itAdj++)
        {
            distancias[(vtcAtual->getIdVertice())-1][(itAdj->getIdAdj())-1] = itAdj->getPeso();
        }
    }


    for(int k=0; k<ordem; k++){
        for(int i=0; i<ordem; i++){
            for(int j=0; j<ordem; j++){
                if(i!=j && i!=k && j!=k && distancias[i][k] < INFINITO && distancias[k][j] < INFINITO) {
                    if (distancias[i][j] > distancias[i][k] + distancias[k][j]){
                        distancias[i][j] = distancias[i][k] + distancias[k][j];
                    }
                }
            }
        }
    }

    return distancias[_idVerticeOrigem-1][_idVerticeDestino-1];
}

bool Grafo::verificaBipartido(){
    for (auto itVert = vertices.begin(); itVert!=vertices.end(); itVert++)
        itVert->setCor(SEMCOR);

    for (auto itVert = vertices.begin(); itVert!=vertices.end(); itVert++)
        if(itVert->getCor()==SEMCOR)
            if(!verificaBipartidoAux(itVert,AZUL))
                return false;
    return true;
}

bool Grafo::verificaBipartidoAux(list<Vertice>::iterator itVert, int c)//(int vertice, int passo)
{
    itVert->setCor(c);
    for(auto itAdj : itVert->getAdjacencia())
    {
        if(getVertice(itAdj.getIdAdj())->getCor() == SEMCOR)
        {
            if(c==AZUL)
            {
                if(!verificaBipartidoAux(getVertice(itAdj.getIdAdj()),VERMELHO))
                    return false;
            }
            else if(c==VERMELHO)
            {
                if(!verificaBipartidoAux(getVertice(itAdj.getIdAdj()),AZUL))
                    return false;
            }
        }
        else
        {
            if(getVertice(itAdj.getIdAdj())->getCor()==c)
                return false;
        }

    }
    return true;
}
void Grafo::auxFechoTransitivoDireto(int id, set<int> *percorridos) {

    if(id<0 || id>= vertices.size())
    {
        cout << "Vertice inválido" << endl;
    }
    else
    {
        //insere à lista de vértices percorridos
        percorridos->insert(id);

        // Define como visitado
        getVertice(id)->setVisita(true);

        for (auto adj : getVertice(id)->getAdjacencia())
        {
            int aux = adj.getIdAdj();
            if (!getVertice(aux)->getVisitado())
                auxFechoTransitivoDireto(aux, percorridos);
        }
    }
}

string Grafo::fechoTransitivoDireto(int id) {
    string fechoTransitivo;
    set<int> verticesPercorridos;

    /// Todos vertices não visitados
    for (auto itVert = vertices.begin(); itVert != vertices.end(); itVert++)
        itVert->setVisita(false);

    auxFechoTransitivoDireto(id, &verticesPercorridos);

    cout << id << " -> ";
    for(auto percorridoAtual : verticesPercorridos)
    {
        if(percorridoAtual != id) {   //Não inclui o próprio vértice
            cout << percorridoAtual << " ";
            fechoTransitivo += to_string(percorridoAtual);
            fechoTransitivo += " ";
        }
    }
    return fechoTransitivo;
}


 void Grafo::auxFechoTransitivoIndireto(Grafo *grafoAux, int id, set<int> *percorridos) {

    //Percorre apenas se o vértice já não tiver sido verificado

    percorridos->insert(id); //insere à lista de vértices percorridos

    grafoAux->getVertice(id)->setCor(AZUL); //define como visitado

    for(Aresta it : grafoAux->getVertice(id)->getAdjacencia()){

        int adj = it.getIdAdj();

        if(grafoAux->getVertice(adj)->getVisitado() == SEMCOR) {

            auxFechoTransitivoIndireto(grafoAux, adj, percorridos);  //chama recursivamente a função passando os vértices adjacentes
        }
    }
}

/*string Grafo::fechoTransitivoIndireto(int id) {
    string fechoIntransitivo = "";
    set<int> verticesPercorridos;

    //Seta todos como não-visitados
    for (auto itVert = vertices.begin(); itVert != vertices.end(); itVert++)
        itVert->setCor(SEMCOR);


    if(!digrafo)
    {
        //Grafo não-direcionado o caminho de ida é o mesmo de volta
        auxFechoTransitivoDireto(id, &verticesPercorridos);
    }
    else
    {
        //Cria grafo auxiliar, invertendo o sentido de ligação das arestas
        long i=1;
        long tamanhoGrafo = ordem;

        Grafo grafoAux;

        while(i < tamanhoGrafo)
        {
            grafoAux.adicionaVertice(i);
            i++;
        }
        grafoAux.digrafo = true;

        //Cria Arestas

    }


    cout << id << "-> ";
    for(auto percorridoAtual : verticesPercorridos) {
        if(percorridoAtual != id) {   //Não inclui o próprio vértice
            cout << percorridoAtual << " ";
            fechoIntransitivo += to_string(percorridoAtual);
            fechoIntransitivo += " ";
        }
    }

    cout << "\n\n" << endl;
    return fechoIntransitivo;

}
*/
int Grafo::numComponentesConexas()
{
    int componenteConexa = 0;
    for(auto itVert = vertices.begin(); itVert!=vertices.end(); itVert++)
    {
        if(!itVert->getVisitado())
        {
            componenteConexa++;
            numComponentesConexasAux(itVert);
        }
    }
    return componenteConexa;
}

void Grafo::numComponentesConexasAux(list<Vertice>::iterator itVert)
{
    if(!itVert->getVisitado())
    {
        itVert->setVisita(true);
    }
}

string Grafo::verificaVerticesArticulacao()
{
    int auxVert;
    //vector<int> vertArt;
    string dados;
    list<Aresta> listAresta;

    dados = "Nos de articulacao:";
    for (auto itVert = vertices.begin(); itVert != vertices.end(); itVert++)
    {
        listAresta = itVert->getAdjacencia();
        auxVert = itVert->getIdVertice();
        removeVertice(itVert->getIdVertice());
        if(!verificaConexo())
        {
            dados += " ";
            dados += itVert->getIdVertice();
        }
        adicionaVertice(auxVert);
        for(auto i = listAresta.begin(); i != listAresta.end(); i++)
        {
            adicionaAresta(auxVert, i->getIdAdj(), i->getPeso());
        }
    }
    return dados;
}

void Grafo::verificaArestasPonte(ofstream *outFile)
{
    int auxVert;
    vector<int> vertPonte;
    list<Aresta> listAre;
    int id1, id2;
    float peso;

    /// Seta todos como não-visitados
    for (auto itVert = vertices.begin(); itVert != vertices.end(); itVert++)
    {
        for(auto adj : itVert->getAdjacencia())
        {
            id1 = itVert->getIdVertice();
            id2 = adj.getIdAdj();
            peso = adj.getPeso();
            removeAresta(itVert->getIdVertice(), adj.getIdAdj());

            if(!verificaConexo())
            {
                cout << "Ponte: " << itVert->getIdVertice() << " " << adj.getIdAdj() << endl;
                *outFile << "Ponte: " << itVert->getIdVertice() << " " << adj.getIdAdj() << endl;
            }
            adicionaAresta(id1, id2, peso);
        }
    }
}

int Grafo::getQntdArestas()
{
    int qntd=0;
    for(auto itVert : vertices)
        qntd += itVert.getGrauEntrada();
    if(digrafo)
        return qntd;
    return qntd/2;
}

typedef struct indice
{
    double val;
    int indice;
    bool solucao;

} Indice;
