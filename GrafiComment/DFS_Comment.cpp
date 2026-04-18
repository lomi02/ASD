#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>

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
     * DFS (Depth-First Search) - IL COORDINATORE
     * Questa funzione prepara il terreno e gestisce grafi con più componenti separate.
     */
    void DFS() {
        // 1. Fase di Reset iniziale
        for (auto u: nodes) {
            u->color = WHITE;      // Ogni nodo deve essere "nuovo"
            u->parent = nullptr;   // Nessun legame precedente
        }

        time = 0; // Azzeriamo il cronometro prima di iniziare l'esplorazione

        // 2. Ciclo di sicurezza per componenti non connesse
        // Questo ciclo garantisce che se il grafo è diviso in due "isole" non collegate,
        // la DFS le visiti entrambe.
        for (auto u: nodes) {
            if (u->color == WHITE) {
                // Se troviamo un nodo ancora bianco, facciamo partire una visita da lì
                DFS_Visit(u);
            }
        }
    }

    /**
     * DFS_VISIT - IL CUORE RICORSIVO
     * Questa funzione si "tuffa" nel grafo finché può.
     */
    void DFS_Visit(Node<T> *u) {

        // --- ENTRATA NEL NODO ---
        u->color = GRAY;            // Il nodo entra in lavorazione
        u->discovery_time = ++time; // Incrementa il tempo e lo segna come tempo di SCOPERTA

        // Esploriamo i vicini del nodo corrente 'u'
        for (auto v: adjacencyList[u->val]) {
            Node<T> *nodeV = nullptr;

            // Traduciamo il valore della lista di adiacenza nel puntatore all'oggetto Node
            for (Node<T> *n: nodes) {
                if (n->val == v.first) {
                    nodeV = n;
                    break;
                }
            }

            if (nodeV == nullptr) continue;

            // Se il vicino è ancora WHITE (mai visto prima)
            if (nodeV->color == WHITE) {
                nodeV->parent = u;     // Stabiliamo che 'u' è il padre di 'nodeV' nell'albero DFS

                // --- CHIAMATA RICORSIVA ---
                // Il computer mette in pausa l'esplorazione di 'u' e si lancia dentro 'nodeV'.
                // Questo crea l'effetto "tunnel" (andiamo in profondità).
                DFS_Visit(nodeV);
            }
        }

        // --- USCITA DAL NODO (BACKTRACKING) ---
        // Se arriviamo qui, significa che abbiamo esplorato tutto quello che potevamo a partire da 'u'.
        u->color = BLACK;          // Il nodo è chiuso, non ci torneremo più
        u->finish_time = ++time;   // Incrementa il tempo e lo segna come tempo di COMPLETAMENTO
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

    output << "DFS" << endl;
    // Eseguiamo la visita
    g.DFS();

    // Stampa dei tempi
    for (Node<int> *node: g.nodes) {
        // La differenza tra finish_time e discovery_time indica quanto tempo
        // la DFS ha passato "dentro" quel nodo e i suoi discendenti.
        output << "Nodo " << node->val << ": tempo di scoperta = " << node->discovery_time <<", tempo di completamento = " << node->finish_time << endl;
    }

    output.close(); // Chiudiamo il file
    cout << "Operazione completata con successo!" << endl;

    return 0;
}