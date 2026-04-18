#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

// Definizione di una costante per rappresentare l'infinito.
// Viene usata per inizializzare le distanze dei nodi non ancora raggiunti.
constexpr int INFINITY = 9999;

// Classe template che rappresenta un singolo Nodo (o Vertice) del grafo.
template<typename T>
class Node {
public:
    T val;             // Il valore o identificativo contenuto nel nodo.
    T dist = INFINITY; // La distanza minima stimata dalla sorgente (inizializzata a infinito).

    // Costruttore: inizializza il valore del nodo.
    explicit Node(T val) : val(val) {}
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

// Classe template che gestisce la struttura del Grafo e l'algoritmo di Dijkstra.
template<typename T>
class Graph {
public:
    vector<Node<T> *> nodes; // Lista di tutti i nodi presenti nel grafo.
    vector<Edge<T> *> edges; // Lista di tutti gli archi presenti nel grafo.

    // Aggiunge un nodo alla lista dei nodi del grafo.
    void addNode(Node<T> *node) { nodes.push_back(node); }

    // Crea un nuovo arco pesato tra due nodi e lo aggiunge alla lista degli archi.
    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        auto *edge = new Edge<T>(weight, source, destination);
        edges.push_back(edge);
    }

    /**
     * Inizializzazione:
     * Imposta la distanza di tutti i nodi a INFINITY e la distanza del nodo sorgente a 0.
     */
    void InitializeSingleSource(Node<T> *source) {
        for (auto node: nodes)
            node->dist = INFINITY;
        source->dist = 0;
    }

    /**
     * Relax (Rilassamento):
     * Tenta di migliorare la distanza minima verso un nodo.
     * Se passare per 'source' costa meno della distanza attuale di 'destination', aggiorna 'destination'.
     */
    void Relax(Edge<T> *edge) {
        T newDist = edge->source->dist + edge->weight;
        if (newDist < edge->destination->dist)
            edge->destination->dist = newDist;
    }

    /**
     * Dijkstra:
     * Calcola i cammini minimi da una sorgente singola in un grafo con pesi non negativi.
     */
    void Dijkstra(Node<T> *source) {

        // Passo 1: Inizializzazione della distanza dei nodi.
        InitializeSingleSource(source);

        // Passo 2: Utilizzo di una coda di priorità (Min-Priority Queue).
        // La coda memorizza coppie (distanza, puntatore al nodo) ordinate per distanza crescente.
        priority_queue<pair<T, Node<T> *>, vector<pair<T, Node<T> *> >, greater<pair<T, Node<T> *> > > pq;
        pq.push({source->dist, source});

        while (!pq.empty()) {

            // Passo 3: Estrazione del nodo con la distanza minima stimata.
            Node<T> *curr = pq.top().second;
            pq.pop();

            // Passo 4: Esplorazione degli archi uscenti dal nodo corrente.
            for (Edge<T> *edge: edges) {
                if (edge->source == curr) {

                    // Tentativo di rilassamento dell'arco.
                    Relax(edge);

                    // Inserimento della nuova distanza nella coda per future elaborazioni.
                    pq.push({edge->destination->dist, edge->destination});
                }
            }
        }
    }
};

int main() {
    bool pesiNegativi = false; // Flag per verificare la presenza di pesi negativi.

    // Apertura del file di input contenente i dati del grafo.
    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Errore durante la lettura del file di input" << endl;
        return 1;
    }

    int numNodes, numEdges;
    // Legge il numero totale di nodi e archi dalla prima riga del file.
    input >> numNodes >> numEdges;

    // Mappa per associare un valore intero al puntatore dell'oggetto Node corrispondente.
    unordered_map<int, Node<int> *> nodesMap;
    Graph<int> g;

    // Ciclo per leggere ogni arco dal file.
    for (int i = 0; i < numEdges; i++) {
        int sourceVal, destVal, weight;
        input >> sourceVal >> destVal >> weight;

        // Se i nodi non esistono ancora nella mappa, vengono creati e aggiunti al grafo.
        for (int val: {sourceVal, destVal}) {
            if (nodesMap.find(val) == nodesMap.end()) {
                auto newNode = new Node<int>(val);
                nodesMap[val] = newNode;
                g.addNode(newNode);
            }
        }
        // Aggiunge l'arco pesato al grafo.
        g.addEdge(nodesMap[sourceVal], nodesMap[destVal], weight);

        // Dijkstra non è applicabile se sono presenti pesi negativi.
        if (weight < 0)
            pesiNegativi = true;
    }
    input.close();

    // Apertura del file di output per scrivere i risultati.
    ofstream output("output.txt");
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    // Controllo validità dell'algoritmo per il grafo fornito.
    if (pesiNegativi) {
        output << "Dijkstra non puo' essere effettuato in quanto vi sono pesi negativi" << endl;
    } else {
        output << "Dijkstra" << endl;

        // Esecuzione dell'algoritmo partendo dal nodo con valore 1 (se presente).
        if (nodesMap.count(1)) {
            g.Dijkstra(nodesMap[1]);

            // Scrittura delle distanze minime calcolate su file.
            for (Node<int> *node: g.nodes) {
                output << "Distanza piu breve da nodo " << nodesMap[1]->val << " a nodo " << node->val << ": " << node->dist << endl;
            }
        }
    }

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
