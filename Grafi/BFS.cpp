#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>

using namespace std;

constexpr int WHITE = 0;
constexpr int GRAY = 1;
constexpr int BLACK = 2;
constexpr int INFINITY = 9999;

template<typename T>
class Node {
public:
    T val;
    int color = WHITE;
    T dist = INFINITY;
    Node *parent;

    explicit Node(T val) : val(val), parent(nullptr) {}
};

template<typename T>
class Graph {
public:
    vector<Node<T> *> nodes;
    map<T, vector<pair<T, T> > > adjacencyList;

    void addNode(Node<T> *node) { nodes.push_back(node); }

    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
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
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    output << "BFS" << endl;
    if (nodesMap.count(1)) {
        g.BFS(nodesMap[1]);
        for (Node<int> *node: g.nodes)
            output << "Distanza dal nodo 1 al nodo " << node->val << ": " << node->dist << endl;
    }

    output << "\nBFS Ricorsivo" << endl;
    for (Node<int> *node: g.nodes) {
        node->color = WHITE;
        node->dist = INFINITY;
    }
    nodesMap[1]->dist = 0;

    g.BFSRecursive(nodesMap[1]);
    for (Node<int> *node: g.nodes)
        output << "Distanza dal nodo 1 al nodo " << node->val << ": " << node->dist << endl;


    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
