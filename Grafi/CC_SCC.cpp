#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>

using namespace std;

template<typename T>
class Node {
public:
    T val;
    int rank{};
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
class Graph {
public:
    vector<Node<T> *> nodes;
    vector<Edge<T> *> edges;
    map<T, vector<pair<T, T> > > adjacencyList;

    void addNode(Node<T> *node) { nodes.push_back(node); }

    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        auto *edge = new Edge<T>(weight, source, destination);
        edges.push_back(edge);
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));
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
        else {
            x->parent = y;
            if (x->rank == y->rank)
                ++y->rank;
        }
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

    void ShowAdjacentList(Node<T> *u, ofstream &output) {
        output << "Lista di adiacenza del nodo " << u->val << ":\n";
        for (auto &it: adjacencyList[u->val])
            output << " " << u->val << " -> " << it.first << " (peso " << it.second << ")\n";
    }

    void DFSUtil(int v, map<int, bool> &visited, stack<int> &st) {
        visited[v] = true;
        for (auto &edge: adjacencyList[v])
            if (!visited[edge.first])
                DFSUtil(edge.first, visited, st);
        st.push(v);
    }

    void DFSUtil2(int v, map<int, bool> &visited, ofstream &output) {
        visited[v] = true;
        output << v << " ";
        for (auto &edge: adjacencyList[v])
            if (!visited[edge.first])
                DFSUtil2(edge.first, visited, output);
    }

    Graph Transpose() {
        Graph transposedGraph;
        unordered_map<T, Node<T> *> transNodesMap;

        for (auto &node: nodes) {
            Node<T> *newNode = new Node<T>(node->val);
            transposedGraph.addNode(newNode);
            transNodesMap[node->val] = newNode;
        }
        for (const auto &edge: edges) {
            transposedGraph.addEdge(transNodesMap[edge->destination->val], transNodesMap[edge->source->val], edge->weight);
        }
        return transposedGraph;
    }

    void PrintSCCs(ofstream &output) {
        stack<int> st;
        map<int, bool> visited;

        for (auto n: nodes) visited[n->val] = false;

        for (auto n: nodes)
            if (!visited[n->val])
                DFSUtil(n->val, visited, st);

        Graph gT = Transpose();
        for (auto n: nodes) visited[n->val] = false;

        while (!st.empty()) {
            int v = st.top();
            st.pop();
            if (!visited[v]) {
                gT.DFSUtil2(v, visited, output);
                output << endl;
            }
        }
    }
};

int main() {
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

        for (int val: {sourceVal, destVal}) {
            if (nodesMap.find(val) == nodesMap.end()) {
                auto newNode = new Node<int>(val);
                nodesMap[val] = newNode;
                g.addNode(newNode);
            }
        }
        g.addEdge(nodesMap[sourceVal], nodesMap[destVal], weight);
    }
    input.close();

    ofstream output("output.txt");
    if (!output.is_open()) return 1;

    output << "Lista di Adiacenza:" << endl;
    for (Node<int> *node: g.nodes)
        g.ShowAdjacentList(node, output);

    output << "\nComponenti connesse: " << endl;
    g.ConnectedComponents(output);

    output << "\nComponenti fortemente connesse:" << endl;
    g.PrintSCCs(output);

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
