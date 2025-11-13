#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

// 颜色定义（ANSI转义序列）
#define RESET_COLOR     "\033[0m"
#define BLACK           "\033[30m"
#define RED             "\033[31m"
#define GREEN           "\033[32m"
#define YELLOW          "\033[33m"
#define BLUE            "\033[34m"
#define MAGENTA         "\033[35m"
#define CYAN            "\033[36m"
#define BOLD            "\033[1m"
#define UNDERLINE       "\033[4m"

// 盒子绘制字符
#define TOP_LEFT        "┌"
#define TOP_RIGHT       "┐"
#define BOTTOM_LEFT     "└"
#define BOTTOM_RIGHT    "┘"
#define HORIZONTAL      "─"
#define VERTICAL        "│"
#define CROSS           "┼"
#define TOP_CROSS       "┬"
#define BOTTOM_CROSS    "┴"
#define LEFT_CROSS      "├"
#define RIGHT_CROSS     "┤"

// 工具函数
int get_display_width(const char* str);

// 显示格式化函数
void print_header(const char* title);
void print_separator();
void print_line(int width);
void print_box_top(int width);
void print_box_bottom(int width);
void print_centered_text(const char* text, int width);
void print_menu_item(int number, const char* text);
void print_success_message(const char* message);
void print_error_message(const char* message);
void print_warning_message(const char* message);
void print_info_message(const char* message);

// 表格显示函数
void print_table_header(const char* headers[], int col_count, int col_widths[]);
void print_table_row(const char* data[], int col_count, int col_widths[]);
void print_table_separator(int col_count, int col_widths[]);

// 专用显示函数
void display_event_table_header();
void display_event_row(const SportEvent* event);
void display_student_table_header();
void display_student_row(const Student* student);
void display_registration_table_header();
void display_registration_row(const char* student_name, const char* event_name, 
                             const char* reg_time, const char* status);

// 菜单显示函数
void display_main_menu();
void display_admin_menu();
void display_event_menu();
void display_student_menu();
void display_registration_menu();
void display_statistics_menu();

// 系统状态显示
void display_system_status();
void display_statistics_summary();

// 分页显示工具函数
void clear_screen();
void pause_for_user();
void wait_for_enter();
void display_page_info(int current_page, int total_pages);
int confirm_action(const char* message);

#endif
