#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "auth.h"
#include "utils.h"
#include "display.h"

// 管理员登录验证
void login() {
    char username[20], password[20];
    int attempts = 3;

    while (attempts > 0) {
        print_header("🔐 管理员登录");
        printf("%s用户名: %s", CYAN, RESET_COLOR); 
        scanf("%19s", username); clear_input_buffer();
        printf("%s密码: %s", CYAN, RESET_COLOR); 
        scanf("%19s", password); clear_input_buffer();

        // 简单验证（实际应加密存储）
        if (strcmp(username, "admin") == 0 && strcmp(password, "123456") == 0) {
            logged_in = 1;
            print_success_message("登录成功！欢迎管理员！");
            return;
        }
        
        char error_msg[100];
        snprintf(error_msg, sizeof(error_msg), "用户名或密码错误！剩余尝试次数：%d", --attempts);
        print_error_message(error_msg);
    }
    print_error_message("登录失败，退出系统！");
    exit(0);
}

// 管理员登出
void logout() {
    logged_in = 0;
    print_info_message("已退出登录，感谢使用！");
    exit(0);
}
