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

Result findMaximumSubarray(const vector<int>& A, int low, int high) {
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