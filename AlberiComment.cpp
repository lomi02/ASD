#include <iostream>
using namespace std;

// Struttura base di un nodo dell'albero binario di ricerca
struct Node {
    int key;        // Chiave del nodo
    Node *parent;   // Puntatore al padre
    Node *left;     // Puntatore al figlio sinistro
    Node *right;    // Puntatore al figlio destro

    // Costruttore del nodo
    Node(int k) : key(k), parent(nullptr), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
    Node *root; // Radice dell'albero

    // Visita inorder (simmetrica): sinistra - radice - destra
    void inorder(Node *x) {
        if (x != nullptr) {
            inorder(x->left);
            cout << x->key << " ";
            inorder(x->right);
        }
    }

    // Visita preorder (anticipata): radice - sinistra - destra
    void preorder(Node *x) {
        if (x != nullptr) {
            cout << x->key << " ";
            preorder(x->left);
            preorder(x->right);
        }
    }

    // Visita postorder (differita): sinistra - destra - radice
    void postorder(Node *x) {
        if (x != nullptr) {
            postorder(x->left);
            postorder(x->right);
            cout << x->key << " ";
        }
    }

    // Restituisce il nodo con chiave minima nel sottoalbero di x
    Node *treeMinimum(Node *x) {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    // Restituisce il nodo con chiave massima nel sottoalbero di x
    Node *treeMaximum(Node *x) {
        while (x->right != nullptr)
            x = x->right;
        return x;
    }

    // Ricerca di una chiave k nel sottoalbero di x
    Node *search(Node *x, int k) {
        if (x == nullptr || x->key == k)
            return x;
        if (k < x->key)
            return search(x->left, k);
        else
            return search(x->right, k);
    }

    // Restituisce il successore del nodo x (minore nodo più grande di x)
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

    // Restituisce il predecessore del nodo x (massimo nodo più piccolo di x)
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

    // Sostituisce un sottoalbero radicato in u con quello radicato in v
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

    // Rimuove un nodo z dall'albero, mantenendo le proprietà dell'ABR
    void deleteNode(Node *z) {
        if (z->left == nullptr) {
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            transplant(z, z->left);
        } else {
            Node *y = treeMinimum(z->right); // Successore di z
            if (y->parent != z) {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
        }
        delete z; // Dealloca il nodo rimosso
    }

public:

    // Costruttore: inizializza l'albero vuoto
    BinarySearchTree() : root(nullptr) {}

    // Inserisce una nuova chiave nell'albero
    void insert(int k) {
        Node *z = new Node(k);
        Node *y = nullptr;
        Node *x = root;

        // Ricerca della posizione in cui inserire il nuovo nodo
        while (x != nullptr) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == nullptr)
            root = z; // Il nuovo nodo è la radice
        else if (z->key < y->key)
            y->left = z; // Inserimento come figlio sinistro
        else
            y->right = z; // Inserimento come figlio destro
    }

    // Rimuove un nodo con chiave k dall'albero
    void remove(int k) {
        Node *z = search(root, k);
        if (z != nullptr)
            deleteNode(z);
    }

    // Stampa l'albero in ordine crescente
    void inorder() {
        inorder(root);
        cout << endl;
    }

    // Stampa l'albero in ordine preorder
    void preorder() {
        preorder(root);
        cout << endl;
    }

    // Stampa l'albero in ordine postorder
    void postorder() {
        postorder(root);
        cout << endl;
    }

    // Ricerca una chiave e stampa se è presente o meno
    void find(int k) {
        if (Node *res = search(root, k))
            cout << "Trovato: " << res->key << endl;
        else
            cout << "Non trovato" << endl;
    }
};

int main() {
    BinarySearchTree bst;

    // Inserimento di nodi nell'ABR
    bst.insert(15);
    bst.insert(6);
    bst.insert(18);
    bst.insert(3);
    bst.insert(7);
    bst.insert(17);
    bst.insert(20);

    // Visite dell'albero
    cout << "Visita inorder: ";
    bst.inorder();

    cout << "Visita preorder: ";
    bst.preorder();

    cout << "Visita postorder: ";
    bst.postorder();

    // Ricerca di un nodo
    cout << "Ricerca del nodo 7: ";
    bst.find(7);

    // Eliminazione di un nodo
    cout << "Eliminazione del nodo 6\n";
    bst.remove(6);

    // Stampa dell'albero dopo la rimozione
    cout << "Visita inorder dopo rimozione: ";
    bst.inorder();

    return 0;
}
