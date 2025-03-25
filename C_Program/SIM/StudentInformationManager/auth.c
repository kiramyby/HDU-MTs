#include "auth.h"
#include <stdio.h>
#include <string.h>

// 当前用户信息
static UserRecord currentUser = { {0}, -1, "", "" };

// 获取当前用户信息
UserRecord* GetCurrentUser() {
    return &currentUser;
}

// 密码处理 - 直接保存明文
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
int RegisterUser() {
    char id[MAX_ID_LEN];
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char confirm[MAX_PASSWORD_LEN];
    int userType = 0;
    
    system("cls");
    printf("\n===== 用户注册 =====\n");
    printf("请输入ID (最多11个字符): ");
    scanf("%11s", id);
    
    // 检查ID是否已存在
    PasswordRecord dummy;
    int dummyType;
    char dummyName[MAX_USERNAME_LEN];
    if (LoadUserData(id, &dummy, &dummyType, dummyName)) {
        printf("该ID已被注册！\n");
        Sleep(2000);
        return -1;
    }
    
    printf("请输入用户名 (最多63个字符): ");
    scanf("%63s", username);
    
    printf("请选择用户类型:\n");
    printf("1. 教师\n");
    printf("2. 学生\n");
    printf("请选择 (1-2): ");
    
    int choice;
    scanf("%d", &choice);
    
    // 转换为内部使用的用户类型编码
    switch(choice) {
        case 1:
            userType = 0x20; // 教师
            break;
        case 2:
            userType = 0x30; // 学生
            break;
        default:
            printf("无效选择！\n");
            Sleep(2000);
            return -1;
    }
    
    printf("请输入密码: ");
    int i = 0;
    while (i < MAX_PASSWORD_LEN - 1) {
        char ch = _getch();
        if (ch == '\r' || ch == '\n') {
            password[i] = '\0';
            break;
        } else if (ch == '\b' && i > 0) { // 处理退格键
            printf("\b \b");
            i--;
        } else if (ch >= 32 && ch <= 126) { // 可打印字符
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
    
    printf("请确认密码: ");
    i = 0;
    while (i < MAX_PASSWORD_LEN - 1) {
        char ch = _getch();
        if (ch == '\r' || ch == '\n') {
            confirm[i] = '\0';
            break;
        } else if (ch == '\b' && i > 0) { // 处理退格键
            printf("\b \b");
            i--;
        } else if (ch >= 32 && ch <= 126) { // 可打印字符
            confirm[i++] = ch;
            printf("*");
        }
    }
    confirm[i] = '\0';
    printf("\n");
    
    if (strcmp(password, confirm) != 0) {
        printf("两次输入的密码不一致！\n");
        Sleep(2000);
        return -1;
    }
    
    PasswordRecord record;
    SavePassword(password, &record);
    
    if (SaveUserData(id, &record, userType, username)) {
        printf("注册成功！您现在可以使用ID: %s 登录。\n", id);
        Sleep(2000);
        return userType;
    } else {
        printf("注册失败，请稍后重试。\n");
        Sleep(2000);
        return -1;
    }
}

// 用户登录并返回用户类型
int LoginUser() {
    UserRecord userRecord;
    char id[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];
    int tries = 0;
    
    while (tries < 3) {
        system("cls");
        printf("\n===== 用户登录 =====\n");
        printf("请输入ID (或输入 'q' 返回): ");
        scanf("%11s", id);
        
        if (strcmp(id, "q") == 0) {
            return -1;
        }
        
        printf("请输入密码: ");
        int i = 0;
        while (i < MAX_PASSWORD_LEN - 1) {
            char ch = _getch();
            if (ch == '\r' || ch == '\n') {
                password[i] = '\0';
                break;
            } else if (ch == '\b' && i > 0) { // 处理退格键
                printf("\b \b");
                i--;
            } else if (ch >= 32 && ch <= 126) { // 可打印字符
                password[i++] = ch;
                printf("*");
            }
        }
        password[i] = '\0';
        printf("\n");
        
        PasswordRecord stored;
        int userType;
        char username[MAX_USERNAME_LEN];
        
        if (LoadUserData(id, &stored, &userType, username)) {
            if (VerifyPassword(password, &stored)) {
                // 设置当前用户信息
                currentUser.userType = userType;
                strncpy(currentUser.id, id, MAX_ID_LEN);
                currentUser.id[MAX_ID_LEN-1] = '\0';
                strncpy(currentUser.username, username, MAX_USERNAME_LEN);
                currentUser.username[MAX_USERNAME_LEN-1] = '\0';
                memcpy(&currentUser.password, &stored, sizeof(PasswordRecord));
                
                printf("登录成功！欢迎 %s (ID: %s)!\n", username, id);
                Sleep(1000);
                return userType;
            }
        }
        
        tries++;
        printf("ID或密码错误！还有 %d 次尝试机会。\n", 3 - tries);
        Sleep(1500);
    }
    
    printf("登录失败次数过多，请稍后再试。\n");
    Sleep(2000);
    return -1;
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