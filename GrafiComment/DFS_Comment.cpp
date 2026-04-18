#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

// Definizione delle costanti per lo stato di visita dei nodi.
constexpr int WHITE = 0;    // Nodo non ancora scoperto.
constexpr int GRAY = 1;     // Nodo scoperto, la cui visita è in corso (nello stack di ricorsione).
constexpr int BLACK = 2;    // Nodo e tutta la sua discendenza completamente esplorati.

// Classe template che rappresenta un singolo Nodo (o Vertice) del grafo.
template<typename T>
class Node {
public:
    T val;                 // Il valore o identificativo contenuto nel nodo.
    int color = WHITE;     // Lo stato di esplorazione del nodo.
    T discovery_time = 0;  // Tempo in cui il nodo viene scoperto (diventa GRAY).
    T finish_time = 0;     // Tempo in cui la visita del nodo termina (diventa BLACK).
    Node *parent;          // Puntatore al nodo padre nella foresta DFS.

    // Costruttore: inizializza il valore e imposta il padre a null.
    explicit Node(T val) : val(val), parent(nullptr) {}
};

// Classe template che gestisce la struttura del Grafo e l'algoritmo DFS.
template<typename T>
class Graph {
public:
    T time = 0;              // Contatore globale del tempo per le marcature temporali.
    vector<Node<T> *> nodes; // Lista di tutti i nodi presenti nel grafo.

    // Lista di adiacenza: mappa il valore di un nodo ai suoi vicini (coppia valore-peso).
    map<T, vector<pair<T, T> > > adjacencyList;

    // Aggiunge un nodo alla lista dei nodi del grafo.
    void addNode(Node<T> *node) { nodes.push_back(node); }

    // Crea un arco non orientato tra sorgente e destinazione e lo aggiunge alla lista di adiacenza.
    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));
        adjacencyList[destination->val].push_back(make_pair(source->val, weight));
    }

    /**
     * DFS (Depth-First Search):
     * Coordina la visita in profondità di tutto il grafo, gestendo anche grafi non connessi.
     */
    void DFS() {

        // Passo 1: Inizializzazione di tutti i nodi del grafo.
        for (auto u: nodes) {
            u->color = WHITE;
            u->parent = nullptr;
        }

        // Passo 2: Reset del contatore temporale globale.
        time = 0;

        // Passo 3: Avvio della visita ricorsiva per ogni nodo ancora bianco.
        // Questo garantisce l'esplorazione di tutte le componenti connesse.
        for (auto u: nodes) {
            if (u->color == WHITE)
                DFS_Visit(u);
        }
    }

    /**
     * DFS_Visit:
     * Funzione ricorsiva che esplora i rami del grafo in profondità.
     */
    void DFS_Visit(Node<T> *u) {

        // Passo 1: Il nodo viene scoperto. Incremento del tempo e cambio colore.
        u->color = GRAY;
        u->discovery_time = ++time;

        // Passo 2: Esplorazione dei vicini del nodo corrente u.
        for (auto v: adjacencyList[u->val]) {
            Node<T> *node = nullptr;

            // Ricerca del puntatore all'oggetto Node basandosi sul valore del vicino.
            for (Node<T> *n: nodes) {
                if (n->val == v.first) {
                    node = n;
                    break;
                }
            }

            if (node == nullptr)
                continue;

            // Passo 3: Se il vicino è ancora Bianco, la visita procede ricorsivamente in profondità.
            if (node->color == WHITE) {
                node->parent = u;
                DFS_Visit(node);
            }
        }

        // Passo 4: Il nodo e i suoi discendenti sono stati visitati.
        // Si imposta il tempo di fine e il colore diventa Nero.
        u->color = BLACK;
        u->finish_time = ++time;
    }
};

int main() {

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

        // Aggiunge l'arco al grafo (trattato come non orientato).
        g.addEdge(nodesMap[sourceVal], nodesMap[destVal], weight);
    }
    input.close();

    // Apertura del file di output per scrivere i risultati della visita.
    ofstream output("output.txt");
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    // Esecuzione dell'algoritmo DFS.
    output << "DFS" << endl;
    g.DFS();

    // Stampa dei tempi di scoperta e di fine per ogni nodo.
    for (Node<int> *node: g.nodes) {
        output << "Nodo " << node->val << ": tempo di scoperta = " << node->discovery_time <<
                ", tempo di completamento = " << node->finish_time << endl;
    }

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
