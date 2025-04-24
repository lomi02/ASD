#include <vector>
#include <unordered_map>
using namespace std;

class DisjointSet {
    struct Node {
        int parent;
        int rank;
    };

    vector<Node> nodes;
    int setCount;

public:
    DisjointSet(int n) {
        nodes.resize(n);
        for (int i = 0; i < n; ++i) {
            nodes[i].parent = i;
            nodes[i].rank = 0;
        }
        setCount = n;
    }

    int findSet(int x) {
        if (nodes[x].parent != x) {
            nodes[x].parent = findSet(nodes[x].parent);
        }
        return nodes[x].parent;
    }

    void unionSets(int x, int y) {
        int xRoot = findSet(x);
        int yRoot = findSet(y);

        if (xRoot == yRoot) return;

        if (nodes[xRoot].rank < nodes[yRoot].rank) {
            nodes[xRoot].parent = yRoot;
        } else if (nodes[xRoot].rank > nodes[yRoot].rank) {
            nodes[yRoot].parent = xRoot;
        } else {
            nodes[yRoot].parent = xRoot;
            nodes[xRoot].rank++;
        }

        setCount--;
    }

    int getSetCount() const {
        return setCount;
    }

    bool isSameSet(int x, int y) {
        return findSet(x) == findSet(y);
    }
};

vector<vector<int> > findConnectedComponents(int numNodes, const vector<pair<int, int> > &edges) {
    DisjointSet dsu(numNodes);

    for (const auto &edge: edges) {
        int u = edge.first;
        int v = edge.second;
        dsu.unionSets(u, v);
    }

    unordered_map<int, vector<int> > componentsMap;
    for (int i = 0; i < numNodes; ++i) {
        int root = dsu.findSet(i);
        componentsMap[root].push_back(i);
    }

    vector<vector<int> > components;
    for (const auto &pair: componentsMap) {
        components.push_back(pair.second);
    }

    return components;
}

int ackermann(int k, int j) {
    if (k == 0) return j + 1;
    if (j == 1) return ackermann(k - 1, 1);
    return ackermann(k - 1, ackermann(k, j - 1));
}

// Funzione inversa di Ackermann
int inverseAckermann(int n) {
    if (n <= 2) return 0;
    if (n == 3) return 1;
    if (n <= 7) return 2;
    if (n <= 2047) return 3;
    return 4;
}
