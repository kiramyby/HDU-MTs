#include "auth.h"
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "bcrypt.lib")

// 当前用户信息
static UserRecord currentUser = { {0}, -1, "", "" };

// 获取当前用户信息
UserRecord* GetCurrentUser() {
    return &currentUser;
}

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
BOOL HashPassword(const wchar_t* password, PasswordRecord* record) {
    if (!GenerateSalt(record->salt, SALT_SIZE)) return FALSE;
    
    record->iterations = ITERATIONS;

    return BCryptDeriveKeyPBKDF2(
        BCRYPT_SHA512_ALG_HANDLE,
        (PUCHAR)password,
        (ULONG)(wcslen(password) * sizeof(wchar_t)),
        record->salt,
        SALT_SIZE,
        ITERATIONS,
        record->hash,
        HASH_SIZE,
        0
    ) == 0;
}

// 密码验证
BOOL VerifyPassword(const wchar_t* attempt, PasswordRecord* stored) {
    BYTE attemptHash[HASH_SIZE];

    NTSTATUS status = BCryptDeriveKeyPBKDF2(
        BCRYPT_SHA512_ALG_HANDLE,
        (PUCHAR)attempt,
        (ULONG)(wcslen(attempt) * sizeof(wchar_t)),
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

// 文件存储操作 - 按ID存储
BOOL SaveUserData(const char* id, PasswordRecord* record, int userType, const char* username) {
    char filename[MAX_PATH];
    snprintf(filename, MAX_PATH, "%s.user", id);

    FILE* file = fopen(filename, "wb");
    if (!file) return FALSE;

    // 扩展用户记录以包含ID、用户名和类型
    UserRecord userRecord;
    strncpy(userRecord.id, id, MAX_ID_LEN-1);
    userRecord.id[MAX_ID_LEN-1] = '\0';
    
    strncpy(userRecord.username, username, MAX_USERNAME_LEN-1);
    userRecord.username[MAX_USERNAME_LEN-1] = '\0';
    
    memcpy(&userRecord.password, record, sizeof(PasswordRecord));
    userRecord.userType = userType;

    size_t written = fwrite(&userRecord, sizeof(UserRecord), 1, file);
    fclose(file);
    
    // 创建用户名到ID的映射文件
    char mapFilename[MAX_PATH];
    snprintf(mapFilename, MAX_PATH, "%s.map", username);
    
    FILE* mapFile = fopen(mapFilename, "wb");
    if (!mapFile) return FALSE;
    
    size_t mapWritten = fwrite(id, sizeof(char), MAX_ID_LEN, mapFile);
    fclose(mapFile);
    
    return (written == 1 && mapWritten == MAX_ID_LEN);
}

// 通过ID加载用户数据
BOOL LoadUserData(const char* id, PasswordRecord* record, int* userType, char* username) {
    char filename[MAX_PATH];
    snprintf(filename, MAX_PATH, "%s.user", id);

    FILE* file = fopen(filename, "rb");
    if (!file) return FALSE;

    UserRecord userRecord;
    size_t read = fread(&userRecord, sizeof(UserRecord), 1, file);
    fclose(file);
    
    if (read == 1) {
        memcpy(record, &userRecord.password, sizeof(PasswordRecord));
        *userType = userRecord.userType;
        if (username) {
            strncpy(username, userRecord.username, MAX_USERNAME_LEN);
        }
        return TRUE;
    }
    return FALSE;
}

// 通过用户名获取ID
BOOL GetUserByUsername(const char* username, char* id) {
    char mapFilename[MAX_PATH];
    snprintf(mapFilename, MAX_PATH, "%s.map", username);
    
    FILE* mapFile = fopen(mapFilename, "rb");
    if (!mapFile) return FALSE;
    
    size_t read = fread(id, sizeof(char), MAX_ID_LEN, mapFile);
    fclose(mapFile);
    
    return (read == MAX_ID_LEN);
}

// 用户注册
void RegisterUser() {
    char username[MAX_USERNAME_LEN];
    char id[MAX_ID_LEN];
    wchar_t password[MAX_PASSWORD_LEN];
    PasswordRecord record;
    int userType = 35;  // 默认注册为学生
    int choice;

    printf("\n=== 注册 ===\n");
    
    printf("用户名: ");
    fgets(username, MAX_USERNAME_LEN, stdin);
    username[strcspn(username, "\n")] = 0;
    
    // 检查用户名是否已存在
    char existingId[MAX_ID_LEN];
    if (GetUserByUsername(username, existingId)) {
        printf("用户名已存在！\n");
        return;
    }
    
    printf("用户ID: ");
    fgets(id, MAX_ID_LEN, stdin);
    id[strcspn(id, "\n")] = 0;
    
    // 检查ID是否已存在
    int dummy;
    char dummyName[MAX_USERNAME_LEN];
    if (LoadUserData(id, &record, &dummy, dummyName)) {
        printf("ID已存在！\n");
        return;
    }

    printf("密码: ");
    fgetws(password, MAX_PASSWORD_LEN, stdin);
    password[wcscspn(password, L"\n")] = 0;
    
    printf("用户类型:\n");
    printf("1. 管理员\n");
    printf("2. 教师\n");
    printf("3. 学生 (默认)\n");
    printf("请选择: ");
    scanf("%d", &choice);
    getchar(); // 清除换行符

    switch (choice) {
    case 1:
        userType = 15;  // 管理员权限
        break;
    case 2:
        userType = 25;  // 教师权限
        break;
    case 3:
    default:
        userType = 35;  // 学生权限
    }

    if (HashPassword(password, &record)) {
        if (SaveUserData(id, &record, userType, username)) {
            printf("注册成功！\n");
        } else {
            printf("保存失败！\n");
        }
    } else {
        printf("密码处理失败！\n");
    }
}

// 用户登录并返回用户类型
int LoginUser() {
    char username[MAX_USERNAME_LEN];
    char id[MAX_ID_LEN];
    wchar_t password[MAX_PASSWORD_LEN];
    PasswordRecord record;
    int userType = -1;

    printf("\n=== 登录 ===\n");
    printf("用户名: ");
    fgets(username, MAX_USERNAME_LEN, stdin);
    username[strcspn(username, "\n")] = 0;

    // 通过用户名查找ID
    if (!GetUserByUsername(username, id)) {
        printf("用户不存在！\n");
        return -1;
    }

    // 通过ID加载用户数据
    char loadedUsername[MAX_USERNAME_LEN];
    if (!LoadUserData(id, &record, &userType, loadedUsername)) {
        printf("用户数据损坏！\n");
        return -1;
    }

    printf("密码: ");
    fgetws(password, MAX_PASSWORD_LEN, stdin);
    password[wcscspn(password, L"\n")] = 0;

    if (VerifyPassword(password, &record)) {
        printf("登录成功！\n");
        
        // 保存当前用户信息
        memcpy(&currentUser.password, &record, sizeof(PasswordRecord));
        currentUser.userType = userType;
        strncpy(currentUser.id, id, MAX_ID_LEN-1);
        currentUser.id[MAX_ID_LEN-1] = '\0';
        strncpy(currentUser.username, loadedUsername, MAX_USERNAME_LEN-1);
        currentUser.username[MAX_USERNAME_LEN-1] = '\0';
        
        return userType;
    } else {
        printf("密码错误！\n");
        return -1;
    }
}

// 身份认证流程
int AuthenticationProcess() {
    int choice;
    int userType = -1;

    while (userType == -1) {
        printf("\n=== 身份验证系统 ===\n");
        printf("1. 登录\n");
        printf("2. 注册\n");
        printf("3. 以访客身份继续\n");  
        printf("4. 退出系统\n");
        printf("请选择: ");
        scanf("%d", &choice);
        getchar(); // 清除输入缓冲区中的换行符

        switch (choice) {
        case 1:
            userType = LoginUser();
            break;
        case 2:
            RegisterUser();
            break;
        case 3:
            userType = -1;
            printf("您正以访客身份访问系统，功能受限。\n");
            return userType;
        case 4:
            exit(0);
        default:
            printf("无效选项，请重试！\n");
        }
    }

    return userType;
}

// 教师课程管理函数
BOOL AssignCourseToTeacher(const char* teacherId, const char* courseCode) {
    // 加载教师数据
    Teacher* teachers = NULL;
    Course* courses = NULL;
    Grade* grades = NULL;
    char dataFile[] = "school_data.dat";
    
    // 反序列化学校数据
    deserialize(&teachers, &courses, &grades, dataFile);
    
    // 查找教师
    Teacher* teacher = NULL;
    for (Teacher* t = teachers; t != NULL; t = t->next) {
        if (strcmp(t->id, teacherId) == 0) {
            teacher = t;
            break;
        }
    }
    
    // 查找课程
    Course* course = NULL;
    for (Course* c = courses; c != NULL; c = c->next) {
        if (strcmp(c->code, courseCode) == 0) {
            course = c;
            break;
        }
    }
    
    if (!teacher || !course) {
        // 释放内存
        // TODO: 添加释放内存的函数调用
        return FALSE;
    }
    
    // 检查是否已分配
    for (TeacherCourse* tc = teacher->courses; tc != NULL; tc = tc->next) {
        if (strcmp(tc->course_code, courseCode) == 0) {
            // 已分配，不需要重复分配
            return TRUE;
        }
    }
    
    // 创建新的教师课程关联
    TeacherCourse* newCourse = (TeacherCourse*)malloc(sizeof(TeacherCourse));
    if (!newCourse) return FALSE;
    
    strncpy(newCourse->course_code, courseCode, sizeof(newCourse->course_code) - 1);
    newCourse->course_code[sizeof(newCourse->course_code) - 1] = '\0';
    newCourse->next = teacher->courses;
    teacher->courses = newCourse;
    
    // 更新课程的教师指针
    course->teacher = teacher;
    
    // 保存更新后的数据
    serialize(teachers, courses, grades, dataFile);
    
    return TRUE;
}

// 检查教师是否被分配到某课程
BOOL IsTeacherAssignedToCourse(const char* teacherId, const char* courseCode) {
    // 加载教师数据
    Teacher* teachers = NULL;
    Course* courses = NULL;
    Grade* grades = NULL;
    char dataFile[] = "school_data.dat";
    
    // 反序列化学校数据
    deserialize(&teachers, &courses, &grades, dataFile);
    
    // 查找教师
    Teacher* teacher = NULL;
    for (Teacher* t = teachers; t != NULL; t = t->next) {
        if (strcmp(t->id, teacherId) == 0) {
            teacher = t;
            break;
        }
    }
    
    if (!teacher) {
        return FALSE;
    }
    
    // 检查是否已分配
    for (TeacherCourse* tc = teacher->courses; tc != NULL; tc = tc->next) {
        if (strcmp(tc->course_code, courseCode) == 0) {
            return TRUE;
        }
    }
    
    return FALSE;
}