/* 编写程序，读入矩阵行数、列数及所有矩阵元素，矩阵中所有元素均为正整数，计算并打印出矩阵中的最大连通块数。注：如果两个元素值相同，并且上、下、左、右四个方向之一相邻，则称两个元素是连通的;连通关系是可传递的，一个元素的连通元素，也是与它连通元素的连通元素。最大连通块定义为所有连通元素组成的最大集，单个元素也可成为最大连通块。要求设计出求连通块数的递归或非递归算法。矩阵行数、列数不超出50。

输入格式:
行数、列数及所有矩阵元素，所有数据均为整型。

输出格式:
连通块数量。

输入样例:
7 6    
4   4   4   4   4   4
4   1   3   2   1   4
4   1   2   2   1   4
4   1   1   1   1   4
4   1   2   2   3   4
4   3   3   3   3   4
4   4   4   4   4   4
输出样例:
在这里给出相应的输出。例如：
6*/

#include <stdio.h>
#define MAX_SIZE 50

int matrix[MAX_SIZE][MAX_SIZE];  // Original matrix
int visited[MAX_SIZE][MAX_SIZE]; // Track visited cells
int rows, cols;

// DFS to mark all connected cells with the same value
void dfs(int r, int c, int value) {
    // Check if out of bounds, already visited, or different value
    if (r < 0 || r >= rows || c < 0 || c >= cols || 
        visited[r][c] || matrix[r][c] != value) {
        return;
    }
    
    // Mark as visited
    visited[r][c] = 1;
    
    // Check all four adjacent cells
    dfs(r + 1, c, value); // Down
    dfs(r - 1, c, value); // Up
    dfs(r, c + 1, value); // Right
    dfs(r, c - 1, value); // Left
}

int main() {
    // Read matrix dimensions
    scanf("%d %d", &rows, &cols);
    
    // Read matrix elements
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix[i][j]);
            visited[i][j] = 0; // Initialize visited array
        }
    }
    
    // Count connected components
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!visited[i][j]) {
                count++;
                dfs(i, j, matrix[i][j]);
            }
        }
    }
    
    // Print the result
    printf("%d\n", count);
    
    return 0;
}
