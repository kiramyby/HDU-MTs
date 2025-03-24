#include "sim.h"

/* ================== 反序列化相关函数 ================== */
// 学生表
void saveStudents(Student* head, FILE* fp) {
    Student* current = head;
    while (current != NULL) {
        fwrite(current->id, sizeof(char), 12, fp);
        fwrite(current->name, sizeof(char), 20, fp);
        current = current->next;
    }
}

// 写入课程链表
void saveCourses(Course* head, FILE* fp) {
    Course* current = head;
    while (current != NULL) {
        fwrite(current->code, sizeof(char), 10, fp);
        fwrite(current->name, sizeof(char), 50, fp);
        current = current->next;
    }
}

// 写入成绩链表
void saveGrades(Grade* head, FILE* fp) {
    Grade* current = head;
    while (current != NULL) {
        fwrite(current->student_id, sizeof(char), 12, fp);
        fwrite(current->course_code, sizeof(char), 10, fp);
        fwrite(&current->score, sizeof(float), 1, fp);
        current = current->next_student;
    }
}

// 主序列化函数
void serialize(Student* students, Course* courses, Grade* grades, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("文件打开失败");
        return;
    }

    // 写入数据
    saveStudents(students, fp);
    saveCourses(courses, fp);
    saveGrades(grades, fp);

    // 计算并写入校验和
    writeChecksum(fp);
    fclose(fp);
}

// 加载学生链表
Student* loadStudents(FILE* fp) {
    Student* head = NULL;
    Student** current = &head;
    while (1) {
        Student* node = (Student*)malloc(sizeof(Student));
        if (fread(node->id, sizeof(char), 12, fp) != 12) break;
        if (fread(node->name, sizeof(char), 20, fp) != 20) break;
        node->grades = NULL;
        node->next = NULL;
        *current = node;
        current = &(node->next);
    }
    return head;
}

// 加载课程链表
Course* loadCourses(FILE* fp) {
    Course* head = NULL;
    Course** current = &head;
    while (1) {
        Course* node = (Course*)malloc(sizeof(Course));
        if (fread(node->code, sizeof(char), 10, fp) != 10) break;
        if (fread(node->name, sizeof(char), 50, fp) != 50) break;
        node->grades = NULL;
        node->next = NULL;
        *current = node;
        current = &(node->next);
    }
    return head;
}

// 加载成绩链表
Grade* loadGrades(FILE* fp) {
    Grade* head = NULL;
    Grade** current = &head;
    while (1) {
        Grade* node = (Grade*)malloc(sizeof(Grade));
        if (fread(node->student_id, sizeof(char), 12, fp) != 12) break;
        if (fread(node->course_code, sizeof(char), 10, fp) != 10) break;
        if (fread(&node->score, sizeof(float), 1, fp) != 1) break;
        node->next_student = NULL;
        node->next_course = NULL;
        *current = node;
        current = &(node->next_student);
    }
    return head;
}

/* ================== 序列化相关函数 ================== */
// 主反序列化函数
void deserialize(Student** students, Course** courses, Grade** grades, const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("文件打开失败");
        return;
    }

    // 校验数据完整性
    if (!verifyChecksum(fp)) {
        printf("数据损坏！\n");
        fclose(fp);
        return;
    }

    // 重置文件指针并加载数据
    fseek(fp, 0, SEEK_SET);
    *students = loadStudents(fp);
    *courses = loadCourses(fp);
    *grades = loadGrades(fp);
    fclose(fp);

    // 关联成绩与课程、学生
    Grade* current = *grades;
    while (current != NULL) {
        // 查找对应学生和课程
        Student* stu = *students;
        while (stu != NULL && strcmp(stu->id, current->student_id) != 0) stu = stu->next;
        Course* cour = *courses;
        while (cour != NULL && strcmp(cour->code, current->course_code) != 0) cour = cour->next;

        // 更新指针
        if (stu && cour) {
            current->next_student = stu->grades;
            stu->grades = current;
            current->next_course = cour->grades;
            cour->grades = current;
        }
        current = current->next_student;
    }
}