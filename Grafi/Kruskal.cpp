#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

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

    void addNode(Node<T> *node) { nodes.push_back(node); }

    void addEdge(Node<T> *source, Node<T> *destination, T weight) {
        auto *edge = new Edge<T>(weight, source, destination);
        edges.push_back(edge);
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

    output << "Kruskal" << endl;
    vector<Edge<int> *> mst_K = g.Kruskal();
    for (auto edge: mst_K)
        output << edge->source->val << " - " << edge->destination->val << " " << edge->weight << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
