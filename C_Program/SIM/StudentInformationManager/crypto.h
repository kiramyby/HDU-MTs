#ifndef CRYPTO_H
#define CRYPTO_H

#include "sim_types.h"    // 包含所有共享类型和常量

#define MAX_USERNAME_LEN 64
#define MAX_PASSWORD_LEN 128
#define SALT_SIZE 32
#define HASH_SIZE 64
#define ITERATIONS 10000

typedef struct {
    BYTE salt[SALT_SIZE];
    BYTE hash[HASH_SIZE];
    DWORD iterations;
} PasswordRecord;

// 函数声明
BOOL GenerateSalt(BYTE* salt, DWORD length);
BOOL HashPassword(LPCWSTR password, PasswordRecord* record);
BOOL VerifyPassword(LPCWSTR attempt, PasswordRecord* stored);
BOOL SaveUserData(LPCTSTR username, PasswordRecord* record);
BOOL LoadUserData(LPCTSTR username, PasswordRecord* record);
void RegisterUser(void);
void LoginUser(void);

#endif /* CRYPTO_H */