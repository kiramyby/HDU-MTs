#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <math.h> 

// 函数声明部分
// 排序算法函数
void bubbleSort(int arr[], int n);
void selectionSort(int arr[], int n);
void insertionSort(int arr[], int n);
void mergeSort(int arr[], int l, int r);
void quickSort(int arr[], int low, int high);
void countingSort(int arr[], int n);
void bucketSort(int arr[], int n);
void radixSort(int arr[], int n);
// 工具函数
void handle_timeout(int sig);
void merge(int arr[], int l, int m, int r);
int partition(int arr[], int low, int high);
int getDigit(int num, int exp);
int checkSorted(int arr[], int n);
void generateRandomArray(int arr[], int n);
void generateNearSortedArray(int arr[], int n);
void runTest(const char* sortName, void* sortFunc, int isMergeQuick, int size, int isNearlySorted);

// 类型定义
typedef void (*SortFunction)(int[], int);
typedef void (*SpecialSortFunction)(int[], int, int);

// 超时标记
volatile int sort_timeout = 0;

// 冒泡排序
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        if (sort_timeout) return; 
        for (int j = 0; j < n-i-1; j++) {
            if (sort_timeout) return;
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// 选择排序
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        if (sort_timeout) return;
        int min_idx = i;
        for (int j = i+1; j < n; j++) {
            if (sort_timeout) return;  
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

// 插入排序
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (sort_timeout) return;  
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            if (sort_timeout) return;  
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 归并排序
void mergeSort(int arr[], int l, int r) {
    if (sort_timeout) return;  
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// 快速排序
void quickSort(int arr[], int low, int high) {
    if (sort_timeout) return;  
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 计数排序
void countingSort(int arr[], int n) {
    if (n <= 0) return;
    
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (sort_timeout) return;  
        if (arr[i] > max)
            max = arr[i];
    }
    
    int* count = (int*)calloc(max + 1, sizeof(int));
    if (count == NULL) return;
    
    for (int i = 0; i < n; i++) {
        if (sort_timeout) {
            free(count);
            return;  
        }
        count[arr[i]]++;
    }
    
    int index = 0;
    for (int i = 0; i <= max; i++) {
        if (sort_timeout) {
            free(count);
            return;
        }
        while (count[i] > 0) {
            if (sort_timeout) {
                free(count);
                return;  
            }
            arr[index++] = i;
            count[i]--;
        }
    }
    
    free(count);
}

// 桶排序
void bucketSort(int arr[], int n) {
    if (n <= 0) return;

    int max = arr[0], min = arr[0];
    for (int i = 1; i < n; i++) {
        if (sort_timeout) return;
        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
    }
    
    int bucketCount = (int)sqrt(max - min + 1);
    if (bucketCount < 1) bucketCount = 1;
    
    typedef struct Node {
        int data;
        struct Node* next;
    } Node;
    
    Node** buckets = (Node**)calloc(bucketCount, sizeof(Node*));
    if (buckets == NULL) return;
    
    for (int i = 0; i < n; i++) {
        if (sort_timeout) {
            for (int j = 0; j < bucketCount; j++) {
                Node* current = buckets[j];
                while (current) {
                    Node* temp = current;
                    current = current->next;
                    free(temp);
                }
            }
            free(buckets);
            return; 
        }
        
        int bucketIndex = (arr[i] - min) / ((max - min) / bucketCount + 1);
        
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) {

            for (int j = 0; j < bucketCount; j++) {
                Node* current = buckets[j];
                while (current) {
                    Node* temp = current;
                    current = current->next;
                    free(temp);
                }
            }
            free(buckets);
            return;
        }
        
        newNode->data = arr[i];
        newNode->next = NULL;
        
        if (buckets[bucketIndex] == NULL) {
            buckets[bucketIndex] = newNode;
        } else {
            if (buckets[bucketIndex]->data > arr[i]) {
                newNode->next = buckets[bucketIndex];
                buckets[bucketIndex] = newNode;
            } else {
                Node* current = buckets[bucketIndex];
                while (current->next && current->next->data < arr[i]) {
                    current = current->next;
                }
                newNode->next = current->next;
                current->next = newNode;
            }
        }
    }
    
    int index = 0;
    for (int i = 0; i < bucketCount; i++) {
        Node* current = buckets[i];
        while (current) {
            if (sort_timeout) {
                for (int j = i; j < bucketCount; j++) {
                    Node* temp = buckets[j];
                    while (temp) {
                        Node* toFree = temp;
                        temp = temp->next;
                        free(toFree);
                    }
                }
                free(buckets);
                return;
            }
            
            arr[index++] = current->data;
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(buckets);
}

// 基数排序
void radixSort(int arr[], int n) {
    if (n <= 0) return;
    
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (sort_timeout) return;
        if (arr[i] > max)
            max = arr[i];
    }
    
    for (int exp = 1; max / exp > 0; exp *= 10) {
        if (sort_timeout) return;
        
        int* output = (int*)malloc(n * sizeof(int));
        if (output == NULL) return;
        
        int count[10] = {0};
        
        for (int i = 0; i < n; i++) {
            if (sort_timeout) {
                free(output);
                return;
            }
            count[getDigit(arr[i], exp)]++;
        }
        
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            if (sort_timeout) {
                free(output);
                return;
            }
            output[count[getDigit(arr[i], exp)] - 1] = arr[i];
            count[getDigit(arr[i], exp)]--;
        }
        
        for (int i = 0; i < n; i++) {
            if (sort_timeout) {
                free(output);
                return;
            }
            arr[i] = output[i];
        }
        
        free(output);
    }
}

// 工具函数
// 归并函数(用于归并排序)
void merge(int arr[], int l, int m, int r) {
    if (sort_timeout) return;  
    
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
        if (sort_timeout) {
            free(L);
            free(R);
            return;  
        }
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    free(L);
    free(R);
}

// 分区函数(用于快速排序)
int partition(int arr[], int low, int high) {
    if (sort_timeout) return low;  
    
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (sort_timeout) return low;  
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

// 获取数字的指定位的值（用于基数排序）
int getDigit(int num, int exp) {
    return (num / exp) % 10;
}

// 超时处理函数
void handle_timeout(int sig) {
    sort_timeout = 1;
    printf("排序超时(>300秒)，已终止当前排序\n");
}

// 检查数组是否已排序
// 返回 1 表示已排序，0 表示未排序
int checkSorted(int arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[i-1])
            return 0;
    return 1;
}

// 生成随机数组
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 1000000;
}

// 生成几乎有序数组
void generateNearSortedArray(int arr[], int n) {
    // 生成已排序数组
    for (int i = 0; i < n; i++)
        arr[i] = i;
    // 交换一些元素，使其几乎有序
    int swaps = n / 20;
    for (int i = 0; i < swaps; i++) {
        int idx1 = rand() % n;
        int idx2 = rand() % n;
        int temp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = temp;
    }
}

void runTest(const char* sortName, void* sortFunc, int isMergeQuick, int size, int isNearlySorted) {
    int* arr = (int*)malloc(size * sizeof(int));
    
    if (isNearlySorted)
        generateNearSortedArray(arr, size);
    else
        generateRandomArray(arr, size);
    
    // 重置超时标志
    sort_timeout = 0;

    // 设置定时器
    alarm(300);
    clock_t start = clock();
    
    // 调用排序函数
    if (isMergeQuick) {
        ((SpecialSortFunction)sortFunc)(arr, 0, size - 1);
    } else {
        ((SortFunction)sortFunc)(arr, size);
    }
    
    // 取消定时器
    alarm(0);
    
    clock_t end = clock();
    
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    if (sort_timeout) {
        printf("%s (%s) - 大小 %d: 超时 - 排序未完成\n", 
                sortName,
                isNearlySorted ? "几乎有序" : "随机",
                size);
    } else {
        printf("%s (%s) - 大小 %d: %.3f 秒 - %s\n", 
                sortName,
                isNearlySorted ? "几乎有序" : "随机",
                size,
                time_spent,
                checkSorted(arr, size) ? "正确" : "失败");
    }
           
    free(arr);
}

int main() {
    srand(time(NULL));
    int sizes[] = {100, 1000, 10000, 100000, 1000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    // 设置超时处理
    signal(SIGALRM, handle_timeout);
    
    for (int i = 0; i < num_sizes; i++) {
        printf("\n*************************************************************\n");
        printf("                 测试大小为 %d 的数组 ", sizes[i]);
        printf("\n*************************************************************");
        
        printf("\n-------- 测试随机数组 --------\n");
        runTest("冒泡排序", bubbleSort, 0, sizes[i], 0);
        runTest("选择排序", selectionSort, 0, sizes[i], 0);
        runTest("插入排序", insertionSort, 0, sizes[i], 0);
        runTest("归并排序", mergeSort, 1, sizes[i], 0);
        runTest("快速排序", quickSort, 1, sizes[i], 0);
        runTest("计数排序", countingSort, 0, sizes[i], 0);
        runTest("桶排序", bucketSort, 0, sizes[i], 0);
        runTest("基数排序", radixSort, 0, sizes[i], 0);
        
        printf("\n-------- 测试几乎有序的数组 --------\n");
        runTest("冒泡排序", bubbleSort, 0, sizes[i], 1);
        runTest("选择排序", selectionSort, 0, sizes[i], 1);
        runTest("插入排序", insertionSort, 0, sizes[i], 1);
        runTest("归并排序", mergeSort, 1, sizes[i], 1);
        runTest("快速排序", quickSort, 1, sizes[i], 1);
        runTest("计数排序", countingSort, 0, sizes[i], 1);
        runTest("桶排序", bucketSort, 0, sizes[i], 1);
        runTest("基数排序", radixSort, 0, sizes[i], 1);
    }
    
    return 0;
}
