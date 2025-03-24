#include "sim.h"

/*================= 查询功能实现 =================*/
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

/*================= 排序功能实现 =================*/
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

/*================= 使用示例 =================*/
int main() {
    // 初始化测试数据
    Student* students = createStudent("S001", "张三");
    Course* courses = createCourse("C001", "数据结构");
    
    // 添加测试成绩
    addGrade(students, courses, 85.5);
    addGrade(students, courses, 92.0);
    addGrade(students, courses, 78.5);

    // 查询演示
    queryStudent(students);
    queryCourse(courses);

    // 排序演示
    sortStudentGrades(students);
    queryStudent(students);  // 显示排序后结果

    sortCourseGrades(courses);
    queryCourse(courses);    // 显示排序后结果

    // 序列化保存
    serialize(students, courses, courses->grades, "data.bin");

    // 释放内存
    freeAll(students, courses, courses->grades);
    return 0;
}
