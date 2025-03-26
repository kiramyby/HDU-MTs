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

/* 数据查询与处理函数声明 */
Student* findStudent(Student* students, const char* id);
Course* findCourse(Course* courses, const char* code);
Teacher* findTeacher(Teacher* teachers, const char* teacher_id);
void queryStudent(Student* students, Course* courses);
void queryCourse(Course* courses, Student* students);

/* 数据排序函数声明 */
void sortGrades(Grade** headRef, CompareFunc compare, int isStudentView);
void sortStudentGrades(Student* students);
void sortCourseGrades(Course* courses);
int compareByScoreDesc(const Grade* a, const Grade* b);

/* 创建数据函数声明 */
Student* createStudent(void);
Course* createCourse(void);
Teacher* createTeacher(void);
Grade* createGrade(Student* students, Course* courses);

/* 文件操作函数声明 */
Student* loadStudentsFromFile(void);
Course* loadCoursesFromFile(void);
Teacher* loadTeachersFromFile(void);
Grade* loadGradesFromFile(Student* students, Course* courses);
void saveStudentsToFile(Student* students);
void saveCoursesToFile(Course* courses);
void saveTeachersToFile(Teacher* teachers);
void saveGradesToFile(Grade* grades, Student* students);
void loadAllData(Student** students, Course** courses, Teacher** teachers, Grade** grades);
void saveAllData(Student* students, Course* courses, Teacher* teachers, Grade* grades);

/* 文件目录操作函数 */
void ensureDirectoryExists(const char* path);
void ensureAllDirectoriesExist(void);

/* 权限管理函数 */
BOOL IsAdmin(int uid);
BOOL IsTeacher(int uid);
BOOL IsStudent(int uid);
BOOL HasAccessToCourse(int uid, const char* course_code);
void DisplayRelevantCourses(int uid, Course* courses, int startY);
void ShowPermissionDeniedMessage(int leftStart, const char* operation);

/* 日志相关函数声明 */
void LogSystemActivity(const char* action, const char* details);
void WriteLogToFile(const char* logEntry);
void ShowRecentLogEntries(int maxEntries);
char* WriteLogFileName(void); // 添加获取日志文件名的函数

#endif /* SIM_H */
