#ifndef SIM_H
#define SIM_H

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <string.h>
#include <tchar.h>
#include <conio.h> // 添加对_getch()的支持

#include "sim_types.h"

// 只保留函数声明，移除类型定义
/* 主要功能函数声明 */
int Login(void);
int Menu(void);
void Create(int uid);
void Read(int uid);
void Update(int uid);
void Delete(int uid);
void Logging(int uid);
void Logout(int uid);
void Window(void);
void Final(void);

/* UI 相关函数声明 */
void HideCursor(void);
void GotoXY(int x, int y);

/* 数据处理函数声明 */
void sortGrades(Grade** headRef, CompareFunc compare, int isStudentView);
void queryStudent(Student* students);
void queryCourse(Course* courses);
void sortStudentGrades(Student* students);
void sortCourseGrades(Course* courses);
Student* findStudent(Student* students, const char* id);
Course* findCourse(Course* courses, const char* code);
int compareByScoreDesc(const Grade* a, const Grade* b);

/* 数据存储函数声明 */
void serialize(Student* students, Course* courses, Grade* grades, const char* filename);
void deserialize(Student** students, Course** courses, Grade** grades, const char* filename);

/* 数据校验函数声明 */
unsigned char calculateChecksum(FILE* fp);
void writeChecksum(FILE* fp);
int verifyChecksum(FILE* fp);

/* 输入缓冲区处理函数声明 */
void FlushInputBuffer(void);

/* 文件目录操作函数 */
void ensureDirectoryExists(const char* path);
void ensureAllDirectoriesExist(void);

#endif /* SIM_H */
