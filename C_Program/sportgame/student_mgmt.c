#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "student_mgmt.h"
#include "file_ops.h"
#include "utils.h"

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

    printf("\n--- 添加学生信息 ---\n");
    printf("学号: "); scanf("%14s", new_student->student_id); clear_input_buffer();
    printf("姓名: "); fgets(new_student->name, sizeof(new_student->name), stdin); new_student->name[strcspn(new_student->name, "\n")] = '\0';

    new_student->next = student_head;
    student_head = new_student;

    // 保存到文件
    save_students_to_file();
    printf("学生信息添加成功！\n");
}

// 显示所有学生信息
void show_all_students() {
    printf("\n--- 所有学生信息 ---\n");
    Student* current = student_head;
    while (current) {
        printf("学号: %s | 姓名: %s\n", current->student_id, current->name);
        current = current->next;
    }
}

// 修改指定的学生信息
void modify_student() {
    char student_id[15];
    printf("\n--- 修改学生信息 ---\n");
    printf("输入要修改的学生学号: "); scanf("%14s", student_id); clear_input_buffer();

    Student* student = find_student(student_id);
    if (!student) {
        printf("学生不存在！\n");
        return;
    }

    printf("新姓名: "); fgets(student->name, sizeof(student->name), stdin); student->name[strcspn(student->name, "\n")] = '\0';

    save_students_to_file(); // 保存更改到文件
    printf("学生信息修改成功！\n");
}

// 查找包含关键字的学生信息
void search_student() {
    char keyword[50];
    printf("\n--- 查找学生信息 ---\n");
    printf("输入关键词: "); fgets(keyword, sizeof(keyword), stdin); keyword[strcspn(keyword, "\n")] = '\0';

    printf("\n查找结果:\n");
    Student* current = student_head;
    while (current) {
        if (strstr(current->name, keyword) || strstr(current->student_id, keyword)) {
            printf("学号: %s | 姓名: %s\n", current->student_id, current->name);
        }
        current = current->next;
    }
}

// 统计每个学生的报名数量
void count_registrations_per_student() {
    printf("\n--- 每个学生的报名数量 ---\n");
    Student* current_student = student_head;
    while (current_student) {
        int count = 0;
        Registration* current_reg = reg_head;
        while (current_reg) {
            if (strcmp(current_reg->student_id, current_student->student_id) == 0) {
                count++;
            }
            current_reg = current_reg->next;
        }
        printf("学号: %s | 姓名: %s | 报名数量: %d\n", current_student->student_id, current_student->name, count);
        current_student = current_student->next;
    }
}

// 学生信息管理菜单
void student_management_menu() {
    int choice;
    do {
        printf("\n=== 学生信息管理 ===\n");
        printf("1. 添加学生信息\n");
        printf("2. 显示所有学生信息\n");
        printf("3. 修改学生信息\n");
        printf("4. 查找学生信息\n");
        printf("0. 返回主菜单\n");
        printf("请选择操作: ");
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            add_student();
            break;
        case 2:
            show_all_students();
            break;
        case 3:
            modify_student();
            break;
        case 4:
            search_student();
            break;
        case 0:
            return;
        default:
            printf("无效选项！\n");
        }
    } while (1);
}
