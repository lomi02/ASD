#include <iostream>         // Libreria per gestire l'input e l'output (scrivere sulla console).
#include <fstream>          // Libreria per leggere e scrivere file.
#include <vector>           // Contenitore che si comporta come una lista dinamica di elementi.
#include <map>              // Contenitore che associa una chiave a un valore (come una rubrica).
#include <unordered_map>    // Simile a map, ma più veloce per cercare elementi specifici.
#include <queue>            // Struttura dati "Coda": il primo che entra è il primo che esce (FIFO).

using namespace std; // Permette di usare i nomi della libreria standard senza scrivere "std::" ogni volta.

/**
 * Definizione di costanti per tracciare lo stato di ogni "stanza" (Nodo) del grafo.
 * Immagina un labirinto:
 * WHITE: Non siamo ancora passati di qui.
 * GRAY: Siamo entrati nella stanza ma non abbiamo ancora controllato tutte le porte che portano altrove.
 * BLACK: Abbiamo controllato questa stanza e tutte le stanze collegate direttamente.
 */
constexpr int WHITE = 0;
constexpr int GRAY = 1;
constexpr int BLACK = 2;
constexpr int INFINITY = 9999; // Un numero molto alto per indicare che un nodo è inizialmente irraggiungibile.

/**
 * Una "Classe Template" è uno stampo. "template<typename T>" significa che questo Nodo
 * può contenere qualsiasi tipo di dato (numeri interi, parole, ecc.), indicato qui come "T".
 */
template<typename T>
class Node {
public:
    T val; // Il contenuto del nodo (es. il numero della stanza).
    int color = WHITE; // Lo stato attuale (inizialmente "non visitato").
    T dist = INFINITY; // Quanti passi servono per arrivare qui dalla partenza.

    /**
     * Il simbolo "*" indica un "Puntatore". Invece di contenere un oggetto,
     * contiene l'indirizzo di memoria dove si trova quell'oggetto.
     * "parent" indica da quale nodo siamo arrivati per raggiungere questo.
     */
    Node *parent;

    // Costruttore: una funzione speciale che crea il nodo e imposta i valori iniziali.
    explicit Node(T val) : val(val), parent(nullptr) {}
};


// Classe che gestisce l'intero Grafo (l'insieme di nodi e i collegamenti tra loro).
template<typename T>
class Graph {
public:
    // Un elenco (vector) di indirizzi (puntatori) a tutti i nodi esistenti.
    vector<Node<T> *> nodes;

    /**
     * Lista di adiacenza: una mappa dove per ogni valore di un nodo (chiave)
     * salviamo una lista di coppie. Ogni coppia contiene (vicino, peso del collegamento).
     */
    map<T, vector<pair<T, T> > > adjacencyList;

    // Funzione per aggiungere un nodo creato in precedenza alla nostra lista generale.
    void addNode(Node<T> *node) { nodes.push_back(node); }

    // Funzione per creare un collegamento (Arco) tra due nodi.
    // Essendo non orientato, il collegamento vale in entrambe le direzioni.
    void addEdge(Node<T> *source, Node<T> *destination, T weight) {

        // Aggiunge la destinazione ai vicini della sorgente.
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));

        // Aggiunge la sorgente ai vicini della destinazione.
        adjacencyList[destination->val].push_back(make_pair(source->val, weight));
    }

    /**
     * BFS (Breadth-First Search) Iterativa:
     * Questo algoritmo esplora il grafo "a cerchi concentrici". Prima visita tutti i vicini
     * diretti della partenza, poi i vicini dei vicini, e così via.
     */
    void BFS(Node<T> *startNode) {

        // Passo 1: Prepariamo tutti i nodi del grafo resettandoli.
        for (Node<T> *node: nodes) {
            node->color = WHITE; // Nessuno è stato visitato.
            node->parent = nullptr; // Nessuno ha ancora un "padre".
            node->dist = INFINITY; // Distanza sconosciuta.
        }

        // Passo 2: Prepariamo il nodo da cui vogliamo far partire la ricerca.
        startNode->color = GRAY; // Lo stiamo visitando ora.
        startNode->parent = nullptr;
        startNode->dist = 0; // La distanza da se stessi è zero.

        // Passo 3: Creiamo una coda di "cose da fare". Mettiamo dentro il nodo di partenza.
        queue<Node<T> *> q;
        q.push(startNode);

        // Finché ci sono nodi nella coda, continuiamo a esplorare.
        while (!q.empty()) {
            // Prendiamo il primo nodo della coda (quello che aspetta da più tempo).
            Node<T> *u = q.front();
            q.pop(); // Lo togliamo dalla coda.

            // Passo 4: Guardiamo tutti i vicini del nodo "u" che abbiamo appena preso.
            for (auto v: adjacencyList[u->val]) {
                Node<T> *node = nullptr;

                // Cerchiamo nella nostra lista generale l'indirizzo del nodo vicino basandoci sul valore (v.first).
                for (Node<T> *n: nodes)
                    if (n->val == v.first) {
                        node = n;
                        break;
                    }

                // Se per qualche motivo il nodo non esiste, lo saltiamo.
                if (node == nullptr)
                    continue;

                // Passo 5: Se il vicino non è mai stato visto (è WHITE).
                if (node->color == WHITE) {
                    node->color = GRAY; // Lo segniamo come "scoperto".
                    node->parent = u; // Diciamo che siamo arrivati qui da "u".

                    // La distanza è quella di "u" più il peso dell'arco (v.second).
                    node->dist = u->dist + v.second;

                    // Lo mettiamo in coda per esplorare i suoi vicini più tardi.
                    q.push(node);
                }
            }
            // Passo 6: Abbiamo finito di guardare tutti i vicini di "u". Diventa BLACK.
            u->color = BLACK;
        }
    }

    /**
     * BFSRecursive (BFS Ricorsiva):
     * Una funzione "ricorsiva" è una funzione che chiama se stessa.
     * Nota: Tecnicamente questa implementazione si comporta più come una DFS (Depth-First)
     * ma mantiene la logica di aggiornamento distanze della BFS.
     */
    void BFSRecursive(Node<T> *u) {

        // Passo 1: Segniamo il nodo corrente come in fase di visita.
        u->color = GRAY;

        // Passo 2: Controlliamo i vicini.
        for (auto v: adjacencyList[u->val]) {
            Node<T> *node = nullptr;

            for (Node<T> *n: nodes)
                if (n->val == v.first) {
                    node = n;
                    break;
                }

            if (node == nullptr)
                continue;

            // Passo 3: Se troviamo un vicino mai visto, ci "tuffiamo" subito dentro
            // chiamando di nuovo questa stessa funzione (ricorsione).
            if (node->color == WHITE) {
                node->parent = u;
                node->dist = u->dist + v.second;
                BFSRecursive(node);
            }
        }
        // Passo 4: Finito il tuffo ricorsivo, il nodo è esplorato.
        u->color = BLACK;
    }
};

int main() {

    // Tentativo di aprire il file di testo che contiene la descrizione del grafo.
    ifstream input("inputG.txt");
    if (!input.is_open()) {
        cerr << "Errore: non trovo il file inputG.txt" << endl;
        return 1;
    }

    int numNodes, numEdges;
    // Leggiamo i primi due numeri del file: quanti nodi e quanti archi ci sono.
    input >> numNodes >> numEdges;

    // Questa mappa ci aiuta a non creare due volte lo stesso nodo mentre leggiamo il file.
    unordered_map<int, Node<int> *> nodesMap;
    Graph<int> g;

    // Leggiamo ogni riga del file che rappresenta un collegamento (arco).
    for (int i = 0; i < numEdges; i++) {
        int sourceVal, destVal, weight;
        input >> sourceVal >> destVal >> weight;

        // Controlliamo se i due nodi collegati dall'arco esistono già. Se no, li creiamo.
        for (int val: {sourceVal, destVal})
            if (nodesMap.find(val) == nodesMap.end()) {

                // "new" riserva un pezzetto di memoria permanente per il nodo.
                auto newNode = new Node<int>(val);
                nodesMap[val] = newNode;
                g.addNode(newNode);
            }

        // Creiamo il collegamento tra i due nodi usando gli indirizzi salvati nella mappa.
        g.addEdge(nodesMap[sourceVal], nodesMap[destVal], weight);
    }
    input.close(); // Chiudiamo il file di lettura.

    // Creiamo un nuovo file per scrivere i risultati.
    ofstream output("outputG.txt");
    if (!output.is_open()) {
        cout << "Errore: non posso creare il file di output." << endl;
        return 1;
    }

    // Eseguiamo l'algoritmo iterativo partendo dal nodo che ha valore "1".
    output << "Risultati BFS (Iterativa):" << endl;
    if (nodesMap.count(1)) {
        g.BFS(nodesMap[1]);
        for (Node<int> *node: g.nodes)
            output << "Distanza dal nodo 1 al nodo " << node->val << ": " << node->dist << endl;
    }

    // Reset dei nodi per testare la versione ricorsiva (altrimenti risulterebbero già visitati).
    output << "\nRisultati BFS (Ricorsiva):" << endl;
    for (Node<int> *node: g.nodes) {
        node->color = WHITE;
        node->dist = INFINITY;
    }

    if (nodesMap.count(1)) {
        nodesMap[1]->dist = 0; // La partenza deve avere sempre distanza 0.
        g.BFSRecursive(nodesMap[1]);
        for (Node<int> *node: g.nodes)
            output << "Distanza dal nodo 1 al nodo " << node->val << ": " << node->dist << endl;
    }

    output.close(); // Chiudiamo il file di scrittura.
    cout << "Operazione completata. Controlla il file outputG.txt" << endl;

    return 0; // Fine del programma.
}
