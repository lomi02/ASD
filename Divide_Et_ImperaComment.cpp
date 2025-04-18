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

// [Ordinamento Divide and Conquer] Algoritmo della Ricerca Binaria
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
