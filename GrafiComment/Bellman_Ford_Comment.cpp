#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

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

// Classe template che gestisce la struttura del Grafo e l'algoritmo di Bellman-Ford.
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
     * Bellman-Ford:
     * Restituisce una stringa che indica se è stato trovato un ciclo negativo.
     */
    string Bellman_Ford(Node<T> *source) {

        // Passo 1: Inizializzazione dei nodi.
        InitializeSingleSource(source);

        int i = 0;
        // Il ciclo principale deve essere eseguito fino a |V| volte.
        // In un grafo senza cicli negativi, la distanza minima si trova in massimo |V|-1 iterazioni.
        while (i < nodes.size()) {

            // Per ogni iterazione, prova a rilassare tutti gli archi del grafo.
            for (Edge<T> *edge: edges)
                Relax(edge);

            // Controllo della presenza di cicli negativi:
            // Se dopo il rilassamento è ancora possibile migliorare una distanza, significa che esiste un ciclo di peso totale negativo.
            for (Edge<T> *edge: edges)
                if (edge->destination->dist > edge->source->dist + edge->weight)
                    return "Trovato ciclo negativo \n";

            i++;
        }
        return "Non trovato ciclo negativo \n";
    }
};

int main() {

    // Apertura del file di input contenente i dati del grafo.
    ifstream input("inputG.txt");
    if (!input.is_open()) {
        cerr << "Errore durante la lettura del file di input" << endl;
        return 1;
    }

    int numNodes, numEdges;
    // Legge il numero totale di nodi e archi dalla prima riga del file.
    input >> numNodes >> numEdges;

    // Mappa per associare un valore intero al puntatore dell'oggetto Node corrispondente.
    // Evita di creare nodi duplicati durante la lettura degli archi.
    unordered_map<int, Node<int> *> nodesMap;

    Graph<int> g;

    // Ciclo per leggere ogni arco dal file.
    for (int i = 0; i < numEdges; i++) {
        int sourceVal, destVal, weight;
        input >> sourceVal >> destVal >> weight;

        // Se i nodi che compongono l'arco non esistono ancora, vengono creati e aggiunti al grafo.
        for (int val: {sourceVal, destVal}) {
            if (nodesMap.find(val) == nodesMap.end()) {
                auto newNode = new Node<int>(val);
                nodesMap[val] = newNode;
                g.addNode(newNode);
            }
        }

        // Aggiunge l'arco pesato al grafo usando i puntatori recuperati dalla mappa.
        g.addEdge(nodesMap[sourceVal], nodesMap[destVal], weight);
    }

    input.close();

    // Apertura del file di output per scrivere i risultati.
    ofstream output("outputG.txt");
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    // Esecuzione dell'algoritmo partendo dal nodo con valore 1.
    string result;
    result = g.Bellman_Ford(nodesMap[1]);

    // Scrittura del risultato sul file di output.
    output << "Bellman_Ford" << endl;
    output << result;
    output << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
