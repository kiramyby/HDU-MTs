#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "auth.h"
#include "utils.h"

// 管理员登录验证
void login() {
    char username[20], password[20];
    int attempts = 3;

    while (attempts > 0) {
        printf("\n=== 登录 ===\n");
        printf("用户名: "); scanf("%19s", username); clear_input_buffer();
        printf("密码: "); scanf("%19s", password); clear_input_buffer();

        // 简单验证（实际应加密存储）
        if (strcmp(username, "admin") == 0 && strcmp(password, "123456") == 0) {
            logged_in = 1;
            return;
        }
        printf("用户名或密码错误！剩余尝试次数：%d\n", --attempts);
    }
    printf("登录失败，退出系统！\n");
    exit(0);
}

// 管理员登出
void logout() {
    logged_in = 0;
    printf("已退出系统！\n");
    exit(0);
}
