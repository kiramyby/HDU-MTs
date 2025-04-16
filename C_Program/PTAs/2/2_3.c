/*1.8 无符号大数加、减运算。程序设计中经常遇到无符号大数加、减运算问题，请在样例程序Ex1.4基础上实现无符号大数减运算。题目要求输入两个无符号大数，保证一个大数不小于第二个大数，输出它们的和、差。

输入格式:
两个无符号大数，前一个大于等于第二个。

输出格式:
第1行为两个无符号大数相加结果，后一行为两个无符号大数相减结果。

输入样例:
1234567890987654321333888999666
147655765659657669789687967867
输出样例:
在这里给出相应的输出。例如：

1234567890987654321333888999666+147655765659657669789687967867=1382223656647311991123576967533
1234567890987654321333888999666-147655765659657669789687967867=1086912125327996651544201031799*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 1000  // Maximum length of a large number

// Structure to represent a large number
typedef struct {
    int digit[MAX_LEN];  // Store digits from right to left
    int size;            // Number of digits
} LargeNum;

// Function to read a large number from input
void readLargeNum(LargeNum *num) {
    char str[MAX_LEN + 1];
    scanf("%s", str);
    
    int len = strlen(str);
    num->size = len;
    
    // Store digits from right to left for easier calculation
    for (int i = 0; i < len; i++) {
        num->digit[len - 1 - i] = str[i] - '0';
    }
}

// Function to display a large number
void displayLargeNum(const LargeNum *num) {
    for (int i = num->size - 1; i >= 0; i--) {
        printf("%d", num->digit[i]);
    }
}

// Function to add two large numbers
LargeNum addLargeNum(const LargeNum *a, const LargeNum *b) {
    LargeNum result;
    int carry = 0;
    int i;
    
    // Calculate maximum possible result size
    int max_size = (a->size > b->size) ? a->size : b->size;
    
    for (i = 0; i < max_size || carry; i++) {
        int sum = carry;
        if (i < a->size) sum += a->digit[i];
        if (i < b->size) sum += b->digit[i];
        
        result.digit[i] = sum % 10;
        carry = sum / 10;
    }
    
    result.size = i;
    return result;
}

// Function to subtract two large numbers (a >= b is assumed)
LargeNum subtractLargeNum(const LargeNum *a, const LargeNum *b) {
    LargeNum result;
    int borrow = 0;
    int i;
    
    for (i = 0; i < a->size; i++) {
        int diff = a->digit[i] - borrow;
        if (i < b->size) diff -= b->digit[i];
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.digit[i] = diff;
    }
    
    // Determine actual size by skipping leading zeros
    result.size = a->size;
    while (result.size > 1 && result.digit[result.size - 1] == 0) {
        result.size--;
    }
    
    return result;
}

int main() {
    LargeNum num1, num2, sum, difference;
    
    // Read two large numbers
    readLargeNum(&num1);
    readLargeNum(&num2);
    
    // Calculate sum and difference
    sum = addLargeNum(&num1, &num2);
    difference = subtractLargeNum(&num1, &num2);
    
    // Display sum
    displayLargeNum(&num1);
    printf("+");
    displayLargeNum(&num2);
    printf("=");
    displayLargeNum(&sum);
    printf("\n");
    
    // Display difference
    displayLargeNum(&num1);
    printf("-");
    displayLargeNum(&num2);
    printf("=");
    displayLargeNum(&difference);
    printf("\n");
    
    return 0;
}