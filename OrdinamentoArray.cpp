#include <iostream>
#include <vector>
using namespace std;

void insertionSort(vector<int> &arr) {
    int n = arr.size();

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void countingSortComment(vector<int> &arr) {
    int n = arr.size();
    if (n == 0) return;

    int minVal = arr[0], maxVal = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    int range = maxVal - minVal + 2;
    vector count(range, 0);

    for (int i = 0; i < n; i++) {
        count[arr[i] - minVal + 1] += 1;
    }

    int k = 0;
    for (int i = 0; i < count.size(); i++) {
        while (count[i] > 0) {
            arr[k] = i + minVal - 1;
            k++;
            count[i]--;
        }
    }
}