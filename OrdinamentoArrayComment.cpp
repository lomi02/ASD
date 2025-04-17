#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> InputArray() {

    // Inserimento dimensione array
    int size;
    cout << "\nDimensione array: ";
    cin >> size;

    // Compilazione array
    vector<int> array(size);
    for (int i = 0; i < size; ++i) {
        cout << "Inserisci il " << (i + 1) << "° elemento: ";
        cin >> array[i];
    }

    return array;
}

void printArray(const vector<int> &arr) {
    for (int num: arr) {
        cout << num << " ";
    }
    cout << "\n" << endl;
}

// [Ordinamento di Confronto] Algoritmo del Mazziere
// Caso Favorevole: θ(n)
// Caso Medio: θ(n^2)
// Caso Sfavorevole: θ(n^2)
void insertionSortComment(vector<int> &arr) {
    int n = arr.size();

    // Scorriamo tutti gli elementi a partire dal secondo
    for (int i = 1; i < n; i++) {
        int key = arr[i]; // L'elemento da inserire nella parte ordinata
        int j = i - 1;
        printArray(arr);
        cout << "Pre-Ordinamento arr[i]: " << arr[i] << endl;
        cout << "Pre-Ordinamento arr[j]: " << arr[j] << endl;

        // Spostiamo gli elementi della parte ordinata che sono maggiori di key verso destra per fare spazio alla key
        while (j >= 0 && arr[j] > key) {
            // Sposta l'elemento una posizione a destra
            arr[j + 1] = arr[j];
            j--;
        }
        cout << "Post-Ordinamento arr[i]: " << arr[i] << endl;
        cout << "Post-Ordinamento arr[j]: " << arr[j] << endl;
        cout << "Chiave conservata: " << key << ". Posiziono la chiave nella posizione di [j + 1]" << endl;
        printArray(arr);
        cout << "-----------------------------" << endl;

        // Inseriamo la key nella posizione corretta
        arr[j + 1] = key;
    }
}

// [Ordinamento di Tempo Lineare] Algoritmo del Counting Sort
// Complessità: O(n)
void countingSortComment(vector<int> &arr) {
    int n = arr.size();
    if (n == 0) return;

    // Trova il minimo e il massimo nell'array
    int minVal = arr[0], maxVal = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    // Calcola il range e inizializza il vettore di conteggio
    int range = maxVal - minVal + 2; // +2 perché usiamo l'indice (val - min + 1)
    vector count(range, 0);

    // Conta le occorrenze usando la formula data
    for (int i = 0; i < n; i++) {
        count[arr[i] - minVal + 1] += 1;
    }

    // Ricostruiamo l'array ordinato
    int k = 0;
    for (int i = 0; i < count.size(); i++) {
        while (count[i] > 0) {
            arr[k] = i + minVal - 1; // Invertiamo la trasformazione dell'indice
            k++;
            count[i]--;
        }

        // Debug: stampa stato
        cout << "\nPosizione indice [i]: " << i << endl;
        cout << "Stato array da ordinare:" << endl;
        printArray(arr);
        cout << "Stato array dei conti:" << endl;
        printArray(count);
        cout << "-----------------------------" << endl;
    }
}

// TODO: Counting sort di Cormen

// [Ordinamento di Tempo Lineare] Algoritmo del Radix Sort
// Complessità: Dipende dall'algoritmo di ordinamento scelto
void radixSort(vector<int>& arr) {
    int n = arr.size();

    // Troviamo il numero massimo per sapere il numero di cifre
    int maxVal = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    // Applichiamo counting sort per ogni cifra (unità, decine, centinaia...)
    for (int exp = 1; maxVal / exp > 0; exp= 10) {

        // Si possono utilizzare tutti gli algoritmi di ordinamento affinché viene
        // passato in input anche l'unità del valore.
        //countingSortComment(arr, exp);
    }
}

// [Ordinamento di Tempo Lineare] Algoritmo del Bucket Sort
// Complessità: Dipende dall'algoritmo di ordinamento scelto
void bucketSort(vector<float>& arr) {
    int n = arr.size();

    // Creiamo n bucket vuoti
    vector<vector<float>> buckets(n);

    // Inseriamo ogni elemento nel bucket appropriato
    for (int i = 0; i < n; i++) {
        int index = n * arr[i]; // Calcola l'indice del bucket
        buckets[index].push_back(arr[i]);
    }

    // Ordiniamo i singoli bucket (possiamo usare sort di STL)
    for (int i = 0; i < n; i++) {

        // Qui si usa il sorting della libreria algorithm, però
        // idealmente va utilizzato un algoritmo di sorting di quelli studiati.
        ranges::sort(buckets[i]);
    }

    // Uniamo tutti i bucket nell'array originale
    int index = 0;
    for (int i = 0; i < n; i++) {
        for (float num : buckets[i]) {
            arr[index++] = num;
        }
    }
}

// TODO: Bucket sort di Cormen

int main() {
    vector array = InputArray();

    cout << "\nArray prima dell'ordinamento: " << endl;
    printArray(array);
    cout << "-----------------------------" << endl;

    insertionSortComment(array);

    cout << "\nArray dopo l'ordinamento: " << endl;
    printArray(array);

    return 0;
}
