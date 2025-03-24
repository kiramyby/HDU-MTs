#ifndef CRYPTO_H
#define CRYPTO_H

#include <Windows.h>
#include <bcrypt.h>
#include <wchar.h>

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
    int userType;  // 用户类型: 15=管理员, 25=教师, 35=学生, -1=访客
    char username[MAX_USERNAME_LEN];
    char id[MAX_USERNAME_LEN];
} UserRecord;

/* 函数声明 */
BOOL GenerateSalt(BYTE* salt, DWORD length);
BOOL HashPassword(const wchar_t* password, PasswordRecord* record);
BOOL VerifyPassword(const wchar_t* attempt, PasswordRecord* stored);
BOOL SaveUserData(const char* username, PasswordRecord* record, int userType);
BOOL LoadUserData(const char* username, PasswordRecord* record, int* userType);
void RegisterUser(void);
int LoginUser(void);
int AuthenticationProcess(void);
UserRecord* GetCurrentUser(void);

#endif /* CRYPTO_H */