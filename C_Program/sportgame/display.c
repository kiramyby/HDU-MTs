#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include "display.h"
#include "types.h"

// 计算字符串的显示宽度（考虑中文字符和emoji）
int get_display_width(const char* str) {
    if (!str) return 0;
    
    int width = 0;
    const unsigned char* s = (const unsigned char*)str;
    
    while (*s) {
        if (*s < 0x80) {
            // ASCII 字符，宽度为 1
            width++;
            s++;
        } else if ((*s & 0xE0) == 0xC0) {
            // UTF-8 两字节字符，宽度为 1
            width++;
            s += 2;
        } else if ((*s & 0xF0) == 0xE0) {
            // UTF-8 三字节字符（包括大部分中文字符），宽度为 2
            width += 2;
            s += 3;
        } else if ((*s & 0xF8) == 0xF0) {
            // UTF-8 四字节字符（包括emoji），宽度为 2
            width += 2;
            s += 4;
        } else {
            // 无效字符，跳过
            s++;
        }
    }
    
    return width;
}

// 打印标题头部
void print_header(const char* title) {
    int title_len = get_display_width(title);
    int box_width = title_len + 6; // 额外空间用于边框和填充
    
    printf("\n");
    print_box_top(box_width);
    printf("%s%s", VERTICAL, BOLD);
    print_centered_text(title, box_width - 2);
    printf("%s%s\n", RESET_COLOR, VERTICAL);
    print_box_bottom(box_width);
    printf("\n");
}

// 打印分隔线
void print_separator() {
    printf("%s", CYAN);
    for (int i = 0; i < 60; i++) {
        printf("%s", HORIZONTAL);
    }
    printf("%s\n", RESET_COLOR);
}

// 打印指定宽度的线
void print_line(int width) {
    printf("%s", CYAN);
    for (int i = 0; i < width; i++) {
        printf("%s", HORIZONTAL);
    }
    printf("%s\n", RESET_COLOR);
}

// 打印盒子顶部
void print_box_top(int width) {
    printf("%s%s%s", CYAN, TOP_LEFT, RESET_COLOR);
    for (int i = 0; i < width - 2; i++) {
        printf("%s%s%s", CYAN, HORIZONTAL, RESET_COLOR);
    }
    printf("%s%s%s\n", CYAN, TOP_RIGHT, RESET_COLOR);
}

// 打印盒子底部
void print_box_bottom(int width) {
    printf("%s%s%s", CYAN, BOTTOM_LEFT, RESET_COLOR);
    for (int i = 0; i < width - 2; i++) {
        printf("%s%s%s", CYAN, HORIZONTAL, RESET_COLOR);
    }
    printf("%s%s%s\n", CYAN, BOTTOM_RIGHT, RESET_COLOR);
}

// 打印居中文本
void print_centered_text(const char* text, int width) {
    int text_len = get_display_width(text);
    int padding = (width - text_len) / 2;
    
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s", text);
    for (int i = 0; i < width - text_len - padding; i++) {
        printf(" ");
    }
}

// 打印菜单项
void print_menu_item(int number, const char* text) {
    printf("  %s%s[%d]%s %s%s%s\n", 
           CYAN, BOLD, number, RESET_COLOR,
           BLACK, text, RESET_COLOR);
}

// 打印成功消息
void print_success_message(const char* message) {
    printf("\n%s%s✓ %s%s\n", GREEN, BOLD, message, RESET_COLOR);
}

// 打印错误消息
void print_error_message(const char* message) {
    printf("\n%s%s✗ %s%s\n", RED, BOLD, message, RESET_COLOR);
}

// 打印警告消息
void print_warning_message(const char* message) {
    printf("\n%s%s⚠ %s%s\n", YELLOW, BOLD, message, RESET_COLOR);
}

// 打印信息消息
void print_info_message(const char* message) {
    printf("\n%s%sℹ %s%s\n", BLUE, BOLD, message, RESET_COLOR);
}

// 打印表格头部
void print_table_header(const char* headers[], int col_count, int col_widths[]) {
    // 打印顶部边框
    printf("%s%s%s", CYAN, TOP_LEFT, RESET_COLOR);
    for (int i = 0; i < col_count; i++) {
        for (int j = 0; j < col_widths[i]; j++) {
            printf("%s%s%s", CYAN, HORIZONTAL, RESET_COLOR);
        }
        if (i < col_count - 1) {
            printf("%s%s%s", CYAN, TOP_CROSS, RESET_COLOR);
        }
    }
    printf("%s%s%s\n", CYAN, TOP_RIGHT, RESET_COLOR);
    
    // 打印头部内容
    printf("%s%s%s", CYAN, VERTICAL, RESET_COLOR);
    for (int i = 0; i < col_count; i++) {
        printf("%s%s", BOLD, headers[i]);
        int padding = col_widths[i] - get_display_width(headers[i]);
        for (int j = 0; j < padding; j++) {
            printf(" ");
        }
        printf("%s", RESET_COLOR);
        if (i < col_count - 1) {
            printf("%s%s%s", CYAN, VERTICAL, RESET_COLOR);
        }
    }
    printf("%s%s%s\n", CYAN, VERTICAL, RESET_COLOR);
    
    // 打印分隔线
    printf("%s%s%s", CYAN, LEFT_CROSS, RESET_COLOR);
    for (int i = 0; i < col_count; i++) {
        for (int j = 0; j < col_widths[i]; j++) {
            printf("%s%s%s", CYAN, HORIZONTAL, RESET_COLOR);
        }
        if (i < col_count - 1) {
            printf("%s%s%s", CYAN, CROSS, RESET_COLOR);
        }
    }
    printf("%s%s%s\n", CYAN, RIGHT_CROSS, RESET_COLOR);
}

// 打印表格行
void print_table_row(const char* data[], int col_count, int col_widths[]) {
    printf("%s%s%s", CYAN, VERTICAL, RESET_COLOR);
    for (int i = 0; i < col_count; i++) {
        printf("%s", data[i]);
        int padding = col_widths[i] - get_display_width(data[i]);
        for (int j = 0; j < padding; j++) {
            printf(" ");
        }
        if (i < col_count - 1) {
            printf("%s%s%s", CYAN, VERTICAL, RESET_COLOR);
        }
    }
    printf("%s%s%s\n", CYAN, VERTICAL, RESET_COLOR);
}

// 打印表格分隔线
void print_table_separator(int col_count, int col_widths[]) {
    printf("%s%s%s", CYAN, BOTTOM_LEFT, RESET_COLOR);
    for (int i = 0; i < col_count; i++) {
        for (int j = 0; j < col_widths[i]; j++) {
            printf("%s%s%s", CYAN, HORIZONTAL, RESET_COLOR);
        }
        if (i < col_count - 1) {
            printf("%s%s%s", CYAN, BOTTOM_CROSS, RESET_COLOR);
        }
    }
    printf("%s%s%s\n", CYAN, BOTTOM_RIGHT, RESET_COLOR);
}

// 显示运动项目表格头部
void display_event_table_header() {
    const char* headers[] = {"项目编号", "项目名称", "类型", "类别", "时间", "地点", "状态", "报名人数"};
    int col_widths[] = {12, 20, 10, 10, 20, 15, 10, 12};
    print_table_header(headers, 8, col_widths);
}

// 显示运动项目行
void display_event_row(const SportEvent* event) {
    char participants[20];
    snprintf(participants, sizeof(participants), "%d/%d", 
             event->current_participants, event->max_participants);
    
    const char* data[] = {
        event->event_id,
        event->name,
        event->type,
        event->category,
        event->time,
        event->location,
        event->status,
        participants
    };
    int col_widths[] = {12, 20, 10, 10, 20, 15, 10, 12};
    print_table_row(data, 8, col_widths);
}

// 显示学生表格头部
void display_student_table_header() {
    const char* headers[] = {"学号", "姓名", "性别", "班级", "联系方式"};
    int col_widths[] = {12, 15, 6, 15, 15};
    print_table_header(headers, 5, col_widths);
}

// 显示学生行
void display_student_row(const Student* student) {
    const char* data[] = {
        student->student_id,
        student->name,
        student->gender,
        student->class_name,
        student->contact
    };
    int col_widths[] = {12, 15, 6, 15, 15};
    print_table_row(data, 5, col_widths);
}

// 显示报名表格头部
void display_registration_table_header() {
    const char* headers[] = {"学生姓名", "项目名称", "报名时间", "状态"};
    int col_widths[] = {15, 25, 20, 10};
    print_table_header(headers, 4, col_widths);
}

// 显示报名行
void display_registration_row(const char* student_name, const char* event_name, 
                             const char* reg_time, const char* status) {
    const char* data[] = {student_name, event_name, reg_time, status};
    int col_widths[] = {15, 25, 20, 10};
    print_table_row(data, 4, col_widths);
}

// 显示主菜单
void display_main_menu() {
    print_header("🏃 运动会管理系统 🏃");
    printf("\n");
    print_menu_item(1, "管理员登录");
    print_menu_item(2, "学生查询");
    print_menu_item(0, "退出系统");
    printf("\n");
    print_separator();
    printf("%s%s请选择操作:%s ", BOLD, CYAN, RESET_COLOR);
}

// 显示管理员菜单
void display_admin_menu() {
    print_header("🔧 管理员控制面板 🔧");
    printf("\n");
    print_menu_item(1, "运动项目管理");
    print_menu_item(2, "学生信息管理");
    print_menu_item(3, "报名管理");
    print_menu_item(4, "统计查询");
    print_menu_item(5, "录入成绩");
    print_menu_item(0, "退出系统");
    printf("\n");
    print_separator();
    printf("%s%s请选择操作:%s ", BOLD, CYAN, RESET_COLOR);
}

// 显示项目管理菜单
void display_event_menu() {
    print_header("🏆 运动项目管理 🏆");
    printf("\n");
    print_menu_item(1, "添加运动项目");
    print_menu_item(2, "显示所有项目");
    print_menu_item(3, "修改项目信息");
    print_menu_item(4, "查找项目");
    print_menu_item(5, "删除项目");
    print_menu_item(0, "返回上级菜单");
    printf("\n");
    print_separator();
    printf("%s%s请选择操作:%s ", BOLD, CYAN, RESET_COLOR);
}

// 显示学生管理菜单
void display_student_menu() {
    print_header("👥 学生信息管理 👥");
    printf("\n");
    print_menu_item(1, "添加学生信息");
    print_menu_item(2, "显示所有学生");
    print_menu_item(3, "修改学生信息");
    print_menu_item(4, "查找学生");
    print_menu_item(5, "删除学生");
    print_menu_item(0, "返回上级菜单");
    printf("\n");
    print_separator();
    printf("%s%s请选择操作:%s ", BOLD, CYAN, RESET_COLOR);
}

// 显示报名管理菜单
void display_registration_menu() {
    print_header("📝 报名管理 📝");
    printf("\n");
    print_menu_item(1, "学生报名");
    print_menu_item(2, "查看所有报名");
    print_menu_item(3, "查询学生报名");
    print_menu_item(4, "取消报名");
    print_menu_item(0, "返回上级菜单");
    printf("\n");
    print_separator();
    printf("%s%s请选择操作:%s ", BOLD, CYAN, RESET_COLOR);
}

// 显示统计查询菜单
void display_statistics_menu() {
    print_header("📊 统计查询 📊");
    printf("\n");
    print_menu_item(1, "各项目报名人数统计");
    print_menu_item(2, "每个学生的报名数量");
    print_menu_item(3, "查询比赛成绩");
    print_menu_item(0, "返回上级菜单");
    printf("\n");
    print_separator();
    printf("%s%s请选择操作:%s ", BOLD, CYAN, RESET_COLOR);
}

// 显示系统状态
void display_system_status() {
    print_header("📈 系统状态");
    
    // 这里可以添加系统状态信息
    printf("%s%s系统运行正常%s\n", GREEN, BOLD, RESET_COLOR);
    printf("当前时间: %s", __TIME__);
    printf("\n");
}

// 显示统计摘要
void display_statistics_summary() {
    print_header("📊 统计摘要");
    
    // 这里可以添加统计信息
    printf("正在加载统计数据...\n");
}

// 清屏函数
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// 等待用户按键继续
void pause_for_user() {
    printf("\n%s%s按任意键继续...%s", CYAN, BOLD, RESET_COLOR);
    getchar();
}

// 等待用户按回车键
void wait_for_enter() {
    printf("\n%s%s按 Enter 键继续...%s", CYAN, BOLD, RESET_COLOR);
    while (getchar() != '\n');
}

// 显示分页信息
void display_page_info(int current_page, int total_pages) {
    printf("\n%s%s第 %d 页，共 %d 页%s", BLUE, BOLD, current_page, total_pages, RESET_COLOR);
    if (current_page < total_pages) {
        printf("  %s%s[空格]下一页 [b]返回上一页 [q]退出查看%s", CYAN, BOLD, RESET_COLOR);
    } else {
        printf("  %s%s[b]返回上一页 [q]退出查看%s", CYAN, BOLD, RESET_COLOR);
    }
}

// 确认操作
int confirm_action(const char* message) {
    printf("\n%s%s%s (y/n): %s", YELLOW, BOLD, message, RESET_COLOR);
    char choice;
    scanf(" %c", &choice);
    return (choice == 'y' || choice == 'Y');
}
