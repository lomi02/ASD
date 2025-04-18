#include <stdlib.h>
#include <climits>
#include <vector>
using namespace std;

struct Result {
    int left, right, sum;
};

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

Result findMaxCrossingSubarray(const vector<int> &A, int low, int mid, int high) {
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

    return {maxLeft, maxRight, leftSum + rightSum};
}

Result findMaximumSubarray(const vector<int> &A, int low, int high) {
    if (low == high) {
        return {low, high, A[low]};
    }
    int mid = (low + high) / 2;

    Result left = findMaximumSubarray(A, low, mid);
    Result right = findMaximumSubarray(A, mid + 1, high);
    Result cross = findMaxCrossingSubarray(A, low, mid, high);

    if (left.sum >= right.sum && left.sum >= cross.sum)
        return left;
    else if (right.sum >= left.sum && right.sum >= cross.sum)
        return right;
    else
        return cross;
}

void merge(vector<int> &arr, int left, int mid, int right) {
    vector leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j])
            arr[k++] = leftArr[i++];
        else
            arr[k++] = rightArr[j++];
    }

    while (i < leftArr.size())
        arr[k++] = leftArr[i++];

    while (j < rightArr.size())
        arr[k++] = rightArr[j++];
}

void mergeSort(vector<int> &arr, int left, int right) {
    if (left >= right) return;

    int mid = (left + right) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}

int partition(vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            swap(arr[++i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);

    return i + 1;
}

void quickSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int randomizedPartition(vector<int> &arr, int low, int high) {
    int randomPivot = low + rand() % (high - low + 1);
    swap(arr[randomPivot], arr[high]);
    return partition(arr, low, high);
}

void randomizedQuickSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = randomizedPartition(arr, low, high);
        randomizedQuickSort(arr, low, pi - 1);
        randomizedQuickSort(arr, pi + 1, high);
    }
}

void hybridSort(vector<int> &arr, int low, int high) {
    int THRESHOLD = 10;
    while (low < high) {
        if (high - low + 1 <= THRESHOLD) {
            // insertionSort(arr, low, high);
            break;
        } else {
            int pi = partition(arr, low, high);

            if (pi - low < high - pi) {
                hybridSort(arr, low, pi - 1);
                low = pi + 1;
            } else {
                hybridSort(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
}
