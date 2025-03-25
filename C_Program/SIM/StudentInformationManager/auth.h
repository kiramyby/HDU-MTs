#ifndef CRYPTO_H
#define CRYPTO_H

#include <Windows.h>
#include <bcrypt.h>
#include <wchar.h>
#include "sim_types.h"

/* 密码系统相关常量 */
#define MAX_USERNAME_LEN 64
#define MAX_PASSWORD_LEN 128
#define SALT_SIZE 32
#define HASH_SIZE 64
#define ITERATIONS 10000

/* 密码记录结构体 */
typedef struct {
    BYTE salt[SALT_SIZE];
    BYTE hash[HASH_SIZE];
    DWORD iterations;
} PasswordRecord;

/* 用户记录结构体 */
typedef struct {
    PasswordRecord password;
    int userType;            // 用户类型: 15=管理员, 25=教师, 35=学生, -1=访客
    char id[MAX_ID_LEN];     // 用户ID，主键
    char username[MAX_USERNAME_LEN];  // 用户名显示
} UserRecord;

/* 函数声明 */
BOOL GenerateSalt(BYTE* salt, DWORD length);
BOOL HashPassword(const wchar_t* password, PasswordRecord* record);
BOOL VerifyPassword(const wchar_t* attempt, PasswordRecord* stored);

// 修改为使用ID作为主键
BOOL SaveUserData(const char* id, PasswordRecord* record, int userType, const char* username);
BOOL LoadUserData(const char* id, PasswordRecord* record, int* userType, char* username);
BOOL GetUserByUsername(const char* username, char* id);

// 用户界面函数
void RegisterUser(void);
int LoginUser(void);
int AuthenticationProcess(void);
UserRecord* GetCurrentUser(void);

// 教师管理函数
BOOL AssignCourseToTeacher(const char* teacherId, const char* courseCode);
BOOL RemoveCourseFromTeacher(const char* teacherId, const char* courseCode);
BOOL IsTeacherAssignedToCourse(const char* teacherId, const char* courseCode);

#endif /* CRYPTO_H */