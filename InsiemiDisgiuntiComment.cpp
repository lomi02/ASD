#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// Implementazione con alberi radicati (foresta di insiemi disgiunti)
class DisjointSet {
    struct Node {
        int parent; // puntatore al padre
        int rank;   // rango del nodo (per euristica unione per rango)
    };

    vector<Node> nodes;
    int setCount;   // contatore degli insiemi

public:
    // Costruttore - inizializza la struttura per n elementi
    DisjointSet(int n) {
        nodes.resize(n);
        for (int i = 0; i < n; ++i) {
            nodes[i].parent = i;    // ogni elemento è il proprio padre (Make-Set)
            nodes[i].rank = 0;      // rango iniziale 0
        }
        setCount = n;
    }

    // Make-Set: crea un nuovo insieme con un solo elemento (già fatto nel costruttore)
    // In questa implementazione, Make-Set è implicito nell'inizializzazione

    // Find-Set con compressione del cammino
    int findSet(int x) {
        if (nodes[x].parent != x) {
            nodes[x].parent = findSet(nodes[x].parent); // compressione ricorsiva
        }
        return nodes[x].parent;
    }

    // Union: unisce gli insiemi contenenti x e y (con euristica unione per rango)
    void unionSets(int x, int y) {
        int xRoot = findSet(x);
        int yRoot = findSet(y);

        if (xRoot == yRoot) return; // già nello stesso insieme

        // Euristica unione per rango
        if (nodes[xRoot].rank < nodes[yRoot].rank) {
            nodes[xRoot].parent = yRoot;
        } else if (nodes[xRoot].rank > nodes[yRoot].rank) {
            nodes[yRoot].parent = xRoot;
        } else {
            // Ranghi uguali, scelgo arbitrariamente e incremento il rango
            nodes[yRoot].parent = xRoot;
            nodes[xRoot].rank++;
        }

        setCount--;
    }

    // Restituisce il numero di insiemi disgiunti
    int getSetCount() const {
        return setCount;
    }

    // Verifica se due elementi sono nello stesso insieme
    bool isSameSet(int x, int y) {
        return findSet(x) == findSet(y);
    }
};

// Algoritmo per trovare le componenti connesse in un grafo usando DSU
vector<vector<int> > findConnectedComponents(int numNodes, const vector<pair<int, int> > &edges) {
    DisjointSet dsu(numNodes);

    // Unione dei nodi connessi
    for (const auto &edge: edges) {
        int u = edge.first;
        int v = edge.second;
        dsu.unionSets(u, v);
    }

    // Raggruppamento dei nodi per componente connessa
    unordered_map<int, vector<int> > componentsMap;
    for (int i = 0; i < numNodes; ++i) {
        int root = dsu.findSet(i);
        componentsMap[root].push_back(i);
    }

    // Conversione in vettore di vettori
    vector<vector<int> > components;
    for (const auto &pair: componentsMap) {
        components.push_back(pair.second);
    }

    return components;
}

// Funzione per dimostrare la crescita della funzione di Ackermann (solo per piccoli valori)
int ackermann(int k, int j) {
    if (k == 0) return j + 1;
    if (j == 1) return ackermann(k - 1, 1);
    return ackermann(k - 1, ackermann(k, j - 1));
}

// Funzione inversa di Ackermann
int inverseAckermann(int n) {
    if (n <= 2) return 0;
    if (n == 3) return 1;
    if (n <= 7) return 2;
    if (n <= 2047) return 3;
    // Per n > 2047, dovremmo calcolare A4(1) che è enorme
    return 4; // Approssimazione per scopi pratici
}

int main() {

    // Esempio di utilizzo degli insiemi disgiunti
    DisjointSet dsu(5);
    cout << "Numero di insiemi iniziali: " << dsu.getSetCount() << endl;

    dsu.unionSets(0, 1);
    dsu.unionSets(2, 3);
    cout << "Numero di insiemi dopo due union: " << dsu.getSetCount() << endl;

    cout << "0 e 1 nello stesso insieme? " << (dsu.isSameSet(0, 1) ? "Sì" : "No") << endl;
    cout << "1 e 2 nello stesso insieme? " << (dsu.isSameSet(1, 2) ? "Sì" : "No") << endl;

    dsu.unionSets(1, 2);
    cout << "Numero di insiemi dopo un'altra union: " << dsu.getSetCount() << endl;
    cout << "1 e 2 nello stesso insieme? " << (dsu.isSameSet(1, 2) ? "Sì" : "No") << endl;

    // Esempio di ricerca delle componenti connesse
    vector<pair<int, int> > edges = {{0, 1}, {1, 2}, {3, 4}};
    auto components = findConnectedComponents(5, edges);

    cout << "\nComponenti connesse nel grafo:" << endl;
    for (size_t i = 0; i < components.size(); ++i) {
        cout << "Componente " << i + 1 << ": ";
        for (int node: components[i]) {
            cout << node << " ";
        }
        cout << endl;
    }

    // Dimostrazione funzione di Ackermann (solo per piccoli valori)
    cout << "\nAlcuni valori della funzione di Ackermann:" << endl;
    cout << "A(0, 1) = " << ackermann(0, 1) << endl;
    cout << "A(1, 1) = " << ackermann(1, 1) << endl;
    cout << "A(2, 1) = " << ackermann(2, 1) << endl;
    cout << "A(3, 1) = " << ackermann(3, 1) << endl;
    // A(4,1) è troppo grande per essere calcolato ricorsivamente

    // Esempi di funzione inversa di Ackermann
    cout << "\nAlcuni valori della funzione inversa di Ackermann:" << endl;
    cout << "α(1) = " << inverseAckermann(1) << endl;
    cout << "α(3) = " << inverseAckermann(3) << endl;
    cout << "α(7) = " << inverseAckermann(7) << endl;
    cout << "α(2047) = " << inverseAckermann(2047) << endl;
    cout << "α(2048) = " << inverseAckermann(2048) << endl;

    return 0;
}