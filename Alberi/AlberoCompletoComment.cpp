#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Classe Node: rappresenta il singolo nodo dell'albero.
 * Utilizza un template <typename T> per gestire qualsiasi tipo di dato comparabile.
 */
template<typename T>
class Node {
public:
    T val;                          // Valore memorizzato nel nodo
    Node *parent, *left, *right;    // Puntatori al padre, al figlio sinistro e al figlio destro

    // Costruttore: inizializza il valore e imposta i puntatori a nullptr
    explicit Node(T v) : val(v), parent(nullptr), left(nullptr), right(nullptr) {
    }
};

/**
 * Classe ABR: implementa l'Albero Binario di Ricerca e le sue operazioni.
 */
template<typename T>
class ABR {
public:
    Node<T> *root; // Puntatore alla radice dell'albero

    // Costruttore dell'albero: inizialmente vuoto (radice nulla)
    ABR() : root(nullptr) {
    }

    /**
     * Funzione clear: dealloca ricorsivamente tutta la memoria occupata dall'albero.
     * Segue una logica post-ordine (elimina i figli prima del nodo corrente).
     */
    void clear(Node<T> *node) {
        if (node) {
            clear(node->left);  // Ricorsione sul figlio sinistro
            clear(node->right); // Ricorsione sul figlio destro
            delete node;        // Deallocazione del nodo corrente
        }
    }

    /**
     * Funzione InsertNode: inserisce un nuovo nodo nell'albero mantenendo la proprietà dell'ABR.
     * Proprietà: a sinistra valori minori, a destra valori maggiori o uguali.
     */
    void InsertNode(Node<T> *nodeToInsert) {
        Node<T> *currNode = root;       // Nodo usato per scorrere l'albero
        Node<T> *parentNode = nullptr;  // Manterrà il riferimento al futuro padre

        // Scendo lungo l'albero fino a trovare la posizione corretta (una foglia nulla)
        while (currNode != nullptr) {
            parentNode = currNode;
            if (nodeToInsert->val < currNode->val)
                currNode = currNode->left;  // Scendo a sinistra
            else
                currNode = currNode->right; // Scendo a destra
        }

        nodeToInsert->parent = parentNode; // Imposto il padre del nuovo nodo

        if (parentNode == nullptr)
            root = nodeToInsert;                // L'albero era vuoto, il nodo diventa la radice
        else if (nodeToInsert->val < parentNode->val)
            parentNode->left = nodeToInsert;    // Aggancio come figlio sinistro
        else
            parentNode->right = nodeToInsert;   // Aggancio come figlio destro
    }

    /**
     * Funzione transplant: utility per la cancellazione.
     * Sostituisce il sottoalbero radicato nel nodo 'u' con il sottoalbero radicato in 'v'.
     */
    void transplant(Node<T> *u, Node<T> *v) {
        if (u->parent == nullptr)
            root = v;               // Se u è radice, v diventa la nuova radice
        else if (u == u->parent->left)
            u->parent->left = v;    // Se u è figlio sinistro, v diventa il nuovo figlio sinistro
        else
            u->parent->right = v;   // Se u è figlio destro, v diventa il nuovo figlio destro

        if (v != nullptr)
            v->parent = u->parent;  // Aggiorno il puntatore al padre nel nodo v
    }

    /**
     * Funzione deleteNode: rimuove un nodo con un determinato valore.
     * Gestisce i tre casi classici: zero figli, un figlio, due figli.
     */
    void deleteNode(T val) {
        Node<T> *nodeToDelete = searchNode(val); // Cerco il puntatore al nodo da eliminare
        if (nodeToDelete == nullptr)
            return; // Se il nodo non esiste, esco

        if (nodeToDelete->left == nullptr)
            transplant(nodeToDelete, nodeToDelete->right);  // Caso 1: solo figlio destro o nessuno
        else if (nodeToDelete->right == nullptr)
            transplant(nodeToDelete, nodeToDelete->left);   // Caso 2: solo figlio sinistro
        else {

            // Caso 3: due figli. Cerco il successore (minimo del sottoalbero destro)
            Node<T> *minimum = tree_Min(nodeToDelete->right);
            if (minimum->parent != nodeToDelete) {

                // Se il minimo non è figlio diretto, devo "staccarlo" e sistemare i puntatori
                transplant(minimum, minimum->right);
                minimum->right = nodeToDelete->right;
                minimum->right->parent = minimum;
            }

            // Sostituisco il nodo da eliminare con il suo successore
            transplant(nodeToDelete, minimum);
            minimum->left = nodeToDelete->left;
            minimum->left->parent = minimum;
        }
        delete nodeToDelete; // Dealloco fisicamente la memoria
    }

    /**
     * Funzione tree_Min: trova il nodo con il valore minimo partendo da un nodo dato.
     * Il minimo si trova sempre scorrendo tutto a sinistra.
     */
    Node<T> *tree_Min(Node<T> *node) {
        while (node && node->left != nullptr)
            node = node->left;
        return node;
    }

    /**
     * Funzione tree_Max: trova il nodo con il valore massimo partendo da un nodo dato.
     * Il massimo si trova sempre scorrendo tutto a destra.
     */
    Node<T> *tree_Max(Node<T> *node) {
        while (node && node->right != nullptr)
            node = node->right;
        return node;
    }

    /**
     * Funzione tree_Successor: trova il successore di un nodo (il più piccolo valore maggiore di quello dato).
     */
    Node<T> *tree_Successor(Node<T> *node) {

        // Se c'è un sottoalbero destro, il successore è il minimo di quel sottoalbero
        if (node->right != nullptr)
            return tree_Min(node->right);

        // Altrimenti, risalgo verso l'alto finché trovo un nodo che sia figlio sinistro di suo padre
        Node<T> *parentNode = node->parent;
        while (parentNode != nullptr && node == parentNode->right) {
            node = parentNode;
            parentNode = parentNode->parent;
        }
        return parentNode;
    }

    /**
     * Funzione tree_Predecessor: trova il predecessore di un nodo (il più grande valore minore di quello dato).
     */
    Node<T> *tree_Predecessor(Node<T> *node) {

        // Se c'è un sottoalbero sinistro, il predecessore è il massimo di quel sottoalbero
        if (node->left != nullptr)
            return tree_Max(node->left);

        // Altrimenti, risalgo finché trovo un nodo che sia figlio destro di suo padre
        Node<T> *parentNode = node->parent;
        while (parentNode != nullptr && node == parentNode->left) {
            node = parentNode;
            parentNode = parentNode->parent;
        }
        return parentNode;
    }

    /**
     * Funzione searchNode: ricerca iterativa di un valore.
     * Complessità media O(log n), peggiore O(n) se l'albero è sbilanciato.
     */
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

    /**
     * Funzione r_searchNode: ricerca ricorsiva di un valore.
     */
    Node<T> *r_searchNode(Node<T> *node, T val) {
        if (node == nullptr || val == node->val)
            return node;
        if (val < node->val)
            return r_searchNode(node->left, val);
        return r_searchNode(node->right, val);
    }

    /**
     * Funzione preorderVisit: visita (Radice, Sinistra, Destra).
     */
    void preorderVisit(Node<T> *node, vector<T> &v) {
        if (node) {
            v.push_back(node->val); // Visita la radice
            preorderVisit(node->left, v); // Vai a sinistra
            preorderVisit(node->right, v); // Vai a destra
        }
    }

    /**
     * Funzione inorderVisit: visita (Sinistra, Radice, Destra).
     * Produce i valori in ordine crescente.
     */
    void inorderVisit(Node<T> *node, vector<T> &v) {
        if (node) {
            inorderVisit(node->left, v);
            v.push_back(node->val);
            inorderVisit(node->right, v);
        }
    }

    /**
     * Funzione postorderVisit: visita (Sinistra, Destra, Radice).
     */
    void postorderVisit(Node<T> *node, vector<T> &v) {
        if (node) {
            postorderVisit(node->left, v);
            postorderVisit(node->right, v);
            v.push_back(node->val);
        }
    }

    /**
     * Funzione getHeight: calcola l'altezza dell'albero (massimo numero di livelli).
     */
    int getHeight(Node<T> *node) {
        if (node == nullptr) return 0; // Un albero vuoto ha altezza 0

        // L'altezza è 1 (il nodo stesso) + il massimo tra le altezze dei figli
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }
};

int main() {

    // Apertura del file di input
    ifstream input("inputT.txt");
    if (!input.is_open()) {
        cerr << "Errore apertura input.txt" << endl;
        return 1;
    }

    ABR<int> albero;
    int element;

    // Leggo gli elementi dal file e li inserisco nell'albero
    while (input >> element)
        albero.InsertNode(new Node<int>(element));

    input.close();

    // Apertura del file di output
    ofstream output("outputT.txt");
    if (!output.is_open()) {
        cerr << "Errore apertura output.txt" << endl;
        return 1;
    }

    // Esempio di ricerca iterativa
    int val_search = 3;
    if (albero.searchNode(val_search))
        output << "Nodo " << val_search << " trovato (iterativo)" << endl;
    else
        output << "Nodo " << val_search << " non trovato" << endl;

    // Esempio di ricerca ricorsiva
    int val_search_r = 5;
    if (albero.r_searchNode(albero.root, val_search_r))
        output << "Nodo " << val_search_r << " trovato (ricorsivo)" << endl;
    else
        output << "Nodo " << val_search_r << " non trovato" << endl;

    // Cancellazione di un nodo
    albero.deleteNode(3);
    if (!albero.searchNode(3))
        output << "Nodo 3 rimosso con successo" << endl;

    output << endl;

    // Ricerca del minimo
    Node<int> *min_node = albero.tree_Min(albero.root);
    if (min_node)
        output << "Il nodo con valore minimo: " << min_node->val << endl;

    // Ricerca del predecessore di un valore specifico
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

    // Esecuzione di una visita inorder per ottenere i dati ordinati
    vector<int> values;
    albero.inorderVisit(albero.root, values);
    output << "Visita in ordine: ";
    for (int v: values)
        output << v << " ";
    output << endl;

    // Stampa dell'altezza dell'albero
    output << "Altezza dell'albero: " << albero.getHeight(albero.root) << endl;

    output.close();
    cout << "File 'output.txt' creato con successo!" << endl;

    return 0;
}
