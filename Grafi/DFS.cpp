#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

constexpr int WHITE = 0;
constexpr int GRAY = 1;
constexpr int BLACK = 2;

template<typename T>
class Node {
public:
    T val;
    int color = WHITE;
    T discovery_time = 0;
    T finish_time = 0;
    Node *parent;

    explicit Node(T val) : val(val), parent(nullptr) {}
};

template<typename T>
class Graph {
public:
    T time = 0;
    vector<Node<T> *> nodes;
    map<T, vector<pair<T, T> > > adjacencyList;

    void addNode(Node<T> *node) { nodes.push_back(node); }

    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        adjacencyList[source->val].push_back(make_pair(destination->val, weight));
        adjacencyList[destination->val].push_back(make_pair(source->val, weight));
    }

    void DFS() {
        for (auto u: nodes) {
            u->color = WHITE;
            u->parent = nullptr;
        }
        time = 0;
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
};

int main() {
    ifstream input("inputG.txt");
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

    ofstream output("outputG.txt");
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    output << "DFS" << endl;
    g.DFS();
    for (Node<int> *node: g.nodes)
        output << "Nodo " << node->val << ": tempo di scoperta = " << node->discovery_time <<
                ", tempo di completamento = " << node->finish_time << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
