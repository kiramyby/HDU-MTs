#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Sorting algorithms
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
}

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        int min_idx = i;
        for (int j = i+1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

void insertionSort(int arr[], int n) {
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

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Utility functions
int checkSorted(int arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[i-1])
            return 0;
    return 1;
}

void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 1000000;
}

void generateNearSortedArray(int arr[], int n) {
    // Generate sorted array
    for (int i = 0; i < n; i++)
        arr[i] = i;
    // Swap some elements to make it nearly sorted
    int swaps = n / 20; // 5% disorder
    for (int i = 0; i < swaps; i++) {
        int idx1 = rand() % n;
        int idx2 = rand() % n;
        int temp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = temp;
    }
}

typedef void (*SortFunction)(int[], int);
typedef void (*SpecialSortFunction)(int[], int, int);

void runTest(const char* sortName, void* sortFunc, int isMergeQuick, int size, int isNearlySorted) {
    int* arr = (int*)malloc(size * sizeof(int));
    
    if (isNearlySorted)
        generateNearSortedArray(arr, size);
    else
        generateRandomArray(arr, size);

    clock_t start = clock();
    
    if (isMergeQuick) {
        if (strcmp(sortName, "Merge Sort") == 0)
            mergeSort(arr, 0, size - 1);
        else
            quickSort(arr, 0, size - 1);
    } else {
        ((SortFunction)sortFunc)(arr, size);
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("%s (%s) - Size %d: %.3f seconds - %s\n", 
           sortName,
           isNearlySorted ? "Nearly Sorted" : "Random",
           size,
           time_spent,
           checkSorted(arr, size) ? "Correct" : "Failed");
           
    free(arr);
}

int main() {
    srand(time(NULL));
    int sizes[] = {100, 1000, 10000, 100000, 1000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int i = 0; i < num_sizes; i++) {
        printf("\nTesting arrays of size %d\n", sizes[i]);
        printf("----------------------------------------\n");
        
        // Test with random arrays
        runTest("Bubble Sort", bubbleSort, 0, sizes[i], 0);
        runTest("Selection Sort", selectionSort, 0, sizes[i], 0);
        runTest("Insertion Sort", insertionSort, 0, sizes[i], 0);
        runTest("Merge Sort", mergeSort, 1, sizes[i], 0);
        runTest("Quick Sort", quickSort, 1, sizes[i], 0);
        
        printf("\nTesting with nearly sorted arrays\n");
        printf("----------------------------------------\n");
        
        // Test with nearly sorted arrays
        runTest("Bubble Sort", bubbleSort, 0, sizes[i], 1);
        runTest("Selection Sort", selectionSort, 0, sizes[i], 1);
        runTest("Insertion Sort", insertionSort, 0, sizes[i], 1);
        runTest("Merge Sort", mergeSort, 1, sizes[i], 1);
        runTest("Quick Sort", quickSort, 1, sizes[i], 1);
    }
    
    return 0;
}
