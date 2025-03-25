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

// 密码处理 - 改为直接保存明文
BOOL SavePassword(const char* password, PasswordRecord* record) {
    strncpy(record->password, password, MAX_PASSWORD_LEN - 1);
    record->password[MAX_PASSWORD_LEN - 1] = '\0';
    return TRUE;
}

// 密码验证 - 直接比较明文
BOOL VerifyPassword(const char* attempt, PasswordRecord* stored) {
    return strcmp(attempt, stored->password) == 0;
}

// 文件存储操作 - 按ID存储
BOOL SaveUserData(const char* id, PasswordRecord* record, int userType, const char* username) {
    char filename[MAX_PATH];
    snprintf(filename, MAX_PATH, "store\\users\\%s.user", id);

    FILE* file = fopen(filename, "w"); // 修改为文本模式
    if (!file) return FALSE;

    // 扩展用户记录以包含ID、用户名和类型
    UserRecord userRecord;
    strncpy(userRecord.id, id, MAX_ID_LEN-1);
    userRecord.id[MAX_ID_LEN-1] = '\0';
    
    strncpy(userRecord.username, username, MAX_USERNAME_LEN-1);
    userRecord.username[MAX_USERNAME_LEN-1] = '\0';
    
    memcpy(&userRecord.password, record, sizeof(PasswordRecord));
    userRecord.userType = userType;

    // 以明文形式写入文件
    fprintf(file, "%s\n%s\n%s\n%d\n", 
            userRecord.id, 
            userRecord.username, 
            userRecord.password.password,
            userRecord.userType);
            
    fclose(file);
    
    // 创建用户名到ID的映射文件
    char mapFilename[MAX_PATH];
    snprintf(mapFilename, MAX_PATH, "store\\users\\%s.map", username);
    
    FILE* mapFile = fopen(mapFilename, "w");
    if (!mapFile) return FALSE;
    
    fprintf(mapFile, "%s", id);
    fclose(mapFile);
    
    return TRUE;
}

// 通过ID加载用户数据
BOOL LoadUserData(const char* id, PasswordRecord* record, int* userType, char* username) {
    char filename[MAX_PATH];
    snprintf(filename, MAX_PATH, "store\\users\\%s.user", id);

    FILE* file = fopen(filename, "r");
    if (!file) return FALSE;

    char id_buf[MAX_ID_LEN];
    char username_buf[MAX_USERNAME_LEN];
    char password_buf[MAX_PASSWORD_LEN];
    int userType_buf;

    // 从文件读取各项数据
    if (fscanf(file, "%s\n%s\n%s\n%d", 
              id_buf, username_buf, password_buf, &userType_buf) != 4) {
        fclose(file);
        return FALSE;
    }
    
    fclose(file);
    
    // 填充返回值
    strncpy(record->password, password_buf, MAX_PASSWORD_LEN);
    *userType = userType_buf;
    
    if (username) {
        strncpy(username, username_buf, MAX_USERNAME_LEN);
    }
    
    return TRUE;
}

// 通过用户名获取ID
BOOL GetUserByUsername(const char* username, char* id) {
    char mapFilename[MAX_PATH];
    snprintf(mapFilename, MAX_PATH, "store\\users\\%s.map", username);
    
    FILE* mapFile = fopen(mapFilename, "r");
    if (!mapFile) return FALSE;
    
    if (fscanf(mapFile, "%s", id) != 1) {
        fclose(mapFile);
        return FALSE;
    }
    
    fclose(mapFile);
    return TRUE;
}

// 用户注册
void RegisterUser() {
    char username[MAX_USERNAME_LEN];
    char id[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];
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
    fgets(password, MAX_PASSWORD_LEN, stdin);
    password[strcspn(password, "\n")] = 0;
    
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

    if (SavePassword(password, &record)) {
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
    char password[MAX_PASSWORD_LEN];
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
    fgets(password, MAX_PASSWORD_LEN, stdin);
    password[strcspn(password, "\n")] = 0;

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

// 身份认证流程 - 改进输入处理
int AuthenticationProcess() {
    int choice;
    int userType = -1;

    // 确保用户目录存在
    ensureDirectoryExists("store");
    ensureDirectoryExists("store\\users");

    while (userType == -1) {
        system("cls");
        printf("\n=== 身份验证系统 ===\n");
        printf("1. 登录\n");
        printf("2. 注册\n");
        printf("3. 以访客身份继续\n");  
        printf("4. 退出系统\n");
        printf("请选择: ");
        
        // 更安全的输入处理
        if (scanf("%d", &choice) != 1) {
            // 清除输入缓冲区
            while (getchar() != '\n');
            printf("输入无效，请重试！\n");
            Sleep(1000); // 延迟以便用户看到错误消息
            continue;
        }
        
        // 清除输入缓冲区中的换行符
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            userType = LoginUser();
            break;
        case 2:
            RegisterUser();
            break;
        case 3:
            printf("您正以访客身份访问系统，功能受限。\n");
            // 设置默认访客用户信息
            strcpy(currentUser.id, "guest");
            strcpy(currentUser.username, "访客");
            currentUser.userType = -1;
            userType = -1;
            Sleep(2000); // 给用户时间阅读信息
            return userType;
        case 4:
            printf("正在退出系统...\n");
            Sleep(1000);
            exit(0);
        default:
            printf("无效选项，请重试！\n");
            Sleep(1000);
        }
    }

    return userType;
}

// 教师课程管理函数 - 简化实现，只关注文本文件存储
BOOL AssignCourseToTeacher(const char* teacherId, const char* courseCode) {
    // 使用简单的文本文件记录关联
    char filename[MAX_PATH];
    snprintf(filename, MAX_PATH, "store\\teachers\\%s_courses.txt", teacherId);
    
    // 检查课程是否已分配
    FILE* checkFile = fopen(filename, "r");
    if (checkFile) {
        char line[20];
        while (fgets(line, sizeof(line), checkFile)) {
            line[strcspn(line, "\n")] = 0;
            if (strcmp(line, courseCode) == 0) {
                fclose(checkFile);
                return TRUE; // 课程已分配
            }
        }
        fclose(checkFile);
    }
    
    // 添加新的课程分配
    FILE* file = fopen(filename, "a");
    if (!file) return FALSE;
    
    fprintf(file, "%s\n", courseCode);
    fclose(file);
    
    return TRUE;
}

// 检查教师是否被分配到某课程
BOOL IsTeacherAssignedToCourse(const char* teacherId, const char* courseCode) {
    char filename[MAX_PATH];
    snprintf(filename, MAX_PATH, "store\\teachers\\%s_courses.txt", teacherId);
    
    FILE* file = fopen(filename, "r");
    if (!file) return FALSE;
    
    char line[20];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, courseCode) == 0) {
            fclose(file);
            return TRUE;
        }
    }
    
    fclose(file);
    return FALSE;
}