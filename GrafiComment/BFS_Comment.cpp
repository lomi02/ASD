#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>

using namespace std;

// Definizione delle costanti per lo stato di visita dei nodi.
constexpr int WHITE = 0;       // Nodo non ancora scoperto.
constexpr int GRAY = 1;        // Nodo scoperto, i cui vicini sono in fase di esplorazione.
constexpr int BLACK = 2;       // Nodo e vicini completamente esplorati.
constexpr int INFINITY = 9999; // Valore per rappresentare la distanza infinita.

// Classe template che rappresenta un singolo Nodo (o Vertice) del grafo.
template<typename T>
class Node {
public:
    T val;             // Il valore o identificativo contenuto nel nodo.
    int color = WHITE; // Lo stato di esplorazione del nodo.
    T dist = INFINITY; // La distanza minima stimata dalla sorgente.
    Node *parent;      // Puntatore al nodo padre nell'albero della visita.

    // Costruttore: inizializza il valore e imposta il padre a null.
    explicit Node(T val) : val(val), parent(nullptr) {}
};

// Classe template che gestisce la struttura del Grafo e gli algoritmi di visita.
template<typename T>
class Graph {
public:
    vector<Node<T> *> nodes; // Lista di tutti i nodi presenti nel grafo.

    // Lista di adiacenza: associa il valore di un nodo ai suoi vicini (valore, peso dell'arco).
    map<T, vector<pair<T, T> > > adjacencyList;

    // Aggiunge un nodo alla lista dei nodi del grafo.
    void addNode(Node<T> *node) { nodes.push_back(node); }

    // Crea un arco non orientato tra sorgente e destinazione e lo aggiunge alla lista di adiacenza.
    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));
        adjacencyList[destination->val].push_back(make_pair(source->val, weight));
    }

    /**
     * BFS (Breadth-First Search) Iterativa:
     * Esplora il grafo a livelli partendo da startNode, calcolando le distanze minime.
     */
    void BFS(Node<T> *startNode) {

        // Passo 1: Inizializzazione di tutti i nodi.
        for (Node<T> *node: nodes) {
            node->color = WHITE;
            node->parent = nullptr;
            node->dist = INFINITY;
        }

        // Passo 2: Configurazione del nodo sorgente.
        startNode->color = GRAY;
        startNode->parent = nullptr;
        startNode->dist = 0;

        // Passo 3: Utilizzo della coda per gestire l'ordine di visita dei nodi.
        queue<Node<T> *> q;
        q.push(startNode);

        while (!q.empty()) {
            Node<T> *u = q.front();
            q.pop();

            // Passo 4: Esplorazione dei vicini del nodo corrente u.
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

                // Passo 5: Se il nodo è Bianco, viene scoperto e aggiunto alla coda.
                if (node->color == WHITE) {
                    node->color = GRAY;
                    node->parent = u;
                    node->dist = u->dist + v.second;
                    q.push(node);
                }
            }
            // Passo 6: Il nodo u è completamente esplorato.
            u->color = BLACK;
        }
    }

    /**
     * BFSRecursive (BFS Ricorsiva):
     * Versione ricorsiva della visita del grafo che mantiene la logica delle distanze.
     */
    void BFSRecursive(Node<T> *u) {

        // Passo 1: Il nodo corrente viene segnato come in fase di esplorazione.
        u->color = GRAY;

        // Passo 2: Esplorazione ricorsiva dei vicini.
        for (auto v: adjacencyList[u->val]) {
            Node<T> *node = nullptr;

            for (Node<T> *n: nodes) {
                if (n->val == v.first) {
                    node = n;
                    break;
                }
            }

            if (node == nullptr)
                continue;

            // Passo 3: Se il vicino è Bianco, si aggiornano le info e si procede con la ricorsione.
            if (node->color == WHITE) {
                node->parent = u;
                node->dist = u->dist + v.second;
                BFSRecursive(node);
            }
        }
        // Passo 4: Conclusione dell'esplorazione del nodo.
        u->color = BLACK;
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

    // Apertura del file di output per scrivere i risultati.
    ofstream output("outputG.txt");
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    // Esecuzione della BFS Iterativa partendo dal nodo con valore 1.
    output << "BFS" << endl;
    if (nodesMap.count(1)) {
        g.BFS(nodesMap[1]);
        for (Node<int> *node: g.nodes)
            output << "Distanza dal nodo 1 al nodo " << node->val << ": " << node->dist << endl;
    }

    // Esecuzione della BFS Ricorsiva (previa reinizializzazione).
    output << "\nBFS Ricorsivo" << endl;
    for (Node<int> *node: g.nodes) {
        node->color = WHITE;
        node->dist = INFINITY;
    }

    if (nodesMap.count(1)) {
        nodesMap[1]->dist = 0;
        g.BFSRecursive(nodesMap[1]);
        for (Node<int> *node: g.nodes)
            output << "Distanza dal nodo 1 al nodo " << node->val << ": " << node->dist << endl;
    }

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
