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
     * PRIM
     * Costruisce l'Albero di Copertura Minimo (MST).
     */
    int Prim(ofstream &output) {
        int src = 0; // Partiamo convenzionalmente dal primo nodo (indice 0)

        // Vettore delle "chiavi": rappresenta il peso minimo per collegare il nodo i all'albero che stiamo costruendo.
        vector<int> key(nodes.size(), INFINITY);

        // Vettore booleano per sapere quali nodi fanno già parte dell'MST.
        vector<bool> inMST(nodes.size(), false);

        // Vettore per ricostruire la struttura dell'albero (chi è collegato a chi).
        vector<int> parent(nodes.size(), -1);

        // Priority Queue (Min-Heap): estrae sempre il nodo con la 'key' più piccola.
        // Formato: <valore_chiave, indice_nodo>
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

        // Inizializzazione: il primo nodo entra nell'albero con costo 0
        pq.emplace(0, src);
        key[src] = 0;

        while (!pq.empty()) {

            // Estraiamo il nodo 'u' che costa meno collegare all'albero corrente
            int u = pq.top().second;
            pq.pop();

            // Aggiungiamo ufficialmente il nodo all'MST
            inMST[u] = true;

            // Esploriamo tutti i vicini di 'u'
            for (auto v: adjacencyList[nodes[u]->val]) {
                int vertex = v.first;  // ID del vicino
                int weight = v.second; // Peso dell'arco (u, v)

                // Qui usiamo solo 'weight', NON 'key[u] + weight'.
                if (!inMST[vertex] && weight < key[vertex]) {
                    key[vertex] = weight;            // Aggiorniamo il costo di connessione
                    pq.emplace(key[vertex], vertex); // Inseriamo in coda
                    parent[vertex] = u;              // Memorizziamo il collegamento
                }
            }
        }

        // Calcolo del costo totale sommando i pesi degli archi scelti
        int costoTotale = 0;
        for (int i = 1; i < nodes.size(); ++i) {
            output << parent[i] << " - " << i << " " << key[i] << endl;
            costoTotale += key[i];
        }
        return costoTotale;
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

    output << "Prim" << endl;

    // Chiamiamo la funzione Prim.
    // Notiamo due cose:
    // 1. Passiamo 'output' come riferimento (ofstream &output) perché la funzione scriverà direttamente nel file i singoli archi dell'MST (es. "0 - 1 peso 5").
    // 2. La funzione restituisce un intero, che rappresenta la somma dei pesi di tutti gli archi scelti per l'albero.
    int costoTotale = g.Prim(output);

    // Stampiamo il valore finale restituito dalla funzione.
    // Questo valore è il "peso del Minimum Spanning Tree".
    output << "Costo totale dell'MST: " << costoTotale << endl;
    output << endl;

    output.close(); // Chiudiamo il file
    cout << "Operazione completata con successo!" << endl;

    return 0;
}