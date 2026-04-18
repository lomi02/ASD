#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>

using namespace std;

// Classe template che rappresenta un singolo Nodo (o Vertice) del grafo.
template<typename T>
class Node {
public:
    T val;             // Il valore o identificativo contenuto nel nodo.
    int rank{};        // Grado del nodo, utilizzato nell'ottimizzazione del Disjoint Set (Union by Rank).
    Node *parent;      // Puntatore al nodo genitore per la struttura a foreste del Disjoint Set.

    // Costruttore: inizializza il valore e imposta inizialmente il padre a null.
    explicit Node(T val) : val(val), parent(nullptr) {}
};

// Classe template che rappresenta un Arco che collega due nodi.
template<typename T>
class Edge {
public:
    T weight;             // Il peso (costo) dell'arco.
    Node<T> *source;      // Puntatore al nodo di partenza.
    Node<T> *destination; // Puntatore al nodo di arrivo.

    // Costruttore: inizializza peso, sorgente e destinazione dell'arco.
    Edge(T weight, Node<T> *source, Node<T> *destination) : weight(weight), source(source), destination(destination) {}
};

// Classe template che gestisce la struttura del Grafo e gli algoritmi di analisi delle componenti.
template<typename T>
class Graph {
public:
    vector<Node<T> *> nodes; // Lista di tutti i nodi presenti nel grafo.
    vector<Edge<T> *> edges; // Lista di tutti gli archi presenti nel grafo.

    // Lista di adiacenza: mappa il valore di un nodo ai suoi vicini (valore, peso).
    map<T, vector<pair<T, T> > > adjacencyList;

    // Aggiunge un nodo alla lista dei nodi del grafo.
    void addNode(Node<T> *node) { nodes.push_back(node); }

    // Crea un nuovo arco orientato, lo aggiunge alla lista archi e aggiorna la lista di adiacenza.
    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        auto *edge = new Edge<T>(weight, source, destination);
        edges.push_back(edge);
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));
    }

    /**
     * Operazioni Disjoint Set (DSU):
     * Utilizzate per calcolare le componenti connesse in un grafo (trattato come non orientato).
     */
    // Crea un nuovo set per il nodo x, dove x è il genitore di se stesso.
    void MakeSet(Node<T> *x) {
        x->parent = x;
        x->rank = 0;
    }

    // Trova il rappresentante (root) del set a cui appartiene x con "Path Compression".
    Node<T> *FindSet(Node<T> *x) {
        if (x != x->parent)
            x->parent = FindSet(x->parent); // Ricorsione per appiattire la struttura.
        return x->parent;
    }

    // Unisce i set di due nodi x e y.
    void UnionSet(Node<T> *x, Node<T> *y) {
        LinkSet(FindSet(x), FindSet(y));
    }

    // Collega due rappresentanti in base al loro rank (Union by Rank).
    void LinkSet(Node<T> *x, Node<T> *y) {
        if (x->rank > y->rank)
            y->parent = x;
        else {
            x->parent = y;
            if (x->rank == y->rank)
                ++y->rank;
        }
    }

    /**
     * ConnectedComponents:
     * Identifica le componenti connesse del grafo usando la struttura dati Disjoint Set.
     */
    void ConnectedComponents(ofstream &output) {
        map<Node<T> *, vector<Node<T> *> > connectedComponentMap;

        // Passo 1: Crea un set individuale per ogni nodo.
        for (auto v: nodes)
            MakeSet(v);

        // Passo 2: Per ogni arco, unisce i set dei due nodi collegati.
        for (auto edge: edges)
            if (FindSet(edge->source) != FindSet(edge->destination))
                UnionSet(edge->source, edge->destination);

        // Passo 3: Raggruppa i nodi in base al loro rappresentante finale.
        for (auto v: nodes) {
            Node<T> *representative = FindSet(v);
            connectedComponentMap[representative].push_back(v);
        }

        // Passo 4: Stampa ogni gruppo (componente) su una riga diversa.
        for (const auto &pair: connectedComponentMap) {
            for (const auto &node: pair.second)
                output << node->val << " ";
            output << endl;
        }
    }

    // Mostra i vicini di un nodo leggendoli dalla lista di adiacenza.
    void ShowAdjacentList(Node<T> *u, ofstream &output) {
        output << "Lista di adiacenza del nodo " << u->val << ":\n";
        for (auto &it: adjacencyList[u->val])
            output << " " << u->val << " -> " << it.first << " (peso " << it.second << ")\n";
    }

    /**
     * Algoritmo di Kosaraju (Strongly Connected Components - SCC):
     */
    // DFS utilizzata nel primo passo per riempire lo stack in ordine di tempo di fine visita.
    void DFSUtil(int v, map<int, bool> &visited, stack<int> &st) {
        visited[v] = true;
        for (auto &edge: adjacencyList[v])
            if (!visited[edge.first])
                DFSUtil(edge.first, visited, st);
        st.push(v); // Una volta finiti i vicini, il nodo viene aggiunto allo stack.
    }

    // DFS utilizzata nel secondo passo sul grafo trasposto per identificare i nodi della stessa SCC.
    void DFSUtil2(int v, map<int, bool> &visited, ofstream &output) {
        visited[v] = true;
        output << v << " ";
        for (auto &edge: adjacencyList[v])
            if (!visited[edge.first])
                DFSUtil2(edge.first, visited, output);
    }

    // Crea e restituisce il grafo trasposto (tutti gli archi invertiti).
    Graph Transpose() {
        Graph transposedGraph;
        unordered_map<T, Node<T> *> transNodesMap;

        // Copia i nodi nel nuovo grafo.
        for (auto &node: nodes) {
            Node<T> *newNode = new Node<T>(node->val);
            transposedGraph.addNode(newNode);
            transNodesMap[node->val] = newNode;
        }
        // Inverte la direzione di ogni arco esistente.
        for (const auto &edge: edges) {
            transposedGraph.addEdge(transNodesMap[edge->destination->val], transNodesMap[edge->source->val], edge->weight);
        }
        return transposedGraph;
    }

    // Esegue l'algoritmo di Kosaraju per trovare e stampare le SCC.
    void PrintSCCs(ofstream &output) {
        stack<int> st;
        map<int, bool> visited;

        // Passo 1: Inizializzazione e prima DFS per ordinare i nodi nello stack.
        for (auto n: nodes) visited[n->val] = false;

        for (auto n: nodes)
            if (!visited[n->val])
                DFSUtil(n->val, visited, st);

        // Passo 2: Trasposizione del grafo.
        Graph gT = Transpose();

        // Passo 3: Seconda DFS seguendo l'ordine dello stack sul grafo trasposto.
        for (auto n: nodes) visited[n->val] = false;

        while (!st.empty()) {
            int v = st.top();
            st.pop();
            if (!visited[v]) {

                // Ogni chiamata a DFSUtil2 qui identifica una nuova Componente Fortemente Connessa.
                gT.DFSUtil2(v, visited, output);
                output << endl;
            }
        }
    }
};

int main() {

    // Apertura del file di input.
    ifstream input("inputG.txt");
    if (!input.is_open()) {
        cerr << "Errore durante la lettura del file di input" << endl;
        return 1;
    }

    int numNodes, numEdges;
    input >> numNodes >> numEdges;

    // Mappa per gestire i puntatori ai nodi ed evitare duplicati.
    unordered_map<int, Node<int> *> nodesMap;
    Graph<int> g;

    // Lettura degli archi e creazione del grafo.
    for (int i = 0; i < numEdges; i++) {
        int sourceVal, destVal, weight;
        input >> sourceVal >> destVal >> weight;

        for (int val: {sourceVal, destVal}) {
            if (nodesMap.find(val) == nodesMap.end()) {
                auto newNode = new Node<int>(val);
                nodesMap[val] = newNode;
                g.addNode(newNode);
            }
        }

        // Aggiunge l'arco al grafo (trattato come non orientato).
        g.addEdge(nodesMap[sourceVal], nodesMap[destVal], weight);
    }
    input.close();

    // Apertura del file di output.
    ofstream output("outputG.txt");
    if (!output.is_open()) return 1;

    // Sezione 1: Visualizzazione liste di adiacenza.
    output << "Lista di Adiacenza:" << endl;
    for (Node<int> *node: g.nodes)
        g.ShowAdjacentList(node, output);

    // Sezione 2: Calcolo Componenti Connesse (DSU).
    output << "\nComponenti connesse: " << endl;
    g.ConnectedComponents(output);

    // Sezione 3: Calcolo Componenti Fortemente Connesse (Kosaraju).
    output << "\nComponenti fortemente connesse:" << endl;
    g.PrintSCCs(output);

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
