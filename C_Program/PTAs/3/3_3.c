#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int bigNumberCompare(const char *num1, const char *num2) {
    int len1 = strlen(num1), len2 = strlen(num2);
    if (len1 != len2) {
        return len1 - len2;
    }
    return strcmp(num1, num2);
}

void bigNumberAdd(const char *num1, const char *num2, char *result) {
    int len1 = strlen(num1), len2 = strlen(num2);
    int carry = 0, i = len1 - 1, j = len2 - 1, k = 0;
    char temp[1000] = {0};

    while (i >= 0 || j >= 0 || carry) {
        int digit1 = (i >= 0) ? num1[i--] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j--] - '0' : 0;
        int sum = digit1 + digit2 + carry;
        temp[k++] = (sum % 10) + '0';
        carry = sum / 10;
    }

    for (int l = 0; l < k; l++) {
        result[l] = temp[k - l - 1];
    }
    result[k] = '\0';
}

void bigNumberSubtract(const char *num1, const char *num2, char *result) {
    int len1 = strlen(num1), len2 = strlen(num2);
    int borrow = 0, i = len1 - 1, j = len2 - 1, k = 0;
    char temp[1000] = {0};

    while (i >= 0 || j >= 0) {
        int digit1 = (i >= 0) ? num1[i--] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j--] - '0' : 0;
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        temp[k++] = diff + '0';
    }

    while (k > 1 && temp[k - 1] == '0') {
        k--; // Remove leading zeros
    }

    for (int l = 0; l < k; l++) {
        result[l] = temp[k - l - 1];
    }
    result[k] = '\0';
}

void bigNumberMultiply(const char *num1, const char *num2, char *result) {
    int len1 = strlen(num1), len2 = strlen(num2);
    int resultLen = len1 + len2;
    int temp[2000] = {0}; // Use int array for intermediate calculations
    
    // Perform multiplication digit by digit
    for (int i = len1 - 1; i >= 0; i--) {
        for (int j = len2 - 1; j >= 0; j--) {
            int digit1 = num1[i] - '0';
            int digit2 = num2[j] - '0';
            temp[i + j + 1] += digit1 * digit2;
        }
    }
    
    // Process carries
    for (int i = resultLen - 1; i > 0; i--) {
        temp[i - 1] += temp[i] / 10;
        temp[i] %= 10;
    }
    
    // Convert to string and remove leading zeros
    int start = 0;
    while (start < resultLen - 1 && temp[start] == 0) {
        start++;
    }
    
    int k = 0;
    for (int i = start; i < resultLen; i++) {
        result[k++] = temp[i] + '0';
    }
    result[k] = '\0';
}

void signedBigNumberAdd(const char *num1, const char *num2, char *result) {
    if (num1[0] == '-' && num2[0] == '-') {
        result[0] = '-';
        bigNumberAdd(num1 + 1, num2 + 1, result + 1);
    } else if (num1[0] == '-') {
        if (bigNumberCompare(num1 + 1, num2) > 0) {
            result[0] = '-';
            bigNumberSubtract(num1 + 1, num2, result + 1);
        } else {
            bigNumberSubtract(num2, num1 + 1, result);
        }
    } else if (num2[0] == '-') {
        if (bigNumberCompare(num1, num2 + 1) >= 0) {
            bigNumberSubtract(num1, num2 + 1, result);
        } else {
            result[0] = '-';
            bigNumberSubtract(num2 + 1, num1, result + 1);
        }
    } else {
        bigNumberAdd(num1, num2, result);
    }
    
    // Handle case where result is zero
    if (strcmp(result, "0") == 0 || (result[0] == '-' && strcmp(result + 1, "0") == 0)) {
        result[0] = '0';
        result[1] = '\0';
    }
}

void signedBigNumberSubtract(const char *num1, const char *num2, char *result) {
    char negNum2[1000];
    if (num2[0] == '-') {
        strcpy(negNum2, num2 + 1);
    } else {
        negNum2[0] = '-';
        strcpy(negNum2 + 1, num2);
    }
    signedBigNumberAdd(num1, negNum2, result);
}

void signedBigNumberMultiply(const char *num1, const char *num2, char *result) {
    // Determine the sign of the result
    int sign = 1;
    const char *n1 = num1;
    const char *n2 = num2;
    
    if (num1[0] == '-') {
        sign = -sign;
        n1++;
    }
    
    if (num2[0] == '-') {
        sign = -sign;
        n2++;
    }
    
    // Check for zero cases
    if ((n1[0] == '0' && n1[1] == '\0') || (n2[0] == '0' && n2[1] == '\0')) {
        result[0] = '0';
        result[1] = '\0';
        return;
    }
    
    // Calculate the product
    if (sign < 0) {
        result[0] = '-';
        bigNumberMultiply(n1, n2, result + 1);
    } else {
        bigNumberMultiply(n1, n2, result);
    }
}

int main() {
    char num1[1000], num2[1000], sum[2000], diff[2000], product[2000];
    
    scanf("%s %s", num1, num2);
    
    signedBigNumberAdd(num1, num2, sum);
    signedBigNumberSubtract(num1, num2, diff);
    signedBigNumberMultiply(num1, num2, product);
    
    printf("%s\n", sum);
    printf("%s\n", diff);
    printf("%s\n", product);
    
    return 0;
}
