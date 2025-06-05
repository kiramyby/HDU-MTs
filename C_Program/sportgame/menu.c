#include <stdio.h>
#include "types.h"
#include "menu.h"
#include "event_mgmt.h"
#include "student_mgmt.h"
#include "registration_mgmt.h"
#include "auth.h"
#include "utils.h"

// 统计查询菜单
void statistics_query_menu() {
    int choice;
    do {
        printf("\n=== 统计查询 ===\n");
        printf("1. 各项目报名人数统计\n");
        printf("2. 每个学生的报名数量\n");
        printf("3. 查询比赛成绩\n");
        printf("0. 返回主菜单\n");
        printf("请选择操作: ");
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            count_participants_per_event();
            break;
        case 2:
            count_registrations_per_student();
            break;
        case 3:
            query_scores();
            break;
        case 0:
            return;
        default:
            printf("无效选项！\n");
        }
    } while (1);
}

// 管理员主菜单
void admin_menu() {
    int choice;
    do {
        printf("\n=== 管理员菜单 ===\n");
        printf("1. 运动项目管理\n");
        printf("2. 学生信息管理\n");
        printf("3. 报名管理\n");
        printf("4. 统计查询\n");
        printf("5. 录入成绩\n");
        printf("0. 退出系统\n");
        printf("请选择操作: ");
        scanf("%d", &choice); clear_input_buffer();

        switch (choice) {
        case 1:
            event_management_menu();
            break;
        case 2:
            student_management_menu();
            break;
        case 3:
            registration_management_menu();
            break;
        case 4:
            statistics_query_menu();
            break;
        case 5:
            enter_score();
            break;
        case 0:
            logout();
            break;
        default:
            printf("无效选项！\n");
        }
    } while (1);
}
