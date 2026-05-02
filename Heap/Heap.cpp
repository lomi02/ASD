#include <iostream>
#include <vector>

using namespace std;

class Heap {
public:
    vector<int> data;
    int heapSize;

    Heap(vector<int> &values) {
        data = values;
        heapSize = data.size();
        buildHeap();
    }

    void buildHeap() {
        for (int i = (heapSize/2) - 1; i >= 0; i--)
            heapify(i);
    }

    void heapify(int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < heapSize && data[left] > data[largest])
            largest = left;
        if (right < heapSize && data[right] > data[largest])
            largest = right;

        if (largest != i) {
            swap(data[i], data[largest]);
            heapify(largest);
        }
    }

    int getMax() {
        return data[0];
    }

    int extractMax() {
        int max = data[0];
        data[0] = data[heapSize - 1];
        heapSize--;

        data.pop_back();

        heapify(0);
        return max;
    }

    void insert(int key) {
        data.push_back(key);
        heapSize = data.size();

        int i = heapSize - 1;
        while (i > 0 && data[(i - 1) / 2] < data[i]) {
            swap(data[i], data[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
};

int main() {
    vector<int> values = {10, 20, 15, 17, 25};
    Heap heap(values);

    cout << "Massimo: " << heap.getMax() << endl;
    cout << "Estrazione massimo: " << heap.extractMax() << endl;
    cout << "Nuovo massimo dopo estrazione: " << heap.getMax() << endl;

    heap.insert(30);
    cout << "Nuovo massimo dopo inserimento 30: " << heap.getMax() << endl;

    return 0;
}
