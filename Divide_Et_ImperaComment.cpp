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