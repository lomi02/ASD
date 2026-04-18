#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Classe template che rappresenta un singolo Nodo (o Vertice) del grafo.
template<typename T>
class Node {
public:
    T val;             // Il valore o identificativo contenuto nel nodo.
    int rank{};        // Grado del nodo, utilizzato per ottimizzare l'unione dei set (Union by Rank).
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

// Classe template che gestisce la struttura del Grafo e l'algoritmo di Kruskal.
template<typename T>
class Graph {
public:
    vector<Node<T> *> nodes; // Lista di tutti i nodi presenti nel grafo.
    vector<Edge<T> *> edges; // Lista di tutti gli archi presenti nel grafo.

    // Aggiunge un nodo alla lista dei nodi del grafo.
    void addNode(Node<T> *node) { nodes.push_back(node); }

    // Crea un nuovo arco pesato tra due nodi e lo aggiunge alla lista globale degli archi.
    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        auto *edge = new Edge<T>(weight, source, destination);
        edges.push_back(edge);
    }

    /**
     * OPERAZIONI DISJOINT SET (DSU):
     * Indispensabili per Kruskal per rilevare cicli in modo efficiente.
     */

    // Inizializza un nuovo set contenente il solo nodo x.
    void MakeSet(Node<T> *x) {
        x->parent = x;
        x->rank = 0;
    }

    // Trova il rappresentante del set di x applicando la "Path Compression".
    Node<T> *FindSet(Node<T> *x) {
        if (x != x->parent)
            x->parent = FindSet(x->parent);
        return x->parent;
    }

    // Unisce i set dei due nodi x e y tramite i rispettivi rappresentanti.
    void UnionSet(Node<T> *x, Node<T> *y) {
        LinkSet(FindSet(x), FindSet(y));
    }

    // Collega due alberi (set) mettendo quello con rank minore sotto la radice di quello con rank maggiore.
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
     * Kruskal:
     * Trova il Minimum Spanning Tree (MST) selezionando gli archi con peso minore che non creano cicli.
     */
    vector<Edge<T> *> Kruskal() {
        vector<Edge<T> *> A; // Insieme degli archi che faranno parte del MST.

        // Passo 1: Crea un set separato per ogni nodo del grafo.
        for (auto node: nodes)
            MakeSet(node);

        // Passo 2: Ordina tutti gli archi del grafo in ordine crescente di peso (approccio greedy).
        sort(edges.begin(), edges.end(), [](Edge<T> *a, Edge<T> *b) {
            return a->weight < b->weight;
        });

        // Passo 3: Esplora gli archi in ordine di peso.
        for (auto edge: edges) {

            // Se i rappresentanti di sorgente e destinazione sono diversi, l'arco non chiude un ciclo.
            if (FindSet(edge->source) != FindSet(edge->destination)) {

                A.push_back(edge);                         // Aggiunge l'arco alla soluzione.
                UnionSet(edge->source, edge->destination); // Unisce i due set.
            }
        }
        return A;
    }
};

int main() {

    // Apertura del file di input contenente la struttura del grafo.
    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Errore durante la lettura del file di input" << endl;
        return 1;
    }

    int numNodes, numEdges;
    // Legge il numero di nodi e di archi.
    input >> numNodes >> numEdges;

    // Mappa per associare gli interi letti agli oggetti Node, evitando duplicati.
    unordered_map<int, Node<int> *> nodesMap;
    Graph<int> g;

    // Lettura degli archi dal file e costruzione del grafo.
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

    // Apertura del file di output per i risultati del MST.
    ofstream output("output.txt");
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    output << "Kruskal" << endl;

    // Esecuzione dell'algoritmo di Kruskal.
    vector<Edge<int> *> mst_K = g.Kruskal();

    // Stampa degli archi selezionati per il Minimum Spanning Tree.
    for (auto edge: mst_K)
        output << edge->source->val << " - " << edge->destination->val << " " << edge->weight << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
