#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "event_mgmt.h"
#include "student_mgmt.h"
#include "file_ops.h"
#include "utils.h"

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

    printf("\n--- 添加运动项目 ---\n");
    printf("项目编号: "); scanf("%9s", new_event->event_id); clear_input_buffer();
    printf("项目名称: "); fgets(new_event->name, sizeof(new_event->name), stdin); new_event->name[strcspn(new_event->name, "\n")] = '\0';
    printf("类型（田赛/径赛）: "); scanf("%19s", new_event->type); clear_input_buffer();
    printf("类别（男子/女子）: "); scanf("%19s", new_event->category); clear_input_buffer();
    printf("比赛时间: "); scanf("%19s", new_event->time); clear_input_buffer();
    printf("比赛地点: "); fgets(new_event->location, sizeof(new_event->location), stdin); new_event->location[strcspn(new_event->location, "\n")] = '\0';
    printf("最大报名人数: "); scanf("%d", &new_event->max_participants); clear_input_buffer();
    strcpy(new_event->status, "报名中");
    new_event->current_participants = 0;

    // 插入链表头部
    new_event->next = event_head;
    event_head = new_event;

    save_events_to_file(); // 保存更改到文件
    printf("项目添加成功！\n");
}

// 显示所有运动项目
void show_all_events() {
    printf("\n--- 所有运动项目 ---\n");
    SportEvent* current = event_head;
    while (current) {
        printf("[%s] %s (%s)\n", current->event_id, current->name, current->type);
        printf("类别: %s | 时间: %s | 地点: %s\n",
            current->category, current->time, current->location);
        printf("状态: %s | 报名人数: %d/%d\n\n",
            current->status, current->current_participants, current->max_participants);
        current = current->next;
    }
}

// 修改指定的运动项目
void modify_event() {
    char event_id[10];
    printf("\n--- 修改运动项目 ---\n");
    printf("输入要修改的项目编号: "); scanf("%9s", event_id); clear_input_buffer();

    SportEvent* event = find_event(event_id);
    if (!event) {
        printf("项目不存在！\n");
        return;
    }

    printf("新项目名称: "); fgets(event->name, sizeof(event->name), stdin); event->name[strcspn(event->name, "\n")] = '\0';
    printf("新类型（田赛/径赛）: "); scanf("%19s", event->type); clear_input_buffer();
    printf("新类别（男子/女子）: "); scanf("%19s", event->category); clear_input_buffer();
    printf("新比赛时间: "); scanf("%19s", event->time); clear_input_buffer();
    printf("新比赛地点: "); fgets(event->location, sizeof(event->location), stdin); event->location[strcspn(event->location, "\n")] = '\0';
    printf("新最大报名人数: "); scanf("%d", &event->max_participants); clear_input_buffer();

    save_events_to_file(); // 保存更改到文件
    printf("项目修改成功！\n");
}

// 查找包含关键字的运动项目
void search_event() {
    char keyword[50];
    printf("\n--- 查找运动项目 ---\n");
    printf("输入关键词: "); fgets(keyword, sizeof(keyword), stdin); keyword[strcspn(keyword, "\n")] = '\0';

    printf("\n查找结果:\n");
    SportEvent* current = event_head;
    while (current) {
        if (strstr(current->name, keyword) || strstr(current->type, keyword) ||
            strstr(current->category, keyword) || strstr(current->time, keyword) ||
            strstr(current->location, keyword)) {
            printf("[%s] %s (%s)\n", current->event_id, current->name, current->type);
            printf("类别: %s | 时间: %s | 地点: %s\n",
                current->category, current->time, current->location);
            printf("状态: %s | 报名人数: %d/%d\n\n",
                current->status, current->current_participants, current->max_participants);
        }
        current = current->next;
    }
}

// 录入某个项目的成绩
void enter_score() {
    char event_id[10];
    printf("\n--- 录入成绩 ---\n");
    printf("输入项目编号: "); scanf("%9s", event_id); clear_input_buffer();

    Registration* current = reg_head;
    while (current) {
        if (strcmp(current->event_id, event_id) == 0) {
            printf("学号: %s | 当前成绩: %.2f\n", current->student_id, current->score);
            printf("请输入新成绩: "); scanf("%f", &current->score); clear_input_buffer();
        }
        current = current->next;
    }

    save_registrations_to_file(); // 保存更改到文件
    printf("成绩录入成功！\n");
}

// 统计各项目报名人数
void count_participants_per_event() {
    printf("\n--- 各项目报名人数 ---\n");
    SportEvent* current = event_head;
    while (current) {
        printf("%-20s: %d/%d\n",
            current->name,
            current->current_participants,
            current->max_participants);
        current = current->next;
    }
}

// 查询比赛成绩
void query_scores() {
    char keyword[50];
    printf("\n--- 查询比赛成绩 ---\n");
    printf("输入关键词（学生姓名或项目名称）: "); fgets(keyword, sizeof(keyword), stdin); keyword[strcspn(keyword, "\n")] = '\0';

    printf("\n查询结果:\n");
    Registration* current = reg_head;
    while (current) {
        Student* student = find_student(current->student_id);
        SportEvent* event = find_event(current->event_id);
        if ((student && strstr(student->name, keyword)) || (event && strstr(event->name, keyword))) {
            printf("学号: %s | 姓名: %s | 项目编号: %s | 项目名称: %s | 成绩: %.2f\n",
                current->student_id, student ? student->name : "未知", current->event_id, event ? event->name : "未知", current->score);
        }
        current = current->next;
    }
}

// 运动项目管理菜单
void event_management_menu() {
    int choice;
    do {
        printf("\n=== 运动项目管理 ===\n");
        printf("1. 添加运动项目\n");
        printf("2. 显示所有项目\n");
        printf("3. 修改运动项目\n");
        printf("4. 查找运动项目\n");
        printf("0. 返回主菜单\n");
        printf("请选择操作: ");
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            add_event();
            break;
        case 2:
            show_all_events();
            break;
        case 3:
            modify_event();
            break;
        case 4:
            search_event();
            break;
        case 0:
            return;
        default:
            printf("无效选项！\n");
        }
    } while (1);
}
