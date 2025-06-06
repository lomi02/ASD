#include <iostream>
#include <climits>
#include <vector>
using namespace std;

struct Result {
    int left, right, sum;
};

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

// [Ordinamento Divide and Conquer] Algoritmo della Ricerca Binaria
int binarySearch(vector<int> &A, int low, int high, int x) {
    if (low > high)
        return -1;

    int mid = (low + high) / 2;

    if (A[mid] == x)
        return mid;
    else if (A[mid] > x)
        return binarySearch(A, low, mid - 1, x);
    else
        return binarySearch(A, mid + 1, high, x);
}

// [Ordinamento Divide and Conquer] Algoritmo del Find-Max-Crossing-Subarray
// Trova il massimo sottoarray che attraversa il punto centrale (mid)
Result findMaxCrossingSubarray(const vector<int> &A, int low, int mid, int high) {
    // Esploriamo la parte sinistra, da mid verso low
    int leftSum = INT_MIN;
    int sum = 0;
    int maxLeft = mid;
    for (int i = mid; i >= low; i--) {
        sum += A[i];
        if (sum > leftSum) {
            leftSum = sum;
            maxLeft = i;
        }
    }

    // Esploriamo la parte destra, da mid+1 verso high
    int rightSum = INT_MIN;
    sum = 0;
    int maxRight = mid + 1;
    for (int j = mid + 1; j <= high; j++) {
        sum += A[j];
        if (sum > rightSum) {
            rightSum = sum;
            maxRight = j;
        }
    }

    // Ritorniamo la somma combinata e gli estremi del sottoarray che attraversa il centro
    return {maxLeft, maxRight, leftSum + rightSum};
}

// [Ordinamento Divide and Conquer] Algoritmo del Find-Maximum-Subarray
// Algoritmo principale Divide et Impera per trovare il massimo sottoarray
Result findMaximumSubarray(const vector<int> &A, int low, int high) {
    // Caso base: il sottoarray ha un solo elemento
    if (low == high)
        return {low, high, A[low]};

    // Calcoliamo il punto centrale
    int mid = (low + high) / 2;

    // Troviamo massimo sottoarray nella parte sinistra
    Result left = findMaximumSubarray(A, low, mid);

    // Troviamo massimo sottoarray nella parte destra
    Result right = findMaximumSubarray(A, mid + 1, high);

    // Troviamo massimo sottoarray che attraversa il centro
    Result cross = findMaxCrossingSubarray(A, low, mid, high);

    // Ritorniamo il sottoarray con la somma massima tra i tre
    if (left.sum >= right.sum && left.sum >= cross.sum)
        return left;
    else if (right.sum >= left.sum && right.sum >= cross.sum)
        return right;
    else
        return cross;
}

// Funzione di supporto MergeSort
void merge(vector<int> &arr, int left, int mid, int right) {

    // Creo i due sottoarray temporanei, sinistra e destra
    vector leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    // Indici per navigare nei due sottoarray
    int i = 0, j = 0, k = left;

    // Confronto gli elementi e copio il più piccolo nell'array principale
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j])
            arr[k++] = leftArr[i++];
        else
            arr[k++] = rightArr[j++];
    }

    // Copia eventuali rimanenti della parte sinistra
    while (i < leftArr.size())
        arr[k++] = leftArr[i++];

    // Copia eventuali rimanenti della parte destra
    while (j < rightArr.size())
        arr[k++] = rightArr[j++];
}

// [Ordinamento Divide and Conquer] Algoritmo del MergeSort
// Complessità: θ(n log n)
void mergeSort(vector<int> &arr, int left, int right) {

    // Caso base: se la parte ha un solo elemento, è già ordinata
    if (left >= right) return;

    // DIVIDE: Calcolo punto centrale
    int mid = (left + right) / 2;

    // IMPERA: Ordina ricorsivamente la parte sinistra e destra
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // MERGE: Combina le due parti ordinate
    merge(arr, left, mid, right);
}

// Funzione di supporto QuickSort
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];  // Pivot scelto come ultimo elemento
    int i = low - 1;        // i tiene traccia della "zona minore del pivot"

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {

            // Se l'elemento è <= pivot, lo spostiamo a sinistra
            swap(arr[++i], arr[j]);
        }
    }

    // Mettiamo il pivot nella sua posizione finale ordinata
    swap(arr[i + 1], arr[high]);

    // Ritorna la posizione del pivot
    return i + 1;
}

// [Ordinamento Divide and Conquer] Algoritmo del QuickSort
// Caso Favorevole: θ(n log n)
// Caso Medio: θ(n log n)
// Caso Sfavorevole: θ(n^2)
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // Divisione in due parti
        quickSort(arr, low, pi - 1);        // Ricorsione sulla parte sinistra
        quickSort(arr, pi + 1, high);       // Ricorsione sulla parte destra
    }
}

// Funzione di supporto Randomized QuickSort
int randomizedPartition(vector<int>& arr, int low, int high) {

    // Scelgo un pivot casuale tra low e high
    int randomPivot = low + rand() % (high - low + 1);
    swap(arr[randomPivot], arr[high]); // Metto il pivot alla fine
    return partition(arr, low, high);  // Procedo come al solito
}

// [Ordinamento Divide and Conquer] Algoritmo del Randomized QuickSort
// Caso Favorevole: θ(n log n)
// Caso Medio: θ(n log n)
// Caso Sfavorevole: θ(n^2)
void randomizedQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {

        // Partizione casuale riduce il rischio del caso peggiore
        int pi = randomizedPartition(arr, low, high);
        randomizedQuickSort(arr, low, pi - 1);
        randomizedQuickSort(arr, pi + 1, high);
    }
}

// [Ordinamento Divide and Conquer] Algoritmo dell'HybridSort
void hybridSort(vector<int>& arr, int low, int high) {
    int THRESHOLD = 10; // Parametro di scelta
    while (low < high) {

        // Se la dimensione è piccola, uso InsertionSort
        if (high - low + 1 <= THRESHOLD) {

            // Qui va utilizzato l'InsertionSort, ma non riscrivo la funzione.
            // insertionSort(arr, low, high);
            break;
        } else {

            // Altrimenti procedo con QuickSort
            int pi = partition(arr, low, high);

            // Ottimizzazione tail-recursion (richiamo la parte più piccola per prima)
            if (pi - low < high - pi) {
                hybridSort(arr, low, pi - 1);
                low = pi + 1; // Ricorsione di destra è trasformata in ciclo
            } else {
                hybridSort(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
}

int main() {
    vector A = InputArray();
    cout << "\nArray in input: " << endl;
    printArray(A);

    // Chiamiamo la funzione per trovare il massimo sottoarray
    Result result = findMaximumSubarray(A, 0, A.size() - 1);

    // Stampiamo il risultato
    cout << "Massimo sottoarray trovato da indice " << result.left
            << " a indice " << result.right
            << " con somma massima: " << result.sum << endl;

    return 0;
}
