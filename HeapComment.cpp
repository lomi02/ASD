#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class MaxHeap {
    vector<int> heap;

    // Restituisce l'indice del padre
    int parent(int i) { return (i - 1) / 2; }

    // Restituisce l'indice del figlio sinistro
    int left(int i) { return 2 * i + 1; }

    // Restituisce l'indice del figlio destro
    int right(int i) { return 2 * i + 2; }

public:

    // Costruttore che crea un heap da un array dato
    MaxHeap(const vector<int> &array) {
        heap = array;
        buildMaxHeap();
    }

    // Costruttore vuoto
    MaxHeap() {}

    // Mantiene la proprietà di Max-Heap dalla posizione i in giù
    void maxHeapify(int i) {
        int l = left(i);
        int r = right(i);
        int largest = i;

        // Trova il maggiore tra il nodo corrente e i suoi figli
        if (l < heap.size() && heap[l] > heap[largest])
            largest = l;
        if (r < heap.size() && heap[r] > heap[largest])
            largest = r;

        // Se un figlio è più grande, scambia e continua ricorsivamente
        if (largest != i) {
            swap(heap[i], heap[largest]);
            maxHeapify(largest);
        }
    }

    // Costruisce il Max-Heap partendo da un array
    void buildMaxHeap() {
        for (int i = parent(heap.size() - 1); i >= 0; i--) {
            maxHeapify(i);
        }
    }

    // Restituisce il massimo (radice)
    int heapMaximum() {
        if (!heap.empty())
            return heap[0];
        return INT_MIN;
    }

    // Estrae il massimo e ripristina la proprietà di heap
    int heapExtractMax() {
        if (heap.size() < 1) return INT_MIN;

        int max = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        maxHeapify(0);

        return max;
    }

    // Aumenta la chiave di un nodo in posizione i
    void heapIncreaseKey(int i, int key) {
        if (key < heap[i]) return;

        heap[i] = key;
        while (i > 0 && heap[parent(i)] < heap[i]) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    // Inserisce un nuovo elemento nella coda di priorità
    void maxHeapInsert(int key) {
        heap.push_back(INT_MIN); // aggiunge -∞
        heapIncreaseKey(heap.size() - 1, key); // aumenta al valore desiderato
    }

    // Restituisce l'heap come vettore
    vector<int> getHeap() {
        return heap;
    }

    // Ordina l'heap utilizzando HeapSort (in-place)
    void heapSort() {
        buildMaxHeap();
        for (int i = heap.size() - 1; i > 0; i--) {
            swap(heap[0], heap[i]);

            // Temporaneamente riduciamo la dimensione dell'heap
            int tempSize = i;
            vector<int> tempHeap = heap;
            heap.resize(tempSize);
            maxHeapify(0);
            heap = tempHeap;
        }
    }
};

int main() {
    vector array = {4, 10, 3, 5, 1};
    MaxHeap h(array);

    cout << "Max: " << h.heapMaximum() << endl;
    h.maxHeapInsert(15);
    cout << "Max dopo inserimento: " << h.heapMaximum() << endl;
    h.heapExtractMax();
    cout << "Max dopo estrazione: " << h.heapMaximum() << endl;

    h.heapSort();
    vector<int> sorted = h.getHeap();
    cout << "Heap ordinato: ";
    for (int i: sorted) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
