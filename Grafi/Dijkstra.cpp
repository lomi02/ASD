#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>

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
};

int main() {
    bool pesiNegativi;
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

        if (weight < 0)
            pesiNegativi = true;
    }
    input.close();

    ofstream output("outputG.txt");
    if (!output.is_open()) {
        cout << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    if (pesiNegativi)
        output << "Dijkstra non puo' essere effettuato in quanto vi sono pesi negativi" << endl;
    else {
        output << "Dijkstra" << endl;
        if (nodesMap.count(1)) {
            g.Dijkstra(nodesMap[1]);
            for (Node<int> *node: g.nodes)
                output << "Distanza piu breve da nodo " << nodesMap[1]->val << " a nodo " << node->val << ": " << node->dist << endl;
        }
    }

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
