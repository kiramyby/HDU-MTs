#ifndef SIM_TYPES_H
#define SIM_TYPES_H

#include <Windows.h>

/* 通用常量定义 */
#define MAX_USERNAME_LEN 64
#define MAX_PASSWORD_LEN 128
#define MAX_ID_LEN 12
#define MAX_COURSE 5
#define MAX_STUDENT 100
#define MAX_TEACHER 20
#define MAX_TEACHER_COURSES 10

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
typedef struct Teacher Teacher;
typedef struct TeacherCourse TeacherCourse;

// 学生结构体
struct Student {
    char id[MAX_ID_LEN];             
    char name[20];
    Grade* grades;   
    struct Student* next;
};

// 课程结构体
struct Course {
    char code[10];           
    char name[50];
    Grade* grades;
    Teacher* teacher;  // 指向负责该课程的教师
    struct Course* next;
};

// 教师结构体
struct Teacher {
    char id[MAX_ID_LEN];      // 教师ID
    char name[20];            // 教师姓名
    TeacherCourse* courses;   // 教师负责的课程列表
    struct Teacher* next;     // 下一个教师
};

// 教师-课程关联结构体（多对多关系）
struct TeacherCourse {
    char course_code[10];            // 课程代码
    struct TeacherCourse* next;      // 下一个关联课程
};

// 成绩结构体
struct Grade {
    char student_id[MAX_ID_LEN];     
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