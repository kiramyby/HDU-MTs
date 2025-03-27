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

// 加载学生链表
Student* loadStudents(FILE* fp) {
    Student* head = NULL;
    Student** current = &head;
    
    while (1) {
        Student* node = (Student*)malloc(sizeof(Student));
        if (!node) {
            printf("内存分配失败!\n");
            // 清理已分配的内存
            Student* temp;
            while (head) {
                temp = head;
                head = head->next;
                free(temp);
            }
            return NULL;
        }
        
        if (fread(node->id, sizeof(char), 12, fp) != 12) {
            free(node);
            break;
        }
        
        if (fread(node->name, sizeof(char), 20, fp) != 20) {
            free(node);
            break;
        }
        
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
        if (!node) {
            printf("内存分配失败!\n");
            // 清理已分配的内存
            Course* temp;
            while (head) {
                temp = head;
                head = head->next;
                free(temp);
            }
            return NULL;
        }
        
        if (fread(node->code, sizeof(char), 10, fp) != 10) {
            free(node);
            break;
        }
        
        if (fread(node->name, sizeof(char), 50, fp) != 50) {
            free(node);
            break;
        }
        
        node->grades = NULL;
        node->next = NULL;
        node->teacher = NULL;
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
        if (!node) {
            printf("内存分配失败!\n");
            // 清理已分配的内存
            Grade* temp;
            while (head) {
                temp = head;
                head = head->next_student;
                free(temp);
            }
            return NULL;
        }
        
        if (fread(node->student_id, sizeof(char), 12, fp) != 12) {
            free(node);
            break;
        }
        
        if (fread(node->course_code, sizeof(char), 10, fp) != 10) {
            free(node);
            break;
        }
        
        if (fread(&node->score, sizeof(float), 1, fp) != 1) {
            free(node);
            break;
        }
        
        node->next_student = NULL;
        node->next_course = NULL;
        *current = node;
        current = &(node->next_student);
    }
    
    return head;
}
