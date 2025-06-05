#ifndef STUDENT_MGMT_H
#define STUDENT_MGMT_H

#include "types.h"

// 学生管理函数声明
Student* find_student(const char* student_id);
void add_student(void);
void show_all_students(void);
void modify_student(void);
void search_student(void);
void student_management_menu(void);
void count_registrations_per_student(void);

#endif
