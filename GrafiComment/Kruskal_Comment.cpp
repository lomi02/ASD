#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>

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

    // Crea un nuovo insieme dove il nodo 'x' è il capo di se stesso
    void MakeSet(Node<T> *x) {
        x->parent = x;    // All'inizio, il padre di x è x
        x->rank = 0;      // Il rank (altezza dell'albero) è zero
    }

    // Trova il "rappresentante" (il capo) dell'insieme a cui appartiene x
    Node<T> *FindSet(Node<T> *x) {
        if (x != x->parent)
            // Path Compression: mentre risale l'albero per cercare il capo, collega x direttamente al capo per velocizzare le ricerche future.
                x->parent = FindSet(x->parent);
        return x->parent;
    }

    // Unisce gli insiemi di x e y
    void UnionSet(Node<T> *x, Node<T> *y) {
        LinkSet(FindSet(x), FindSet(y)); // Trova i capi e li collega
    }

    // Collega fisicamente due alberi (insiemi)
    void LinkSet(Node<T> *x, Node<T> *y) {
        // Union by Rank: attacca l'albero più basso a quello più alto per evitare che l'albero diventi troppo lungo e lento da percorrere.
        if (x->rank > y->rank)
            y->parent = x;
        else
            x->parent = y;

        // Se hanno la stessa altezza, uno diventa padre e il suo rank aumenta
        if (x->rank == y->rank)
            ++y->rank;
    }

    /**
     * KRUSKAL
     * Trova l'MST selezionando gli archi in ordine di peso.
     */
    vector<Edge<T> *> Kruskal() {

        // 'A' sarà il contenitore degli archi che faranno parte del nostro MST finale.
        vector<Edge<T> *> A;

        // 1. INIZIALIZZAZIONE (DSU)
        // Creiamo un insieme separato per ogni nodo.
        // All'inizio ogni nodo è una "componente connessa" a sé stante.
        for (auto node: nodes)
            MakeSet(node);

        // 2. ORDINAMENTO DEGLI ARCHI (Sort)
        // Questa è la fase cruciale: ordiniamo TUTTI gli archi del grafo dal più leggero al più pesante.
        // Utilizziamo una funzione lambda per confrontare i pesi.
        sort(edges.begin(), edges.end(), [](Edge<T> *a, Edge<T> *b) {
            return a->weight < b->weight;
        });

        // 3. CICLO DI SELEZIONE GREEDY
        // Scorriamo gli archi ordinati uno per uno.
        for (auto edge: edges) {
            // Controlliamo se la sorgente e la destinazione dell'arco appartengono già allo stesso insieme (cioè se sono già collegati).
            if (FindSet(edge->source) != FindSet(edge->destination)) {

                // Se NON sono collegati, aggiungere questo arco non creerà un ciclo.
                // Quindi lo aggiungiamo al nostro MST (A).
                A.push_back(edge);

                // Fondiamo i due insiemi: ora source e destination fanno parte della stessa struttura.
                UnionSet(edge->source, edge->destination);
            }
            // Se FindSet fosse stato uguale, avremmo scartato l'arco perché avrebbe creato un ciclo (chiudendo un percorso già esistente).
        }

        return A; // Restituiamo la lista degli archi che formano l'MST.
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

    output << "Kruskal" << endl;

    // Chiamiamo la funzione Kruskal che restituisce un vector di puntatori ad archi.
    // Questi archi rappresentano i collegamenti scelti per formare l'MST.
    vector<Edge<int> *> mst_K = g.Kruskal();

    // Ciclo "range-based": scorre ogni puntatore a Edge contenuto nel vettore mst_K
    for (auto edge: mst_K) {
        // Accediamo all'oggetto Edge tramite il puntatore:
        // edge->source->val: prende l'ID del nodo di partenza dell'arco
        // edge->destination->val: prende l'ID del nodo di arrivo
        // edge->weight: prende il costo di quel collegamento specifico
        output << edge->source->val << " - " << edge->destination->val << " " << edge->weight << endl;
    }

    output.close(); // Chiudiamo il file
    cout << "Operazione completata con successo!" << endl;

    return 0;
}