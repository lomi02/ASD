#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <stack>

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

    // Componenti Connesse
    void ConnectedComponents(ofstream &output) {
        map<Node<T> *, vector<Node<T> *> > connectedComponentMap;

        // Ogni nodo parte come un insieme isolato
        for (auto v: nodes)
            MakeSet(v);

        // Per ogni arco, se i due nodi non sono nello stesso insieme, uniscili
        for (auto edge: edges)
            if (FindSet(edge->source) != FindSet(edge->destination))
                UnionSet(edge->source, edge->destination);

        // Raggruppa i nodi in base al loro rappresentante finale
        for (auto v: nodes) {
            Node<T> *representative = FindSet(v);
            connectedComponentMap[representative].push_back(v);
        }

        // Stampa ogni gruppo su una riga diversa
        for (const auto &pair: connectedComponentMap) {
            for (const auto &node: pair.second)
                output << node->val << " ";
            output << endl;
        }
    }

    // Mostra Lista di adiacenza
    void ShowAdjacentList(Node<T> *u, ofstream &output) {

        // Scrive l'intestazione: "Lista di adiacenza del nodo X:"
        output << "Lista di adiacenza del nodo " << u->val << ":\n";

        // Inizia un ciclo iterator che percorre tutto il vettore di vicini associato a u->val
        // adjacencyList[u->val] restituisce il vector<pair<T, T>> dei vicini
        for (auto it = adjacencyList[u->val].begin(); it != adjacencyList[u->val].end(); ++it) {

            // 'it' è un puntatore alla coppia (pair) corrente.
            // it->first  è il valore del nodo di destinazione.
            // it->second è il peso dell'arco che li unisce.
            output << " " << u->val << " -> " << it->first << " (peso " << it->second << ")\n";
        }
    }

    // Primo passo SCC: DFS che riempie uno stack in base al tempo di fine (finish_time)
    void DFSUtil(int v, bool visited[], stack<int> &stack) {
        visited[v] = true; // Segna come visitato
        for (auto i = adjacencyList[v].begin(); i != adjacencyList[v].end(); ++i)
            if (!visited[i->first])
                DFSUtil(i->first, visited, stack);

        // Quando non ci sono più vicini, aggiungi allo stack
        // I nodi che finiscono più tardi staranno in cima
        stack.push(v);
    }

    // Secondo passo SCC: DFS sul grafo trasposto per stampare i nodi
    void DFSUtil2(int v, bool visited[], ofstream &output) {
        visited[v] = true;
        output << v << " "; // Stampa il nodo corrente
        for (auto i = adjacencyList[v].begin(); i != adjacencyList[v].end(); ++i)
            if (!visited[i->first])
                DFSUtil2(i->first, visited, output);
    }

    // Terzo passo SCC: Crea un nuovo grafo con gli stessi nodi e archi
    Graph Transpose() {
        Graph transposedGraph;
        for (auto &node: nodes) {
            transposedGraph.addNode(new Node<T>(node->val));
        }
        for (const auto &edge: edges) {
            // Se volessi il vero trasposto per Kosaraju, dovresti invertire source e destination
            transposedGraph.addEdge(edge->source, edge->destination, edge->weight);
        }
        return transposedGraph;
    }

    // SCC
    void PrintSCCs(ofstream &output) {
        stack<int> stack;
        bool *visited = new bool[nodes.size()];
        for (int i = 0; i < nodes.size(); i++) visited[i] = false;

        // 1. Riempie lo stack con l'ordine di completamento
        for (int i = 0; i < nodes.size(); i++)
            if (!visited[i])
                DFSUtil(i, visited, stack);

        // 2. Crea il grafo trasposto (invertito)
        Graph g = Transpose();
        for (int i = 0; i < nodes.size(); i++) visited[i] = false;

        // 3. Elabora i nodi nell'ordine dello stack
        while (!stack.empty()) {
            int v = stack.top(); // Prendi il nodo con finish_time più alto
            stack.pop();
            if (!visited[v]) {
                // Ogni DFS qui identifica una Componente Fortemente Connessa
                g.DFSUtil2(v, visited, output);
                output << endl;
            }
        }
        delete[] visited; // Libera la memoria dell'array
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

    // Questo blocco serve a verificare visivamente come il grafo è memorizzato internamente.
    output << "Lista di Adiacenza:" << endl;
    for (Node<int> *node: g.nodes) {

        // Chiama la funzione che cicla sui vicini di ogni singolo nodo
        g.ShowAdjacentList(node, output);
    }
    output << endl;

    // Calcolo Componenti Connesse
    // Qui il programma usa la logica Disjoint Set (MakeSet/Union/Find).
    output << "Componenti connesse: " << endl;
    // Questa funzione scriverà nel file gruppi di nodi separati da una riga.
    // Esempio: se il nodo 1 è collegato al 2, e il 3 è isolato, vedrai:
    // 1 2
    // 3
    g.ConnectedComponents(output);
    output << endl;

    // Calcolo Componenti Fortemente Connesse
    // Qui viene eseguito l'algoritmo di Kosaraju (Doppia DFS).
    output << "Componenti fortemente connesse:" << endl;
    // PrintSCCs gestisce internamente la creazione dello stack, la trasposizione
    // del grafo e la seconda mandata di visite.
    g.PrintSCCs(output);
    output << endl;

    // Chiudiamo il canale di comunicazione con il file di output per salvare i dati su disco
    output.close();
    cout << "Operazione completata con successo!" << endl;

    return 0;
}