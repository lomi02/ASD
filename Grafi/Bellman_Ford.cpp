#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

constexpr int INFINITY = 9999;

template<typename T>
class Node {
public:
    T val;
    T dist = INFINITY;

    explicit Node(T val) : val(val) {}
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

    string result;
    result = g.Bellman_Ford(nodesMap[1]);
    output << "Bellman_Ford" << endl;
    output << result;
    output << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
