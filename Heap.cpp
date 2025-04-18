#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class MaxHeap {
    vector<int> heap;
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

public:
    MaxHeap(const vector<int> &array) {
        heap = array;
        buildMaxHeap();
    }

    MaxHeap() {}

    void maxHeapify(int i) {
        int l = left(i);
        int r = right(i);
        int largest = i;

        if (l < heap.size() && heap[l] > heap[largest])
            largest = l;
        if (r < heap.size() && heap[r] > heap[largest])
            largest = r;

        if (largest != i) {
            swap(heap[i], heap[largest]);
            maxHeapify(largest);
        }
    }

    void buildMaxHeap() {
        for (int i = parent(heap.size() - 1); i >= 0; i--) {
            maxHeapify(i);
        }
    }

    int heapMaximum() {
        if (!heap.empty())
            return heap[0];
        return INT_MIN;
    }

    int heapExtractMax() {
        if (heap.size() < 1) return INT_MIN;

        int max = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        maxHeapify(0);

        return max;
    }

    void heapIncreaseKey(int i, int key) {
        if (key < heap[i]) return;

        heap[i] = key;
        while (i > 0 && heap[parent(i)] < heap[i]) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void maxHeapInsert(int key) {
        heap.push_back(INT_MIN);
        heapIncreaseKey(heap.size() - 1, key);
    }

    vector<int> getHeap() {
        return heap;
    }

    void heapSort() {
        buildMaxHeap();
        for (int i = heap.size() - 1; i > 0; i--) {
            swap(heap[0], heap[i]);
            int tempSize = i;
            vector<int> tempHeap = heap;
            heap.resize(tempSize);
            maxHeapify(0);
            heap = tempHeap;
        }
    }
};
