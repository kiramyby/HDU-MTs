#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 8  // 棋盘尺寸
#define ANIMATION_DELAY 200000  // 动画延迟（微秒）

// 马可以移动的八个方向
int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

// 棋盘，用于记录马移动的顺序，初始为-1表示未访问
int board[N][N];

// 检查位置是否有效（在棋盘内且未被访问）
int isValid(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N && board[x][y] == -1);
}

// 计算从(x,y)位置可以进行的下一步合法移动的数量
int countNextMoves(int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        int nextX = x + dx[i];
        int nextY = y + dy[i];
        if (isValid(nextX, nextY)) {
            count++;
        }
    }
    return count;
}

// 使用Warnsdorff启发式选择下一步移动
int findNextMove(int x, int y, int *nextX, int *nextY) {
    int min_deg = N + 1; // 设置为一个不可能的大值
    int min_idx = -1;
    
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (isValid(nx, ny)) {
            int deg = countNextMoves(nx, ny);
            if (deg < min_deg) {
                min_deg = deg;
                min_idx = i;
            }
        }
    }
    
    if (min_idx != -1) {
        *nextX = x + dx[min_idx];
        *nextY = y + dy[min_idx];
        return 1;
    }
    return 0;
}

// 清屏函数 - 跨平台
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 在控制台绘制棋盘
void drawBoard() {
    clearScreen();
    
    // 打印列标题 (0-7)
    printf("   ");
    for (int j = 0; j < N; j++) {
        printf(" %d  ", j);
    }
    printf("\n");
    
    // 打印顶部边框
    printf("   ");
    for (int j = 0; j < N; j++) {
        printf("----");
    }
    printf("\n");
    
    // 打印棋盘内容
    for (int i = 0; i < N; i++) {
        // 打印行号
        printf("%d | ", i);
        
        for (int j = 0; j < N; j++) {
            if (board[i][j] == -1) {
                // 未访问格子
                if ((i + j) % 2 == 0) {
                    printf("   "); // 浅色格子
                } else {
                    printf("███"); // 深色格子
                }
            } else {
                // 已访问格子，显示移动顺序编号
                printf("%2d ", board[i][j]);
            }
            printf("|");
        }
        printf("\n");
        
        // 打印行间分隔线
        printf("   ");
        for (int j = 0; j < N; j++) {
            printf("----");
        }
        printf("\n");
    }
    
    // 输出提示信息
    printf("\n当前马踏棋盘解算中，请等待...\n");
    printf("数字表示马的移动顺序（从0开始）\n");
}

// 显示最终解决方案
void showFinalSolution() {
    clearScreen();
    printf("\n最终解决方案:\n\n");
    
    printf("   ");
    for (int j = 0; j < N; j++) {
        printf(" %d  ", j);
    }
    printf("\n");
    
    printf("   ");
    for (int j = 0; j < N; j++) {
        printf("----");
    }
    printf("\n");
    
    for (int i = 0; i < N; i++) {
        // 打印行号
        printf("%d | ", i);
        
        for (int j = 0; j < N; j++) {
            printf("%2d ", board[i][j]);
            printf("|");
        }
        printf("\n");
        printf("   ");
        for (int j = 0; j < N; j++) {
            printf("----");
        }
        printf("\n");
    }
}

// 使用回溯法解决马踏棋盘问题
int solveKnightTour(int x, int y, int moveCount) {
    // 如果所有方格都已访问，返回成功
    if (moveCount == N * N) {
        return 1;
    }
    
    // 更新显示
    drawBoard();
    usleep(ANIMATION_DELAY);
    
    int nextX, nextY;
    // 查找下一个合法移动
    if (findNextMove(x, y, &nextX, &nextY)) {
        board[nextX][nextY] = moveCount;
        if (solveKnightTour(nextX, nextY, moveCount + 1)) {
            return 1;
        }
        // 回溯
        board[nextX][nextY] = -1;
    }
    
    return 0;
}

// 初始化棋盘并开始解决问题
int startKnightTour(int startX, int startY) {
    // 初始化棋盘
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = -1;
        }
    }
    board[startX][startY] = 0;

    drawBoard();
    usleep(ANIMATION_DELAY);

    if (solveKnightTour(startX, startY, 1)) {
        printf("\n马踏棋盘问题已解决!\n");
        showFinalSolution();
        return 1;
    } else {
        printf("无法找到解决方案。\n");
        return 0;
    }
}

int main() {
    int startX, startY;
    
    printf("马踏棋盘求解程序!\n");
    printf("请输入马的起始位置 (0-7 0-7): ");
    scanf("%d %d", &startX, &startY);
    
    // 确保起始位置有效
    if (startX < 0 || startX >= N || startY < 0 || startY >= N) {
        printf("起始位置无效，请输入0到7之间的坐标。\n");
        return 1;
    }
    
    // 记录开始时间
    clock_t start = clock();
    
    startKnightTour(startX, startY);
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\n求解耗时: %f 秒\n", time_spent);
    
    return 0;
}
