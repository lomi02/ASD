#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <stack>
#include <queue>

using namespace std;

constexpr int WHITE = 0;
constexpr int GRAY = 1;
constexpr int BLACK = 2;
constexpr int INFINITY = 9999;

template<typename T>
class Node {
public:
    T val, key;
    int rank{};
    int color = WHITE;
    T dist = INFINITY;
    T discovery_time = 0;
    T finish_time = 0;

    Node *parent;

    explicit Node(T val) : val(val), parent(nullptr) {}
};

template<typename T>
class Edge {
public:
    T weight;
    Node<T> *source;
    Node<T> *destination;

    Edge(T weight, Node<T> *source, Node<T> *destination) : weight(weight), source(source), destination(destination) {}
};

template<typename T>
class minCompare {
public:
    bool operator()(Node<T> *a, Node<T> *b) {return a->key > b->key;}
};

template<typename T>
class Graph {
public:
    T time = 0;
    vector<Node<T> *> nodes;
    vector<Edge<T> *> edges;
    map<T, vector<pair<T, T> > > adjacencyList;

    void addNode(Node<T> *node) {nodes.push_back(node);}

    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        auto *edge = new Edge<T>(weight, source, destination);
        edges.push_back(edge);
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));
        adjacencyList[destination->val].push_back(make_pair(source->val, weight));
    }

    void BFS(Node<T> *startNode) {
        for (Node<T> *node: nodes) {
            node->color = WHITE;
            node->parent = nullptr;
            node->dist = INFINITY;
        }
        startNode->color = GRAY;
        startNode->parent = nullptr;
        startNode->dist = 0;
        queue<Node<T> *> q;
        q.push(startNode);
        while (!q.empty()) {
            Node<T> *u = q.front();
            q.pop();
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

                if (node->color == WHITE) {
                    node->color = GRAY;
                    node->parent = u;
                    node->dist = u->dist + v.second;
                    q.push(node);
                }
            }
            u->color = BLACK;
        }
    }

    void BFSRecursive(Node<T> *u) {
        u->color = GRAY;
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

            if (node->color == WHITE) {
                node->parent = u;
                node->dist = u->dist + v.second;
                BFSRecursive(node);
            }
        }
        u->color = BLACK;
    }

    void DFS() {
        for (auto u: nodes) {
            u->color = WHITE;
            u->parent = nullptr;
        }
        for (auto u: nodes) {
            if (u->color == WHITE)
                DFS_Visit(u);
        }
    }

    void DFS_Visit(Node<T> *u) {
        u->color = GRAY;
        u->discovery_time = ++time;
        for (auto v: adjacencyList[u->val]) {
            Node<T> *node = nullptr;
            for (Node<T> *n: nodes)
                if (n->val == v.first) {
                    node = n;
                    break;
                }

            if (node == nullptr)
                continue;
            if (node->color == WHITE) {
                node->parent = u;
                DFS_Visit(node);
            }
        }
        u->color = BLACK;
        u->finish_time = ++time;
    }

    void MakeSet(Node<T> *x) {
        x->parent = x;
        x->rank = 0;
    }

    Node<T> *FindSet(Node<T> *x) {
        if (x != x->parent)
            x->parent = FindSet(x->parent);
        return x->parent;
    }

    void UnionSet(Node<T> *x, Node<T> *y) {
        LinkSet(FindSet(x), FindSet(y));
    }

    void LinkSet(Node<T> *x, Node<T> *y) {
        if (x->rank > y->rank)
            y->parent = x;

        else
            x->parent = y;

        if (x->rank == y->rank)
            ++y->rank;
    }

    void ConnectedComponents(ofstream &output) {
        map<Node<T> *, vector<Node<T> *> > connectedComponentMap;

        for (auto v: nodes)
            MakeSet(v);

        for (auto edge: edges)
            if (FindSet(edge->source) != FindSet(edge->destination))
                UnionSet(edge->source, edge->destination);

        for (auto v: nodes) {
            Node<T> *representative = FindSet(v);
            connectedComponentMap[representative].push_back(v);
        }

        for (const auto &pair: connectedComponentMap) {
            for (const auto &node: pair.second)
                output << node->val << " ";
            output << endl;
        }
    }

    void InitializeSingleSource(Node<T> *source) {
        for (auto node: nodes)
            node->dist = INFINITY;
        source->dist = 0;
    }

    void Relax(Edge<T> *edge) {
        T newDist = edge->source->dist + edge->weight;
        if (newDist < edge->destination->dist)
            edge->destination->dist = newDist;
    }

    void Dijkstra(Node<T> *source) {
        InitializeSingleSource(source);
        priority_queue<pair<T, Node<T> *>, vector<pair<T, Node<T> *> >, greater<pair<T, Node<T> *> > > pq;
        pq.push({source->dist, source});
        while (!pq.empty()) {
            Node<T> *curr = pq.top().second;
            pq.pop();
            for (Edge<T> *edge: edges)
                if (edge->source == curr) {
                    Relax(edge);
                    pq.push({edge->destination->dist, edge->destination});
                }
        }
    }

    string Bellman_Ford(Node<T> *source) {
        InitializeSingleSource(source);
        int i = 0;
        while (i < nodes.size()) {
            for (Edge<T> *edge: edges)
                Relax(edge);
            for (Edge<T> *edge: edges)
                if (edge->destination->dist > edge->source->dist + edge->weight)
                    return "Trovato ciclo negativo \n";
            i++;
        }
        return "Non trovato ciclo negativo \n";
    }

    void ShowAdjacentList(Node<T> *u, ofstream &output) {
        output << "Lista di adiacenza del nodo " << u->val << ":\n";
        for (auto it = adjacencyList[u->val].begin(); it != adjacencyList[u->val].end(); ++it)
            output << " " << u->val << " -> " << it->first << " (peso " << it->second << ")\n";
    }

    int Prim(ofstream &output) {
        int src = 0;
        vector<int> key(nodes.size(), INFINITY);
        vector<bool> inMST(nodes.size(), false);
        vector<int> parent(nodes.size(), -1);

        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

        pq.emplace(0, src);
        key[src] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            inMST[u] = true;

            for (auto v: adjacencyList[nodes[u]->val]) {
                int vertex = v.first;
                int weight = v.second;

                if (!inMST[vertex] && weight < key[vertex]) {
                    key[vertex] = weight;
                    pq.emplace(key[vertex], vertex);
                    parent[vertex] = u;
                }
            }
        }

        int costoTotale = 0;
        for (int i = 1; i < nodes.size(); ++i) {
            output << parent[i] << " - " << i << " " << key[i] << endl;
            costoTotale += key[i];
        }
        return costoTotale;
    }

    vector<Edge<T> *> Kruskal() {
        vector<Edge<T> *> A;
        for (auto node: nodes)
            MakeSet(node);
        sort(edges.begin(), edges.end(), [](Edge<T> *a, Edge<T> *b) {
            return a->weight < b->weight;
        });

        for (auto edge: edges)
            if (FindSet(edge->source) != FindSet(edge->destination)) {
                A.push_back(edge);
                UnionSet(edge->source, edge->destination);
            }
        return A;
    }

    void DFSUtil(int v, bool visited[], stack<int> &stack) {
        visited[v] = true;
        for (auto i = adjacencyList[v].begin(); i != adjacencyList[v].end(); ++i)
            if (!visited[i->first])
                DFSUtil(i->first, visited, stack);
        stack.push(v);
    }

    void DFSUtil2(int v, bool visited[], ofstream &output) {
        visited[v] = true;
        output << v << " ";
        for (auto i = adjacencyList[v].begin(); i != adjacencyList[v].end(); ++i)
            if (!visited[i->first])
                DFSUtil2(i->first, visited, output);
    }

    Graph Transpose() {
        Graph transposedGraph;
        for (auto &node: nodes) {
            transposedGraph.addNode(new Node<T>(node->val));
        }
        for (const auto &edge: edges) {
            transposedGraph.addEdge(edge->source, edge->destination, edge->weight);
        }
        return transposedGraph;
    }

    void PrintSCCs(ofstream &output) {
        stack<int> stack;
        bool *visited = new bool[nodes.size()];
        for (int i = 0; i < nodes.size(); i++)
            visited[i] = false;

        for (int i = 0; i < nodes.size(); i++)
            if (!visited[i])
                DFSUtil(i, visited, stack);

        Graph g = Transpose();
        for (int i = 0; i < nodes.size(); i++)
            visited[i] = false;

        while (!stack.empty()) {
            int v = stack.top();
            stack.pop();
            if (!visited[v]) {
                g.DFSUtil2(v, visited, output);
                output << endl;
            }
        }
        delete[] visited;
    }
};

int main() {
    bool pesiNegativi;
    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Errore durante la lettura del file di input" << endl;
        return 1;
    }

    int numNodes, numEdges;
    input >> numNodes >> numEdges;

    unordered_map<int, Node<int> *> nodesMap;

    Graph<int> g;

    for (int i = 0; i < numEdges; i++) {
        int sourceVal, destVal, weight;
        input >> sourceVal >> destVal >> weight;

        Node<int> *sourceNode = nullptr;
        auto itSource = nodesMap.find(sourceVal);
        if (itSource == nodesMap.end()) {
            sourceNode = new Node<int>(sourceVal);
            nodesMap[sourceVal] = sourceNode;
            g.addNode(sourceNode);
        } else
            sourceNode = itSource->second;

        Node<int> *destNode = nullptr;
        auto itDest = nodesMap.find(destVal);
        if (itDest == nodesMap.end()) {
            destNode = new Node<int>(destVal);
            nodesMap[destVal] = destNode;
            g.addNode(destNode);
        } else
            destNode = itDest->second;

        g.addEdge(sourceNode, destNode, weight);

        if (weight < 0)
            pesiNegativi = true;
    }

    input.close();

    ofstream output("output.txt");
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    output << "Lista di Adiacenza:" << endl;
    for (Node<int> *node: g.nodes)
        g.ShowAdjacentList(node, output);

    output << endl;
    output << "BFS" << endl;
    g.BFS(nodesMap[1]);
    for (Node<int> *node: g.nodes)
        output << "Distanza dal nodo " << nodesMap[1]->val << " al nodo " << node->val << ": " << node->dist << endl;

    output << endl;
    output << "BFS Ricorsivo" << endl;
    g.BFSRecursive(nodesMap[1]);
    for (Node<int> *node: g.nodes)
        output << "Distanza dal nodo " << nodesMap[1]->val << " al nodo " << node->val << ": " << node->dist << endl;

    output << endl;
    output << "DFS" << endl;
    g.DFS();
    for (Node<int> *node: g.nodes)
        output << "Nodo " << node->val << ": tempo di scoperta = " << node->discovery_time <<
                ", tempo di completamento = " << node->finish_time << endl;

    output << endl;
    if (pesiNegativi)
        output << "Dijkstra non puo' essere effettuato in quanto vi sono pesi negativi" << endl;
    else {
        output << "Dijkstra" << endl;
        g.Dijkstra(nodesMap[1]);
        for (Node<int> *node: g.nodes)
            output << "Distanza piu breve da nodo " << nodesMap[1]->val << " a nodo " << node->val << ": " << node->dist
                    << endl;
    }

    output << endl;
    string result;
    result = g.Bellman_Ford(nodesMap[1]);
    output << "Bellman_Ford" << endl;
    output << result;
    output << endl;

    output << "Prim" << endl;
    int costoTotale = g.Prim(output);
    output << "Costo totale dell'MST: " << costoTotale << endl;
    output << endl;

    output << "Kruskal" << endl;
    vector<Edge<int> *> mst_K = g.Kruskal();
    for (auto edge: mst_K)
        output << edge->source->val << " - " << edge->destination->val << " " << edge->weight << endl;

    output << endl;

    output << "Componenti fortemente connesse:" << endl;
    g.PrintSCCs(output);
    output << endl;

    output << "Componenti connesse: " << endl;
    g.ConnectedComponents(output);
    output << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
