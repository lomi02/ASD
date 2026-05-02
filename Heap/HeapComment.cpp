#include <iostream>
#include <vector>

using namespace std;

/**
 * Classe Heap: Implementa un Max-Heap (Heap Binario).
 * Fornisce metodi per la costruzione, l'inserimento, l'estrazione del massimo
 * e il mantenimento delle proprietà dell'heap.
 */
class Heap {
public:
    vector<int> data;   // Vettore dinamico che memorizza gli elementi dell'heap
    int heapSize;       // Numero corrente di elementi validi nell'heap

    /**
     * Costruttore: Inizializza l'heap a partire da un vettore di valori.
     * @param values Vettore di interi da trasformare in heap.
     */
    Heap(vector<int> &values) {
        data = values;
        heapSize = data.size();
        buildHeap(); // Trasforma il vettore disordinato in un heap
    }

    /**
     * buildHeap: Converte il vettore 'data' in un Max-Heap.
     * Itera dai nodi interni (partendo dall'ultimo genitore) verso la radice.
     */
    void buildHeap() {

        // L'ultimo nodo genitore si trova all'indice (heapSize/2) - 1
        for (int i = (heapSize / 2) - 1; i >= 0; i--)
            heapify(i);
    }

    /**
     * Heapify: Ripristina la proprietà di Max-Heap per il sotto-albero con radice in i.
     * @param i Indice del nodo da cui far partire il controllo.
     */
    void heapify(int i) {
        int largest = i;        // Inizializza il più grande come radice
        int left = 2 * i + 1;   // Indice del figlio sinistro
        int right = 2 * i + 2;  // Indice del figlio destro

        // Se il figlio sinistro esiste ed è maggiore della radice corrente
        if (left < heapSize && data[left] > data[largest])
            largest = left;

        // Se il figlio destro esiste ed è maggiore del più grande trovato finora
        if (right < heapSize && data[right] > data[largest])
            largest = right;

        // Se il valore più grande non è la radice, scambia e continua ricorsivamente
        if (largest != i) {
            swap(data[i], data[largest]);
            heapify(largest); // Ricorsione sul sotto-albero interessato dallo scambio
        }
    }

    /**
     * getMax: Restituisce l'elemento massimo senza rimuoverlo.
     * @return Il valore alla radice dell'heap (data[0]).
     */
    int getMax() {
        return data[0];
    }

    /**
     * extractMax: Rimuove e restituisce l'elemento massimo.
     * Sostituisce la radice con l'ultimo elemento e ripristina l'heap.
     * @return Il valore massimo rimosso.
     */
    int extractMax() {
        if (heapSize < 1) return -1; // Protezione minima per heap vuoto

        int max = data[0];              // Salva il massimo (radice)
        data[0] = data[heapSize - 1];   // Sposta l'ultimo elemento in radice
        heapSize--;                     // Riduce la dimensione logica dell'heap

        data.pop_back(); // Rimuove fisicamente l'elemento dal vettore

        heapify(0); // Ripristina l'heap partendo dalla radice
        return max;
    }

    /**
     * Insert: Inserisce una nuova chiave nell'heap.
     * Aggiunge l'elemento in coda e lo sposta verso l'alto (bubble-up) finché necessario.
     * @param key Valore da inserire.
     */
    void insert(int key) {
        data.push_back(key);        // Aggiunge l'elemento in fondo al vettore
        heapSize = data.size();     // Aggiorna la dimensione

        int i = heapSize - 1; // Indice del nuovo elemento

        // Finché non siamo alla radice e il genitore è minore del nodo corrente
        while (i > 0 && data[(i - 1) / 2] < data[i]) {

            swap(data[i], data[(i - 1) / 2]);   // Scambia con il genitore
            i = (i - 1) / 2;                    // Risale l'albero verso il genitore
        }
    }
};

/**
 * Funzione Main:
 * Punto di ingresso del programma per testare le funzionalità dell'Heap.
 */
int main() {

    // Definizione di un vettore di test disordinato
    vector<int> values = {10, 20, 15, 17, 25};

    // Istanza dell'oggetto Heap (avvia buildHeap internamente)
    Heap heap(values);

    cout << "Massimo: " << heap.getMax() << endl;

    // Test estrazione del massimo
    cout << "Estrazione massimo: " << heap.extractMax() << endl;
    cout << "Nuovo massimo dopo estrazione: " << heap.getMax() << endl;

    // Test inserimento nuovo elemento
    heap.insert(30);
    cout << "Nuovo massimo dopo inserimento 30: " << heap.getMax() << endl;

    return 0;
}
