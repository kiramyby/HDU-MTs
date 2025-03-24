#ifndef SIM_TYPES_H
#define SIM_TYPES_H

#include <Windows.h>

/* 通用常量定义 */
#define MAX_USERNAME_LEN 64
#define MAX_PASSWORD_LEN 128
#define MAX_COURSE 5
#define MAX_STUDENT 100

/* UI 相关常量 */
#define LEFT_HEAD 30
#define RIGHT_HEAD 110
#define UP_HEAD 10
#define DOWN_HEAD 110

/* 加密相关常量 */
#define SALT_SIZE 32
#define HASH_SIZE 64
#define ITERATIONS 10000

/* 基本类型定义 */
typedef int User;  // -1 Guest \ 1[5] Admin \ 2[5] Teacher \ 3[5] Student

/* 数据结构定义 */
typedef struct Grade Grade;
typedef struct Student Student;
typedef struct Course Course;

// 学生结构体
struct Student {
    char id[12];             
    char name[20];
    Grade* grades;   
    struct Student* next;
};

// 课程结构体
struct Course {
    char code[10];           
    char name[50];
    Grade* grades;    
    struct Course* next;
};

// 成绩结构体
struct Grade {
    char student_id[12];     
    char course_code[10];    
    float score;
    struct Grade* next_student;
    struct Grade* next_course;  
};

// 密码记录结构体
typedef struct {
    BYTE salt[SALT_SIZE];
    BYTE hash[HASH_SIZE];
    DWORD iterations;
} PasswordRecord;

// 比较函数类型
typedef int (*CompareFunc)(const Grade*, const Grade*);

#endif /* SIM_TYPES_H */