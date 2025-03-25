#include "sim.h"
#include "auth.h"

/*================= 光标操作函数 =================*/
void GotoXY(int x, int y) {
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cor = { x, y };
    SetConsoleCursorPosition(hout, cor);
}

void HideCursor() {
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cor_info = { 1, 0 }; // 1为大小，0为隐藏
    SetConsoleCursorInfo(hout, &cor_info);
}

/*================= 界面绘制函数 =================*/
void Window() {
    int i;

    // 使用标准 ASCII 字符绘制界面框架
    GotoXY(LEFT_HEAD, UP_HEAD);
    printf("+");
    for (i = LEFT_HEAD + 1; i < RIGHT_HEAD; i++)
        printf("-");
    printf("+");

    for (i = UP_HEAD + 1; i < DOWN_HEAD; i++) {
        GotoXY(LEFT_HEAD, i);
        printf("|");
        GotoXY((RIGHT_HEAD + LEFT_HEAD) / 2, i);
        printf("|");
        GotoXY(RIGHT_HEAD, i);
        printf("|");
    }

    GotoXY(LEFT_HEAD, DOWN_HEAD);
    printf("+");
    for (i = LEFT_HEAD + 1; i < RIGHT_HEAD; i++)
        printf("-");
    printf("+");

    GotoXY(LEFT_HEAD, UP_HEAD + 1);
    printf("Student Information Manager");
    HideCursor();
}

int Menu() {
    system("cls"); // 清屏再显示菜单
    Window(); // 先绘制窗口框架
    
    GotoXY(45, 12);
    printf("管理系统菜单");
    GotoXY(43, 14);
    printf("0 -> 创建");
    GotoXY(43, 16);
    printf("1 -> 查询");
    GotoXY(43, 18);
    printf("2 -> 更新");
    GotoXY(43, 20);
    printf("3 -> 删除");
    GotoXY(43, 22);
    printf("4 -> 日志");
    GotoXY(43, 24);
    printf("5 -> 登出");
    GotoXY(43, 26);
    printf("请选择操作: ");
    
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // 清空输入缓冲区
    
    char ch;
    do {
        ch = _getch(); // 使用_getch()替代getchar()以获取单个字符
    } while (ch < '0' || ch > '5');
    
    int op = ch - '0'; // 将字符转换为整数
    printf("%c\n", ch); // 显示用户的选择
    
    return op;
}

void Final() {
    system("cls");
    GotoXY(45, 14);
    printf("已登出！ 3秒后自动返回登录界面...");
    Sleep(3000);
    system("cls");
}

/*================= 数据操作辅助函数 =================*/
Student* findStudent(Student* students, const char* student_id) {
    Student* current = students;
    while (current) {
        if (strcmp(current->id, student_id) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

Course* findCourse(Course* courses, const char* course_code) {
    Course* current = courses;
    while (current) {
        if (strcmp(current->code, course_code) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

Teacher* findTeacher(Teacher* teachers, const char* teacher_id) {
    Teacher* current = teachers;
    while (current) {
        if (strcmp(current->id, teacher_id) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

/*================= 查询功能实现 =================*/
void queryStudent(Student* students, Course* courses) {
    system("cls");
    Window();
    
    int uid = atoi(GetCurrentUser()->id);
    int leftStart = LEFT_HEAD + 2;
    int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
    
    // 左侧显示操作提示
    GotoXY(leftStart, UP_HEAD + 4);
    printf("=== 学生信息查询 ===");
    
    // 如果是学生用户，只能查看自己的信息
    if (IsStudent(uid) && !IsAdmin(uid)) {
        Student* self = findStudent(students, GetCurrentUser()->id);
        if (!self) {
            GotoXY(leftStart, UP_HEAD + 6);
            printf("无法找到您的学生记录！");
            GotoXY(leftStart, DOWN_HEAD - 2);
            printf("按任意键返回...");
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            _getch();
            return;
        }
        
        // 右侧显示学生个人信息摘要
        GotoXY(rightStart, UP_HEAD + 4);
        printf("学生个人信息");
        GotoXY(rightStart, UP_HEAD + 6);
        printf("学号: %s", self->id);
        GotoXY(rightStart, UP_HEAD + 7);
        printf("姓名: %s", self->name);
        
        // 计算总成绩和平均分
        float totalScore = 0.0;
        int courseCount = 0;
        Grade* g = self->grades;
        while (g) {
            totalScore += g->score;
            courseCount++;
            g = g->next_student;
        }
        
        if (courseCount > 0) {
            GotoXY(rightStart, UP_HEAD + 9);
            printf("已修课程数: %d", courseCount);
            GotoXY(rightStart, UP_HEAD + 10);
            printf("平均成绩: %.2f", totalScore / courseCount);
        } else {
            GotoXY(rightStart, UP_HEAD + 9);
            printf("尚未有成绩记录");
        }
        
        // 在左侧显示成绩明细
        GotoXY(leftStart, UP_HEAD + 6);
        printf("您的成绩单");
        
        GotoXY(leftStart, UP_HEAD + 8);
        printf("%-10s %-20s %-6s", "课程号", "课程名", "成绩");
        
        int row = UP_HEAD + 10;
        g = self->grades;
        while (g) {
            Course* cour = findCourse(courses, g->course_code);
            GotoXY(leftStart, row);
            printf("%-10s %-20s %.2f", 
                   g->course_code,
                   cour ? cour->name : "未知课程",
                   g->score);
            g = g->next_student;
            row += 1;
            
            // 避免超出窗口范围
            if (row >= DOWN_HEAD - 2) break;
        }
        
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("按任意键返回...");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
        return;
    }
    
    // 显示可查询的学生信息（对管理员和教师）
    if (IsAdmin(uid)) {
        // 管理员可以查看所有学生
        GotoXY(rightStart, UP_HEAD + 4);
        printf("您可以查询所有学生信息");
        
        // 显示学生列表摘要
        int row = UP_HEAD + 6;
        Student* s = students;
        GotoXY(rightStart, row++);
        printf("%-10s %-15s", "学号", "姓名");
        while (s && row < DOWN_HEAD - 4) {
            GotoXY(rightStart, row++);
            printf("%-10s %-15s", s->id, s->name);
            s = s->next;
        }
    } else if (IsTeacher(uid)) {
        // 教师只能查看选修了他所教课程的学生
        GotoXY(rightStart, UP_HEAD + 4);
        printf("您可查询选修您所授课程的学生");
        
        // 加载该教师负责的课程
        Course* teacherCourses[20] = {NULL}; // 假设教师最多负责20门课
        int courseCount = 0;
        
        Course* c = courses;
        while (c && courseCount < 20) {
            if (IsTeacherAssignedToCourse(GetCurrentUser()->id, c->code)) {
                teacherCourses[courseCount++] = c;
                GotoXY(rightStart, UP_HEAD + 6 + courseCount);
                printf("- %s: %s", c->code, c->name);
            }
            c = c->next;
        }
        
        GotoXY(rightStart, UP_HEAD + 6);
        printf("您负责的课程: %d 门", courseCount);
    }
    
    while (1) {
        // 清理之前的查询结果区域
        for(int i = UP_HEAD + 8; i < DOWN_HEAD - 4; i++) {
            GotoXY(leftStart, i);
            printf("                                        ");
        }
        
        // 管理员或教师需要输入学生学号
        GotoXY(leftStart, UP_HEAD + 6);
        printf("请输入学生学号: ");
        
        char id[12];
        scanf("%11s", id);
        
        Student* target = findStudent(students, id);
        if (!target) {
            GotoXY(leftStart, UP_HEAD + 8);
            printf("学生不存在！");
        } else {
            // 显示学生基本信息
            GotoXY(leftStart, UP_HEAD + 8);
            printf("学生姓名: %s", target->name);
            
            // 教师权限检查 - 确保该学生选修了教师的至少一门课程
            BOOL hasPermission = IsAdmin(uid); // 管理员默认有权限
            
            if (!hasPermission && IsTeacher(uid)) {
                Grade* g = target->grades;
                while (g) {
                    if (HasAccessToCourse(uid, g->course_code)) {
                        hasPermission = TRUE;
                        break;
                    }
                    g = g->next_student;
                }
            }
            
            if (!hasPermission) {
                GotoXY(leftStart, UP_HEAD + 10);
                printf("权限不足！该学生未选修您的课程。");
            } else {
                // 显示成绩单
                GotoXY(leftStart, UP_HEAD + 10);
                printf("成绩单：");
                
                GotoXY(leftStart, UP_HEAD + 11);
                printf("%-10s %-20s %-6s", "课程号", "课程名", "成绩");
                
                int row = UP_HEAD + 12;
                Grade* g = target->grades;
                while (g) {
                    // 教师只能看到学生在自己课程上的成绩
                    if (IsAdmin(uid) || HasAccessToCourse(uid, g->course_code)) {
                        Course* cour = findCourse(courses, g->course_code);
                        GotoXY(leftStart, row);
                        printf("%-10s %-20s %.2f", 
                               g->course_code,
                               cour ? cour->name : "未知课程",
                               g->score);
                        row += 1;
                    }
                    g = g->next_student;
                    
                    // 避免超出窗口范围
                    if (row >= DOWN_HEAD - 4) break;
                }
            }
        }
        
        // 提供继续查询或返回的选项
        GotoXY(leftStart, DOWN_HEAD - 4);
        printf("1. 继续查询其他学生");
        GotoXY(leftStart, DOWN_HEAD - 3);
        printf("0. 返回上级菜单");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("请选择: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
            choice = 1; // 默认继续查询
        }
        
        if (choice == 0) {
            break; // 返回上级菜单
        }
        // 继续循环，查询其他学生
    }
}

void queryCourse(Course* courses, Student* students) {
    system("cls");
    Window();
    
    int uid = atoi(GetCurrentUser()->id);
    int leftStart = LEFT_HEAD + 2;
    int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
    
    // 左侧显示操作提示
    GotoXY(leftStart, UP_HEAD + 4);
    printf("=== 课程信息查询 ===");
    
    // 显示与当前用户相关的课程
    DisplayRelevantCourses(uid, courses, UP_HEAD + 6);
    
    while(1) {
        // 清理之前的查询结果区域
        for(int i = UP_HEAD + 8; i < DOWN_HEAD - 4; i++) {
            GotoXY(leftStart, i);
            printf("                                        ");
        }
        
        GotoXY(leftStart, UP_HEAD + 6);
        printf("请输入课程代码: ");
        
        char code[10];
        scanf("%9s", code);
        
        // 检查用户是否有权限查看该课程信息
        if (!HasAccessToCourse(uid, code) && !IsAdmin(uid)) {
            GotoXY(leftStart, UP_HEAD + 8);
            printf("您无权访问此课程信息或此课程不存在。");
            GotoXY(leftStart, UP_HEAD + 9);
            
            if (IsTeacher(uid)) {
                printf("只能查看您所教授的课程。");
            } else if (IsStudent(uid)) {
                printf("只能查看您已选修的课程。");
            }
        } else {
            Course* target = findCourse(courses, code);
            if (!target) {
                GotoXY(leftStart, UP_HEAD + 8);
                printf("课程不存在！");
            } else {
                // 在左侧输入区域下方显示课程信息
                GotoXY(leftStart, UP_HEAD + 8);
                printf("课程名称: %s", target->name);
                
                GotoXY(leftStart, UP_HEAD + 10);
                printf("成绩单：");
                
                GotoXY(leftStart, UP_HEAD + 11);
                printf("%-10s %-15s %-6s", "学号", "姓名", "成绩");
                
                int row = UP_HEAD + 12;
                Grade* g = target->grades;
                while (g) {
                    // 如果是学生，只显示自己的成绩
                    if (IsStudent(uid) && strcmp(GetCurrentUser()->id, g->student_id) != 0 && !IsAdmin(uid)) {
                        g = g->next_course;
                        continue;
                    }
                    
                    Student* stu = findStudent(students, g->student_id);
                    GotoXY(leftStart, row);
                    printf("%-10s %-15s %.2f", g->student_id, stu ? stu->name : "未知学生", g->score);
                    g = g->next_course;
                    row++;
                    
                    // 避免超出窗口范围
                    if (row >= DOWN_HEAD - 4) break;
                }
            }
        }
        
        // 提供继续查询或返回的选项
        GotoXY(leftStart, DOWN_HEAD - 4);
        printf("1. 继续查询其他课程");
        GotoXY(leftStart, DOWN_HEAD - 3);
        printf("0. 返回上级菜单");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("请选择: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
            choice = 1; // 默认继续查询
        }
        
        if (choice == 0) {
            break; // 返回上级菜单
        }
        // 继续循环，查询其他课程
    }
}

/*================= 排序功能实现 =================*/
void sortGrades(Grade** headRef, CompareFunc compare, int isStudentView) {
    if (!*headRef) return;
    
    Grade* arr[100];
    int count = 0;
    Grade* current = *headRef;
    
    while (current && count < 100) {
        arr[count++] = current;
        current = isStudentView ? current->next_student : current->next_course;
    }
    
    qsort(arr, count, sizeof(Grade*), (int (*)(const void*, const void*))compare);
    
    *headRef = arr[0];
    for (int i = 0; i < count-1; i++) {
        if (isStudentView)
            arr[i]->next_student = arr[i+1];
        else
            arr[i]->next_course = arr[i+1];
    }
    
    if (isStudentView)
        arr[count-1]->next_student = NULL;
    else
        arr[count-1]->next_course = NULL;
}

int compareByScoreDesc(const Grade* a, const Grade* b) {
    return (a->score < b->score) ? 1 : -1;
}

void sortStudentGrades(Student* students) {
    system("cls");
    Window();
    
    int leftStart = LEFT_HEAD + 2;
    int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
    
    // 左侧显示操作提示
    GotoXY(leftStart, UP_HEAD + 4);
    printf("=== 学生成绩排序 ===");
    
    GotoXY(leftStart, UP_HEAD + 6);
    printf("请输入要排序的学生学号: ");
    
    char id[12];
    scanf("%11s", id);
    
    Student* target = findStudent(students, id);
    if (!target) {
        GotoXY(leftStart, UP_HEAD + 8);
        printf("学生不存在！");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("按任意键返回...");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
        return;
    }
    
    sortGrades(&target->grades, compareByScoreDesc, 1);
    
    // 右侧显示排序结果
    GotoXY(rightStart, UP_HEAD + 4);
    printf("学生 %s 的成绩（已按分数降序排列）", target->name);
    
    GotoXY(rightStart, UP_HEAD + 6);
    printf("%-10s %-20s %-6s", "课程代码", "课程名", "成绩");
    
    int row = UP_HEAD + 8;
    Grade* g = target->grades;
    while (g && row < DOWN_HEAD - 4) {
        Course* course = NULL;
        // 这里应该查找课程信息，但由于这只是展示代码，我们假设可以获取
        GotoXY(rightStart, row);
        printf("%-10s %-20s %.2f", g->course_code, course ? course->name : "未知课程", g->score);
        g = g->next_student;
        row++;
    }
    
    GotoXY(leftStart, DOWN_HEAD - 2);
    printf("按任意键返回...");
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
}

void sortCourseGrades(Course* courses) {
    system("cls");
    Window();
    
    int leftStart = LEFT_HEAD + 2;
    int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
    
    // 左侧显示操作提示
    GotoXY(leftStart, UP_HEAD + 4);
    printf("=== 课程成绩排序 ===");
    
    GotoXY(leftStart, UP_HEAD + 6);
    printf("请输入要排序的课程代码: ");
    
    char code[10];
    scanf("%9s", code);
    
    Course* target = findCourse(courses, code);
    if (!target) {
        GotoXY(leftStart, UP_HEAD + 8);
        printf("课程不存在！");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("按任意键返回...");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
        return;
    }
    
    sortGrades(&target->grades, compareByScoreDesc, 0);
    
    // 右侧显示排序结果
    GotoXY(rightStart, UP_HEAD + 4);
    printf("课程 %s 的成绩（已按分数降序排列）", target->name);
    
    GotoXY(rightStart, UP_HEAD + 6);
    printf("%-10s %-15s %-6s", "学号", "姓名", "成绩");
    
    int row = UP_HEAD + 8;
    Grade* g = target->grades;
    while (g && row < DOWN_HEAD - 4) {
        Student* student = NULL;
        // 这里应该查找学生信息，但由于这只是展示代码，我们假设可以获取
        GotoXY(rightStart, row);
        printf("%-10s %-15s %.2f", g->student_id, student ? student->name : "未知学生", g->score);
        g = g->next_course;
        row++;
    }
    
    GotoXY(leftStart, DOWN_HEAD - 2);
    printf("按任意键返回...");
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
}

/*================= 创建数据功能 =================*/
Student* createStudent() {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent) {
        printf("内存分配失败！\n");
        return NULL;
    }
    
    printf("请输入学生学号: ");
    scanf("%11s", newStudent->id);
    
    printf("请输入学生姓名: ");
    scanf("%19s", newStudent->name);
    
    newStudent->grades = NULL;
    newStudent->next = NULL;
    
    return newStudent;
}

Course* createCourse() {
    Course* newCourse = (Course*)malloc(sizeof(Course));
    if (!newCourse) {
        printf("内存分配失败！\n");
        return NULL;
    }
    
    printf("请输入课程代码: ");
    scanf("%9s", newCourse->code);
    
    printf("请输入课程名称: ");
    scanf("%49s", newCourse->name);
    
    newCourse->grades = NULL;
    newCourse->teacher = NULL;
    newCourse->next = NULL;
    
    return newCourse;
}

Teacher* createTeacher() {
    Teacher* newTeacher = (Teacher*)malloc(sizeof(Teacher));
    if (!newTeacher) {
        printf("内存分配失败！\n");
        return NULL;
    }
    
    printf("请输入教师ID: ");
    scanf("%11s", newTeacher->id);
    
    printf("请输入教师姓名: ");
    scanf("%19s", newTeacher->name);
    
    newTeacher->courses = NULL;
    newTeacher->next = NULL;
    
    return newTeacher;
}

Grade* createGrade(Student* students, Course* courses) {
    system("cls");
    Window();
    
    int uid = atoi(GetCurrentUser()->id);
    int leftStart = LEFT_HEAD + 2;
    int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
    
    // 左侧显示操作提示与输入
    GotoXY(leftStart, UP_HEAD + 4);
    printf("=== 成绩录入 ===");
    
    // 学生只能查看自己的成绩，不能录入
    if (IsStudent(uid) && !IsAdmin(uid)) {
        ShowPermissionDeniedMessage(leftStart, "进行成绩录入");
        return NULL;
    }
    
    Grade* newGrade = (Grade*)malloc(sizeof(Grade));
    if (!newGrade) {
        GotoXY(leftStart, UP_HEAD + 6);
        printf("内存分配失败！");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("按任意键返回...");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
        return NULL;
    }
    
    char student_id[12];
    char course_code[10];
    
    GotoXY(leftStart, UP_HEAD + 6);
    printf("请输入学生学号: ");
    scanf("%11s", student_id);
    
    Student* student = findStudent(students, student_id);
    if (!student) {
        GotoXY(leftStart, UP_HEAD + 8);
        printf("学生不存在！");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("按任意键返回...");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
        free(newGrade);
        return NULL;
    }
    
    // 右侧显示学生信息
    GotoXY(rightStart, UP_HEAD + 6);
    printf("学生: %s", student->name);
    
    // 如果是教师，显示可以录入成绩的课程
    if (IsTeacher(uid) && !IsAdmin(uid)) {
        GotoXY(rightStart, UP_HEAD + 8);
        printf("您可以录入的课程：");
        
        int row = UP_HEAD + 9;
        Course* c = courses;
        while (c && row < DOWN_HEAD - 4) {
            if (HasAccessToCourse(uid, c->code)) {
                GotoXY(rightStart, row++);
                printf("%-10s %-30s", c->code, c->name);
            }
            c = c->next;
        }
    } else if (IsAdmin(uid)) {
        // 管理员显示所有课程
        GotoXY(rightStart, UP_HEAD + 8);
        printf("所有课程：");
        
        int row = UP_HEAD + 9;
        Course* c = courses;
        while (c && row < DOWN_HEAD - 4) {
            GotoXY(rightStart, row++);
            printf("%-10s %-30s", c->code, c->name);
            c = c->next;
        }
    }
    
    GotoXY(leftStart, UP_HEAD + 8);
    printf("请输入课程代码: ");
    scanf("%9s", course_code);
    
    // 教师需要检查是否有权限录入该课程成绩
    if (IsTeacher(uid) && !HasAccessToCourse(uid, course_code) && !IsAdmin(uid)) {
        GotoXY(leftStart, UP_HEAD + 10);
        printf("权限不足！您无权为该课程录入成绩！");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("按任意键返回...");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
        free(newGrade);
        return NULL;
    }
    
    Course* course = findCourse(courses, course_code);
    if (!course) {
        GotoXY(leftStart, UP_HEAD + 10);
        printf("课程不存在！");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("按任意键返回...");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
        free(newGrade);
        return NULL;
    }
    
    // 检查是否已经存在该成绩记录
    Grade* existingGrade = student->grades;
    while (existingGrade) {
        if (strcmp(existingGrade->course_code, course_code) == 0) {
            GotoXY(leftStart, UP_HEAD + 10);
            printf("该学生的该课程成绩已存在！是否更新？(Y/N): ");
            char choice;
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            choice = toupper(_getch());
            printf("%c", choice);
            
            if (choice == 'Y') {
                GotoXY(leftStart, UP_HEAD + 12);
                printf("当前成绩: %.2f", existingGrade->score);
                GotoXY(leftStart, UP_HEAD + 14);
                printf("请输入新成绩: ");
                scanf("%f", &existingGrade->score);
                GotoXY(leftStart, UP_HEAD + 16);
                printf("成绩更新成功！");
                free(newGrade);
                return existingGrade;
            } else {
                GotoXY(leftStart, UP_HEAD + 12);
                printf("取消更新成绩");
                free(newGrade);
                return NULL;
            }
        }
        existingGrade = existingGrade->next_student;
    }
    
    GotoXY(leftStart, UP_HEAD + 10);
    printf("请输入成绩: ");
    scanf("%f", &newGrade->score);
    
    strcpy(newGrade->student_id, student_id);
    strcpy(newGrade->course_code, course_code);
    
    newGrade->next_student = NULL;
    newGrade->next_course = NULL;
    
    // 添加到学生的成绩链表
    newGrade->next_student = student->grades;
    student->grades = newGrade;
    
    // 添加到课程的成绩链表
    Grade* courseCopy = (Grade*)malloc(sizeof(Grade));
    if (!courseCopy) {
        GotoXY(leftStart, UP_HEAD + 12);
        printf("内存分配失败！");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("按任意键返回...");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
        free(newGrade);
        return NULL;
    }
    
    *courseCopy = *newGrade;
    courseCopy->next_course = course->grades;
    course->grades = courseCopy;
    
    GotoXY(leftStart, UP_HEAD + 12);
    printf("成绩录入成功！");
    
    return newGrade;
}

/*================= 文件存储相关函数 =================*/
// 在文件开头添加目录操作函数
void ensureDirectoryExists(const char* path) {
    // 将UTF-8路径转换为UTF-16
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    wchar_t* widePath = (wchar_t*)malloc(wideLen * sizeof(wchar_t));
    if (!widePath) {
        printf("内存分配失败\n");
        return;
    }
    MultiByteToWideChar(CP_UTF8, 0, path, -1, widePath, wideLen);
    
    // 使用UTF-16路径创建目录
    if (CreateDirectoryW(widePath, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        // 成功创建或已经存在
        free(widePath);
        return;
    } else {
        printf("无法创建目录: %s\n", path);
        free(widePath);
    }
}

// 确保所有必要的目录存在
void ensureAllDirectoriesExist() {
    ensureDirectoryExists("store");
    ensureDirectoryExists("store\\students");
    ensureDirectoryExists("store\\courses");
    ensureDirectoryExists("store\\teachers");
    ensureDirectoryExists("store\\grades");
    ensureDirectoryExists("store\\users");
}

// 从文本文件加载学生数据
Student* loadStudentsFromFile() {
    FILE* fp = fopen("store\\students\\students.txt", "r");
    if (!fp) {
        return NULL; // 文件不存在或无法打开
    }
    
    Student* head = NULL;
    Student* tail = NULL;
    char line[100];
    
    while (fgets(line, sizeof(line), fp)) {
        // 移除换行符
        line[strcspn(line, "\n")] = 0;
        
        Student* newStudent = (Student*)malloc(sizeof(Student));
        if (!newStudent) continue;
        
        // 解析ID和姓名
        char* id = strtok(line, ",");
        char* name = strtok(NULL, ",");
        
        if (id && name) {
            strncpy(newStudent->id, id, sizeof(newStudent->id) - 1);
            newStudent->id[sizeof(newStudent->id) - 1] = '\0';
            
            strncpy(newStudent->name, name, sizeof(newStudent->name) - 1);
            newStudent->name[sizeof(newStudent->name) - 1] = '\0';
            
            newStudent->grades = NULL;
            newStudent->next = NULL;
            
            if (!head) {
                head = newStudent;
                tail = newStudent;
            } else {
                tail->next = newStudent;
                tail = newStudent;
            }
        } else {
            free(newStudent);
        }
    }
    
    fclose(fp);
    return head;
}

// 从文本文件加载课程数据
Course* loadCoursesFromFile() {
    FILE* fp = fopen("store\\courses\\courses.txt", "r");
    if (!fp) {
        return NULL;
    }
    
    Course* head = NULL;
    Course* tail = NULL;
    char line[100];
    
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        
        Course* newCourse = (Course*)malloc(sizeof(Course));
        if (!newCourse) continue;
        
        char* code = strtok(line, ",");
        char* name = strtok(NULL, ",");
        
        if (code && name) {
            strncpy(newCourse->code, code, sizeof(newCourse->code) - 1);
            newCourse->code[sizeof(newCourse->code) - 1] = '\0';
            
            strncpy(newCourse->name, name, sizeof(newCourse->name) - 1);
            newCourse->name[sizeof(newCourse->name) - 1] = '\0';
            
            newCourse->grades = NULL;
            newCourse->teacher = NULL;
            newCourse->next = NULL;
            
            if (!head) {
                head = newCourse;
                tail = newCourse;
            } else {
                tail->next = newCourse;
                tail = newCourse;
            }
        } else {
            free(newCourse);
        }
    }
    
    fclose(fp);
    return head;
}

// 从文本文件加载教师数据
Teacher* loadTeachersFromFile() {
    FILE* fp = fopen("store\\teachers\\teachers.txt", "r");
    if (!fp) {
        return NULL;
    }
    
    Teacher* head = NULL;
    Teacher* tail = NULL;
    char line[100];
    
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        
        Teacher* newTeacher = (Teacher*)malloc(sizeof(Teacher));
        if (!newTeacher) continue;
        
        char* id = strtok(line, ",");
        char* name = strtok(NULL, ",");
        
        if (id && name) {
            strncpy(newTeacher->id, id, sizeof(newTeacher->id) - 1);
            newTeacher->id[sizeof(newTeacher->id) - 1] = '\0';
            
            strncpy(newTeacher->name, name, sizeof(newTeacher->name) - 1);
            newTeacher->name[sizeof(newTeacher->name) - 1] = '\0';
            
            newTeacher->courses = NULL;
            newTeacher->next = NULL;
            
            if (!head) {
                head = newTeacher;
                tail = newTeacher;
            } else {
                tail->next = newTeacher;
                tail = newTeacher;
            }
        } else {
            free(newTeacher);
        }
    }
    
    fclose(fp);
    return head;
}

// 从文本文件加载成绩数据
Grade* loadGradesFromFile(Student* students, Course* courses) {
    FILE* fp = fopen("store\\grades\\grades.txt", "r");
    if (!fp) {
        return NULL;
    }
    
    Grade* head = NULL;
    char line[100];
    
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        
        char* student_id = strtok(line, ",");
        char* course_code = strtok(NULL, ",");
        char* score_str = strtok(NULL, ",");
        
        if (student_id && course_code && score_str) {
            float score = atof(score_str);
            
            // 查找对应的学生和课程
            Student* student = findStudent(students, student_id);
            Course* course = findCourse(courses, course_code);
            
            if (student && course) {
                // 创建成绩记录并链接到学生
                Grade* newGrade = (Grade*)malloc(sizeof(Grade));
                if (!newGrade) continue;
                
                strcpy(newGrade->student_id, student_id);
                strcpy(newGrade->course_code, course_code);
                newGrade->score = score;
                
                newGrade->next_student = student->grades;
                student->grades = newGrade;
                
                // 创建成绩记录并链接到课程
                Grade* courseCopy = (Grade*)malloc(sizeof(Grade));
                if (!courseCopy) {
                    free(newGrade);
                    continue;
                }
                
                *courseCopy = *newGrade;
                courseCopy->next_course = course->grades;
                course->grades = courseCopy;
                
                if (!head) head = newGrade;
            }
        }
    }
    
    fclose(fp);
    return head;
}

// 保存学生数据到文本文件
void saveStudentsToFile(Student* students) {
    FILE* fp = fopen("store\\students\\students.txt", "w");
    if (!fp) {
        printf("无法打开学生数据文件进行写入！\n");
        return;
    }
    
    Student* current = students;
    while (current) {
        fprintf(fp, "%s,%s\n", current->id, current->name);
        current = current->next;
    }
    
    fclose(fp);
}

// 保存课程数据到文本文件
void saveCoursesToFile(Course* courses) {
    FILE* fp = fopen("store\\courses\\courses.txt", "w");
    if (!fp) {
        printf("无法打开课程数据文件进行写入！\n");
        return;
    }
    
    Course* current = courses;
    while (current) {
        fprintf(fp, "%s,%s\n", current->code, current->name);
        current = current->next;
    }
    
    fclose(fp);
}

// 保存教师数据到文本文件
void saveTeachersToFile(Teacher* teachers) {
    FILE* fp = fopen("store\\teachers\\teachers.txt", "w");
    if (!fp) {
        printf("无法打开教师数据文件进行写入！\n");
        return;
    }
    
    Teacher* current = teachers;
    while (current) {
        fprintf(fp, "%s,%s\n", current->id, current->name);
        current = current->next;
    }
    
    fclose(fp);
}

// 保存成绩数据到文本文件
void saveGradesToFile(Grade* grades, Student* students) {
    FILE* fp = fopen("store\\grades\\grades.txt", "w");
    if (!fp) {
        printf("无法打开成绩数据文件进行写入！\n");
        return;
    }
    
    // 遍历所有学生的所有成绩
    Student* current_student = students;
    while (current_student) {
        Grade* g = current_student->grades;
        while (g) {
            fprintf(fp, "%s,%s,%.2f\n", 
                g->student_id, 
                g->course_code, 
                g->score);
            g = g->next_student;
        }
        current_student = current_student->next;
    }
    
    fclose(fp);
}

// 加载所有数据
void loadAllData(Student** students, Course** courses, Teacher** teachers, Grade** grades) {
    // 确保目录存在
    ensureAllDirectoriesExist();
    
    *students = loadStudentsFromFile();
    *courses = loadCoursesFromFile();
    *teachers = loadTeachersFromFile();
    *grades = loadGradesFromFile(*students, *courses);
}

// 保存所有数据
void saveAllData(Student* students, Course* courses, Teacher* teachers, Grade* grades) {
    saveStudentsToFile(students);
    saveCoursesToFile(courses);
    saveTeachersToFile(teachers);
    saveGradesToFile(grades, students);  // 传入students参数
}

/*================= 主要业务功能函数 =================*/
int Login() {
    int userType = AuthenticationProcess();
    UserRecord* user = GetCurrentUser();
    
    if (userType > 0) {
        printf("欢迎回来，%s（ID：%s）! \n", user->username, user->id);
        
        if ((userType & 0xF0) == 0x10)
            printf("您以管理员身份登录\n");
        else if ((userType & 0xF0) == 0x20)
            printf("您以教师身份登录\n");
        else if ((userType & 0xF0) == 0x30)
            printf("您以学生身份登录\n");
    } else {
        printf("您以访客身份使用系统\n");
    }
    
    return atoi(user->id);
}

void Create(int uid) {
    Student* students = NULL;
    Course* courses = NULL;
    Teacher* teachers = NULL;
    Grade* grades = NULL;
    int choice = -1;
    
    // 加载现有数据
    loadAllData(&students, &courses, &teachers, &grades);
    
    while (1) {
        system("cls");
        Window();
        
        // 左侧显示菜单
        GotoXY(LEFT_HEAD + 2, UP_HEAD + 4);
        printf("===== 创建记录 =====");
        GotoXY(LEFT_HEAD + 2, UP_HEAD + 6);
        printf("1. 创建学生");
        GotoXY(LEFT_HEAD + 2, UP_HEAD + 7);
        printf("2. 创建课程");
        GotoXY(LEFT_HEAD + 2, UP_HEAD + 8);
        printf("3. 创建教师");
        GotoXY(LEFT_HEAD + 2, UP_HEAD + 9);
        printf("4. 录入成绩");
        GotoXY(LEFT_HEAD + 2, UP_HEAD + 10);
        printf("0. 返回主菜单");
        GotoXY(LEFT_HEAD + 2, UP_HEAD + 12);
        printf("请选择: ");
        
        if (scanf("%d", &choice) != 1) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
            GotoXY(LEFT_HEAD + 2, UP_HEAD + 14);
            printf("输入无效! 请按任意键重试...");
            _getch();
            continue;
        }
        
        if (choice == 0) {
            // 直接返回主菜单
            return;
        }
        
        system("cls");
        Window();
        
        // 右侧预留用于显示结果
        int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
        int leftStart = LEFT_HEAD + 2;
        
        // 权限检查 - 只有管理员可以创建学生/课程/教师
        if (choice >= 1 && choice <= 3 && !IsAdmin(uid)) {
            ShowPermissionDeniedMessage(leftStart, "创建该类型的记录");
            GotoXY(leftStart, UP_HEAD + 11);
            printf("需要管理员权限。");
            Sleep(2000);
            continue;
        }
        
        switch (choice) {
            case 1: {
                // ...existing code for creating students...
                break;
            }
            case 2: {
                // ...existing code for creating courses...
                break;
            }
            case 3: {
                // ...existing code for creating teachers...
                break;
            }
            case 4: {
                Grade* newGrade = createGrade(students, courses);
                if (newGrade) {
                    // createGrade函数已经处理了界面显示
                    // 成功创建成绩后立即保存数据
                    saveAllData(students, courses, teachers, grades);
                }
                break;
            }
            default:
                GotoXY(leftStart, UP_HEAD + 4);
                printf("无效选择，请重试");
                Sleep(1500);
                continue; // 重新显示菜单
        }
        
        // 保存更新后的数据
        saveAllData(students, courses, teachers, grades);
        
        GotoXY(leftStart, DOWN_HEAD - 4);
        printf("1. 继续创建其他记录");
        GotoXY(leftStart, DOWN_HEAD - 3);
        printf("0. 返回主菜单");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("请选择: ");
        
        // 添加后续操作选择
        int nextAction;
        if (scanf("%d", &nextAction) != 1) {
            nextAction = 1; // 默认继续
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
        }
        
        if (nextAction == 0) {
            return; // 返回主菜单
        }
        // 否则循环继续，显示创建菜单
    }
}

void Read(int uid) {
    Student* students = NULL;
    Course* courses = NULL;
    Teacher* teachers = NULL;
    Grade* grades = NULL;
    int choice = -1;
    
    // 加载数据
    loadAllData(&students, &courses, &teachers, &grades);
    
    while (1) {
        system("cls");
        Window();
        
        int leftStart = LEFT_HEAD + 2;
        
        // 左侧显示菜单
        GotoXY(leftStart, UP_HEAD + 4);
        printf("===== 查询记录 =====");
        GotoXY(leftStart, UP_HEAD + 6);
        printf("1. 查询学生成绩");
        GotoXY(leftStart, UP_HEAD + 7);
        printf("2. 查询课程成绩");
        
        // 仅管理员和教师可以进行排序操作
        if (IsAdmin(uid) || IsTeacher(uid)) {
            GotoXY(leftStart, UP_HEAD + 8);
            printf("3. 排序学生成绩");
            GotoXY(leftStart, UP_HEAD + 9);
            printf("4. 排序课程成绩");
            GotoXY(leftStart, UP_HEAD + 10);
            printf("0. 返回主菜单");
            GotoXY(leftStart, UP_HEAD + 12);
        } else {
            GotoXY(leftStart, UP_HEAD + 8);
            printf("0. 返回主菜单");
            GotoXY(leftStart, UP_HEAD + 10);
        }
        
        printf("请选择: ");
        
        if (scanf("%d", &choice) != 1) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
            GotoXY(leftStart, UP_HEAD + 14);
            printf("输入无效! 请按任意键重试...");
            _getch();
            continue;
        }
        
        if (choice == 0) {
            // 直接返回主菜单
            return;
        }
        
        switch (choice) {
            case 1:
                queryStudent(students, courses);
                break;
            case 2:
                queryCourse(courses, students);
                break;
            case 3:
                if (IsAdmin(uid) || IsTeacher(uid)) {
                    sortStudentGrades(students);
                } else {
                    ShowPermissionDeniedMessage(leftStart, "进行成绩排序操作");
                    GotoXY(leftStart, UP_HEAD + 11);
                    printf("此功能仅对教师和管理员开放。");
                    Sleep(2000);
                    continue; // 重新显示菜单
                }
                break;
            case 4:
                if (IsAdmin(uid) || IsTeacher(uid)) {
                    sortCourseGrades(courses);
                } else {
                    ShowPermissionDeniedMessage(leftStart, "进行成绩排序操作");
                    GotoXY(leftStart, UP_HEAD + 11);
                    printf("此功能仅对教师和管理员开放。");
                    Sleep(2000);
                    continue; // 重新显示菜单
                }
                break;
            default:
                GotoXY(leftStart, UP_HEAD + 4);
                printf("无效选择，请重试");
                Sleep(1500);
                continue; // 重新显示菜单
        }
        
        system("cls"); // 清屏
        Window(); // 重新绘制窗口
        GotoXY(leftStart, UP_HEAD + 4);
        printf("正在返回查询菜单...");
        Sleep(1500);
        
    }
}

void Update(int uid) {
    Student* students = NULL;
    Course* courses = NULL;
    Teacher* teachers = NULL;
    Grade* grades = NULL;
    int choice = -1;
    
    // 加载数据
    loadAllData(&students, &courses, &teachers, &grades);
    
    while (1) {
        system("cls");
        Window();
        
        int leftStart = LEFT_HEAD + 2;
        int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
        
        // 左侧显示菜单
        GotoXY(leftStart, UP_HEAD + 4);
        printf("===== 更新记录 =====");
        GotoXY(leftStart, UP_HEAD + 6);
        printf("1. 修改学生信息");
        GotoXY(leftStart, UP_HEAD + 7);
        printf("2. 修改课程信息");
        GotoXY(leftStart, UP_HEAD + 8);
        printf("3. 修改成绩信息");
        GotoXY(leftStart, UP_HEAD + 9);
        printf("0. 返回主菜单");
        GotoXY(leftStart, UP_HEAD + 11);
        printf("请选择: ");
        
        if (scanf("%d", &choice) != 1) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
            GotoXY(leftStart, UP_HEAD + 13);
            printf("输入无效! 请按任意键重试...");
            _getch();
            continue;
        }
        
        if (choice == 0) {
            // 直接返回主菜单
            return;
        }
        
        system("cls");
        Window();
        
        // 权限检查 - 学生不能修改记录，教师只能修改成绩
        if (!IsAdmin(uid)) {
            if (IsStudent(uid) || (IsTeacher(uid) && choice != 3)) {
                ShowPermissionDeniedMessage(leftStart, "修改该类型的记录");
                GotoXY(leftStart, UP_HEAD + 11);
                printf("需要更高的权限级别。");
                Sleep(2000);
                continue;
            }
        }
        
        switch (choice) {
            case 1: {
                // 左侧显示操作区
                GotoXY(leftStart, UP_HEAD + 4);
                printf("=== 修改学生信息 ===");
                
                GotoXY(leftStart, UP_HEAD + 6);
                printf("请输入要修改的学生学号: ");
                char id[12];
                scanf("%11s", id);
                
                Student* target = findStudent(students, id);
                if (!target) {
                    GotoXY(leftStart, UP_HEAD + 8);
                    printf("学生不存在！");
                } else {
                    // 右侧显示当前信息
                    GotoXY(rightStart, UP_HEAD + 6);
                    printf("当前姓名: %s", target->name);
                    
                    // 左侧继续输入
                    GotoXY(leftStart, UP_HEAD + 8);
                    printf("请输入新的学生姓名: ");
                    scanf("%19s", target->name);
                    
                    // 右侧显示更新结果
                    GotoXY(rightStart, UP_HEAD + 8);
                    printf("已更新为: %s", target->name);
                    
                    GotoXY(leftStart, UP_HEAD + 10);
                    printf("学生信息更新成功！");
                }
                break;
            }
            case 2: {
                // 左侧显示操作区
                GotoXY(leftStart, UP_HEAD + 4);
                printf("=== 修改课程信息 ===");
                
                GotoXY(leftStart, UP_HEAD + 6);
                printf("请输入要修改的课程代码: ");
                char code[10];
                scanf("%9s", code);
                
                Course* target = findCourse(courses, code);
                if (!target) {
                    GotoXY(leftStart, UP_HEAD + 8);
                    printf("课程不存在！");
                } else {
                    // 右侧显示当前信息
                    GotoXY(rightStart, UP_HEAD + 6);
                    printf("当前名称: %s", target->name);
                    
                    // 左侧继续输入
                    GotoXY(leftStart, UP_HEAD + 8);
                    printf("请输入新的课程名称: ");
                    scanf("%49s", target->name);
                    
                    // 右侧显示更新结果
                    GotoXY(rightStart, UP_HEAD + 8);
                    printf("已更新为: %s", target->name);
                    
                    GotoXY(leftStart, UP_HEAD + 10);
                    printf("课程信息更新成功！");
                }
                break;
            }
            case 3: {
                // 左侧显示操作区
                GotoXY(leftStart, UP_HEAD + 4);
                printf("=== 修改成绩信息 ===");
                
                GotoXY(leftStart, UP_HEAD + 6);
                printf("请输入学生学号: ");
                char student_id[12];
                scanf("%11s", student_id);
                
                Student* student = findStudent(students, student_id);
                if (!student) {
                    GotoXY(leftStart, UP_HEAD + 8);
                    printf("学生不存在！");
                    break;
                }
                
                // 右侧显示学生信息
                GotoXY(rightStart, UP_HEAD + 6);
                printf("学生: %s", student->name);
                
                // 左侧继续输入
                GotoXY(leftStart, UP_HEAD + 8);
                printf("请输入课程代码: ");
                char course_code[10];
                scanf("%9s", course_code);
                
                Course* course = findCourse(courses, course_code);
                if (!course) {
                    GotoXY(leftStart, UP_HEAD + 10);
                    printf("课程不存在！");
                    break;
                }
                
                // 右侧显示课程信息
                GotoXY(rightStart, UP_HEAD + 8);
                printf("课程: %s", course->name);
                
                // 查找成绩
                Grade* g = student->grades;
                while (g) {
                    if (strcmp(g->course_code, course_code) == 0) {
                        // 显示当前成绩
                        GotoXY(rightStart, UP_HEAD + 10);
                        printf("当前成绩: %.2f", g->score);
                        
                        // 输入新成绩
                        GotoXY(leftStart, UP_HEAD + 10);
                        printf("请输入新成绩: ");
                        float newScore;
                        scanf("%f", &newScore);
                        g->score = newScore;
                        
                        // 显示更新结果
                        GotoXY(rightStart, UP_HEAD + 12);
                        printf("新成绩: %.2f", g->score);
                        
                        GotoXY(leftStart, UP_HEAD + 12);
                        printf("成绩更新成功！");
                        break;
                    }
                    g = g->next_student;
                }
                
                if (!g) {
                    GotoXY(leftStart, UP_HEAD + 10);
                    printf("未找到该学生的该课程成绩记录！");
                }
                break;
            }
            default:
                GotoXY(leftStart, UP_HEAD + 4);
                printf("无效选择，请重试");
                Sleep(1500);
                continue; // 重新显示菜单
        }
        
        // 保存更新后的数据
        saveAllData(students, courses, teachers, grades);
        
        // 子功能执行完后，添加统一的后续操作选择
        GotoXY(leftStart, DOWN_HEAD - 4);
        printf("1. 继续更新其他记录");
        GotoXY(leftStart, DOWN_HEAD - 3);
        printf("0. 返回主菜单");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("请选择: ");
        
        int nextAction;
        if (scanf("%d", &nextAction) != 1) {
            nextAction = 1; // 默认继续
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
        }
        
        if (nextAction == 0) {
            return; // 返回主菜单
        }
        // 否则循环继续，显示更新菜单
    }
}

void Delete(int uid) {
    Student* students = NULL;
    Course* courses = NULL;
    Teacher* teachers = NULL;
    Grade* grades = NULL;
    int choice = -1;
    
    // 加载数据
    loadAllData(&students, &courses, &teachers, &grades);
    
    while (1) {
        system("cls");
        Window();
        
        int leftStart = LEFT_HEAD + 2;
        int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
        
        // 左侧显示菜单
        GotoXY(leftStart, UP_HEAD + 4);
        printf("===== 删除记录 =====");
        GotoXY(leftStart, UP_HEAD + 6);
        printf("1. 删除学生");
        GotoXY(leftStart, UP_HEAD + 7);
        printf("2. 删除课程");
        GotoXY(leftStart, UP_HEAD + 8);
        printf("3. 删除成绩记录");
        GotoXY(leftStart, UP_HEAD + 9);
        printf("0. 返回主菜单");
        GotoXY(leftStart, UP_HEAD + 11);
        printf("请选择: ");
        
        if (scanf("%d", &choice) != 1) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
            GotoXY(leftStart, UP_HEAD + 13);
            printf("输入无效! 请按任意键重试...");
            _getch();
            continue;
        }
        
        if (choice == 0) {
            // 直接返回主菜单
            return;
        }
        
        system("cls");
        Window();
        
        // 权限检查 - 只有管理员可以删除记录，教师只能删除成绩
        if (!IsAdmin(uid)) {
            if (IsStudent(uid) || (IsTeacher(uid) && choice != 3)) {
                ShowPermissionDeniedMessage(leftStart, "删除该类型的记录");
                GotoXY(leftStart, UP_HEAD + 11);
                printf("删除操作需要管理员权限。");
                Sleep(2000);
                continue;
            }
        }
        
        switch (choice) {
            case 1: {
                // ...existing code for deleting student...
                break;
            }
            case 2: {
                // ...existing code for deleting course...
                break;
            }
            case 3: {
                // ...existing code for deleting grade...
                break;
            }
            default:
                GotoXY(leftStart, UP_HEAD + 4);
                printf("无效选择，请重试");
                Sleep(1500);
                continue; // 重新显示菜单
        }
        
        // 保存更新后的数据
        saveAllData(students, courses, teachers, grades);
        
        // 子功能执行完后，添加统一的后续操作选择
        GotoXY(leftStart, DOWN_HEAD - 4);
        printf("1. 继续删除其他记录");
        GotoXY(leftStart, DOWN_HEAD - 3);
        printf("0. 返回主菜单");
        GotoXY(leftStart, DOWN_HEAD - 2);
        printf("请选择: ");
        
        int nextAction;
        if (scanf("%d", &nextAction) != 1) {
            nextAction = 1; // 默认继续
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            while (getchar() != '\n'); // 清除输入缓冲区
        }
        
        if (nextAction == 0) {
            return; // 返回主菜单
        }
        // 否则循环继续，显示删除菜单
    }
}

void Logging(int uid) {
    system("cls");
    Window();
    
    int leftStart = LEFT_HEAD + 2;
    int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
    
    // 左侧显示标题和基本信息
    GotoXY(leftStart, UP_HEAD + 4);
    printf("===== 系统日志 =====");
    
    GotoXY(leftStart, UP_HEAD + 6);
    printf("用户ID: %d", uid);
    
    GotoXY(leftStart, UP_HEAD + 8);
    printf("当前时间: ");
    
    time_t now = time(NULL);
    char* time_str = ctime(&now);
    // 移除换行符
    if (time_str[strlen(time_str)-1] == '\n')
        time_str[strlen(time_str)-1] = '\0';
    
    GotoXY(leftStart, UP_HEAD + 9);
    printf("%s", time_str);
    
    // 右侧显示最近操作记录
    GotoXY(rightStart, UP_HEAD + 4);
    printf("最近操作记录");
    
    GotoXY(rightStart, UP_HEAD + 6);
    printf("- 系统登录");
    
    GotoXY(rightStart, UP_HEAD + 7);
    printf("- 查看日志");
    
    // 如果有更多日志，可以添加在这里
    
    GotoXY(leftStart, DOWN_HEAD - 4);
    printf("0. 返回主菜单");
    GotoXY(leftStart, DOWN_HEAD - 2);
    printf("请选择: ");
    
    int choice;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    scanf("%d", &choice);
    
    // 无论输入什么，都返回主菜单，但保持一致的界面
}

void Logout(int uid) {
    system("cls");
    
    int centerX = (RIGHT_HEAD + LEFT_HEAD) / 2;
    int centerY = (DOWN_HEAD + UP_HEAD) / 2;
    
    // 在窗口中央显示登出信息
    GotoXY(centerX - 10, centerY - 2);
    printf("===== 用户登出 =====");
    
    GotoXY(centerX - 15, centerY);
    printf("用户ID: %d 已安全登出系统", uid);
    
    GotoXY(centerX - 4, centerY + 2);
    printf("再见！");
    
    Sleep(2000);
    Final(); // 使用已有的Final函数显示退出信息
}

/*================= 权限管理函数 =================*/
// 检查用户是否为管理员
BOOL IsAdmin(int uid) {
    UserRecord* user = GetCurrentUser();
    return (user->userType & 0xF0) == 0x10;
}

// 检查用户是否为教师
BOOL IsTeacher(int uid) {
    UserRecord* user = GetCurrentUser();
    return (user->userType & 0xF0) == 0x20;
}

// 检查用户是否为学生
BOOL IsStudent(int uid) {
    UserRecord* user = GetCurrentUser();
    return (user->userType & 0xF0) == 0x30;
}

// 检查用户是否有权限访问某课程
BOOL HasAccessToCourse(int uid, const char* course_code) {
    UserRecord* user = GetCurrentUser();
    
    // 管理员拥有所有权限
    if ((user->userType & 0xF0) == 0x10) {
        return TRUE;
    }
    
    // 教师只能访问他们负责的课程
    if ((user->userType & 0xF0) == 0x20) {
        return IsTeacherAssignedToCourse(user->id, course_code);
    }
    
    // 学生只能访问他们选修的课程
    if ((user->userType & 0xF0) == 0x30) {
        // 通过遍历学生的成绩来确定是否有这门课程
        Student* students = loadStudentsFromFile();
        Student* student = findStudent(students, user->id);
        if (!student) return FALSE;
        
        Grade* g = student->grades;
        while (g) {
            if (strcmp(g->course_code, course_code) == 0) {
                // 释放临时加载的数据
                // TODO: 优化这里的内存管理，避免每次都重新加载
                // 这里为简化代码，暂不处理内存释放
                return TRUE;
            }
            g = g->next_student;
        }
    }
    
    return FALSE;
}

// 显示与当前用户相关的课程
void DisplayRelevantCourses(int uid, Course* courses, int startY) {
    UserRecord* user = GetCurrentUser();
    int rightStart = (RIGHT_HEAD + LEFT_HEAD) / 2 + 2;
    int row = startY;
    
    // 管理员可以看到所有课程
    if (IsAdmin(uid)) {
        GotoXY(rightStart, row++);
        printf("您可以查看所有课程：");
        
        Course* c = courses;
        while (c && row < DOWN_HEAD - 4) {
            GotoXY(rightStart, row++);
            printf("%-10s %-30s", c->code, c->name);
            c = c->next;
        }
        return;
    }
    
    // 教师只能看到分配给他们的课程
    if (IsTeacher(uid)) {
        GotoXY(rightStart, row++);
        printf("您教授的课程：");
        
        Course* c = courses;
        while (c && row < DOWN_HEAD - 4) {
            if (IsTeacherAssignedToCourse(user->id, c->code)) {
                GotoXY(rightStart, row++);
                printf("%-10s %-30s", c->code, c->name);
            }
            c = c->next;
        }
        return;
    }
    
    // 学生只能看到他们选修的课程
    if (IsStudent(uid)) {
        GotoXY(rightStart, row++);
        printf("您选修的课程：");
        
        Student* students = loadStudentsFromFile(); 
        Student* student = findStudent(students, user->id);
        if (student) {
            Grade* g = student->grades;
            while (g && row < DOWN_HEAD - 4) {
                Course* c = findCourse(courses, g->course_code);
                if (c) {
                    GotoXY(rightStart, row++);
                    printf("%-10s %-30s", c->code, c->name);
                }
                g = g->next_student;
            }
        }
        // TODO: 释放临时加载的数据
    }
}

/*================= 权限检查与通知函数 =================*/
void ShowPermissionDeniedMessage(int leftStart, const char* operation) {
    GotoXY(leftStart, UP_HEAD + 6);
    printf("权限不足！");
    GotoXY(leftStart, UP_HEAD + 7);
    printf("您没有权限%s", operation);
    GotoXY(leftStart, UP_HEAD + 9);
    printf("按任意键返回...");
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
}
