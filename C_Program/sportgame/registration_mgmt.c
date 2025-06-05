#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "registration_mgmt.h"
#include "student_mgmt.h"
#include "event_mgmt.h"
#include "file_ops.h"
#include "utils.h"

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
    printf("\n--- 项目报名 ---\n");
    printf("输入学号: "); scanf("%14s", stu_id); clear_input_buffer();
    printf("输入项目编号: "); scanf("%9s", event_id); clear_input_buffer();

    Student* stu = find_student(stu_id);
    SportEvent* event = find_event(event_id);

    if (!stu) {
        printf("学生不存在！\n");
        return;
    }
    if (!event) {
        printf("项目不存在！\n");
        return;
    }

    if (strcmp(event->status, "报名中") != 0) {
        printf("该项目已结束报名！\n");
        return;
    }

    if (event->current_participants >= event->max_participants) {
        printf("该项目已报满！\n");
        return;
    }

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
    printf("报名成功！报名编号：%s\n", new_reg->reg_id);
}

// 取消某个报名记录
void cancel_registration() {
    char reg_id[20];
    printf("\n--- 取消报名 ---\n");
    printf("输入报名编号: "); scanf("%19s", reg_id); clear_input_buffer();

    Registration* prev = NULL;
    Registration* current = reg_head;
    while (current) {
        if (strcmp(current->reg_id, reg_id) == 0) {
            // 更新项目报名人数
            SportEvent* event = find_event(current->event_id);
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
            printf("报名取消成功！\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("报名编号不存在！\n");
}

// 显示所有报名信息
void show_all_registrations() {
    printf("\n--- 所有报名信息 ---\n");
    Registration* current = reg_head;
    while (current) {
        printf("报名编号: %s | 学号: %s | 项目编号: %s | 成绩: %.2f\n",
            current->reg_id, current->student_id, current->event_id, current->score);
        current = current->next;
    }
}

// 查找包含关键字的报名信息
void search_registration() {
    char keyword[50];
    printf("\n--- 查找报名信息 ---\n");
    printf("输入关键词（学生姓名或项目名称）: "); fgets(keyword, sizeof(keyword), stdin); keyword[strcspn(keyword, "\n")] = '\0';

    printf("\n查找结果:\n");
    Registration* current = reg_head;
    while (current) {
        Student* student = find_student(current->student_id);
        SportEvent* event = find_event(current->event_id);
        if ((student && strstr(student->name, keyword)) || (event && strstr(event->name, keyword))) {
            printf("报名编号: %s | 学号: %s | 项目编号: %s | 成绩: %.2f\n",
                current->reg_id, current->student_id, current->event_id, current->score);
        }
        current = current->next;
    }
}

// 报名管理菜单
void registration_management_menu() {
    int choice;
    do {
        printf("\n=== 报名管理 ===\n");
        printf("1. 项目报名\n");
        printf("2. 取消报名\n");
        printf("3. 显示所有报名信息\n");
        printf("4. 查找报名信息\n");
        printf("0. 返回主菜单\n");
        printf("请选择操作: ");
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            register_event();
            break;
        case 2:
            cancel_registration();
            break;
        case 3:
            show_all_registrations();
            break;
        case 4:
            search_registration();
            break;
        case 0:
            return;
        default:
            printf("无效选项！\n");
        }
    } while (1);
}
