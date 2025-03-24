#include "sim.h"
#include "auth.h"

/* 登录 */
int Login() {
    // 调用身份验证流程
    int userType = AuthenticationProcess();
    UserRecord* user = GetCurrentUser();
    
    if (userType > 0) {
        printf("欢迎回来，%s! \n", user->username);
        
        // 根据用户类型显示不同的欢迎信息
        if ((userType & 0xF0) == 0x10) {
            printf("您以管理员身份登录\n");
        } else if ((userType & 0xF0) == 0x20) {
            printf("您以教师身份登录\n");
        } else if ((userType & 0xF0) == 0x30) {
            printf("您以学生身份登录\n");
        }
    } else {
        printf("您以访客身份使用系统\n");
    }
    
    return user->id;
}

/* 视图绘制 */
//窗口
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

//菜单
int Menu() {
	GotoXY(40, 12); 			     //定位光标位置
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
	char ch;
	int op = 0;
	ch = _getch();   			    //接收用户输入的菜单选项
	switch (ch) {				//根据选项设置返回结果值
		case '1': op = 1; break;
        case '2': op = 2; break;
        case '3': op = 3; break;
        case '4': op = 4; break;
        case '5': op = 5; break;
        default: 
	}
	system("cls");  				//调用系统命令cls完成清屏操作
	return op;
}

// 结束
void Final() {
    GotoXY(45, 14);
    printf("已登出！ 5秒后自动返回主菜单...");
    Sleep(5000);
    system("cls");
}

/*================= 光标操作 =================*/
//定位
void GotoXY(int x, int y) {
	HANDLE hout;
	COORD cor;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = x;
	cor.Y = y;
	SetConsoleCursorPosition(hout, cor);
}

//隐藏
void HideCursor() {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cor_info = { 1, 0 };
	SetConsoleCursorInfo(hout, &cor_info);
}

/* 查询功能实现 */
// 查找学生函数
Student* findStudent(Student* students, const char* student_id) {
    Student* current = students;
    while (current) {
        if (strcmp(current->id, student_id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 查找课程函数
Course* findCourse(Course* courses, const char* course_code) {
    Course* current = courses;
    while (current) {
        if (strcmp(current->code, course_code) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 学生成绩查询
void queryStudent(Student* students) {
    char id[12];
    printf("请输入要查询的学生学号: ");
    scanf("%11s", id);

    Student* target = findStudent(students, id);
    if (!target) {
        printf("学生不存在！\n");
        return;
    }

    printf("\n学生 %s 的成绩单：\n", target->name);
    printf("%-12s %-10s %-6s\n", "课程号", "课程名", "成绩");
    
    Grade* g = target->grades;
    while (g) {
        // 查找对应课程信息
        Course* cour = findCourse(students->next ? students->next : students, g->course_code); // 简化查找
        printf("%-12s %-10s %.2f\n", 
               g->course_code,
               cour ? cour->name : "未知课程",
               g->score);
        g = g->next_student;
    }
}

// 课程成绩查询
void queryCourse(Course* courses) {
    char code[10];
    printf("请输入要查询的课程代码: ");
    scanf("%9s", code);

    Course* target = findCourse(courses, code);
    if (!target) {
        printf("课程不存在！\n");
        return;
    }

    printf("\n课程 %s 成绩单：\n", target->name);
    printf("%-12s %-10s %-6s\n", "学号", "姓名", "成绩");
    
    Grade* g = target->grades;
    while (g) {
        // 查找对应学生信息
        Student* stu = findStudent(courses->next ? courses->next : courses, g->student_id); // 简化查找
        printf("%-12s %-10s %.2f\n", 
               g->student_id,
               stu ? stu->name : "未知学生",
               g->score);
        g = g->next_course;
    }
}

/* 排序功能实现 */
// 通用排序函数（降序）
void sortGrades(Grade** headRef, CompareFunc compare, int isStudentView) {
    if (!*headRef || !(*headRef)->next_student) return;

    // 把链表转换为数组
    Grade* arr[100];
    int count = 0;
    Grade* current = *headRef;
    
    while (current && count < 100) {
        arr[count++] = current;
        current = isStudentView ? current->next_student : current->next_course;
    }

    // 使用标准库qsort排序
    qsort(arr, count, sizeof(Grade*), (int (*)(const void*, const void*))compare);

    // 重建链表
    *headRef = arr[0];
    for (int i = 0; i < count-1; i++) {
        if (isStudentView) {
            arr[i]->next_student = arr[i+1];
        } else {
            arr[i]->next_course = arr[i+1];
        }
    }
    if (isStudentView) {
        arr[count-1]->next_student = NULL;
    } else {
        arr[count-1]->next_course = NULL;
    }
}

// 比较函数（按成绩降序）
int compareByScoreDesc(const Grade* a, const Grade* b) {
    return (a->score < b->score) ? 1 : -1;
}

// 学生成绩排序接口
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

// 课程成绩排序接口
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
