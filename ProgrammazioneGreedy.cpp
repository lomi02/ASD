#include <iostream>
#include <queue>
#include <unordered_map>
using namespace std;

// Nodo dell'albero di Huffman
struct Nodo {
    char carattere; // Carattere (se foglia)
    int frequenza; // Frequenza del carattere
    Nodo *sinistra, *destra; // Figli sinistro e destro

    Nodo(char c, int f) : carattere(c), frequenza(f), sinistra(nullptr), destra(nullptr) {
    }
};

// Comparatore per la coda di priorità (min-heap)
struct Compare {
    bool operator()(Nodo *a, Nodo *b) {
        return a->frequenza > b->frequenza; // Min-heap: minore frequenza ha priorità
    }
};

// Funzione ricorsiva per costruire la tabella di codifica
void buildCode(Nodo *root, string codice, unordered_map<char, string> &codifica) {
    if (!root) return;

    // Se è una foglia, salva il codice
    if (!root->sinistra && !root->destra) {
        codifica[root->carattere] = codice;
    }

    // Ricorsione a sinistra e destra
    buildCode(root->sinistra, codice + "0", codifica);
    buildCode(root->destra, codice + "1", codifica);
}

// Costruisce l'albero di Huffman dato un dizionario di frequenze
Nodo *buildHuffman(unordered_map<char, int> &frequenze) {
    priority_queue<Nodo *, vector<Nodo *>, Compare> heap;

    // Inserisci ogni carattere nella heap
    for (auto &entry: frequenze) {
        heap.push(new Nodo(entry.first, entry.second));
    }

    // Finché resta più di un nodo
    while (heap.size() > 1) {
        Nodo *sin = heap.top();
        heap.pop(); // nodo con freq minore
        Nodo *des = heap.top();
        heap.pop(); // secondo minore

        // Crea un nuovo nodo interno con freq somma
        Nodo *interno = new Nodo('\0', sin->frequenza + des->frequenza);
        interno->sinistra = sin;
        interno->destra = des;

        heap.push(interno); // Inserisci nuovo nodo nella heap
    }

    return heap.top(); // Radice dell'albero
}

// Codifica la stringa originale
string codeText(string testo, unordered_map<char, string> &codifica) {
    string risultato = "";
    for (char c: testo) {
        risultato += codifica[c];
    }
    return risultato;
}

// Decodifica il testo codificato usando l'albero di Huffman
string decodeText(string testoCodificato, Nodo *root) {
    string risultato = "";
    Nodo *corrente = root;

    for (char bit: testoCodificato) {

        // Vai a sinistra per 0, destra per 1
        if (bit == '0') corrente = corrente->sinistra;
        else corrente = corrente->destra;

        // Se hai raggiunto una foglia, aggiungi il carattere
        if (!corrente->sinistra && !corrente->destra) {
            risultato += corrente->carattere;
            corrente = root; // Riparti dalla radice
        }
    }

    return risultato;
}

int main() {
    string testo = "Esempio Huffman";

    // Calcolo frequenze
    unordered_map<char, int> frequenze;
    for (char c: testo) {
        frequenze[c]++;
    }

    // Costruzione albero e codici
    Nodo *radice = buildHuffman(frequenze);
    unordered_map<char, string> codifica;
    buildCode(radice, "", codifica);

    // Mostra codici
    cout << "Codici di Huffman:\n";
    for (auto &p: codifica) {
        cout << p.first << " : " << p.second << endl;
    }

    string testoCodificato = codeText(testo, codifica);
    cout << "\nTesto codificato: " << testoCodificato << endl;

    string testoDecodificato = decodeText(testoCodificato, radice);
    cout << "Testo decodificato: " << testoDecodificato << endl;

    return 0;
}
