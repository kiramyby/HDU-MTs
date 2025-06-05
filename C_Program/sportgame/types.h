#ifndef TYPES_H
#define TYPES_H

#define MAX_EVENTS 50
#define MAX_STUDENTS 100
#define MAX_REGISTRATIONS 200

// 数据结构定义
typedef struct SportEvent {
    char event_id[10];          // 项目编号
    char name[50];              // 项目名称
    char type[20];              // 田赛/径赛
    char category[20];          // 男子/女子
    char time[20];              // 比赛时间
    char location[50];          // 比赛地点
    char status[20];            // 报名中/已结束
    int max_participants;       // 最大报名人数
    int current_participants;   // 当前报名人数
    struct SportEvent* next;    // 下一个事件节点指针
} SportEvent;

typedef struct Student {
    char student_id[15];        // 学号
    char name[20];              // 姓名
    struct Student* next;       // 下一个学生节点指针
} Student;

typedef struct Registration {
    char reg_id[20];            // 报名编号
    char student_id[15];        // 学号
    char event_id[10];          // 项目编号
    float score;                // 成绩，-1表示未录入
    char time[20];
    struct Registration* next;  // 下一个报名记录节点指针
} Registration;

// 全局链表头指针声明
extern SportEvent* event_head;
extern Student* student_head;
extern Registration* reg_head;

// 当前登录状态
extern int logged_in;

#endif
