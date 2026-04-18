#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <queue>

using namespace std;

// Definiamo dei numeri interi per rappresentare lo stato di esplorazione di un nodo
constexpr int WHITE = 0;       // Il nodo è "nuovo", non è ancora stato toccato
constexpr int GRAY = 1;        // Il nodo è stato scoperto ma i suoi vicini sono ancora da controllare
constexpr int BLACK = 2;       // Il nodo e tutti i suoi vicini sono stati esplorati completamente
constexpr int INFINITY = 9999; // Usato per dire "non so ancora quanto sia lontano questo nodo"

// Template <typename T> permette alla classe di gestire sia int, che float, che altri tipi
template<typename T>
class Node {
public:
    T val;                // Il nome o ID del nodo (es. 1, 2, 3...)
    T key;                // Variabile di supporto (spesso usata in algoritmi come Prim)
    int rank{};           // Usato per l'unione di set (algoritmo di Kruskal)
    int color = WHITE;    // All'inizio, ogni nodo creato è WHITE (non visitato)
    T dist = INFINITY;    // All'inizio, la distanza dalla sorgente è sconosciuta (infinito)
    T discovery_time = 0; // Quando il nodo viene trovato (usato in DFS)
    T finish_time = 0;    // Quando il nodo ha finito di essere esplorato (usato in DFS)

    Node *parent;         // Puntatore al nodo che ci ha permesso di scoprire questo nodo

    // Costruttore: inizializza il valore e imposta il genitore a nullo
    explicit Node(T val) : val(val), parent(nullptr) {}
};

template<typename T>
class Edge {
public:
    T weight;             // Il "costo" o peso per attraversare questo arco
    Node<T> *source;      // Da dove parte l'arco (puntatore a oggetto Node)
    Node<T> *destination; // Dove arriva l'arco (puntatore a oggetto Node)

    // Costruttore dell'arco
    Edge(T weight, Node<T> *source, Node<T> *destination) : weight(weight), source(source), destination(destination) {}
};

template<typename T>
class Graph {
public:
    T time = 0;              // Contatore globale per i tempi di scoperta/fine
    vector<Node<T> *> nodes; // Contenitore che tiene traccia di tutti i nodi nel grafo
    vector<Edge<T> *> edges; // Contenitore che tiene traccia di tutti gli archi nel grafo

    // Mappa dove la chiave è il valore del nodo e il valore è una lista di coppie (vicino, peso)
    map<T, vector<pair<T, T> > > adjacencyList;

    // Aggiunge un nodo alla lista globale dei nodi
    void addNode(Node<T> *node) { nodes.push_back(node); }

    void addEdge(Node<T> *source, Node<T> *destination, T weight) {

        // Crea dinamicamente un nuovo oggetto Edge nella memoria (heap)
        auto *edge = new Edge<T>(weight, source, destination);

        // Lo salva nel vettore globale degli archi
        edges.push_back(edge);

        // Aggiunge alla lista di adiacenza del nodo sorgente il nodo destinazione e il suo peso
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));

        // Poiché il grafo è non orientato, fa lo stesso al contrario
        adjacencyList[destination->val].push_back(make_pair(source->val, weight));
    }

    /**
     * INITIALIZE SINGLE SOURCE
     * Prepara il grafo per il calcolo.
     */
    void InitializeSingleSource(Node<T> *source) {
        for (auto node: nodes)
            node->dist = INFINITY; // Tutti i nodi sono inizialmente irraggiungibili

        source->dist = 0; // La distanza dalla sorgente a se stessa è zero
    }

    /**
     * RELAX (Rilassamento)
     * È l'operazione fondamentale: prova a migliorare la distanza nota di un nodo.
     */
    void Relax(Edge<T> *edge) {

        // Calcoliamo: "Distanza per arrivare alla sorgente dell'arco" + "Peso dell'arco"
        T newDist = edge->source->dist + edge->weight;

        // Se questa nuova distanza è MINORE di quella che già conoscevamo per la destinazione
        if (newDist < edge->destination->dist) {

            // Allora abbiamo trovato una strada più breve! Aggiorniamo il nodo.
            edge->destination->dist = newDist;

            // (Opzionale nel tuo codice) edge->destination->parent = edge->source;
        }
    }

    /**
     * DIJKSTRA
     * L'algoritmo "greedy" che sceglie sempre il nodo più vicino non ancora elaborato.
     */
    void Dijkstra(Node<T> *source) {

        // 1. Reset iniziale delle distanze
        InitializeSingleSource(source);

        // 2. Creazione della CODA DI PRIORITÀ (Min-Priority Queue)
        // A differenza della BFS (coda semplice), questa tiene i nodi ordinati per distanza.
        // Il più "vicino" (distanza minore) è sempre in cima (top).
        priority_queue<pair<T, Node<T> *>, vector<pair<T, Node<T> *> >, greater<pair<T, Node<T> *> > > pq;

        // Inseriamo la sorgente nella coda: {distanza, puntatore_nodo}
        pq.push({source->dist, source});

        while (!pq.empty()) {

            // Estraiamo il nodo con la distanza MINIMA attualmente presente nella coda
            Node<T> *curr = pq.top().second;
            pq.pop();

            // 3. Esplorazione degli archi
            for (Edge<T> *edge: edges) {

                // Se l'arco parte dal nodo che stiamo analizzando...
                if (edge->source == curr) {
                    T oldDist = edge->destination->dist;

                    // Proviamo a "rilassare" l'arco
                    Relax(edge);

                    // Se dopo il relax la distanza è diminuita, aggiungiamo il nodo alla coda per esplorare i suoi vicini con la nuova distanza migliorata.
                    if (edge->destination->dist < oldDist) {
                        pq.push({edge->destination->dist, edge->destination});
                    }
                }
            }
        }
    }
};

int main() {
    bool pesiNegativi = false;   // Flag per ricordarci se abbiamo incontrato pesi < 0
    ifstream input("input.txt"); // Apriamo il file in lettura

    // Controllo sicurezza: il file esiste?
    if (!input.is_open()) {
        cerr << "Errore: File input.txt non trovato!" << endl;
        return 1;
    }

    int numNodes, numEdges;
    input >> numNodes >> numEdges; // Legge i primi due numeri dal file

    // Mappa per associare un numero intero (ID) al puntatore dell'oggetto Node reale
    unordered_map<int, Node<int> *> nodesMap;
    Graph<int> g;

    // Ciclo per leggere ogni arco definito nel file
    for (int i = 0; i < numEdges; i++) {
        int sourceVal, destVal, weight;
        input >> sourceVal >> destVal >> weight; // Legge: Da dove, A dove, Quanto pesa

        // GESTIONE SORGENTE:
        Node<int> *sourceNode = nullptr;
        auto itSource = nodesMap.find(sourceVal);
        if (itSource == nodesMap.end()) {

            // Se non esiste nella mappa, è un nodo nuovo: lo creiamo
            sourceNode = new Node<int>(sourceVal);
            nodesMap[sourceVal] = sourceNode; // Lo salviamo nella mappa
            g.addNode(sourceNode);            // Lo aggiungiamo al grafo
        } else {

            // Se esisteva già, prendiamo il puntatore esistente
            sourceNode = itSource->second;
        }

        // GESTIONE DESTINAZIONE (Logica identica alla sorgente):
        Node<int> *destNode = nullptr;
        auto itDest = nodesMap.find(destVal);
        if (itDest == nodesMap.end()) {
            destNode = new Node<int>(destVal);
            nodesMap[destVal] = destNode;
            g.addNode(destNode);
        } else {
            destNode = itDest->second;
        }

        // Ora che abbiamo i puntatori ai due nodi, creiamo l'arco tra loro
        g.addEdge(sourceNode, destNode, weight);

        if (weight < 0) pesiNegativi = true; // Annotiamo se il peso è negativo
    }

    input.close(); // Chiudiamo il file di input perché abbiamo finito di leggere

    ofstream output("output.txt"); // Apriamo il file di output per scrivere i risultati
    if (!output.is_open()) {
        cout << "Errore creazione output.txt" << endl;
        return 1;
    }

    if (pesiNegativi) {
        // Se il flag è true, informiamo l'utente che Dijkstra non può lavorare correttamente.
        // In questi casi si dovrebbe usare un algoritmo diverso come Bellman-Ford.
        output << "Dijkstra non puo' essere effettuato in quanto vi sono pesi negativi" << endl;
    }
    else {
        // Se tutti i pesi sono >= 0, procediamo con l'algoritmo.
        output << "Dijkstra" << endl;

        // Eseguiamo Dijkstra partendo dal nodo con ID 1.
        // nodesMap[1] recupera il puntatore all'oggetto Node creato durante la lettura.
        g.Dijkstra(nodesMap[1]);

        // Ciclo finale per stampare i risultati:
        for (Node<int> *node: g.nodes) {
            // node->dist ora contiene il cammino minimo calcolato dall'algoritmo.
            output << "Distanza piu breve da nodo " << nodesMap[1]->val << " a nodo " << node->val << ": " << node->dist << endl;
        }
    }

    output.close(); // Chiudiamo il file
    cout << "Operazione completata con successo!" << endl;

    return 0;
}