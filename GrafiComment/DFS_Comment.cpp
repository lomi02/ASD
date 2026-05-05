#include <iostream>         // Libreria per comunicare con l'utente (mostrare messaggi a schermo).
#include <fstream>          // Libreria per leggere e scrivere su file di testo esterni.
#include <vector>           // Contenitore simile a un elenco dinamico di elementi.
#include <map>              // Una "mappa" o dizionario: associa un nome (chiave) a un valore.
#include <unordered_map>    // Simile a map, ma ottimizzata per ricerche molto veloci.

using namespace std; // Evita di dover scrivere "std::" prima di ogni comando della libreria standard.

/**
 * Questi tre stati servono a capire cosa stiamo facendo in ogni "stanza" (Nodo) del grafo:
 * WHITE: Stanza mai vista, la porta è ancora chiusa.
 * GRAY: Siamo entrati nella stanza e stiamo esplorando i corridoi che partono da qui.
 * BLACK: Abbiamo esplorato ogni corridoio possibile partendo da questa stanza e siamo usciti.
 */
constexpr int WHITE = 0;
constexpr int GRAY = 1;
constexpr int BLACK = 2;

/**
 * La parola "template" permette alla classe di gestire qualsiasi tipo di dato (numeri, lettere).
 * "T" rappresenta il tipo di dato che sceglieremo in seguito.
 */
template<typename T>
class Node {
public:
    T val;                  // Il valore contenuto nel nodo (es. un numero identificativo).
    int color = WHITE;      // Lo stato attuale del nodo (inizialmente "mai visto").
    T discovery_time = 0;   // Il "minuto" in cui entriamo per la prima volta nel nodo.
    T finish_time = 0;      // Il "minuto" in cui finiamo di esplorare tutto ciò che è collegato a lui.

    /**
     * Il simbolo "*" indica un "Puntatore". Invece di contenere il nodo, contiene
     * l'indirizzo di memoria per trovarlo. Qui serve a segnare da quale nodo siamo arrivati.
     */
    Node *parent;

    // Funzione speciale (costruttore) che inizializza il nodo quando lo creiamo.
    explicit Node(T val) : val(val), parent(nullptr) {}
};

// Classe che gestisce il Grafo (l'intera rete di nodi e collegamenti).
template<typename T>
class Graph {
public:
    T time = 0;                 // Un cronometro globale che avanza a ogni passo della visita.
    vector<Node<T> *> nodes;    // Un elenco che contiene gli indirizzi di tutti i nodi della rete.

    /**
     * Lista di adiacenza: per ogni nodo (chiave), salviamo un elenco dei nodi
     * a cui è direttamente collegato (vicini).
     */
    map<T, vector<pair<T, T> > > adjacencyList;

    // Aggiunge un nodo creato all'elenco generale del grafo.
    void addNode(Node<T> *node) { nodes.push_back(node); }

    // Crea un collegamento (arco) tra due nodi e specifica quanto "pesa" il legame.
    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));
        adjacencyList[destination->val].push_back(make_pair(source->val, weight));
    }

    /**
     * DFS (Depth-First Search):
     * Questa funzione coordina la visita "in profondità". Immagina di entrare in un
     * labirinto e andare sempre avanti finché non trovi un vicolo cieco, per poi tornare indietro.
     */
    void DFS() {

        // Passo 1: Prepariamo tutti i nodi resettando il colore e il padre.
        for (auto u: nodes) {
            u->color = WHITE;
            u->parent = nullptr;
        }

        // Passo 2: Azzeriamo il cronometro globale.
        time = 0;

        /**
         * Passo 3: Controlliamo ogni nodo del grafo. Se è ancora WHITE, facciamo
         * partire un'esplorazione da lì. Questo serve se il grafo è diviso in
         * "isole" non collegate tra loro.
         */
        for (auto u: nodes) {
            if (u->color == WHITE)
                DFS_Visit(u); // Avvia l'esplorazione profonda.
        }
    }

    /**
     * DFS_Visit:
     * La parte "ricorsiva" dell'algoritmo. Una funzione ricorsiva è una funzione
     * che chiama se stessa per andare sempre più a fondo.
     */
    void DFS_Visit(Node<T> *u) {

        // Passo 1: Siamo appena entrati nel nodo "u".
        u->color = GRAY;                // Lo segniamo come "in fase di visita".
        u->discovery_time = ++time;     // Segniamo il tempo di entrata e facciamo avanzare il cronometro.

        // Passo 2: Controlliamo tutti i vicini del nodo corrente.
        for (auto v: adjacencyList[u->val]) {
            Node<T> *node = nullptr;

            // Cerchiamo l'indirizzo del nodo vicino nell'elenco generale.
            for (Node<T> *n: nodes) {
                if (n->val == v.first) {
                    node = n;
                    break;
                }
            }

            if (node == nullptr)
                continue;

            // Passo 3: Se il vicino è bianco (mai visitato), ci entriamo subito.
            if (node->color == WHITE) {
                node->parent = u;   // Diciamo al vicino che siamo noi il suo "punto di origine".
                DFS_Visit(node);    // Chiamata ricorsiva: ci tuffiamo nel vicino.
            }
        }

        /*
         * Passo 4: Una volta che abbiamo esplorato tutti i possibili percorsi
         * partendo dai vicini di "u", possiamo considerare "u" completato.
         */
        u->color = BLACK;           // Nodo chiuso.
        u->finish_time = ++time;    // Segniamo il tempo di uscita.
    }
};

int main() {

    // Proviamo ad aprire il file che contiene i dati dei collegamenti.
    ifstream input("inputG.txt");
    if (!input.is_open()) {
        cerr << "Errore: non trovo il file inputG.txt!" << endl;
        return 1;
    }

    int numNodes, numEdges;

    // Leggiamo quanti nodi e quanti archi (collegamenti) ci sono nel file.
    input >> numNodes >> numEdges;

    // Questa mappa serve a non creare duplicati dello stesso nodo durante la lettura.
    unordered_map<int, Node<int> *> nodesMap;
    Graph<int> g;

    // Ciclo per leggere ogni riga che descrive un collegamento.
    for (int i = 0; i < numEdges; i++) {
        int sourceVal, destVal, weight;
        input >> sourceVal >> destVal >> weight;

        // Se un nodo menzionato nel file non esiste ancora, lo creiamo (usando 'new').
        for (int val: {sourceVal, destVal})
            if (nodesMap.find(val) == nodesMap.end()) {
                auto newNode = new Node<int>(val);
                nodesMap[val] = newNode;
                g.addNode(newNode);
            }

        // Aggiungiamo il collegamento effettivo tra i due nodi nella struttura del grafo.
        g.addEdge(nodesMap[sourceVal], nodesMap[destVal], weight);
    }
    input.close(); // Abbiamo finito di leggere, chiudiamo il file.

    // Prepariamo un nuovo file per scrivere i risultati ottenuti.
    ofstream output("outputG.txt");
    if (!output.is_open()) {
        cout << "Errore: impossibile creare il file di output." << endl;
        return 1;
    }

    // Lanciamo l'algoritmo DFS definito sopra.
    output << "Risultati visita DFS (Profondità):" << endl;
    g.DFS();

    // Per ogni nodo, scriviamo nel file quando siamo entrati e quando siamo usciti.
    for (Node<int> *node: g.nodes) {
        output << "Nodo " << node->val << ": scoperto al minuto " << node->discovery_time << ", completato al minuto " << node->finish_time << endl;
    }

    output.close(); // Salviamo e chiudiamo il file.
    cout << "Analisi completata! I risultati sono in outputG.txt" << endl;

    return 0; // Il programma termina correttamente.
}
