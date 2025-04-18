#include <iostream>
using namespace std;

// Struttura base di un nodo dell'albero binario di ricerca
struct Node {
    int key;
    Node *parent;
    Node *left;
    Node *right;

    Node(int k) : key(k), parent(nullptr), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
    Node *root;

    void inorder(Node *x) {
        if (x != nullptr) {
            inorder(x->left);
            cout << x->key << " ";
            inorder(x->right);
        }
    }

    void preorder(Node *x) {
        if (x != nullptr) {
            cout << x->key << " ";
            preorder(x->left);
            preorder(x->right);
        }
    }

    void postorder(Node *x) {
        if (x != nullptr) {
            postorder(x->left);
            postorder(x->right);
            cout << x->key << " ";
        }
    }

    Node *treeMinimum(Node *x) {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    Node *treeMaximum(Node *x) {
        while (x->right != nullptr)
            x = x->right;
        return x;
    }

    Node *search(Node *x, int k) {
        if (x == nullptr || x->key == k)
            return x;
        if (k < x->key)
            return search(x->left, k);
        else
            return search(x->right, k);
    }

    Node *successor(Node *x) {
        if (x->right != nullptr)
            return treeMinimum(x->right);
        Node *y = x->parent;
        while (y != nullptr && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    Node *predecessor(Node *x) {
        if (x->left != nullptr)
            return treeMaximum(x->left);
        Node *y = x->parent;
        while (y != nullptr && x == y->left) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    void transplant(Node *u, Node *v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    void deleteNode(Node *z) {
        if (z->left == nullptr) {
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            transplant(z, z->left);
        } else {
            Node *y = treeMinimum(z->right);
            if (y->parent != z) {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
        }
        delete z;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(int k) {
        Node *z = new Node(k);
        Node *y = nullptr;
        Node *x = root;

        while (x != nullptr) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
    }

    void remove(int k) {
        Node *z = search(root, k);
        if (z != nullptr)
            deleteNode(z);
    }

    void inorder() {
        inorder(root);
        cout << endl;
    }

    void preorder() {
        preorder(root);
        cout << endl;
    }

    void postorder() {
        postorder(root);
        cout << endl;
    }

    void find(int k) {
        if (Node *res = search(root, k))
            cout << "Trovato: " << res->key << endl;
        else
            cout << "Non trovato" << endl;
    }
};
