#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
using namespace std;

// Struttura per rappresentare un arco pesato
struct Edge {
    int src, dest;
    double weight;

    Edge(int s, int d, double w) : src(s), dest(d), weight(w) {
    }

    // Per ordinare gli archi in base al peso
    bool operator<(const Edge &other) const {
        return weight < other.weight;
    }
};

// Classe per rappresentare un grafo non orientato e pesato
class Graph {
    int V; // Numero di vertici
    vector<Edge> edges; // Lista di archi

public:
    Graph(int vertices) : V(vertices) {
    }

    // Aggiungi un arco al grafo
    void addEdge(int src, int dest, double weight) {
        edges.emplace_back(src, dest, weight);
        edges.emplace_back(dest, src, weight); // Grafo non orientato
    }

    // Restituisce il numero di vertici
    int getVertices() const { return V; }

    // Restituisce la lista di archi
    const vector<Edge> &getEdges() const { return edges; }

    // Trova tutti i cicli nel grafo usando Union-Find
    vector<vector<int> > findCycles() {
        vector<vector<int> > cycles;
        DisjointSet ds(V);

        for (const Edge &edge: edges) {
            int u = edge.src;
            int v = edge.dest;

            if (ds.findSet(u) == ds.findSet(v)) {
                // Trovato un ciclo
                vector<int> cycle = findPath(u, v);
                cycles.push_back(cycle);
            } else {
                ds.unionSets(u, v);
            }
        }

        return cycles;
    }

    // Trova l'arco leggero in un taglio (cut) specifico
    Edge findLightEdge(const vector<bool> &inSet) const {
        Edge lightEdge(-1, -1, numeric_limits<double>::max());

        for (const Edge &edge: edges) {
            bool srcInSet = inSet[edge.src];
            bool destInSet = inSet[edge.dest];

            // Se l'arco attraversa il taglio
            if (srcInSet != destInSet) {
                if (edge.weight < lightEdge.weight) {
                    lightEdge = edge;
                }
            }
        }

        return lightEdge;
    }

    // Struttura per insiemi disgiunti (usata per Kruskal e per trovare cicli)
    class DisjointSet {
        vector<int> parent, rank;

    public:
        DisjointSet(int n) {
            parent.resize(n);
            rank.resize(n, 0);
            for (int i = 0; i < n; ++i)
                parent[i] = i;
        }

        int findSet(int u) {
            if (u != parent[u])
                parent[u] = findSet(parent[u]);
            return parent[u];
        }

        void unionSets(int u, int v) {
            u = findSet(u);
            v = findSet(v);

            if (u == v) return;

            if (rank[u] > rank[v]) {
                parent[v] = u;
            } else {
                parent[u] = v;
                if (rank[u] == rank[v])
                    rank[v]++;
            }
        }
    };

    // Trova il percorso tra u e v (usato per identificare cicli)
    vector<int> findPath(int u, int v) {
        // Implementazione semplificata (in una reale implementazione servirebbe BFS/DFS)
        return {u, v}; // Per semplicità restituiamo solo i due nodi
    }
};

// Algoritmo di Kruskal per trovare l'MST
vector<Edge> kruskalMST(const Graph &graph) {
    vector<Edge> mst;
    int V = graph.getVertices();
    vector<Edge> edges = graph.getEdges();

    // Ordina gli archi in ordine crescente di peso
    sort(edges.begin(), edges.end());

    Graph::DisjointSet ds(V);

    for (const Edge &edge: edges) {
        int u = edge.src;
        int v = edge.dest;

        if (ds.findSet(u) != ds.findSet(v)) {
            mst.push_back(edge);
            ds.unionSets(u, v);

            // Se abbiamo incluso abbastanza archi, usciamo
            if (mst.size() == V - 1)
                break;
        }
    }

    return mst;
}

// Algoritmo di Prim per trovare l'MST
vector<Edge> primMST(const Graph &graph) {
    int V = graph.getVertices();
    const vector<Edge> &edges = graph.getEdges();
    vector<Edge> mst;

    // Strutture per l'algoritmo
    vector key(V, numeric_limits<double>::max());
    vector parent(V, -1);
    vector inMST(V, false);

    // Usiamo una coda di priorità per selezionare il prossimo nodo
    priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;

    // Partiamo dal nodo 0
    key[0] = 0.0;
    pq.push({0.0, 0});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        inMST[u] = true;

        // Trova tutti gli archi adiacenti a u
        for (const Edge &edge: edges) {
            if (edge.src == u || edge.dest == u) {
                int v = (edge.src == u) ? edge.dest : edge.src;

                if (!inMST[v] && edge.weight < key[v]) {
                    key[v] = edge.weight;
                    parent[v] = u;
                    pq.push({key[v], v});
                }
            }
        }
    }

    // Costruiamo l'MST dai parent
    for (int i = 1; i < V; ++i) {
        if (parent[i] != -1) {
            mst.emplace_back(parent[i], i, key[i]);
        }
    }

    return mst;
}

// Funzione per stampare l'MST
void printMST(const vector<Edge> &mst) {
    double totalWeight = 0.0;
    cout << "Edges in MST:\n";
    for (const Edge &edge: mst) {
        cout << edge.src << " - " << edge.dest << " : " << edge.weight << "\n";
        totalWeight += edge.weight;
    }
    cout << "Total weight of MST: " << totalWeight << "\n";
}

int main() {

    // Creazione di un grafo di esempio
    Graph graph(5);
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 3, 6);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 8);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 4, 7);
    graph.addEdge(3, 4, 9);

    // Test algoritmo di Kruskal
    cout << "Kruskal's Algorithm:\n";
    vector<Edge> kruskalResult = kruskalMST(graph);
    printMST(kruskalResult);

    // Test algoritmo di Prim
    cout << "\nPrim's Algorithm:\n";
    vector<Edge> primResult = primMST(graph);
    printMST(primResult);

    // Trova cicli nel grafo
    cout << "\nFinding cycles in the graph:\n";
    vector<vector<int> > cycles = graph.findCycles();
    for (const auto &cycle: cycles) {
        cout << "Cycle: ";
        for (int node: cycle) {
            cout << node << " ";
        }
        cout << "\n";
    }

    // Trova l'arco leggero in un taglio specifico
    cout << "\nFinding light edge in a cut:\n";
    vector inSet = {true, true, false, false, false}; // Nodi 0 e 1 in un insieme
    Edge lightEdge = graph.findLightEdge(inSet);
    cout << "Light edge: " << lightEdge.src << " - " << lightEdge.dest << " with weight " << lightEdge.weight << "\n";

    return 0;
}
