#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "student_mgmt.h"
#include "file_ops.h"
#include "utils.h"
#include "display.h"

// 根据学号查找学生
Student* find_student(const char* student_id) {
    Student* current = student_head;
    while (current) {
        if (strcmp(current->student_id, student_id) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

// 添加新的学生信息
void add_student() {
    Student* new_student = (Student*)malloc(sizeof(Student));

    print_header("👤 添加学生信息");
    printf("%s学号: %s", CYAN, RESET_COLOR); 
    scanf("%14s", new_student->student_id); clear_input_buffer();
    printf("%s姓名: %s", CYAN, RESET_COLOR); 
    fgets(new_student->name, sizeof(new_student->name), stdin); 
    new_student->name[strcspn(new_student->name, "\n")] = '\0';
    printf("%s性别: %s", CYAN, RESET_COLOR); 
    scanf("%9s", new_student->gender); clear_input_buffer();
    printf("%s班级: %s", CYAN, RESET_COLOR); 
    fgets(new_student->class_name, sizeof(new_student->class_name), stdin); 
    new_student->class_name[strcspn(new_student->class_name, "\n")] = '\0';
    printf("%s联系方式: %s", CYAN, RESET_COLOR); 
    scanf("%19s", new_student->contact); clear_input_buffer();

    new_student->next = student_head;
    student_head = new_student;

    // 保存到文件
    save_students_to_file();
    print_success_message("学生信息添加成功！");
}

// 显示所有学生信息
void show_all_students() {
    clear_screen();
    print_header("👥 所有学生信息");
    
    Student* current = student_head;
    if (!current) {
        print_warning_message("暂无学生数据");
        wait_for_enter();
        return;
    }
    
    // 统计总数
    int total_count = 0;
    Student* temp = current;
    while (temp) {
        total_count++;
        temp = temp->next;
    }
    
    const int PAGE_SIZE = 10; // 每页显示10条记录
    int total_pages = (total_count + PAGE_SIZE - 1) / PAGE_SIZE;
    int current_page = 1;
    
    while (current_page <= total_pages) {
        clear_screen();
        print_header("👥 所有学生信息");
        
        printf("%s%s总共 %d 名学生%s\n", BLUE, BOLD, total_count, RESET_COLOR);
        
        display_student_table_header();
        
        // 显示当前页的数据
        Student* page_current = student_head;
        int page_displayed = 0;
        
        // 跳到当前页的起始位置
        for (int i = 0; i < (current_page - 1) * PAGE_SIZE && page_current; i++) {
            page_current = page_current->next;
        }
        
        // 显示当前页的记录
        while (page_current && page_displayed < PAGE_SIZE) {
            display_student_row(page_current);
            page_current = page_current->next;
            page_displayed++;
        }
        
        // 打印表格底部
        int col_widths[] = {12, 15, 6, 15, 15};
        print_table_separator(5, col_widths);
        
        // 显示分页信息
        display_page_info(current_page, total_pages);
        
        // 处理用户输入
        printf("\n%s输入: %s", CYAN, RESET_COLOR);
        char input = getchar();
        while (getchar() != '\n'); // 清除输入缓冲区
        
        if (input == 'q' || input == 'Q') {
            break;
        } else if (input == ' ') {
            current_page++;
        } else if ((input == 'b' || input == 'B') && current_page > 1) {
            current_page--;
        }
    }
    
    print_info_message("学生信息查看完成");
}

// 修改指定的学生信息
void modify_student() {
    char student_id[15];
    print_header("✏️ 修改学生信息");
    printf("%s输入要修改的学生学号: %s", CYAN, RESET_COLOR); 
    scanf("%14s", student_id); clear_input_buffer();

    Student* student = find_student(student_id);
    if (!student) {
        print_error_message("学生不存在！");
        return;
    }

    printf("\n%s当前学生信息:%s\n", BOLD, RESET_COLOR);
    display_student_table_header();
    display_student_row(student);
    int col_widths[] = {12, 15, 6, 15, 15};
    print_table_separator(5, col_widths);

    printf("\n%s请输入新信息:%s\n", BOLD, RESET_COLOR);
    printf("%s新姓名: %s", CYAN, RESET_COLOR); 
    fgets(student->name, sizeof(student->name), stdin); 
    student->name[strcspn(student->name, "\n")] = '\0';
    printf("%s新性别: %s", CYAN, RESET_COLOR); 
    scanf("%9s", student->gender); clear_input_buffer();
    printf("%s新班级: %s", CYAN, RESET_COLOR); 
    fgets(student->class_name, sizeof(student->class_name), stdin); 
    student->class_name[strcspn(student->class_name, "\n")] = '\0';
    printf("%s新联系方式: %s", CYAN, RESET_COLOR); 
    scanf("%19s", student->contact); clear_input_buffer();

    save_students_to_file(); // 保存更改到文件
    print_success_message("学生信息修改成功！");
}

// 查找包含关键字的学生信息
void search_student() {
    char keyword[50];
    print_header("🔍 查找学生信息");
    printf("%s输入关键词: %s", CYAN, RESET_COLOR); 
    fgets(keyword, sizeof(keyword), stdin); 
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("\n%s查找结果:%s\n", BOLD, RESET_COLOR);
    Student* current = student_head;
    int found_count = 0;
    
    // 先检查是否有匹配的结果
    Student* temp = current;
    while (temp) {
        if (strstr(temp->name, keyword) || strstr(temp->student_id, keyword) ||
            strstr(temp->class_name, keyword)) {
            found_count++;
        }
        temp = temp->next;
    }
    
    if (found_count == 0) {
        print_warning_message("未找到匹配的学生信息");
        return;
    }
    
    display_student_table_header();
    while (current) {
        if (strstr(current->name, keyword) || strstr(current->student_id, keyword) ||
            strstr(current->class_name, keyword)) {
            display_student_row(current);
        }
        current = current->next;
    }
    
    // 打印表格底部
    int col_widths[] = {12, 15, 6, 15, 15};
    print_table_separator(5, col_widths);
    
    char result_msg[100];
    snprintf(result_msg, sizeof(result_msg), "找到 %d 个匹配学生", found_count);
    print_info_message(result_msg);
}

// 统计每个学生的报名数量
void count_registrations_per_student() {
    print_header("📊 每个学生的报名数量统计");
    
    Student* current_student = student_head;
    if (!current_student) {
        print_warning_message("暂无学生数据");
        return;
    }

    const char* headers[] = {"学号", "姓名", "班级", "报名数量"};
    int col_widths[] = {12, 15, 15, 12};
    print_table_header(headers, 4, col_widths);
    
    while (current_student) {
        int count = 0;
        Registration* current_reg = reg_head;
        while (current_reg) {
            if (strcmp(current_reg->student_id, current_student->student_id) == 0) {
                count++;
            }
            current_reg = current_reg->next;
        }
        
        char count_str[20];
        snprintf(count_str, sizeof(count_str), "%d", count);
        
        const char* data[] = {
            current_student->student_id, 
            current_student->name, 
            current_student->class_name,
            count_str
        };
        print_table_row(data, 4, col_widths);
        current_student = current_student->next;
    }
    
    print_table_separator(4, col_widths);
    print_info_message("统计数据显示完成");
}

// 学生信息管理菜单
void student_management_menu() {
    int choice;
    do {
        clear_screen();
        display_student_menu();
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            clear_screen();
            add_student();
            wait_for_enter();
            break;
        case 2:
            show_all_students();
            break;
        case 3:
            clear_screen();
            modify_student();
            wait_for_enter();
            break;
        case 4:
            clear_screen();
            search_student();
            wait_for_enter();
            break;
        case 0:
            return;
        default:
            print_error_message("无效选项！");
            wait_for_enter();
        }
    } while (1);
}
