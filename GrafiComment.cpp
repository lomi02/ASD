#include <iostream>     // Per operazioni di input/output
#include <vector>       // Per utilizzare vettori dinamici
#include <queue>        // Per la BFS (utilizza una coda FIFO)
#include <algorithm>    // Per funzioni come find() e min()

using namespace std;

// Classe che rappresenta un grafo non orientato usando liste di adiacenza
class Graph {
    int V;  // Numero di vertici nel grafo
    vector<vector<int>> adj;  // Lista di adiacenza: per ogni nodo, un vettore di nodi adiacenti

public:
    // Costruttore: inizializza il numero di vertici e la lista di adiacenza
    Graph(int vertices) : V(vertices), adj(vertices) {}

    // Aggiunge un arco non orientato tra i vertici u e v
    void addEdge(int u, int v) {
        adj[u].push_back(v);  // Aggiunge v alla lista dei vicini di u
        adj[v].push_back(u);  // Aggiunge u alla lista dei vicini di v (grafo non orientato)
    }

    // Stampa la lista di adiacenza di ciascun nodo
    void printGraph() const {
        cout << "Lista di adiacenza:\n";
        for (int u = 0; u < V; u++) {
            cout << u << ": ";
            for (int v : adj[u])
                cout << v << " ";
            cout << "\n";
        }
    }

    // Esegue una visita in ampiezza (BFS) a partire dal nodo 'start'
    void BFS(int start) {
        vector visited(V, false);   // Stato visitato di ogni nodo
        vector distance(V, -1);     // Distanza del nodo da 'start'
        vector parent(V, -1);       // Nodo padre nel cammino minimo
        queue<int> q;

        // Inizializzazione del nodo di partenza
        visited[start] = true;
        distance[start] = 0;
        q.push(start);

        // Ciclo principale della BFS
        while (!q.empty()) {
            int u = q.front(); q.pop();  // Estrae il nodo corrente dalla coda

            // Esamina tutti i vicini di u
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;              // Marca come visitato
                    parent[v] = u;                  // Salva il padre
                    distance[v] = distance[u] + 1;  // Calcola distanza
                    q.push(v);                      // Aggiunge alla coda
                }
            }
        }

        // Stampa dei risultati
        cout << "\nRisultato BFS da nodo " << start << ":\n";
        for (int i = 0; i < V; i++)
            cout << "Nodo: " << i << ", Distanza: " << distance[i] << ", Padre: " << parent[i] << "\n";
    }

    // Esegue una visita in profondità (DFS) per l'intero grafo
    void DFS() {
        vector visited(V, false);      // Stato visitato
        vector<int> discovery(V);      // Tempo di scoperta
        vector<int> finish(V);         // Tempo di fine visita
        vector parent(V, -1);          // Padre nel DFS tree
        int time = 0;

        cout << "\nRisultato DFS:\n";

        // Avvia la DFS da ogni nodo non ancora visitato (per componenti connesse multiple)
        for (int u = 0; u < V; u++) {
            if (!visited[u])
                DFSVisit(u, visited, discovery, finish, parent, time);
        }

        // Stampa dei tempi e dei padri
        for (int i = 0; i < V; i++)
            cout << "Nodo: " << i << ", d: " << discovery[i] << ", f: " << finish[i] << ", Padre: " << parent[i] << "\n";
    }

    // Controlla se esiste un arco diretto tra u e v
    bool isAdjacent(int u, int v) const {
        return ranges::find(adj[u], v) != adj[u].end();
    }

    // Verifica se esiste un cammino tra i nodi u e v usando DFS
    bool hasPath(int u, int v) {
        vector visited(V, false);
        return DFSPath(u, v, visited);
    }

    // Trova e stampa tutti i ponti (bridge edges) del grafo
    void edgeCut() {
        vector visited(V, false);
        vector<int> disc(V);            // Tempo di scoperta
        vector<int> low(V);             // Valore low-link
        vector parent(V, -1);           // Padre nel DFS tree
        vector<pair<int, int>> bridges; // Lista dei ponti
        int time = 0;

        // Esegue la ricerca di ponti da ogni nodo non visitato
        for (int i = 0; i < V; i++) {
            if (!visited[i])
                findBridges(i, visited, disc, low, parent, bridges, time);
        }

        // Stampa dei ponti trovati
        cout << "\nBridge edges (archi di taglio):\n";
        for (auto &[u, v] : bridges)
            cout << u << " - " << v << "\n";
    }

private:

    // Funzione ausiliaria ricorsiva per DFS
    void DFSVisit(int u, vector<bool> &visited, vector<int> &discovery, vector<int> &finish, vector<int> &parent, int &time) {
        visited[u] = true;
        discovery[u] = time++;  // Marca l'inizio della visita di u

        for (int v : adj[u]) {
            if (!visited[v]) {
                parent[v] = u;  // Salva il padre
                DFSVisit(v, visited, discovery, finish, parent, time);
            }
        }

        finish[u] = time++;  // Marca la fine della visita di u
    }

    // DFS modificata per verificare se esiste un cammino tra u e v
    bool DFSPath(int u, int target, vector<bool> &visited) {
        if (u == target) return true;
        visited[u] = true;

        for (int v : adj[u]) {
            if (!visited[v] && DFSPath(v, target, visited))
                return true;
        }

        return false;
    }

    // DFS modificata per trovare archi di taglio (bridge edges)
    void findBridges(int u, vector<bool> &visited, vector<int> &disc, vector<int> &low, vector<int> &parent, vector<pair<int, int>> &bridges, int &time) {
        visited[u] = true;
        disc[u] = low[u] = time++;  // Inizializza discovery e low time

        for (int v : adj[u]) {
            if (!visited[v]) {
                parent[v] = u;
                findBridges(v, visited, disc, low, parent, bridges, time);

                // Al ritorno, aggiorna il valore low di u
                low[u] = min(low[u], low[v]);

                // Se il valore low del figlio è maggiore del discovery di u, è un ponte
                if (low[v] > disc[u])
                    bridges.emplace_back(u, v);

            } else if (v != parent[u]) {

                // Caso di back-edge (non si torna al padre)
                low[u] = min(low[u], disc[v]);
            }
        }
    }
};

// Funzione principale per testare il grafo
int main() {
    Graph g(6);  // Crea un grafo con 6 nodi (da 0 a 5)

    // Aggiunge archi al grafo
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);

    g.printGraph();  // Stampa la lista di adiacenza

    // Verifica se due nodi sono adiacenti
    cout << "\nisAdjacent(1, 3): " << g.isAdjacent(1, 3) << "\n";

    // Verifica se esiste un cammino tra due nodi
    cout << "hasPath(0, 5): " << g.hasPath(0, 5) << "\n";

    g.BFS(0);       // Esegue BFS dal nodo 0
    g.DFS();        // Esegue DFS su tutto il grafo
    g.edgeCut();    // Trova e stampa gli archi di taglio

    return 0;
}
