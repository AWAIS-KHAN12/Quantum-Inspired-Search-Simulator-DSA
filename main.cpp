#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>

using namespace std;

const int MAX = 1000;
const double PI = 3.141592653589793;

/* ================= SAFE INPUT ================= */

int getValidInt(bool allowNegative = false) {
    int x;
    while (true) {
        cin >> x;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "❌ Invalid input! Enter a number: ";
        }
        else if (!allowNegative && x < 0) {
            cout << "❌ Negative not allowed. Enter again: ";
        }
        else {
            return x;
        }
    }
}

void clearExtraInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/* ================= PRIORITY QUEUE (MAX HEAP) ================= */

struct HeapNode {
    double probability;
    int index;
};

HeapNode heap[MAX];
int heapSize = 0;

void swapNodes(HeapNode &a, HeapNode &b) {
    HeapNode temp = a;
    a = b;
    b = temp;
}

void heapifyUp(int i) {
    while (i > 0 && heap[(i - 1) / 2].probability < heap[i].probability) {
        swapNodes(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void heapifyDown(int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heapSize && heap[left].probability > heap[largest].probability)
        largest = left;
    if (right < heapSize && heap[right].probability > heap[largest].probability)
        largest = right;

    if (largest != i) {
        swapNodes(heap[i], heap[largest]);
        heapifyDown(largest);
    }
}

void insertHeap(double probability, int index) {
    heap[heapSize].probability = probability;
    heap[heapSize].index = index;
    heapSize++;
    heapifyUp(heapSize - 1);
}

HeapNode extractMax() {
    HeapNode root = heap[0];
    heap[0] = heap[--heapSize];
    heapifyDown(0);
    return root;
}

/* ================= SEARCH ALGORITHMS ================= */

int linearSearch(int arr[], int n, int target, int &steps) {
    steps = 0;
    for (int i = 0; i < n; i++) {
        steps++;
        if (arr[i] == target)
            return i;
    }
    return -1;
}

int binarySearch(int arr[], int n, int target, int &steps) {
    int low = 0, high = n - 1;
    steps = 0;

    while (low <= high) {
        steps++;
        int mid = low + (high - low) / 2;
        if (arr[mid] == target)
            return mid;
        else if (arr[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

/* ================= QUANTUM SEARCH ================= */

int quantumSearch(int arr[], int n, int target, int &iterations) {
    double amplitudes[MAX];
    double probabilities[MAX];

    int targetIndex = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            targetIndex = i;
            break;
        }
    }

    iterations = (int)((PI / 4) * sqrt(n));

    if (targetIndex == -1)
        return -1;

    double initAmp = 1.0 / sqrt(n);
    for (int i = 0; i < n; i++)
        amplitudes[i] = initAmp;

    for (int iter = 0; iter < iterations; iter++) {
        amplitudes[targetIndex] *= -1;

        double mean = 0.0;
        for (int i = 0; i < n; i++)
            mean += amplitudes[i];
        mean /= n;

        for (int i = 0; i < n; i++)
            amplitudes[i] = 2 * mean - amplitudes[i];
    }

    heapSize = 0;
    for (int i = 0; i < n; i++) {
        probabilities[i] = amplitudes[i] * amplitudes[i];
        insertHeap(probabilities[i], i);
    }

    return extractMax().index;
}

/* ================= MAIN ================= */

int main() {
    srand(time(0));

    cout << "===== Quantum-Inspired Search Simulator =====\n";

    cout << "Enter number of elements (1 - 1000): ";
    int n = getValidInt();

    while (n <= 0 || n > MAX) {
        cout << "❌ Enter value between 1 and 1000: ";
        n = getValidInt();
    }

    int arr[MAX];

    cout << "Enter sorted elements:\n";
    for (int i = 0; i < n; i++) {
        while (true) {
            cout << "Element [" << i << "]: ";
            cin >> arr[i];

            if (cin.fail()) {
                cin.clear();
                clearExtraInput();
                cout << "❌ Invalid input! Enter number only.\n";
            }
            else if (i > 0 && arr[i] < arr[i - 1]) {
                cout << "❌ Array must be sorted in ascending order.\n";
            }
            else {
                break;
            }
        }
    }

    clearExtraInput(); // Ignore extra accidental input

    cout << "Enter target value: ";
    int target = getValidInt(true);

    int stepsL, stepsB, quantumIters;

    int lin = linearSearch(arr, n, target, stepsL);
    int bin = binarySearch(arr, n, target, stepsB);
    int q   = quantumSearch(arr, n, target, quantumIters);

    cout << "\n----- RESULTS -----\n";

    if (lin == -1) {
        cout << "❌ Target not found in array.\n";
    } else {
        cout << "Linear Search Index: " << lin << " | Steps: " << stepsL << endl;
        cout << "Binary Search Index: " << bin << " | Steps: " << stepsB << endl;
        cout << "Quantum Search Index (Max Probability): " << q
             << " | Iterations: " << quantumIters << endl;
    }

    return 0;
}
