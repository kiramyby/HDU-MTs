#include "crypto.h"

#pragma comment(lib, "bcrypt.lib")

// 生成安全盐值
BOOL GenerateSalt(BYTE* salt, DWORD length) {
    return BCryptGenRandom(
        NULL,
        salt,
        length,
        BCRYPT_USE_SYSTEM_PREFERRED_RNG
    ) == 0;
}

// 密码哈希生成
BOOL HashPassword(LPCWSTR password, PasswordRecord* record) {
    if (!GenerateSalt(record->salt, SALT_SIZE)) return FALSE;
    
    record->iterations = ITERATIONS;

    return BCryptDeriveKeyPBKDF2(
        BCRYPT_SHA512_ALG_HANDLE,
        (PUCHAR)password,
        (ULONG)(wcslen(password) * sizeof(WCHAR)),
        record->salt,
        SALT_SIZE,
        ITERATIONS,
        record->hash,
        HASH_SIZE,
        0
    ) == 0;
}

// 密码验证
BOOL VerifyPassword(LPCWSTR attempt, PasswordRecord* stored) {
    BYTE attemptHash[HASH_SIZE];

    NTSTATUS status = BCryptDeriveKeyPBKDF2(
        BCRYPT_SHA512_ALG_HANDLE,
        (PUCHAR)attempt,
        (ULONG)(wcslen(attempt) * sizeof(WCHAR)),
        stored->salt,
        SALT_SIZE,
        stored->iterations,
        attemptHash,
        HASH_SIZE,
        0
    );

    if (status != 0) return FALSE;

    return memcmp(stored->hash, attemptHash, HASH_SIZE) == 0;
}

// 文件存储操作
BOOL SaveUserData(LPCTSTR username, PasswordRecord* record) {
    TCHAR filename[MAX_PATH];
    _stprintf_s(filename, MAX_PATH, _T("%s.pass"), username);

    HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, 
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    DWORD written;
    BOOL success = WriteFile(hFile, record, sizeof(PasswordRecord), &written, NULL);
    CloseHandle(hFile);
    return success && (written == sizeof(PasswordRecord));
}

BOOL LoadUserData(LPCTSTR username, PasswordRecord* record) {
    TCHAR filename[MAX_PATH];
    _stprintf_s(filename, MAX_PATH, _T("%s.pass"), username);

    HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, 
                            OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    DWORD read;
    BOOL success = ReadFile(hFile, record, sizeof(PasswordRecord), &read, NULL);
    CloseHandle(hFile);
    return success && (read == sizeof(PasswordRecord));
}

// 用户注册
void RegisterUser() {
    TCHAR username[MAX_USERNAME_LEN];
    WCHAR password[MAX_PASSWORD_LEN];
    PasswordRecord record;

    _tprintf(_T("\n=== 注册 ===\n"));
    _tprintf(_T("用户名: "));
    _getts_s(username, MAX_USERNAME_LEN);

    if (LoadUserData(username, &record)) {
        _tprintf(_T("用户名已存在！\n"));
        return;
    }

    _tprintf(_T("密码: "));
    _getts_s(password, MAX_PASSWORD_LEN);

    if (HashPassword(password, &record)) {
        if (SaveUserData(username, &record)) {
            _tprintf(_T("注册成功！\n"));
        } else {
            _tprintf(_T("保存失败！\n"));
        }
    } else {
        _tprintf(_T("密码处理失败！\n"));
    }
}

// 用户登录
void LoginUser() {
    TCHAR username[MAX_USERNAME_LEN];
    WCHAR password[MAX_PASSWORD_LEN];
    PasswordRecord record;

    _tprintf(_T("\n=== 登录 ===\n"));
    _tprintf(_T("用户名: "));
    _getts_s(username, MAX_USERNAME_LEN);

    if (!LoadUserData(username, &record)) {
        _tprintf(_T("用户不存在！\n"));
        return;
    }

    _tprintf(_T("密码: "));
    _getts_s(password, MAX_PASSWORD_LEN);

    if (VerifyPassword(password, &record)) {
        _tprintf(_T("登录成功！\n"));
    } else {
        _tprintf(_T("密码错误！\n"));
    }
}

int _tmain() {
    int choice;

    while (1) {
        _tprintf(_T("\n=== 主菜单 ===\n"));
        _tprintf(_T("1. 登录\n"));
        _tprintf(_T("2. 注册\n"));
        _tprintf(_T("3. 退出\n"));
        _tprintf(_T("请选择: "));
        _tscanf_s(_T("%d"), &choice);
        fflush(stdin);

        switch (choice) {
        case 1:
            LoginUser();
            break;
        case 2:
            RegisterUser();
            break;
        case 3:
            return 0;
        default:
            _tprintf(_T("无效选项！\n"));
        }
    }
}