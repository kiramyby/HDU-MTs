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
    int M;
    scanf("%d", &M);

    int *queue = (int *)malloc(M * sizeof(int));
    int *temp = (int *)malloc(M * sizeof(int));
    int *result = (int *)malloc(M * sizeof(int));

    // 初始化队列
    for (int i = 0; i < M; i++) {
        queue[i] = i + 1;
    }

    int step;
    while (scanf("%d", &step) == 1) { // 读取每轮的报数密码
        josephus(queue, M, step, temp);

        // 更新队列为新顺序
        for (int i = 0; i < M; i++) {
            queue[i] = temp[i];
        }
    }

    // 输出最终结果
    for (int i = 0; i < M; i++) {
        printf("%4d", queue[i]);
    }

    // 释放内存
    free(queue);
    free(temp);
    free(result);

    return 0;
}