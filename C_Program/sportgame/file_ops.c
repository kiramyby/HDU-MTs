#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "file_ops.h"

// 将所有运动项目保存到文件
void save_events_to_file() {
    FILE* fp = fopen("events.dat", "w");
    if (!fp) {
        perror("无法打开事件文件"); // 打印错误信息
        return;
    }
    SportEvent* current = event_head;
    while (current) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%d|%d\n",
            current->event_id,
            current->name,
            current->type,
            current->category,
            current->time,
            current->location,
            current->status,
            current->max_participants,
            current->current_participants); // 将每个项目的详细信息写入文件
        current = current->next;
    }
    fclose(fp);
}

// 从文件加载所有运动项目
void load_events_from_file() {
    FILE* fp = fopen("events.dat", "r");
    if (!fp) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        SportEvent* new_event = (SportEvent*)malloc(sizeof(SportEvent));
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d",
            new_event->event_id,
            new_event->name,
            new_event->type,
            new_event->category,
            new_event->time,
            new_event->location,
            new_event->status,
            &new_event->max_participants,
            &new_event->current_participants); // 从文件读取每个项目的详细信息
        new_event->next = event_head;
        event_head = new_event;
    }
    fclose(fp);
}

// 将所有学生信息保存到文件
void save_students_to_file() {
    FILE* fp = fopen("students.dat", "w");
    if (!fp) {
        perror("无法打开学生文件"); // 打印错误信息
        return;
    }
    Student* current = student_head;
    while (current) {
        fprintf(fp, "%s|%s|%s|%s|%s\n",
            current->student_id,
            current->name,
            current->gender,
            current->class_name,
            current->contact); // 将每个学生的详细信息写入文件
        current = current->next;
    }
    fclose(fp);
}

// 从文件加载所有学生信息
void load_students_from_file() {
    FILE* fp = fopen("students.dat", "r");
    if (!fp) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        Student* new_student = (Student*)malloc(sizeof(Student));
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|\n]",
            new_student->student_id,
            new_student->name,
            new_student->gender,
            new_student->class_name,
            new_student->contact); // 从文件读取每个学生的详细信息
        new_student->next = student_head;
        student_head = new_student;
    }
    fclose(fp);
}

// 将所有报名记录保存到文件
void save_registrations_to_file() {
    FILE* fp = fopen("registrations.dat", "w");
    if (!fp) {
        perror("无法打开报名文件"); // 打印错误信息
        return;
    }
    Registration* current = reg_head;
    while (current) {
        fprintf(fp, "%s|%s|%s|%.2f|%s\n",
            current->reg_id,
            current->student_id,
            current->event_id,
            current->score,
            current->time); // 将每个报名记录的详细信息写入文件
        current = current->next;
    }
    fclose(fp);
}

// 从文件加载所有报名记录
void load_registrations_from_file() {
    FILE* fp = fopen("registrations.dat", "r");
    if (!fp) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        Registration* new_reg = (Registration*)malloc(sizeof(Registration));
        sscanf(line, "%[^|]|%[^|]|%[^|]|%f|%[^|\n]",
            new_reg->reg_id,
            new_reg->student_id,
            new_reg->event_id,
            &new_reg->score,
            new_reg->time); // 从文件读取每个报名记录的详细信息
        new_reg->next = reg_head;
        reg_head = new_reg;
    }
    fclose(fp);
}
