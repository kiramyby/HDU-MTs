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
    GotoXY(LEFT_HEAD, UP_HEAD);
    printf("┌");
    for (i = LEFT_HEAD + 1; i < RIGHT_HEAD; i++)
        printf("─");
    printf("┐");
    for (i = UP_HEAD + 1; i < 20; i++) {
        GotoXY(LEFT_HEAD, i);
        printf("│");
        GotoXY((RIGHT_HEAD+LEFT_HEAD)/2, i);
        printf("│");
        GotoXY(RIGHT_HEAD, i);
        printf("│");
    }
    GotoXY(LEFT_HEAD, 20);
    printf("└");
    for (i = LEFT_HEAD + 1; i < RIGHT_HEAD; i++)
        printf("─");
    printf("┘");
    GotoXY(LEFT_HEAD, UP_HEAD + 1);
    printf("Student Information Manager");
    HideCursor();
}

int Menu() {
    GotoXY(40, 12);
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
    HideCursor();
    
    char ch = getchar();
    int op = -1;
    switch (ch) {
        case '0': op = 0; break;
        case '1': op = 1; break;
        case '2': op = 2; break;
        case '3': op = 3; break;
        case '4': op = 4; break;
        case '5': op = 5; break;
        default: break;
    }
    system("cls");
    return op;
}

void Final() {
    GotoXY(45, 14);
    printf("已登出！ 5秒后自动返回主菜单...");
    Sleep(5000);
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
    char id[12];
    printf("请输入要查询的学生学号: ");
    scanf("%11s", id);
    
    Student* target = findStudent(students, id);
    if (!target) {
        printf("学生不存在！\n");
        return;
    }
    
    printf("\n学生 %s 的成绩单：\n", target->name);
    printf("%-12s %-30s %-6s\n", "课程号", "课程名", "成绩");
    
    Grade* g = target->grades;
    while (g) {
        Course* cour = findCourse(courses, g->course_code);
        printf("%-12s %-30s %.2f\n", 
               g->course_code,
               cour ? cour->name : "未知课程",
               g->score);
        g = g->next_student;
    }
}

void queryCourse(Course* courses, Student* students) {
    char code[10];
    printf("请输入要查询的课程代码: ");
    scanf("%9s", code);
    
    Course* target = findCourse(courses, code);
    if (!target) {
        printf("课程不存在！\n");
        return;
    }
    
    printf("\n课程 %s 成绩单：\n", target->name);
    printf("%-12s %-20s %-6s\n", "学号", "姓名", "成绩");
    
    Grade* g = target->grades;
    while (g) {
        Student* stu = findStudent(students, g->student_id);
        printf("%-12s %-20s %.2f\n", 
               g->student_id,
               stu ? stu->name : "未知学生",
               g->score);
        g = g->next_course;
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
    char id[12];
    printf("请输入要排序的学生学号: ");
    scanf("%11s", id);
    
    Student* target = findStudent(students, id);
    if (!target) {
        printf("学生不存在！\n");
        return;
    }
    
    sortGrades(&target->grades, compareByScoreDesc, 1);
    printf("已按成绩降序排序！\n");
}

void sortCourseGrades(Course* courses) {
    char code[10];
    printf("请输入要排序的课程代码: ");
    scanf("%9s", code);
    
    Course* target = findCourse(courses, code);
    if (!target) {
        printf("课程不存在！\n");
        return;
    }
    
    sortGrades(&target->grades, compareByScoreDesc, 0);
    printf("已按成绩降序排序！\n");
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
    Grade* newGrade = (Grade*)malloc(sizeof(Grade));
    if (!newGrade) {
        printf("内存分配失败！\n");
        return NULL;
    }
    
    char student_id[12];
    char course_code[10];
    
    printf("请输入学生学号: ");
    scanf("%11s", student_id);
    
    Student* student = findStudent(students, student_id);
    if (!student) {
        printf("学生不存在！\n");
        free(newGrade);
        return NULL;
    }
    
    printf("请输入课程代码: ");
    scanf("%9s", course_code);
    
    Course* course = findCourse(courses, course_code);
    if (!course) {
        printf("课程不存在！\n");
        free(newGrade);
        return NULL;
    }
    
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
        printf("内存分配失败！\n");
        free(newGrade);
        return NULL;
    }
    
    *courseCopy = *newGrade;
    courseCopy->next_course = course->grades;
    course->grades = courseCopy;
    
    return newGrade;
}

/*================= 主要业务功能函数 =================*/
int Login() {
    int userType = AuthenticationProcess();
    UserRecord* user = GetCurrentUser();
    
    if (userType > 0) {
        printf("欢迎回来，%s! \n", user->username);
        
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
    char dataFile[] = "school_data.dat";
    
    // 加载现有数据
    deserialize(&students, &courses, &grades, dataFile);
    
    int choice;
    printf("===== 创建记录 =====\n");
    printf("1. 创建学生\n");
    printf("2. 创建课程\n");
    printf("3. 创建教师\n");
    printf("4. 录入成绩\n");
    printf("0. 返回\n");
    printf("请选择: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            Student* newStudent = createStudent();
            if (newStudent) {
                // 检查是否已存在
                if (findStudent(students, newStudent->id)) {
                    printf("学生ID已存在！\n");
                    free(newStudent);
                } else {
                    newStudent->next = students;
                    students = newStudent;
                    printf("学生创建成功！\n");
                }
            }
            break;
        }
        case 2: {
            Course* newCourse = createCourse();
            if (newCourse) {
                if (findCourse(courses, newCourse->code)) {
                    printf("课程代码已存在！\n");
                    free(newCourse);
                } else {
                    newCourse->next = courses;
                    courses = newCourse;
                    printf("课程创建成功！\n");
                }
            }
            break;
        }
        case 3: {
            Teacher* newTeacher = createTeacher();
            if (newTeacher) {
                if (findTeacher(teachers, newTeacher->id)) {
                    printf("教师ID已存在！\n");
                    free(newTeacher);
                } else {
                    newTeacher->next = teachers;
                    teachers = newTeacher;
                    printf("教师创建成功！\n");
                }
            }
            break;
        }
        case 4: {
            Grade* newGrade = createGrade(students, courses);
            if (newGrade) {
                printf("成绩录入成功！\n");
            }
            break;
        }
        case 0:
            printf("返回主菜单\n");
            break;
        default:
            printf("无效选择\n");
    }
    
    // 保存更新后的数据
    serialize(students, courses, grades, dataFile);
    
    printf("按任意键继续...");
    getchar();
}

void Read(int uid) {
    Student* students = NULL;
    Course* courses = NULL;
    Grade* grades = NULL;
    char dataFile[] = "school_data.dat";
    
    // 加载数据
    deserialize(&students, &courses, &grades, dataFile);
    
    int choice;
    printf("===== 查询记录 =====\n");
    printf("1. 查询学生成绩\n");
    printf("2. 查询课程成绩\n");
    printf("3. 排序学生成绩\n");
    printf("4. 排序课程成绩\n");
    printf("0. 返回\n");
    printf("请选择: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            queryStudent(students, courses);
            break;
        case 2:
            queryCourse(courses, students);
            break;
        case 3:
            sortStudentGrades(students);
            break;
        case 4:
            sortCourseGrades(courses);
            break;
        case 0:
            printf("返回主菜单\n");
            break;
        default:
            printf("无效选择\n");
    }
    
    printf("按任意键继续...");
    getchar();
}

void Update(int uid) {
    Student* students = NULL;
    Course* courses = NULL;
    Grade* grades = NULL;
    char dataFile[] = "school_data.dat";
    
    // 加载数据
    deserialize(&students, &courses, &grades, dataFile);
    
    printf("===== 更新记录 =====\n");
    
    int choice;
    printf("1. 修改学生信息\n");
    printf("2. 修改课程信息\n");
    printf("3. 修改成绩信息\n");
    printf("0. 返回\n");
    printf("请选择: ");
    scanf("%d", &choice);
    
    // 简单的更新示例，实际应用中需要更详细的实现
    switch (choice) {
        case 1: {
            char id[12];
            printf("请输入要修改的学生学号: ");
            scanf("%11s", id);
            
            Student* target = findStudent(students, id);
            if (!target) {
                printf("学生不存在！\n");
            } else {
                printf("请输入新的学生姓名: ");
                scanf("%19s", target->name);
                printf("学生信息更新成功！\n");
            }
            break;
        }
        case 2: {
            char code[10];
            printf("请输入要修改的课程代码: ");
            scanf("%9s", code);
            
            Course* target = findCourse(courses, code);
            if (!target) {
                printf("课程不存在！\n");
            } else {
                printf("请输入新的课程名称: ");
                scanf("%49s", target->name);
                printf("课程信息更新成功！\n");
            }
            break;
        }
        case 3: {
            char student_id[12], course_code[10];
            float newScore;
            
            printf("请输入学生学号: ");
            scanf("%11s", student_id);
            
            printf("请输入课程代码: ");
            scanf("%9s", course_code);
            
            Student* student = findStudent(students, student_id);
            if (!student) {
                printf("学生不存在！\n");
                break;
            }
            
            Grade* g = student->grades;
            while (g) {
                if (strcmp(g->course_code, course_code) == 0) {
                    printf("当前成绩: %.2f\n", g->score);
                    printf("请输入新成绩: ");
                    scanf("%f", &newScore);
                    g->score = newScore;
                    printf("成绩更新成功！\n");
                    break;
                }
                g = g->next_student;
            }
            
            if (!g) {
                printf("未找到该学生的该课程成绩记录！\n");
            }
            break;
        }
        case 0:
            printf("返回主菜单\n");
            break;
        default:
            printf("无效选择\n");
    }
    
    // 保存更新后的数据
    serialize(students, courses, grades, dataFile);
    
    printf("按任意键继续...");
    getchar();
}

void Delete(int uid) {
    Student* students = NULL;
    Course* courses = NULL;
    Grade* grades = NULL;
    char dataFile[] = "school_data.dat";
    
    // 加载数据
    deserialize(&students, &courses, &grades, dataFile);
    
    printf("===== 删除记录 =====\n");
    
    int choice;
    printf("1. 删除学生\n");
    printf("2. 删除课程\n");
    printf("3. 删除成绩记录\n");
    printf("0. 返回\n");
    printf("请选择: ");
    scanf("%d", &choice);
    
    // 删除操作示例
    switch (choice) {
        case 1: {
            char id[12];
            printf("请输入要删除的学生学号: ");
            scanf("%11s", id);
            
            if (!students) {
                printf("学生记录为空！\n");
                break;
            }
            
            if (strcmp(students->id, id) == 0) {
                Student* temp = students;
                students = students->next;
                free(temp);
                printf("学生删除成功！\n");
            } else {
                Student* prev = students;
                Student* curr = students->next;
                while (curr && strcmp(curr->id, id) != 0) {
                    prev = curr;
                    curr = curr->next;
                }
                
                if (curr) {
                    prev->next = curr->next;
                    free(curr);
                    printf("学生删除成功！\n");
                } else {
                    printf("学生不存在！\n");
                }
            }
            break;
        }
        case 0:
            printf("返回主菜单\n");
            break;
        default:
            printf("无效选择或功能尚未实现\n");
    }
    
    // 保存更新后的数据
    serialize(students, courses, grades, dataFile);
    
    printf("按任意键继续...");
    getchar();
}

void Logging(int uid) {
    printf("===== 系统日志 =====\n");
    printf("用户ID: %d\n", uid);
    printf("当前时间: ");
    
    time_t now = time(NULL);
    printf("%s", ctime(&now));
    
    printf("最近操作记录:\n");
    printf("- 系统登录\n");
    printf("- 查看日志\n");
    
    printf("\n按任意键继续...");
    getchar();
}

void Logout(int uid) {
    printf("===== 用户登出 =====\n");
    printf("用户ID: %d 已安全登出系统\n", uid);
    printf("再见！\n");
    
    Sleep(1000);
}
