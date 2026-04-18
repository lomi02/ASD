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

    // Algoritmo di Ricerca in Ampiezza (Iterativo)
    void BFS(Node<T> *startNode) {

        // Ciclo su tutti i nodi registrati nel grafo
        for (Node<T> *node: nodes) {
            node->color = WHITE;      // Reset dello stato: nessuno è visitato
            node->parent = nullptr;   // Reset del percorso: nessuno ha un padre
            node->dist = INFINITY;    // Reset della distanza: tutti sono lontani
        }

        startNode->color = GRAY;      // Il nodo di partenza viene scoperto ora
        startNode->parent = nullptr;  // Il nodo di partenza non ha genitori
        startNode->dist = 0;          // La distanza da se stessi è sempre 0

        queue<Node<T> *> q;           // Creiamo una coda FIFO (First In, First Out)
        q.push(startNode);            // Mettiamo il nodo di partenza nella coda

        // Finché c'è qualcuno nella coda, il lavoro non è finito
        while (!q.empty()) {
            Node<T> *u = q.front();   // Prendiamo il primo nodo della coda
            q.pop();                  // Lo rimuoviamo dalla coda per lavorarlo

            // Guardiamo la lista dei vicini di 'u' nella adjacencyList
            for (auto v: adjacencyList[u->val]) {
                Node<T> *node = nullptr;

                // Cerchiamo l'oggetto Node effettivo che corrisponde al valore 'v.first'
                for (Node<T> *n: nodes) {
                    if (n->val == v.first) {
                        node = n;     // Trovato l'oggetto nodo corrispondente al vicino
                        break;
                    }
                }

                if (node == nullptr) continue; // Se per errore non lo troviamo, passiamo oltre

                // Se il vicino è WHITE, significa che non lo abbiamo mai visto prima
                if (node->color == WHITE) {
                    node->color = GRAY;              // Lo marchiamo come "scoperto"
                    node->parent = u;                // Diciamo che è stato scoperto grazie a 'u'
                    node->dist = u->dist + v.second; // Distanza totale = distanza di u + peso arco
                    q.push(node);                    // Mettiamo il vicino in coda per esplorare i SUOI vicini dopo
                }
            }
            u->color = BLACK; // Abbiamo finito di guardare tutti i vicini di 'u'
        }
    }

    // Algoritmo di Ricerca (Ricorsivo)
    void BFSRecursive(Node<T> *u) {
        u->color = GRAY; // Entriamo nel nodo e lo marchiamo come in lavorazione

        // Esploriamo i vicini di u
        for (auto v: adjacencyList[u->val]) {
            Node<T> *node = nullptr;

            // Cerchiamo l'oggetto Node reale nel vettore globale
            for (Node<T> *n: nodes) {
                if (n->val == v.first) {
                    node = n;
                    break;
                }
            }

            if (node == nullptr) continue;

            // Se il vicino non è mai stato visitato
            if (node->color == WHITE) {
                node->parent = u;                // Imposta il padre
                node->dist = u->dist + v.second; // Calcola la distanza cumulata
                BFSRecursive(node);              // CHIAMATA RICORSIVA: sospende il nodo 'u' e va dentro 'node'
            }
        }
        u->color = BLACK; // Finito l'esplorazione di questo ramo, il nodo diventa nero
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

    output << "--- ANALISI BFS ---" << endl;
    // Avviamo la BFS partendo dal nodo che ha valore ID = 1
    g.BFS(nodesMap[1]);

    // Scriviamo nel file le distanze calcolate per ogni nodo
    for (Node<int> *node: g.nodes) {
        output << "Distanza dal nodo 1 al nodo " << node->val << ": " << node->dist << endl;
    }

    output.close(); // Chiudiamo il file
    cout << "Operazione completata con successo!" << endl;

    return 0;
}