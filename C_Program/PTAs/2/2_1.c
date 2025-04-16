#include <stdio.h>
#include <stdlib.h>

void josephus(int *arr, int size, int step, int *result) {
    int index = 0, count = 0, remaining = size;

    while (remaining > 0) {
        if (arr[index] != 0) { // 如果当前编号未出列
            count++;
            if (count == step) { // 报数到step时出列
                *result++ = arr[index];
                arr[index] = 0; // 标记为出列
                count = 0;
                remaining--;
            }
        }
        index = (index + 1) % size; // 循环移动到下一个人
    }
}

int main() {
    int M, N, K;
    scanf("%d %d %d", &M, &N, &K);

    int *firstRound = (int *)malloc(M * sizeof(int));
    int *secondRound = (int *)malloc(M * sizeof(int));
    int *temp = (int *)malloc(M * sizeof(int));

    // 初始化数组
    for (int i = 0; i < M; i++) {
        firstRound[i] = i + 1;
    }

    // 第一轮报数
    josephus(firstRound, M, N, temp);

    // 第二轮报数
    josephus(temp, M, K, secondRound);

    // 输出结果
    for (int i = 0; i < M; i++) {
        printf("%4d", secondRound[i]);
    }

    // 释放内存
    free(firstRound);
    free(secondRound);
    free(temp);

    return 0;
}