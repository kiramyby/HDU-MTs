#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "event_mgmt.h"
#include "student_mgmt.h"
#include "file_ops.h"
#include "utils.h"
#include "display.h"

// 根据项目编号查找项目
SportEvent* find_event(const char* event_id) {
    SportEvent* current = event_head;
    while (current) {
        if (strcmp(current->event_id, event_id) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

// 添加新的运动项目
void add_event() {
    SportEvent* new_event = (SportEvent*)malloc(sizeof(SportEvent));

    print_header("➕ 添加运动项目");
    printf("%s项目编号: %s", CYAN, RESET_COLOR); 
    scanf("%9s", new_event->event_id); clear_input_buffer();
    printf("%s项目名称: %s", CYAN, RESET_COLOR); 
    fgets(new_event->name, sizeof(new_event->name), stdin); 
    new_event->name[strcspn(new_event->name, "\n")] = '\0';
    printf("%s类型（田赛/径赛）: %s", CYAN, RESET_COLOR); 
    scanf("%19s", new_event->type); clear_input_buffer();
    printf("%s类别（男子/女子）: %s", CYAN, RESET_COLOR); 
    scanf("%19s", new_event->category); clear_input_buffer();
    printf("%s比赛时间: %s", CYAN, RESET_COLOR); 
    scanf("%19s", new_event->time); clear_input_buffer();
    printf("%s比赛地点: %s", CYAN, RESET_COLOR); 
    fgets(new_event->location, sizeof(new_event->location), stdin); 
    new_event->location[strcspn(new_event->location, "\n")] = '\0';
    printf("%s最大报名人数: %s", CYAN, RESET_COLOR); 
    scanf("%d", &new_event->max_participants); clear_input_buffer();
    strcpy(new_event->status, "报名中");
    new_event->current_participants = 0;

    // 插入链表头部
    new_event->next = event_head;
    event_head = new_event;

    save_events_to_file(); // 保存更改到文件
    print_success_message("项目添加成功！");
}

// 显示所有运动项目
void show_all_events() {
    clear_screen();
    print_header("🏆 所有运动项目");
    
    SportEvent* current = event_head;
    if (!current) {
        print_warning_message("暂无运动项目数据");
        wait_for_enter();
        return;
    }
    
    // 统计总数
    int total_count = 0;
    SportEvent* temp = current;
    while (temp) {
        total_count++;
        temp = temp->next;
    }
    
    const int PAGE_SIZE = 8; // 每页显示8条记录
    int total_pages = (total_count + PAGE_SIZE - 1) / PAGE_SIZE;
    int current_page = 1;
    
    while (current_page <= total_pages) {
        clear_screen();
        print_header("🏆 所有运动项目");
        
        printf("%s%s总共 %d 个项目%s\n", BLUE, BOLD, total_count, RESET_COLOR);
        
        display_event_table_header();
        
        // 显示当前页的数据
        SportEvent* page_current = event_head;
        int page_displayed = 0;
        
        // 跳到当前页的起始位置
        for (int i = 0; i < (current_page - 1) * PAGE_SIZE && page_current; i++) {
            page_current = page_current->next;
        }
        
        // 显示当前页的记录
        while (page_current && page_displayed < PAGE_SIZE) {
            display_event_row(page_current);
            page_current = page_current->next;
            page_displayed++;
        }
        
        // 打印表格底部
        int col_widths[] = {10, 20, 8, 8, 12, 15, 8, 12};
        print_table_separator(8, col_widths);
        
        // 显示分页信息
        display_page_info(current_page, total_pages);
        
        // 处理用户输入
        printf("\n%s输入: %s", CYAN, RESET_COLOR);
        char input = getchar();
        while (getchar() != '\n'); // 清除输入缓冲区
        
        if (input == 'q' || input == 'Q') {
            break;
        } else if (input == '\n' || input == ' ') {
            current_page++;
        }
    }
    
    print_info_message("运动项目查看完成");
}

// 修改指定的运动项目
void modify_event() {
    char event_id[10];
    print_header("✏️ 修改运动项目");
    printf("%s输入要修改的项目编号: %s", CYAN, RESET_COLOR); 
    scanf("%9s", event_id); clear_input_buffer();

    SportEvent* event = find_event(event_id);
    if (!event) {
        print_error_message("项目不存在！");
        return;
    }

    printf("\n%s当前项目信息:%s\n", BOLD, RESET_COLOR);
    display_event_table_header();
    display_event_row(event);
    int col_widths[] = {10, 20, 8, 8, 12, 15, 8, 12};
    print_table_separator(8, col_widths);

    printf("\n%s请输入新信息:%s\n", BOLD, RESET_COLOR);
    printf("%s新项目名称: %s", CYAN, RESET_COLOR); 
    fgets(event->name, sizeof(event->name), stdin); 
    event->name[strcspn(event->name, "\n")] = '\0';
    printf("%s新类型（田赛/径赛）: %s", CYAN, RESET_COLOR); 
    scanf("%19s", event->type); clear_input_buffer();
    printf("%s新类别（男子/女子）: %s", CYAN, RESET_COLOR); 
    scanf("%19s", event->category); clear_input_buffer();
    printf("%s新比赛时间: %s", CYAN, RESET_COLOR); 
    scanf("%19s", event->time); clear_input_buffer();
    printf("%s新比赛地点: %s", CYAN, RESET_COLOR); 
    fgets(event->location, sizeof(event->location), stdin); 
    event->location[strcspn(event->location, "\n")] = '\0';
    printf("%s新最大报名人数: %s", CYAN, RESET_COLOR); 
    scanf("%d", &event->max_participants); clear_input_buffer();

    save_events_to_file(); // 保存更改到文件
    print_success_message("项目修改成功！");
}

// 查找包含关键字的运动项目
void search_event() {
    char keyword[50];
    print_header("🔍 查找运动项目");
    printf("%s输入关键词: %s", CYAN, RESET_COLOR); 
    fgets(keyword, sizeof(keyword), stdin); 
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("\n%s查找结果:%s\n", BOLD, RESET_COLOR);
    SportEvent* current = event_head;
    int found_count = 0;
    
    // 先检查是否有匹配的结果
    SportEvent* temp = current;
    while (temp) {
        if (strstr(temp->name, keyword) || strstr(temp->type, keyword) ||
            strstr(temp->category, keyword) || strstr(temp->time, keyword) ||
            strstr(temp->location, keyword)) {
            found_count++;
        }
        temp = temp->next;
    }
    
    if (found_count == 0) {
        print_warning_message("未找到匹配的运动项目");
        return;
    }
    
    display_event_table_header();
    while (current) {
        if (strstr(current->name, keyword) || strstr(current->type, keyword) ||
            strstr(current->category, keyword) || strstr(current->time, keyword) ||
            strstr(current->location, keyword)) {
            display_event_row(current);
        }
        current = current->next;
    }
    
    // 打印表格底部
    int col_widths[] = {10, 20, 8, 8, 12, 15, 8, 12};
    print_table_separator(8, col_widths);
    
    char result_msg[100];
    snprintf(result_msg, sizeof(result_msg), "找到 %d 个匹配项目", found_count);
    print_info_message(result_msg);
}

// 录入某个项目的成绩
void enter_score() {
    char event_id[10];
    print_header("📊 录入成绩");
    printf("%s输入项目编号: %s", CYAN, RESET_COLOR); 
    scanf("%9s", event_id); clear_input_buffer();

    SportEvent* event = find_event(event_id);
    if (!event) {
        print_error_message("项目不存在！");
        return;
    }

    printf("\n%s项目: %s%s (%s)%s\n", BOLD, GREEN, event->name, event->event_id, RESET_COLOR);
    print_separator();

    Registration* current = reg_head;
    int participant_count = 0;
    
    while (current) {
        if (strcmp(current->event_id, event_id) == 0) {
            Student* student = find_student(current->student_id);
            printf("%s学号: %s%s | %s姓名: %s%s | %s当前成绩: %s%.2f%s\n", 
                   CYAN, RESET_COLOR, current->student_id,
                   CYAN, RESET_COLOR, student ? student->name : "未知",
                   CYAN, RESET_COLOR, current->score, RESET_COLOR);
            printf("%s请输入新成绩: %s", YELLOW, RESET_COLOR); 
            scanf("%f", &current->score); clear_input_buffer();
            participant_count++;
        }
        current = current->next;
    }

    if (participant_count == 0) {
        print_warning_message("该项目暂无报名学生");
        return;
    }

    save_registrations_to_file(); // 保存更改到文件
    char success_msg[100];
    snprintf(success_msg, sizeof(success_msg), "成功录入 %d 名学生的成绩", participant_count);
    print_success_message(success_msg);
}

// 统计各项目报名人数
void count_participants_per_event() {
    print_header("📈 各项目报名人数统计");
    
    SportEvent* current = event_head;
    if (!current) {
        print_warning_message("暂无运动项目数据");
        return;
    }

    const char* headers[] = {"项目名称", "报名人数", "最大人数", "占用率"};
    int col_widths[] = {25, 12, 12, 12};
    print_table_header(headers, 4, col_widths);
    
    while (current) {
        char participants[20], max_participants[20], usage_rate[20];
        snprintf(participants, sizeof(participants), "%d", current->current_participants);
        snprintf(max_participants, sizeof(max_participants), "%d", current->max_participants);
        
        float rate = (current->max_participants > 0) ? 
                     (float)current->current_participants / current->max_participants * 100 : 0;
        snprintf(usage_rate, sizeof(usage_rate), "%.1f%%", rate);
        
        const char* data[] = {current->name, participants, max_participants, usage_rate};
        print_table_row(data, 4, col_widths);
        current = current->next;
    }
    
    print_table_separator(4, col_widths);
    print_info_message("统计数据显示完成");
}

// 查询比赛成绩
void query_scores() {
    char keyword[50];
    print_header("🏆 查询比赛成绩");
    printf("%s输入关键词（学生姓名或项目名称）: %s", CYAN, RESET_COLOR); 
    fgets(keyword, sizeof(keyword), stdin); 
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("\n%s查询结果:%s\n", BOLD, RESET_COLOR);
    
    const char* headers[] = {"学号", "姓名", "项目编号", "项目名称", "成绩"};
    int col_widths[] = {12, 15, 10, 20, 10};
    
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
        print_warning_message("未找到匹配的成绩记录");
        return;
    }
    
    print_table_header(headers, 5, col_widths);
    
    while (current) {
        Student* student = find_student(current->student_id);
        SportEvent* event = find_event(current->event_id);
        if ((student && strstr(student->name, keyword)) || (event && strstr(event->name, keyword))) {
            char score_str[20];
            snprintf(score_str, sizeof(score_str), "%.2f", current->score);
            
            const char* data[] = {
                current->student_id, 
                student ? student->name : "未知", 
                current->event_id, 
                event ? event->name : "未知", 
                score_str
            };
            print_table_row(data, 5, col_widths);
        }
        current = current->next;
    }
    
    print_table_separator(5, col_widths);
    
    char result_msg[100];
    snprintf(result_msg, sizeof(result_msg), "找到 %d 条成绩记录", found_count);
    print_info_message(result_msg);
}

// 运动项目管理菜单
void event_management_menu() {
    int choice;
    do {
        clear_screen();
        display_event_menu();
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            clear_screen();
            add_event();
            wait_for_enter();
            break;
        case 2:
            show_all_events();
            break;
        case 3:
            clear_screen();
            modify_event();
            wait_for_enter();
            break;
        case 4:
            clear_screen();
            search_event();
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
