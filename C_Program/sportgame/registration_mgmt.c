#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "registration_mgmt.h"
#include "student_mgmt.h"
#include "event_mgmt.h"
#include "file_ops.h"
#include "utils.h"
#include "display.h"

// 根据报名编号查找报名记录
Registration* find_registration(const char* reg_id) {
    Registration* current = reg_head;
    while (current) {
        if (strcmp(current->reg_id, reg_id) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

// 为学生报名参加某个项目
void register_event() {
    char stu_id[15], event_id[10];
    print_header("📝 项目报名");
    printf("%s输入学号: %s", CYAN, RESET_COLOR); 
    scanf("%14s", stu_id); clear_input_buffer();
    printf("%s输入项目编号: %s", CYAN, RESET_COLOR); 
    scanf("%9s", event_id); clear_input_buffer();

    Student* stu = find_student(stu_id);
    SportEvent* event = find_event(event_id);

    if (!stu) {
        print_error_message("学生不存在！");
        return;
    }
    if (!event) {
        print_error_message("项目不存在！");
        return;
    }

    if (strcmp(event->status, "报名中") != 0) {
        print_warning_message("该项目已结束报名！");
        return;
    }

    if (event->current_participants >= event->max_participants) {
        print_warning_message("该项目已报满！");
        return;
    }

    // 显示报名信息
    printf("\n%s报名信息确认:%s\n", BOLD, RESET_COLOR);
    printf("%s学生: %s%s (%s)%s\n", CYAN, GREEN, stu->name, stu->student_id, RESET_COLOR);
    printf("%s项目: %s%s (%s)%s\n", CYAN, GREEN, event->name, event->event_id, RESET_COLOR);
    printf("%s当前报名: %s%d/%d%s\n", CYAN, YELLOW, event->current_participants, event->max_participants, RESET_COLOR);

    // 创建新报名记录
    Registration* new_reg = (Registration*)malloc(sizeof(Registration));
    generate_reg_id(new_reg->reg_id);
    strcpy(new_reg->student_id, stu_id);
    strcpy(new_reg->event_id, event_id);
    get_time_string(new_reg->time);
    new_reg->score = -1;

    // 更新项目报名人数
    event->current_participants++;

    // 插入链表头部
    new_reg->next = reg_head;
    reg_head = new_reg;

    save_registrations_to_file(); // 保存更改到文件
    save_events_to_file(); // 保存项目变更到文件
    
    char success_msg[100];
    snprintf(success_msg, sizeof(success_msg), "报名成功！报名编号：%s", new_reg->reg_id);
    print_success_message(success_msg);
}

// 取消某个报名记录
void cancel_registration() {
    char reg_id[20];
    print_header("❌ 取消报名");
    printf("%s输入报名编号: %s", CYAN, RESET_COLOR); 
    scanf("%19s", reg_id); clear_input_buffer();

    Registration* prev = NULL;
    Registration* current = reg_head;
    while (current) {
        if (strcmp(current->reg_id, reg_id) == 0) {
            // 显示要取消的报名信息
            Student* stu = find_student(current->student_id);
            SportEvent* event = find_event(current->event_id);
            
            printf("\n%s要取消的报名信息:%s\n", BOLD, RESET_COLOR);
            printf("%s学生: %s%s (%s)%s\n", CYAN, YELLOW, 
                   stu ? stu->name : "未知", current->student_id, RESET_COLOR);
            printf("%s项目: %s%s (%s)%s\n", CYAN, YELLOW, 
                   event ? event->name : "未知", current->event_id, RESET_COLOR);
            
            // 更新项目报名人数
            if (event) {
                event->current_participants--;
            }

            if (prev) {
                prev->next = current->next;
            }
            else {
                reg_head = current->next;
            }
            free(current);

            save_registrations_to_file(); // 保存更改到文件
            save_events_to_file(); // 保存项目变更到文件
            print_success_message("报名取消成功！");
            return;
        }
        prev = current;
        current = current->next;
    }
    print_error_message("报名编号不存在！");
}

// 显示所有报名信息
void show_all_registrations() {
    Registration* current = reg_head;
    if (!current) {
        clear_screen();
        print_header("📋 所有报名信息");
        print_warning_message("暂无报名数据");
        wait_for_enter();
        return;
    }
    
    // 统计总数
    int total_count = 0;
    Registration* temp = current;
    while (temp) {
        total_count++;
        temp = temp->next;
    }
    
    const int PAGE_SIZE = 8; // 每页显示8条记录
    int total_pages = (total_count + PAGE_SIZE - 1) / PAGE_SIZE;
    int current_page = 1;
    
    while (current_page <= total_pages) {
        clear_screen();
        print_header("📋 所有报名信息");
        
        printf("%s%s总共 %d 条报名记录%s\n", BLUE, BOLD, total_count, RESET_COLOR);
        
        display_registration_table_header();
        
        // 显示当前页的数据
        Registration* page_current = reg_head;
        int page_displayed = 0;
        
        // 跳到当前页的起始位置
        for (int i = 0; i < (current_page - 1) * PAGE_SIZE && page_current; i++) {
            page_current = page_current->next;
        }
        
        // 显示当前页的记录
        while (page_current && page_displayed < PAGE_SIZE) {
            Student* student = find_student(page_current->student_id);
            SportEvent* event = find_event(page_current->event_id);
            
            display_registration_row(
                student ? student->name : "未知",
                event ? event->name : "未知",
                page_current->time,
                page_current->score >= 0 ? "已录入成绩" : "未录入成绩"
            );
            page_current = page_current->next;
            page_displayed++;
        }
        
        // 打印表格底部
        int col_widths[] = {15, 25, 20, 10};
        print_table_separator(4, col_widths);
        
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
    
    print_info_message("报名信息查看完成");
}

// 查找包含关键字的报名信息
void search_registration() {
    char keyword[50];
    print_header("🔍 查找报名信息");
    printf("%s输入关键词（学生姓名或项目名称）: %s", CYAN, RESET_COLOR); 
    fgets(keyword, sizeof(keyword), stdin); 
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("\n%s查找结果:%s\n", BOLD, RESET_COLOR);
    Registration* current = reg_head;
    int found_count = 0;
    
    // 先检查是否有匹配的结果
    Registration* temp = current;
    while (temp) {
        Student* student = find_student(temp->student_id);
        SportEvent* event = find_event(temp->event_id);
        if ((student && strstr(student->name, keyword)) || (event && strstr(event->name, keyword))) {
            found_count++;
        }
        temp = temp->next;
    }
    
    if (found_count == 0) {
        print_warning_message("未找到匹配的报名信息");
        return;
    }
    
    display_registration_table_header();
    while (current) {
        Student* student = find_student(current->student_id);
        SportEvent* event = find_event(current->event_id);
        if ((student && strstr(student->name, keyword)) || (event && strstr(event->name, keyword))) {
            display_registration_row(
                student ? student->name : "未知",
                event ? event->name : "未知",
                current->time,
                current->score >= 0 ? "已录入成绩" : "未录入成绩"
            );
        }
        current = current->next;
    }
    
    // 打印表格底部
    int col_widths[] = {15, 25, 20, 10};
    print_table_separator(4, col_widths);
    
    char result_msg[100];
    snprintf(result_msg, sizeof(result_msg), "找到 %d 条报名记录", found_count);
    print_info_message(result_msg);
}

// 报名管理菜单
void registration_management_menu() {
    int choice;
    do {
        clear_screen();
        display_registration_menu();
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            clear_screen();
            register_event();
            wait_for_enter();
            break;
        case 2:
            show_all_registrations();
            break;
        case 3:
            clear_screen();
            search_registration();
            wait_for_enter();
            break;
        case 4:
            clear_screen();
            cancel_registration();
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
