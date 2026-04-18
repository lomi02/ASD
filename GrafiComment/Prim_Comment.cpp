#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>

using namespace std;

// Definizione di una costante per rappresentare l'infinito.
// Viene usata per inizializzare i pesi minimi (chiavi) dei nodi non ancora inclusi nell'MST.
constexpr int INFINITY = 9999;

// Classe template che rappresenta un singolo Nodo (o Vertice) del grafo.
template<typename T>
class Node {
public:
    T val; // Il valore o identificativo contenuto nel nodo.

    // Costruttore: inizializza il valore del nodo.
    explicit Node(T val) : val(val) {}
};

// Classe template che gestisce la struttura del Grafo e l'algoritmo di Prim.
template<typename T>
class Graph {
public:
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
     * Prim:
     * Costruisce l'MST partendo da un nodo arbitrario, aggiungendo iterativamente l'arco
     * di peso minimo che collega un nodo interno all'MST con uno esterno.
     */
    int Prim(ofstream &output) {
        if (nodes.empty()) return 0;

        // Passo 1: Inizializzazione delle strutture dati di supporto.
        int src = 0;                              // Si assume il primo nodo come sorgente.
        vector<int> key(nodes.size(), INFINITY);  // Peso minimo per collegare il nodo all'MST.
        vector<bool> inMST(nodes.size(), false);  // Specifica se il nodo è già parte dell'MST.
        vector<int> parent(nodes.size(), -1);     // Memorizza la struttura dell'albero (padre di ogni nodo).

        // Passo 2: Utilizzo di una coda di priorità (Min-Priority Queue).
        // Memorizza coppie (peso, indice_nodo) per estrarre sempre l'arco più leggero.
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

        // Inserimento del nodo sorgente nella coda.
        pq.emplace(0, src);
        key[src] = 0;

        while (!pq.empty()) {
            // Passo 3: Estrazione del nodo u con la chiave minima.
            int u = pq.top().second;
            pq.pop();

            // Se il nodo è già nell'MST, viene saltato (gestione dei duplicati nella pq).
            if (inMST[u]) continue;
            inMST[u] = true;

            // Passo 4: Esplorazione dei vicini del nodo u.
            for (auto &neighbor: adjacencyList[nodes[u]->val]) {
                int v_val = neighbor.first;
                int weight = neighbor.second;

                // Ricerca dell'indice del nodo vicino basandosi sul suo valore.
                int v_index = -1;
                for (int i = 0; i < nodes.size(); ++i) {
                    if (nodes[i]->val == v_val) {
                        v_index = i;
                        break;
                    }
                }

                // Passo 5: Rilassamento. Se il vicino non è nell'MST e il peso dell'arco è minore della chiave attuale, si aggiornano il peso e il padre.
                if (v_index != -1 && !inMST[v_index] && weight < key[v_index]) {
                    key[v_index] = weight;
                    pq.emplace(key[v_index], v_index);
                    parent[v_index] = u;
                }
            }
        }

        // Passo 6: Calcolo del costo totale e stampa degli archi dell'MST su file.
        int costoTotale = 0;
        for (int i = 0; i < nodes.size(); ++i) {
            if (parent[i] != -1) {
                output << nodes[parent[i]]->val << " - " << nodes[i]->val << " " << key[i] << endl;
                costoTotale += key[i];
            }
        }
        return costoTotale;
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
    // Legge il numero totale di nodi e archi.
    input >> numNodes >> numEdges;

    // Mappa per associare un valore intero al puntatore dell'oggetto Node corrispondente.
    unordered_map<int, Node<int> *> nodesMap;
    Graph<int> g;

    // Ciclo per leggere ogni arco dal file e popolare il grafo.
    for (int i = 0; i < numEdges; i++) {
        int sourceVal, destVal, weight;
        input >> sourceVal >> destVal >> weight;

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

    // Apertura del file di output per scrivere i risultati dell'MST.
    ofstream output("outputG.txt");
    if (!output.is_open()) {
        cerr << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    output << "Prim" << endl;

    // Esecuzione dell'algoritmo di Prim e recupero del costo totale.
    int costoTotale = g.Prim(output);
    output << "Costo totale dell'MST: " << costoTotale << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}