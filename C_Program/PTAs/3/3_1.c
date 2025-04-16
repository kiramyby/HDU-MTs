/*2.1 给定一个含n个整数顺序存储的线性表，按分治法思路，采用二分策略，设计一个求出其最大值和最小值算法，编写相应测试程序。要求使用分治法设计出其中求最大值、最小值组合的递归算法。

输入格式:
若干整数，个数小于等于10000。

输出格式:
最小值,最大值。

输入样例:
100 2 3 -2 -8 -6 -9 -10 50 2 -1
输出样例:
-10,100
*/

#include <stdio.h>
#include <stdlib.h>

// Structure to hold both max and min values
typedef struct {
    int max;
    int min;
} MaxMin;

// Divide and conquer function to find max and min
MaxMin findMaxMin(int arr[], int left, int right) {
    MaxMin result, leftResult, rightResult;
    int mid;
    
    // Base case: Only one element
    if (left == right) {
        result.max = arr[left];
        result.min = arr[left];
        return result;
    }
    
    // Base case: Two elements
    if (right - left == 1) {
        if (arr[left] > arr[right]) {
            result.max = arr[left];
            result.min = arr[right];
        } else {
            result.max = arr[right];
            result.min = arr[left];
        }
        return result;
    }
    
    // Divide the array into two halves
    mid = (left + right) / 2;
    
    // Recursively find max and min in left half
    leftResult = findMaxMin(arr, left, mid);
    
    // Recursively find max and min in right half
    rightResult = findMaxMin(arr, mid + 1, right);
    
    // Combine results
    result.max = leftResult.max > rightResult.max ? leftResult.max : rightResult.max;
    result.min = leftResult.min < rightResult.min ? leftResult.min : rightResult.min;
    
    return result;
}

int main() {
    int numbers[10000]; // Maximum 10000 integers as per requirement
    int count = 0;
    int num;
    
    // Read input numbers
    while (scanf("%d", &num) != EOF) {
        numbers[count++] = num;
    }
    
    // Find max and min using divide and conquer
    MaxMin result = findMaxMin(numbers, 0, count - 1);
    
    // Print the result in required format
    printf("%d,%d\n", result.min, result.max);
    
    return 0;
}
