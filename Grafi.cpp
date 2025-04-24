#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Graph {
    int V;
    vector<vector<int> > adj;

public:
    Graph(int vertices) : V(vertices), adj(vertices) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void printGraph() const {
        cout << "Lista di adiacenza:\n";
        for (int u = 0; u < V; u++) {
            cout << u << ": ";
            for (int v: adj[u])
                cout << v << " ";
            cout << "\n";
        }
    }

    void BFS(int start) {
        vector visited(V, false);
        vector distance(V, -1);
        vector parent(V, -1);
        queue<int> q;

        visited[start] = true;
        distance[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v: adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    distance[v] = distance[u] + 1;
                    q.push(v);
                }
            }
        }

        cout << "\nRisultato BFS da nodo " << start << ":\n";
        for (int i = 0; i < V; ++i)
            cout << "Nodo: " << i << ", Distanza: " << distance[i] << ", Padre: " << parent[i] << "\n";
    }

    void DFS() {
        vector visited(V, false);
        vector<int> discovery(V), finish(V), parent(V, -1);
        int time = 0;

        cout << "\nRisultato DFS:\n";
        for (int u = 0; u < V; ++u) {
            if (!visited[u])
                DFSVisit(u, visited, discovery, finish, parent, time);
        }

        for (int i = 0; i < V; ++i)
            cout << "Nodo: " << i << ", d: " << discovery[i] << ", f: " << finish[i] << ", Padre: " << parent[i] << "\n";
    }

    bool isAdjacent(int u, int v) const {
        return ranges::find(adj[u], v) != adj[u].end();
    }

    bool hasPath(int u, int v) {
        vector visited(V, false);
        return DFSPath(u, v, visited);
    }

    void edgeCut() {
        vector visited(V, false);
        vector<int> disc(V), low(V), parent(V, -1);
        vector<pair<int, int> > bridges;
        int time = 0;

        for (int i = 0; i < V; i++) {
            if (!visited[i])
                findBridges(i, visited, disc, low, parent, bridges, time);
        }

        cout << "\nBridge edges (archi di taglio):\n";
        for (auto &[u, v]: bridges)
            cout << u << " - " << v << "\n";
    }

private:
    void DFSVisit(int u, vector<bool> &visited, vector<int> &discovery, vector<int> &finish, vector<int> &parent, int &time) {
        visited[u] = true;
        discovery[u] = time++;

        for (int v: adj[u]) {
            if (!visited[v]) {
                parent[v] = u;
                DFSVisit(v, visited, discovery, finish, parent, time);
            }
        }

        finish[u] = time++;
    }

    bool DFSPath(int u, int target, vector<bool> &visited) {
        if (u == target) return true;
        visited[u] = true;

        for (int v: adj[u]) {
            if (!visited[v] && DFSPath(v, target, visited))
                return true;
        }

        return false;
    }

    void findBridges(int u, vector<bool> &visited, vector<int> &disc, vector<int> &low, vector<int> &parent, vector<pair<int, int> > &bridges, int &time) {
        visited[u] = true;
        disc[u] = low[u] = time++;

        for (int v: adj[u]) {
            if (!visited[v]) {
                parent[v] = u;
                findBridges(v, visited, disc, low, parent, bridges, time);

                low[u] = min(low[u], low[v]);

                if (low[v] > disc[u]) {
                    bridges.emplace_back(u, v);
                }
            } else if (v != parent[u]) {
                low[u] = min(low[u], disc[v]);
            }
        }
    }
};