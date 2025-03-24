#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <string.h>

#define MAX_COURSE 5
#define MAX_STUDENT 100
#define LEFT_HEAD 30
#define RIGHT_HEAD 110
#define UP_HEAD 10
#define DOWN_HEAD 110


/* ================== 数据结构定义 ================== */

// 用户类型
typedef int User;	// -1 Guest \ 1[5] Admin \ 2[5] Teacher \ 3[5] Student

// 学生结构体
typedef struct Student {
    char id[12];             
    char name[20];
    struct Grade* grades;   
    struct Student* next;
} Student;

// 课程结构体
typedef struct Course {
    char code[10];           
    char name[50];
    struct Grade* grades;    
    struct Course* next;
} Course;

// 成绩结构体
typedef struct Grade {
    char student_id[12];     
    char course_code[10];    
    float score;
    struct Grade* next_student;
    struct Grade* next_course;  
} Grade;

typedef int (*CompareFunc)(const Grade*, const Grade*);

/* ================== 函数声明 ================== */

int Login();
int Menu();
void Create(int uid);
void Read(int uid);
void Update(int uid);
void Delete(int uid);
void Logging(int uid);
void InputScore();
void Window();
void Final();

void HideCursor();
void GotoXY(int x, int y);

void sortGrades(Grade** headRef, CompareFunc compare, int isStudentView);
void queryStudent(Student* students);
void queryCourse(Course* courses);
void sortStudentGrades(Student* students);
void sortCourseGrades(Course* courses);
Student* findStudent(Student* students, const char* id);
Course* findCourse(Course* courses, const char* code);
int compareByScoreDesc(const Grade* a, const Grade* b);
