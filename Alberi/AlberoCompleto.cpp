#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class Node {
public:
    T val;
    Node *parent, *left, *right;

    explicit Node(T v) : val(v), parent(nullptr), left(nullptr), right(nullptr) {}
};

template<typename T>
class ABR {
public:
    Node<T> *root;

    ABR() : root(nullptr) {}

    void clear(Node<T> *node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void InsertNode(Node<T> *nodeToInsert) {
        Node<T> *currNode = root;
        Node<T> *parentNode = nullptr;

        while (currNode != nullptr) {
            parentNode = currNode;
            if (nodeToInsert->val < currNode->val)
                currNode = currNode->left;
            else
                currNode = currNode->right;
        }

        nodeToInsert->parent = parentNode;
        if (parentNode == nullptr)
            root = nodeToInsert;
        else if (nodeToInsert->val < parentNode->val)
            parentNode->left = nodeToInsert;
        else
            parentNode->right = nodeToInsert;
    }

    void transplant(Node<T> *u, Node<T> *v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v != nullptr)
            v->parent = u->parent;
    }

    void deleteNode(T val) {
        Node<T> *nodeToDelete = searchNode(val);
        if (nodeToDelete == nullptr)
            return;

        if (nodeToDelete->left == nullptr)
            transplant(nodeToDelete, nodeToDelete->right);
        else if (nodeToDelete->right == nullptr)
            transplant(nodeToDelete, nodeToDelete->left);
        else {
            Node<T> *minimum = tree_Min(nodeToDelete->right);
            if (minimum->parent != nodeToDelete) {
                transplant(minimum, minimum->right);
                minimum->right = nodeToDelete->right;
                minimum->right->parent = minimum;
            }
            transplant(nodeToDelete, minimum);
            minimum->left = nodeToDelete->left;
            minimum->left->parent = minimum;
        }
        delete nodeToDelete;
    }

    Node<T> *tree_Min(Node<T> *node) {
        while (node && node->left != nullptr)
            node = node->left;
        return node;
    }

    Node<T> *tree_Max(Node<T> *node) {
        while (node && node->right != nullptr)
            node = node->right;
        return node;
    }

    Node<T> *tree_Successor(Node<T> *node) {
        if (node->right != nullptr)
            return tree_Min(node->right);

        Node<T> *parentNode = node->parent;
        while (parentNode != nullptr && node == parentNode->right) {
            node = parentNode;
            parentNode = parentNode->parent;
        }
        return parentNode;
    }

    Node<T> *tree_Predecessor(Node<T> *node) {
        if (node->left != nullptr)
            return tree_Max(node->left);

        Node<T> *parentNode = node->parent;
        while (parentNode != nullptr && node == parentNode->left) {
            node = parentNode;
            parentNode = parentNode->parent;
        }
        return parentNode;
    }

    Node<T> *searchNode(T val) {
        Node<T> *curr = root;
        while (curr != nullptr && val != curr->val) {
            if (val < curr->val)
                curr = curr->left;
            else
                curr = curr->right;
        }
        return curr;
    }

    Node<T> *r_searchNode(Node<T> *node, T val) {
        if (node == nullptr || val == node->val)
            return node;
        if (val < node->val)
            return r_searchNode(node->left, val);
        return r_searchNode(node->right, val);
    }

    void preorderVisit(Node<T> *node, vector<T> &v) {
        if (node) {
            v.push_back(node->val);
            preorderVisit(node->left, v);
            preorderVisit(node->right, v);
        }
    }

    void inorderVisit(Node<T> *node, vector<T> &v) {
        if (node) {
            inorderVisit(node->left, v);
            v.push_back(node->val);
            inorderVisit(node->right, v);
        }
    }

    void postorderVisit(Node<T> *node, vector<T> &v) {
        if (node) {
            postorderVisit(node->left, v);
            postorderVisit(node->right, v);
            v.push_back(node->val);
        }
    }

    int getHeight(Node<T> *node) {
        if (node == nullptr) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }
};

int main() {
    ifstream input("inputT.txt");
    if (!input.is_open()) {
        cerr << "Errore apertura input.txt" << endl;
        return 1;
    }

    ABR<int> albero;
    int element;
    while (input >> element)
        albero.InsertNode(new Node<int>(element));

    input.close();

    ofstream output("outputT.txt");
    if (!output.is_open()) {
        cerr << "Errore apertura output.txt" << endl;
        return 1;
    }

    int val_search = 3;
    if (albero.searchNode(val_search))
        output << "Nodo " << val_search << " trovato (iterativo)" << endl;
    else
        output << "Nodo " << val_search << " non trovato" << endl;

    int val_search_r = 5;
    if (albero.r_searchNode(albero.root, val_search_r))
        output << "Nodo " << val_search_r << " trovato (ricorsivo)" << endl;
    else
        output << "Nodo " << val_search_r << " non trovato" << endl;

    albero.deleteNode(3);
    if (!albero.searchNode(3))
        output << "Nodo 3 rimosso con successo" << endl;

    output << endl;

    Node<int> *min_node = albero.tree_Min(albero.root);
    if (min_node)
        output << "Il nodo con valore minimo: " << min_node->val << endl;

    int val_pred = 5;
    Node<int> *node_pred = albero.searchNode(val_pred);
    if (node_pred) {
        Node<int> *pred = albero.tree_Predecessor(node_pred);
        if (pred)
            output << "Il predecessore di " << val_pred << ": " << pred->val << endl;
        else
            output << "Nessun predecessore per " << val_pred << endl;
    }

    output << endl;

    vector<int> values;
    albero.inorderVisit(albero.root, values);
    output << "Visita in ordine: ";
    for (int v: values)
        output << v << " ";
    output << endl;

    output << "Altezza dell'albero: " << albero.getHeight(albero.root) << endl;

    output.close();
    cout << "File 'output.txt' creato con successo!" << endl;

    return 0;
}
