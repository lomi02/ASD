#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>

using namespace std;

constexpr int INFINITY = 9999;

template<typename T>
class Node {
public:
    T val;

    explicit Node(T val) : val(val) {}
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

    int Prim(ofstream &output) {
        if (nodes.empty()) return 0;

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

            if (inMST[u]) continue;
            inMST[u] = true;

            for (auto &neighbor: adjacencyList[nodes[u]->val]) {
                int v_val = neighbor.first;
                int weight = neighbor.second;

                int v_index = -1;
                for (int i = 0; i < nodes.size(); ++i) {
                    if (nodes[i]->val == v_val) {
                        v_index = i;
                        break;
                    }
                }

                if (v_index != -1 && !inMST[v_index] && weight < key[v_index]) {
                    key[v_index] = weight;
                    pq.emplace(key[v_index], v_index);
                    parent[v_index] = u;
                }
            }
        }

        int costoTotale = 0;
        for (int i = 0; i < nodes.size(); ++i) {
            if (parent[i] != -1) {
                output << nodes[parent[i]]->val << " - " << nodes[i]->val << " " << key[i] << endl;
                costoTotale += key[i];
            }
        }
        return costoTotale;
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
        cerr << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    output << "Prim" << endl;
    int costoTotale = g.Prim(output);
    output << "Costo totale dell'MST: " << costoTotale << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
